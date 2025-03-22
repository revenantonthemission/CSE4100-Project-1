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
    struct hash_iterator *iter = NULL;
    struct bitmap *bitmap = NULL;
    char bool_values[2][10] = {"false", "true"};
    size_t bit_cnt=0;
    
    while(1)
    {
        memset(command, '\0', MAX_COMMAND_LENGTH);
        memset(tokens, '\0', MAX_TOKEN_NUM*MAX_TOKEN_LENGTH);
        fflush(stdin);
        fflush(stdout);
        fgets(command, MAX_COMMAND_LENGTH-1, stdin);
        sscanf(command, "%s %s %s %s %s", tokens[0], tokens[1], tokens[2], tokens[3], tokens[4]);
        /*for(int i=0; i<MAX_TOKEN_NUM; i++)
        {
            fputs(tokens[i], stdout);
            fputs("\n", stdout);
        }*/
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
            else if(!strcmp(tokens[1], OBJECT_BITMAP))
            {
                bit_cnt = atoi(tokens[2]);
                bitmap = bitmap_create(bit_cnt);
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
            else if(!strcmp(tokens[1], OBJECT_BITMAP))
            {
                bitmap_destroy(bitmap);
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
                }
                fputs("\n", stdout);
            }
            else if(!strcmp(tokens[1], OBJECT_HASHTABLE))
            {
                hash_first(iter, hashtable);
                while(hash_next(iter))
                {
                    struct hash_elem* elem = hash_cur(iter);
                    fprintf(stdout, "%d ", elem->value);
                }
                fputs("\n", stdout);
            }
            else if(!strcmp(tokens[1], OBJECT_BITMAP))
            {
                for(size_t i=0; i<bit_cnt; i++)
                {
                    fprintf(stdout, "%d ", bitmap_test(bitmap, i));
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
            // list_push_front list0 3
            // token[0]: list_push_front, token[1]: list0, token[2]: 3
            item = (struct list_item*)malloc(sizeof(struct list_item));
            item->data = atoi(tokens[2]);
            list_push_front(list, &item->elem);
        }
        else if(!strcmp(tokens[0], LIST_PUSH_BACK))
        {
            // list_push_back list0 4
            // token[0]: list_push_back, token[1]: list0, token[2]: 4
            item = (struct list_item*)malloc(sizeof(struct list_item));
            item->data = atoi(tokens[2]);
            list_push_back(list, &item->elem);
        }
        else if(!strcmp(tokens[0], LIST_POP_FRONT))
        {
            // list_pop_front list0
            // token[0]: list_pop_front, token[1]: list0
            list_pop_front(list);
        }
        else if(!strcmp(tokens[0], LIST_POP_BACK))
        {
            // list_pop_back list0
            // token[0]: list_pop_back, token[1]: list0
            list_pop_back(list);
        }
        else if(!strcmp(tokens[0], LIST_INSERT))
        {
            // list_insert list0 0 1
            // token[0]: list_insert, token[1]: list0, token[2]: 0, token[3]: 1
            struct list_elem* e = list_begin(list);
            for(int i=0; i<atoi(tokens[2]); i++)
            {
                e = list_next(e);
            }
            item = (struct list_item*)malloc(sizeof(struct list_item));
            item->elem.next = NULL;
            item->elem.prev = NULL;
            item->data = atoi(tokens[3]);
            list_insert(e, &item->elem);    
        }
        else if(!strcmp(tokens[0], LIST_REMOVE))
        {
            // list_remove list0 0
            // token[0]: list_remove, token[1]: list0, token[2]: 0
            struct list_elem* e = list_begin(list);
            for(int i=0; i<atoi(tokens[2]); i++)
            {
                e = list_next(e);
            }
            list_remove(e);
        }
        else if(!strcmp(tokens[0], LIST_SHUFFLE))
        {
            list_shuffle(list);
        }
        else if(!strcmp(tokens[0], LIST_SORT))
        {
            list_sort(list, list_less, tokens[1]);
        }
        else if(!strcmp(tokens[0], LIST_REVERSE))
        {
            list_reverse(list);
        }
        else if(!strcmp(tokens[0], LIST_UNIQUE))
        {
            fputs("1\n", stdout);
        }
        else if(!strcmp(tokens[0], LIST_MAX))
        {
            fprintf(stdout, "%d\n", list_entry(list_max(list, list_less, tokens[1]), struct list_item, elem));
        }
        else if(!strcmp(tokens[0], LIST_MIN))
        {
            fprintf(stdout, "%d\n", list_entry(list_min(list, list_less, tokens[1]), struct list_item, elem));
        }
        else if(!strcmp(tokens[0], LIST_INSERT_ORDERED))
        {
            struct list_elem* e = list_begin(list);
            for(int i=0; i<atoi(tokens[2]); i++)
            {
                e = list_next(e);
            }
            item = (struct list_item*)malloc(sizeof(struct list_item));
            item->data = atoi(tokens[3]);
            list_insert_ordered(e, &item->elem, list_less, tokens[1]);    
        }
        else if(!strcmp(tokens[0], LIST_SPLICE))
        {
            fputs("1\n", stdout);
        }
        else if(!strcmp(tokens[0], HASHTABLE_INSERT))
        {
            struct hash_elem* e = (struct hash_elem*)malloc(sizeof(struct hash_elem));
            e->value = atoi(tokens[3]);
            hash_insert(hashtable, e);
        }
        else if(!strcmp(tokens[0], HASHTABLE_DELETE))
        {
            struct hash_elem* e = (struct hash_elem*)malloc(sizeof(struct hash_elem));
            e->value = atoi(tokens[3]);
            hash_delete(hashtable, e);
        }
        else if(!strcmp(tokens[0], HASHTABLE_FIND))
        {
            struct hash_elem* e = (struct hash_elem*)malloc(sizeof(struct hash_elem));
            e->value = atoi(tokens[3]);
            struct hash_elem* result = hash_find(hashtable, e);
            if(result != NULL)
            {
                fprintf(stdout, "%d\n", result->value);
            }
            else
            {
                fputs("Not found\n", stdout);
            }
        }
        else if(!strcmp(tokens[0], HASHTABLE_REPLACE))
        {
            struct hash_elem* e = (struct hash_elem*)malloc(sizeof(struct hash_elem));
            e->value = atoi(tokens[3]);
            hash_replace(hashtable, e);
        }
        else if(!strcmp(tokens[0], HASHTABLE_APPLY))
        {
           //hash_apply(hashtable, hash_action);
        }
        else if(!strcmp(tokens[0], HASHTABLE_FIRST))
        {
            iter = (struct hash_iterator*)malloc(sizeof(struct hash_iterator));
            iter->hash = hashtable;
            iter->bucket = list_begin(hashtable->buckets);
            iter->elem = list_begin(iter->bucket);
            if(iter->elem != NULL)
            {
                fprintf(stdout, "%d\n", iter->elem->value);
            }
            else
            {
                fputs("Empty\n", stdout);
            }
        }
        else if(!strcmp(tokens[0], HASHTABLE_NEXT))
        {
            struct hash_elem* next = hash_next(iter);
            if(next != NULL)
            {
                fprintf(stdout, "%d\n", next->value);
            }
            else 
            {
                fputs("Empty\n", stdout);
            }
        }
        else if(!strcmp(tokens[0], HASHTABLE_CURRENT))
        {
            struct hash_elem* e = hash_cur(iter);
            if(e != NULL)
            {
                fprintf(stdout, "%d\n", e->value);
            }
            else
            {
                fputs("Empty\n", stdout);
            }
        } 
        else if(!strcmp(tokens[0], BITMAP_MARK))
        {
            bitmap_mark(bitmap, atoi(tokens[2]));
        }
        else if(!strcmp(tokens[0], BITMAP_TEST))
        {
            bitmap_test(bitmap, atoi(tokens[2]));
        }
        else if(!strcmp(tokens[0], BITMAP_SIZE))
        {
            fprintf(stdout, "%zu\n", bitmap_size(bitmap));
        }
        else if(!strcmp(tokens[0], BITMAP_SET))
        {
            bitmap_set(bitmap, atoi(tokens[2]), atoi(tokens[3]));
        }
        else if(!strcmp(tokens[0], BITMAP_SET_ALL))
        {
            bitmap_set_all(bitmap, atoi(tokens[2]));
        }
        else if(!strcmp(tokens[0], BITMAP_SET_MULTIPLE))
        {
            bitmap_set_multiple(bitmap, atoi(tokens[2]), atoi(tokens[3]), atoi(tokens[4]));
        }
        else if(!strcmp(tokens[0], BITMAP_SCAN))
        {
            if(!strcmp(tokens[4], "true"))
            {
                bitmap_scan(bitmap, atoi(tokens[2]), atoi(tokens[3]), true);
            }
            else if(!strcmp(tokens[4], "false"))
            {
                bitmap_scan(bitmap, atoi(tokens[2]), atoi(tokens[3]), false);
            }
        }
        else if(!strcmp(tokens[0], BITMAP_SCAN_AND_FLIP))
        {
            if(!strcmp(tokens[4], "true"))
            {
                bitmap_scan_and_flip(bitmap, atoi(tokens[2]), atoi(tokens[3]), true);
            }
            else if(!strcmp(tokens[4], "false"))
            {
                bitmap_scan_and_flip(bitmap, atoi(tokens[2]), atoi(tokens[3]), false);
            }
        }
        else if(!strcmp(tokens[0], BITMAP_RESET))
        {
            bitmap_reset(bitmap, atoi(tokens[2]));
        }
        else if(!strcmp(tokens[0], BITMAP_NONE))
        {
            bitmap_none(bitmap, atoi(tokens[2]), atoi(tokens[3]));
        }
        else if(!strcmp(tokens[0], BITMAP_FLIP))
        {
            bitmap_flip(bitmap, atoi(tokens[2]));
        }
        else if(!strcmp(tokens[0], BITMAP_EXPAND))
        {
            bitmap_expand(bitmap, atoi(tokens[2]));
        }
        else if(!strcmp(tokens[0], BITMAP_CONTAINS))
        {
            if(!strcmp(tokens[4], "true"))
            {
                bitmap_contains(bitmap, atoi(tokens[2]), atoi(tokens[3]), true);
            }
            else if(!strcmp(tokens[4], "false"))
            {
                bitmap_contains(bitmap, atoi(tokens[2]), atoi(tokens[3]), false);
            }
        }
        else if(!strcmp(tokens[0], BITMAP_COUNT))
        {
            // bitmap_count bitmap 0 5 true
            // token[0]: bitmap_count, token[1]: bitmap, token[2]: 0, token[3]: 5, token[4]: true
            if(!strcmp(tokens[4], "true"))
            {
                bitmap_count(bitmap, atoi(tokens[2]), atoi(tokens[3]), true);
            }
            else if(!strcmp(tokens[4], "false"))
            {
                bitmap_count(bitmap, atoi(tokens[2]), atoi(tokens[3]), false);
            }
        }
        else if(!strcmp(tokens[0], BITMAP_ANY))
        {
            bitmap_any(bitmap, atoi(tokens[2]), atoi(tokens[3]));
        }
        else if(!strcmp(tokens[0], BITMAP_ALL))
        {
            bitmap_all(bitmap, atoi(tokens[2]), atoi(tokens[3]));
        }
        else
        {
            fputs("Invalid command.\n", stdout);
        }
    }
    return 0;
}