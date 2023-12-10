
#include "Sensor.h"
#include <cstdlib>


Sensor::Sensor(std::string name, int period_sec, int period_msec, int upper_bound, int offset, bool *lock) {

	this->period_sec=period_sec;
	this->period_msec=period_msec;
	this->upper_bound = upper_bound;
	this->th_name = name;
	this->map_offset = offset;
	this->generallock = lock;
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
	ptr = mmap(0,SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, map_offset);
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

	float *iptr=(float*)ptr;
	memset (iptr, 0, sizeof(float));
	while(!(*generallock)){

		pthread_mutex_lock(&mutex);
		if(th_name=="Fuel"){
		*iptr = sensor.generate_random();
		}
		else if (th_name=="Pressure"||th_name=="Temperature"){
			*iptr = sensor.generate_random1();}
		pthread_mutex_unlock(&mutex);

		timer.waitTimer();
	}
}

Sensor::~Sensor(){
	shm_unlink(name);
	pthread_mutex_destroy( &mutex );
	thread_id == NULL;
}

float Sensor::generate_random() {
    static float fuelLevel = 40.0; // Gallons, starts at 100
    static auto lastTime = std::chrono::steady_clock::now();

    // Only modify the fuel level for the Fuel sensor
    if (th_name == "Fuel") {
        auto currentTime = std::chrono::steady_clock::now();
        std::chrono::duration<float> elapsed = currentTime - lastTime;

        // Convert elapsed time to seconds and decrease fuel
        float decreaseAmount = 5.0 * elapsed.count(); // 5 liters per second
        fuelLevel -= decreaseAmount / 3.78541; // Convert liters to gallons (1 gallon = 3.78541 liters)

        // Update the last time
        lastTime = currentTime;

        // Prevent fuel level from going negative
        if (fuelLevel < 0) {
            fuelLevel = 0;
        }
    }

    return fuelLevel;
}
float Sensor::generate_random1() {
    return rand() % upper_bound;;
}






void Sensor::sayhello(){
	while(true){
		std::cout<<"HELLO \n";
	}
}

//we need to add a function that will compare the sensor reading to the safe sensor reading
//each time the sensor is being read
//and if it the comparison is above safe value three consecutive times in a row, the red light pops on



