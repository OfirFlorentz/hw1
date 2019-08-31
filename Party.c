#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include "list.h"
#include "Person.h"
#include "Party.h"

#define NAME_SIZE 51
#define ID_SIZE 10
#define M_SIGN 'M'
#define F_SIGN 'F'
#define ZERO_SIGN '0'
#define NINE_SIGN '9'
#define EOS '\0'


struct party {
    char name[NAME_SIZE];
    char combination_code[NAME_SIZE];
    List party_members;
};

//the function change the types of personCopy for createList
static void* genericPersonCopy(void* person) {
    return (void*)personCopy((Person)person);
}

//the function change the types of personDestroy for createList
static void genericPersonDestroy(void* person) {
    personDestroy((Person)person);
}

/*the func get an ID as an in input
*return true if its valid and the ID is not already in the list
*return false otherwise */
static bool personIsIdValid(char* id)
{
    assert(id != NULL);
    int counter = 0;
    while (id[counter] != EOS) {
        if (id[counter] < ZERO_SIGN || id[counter] > NINE_SIGN) return false;
        counter++;
    }
    counter++;
    if (counter != ID_SIZE) return false;
    return true;
}

//the get a char that reprasent gender and return the gender in the Gender type
static Gender charToGender(char c) {
    if(c == M_SIGN) return MASCULINE;
    return FEMININE;
}

//The get a pointer to a file that his first line containe name, id, gender
//the func return a Person struct that contain hold the values of the line
static Person fileLineToPerson(FILE* file) {
    assert(file != NULL);
    char name[NAME_SIZE];
    char id[ID_SIZE];
    char gender;
    fscanf(file, " %s %s %c ", name, id, &gender);
    return personCreate(name, id, charToGender(gender));
}

//----------------------------------------------------------------------
/* the func get a line from the file and a list of pepoles and insert the data from the line to the last place
in the list*/
static void insertPersonsFromFile(List list, FILE* file) {
    assert(file != NULL && list != NULL);
    while (!feof(file)) {
        Person person = fileLineToPerson(file);
        listInsertLast(list, person);
        personDestroy(person);
    }
}
//----------------------------------------------------------------------
// changing Enum type to string. helps to SaveParty
static char printGenderName(Gender gender) {
    if (gender == MASCULINE) return M_SIGN;
    return F_SIGN;
}

//The func get a name and a code. the func create and return a new  party with those values
static Party newParty(char *name, char *code) {
    Party party = malloc(sizeof(*party));
    if (!party) return NULL;
    strcpy(party->name, name);
    strcpy(party->combination_code, code);
    return  party;
}

Party createParty(char *party_data_file) {
    FILE *file = fopen(party_data_file, "r");
    if (file == NULL) {
        return NULL;
    }
    Party party = malloc(sizeof(*party));
    if (!party) {
        fclose(file);
        return NULL;
    }
    fscanf(file, " %s ", party->name);
    fscanf(file, " %s ", party->combination_code);
    party->party_members = listCreate(genericPersonCopy, genericPersonDestroy);
    if (!party->party_members) {
        fclose(file);
        free(party);
        return NULL;
    }
    insertPersonsFromFile(party->party_members, file);
    fclose(file);
    return party;
}

void destroyParty(Party party)
{
    if (party == NULL) return;
    listDestroy(party->party_members);
    free(party);
}

PartyResult addPerson(Party party, char *name, char *id, Gender gender, int position) {
    assert(party != NULL && name != NULL && id != NULL);
    if(!personIsIdValid(id) || position < 1 || strcmp("", name) == 0) return PARTY_FAIL;
    if(isMember(party, id))  return PARTY_FAIL;
    List tmpList = party->party_members;
    Person person = personCreate(name, id, gender);
    if(person == NULL) {
        return PARTY_FAIL;
    }

    if(position > listGetSize(tmpList)) {
        listInsertLast(tmpList, person);
        personDestroy(person);
        return PARTY_SUCCESS;
    }
    listGetFirst(tmpList);
    for (int i = 1; i < position; i++) {
        listGetNext(tmpList);
    }
    ListResult list_result = listInsertBeforeCurrent(tmpList, person);
    personDestroy(person);
    tmpList = NULL;
    if (list_result == LIST_SUCCESS) return PARTY_SUCCESS;
    return  PARTY_FAIL;
}

//The function get a party and an id and return how much persons with the id are in the party
static int checkMoreThanOne(Party party, char *id) {
    int counter = 0;
    LIST_FOREACH(Person, iterator, party->party_members) {
        if(strcmp(id, personGetId(iterator)) == 0) counter++;
    }
    return counter;
}

