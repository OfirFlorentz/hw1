//
// Created by ofir on 28/08/2019.
//
#include "list.h"
#include <stdio.h>
#include <stdlib.h>

void* copyInt(void* ptr) {
    int* copy = malloc(sizeof(int));
    *copy = *(int*)ptr;
    return copy;
}

void deleteInt(void* ptr) {
    free(ptr);
}

int main() {
    List list;
    list = listCreate(copyInt, deleteInt);
    int* ptr = malloc(sizeof(int));
    *ptr = 5;
    listInsertFirst(list, ptr);
    int* new_ptr =  (int*)listGetFirst(list);
    printf("hello world %d", *new_ptr);

    return  0;
}
