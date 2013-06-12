#include "TimeReader.h"
#include <time.h>
#include <fstream>
// #include <stdio.h>
// #include <vector>
TimeReader::TimeReader(std::map<std::string, TimeList> &map): progTime(map) {}
void TimeReader::read() {
    std::string summary("summary.at");
    std::string detail;

    std::ifstream detailFile;
	// 读summary.at内容
    std::ifstream summaryFile(summary.c_str());
    if (!summaryFile) {
        return;
    }
	std::string progName;
	unsigned int totalTime;
	// 读入程序名，时间
	while (summaryFile >> progName >> totalTime) {
		progTime[progName] = TimeList(totalTime);
	}
	summaryFile.close();
	// 读每个程序细节
	time_t timeStamp;
	unsigned int duration;
    for (std::map<std::string, TimeList>::iterator it = progTime.begin(); 
			it != progTime.end(); ++it) {
        detail = it->first;
        detail.append(".at");
        detailFile.open(detail.c_str());
        if (!detailFile) {
            return;
        }
		while (detailFile >> timeStamp >> duration) {
			it->second.add(timeStamp, duration);
		}
        detailFile.close();
        detailFile.clear();
	}
}
void TimeReader::readSummary(std::map<std::string, unsigned int> &brief) {
	brief.clear();
    std::string summary("summary.at");
	// 读summary.at内容
    std::ifstream summaryFile(summary.c_str());
    if (!summaryFile) {
        return;
    }
	std::string progName;
	unsigned int totalTime;
	// 读入程序名，时间
	while (summaryFile >> progName >> totalTime) {
		brief[progName] = totalTime;
	}
	summaryFile.close();
}
// void getFileName(std::vector<std::string> &result) {
// 	std::string cmd = "ls | awk \'/.at/{if ($0!~/summary.at/) {print}}\'";
// 	std::string str;
//     FILE* pipe = popen(cmd.c_str(), "r");
//     if (!pipe) {
//         return;
//     }
//     char buffer[128];
//     while (!feof(pipe)) {
//         if (fgets(buffer, 128, pipe) != NULL)
//             str += buffer;
//     }
//     pclose(pipe);
// 	
// 	int i,j;
// 	for (i = 0, j = 0; j < str.size(); ++j) {
// 		if (str[j] == '\n') {
// 			result.push_back(str.substr(i, j - i));
// 			i = j + 1;
// 		}
// 	}
// 	--j;
// 	result.push_back(str.substr(i, j - i));
// }
