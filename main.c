#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "hash.h"
#include "bitmap.h"

#define MAX_COMMAND_LENGTH 30
#define MAX_TOKEN_LENGTH 10
#define MAX_TOKEN_NUM 5

// Command List
#define COMMAND_CREATE "create"
#define COMMAND_DELETE "delete"
#define COMMAND_DUMPDATA "dumpdata"
#define COMMAND_QUIT "quit"

int main(int argc, char *argv[])
{
    char command[MAX_COMMAND_LENGTH]={'\0',};
    char tokens[MAX_TOKEN_NUM][MAX_TOKEN_LENGTH]={'\0',};
    size_t bit_cnt=0;
    
    while(1)
    {
        fgets(command, MAX_COMMAND_LENGTH-1, stdin);
        sscanf(command, "%s %s %s %s %s", tokens[0], tokens[1], tokens[2], tokens[3], tokens[4]);
        for(int i=0; i<MAX_TOKEN_NUM; i++)
        {
            fputs(tokens[i], stdout);
            fputs("\n", stdout);
        }
        if(!strcmp(tokens[0], COMMAND_CREATE))
        {
            if(!strcmp(tokens[1], OBJECT_LIST))
            {
                fputs("list create\n", stdout);
            }
            else
            {
                fputs("Invalid object type.\n", stdout);
            }
        }
        else if(!strcmp(tokens[0], COMMAND_DELETE))
        {
            if(!strcmp(tokens[1], OBJECT_LIST))
            {
                fputs("list delete\n", stdout);
            }
            else
            {
                fputs("Invalid object type.\n", stdout);
            }
        }
        else if(!strcmp(tokens[0], COMMAND_DUMPDATA))
        {
            if(!strcmp(tokens[1], OBJECT_LIST))
            {
                fputs("list dumpdata\n", stdout);
            }
            else
            {
                fputs("Invalid object type.\n", stdout);
            }
        }
        else if(!strcmp(tokens[0], COMMAND_QUIT))
        {
            break;
        }
        else if(!strcmp(tokens[0], LIST_FRONT))
        {
            
        }
        else if(!strcmp(tokens[0], LIST_BACK))
        {

        }
        else if(!strcmp(tokens[0], LIST_SIZE))
        {

        }
        else if(!strcmp(tokens[0], LIST_EMPTY))
        {

        }
        else if(!strcmp(tokens[0], LIST_PUSH_FRONT))
        {

        }
        else if(!strcmp(tokens[0], LIST_PUSH_BACK))
        {

        }
        else if(!strcmp(tokens[0], LIST_POP_FRONT))
        {

        }
        else if(!strcmp(tokens[0], LIST_POP_BACK))
        {

        }
        else if(!strcmp(tokens[0], LIST_INSERT))
        {

        }
        else if(!strcmp(tokens[0], LIST_REMOVE))
        {

        }
        else if(!strcmp(tokens[0], LIST_SHUFFLE))
        {

        }
        else if(!strcmp(tokens[0], LIST_SORT))
        {

        }
        else if(!strcmp(tokens[0], LIST_REVERSE))
        {

        }
        else if(!strcmp(tokens[0], LIST_UNIQUE))
        {

        }
        else if(!strcmp(tokens[0], LIST_MAX))
        {

        }
        else if(!strcmp(tokens[0], LIST_MIN))
        {

        }
        else if(!strcmp(tokens[0], LIST_INSERT_ORDERED))
        {

        }
        else if(!strcmp(tokens[0], LIST_SPLICE))
        {

        }
        else
        {
            fputs("Invalid command.\n", stdout);
        }
        memset(command, '\0', MAX_COMMAND_LENGTH);
        memset(tokens, '\0', MAX_TOKEN_NUM*MAX_TOKEN_LENGTH);
    }

    return 0;
}