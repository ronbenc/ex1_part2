#include <stdio.h>
#include <stdlib.h>
#include "election.h"

bool remove_func(int area_id)
{
    return (area_id == 3);
    
}

int main()
{
    Election election = electionCreate();
    electionAddTribe(election, 01, "likud");
    electionAddTribe(election, 02, "kahol lavan");
    electionAddArea(election, 04, "haifa");
    electionAddArea(election, 03, "tel aviv");

    electionRemoveTribe(election, 02);
    electionRemoveAreas(election, *remove_func);
    printf("Check\n");

    return 0;
}