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