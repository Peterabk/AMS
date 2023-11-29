#include <cstdlib>
#include <iostream>
#include "Writer.h"
#include "Reader.h"
#include "Sensor.h"
#include "Sensor1.h"
#include "Sensor2.h"
#include "Dial.h"


#define Max_Ran_Value_Pressure int(100)
#define Max_Ran_Value_Temperature int(50)
#define Safe_Value_Pressure float(10) // Adjust this value as needed
#define Safe_Value_Temperature float(10) // Adjust this value as needed

int main(int argc, char *argv[]) {
    int writer_period_sec = 1;
    int writer_period_msec = 0;


    Sensor Pressure(writer_period_sec, writer_period_msec, Max_Ran_Value_Pressure);
   Sensor1 Temperature(writer_period_sec, writer_period_msec, Max_Ran_Value_Temperature);
   Sensor2 Fuel(writer_period_sec, writer_period_msec, 20);

    // Initialize Dial
    Dial dial(writer_period_sec, writer_period_msec,&Pressure,&Temperature,&Fuel);


    // Start Dial thread
    pthread_create(dial.getThreadID(), nullptr, Dial::start_routine, &dial);



    // Allow threads to run independently
    sleep(10);  // Adjust the sleep duration based on your requirements

    // Join threads when you're ready to exit
    pthread_join(Pressure.thread_id, nullptr);

   pthread_join(Temperature.thread_id, nullptr);

   pthread_join(Fuel.thread_id, nullptr);
    pthread_join(*(dial.getThreadID()), nullptr);



    return EXIT_SUCCESS;
}
