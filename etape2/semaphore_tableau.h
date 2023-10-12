#ifndef __SEM_TAB__
#define __SEM_TAB__

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>

int semtab_init(int *semid, key_t key, unsigned int value); //: Initialise un sémaphore avec une valeur initiale.

int semtab_p(int semid); //: Effectue l'opération P (wait) sur le sémaphore (d'indice 0).

int semtab_v(int semid); //: Effectue l'opération V (signal) sur le sémaphore (d'indice 0).

int semtab_p_n(int semid, int n); //: Effectue l'opération P (wait) sur le sémaphore (d'indice 0) pour consommer n jetons.

int semtab_v_n(int semid, int n); //: Effectue l'opération V (signal) sur le sémaphore (d'indice 0) pour libérer n jetons.

int semtab_destroy(int semid); //: Détruit un sémaphore.

#endif
