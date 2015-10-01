#define GLM_FORCE_RADIANS

#include <ChibiEngine/Render/Particles/Rain.h>
#include <ChibiEngine/Render/Shaders.h>
#include <glm/gtx/fast_square_root.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <ChibiEngine/Resource/Resources.h>
#include <ChibiEngine/Resource/Texture.h>

using namespace game;
using namespace glm;
using namespace std;

static const int ATR_PER_PARTICLE = 6;
static const int particesNum = 300;
const std::string RainTemplate::NAME="rain";

inline float randf(float min, float max){
	 return min + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(max-min)));
}

void RainTemplate::generateBuffer(){
    float data[particesNum*ATR_PER_PARTICLE];

    for(int i=0;i<particesNum;i+=2){
        int cur=ATR_PER_PARTICLE*i;

        vec3 begin(randf(-1,1), randf(0.9,1.0), randf(-1.0,1.0));
        vec2 vel = fastNormalize(vec2(randf(-0.1,0.0),randf(-0.8,-0.7)));

        float v=randf(5,7);
        float lifeTimeX=100.0;
        if(fabs(vel.x)>std::numeric_limits<float>::epsilon()){
            lifeTimeX=fabs((-1-begin.x)/(vel.x*v));
        }
        float lifeTimeY=100.0;
        if(fabs(vel.y)>std::numeric_limits<float>::epsilon()){
            lifeTimeX=fabs((-1-begin.y)/(vel.y*v));
        }
        float lifeTime = std::min(lifeTimeX,lifeTimeY);


        data[cur++]=begin.x;
        data[cur++]=begin.y;
        data[cur++]=begin.z;

        data[cur++]=vel.x*v;
        data[cur++]=vel.y*v;

        data[cur++]=lifeTime;

        float k = randf(0.03,0.06);
        data[cur++]=begin.x+vel.x*k;
        data[cur++]=begin.y+vel.y*k;
        data[cur++]=begin.z;

        data[cur++]=vel.x*v;
        data[cur++]=vel.y*v;

        data[cur++]=lifeTime;
    }

    shaderValues.fillBuffer(data, particesNum*ATR_PER_PARTICLE);
}

RainTemplate::RainTemplate()
        :EffectTemplate(NAME),
         texture(Game::getResources()->findTexture("rain")){
    shaderValues.drawMode = GL_LINES;
    /*shaderValues.samplerValues["u_texture"]=texture->getUID();*/
    shaderValues.count=particesNum;
}

void RainTemplate::draw(CameraCHandle camera, float dt_s, ParticleEffectCHandle effect) const{
	glm::mat4 result = camera->getMVPMatrix() * glm::translate(mat4(1.0f),effect->getPosition());
    Game::getShaderSystem()->get("rain").draw(shaderValues,
            unordered_map<string, UniformValue>({
                    {"u_mvp",UniformValue(result)},
                    {"u_size",UniformValue(effect->getSize())},
                    {"u_time",UniformValue(dt_s)},
                    {"u_color",UniformValue(Color(1.0f,1.0f,1.0f,0.6f))}}));
}

