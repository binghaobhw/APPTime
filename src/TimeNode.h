#ifndef TIMENODE_H
#define TIMENODE_H
#include <string> 
#include <time.h> 
#include <iostream>
class TimeNode {
    public:
        TimeNode();
		TimeNode(time_t ts, unsigned int d);
        void addDuration(unsigned int d);
        void setNext(TimeNode *n);
        unsigned int getDuration();
        TimeNode *getNext();
        void getTime(std::string &t);
        time_t getTimeStamp();
    private:
        time_t timeStamp;  // 每个节点创建的时间戳
        unsigned int duration;  // 单次窗口置顶的持续时间
        TimeNode *next;  // 下个节点
};
#endif
