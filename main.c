#include <stdio.h>
#include <stdlib.h>
#include "election.h"
#include <assert.h>


//returns an integer length. will be used in convertion of int to string
static int getIntLength(int num)
{
    int len = 0;
    while (num>0)
    {
        num/=10;
        len++;
    }
    
    return len;
}

char* intToString(int num)
{
    char* str_tribe_id = malloc(getIntLength(num) + 1);
    sprintf(str_tribe_id, "%d", num);
    return str_tribe_id;
}
int main()
{
    printf("%s\n", intToString(123456));

    
    
    return 0;
}