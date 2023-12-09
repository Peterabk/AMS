#include <cstdlib>
#include <iostream>
#include "Writer.h"
#include "Reader.h"
#include "Sensor.h"
#include "Dial.h"
#include "SmokeDetector.h"

#define Max_Ran_Value_Pressure int(100)
#define Max_Ran_Value_Temperature int(50)
#define Safe_Value_Pressure float(50)
#define Safe_Value_Temperature float(25)
#define Safe_Value_Fuel float(10)

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t condition = PTHREAD_COND_INITIALIZER;

int main(int argc, char *argv[]) {

	// Init Writer
	int writer_period_sec=1;
	int writer_period_msec=0;
	int offset1 = 4;
	int offset2 = 8;

	SmokeDetector smoke_one(mutex,condition);

	//maybe the sensor should call the Dial from whithin the Sensor class, idk if it's better or not
	Sensor Pressure("Pressure",writer_period_sec,writer_period_msec, Max_Ran_Value_Pressure,0);
	Dial Pressure_dial("Pressure",writer_period_sec,writer_period_msec,Safe_Value_Pressure,0);

	Sensor Temperature("Temperature",writer_period_sec,writer_period_msec,Max_Ran_Value_Temperature,offset1);
	Dial Temperature_dial("Temperature",writer_period_sec,writer_period_msec,Safe_Value_Temperature,offset1);

	Sensor Fuel("Fuel",writer_period_sec,writer_period_msec,Max_Ran_Value_Pressure,offset2);
	Dial Fuel_dial("Fuel",writer_period_sec,writer_period_msec,Safe_Value_Fuel,offset2);


	pthread_join(smoke_one.thread_id,NULL);

	pthread_join(Pressure.thread_id,NULL);
	pthread_join(Pressure_dial.thread_id,NULL);

	pthread_join(Temperature.thread_id,NULL);
	pthread_join(Temperature_dial.thread_id,NULL);

	pthread_join(Fuel.thread_id,NULL);
	pthread_join(Fuel_dial.thread_id,NULL);



	return EXIT_SUCCESS;
}
