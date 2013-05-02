#include <string>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <fstream>
#include <map>
#include <time.h>
using namespace std;

// 0 正常  -1 失败
int getPID(const string &cmd, string &pid) {
    pid.clear();
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) {
        return -1;
    }
    char buffer[128];
    while(!feof(pipe)) {
        if(fgets(buffer, 128, pipe) != NULL)
            pid += buffer;
    }
    pclose(pipe);
    if(pid.length() > 0) {
        pid.erase(pid.length() - 1, 1);
    }
    return 0;
}
// -1 出错  0 新的  1 旧的
int getProgName(const string &fileName, string &progName) {
    string temp;
    ifstream in(fileName.c_str());
    if(!in) {
        return -1;
    }
    int i;
    for(i = 2; i > 0 && in >> temp; i--);
    if(i == 0 && progName != temp) {
        progName = temp;
        return 0;
    }
    return (i == 0)? 1: -1;
}
class TimeNode {
    private:
        time_t timeStamp;
        unsigned int duration;
        TimeNode *next;
    public:
        TimeNode(): duration(0), next(NULL) {
            time(&timeStamp);
        }
        void setDuration(unsigned int d) {
            duration = d;
        }
        void setNext(TimeNode *n) {
            next = n;
        }
        unsigned int getDuration() {
            return duration;
        }
        TimeNode *getNext() {
            return next;
        }
        void getTime(string &t) {
            t = string(ctime(&timeStamp));
        }

};
class TimeList {
    private:
        unsigned int total;
        TimeNode *first;
        TimeNode *last;
    public:
        TimeList(): total(0), first(NULL), last(NULL) { }
        /*新一个时间戳,表示又切换回来了
         *0 正常加入 */
        int add() {
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
        int finish(unsigned int duration) {
            if(last == NULL) {
                return -1;
            }
            last->setDuration(duration);
            total += duration;
            return 0;
        }
        unsigned int getTotal() {
            return this->total;
        }
        TimeNode *getFirst() {
            return this->first;
        }
        ~TimeList() {
            TimeNode *x = first;
            TimeNode *y;
            while(x != NULL) {
                y = x;
                x = x->getNext();
                delete y;
            }
        }
};

int main()
{
    const string cmd("xprop -id $(xprop -root | awk '/_NET_ACTIVE_WINDOW\\(WINDOW\\)/{print $NF}') | awk '/_NET_WM_PID\\(CARDINAL\\)/{print $NF}'");
    string fileName;
    string progName;
    string newProg;
    string pid;
    const unsigned int interval = 5;  // 轮询间隔（s）
    unsigned int duration = 0;
    map<string, TimeList> progMap;
    int count = 5;
    while(count > 0) {
        count--;
        if(getPID(cmd, pid) == 0) {
            fileName = "/proc/"+ pid +"/status";
            if(getProgName(fileName, newProg) == 0) {
                // 第一次记录，progName为空
                if(progName.length() == 0) {
                    progName = newProg;
                    progMap[progName].add();
                }
                // 除上面的情况外
                else {
                    progMap[progName].finish(duration);
                    progName = newProg;
                    progMap[progName].add();
                    duration = 0;
                }
            }
        }
        sleep(interval);
        duration += interval;
    }
    progMap[progName].finish(duration);
    map<string, TimeList>::iterator it = progMap.begin();
    while(it != progMap.end()) {
        cout << it->first << "\t" << it->second.getTotal() << endl;
        TimeNode *p = it->second.getFirst();
        while(p != NULL) {
            string t;
            p->getTime(t);
            cout << t << "\t" << p->getDuration() << endl;
            p = p->getNext();
        }
        it++;
    }
    return 0;
}
