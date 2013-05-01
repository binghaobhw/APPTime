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
        // 0 正常加入  -1 新建
        int add(unsigned int yetDuration) {
            if(last == NULL) {
                last = new TimeNode();
                first = last;
                return -1;
            }
            last->setDuration(yetDuration);
            total += yetDuration;
            TimeNode *p = new TimeNode();
            last->setNext(p);
            last = p;
            return 0;
        }
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
    string pid;
    const unsigned int interval = 5;
    unsigned int duration = 0;
    map<string, TimeList> progMap;
    int count = 10;
    while(count > 0) {
        count--;
        if(getPID(cmd, pid) == 0) {
            cout << pid << endl;
            fileName = "/proc/"+ pid +"/status";
            cout << fileName << endl;
            switch(getProgName(fileName, progName)) {
                case 0: // 新程序
                    progMap[progName].add(duration);
                    duration = 0;
                    break;
                case 1: // 旧程序
                    break;
                case -1:
                    break;
                default:
                    break;
            }
            cout << progName << endl;
        }
        sleep(interval);
        duration += interval;
    }
    progMap[progName].finish(duration);
    map<string, TimeList>::iterator it = progMap.begin();
    while(it != progMap.end()) {
        cout << it->first << endl;
        cout << it->second.getTotal() << endl;
        it++;
    }
    return 0;
}
