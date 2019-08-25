#ifndef PARTY_H_
#define PARTY_H_

#include "person.h"
#include <stdbool.h>

#define BUFFER_SIZE 256
#define ONE '1'
#define NINE '9'

//----------------------------------------------------------------------
typedef enum party_result { PARTY_SUCCESS, PARTY_FAIL } PartyResult;
typedef char* PartyCode; // letter combination used for voting in elections.

typedef struct party *Party;

//----------------------------------------------------------------------
// create a new party. The data file contains all data relevant to the party.
// first line - party name. next line - letter combination code (used in voting
// in elections). in each of the next lines we have the candidates of the party.
// each candidate in a separate line. in each line we have (left to right) :
// candidate name(one word without spaces), then ID (9 digits exactly), then
// M (Masculine) of F (Feminine). between every 2 successive word in the same line
// there is 1 or more spaces. there can be also spaces in the start of a line and/or
// in the end of a line. in the end of each line there is a new line.
// all candidates appear in the file according to their position in the party.
// third line in the file gives the first candidate. 4th line in the file
// gives the 2nd candidate, and so on.
// we can assume that the file is legal, all candidates appear only once with all
// needed data, there are at least 4 candidates in the list.
// the function can fail in the following cases : file does not exist, memory problem.

Party createParty(char *party_data_file);

//----------------------------------------------------------------------
// destroy party and cleans memory if needed.
// if party==NULL then nothing happens.

void destroyParty(Party party);

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

PartyResult addPerson(Party party, char *name, char *id, Gender gender,
	int position);

//----------------------------------------------------------------------
// delete a person with the given id from the party.
// fail if such a person does not exist in the party, or id is not NULL
// but does not give a legal id value (a string with exactly 9 digit chars).
// if party or id is NULL or the person appears in the party in more than 1 position - then
// handle with assert.

PartyResult deletePerson(Party party, char *id);

//----------------------------------------------------------------------
// check if a person with the given id is a member in the given party.
// returns true or false.
// if any parameter is NULL - handle with assert.
// if id is illegal - returns false.

bool isMember(Party party, char *id);

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

PartyResult joinParties(Party* original_party_1, Party* original_party_2,
	Party* outcome_party, int position_party_2[],
	int n /*length of position_party_2*/,
	char *new_name, char *new_code);

//----------------------------------------------------------------------
// display the party (part of the party) in standard output.
// the display format is according to the format of the party data file given
// as parameter to the function partyCreate with the following exceptions :
// between any 2 successive words in the same line there appears exactly one space char, not more.
// in addition, no spaces can appear in the beginning and/or at the end of a line.
// the persons displayed here are only those from position from_position (including) to
// position to_position (including). if from_position <= size of the party then the display
// is from position 1. if to_position >= size of the party then the display is till last position
// (including).
// if party is NULL, then handle with assert.

PartyResult displayParty(Party party, int from_position, int to_position);

//----------------------------------------------------------------------
// save the party in the file whose name is given.
// all persons in all positions are saved. the file format is as described
// in displayParty format.
// fail if there is any problem concerned in creating/opening/writing to the file.
// if the file already exists, then its old contents is lost.
// if any parameter is NULL, then handle with assert.

PartyResult saveParty(Party party, char *party_data_file);

//----------------------------------------------------------------------
// check if there is any candidate that appears both in two parties.
// returns true or false.
// a candidate is identified according to its ID (ONLY). 
// if any parameter is NULL - handle with assert.
bool haveCommonMembers(Party party1, Party party2);

//----------------------------------------------------------------------
// get the name, code and size (number of persons) of a party.
// party code has exactly 9 char digits. party_name has 1 or more chars (length is
// not known in advance).
// if any parameter (both input parameter and output parameters) is NULL then handle by assert.
// fail in any other problem such as memory problem.

PartyResult getPartyDetails(Party party, char **party_name, char **party_code, int **party_size);

//----------------------------------------------------------------------
#endif // PARTY_H_
