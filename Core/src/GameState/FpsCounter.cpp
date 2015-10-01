#include <ChibiEngine/GameState/FpsCounter.hpp>

using namespace game;

const static int READY_FOR_UPDATE = 10;

FpsCounter::FpsCounter()
	:pos(0),cur_sum(0.0f), more(false),max(-1000),min(1000),delay(20),updates(0){
}

void FpsCounter::add(double el){
	if(delay>0){
		delay--;
		return;
	}

	updates++;
	if(el>max) max = el;
	if(el<min) min = el;

	if(!more){
		if(pos<100){
			cur_sum+=el;
			els[pos]=el;
			pos++;
			return;
		}
		more = true;
	}

	if (pos == 100) pos=0;

	cur_sum-=els[pos];
	els[pos]=el;
	cur_sum+=el;
	pos++;
}

bool FpsCounter::isReady2Update(){
	return updates>READY_FOR_UPDATE;
}

double FpsCounter::calcAverageFps(){
	if(delay>0) return 0;
	updates=0;
	return more?cur_sum*0.01f:cur_sum/pos;
}

