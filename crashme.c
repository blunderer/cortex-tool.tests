
/*
 * Test crash program
 * args:
 *  - generate SIGFPE, SIGSEGV
 *  - select number of threads
 *  - link to libraries
 */

#include <dlfcn.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

enum crashme_type {
	CRASHME_SIGFPE = 0,
	CRASHME_SIGSEGV,
	CRASHME_NOT,
};

int crashme_sigsegv(void)
{
	int *a = 0;
	*a = 42;
	return *a;
}

int crashme_sigfpe(void)
{
	int a = 0;
	int b = 42;
	int c = b/a;
	return c;
}

void *crashme(void *t)
{
	enum crashme_type action = *(int*)t;
	int delay_us = 1000000.f * (rand() / (float)RAND_MAX);

	/* wait between 1 and 2 seconds then crash */
	printf("sleep for %dus\n", delay_us+1000000);
	usleep(1000000);
	usleep(delay_us);

	switch(action) {
	case CRASHME_SIGFPE:
		crashme_sigfpe();
		break;
	case CRASHME_SIGSEGV:
		crashme_sigsegv();
		break;
	case CRASHME_NOT:
	default:
		break;
	}

	return NULL;
}

int main(int argc, char **argv)
{
	int i = 0;
	int iargs = 1;
	int threads = 0;
	int library = 0;

	pthread_t th;
	enum crashme_type action = CRASHME_SIGSEGV;
	void *handle = NULL;

	srand(time(NULL));

	while(iargs < argc) {
		if(strcmp(argv[iargs], "-h") == 0) {
			printf("crashme: %s [-t] [-a] [-l]\n", argv[0]);
			exit(0);
		} else if(strcmp(argv[iargs], "-t") == 0) {
			iargs++;
			threads = atoi(argv[iargs]);
		} else if(strcmp(argv[iargs], "-a") == 0) {
			iargs++;
			action = atoi(argv[iargs]);
		} else if(strcmp(argv[iargs], "-l") == 0) {
			iargs++;
			library = atoi(argv[iargs]);
		}
		iargs++;
	}

	if(library) {
		handle = dlopen("libxml2.so.2.8.0", RTLD_NOW);
	}

	for(i = 0; i < threads; i++) {
		pthread_create(&th, NULL, crashme, (void*)&action);
	}

	crashme(&action);

	if(library) {
		dlclose(handle);
	}

	return 0;
}


