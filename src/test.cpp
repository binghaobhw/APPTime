#include <map>
#include <string>
#include <iostream>
#include <pthread.h>
#include "TimeManager.h"

TimeManager timeManager;
unsigned int interval = 1;

void *backend(void *arg) {
	std::string name;
	while (1) {
		name = timeManager.keepTrack();
		if (name.size() != 0) {
			std::cout << name << " timeout" << std::endl;
		}
		sleep(interval);
	}
}
void *frontend(void *arg) {
	int args;
	while (std::cin >> args) {
		if (args == 0) {
			pthread_exit(0);
		}
		else if (args == 1) {
			std::map<std::string, unsigned int> briefHistory;
			timeManager.getBriefHistory(briefHistory);
			for (std::map<std::string, unsigned int>::iterator it = briefHistory.begin(); 
					it != briefHistory.end(); ++it) {
				std::cout << it->first << " " << it->second << std::endl;

			}
		}
		else if (args == 2) {
			timeManager.addTimer(std::string("gnome-terminal"), (unsigned int)30);
		}
		else if (args == 3) {
			timeManager.removeTimer(std::string("gnome-terminal"));
		}
	}
}
int main() {
	interval = 5;
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
    // std::map<std::string, TimeList> progMap;
	// TimeReader timeReader(progMap);
	// timeReader.read();
	// std::cout << "ok" << std::endl;
    // for (std::map<std::string, TimeList>::iterator it = progMap.begin(); 
	// 		it != progMap.end(); ++it) {
	// 	std::cout << it->first << std::endl;
    //     std::cout << it->second.getTotal() << std::endl;

    //     TimeNode *p = it->second.getFirst();
    //     while (p != NULL) {
    //         std::cout << p->getTimeStamp();
    //         std::cout << " " << p->getDuration() << std::endl;
    //         p = p->getNext();
    //     }
	// }
	
	return 0;
}
