#include <LevelObjects/StaticDecorations.h>

#include <map>
#include <ChibiEngine/XMLParser/WorldParser.h>
#include <ChibiEngine/Render/Sprites/BatchTexture.h>
#include <ChibiEngine/Common/ContainerUtils.h>
#include <ChibiEngine/Resource/Resources.h>
#include <ChibiEngine/Common/ParserUtils.h>

using namespace game;
using namespace std;
using namespace glm;

StaticDecorations::~StaticDecorations(){
	deleteAll(textures);
}

StaticDecorations::StaticDecorations(int uid, const std::vector<ArtType* >& art, float z, Layer* layer)
    :LevelObject(uid,layer,-1){
	map<const Texture*, vector<BatchElement>*> ref;
	vector<const Texture*> textureOrder;
	// Подготавливаем данные для списка батчей
	for(auto iter = art.begin(); iter!=art.end(); ++iter){
	//for(std::vector<ArtType* >::iterator iter = data->art.begin(); iter!=data->art.end(); ++iter){
		SpriteInfo::CHandle pres = Game::getResources()->findSpriteInfo((*iter)->image->name);
		const Texture* tex = pres->texture;

		vector<BatchElement>* batchElements = ref[tex];
		if(batchElements==nullptr){
			batchElements = new vector<BatchElement>();
			ref[tex]=batchElements;
			textureOrder.push_back(tex);
		}
		{
			batchElements->push_back(BatchElement(
                    parse((*iter)->pos),
                    parse((*iter)->scale),
					vec2(pres->hw,pres->hh),
					(*iter)->rotation,
                  pres->frame->topLeftPnt,
                  pres->frame->bottomRightPnt,
					(*iter)->repeat!=nullptr?
							vec2((*iter)->repeat->x, (*iter)->repeat->y):
							vec2(1.00,1.00),
                    (*iter)->zOrder
				));
		}
	}

	// создаем текстуры-пачки
	for(auto iter : textureOrder){

        vector<BatchElement>* els = ref[iter];

        sort(els->begin(),els->end(), [](const BatchElement& e1, const BatchElement& e2){
            return e1.zOrder<e2.zOrder;
        });

		textures.push_back(new BatchTexture(iter,*els));
		delete ref[iter];
	}
}

void StaticDecorations::draw(CameraCHandle camera,const Color& ambientLight){
	if(isEnabled()) {
        for (auto iter: textures) {
            iter->draw(camera, ambientLight);
        }
    }
}


glm::vec3 StaticDecorations::getPosition(){
	return vec3(0,0,0);
}