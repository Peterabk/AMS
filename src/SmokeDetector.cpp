/*
 * SmokeDetector.cpp
 *
 *  Created on: Dec 7, 2023
 *      Author: peter
 */

#include "SmokeDetector.h"
#include "Sensor.h"

SmokeDetector::SmokeDetector(pthread_mutex_t *m) {
	// TODO Auto-generated constructor stub
	this->mutex = m;
	pthread_mutex_init(mutex, NULL);
	//this->condit = *c;
	this->done = 2;
	initialize_smokedetector();

}

SmokeDetector::~SmokeDetector() {
	// TODO Auto-generated destructor stub
}

void* start_routine_smoke(void* arg){
	SmokeDetector& smoke = *(SmokeDetector*) arg;
	smoke.smokeInterrupt(&smoke);
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

void SmokeDetector::signalHandler() {
    // Signal to sensor1 and sensor3 using SIGEV_SIGNAL_THREAD
	if(done != 1){
		pthread_mutex_lock(mutex);
		std::cout << "Signal received....\n";
		std::cout << "Mutex locked....\n";
		std::cout << "Stopping....\n";
		std::cout << "\n";
		std::cout << "\n";
		std::cout << mutex;
		std::cout << "\n";
		std::cout << "\n";
		done = 1;
	}
	else if(done != 2){
		pthread_mutex_unlock(mutex);
		std::cout << "Signal received....\n";
		std::cout << "Mutex unlocked....\n";
		std::cout << "Starting....\n";
		std::cout << "\n";
		std::cout << "\n";
		std::cout << mutex;
		std::cout << "\n";
		std::cout << "\n";
		done = 2;
	}

//    pthread_cond_signal(&condit);
//    pthread_mutex_unlock(&mutex);
//	SmokeDetector& smoke = *(SmokeDetector*) sig.sival_ptr;
//	if(smoke.done == 1){
//		pthread_mutex_unlock(&(smoke.mutex));
//	}
//	else if(smoke.done == 2){
//		pthread_mutex_lock(&(smoke.mutex));
//	}
}

void SmokeDetector::smokeInterrupt(SmokeDetector *smoke){
	cTimer tm(5,0);
	//int *iptr=(int*)ptr;
	//int i=0;
	//signal( sigev_signo = SIGEV_SIGNAL ,signalHandler);
	//memset (iptr, 0, sizeof(float));
	int rcvid;
	pthread_mutex_setprioceiling(mutex,1,0);
	while(true){
		//std::cout << "REEEEEEEEE.... \n";
		//sig_event.sigev_notify = SIGEV_SIGNAL_THREAD;
		//sig_event.__sigev_un1.__sigev_notify_function = signalHandler;
		//SIGEV_SIGNAL_THREAD_INIT(&sig_event,SIGUSR1,smoke,0);
		//std::cout << "REEEEEEEEEp.... \n";

//		 if (SIGEV_SIGNAL_THREAD_INIT(&sig_event, SIGUSR1, smoke, 0) != 0) {
//		        std::cerr << "Error in SIGEV_SIGNAL_THREAD_INIT" << strerror(errno) << "\n";
//		        // Handle the error as appropriate
//		    }
//		 signal(SIGUSR1,signalHandler);
//		 raise(SIGUSR1);

		    std::cout << "Smoke Critical section .... \n";


		rcvid = MsgReceive(tm.channel_id, &(tm.msg_buffer), sizeof(tm.msg_buffer), NULL);
		if(rcvid != 0){
			std::cout<<"MESSAGE ERROR";
		}
		else{
			signalHandler();
		}

	}


}



