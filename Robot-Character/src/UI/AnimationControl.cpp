#define GLM_FORCE_RADIANS
#include <glm/ext.hpp>

#include <UI/AnimationControl.h>
#include <UI/HeaderTransformLine.h>
#include "../Generated/ChedSettingsParser.h"
#include "../Generated/SaveParser.h"
#include <ChibiEngine/Render/ScreenSystem.h>
#include <ChibiEngine/Common/ParserUtils.h>
#include <ChibiEngine/Log/LoggingSystem.h>
#include <sstream>
#include <ChedLevel.h>
#include <UI/TimeLine.h>
#include <UI/TransformLabel.h>
#include <ChibiEngine/UserInterface/Button.h>
#include <ChedBone.h>

using namespace std;
using namespace game;
using namespace glm;

static const float MILLISEC_OFFSET = 10.0f;
static constexpr float CAPT_HEIGHT = 0.02f;


inline void setPlayImg(Button* playButton, bool play) {
    ImageButtonDraw* drawer = static_cast<ImageButtonDraw*>(playButton->getDrawer());
    if(play){
        drawer->setNormal("play.normal");
        drawer->setHovered("play.hovered");
        drawer->setPushed("play.pushed");
        ChedLevel::instance()->getInstrumentHelper()->setVisible(true);
    }else{
        drawer->setNormal("stop.normal");
        drawer->setHovered("stop.hovered");
        drawer->setPushed("stop.pushed");
        ChedLevel::instance()->getInstrumentHelper()->setVisible(false);
    }
}

AnimationControl::AnimationControl(ChedTimeLine* data,
                                   TimeLineType* settings,
                                   ClockHandle clock,
                                   std::vector<unique_ptr<ChedBone>>& bones,
                                   Skeleton* skeleton):
		 bones(bones),
        timeline(new TimeLine(this,data,settings)),
        logger(Game::getLoggingSystem()->createLogger("AnimationControl")),
        skeleton(skeleton){

    skeleton->setIsPlaying(false);
    skeleton->addEventListeners([this](const AnimationEvent& event){
        if(event.eventName==Skeleton::ANIMATION_END && this->skeleton->isPlaying() && !animationTimeLine->isCycled()){
            setPlayImg(playButton,true);
            this->skeleton->setIsPlaying(false);
            this->skeleton->setCurrentTime(0.0);
        }
    });

    skeleton->addTimeEvents([this](const AnimationTimeEvent& event){
       timeline->setCurrentTime(toTransformKeyTime(event.currTime));
    });

    setName("AnimationControl");

    { // Calculating position related on timeline
        float hw = timeline->getHW();
        float hh = timeline->getHH();
        float x = timeline->getPosition().x;
        float y = timeline->getPosition().y;
        float bhw = 1.0f-0.2f-hw;
        float bx = x-hw-bhw;
        ElementCollection::setPosition(vec3 ((x+hw+bx-bhw)*0.5f, y,-1.0));
        ElementCollection::setSize(vec2(0.5f*((x+hw)-(bx-bhw)),hh));
    }

    // Создаем хедеры
	{
		//vec3 cpos(timeline->getPosition().x, timeline->getPosition().y-getHH()*2.0,0);
		for(auto& bone : bones){
            addBone(bone.get());
            /*auto ptr = make_unique<TransformLinesGroup>(bone,cpos, timeline->getSize());
            add(ptr->getHeader());
            add(ptr->getHeader()->getLabel());
            for(auto& line : ptr->getLines()){
                add(line);
                add(line->getLabel());
            }
            linesById.insert(make_pair(bone->getId(),ptr.get()));
			lines.insert(make_pair(bone->getName(),std::move(ptr)));*/
		}
	}{ // Помещаем кнопочки
        // Play
        float sizex = timeline->getSize().x;
        float sizey = timeline->getSize().y;
        float labelSizex = 1.0f-0.2f-sizex;

        float posx = timeline->getPosition().x-sizex-(1.0f-0.2f-sizex) - labelSizex;
        float posy = timeline->getPosition().y;
        float posz = 0;

        vec2 butSize(sizey*0.95f/Game::getScreen()->getAspectRatio(),sizey*0.95f);
        backButton= new Button (
                "",
                vec3(0,0,0),
                butSize,
                "back",
                make_unique<ImageButtonDraw>("back.normal","back.hovered","back.pushed",butSize));
        playButton= new Button (
                "",
                vec3(0,0,0),
                butSize,
                "play",
                make_unique<ImageButtonDraw>("play.normal","play.hovered","play.pushed",butSize));
        forwardButton= new Button (
                "",
                vec3(0,0,0),
                butSize,
                "forward",
                make_unique<ImageButtonDraw>("forward.normal","forward.hovered","forward.pushed",butSize));
        cycledButton = new Button (
                "",
                vec3(0,0,0),
                butSize,
                "repeat",
                make_unique<ImageButtonDraw>("repeat.normal","repeat.hovered","repeat.pushed",butSize));

        constexpr float interval = 1.2;
        posx+=backButton->getSize().x*interval;
        backButton->setPosition(vec3(posx,posy,posz));
        posx+=backButton->getSize().x*interval;
        add(backButton);

        posx+=playButton->getSize().x;
        playButton->setPosition(vec3(posx,posy,posz));
        posx+=playButton->getSize().x*interval;
        add(playButton);

        posx+=forwardButton->getSize().x;
        forwardButton->setPosition(vec3(posx,posy,posz));
        posx+=forwardButton->getSize().x*interval;
        add(forwardButton);

        posx+= cycledButton->getSize().x;
        cycledButton->setPosition(vec3(posx,posy,posz));
        add(cycledButton);

        Game::getUserInterface()->addEventListener("play",bind(&AnimationControl::onPlay, this, placeholders::_1));
        Game::getUserInterface()->addEventListener("repeat",[this](const std::string& name){
            if(animationTimeLine){
                animationTimeLine->setCycled(!animationTimeLine->isCycled());
                static_cast<ImageButtonDraw*>(cycledButton->getDrawer())->setNormal(
                        animationTimeLine->isCycled()?"repeat.pushed":"repeat.normal");
            }
        });
        Game::getUserInterface()->addEventListener("back",[this](const std::string& name){
            if(animationTimeLine){
                animationTimeLine->setCurrentTime(0);
            }
        });
        Game::getUserInterface()->addEventListener("forward",[this](const std::string& name){
            if(animationTimeLine){
                animationTimeLine->setCurrentTime(toAnimationTime(timeline->getMaxTime()));
            }
        });
    }

    setCaption("Timeline");
    getCaption()->setPosition(vec3(getPosition().x, getPosition().y+getHH()+getCaption()->getHH(),0));
    add(timeline);
    setVisible(true);
}

