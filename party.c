#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include "list.h"
#include "person.h"
#include "party.h"


#define NAME_BUFFER 51
#define ONE '1'
#define NINE '9'
#define MIN_PEPOLE_IN_FILE 2

struct party
{
	char name[NAME_BUFFER];
	char combination_code[NAME_BUFFER];
	List party_members;
};

/*the func get an ID as an in input
*return true if its valid and the ID is not already in the list
*return false otherwise */
static bool checkId(char* id)
{
	assert(id != NULL); //not necsesry already checked
	int counter = 0;
	while (id[counter] != 0) {
		if (id[counter] < ONE || id[counter] > NINE) return false;
		counter++;
	}
	if (counter != 9) return false;
	return true;
}
//----------------------------------------------------------------------
/* the func get a line from the file and a list of pepoles and insert the data from the line to the last place
in the list*/
static void fileToPartyMember(List list, FILE* fp) {
	assert(fp != NULL && list != NULL);
	char name[NAME_BUFFER];
	char id[ID_SIZE + 1];
	int name_counter = 1;
	int digit;
	Person p;
	Gender gender;
	while (true)
	{
		digit = fgetc(fp);
		assert(digit != EOF); //empty spaces
		while (digit == ' ') {
			digit = fgetc(fp);
			assert(digit != EOF);
		}
		while (digit != ' ') { //get name
			name[name_counter++] = digit;
			digit = fgetc(fp);
			assert(digit != EOF);
		}
		while (digit == ' ') { //empty spaces
			digit = fgetc(fp);
			assert(digit != EOF);
		}
		for (int i = 0; i < ID_SIZE; i++) { //get id
			id[i] = digit;
			digit = fgetc(fp);
		}
		while (digit == ' ') { //empty spaces
			digit = fgetc(fp);
			assert(digit != EOF);
		}
		if (digit == M) {
			gender = MASCULINE;
		}
		else {
			gender = FEMININE;
		}
		p = createPerson(name, id, gender);
		listInsertLast(list, p);
		assert(p != NULL);
		free(p);
		while (digit == ' ') { //empty spaces
			digit = fgetc(fp);
		}
		if (digit == EOF) break;
		assert(digit == '\n');
	}
}
//----------------------------------------------------------------------
// changing Enum type to string. helps to SaveParty
static char* printGenderName(Gender gender)
{
	if (gender == MASCULINE) return "M"; //we need to ask isreal if it should be M or Mascoline
	return "F"; //same
}
//----------------------------------------------------------------------
// create a new party. The data file contains all data relevant to the party.
// first line - party name. next line - letter combination code (used in voting
// in elections). in each of the next lines we have the candidates of the party.
// each candidate in a separate line. in each line we have (left to right) :
// each candidate in a separate line. in each line we have (left to right) :
// candidate name(one word without spaces), then ID (9 digits exactly), then
// M (Masculine) of F (Feminine). between every 2 successive word in the same line
// there is 1 or more spaces. there can be also spaces in the start of a line and/or
// inthe end of a line. in the end of each line there is a new line.
// all candidates appear in the file according to their position in the party.
// third line in the file gives the first candidate. 4th line in the file
// gives the 2nd candidate, and so on.
// we can assume that the file is legal, all candidates appear only once with all
// needed data, there are at least 4 candidates in the list.
// the function can fail in the following cases : file does not exist, memory problem.

Party createParty(char *party_data_file) {
	FILE *fp = fopen(party_data_file, "r");
	if (fp == NULL) {
		fprintf(stdout, "file does not exists "); // should we use assert insted
		return NULL; 
	}
	Party party = malloc(sizeof(*party));
	if (!party) {
		fclose(fp);
		return NULL;
	}
	fgets(party->name, NAME_BUFFER, fp); //is it good? is +1 is neccesry
	fgets(party->combination_code, NAME_BUFFER, fp); //is it good?
	party->party_members = listCreate(copyPerson, freePerson);
	if (!party->party_members) {
		free(party);
		return NULL;
	}
	fileToPartyMember(party->party_members, fp);
	fclose(fp);
	assert (listGetSize(party->party_members) >= MIN_PEPOLE_IN_FILE); // we can delete we can assuume it and its a lot of code
	return party;
}
//----------------------------------------------------------------------
// destroy party and cleans memory if needed.
// if party==NULL then nothing happens.

void destroyParty(Party party)
{
	if (!party) return;
	listDestroy(party->party_members);
	free(party);
}


//----------------------------------------------------------------------
// add new person to the party in such a way that his (her) position
// after the adding operation is as the relevant parameter.
// all relevant entities should be saved in the Party ADT by deep copy.
// if there is already a person in that position, then all existing persons
// starting from this position move 1 position forward. if the position is
// greater than the current size of the party, then the person is added
// as the last person in the party.
// fail in the following cases : memory problem, position <=1, name
// is not NULL but gives empty string, id not NULL but does not supply
// a string with exactly 9 digit chars, there is already a person with the same id
// in the party.
// if party or name or id are NULL - then handle by assert. 


PartyResult addPerson(Party party, char *name, char *id, Gender gender, int position) {
	assert(party != NULL && name != NULL && id != NULL);
	if(!checkId(id) || position <= 1 || strcmp("", name) == 0 || isMember(party, id))  return PARTY_FAIL;
	List list_party = party->party_members;
	Person p = createPerson(name, id, gender);
	assert(p != NULL);
	for (int i = 0; i < position; i++) {
		listGetNext(list_party);
	}
	ListResult result = listInsertBeforeCurrent(list_party, p);
	freePerson(p);
	assert(result == LIST_SUCCESS);
	return PARTY_SUCCESS;
}



