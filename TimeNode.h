#ifndef TIMENODE_H
#define TIMENODE_H
#include <string> 
#include <time.h> 
#include <iostream>
using std::string;
class TimeNode {
    private:
        time_t timeStamp;
        unsigned int duration;
        TimeNode *next;
    public:
        TimeNode();
        void setDuration(unsigned int d);
        void setNext(TimeNode *n);
        unsigned int getDuration();
        TimeNode *getNext();
        void getTime(string &t);
        time_t getTimeStamp();
};
#endif
