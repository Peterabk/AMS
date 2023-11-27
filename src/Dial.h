#ifndef DIAL_H_
#define DIAL_H_

#include "Sensor.h"
#include "Sensor1.h"
#include "Sensor2.h"
#include "cTimer.h"

class Dial {
	pthread_mutex_t mutex;
    pthread_t thread_id;
    Sensor* pressureSensor;
    Sensor1* temperatureSensor;
    Sensor2* fuelSensor;
    int period_sec;  // seconds
      int period_msec;

public:
    static const float Safe_Value_Pressure;
    static const float Safe_Value_Temperature;
    static const float Safe_Value_Fuel;

    Dial(Sensor* pressureSensor, Sensor1 *temperatureSensor, Sensor2 *fuelSensor);


    static void* start_routine(void* arg);
    void initialize_dial();
    void monitor_sensors();
    bool check_safe_values(float sensor_value, float safe_limit);
    pthread_t* getThreadID() const {
        return const_cast<pthread_t*>(&thread_id);
    } // Getter for thread_id
    ~Dial();
};

#endif /* DIAL_H_ */

