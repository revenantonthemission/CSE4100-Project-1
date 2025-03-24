#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "hash.h"
#include "bitmap.h"

#define MAX_COMMAND_LENGTH 150
#define MAX_TOKEN_LENGTH 25
#define MAX_TOKEN_NUM 6

// Command List
#define COMMAND_CREATE "create"
#define COMMAND_DELETE "delete"
#define COMMAND_DUMPDATA "dumpdata"
#define COMMAND_QUIT "quit"

int main(int argc, char *argv[])
{
    char command[MAX_COMMAND_LENGTH]={};
    char tokens[MAX_TOKEN_NUM][MAX_TOKEN_LENGTH]={};
    struct list *instances = NULL;
    struct list *list = NULL;
    struct list_item *item = NULL, *ptr = NULL;
    struct hash *hashtable = NULL;
    struct hash_iterator *iter = NULL;
    struct bitmap *bitmap = NULL;

    instances = (struct list*)malloc(sizeof(struct list));
    list_init(instances);

    while(1)
    {
        memset(command, '\0', MAX_COMMAND_LENGTH);
        memset(tokens, '\0', MAX_TOKEN_NUM*MAX_TOKEN_LENGTH);
        fflush(stdin);
        fflush(stdout);

        fgets(command, MAX_COMMAND_LENGTH-1, stdin);
        sscanf(command, "%s %s %s %s %s %s", tokens[0], tokens[1], tokens[2], tokens[3], tokens[4], tokens[5]);

        if(!strcmp(tokens[0], COMMAND_CREATE))
        {
            if(!strcmp(tokens[1], OBJECT_LIST))
            {
                list = (struct list*)malloc(sizeof(struct list));
                list_init(list);
                item = (struct list_item*)malloc(sizeof(struct list_item));
                list_item_init(item, list, tokens[2], 0);
                list_insert(list_end(instances), &item->elem);
            }
            else if(!strcmp(tokens[1], OBJECT_HASHTABLE))
            {
                hashtable = (struct hash*)malloc(sizeof(struct hash));
                hash_init(hashtable, &hash_hash, &hash_less, NULL);
                item = (struct list_item*)malloc(sizeof(struct list_item));
                list_item_init(item, hashtable, tokens[2], 1);
                list_insert(list_end(instances), &item->elem);
            }
            else if(!strcmp(tokens[1], OBJECT_BITMAP))
            {
                bitmap = bitmap_create(atoi(tokens[3]));
                item = (struct list_item*)malloc(sizeof(struct list_item));
                list_item_init(item, bitmap, tokens[2], 2);
                list_insert(list_end(instances), &item->elem);
            }
        }
        else if(!strcmp(tokens[0], COMMAND_DELETE))
        {
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));
            list_remove(&ptr->elem);
            switch(ptr->type)
            {
                case 0:
                    list_delete(ptr->data);
                    break;
                case 1:
                    hash_destroy(ptr->data, hash_destroyer);
                    break;
                case 2:
                    bitmap_destroy(ptr->data);
                    break;
                default:
                    break;
            }
        }
        else if(!strcmp(tokens[0], COMMAND_DUMPDATA))
        {
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));
            switch(ptr->type)
            {
                case 0:
                    if(!list_empty(ptr->data))
                    {
                        for (struct list_elem* e = list_begin(ptr->data); e != list_end(ptr->data); e = list_next(e))
                        {
                            fprintf(stdout, "%d ", *(int*)list_entry(e, struct list_item, elem)->data);
                        }
                        fputs("\n", stdout);
                    }
                    break;
                case 1:
                    if(!hash_empty(ptr->data))
                    {
                        iter = (struct hash_iterator*)malloc(sizeof(struct hash_iterator));
                        hash_first(iter, (ptr->data));
                        for (; hash_next(iter);)
                        {
                            fprintf(stdout, "%d ", hash_cur(iter)->value);
                        }
                        fputs("\n", stdout);
                        free(iter);
                    }
                    break;
                case 2:
                    for (size_t i = 0; i < bitmap_size(ptr->data); i++)
                    {
                        fprintf(stdout, "%d", bitmap_test(ptr->data, i));
                    }
                    fputs("\n", stdout);
                    break;
                default:
                    break;
            }
        }
        else if(!strcmp(tokens[0], COMMAND_QUIT))
        {
            exit(EXIT_FAILURE);
        }
        else if(!strcmp(tokens[0], LIST_FRONT))
        {
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));
            fprintf(stdout, "%d\n", *(int*)list_entry(list_front(ptr->data), struct list_item, elem)->data);
        }
        else if(!strcmp(tokens[0], LIST_BACK))
        {
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));
            fprintf(stdout, "%d\n", *(int*)list_entry(list_back(ptr->data), struct list_item, elem)->data);
        }
        else if(!strcmp(tokens[0], LIST_SIZE))
        {
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));
            fprintf(stdout, "%zu\n", list_size(ptr->data));
        }
        else if(!strcmp(tokens[0], LIST_EMPTY))
        {
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));
            if(list_empty(ptr->data))
            {
                fputs("true\n", stdout);
            }
            else
            {
                fputs("false\n", stdout);
            }
        }
        else if(!strcmp(tokens[0], LIST_PUSH_FRONT))
        {
            // list_push_front list0 3
            // token[0]: list_push_front, token[1]: list0, token[2]: 3
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));
            item = (struct list_item*)malloc(sizeof(struct list_item));
            item->data = malloc(sizeof(int));
            *(int*)item->data = atoi(tokens[2]);
            list_push_front(ptr->data, &item->elem);
        }
        else if(!strcmp(tokens[0], LIST_PUSH_BACK))
        {
            // list_push_back list0 4
            // token[0]: list_push_back, token[1]: list0, token[2]: 4
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));
            item = (struct list_item*)malloc(sizeof(struct list_item));
            item->data = malloc(sizeof(int));
            *(int*)item->data = atoi(tokens[2]);
            list_push_back(ptr->data, &item->elem);
        }
        else if(!strcmp(tokens[0], LIST_POP_FRONT))
        {
            // list_pop_front list0
            // token[0]: list_pop_front, token[1]: list0
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));
            list_pop_front(ptr->data);
        }
        else if(!strcmp(tokens[0], LIST_POP_BACK))
        {
            // list_pop_back list0
            // token[0]: list_pop_back, token[1]: list0
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));
            list_pop_back(ptr->data);
        }
        else if(!strcmp(tokens[0], LIST_INSERT))
        {
            // list_insert list0 0 1
            // token[0]: list_insert, token[1]: list0, token[2]: 0, token[3]: 1
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));
            struct list_elem *e = list_begin(ptr->data);
            for(int i=0; i<atoi(tokens[2]); i++)
            {
                e = list_next(e);
            }
            item = (struct list_item*)malloc(sizeof(struct list_item));
            item->data = malloc(sizeof(int));
            *(int*)item->data = atoi(tokens[3]);
            list_insert(e, &item->elem);
        }
        else if(!strcmp(tokens[0], LIST_REMOVE))
        {
            // list_remove list0 0
            // token[0]: list_remove, token[1]: list0, token[2]: 0
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));
            struct list_elem *e = list_begin(ptr->data);
            for(int i=0; i<atoi(tokens[2]); i++)
            {
                e = list_next(e);
            }
            list_remove(e);
        }
        else if(!strcmp(tokens[0], LIST_SHUFFLE))
        {
            // list_shuffle list0
            // token[0]: list_shuffle, token[1]: list0
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));
            list_shuffle(ptr->data);
        }
        else if(!strcmp(tokens[0], LIST_SORT))
        {
            // list_sort list0
            // token[0]: list_sort, token[1]: list0
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));
            list_sort(ptr->data, list_less, NULL);
        }
        else if(!strcmp(tokens[0], LIST_REVERSE))
        {
            // list_reverse list0
            // token[0]: list_reverse, token[1]: list0
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));
            list_reverse(ptr->data);
        }
        else if(!strcmp(tokens[0], LIST_UNIQUE))
        {
            // list_unique list0 list1
            // token[0]: list_unique, token[1]: list0, token[2]: list1
            struct list *list1 = NULL;
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[2]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));
             // list1이 유효한지 확인
            if (&ptr->elem != list_end(instances))
            {  
                list1 = ptr->data;
            }
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));
            list_unique(ptr->data, list1, list_less, NULL);
        }
        else if(!strcmp(tokens[0], LIST_MAX))
        {
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));
            fprintf(stdout, "%d\n", *(int*)(list_entry(list_max(ptr->data, list_less, NULL), struct list_item, elem)->data));
        }
        else if(!strcmp(tokens[0], LIST_MIN))
        {
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));
            fprintf(stdout, "%d\n", *(int*)(list_entry(list_min(ptr->data, list_less, NULL), struct list_item, elem)->data));
        }
        else if(!strcmp(tokens[0], LIST_INSERT_ORDERED))
        {
            // list_insert_ordered list0 5
            // token[0]: list_insert_ordered, token[1]: list0, token[2]: 5
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));
            item = (struct list_item*)malloc(sizeof(struct list_item));
            item->data = malloc(sizeof(int));
            *(int*)item->data = atoi(tokens[2]);
            list_insert_ordered(ptr->data, &item->elem, list_less, NULL);
        }
        else if(!strcmp(tokens[0], LIST_SPLICE))
        {
            // list_splice list0 2 list1 1 4
            // token[0]: list_splice, token[1]: list0, token[2]: 2, token[3]: list1, token[4]: 1, token[5]: 4
            struct list_elem *before = NULL, *first = NULL, *last = NULL;
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));
            before = list_begin(ptr->data);
            for(int i=0; i<atoi(tokens[2]); i++)
            {
                before = list_next(before);
            }
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[3]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));
            first = list_begin(ptr->data);
            for(int i=0; i<atoi(tokens[4]); i++)
            {
                first = list_next(first);
            }
            last = list_begin(ptr->data);
            for(int i=0; i<atoi(tokens[5]); i++)
            {
                last = list_next(last);
            }
            list_splice(before, first, last);
        }
        else if(!strcmp(tokens[0], LIST_SWAP))
        {
            // list_swap list0 0 1
            struct list_elem *pos_a = NULL, *pos_b = NULL;
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));
            pos_a = list_begin(ptr->data);
            pos_b = list_begin(ptr->data);
            for(int i=0; i<atoi(tokens[2]); i++)
            {
                pos_a = list_next(pos_a);
            }
            for(int i=0; i<atoi(tokens[3]); i++)
            {
                pos_b = list_next(pos_b);
            }
            list_swap(pos_a, pos_b);
        }
        else if(!strcmp(tokens[0], HASHTABLE_INSERT))
        {
            // hash_insert hash0 10
            // token[0]: hash_insert, token[1]: hash0, token[2]: 10
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));
            struct hash_elem* e = (struct hash_elem*)malloc(sizeof(struct hash_elem));
            e->value = atoi(tokens[2]);
            hash_insert(ptr->data, e);
        }
        else if(!strcmp(tokens[0], HASHTABLE_DELETE))
        {
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));
            struct hash_elem* e = (struct hash_elem*)malloc(sizeof(struct hash_elem));
            e->value = atoi(tokens[2]);
            hash_delete(ptr->data, e);
        }
        else if(!strcmp(tokens[0], HASHTABLE_FIND))
        {
            // hash_find hash0 10
            // token[0]: hash_find, token[1]: hash0, token[2]: 10
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));
            struct hash_elem* e = (struct hash_elem*)malloc(sizeof(struct hash_elem));
            e->value = atoi(tokens[2]);
            if(hash_find(ptr->data, e))
            {
                fprintf(stdout, "%d\n", hash_find(ptr->data, e)->value);
            }
        }
        else if(!strcmp(tokens[0], HASHTABLE_REPLACE))
        {
            // hash_replace hash0 10
            // token[0]: hash_replace, token[1]: hash0, token[2]: 10
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));
            struct hash_elem* e = (struct hash_elem*)malloc(sizeof(struct hash_elem));
            e->value = atoi(tokens[2]);
            hash_replace(ptr->data, e);
        }
        else if(!strcmp(tokens[0], HASHTABLE_APPLY))
        {
            // hash_apply hash0 square
            // hash_apply hash0 triple
            // token[0]: hash_apply, token[1]: hash0, token[2]: square/triple
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));
            if(!strcmp(tokens[2], "square"))
            {
                hash_apply(ptr->data, &hash_square);
            }
            else if(!strcmp(tokens[2], "triple"))
            {
                hash_apply(ptr->data, &hash_triple);
            }
        }
        else if(!strcmp(tokens[0], HASHTABLE_EMPTY))
        {
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));
            if(hash_empty(ptr->data))
            {
               fputs("true\n", stdout); 
            }
            else
                fputs("false\n", stdout);
        }
        else if(!strcmp(tokens[0], HASHTABLE_SIZE))
        {
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));
            fprintf(stdout, "%zu\n", hash_size(ptr->data));
        }
        else if(!strcmp(tokens[0], HASHTABLE_CLEAR))
        {
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));
            hash_clear(ptr->data, &hash_destroyer);
        }
        else if(!strcmp(tokens[0], BITMAP_MARK))
        {
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));
            bitmap_mark(ptr->data, atoi(tokens[2]));
        }
        else if(!strcmp(tokens[0], BITMAP_TEST))
        {
            // bitmap_test bm0 4
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));
            
            if(bitmap_test(ptr->data, atoi(tokens[2])))
            {
                fputs("true\n", stdout);
            }
            else
                fputs("false\n", stdout);
        }
        else if(!strcmp(tokens[0], BITMAP_SIZE))
        {
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));
            fprintf(stdout, "%zu\n", bitmap_size(ptr->data));
        }
        else if(!strcmp(tokens[0], BITMAP_SET))
        {
            //bitmap_set bm0 0 false
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));
            if(!strcmp(tokens[3], "true"))
            {
                bitmap_set(ptr->data, atoi(tokens[2]), true);
            }
            else
                bitmap_set(ptr->data, atoi(tokens[2]), false);
        }
        else if(!strcmp(tokens[0], BITMAP_SET_ALL))
        {
            //bitmap_set_all bm0 true
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));
            
            if(!strcmp(tokens[2], "true"))
            {
                bitmap_set_all(ptr->data, true);
            }
            else
                bitmap_set_all(ptr->data, false);
        }
        else if(!strcmp(tokens[0], BITMAP_SET_MULTIPLE))
        {
            //bitmap_set_multiple bm0 2 9 false
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));

            if(!strcmp(tokens[4], "true"))
            {
                bitmap_set_multiple(ptr->data, atoi(tokens[2]), atoi(tokens[3]), true);
            }
            else
                bitmap_set_multiple(ptr->data, atoi(tokens[2]), atoi(tokens[3]), false);
        }
        else if(!strcmp(tokens[0], BITMAP_SCAN))
        {
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));
            if(!strcmp(tokens[4], "true"))
            {
                fprintf(stdout, "%zu\n", bitmap_scan(ptr->data, atoi(tokens[2]), atoi(tokens[3]), true));
            }
            else if(!strcmp(tokens[4], "false"))
            {
                fprintf(stdout, "%zu\n", bitmap_scan(ptr->data, atoi(tokens[2]), atoi(tokens[3]), false));
            }
        }
        else if(!strcmp(tokens[0], BITMAP_SCAN_AND_FLIP))
        {
            //bitmap_scan_and_flip bm0 0 1 true
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));
            if(!strcmp(tokens[4], "true"))
            {
                fprintf(stdout, "%zu\n", bitmap_scan_and_flip(ptr->data, atoi(tokens[2]), atoi(tokens[3]), true));
            }
            else if(!strcmp(tokens[4], "false"))
            {
                fprintf(stdout, "%zu\n", bitmap_scan_and_flip(ptr->data, atoi(tokens[2]), atoi(tokens[3]), false));
            }
        }
        else if(!strcmp(tokens[0], BITMAP_RESET))
        {
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));
            bitmap_reset(ptr->data, atoi(tokens[2]));
        }
        else if(!strcmp(tokens[0], BITMAP_NONE))
        {
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));
            if(bitmap_none(ptr->data, atoi(tokens[2]), atoi(tokens[3])))
            {
                fputs("true\n", stdout);
            }
            else
                fputs("false\n", stdout);
        }
        else if(!strcmp(tokens[0], BITMAP_FLIP))
        {
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));
            bitmap_flip(ptr->data, atoi(tokens[2]));
        }
        else if(!strcmp(tokens[0], BITMAP_EXPAND))
        {
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));
            ptr->data = bitmap_expand(ptr->data, bitmap_size(ptr->data) + atoi(tokens[2]));
        }
        else if(!strcmp(tokens[0], BITMAP_CONTAINS))
        {
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));
            if(!strcmp(tokens[4], "true"))
            {
                if(bitmap_contains(ptr->data, atoi(tokens[2]), atoi(tokens[3]), true))
                {
                    fputs("true\n", stdout);
                }
                else
                    fputs("false\n", stdout);
            }
            else if(!strcmp(tokens[4], "false"))
            {
                
                if(bitmap_contains(ptr->data, atoi(tokens[2]), atoi(tokens[3]), false))
                {
                    fputs("true\n", stdout);
                }
                else
                    fputs("false\n", stdout);
            }
        }
        else if(!strcmp(tokens[0], BITMAP_COUNT))
        {
            // bitmap_count bitmap 0 5 true
            // token[0]: bitmap_count, token[1]: bitmap, token[2]: 0, token[3]: 5, token[4]: true
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));
            if(!strcmp(tokens[4], "true"))
            {
                fprintf(stdout, "%zu\n", bitmap_count(ptr->data, atoi(tokens[2]), atoi(tokens[3]), true));
            }
            else if(!strcmp(tokens[4], "false"))
            {
                fprintf(stdout, "%zu\n", bitmap_count(ptr->data, atoi(tokens[2]), atoi(tokens[3]), false));
            }
        }
        else if(!strcmp(tokens[0], BITMAP_ANY))
        {
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));
            if(bitmap_any(ptr->data, atoi(tokens[2]), atoi(tokens[3])))
            {
                fputs("true\n", stdout);
            }
            else
                fputs("false\n", stdout);
        }
        else if(!strcmp(tokens[0], BITMAP_ALL))
        {
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));
            if(bitmap_all(ptr->data, atoi(tokens[2]), atoi(tokens[3])))
            {
                fputs("true\n", stdout);
            }
            else
                fputs("false\n", stdout);
        }
        else if(!strcmp(tokens[0], BITMAP_DUMP))
        {
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));
            bitmap_dump(ptr->data);
        }
    }
    list_delete(instances);
    return 0;
}