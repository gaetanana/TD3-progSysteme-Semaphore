#include "blockchain.h"
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#define BUFFER_SIZE 256 // La taille du buffer circulaire
#define MAX_TRANSACTIONS_PER_BLOCK 256

/* Phase n°0 initialisation des sémaphores et des mutex et du buffer circulaire */
// Buffer circulaire pour les transactions
Transaction buffer[BUFFER_SIZE];
int writeIndex = 0;
int readIndex = 0;

// Sémaphores et mutex
sem_t empty;
sem_t full;
pthread_mutex_t mutex;
/* Fin phase n°0 */

/* Phase n°1 production de transaction */
// Fonction pour produire des transactions
void* producer(void* arg) {
    while (1) {
        Transaction* transaction = nextTransaction();
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);

        buffer[writeIndex] = *transaction;
        writeIndex = (writeIndex + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutex);
        sem_post(&full);

        free(transaction);
    }
    return NULL;
}

/* Fin phase n°1 */

/* Phase n°2 traitement de transaction et validation de transaction */

int isTransactionValid(Transaction* transaction){
    // Vérifier que la transaction est valide
    return 1;
}

void* consumer(void* arg) {
    Block* currentBlock = (Block*)arg;
    while (1) { // Remplacer par une condition pour arrêter proprement
        sem_wait(&full);
        pthread_mutex_lock(&mutex);

        Transaction transaction = buffer[readIndex];
        readIndex = (readIndex + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutex);
        sem_post(&empty);

        // Ajouter la transaction valide au bloc
        if (isTransactionValid(&transaction)) {
            // Assumer que le bloc ne sera jamais plein dans cet exemple
            currentBlock->transactions[currentBlock->num_transactions++] = transaction;
            if (currentBlock->num_transactions == MAX_TRANSACTIONS_PER_BLOCK) {
                if (isBlockValid(currentBlock)) {
                    // Ici, vous pourriez ajouter le bloc à la blockchain
                    // et commencer un nouveau bloc
                }
                currentBlock->num_transactions = 0; // Réinitialiser le bloc pour l'exemple
            }
        }
    }
    return NULL;
}

/* Fin phase n°2 */

/* Phase n°3 validation de bloc */

int isBlockValid(Block* block){
    // Vérifier que le bloc est valide
    return 1;
}

int main() {
    // Initialisation
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    // Création du premier bloc
    Block* currentBlock = (Block*)malloc(sizeof(Block));
    memset(currentBlock, 0, sizeof(Block)); // Initialiser le bloc

    // Création des threads producteur et consommateur
    pthread_t tid_producer, tid_consumer;

    pthread_create(&tid_producer, NULL, producer, NULL);
    pthread_create(&tid_consumer, NULL, consumer, currentBlock);

    // Joindre les threads producteur et consommateur
    pthread_join(tid_producer, NULL);
    pthread_join(tid_consumer, NULL);


    // Nettoyage
    free(currentBlock); // Libérer le bloc courant
    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;
}
