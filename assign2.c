/*
 * example.c
 *
 *  Created on: Feb 18, 2014
 *      Author: Samiam
 */
#include "assign2.h"

/*global vars */
//volatile int stop = 0;
pthread_mutex_t lock;
pthread_mutex_t ops_lock;
queue q;
global_info q_info;

int main(int argc, char *argv[]) {

	/* information about thread count, number created so far, and duration for execution */
	int num_threads = 0;
	int num_created = 0;
	int duration = 0;
	int created_time;
	int end_time;
	pthread_t * threads;
	int s;

	//-------- INITIALIZING QUEUE AND LOCKING STRUCTURES ------------------//

	/* zero out the queue */
	memset(&q_info, 0, sizeof(q_info));
//	memset(&q, 0, sizeof(q));

	if (pthread_mutex_init(&lock, NULL) != 0) {
		handle_error("mutex init failed");
		return 1;
	}

	if (pthread_mutex_init(&ops_lock, NULL) != 0) {
		handle_error("mutex init failed");
		return 1;
	}

	//----------------- PARSING CMDLN INPUT -------------------------//
	if (argc != 5) /* Test for correct number of arguments */
	{
		fprintf(stderr, "Usage:  %s <-p PROCESS_NUMBER -t RUNTIME>\n", argv[0]);
		exit(1);
	}

	num_threads = atoi(argv[2]); /* First arg:  number threads */
	duration = atoi(argv[4]);/* Second arg:  duration of execution */
	threads = malloc(sizeof(pthread_t) * num_threads);

	created_time = (int) time(NULL);

	if (threads == NULL)
		handle_error("malloc of thread array");

	for (num_created = 0; num_created < num_threads; num_created++) { /* then spawn threads */

		s = pthread_create(&threads[num_created], NULL, ThreadMain, NULL);

		if (s != 0) {
			handle_error ("Create pthread error!\n");
			exit(1);
		}

	}

	sleep(duration);

	for (num_created = 0; num_created < num_threads; num_created++) { /* then cancel threads */
		s = pthread_cancel(threads[num_created]);
		if (s != 0) {
			handle_error ("Cancel pthread error!\n");
			exit(1);
		}

		//		free(threads[num_created]);
	}

	free(threads);
	pthread_mutex_destroy(&lock);
	pthread_mutex_destroy(&ops_lock);

	end_time = (int) time(NULL);
	printf("\nAlarm sounded after %d seconds. Should be after %d seconds\n",
			(end_time - created_time), duration);

	//---------------- WRITING TO FILE -----------------------------//
	FILE *f = fopen("file.txt", "a");
	if (f == NULL) {
		handle_error("Error opening file!\n");
	}

	printf("#Êof threads=%dÊtimeÊ(seconds)=%dÊtotalÊnumberÊofÊoperations=%d\n", num_threads, duration, q_info.no_ops);

	fprintf(f,"#ÊofÊthreads=%dÊtimeÊ(seconds)=%dÊtotalÊnumberÊofÊoperations=%d\n", num_threads, duration, q_info.no_ops);
	fprintf(f,"%d, %d\n", q_info.no_ops, num_threads);

	fclose(f);
	exit(0);

}

void *ThreadMain(void *threadArgs) {

	pthread_detach(pthread_self());
	int val = DoOperation();

	if (val != 1) {
		handle_error("Bad DoOperation");
	}
	return (NULL);
}

int DoOperation() {

	operation_t myops;
	generator_state_t *state = NULL;
	while (load_generator(&myops, &state)) {
		pthread_mutex_lock(&lock);
		//these things should be filled now
		if (q.head > q.tail) {
//			handle_error("Head is passed tail");
			//return 0;
		}

		if (myops.operation == 1) { //enque value
			//add value at tail
			q.content[q.tail] = myops.value;
			//increment the tail
			q.tail = q.tail + 1;
		} else if (myops.operation == 0) {
			//dequeue
			q.head = q.head + 1;
		} else {
//			handle_error("Invalid operation type");
			//return 0;
		}
		pthread_mutex_unlock(&lock);
	}
	return 1; //todo: not sure about this...

}
//Example
int load_generator(operation_t *op, generator_state_t **s) {
	pthread_mutex_lock(&ops_lock);
	q_info.no_ops = q_info.no_ops + 1;

	if (*s == NULL) {
		*s = (generator_state_t*) malloc(sizeof(generator_state_t)); //initialize state
		//not sure how to deal with state!!?
		//		**s = **s + 1;
	}

	int random_int = rand();
	op->value = random_int;

	int val = (random_int % 2);
	if ((val == 0) && (q_info.no_add > q_info.no_remove)) {
		op->operation = 0;
		q_info.no_remove = q_info.no_remove + 1;
	} else {
		op->operation = 1;
		q_info.no_add = q_info.no_add + 1;
	}

	pthread_mutex_unlock(&ops_lock);
	return 1; //1 for success, 0 for fail
}

int whatToDo() {
	// use the function as follows
	operation_t myops;
	generator_state_t *state = NULL;
	while (load_generator(&myops, &state)) {
		//do stuff
	}
	return 1;

}


