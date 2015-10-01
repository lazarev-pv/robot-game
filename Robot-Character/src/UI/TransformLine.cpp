#include <UI/TransformLine.h>
#include <UI/TransformLinesGroup.h>
#include <ChibiEngine/Render/Primitives/PrimitiveDrawer.h>
#include <ChibiEngine/Common/MathUtils.h>
#include <ChedLevel.h>
#include <Input/ChedInputListener.h>
#include <UI/AnimationControl.h>


#define GLM_FORCE_RADIANS
#include <glm/ext.hpp>
#include <ChibiEngine/Resource/Resources.h>
#include <UI/TimeLine.h>
#include <UI/HeaderTransformLine.h>
#include <UI/TransformLabel.h>

using namespace game;
using namespace std;
using namespace glm;

static const float KEY_HW = 0.006;
static const float KEY_HH_PERCENT = 0.8f;

inline void updateEndLable(TransformKey::Time time){
    TimeLine* timeLine = ChedLevel::instance()->getAnimationControl()->getTimeLine();
    if(timeLine->getMaxTime()<time){
        timeLine->setMaxTime(time);
    }
}

TransformLine::TransformLine(TransformLinesGroup* parent,
        const std::string& name,
        const std::string& icon,
        const Color& color,
        const glm::vec3& pos,
        const glm::vec2& size,
        TransformId transformId):
        keyColor(color),
        hookedKey(-1),
        parent(parent),
        transformId(transformId),
        label(new TransformLabel(
                name,
                icon,
                rgb(86,86,86),
                transformId,
                vec3(pos.x-size.x-(1.0f-0.2f-size.x), pos.y,pos.z),
                vec2(1.0f-0.2f-size.x, size.y)))
{

    setPosition(pos);
    setSize(size);

    setVisible(false);
    bgColor = rgb(86,86,86);
    sv.drawMode=GL_LINES;
    sv.uniformValues["u_color"]=UniformValue(rgb(40,40,40));
}

void TransformLine::processHighlight(const glm::vec2& pnt){
    vec2 interval = ChedLevel::instance()->getAnimationControl()->
            getTimeLine()->getInterval();

    for(auto& key : keys) {
        if (key.first < interval.x) {
            continue;
        } else if (key.first > interval.y) {
            break;
        }

        float posX = transformInterval<float>(interval.x, interval.y, -1.0f, 1.0f, key.first) * getHW() + getPosition().x;
        key.second.setHighlighted(game::contains(vec2(posX,getPosition().y), KEY_HW, KEY_HH_PERCENT*getHH(), pnt));
    }
}

TransformKey::Time TransformLine::getTimeByMouse(const glm::vec2& pnt){
    vec2 interval = ChedLevel::instance()->getAnimationControl()->
            getTimeLine()->getInterval();

    float localX = (pnt.x-getPosition().x)/getSize().x;
    TransformKey::Time currTime = static_cast<int>(round(
            transformInterval<float>(-1.0f, 1.0f, interval.x, interval.y,localX)));
    if(currTime<interval.x || currTime>interval.y){
        return -1;
    }else{
        return currTime;
    }
}

TransformKey::Time TransformLine::findKey(const glm::vec2& pnt){
    vec2 interval = ChedLevel::instance()->getAnimationControl()->
            getTimeLine()->getInterval();

    for(auto& key : keys) {
        if (key.first < interval.x) {
            continue;
        } else if (key.first > interval.y) {
            break;
        }

        float posX = transformInterval<float>(interval.x, interval.y, -1.0f, 1.0f, key.first) * getSize().x + getPosition().x;
        if(game::contains(vec2(posX, getPosition().y), KEY_HW, KEY_HH_PERCENT*getSize().y, pnt)){
            return key.first;
        }
    }

    return -1;
}

template<typename K, typename V>
void logSet(const string& name, map<K,V>& s){
    cout<<name;
    for(auto& i : s){
        cout<<" "<<i.first;
    }
    cout<<endl;
}

void TransformLine::onKeyPressed(KeyCode keyCode, bool special){
    if(keyCode==RawEventListener::DELETE_KEY){
        deleteKey(selectedKeyTime);
    }else if(keyCode==RawEventListener::CTRL_C){
        copyedKeyTime = selectedKeyTime;
    }else if(keyCode==RawEventListener::CTRL_V){
        copyKey(copyedKeyTime);
    }
}

void TransformLine::deleteKey(TransformKey::Time key){
    if(key<0)
        return;

    if(copyedKeyTime==key){
        copyedKeyTime=-1;
    }

    if(selectedKeyTime==key){
        selectedKeyTime=-1;
    }

    onKeyDelete(key);
    keys.erase(key);
    parent->getHeader()->recalcKeys();
    ChedLevel::instance()->getAnimationControl()->calcMaxTime();
}

void TransformLine::select(TransformKey::Time time){
    if(selectedKeyTime>0){
        keys.find(selectedKeyTime)->second.setSelected(false);
    }
    if(time>0){
        keys.find(time)->second.setSelected(true);
    }
    selectedKeyTime=time;
}


void TransformLine::executePushOnAnimation(){
    parent->setSelectedTransform(transformId);
    try2HookKey(Game::getInputSystem()->getCurrentMousePosition());
}

