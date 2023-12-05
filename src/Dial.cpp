
#include "Dial.h"


Dial::Dial(std::string name, int period_sec, int period_msec, float Safe_Value) {

	this->period_sec=period_sec;
	this->period_msec=period_msec;
	this->Dial_name = name;
	this->safe_value = Safe_Value;
	//this->name = name;
	initialize_reader();

	//printf("here");
}

void* Dial_start_routine(void *arg){

	Dial& dial = *(Dial*) arg;
	dial.read(dial);
	return NULL;
}

void Dial::initialize_reader(){

	/* open the shared memory segment */
	shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
	if (shm_fd == -1) {
		perror("in shm_open reader()");
		exit(1);
	}

	/* now map the shared memory segment in the address space of the process */
	ptr = mmap(0,SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if (ptr == MAP_FAILED) {
		perror("in mmap()");
		exit(1);
	}

	pthread_mutex_init(&mutex, NULL);

	if(pthread_create(&thread_id,NULL, Dial_start_routine,(void *) this)!=EOK){
		thread_id=NULL;
	}
}

Dial::~Dial() {
	pthread_mutex_destroy( &mutex );
	thread_id = NULL;

	if (shm_unlink(name) == -1) {
		perror("in shm_unlink()");
		exit(1);
	}


}

void Dial::read(Dial dial){


	cTimer timer(period_sec,period_msec);

	float* iptr=(float*)ptr;

	while(true){
		/* now read from the shared memory region */
		pthread_mutex_lock(&mutex);

		printf("The reader %s read %f from the location %p \n", dial.Dial_name ,*(iptr), iptr);
		manage_ofr_queue(iptr,dial.Dial_name);
		*iptr = 0;
		printf("\n");

		pthread_mutex_unlock(&mutex);

		timer.waitTimer();
	}
	/* remove the shared memory segment */

}

void Dial::manage_ofr_queue(float* ptr_value, std::string dialname){
	//if the read value is bigger than the safe value then put it in the queue
	if(*ptr_value > safe_value && lamp.Lamp_status == "GREEN" ){
		ofr_value.push(*ptr_value);
		if(is_queue_ofr()){
			lamp.Lamp_status = "RED";
			printf("The Engine-%s Lamp is %s \n", dialname ,lamp.Lamp_status);
		}
	}
	//if the queue is not empty and the value is in the safe zone then delete the whole queue
	else if(!ofr_value.empty() && (*ptr_value < safe_value)){
		while(!ofr_value.empty()){
			ofr_value.pop();
		}
	}
}

bool Dial::is_queue_ofr(){
	if(ofr_value.size() > 2){
		return true;
	}
	return false;
}
