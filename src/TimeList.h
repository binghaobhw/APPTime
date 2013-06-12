#ifndef TIMELIST_H
#define TIMELIST_H
#include "TimeNode.h"
#include <time.h>
class TimeList {
    public:
        TimeList();
		TimeList(unsigned int t);
        // 新一个时间戳,表示又从别的程序切换回来了
        void add();
		void add(time_t ts, unsigned int d);
		// 更新last节点duration
		void update();
        unsigned int getTotal();
        TimeNode *getFirst();
		static void setInterval(unsigned int i);
        ~TimeList();
	private:
        unsigned int total;
        static unsigned int interval;
        TimeNode *first;
        TimeNode *last;
};
#endif
