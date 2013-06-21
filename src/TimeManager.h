#ifndef TIMEMANAGER_H
#define TIMEMANAGER_H
#include <string>
#include <map>
#include <vector>
#include "Detector.h"
#include "TimeList.h"
class TimeManager {
	public:
		TimeManager();
		// 追踪并定时
		// 返回：程序名（如果有超时）
		//       空串（没超时程序）
		std::string keepTrack();
		// 追踪前要设定时间间隔，keepTrack内部要计数
		void setInterval(unsigned int interval);
		// 给定程序名和时长进行定时
		void addTimer(std::string name, unsigned int time);
		// 删除一个定时
		void removeTimer(std::string name);
		// 定时器延迟
		void delay(std::string name);
		// 获取历史内容：程序名，所用总时长
		// 写入briefHistory
		void getBriefHistory(std::map<std::string, unsigned int> &briefHistory);
		// 获取name指定程序的历史使用细节：每次使用时长
		// 写入durationList
		void getHistory(std::string name, std::vector<unsigned int> &durationList);
		// 获取name指定程序的历史使用细节：每次开始使用时间戳，每次使用时长
		// 写入timeStampList, durationList
		void getHistory(std::string name, std::vector<std::string> &timeStampList, std::vector<unsigned int> &durationList);
		// 返回本次运行时记录内容
		// 写入timeList
		void getProgramTime(std::map<std::string, unsigned int> &timeList);
		// 返回定时器信息
		std::map<std::string, unsigned int> getTimerList();
		~TimeManager();
	private:
		// 获取置顶窗口程序名，写入programName
		// 出错写入空串
		void getTopWindowName(std::string &programName);
	private:
		std::map<std::string, TimeList> programTime;
		std::map<std::string, TimeList> history;
		std::string previousName;
		Detector detector;
};
#endif
