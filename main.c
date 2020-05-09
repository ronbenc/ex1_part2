#include <stdio.h>
#include <stdlib.h>
#include "election.h"
#include "mtm_map/map.h"




/*
bool removalFunc(int area_id)
{
    return (area_id == 20);
}


int main()
{
    bool is_ok = votesGetTest("123", "456");
    if(!is_ok){
        printf("problem detected\n");
    }
    else{
        printf("function is good\n");
    }

    Election election = electionCreate();
    electionAddTribe(election, 1, "likud");
    electionAddTribe(election, 2, "kahol lavan");
    electionAddArea(election, 10, "haifa");
    electionAddArea(election, 20, "tel aviv");
    electionAddVote(election, 10, 1, 8);
    electionAddVote(election, 20, 2, 3);
    electionAddVote(election, 20, 1, 5);
    electionRemoveAreas(election, *removalFunc);
    electionDestroy(election);
    return 0;
}
*/

//itay's main function 090520 0045

int main()
{
    printf("hello world!\n");
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
    printf("printing areas map after adding two areas:\n");
    areasPrint(election);
    printf("printing tribes map after adding two tribes:\n");
    tribesPrint(election);

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
    printf("printing votes map after 2 votes!\n");
    votesPrint(election);

    ElectionResult Remove1 = electionRemoveVote(election, 4, 2, 1);
    if(Remove1 == ELECTION_OUT_OF_MEMORY)
    {
        printf("Remove1 = ELECTION_OUT_OF_MEMORY\n");
        electionDestroy(election);
        return 0;
    }
    if(Remove1 != ELECTION_SUCCESS)
    {
        printf("7th function failed!\n");
        electionDestroy(election);
        return 0;
    }
    printf("printing votes map after 1 remove \n");
    votesPrint(election);
    ElectionResult Remove2 = electionRemoveVote(election, 4, 2, 2);
    if(Remove2 == ELECTION_OUT_OF_MEMORY)
    {
        printf("Remove2 = ELECTION_OUT_OF_MEMORY\n");
        electionDestroy(election);
        return 0;
    }
    if(Remove2 != ELECTION_SUCCESS)
    {
        printf("8th function failed!\n");
        electionDestroy(election);
        return 0;
    }
    
    printf("printing votes map after 2 removes \n");
    votesPrint(election);
    

    printf("computing results!\n");
    Map electionComputeResult = electionComputeAreasToTribesMapping(election);
    if(!electionComputeResult)
    {
        printf("problem in compute funtion detected!\n");
        return 0;
    }
    printf("printing final results map:\n");
    mapPrint(electionComputeResult);
    mapDestroy(electionComputeResult);
    electionDestroy(election);
    return 1;
}



/*
bool remove_func(int area_id)
{
    return (area_id == 3);
    
}


{
    Election election = electionCreate();
    electionAddTribe(election, 01, "likud");
    electionAddTribe(election, 02, "kahol lavan");
    electionAddArea(election, 04, "haifa");
    electionAddArea(election, 03, "tel aviv");

    electionRemoveTribe(election, 02);
    electionRemoveAreas(election, *remove_func);

    return 0;
}*/
