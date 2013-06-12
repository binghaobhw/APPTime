#ifndef DETECTOR_H
#define DETECTOR_H
#include "TimeList.h"
#include <map>
#include <string>
class Detector {
	public:
		Detector(std::map<std::string, TimeList> &map);
		// 查询定时的程序时间是否到了
		// 返回程序名或空串
		std::string poll();
		// 新增要定时的程序
		void addTimer(std::string name, unsigned int time);
		void removeTimer(std::string name);
		// 把timerList写到文件
		void write();
		// 读文件到timeList
		void read();
	private:
		std::map<std::string, unsigned int> timerList;
		std::map<std::string, TimeList> &progTime;
};
#endif