void AnimationControl::deleteBone(ChedBone * bone){
    auto it = linesById.find(bone->getId());
    if(it==linesById.end())
        return;

    UserInterface* ui = Game::getUserInterface();
    setActiveLine(nullptr);
    TransformLinesGroup* group = it->second;
    for(auto& line : group->getLines()){
        ui->remove(line->getLabel());
        ui->remove(line);

        remove(line->getLabel());
        remove(line);
    }
    ui->remove(group->getHeader()->getLabel());
    ui->remove(group->getHeader());
    remove(group->getHeader()->getLabel());
    remove(group->getHeader());

    linesById.erase(bone->getId());
    lines.erase(bone->getName());
}

void AnimationControl::addBone(ChedBone * bone){
    vec3 cpos(timeline->getPosition().x, timeline->getPosition().y-getHH()*2.0,0);

    auto ptr = new TransformLinesGroup(bone,cpos, timeline->getSize());

    add(ptr->getHeader());
    add(ptr->getHeader()->getLabel());
    for(auto& line : ptr->getLines()){
        add(line);
        add(line->getLabel());
    }

    if(isManagedByUI()){
        UserInterface* ui = Game::getUserInterface();
        ptr->getHeader()->add2UI(ui);
        ptr->getHeader()->getLabel()->add2UI(ui);
        for(auto& line : ptr->getLines()){
            line->add2UI(ui);
            line->getLabel()->add2UI(ui);
        }
    }

    linesById.insert(make_pair(bone->getId(),ptr));
    lines.insert(make_pair(bone->getName(),unique_ptr<TransformLinesGroup>(ptr)));
}

