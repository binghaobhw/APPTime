#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <pthread.h>
#include "TimeManager.h"
#include <stdio.h> 
#include <string.h> 
#include <signal.h> 


TimeManager timeManager;
unsigned int interval = 1;
int used = 0;

void *warning(void *arg) {
	used = 1;
	std::string name((char *)arg);
    std::string cmd("./warning " + name);
	std::string pid;
	FILE* pipe = popen(cmd.c_str(), "r");
	pclose(pipe);
	timeManager.delay(name);
	used = 0;
}
void *backend(void *arg) {
	std::string name;
	pthread_t tid;
	while (1) {
		name = timeManager.keepTrack();
		if (name.size() != 0) {
			if (used == 0) {
				char c[64];
				strcpy(c, name.c_str());
				pthread_create(&tid, NULL, warning, c);
			}
		}
		sleep(interval);
	}
}
void *frontend(void *arg) {
	int args;
	std::string name;
	unsigned int time;
	std::map<std::string, unsigned int>::iterator itMap;
	std::vector<unsigned int>::iterator itVint;
	std::vector<std::string>::iterator itVstring;
	std::map<std::string, unsigned int> briefHistory;
	std::map<std::string, unsigned int> timeList;
	std::vector<unsigned int> durationList;
	std::vector<std::string> timeStampList;
	std::map<std::string, unsigned int> timerList;
	std::cout << "0-退出 1-查看历史概要 2-设置定时器 3-取消定时器 4-查看当前记录"<< std::endl;
	std::cout << "5-查看历史时长 6-查看历史细节 7-查看定时器"<< std::endl;
	while (std::cin >> args) {
		switch (args) {
			case 0:
				pthread_exit(0);
				break;
				// 查看历史概要
			case 1:
				timeManager.getBriefHistory(briefHistory);
				for (itMap = briefHistory.begin(); itMap != briefHistory.end(); ++itMap) {
					std::cout << itMap->first << " " << itMap->second << std::endl;
				}
				break;
				// 设置定时器
			case 2:
				std::cout << "name time"<< std::endl;
				if (std::cin >> name >> time) {
					std::cout << "ok"<< std::endl;
					timeManager.addTimer(name, time);
				}
				break;
				// 取消定时器
			case 3:
				std::cout << "name"<< std::endl;
				if (std::cin >> name) {
					std::cout << "ok"<< std::endl;
					timeManager.removeTimer(name);
				}
				break;
				// 得到当前记录
			case 4:
				timeManager.getProgramTime(timeList);
				for (itMap = timeList.begin(); itMap != timeList.end(); ++itMap) {
					std::cout << itMap->first << " " << itMap->second << std::endl;
				}
				break;
				// 查看制定程序历史细节(只有时长)
			case 5:
				std::cout << "name"<< std::endl;
				if (std::cin >> name) {
					std::cout << "ok"<< std::endl;
					timeManager.getHistory(name, durationList);
					for (itVint = durationList.begin();
							itVint != durationList.end(); ++itVint) {
						std::cout << *itVint << std::endl;
					}
				}
				break;
				// 查看制定程序历史细节(时间戳，时长)
			case 6:
				std::cout << "name"<< std::endl;
				if (std::cin >> name) {
					std::cout << "ok"<< std::endl;
					timeManager.getHistory(name, timeStampList, durationList);
					for (itVstring = timeStampList.begin(), itVint = durationList.begin();
							itVstring != timeStampList.end(), itVint != durationList.end(); ++itVstring, ++itVint) {
						std::cout << *itVstring << " " << *itVint << std::endl;
					}
				}
				break;
				// 查看定时器内容
			case 7:
				timerList = timeManager.getTimerList();
				for (itMap = timerList.begin(); itMap != timerList.end(); ++itMap) {
					std::cout << itMap->first << " " << itMap->second << std::endl;
				}
				break;
				
			default:
				break;
		}
		std::cout << "0-退出 1-查看历史概要 2-设置定时器 3-取消定时器 4-查看当前记录"<< std::endl;
		std::cout << "5-查看历史时长 6-查看历史细节 7-查看定时器"<< std::endl;
	}
}
int main() {
	interval = 2;
	timeManager.setInterval(interval);

	pthread_t tid1, tid2;
	int rc1=0, rc2=0;
	rc1 = pthread_create(&tid1, NULL, backend, NULL);
	if (rc1 != 0) {
		std::cout << "t1 error" << std::endl;
	}
	rc2 = pthread_create(&tid2, NULL, frontend, NULL);
	if (rc2 != 0) {
		std::cout << "t2 error" << std::endl;
	}
	pthread_join(tid2, NULL);
	return 0;
}
