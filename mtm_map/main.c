#include <stdio.h>
#include <stdlib.h>
#include "election.h"
#include <assert.h>


int main()
{
    Election election = electionCreate();
    electionAddTribe(election, 01, "likud");
    electionAddTribe(election, 02, "kahol lavan");

    
    
    return 0;
}