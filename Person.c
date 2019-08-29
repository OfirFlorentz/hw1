#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Person.h"

#define ID_SIZE 10
#define M_SIGN 'M'
#define F_SIGN 'F'

struct person_t {
	char* name;
	char id[ID_SIZE];
	PersonGender gender;
};

//The function gets a person and an id, and set the person id's to be id
static Person_Result personSetId(char* id, Person person) {
	if (strlen(id) != ID_SIZE - 1) return INPUT_ERROR;
	strcpy(person->id, id);
	return PERSON_SUCSSESS;
}

//The function gets a person and a name, and set the person's name to be name
static void personSetName(char *name, Person person) {
    strcpy(person->name, name);
}

Person personCreate (char* name, char* id, PersonGender gender) {
	Person person = malloc(sizeof(*person));
	if (!person) return NULL;
	person->name = malloc((strlen(name)+1)* sizeof(char));
	if(person->name == NULL) {
	    free(person);
	    return NULL;
	}
	if (personSetId(id, person) != PERSON_SUCSSESS) {
		personDestroy(person);
		return NULL;
	}
	person->gender = gender;
	personSetName(name, person);
	return person;
}

Person personCopy(Person person) {
	return personCreate(person->name, person->id, person->gender);
}

void personDestroy(Person person) {
    if (person == NULL) return;
	free(person->name);
	free(person);
}

char* personGetId(Person person) {
	return person->id;
}

PersonGender personGetGender(Person person) {
	return person->gender;
}

char* personGetName(Person person) {
	return person->name;
}


