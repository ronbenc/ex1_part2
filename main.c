#include <stdio.h>
#include <stdlib.h>
#include "election.h"
#include "mtm_map/map.h"


int main()
{
    Election election = electionCreate();
    electionAddTribe(election, 10, "likud");
    electionAddTribe(election, 20, "kahol lavan");
    electionAddArea(election, 1, "haifa");
    electionAddArea(election, 2, "tel aviv");
    electionAddArea(election, 3, "beer sheva");
    electionAddVote(election, 1, 20, 10); 
    electionAddVote(election, 2, 20, 10); 
    Map results = electionComputeAreasToTribesMapping(election);
    mapDestroy(results);

    electionDestroy(election);
}

