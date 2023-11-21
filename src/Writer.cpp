
#include "Writer.h"



Writer::Writer(int period_sec, int period_msec) {

	this->period_sec=period_sec;
	this->period_msec=period_msec;
	initialize_writer();


	//printf("here");
}

void* writer_start_routine(void *arg){

	Writer& writer = *(Writer*) arg;
	writer.wwrite();
	return NULL;
}

void Writer::initialize_writer(){
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

	if(pthread_create(&thread_id,NULL,writer_start_routine,(void *) this)!=EOK){
		thread_id=NULL;
	}
}


Writer::~Writer() {
	shm_unlink(name);
	pthread_mutex_destroy( &mutex );
	thread_id = NULL;
}

void Writer::wwrite(){
	static int n=0;
	cTimer timer(period_sec,period_msec);


	int *iptr=(int*)ptr;

	memset (iptr, 0, 15*sizeof(int));

	while(n<15){


		pthread_mutex_lock(&mutex);


		printf("The writer wrote: %d at the location %p \n",n,iptr+n);
		*(iptr+n)=n;

		pthread_mutex_unlock(&mutex);


		n++;
		timer.waitTimer();
	}
}
