#ifndef PARTY_SYSTEM_H_
#define PARTY_SYSTEM_H_

#include "Party.h"

//----------------------------------------------------------------------
typedef struct partySystem *PartySystem;

typedef enum partySystemResult {SYSTEM_SUCCESS , SYSTEM_FAIL} PartySystemResult;

//----------------------------------------------------------------------
// create a new empty system.
// if fail - then NULL is returned.
PartySystem createPartySystem();

//----------------------------------------------------------------------
// add a party to the system. the name of the party data file is given as a parameter.
// fail in the following cases :
// any problem with the file, the file is empty, there is any person member in the party
// that is already a member in another party in the system, there is already
// another party in the system with the same party name or party code, a memory problem.
// if there is any such fail then the system stays in its former status
// before the addParty operation 
PartySystemResult addParty(PartySystem, char *party_data_file);

//----------------------------------------------------------------------
// delete a party from the system. if the system is empty then nothing happens.
// fail in the following cases :
// no such party
PartySystemResult deleteParty(PartySystem, PartyCode prt_code);

//----------------------------------------------------------------------
// displays all parties in the system. each party is displayed with the
// function displayParty. after each display of a party, there is a new line
// and the a line containing 80 '-' and a new line in the end.
// the order in which the parties are displayed is NOT given in advance and
// it can be redefined again each time that the function is called.
// e.g. one call will display all parties sorted according to their name, then
// another call will display all parties sorted according to their size (number
// of persons participate in it.)
// if any parameter is NULL - handle with assert.
PartySystemResult displayPartySystem(PartySystem party_system, int compare(Party prt1, Party prt2));

//----------------------------------------------------------------------
//
void destroyPartySystem(PartySystem);

//----------------------------------------------------------------------

#endif
