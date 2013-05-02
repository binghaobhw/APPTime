#ifndef TIMELIST_H
#define TIMELIST_H
#include <iostream>
#include "TimeNode.h"
class TimeList {
    private:
        unsigned int total;
        TimeNode *first;
        TimeNode *last;
    public:
        TimeList();
        /*新一个时间戳,表示又切换回来了
         *0 正常加入 */
        int add();
        /* 把当前的时间戳加上duration
         * 更新total */
        int finish(unsigned int duration);
        unsigned int getTotal();
        TimeNode *getFirst();
        ~TimeList();
};
#endif
