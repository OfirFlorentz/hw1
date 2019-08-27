#include <stdio.h>
#include <stdlib.h>
#include "Party.h"
#include "PartySystem.h"

#define ASSERT_FILE(file) system("cat " file)
#define ASSERT(condition) printf("%d\n", (condition))
#define TEST(num) printf("\nTEST %s\n", num);


int main()
{

   PartyResult pr1=PARTY_SUCCESS;
   bool b1;

TEST("1.1")
   Party p1=createParty("party_1.party");
   pr1=displayParty(p1, 0, 100);

TEST("1.2")
   Party p2=createParty("party_2.party");
   pr1=displayParty(p2, 0, 100);

TEST("1.3")
   pr1=addPerson(p1, "Avi_Ram", "354653423", MASCULINE, 2);
   ASSERT(pr1==PARTY_FAIL);
   
TEST("1.4")
   pr1=addPerson(p1, "Avi_Ram", "354000423", MASCULINE, 2);
   ASSERT(pr1==PARTY_SUCCESS);
TEST("1.5")
   pr1=displayParty(p1, 0, 100);
TEST("1.6")
   b1=isMember(p1, "354000423");
   ASSERT(b1==true);

TEST("1.7")
   pr1=deletePerson(p1, "354000423");
   b1=isMember(p1, "354000423");
   ASSERT(b1==false);

TEST("1.8")
   Party p3=NULL;
   int positions[] = {2,3,5,8,15,23,36};
   pr1=joinParties(&p1, &p2, &p3, positions, 7, "Party_3", "PN-G");
   pr1=displayParty(p3, 0, 100);
TEST("1.9")
   ASSERT(p1==NULL && p2==NULL);
TEST("1.10")
   pr1=saveParty(p3, "party_3_test.party");
   ASSERT_FILE("party_3_test.party");

   destroyParty(p1);
   destroyParty(p2);
   destroyParty(p3);

  return 0;
}