PartyResult deletePerson(Party party, char *id) {
    assert(party != NULL && id != NULL);
    assert(checkMoreThanOne(party, id) <= 1);
    List tmp_list = party->party_members;
    Person  person = listGetFirst(tmp_list);
    while (person != NULL) {
        if (strcmp(personGetId(person), id) == 0) {
            listRemoveCurrent(tmp_list);
            return PARTY_SUCCESS;
        }
        person = listGetNext(tmp_list);
    }
    return PARTY_FAIL;
}

bool isMember(Party party, char *id) {
    assert(party != NULL && id != NULL);
    List party_list = party->party_members;
    int list_len = listGetSize(party_list);
    for (int i = 0; i < list_len ; i++) {
        if (i == 0) {
            if (strcmp(id,personGetId(listGetFirst(party_list))) == 0) return  true;
        } else {
            if (strcmp(id,personGetId(listGetNext(party_list))) == 0) return  true;
        }
    }
    return false;
}


PartyResult joinParties(Party* original_party_1, Party* original_party_2, Party* outcome_party, int position_party_2[],
                        int n /*length of position_party_2*/, char *new_name, char *new_code) {
    assert(original_party_1 != NULL && original_party_2 != NULL && outcome_party != NULL && new_name != NULL &&
           new_code != NULL && position_party_2 != NULL);
    if (haveCommonMembers(*original_party_1, *original_party_2)) return PARTY_FAIL;
    Party new_party = newParty(new_name, new_code);
    if (new_party == NULL) return PARTY_FAIL;
    new_party->party_members = listCopy((*original_party_1)->party_members);
    if (new_party->party_members == NULL) {
        destroyParty(new_party);
        return PARTY_FAIL;
    }
    List party2_list = (*original_party_2)->party_members;
    Person party2_person = listGetFirst(party2_list);
    int p2_counter = 0;
    while (party2_person != NULL && p2_counter < n )
    {
        addPerson(new_party, personGetName(party2_person), personGetId(party2_person),
                  personGetGender(party2_person), position_party_2[p2_counter++]);
        party2_person = (listGetNext(party2_list));

    }
    while(party2_person != NULL) {
        listInsertLast(new_party->party_members, party2_person);
        party2_person = listGetNext(party2_list);
    }
    *outcome_party = new_party;
    destroyParty(*original_party_1);
    destroyParty(*original_party_2);
    *original_party_1 = NULL;
    *original_party_2 = NULL;
    return PARTY_SUCCESS;
}

PartyResult displayParty(Party party, int from_position, int to_position) {
    assert(party != NULL);
    fprintf(stdout, "%s\n" ,party->name);
    fprintf(stdout, "%s\n", party->combination_code);
    List party_list = party->party_members;
    if (from_position < 1) from_position = 1;
    Person person;
    for (int i = 0; i <= to_position - from_position; i++) {
        if (i == 0) {
            person = listGetFirst(party_list);
        } else {
            person = listGetNext(party_list);
            if (person == NULL) break;
        }
        fprintf(stdout, "%s %s %c\n", personGetName(person), personGetId(person), printGenderName(personGetGender(person)));
        person = NULL;
    }
    return PARTY_SUCCESS;
}

PartyResult saveParty(Party party, char *party_data_file) {
    assert(party != NULL && party_data_file != NULL);
    FILE* file =  fopen(party_data_file, "w");
    if (!file) return PARTY_FAIL;
    fputs(party->name, file);
    fprintf(file,"\n");
    fputs(party->combination_code, file);
    fprintf(file,"\n");
    List party_list = party->party_members;
    Person person = listGetFirst(party_list);

    while (person != NULL) {
        fprintf(file, "%.50s %s %c\n", personGetName(person), personGetId(person),
                printGenderName(personGetGender(person)));
        person = listGetNext(party_list);
    }
    fclose(file);
    return PARTY_SUCCESS;
}

bool haveCommonMembers(Party party1, Party party2) {
    assert(party1 != NULL && party2 != NULL);
    Person party1_person = listGetFirst(party1->party_members);
    while (party1_person != NULL) {
        if (isMember(party2, personGetId(party1_person))) return true;
        party1_person = listGetNext(party1->party_members);
    }
    return false;
}

PartyResult getPartyDetails(Party party, char **party_name, PartyCode *party_code, int *party_size) {
    assert(party != NULL && party_name != NULL && party_code != NULL && party_size != NULL);
    *party_name = malloc(sizeof(char) * NAME_SIZE);
    if (*party_name == NULL) {
        return PARTY_FAIL;
    }
    *party_code = malloc(sizeof(char) * NAME_SIZE);
    if (*party_code == NULL) {
        free(*party_name);
        return PARTY_FAIL;
    }
    strcpy(*party_name, party->name);
    strcpy(*party_code, party->combination_code);
    *party_size = listGetSize(party->party_members);
    return PARTY_SUCCESS;
}

