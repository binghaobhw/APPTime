#ifndef TIMEMANAGER_H
#define TIMEMANAGER_H
#include <string>
#include <map>
#include "Detector.h"
#include "TimeList.h"
class TimeManager {
	public:
		TimeManager();
		// 追踪并定时
		std::string keepTrack();
		// 追踪前要设定时间间隔，keepTrack内部要计数
		void setInterval(unsigned int interval);
		// 给定程序名和时长进行定时
		void addTimer(std::string name, unsigned int time);
		// 删除一个定时
		void removeTimer(std::string name);
		// 获取历史内容：程序名，所用总时长
		void getBriefHistory(std::map<std::string, unsigned int> &briefHistory);
		// 获取完整历史内容：程序名，所用总时长，每次使用细节
		std::map<std::string, TimeList> getFullHistory();
		// 返回本次运行时记录内容
		std::map<std::string, TimeList> getProgramTime();
		~TimeManager();
	private:
		// 获取置顶窗口程序名，写入programName
		// 出错写入空串
		void getTopWindowName(std::string &programName);
		// 获取历史记录，写入history
		void getHistory();
	private:
		std::map<std::string, TimeList> programTime;
		std::map<std::string, TimeList> history;
		std::string previousName;
		Detector detector;
};
#endif
