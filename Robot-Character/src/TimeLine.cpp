#include <UI/TimeLine.h>
#include <UI/AnimationControl.h>
#include <ChibiEngine/Render/Primitives/PrimitiveDrawer.h>
#include <ChibiEngine/Common/ParserUtils.h>
#include <ChibiEngine/Common/StringUtils.h>
#include "Generated/ChedSettingsParser.h"
#include "Generated/SaveParser.h"

using namespace game;
using namespace glm;
using namespace std;

game::TimeLine::TimeLine(game::AnimationControl *parent,ChedTimeLine* data,TimeLineType *settings)
    :parent(parent),
     labelSize(data->labelSize),
     lineHeight(2.0f*data->lineHeightPercent-1.0f),
     midLineHeight(2.0f*data->midLineHeightPercent-1.0f),
     currTimeLabel(Game::getLookAndFeel().font.font,"",data->labelSize),
     endTimeLabel(Game::getLookAndFeel().font.font,"",data->labelSize){

    sv.drawMode=GL_LINES;
    sv.uniformValues["u_color"]=rgb(118,118,118);
    originalDivLen = 2.0f * 1.0f/(40.0f - 0.0f);

    setPosition(vec3(parse(data->pos),10));
    setSize(vec2(data->hw,data->hh));

    if(settings){
        calcTimeMarks(settings->startX, settings->endX, settings->divStep, settings->markFreq);
    }else{
        calcTimeMarks(0.0,40.0,1,5);
    }
    setMaxTime(parent->calcMaxTime());
}

void TimeLine::saveSettings(TimeLineType* res){
    res->startX=startX;
    res->endX=endX;
    res->divStep=divStep;
    res->markFreq=markFreq;
}

void TimeLine::processDraggingAction(const glm::vec2 &delta) {
    float localDelta = -delta.x*getHW();
    float offset = 0.5f*localDelta*(endX - startX);
    if(startX+offset<0){
        calcTimeMarks(0, endX-startX, divStep, markFreq);
    }else{
        calcTimeMarks(startX+offset, endX+offset, divStep, markFreq);
    }
}


TimeLine::Label::Label(const std::string& txt, glm::vec3 coords, int size):
        sprite(Game::getLookAndFeel().font.font, txt, size),
        coords(coords){}

void TimeLine::Label::draw(const glm::mat4 &transform, const glm::vec3 &labelColor) {

}

void TimeLine::setCurrentTime(float time){
    {
        int newTime = static_cast<int>(round(time));
        int oldTime = static_cast<int>(round(currentTime));
        if(oldTime!=newTime)
            currTimeLabel.setText(toString(newTime));
    }
    currentTime = time;
}

void TimeLine::calcTimeMarks(float startX, float endX, int divStep, int markFreq) {

    labels.clear();
    TimeLine::startX = startX;
    TimeLine::endX = endX;
    TimeLine::divStep = divStep;
    TimeLine::markFreq = markFreq;

    recalcActiveLineTimeMarks();

    vector<GLfloat> vects;
    float divLen = 2.0f*divStep/(endX - startX);
    float currentPos = -1.0f+divLen*(1.0f-fract(startX/divStep));
    int currentMark = (static_cast<int>(startX/divStep)+1)*divStep;

    divCount = 0;
    while(currentPos<1.0){ // before scalling hw = 1.0

        // ------- UP
        vects.push_back(currentPos); //x up
        if(currentMark%markFreq==0){
            vects.push_back(lineHeight); //y up - tall
            labels.emplace_back(toString(currentMark), vec3(currentPos, lineHeight,0),labelSize);
        }else{
            vects.push_back(midLineHeight); //y up - middle
        }
        vects.push_back(0.0f); //z up

        // ------- Down
        vects.push_back(currentPos); //x down
        vects.push_back(-1.0f); //y down
        vects.push_back(0.0f); //z down

        currentPos+=divLen;
        currentMark+=divStep;
        divCount++;
    }
    sv.count=divCount*2;
    sv.fillBuffer(vects);

    vector<GLubyte> inds(divCount*2);
    for(GLubyte i=0;i<divCount*2;++i){
        inds[i]=i;
    }
    sv.fillIndex(inds);
}

