#include "../election.h"
#include "../mtm_map/map.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define OPTIONS 14
#define MAX_ELECTIONS 1000

typedef enum Operation_t
{
    ELECTION_CREATE = 1,
    ELECTION_DESTROY,
    ADD_TRIBE,
    ADD_AREA,
    GET_TRIBE_NAME,
    ADD_VOTE,
    REMOVE_VOTE,
    SET_TRIBE_NAME,
    REMOVE_TRIBE,
    REMOVE_AREAS,
    ELECTION_PRINT,
    TRIBES_MAP,

    ELECTION_TESTS,
    QUIT
} OperationType;
typedef struct node_tt
{
    char *key;
    char *value;
    struct node_tt *next;
} * node;

Election elections[MAX_ELECTIONS];
int area_id_for_func = 0;
int area_lower_limit = 0;
int inputs = 0;
static bool suppress_print = false;

void printMapResult(MapResult result);
void operationsOverrideInputs();
void operationCreateElection();
void operationGetTribeName();
void operationRemoveVote();
void printOptions();
void operationAddArea();
void operationAddVote();
void operationSetTribeName();
void OperationRemoveAreas();
bool RemoveAreaID(int area_id);
void operationAddTribe();
int getSelection();
void operationRemoveTribe();
int stringToIntconv(char *str);
bool RemoveAreaIDLowerLimit(int area_id);
void printElectionResult(ElectionResult result);
void printLexicographic(Map map, char *first_label, char *second_label);
void writeLine(char *text);
void operationMapping(bool with_tribe_names);
bool executeOperations();
void printElection(Election e);
void operationDestroyElection();
int operationPrintElectionsInternal();
int operationPrintElections();
int operationPrintElectionSelector();
int addElecToElecs(Election elec);
void removeFromElections(int index);

int getMaxIndexOfElectionsWithPrint();
char *getOptionText(int i);

