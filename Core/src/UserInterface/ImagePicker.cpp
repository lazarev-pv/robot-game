#include <ChibiEngine/UserInterface/ImagePicker.h>
#include <ChibiEngine/UserInterface/LabelsList.h>
#include <ChibiEngine/Resource/Resources.h>
#include <ChibiEngine/Render/ScreenSystem.h>
#include <ChibiEngine/XMLParser/UserInterfaceParser.h>
#include <ChibiEngine/UserInterface/Image.h>
#include <ChibiEngine/UserInterface/LookAndFeel.h>
#include <cmath>
#include <iostream>

using namespace game;
using namespace std;
using namespace glm;

const string ImagePicker::TYPE_NAME = "ImagePicker";

ImagePicker::ImagePicker(SlotType* it):
        laf(Game::getLookAndFeel().imagePicker) {

	UIElement::setPosition(vec3(it->x,it->y,it->z));
	setSize(vec2(it->hw>0?it->hw:laf.baseSize.x,it->hh>0?it->hh:laf.baseSize.y));
	setName(it->name);

    vec2 k(getHW()/laf.baseSize.x, getHH()/laf.baseSize.y);
    image = new Image(nullptr, getPosition()+vec3(0,0,0.1)+laf.imageOffset*vec3(k,1),laf.imageSize*k);
    add(image);
    values = new LabelsList(getPosition()+vec3(0,0,0.1)+laf.labelsOffset*vec3(k,1),laf.labelsSize*k,getName()+".list");
    add(values);

    Game::getUserInterface()->addEventListener(getName()+".list",
            [this](const std::string& name){
                image->setImage(Game::getResources()->findSprite(name));
            });
}

void ImagePicker::draw(){
    Game::getPrimitiveDrawer()->drawRectangleBorder(
            createMatrix(getPosition(),getSize()),
            laf.imageBorderColor);
}

const TexturedObject* ImagePicker::getSelected() const{
	return static_cast<const TexturedObject*>(image->getImage());
}

glm::vec2 ImagePicker::getDrawScale() const{
    const DrawableObject* obj = image->getImage();
    if(obj==nullptr){
        return vec2(1.0);
    }

    return Game::getCamera()->calcScale(obj->getHW()*image->getScale().x, obj->getHH()*image->getScale().y);
}

void ImagePicker::addImages(const std::vector<std::string>& names){
    values->add(names);
}

void ImagePicker::clearImages(){
    values->clear();
}