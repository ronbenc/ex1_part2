#include "election.h"
#include <stdio.h>
#include <assert.h>

#define PARTITION_CHAR '_'

struct election_t
{
    Map tribes;
    Map areas;
    Map votes;
};

//currently uses strings.
static char* votesKeyGenerate(char* area_id, char* tribe_id);

static char* votesGetTribeId(char* votes_key);

static char* votesGetAreaId(char* votes_key);

//true if id is a positive integer. false otherwise.
static inline bool isIdValid(int id)
{
    return (id>=0);
}

//true if name contains only lowercase letters and spaces (' '). false otherwise
static bool isNameValid(char* name)
{
    assert(name != NULL);
    while (name[0] != 0)
    {
        if((name < 'a' || name > 'z') && (name != ' '))
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


Election electionCreate() //Ron
{
    Election election = malloc(sizeof(*election));
    if(election == NULL)
        return NULL;
    
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
    if(election == NULL || tribe_id == NULL || tribe_name == NULL)
        return ELECTION_NULL_ARGUMENT;
    
    if(!isIdValid(tribe_id))
        return ELECTION_INVALID_ID;
    
    if(str_tribe_id == NULL)
        return ELECTION_OUT_OF_MEMORY;

    if(mapContains(election->tribes, str_tribe_id))
        return ELECTION_TRIBE_ALREADY_EXIST;
    
    if(!isNameValid(tribe_name))
        return ELECTION_INVALID_NAME;
    


    if(mapPut(election->tribes, str_tribe_id, tribe_name) != MAP_SUCCESS)
        return ELECTION_OUT_OF_MEMORY;
    
    return ELECTION_SUCCESS;
}

ElectionResult electionAddArea(Election election, int area_id, const char* area_name); //Ron

char* electionGetTribeName (Election election, int tribe_id); //Ron

ElectionResult electionSetTribeName (Election election, int tribe_id, const char* tribe_name); //Ron

ElectionResult electionRemoveTribe (Election election, int tribe_id); //Seperate to static func

static void electionRemoveTribeFromTribes(Election election, int tribe_id);

static void electionRemoveTribeFromVotes(Election election, int tribe_id);


ElectionResult electionRemoveAreas(Election election, AreaConditionFunction should_delete_area); //Seperate to static func

static void electionRemoveAreaFromTribes(Election election, int area_id);

static void electionRemoveAreaFromVotes(Election election, int area_id);


Map electionComputeAreasToTribesMapping (Election election); //Itay

ElectionResult electionAddVote (Election election, int area_id, int tribe_id, int num_of_votes); //Itay

ElectionResult electionRemoveVote(Election election, int area_id, int tribe_id, int num_of_votes); //Itay