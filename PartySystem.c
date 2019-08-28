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
    char s;
    fscanf(file, " %s", &s);//Need to check
    if (feof(file)) {
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
    PartyResult check;
    int party_size;
    char *party_name;
    PartyCode party_code;
    SET_FOREACH(Party, iterator, party_system->partySet) {
        check = getPartyDetails(iterator, &party_name, &party_code, &party_size);
        if(check == PARTY_FAIL) {
            return SYSTEM_FAIL;
        }
        if(!strcmp(party_code, prt_code)) {
            destroyParty(iterator);
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
    Set tmpSet = setCreate(copyElement, freeElement, compConverter);
    if(tmpSet == NULL) return SYSTEM_FAIL;

    SET_FOREACH(Party, iterator, party_system->partySet) {
        setAdd(tmpSet, iterator);
    }
    SET_FOREACH(Party, iterator, tmpSet) {
        displayParty(iterator, 0, MAX_INT);
    }
    setDestroy(tmpSet);
    printf("\n--------------------------------------------------------------------------------\n");
    return SYSTEM_SUCCESS;
}

void destroyPartySystem(PartySystem party_system) {
    setDestroy(party_system->partySet);
    free(party_system);
}

