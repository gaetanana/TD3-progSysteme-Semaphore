#include "blockchain.h"
#include "util.h"
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <unistd.h>
#define BUFFER_SIZE 256 // La taille du buffer circulaire
#define MAX_TRANSACTIONS_PER_BLOCK 256
#define NUM_PRODUCERS 1 // Nombre de producteurs
#define NUM_CONSUMERS 1 // Nombre de consommateurs
Blockchain blockchain;
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
        printf("Producer: Transaction produite avec ID %ld\n", transaction->id);
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
    if(transaction->data[0] < 250){
        return 1;
    }
    else{
        return 0;
    }
}

// Fonction pour le traitement des transactions (consommateur)
void* consumer(void* arg) {
    Block* currentBlock = (Block*)arg;
    while (1) {
        sem_wait(&full);
        pthread_mutex_lock(&mutex);

        Transaction transaction = buffer[readIndex];
        printf("Consumer: Traitement de la transaction ID %ld\n", transaction.id);
        readIndex = (readIndex + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutex);
        sem_post(&empty);

        // Ajouter la transaction valide au bloc
        if (isTransactionValid(&transaction)) {
            currentBlock->transactions[currentBlock->num_transactions++] = transaction;
            printf("Consumer: Transaction valide ajoutée au bloc\n");
            if (currentBlock->num_transactions == MAX_TRANSACTIONS_PER_BLOCK) {
                printf("Consumer: Le bloc est plein, ajout à la blockchain\n");

                if (isBlockValid(currentBlock)) {
                    addBlockToBlockchain(&blockchain, currentBlock);
                    printf("Consumer: Bloc validé et ajouté à la blockchain\n");

                }

                // Libérer le bloc actuel et en créer un nouveau
                free(currentBlock);
                currentBlock = (Block*)malloc(sizeof(Block));
                memset(currentBlock, 0, sizeof(Block));

                if (currentBlock == NULL) {
                    fprintf(stderr, "Erreur d'allocation de mémoire pour un nouveau bloc\n");
                    return NULL;
                }


                // Le bloc est plein, mais sa validation et son ajout à la blockchain
                // se feront dans la phase n°3
                // Pour l'instant, on suppose qu'un nouveau bloc est initialisé en dehors de cette fonction
                // currentBlock = nextBlock(currentBlock); // Cette partie sera gérée dans la phase n°3
            }
        }
    }
    return NULL;
}
/* Fin phase n°2 */
/* Phase n°3 validation de bloc */
// Fonction pour valider un bloc
int isBlockValid(Block* block){
    // Vérifier que le bloc est valide
    return 1;
}
// Fonction pour ajouter un bloc à la blockchain
void addBlockToBlockchain(Blockchain* blockchain, Block* currentBlock) {
    // Calculer le hashcode du bloc
    unsigned long hash = getBlockHash(currentBlock);
    memcpy(currentBlock->hashcode, &hash, sizeof(hash));

    // Créer un nouveau noeud pour la blockchain
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        // Gérer l'erreur d'allocation de mémoire
        fprintf(stderr, "Erreur d'allocation de mémoire pour un nouveau noeud\n");
        return;
    }

    // Copier les données du bloc dans le nouveau noeud
    newNode->block = *currentBlock;
    newNode->next = NULL; // Le nouveau noeud sera le dernier, donc son suivant est NULL

    // Si la blockchain est vide, ajouter le nouveau noeud en tête
    if (blockchain->head == NULL) {
        blockchain->head = newNode;
    } else {
        // Sinon, parcourir la blockchain pour trouver le dernier noeud
        Node* current = blockchain->head;
        while (current->next != NULL) {
            current = current->next;
        }
        // Ajouter le nouveau noeud à la fin de la blockchain
        current->next = newNode;
    }
}
/* Fin phase n°3 */

int main(){

    printf("Démarrage du programme de simulation de blockchain\n");

    blockchain.head = NULL;

    // Initialisation du buffer circulaire, des sémaphores et des mutex
    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);

    // Initialisation de la blockchain

    // Création du premier bloc
    Block* currentBlock = (Block*)malloc(sizeof(Block));
    memset(currentBlock, 0, sizeof(Block)); // Initialiser le bloc

    // Création des threads producteurs et consommateurs
    pthread_t producers[NUM_PRODUCERS], consumers[NUM_CONSUMERS];

    for (int i = 0; i < NUM_PRODUCERS; i++) {
        pthread_create(&producers[i], NULL, producer, NULL);
    }

    for (int i = 0; i < NUM_CONSUMERS; i++) {
        pthread_create(&consumers[i], NULL, consumer, currentBlock);
    }

    usleep(10000); // Attente de 10 millisecondes

    // Joindre les threads producteurs et consommateurs
    for (int i = 0; i < NUM_PRODUCERS; i++) {
        pthread_join(producers[i], NULL);
    }

    for (int i = 0; i < NUM_CONSUMERS; i++) {
        pthread_join(consumers[i], NULL);
    }

    // Nettoyage
    free(currentBlock);
    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);

    //Pour lancer le main
    //gcc -o blockchain blockchain.c util.c
    //./blockchain
    return 0;
}
