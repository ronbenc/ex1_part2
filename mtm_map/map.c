#include "map.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct node
{
    char* key;
    char* value;
    struct node* next;
} *Node;

struct Map_t
{
    Node head;
    Node current;
};

//developers comment: this function exists in election.c aswell. we chose to duplicate it because it's a simple and short function that doesn"t justify a utilty file
//allocates a new string and copies given string data
static char* copyString(const char* str)
{
    long int len = strlen(str);
    char* newStr = malloc(len + 1);

    if (newStr == NULL)
    {
        return NULL;
    }

    return strcpy(newStr, str);
}

//allocates a new node and assigns key and value 
static Node nodeCreate(const char* key, const char* value)
{
    Node new_node = malloc(sizeof(struct node)); //create new node
    if(new_node == NULL)
    {
        return NULL;
    }

    new_node->key = copyString(key);
    new_node->value = copyString(value);

    if(new_node->key == NULL || new_node->value == NULL)
    {
        return NULL;
    }
    
    return new_node;
}

static void nodeDestroy(Node node)
{
    assert(node);
    free(node->key);
    free(node->value);
    free(node);
}

static Node mapGetNode(Map map, const char* key)
{
    Node current = map->head;

    if(map == NULL)
    {
        return NULL;
    }

    while (current != NULL && strcmp(current->key, key) != 0)
    {
        current = current->next;
    }

    return current;
}

Map mapCreate()
{
    Map map = malloc(sizeof(*map));
    if(map == NULL)
    {
        return NULL;
    }

    map->head = NULL;
    map->current = NULL;

    return map;
}

void mapDestroy(Map map)
{
    mapClear(map);
    free(map);
}

Map mapCopy(Map map)
{
    if(map == NULL)
    {
        return NULL;
    }

    Map new_map = mapCreate();
    if(new_map == NULL)
    {
        return NULL;
    }
    

    MAP_FOREACH(key_iterator, map)
    {
        Node prev_head = new_map->head;
        new_map->head = nodeCreate(map->current->key, map->current->value);     
        if(new_map->head == NULL)
        {
            return NULL;
        }

        new_map->head->next = prev_head;
    }
    new_map->current = NULL;
    return new_map;
}

int mapGetSize(Map map)
{
    if(map == NULL)
    {
        return -1;
    }
    
    Node save_curr = map->current;
    int len=0;;
    MAP_FOREACH(iterator, map)
    {
        len++;
    }
    map->current = save_curr;
    return len;
}

bool mapContains(Map map, const char* key)
{
    if(map == NULL || key == NULL)
    {
        return NULL;
    }
    
    if(mapGetNode(map, key) != NULL)
    {
        return true;
    }

    else
    {
        return false;
    }
}

//Reassign a value for a certain node
static MapResult mapPutReassign(Node ptr, const char* data)
{
    free(ptr->value);
    ptr->value = copyString(data);
    if(ptr->value == NULL)
    {
        return MAP_OUT_OF_MEMORY;
    }
    
    return MAP_SUCCESS;
}

//Assign a value for a certain node
static MapResult mapPutAssign(Map map, const char* key, const char* data)
{
    Node new_node = nodeCreate(key, data);

    if(new_node == NULL)
    {
        return MAP_OUT_OF_MEMORY;
    }

    new_node->next = map->head;
    map->head = new_node;

    return MAP_SUCCESS;
}

MapResult mapPut(Map map, const char* key, const char* data)
{
    if(map == NULL || key == NULL || data == NULL)
    {
        return MAP_NULL_ARGUMENT;
    }

    Node ptr = mapGetNode(map, key);
    if(ptr != NULL) //key exists in the list. we will update its value;
    {
        return mapPutReassign(ptr, data);
    }

    // key doesn't exists in list. we will add it to the head adn assign key and value
    return mapPutAssign(map, key, data);
}

char* mapGet(Map map, const char* key)
{
    if(map == NULL || key == NULL)
    {
        return NULL; 
    }

    Node node = mapGetNode(map, key);
    if(node == NULL)
    {
        return NULL;
    }
    
    return(node->value);
}

MapResult mapRemove(Map map, const char* key)
{
    if(map == NULL || key == NULL)
    {
        return MAP_NULL_ARGUMENT;
    }

    // Concern#1: search the key and update both map->current and previous_node
    Node previous_node = NULL;

    map->current = map->head;
    while(map->current != NULL && strcmp(map->current->key, key) != 0)
    {
        previous_node = map->current;
        map->current = map->current->next;
    }

    if(map->current == NULL)
    {
        return MAP_ITEM_DOES_NOT_EXIST;
    }

    // Concern#2: handle special case of key found at the head of the list
    if(previous_node != NULL)
    {
        assert(map->current != map->head);
        previous_node->next = map->current->next;
    } 
    else
    {
        assert(map->current == map->head);
        map->head = map->head->next;
    }

    // Concern #3: free the memory of the found node
    nodeDestroy(map->current);

    return MAP_SUCCESS;
}

char* mapGetFirst(Map map)
{
    if(map == NULL || (map->head) == NULL)
    {
        return NULL;
    }

    // update current for the iterator
    map->current = map->head;

    return map->head->key;
}

char* mapGetNext(Map map)
{
    if(map == NULL || (map->current) == NULL || (map->current->next) == NULL)
    {
        return NULL;
    }

    // update current for the iterator
    map->current = map->current->next;
 
    return map->current->key;
}

MapResult mapClear(Map map)
{
    if(map == NULL)
    {
        return MAP_NULL_ARGUMENT;
    }

    while (map->head != NULL)
    {
        Node tmp = map->head->next;
        nodeDestroy(map->head);
        map->head = tmp;
    }
    
    map->head = NULL;
    return MAP_SUCCESS;
}