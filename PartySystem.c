#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "PartySystem.h"
#include "set.h"

#define MAX_LENGTH 50//Need to delete
#define MAX_INT 2147483647

struct partySystem {
    Set partySet;
};

/*static SetElement copyElement(SetElement element) {
    FILE *file = fopen("copyfile.txt", "w+");
    if(file == NULL) exit(1);
    int check = saveParty((Party)element, "copyfile.txt");
    if(check == PARTY_FAIL) exit(1);
    Party newParty = malloc(sizeof(*newParty));
    if(newParty == NULL) exit(1);
    newParty = createParty("copyfile.txt");
    if(newParty == NULL) exit(1);
    fclose(file);
    return newParty;
}*/

static SetElement copyElement(SetElement element) {
    return element;
}

static int compareElements(SetElement element1, SetElement element2) {
    int party_size1, party_size2;
    char *party_name1, *party_name2;
    PartyCode party_code1;
    PartyCode party_code2;
    PartyResult check1, check2;
    check1 = getPartyDetails((Party)element1, &party_name1, &party_code1, &party_size1);
    check2 = getPartyDetails((Party)element2, &party_name2, &party_code2, &party_size2);
    if(check1 == PARTY_FAIL || check2 == PARTY_FAIL) {
        printf("Error in compareElements");
        exit(1);
    }
    int result1 = strcmp(party_code1, party_code2);
    int result2 = strcmp(party_name1, party_name2);
    free(party_name1);
    free(party_code1);
    free(party_name2);
    free(party_code2);
    return result1 * result2;
}

static void freeElement(SetElement element) {
    destroyParty((Party)element);
}

PartySystem createPartySystem() {
    PartySystem party_system = malloc(sizeof(*party_system));
    if(party_system == NULL) return NULL;

    party_system->partySet = setCreate(copyElement, freeElement, compareElements);
    if(party_system->partySet == NULL) {
        free(party_system);
        return NULL;
    }
    return party_system;
}

PartySystemResult addParty(PartySystem party_system, char *party_data_file) {
    FILE* file = fopen(party_data_file, "r");
    if(file == NULL) return SYSTEM_FAIL;
    int c = fgetc(file);
    if (c == EOF) {
        /* file empty */
        return SYSTEM_FAIL;
    }
    Party party = createParty(party_data_file);
    if(party == NULL) {
        fclose(file);
        return SYSTEM_FAIL;
    }
    SET_FOREACH(Party, iterator, party_system->partySet) {
        if(haveCommonMembers(iterator, party)) {
            fclose(file);
            destroyParty(party);
            return SYSTEM_FAIL;
        }
    }
    int result = setAdd(party_system->partySet, party);
    fclose(file);
    party = NULL;
    return (result == SET_SUCCESS) ? SYSTEM_SUCCESS : SYSTEM_FAIL;
}
//Need to add destroy Party
PartySystemResult deleteParty(PartySystem party_system, PartyCode prt_code) {
    int setResult = SET_NULL_ARGUMENT;
    int *party_size = malloc(sizeof(int));
    char *party_name = malloc(MAX_LENGTH*sizeof(char));
    char *party_code = malloc(MAX_LENGTH*sizeof(char));
    if(party_size == NULL || party_name == NULL || party_code == NULL) {
        free(party_size);
        free(party_name);
        free(party_code);
        return SYSTEM_FAIL;
    }
    SET_FOREACH(Party, iterator, party_system->partySet) {
        getPartyDetails(iterator, party_name, party_code, party_size);
        if(strcmp(party_code, prt_code)) {
            setResult = setRemove(party_system->partySet, iterator);
        }
    }
    free(party_size);
    free(party_name);
    free(party_code);
    return (setResult == SET_SUCCESS) ? SYSTEM_SUCCESS : SYSTEM_FAIL;
}

static int myCompare(const void* prt1, const void* prt2/*, int compare(Party prt1, Party prt2)*/) {
    return
}

typedef int (*my_comp)(void* , void*);
my_comp p1;

PartySystemResult displayPartySystem(PartySystem party_system, int compare(Party prt1, Party prt2)) {
    assert(compare != NULL && party_system != NULL);
    int party_system_size = setGetSize(party_system->partySet), i = 0;
    Party arr = malloc(party_system_size*sizeof(arr));
    if(arr == NULL) return SYSTEM_FAIL;

    SET_FOREACH(Party, iterator, party_system->partySet) {
        arr[i] = iterator;
        i++;
    }
    p1=compare;
    qsort(arr, party_system_size, sizeof(struct party), p1);//Check about the compare
//    qsort(arr, party_system_size, sizeof(struct party), (my_comp)compare);//Check about the compare
    for(int j = 0; j < party_system_size; j++) {
        displayParty(arr[j], 0, MAX_INT)///Maybe there is a better way than MAX_INT
    }
    free(arr);
    printf("\n--------------------------------------------------------------------------------\n");
}

void destroyPartySystem(PartySystem party_system) {
    setDestroy(party_system->partySet);
    free(party_system);
}