char *getOptionText(int i)
{
    char *option_text;
    switch (i)
    {
    case ELECTION_CREATE:
        option_text = "Create Election";
        break;
    case ELECTION_DESTROY:
        option_text = "Destroy Election";
        break;
    case ADD_TRIBE:
        option_text = "Add Tribe";
        break;
    case ADD_AREA:
        option_text = "Add Area";
        break;

    case GET_TRIBE_NAME:
        option_text = "Get Tribe Name";
        break;
    case ADD_VOTE:
        option_text = "Add Vote";
        break;
    case REMOVE_VOTE:
        option_text = "Remove Vote";
        break;
    case SET_TRIBE_NAME:
        option_text = "Set Tribe Name";
        break;
    case REMOVE_TRIBE:
        option_text = "Remove Tribe";
        break;

    case REMOVE_AREAS:
        option_text = "Remove Areas";
        break;
    case TRIBES_MAP:
        option_text = "Areas-to-Tribes mapping";
        break;
    case ELECTION_PRINT:
        option_text = "Election print";
        break;
    case ELECTION_TESTS:
        option_text = "Run Tests Cases";
        break;
    default:
    case QUIT:
        option_text = "Quit";
        break;
    }

    return option_text;
}
void printOptions()
{
    printf("Please select option: [1-%d]\n", OPTIONS);
    char *option_text;
    for (int i = 1; i <= OPTIONS; i++)
    {
        option_text = getOptionText(i);
        printf("%d - %s\n", i, option_text);
    }
}
int getSelection(int lower, int upper, int exception)
{
    int option = -1;
    while (!scanf("%d", &option))
    {
        inputs++;
        printf("Invalid input - input N.%d\n", inputs);
    }
    printf("Got selection : %d\n", option);
    inputs++;
    return option;
}
bool executeOperations()
{
    int selection = getSelection(1, OPTIONS, 1);
    printf("Chosen operation : %s\n\n", getOptionText(selection));
    switch (selection)
    {
    case ELECTION_CREATE:
        operationCreateElection();
        break;
    case ELECTION_DESTROY:
        operationDestroyElection();
        break;
    case ADD_TRIBE:
        operationAddTribe();
        break;

    case ADD_AREA:
        operationAddArea();
        break;
    case GET_TRIBE_NAME:
        operationGetTribeName();
        break;
    case ADD_VOTE:
        operationAddVote();
        break;
    case REMOVE_VOTE:
        operationRemoveVote();
        break;
    case SET_TRIBE_NAME:
        operationSetTribeName();
        break;
    case REMOVE_TRIBE:
        operationRemoveTribe();
        break;
    case REMOVE_AREAS:
        OperationRemoveAreas();
        break;
    case TRIBES_MAP:
        operationMapping(false);
        break;
    case ELECTION_PRINT:
        operationMapping(true);
        break;
    case ELECTION_TESTS:
        operationsOverrideInputs();
        break;
    default:
    case QUIT:
        return true;
    }
    return false;
}
void getTribeNameAndFree(Election elec,int tribe_id)
{
       char* temp =electionGetTribeName(elec, tribe_id);
       printf("Got tribe name: %s\n",temp);
       free(temp);
}
void operationsOverrideInputs()
{
    writeLine("electionCreate();");
    Election elec = electionCreate();

    writeLine(" electionDestroy(elec);");
    electionDestroy(elec);

    writeLine(" electionDestroy(NULL);");
    electionDestroy(NULL);
    for (int i = 0; i < 3; i++)
    {
        writeLine("electionCreate();");
        elec = electionCreate();
        if (i == 1)
        {

            writeLine("electionAddArea(elec, 1,  area 1 );");
            printElectionResult(electionAddArea(elec, 1, "area 1"));
            printElection(elec);
            writeLine("electionAddArea(elec, 2,  area 2 );");
            printElectionResult(electionAddArea(elec, 2, "area 2"));
            printElection(elec);
            writeLine("electionAddTribe(elec, 1001,  tribe 1 );");
            printElectionResult(electionAddTribe(elec, 1001, "tribe 1"));
            printElection(elec);
            writeLine("electionAddTribe(elec, 1002,  tribe 2 );");
            printElectionResult(electionAddTribe(elec, 1002, "tribe 2"));
            printElection(elec);
            writeLine("electionAddVote(elec, 1, 1001, 20);");
            printElectionResult(electionAddVote(elec, 1, 1001, 20));
            printElection(elec);
            writeLine("electionAddVote(elec, 1, 1002, 10);");
            printElectionResult(electionAddVote(elec, 1, 1002, 10));
            printElection(elec);
            writeLine("electionAddVote(elec, 2, 1002, 40);");
            printElectionResult(electionAddVote(elec, 2, 1002, 40));
            printElection(elec);
            writeLine("electionAddVote(elec, 2, 1001, 30);");
            printElectionResult(electionAddVote(elec, 2, 1001, 30));
            printElection(elec);
        }
        if (i == 2)
        {
            writeLine("printElectionResult(electionAddTribe(elec, 1001,  tribe 1 );");
            printElectionResult(electionAddTribe(elec, 1001, "tribe 1"));
            printElection(elec);
            writeLine("printElectionResult(electionAddTribe(elec, 1002,  tribe 2 );");
            printElectionResult(electionAddTribe(elec, 1002, "tribe 2"));
            printElection(elec);
            writeLine("printElectionResult(electionAddArea(elec, 1,  area 1 );");
            printElectionResult(electionAddArea(elec, 1, "area 1"));
            printElection(elec);
            writeLine("printElectionResult(electionAddArea(elec, 2,  area 2 );");
            printElectionResult(electionAddArea(elec, 2, "area 2"));
            printElection(elec);
            writeLine("");

            printElection(elec);
            writeLine("electionAddVote(elec, 1, 1001, 20);");
            printElectionResult(electionAddVote(elec, 1, 1001, 20));
            printElection(elec);
            writeLine("printElectionResult(electionAddVote(elec, 1, 1002, 10);");
            printElectionResult(electionAddVote(elec, 1, 1002, 10));
            printElection(elec);
            writeLine("printElectionResult(electionAddVote(elec, 2, 1002, 40);");
            printElectionResult(electionAddVote(elec, 2, 1002, 40));
            printElection(elec);
            writeLine("printElectionResult(electionAddVote(elec, 2, 1001, 30);");
            printElectionResult(electionAddVote(elec, 2, 1001, 30));
            printElection(elec);
        }
        writeLine("printElectionResult(electionAddTribe(elec, 2000,  tribe test );");
        printElectionResult(electionAddTribe(elec, 2000, "tribe test"));
        printElection(elec);
        writeLine("printElectionResult(electionAddTribe(elec, 2000,  tribe test );");
        printElectionResult(electionAddTribe(elec, 2000, "tribe test"));
        printElection(elec);
        writeLine("printElectionResult(electionAddTribe(NULL, 3000,  tribe test );");
        printElectionResult(electionAddTribe(NULL, 3000, "tribe test"));
        printElection(elec);
        writeLine("printElectionResult(electionAddTribe(elec, 3000, NULL);");
        printElectionResult(electionAddTribe(elec, 3000, NULL));
        printElection(elec);
        writeLine("printElectionResult(electionAddTribe(elec, -1,  tribe test );");
        printElectionResult(electionAddTribe(elec, -1, "tribe test"));
        printElection(elec);
        writeLine("printElectionResult(electionAddTribe(elec, 0,  tribe test );");
        printElectionResult(electionAddTribe(elec, 0, "tribe test"));
        printElection(elec);
        writeLine("printElectionResult(electionAddTribe(elec, 3000,  Tribe test );");
        printElectionResult(electionAddTribe(elec, 3000, "Tribe test"));
        printElection(elec);
        writeLine("printElectionResult(electionAddTribe(elec, 3000,  tribe-test );");
        printElectionResult(electionAddTribe(elec, 3000, "tribe-test"));
        printElection(elec);
        writeLine("printElectionResult(electionAddTribe(elec, 2000,  ? );");
        printElectionResult(electionAddTribe(elec, 2000, "?"));
        printElection(elec);
        writeLine("printElectionResult(electionAddTribe(elec, 2000,   );");
        printElectionResult(electionAddTribe(elec, 2000, ""));
        printElection(elec);
        writeLine("printElectionResult(electionAddTribe(NULL, -1,  tribe test );");
        printElectionResult(electionAddTribe(NULL, -1, "tribe test"));
        printElection(elec);
        writeLine("printElectionResult(electionAddTribe(NULL, 3000, NULL);");
        printElectionResult(electionAddTribe(NULL, 3000, NULL));
        printElection(elec);
        writeLine("printElectionResult(electionAddTribe(NULL, -1,  Tribe test );");
        printElectionResult(electionAddTribe(NULL, -1, "Tribe test"));
        printElection(elec);
        writeLine("");

        printElection(elec);
        writeLine("printElectionResult(electionAddArea(elec, 10000,  area test );");
        printElectionResult(electionAddArea(elec, 10000, "area test"));
        printElection(elec);
        writeLine("printElectionResult(electionAddArea(elec, 10000,  area test );");
        printElectionResult(electionAddArea(elec, 10000, "area test"));
        printElection(elec);
        writeLine("printElectionResult(electionAddArea(NULL, 10001,  area test );");
        printElectionResult(electionAddArea(NULL, 10001, "area test"));
        printElection(elec);
        writeLine("printElectionResult(electionAddArea(elec, 10001, NULL);");
        printElectionResult(electionAddArea(elec, 10001, NULL));
        printElection(elec);
        writeLine("printElectionResult(electionAddArea(elec, -1,  area test );");
        printElectionResult(electionAddArea(elec, -1, "area test"));
        printElection(elec);
        writeLine("printElectionResult(electionAddArea(elec, 0,  area test );");
        printElectionResult(electionAddArea(elec, 0, "area test"));
        printElection(elec);
        writeLine("printElectionResult(electionAddArea(elec, 10001,  Area test );");
        printElectionResult(electionAddArea(elec, 10001, "Area test"));
        printElection(elec);
        writeLine("printElectionResult(electionAddArea(elec, 10001,  area-test );");
        printElectionResult(electionAddArea(elec, 10001, "area-test"));
        printElection(elec);
        writeLine("printElectionResult(electionAddArea(elec, 10000,  ? );");
        printElectionResult(electionAddArea(elec, 10000, "?"));
        printElection(elec);
        writeLine("printElectionResult(electionAddArea(elec, 10000,   );");
        printElectionResult(electionAddArea(elec, 10000, ""));
        printElection(elec);
        writeLine("printElectionResult(electionAddArea(NULL, -1,  area test );");
        printElectionResult(electionAddArea(NULL, -1, "area test"));
        printElection(elec);
        writeLine("printElectionResult(electionAddArea(NULL, 10001, NULL);");
        printElectionResult(electionAddArea(NULL, 10001, NULL));
        printElection(elec);
        writeLine("printElectionResult(electionAddArea(NULL, -1,  Area test );");
        printElectionResult(electionAddArea(NULL, -1, "Area test"));
        printElection(elec);
        writeLine("");

        printElection(elec);
        writeLine("getTribeNameAndFree(elec, 2000);");
        getTribeNameAndFree(elec, 2000);
        printElection(elec);
        writeLine("getTribeNameAndFree(elec, 1001);");
        getTribeNameAndFree(elec, 1001);
        printElection(elec);
        writeLine("getTribeNameAndFree(elec, 23);");
        getTribeNameAndFree(elec, 23);
        printElection(elec);
        writeLine("getTribeNameAndFree(NULL, 1001);");
        getTribeNameAndFree(NULL, 1001);
        printElection(elec);
        writeLine("getTribeNameAndFree(NULL, -1);");
        getTribeNameAndFree(NULL, -1);
        printElection(elec);
        writeLine("getTribeNameAndFree(NULL, 0);");
        getTribeNameAndFree(NULL, 0);
        printElection(elec);
        writeLine("getTribeNameAndFree(elec, -1);");
        getTribeNameAndFree(elec, -1);
        printElection(elec);
        writeLine("getTribeNameAndFree(elec, 0);");
        getTribeNameAndFree(elec, 0);
        printElection(elec);
        writeLine("");

        printElection(elec);
        writeLine("printElectionResult(electionAddVote(elec,10000,2000,0);");
        printElectionResult(electionAddVote(elec, 10000, 2000, 0));
        printElection(elec);
        writeLine("printElectionResult(electionAddVote(elec,10000,3000,0);");
        printElectionResult(electionAddVote(elec, 10000, 3000, 0));
        printElection(elec);
        writeLine("printElectionResult(electionAddVote(elec,10000,-1,0);");
        printElectionResult(electionAddVote(elec, 10000, -1, 0));
        printElection(elec);
        writeLine("printElectionResult(electionAddVote(elec,-1,3000,0);");
        printElectionResult(electionAddVote(elec, -1, 3000, 0));
        printElection(elec);
        writeLine("printElectionResult(electionAddVote(elec,10000,2000,1);");
        printElectionResult(electionAddVote(elec, 10000, 2000, 1));
        printElection(elec);
        writeLine("printElectionResult(electionAddVote(elec,10000,2000,-1);");
        printElectionResult(electionAddVote(elec, 10000, 2000, -1));
        printElection(elec);
        writeLine("printElectionResult(electionAddVote(elec,10000,2500,10);");
        printElectionResult(electionAddVote(elec, 10000, 2500, 10));
        printElection(elec);
        writeLine("printElectionResult(electionAddVote(elec,10000,2500,-1);");
        printElectionResult(electionAddVote(elec, 10000, 2500, -1));
        printElection(elec);
        writeLine("printElectionResult(electionAddVote(elec,10030,2500,10);");
        printElectionResult(electionAddVote(elec, 10030, 2500, 10));
        printElection(elec);
        writeLine("printElectionResult(electionAddVote(elec,10030,2500,-10);");
        printElectionResult(electionAddVote(elec, 10030, 2500, -10));
        printElection(elec);
        writeLine("printElectionResult(electionAddVote(NULL,10000,2000,0);");
        printElectionResult(electionAddVote(NULL, 10000, 2000, 0));
        printElection(elec);
        writeLine("");

        printElection(elec);
        writeLine("electionRemoveVote(elec,10000,2000,0);");
        printElectionResult(electionRemoveVote(elec, 10000, 2000, 0));
        printElection(elec);
        writeLine("printElectionResult(electionRemoveVote(elec,10000,3000,0);");
        printElectionResult(electionRemoveVote(elec, 10000, 3000, 0));
        printElection(elec);
        writeLine("printElectionResult(electionRemoveVote(elec,10000,2000,1);");
        printElectionResult(electionRemoveVote(elec, 10000, 2000, 1));
        printElection(elec);
        writeLine("printElectionResult(electionRemoveVote(elec,-1,3000,0);");
        printElectionResult(electionRemoveVote(elec, -1, 3000, 0));
        printElection(elec);
        writeLine("printElectionResult(electionRemoveVote(elec,10000,-1,1);");
        printElectionResult(electionRemoveVote(elec, 10000, -1, 1));
        printElection(elec);
        writeLine("printElectionResult(electionRemoveVote(elec,10000,2000,-1);");
        printElectionResult(electionRemoveVote(elec, 10000, 2000, -1));
        printElection(elec);
        writeLine("printElectionResult(electionRemoveVote(elec,10000,2500,10);");
        printElectionResult(electionRemoveVote(elec, 10000, 2500, 10));
        printElection(elec);
        writeLine("printElectionResult(electionRemoveVote(elec,10000,2500,-1);");
        printElectionResult(electionRemoveVote(elec, 10000, 2500, -1));
        printElection(elec);
        writeLine("printElectionResult(electionRemoveVote(elec,10030,2500,10);");
        printElectionResult(electionRemoveVote(elec, 10030, 2500, 10));
        printElection(elec);
        writeLine("printElectionResult(electionRemoveVote(elec,10030,2500,-10);");
        printElectionResult(electionRemoveVote(elec, 10030, 2500, -10));
        printElection(elec);
        writeLine("printElectionResult(electionRemoveVote(NULL,10000,2000,0);");
        printElectionResult(electionRemoveVote(NULL, 10000, 2000, 0));
        printElection(elec);
        writeLine("");

        printElection(elec);
        writeLine("printElectionResult(electionAddArea(elec,61, area sixty one );");
        printElectionResult(electionAddArea(elec, 61, "area sixty one"));
        printElection(elec);
        writeLine("printElectionResult(electionRemoveVote(elec,61,3000,100);");
        printElectionResult(electionRemoveVote(elec, 61, 3000, 100));
        printElection(elec);
        writeLine("printElectionResult(electionAddVote(elec,61,3000,200);");
        printElectionResult(electionAddVote(elec, 61, 3000, 200));
        printElection(elec);
        writeLine("electionRemoveVote(elec,61,3000,300);");
        printElectionResult(electionRemoveVote(elec, 61, 3000, 300));
        printElection(elec);
        writeLine("");

        printElection(elec);
        writeLine("");

        printElection(elec);
        writeLine("printElectionResult(electionSetTribeName(elec,2000, new name );");
        printElectionResult(electionSetTribeName(elec, 2000, "new name"));
        printElection(elec);
        writeLine("printElectionResult(electionRemoveAreas(elec,2000, New name );");
        printElectionResult(electionSetTribeName(elec, 2000, "New name"));
        printElection(elec);
        writeLine("printElectionResult(electionRemoveAreas(elec,2000, new_name );");
        printElectionResult(electionSetTribeName(elec, 2000, "new_name"));
        printElection(elec);
        writeLine("printElectionResult(electionRemoveAreas(elec,2000, ? );");
        printElectionResult(electionSetTribeName(elec, 2000, "?"));
        printElection(elec);
        writeLine("printElectionResult(electionRemoveAreas(elec,2000,  );");
        printElectionResult(electionSetTribeName(elec, 2000, ""));
        printElection(elec);
        writeLine("printElectionResult(electionRemoveAreas(elec,2000, back );");
        printElectionResult(electionSetTribeName(elec, 2000, "back"));
        printElection(elec);
        writeLine("printElectionResult(electionRemoveAreas(NULL,2000, backagain );");
        printElectionResult(electionSetTribeName(NULL, 2000, "backagain"));
        printElection(elec);
        writeLine("printElectionResult(electionRemoveAreas(NULL,2000,NULL);");
        printElectionResult(electionSetTribeName(NULL, 2000, NULL));
        printElection(elec);
        writeLine("printElectionResult(electionRemoveAreas(elec,1111, backagain );");
        printElectionResult(electionSetTribeName(elec, 1111, "backagain"));
        printElection(elec);
        writeLine("printElectionResult(electionRemoveAreas(elec,1111, A );");
        printElectionResult(electionSetTribeName(elec, 1111, "A"));
        printElection(elec);
        writeLine("printElectionResult(electionRemoveAreas(NULL,1111,NULL);");
        printElectionResult(electionSetTribeName(NULL, 1111, NULL));
        printElection(elec);
        writeLine("printElectionResult(electionRemoveAreas(elec,-1, new name );");
        printElectionResult(electionSetTribeName(elec, -1, "new name"));
        printElection(elec);
        writeLine("printElectionResult(electionRemoveAreas(elec,-1, New name );");
        printElectionResult(electionSetTribeName(elec, -1, "New name"));
        printElection(elec);
        writeLine("");

        printElection(elec);
        writeLine("electionRemoveTribe(elec,1111);");
        printElectionResult(electionRemoveTribe(elec, 1111));
        printElection(elec);
        writeLine("electionRemoveTribe(elec,2000);");
        printElectionResult(electionRemoveTribe(elec, 2000));
        printElection(elec);
        writeLine("printElectionResult(electionRemoveTribe(elec,-1);");
        printElectionResult(electionRemoveTribe(elec, -1));
        printElection(elec);
        writeLine("printElectionResult(electionRemoveTribe(NULL,3000);");
        printElectionResult(electionRemoveTribe(NULL, 3000));
        printElection(elec);
        writeLine("printElectionResult(electionRemoveTribe(NULL,-1)");
        printElectionResult(electionRemoveTribe(NULL, -1));
        printElection(elec);

        writeLine("area_id_for_func = 10000;");
        area_id_for_func = 10000;
        writeLine("printElectionResult(electionRemoveAreas(elec, RemoveAreaID);");
        printElectionResult(electionRemoveAreas(elec, RemoveAreaID));
        printElection(elec);

        writeLine("area_id_for_func = 10101;");
        area_id_for_func = 10101;
        writeLine("printElectionResult(electionRemoveAreas(elec, RemoveAreaID);");
        printElectionResult(electionRemoveAreas(elec, RemoveAreaID));
        printElection(elec);
        writeLine("printElectionResult(electionRemoveAreas(NULL, RemoveAreaID);");
        printElectionResult(electionRemoveAreas(NULL, RemoveAreaID));
        printElection(elec);

        writeLine("printElectionResult(electionAddTribe(elec, 20000,  testy );");
        printElectionResult(electionAddTribe(elec, 20000, "testy"));
        printElection(elec);
        writeLine("printElectionResult(electionAddTribe(elec, 20001,  testy );");
        printElectionResult(electionAddTribe(elec, 20001, "testy"));
        printElection(elec);
        writeLine("printElectionResult(electionAddTribe(elec, 20002,  testy );");
        printElectionResult(electionAddTribe(elec, 20002, "testy"));
        printElection(elec);
        writeLine("printElectionResult(electionAddTribe(elec, 20003,  testy );");
        printElectionResult(electionAddTribe(elec, 20003, "testy"));
        printElection(elec);

        writeLine("");

        printElection(elec);
        writeLine("area_lower_limit = 20000;");
        area_lower_limit = 20000;
        writeLine("printElectionResult(electionRemoveAreas(NULL, RemoveAreaIDLowerLimit);");
        printElectionResult(electionRemoveAreas(NULL, RemoveAreaIDLowerLimit));
        printElection(elec);
        writeLine("electionRemoveAreas(elec, RemoveAreaIDLowerLimit);");
        printElectionResult(electionRemoveAreas(elec, RemoveAreaIDLowerLimit));
        printElection(elec);

        writeLine("free(electionComputeAreasToTribesMapping(elec));");
        mapDestroy(electionComputeAreasToTribesMapping(elec));
        printElection(elec);
        writeLine("free(electionComputeAreasToTribesMapping(NULL));");
        mapDestroy(electionComputeAreasToTribesMapping(NULL));
        printElection(elec);

        printElection(elec);
        printElection(NULL);

        for (int k = 50; k < 300; k++)
        {
            printf("printElectionResult( electionAddArea(elec,%d,fif));\n", k);
            printElectionResult(electionAddArea(elec, k, "fif"));

            printf("printElectionResult(electionAddTribe( electionAddArea(elec,%d,fif));\n", k);
            printElectionResult(electionAddTribe(elec, k, "fif"));
        }
        printElection(elec);
        Election elec_empty = electionCreate();
        writeLine("printElection(elec_empty);");
        printElection(elec_empty);
        printElection(elec_empty);
        writeLine("electionAddTribe(elec_empty, 12,  agag );");
        printElectionResult(electionAddTribe(elec_empty, 12, "agag"));
        printElection(elec_empty);
        writeLine("printElection(elec_empty);");
        printElection(elec_empty);
        printElection(elec_empty);
        writeLine("electionAddArea(elec_empty, 101,  area );");
        printElectionResult(electionAddArea(elec_empty, 101, "area"));
        printElection(elec_empty);
        writeLine("printElection(elec_empty);");
        printElection(elec_empty);
        printElection(elec_empty);
        writeLine("printElectionResult(electionRemoveTribe(elec_empty, 12);");
        printElectionResult(electionRemoveTribe(elec_empty, 12));
        printElection(elec_empty);
        writeLine("printElection(elec_empty);");
        printElection(elec_empty);

        writeLine(" electionDestroy(elec_empty);");
        electionDestroy(elec_empty);
        writeLine(" electionDestroy(elec);");
        electionDestroy(elec);

        // printElection(elec);
        // printElection(elec_empty);
    }
}
void printElection(Election e)
{
    Map mapping = electionComputeAreasToTribesMapping(e);
    if (!mapping)
        printf("Map is NULL\n");
    printLexicographic(mapping, "Area ID", "Tribe ID");
    printf("As for tribes: (Based on mapping, don't assume these are all the tribes)\n\n");
    Map tribes = mapCreate();
    MAP_FOREACH(area_id, mapping)
    {
        char *tribe_id = mapGet(mapping, area_id);
        char *tribe_name = electionGetTribeName(e, stringToIntconv(tribe_id));
        
        mapPut(tribes, tribe_id, tribe_name);
        free(tribe_name);
    }
    mapDestroy(mapping);
    printLexicographic(tribes, "Tribe ID", "Tribe Name");
    mapDestroy(tribes);
}
void writeLine(char *text)
{
    printf("%s\n", text);
}
void operationMapping(bool with_tribe_names)
{
    int maxi = getMaxIndexOfElectionsWithPrint();
    if (maxi == -1)
    {
        return;
    }
    printf("Which election would you like to map? [0-%d]\n", maxi);
    int selection = getSelection(0, maxi, 0);
    inputs++;
    Map mapping = electionComputeAreasToTribesMapping(elections[selection]);
    if (!mapping)
        printf("Map is NULL\n");
    printLexicographic(mapping, "Area ID", "Tribe ID");

    if (!with_tribe_names)
    {
        mapDestroy(mapping);
        return;
    }
    printf("As for tribes: (Based on mapping, don't assume these are all the tribes)\n\n");
    Map tribes = mapCreate();
    MAP_FOREACH(area_id, mapping)
    {
        char *tribe_id = mapGet(mapping, area_id);
      char *tribe_name = electionGetTribeName(elections[selection], stringToIntconv(tribe_id));
        mapPut(tribes, tribe_id, tribe_name);
        free(tribe_name);
    }
    mapDestroy(mapping);
    printLexicographic(tribes, "Tribe ID", "Tribe Name");
    mapDestroy(tribes);
    //printMapResult(status);
}