void TimeLine::executeAction() {
    vec2 pnt = Game::getInputSystem()->getCurrentMousePosition();
    float localPoint = (pnt.x- getPosition().x)/getHW();
    if(localPoint<1.0 && localPoint>-1.0){
        setCurrentTime(round(transformInterval(-1.0f, 1.0f, startX, endX, localPoint)));
        parent->setCurrentTimeSkeleton(currentTime);
        parent->refreshAnimation();
    }
}

void TimeLine::setMaxTime(TransformKey::Time time){
    maxTime = time;
    endTimeLabel.setText(toString(time));
    if(parent->getAnimation())
        parent->getAnimation()->setDuration(toAnimationTime(time));
}

bool TimeLine::executeScroll(bool up){
    int newDivStep = divStep;
    int newMarkFreq = markFreq;
    float divLen = 2.0f*divStep/(endX - startX);

    if(!up){
        if(newMarkFreq==1){
            newMarkFreq = 5;
        }

        if (divLen*2.0f<originalDivLen){
            newDivStep*=2;
            newMarkFreq*=2;

        }

        if(startX-divStep<0){
            calcTimeMarks(0,endX+divStep+(divStep-startX),newDivStep,newMarkFreq);
        }else{
            calcTimeMarks(startX-divStep, endX+divStep,newDivStep,newMarkFreq);
        }
    }
    else{

        if(divLen/2.0f>originalDivLen){
            if(newDivStep<=1){
                calcTimeMarks(startX, endX, newDivStep,1);
                return true;
            }

            newDivStep/=2;
            newMarkFreq/=2;
        }

        calcTimeMarks(startX, endX-divStep, newDivStep,newMarkFreq);
    }

    return true;
}


void TimeLine::draw(){
    // Draw line marks
    sv.uniformValues["u_mvp_matrix"]=UniformValue(createMatrix(
            vec3(getPosition().x, getPosition().y,0), getSize()));
    Game::getShaderSystem()->get("simple").draw(sv);

    // Draw labels
    for(auto& it : labels){
        /*if(static_cast<int>(currentTime)== toInt(it.sprite.getText()) ||
                static_cast<int>(maxTime)== toInt(it.sprite.getText()))
            continue;*/
        vec3 _pos = it.coords*vec3(getSize(),1.0f)+vec3(getPosition().x, getPosition().y+it.sprite.getScreenHHeight(),0);
        it.sprite.draw(_pos,Color::White);
    }

    size_t dependentCount = parent->getActiveLine()?parent->getActiveLine()->getDependentCout():-1;
    // Draw current frame arrow
    if(currentTime>=startX && currentTime<=endX){
        float ctx = transformInterval(startX, endX, getPosition().x-getHW(), getPosition().x+ getHW(), currentTime);
        Game::getPrimitiveDrawer()->drawLine(
                vec3(ctx,lineHeight*getHH() + getPosition().y,0),
                vec3(ctx, getPosition().y-getHH()*((dependentCount+1)*2+1),0),
                Color::Aqua);

        currTimeLabel.draw(vec3(ctx,lineHeight*getHH()+ getPosition().y+currTimeLabel.getScreenHHeight(),0),Color::Aqua);
    }

    // Draw end frame
    if(maxTime>=startX && maxTime<=endX && static_cast<TransformKey::Time>(currentTime)!=maxTime){
        float ctx = transformInterval<float>(startX, endX, getPosition().x-getHW(), getPosition().x+ getHW(), maxTime);
        Game::getPrimitiveDrawer()->drawLine(
                vec3(ctx,lineHeight*getHH()+ getPosition().y,0),
                vec3(ctx, getPosition().y-getHH()*((dependentCount+1)*2+1),0),
                Color::Orange);

        endTimeLabel.draw(vec3(ctx,lineHeight*getHH()+ getPosition().y+currTimeLabel.getScreenHHeight(),0),Color::Orange);
    }

}