#ifndef PERSON_H_
#define PERSON_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define NAME_BUFFER 51
#define ID_SIZE 9
#define M 'M'
#define F 'F'

typedef struct person_t *Person;
typedef enum  {PERSON_SUCSSESS, INPUT_ERROR, MEMORY_ERROR} Person_Result;
typedef enum  { MASCULINE, FEMININE } Gender;


/*the func get data and put into a Person starcture
the func return a pointer to the person*/
Person createPerson(char* name, char* id, char gender);

/* the func free the memory of the pointer to person stracture*/
void freePerson(void* p);

/* the func get a person and return a copy of him*/
void* copyPerson(void* object);

/*the func return the id of the person*/
char* getId(Person p);


#endif // ! PERSON_H_
