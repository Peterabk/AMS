
#include "Sensor.h"
#include <cstdlib>


Sensor::Sensor(std::string name, int period_sec, int period_msec, int upper_bound) {

	this->period_sec=period_sec;
	this->period_msec=period_msec;
	this->upper_bound = upper_bound;
	this->th_name = name;
	initialize_sensor();

	//printf("here");
}

void* start_routine(void* arg){
	Sensor& sensor = *(Sensor*) arg;
	sensor.sensorpoll(sensor);
	return NULL;
}

void Sensor::initialize_sensor(){

	/* Create a new memory object */
	// create the shared memory segment
	shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
	if (shm_fd == -1){
		perror("In shm_open() writer");
		exit(1);
	}

	// configure the size of the shared memory segment
	ftruncate(shm_fd, SIZE);

	// now map the shared memory segment in the address space of the process
	ptr = mmap(0,SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if (ptr == MAP_FAILED) {
		printf("Map failed\n");
	}


	pthread_mutex_init(&mutex, NULL);


	if(pthread_create(&thread_id,NULL,start_routine,(void *) this)!=EOK){
		thread_id=NULL;
	}

}

void Sensor::sensorpoll(Sensor sensor){

	cTimer timer(period_sec,period_msec);
	int n = 0;

	int *iptr=(int*)ptr;
	memset (iptr, 0, sizeof(int));

	while(true){

		pthread_mutex_lock(&mutex);
		*iptr = 10;
		printf("The %s Sensor did this : %f at the location %p \n",sensor.th_name, float(*iptr), iptr );
		pthread_mutex_unlock(&mutex);

		timer.waitTimer();
	}
}

Sensor::~Sensor(){
	shm_unlink(name);
	pthread_mutex_destroy( &mutex );
	thread_id == NULL;
}

float Sensor::generate_random(){
	return rand() % upper_bound;
}

//we need to add a function that will compare the sensor reading to the safe sensor reading
//each time the sensor is being read
//and if it the comparison is above safe value three consecutive times in a row, the red light pops on



