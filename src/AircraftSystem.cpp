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

static bool mutex = false;

int main(int argc, char *argv[]) {

	CRT crtDisplay;
	int writer_period_sec=1;
	int writer_period_msec=0;
	int offset1 = 4;
	int offset2 = 8;

	//SmokeDetector smoke_one(&mutex);

	//maybe the sensor should call the Dial from whithin the Sensor class, idk if it's better or not
	Sensor Pressure("Pressure",writer_period_sec,writer_period_msec, Max_Ran_Value_Pressure,0,&mutex);
	Dial Pressure_dial("Pressure",writer_period_sec,writer_period_msec,Safe_Value_Pressure,0,&crtDisplay);

	Sensor Temperature("Temperature",writer_period_sec,writer_period_msec,Max_Ran_Value_Temperature,offset1,&mutex);
	Dial Temperature_dial("Temperature",writer_period_sec,writer_period_msec,Safe_Value_Temperature,offset1,&crtDisplay);

	Sensor Fuel("Fuel",writer_period_sec,writer_period_msec,Max_Ran_Value_Pressure,offset2,&mutex);
	Dial Fuel_dial("Fuel",writer_period_sec,writer_period_msec,Safe_Value_Fuel,offset2,&crtDisplay);



	// Inside a loop or wherever you update your sensor readings
		// Assuming you have an instance of Dial named 'Pressure_dial'
	std::string lampStatus = Pressure_dial.getLampStatus();
	std::string lampStatus1 = Temperature_dial.getLampStatus();
	std::string lampStatus2 = Fuel_dial.getLampStatus();
	crtDisplay.setLampStatus("Pressure", lampStatus);
	crtDisplay.setLampStatus("Temperature", lampStatus1);
	crtDisplay.setLampStatus("Fuel", lampStatus2);// Assuming CRT has a method to accept this information

		// Then display the CRT interface
	//crtDisplay.displayInterface( writer_period_sec, writer_period_msec );


	pthread_join(Pressure.thread_id,NULL);
	pthread_join(Pressure_dial.thread_id,NULL);

	pthread_join(Temperature.thread_id,NULL);
	pthread_join(Temperature_dial.thread_id,NULL);

	pthread_join(Fuel.thread_id,NULL);
	pthread_join(Fuel_dial.thread_id,NULL);

	//pthread_join(smoke_one.thread_id,NULL);



	return EXIT_SUCCESS;
}
