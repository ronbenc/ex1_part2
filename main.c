#include <stdio.h>
#include <stdlib.h>
#include "election.h"
#include "mtm_map/map.h"

int main()
{
    Election election = electionCreate();
    if(!election){
        return 0;
    }
    
    ElectionResult Add1 = electionAddTribe(election, 1, "likud");
    if(Add1 == ELECTION_OUT_OF_MEMORY)
    {
        electionDestroy(election);
        return 0;
    }    
    if(Add1 != ELECTION_SUCCESS)
    {
        printf("1st function failed!\n");        
        return 0;
    }
    areasPrint(election);
    printf("finished printing areas map after 1st adding\n");
    tribesPrint(election);
    printf("finished printing tribes map after 1st adding\n");


    ElectionResult Add2 = electionAddTribe(election, 2, "kahol lavan");
    if(Add2 == ELECTION_OUT_OF_MEMORY)
    {
        electionDestroy(election);
        return 0;
    }
    if(Add2 != ELECTION_SUCCESS)
    {
        printf("2nd function failed!\n");
        electionDestroy(election);
        return 0;
    }
    areasPrint(election);
    printf("finished printing areas map after 2nd adding\n");
    tribesPrint(election);
    printf("finished printing tribes map after 2nd adding\n");

    ElectionResult Add3 = electionAddArea(election, 3, "haifa");
    if(Add3 == ELECTION_OUT_OF_MEMORY)
    {
        electionDestroy(election);
        return 0;
    }
    if(Add3 != ELECTION_SUCCESS)
    {
        printf("3rd function failed!\n");
        electionDestroy(election);
        return 0;
    }
    areasPrint(election);
    printf("finished printing areas map after 3rd adding\n");
    tribesPrint(election);
    printf("finished printing tribes map after 3rd adding\n");

    ElectionResult Add4 = electionAddArea(election, 4, "tel aviv");
    if(Add4 == ELECTION_OUT_OF_MEMORY)
    {
        electionDestroy(election);
        return 0;
    }
    if(Add4 != ELECTION_SUCCESS)
    {
        printf("4th function failed!\n");
        electionDestroy(election);
        return 0;
    }
    areasPrint(election);
    printf("finished printing areas map after 4th adding\n");
    tribesPrint(election);
    printf("finished printing tribes map after 4th adding\n");

    ElectionResult Add5 = electionAddVote(election, 3, 1, 4);
    if(Add5 == ELECTION_OUT_OF_MEMORY)
    {
        printf("Add5 = ELECTION_OUT_OF_MEMORY\n");
        electionDestroy(election);
        return 0;
    }
    if(Add5 != ELECTION_SUCCESS) 
    {
        printf("5th function failed!\n");
        return 0;
    }

    ElectionResult Add6 = electionAddVote(election, 4, 2, 4);
    if(Add5 == ELECTION_OUT_OF_MEMORY)
    {
        printf("Add6 = ELECTION_OUT_OF_MEMORY\n");
        electionDestroy(election);
        return 0;
    }
    if(Add6 != ELECTION_SUCCESS)
    {
        printf("6th function failed!\n");
        return 0;
    }
    printf("printing votes map\n");
    votesPrint(election);
    electionDestroy(election);
 
    return 1;
}

    //mapPrint(electionComputeAreasToTribesMapping(election));//debug

    /*Map electionComputeResult = electionComputeAreasToTribesMapping(election);
    if(!electionComputeResult)
    {
        printf("problem in compute funtion detected!\n");
        return 0;
    }
    printf("printing final results map:\n");
    mapPrint(electionComputeResult);*/
    
/*{
    Election election = electionCreate();
    electionAddTribe(election, 01, "likud");
    electionAddTribe(election, 02, "kahol lavan");
    electionAddArea(election, 04, "haifa");
    electionAddArea(election, 03, "tel aviv");

    electionRemoveTribe(election, 02);
    electionRemoveAreas(election, *remove_func);

    return 0;
}*/