int stringToIntconv(char *str)
{
    return atoi(str);
}

void operationSetTribeName()
{
    int maxi = getMaxIndexOfElectionsWithPrint();
    if (maxi == -1)
    {
        return;
    }
    printf("From which election would you like to set tribe name? [0-%d]\n", maxi);
    int selection = getSelection(0, maxi, 0);

    int tribe_id = 0;
    printf("Which tribe would you like to name? (enter tribe ID)\n");
    scanf("%d", &tribe_id);
    printf("Got tribe_id : %d\n", tribe_id);

    char tribe_name[50];
    printf("Enter new name:\n");
    scanf("%s", tribe_name);
    printf("Got name : %s\n", tribe_name);
    inputs++;

    printElectionResult(electionSetTribeName(elections[selection], tribe_id, tribe_name));
}

void OperationRemoveAreas()
{
    int maxi = getMaxIndexOfElectionsWithPrint();
    if (maxi == -1)
    {
        return;
    }
    printf("From which election would you like to remove areas? [0-%d]\n", maxi);
    int selection = getSelection(0, maxi, 0);
    printf("On what condition would you like to remove areas?\n1 - Area ID equals to\n2 - Area ID is larger than\n");
    int sub_selection = getSelection(1, 2, 0);
    if (sub_selection == 1)
    {
        area_id_for_func = 0;
        printf("Which area would you like to remove? (enter area ID)\n");
        scanf("%d", &area_id_for_func);
        printf("Got area_id_for_func : %d\n", area_id_for_func);
        inputs++;

        printElectionResult(electionRemoveAreas(elections[selection], RemoveAreaID));
    }
    else if (sub_selection == 2)
    {
        area_lower_limit = 0;
        printf("What is the lower limit for removing (inclusive) an area? (enter area ID)\n");
        scanf("%d", &area_lower_limit);
        printf("Got area_lower_limit : %d\n", area_lower_limit);
        inputs++;

        printElectionResult(electionRemoveAreas(elections[selection], RemoveAreaIDLowerLimit));
    }
}
bool RemoveAreaIDLowerLimit(int area_id)
{
    return area_id >= area_lower_limit;
}
bool RemoveAreaID(int area_id)
{
    return area_id == area_id_for_func;
}
void operationRemoveTribe()
{
    int maxi = getMaxIndexOfElectionsWithPrint();
    if (maxi == -1)
    {
        return;
    }
    printf("From which election would you like to remove tribe? [0-%d]\n", maxi);
    int selection = getSelection(0, maxi, 0);

    int tribe_id = 0;
    printf("Which tribe would you like to remove? (enter tribe ID)\n");
    scanf("%d", &tribe_id);
    printf("Got tribe_id : %d\n", tribe_id);
    inputs++;

    printElectionResult(electionRemoveTribe(elections[selection], tribe_id));
}
void operationGetTribeName()
{
    int maxi = getMaxIndexOfElectionsWithPrint();
    if (maxi == -1)
    {
        return;
    }
    printf("From which election would you like to get tribe name? [0-%d]\n", maxi);
    int selection = getSelection(0, maxi, 0);

    int tribe_id = 0;
    printf("Please enter tribe ID:\n");
    scanf("%d", &tribe_id);

    printf("Got tribe_id : %d\n", tribe_id);
   getTribeNameAndFree(elections[selection], tribe_id);
}
void operationRemoveVote()
{
    int maxi = getMaxIndexOfElectionsWithPrint();
    if (maxi == -1)
    {
        return;
    }
    printf("To which election would you like to remove votes? [0-%d]\n", maxi);

    int selection = getSelection(0, maxi, 0);
    printf("To which area would you like to remove (enter area_id)?\n");

    int area_id = 0;
    while (!scanf("%d", &area_id))
    {
        inputs++;
        printf("Invalid input input N.%d\n", inputs);
    }

    printf("Got area_id : %d\n", area_id);
    inputs++;

    printf("To how many tribes would you like to remove votes?\n");
    int tribes = 0;
    while (!scanf("%d", &tribes))
    {
        inputs++;
        printf("Invalid input input N.%d\n", inputs);
    }
    printf("Got tribes : %d\n", tribes);

    for (int i = 0; i < tribes; i++)
    {
        int votes;
        int tribe_id;
        printf("Please enter tribe ID:\n");
        scanf("%d", &tribe_id);

        printf("Got tribe_id : %d\n", tribe_id);
        inputs++;
        printf("Please enter votes to remove:\n");
        scanf("%d", &votes);
        printf("Got votes : %d\n", votes);
        inputs++;
        printElectionResult(electionRemoveVote(elections[selection], area_id, tribe_id, votes));
    }
}
void operationAddVote()
{
    int maxi = getMaxIndexOfElectionsWithPrint();
    if (maxi == -1)
    {
        return;
    }
    printf("To which election would you like to add votes? [0-%d]\n", maxi);

    int selection = getSelection(0, maxi, 0);
    printf("To which area would you like to add (enter area_id)?\n");

    int area_id = 0;
    while (!scanf("%d", &area_id))
    {
        inputs++;
        printf("Invalid input input N.%d\n", inputs);
    }

    printf("Got area_id : %d\n", area_id);
    inputs++;

    printf("To how many tribes would you like to vote?\n");
    int tribes = 0;
    while (!scanf("%d", &tribes))
    {
        inputs++;
        printf("Invalid input input N.%d\n", inputs);
    }
    printf("Got tribes : %d\n", tribes);

    for (int i = 0; i < tribes; i++)
    {
        int votes;
        int tribe_id;
        printf("Please enter tribe ID:\n");
        scanf("%d", &tribe_id);

        printf("Got tribe_id : %d\n", tribe_id);
        inputs++;
        printf("Please enter votes:\n");
        scanf("%d", &votes);
        printf("Got votes : %d\n", votes);
        inputs++;
        printElectionResult(electionAddVote(elections[selection], area_id, tribe_id, votes));
    }
}
int getMaxIndexOfElectionsWithPrint()
{
    int maxi = operationPrintElectionSelector();
    if (maxi == -1)
    {
        printf("No elections stored\n");
    }
    return maxi;
}
int operationPrintElectionSelector()
{
    if (suppress_print)
    {
        return operationPrintElectionsInternal();
    }
    return operationPrintElections();
}
void operationCreateElection()
{
    Election new_elec = electionCreate();
    addElecToElecs(new_elec);
}
void operationDestroyElection()
{
    int maxi = getMaxIndexOfElectionsWithPrint();
    if (maxi == -1)
    {
        return;
    }
    printf("Which election would you like to destroy? [0-%d] [-9->Destroy all]\n", maxi);
    int selection = getSelection(0, maxi, -9);
    if (selection == -9)
    {
        for (int i = 0; i < MAX_ELECTIONS; i++)
        {
            if (elections[i])
            {
                electionDestroy(elections[i]);
                removeFromElections(i);
            }
        }
    }
    else
    {
        electionDestroy(elections[selection]);
        removeFromElections(selection);
    }
}
void operationAddTribe()
{
    int maxi = getMaxIndexOfElectionsWithPrint();
    if (maxi == -1)
    {
        return;
    }
    printf("To which election would you like to add a tribe? [0-%d]\n", maxi);

    int selection = getSelection(0, maxi, 0);
    printf("How many tribes would you like to insert?\n");
    int num_of_vals = 0;
    while (!scanf("%d", &num_of_vals))
    {
        inputs++;
        printf("Invalid input input N.%d\n", inputs);
    }
    printf("Got num_of_vals : %d\n", num_of_vals);
    inputs++;
    for (int i = 0; i < num_of_vals; i++)
    {
        char tribe_name[50];
        int tribe_id;
        printf("Please enter tribe ID:\n");
        scanf("%d", &tribe_id);

        printf("Got tribe_id : %d\n", tribe_id);
        inputs++;
        printf("Please enter tribe name:\n");
        scanf("%s", tribe_name);
        printf("Got tribe_name : %s\n", tribe_name);
        inputs++;
        printElectionResult(electionAddTribe(elections[selection], tribe_id, tribe_name));
    }
}
void operationAddArea()
{
    int maxi = getMaxIndexOfElectionsWithPrint();
    if (maxi == -1)
    {
        return;
    }
    printf("To which election would you like to add an area? [0-%d]\n", maxi);
    int selection = getSelection(0, maxi, 0);
    printf("How many areas would you like to insert?\n");
    int num_of_vals = 0;
    if (!scanf("%d", &num_of_vals))
    {
        inputs++;
        printf("Invalid input input N.%d\n", inputs);
    }
    printf("Got num_of_vals : %d\n", num_of_vals);
    inputs++;
    for (int i = 0; i < num_of_vals; i++)
    {
        char area_name[50] = "";
        int area_id = -99;
        printf("Please enter area ID:\n");
        scanf("%d", &area_id);

        printf("Got area_id : %d\n", area_id);
        inputs++;
        printf("Please enter area name:\n");
        scanf("%s", area_name);
        printf("Got area_name : %s\n", area_name);
        inputs++;
        printElectionResult(electionAddArea(elections[selection], area_id, area_name));
    }
}
void printElectionResult(ElectionResult result)
{
    char *status_string = "UNDEFINED";
    switch (result)
    {
    case ELECTION_SUCCESS:
        status_string = "ELECTION_SUCCESS";
        break;
    case ELECTION_OUT_OF_MEMORY:
        status_string = "ELECTION_OUT_OF_MEMORY";
        break;
    case ELECTION_NULL_ARGUMENT:
        status_string = "ELECTION_NULL_ARGUMENT";
        break;
    case ELECTION_INVALID_ID:
        status_string = "ELECTION_INVALID_ID";
        break;
    case ELECTION_TRIBE_ALREADY_EXIST:
        status_string = "ELECTION_TRIBE_ALREADY_EXIST";
        break;
    case ELECTION_INVALID_NAME:
        status_string = "ELECTION_INVALID_NAME";
        break;
    case ELECTION_AREA_ALREADY_EXIST:
        status_string = "ELECTION_AREA_ALREADY_EXIST";
        break;
    case ELECTION_TRIBE_NOT_EXIST:
        status_string = "ELECTION_TRIBE_NOT_EXIST";
        break;
    case ELECTION_AREA_NOT_EXIST:
        status_string = "ELECTION_AREA_NOT_EXIST";
        break;
    case ELECTION_INVALID_VOTES:
        status_string = "ELECTION_INVALID_VOTES";
        break;
    default:
        break;
    }
    printf("Operation end with status of %s\n", status_string);
}
void sortEntries(Map map, node *head)
{

    (*head) = NULL;
    MAP_FOREACH(key, map)
    { //=malloc(sizeof(*sorted_list))
        char *value = mapGet(map, key);

        node dummy = malloc(sizeof(*dummy));
        node temp = dummy;
        temp->next = (*head);
        int k = 0;
        while (temp->next && strcmp(key, temp->next->key) > 0)
        {
            temp = temp->next;
            k++;
        }

        node after = temp->next;
        node new_node = malloc(sizeof(*new_node));
        new_node->key = key;
        new_node->value = value;

        temp->next = new_node;

        if (k == 0)
        {
            *head = temp->next;
        }
        new_node->next = after;
        free(dummy);
    }
    return;
}
void printLexicographic(Map map, char *first_label, char *second_label)
{
    //printf("Map N. %d\n\n", i);
    //        max_map_index = i;
    int num_elements = mapGetSize(map);
    int k = 0;
    int spaces_for_key = 20;
    int spaces_for_value = 20;
    node head;
    sortEntries(map, &head);
    node sorted = head;
    while (sorted)
    {

        if (k == 0)
        {
            putchar(' ');
            printf("%s", first_label);
            for (int i = 0; i <= spaces_for_key - strlen(first_label); i++)
            {
                putchar(' ');
            }
            printf("%s\n", second_label);

            putchar(' ');
            for (int i = 0; i < spaces_for_key; i++)
            {
                putchar('-');
            }
            putchar(' ');
            for (int i = 0; i < spaces_for_value; i++)
            {
                putchar('-');
            }
            putchar(' ');
            putchar('\n');
        }

        printf("|%-20s|", (sorted->key));

        printf("%-20s|\n", sorted->value);

        k++;
        if (k == num_elements)
        { // Print ending
            putchar(' ');
            for (int i = 0; i < spaces_for_key; i++)
            {
                putchar('-');
            }
            putchar(' ');
            for (int i = 0; i < spaces_for_value; i++)
            {
                putchar('-');
            }
            putchar(' ');
            putchar('\n');
        }
        sorted = sorted->next;
    }

    printf("\n");
    while (head)
    {
        node to_free = head;
        head = head->next;
        free(to_free);
    }

    free(sorted);
}

