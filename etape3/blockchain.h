#include <stdio.h>
#include <stdlib.h>

// Structure pour une transaction
typedef struct {
    long id ;
    unsigned char data[256] ;
} Transaction ;
// Structure pour un block
typedef struct {
    Transaction transactions[256] ;
    unsigned char hashcode[32] ;
    unsigned char previous_hashcode[32] ;
    int num_transactions;
} Block ;
// Node for the blockchain list
typedef struct Node {
    Block block ;
    struct Node *next ;
} Node ;
// Structure for a Blockchain
typedef struct {
    Node *head ;
} Blockchain ;