//
// Created by HP on 25/10/2026.
//
#include "testing.h"
#include <windows.h>
#include <stddef.h>
#include <string.h>

#define ALIGN 16
#define ALIGN_UP(x) (((x) + (ALIGN - 1)) & ~(size_t)(ALIGN - 1))
#define CHUNK_MIN (1024 * 1024)     // on demande 1 Mo à l'OS à la fois
#define PAGE 4096
#define HEADER ALIGN_UP(sizeof(block_t))
static block_t *head = NULL;
static block_t *find_free(block_t **last, size_t size) {
    block_t *cur = head;
    while (cur && !(cur->free && cur->size >= size)) {
        *last = cur;
        cur = cur->next;
    }
    return cur;
}

static void split(block_t *b, size_t size) {
    if (b->size >= size + HEADER + ALIGN) {
        block_t *n = (block_t *)((char *)b + HEADER + size);
        n->size = b->size - size - HEADER;
        n->free = 1;
        n->next = b->next;
        b->size = size;
        b->next = n;
    }
}

static block_t *request_space(block_t *last, size_t size) {
    size_t needed = HEADER + size;
    size_t chunk = needed > CHUNK_MIN ? needed : CHUNK_MIN;
    chunk = (chunk + PAGE - 1) & ~(size_t)(PAGE - 1);

    block_t *b = VirtualAlloc(NULL, chunk, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!b)
        return NULL;

    b->size = chunk - HEADER;
    b->next = NULL;
    b->free = 0;
    if (last)
        last->next = b;
    split(b, size);
    return b;
}

void *my_malloc(size_t size) {
    if (size == 0 || size > (size_t)-1 - HEADER - ALIGN - PAGE)
        return NULL;
    size = ALIGN_UP(size);

    block_t *last = NULL;
    block_t *b = find_free(&last, size);
    if (b) {
        split(b, size);
        b->free = 0;
    } else {
        b = request_space(last, size);
        if (!b)
            return NULL;
        if (!head)
            head = b;
    }
    return (char *)b + HEADER;
}

void my_free(void *ptr) {
    if (!ptr)
        return;
    block_t *b = (block_t *)((char *)ptr - HEADER);
    b->free = 1;

    while (b->next && b->next->free &&
           (char *)b + HEADER + b->size == (char *)b->next) {
        b->size += HEADER + b->next->size;
        b->next = b->next->next;
    }
}