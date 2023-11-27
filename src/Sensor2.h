#ifndef SENSOR2_H_
#define SENSOR2_H_

#include "cTimer.h"
#include <iostream>

class Sensor2 {
private:
    pthread_mutex_t mutex;
    int period_sec;  // seconds
    int period_msec; // milliseconds
    int upper_bound;

    float last_reading;


public:
    pthread_t thread_id;
    static const float Safe_Value;

    static void* start_routine(void* arg);
    void initialize_sensor();
    void sensorpoll();

    Sensor2(int period_sec, int period_msec, int upper_bound);
    ~Sensor2();

    float generate_random();

    // Declarations for the new functions
    bool check_safe_values(float sensor_value);
    std::string get_sensor_type();
    float get_last_reading() const; // Move the declaration here
    void set_last_reading(float reading);
};

#endif /* SENSOR_H_ */
