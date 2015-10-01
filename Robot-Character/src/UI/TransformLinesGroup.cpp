#include <UI/TransformLinesGroup.h>
#include <UI/HeaderTransformLine.h>
#include <ChedLevel.h>
#include <ChibiEngine/Log/LoggingSystem.h>
#include <ChibiEngine/Common/StringUtils.h>
#include <UI/ConcreteTransformLines.h>
#include <UI/TransformLabel.h>
#include <ChedBone.h>

using namespace game;
using namespace glm;
using namespace std;

void TransformLinesGroup::processHighlight(const glm::vec2& pnt){
    header->processHighlight(pnt);
    for(auto& line : lines){
        line->processHighlight(pnt);
    }
}

void TransformLinesGroup::clear(){
    header->clear();
    for(auto& line : lines){
        line->clear();
    }
}

void TransformLinesGroup::calcTimeMarks(float startX, float endX, int divStep, int markFreq){
    header->calcTimeMarks(startX, endX, divStep, markFreq);
    for(auto& line : lines){
        line->calcTimeMarks(startX, endX, divStep,markFreq);
    }
}

void TransformLinesGroup::setVisible(bool visible){
    header->setVisible(visible);
    for(auto& line : lines){
        line->setVisible(visible);
    }
}

TransformKey::Time TransformLinesGroup::calcMaxTime() const {
    TransformKey::Time maxTime = 0;
    for(auto& line:lines){
        maxTime = glm::max(maxTime,line->calcMaxTime());
    }

    return maxTime;
}

void TransformLinesGroup::setSelectedTransform(TransformId transformId){
    if(lastSelectedTimeLine>0 && lastSelectedTimeLine<static_cast<int>(lines.size())){
        lines[lastSelectedTimeLine]->setSelected(false);
    }

    lastSelectedTimeLine = transformId;

    if(lastSelectedTimeLine>0 && lastSelectedTimeLine<static_cast<int>(lines.size())) {
        lines[lastSelectedTimeLine]->setSelected(true);
    }

    ChedLevel::instance()->setCurrentTransformation(lastSelectedTimeLine);
}

void TransformLinesGroup::setBoneAnimation(const Animation::Bone * anim){
    for(const MoveTransition& mt : anim->moves.transforms){
        lines[MoveTransform]->addMark(static_cast<int>(mt.time*100.0f));
    }
    for(const RotateTransition& rt : anim->rotations.transforms){
        lines[RotateTransform]->addMark(static_cast<int>(rt.time*100.0f));
    }
    for(const ScaleTransition& rt : anim->scales.transforms){
        lines[ScaleTransform]->addMark(static_cast<int>(rt.time*100.0f));
    }
}

TransformLinesGroup::TransformLinesGroup(ChedBone * bone, const glm::vec3& pos, const glm::vec2& size):
        bone(bone),
        header(new HeaderTransformLine(this,
                    bone->getName()+" ("+toString(bone->getId())+')',
                    pos,
                    size))
{
    vec3 cpos(pos.x,pos.y-size.y*2.0,0);
    lines.push_back(new MoveTransformLine(this,cpos,size));
    cpos.y -= size.y*2.0;
    lines.push_back(new RotateTransformLine(this,cpos,size));
    cpos.y -= size.y*2.0;
    lines.push_back(new ScaleTransformLine(this,cpos,size));
}

void TransformLinesGroup::saveTransform(const glm::vec2& delta, const glm::vec2& currPnt, TransformKey::Time currentTime){
    if(lastSelectedTimeLine>=0){
        lines[lastSelectedTimeLine]->add(delta,currPnt,currentTime);
    }
}
