#ifndef CRT_H
#define CRT_H

#include <string>
#include <map>
#include "cTimer.h"

class CRT {
	int period_sec; //seconds
		int period_msec;//milliseconds
private:
    std::string lastWarning;
    bool warningAcknowledged;
    std::map<std::string, std::string> lampStatuses;
    std::map<std::string, float> dialStatusMap;
    std::map<std::string, bool> dialWarningMap;


public:
    CRT();
    void updateLampStatus(const std::string& dialName, const std::string& status);
    void updateDialStatus(const std::string& dialName, float dialValue);
    void updateWarningStatus(const std::string& dialName, bool warning);
    // Function to display sensor readings and calculated measures
    void displayReadings(const std::string& reading);
    // Add a method to set lamp status information
    void setLampStatus(const std::string& dialName, const std::string& status);
    void displayInterface(int period_sec, int period_msec);
    // Function to display warning messages
    void displayWarning(const std::string& warning);
    void displayLamp();
    // Function to acknowledge warning messages
    void acknowledgeWarning();

    // Function to check if there is an unacknowledged warning
    bool isWarningActive() const;
};

#endif // CRT_H
