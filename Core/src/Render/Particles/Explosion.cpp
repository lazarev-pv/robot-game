#define GLM_FORCE_RADIANS

#include <ChibiEngine/Render/Particles/Explosion.h>
#include <ChibiEngine/Render/Shaders.h>
#include <ChibiEngine/Resource/Resources.h>
#include <ChibiEngine/Resource/Texture.h>
#include <glm/gtx/fast_square_root.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <random>
#include <ChibiEngine/Render/Particles/ParticleEffect.h>

using namespace game;
using namespace glm;
using namespace std;

const std::string ExplosionTemplate::NAME="explosion";

static const int ATR_PER_PARTICLE = 7;
static const int particesNum = 200;
static const float minH = 0.3;

inline float randf(float min, float max){
    return min + static_cast <float> (rand()) / (RAND_MAX/(max-min));
}

ExplosionTemplate::ExplosionTemplate()
    :EffectTemplate(NAME),
     texture(Game::getResources()->findTexture("explosion")){
    shaderValues.count=particesNum;
    shaderValues.drawMode = GL_POINTS;
    shaderValues.samplerValues["u_texture"]=texture->getUID();
}

void ExplosionTemplate::generateBuffer() {
    GLfloat data[particesNum*ATR_PER_PARTICLE];

    for(int i=0;i<particesNum;i++){
        int cur=ATR_PER_PARTICLE*i;

        data[cur++]=randf(0.5f,1.0f);

        data[cur++]=randf(-0.125f,0.125f);
        data[cur++]=randf(-0.125f,0.125f);
        data[cur++]=0.0f;//randf(-1.0f,1.0f);

        data[cur++]=randf(-1.0f,1.0f);
        data[cur++]=randf(-1.0f,1.0f);


        data[cur++]=0.0f;//randf(-0.125f,0.125f);
    }
    shaderValues.fillBuffer(data, particesNum*ATR_PER_PARTICLE);
}

void ExplosionTemplate::draw(CameraCHandle camera, float dt_s, ParticleEffectCHandle effect) const{
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glm::mat4 result = camera->getMVPMatrix() * glm::translate(mat4(1.0f),effect->getPosition());
    Game::getShaderSystem()->get("explosion").draw(shaderValues,
            unordered_map<string, UniformValue>({
                    {"u_mvp",UniformValue(result)},
                    {"u_size",UniformValue(effect->getSize())},
                    //{"u_dir",UniformValue(effect->getDirection())},
                    {"u_time",UniformValue(dt_s)},
                    {"u_color",UniformValue(effect->getColor())}}));
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
}