//----------------------------------------------------------------------
// delete a person with the given id from the party.
// fail if such a person does not exist in the party, or id is not NULL
// but does not give a legal id value (a string with exactly 9 digit chars).
// if party or id is NULL or the person appears in the party in more than 1 position - then
// handle with assert.

PartyResult deletePerson(Party party, char *id) {
	assert(party != NULL && id != NULL);
	List list_party = party->party_members;
	while (list_party != NULL) {
		if (strcmp(getId(listGetCurrent(list_party)), id) == 0) {
			listRemoveCurrent(list_party);
			return PARTY_SUCCESS;
		}
		listGetNext(list_party);
	}
	return PARTY_FAIL;
}

//----------------------------------------------------------------------
// check if a person with the given id is a member in the given party.
// returns true or false.
// if any parameter is NULL - handle with assert.
// if id is illegal - returns false.
bool isMember(Party party, char *id) {
	List list_party = party->party_members;
	while (list_party != NULL) {
		if (strcmp(getId(listGetCurrent(list_party)), id) == 0) return true;
		listGetNext(list_party);
	}
	return false;
}

//----------------------------------------------------------------------
// join two existing parties into a new outcome party.
// the outcome party has the name new_name and the code new_code (given as parameters).
// the array position_party_2 gives the positions of persons from party_2
// in the unified outcome party. if the array is shorter than the size of party_2, then
// all persons from party_2 that do not have positions in the array are positioned in the end of
// the outcome party. if the array is longer than the size of party_2, then just ignore
// the redundant positions in the array.
// if operation succeeds, then the outcome party is created and given by the output parameter
// outcome_party and the 2 original parties are destroyed and become NULL (therefore they
// are also output parameters).
// if fail, then the 2 original parties stay.
// fail if any memory problem, there is any person who appears in both parties, 
// if one of the pointer parameters is NULL, handle with assert.

PartyResult joinParties(Party* original_party_1, Party* original_party_2, Party* outcome_party, int position_party_2[],
	int n /*length of position_party_2*/, char *new_name, char *new_code) {
	assert(original_party_1 != NULL && original_party_2 != NULL && outcome_party != NULL && new_name != NULL &&
	new_code != NULL);
	if (haveCommonMembers(*original_party_1, original_party_2)) return PARTY_FAIL;
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

//----------------------------------------------------------------------
// display the party (part of the party) in standard output.
// the display format is according to the format of the party data file given
// as parameter to the function partyCreate with the following exceptions :
// between any 2 successive words in the same line there appears exactly one space char, not more.
// in addition, no spaces can appear in the beginning and/or at the end of a line.
// in addition, a person name can be of any length and is not limited to 50 chars.
// the persons displayed here are only those from position from_position (including) up to
// position to_position (including). if from_position < 1 then the display
// is from position 1. if to_position > size of the party then the display is up to (including)
// the last position. can assume here that from_position <= to_position.
// if party is NULL, then handle with assert.

PartyResult displayParty(Party party, int from_position, int to_position) {
	assert(party != NULL);
	fprintf(stdout, "%s" ,party->name);
	fprintf(stdout, "%s", party->combination_code);
	List party_list = party->party_members;
	if (from_position < 1) from_position = 1;
	for (int i = 0; i <= to_position - from_position && party_list != NULL; i++) {
		Person p = listGetCurrent(party_list);
		fprintf(stdout, "%s %s %s /n", getName(p), getId(p), printGenderName(getGender(p)));
		listGetNext(party_list);
	}
	return PARTY_SUCCESS;
}

//----------------------------------------------------------------------
// save the party in the file whose name is given.
// all persons in all positions are saved. the file format is as described
// in displayParty format.
// fail if there is any problem concerned in creating/opening/writing to the file.
// if the file already exists, then its old contents is lost.
// if any parameter is NULL, then handle with assert.

PartyResult saveParty(Party party, char *party_data_file) {
	assert(party != NULL);
	FILE* fp =  fopen(party_data_file, "w");
	if (!fp) return PARTY_FAIL;
	fputs(party->name, fp);
	fputs(party->combination_code, fp);
	List party_list = party->party_members;
	while (party_list != NULL) {
		Person p = listGetCurrent(party_list);
		fprintf(fp, "%50s %s %s /n", getName(p), getId(p), printGenderName(getGender(p)));
		listGetNext(party_list);
	}
	fclose(fp);
	return PARTY_SUCCESS;
}


//----------------------------------------------------------------------
// check if there is any candidate that appears both in two parties.
// returns true or false.
// a candidate is identified according to its ID (ONLY). 
// if any parameter is NULL - handle with assert.
bool haveCommonMembers(Party party1, Party party2) {
	List party_1_list = party1->party_members;
	while (party_1_list != NULL) {
		char* cur_id = getId(listGetCurrent(party_1_list));
		if (isMember(party2, cur_id)) return true;
		listGetNext(party_1_list);
	}
	return false;
}
//----------------------------------------------------------------------
// get the name, code and size (number of persons) of a party.
// party code has exactly 9 char digits. party_name has 1 or more chars (length is
// not known in advance).
// if any parameter (both input parameter and output parameters) is NULL then handle by assert.
// fail in any other problem such as memory problem.

//i deleted * from int *party_size

PartyResult getPartyDetails(Party party, char **party_name, char **party_code, int *party_size) {
	assert(party_name != NULL && party_code != NULL && party_size != NULL);
	*party_name = malloc(sizeof(char) * NAME_BUFFER);
	*party_code = malloc(sizeof(char) * NAME_BUFFER);
	strcpy(*party_name, party->name);
	strcpy(*party_code, party->combination_code);
	*party_size = listGetSize(party->party_members);
	return PARTY_SUCCESS;
}


