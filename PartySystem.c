#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "PartySystem.h"
#include "set.h"

#define MAX_INT 2147483647

struct partySystem {
    Set partySet;
};

//The func get an element (a party) and return the pointer
//The func purpose is for setCreate
static SetElement copyElement(SetElement element) {
    return (Party)element;
}

//The func get two elements (parties) and compare tham by their name and code
//The func purpose is for setCreate
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
        free(party_name1);
        free(party_code1);
        free(party_name2);
        free(party_code2);
        exit(1);
    }
    int result1 = strcmp(party_code1, party_code2);
    int result2 = strcmp(party_name1, party_name2);
    free(party_name1);
    free(party_code1);
    free(party_name2);
    free(party_code2);
    return (result1*result2);
}

//The func get an element (a party) and destory it
//The func purpose is for setCreate
static void freeElement(SetElement element) {
    destroyParty((Party)element);
}

//The func get an element (a party) and return the pointer
//The func purpose is for setCreate in displayPartySystem
static void notDestroy(SetElement element) {
    return;
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
    char s = EOF;
    fscanf(file, " %s ", &s);
    if (feof(file) && s == EOF) {
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
    SetResult result = setAdd(party_system->partySet, party);
    fclose(file);
    if(result == SET_SUCCESS) {
        party = NULL;
        return SYSTEM_SUCCESS;
    }
    destroyParty(party);
    party = NULL;
    return SYSTEM_FAIL;
}

PartySystemResult deleteParty(PartySystem party_system, PartyCode prt_code) {
    assert(party_system != NULL);
    SetResult setResult = SET_NULL_ARGUMENT;
    PartyResult check;
    int party_size;
    char *party_name;
    PartyCode party_code;
    SET_FOREACH(Party, iterator, party_system->partySet) {
        check = getPartyDetails(iterator, &party_name, &party_code, &party_size);
        if(check == PARTY_FAIL) {
            free(party_name);
            free(party_code);
            return SYSTEM_FAIL;
        }
        if(strcmp(party_code, prt_code) == 0) {
            setResult = setRemove(party_system->partySet, iterator);
        }
    }
    free(party_name);
    free(party_code);
    return (setResult == SET_SUCCESS) ? SYSTEM_SUCCESS : SYSTEM_FAIL;
}

typedef int (*my_comp)(Party, Party);
my_comp comp;

static int compConverter(void* p1, void* p2) {
    return comp((Party)p1, (Party)p2);
}

PartySystemResult displayPartySystem(PartySystem party_system, int compare(Party prt1, Party prt2)) {
    assert(compare != NULL && party_system != NULL);
    comp = compare;
    Set tmpSet = setCreate(copyElement, notDestroy, compConverter);
    if(tmpSet == NULL) return SYSTEM_FAIL;

    SET_FOREACH(Party, iterator, party_system->partySet) {
        setAdd(tmpSet, iterator);
    }

    SET_FOREACH(Party, iterator, tmpSet) {
        displayParty(iterator, 0, MAX_INT);
        printf("\n--------------------------------------------------------------------------------\n");
    }
    setDestroy(tmpSet);
    return SYSTEM_SUCCESS;
}

void destroyPartySystem(PartySystem party_system) {
    setDestroy(party_system->partySet);
    free(party_system);
}