#include <ChibiEngine/UserInterface/Image.h>
#include <ChibiEngine/XMLParser/UserInterfaceParser.h>
#include <ChibiEngine/Render/ScreenSystem.h>
#include <ChibiEngine/Resource/Resources.h>

using namespace game;
using namespace std;
using namespace glm;

const string Image::TYPE_NAME = "Image";

Image::Image(SlotType* iter):UIElement(){
	image = Game::getResources()->findSprite(iter->choiceValue.image->presentation);
	setPosition(vec3(iter->x,iter->y,iter->z));
	setName(iter->name);
	setSize(vec2(iter->hw>0?iter->hw:image->getHW()*Game::getScreen()->getWidthDensity(),
            iter->hh>0?iter->hh:image->getHH()*Game::getScreen()->getHeightDensity()));
    calcScale();
}

Image::Image(const DrawableObject* image, const glm::vec3& pos, const glm::vec2& size)
    :UIElement(),image(image){

    setPosition(pos);
    if(image!=nullptr){
        setSize(vec2(size.x<0?image->getHW():size.x,size.y<0?image->getHH():size.y));
    }else{
        setSize(size);
    }

    calcScale();
}

void Image::draw(){
    if(image!=nullptr) {
        image->draw(createMatrix(getPosition(), scale));
    }
}

void Image::setImage(const DrawableObject* image){
    this->image = image;
    calcScale();
}

void Image::calcScale(){
    if(image!=nullptr) {

        scale.x = getHW() / image->getHW();
        scale.y = getHH() / image->getHH();

        scale.x = glm::min(scale.x,scale.y);
        scale.y=scale.x;
    }
}


void Image::setValue(const std::string& v){
    image = Game::getResources()->findSprite(v);
}

std::string Image::getValue() const{
    return image->getName();
}

