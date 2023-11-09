#include "blockchain.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void fill_transaction(Transaction *transaction)
{
    srand(time(NULL));
    transaction->id = rand();

    for (int i = 0; i < 256; ++i)
    {
        transaction->data[i] = rand() % 256;
    }
}

Transaction *nextTransaction()
{

    Transaction *ptr = (Transaction *)malloc(sizeof(Transaction));
    fill_transaction(ptr);
    return ptr;
}

Block *nextBlock(Block *previous)
{
    Block *newBlock = (Block *)malloc(sizeof(Block));
    memcpy(&newBlock->previous_hashcode, &previous->hashcode, sizeof(previous->hashcode));
    return newBlock;
}

unsigned long simple_hash(byte *data, size_t length)
{
    unsigned long hash = 5381;
    for (size_t i = 0; i < length; ++i)
    {
        hash = ((hash << 5) + hash) + data[i];
    }
    return hash;
}


unsigned long getBlockHash(Block *block)
{
    int s = 256 * sizeof(Transaction) + 32;
    unsigned char buff[s];
    memcpy(buff, &block->transactions, 256 * sizeof(Transaction));
    memcpy(buff + 256 * sizeof(Transaction), &block->previous_hashcode, 32);
    return simple_hash(buff, s);
};

