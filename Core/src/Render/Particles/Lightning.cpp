#include <ChibiEngine/Render/Particles/Lightning.h>
#include <ChibiEngine/Resource/Resources.h>
#include <ChibiEngine/Clock/EventSystem.h>
#define GLM_FORCE_RADIANS
#include <glm/ext.hpp>

#include <random>

using namespace game;
using namespace glm;
using namespace std;

const std::string LightningTemplate::NAME="lightning";

static const int ATR_PER_POINT = 4*2;
static const int NUM = 3;
static const int pointsNum = 20;

inline float randf(float min, float max){
    return min + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(max-min)));
}

LightningTemplate::LightningTemplate()
    :EffectTemplate(NAME),
     regenerationStarted(false){
    shaderValues.drawMode= GL_TRIANGLE_STRIP;
    //shaderValues.count=pointsNum*2*NUM;
}

void LightningTemplate::generateBuffer() {
    vector<float> data(NUM * pointsNum * ATR_PER_POINT);
    for(int gb=0; gb<NUM;++gb) {
        vector<float> xes(pointsNum);
        xes[0] = -1.0;
        for (int i = 1; i < pointsNum - 1; ++i) {
            xes[i] = randf(-0.9990, 0.9990);
        }
        xes[pointsNum - 1] = 1.000;
        sort(xes.begin(), xes.end());

        vector<float> yes(pointsNum);
        //    const float sway = 2;
        //    const float jaggedness = 1/sway;
        float prevDisplacement = 0;
        yes[0] = 0.0;
        for (int i = 1; i < pointsNum; ++i) {

            float envelope = fabs(xes[i]) > 0.85f ? 0.25 * (1 - fabs(xes[i])) : 1;

            float displacement = randf(-1.0, 1.0);
            displacement -= (displacement - prevDisplacement) /** (1 - (xes[i]-xes[i-1])*0.5)*/* 1.1;
            displacement *= envelope;
            prevDisplacement = displacement;

            yes[i] = (displacement);
        }
        // yes[pointsNum-1]=0.0;

        const static float hh = 0.1;
        for (int i = 0; i < pointsNum; ++i) {
            int cur = ATR_PER_POINT * i + gb * (pointsNum * ATR_PER_POINT);

            data[cur++] = xes[i];
            data[cur++] = yes[i] + hh;
            data[cur++] = 0.0;
            data[cur++] = 1.0;

            data[cur++] = xes[i];
            data[cur++] = yes[i] - hh;
            data[cur++] = 0.0;
            data[cur++] = -1.0;
        }

    }

    shaderValues.fillBuffer(data);
    if(!regenerationStarted){
        ClockHandle clock = Game::getLevelClock();
        Game::getEventSystem()->scheduleInfiteRepeat(clock, bind(&LightningTemplate::generateBuffer, this), clock->getTime(), 100);
        regenerationStarted=true;
    }

}

void LightningTemplate::draw(CameraCHandle camera, float /*dt_s*/, const ParticleEffect* effect) const{
    glm::mat4 result = camera->getMVPMatrix() * glm::translate(effect->getPosition()) * scale(vec3(effect->getSize(),1.0f));
    shaderValues.uniformValues["u_mvp"]=UniformValue(result);
        shaderValues.count=pointsNum*2*NUM;

    //for(int i=0;i<NUM;++i){
     //   shaderValues.startOffset = i*pointsNum*2;
        Game::getShaderSystem()->get("lightning").draw(shaderValues);
    //}
}

