// Inclusion des bibliothèques nécessaires
#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <unistd.h>

int semtab_init(int *semid, key_t key, unsigned int value){
    return semget(key, 1, IPC_CREAT | 0666);
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

int main(){
    key_t key = ftok("./semfilee", 65) ;
    int init = semtab_init(&key, key, 1);
    printf("init: %d\n", init);
}

