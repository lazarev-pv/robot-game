#ifndef FPSCOUNTER_H_
#define FPSCOUNTER_H_

namespace game{

	class FpsCounter{
	public:

		FpsCounter();

		void add(double el);

		double calcAverageFps();

		inline double getMinFps(){
			return min;
		}

		inline double getMaxFps(){
			return max;
		}

		bool isReady2Update();

	private:
		int pos;
		double els[100];
		double cur_sum;
		bool more;
		double max, min;
		int delay;

		int updates;
	};

}

#endif /* FPSCOUNTER_H_ */
