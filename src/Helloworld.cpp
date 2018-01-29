//============================================================================
// Name        : Helloworld.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <leveldb/db.h>
#include <boost/random.hpp>
#include <sys/time.h>
#include <signal.h>
#include <pthread.h>

#define MySig 64

void sig_handler(int sig) {
//	printf("receive : %d,thread id :%d", sig, pthread_self());
	fprintf(stdout, "receive : %d,thread id :%ld\n", sig, pthread_self());
	int i = 0;
	raise(MySig);
	while (i < 5) {
		fprintf(stdout, "%d\n", i);
		sleep(1);
		i++;
	}
}

void *function_pthread(void *arg) {
	int i = 1;
	while (i--) {
		sleep(1);
		fprintf(stdout, "send Sig\n");
		printf("current thread:%ld\n", pthread_self());
		raise(MySig);
	}

	return NULL;
}

int main(int argc, char **argv) {
	struct sigaction sa;
	sa.sa_handler = sig_handler;
	sa.sa_flags = SA_NODEFER;
	sigaction(MySig, &sa, NULL);

	printf("current thread:%ld\n", pthread_self());
	pthread_t pthread_id;
	pthread_create(&pthread_id,NULL,function_pthread,NULL);

	while(1) sleep(5);
	return 0;
}

