#include <stdio.h>      /* for printf() and fprintf() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */
#include <pthread.h> /* for threads */
#include <signal.h>
#include <unistd.h>

/* Structure of arguments to pass to client thread */
typedef struct thread_args {
	int created_time; /* Socket descriptor for client */
	char *string; /* From command-line argument */

} thread_args;

typedef struct thread_info { /* Used as argument to thread_start() */
	pthread_t thread_id; /* ID returned by pthread_create() */
	int thread_num; /* Application-defined thread # */
	thread_args args;
} thread_info;

typedef struct queue_operation {
	int operation; //1 to insert, 0 to remove
	int value; //value should be ignored if operation = 0
} operation_t;

typedef struct generator_state {
	int state[4]; //for internal use
} generator_state_t;

typedef struct global_info{
	int no_ops;
	int no_add;
	int no_remove;
} global_info;

typedef struct queue {
	int head;
	int tail;
	int content[2000000000];
} queue;

/*
 * so you know whats going on
 */
//#define handle_error_en(en, msg) do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

#define handle_error(msg) \
               do { perror(msg); exit(EXIT_FAILURE); } while (0)
int AcceptTCPConnection(int servSock); /* Accept TCP connection request */
void *ThreadMain(void *arg); /* Main program of a thread */
void sigalrm_handler(int sig);
int DoOperation();
