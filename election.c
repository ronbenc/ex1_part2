#include "election.h"
#include "mtm_map/map.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>

#define PARTITION_CHAR '_'
#define NO_VOTES 0
#define NEGATIVE(x) -1*(x)

struct election_t
{
    Map tribes;
    Map areas;
    Map votes;
};

static char* votesTribeGet(char* generated_key);
static char* votesAreaGet(char* generated_key);

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
    int len = 0;
    while (num>0)
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

    if(election->tribes == NULL || election->areas == NULL || election->votes == NULL)
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
    if(election == NULL || item_name == NULL)
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
    return electionAddItemToMap(election, election->tribes, tribe_id, tribe_name);
}

ElectionResult electionAddArea(Election election, int area_id, const char* area_name) //Ron
{
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
    
    char* tribe_name = mapGet(election->tribes, str_tribe_id);
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

//removes a tribe and its votes from Votes. tested
static ElectionResult electionRemoveTribeFromVotes(Election election, const char* str_tribe_id)
{
    assert(election != NULL && election->votes != NULL && str_tribe_id != NULL);
    Map iterator_map = mapCopy(election->votes);
    if(iterator_map == NULL)
        return ELECTION_OUT_OF_MEMORY;
    MAP_FOREACH(key_iterator, iterator_map)
    {
        char* curr_tribe = votesTribeGet(key_iterator);
        if(strcmp(curr_tribe, str_tribe_id) == 0)
        {
            mapRemove(election->votes, key_iterator);
        }
    }

    mapDestroy(iterator_map);

    return ELECTION_SUCCESS;
}

ElectionResult electionRemoveTribe (Election election, int tribe_id) //Seperate to static func
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

    if(!mapContains(election->tribes, str_tribe_id))
    {
        free(str_tribe_id);
        return ELECTION_TRIBE_NOT_EXIST;
    }

    electionRemoveItemFromMap(election->tribes, str_tribe_id);

    if(electionRemoveTribeFromVotes(election, str_tribe_id) == ELECTION_OUT_OF_MEMORY)
        return ELECTION_OUT_OF_MEMORY;

    return ELECTION_SUCCESS;
}

static ElectionResult electionRemoveAreaFromVotes(Election election, const char* str_area_id)
{
    assert(election != NULL && election->votes != NULL && str_area_id != NULL);
    Map iterator_map = mapCopy(election->votes);
    if(iterator_map == NULL)
        return ELECTION_OUT_OF_MEMORY;
    MAP_FOREACH(key_iterator, iterator_map)
    {
        char* curr_area = votesAreaGet(key_iterator);

        if(strcmp(curr_area, str_area_id) == 0)
        {
            mapRemove(election->votes, key_iterator);
        }
    }

    mapDestroy(iterator_map);

    return ELECTION_SUCCESS;
}

ElectionResult electionRemoveAreas(Election election, AreaConditionFunction should_delete_area) //Seperate to static func
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
             if(electionRemoveAreaFromVotes(election, area_id_iterator) == ELECTION_OUT_OF_MEMORY)
        return ELECTION_OUT_OF_MEMORY;
        }
    }

    mapDestroy(iterator_map);

    return MAP_SUCCESS;
}

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

    int id_len = strlen(area_id);
    printf("area id: %s, length: %d\n", area_id, id_len);
    
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
    char* tribe_id = malloc(tribe_len*sizeof(*tribe_id));
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
    char* new_key = malloc(len*sizeof(char));
    //printf("generated key string length is %lld\n", len);
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

//back to static / remove
bool votesGetTest (char* area_id, char* tribe_id){
    printf("input in votesGetTest: area_id is %s, tribe_id is %s\n", area_id, tribe_id);
    char* generated_key = votesKeyGenerate(area_id, tribe_id);
    printf("generated key is %s\n", generated_key);
    char* area_extract = votesAreaGet(generated_key);
    char* tribe_extract = votesTribeGet(generated_key);
    printf("area_id extracted from generated key is %s\n", area_extract);
    printf("tribe_id extracted from generated key is %s\n", tribe_extract);
    if(strcmp(area_id, area_extract) == 0){
        printf("area_id was extracted successfully, verified by strcmp!\n");
    }
    else{
        printf("area_id failed extracting, verified by strcmp!\n");
    }
    if(strcmp(tribe_id, tribe_extract) == 0){
        printf("tribe_id was extracted successfully, verified by strcmp!\n");
    }
    else{
        printf("tribe_id failed extracting, verified by strcmp!\n");
    }
    bool strcheck1 = strcmp(area_extract, area_id);
    bool strcheck2 = strcmp(tribe_extract, tribe_id);
    free(area_extract);
    free(tribe_extract);
    free(generated_key);
    return(!strcheck1 && !strcheck2);

    //return(!strcmp(votesAreaGet(generated_key), area_id) && 
      //      !strcmp(votesTribeGet(generated_key), tribe_id));
}


