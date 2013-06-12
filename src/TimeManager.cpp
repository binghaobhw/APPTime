#include "TimeManager.h"
#include "TimeReader.h"
#include "TimeWriter.h"
#include <fstream>
TimeManager::TimeManager(): 
		programTime(std::map<std::string, TimeList>()),
		history(std::map<std::string, TimeList>()),
		detector(Detector(programTime)) {
	history.clear();
	// 读取定时的内容
	detector.read();
}
std::string TimeManager::keepTrack() {
	std::string currentName;
	getTopWindowName(currentName);
	if (currentName.size() != 0) {
		// 第一次记录时
		if (previousName.size() == 0) {
			previousName = currentName;
			programTime[previousName].add();
		} else {
			// 窗口程序有变化
			if (previousName != currentName) {
				programTime[previousName].update();
				previousName = currentName;
				programTime[previousName].add();
			// 窗口程序不变
			} else {
				programTime[previousName].update();
			}
		}
	}
	// 检查定时程序是否超时
	return detector.poll();
}
void TimeManager::setInterval(unsigned int interval) {
	TimeList::setInterval(interval);
}
void TimeManager::addTimer(std::string name, unsigned int time) {
	detector.addTimer(name, time);
}
void TimeManager::removeTimer(std::string name) {
	detector.removeTimer(name);
}
void TimeManager::getBriefHistory(std::map<std::string, unsigned int> &briefHistory) {
	TimeReader::readSummary(briefHistory);
}
std::map<std::string, TimeList> TimeManager::getFullHistory() {
	return history;
}
std::map<std::string, TimeList> TimeManager::getProgramTime() {
	return programTime;
}
TimeManager::~TimeManager() {
	// 写统计信息
	TimeWriter timeWriter(programTime);
	timeWriter.write();
	// 写定时内容
	detector.write();
}
// 获取置顶窗口程序名，写入programName
// 出错写入空串
void TimeManager::getTopWindowName(std::string &programName) {
	programName.clear();
	// 获取置顶窗口pid
    const std::string cmd("xprop -id $(xprop -root | \
        awk '/_NET_ACTIVE_WINDOW\\(WINDOW\\)/{print $NF}') | \
        awk '/_NET_WM_PID\\(CARDINAL\\)/{print $NF}'");
	std::string pid;
	FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) {
		return;
    }
    char buffer[128];
    while (!feof(pipe)) {
        if (fgets(buffer, 128, pipe) != NULL)
            pid += buffer;
    }
    pclose(pipe);
    if (pid.length() > 0) {
        pid.erase(pid.length() - 1, 1);
    }
	// 从/proc/{pid}/status内容获取程序名
	std::string fileName("/proc/"+ pid +"/status");
    std::ifstream in(fileName.c_str());
    if (!in) {
		return;
    }
    int i;
	std::string temp;
	// 程序名在第2个字段
    for (i = 2; i > 0 && in >> temp; --i);
    if (i == 0) {
        programName = temp;
        return;
    }
    return;
}
void TimeManager::getHistory() {
	if (history.size() == 0) {
		TimeReader historyReader(history);
		historyReader.read();
	}
}
