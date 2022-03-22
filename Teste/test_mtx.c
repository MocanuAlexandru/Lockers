
#include "../Implementari/my_locks.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>


#define max_resources 30

int available_resources=max_resources;
const int request_nr = 30;

my_mutex mtx;

int decrease_count (int count)
{
	
	my_mutex_lock(&mtx);
	
	if(max_resources<count){
		my_mutex_unlock(&mtx);
		return -2;
	}
	if(available_resources<count){
		my_mutex_unlock(&mtx);
		return -1;
	}

	available_resources-=count;
	printf("Got %d resources; %d remaining.\n", count, available_resources);
	my_mutex_unlock(&mtx);
	return 0;
}

int increase_count (int count)
{
	my_mutex_lock(&mtx);
	available_resources+=count;
	printf("Released %d resources; %d remaining.\n", count, available_resources);
	my_mutex_unlock(&mtx);
	return 0;
}

///====== suitable function for thread ================

void* get_and_release (void* v)
{
	int count=*((int*)v);
	int response;
	while((response=decrease_count(count))==-1);
	if(response==-2)
		return 0;

	int aux=1000000*count;
	while(aux--);

	increase_count(count);
	return 0;
}
///====================================================


int main()
{
	time_t t;
	srand((unsigned) time(&t));

	int i;
	int request[request_nr];
	for(i=0;i<request_nr;++i)
		request[i]=rand()%15+1;

	my_mutex_init(&mtx);

	printf("\nmax resources: %d\n\n", max_resources);

	pthread_t thr[request_nr];

	for(i=0;i<request_nr;++i)
	{
		if(pthread_create(&thr[i], NULL, get_and_release, &request[i]))
		{
			perror(NULL);
			my_mutex_destroy(&mtx);
			return errno;
		}
	}

	for(i=0;i<request_nr;++i)
	{
		if(pthread_join(thr[i],NULL))
		{
			perror(NULL);
			my_mutex_destroy(&mtx);
			return errno;
		}
	}
	
	printf("\n");
	my_mutex_destroy(&mtx);

	return 0;
}