TimeLineType* AnimationControl::saveSettings(){
    TimeLineType* res = new TimeLineType();
    timeline->saveSettings(res);
    return res;
}

void AnimationControl::setActiveLine(TransformLinesGroup* active){
    TransformId currentTransform = NoTransform;

    if(activeLine){
        activeLine->setVisible(false);
        currentTransform = activeLine->getSelectedTransform();
    }

    activeLine=active;
    if(activeLine){
        activeLine->setVisible(true);
        activeLine->setSelectedTransform(currentTransform);
        timeline->recalcActiveLineTimeMarks();
    }

 //   setVisible(activeLine!=nullptr);
}

void AnimationControl::selectBone(const std::string& boneName){
	auto iter = lines.find(boneName);
    setActiveLine(iter!=lines.end()?iter->second.get():nullptr);
}

void AnimationControl::processHighlight(const glm::vec2& pnt){
	if(activeLine)
		activeLine->processHighlight(pnt);
}

void AnimationControl::setAnimation(const std::string& name){
    for(auto& header : lines){
        header.second->clear();
    }

    if(animationTimeLine){
        skeleton->removeAnimation(animationTimeLine->getAnimation()->second.getName());
    }

    animationTimeLine = skeleton->addAnimation(name, false, false);

    if(!animationTimeLine){
        timeline->setMaxTime(0);
      //  setVisible(false);
        return;
    }

    for(auto& boneAnim : getAnimation()->getBoneAnimations()){
       auto headerIt = linesById.find(boneAnim->boneId);
        if(headerIt==linesById.end()){
            logger.error("HeaderLine with id [%d] not found!",boneAnim->boneId);
            continue;
        }
        headerIt->second->setBoneAnimation(boneAnim.get());
    }
    timeline->setMaxTime(calcMaxTime());
}

void AnimationControl::draw(){
    Game::getPrimitiveDrawer()->drawFilledRectangle(createMatrix(getPosition(), getSize()), rgb(86,86,86));
    Game::getPrimitiveDrawer()->drawRectangleBorder(createMatrix(getPosition(), getSize()), Color::Black);
}

void AnimationControl::setPlaybackMultiplier(float multiplier){
	skeleton->setPlaybackMultiplier(multiplier);
}

void AnimationControl::saveTransform(const glm::vec2& delta, const glm::vec2& currPnt){
    if(activeLine)
        activeLine->saveTransform(delta, currPnt, static_cast<int>(timeline->getCurrentTime()));
}

void AnimationControl::selectTransform(TransformId transformId){
    if(activeLine)
        activeLine->setSelectedTransform(transformId);
    ChedLevel::instance()->setCurrentTransformation(transformId);
}

TransformKey::Time AnimationControl::calcMaxTime(){
    TransformKey::Time maxTime = 0;
    for(auto& header : lines){
        maxTime = glm::max(header.second->calcMaxTime(),maxTime);
    }
    return maxTime;
}

void AnimationControl::refreshAnimation() {
    skeleton->refreshAnimation();
    for(auto& bone : bones){
        bone->skeletonChanged(ChedLevel::instance());
    }
}

void AnimationControl::setSlotsVisible(bool flag) {
    UIElement::setVisible(flag);
    timeline->setVisible(flag);
    playButton->setVisible(flag);
    cycledButton->setVisible(flag);
    forwardButton->setVisible(flag);
    backButton->setVisible(flag);
    if(activeLine) activeLine->setVisible(flag);
}

void AnimationControl::setPlaying(bool playing){
    ChedLevel::instance()->getInstrumentHelper()->setVisible(!playing);
    skeleton->setIsPlaying(playing);
    setPlayImg(playButton,!playing);
    if(!playing) {
        animationTimeLine->setCurrentTime(toAnimationTime(trunc(timeline->getCurrentTime())));
        skeleton->refreshAnimation();
        ChedLevel::instance()->refreshAllObjects();
    }
}

void AnimationControl::setCurrentTimeSkeleton(TransformKey::Time time){
    skeleton->setCurrentTime(toAnimationTime(time));
}

void AnimationControl::onPlay(const std::string &name) {
    setPlaying(!skeleton->isPlaying());
}
