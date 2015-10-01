#include <ChibiEngine/UserInterface/LineContainer.h>
#include <ChibiEngine/UserInterface/UserInterface.h>
#include <ChibiEngine/Render/Primitives/PrimitiveDrawer.h>
#include <ChibiEngine/Log/Logger.h>
#include <ChibiEngine/XMLParser/UserInterfaceParser.h>
#include <ChibiEngine/Common/StringUtils.h>
#include <ChibiEngine/Game.h>
#include <ChibiEngine/Common/ParserUtils.h>

using namespace game;
using namespace std;
using namespace glm;

const string LineContainer::TYPE_NAME = "LineContainer";

LineContainer::LineContainer(SlotType* iter)
        :yIntent(iter->choiceValue.lineContainer->yIntent),
         xIntent(iter->choiceValue.lineContainer->xIntent){

    setPosition(vec3(iter->x,iter->y,iter->z));
    setSize(vec2(iter->hw,iter->hh));
    setName(iter->name);

    if(iter->choiceValue.lineContainer->caption.size()>0){
        setCaption(iter->choiceValue.lineContainer->caption);
    }

    if(iter->choiceValue.lineContainer->backColor==nullptr){
        backColor.setAlpha(0.0f);
    }else{
        backColor=parse(iter->choiceValue.lineContainer->backColor);
    }

    addElementsAndWeights(iter->choiceValue.lineContainer);
    pack();
}

void LineContainer::addElementsAndWeights(LineContainerSlot* slot){
    UserInterface* ui = Game::getUserInterface();

    for(ContainerLineType* el: slot->line){
        vector<float> weights = extractFloats(el->weight,";");
        float sum=0.0f;
        for(auto w : weights){
            sum+=w;
        }
        if(sum>=100.0f){
            ui->getLogger().
                    error("LineContainer::addElementsAndWeights - sum greater than 100: " + el->weight);
        }else{
            weights.push_back(100.0f-sum);
        }

        if(weights.size()!=el->slot.size()){
            ui->getLogger().
                    error("LineContainer::addElementsAndWeights - "
                            "slots size [%i] not equal weights size [%i], weights: %s. Name: %s",
                    el->slot.size(), weights.size(), el->weight.c_str(),
                    getName().c_str());
            return;
        }
        LineContainer::weights.push_back(weights);
        for(SlotType* slot : el->slot){
            add(ui->createElement(slot));
        }
    }
}

vector<float> LineContainer::calcHeights(){
    vector<float> heights;
    float commonHeight = 0;
    unsigned currentElement = 0;
    for(unsigned line = 0; line<weights.size();++line){
        float maxH = 0;
        for(unsigned col=0; col<weights[line].size();++col){
            maxH = std::max(maxH, getSlotElem(currentElement)->getHH());
            currentElement++;
        }
        maxH+=yIntent;
        commonHeight+=maxH;
        heights.push_back(maxH);
    }

    if(getCaption()){
        commonHeight+=Game::getLookAndFeel().caption.hh;
    }

    ElementCollection::setSize(vec2(getHW(),commonHeight));
    return heights;
}

void LineContainer::pack(){
    vector<float> heights = calcHeights();
    unsigned currentElement = 0;

    float currentTopBorder = getPosition().y+getHH();
    if(getCaption()){
        getCaption()->setPosition(vec3(getPosition().x,currentTopBorder-Game::getLookAndFeel().caption.hh, getSlotElem(0)->getPosition().z));
        currentTopBorder-=Game::getLookAndFeel().caption.hh*2;
    }

    for(unsigned line = 0; line<weights.size();++line){

        float currentLeftBorder = getPosition().x-getHW();
        for(unsigned col=0; col<weights[line].size();++col) {

            float elHW = getHW()*weights[line][col]*0.01f;
            float elHH = heights[line];
            getSlotElem(currentElement)->setPosition(vec3(
                    currentLeftBorder+elHW,
                    currentTopBorder-elHH,
                    getSlotElem(currentElement)->getPosition().z));
            getSlotElem(currentElement)->setSize(vec2(elHW-xIntent,elHH-yIntent));
            currentLeftBorder+=elHW*2;

            currentElement++;
        }
        currentTopBorder-=heights[line]*2;
    }
}

void LineContainer::draw(){
    if(backColor.toVector().a>0) {
        Game::getPrimitiveDrawer()->drawFilledRectangle(
                createMatrix(getPosition(), getSize()),
                backColor);
    }
    Game::getPrimitiveDrawer()->drawRectangleBorder(createMatrix(getPosition(), getSize()), Color::White);
}