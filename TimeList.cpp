#include "TimeList.h"
TimeList::TimeList(): total(0), first(NULL), last(NULL) { }
/*新一个时间戳,表示又切换回来了
 *0 正常加入 */
int TimeList::add() {
    if(last == NULL) {
        last = new TimeNode();
        first = last;
        return -1;
    }
    TimeNode *p = new TimeNode();
    last->setNext(p);
    last = p;
    return 0;
}
/* 把当前的时间戳加上duration
 * 更新total */
int TimeList::finish(unsigned int duration) {
    if(last == NULL) {
        return -1;
    }
    last->setDuration(duration);
    total += duration;
    return 0;
}
unsigned int TimeList::getTotal() {
    return this->total;
}
TimeNode *TimeList::getFirst() {
    return this->first;
}
TimeList::~TimeList() {
    TimeNode *x = first;
    TimeNode *y;
    while(x != NULL) {
        y = x;
        x = x->getNext();
        delete y;
    }
}
