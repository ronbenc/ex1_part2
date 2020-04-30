#include <stdio.h>
#include <stdlib.h>
#include "election.h"


int main()
{
    Election election = electionCreate();
    electionAddTribe(election, 01, "liz!$8l ^^ku d");
    electionAddTribe(election, 02, "kahol lavan");
    electionAddArea(election, 04, "haifa");
    electionAddArea(election, 03, "tel aviv");

    
    
    return 0;
}