Map electionComputeAreasToTribesMapping (Election election)
{
    Map electionFinalResults = mapCopy(election->areas);
    if(!electionFinalResults)
    {
        printf("mapCopy in compute function failed!\n");//debug
        return NULL;
    }
    printf("mapCopy in compute function SUCCESS!\n");//debug
    MAP_FOREACH(areas_iter, election->areas)
    {
        char* max_vote = NULL;
        char* max_tribe = NULL;
        printf("we are looking for all the votes from area %s\n", areas_iter);
        MAP_FOREACH(votes_iter, election->votes)
        {
            char* curr_area = votesAreaGet(votes_iter);
            printf("we see a vote from area %s, while looking for votes from area %s\n", curr_area, areas_iter);
            if(!curr_area)
            {
                mapDestroy(electionFinalResults);
                return NULL;
            }
            if(strcmp(areas_iter, curr_area) == 0)
            {
                printf("we detected this is a vote we need to consider\n");//debug
                char* curr_vote = mapGet(election->votes, votes_iter);
                printf("Until now the max vote is %s. We just detected that area %s voted %s times\n", max_vote, curr_area, curr_vote);//debug
                if(stringToInt(curr_vote) >= stringToInt(max_vote))
                {
                    printf("we detected a new max vote\n");//debug
                    max_vote = curr_vote;
                    char* curr_tribe = votesTribeGet(votes_iter);
                    if(!curr_tribe)
                    {
                        mapDestroy(electionFinalResults);
                        return NULL;
                    }
                    printf("we just detected the tribe that has been voted was %s\n", curr_tribe);
                    if(strcmp(max_tribe, curr_tribe) > 0)
                    {
                        max_tribe = curr_tribe;
                    }
                }
            }
        }
        //no voters in the area
        if(!max_tribe)
        {
            char* no_voters_str = "00";
            printf("no_voters_str is %s\n", no_voters_str);//debug
            printf("areas_iter %s\n", areas_iter);//debug
            MapResult mapPutResult1 = mapPut(electionFinalResults, areas_iter, no_voters_str);
            if(mapPutResult1 != MAP_SUCCESS)
            {
                assert(mapPutResult1 == MAP_OUT_OF_MEMORY);
                mapDestroy(electionFinalResults);
                return NULL;
            }
        }
        //voters exist in the area
        else
        {
            MapResult mapPutResult2 = mapPut(electionFinalResults, areas_iter, max_tribe);
            if (mapPutResult2 != MAP_SUCCESS)
            {
                assert(mapPutResult2 == MAP_OUT_OF_MEMORY);
                mapDestroy(electionFinalResults);
                return NULL;
            }
        }
    }
    return electionFinalResults;
}

//checks if all arguments to electionAddVote are valid
static ElectionResult electionVoteUpdateArgCheck(Election election, int area_id, int tribe_id, int num_of_votes){
    if(!election || !area_id || !tribe_id){
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
        printf("area_id_str detected as null ptr after intToString call!\n");//debug
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

//returning the max between two integers
static int max_int(int a, int b)
{
    return ((a >= b) ? a : b);
}

//updating number of votes of a certain area to a certain tribe
//tests needed - Itay
static ElectionResult votesUpdate(Election election, char* curr_key, int num_of_votes)
{
    assert(election->votes && curr_key && num_of_votes >= 0);
    int prev_val = stringToInt(mapGet(election->votes, curr_key));
    assert(prev_val >= 0);
    char* new_val = intToString(max_int(prev_val + num_of_votes, NO_VOTES));
    if(!new_val)
    {
        free(curr_key);
        free(new_val);        
        return ELECTION_OUT_OF_MEMORY;
    }
    MapResult mapPutResult = mapPut(election->votes, curr_key, new_val);
    if(mapPutResult == MAP_OUT_OF_MEMORY)
    {
        free(curr_key);
        free(new_val);
        return ELECTION_OUT_OF_MEMORY;
    }
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
        printf("arguments to electionAddVote function invalid!\n");//debug
        return argCheckResult;
    }
    printf("arguments to electionAddVote function are valid!\n");//debug
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
    printf("area_id_str is %s, tribe_id_str is %s, let's generate!\n", area_id_str, tribe_id_str);
    char* curr_key = votesKeyGenerate(area_id_str, tribe_id_str);
    if(!curr_key)
    {
        printf("votesKeyGenerator failed!\n");//debug
        free(area_id_str);
        free(tribe_id_str);
        return ELECTION_OUT_OF_MEMORY;
    }
    printf("key was generated successfully, it is %s!\n", curr_key);//debug
    assert(election->votes && curr_key && num_of_votes); //supposed to be valid       
    if(mapContains(election->votes, curr_key))
    {
        printf("detected there are already votes from the area to the tribe!\n");
        //debug changes
       ElectionResult updateSuccess = votesUpdate(election, curr_key, num_of_votes);
       if(updateSuccess != ELECTION_SUCCESS)
        {
            printf("problem in votesUpdate function, inside implementation of electionAddVote!\n");
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
            printf("detected a problem in putting the totally new value to votes map!\n");//debug
            return mapPutResult;
        }
        free(toAdd);
    }
    free(curr_key);
    free(area_id_str);
    free(tribe_id_str);
    return ELECTION_SUCCESS;
}


//tests needed - Itay
ElectionResult electionRemoveVote(Election election, int area_id, int tribe_id, int num_of_votes)
{
    ElectionResult argCheckResult = electionVoteUpdateArgCheck(election, area_id, tribe_id, num_of_votes);
    if(argCheckResult != ELECTION_SUCCESS)
    {
        return argCheckResult;
    }
    char* curr_key = votesKeyGenerate(intToString(area_id), intToString(tribe_id));
    if(!curr_key)
    {
        electionDestroy(election);
        return ELECTION_OUT_OF_MEMORY;
    }
    assert(election->votes && curr_key && num_of_votes); //supposed to be valid       
    if(mapContains(election->votes, curr_key))
    {
        return votesUpdate(election, curr_key, NEGATIVE(num_of_votes));
    }
    else
    {
        return ELECTION_SUCCESS;
    }
}

void areasPrint(Election election) //debug
{
    MAP_FOREACH(iter, election->areas)
    {
        printf("Key: %s |||| Value: %s\n", iter, mapGet(election->areas, iter));
    }
}

void tribesPrint(Election election) //debug
{
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