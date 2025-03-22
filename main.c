#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "hash.h"
#include "bitmap.h"

#define MAX_COMMAND_LENGTH 100
#define MAX_TOKEN_LENGTH 20
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
    struct list *list = NULL;
    struct list_item *item = NULL;
    struct hash *hashtable = NULL;
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
                list = list_create(tokens[2]);
            }
            else if(!strcmp(tokens[1], OBJECT_HASHTABLE))
            {
                hashtable = hash_create();
                hash_init(hashtable, hash_hash, hash_less, tokens[2]);
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
                list_delete(list);
            }
            else if(!strcmp(tokens[1], OBJECT_HASHTABLE))
            {
                hash_destroy(hashtable, hash_destroyer);
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
                for(struct list_elem* ptr = list_begin(list); ptr != list_end(list); ptr = list_next(ptr))
                {
                    struct list_item* item = list_entry(ptr, struct list_item, elem);
                    fprintf(stdout, "%d ", item->data);
                    ptr = list_next(ptr);
                }
                fputs("\n", stdout);
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
            fprintf(stdout, "%d\n", list_entry(list_front(list), struct list_item, elem)->data);
        }
        else if(!strcmp(tokens[0], LIST_BACK))
        {
            fprintf(stdout, "%d\n", list_entry(list_back(list), struct list_item, elem)->data);
        }
        else if(!strcmp(tokens[0], LIST_SIZE))
        {
            fprintf(stdout, "%zu\n", list_size(list));
        }
        else if(!strcmp(tokens[0], LIST_EMPTY))
        {
            fputs("The list is empty or not\n", stdout);
        }
        else if(!strcmp(tokens[0], LIST_PUSH_FRONT))
        {
            item = (struct list_item*)malloc(sizeof(struct list_item));
            item->data = atoi(tokens[1]);
            list_push_front(list, &item->elem);
        }
        else if(!strcmp(tokens[0], LIST_PUSH_BACK))
        {
            item = (struct list_item*)malloc(sizeof(struct list_item));
            item->data = atoi(tokens[1]);
            list_push_back(list, &item->elem);
        }
        else if(!strcmp(tokens[0], LIST_POP_FRONT))
        {
            list_pop_front(list);
        }
        else if(!strcmp(tokens[0], LIST_POP_BACK))
        {
            list_pop_back(list);
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