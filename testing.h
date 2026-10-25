//
// Created by HP on 25/10/2026.
//

#ifndef PROJET_UGO_TESTING_H
#define PROJET_UGO_TESTING_H
#include "stddef.h"
typedef struct block {
    size_t size;
    struct block *next;
    int free;
} block_t;
#endif //PROJET_UGO_TESTING_H
