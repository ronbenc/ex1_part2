#include <stdio.h>
#include <stdlib.h>
#include "election.h"


int main()
{
    Election election = electionCreate();
    electionAddTribe(election, 01, "liz!$8l ^^ku d");
    electionAddTribe(election, 02, "kahol lavan");

    
    
    return 0;
}