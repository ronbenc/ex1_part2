#include <stdio.h>
#include "election.h"
#include <assert.h>

static bool isIdValid(int id)
{
    return (id>=0);
}

//true if name contains only lowercase letters and spaces (' '). false otherwise
static bool isNameValid(char* name)
{
    
    while (name[0] != 0)
    {
        printf("%c\n", name[0]);
        if((name[0] < 'a' || name[0] > 'z') && (name[0] != ' '))
        {
            printf("%d\n", name[0]);
            return false;
        }

        name++;
    }
    
    return true;
}

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
int main()
{
    int num = 1234567;
    printf("%d\n", getIntLength(num));

    if(isNameValid("aa b v f ! +"))
        printf("name is valid\n");
    else
        printf("name is not valid\n");
    
    if(isIdValid(0))
        printf("id valid");
    else
        printf("id is not valid\n");
    
    
    return 0;
}