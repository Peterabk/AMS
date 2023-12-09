/*
 * SmokeDetector.cpp
 *
 *  Created on: Dec 7, 2023
 *      Author: peter
 */

#include "SmokeDetector.h"
#include "Sensor.h"

SmokeDetector::SmokeDetector(pthread_mutex_t m ,pthread_cond_t c) {
	// TODO Auto-generated constructor stub
	this->mutex = m;
	this->condit = c;
	initialize_smokedetector();

}

SmokeDetector::~SmokeDetector() {
	// TODO Auto-generated destructor stub
}

void* start_routine_smoke(void* arg){
	SmokeDetector& smoke = *(SmokeDetector*) arg;
	smoke.smokeInterrupt(smoke);
	return NULL;
}

void SmokeDetector::initialize_smokedetector(){

	if(pthread_create(&thread_id,NULL,start_routine_smoke,(void *) this)!=EOK){
		thread_id=NULL;
	}

}

//void SmokeDetector::signalHandler(int signum ) {
//   std::cout << "Interrupt signal (" << signum << ") received.\n";
//
//   // cleanup and close up stuff here
//   // terminate program
//
//   exit(signum);
//}

void SmokeDetector::signalHandler(union sigval sig) {
    // Signal to sensor1 and sensor3 using SIGEV_SIGNAL_THREAD
    pthread_mutex_lock(&mutex);
    pthread_cond_signal(&condit);
    pthread_mutex_unlock(&mutex);
}

void SmokeDetector::smokeInterrupt(SmokeDetector smoke){

	cTimer timer(1,0);

	//int *iptr=(int*)ptr;
	int i=0;
	//signal( sigev_signo = SIGEV_SIGNAL ,signalHandler);
	//memset (iptr, 0, sizeof(float));

	SIGEV_SIGNAL_THREAD_INIT(&sig_event,SIGEV_SIGNAL_THREAD,NULL,0);
	sig_event.__sigev_un1.__sigev_notify_function = signalHandler ;

	while(++i){
		if(i>3){
			raise(SIGEV_SIGNAL);
			break;
		}
		std::cout << "Going to sleep.... \n";


	timer.waitTimer();
	}
}



