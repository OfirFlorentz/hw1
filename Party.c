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
#define ONE_SIGN '1'
#define NINE_SIGN '9'
#define EOS '\0'

struct party {
    char name[NAME_SIZE];
    char combination_code[NAME_SIZE];
    List party_members;
};

//Need to explain
static void* genericPersonCopy(void* person) {
    return (void*)personCopy((Person)person);
}

//Need to explain
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
        if (id[counter] < ONE_SIGN || id[counter] > NINE_SIGN) return false;
        counter++;
    }
    counter++;
    if (counter != ID_SIZE) return false;
    return true;
}

static Gender charToGender(char c) {
    if(c == M_SIGN) return MASCULINE;
    return FEMININE;
}

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
    ListResult list_result;
    while (!feof(file))
    {
        Person person = fileLineToPerson(file);
        if(person == NULL) {
            printf("Memory problem");
            exit(1);
        }
        list_result = listInsertLast  (list,person);
        printf("%d", list_result);
        //listGetNext(list);
    }
}
//----------------------------------------------------------------------
// changing Enum type to string. helps to SaveParty
static char printGenderName(Gender gender)
{
    if (gender == MASCULINE) return M_SIGN;//we need to ask isreal if it should be M_SIGN or Mascoline
    return F_SIGN;
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
    listDestroy(party->party_members);
    free(party);
}

PartyResult addPerson(Party party, char *name, char *id, Gender gender, int position) {
    assert(party != NULL && name != NULL && id != NULL);
    if(!personIsIdValid(id) || position < 1 || strcmp("", name) == 0 || isMember(party, id))  return PARTY_FAIL;
    List tmpList = party->party_members;
    Person person = personCreate(name, id, gender);
    if(person == NULL) {
        return PARTY_FAIL;
    }
    for (int i = 1; i < position; i++) {
        listGetNext(tmpList);
    }
    ListResult result = listInsertBeforeCurrent(tmpList, person);
    personDestroy(person);
    tmpList = NULL;
    return result;
}

PartyResult deletePerson(Party party, char *id) {
    assert(party != NULL && id != NULL);//Need to also check if person appears in the party in more than 1 position
    List tmpList = party->party_members;
    while (tmpList != NULL) {
        if (strcmp(personGetId(listGetCurrent(tmpList)), id) == 0) {
            listRemoveCurrent(tmpList);
            return PARTY_SUCCESS;
        }
        listGetNext(tmpList);
    }
    return PARTY_FAIL;
}

bool isMember(Party party, char *id) {
    List tmpList = party->party_members;
    while (tmpList != NULL) {
        if (strcmp(personGetId(listGetCurrent(tmpList)), id) == 0) return true;
        listGetNext(tmpList);
    }
    return false;
}

PartyResult joinParties(Party* original_party_1, Party* original_party_2, Party* outcome_party, int position_party_2[],
                        int n /*length of position_party_2*/, char *new_name, char *new_code) {
    assert(original_party_1 != NULL && original_party_2 != NULL && outcome_party != NULL && new_name != NULL &&
           new_code != NULL);
    if (haveCommonMembers(*original_party_1, *original_party_2)) return PARTY_FAIL;
    Party new_party = malloc(sizeof(*new_party));
    if (!new_party) return PARTY_FAIL;
    strcpy(new_party->name, new_name);
    strcpy(new_party->combination_code, new_code);
    new_party->party_members = listCopy((*original_party_1)->party_members);
    if (new_party->party_members == NULL) {
        free(new_party);
        return PARTY_FAIL;
    }
    List new_list = new_party->party_members;
    List p2_list = (*original_party_2)->party_members;
    // i assume array is ordered we need to check
    int current_postion = 1;
    int p2_counter = 0;
    while (p2_list != NULL && p2_counter < n )
    {
        if (position_party_2[p2_counter] == current_postion) {
            p2_counter++;
            listInsertBeforeCurrent(new_list, listGetCurrent(p2_list));
            listGetNext(p2_list);
        }
        else {
            listGetNext(new_list);
        }
        current_postion++;
    }
    while(p2_list != NULL) {
        listInsertAfterCurrent(new_list, listGetCurrent(p2_list));
        listGetNext(new_list);
        listGetNext(p2_list);
    }
    destroyParty(*original_party_1);
    destroyParty(*original_party_2);
    return PARTY_SUCCESS;
}

PartyResult displayParty(Party party, int from_position, int to_position) {
    assert(party != NULL);
    fprintf(stdout, "%s\n" ,party->name);
    fprintf(stdout, "%s\n", party->combination_code);
    List party_list = party->party_members;
    if (from_position < 1) from_position = 1;
    Person person;
    for (int i = 0; i <= to_position - from_position && party_list != NULL; i++) {
        person = listGetCurrent(party_list);
        fprintf(stdout, "%s %s %c\n", personGetName(person), personGetId(person), printGenderName(personGetGender(person)));
    }
    person = NULL;
    return PARTY_SUCCESS;
}

PartyResult saveParty(Party party, char *party_data_file) {
    assert(party != NULL);
    FILE* file =  fopen(party_data_file, "w");
    if (!file) return PARTY_FAIL;
    fputs(party->name, file);
    fputs(party->combination_code, file);
    List party_list = party->party_members;
    while (party_list != NULL) {
        Person p = listGetCurrent(party_list);
        fprintf(file, "%50s %s %c/n", personGetName(p), personGetId(p), printGenderName(personGetGender(p)));
        listGetNext(party_list);
    }
    fclose(file);
    return PARTY_SUCCESS;
}

bool haveCommonMembers(Party party1, Party party2) {
    assert(party1 != NULL && party2 != NULL);
    List party_1_list = party1->party_members;
    char cur_id[ID_SIZE];
    while (party_1_list != NULL) {
        strcpy(cur_id, personGetId(listGetCurrent(party_1_list)));
        if (isMember(party2, cur_id)) return true;
        listGetNext(party_1_list);
    }
    return false;
}

PartyResult getPartyDetails(Party party, char **party_name, PartyCode *party_code, int *party_size) {
    assert(party_name != NULL && party_code != NULL && party_size != NULL);
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