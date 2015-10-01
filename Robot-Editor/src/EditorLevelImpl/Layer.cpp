#include <EditorLevelImpl/Layer.h>
#include <ChibiEngine/Common/ContainerUtils.h>
#include <ChibiEngine/Render/Shaders.h>
#include <ChibiEngine/Render/ScreenSystem.h>
#include <algorithm>
#include <glm/glm.hpp>

#include <EditorLevelImpl/GeometryLineChain.h>
#include <EditorLevelImpl/EditorPres2D.h>
#include <EditorLevelImpl/EditorGeometryObject.h>
#include <EditorLevelImpl/EditorGeomJoint.h>
#include <EditorLevelImpl/FXObject.h>
#include <EditorLevelImpl/PointObject.h>
#include <EditorLevelImpl/EnemyObject.h>
#include <EditorLevelImpl/LightObject.h>
#include <EditorLevelImpl/DirLightObject.h>
#include <EditorLevelImpl/EditorPres3D.h>
#include <EditorLevelImpl/EditorConstants.h>

using namespace game;
using namespace std;
using namespace glm;

struct OpObjComp{
    bool operator()(const OperableObject* o1, const OperableObject* o2){
        return o1->getZOrder()<o2->getZOrder();
    };
};

Layer::Layer(const std::string& name):
        selectable(true),
        drawable(true),
        lights(),
        drawList(),
        z(0.0),
        name(name),
        frameId(Game::getScreen()->generateFrameNumber()),
        lightFrameId(Game::getScreen()->generateFrameNumber()){
    Game::getUserInterface()->addEventListener(name+".layer", bind(&Layer::onRename, this, placeholders::_1));
    Game::getUserInterface()->addEventListener(name+".visible", bind(&Layer::onDraw, this, placeholders::_1));
}

Layer::Layer(WorldLayers *iter):
        selectable(true),
        drawable(true),
        lights(),
        drawList(),
        z(iter->z),
        name(iter->name),
        frameId(Game::getScreen()->generateFrameNumber()),
        lightFrameId(Game::getScreen()->generateFrameNumber()){

    for(auto dec : iter->art){
        drawList.push_back(add2Index(new EditorPres2D(dec, nullptr,this)));
    }


    if(iter->geometry!=nullptr){
        for(GeometrySlot* geom : iter->geometry->slot){
            if(geom->box.size()>0){
                drawList.push_back(add2Index(new EditorGeometryObject(geom,this)));
            }else if (geom->chain!=nullptr){
                drawList.push_back(add2Index(new GeometryLineChain(geom,this)));
            }
        }
        for(JointType* j : iter->geometry->join){
            drawList.push_back(add2Index(new EditorGeomJoint(j,this)));
        }
    }

    if(iter->fxes!=nullptr){
        for(auto fx : iter->fxes->fx){
            drawList.push_back(add2Index(new FXObject(fx,this)));
        }
    }

    if(iter->light!=nullptr){
        for(auto light : iter->light->light){
            if(light->choiceValue.spot!=nullptr){
                lights.push_back(add2Index(new SpotLightObject(light)));
            }else if(light->choiceValue.directional!=nullptr){
                lights.push_back(add2Index(new DirLightObject(light)));
            }
        }
    }

    if(iter->objs3d!=nullptr){
        for(auto box : iter->objs3d->box3D){
            drawList.push_back(add2Index(new EditorPres3D(box,this)));
        }
    }
    resort();

    Game::getUserInterface()->addEventListener(name+".layer", bind(&Layer::onRename, this, placeholders::_1));
    Game::getUserInterface()->addEventListener(name+".visible", bind(&Layer::onDraw, this, placeholders::_1));
}

OperableObject* Layer::add2Index(OperableObject* obj){
    string s = obj->getName();
    if(s.size()>0){
        if(indexedObjs[s]!=nullptr){
            Game::getLogger().warn("EditorLevel Object with name %s already exists ",s.c_str());
        }
        indexedObjs[s]=obj;
    }
    return obj;
}

void Layer::rename(const std::string& oldName, OperableObject* obj){
    if(oldName.size()>0){
        indexedObjs.erase(oldName);
    }
    add2Index(obj);
}

Layer::~Layer() {
    deleteAll(drawList);
    deleteAll(lights);
}

void Layer::add(OperableObject *obj) {
    drawList.push_back(obj);
    resort();
}

void Layer::add(SpotLightObject* light){
    lights.push_back(light);
}

void Layer::add(DirLightObject* light){
    lights.push_back(light);
}

void Layer::resort(){
    sort(drawList.begin(), drawList.end(), OpObjComp());
}

void Layer::deleteObject(OperableObject *selectedObject) {
    drawList.erase(remove(drawList.begin(), drawList.end(), selectedObject), drawList.end());
    lights.erase(remove(lights.begin(),lights.end(),selectedObject),lights.end());
}

OperableObject *Layer::find(const glm::vec3 &pntInObjPlane){
    for (auto iter = drawList.rbegin(); iter != drawList.rend(); ++iter) {
        OperableObject *obj = (*iter)->containsPoint(vec2(pntInObjPlane.x, pntInObjPlane.y));
        if (obj != nullptr)
            return obj;
    }
    for (auto iter = lights.rbegin(); iter != lights.rend(); ++iter) {
        OperableObject *obj = (*iter)->containsPoint(vec2(pntInObjPlane.x, pntInObjPlane.y));
        if (obj != nullptr)
            return obj;
    }

    return nullptr;
}

void Layer::save(WorldType* world){
    WorldLayers* layer = new WorldLayers();
    layer->z=z;
    layer->name=name;
    layer->geometry=new GeometryList();
    layer->fxes=new FXListType();
    layer->light = new LightListType();
    layer->objs3d = new Obj3DType();

    for(OperableObject* el : this->drawList){
        el->save2Layer(layer);
    }
    for(OperableObject* el : this->lights){
        el->save2Layer(layer);
    }

    world->layer.push_back(layer);
}

void Layer::draw() {
    if(!drawable)
        return;

    ScreenSystem* screen = Game::getScreen();
    if(lights.size()>0){

        screen->bindFrame(frameId);
        CameraCHandle cam = Game::getCamera();
        for(auto el : drawList){
            el->draw(cam);
        }

        screen->bindFrame(lightFrameId);
        for(auto light : lights){
            light->draw(cam);
        }

        Color am = EditorLevel::instance()->getAmbientLight();

        screen->mainFrame();

        ShaderValues sv(screen->getVertexBuffer(),screen->getIndexBuffer(),6);
        sv.samplerValues["u_light_tex"]= screen->getFrameTexture(lightFrameId).getUID();
        sv.samplerValues["u_layer_tex"]= screen->getFrameTexture(frameId).getUID();
        sv.uniformValues["u_ambientLight"]=UniformValue(
                vec3(am.toVector().r,am.toVector().g,am.toVector().b));
            Game::getShaderSystem()->get("light").draw(sv);

    }else{
        CameraCHandle cam = Game::getCamera();
        for(auto el : drawList){
            el->draw(cam);
        }
    }
}

void Layer::onRename(const std::string &name) {
    this->name=name;
}

void Layer::onDraw(const std::string &name) {
    drawable="true"==name;
}

OperableObject* Layer::findByName(const std::string& name){
    OperableObject* obj = indexedObjs[name];
    if(obj==nullptr){
        EditorLevel::instance()->getLogger().warn("Operable Object with name %s not found",name.c_str());
    }
    return obj;
}