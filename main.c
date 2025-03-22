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
    char command[MAX_COMMAND_LENGTH]={};
    char tokens[MAX_TOKEN_NUM][MAX_TOKEN_LENGTH]={};
    size_t bit_cnt=0;
    
    while(1)
    {
        memset(command, '\0', MAX_COMMAND_LENGTH);
        memset(tokens, '\0', MAX_TOKEN_NUM*MAX_TOKEN_LENGTH);
        fflush(stdin);
        fflush(stdout);
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
            fputs("Program Terminated.\n", stdout);
        }
        else if(!strcmp(tokens[0], LIST_FRONT))
        {
            fputs("The front element in list\n", stdout);
        }
        else if(!strcmp(tokens[0], LIST_BACK))
        {
            fputs("The back element in list\n", stdout);
        }
        else if(!strcmp(tokens[0], LIST_SIZE))
        {
            fputs("The number of elements in list\n", stdout);
        }
        else if(!strcmp(tokens[0], LIST_EMPTY))
        {
            fputs("The list is empty or not\n", stdout);
        }
        else if(!strcmp(tokens[0], LIST_PUSH_FRONT))
        {
            fputs("1\n", stdout);
        }
        else if(!strcmp(tokens[0], LIST_PUSH_BACK))
        {
            fputs("1\n", stdout);
        }
        else if(!strcmp(tokens[0], LIST_POP_FRONT))
        {
            fputs("1\n", stdout);
        }
        else if(!strcmp(tokens[0], LIST_POP_BACK))
        {
            fputs("1\n", stdout);
        }
        else if(!strcmp(tokens[0], LIST_INSERT))
        {
            fputs("1\n", stdout);
        }
        else if(!strcmp(tokens[0], LIST_REMOVE))
        {
            fputs("1\n", stdout);
        }
        else if(!strcmp(tokens[0], LIST_SHUFFLE))
        {
            fputs("1\n", stdout);
        }
        else if(!strcmp(tokens[0], LIST_SORT))
        {
            fputs("1\n", stdout);
        }
        else if(!strcmp(tokens[0], LIST_REVERSE))
        {
            fputs("1\n", stdout);
        }
        else if(!strcmp(tokens[0], LIST_UNIQUE))
        {
            fputs("1\n", stdout);
        }
        else if(!strcmp(tokens[0], LIST_MAX))
        {
            fputs("1\n", stdout);
        }
        else if(!strcmp(tokens[0], LIST_MIN))
        {
            fputs("1\n", stdout);
        }
        else if(!strcmp(tokens[0], LIST_INSERT_ORDERED))
        {
            fputs("1\n", stdout);
        }
        else if(!strcmp(tokens[0], LIST_SPLICE))
        {
            fputs("1\n", stdout);
        }
        else
        {
            fputs("Invalid command.\n", stdout);
        }
    }
    return 0;
}