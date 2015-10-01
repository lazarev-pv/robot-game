#include <Layer.h>
#include <ChibiEngine/XMLParser/WorldParser.h>
#include <ChibiEngine/Render/ScreenSystem.h>
#include <LevelObjects/Ground.h>
#include <LevelObjects/Water.h>
#include <LevelObjects/Box.h>
#include <LevelObjects/Joint.h>
#include <LevelObjects/FXObject.h>
#include <LevelObjects/DecorativeLevelObject.h>
#include <LevelObjects/StaticDecorations.h>
#include <LevelObjects/Object3D.h>
#include <LightSource.h>
#include <RobotLevel.h>
#include <ChibiEngine/Render/Particles/ParticleSystem.h>
#include <ChibiEngine/Render/Particles/Rain.h>
#include <ChibiEngine/Render/Particles/Splash.h>
#include <ChibiEngine/Common/ContainerUtils.h>
#include <ChibiEngine/Clock/Clock.h>
#include <ChibiEngine/Log/LoggingSystem.h>
#include <ChibiEngine/Physics/Physics.h>


using namespace game;
using namespace glm;
using namespace std;


Layer::Layer(WorldLayers* layer,RobotLevel* level):
        level(level),
        z(layer->z),
        name(layer->name),
        frameId(Game::getScreen()->generateFrameNumber()),
        lightFrameId(Game::getScreen()->generateFrameNumber()){


    for(auto iter : layer->geometry->slot){
        LevelObject* lo;
        if(iter->chain!=nullptr){
            lo = new Ground(level->generateUid(),iter,this);
        }else if(iter->box.size()>0){
            lo = new Box(level->generateUid(),iter, this);
        }

        add(lo);
    }

    for(auto iter : layer->geometry->join){
        add(new Joint(level->generateUid(),iter,this));
    }

    for(auto iter:layer->art){
        add(new DecorativeLevelObject(level->generateUid(),iter,layer->z,nullptr,this));
    }
    /*if(layer->decorations!=nullptr) {
        add(new StaticDecorations(layer->decorations, layer->z, this));
    }*/

    if(layer->fxes!=nullptr){
        for(FXType* fx : layer->fxes->fx){
            add(new FXObject(level->generateUid(),fx,this));
        }
    }
    if(layer->light!=nullptr){
        for(LightType* light : layer->light->light){
            add(LightSource::create(level->generateUid(),light,this));
        }
    }
    if(layer->objs3d!=nullptr){
        for(Box3DType* iter : layer->objs3d->box3D){
            add(new Object3D(level->generateUid(),iter, this));
        }
    }

    resort();
    /*sort(drawList.begin(),drawList.end(),[](const LevelObject* o1, const LevelObject* o2){
        return o1->getZOrder()<o2->getZOrder();
    });*/
}

LevelObject* Layer::findObject(const std::string& name){
    return objectIndex[name];
}

b2Body* Layer::findPhysBody(const std::string& name){
    PhysLevelObject* box = dynamic_cast<PhysLevelObject*>(objectIndex[name]);
    if(box==nullptr) {
        Game::getLogger().error("Box %s not found", name.c_str());
        return nullptr;
    }
    return box->getBody();
}

Layer::~Layer(){
    for(auto iter:deleteList){
        iter->requestDelete(true);
    };
}

void Layer::draw(const Color& ambientLight){
    CameraCHandle camera = Game::getCamera();
    ScreenSystem* screen = Game::getScreen();
    if(lights.size()==0){

        for(auto obj : drawList){
            obj->draw(camera,ambientLight);
        }

    }else{
        screen->bindFrame(frameId);
        for(auto obj : drawList){
            obj->draw(camera,Color::White);
        }

        screen->bindFrame(lightFrameId);
        for(auto light : lights){
            light->draw(camera);
        }

        screen->mainFrame();
        ShaderValues sv(screen->getVertexBuffer(),screen->getIndexBuffer(),6);
        sv.samplerValues["u_light_tex"]= screen->getFrameTexture(lightFrameId).getUID();
        sv.samplerValues["u_layer_tex"]= screen->getFrameTexture(frameId).getUID();
        sv.uniformValues["u_ambientLight"]=UniformValue(vec3(
                ambientLight.toVector().r,
                ambientLight.toVector().g,
                ambientLight.toVector().b));
        Game::getShaderSystem()->get("light").draw(sv);

    }

}

void Layer::setDrawDebug(bool val){
    for(auto iter : drawList){
        iter->setDrawDebug(val);
    }
}

void Layer::setDrawPres(bool val){
    for(auto iter : drawList){
        iter->setEnabled(val);
    }
}

LevelObject* Layer::add2Index(LevelObject* obj){
    if(obj->getName().size()==0){
        return obj;
    }

    if(objectIndex[obj->getName()]!=nullptr){
        Game::getLogger().warn("RobotLevel Object with name %s already exists ", obj->getName().c_str());
    }

    objectIndex[obj->getName()]=obj;
    return obj;
}

void Layer::resort(){
    sort(drawList.begin(), drawList.end(), [](const LevelObject* o1, const LevelObject* o2){
        return o1->getZOrder()<o2->getZOrder();
    });
}

void Layer::deleteObject(LevelObject* lo){
    if(lo==nullptr)
        return;

    objectIndex.erase(name);
    game::remove(&drawList,lo);
    game::remove(&deleteList,lo);
    lo->requestDelete(false);
}

void Layer::add(LightSource* obj){
    level->add2Index(obj);
    lights.push_back(obj);
    add(static_cast<LevelObject*>(obj));
}

void Layer::add(LevelObject* obj){
    level->add2Index(obj);
    LevelObject* lo = add2Index(obj);
    drawList.push_back(lo);
    deleteList.push_back(lo);
}

void Layer::add(Interactive* obj){
    level->add2Index(obj);
    LevelObject* lo = add2Index(obj);
    drawList.push_back(lo);
}
