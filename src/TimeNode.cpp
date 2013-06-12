#include "TimeNode.h"
TimeNode::TimeNode(): duration(0), next(NULL) {
    time(&timeStamp);
}
TimeNode::TimeNode(time_t ts, unsigned int d): 
	timeStamp(ts), duration(d), next(NULL) {
}
void TimeNode::addDuration(unsigned int d) {
    duration += d;
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
void TimeNode::getTime(std::string &t) {
    t = std::string(ctime(&timeStamp));
}
time_t TimeNode::getTimeStamp(){
    return this->timeStamp;
}
