#include "TimeNode.h"
TimeNode::TimeNode(): duration(0), next(NULL) {
    time(&timeStamp);
}
void TimeNode::setDuration(unsigned int d) {
    duration = d;
}
void TimeNode::setNext(TimeNode *n) {
    next = n;
}
unsigned int TimeNode::getDuration() {
    return duration;
}
TimeNode *TimeNode::getNext() {
    return next;
}
void TimeNode::getTime(string &t) {
    t = string(ctime(&timeStamp));
}
time_t TimeNode::getTimeStamp(){
    return this->timeStamp;
}
