#define GLM_FORCE_RADIANS

#include <ChibiEngine/Render/Particles/Fire.h>
#include <ChibiEngine/Render/Shaders.h>
#include <ChibiEngine/Resource/Resources.h>
#include <ChibiEngine/Resource/Texture.h>
#include <glm/gtx/fast_square_root.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <random>

using namespace game;
using namespace glm;
using namespace std;

const std::string FireTemplate::NAME="fire";

static const int ATR_PER_PARTICLE = 6;
static const int particesNum = 200;
static const float minH = 0.3;

inline float randf(float min, float max){
    return min + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(max-min)));
}

FireTemplate::FireTemplate()
    :EffectTemplate(NAME),
     texture(Game::getResources()->findTexture("fire")){
    shaderValues.count=particesNum;
    shaderValues.drawMode = GL_POINTS;
    shaderValues.samplerValues["u_texture"]=texture->getUID();
}


void FireTemplate::generateBuffer() {
    float data[particesNum*ATR_PER_PARTICLE];

    std::default_random_engine generator;
    std::normal_distribution<float> distribution(0.0,1.0);
    for(int i=0;i<particesNum;i++){
        int cur=ATR_PER_PARTICLE*i;
        vec3 begin(glm::clamp(distribution(generator)/2,-1.0f,1.0f), -1, 0);
        vec2 vel(0,randf(0.5,1.0));

        float maxH = (-2+minH)*fabs(begin.x)+1;
        float lifeTime = randf(minH/2.0+1,maxH+1)/vel.y;

        data[cur++]=begin.x;
        data[cur++]=begin.y;
        data[cur++]=begin.z;

        data[cur++]=vel.x;
        data[cur++]=vel.y;

        data[cur++]=lifeTime;
    }

    shaderValues.fillBuffer(data, particesNum*ATR_PER_PARTICLE);
}

void FireTemplate::draw(CameraCHandle camera, float dt_s, ParticleEffectCHandle effect) const{
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glm::mat4 result = camera->getMVPMatrix() * glm::translate(mat4(1.0f),effect->getPosition());
    Game::getShaderSystem()->get("fire").draw(shaderValues,
            unordered_map<string, UniformValue>({
                    {"u_mvp",UniformValue(result)},
                    {"u_size",UniformValue(effect->getSize())},
                    {"u_dir",UniformValue(effect->getDirection())},
                    {"u_time",UniformValue(dt_s)}/*,
                    {"u_color",UniformValue(vec4(1,1,1,1))}*/}));
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
}

