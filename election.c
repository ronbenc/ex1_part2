#include "election.h"
//#include "mtm_map/map.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>

#define PARTITION_CHAR '_'
#define NEGATIVE(x) -1*(x)


struct election_t
{
    Map tribes;
    Map areas;
    Map votes;
};

static char* votesTribeGet(char* generated_key);
static char* votesAreaGet(char* generated_key);


//developers comment: this function exists in map.c aswell. we chose to duplicate it because it's a simple and short function that doesn"t justify a utilty file
//allocates a new string and copies given string data. returns NULL if allocation is failed
static char* copyString(const char* str)
{
    long int len = strlen(str);
    char* newStr = malloc(len + 1);

    if (newStr == NULL)
        return NULL;

    return strcpy(newStr, str);
}

//true if id is a positive integer. false otherwise.
static inline bool isIdValid(int id)
{
    return (id>=0);
}

//true if name contains only lowercase letters and/or spaces (' '). false otherwise
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
    int len = 1;
    while (num>9)
    {
        num/=10;
        len++;
    }
    
    return len;
}

//converts an integer into a new allocated string. imprtant to free allocation after use
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

Election electionCreate() //Ron
{
    Election election = malloc(sizeof(*election));
    if(election == NULL)
        return NULL;
    
    election->tribes = mapCreate();
    election->areas = mapCreate();
    election->votes = mapCreate();

    if(election->tribes == NULL || election->areas == NULL || election->votes == NULL) //if any of mapsCreate fails
    {
        electionDestroy(election);
        return NULL;
    }
    return election;
}

void electionDestroy(Election election) //Ron
{
    mapDestroy(election->tribes);
    mapDestroy(election->areas);
    mapDestroy(election->votes);
    free(election);
}

//adds an item to a specified map
static ElectionResult electionAddItemToMap(Election election, Map map, int item_id, const char* item_name)
{
    assert(election != NULL);
    if(item_name == NULL)
    {
        return ELECTION_NULL_ARGUMENT;
    }
    
    if(!isIdValid(item_id))
    {
        return ELECTION_INVALID_ID;
    }
    
    char* str_item_id = intToString(item_id);
    if(str_item_id == NULL)
    {
        return ELECTION_OUT_OF_MEMORY;
    }
    
    if(mapContains(map, str_item_id))
    {
        free(str_item_id);
        if(election->tribes == map)
        {
            return ELECTION_TRIBE_ALREADY_EXIST;
        }

        assert(election->areas == map);
        return ELECTION_AREA_ALREADY_EXIST;
    }
    
    if(!isNameValid(item_name))
    {
        free(str_item_id);
        return ELECTION_INVALID_NAME;
    }

    MapResult tmp_map_result = mapPut(map, str_item_id, item_name);
    assert(tmp_map_result != MAP_NULL_ARGUMENT); // not supposed to happen

    if(tmp_map_result != MAP_SUCCESS)
    {
        free(str_item_id);
        return ELECTION_OUT_OF_MEMORY;
    }
    else
    {
        assert(tmp_map_result == MAP_SUCCESS);
        free(str_item_id);
        return ELECTION_SUCCESS;
    }  
}

ElectionResult electionAddTribe (Election election, int tribe_id, const char* tribe_name) //Ron
{
    if(election == NULL)
        return ELECTION_NULL_ARGUMENT;

    return electionAddItemToMap(election, election->tribes, tribe_id, tribe_name);
}

ElectionResult electionAddArea(Election election, int area_id, const char* area_name) //Ron
{
    if(election == NULL)
        return ELECTION_NULL_ARGUMENT;

    return electionAddItemToMap(election, election->areas, area_id, area_name);
}


char* electionGetTribeName (Election election, int tribe_id) //Ron
{
    if(election == NULL || !isIdValid(tribe_id))
    {
        return NULL;
    }
    
    char* str_tribe_id = intToString(tribe_id);
    if(str_tribe_id == NULL)
    {
        return NULL;
    }
    
    if(!mapContains(election->tribes, str_tribe_id))
    {
        free(str_tribe_id);
        return NULL;
    }
    
    char* tribe_name = copyString(mapGet(election->tribes, str_tribe_id)); //will return NULL if allocation is failed so no need to check

    free(str_tribe_id);

    return tribe_name;
}

