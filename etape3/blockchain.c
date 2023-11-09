#include "blockchain.h"
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define BUFFER_SIZE 256

// Définition du buffer circulaire
Transaction buffer[BUFFER_SIZE];
int in = 0; // index où le producteur placera la prochaine transaction
int out = 0; // index d'où le consommateur prendra la prochaine transaction

// Sémaphores pour la synchronisation
sem_t mutex, empty, full;

// Blockchain
Blockchain blockchain;
Block *currentBlock;
int currentTransactionIndex = 0;

// Prototypes
Transaction* nextTransaction(void);
int isTransactionValid(Transaction *transaction);
int isBlockValid(Block *block);
unsigned long getBlockHash(Block *block);
Block* nextBlock(Block *previous);
void validateBlock();

void *producer(void *arg) {
    while(1) {
        Transaction *transaction = nextTransaction();

        sem_wait(&empty);
        sem_wait(&mutex);

        buffer[in] = *transaction;
        in = (in + 1) % BUFFER_SIZE;

        printf("Transaction produite : [ID: %ld, Données: %s]\n", transaction->id, transaction->data);

        sem_post(&mutex);
        sem_post(&full);

        free(transaction);
    }
    return NULL;
}

void *consumer(void *arg) {
    while(1) {
        sem_wait(&full);
        sem_wait(&mutex);

        Transaction transaction = buffer[out];
        out = (out + 1) % BUFFER_SIZE;

        printf("Transaction consommée : [ID: %ld, Données: %s]\n", transaction.id, transaction.data);

        sem_post(&mutex);
        sem_post(&empty);

        if(isTransactionValid(&transaction)) {
            currentBlock->transactions[currentTransactionIndex++] = transaction;
            if(currentTransactionIndex == 256) {
                validateBlock();
                printf("Bloc validé et ajouté à la blockchain.\n");
                currentTransactionIndex = 0;
            }
        }
    }
    return NULL;
}

void validateBlock() {
    if(isBlockValid(currentBlock)) {
        unsigned long hash = getBlockHash(currentBlock);
        memcpy(currentBlock->hashcode, &hash, sizeof(hash));

        // Ajouter le bloc à la blockchain
        Node *node = (Node *)malloc(sizeof(Node));
        node->block = *currentBlock;
        node->next = blockchain.head;
        blockchain.head = node;

        printf("Hash du bloc ajouté: %lu\n", hash);

        currentBlock = nextBlock(currentBlock);
    }
}


int isTransactionValid(Transaction *transaction) {
    // Implémentez la logique de validation de la transaction ici
    return 1;  // supposons que toutes les transactions sont valides pour le moment
}

int isBlockValid(Block *block) {
    // Implémentez la logique de validation du bloc ici
    return 1;  // supposons que tous les blocs sont valides pour le moment
}

int main() {
    pthread_t producer_thread, consumer_thread;
    sem_init(&mutex, 0, 1);
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);

    // Initialisation de la blockchain et du bloc courant
    blockchain.head = NULL;
    currentBlock = (Block *)malloc(sizeof(Block));

    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);

    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    return 0;
}
