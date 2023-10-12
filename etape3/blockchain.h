
typedef struct
{
    long id;
    unsigned char data[256];
} Transaction;

typedef struct
{
    Transaction transactions[256];
    unsigned char previous_hashcode[32];
    unsigned char hashcode[32];
} Block;

// Node for the blockchain list
typedef struct Node
{
    Block block;
    struct Node *next;
} Node;

// Structure for a Blockchain
typedef struct
{
    Node *head;
} Blockchain;

typedef unsigned char byte;
