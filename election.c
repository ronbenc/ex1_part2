#include "election.h"
#include "map.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#define PARTITION_CHAR '_'
#define NO_VOTES 0

struct election_t
{
    Map tribes;
    Map areas;
    Map votes;
};

//currently uses strings.


// static char* votesGetTribeId(char* votes_key);

// static char* votesGetAreaId(char* votes_key);

//true if id is a positive integer. false otherwise.
static inline bool isIdValid(int id)
{
    return (id>=0);
}

//true if name contains only lowercase letters and spaces (' '). false otherwise
static bool isNameValid(const char* name)
{
    assert(name != NULL);
    while (name[0] != 0)
    {
        if((name[0] < 'a' || name[0] > 'z') && (name[0] != ' '))
            return false;
        name++;
    }
    
    return true;
}

//returns an integer length. will be used in convertion of int to string
static int getIntLength(int num)
{
    assert(isIdValid(num));
    int len = 0;
    while (num>0)
    {
        num/=10;
        len++;
    }
    
    return len;
}

//converts an integer into a new allocated string
static char* intToString(int num)
{
    char* str_num = malloc(getIntLength(num) + 1);
    if(str_num == NULL)
        return NULL;
    sprintf(str_num, "%d", num);
    return str_num;
}

//converts an allocated string into an integer 
static int stringToInt(const char* str){
    return atoi(str);
}

//checks if all arguments to electionAddVote are valid
static ElectionResult argCheck(Election election, int area_id, int tribe_id, int num_of_votes){
    if(!election || !area_id || !tribe_id){
        return ELECTION_NULL_ARGUMENT;
    }
    if(tribe_id < 0 || area_id < 0){
        return ELECTION_INVALID_ID;
    }
    if(num_of_votes <= 0){
        return ELECTION_INVALID_VOTES;
    }
    if(!mapContains(election->areas, intToString(area_id))){
        return ELECTION_AREA_NOT_EXIST;
    }
    if(!mapContains(election->tribes, intToString(tribe_id))){
        return ELECTION_TRIBE_NOT_EXIST;
    }
    return ELECTION_SUCCESS;
}


//generates a unique key to votes map, which contains area and tribe voted for
//tests needed
static char* votesKeyGenerate(char* area_id, char* tribe_id){
    char partition_char = PARTITION_CHAR;
    char* ptr = &partition_char;
    int len = strlen(area_id) + strlen(tribe_id) + 2; //additional partition char & /0
    char* new_key = malloc(len*sizeof(char));
    if(new_key == NULL){
        return NULL;
    }
    strcpy(new_key, area_id);
    strcat(new_key, ptr);
    strcat(new_key, tribe_id);
    assert(strlen(new_key) == len);
    return new_key;
}


Election electionCreate() //Ron
{
    Election election = malloc(sizeof(*election));
    if(election == NULL)
        return NULL;
    
    election->tribes = mapCreate();
    election->areas = mapCreate();
    election->votes = mapCreate();

    return election;
}

void electionDestroy(Election election) //Ron
{
    mapDestroy(election->tribes);
    mapDestroy(election->areas);
    mapDestroy(election->votes);
    free(election);
}

ElectionResult electionAddTribe (Election election, int tribe_id, const char* tribe_name) //Ron
{
    if(election == NULL || tribe_name == NULL)
        return ELECTION_NULL_ARGUMENT;
    
    if(!isIdValid(tribe_id))
        return ELECTION_INVALID_ID;
    
    char* str_tribe_id = intToString(tribe_id);
    if(str_tribe_id == NULL)
        return ELECTION_OUT_OF_MEMORY;
    
    if(mapContains(election->tribes, str_tribe_id))
    {
        free(str_tribe_id);
        return ELECTION_TRIBE_ALREADY_EXIST;
    }
    
    if(!isNameValid(tribe_name))
    {
        free(str_tribe_id);
        return ELECTION_INVALID_NAME;
    }

    MapResult tmp_map_result = mapPut(election->tribes, str_tribe_id, tribe_name);
    assert(tmp_map_result != MAP_NULL_ARGUMENT); // not supposed to happen

    if(tmp_map_result != MAP_SUCCESS)
    {
        free(str_tribe_id);
        return ELECTION_OUT_OF_MEMORY;
    }
    else
    {
        free(str_tribe_id);
        return ELECTION_SUCCESS;
    }
}

ElectionResult electionAddArea(Election election, int area_id, const char* area_name) //Ron
{
    if(election == NULL || area_name == NULL)
        return ELECTION_NULL_ARGUMENT;
    
    if(!isIdValid(area_id))
        return ELECTION_INVALID_ID;
    
    char* str_area_id = intToString(area_id);
    if(str_area_id == NULL)
        return ELECTION_OUT_OF_MEMORY;
    
    if(mapContains(election->tribes, str_area_id))
    {
        free(str_area_id);
        return ELECTION_TRIBE_ALREADY_EXIST;
    }
    
    if(!isNameValid(area_name))
    {
        free(str_area_id);
        return ELECTION_INVALID_NAME;
    }

    MapResult tmp_map_result = mapPut(election->areas, str_area_id, area_name);
    assert(tmp_map_result != MAP_NULL_ARGUMENT); // not supposed to happen

    if(tmp_map_result != MAP_SUCCESS)
    {
        free(str_area_id);
        return ELECTION_OUT_OF_MEMORY;
    }
    else
    {
        free(str_area_id);
        return ELECTION_SUCCESS;
    }
}

