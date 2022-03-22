#ifndef __MY_LOCKS_H
#define __MY_LOCKS_H

#include <stdatomic.h>
#include <stdbool.h>


//=================================================================================== MUTEX

typedef struct { 
	atomic_bool locked; // true: locked; false: unlocked
} my_mutex;

void my_mutex_init (my_mutex* mtx);
void my_mutex_destroy (my_mutex* mtx);
void my_mutex_lock (my_mutex* mtx);
void my_mutex_unlock (my_mutex* mtx);

//=================================================================================== SEMAPHORE

typedef struct {
	atomic_uint counter; // cate valori pot trece de semafor
	my_mutex mtx; // mutex pentru modificarea lui counter 
} my_semaphore;

void my_semaphore_init(my_semaphore* sem, const unsigned int value);
void my_semaphore_destroy(my_semaphore* sem);
void my_semaphore_wait(my_semaphore* sem);
void my_semaphore_post(my_semaphore* sem);

//=================================================================================== READ-WRITE-LOCKS

typedef struct{
	atomic_uint read_counter; // cati cititori se afla in zona critica
	my_mutex read_mtx; // mutex pentru modificarea lui read_counter
	my_mutex rw_mtx; // mutex pentru accesarea zonei critice (fie de cititori, fie de un scriitor)
} my_read_write_lock;

void my_rw_init (my_read_write_lock* rwl);
void my_rw_destroy(my_read_write_lock* rwl);
void my_rw_rlock(my_read_write_lock* rwl);
void my_rw_runlock(my_read_write_lock* rwl);
void my_rw_wlock (my_read_write_lock* rwl);
void my_rw_wunlock (my_read_write_lock* rwl);

#endif
