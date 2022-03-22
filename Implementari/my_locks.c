
#include "my_locks.h"

//=================================================================================== MUTEX

void my_mutex_init (my_mutex* mtx) //~~~~~~~~~~~~~~~~~~~ mtx init
{
	atomic_init (&(mtx->locked), false);
}

void my_mutex_destroy (my_mutex* mtx) //~~~~~~~~~~~~~~~~ mtx destroy
{
	atomic_store (&(mtx->locked), true);
}

void my_mutex_lock (my_mutex* mtx) //~~~~~~~~~~~~~~~~~~~ mtx lock
{
	atomic_bool expected;
	atomic_init (&expected, false);
	while (!atomic_compare_exchange_weak(&(mtx->locked), &expected, true))
		atomic_store (&expected, false);
}

void my_mutex_unlock (my_mutex* mtx) //~~~~~~~~~~~~~~~~~ mtx unlock
{
	atomic_store (&(mtx->locked), false);
}


//=================================================================================== SEMAPHORE

void my_semaphore_init(my_semaphore* sem, const unsigned int value)  //~ sem init
{
   atomic_init(&sem->counter,value);
   my_mutex_init(&sem->mtx);
}

void my_semaphore_destroy(my_semaphore* sem)  //~~~~~~~~~~~~~~~~~~~~~~~~ sem destroy
{
   atomic_store(&sem->counter,0);
   my_mutex_destroy(&sem->mtx);
}

void my_semaphore_wait(my_semaphore* sem) //~~~~~~~~~~~~~~~~~~~~~~~~~~~~ sem wait
{
   while(1)
	{
	   my_mutex_lock(&sem->mtx);
	   if(sem->counter>0)
		{
		   atomic_fetch_sub(&sem->counter,1);
		   my_mutex_unlock(&sem->mtx);
		   break;
		}
	   my_mutex_unlock(&sem->mtx);
	}
}

void my_semaphore_post(my_semaphore* sem) //~~~~~~~~~~~~~~~~~~~~~~~~~~~~ sem post
{
   my_mutex_lock(&sem->mtx);
   atomic_fetch_add(&sem->counter,1);
   my_mutex_unlock(&sem->mtx);
}


//=================================================================================== READ-WRITE-LOCKS

void my_rw_init (my_read_write_lock* rwl)  //~~~~~~~~~~~~~~~~~~~~~ rw init
{
	atomic_init (&(rwl->read_counter),0);
	my_mutex_init (&(rwl->read_mtx));
	my_mutex_init (&(rwl->rw_mtx));
}

void my_rw_destroy(my_read_write_lock* rwl)  //~~~~~~~~~~~~~~~~~~~ rw destroy
{
   my_mutex_destroy(&rwl->read_mtx);
   my_mutex_destroy(&rwl->rw_mtx);
}

void my_rw_rlock(my_read_write_lock* rwl) //~~~~~~~~~~~~~~~~~~~~~~ read lock
{

   my_mutex_lock(&rwl->read_mtx);
   if(rwl->read_counter==0) my_mutex_lock(&rwl->rw_mtx);
   atomic_fetch_add(&rwl->read_counter,1);
   my_mutex_unlock(&rwl->read_mtx);
}

void my_rw_runlock(my_read_write_lock* rwl)  //~~~~~~~~~~~~~~~~~~~ read unlock
{
   my_mutex_lock(&rwl->read_mtx);
   atomic_fetch_sub(&rwl->read_counter,1);
   if(rwl->read_counter==0) my_mutex_unlock(&rwl->rw_mtx);
   my_mutex_unlock(&rwl->read_mtx);
}

void my_rw_wlock (my_read_write_lock* rwl)  //~~~~~~~~~~~~~~~~~~~~ write lock
{
	my_mutex_lock (&(rwl->rw_mtx));
}

void my_rw_wunlock (my_read_write_lock* rwl)  //~~~~~~~~~~~~~~~~~~ write unlock
{
	my_mutex_unlock (&(rwl->rw_mtx));
}

