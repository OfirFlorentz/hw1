#include <stdio.h>
#include <stdlib.h>
#include "hw1dryq1.h"

struct list_t {
	void* data;
	List next;
};

/* the func get an array from any data type, a copy func sutible for the the data, a free func sutible for the elment
   the func create a list that holds the values of the array and return a pointer to the head of the list
   */
void* listCreate(void* arr[], int n, void* copyElement(void*), void freeElement(void*))
{	
	List head = malloc(sizeof(List));
	if (head == NULL) return -1;
	List list = malloc(sizeof(List));
	if (list == NULL) {
		free(head);
		return -1;
	}
	head->next = list;
	List p1 = NULL;
	for (int i = 0; i < n; i++)	{
		list->data = copyElement(arr[i]);
		p1 = malloc(sizeof(List));
		if (p1 == NULL) {
			listDestroy(head, freeElement);
			return -1;
		}
		list->next = p1;
		p1 = NULL;
	}
	return head;
}


/* the func get a list from any data type,  a free func sutible for the elment
   the free the list and the values it holds*/
void listDestroy(List list, void freeElement(void*)) {
	List p1;
	while (list != NULL) {
		freeElement(list->data);
		p1 = list->next;
		free(list);
		list = p1;
	}
}


/* the func get a list from any data type, a print func sutible for the elment
   the func print the values in the list */
void listDisplay(List list, void printElement(void*)) {
	while (!list) {
		printElement(list->data);
		list = list->next;
	}
}
