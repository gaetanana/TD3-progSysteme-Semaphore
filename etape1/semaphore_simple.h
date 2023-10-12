#ifndef __SEM_SIMPLE__
#define __SEM_SIMPLE__

#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

int sem_initialiser(sem_t *sem, unsigned int value);
int sem_p(sem_t *sem);
int sem_v(sem_t *sem);
int sem_detruire(sem_t *sem);

#endif
