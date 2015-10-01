#define GLM_FORCE_RADIANS


#include <ChibiEngine/Render/Particles/Splash.h>
#include <ChibiEngine/Render/Shaders.h>
#include <glm/gtx/fast_square_root.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace game;
using namespace glm;
using namespace std;

const std::string SplashTemplate::NAME="splash";

static const int ATR_PER_PARTICLE = 6;
static const int partPerGroup = 10;
static const int groupsNum = 20;
static const int particesNum = partPerGroup*groupsNum;

static const float _1_GY_2 = 2.0/9.8;

inline float randf(float min, float max){
	 return min + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(max-min)));
}

SplashTemplate::SplashTemplate()
        :EffectTemplate(NAME){
    shaderValues.count = particesNum;
    shaderValues.drawMode = GL_POINTS;
}

void SplashTemplate::generateBuffer() {
    float data[particesNum*ATR_PER_PARTICLE];

    for(int groupNum =0 ;groupNum<groupsNum;++groupNum){
        vec3 begin(randf(-1,1), randf(-1.0,1.0), randf(0,0));

        for(int part=0; part<partPerGroup;++part){
            int cur=ATR_PER_PARTICLE*part*groupNum;
            vec2 vel = fastNormalize(vec2(randf(-1.0,1.0),1.0))*randf(1,2);

            data[cur++]=begin.x;
            data[cur++]=begin.y;
            data[cur++]=begin.z;

            data[cur++]=vel.x;
            data[cur++]=vel.y;

            data[cur++]=vel.y*_1_GY_2;

        }
    }
    shaderValues.fillBuffer(data, particesNum*ATR_PER_PARTICLE);
}

void SplashTemplate::draw(CameraCHandle camera, float dt_s, ParticleEffectCHandle effect) const{
	glm::mat4 result = camera->getMVPMatrix() * glm::translate(mat4(1.0f),effect->getPosition());
    Game::getShaderSystem()->get("splash").draw(shaderValues,
            unordered_map<string, UniformValue>({
            {"u_mvp",UniformValue(result)},
            {"u_size",UniformValue(effect->getSize())},
            {"u_time",UniformValue(dt_s)},
            {"u_color",UniformValue(Color(1.0f,1.0f,1.0f,0.6f))}}));
}

