#include <string>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <fstream>
#include <map>
#include <time.h>
#include "TimeList.h"
#include "TimeNode.h"
using std::string;
using std::map;
using std::ifstream;
using std::endl;
using std::cout;
using std::ofstream;
using std::ios;
/*各程序名和总时长写入"summary.at"
 *格式: 第一行程序名,第二行总时长(s)
 *每个程序每次使用时刻和时长写入"程序名.at"
 *格式: 时间戳+空格+时长(s) */
int outPut(map<string,TimeList> &progMap) {
    string summary("summary.at");
    string detail;

    ofstream detailFile;
    ofstream summaryFile(summary.c_str(),ios::out);
    if (!summaryFile) {
        return -1;
    }
    map<string, TimeList>::iterator it = progMap.begin();
    while(it != progMap.end()) {
        summaryFile << it->first << endl;
        summaryFile << it->second.getTotal() << endl;

        detail = it->first;
        detail.append(".at");

        detailFile.open(detail.c_str(),ios::out);
        if (!detailFile) {
            return -1;
        }

        TimeNode *p = it->second.getFirst();
        while(p != NULL) {
            detailFile << p->getTimeStamp();
            detailFile << " " << p->getDuration() << endl;
            p = p->getNext();
        }
        detailFile.close();
        detailFile.clear();

        ++it;
    }
    summaryFile.close();
    return 0;
}

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

int main()
{
    const string cmd("xprop -id $(xprop -root | \
        awk '/_NET_ACTIVE_WINDOW\\(WINDOW\\)/{print $NF}') | \
        awk '/_NET_WM_PID\\(CARDINAL\\)/{print $NF}'");
    string fileName;
    string progName;
    string newProg;
    string pid;
    const unsigned int interval = 1;  // 轮询间隔（s）
    unsigned int duration = 0;
    map<string, TimeList> progMap;
    int count = 10;
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
                // 主窗口切换
                else {
                    progMap[progName].finish(duration);
                    progName = newProg;
                    progMap[progName].add();
                    duration = 0;
                }
                cout << progName << "\t" << progMap[progName].getTotal() << endl;
            }
        }
        sleep(interval);
        duration += interval;
    }
    progMap[progName].finish(duration);
    outPut(progMap);
    return 0;
}
