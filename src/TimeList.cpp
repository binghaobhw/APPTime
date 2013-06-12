#include "TimeList.h"
// 静态变量必须定义，以分配空间
unsigned int TimeList::interval = 5;
TimeList::TimeList(): total(0), first(NULL), last(NULL) {
}
TimeList::TimeList(unsigned int t):
	total(t), first(NULL), last(NULL) {
}
// 新一个时间戳,表示又切换回来了
void TimeList::add() {
    if (last == NULL) {
        last = new TimeNode();
        first = last;
    } else {
		TimeNode *p = new TimeNode();
		last->setNext(p);
		last = p;
	}
}
void TimeList::add(time_t ts, unsigned int d) {
	if (last == NULL) {
		last = new TimeNode(ts, d);
		first = last;
	} else {
		TimeNode *p = new TimeNode(ts, d);
		last->setNext(p);
		last = p;
	}
}
void TimeList::update() {
	if (last != NULL) {
		last->addDuration(interval);
		total += interval;
	}
}
unsigned int TimeList::getTotal() {
    return total;
}
TimeNode *TimeList::getFirst() {
    return this->first;
}
void TimeList::setInterval(unsigned int i) {
	interval = i;
}
TimeList::~TimeList() {
    TimeNode *x = first;
    TimeNode *y;
    while (x != NULL) {
        y = x;
        x = x->getNext();
        delete y;
    }
}
