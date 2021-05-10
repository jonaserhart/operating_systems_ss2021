#ifndef MY_MUTEX_H
#define MY_MUTEX_H

#include <stdatomic.h>

typedef atomic_flag my_mutex;

/**
 * my_mutex_lock(my_mutex* m)
 * locks a my_mutex_object
 **/
#define my_mutex_lock(m) while (atomic_flag_test_and_set(m))

/**
 * my_mutex_unlock(my_mutex* m)
 * unlockslocks a my_mutex_object
 **/
#define my_mutex_unlock(m) atomic_flag_clear(m)

#endif