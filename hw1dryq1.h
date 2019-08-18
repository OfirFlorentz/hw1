#ifndef _LIST_H
#define _LIST_H


typedef struct list_t* List;

void* listCreate(void* arr[], int n, void* copyElement(void*), void freeElement(void*));
void listDestroy(List list, void freeElement(void*));
void listDisplay(List list, void printElement(void*));


#endif // ! LIST_H

