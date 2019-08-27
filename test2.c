#include <stdio.h>
#include <stdlib.h>
#include "Party.h"
#include "PartySystem.h"

#define ASSERT_FILE(file) system("cat " file)
#define ASSERT(condition) printf("%d\n", (condition))
#define TEST(num) printf("\nTEST %s\n", num);

#include "test2.1.txt"

int main()
{
   PartySystemResult psr=SYSTEM_SUCCESS;
   PartySystem ps1 = createPartySystem();
   if (ps1==NULL) return 0;

TEST("2.1")
   psr=addParty(ps1, "party_1.data");
   ASSERT(psr==SYSTEM_SUCCESS);
   psr=addParty(ps1, "party_2.data");
   ASSERT(psr==SYSTEM_SUCCESS);


TEST("2.2")
// COMPLETE : display the PartySystem in a way that the parties are displayed
// from big to small. if there are parties whose size is the same then these parties
// are displayed according to their name in a lexicograph order, from small to big.

#include "test2.2.txt"

   destroyPartySystem(ps1);

  return 0;
}
