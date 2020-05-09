//
// Created by Ortal on 3/12/2020.
// Updated by Ron on 08/05/2020/

#ifndef MTM_ELECTION_H
#define MTM_ELECTION_H

#include "mtm_map/map.h"

typedef struct election_t* Election;

typedef enum ElectionResult_t {
    ELECTION_OUT_OF_MEMORY,
    ELECTION_NULL_ARGUMENT,
    ELECTION_SUCCESS,
    ELECTION_INVALID_ID,
    ELECTION_TRIBE_ALREADY_EXIST,
    ELECTION_INVALID_NAME,
    ELECTION_AREA_ALREADY_EXIST,
    ELECTION_TRIBE_NOT_EXIST,
    ELECTION_AREA_NOT_EXIST,
    ELECTION_INVALID_VOTES,
	ELECTION_ERROR
} ElectionResult;

typedef bool (*AreaConditionFunction) (int);

Election electionCreate(); //Ron

void electionDestroy(Election election); //Ron

ElectionResult electionAddTribe (Election election, int tribe_id, const char* tribe_name); //Ron

ElectionResult electionAddArea(Election election, int area_id, const char* area_name); //Ron

char* electionGetTribeName (Election election, int tribe_id); //Ron

ElectionResult electionAddVote (Election election, int area_id, int tribe_id, int num_of_votes); //Itay

ElectionResult electionRemoveVote(Election election, int area_id, int tribe_id, int num_of_votes); //Itay

ElectionResult electionSetTribeName (Election election, int tribe_id, const char* tribe_name); //Ron

ElectionResult electionRemoveTribe (Election election, int tribe_id); //Seperate to static func

ElectionResult electionRemoveAreas(Election election, AreaConditionFunction should_delete_area); //Seperate to static func

Map electionComputeAreasToTribesMapping (Election election); //Itay

bool votesGetTest (char* area_id, char* tribe_id);//debug temp

void votesPrint(Election election); //debug

void areasPrint(Election election); //debug

void tribesPrint(Election election); //debug

void mapPrint(Map map);//debug


#endif //MTM_ELECTION_H