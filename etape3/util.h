#ifndef UTIL_H
#define UTIL_H

#include "blockchain.h"

Transaction *nextTransaction();
Block *nextBlock(Block *previous);
unsigned long getBlockHash(Block *block);
unsigned long simple_hash(unsigned char *data, size_t length);

#endif // UTIL_H
