#include <ChibiEngine/Render/Sprites/BatchTexture.h>
#include <ChibiEngine/Render/Camera.h>
#include <ChibiEngine/Render/Shaders.h>

#define GLM_FORCE_RADIANS
#include <glm/ext.hpp>


using namespace game;
using namespace glm;
using namespace std;

static const float EPSILON = 0.0001;

static void expand(float val, std::vector<GLfloat>& xInds){
	float xInteger, xMod;
	xMod = modf(val, &xInteger);
	int xCount = static_cast<int>(xInteger);

	for(int i = 0; i<=xCount;++i){
		xInds.push_back(i);
	}
	if(xMod>EPSILON){
		xInds.push_back(xInds.size()-1+xMod);
	}
}


static void pushBackVert(float x, float y, const mat4& mat, vector<GLfloat>& verts){
	vec4 pnt = mat*vec4(x,y,0,1);
	pnt/=pnt.w;
	verts.push_back(pnt.x);
	verts.push_back(pnt.y);
	verts.push_back(pnt.z);
}

BatchTexture::BatchTexture(TextureCHandle texture, std::vector<BatchElement>& elements) {

    sv.samplerValues["s_texture"]=texture->getUID();

	std::vector<GLfloat> verts;
	std::vector<GLubyte> indices;
	int currentLastIndex = 0;
	for(vector<BatchElement>::const_iterator it = elements.cbegin(); it!=elements.cend();++it){

		std::vector<float> xInds;
		std::vector<float> yInds;
		expand(it->repeats.x, xInds);
		expand(it->repeats.y, yInds);

		for(size_t i=0; i<(xInds.size()-1)*(yInds.size()-1);++i){
			indices.push_back(currentLastIndex+0+i*4);
			indices.push_back(currentLastIndex+1+i*4);
			indices.push_back(currentLastIndex+2+i*4);
			indices.push_back(currentLastIndex+1+i*4);
			indices.push_back(currentLastIndex+3+i*4);
			indices.push_back(currentLastIndex+2+i*4);
		}
		currentLastIndex+=4*(xInds.size()-1)*(yInds.size()-1);

		glm::mat4 mat = glm::translate(it->pos) * glm::rotate(
				              glm::scale(glm::mat4(1.0f), glm::vec3(it->scale.x,it->scale.y,1.0f)),
						      it->angle,glm::vec3(0.0f,0.0f,1.0f));

		float dx=it->t2.x-it->t1.x;
		float dy=it->t1.y-it->t2.y;
		float texHW = it->size.x;
		float texHH = it->size.y;
		float targetHW = texHW*it->repeats.x;
		float targetHH = texHH*it->repeats.y;
		for(size_t y=0;y<yInds.size()-1;++y){
			for(size_t x=0;x<xInds.size()-1;++x){

				pushBackVert(
						-targetHW + texHW*2*xInds[x],
						-targetHH + texHH*2*yInds[y],
						mat,verts);
				verts.push_back(it->t1.x);
				verts.push_back(it->t2.y);

				pushBackVert(
						-targetHW + texHW*2*xInds[x],
						-targetHH + texHH*2*yInds[1+y],
						mat,verts);
				verts.push_back(it->t1.x);
				verts.push_back(it->t2.y+dy*(yInds[1+y]-y));


				pushBackVert(
						-targetHW + texHW*2*xInds[1+x],
						-targetHH + texHH*2*yInds[y],
						mat,verts);
				verts.push_back(it->t1.x+dx*(xInds[1+x]-x));
				verts.push_back(it->t2.y);

				pushBackVert(
						-targetHW + texHW*2*xInds[1+x],
						-targetHH + texHH*2*yInds[1+y],
						mat,verts);
				verts.push_back(it->t1.x+dx*(xInds[1+x]-x));
				verts.push_back(it->t2.y+dy*(yInds[1+y]-y));
			}
		}
	}

	sv.fillBuffer(verts);
	sv.fillIndex(indices);
    sv.count = indices.size();

}

void BatchTexture::draw(CameraCHandle camera,const Color& ambient) const{
    Game::getShaderSystem()->get("sprite")
            .draw(sv,unordered_map<string, UniformValue>({
                    {"u_ambientLight",UniformValue(ambient)},
                    {"u_mvp_matrix",UniformValue(camera->getMVPMatrix())}}));
}
