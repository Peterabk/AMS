
#include "Dial.h"
#include "CRT.h"

Dial::Dial(std::string name, int period_sec, int period_msec, float Safe_Value, int offset, CRT* crt)
    : crtDisplay(crt) {

	this->period_sec=period_sec;
	this->period_msec=period_msec;
	this->Dial_name = name;
	this->safe_value = Safe_Value;
	this->map_offset = offset;
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
	ptr = mmap(0,SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, map_offset);
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

		//printf("The reader %s read %f from the location %p \n", dial.Dial_name ,*(iptr), iptr);
		manage_ofr_queue(iptr,dial.Dial_name);
		*iptr = 0;
		printf("\n");

		pthread_mutex_unlock(&mutex);

		timer.waitTimer();
	}
	/* remove the shared memory segment */

}
std::string Dial::getLampStatus()  {
    return lamp.Lamp_status; // Assuming `lamp` is a member of `Dial` and `Lamp_status` is a member of `lamp`.
}



void Dial::manage_ofr_queue(float* ptr_value, std::string dialname){
	static int overSafeValueCountPressure = 0;
	    static int overSafeValueCountTemperature = 0;
	    bool statusChanged = false;
	    bool statusWarning = false;

	    if(Dial_name == "Pressure") {
	        if(*ptr_value < safe_value) {
	            overSafeValueCountPressure++;
	            if(overSafeValueCountPressure >= 3 && lamp.Lamp_status != "RED") {
	                lamp.Lamp_status = "RED";
	                statusChanged = true;
	                statusWarning = true;
	            }
	        } else {
	            if(lamp.Lamp_status == "RED") {
	                lamp.Lamp_status = "GREEN";
	                statusChanged = true;
	            }
	            overSafeValueCountPressure = 0;
	        }
	    } else if(Dial_name == "Temperature") {
	        if(*ptr_value < safe_value) {
	            overSafeValueCountTemperature++;
	            if(overSafeValueCountTemperature >= 3 && lamp.Lamp_status != "RED") {
	                lamp.Lamp_status = "RED";
	                statusChanged = true;
	                statusWarning = true;
	            }
	        } else {
	            if(lamp.Lamp_status == "RED") {
	                lamp.Lamp_status = "GREEN";
	                statusChanged = true;
	            }
	            overSafeValueCountTemperature = 0;
	        }
	    }
	    else if(Dial_name=="Fuel"){
    // If the read value is lower than the safe value then change status to RED
    if(*ptr_value <= safe_value && lamp.Lamp_status == "GREEN"){
        printf("The Engine-%s Lamp is RED\n", dialname);
        lamp.Lamp_status = "RED";
        statusChanged = true;
        statusWarning = true;
    }
    // If the value is back in the safe zone and the lamp is RED, change it to GREEN
    else if(*ptr_value >= safe_value && lamp.Lamp_status == "RED"){
        printf("The Engine-%s Lamp is GREEN\n", dialname);
        lamp.Lamp_status = "GREEN";
        statusChanged = true;
        statusWarning = true;
    }}
    // Update CRT display if there's a change in status
    if(statusChanged && crtDisplay != nullptr){
        crtDisplay->updateLampStatus(dialname, lamp.Lamp_status);
    }
    // warning crt
    if(statusChanged && crtDisplay != nullptr){
           crtDisplay->updateWarningStatus(dialname,statusWarning );
       }
    //dials value
    if(crtDisplay != nullptr){
          crtDisplay->updateDialStatus(dialname, *ptr_value);
      }

}


bool Dial::is_queue_ofr(){
	if(ofr_value.size() > 2){
		return true;
	}
	return false;
}
