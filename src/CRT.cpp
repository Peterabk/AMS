#include "CRT.h"
#include "Dial.h"
#include <iostream>
#include <map>
CRT::CRT() {


}

std::map<std::string, std::string> lampStatuses;

// Implement the setLampStatus method
void CRT::setLampStatus(const std::string& dialName, const std::string& status) {
    lampStatuses[dialName] = status;
}
void CRT::updateLampStatus(const std::string& dialName, const std::string& status) {
    lampStatuses[dialName] = status;
}
void CRT::updateDialStatus(const std::string& dialName, float dialValue) {
    // Assuming you have a map named dialStatusMap to store the statuses
    dialStatusMap[dialName] = dialValue;
}
void CRT::updateWarningStatus(const std::string& dialName, bool warning) {
    // Assuming you have a map named dialStatusMap to store the statuses
    dialWarningMap[dialName] = warning;
}

void CRT::displayInterface(int period_sec, int period_msec ) {
//	this->period_sec=period_sec;
//	this->period_msec=period_msec;
//	cTimer timer(period_sec,period_msec);
//	bool answer;
//    // Display LAMPS with their statuses
//	while(true){
//    std::cout << "================ LAMPS ================\n";
//    for (const auto& lamp : lampStatuses) {
//        std::cout << lamp.first << "-> " << lamp.second << "\n";
//
//    }
//    std::cout << "================ DIALS ================\n";
//       for (const auto& dial : dialStatusMap) {
//           std::cout << dial.first << "-> " << dial.second << "\n";
//
//       }
//       std::cout << "================ WARNING ================\n";
//       for (const auto& lamp : lampStatuses) {
//    	   if(lamp.second=="RED"){
//            std::cout << "WARNING CHECK :"<<lamp.first << "\n";
//    	   }
//        }
//  timer.waitTimer();
//
//	}

}

