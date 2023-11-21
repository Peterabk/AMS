#include <cstdlib>
#include <iostream>
#include "Writer.h"
#include "Reader.h"
#include "Sensor.h"

#define Max_Ran_Value_Pressure int(100)
#define Max_Ran_Value_Temperature int(50)
#define Safe_Value_Pressure float()
#define Safe_Value_Temperature float()


int main(int argc, char *argv[]) {

	// Init Writer
	int writer_period_sec=1;
	int writer_period_msec=0;
	Sensor Pressure(writer_period_sec,writer_period_msec, Max_Ran_Value_Pressure);
	Sensor Temperature(writer_period_sec,writer_period_msec,Max_Ran_Value_Temperature);



	pthread_join(Pressure.thread_id,NULL);
	pthread_join(Temperature.thread_id,NULL);
	return EXIT_SUCCESS;
}
