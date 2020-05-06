#include <stdio.h>
#include <stdlib.h>
#include "election.h"
#include "mtm_map/map.h"

int main(){
    /*
    Election election = electionCreate();
    if(!election){
        return 0;
    }
    ElectionResult Add1 = electionAddTribe(election, 1, "likud");    
    if(Add1 != ELECTION_SUCCESS){
        printf("1st function failed!\n");
        return 0;
    }
    mapPrint(election->areas);
    printf("finished printing areas map after 1st adding\n");
    mapPrint(election->tribes);
    printf("finished printing tribes map after 1st adding\n");

    ElectionResult Add2 = electionAddTribe(election, 2, "kahol lavan");
    if(Add2 != ELECTION_SUCCESS){
        printf("second function failed!\n");
        return 0;
    }
    mapPrint(election->areas);
    printf("finished printing areas map after 2nd adding\n");
    mapPrint(election->tribes);
    printf("finished printing tribes map after 2nd adding\n");

    ElectionResult Add3 = electionAddArea(election, 3, "haifa");
       if(Add3 != ELECTION_SUCCESS){
        printf("3rd function failed!\n");
        return 0;
    }
    mapPrint(election->areas);
    printf("finished printing areas map after 3rd adding\n");
    mapPrint(election->tribes);
    printf("finished printing tribes map after 3rd adding\n");

    ElectionResult Add4 = electionAddArea(election, 4, "tel aviv");
       if(Add4 != ELECTION_SUCCESS){
        printf("4th function failed!\n");
        return 0;
    }
    mapPrint(election->areas);
    printf("finished printing areas map after 4th adding\n");
    mapPrint(election->tribes);
    printf("finished printing tribes map after 4th adding\n");
    
    
    Map map_temp = mapCreate();
    mapPut(map_temp, "itay", "slepian");
    mapPut(map_temp, "yuval", "slepian");
    mapPut(map_temp, "maya", "slepian");
    mapPrint(map_temp);
    Map map_temp_copy = mapCopy(map_temp);
    if(!map_temp_copy){
        printf("mapCopy function failed in main!\n");
    }
    printf("copied map is:\n");
    mapPrint(map_temp_copy);
    printf("finish check\n");//debug
    
    //mapPrint(electionComputeAreasToTribesMapping(election));//debug
    
    ElectionResult Add5 = electionAddVote(election, 3, 1, 4);
       if(Add5 != ELECTION_SUCCESS){
        printf("5th function failed!\n");
        return 0;
    }
    ElectionResult Add6 = electionAddVote(election, 4, 2, 4);
       if(Add6 != ELECTION_SUCCESS){
        printf("6th function failed!\n");
        return 0;
    }
    printf("printing votes map\n");
    mapPrint(election->votes);
    Map electionComputeResult = electionComputeAreasToTribesMapping(election);
    if(electionComputeResult != MAP_SUCCESS)
    {
        printf("problem in compute funtion detected!\n");
        return 0;
    }
    printf("printing final results map:\n");*/
    Map map = mapCreate();
    mapPrint(map);

    return 1;
}

/*
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
    
    return 0;
}*/