ElectionResult electionSetTribeName (Election election, int tribe_id, const char* tribe_name) //Ron
{
    if(election == NULL || tribe_name == NULL)
    {
        return ELECTION_NULL_ARGUMENT;
    }

    if(!isIdValid(tribe_id))
    {
        return ELECTION_INVALID_ID;
    }
    
    char* str_tribe_id = intToString(tribe_id);
    if(str_tribe_id == NULL)
    {
        return ELECTION_OUT_OF_MEMORY;
    }

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

//removes an item (key and value) from a map
static void electionRemoveItemFromMap(Map map, const char* item)
{
    assert(map != NULL && item != NULL);
    assert(mapContains(map, item));
    mapRemove(map, item);
}

//removes an item (tribe/area) and its votes from votes. 
//returns ELECTION_OUT_OF_MEMORY if allocation is failed and ELECTION_SUCCESS if removal is succesful
static ElectionResult electionRemoveItemFromVotes(Election election, const char* str_item_id, char* (*votesItemGet) (char* votes_key))
{
    assert(election != NULL && election->votes != NULL && str_item_id != NULL);
    Map iterator_map = mapCopy(election->votes);
    if(iterator_map == NULL)
    {
        return ELECTION_OUT_OF_MEMORY;
    }

    MAP_FOREACH(key_iterator, iterator_map)
    {
        char* curr_tribe = votesItemGet(key_iterator);
        if(curr_tribe == NULL)
        {
            mapDestroy(iterator_map);
            return ELECTION_OUT_OF_MEMORY;
        }

        if(strcmp(curr_tribe, str_item_id) == 0)
        {
            mapRemove(election->votes, key_iterator);
        }

        free(curr_tribe);
    }


    mapDestroy(iterator_map);

    return ELECTION_SUCCESS;
}

ElectionResult electionRemoveTribe (Election election, int tribe_id)
{
    if(election == NULL)
    {
        return ELECTION_NULL_ARGUMENT;
    }

    if(!isIdValid(tribe_id))
    {
        return ELECTION_INVALID_ID;
    }
    
    char* str_tribe_id = intToString(tribe_id);
    if(str_tribe_id == NULL)
    {
        return ELECTION_OUT_OF_MEMORY;
    }

    ElectionResult election_result = ELECTION_ERROR; //used as a flag 
    if(!mapContains(election->tribes, str_tribe_id))
    {
        election_result = ELECTION_TRIBE_NOT_EXIST;
    }

    if(election_result == ELECTION_ERROR)
    {
        electionRemoveItemFromMap(election->tribes, str_tribe_id);
        election_result = electionRemoveItemFromVotes(election, str_tribe_id, *votesTribeGet);
    }

    free(str_tribe_id);
    assert(election_result != ELECTION_ERROR);
    return election_result;
}

ElectionResult electionRemoveAreas(Election election, AreaConditionFunction should_delete_area)
{
     if(election == NULL || should_delete_area == NULL)
     {
        return ELECTION_NULL_ARGUMENT;
     }
    
    Map iterator_map = mapCopy(election->areas); // a copy for iteration
    if(iterator_map == NULL)
    {
        return ELECTION_OUT_OF_MEMORY;
    }
    
    MAP_FOREACH(area_id_iterator,iterator_map)
    {
        int curr_area_id = stringToInt(area_id_iterator);
        if(should_delete_area(curr_area_id))
        {
            electionRemoveItemFromMap(election->areas, area_id_iterator);
             if(electionRemoveItemFromVotes(election, area_id_iterator, *votesAreaGet) == ELECTION_OUT_OF_MEMORY)
             {
                mapDestroy(iterator_map); 
                return ELECTION_OUT_OF_MEMORY;
             }
        }
    }

    mapDestroy(iterator_map);
    return ELECTION_SUCCESS;
}


//string (not pointers) implementation
//extracts area id from a generated key in votes map
static char* votesAreaGet(char* generated_key)
{
    char* ptr = generated_key;
    int len = 0;
    while(*ptr != PARTITION_CHAR)
    {
        len++;
        ptr++;
    }
    char* area_id = malloc(len*(sizeof(*area_id) + 1));
    area_id[len] = '\0';
    if(!area_id)
    {
        return NULL;
    }
    strncpy(area_id, generated_key, len);   
    return area_id;
}

//extracts tribe id from a generated key in votes map
static char* votesTribeGet(char* generated_key)
{
    char* ptr = generated_key;
    int area_len = 0, tribe_len = 0;
    while(*ptr != PARTITION_CHAR)
    {
        area_len++;
        ptr++;
    }
    ptr++;
    while(*ptr != '\0')
    {
        tribe_len++;
        ptr++;
    }
    char* tribe_id = malloc(tribe_len*(sizeof(*tribe_id) + 1));
    if(!tribe_id)
    {
        return NULL;
    }
    //assert(*(generated_key + area_len + 1 + tribe_len) == "\0");//ptr to end of string
    //return strncpy(tribe_id, generated_key + area_len + 1, tribe_len + 1); debug
    return strcpy(tribe_id, generated_key + area_len +1);
}

//generates a unique key to votes map, which contains area and tribe voted for
//tests needed
static char* votesKeyGenerate(char* area_id, char* tribe_id)
{
    if(!area_id || !tribe_id){
        return NULL;
    }
    char partition_str[2] = {PARTITION_CHAR,'\0'};
    long long int len = strlen(area_id) + strlen(tribe_id) + strlen(partition_str); //additional partition char & /0
    char* new_key = malloc(len*sizeof(char) + 1);
    if(new_key == NULL)
    {
        return NULL;
    }
    strcpy(new_key, area_id);
    strcpy(new_key + strlen(area_id), partition_str);
    strcpy(new_key + strlen(area_id) + strlen(partition_str), tribe_id);
    assert(strlen(new_key) == len);
    return new_key;
}

//sets all values in a map to a certain value
static void SetAllValues(Map map, const char* value)
{
    assert(map != NULL && value != NULL);

    MAP_FOREACH(key_iterator, map)
    {
        mapPut(map, key_iterator, value);
    }
}

//returns the minimum key in map (alphabet order). assuming all keys (int values) are not negative
static char* GetMinId(Map map)
{
    if(map == NULL)
    {
        return NULL;
    }
    
    char* min_key_str = mapGetFirst(map);
    assert(min_key_str != NULL);// not supposed to be an empty map

    MAP_FOREACH(key_iterator, map)
    {
        if(strcmp(key_iterator, min_key_str) < 0)
        {
            min_key_str = key_iterator;
        }
    }

    return min_key_str;
}

// Initializes a map for electionComputeAreasToTribesMapping function.
// if tribes or areas maps are emty than initializes an empty map. otherwise initializes a map where the keys are area is's and the values are the lowest tribe id.
//returns NULL if arguments are not valid or allocation fails.
static Map InitilizeMapForComputeAreasToTribesMapping(Election election)
{
    if(election == NULL)
    {
        return NULL;
    }
    
    assert(election->tribes != NULL);
    assert(election->areas != NULL);

    Map results = mapCopy(election->areas);
    if(results == NULL)
    {
        return NULL;
    }

    if(mapGetSize(election->tribes) <=0 || mapGetSize(election->areas)<=0)
    {
        mapClear(results);// empty map
        return results;
    }

    char* min_tribe_id = GetMinId(election->tribes);
    assert(min_tribe_id != NULL);
    
    SetAllValues(results, min_tribe_id); //initialize results map to lowest id tribes

    return results;
}

Map electionComputeAreasToTribesMapping (Election election)// Ron's version
{
    Map results = InitilizeMapForComputeAreasToTribesMapping(election);

    if(results == NULL)
    {
        return NULL;
    }

    MAP_FOREACH(votes_key_iterator, election->votes)
    {
        //get current area id
        char* curr_area_id = votesAreaGet(votes_key_iterator);
        if(curr_area_id == NULL)
        {
            mapDestroy(results);
            return NULL;
        }

        //get current tribe id
        char* curr_tribe_id = votesTribeGet(votes_key_iterator);
        if(curr_tribe_id == NULL)
        {
            free(curr_area_id);
            mapDestroy(results);
            return NULL;
        }

        int curr_num_of_votes = stringToInt(mapGet(election->votes, votes_key_iterator)); 

        //get current area max votes tribe
        char* results_area_max_votes_tribe = mapGet(results, curr_area_id);
        assert(results_area_max_votes_tribe != NULL); 

        //generate a votes key for the max voted tribe for comparison with the current votes  key in order to access its votes
        char* results_area_max_vote_generated_key = votesKeyGenerate(curr_area_id, results_area_max_votes_tribe);
        if(results_area_max_vote_generated_key == NULL)
        {
            free(curr_area_id);
            free(curr_tribe_id);
            mapDestroy(results);
            return NULL;
        }

        int area_max_votes_num = 0; //set as a default in case vote doesn"t exist in Votes
        char* area_max_votes_str = mapGet(election->votes, results_area_max_vote_generated_key);
        if(area_max_votes_str != NULL)// vote exist in Votes
        {
            area_max_votes_num = stringToInt(area_max_votes_str);
        }

        //in case of equality place the lower value tribe id
        if((curr_num_of_votes > area_max_votes_num) || (curr_num_of_votes == area_max_votes_num && strcmp(curr_tribe_id, results_area_max_votes_tribe) < 0))
        {
            mapPut(results, curr_area_id, curr_tribe_id);
        }
        
        free(curr_area_id);
        free(curr_tribe_id);
        free(results_area_max_vote_generated_key);     
    }

    return results;
}

// //checks if all arguments to electionAddVote are valid
static ElectionResult electionVoteUpdateArgCheck(Election election, int area_id, int tribe_id, int num_of_votes){
    if(!election){
        return ELECTION_NULL_ARGUMENT;
    }
    if(tribe_id < 0 || area_id < 0){
        return ELECTION_INVALID_ID;
    }
    if(num_of_votes <= 0){
        return ELECTION_INVALID_VOTES;
    }
    char* area_id_str = intToString(area_id);
    if(!area_id_str)
    {
        return ELECTION_OUT_OF_MEMORY;
    }
    if(!mapContains(election->areas, area_id_str)){
        free(area_id_str);
        return ELECTION_AREA_NOT_EXIST;
    }
    char* tribe_id_str = intToString(tribe_id);
    if(!tribe_id_str)
    {
        free(area_id_str);
        return ELECTION_OUT_OF_MEMORY;
    }
    if(!mapContains(election->tribes, tribe_id_str)){
        free(area_id_str);
        free(tribe_id_str);
        return ELECTION_TRIBE_NOT_EXIST;
    }
    free(area_id_str);
    free(tribe_id_str);
    return ELECTION_SUCCESS;
}

// //updating number of votes of a certain area to a certain tribe
// //tests needed - Itay
static ElectionResult votesUpdate(Election election, char* curr_key, int num_of_votes)
{
    assert(election->votes && curr_key && num_of_votes);//number of votes must not be zero (can be negative)
    char* prev_val_str = mapGet(election->votes, curr_key);
    if(!prev_val_str)
    {
        return ELECTION_NULL_ARGUMENT;
    }
    int prev_val = stringToInt(prev_val_str);
    if(!prev_val)
    {
        return ELECTION_OUT_OF_MEMORY;
    }
    if(prev_val + num_of_votes <= 0)
    {   
        MapResult mapRemoveResult = mapRemove(election->votes, curr_key);
        if(mapRemoveResult != MAP_SUCCESS)
        {
            printf("error detected in call to mapRemove!\n");//debug
            return ELECTION_ERROR;//itay
        }
        return ELECTION_SUCCESS;
    }
    assert(prev_val >= 0);
    char* new_val_str = intToString(prev_val + num_of_votes);
    if(!new_val_str)
    {   
        return ELECTION_OUT_OF_MEMORY;
    }
    MapResult mapPutResult = mapPut(election->votes, curr_key, new_val_str);
    free(new_val_str);
    if(mapPutResult == MAP_OUT_OF_MEMORY)
    {
        return ELECTION_OUT_OF_MEMORY;
    }
    assert(mapPutResult == MAP_SUCCESS);
    return ELECTION_SUCCESS;
}

//tests needed
ElectionResult electionAddVote (Election election, int area_id, int tribe_id, int num_of_votes)
{
    ElectionResult argCheckResult = electionVoteUpdateArgCheck(election, area_id, tribe_id, num_of_votes);
    if(argCheckResult == ELECTION_OUT_OF_MEMORY)
    {        
        return ELECTION_OUT_OF_MEMORY;
    }
    if(argCheckResult != ELECTION_SUCCESS)
    {
        return argCheckResult;
    }
    char* area_id_str = intToString(area_id);
    if(!area_id_str)
    {
        return ELECTION_OUT_OF_MEMORY;
    }
    char* tribe_id_str = intToString(tribe_id);
    if(!tribe_id_str)
    {
        free(area_id_str);
        return ELECTION_OUT_OF_MEMORY;
    }
    char* curr_key = votesKeyGenerate(area_id_str, tribe_id_str);
    if(!curr_key)
    {
        free(area_id_str);
        free(tribe_id_str);
        return ELECTION_OUT_OF_MEMORY;
    }
    assert(election->votes && curr_key && num_of_votes); //supposed to be valid       
    if(mapContains(election->votes, curr_key))
    {
        ElectionResult updateSuccess = votesUpdate(election, curr_key, num_of_votes);
        if(updateSuccess != ELECTION_SUCCESS)
        {
            free(area_id_str);
            free(tribe_id_str);
            free(curr_key);
            return ELECTION_OUT_OF_MEMORY;//DEBUG!!!!
        }
        else
        {
            free(area_id_str);
            free(tribe_id_str);
            free(curr_key);
            return ELECTION_SUCCESS;
        }
    }
    else 
    {
        char* toAdd = intToString(num_of_votes);
        if(!toAdd)
        {
            free(area_id_str);
            free(tribe_id_str);
            return ELECTION_OUT_OF_MEMORY;
        }
        assert(election->votes && curr_key && toAdd);
        MapResult mapPutResult = mapPut(election->votes, curr_key, toAdd);//debug change
        if(mapPutResult != MAP_SUCCESS){
            free(area_id_str);
            free(tribe_id_str);
            free(toAdd);
            free(curr_key);
            return mapPutResult;
        }
        free(toAdd);
    }
    free(curr_key);
    free(area_id_str);
    free(tribe_id_str);
    return ELECTION_SUCCESS;
}


ElectionResult electionRemoveVote(Election election, int area_id, int tribe_id, int num_of_votes)
{
    ElectionResult argCheckResult = electionVoteUpdateArgCheck(election, area_id, tribe_id, num_of_votes);
    if(argCheckResult == ELECTION_OUT_OF_MEMORY)
    {
        return ELECTION_OUT_OF_MEMORY;
    }
    if(argCheckResult != ELECTION_SUCCESS)
    {
        return argCheckResult;
    }
    char* area_id_str = intToString(area_id);
    if(!area_id_str)
    {
        return ELECTION_OUT_OF_MEMORY;
    }
    char* tribe_id_str = intToString(tribe_id);
    if(!tribe_id_str)
    {
        free(area_id_str);
        return ELECTION_OUT_OF_MEMORY;
    }
    char* curr_key = votesKeyGenerate(area_id_str, tribe_id_str);
    if(!curr_key)
    {
        free(area_id_str);
        free(tribe_id_str);
        return ELECTION_OUT_OF_MEMORY;
    }
    assert(election->votes && curr_key && num_of_votes); //supposed to be valid
    if(mapContains(election->votes, curr_key))
    {
        ElectionResult votesUpdateResult = votesUpdate(election, curr_key, NEGATIVE(num_of_votes));
        free(area_id_str);
        free(tribe_id_str);
        free(curr_key);
        return votesUpdateResult;
    }
    else
    {
        free(area_id_str);
        free(tribe_id_str);
        free(curr_key);
        return ELECTION_SUCCESS;
    }
}

void areasPrint(Election election) //debug
{
    printf("printing areas map\n");
    MAP_FOREACH(iter, election->areas)
    {
        printf("Key: %s |||| Value: %s\n", iter, mapGet(election->areas, iter));
    }
}

void tribesPrint(Election election) //debug
{
    printf("printing tribes map\n");
    MAP_FOREACH(iter, election->tribes)
    {
        printf("Key: %s |||| Value: %s\n", iter, mapGet(election->tribes, iter));
    }
}

void votesPrint(Election election) //debug
{
    MAP_FOREACH(iter, election->votes)
    {
        printf("Key: %s |||| Value: %s\n", iter, mapGet(election->votes, iter));
    }
}

void mapPrint(Map map) //debug
{
    MAP_FOREACH(iter, map)
    {
        printf("Key: %s |||| Value: %s\n", iter, mapGet(map, iter));
    }
}