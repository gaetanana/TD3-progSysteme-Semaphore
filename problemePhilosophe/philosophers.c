#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 5

sem_t fourchettes[N];

void* philosophe_simple(void* num) {
    int* i = (int*) num;
    while(1) {
        sem_wait(&fourchettes[*i]);
        sem_wait(&fourchettes[(*i+1)%N]);

        printf("Philosophe %d mange...\n", *i);
        sleep(1);

        sem_post(&fourchettes[*i]);
        sem_post(&fourchettes[(*i+1)%N]);

        printf("Philosophe %d pense...\n", *i);
        sleep(1);
    }
}

void* philosophe_ordre(void* num) {
    int* i = (int*) num;
    while(1) {
        if (*i % 2 == 0) {
            sem_wait(&fourchettes[*i]);
            sem_wait(&fourchettes[(*i+1)%N]);
        } else {
            sem_wait(&fourchettes[(*i+1)%N]);
            sem_wait(&fourchettes[*i]);
        }

        printf("Philosophe %d mange...\n", *i);
        sleep(1);

        sem_post(&fourchettes[*i]);
        sem_post(&fourchettes[(*i+1)%N]);

        printf("Philosophe %d pense...\n", *i);
        sleep(1);
    }
}

sem_t controleur;

void* philosophe_controleur(void* num) {
    int* i = (int*) num;
    while(1) {
        sem_wait(&controleur);

        sem_wait(&fourchettes[*i]);
        sem_wait(&fourchettes[(*i+1)%N]);

        printf("Philosophe %d mange...\n", *i);
        sleep(1);

        sem_post(&fourchettes[*i]);
        sem_post(&fourchettes[(*i+1)%N]);

        sem_post(&controleur);

        printf("Philosophe %d pense...\n", *i);
        sleep(1);
    }
}

int main() {
    pthread_t tid[N];
    int nums[N];

    for (int i = 0; i < N; i++) {
        sem_init(&fourchettes[i], 0, 1);
    }

    sem_init(&controleur, 0, N-1);

    for (int i = 0; i < N; i++) {
        nums[i] = i;
        pthread_create(&tid[i], NULL, philosophe_simple, &nums[i]);
        // Pour tester l'autre approche:
        // pthread_create(&tid[i], NULL, philosophe_ordre, &nums[i]);
        // pthread_create(&tid[i], NULL, philosophe_controleur, &nums[i]);
    }

    for (int i = 0; i < N; i++) {
        pthread_join(tid[i], NULL);
    }

    return 0;
}

//gcc -o philosophers philosophers.c -lpthread -lrt