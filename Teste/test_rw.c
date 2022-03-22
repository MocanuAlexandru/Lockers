
#include "../Implementari/my_locks.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>

int numere[1000001],nr[1000001];
my_read_write_lock rw_lock;

void* search(void* v)
{
  int  nr = *((int*)v);	//numarul pe care il cauta in vector
  while(1)
	{
	  int aux =nr*1000000;
	  while(aux--);

	  my_rw_rlock(&rw_lock);
	  printf("Caut numarul %d\n",nr);
	  bool flag=false;	//nu l-a gasit
		for(int i=1;i<=numere[0] && !flag; i++)
			if(numere[i]==nr) flag=true;	//a gasit
	  my_rw_runlock(&rw_lock);
	  if(flag) break;	
	}
  printf("Am gasit numarul %d.\n",nr);  
  return NULL;
}


int main()
{
  time_t t;
  srand((unsigned) time(&t));
  const int n = 50;	// main = writer: scrie numerele de la 1 la n intr-un vector
  const int k = 10;	// k thread-uri = readers: parcurg vectorul, cautand un numar in el
  int i;

	my_rw_init(&rw_lock);    

  for(i=0;i<k;++i)
		nr[i]=rand()%n + 1; // numarul cautat de thread-ul i

  pthread_t thr[k];
  numere[0]=0;	//initial, vectorul este gol

  for(i=0;i<k;++i)
  {
		if(pthread_create(&thr[i], NULL, search, &nr[i]))
		{
			perror(NULL);
			my_rw_destroy(&rw_lock);
			return errno;
		}
	}

  for(i=1;i<=n;++i)
	{
	   int aux = 1000000;
	   while(aux--);

	   my_rw_wlock(&rw_lock);
	   printf("Am scris numarul %d\n",i);	//main = writer
	   numere[0]++;
	   numere[numere[0]]=i;	//scrie in vector numarul i
	   my_rw_wunlock(&rw_lock);
	}

	for(i=0;i<k;++i)
	{
		if(pthread_join(thr[i],NULL))
		{
			perror(NULL);
			my_rw_destroy(&rw_lock);
			return errno;
		}
	}
	
	my_rw_destroy(&rw_lock);	

  return 0;		
}
