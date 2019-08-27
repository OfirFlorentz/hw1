#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "person.h"

struct person_t {
	char name[NAME_BUFFER];
	char id[ID_SIZE + 1];
	Gender gender;
};

static bool copyId(char* input, char* output) {
	if (strlen(input) != 9) return false;
	strcpy(output, input);
	return true;
}

static bool isGender(char c, Gender gender) {
	if (c == M) {
		gender = MASCULINE;
		return true;
	}
	else if (c == F) {
		gender = MASCULINE;
		return true;
	}
	return false;
}




//the function get from build a person usuing data from the user
Person createPerson (char* input_name, char* id, Gender
 gender) {
	Person p = malloc(sizeof(*p));
	if (!p) return NULL;
	if (!(copyId(id, p->id))) {
		freePerson(p);
		return NULL;
	}
	if (!(isGender(gender, p->gender))) {
		freePerson(p);
		return NULL;
	}
	return p;
}

//delete a person with all his data
void* copyPerson(void* object) {
	Person p = object;
	char id[ID_SIZE + 1];	
	char name[NAME_BUFFER + 1];
	strcpy(name, p->name);
	strcpy(id, p->id);
	return (createPerson(name, id, p->gender));
}


//delete a person with all his data
void freePerson(void* object) {
	free(object);
}

char* getId(Person p) {
	return p->id;
}

Gender getGender(Person p) {
	return p->gender;
}

/*the func return the name of the person*/
char* getName(Person p) {
	return p->name;
}


