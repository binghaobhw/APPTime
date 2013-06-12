#include "TimeWriter.h"
#include <fstream>
#include "TimeNode.h"
#include "TimeReader.h"
TimeWriter::TimeWriter(std::map<std::string, TimeList> &map): progTime(map) {}
void TimeWriter::write() {
	// 先把summary内容读出
	std::map<std::string, unsigned int> summaryContent;
	TimeReader::readSummary(summaryContent);
	// 把本次记录的东西更新到summaryContent
    std::map<std::string, TimeList>::iterator it;
	for (it = progTime.begin(); it != progTime.end(); ++it) {
		summaryContent[it->first] += it->second.getTotal();
	}
	// 把summaryContent写入文件
    std::string summary("summary.at");
    std::ofstream summaryFile(summary.c_str());
    if (!summaryFile) {
        return;
    }
	for (std::map<std::string, unsigned int>::iterator itSummary = summaryContent.begin(); 
			itSummary != summaryContent.end(); ++itSummary) {
        summaryFile << itSummary->first;
        summaryFile << " " << itSummary->second << std::endl;
	}
	summaryFile.close();
	// 写每个程序的细节，追加
    std::string detail;
    std::ofstream detailFile;
	for (it = progTime.begin(); it != progTime.end(); ++it) {
        detail = it->first;
        detail.append(".at");
		// 追加记录
        detailFile.open(detail.c_str(),std::ios::app);
        if (!detailFile) {
            return;
        }
        TimeNode *p = it->second.getFirst();
        while (p != NULL) {
            detailFile << p->getTimeStamp();
            detailFile << " " << p->getDuration() << std::endl;
            p = p->getNext();
        }
        detailFile.close();
        detailFile.clear();
	}
}
