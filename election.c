#include "election.h"
#include <stdio.h>

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

static bool isIdValid(int id);



Election electionCreate(); //Ron

void electionDestroy(Election election); //Ron

ElectionResult electionAddTribe (Election election, int tribe_id, const char* tribe_name); //Ron

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