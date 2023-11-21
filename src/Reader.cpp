
#include "Reader.h"


Reader::Reader(int period_sec, int period_msec) {

	this->period_sec=period_sec;
	this->period_msec=period_msec;
	initialize_reader();

	//printf("here");
}

void* reader_start_routine(void *arg){

	Reader& reader = *(Reader*) arg;
	reader.read();
	return NULL;
}

void Reader::initialize_reader(){

	/* open the shared memory segment */
	shm_fd = shm_open(name, O_RDONLY, 0666);
	if (shm_fd == -1) {
		perror("in shm_open reader()");
		exit(1);
	}

	/* now map the shared memory segment in the address space of the process */
	ptr = mmap(0,SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
	if (ptr == MAP_FAILED) {
		perror("in mmap()");
		exit(1);
	}

	pthread_mutex_init(&mutex, NULL);

	if(pthread_create(&thread_id,NULL,reader_start_routine,(void *) this)!=EOK){
		thread_id=NULL;
	}
}

Reader::~Reader() {
	pthread_mutex_destroy( &mutex );
	thread_id = NULL;

	if (shm_unlink(name) == -1) {
		perror("in shm_unlink()");
		exit(1);
	}


}

void Reader::read(){


	cTimer timer(period_sec,period_msec);

	static int n=0;


	int num;

	int* iptr=(int*)ptr;

	while(n<15){

		/* now read from the shared memory region */

		pthread_mutex_lock(&mutex);

		printf("The reader read %d from the location %p \n", *(iptr+n), iptr+n);
		printf("\n");

		pthread_mutex_unlock(&mutex);
		n++;

		timer.waitTimer();
	}
	/* remove the shared memory segment */

}
