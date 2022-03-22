
#include "../Implementari/my_locks.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>

#define nthr 15

my_semaphore sem;
my_mutex mtx;

int tcount=0;

int barrier_point()
{
	my_mutex_lock(&mtx);

	tcount++;

	if(tcount==nthr)
	{
		int i;
		my_semaphore_post(&sem);
	}

	my_mutex_unlock(&mtx);

	my_semaphore_wait(&sem);

	my_semaphore_post(&sem);

	return 0;
}

void* tfun (void* v)
{
	int* tid=(int*)v;

	int aux=(*tid)*1000000;
	while(aux--);

	printf("%d reached the barrier.\n", *tid);
	barrier_point();
	printf("%d passed the barrier.\n", *tid);

	return NULL;
}

int main()
{
	my_semaphore_init(&sem,0);

	my_mutex_init(&mtx);

	pthread_t thr[nthr];
	int thrid[nthr];

	int i;
	for(i=0;i<nthr;++i)
	{
		thrid[i]=i;
		if(pthread_create(&thr[i], NULL, tfun, &thrid[i]))
		{
			perror(NULL);
			my_mutex_destroy(&mtx);
			my_semaphore_destroy(&sem);
			return errno;
		}
	}

	for(i=0;i<nthr;++i)
	{
		if(pthread_join(thr[i],NULL))
		{
			perror(NULL);
			my_mutex_destroy(&mtx);
			my_semaphore_destroy(&sem);
			return errno;
		}
	}

	my_mutex_destroy(&mtx);
	my_semaphore_destroy(&sem);

	return 0;
}
