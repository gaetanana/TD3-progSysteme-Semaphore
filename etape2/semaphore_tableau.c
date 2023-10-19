// Inclusion des bibliothèques nécessaires
#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <unistd.h>
#include "./semaphore_tableau.h"

int semtab_init(int *semid, key_t key, unsigned int value) {
    *semid = semget(key, 1, IPC_CREAT | 0666);
    if (*semid == -1) {
        // Erreur pendant semget
        return -1;
    }

    // Initialiser la valeur du sémaphore
    if (semctl(*semid, 0, SETVAL, value) == -1) {
        // Erreur pendant semctl
        return -1;
    }
    return 0;
}

int semtab_p(int semid){
    struct sembuf op;
    op.sem_num = 0;
    op.sem_op = -1;  // décrémenter de 1
    op.sem_flg = 0;
    return semop(semid, &op, 1);
}

int semtab_v(int semid){
    struct sembuf op;
    op.sem_num = 0;
    op.sem_op = 1;  // incrémenter de 1
    op.sem_flg = 0;
    return semop(semid, &op, 1);
}

int semtab_p_n(int semid, int n){
    struct sembuf op;
    op.sem_num = 0;
    op.sem_op = -n;  // décrémenter de n
    op.sem_flg = 0;
    return semop(semid, &op, 1);
}

int semtab_v_n(int semid, int n){
    struct sembuf op;
    op.sem_num = 0;
    op.sem_op = n;  // incrémenter de n
    op.sem_flg = 0;
    return semop(semid, &op, 1);
}

int semtab_destroy(int semid){
    return semctl(semid, 0, IPC_RMID);
}

int main() {
    int semid;
    key_t key = 4321;

    if ((semid = semtab_init(&semid, key, 0)) == -1) {
        perror("Erreur lors de l'initialisation du sémaphore");
        return 1;
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("Erreur fork");
        return 1;
    }

    if (pid == 0) {  // Processus enfant (consommateur)
        for (int i = 0; i < 5; i++) {
            printf("Consommateur attend un élément...\n");
            if (semtab_p(semid) == -1) {
                perror("Erreur lors de l'opération P par le consommateur");
                return 1;
            }
            printf("Consommateur a consommé un élément! \n");
            sleep(1);
        }
        return 0;
    } else {  // Processus parent (producteur)
        for (int i = 0; i < 5; i++) {
            printf("Producteur produit un élément...\n");
            sleep(2);
            if (semtab_v(semid) == -1) {
                perror("Erreur lors de l'opération V par le producteur");
                return 1;
            }
            printf("Producteur a produit un élément!\n");
        }
        wait(NULL);
    }

    //Destruction du sémaphore
    if (semtab_destroy(semid) == -1) {
        perror("Erreur lors de la destruction du sémaphore");
        return 1;
    }

    printf("Fin du programme\n");
    return 0;
}