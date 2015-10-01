#include <ChibiEngine/UserInterface/TableContainer.h>
#include <ChibiEngine/Render/Primitives/PrimitiveDrawer.h>
#include <ChibiEngine/XMLParser/UserInterfaceParser.h>
#include <ChibiEngine/Common/ParserUtils.h>

using namespace game;
using namespace std;
using namespace glm;

const string TableContainer::TYPE_NAME = "TableContainer";

TableContainer::TableContainer(SlotType* iter):
	intent(iter->choiceValue.tableContainer->intent){
    if(iter->choiceValue.tableContainer->backColor==nullptr){
        TableContainer::backColor.setAlpha(0.0f);
    }else{
        TableContainer::backColor=parse(iter->choiceValue.tableContainer->backColor);
    }
	float summWeight = 0;
	for(float w: iter->choiceValue.tableContainer->weight){
		summWeight+=w;
		weights.push_back(w);
	}
	weights.push_back(1.0f-summWeight);

	setPosition(vec3(iter->x,iter->y,iter->z));

	ElementCollection::setSize(vec2(
			iter->hw>0?iter->hw:getHW(),
			iter->hh>0?iter->hh:getHH()));
	setName(iter->name);
    drawLines = iter->choiceValue.tableContainer->drawLines;
    align = iter->choiceValue.tableContainer->align;

    if(iter->choiceValue.tableContainer->caption.size()>0){
        setCaption(iter->choiceValue.tableContainer->caption);
    }
    alignPos();
}

void TableContainer::alignPos(){
    if(align=="top"){
        vec3 p = getPosition();
        p.y=1.0f-getHH();
        setPosition(p);
    }
}

void TableContainer::setSize(const glm::vec2& size){
	ElementCollection::setSize(size);
    alignPos();
}

void TableContainer::draw(){
    if(backColor.toVector().a>0) {
		Game::getPrimitiveDrawer()->drawFilledRectangle(
				createMatrix(vec3(getPosition().x,getPosition().y,0), getSize()),
                backColor);
    }
    Game::getPrimitiveDrawer()->drawRectangleBorder(
			createMatrix(vec3(getPosition().x,getPosition().y,0), getSize()),
			Color::White);
}

void TableContainer::pack(){

	//Высчитываем высоты
	size_t colsNum = weights.size();
	size_t rowsNum = getSlotsSize()/(colsNum);
	if(getSlotsSize()%(colsNum)!=0){
		rowsNum++;
	}

	vector<float> heights(rowsNum);
	{
		float commonHeight = 0;
		{
			int sN = -1;
			for(size_t row = 0; row<heights.size();++row){
				float currentH = 0;
				for(size_t col = 0; col<colsNum; ++col){
					if(sN<(static_cast<int>(getSlotsSize())-1)){
						sN++;
					}else{
						break;
					}
					currentH = std::max(currentH, getSlotElem(sN)->getHH());
				}
				commonHeight+=currentH+intent;
				heights[row]=currentH+intent;
			}

            if(getCaption()){
                commonHeight+=Game::getLookAndFeel().caption.hh;
            }
			ElementCollection::setSize(vec2(getHW(),commonHeight));
		}
	}

	int slotNum = -1;
	float currentTopBorder = getPosition().y+getHH();
    if(getCaption()){
        getCaption()->setPosition(vec3(getPosition().x,currentTopBorder-Game::getLookAndFeel().caption.hh, getSlotElem(0)->getPosition().z));
        currentTopBorder-=Game::getLookAndFeel().caption.hh*2;
    }

	for(size_t row = 0; row<heights.size();++row){

		float currentLeftBorder = getPosition().x-getHW();
		for(size_t col = 0; col<colsNum; ++col){
			if(slotNum<(static_cast<int>(getSlotsSize())-1)){
				slotNum++;
			}else{
				break;
			}

			float elHW = getHW()*weights[col];
			float elHH = heights[row];
            getSlotElem(slotNum)->setPosition(vec3(currentLeftBorder+elHW, currentTopBorder-elHH,getSlotElem(slotNum)->getPosition().z));
            getSlotElem(slotNum)->setSize(vec2(elHW,elHH-intent));
			currentLeftBorder+=elHW*2;
		}
		currentTopBorder-=heights[row]*2;
	}

}

UIElement *TableContainer::containPoint(const glm::vec2 &pnt) {
	return UIElement::containPoint(pnt);
}
