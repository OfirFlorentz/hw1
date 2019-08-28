#ifndef PERSON_H_
#define PERSON_H_

typedef struct person_t *Person;
typedef enum  {PERSON_SUCSSESS, INPUT_ERROR, MEMORY_ERROR} Person_Result;
typedef enum  { P_MASCULINE, P_FEMININE } PersonGender;

/*the func get data and put into a Person starcture
the func return a pointer to the person*/
Person personCreate(char* name, char* id, PersonGender gender);

/* the func free the memory of the pointer to person stracture*/
void personDestroy(Person person);

/* the func get a person and return a copy of him*/
void* personCopy(Person person);

/*the func return the id of the person*/
char* personGetId(Person person);

/*the func return the name of the person*/
char* personGetName(Person person);

/*the func return the gender of the person*/
PersonGender personGetGender(Person person);


#endif // PERSON_H_
