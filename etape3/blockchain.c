#include "blockchain.h"

// Supposons que ces fonctions soient fournies
Transaction nextTransaction() {
    Transaction t;
    // générer une transaction
    return t;
}

int isTransactionValid(Transaction t) {
    // vérifier la validité de la transaction
    return 1; // 1 pour valide, 0 sinon
}

int isBlockValid(Block b) {
    // vérifier la validité du block
    return 1; // 1 pour valide, 0 sinon
}

unsigned char* getBlockHash(Block b) {
    // générer un hash pour le block
    static unsigned char hash[32];
    return hash;
}

Block nextBlock() {
    Block b;
    // initialiser un nouveau block
    return b;
}

// Variables globales
sem_t vide, plein, mutex;
Transaction buffer[256]; // Buffer circulaire
int in = 0, out = 0;     // Indices d'écriture et de lecture
Block currentBlock;
int currentTransactionIndex = 0;

void* producer(void* arg) {
    while (1) {
        Transaction t = nextTransaction();

        sem_wait(&vide);
        sem_wait(&mutex);

        buffer[in] = t;
        in = (in + 1) % 256;

        sem_post(&mutex);
        sem_post(&plein);
    }
    return NULL;
}

void* consumer(void* arg) {
    while (1) {
        sem_wait(&plein);
        sem_wait(&mutex);

        Transaction t = buffer[out];
        out = (out + 1) % 256;

        sem_post(&mutex);
        sem_post(&vide);

        if (isTransactionValid(t)) {
            currentBlock.transactions[currentTransactionIndex++] = t;
            if (currentTransactionIndex == 256) {
                // Passer à la phase de validation de blocs
                if (isBlockValid(currentBlock)) {
                    currentBlock.hashcode = getBlockHash(currentBlock);
                    // Ajouter le bloc à la blockchain (omis pour simplification)
                    currentBlock = nextBlock();
                    currentTransactionIndex = 0;
                }
            }
        }
    }
    return NULL;
}

int main() {
    // Initialisation
    sem_init(&vide, 0, 256); // 256 places libres au départ
    sem_init(&plein, 0, 0); // 0 places occupées
    sem_init(&mutex, 0, 1); // Mutex

    pthread_t prod, cons;
    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);

    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    return 0;
}
