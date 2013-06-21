#include "Detector.h"
#include <iostream>
#include <fstream>
Detector::Detector(std::map<std::string, TimeList> &map): 
	timerList(std::map<std::string, unsigned int>()), progTime(map) {}

std::string  Detector::poll() {
	std::map<std::string, unsigned int>::iterator it = timerList.begin();
    while (it != timerList.end()) {
	    if (progTime[it->first].getTotal() >= it->second) {
			return it->first;
		}
		++it;
	}
	return std::string();
}
void Detector::addTimer(std::string name, unsigned int time) {
	timerList[name] = time;
}
void Detector::removeTimer(std::string name) {
	timerList.erase(name);
}
void Detector::write() {
	std::string file("timerList.tl");
	std::ofstream out(file.c_str());
    if (!out) {
        return;
    }
	for(std::map<std::string, unsigned int>::iterator it = timerList.begin(); 
			it != timerList.end(); ++it) {
		out << it->first << std::endl;
		out << it->second << std::endl;
	}
	out.close();
}
void Detector::read() {
	std::string file("timerList.tl");
	std::ifstream in(file.c_str());
    if (!in) {
        return;
    }
	std::string name;
	unsigned int time;
	timerList.clear();
	while (in >> name >> time) {
		timerList[name] = time;
	}
	in.close();
}
std::map<std::string, unsigned int> Detector::getTimerList() {
	return timerList;
}
void Detector::delay(std::string name) {
	timerList[name] += 60;
}
