 #include "Dial.h"
#include <iostream>

#define Safe_Value_Pressure float(35)
#define Safe_Value_Temperature float(30)
#define Safe_Value_Fuel float(100)

Dial::Dial(int period_sec, int period_msec,Sensor* pressureSensor, Sensor1 *temperatureSensor, Sensor2 *fuelSensor) {
    this->pressureSensor = pressureSensor;
  this->temperatureSensor = temperatureSensor;
  this->fuelSensor = fuelSensor;
  this->period_sec = period_sec;
     this->period_msec = period_msec;
    initialize_dial();
}


void* Dial::start_routine(void* arg) {
    Dial* dial = static_cast<Dial*>(arg);
    dial->monitor_sensors();
    return NULL;
}

void Dial::initialize_dial() {
    if (pthread_create(&thread_id, nullptr, start_routine, this) != EOK) {
        thread_id = NULL;
    }
}

void Dial::monitor_sensors() {
	 cTimer timer1(period_sec, period_msec);

    int consecutiveOutOfRangePressure = 0;
    int consecutiveOutOfRangeTemperature=0;
    int consecutiveOutOfRangeFuel=0;


    while (true) {
        // Obtain sensor values
        float pressure_value = pressureSensor->get_last_reading();
        float temperature_value = temperatureSensor->get_last_reading();
        float fuel_value = fuelSensor->get_last_reading();

        // Check for safe values for each sensor
        bool pressureSafe = check_safe_values(pressure_value, Safe_Value_Pressure);
        bool fuelSafe = check_safe_values(fuel_value, Safe_Value_Fuel);
        bool temperatureSafe = check_safe_values(temperature_value, Safe_Value_Temperature);

        // Check if any sensor is out of range
        if (!pressureSafe) {
            consecutiveOutOfRangePressure++;
        } else {
            consecutiveOutOfRangePressure = 0; // Reset if the value is in range
        }
        if (!temperatureSafe) {
                  consecutiveOutOfRangeTemperature++;
              } else {
                  consecutiveOutOfRangeTemperature = 0; // Reset if the value is in range
              }
        if (!fuelSafe) {
            consecutiveOutOfRangeFuel++;
        } else {
            consecutiveOutOfRangeFuel = 0; // Reset if the value is in range
        }
//


        // Trigger warning for each individual sensor if consecutiveOutOfRange reaches the threshold
        if (consecutiveOutOfRangePressure >= 3) {
            std::cout << "LAMP for PRESSURE: RED " << std::endl;
            // Reset after triggering the warning
            consecutiveOutOfRangePressure = 0;}
        else { std::cout << "LAMP for PRESSURE: GREEN " << std::endl;}
      //     	usleep(1000000);
        if (consecutiveOutOfRangeTemperature >= 3) {
        	std::cout << "LAMP for Temperature: RED " << std::endl;
        	            // Reset after triggering the warning
        	            consecutiveOutOfRangePressure = 0;}
        	        else { std::cout << "LAMP for Temperature: GREEN " << std::endl;}
      timer1.waitTimer();
        if (consecutiveOutOfRangeFuel >= 3) {
           // std::cout << "Warning: Three consecutive fuel readings out of range!" << std::endl;
            // Reset after triggering the warning
            consecutiveOutOfRangeFuel = 0;}

        timer1.waitTimer();
    }
}

bool Dial::check_safe_values(float sensor_value, float safe_limit) {
    return sensor_value >= safe_limit;
}

Dial::~Dial() {
    thread_id = NULL;
}