int operationPrintElections()
{

    int max_map_index = -1;
    for (int i = 0; i < MAX_ELECTIONS; i++)
    {

        if (elections[i])
        {
            printf("Election N. %d\n\n", i);
            max_map_index = i;

            Map tempMap = electionComputeAreasToTribesMapping(elections[i]);
            printLexicographic(tempMap, "Area ID", "Tribe ID");
            mapDestroy(tempMap);
            printf("\n");
        }
    }
    return max_map_index;
}
int operationPrintElectionsInternal()
{
    int count = 0;
    int max_election_index = -1;
    for (int i = 0; i < MAX_ELECTIONS; i++)
    {

        if (elections[i])
        {
            printf("Map N. %d\n\n", i);
            max_election_index = i;
            //mapPrint(maps[i]);
            count++;
            printf("\n");
        }
    }
    return max_election_index;
}

int addElecToElecs(Election elec)
{
    for (int i = 0; i < MAX_ELECTIONS; i++)
    {
        if (!elections[i])
        {
            elections[i] = elec;
            return i;
        }
    }
    return -1;
}
void removeFromElections(int index)
{
    elections[index] = NULL;
}

int main()
{
    for (int i = 0; i < MAX_ELECTIONS; i++)
    {
        elections[i] = NULL;
    }
    printf("Welcome to election tester V1 C Shai Yehezkel\n\n");
    printf("Would you like to suppress prints? (Y/N) ");
    char selection = getchar();
    inputs++;
    if (selection == 'y' || selection == 'Y')
    {
        suppress_print = true;
    }

    do
    {
        printOptions();
    } while (!executeOperations());
    return 0;
}
