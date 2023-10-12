#include "semaphore_simple.h"
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>


int sem_initialiser(sem_t *sem, unsigned int value){
    return sem_init(sem, 0, value);
}
//Décrémente
int sem_p(sem_t *sem){
    return sem_wait(sem);
}
//Incrémente
int sem_v(sem_t *sem){
    return sem_post(sem);
}
int sem_detruire(sem_t *sem){
    return sem_destroy(sem);
}

int main(){
    int calcule = 0;
    sem_t sem;

    if (sem_initialiser(&sem, 1) != 0) {
        perror("Erreur lors de l'initialisation du semaphore");
        return 1;
    }

    if (sem_p(&sem) != 0) {
        perror("Erreur lors de l'attente du semaphore");
        return 1;
    }
    calcule = 1 + 2;
    printf("Resultat du calcul: %d\n", calcule);

    if (sem_v(&sem) != 0) {
        perror("Erreur lors de la signalisation du semaphore");
        return 1;
    }
    sem_destroy(&sem); //Detruit le semaphore

    return 0;
}