#include "Sensor1.h"
#include <cstdlib>
#include <iostream>

const float Sensor1::Safe_Value = 10.0;

void* Sensor1::start_routine(void* arg) {
    Sensor1* sensor1 = static_cast<Sensor1*>(arg);
    sensor1->sensorpoll();
    return NULL;
}

void Sensor1::initialize_sensor() {
    if (pthread_create(&thread_id, nullptr, start_routine, this) != EOK) {
        thread_id = pthread_t(); // Use default constructor to set to a valid thread_id
    }
}

Sensor1::Sensor1(int period_sec, int period_msec, int upper_bound) {
    this->period_sec = period_sec;
    this->period_msec = period_msec;
    this->upper_bound = upper_bound;
    initialize_sensor();
}

void Sensor1::sensorpoll() {
    cTimer timer(period_sec, period_msec);
    float last_reading = 0; // Store the last reading

    while (true) {
        float current_reading = generate_random();

        last_reading = current_reading;


        // Print the sensor reading
      //  std::cout << "The Sensor Temperature  did this: " << current_reading << " at the location\n";

        set_last_reading(last_reading);
        timer.waitTimer();

    }
}


Sensor1::~Sensor1() {
    thread_id = pthread_t(); // Use default constructor to set to a valid thread_id
}

float Sensor1::generate_random() {
    return static_cast<float>(rand() % upper_bound);
}


bool Sensor1::check_safe_values(float sensor_value) {
    // Implement your logic to check if the sensor value is within a safe range
    // Replace the following line with your actual logic
    return sensor_value <= Safe_Value; // You need to define Safe_Value
}

// In Sensor.cpp
float Sensor1::get_last_reading() const {
    return last_reading;
}
// In Sensor.cpp
void Sensor1::set_last_reading(float reading) {
    last_reading = reading;
}





