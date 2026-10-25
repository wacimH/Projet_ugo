//
// Created by HP on 25/10/2026.
//
#include <stdio.h>
#include <stddef.h>

void *my_malloc(size_t);
void my_free(void *);

int main(void) {
    int *a = my_malloc(sizeof(int));
    int *b = my_malloc(10 * sizeof(int));
    *a=3;
    printf("a = %d, b = %p \n", *a, (void *)b);

    my_free(a);
    my_free(b);
    return 0;
}