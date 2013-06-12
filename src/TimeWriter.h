#ifndef TIMEWRITER_H
#define TIMEWRITER_H
#include <map>
#include <string>
#include "TimeList.h"
class TimeWriter {
	public:
		TimeWriter(std::map<std::string, TimeList> &map);
		void write();
	private:
		std::map<std::string, TimeList> &progTime;
};
#endif