void TransformLine::executePushOffAnimation(){
    releaseKey(Game::getInputSystem()->getCurrentMousePosition());
}

void TransformLine::releaseKey(const glm::vec2& mousePnt){
    if(!keyHooked) {
        return;
    }

    TransformKey::Time time = getTimeByMouse(mousePnt);
    if(time>0){
        keys.erase(time);
    }

    onMoveKey(hookedKey.getTime(), time);
    hookedKey.setTime(time);
    keys.insert({time,hookedKey});
    updateEndLable(time);
    parent->getHeader()->recalcKeys();
    keyHooked = false;
    select(time);
}

void TransformLine::try2HookKey(const glm::vec2& pnt){
    TransformKey::Time time = findKey(pnt);
    if(time<0){
        return;
    }
    hookedKey = keys.find(time)->second;
    keys.erase(hookedKey.getTime());
    parent->getHeader()->recalcKeys();
    keyHooked = true;
}


void TransformLine::calcTimeMarks(float startX, float endX, int divStep, int markFreq){
    vector<GLfloat> vects;
    float divLen = 2.0f*divStep/(endX - startX);
    float currentPos = -1.0f+divLen*(1.0f-fract(startX/divStep));
    int _divCount = 0;
    int currentMark = (static_cast<int>(startX/divStep)+1)*divStep;

    while(currentPos<1.0){

        if(currentMark%markFreq==0){
            // ------- UP
            vects.push_back(currentPos);
            vects.push_back(1.0);
            vects.push_back(0.0f);
            // ------- Down
            vects.push_back(currentPos);
            vects.push_back(-1.0f);
            vects.push_back(0.0f);
            _divCount++;
        }

        currentPos+=divLen;
        currentMark+=divStep;
    }
    sv.count=_divCount*2;
    sv.fillBuffer(vects);

    vector<GLubyte> inds(_divCount*2);
    for(GLubyte i=0;i<_divCount*2;++i){
        inds[i]=i;
    }
    sv.fillIndex(inds);
}

void TransformLine::setSelected(bool selected){
    TransformLine::selected = selected;
    label->setSelected(selected);
}

void TransformLine::add(const glm::vec2& delta, const glm::vec2& currPnt, TransformKey::Time currentTime){
    auto iter = keys.find(currentTime);
    if(iter == keys.end()){
        addMark(currentTime);
        parent->getHeader()->recalcKeys();
    }
    updateEndLable(currentTime);
    onKeyModify(delta, currPnt, currentTime);
}

void TransformLine::draw(){
    vec2 interval = ChedLevel::instance()->getAnimationControl()->
            getTimeLine()->getInterval();

    {   //Background
        Game::getPrimitiveDrawer()->drawFilledRectangle(
                createMatrix(getPosition(), getSize()),
                selected?rgba(104, 119, 119,255):bgColor);
        Game::getPrimitiveDrawer()->drawRectangleBorder(
                createMatrix(getPosition(), getSize()),
                Color::Black);
    }{  //Marks
        sv.uniformValues["u_mvp_matrix"]=UniformValue(createMatrix(getPosition(), getSize()));
        Game::getShaderSystem()->get("simple").draw(sv);
    }
    { //Keys

        if (drawCurves && keys.size()>0) {
            for (auto key =  ++keys.begin(); key != keys.end(); ++key) {
                if (key->first < interval.x) {
                    continue;
                } else if (key->first > interval.y) {
                    break;
                }

                auto pr = prev(key);
                pr->second.getCurve().draw(
                        glm::max(pr->second.getPosition(getPosition(), getSize()), vec2(getPosition().x-getSize().x,getPosition().y)),
                        key->second.getPosition(getPosition(), getSize()), pr->second.isHighlighted());
            }
        }

        for(auto key = keys.begin(); key!=keys.end(); ++key){
            if(key->first<interval.x){
                continue;
            }else if(key->first>interval.y){
                break;
            }

            key->second.draw(getPosition(), getSize(),keyColor);
        }

    }{// mouse cursor
        if(keyHooked){
            TransformKey::Time time = getTimeByMouse(Game::getInputSystem()->getCurrentMousePosition());
            if(time>=0){
                TransformKey::drawImpl(getPosition(), getSize(), keyColor, true, time);
            }
        }
    }
}

void TransformLine::clear(){
    keys.clear();
    keyHooked=false;
}

TransformKey::Time TransformLine::calcMaxTime() const {
    TransformKey::Time time=0;
    for(auto& key:keys){
        time = glm::max(key.first,time);
    }
    return time;
}

void TransformLine::setVisible(bool flag) {
    UIElement::setVisible(flag);
    label->setVisible(flag);
}

void TransformLine::executeAction() {
    UIElement::executeAction();

    ChedLevel::instance()->getAnimationControl()->
            getTimeLine()->executeAction();
}

void TransformLine::copyKey(TransformKey::Time key) {
    if(key==-1){
        return;
    }

    TransformKey::Time currentTime = static_cast<TransformKey::Time>(round(
            ChedLevel::instance()->getAnimationControl()->getTimeLine()->getCurrentTime()));

    addMark(currentTime);
    onCopyKey(key,currentTime);
}
