#include "CRT.h"
#include "Dial.h"
#include <iostream>
#include <sstream>
#include <map>


#include <cstdio>
char CRT::getChar() {
    char input = getchar();
    clearInputBuffer();
    return input;
}

char CRT::getConfirmation() {
    char input;
    do {
        input = getchar();
        if (input != 'y' && input != 'n') {
            std::cout << "Wrong input!\n";
        }
    } while (input != 'y' && input != 'n');
    clearInputBuffer();
    return input;
}

void CRT::clearInputBuffer() {
    char c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

// Implementation of to_string template function
template <typename T>
std::string CRT::to_string(T value) {
    std::ostringstream os;
    os << value;
    return os.str();
}
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

void CRT::displayInterface(int period_sec, int period_msec) {
    this->period_sec = period_sec;
    this->period_msec = period_msec;
    cTimer timer(period_sec, period_msec);

    while(true) {
        bool hasWarning = false;

        // First, check for warnings
        for (const auto& lamp : lampStatuses) {
            if(lamp.second == "RED") {
                std::cout << "================ WARNING ================\n";
                std::cout << "WARNING CHECK: " << lamp.first << "\n";
                hasWarning = true;
            }
        }

        // If there's a warning, wait for user input
        if (hasWarning) {
            std::cout << "Press 'y' to acknowledge and view statuses (or 'n' to ignore): ";
            char answer = getChar();
            if (answer != 'y' && answer != 'Y') {
                continue; // Skip the rest of the loop if 'y' is not pressed
            }
        }

        // Display LAMPS and DIALS statuses if there's no warning or warning is acknowledged
        std::cout << "================ LAMPS ================\n";
        for (const auto& lamp : lampStatuses) {
            std::cout << lamp.first << "-> " << lamp.second << "\n";
        }

        std::cout << "================ DIALS ================\n";
        for (const auto& dial : dialStatusMap) {
            std::cout << dial.first << "-> " << dial.second << "\n";
        }

        timer.waitTimer();
    }
}