char* electionGetTribeName (Election election, int tribe_id) //Ron
{
    if(election == NULL || !isIdValid(tribe_id))
        return NULL;
    
    char* str_tribe_id = intToString(tribe_id);
    if(str_tribe_id == NULL)
        return NULL;
    
    if(!mapContains(election->tribes, str_tribe_id))
        return NULL;
    
    char* tribe_name = mapGet(election->tribes, str_tribe_id);
    free(str_tribe_id);

    return tribe_name;
}

ElectionResult electionSetTribeName (Election election, int tribe_id, const char* tribe_name) //Ron
{
    if(election == NULL || tribe_name == NULL)
        return ELECTION_NULL_ARGUMENT;

    if(!isIdValid(tribe_id))
        return ELECTION_INVALID_ID;
    
    char* str_tribe_id = intToString(tribe_id);
    if(str_tribe_id == NULL)
        return ELECTION_OUT_OF_MEMORY;

    if(!mapContains(election->tribes, str_tribe_id))
    {
        free(str_tribe_id);
        return ELECTION_TRIBE_NOT_EXIST;
    }

    if(!isNameValid(tribe_name))
    {
        free(str_tribe_id);
        return ELECTION_INVALID_NAME;
    }
    MapResult tmp_map_result = mapPut(election->tribes, str_tribe_id, tribe_name);
    assert(tmp_map_result != MAP_NULL_ARGUMENT); // not supposed to happen

    if(tmp_map_result != MAP_SUCCESS)
    {
        free(str_tribe_id);
        return ELECTION_OUT_OF_MEMORY;
    }
    else
    {
        free(str_tribe_id);
        return ELECTION_SUCCESS;
    }
}


static void electionRemoveTribeFromTribes(Map tribes, const char* str_tribe_id)
{
    assert(tribes != NULL);
    assert(mapContains(tribes, str_tribe_id));
    mapRemove(tribes, str_tribe_id);
}

// static void electionRemoveTribeFromVotes(Election election, int tribe_id);

ElectionResult electionRemoveTribe (Election election, int tribe_id) //Seperate to static func
{
    if(election == NULL)
        return ELECTION_NULL_ARGUMENT;

    if(!isIdValid(tribe_id))
        return ELECTION_INVALID_ID;
    
    char* str_tribe_id = intToString(tribe_id);
    if(str_tribe_id == NULL)
        return ELECTION_OUT_OF_MEMORY;

    if(!mapContains(election->tribes, str_tribe_id))
    {
        free(str_tribe_id);
        return ELECTION_TRIBE_NOT_EXIST;
    }

    electionRemoveTribeFromTribes(election->tribes, str_tribe_id);
    //electionRemoveTribeFromVotes

    return ELECTION_SUCCESS;
}

static void electionRemoveAreaFromTribes(Map areas, const char* str_area_id)
{
    assert(areas != NULL);
    assert(mapContains(areas, str_area_id));
    mapRemove(areas, str_area_id);
}

// static void electionRemoveAreaFromVotes(Election election, int area_id);


ElectionResult electionRemoveAreas(Election election, AreaConditionFunction should_delete_area) //Seperate to static func
{
     if(election == NULL || should_delete_area == NULL)
        return ELECTION_NULL_ARGUMENT;
    
    Map iterator_map = mapCopy(election->areas); // a copy for iteration
    if(iterator_map == NULL)
        return ELECTION_OUT_OF_MEMORY;
    
    MAP_FOREACH(area_id_iterator,iterator_map)
    {
        int curr_area_id = stringToInt(area_id_iterator);
        if(should_delete_area(curr_area_id))
        {
            electionRemoveAreaFromTribes(election->areas, area_id_iterator);
            //electionRemoveAreaFromVotes
        }
    }

    mapDestroy(iterator_map);

    return MAP_SUCCESS;
}



Map electionComputeAreasToTribesMapping (Election election); //Itay

ElectionResult electionAddVote (Election election, int area_id, int tribe_id, int num_of_votes){
    ElectionResult argCheckResult = argCheck(election, area_id, tribe_id, num_of_votes);
    if(argCheckResult != ELECTION_SUCCESS){
        return argCheckResult;
    }
    char* curr_key = votesKeyGenerate(intToString(area_id), intToString(tribe_id));
    if(!curr_key){
        electionDestroy(election);
        return ELECTION_OUT_OF_MEMORY;
    }
    assert(election->votes && curr_key && num_of_votes); //supposed to be valid    
    
    char* prev_votes = stringToInt(mapContains(election->votes, curr_key)
                            ? mapGet(election->votes, curr_key) : NO_VOTES);
    if(!prev_votes){
        free(curr_key);
        electionDestroy(election);
        return ELECTION_OUT_OF_MEMORY;
    }    
    
    char* curr_votes = intToString(max(num_of_votes + stringToInt(prev_votes), NO_VOTES)); //in case a bigger number should be subtracted
    if(!curr_votes){
        free(curr_key);
        electionDestroy(election);
        return ELECTION_OUT_OF_MEMORY;
    }

    MapResult mapPutResult = mapPut(election->votes, curr_key, curr_votes);
    free(curr_key);
    if(mapPutResult == MAP_NULL_ARGUMENT){
        electionDestroy(election);
        return ELECTION_OUT_OF_MEMORY;
    }
    assert(mapPutResult == MAP_SUCCESS);
    return ELECTION_SUCCESS;
}

ElectionResult electionRemoveVote(Election election, int area_id, int tribe_id, int num_of_votes); //Itay