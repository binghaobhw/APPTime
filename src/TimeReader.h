#ifndef TIMEREADER_H
#define TIMEREADER_H
#include <map>
#include <string>
#include "TimeList.h"
class TimeReader {
	public:
		TimeReader(std::map<std::string, TimeList> &map);
		void read();
		static void readSummary(std::map<std::string, unsigned int> &brief);
		// void getFileName(std::string &result);
	private:
		std::map<std::string, TimeList> &progTime;
};
#endif
