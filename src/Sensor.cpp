
#include "Sensor.h"
#include <cstdlib>


Sensor::Sensor(int period_sec, int period_msec, int upper_bound) {

	this->period_sec=period_sec;
	this->period_msec=period_msec;
	this->upper_bound = upper_bound;
	initialize_sensor();

	//printf("here");
}

void* start_routine(void* arg){
	Sensor& sensor = *(Sensor*) arg;
	sensor.sensorpoll(sensor);
	return NULL;
}

void Sensor::initialize_sensor(){
	//create a function that generates a random value between a certain range
	//then send this as the sensor poll value
	//to be later on compared in the dial to the better values
	//we can have the equilibrium value pre defined #define
	//and we can also have the range pre defined #define for all sensors
	//we either pass them as argumetns or use them as known knowledge


	if(pthread_create(&thread_id,NULL,start_routine,(void *) this)!=EOK){
		thread_id=NULL;
	}

}

void Sensor::sensorpoll(Sensor sensor){

	cTimer timer(period_sec,period_msec);
	int n = 0;
	while(true){

		printf("The Sensor did this : %f at the location \n",sensor.generate_random());

		timer.waitTimer();
	}
}

Sensor::~Sensor(){
	thread_id == NULL;
}

float Sensor::generate_random(){
	return rand() % upper_bound;
}

//we need to add a function that will compare the sensor reading to the safe sensor reading
//each time the sensor is being read
//and if it the comparison is above safe value three consecutive times in a row, the red light pops on



