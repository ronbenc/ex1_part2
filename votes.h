#ifndef MTM_VOTES_H
#define MTM_VOTES_H

#include "mtm_map/map.h"

typedef struct vote_t* Votes;

// enum VotesResluts_t

Votes votesCreate();
void votesDestroy(Votes votes);
Map votesGetAreaMap(Votes votes, char* area_id);

//votesPut
// votes remove area
//votes remove tribe


#endif //MTM_VOTES_H