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
    // Variable List
    char command[MAX_COMMAND_LENGTH]={};
    char tokens[MAX_TOKEN_NUM][MAX_TOKEN_LENGTH]={};
    struct list *instances = NULL;
    struct list *list = NULL;
    struct list_item *item = NULL, *ptr = NULL;
    struct hash *hashtable = NULL;
    struct hash_iterator *iter = NULL;
    struct bitmap *bitmap = NULL;

    // instances: list of instances. Each instance is a list, hash, or bitmap.
    instances = (struct list*)malloc(sizeof(struct list));
    list_init(instances);

    while(1)
    {
        // Initialize I/O buffers
        memset(command, '\0', MAX_COMMAND_LENGTH);
        memset(tokens, '\0', MAX_TOKEN_NUM*MAX_TOKEN_LENGTH);
        fflush(stdin);
        fflush(stdout);

        // Read command from *.in
        fgets(command, MAX_COMMAND_LENGTH-1, stdin);
        sscanf(command, "%s %s %s %s %s %s", tokens[0], tokens[1], tokens[2], tokens[3], tokens[4], tokens[5]);

        // This section deals commands that start with 'create'
        if(!strcmp(tokens[0], COMMAND_CREATE))
        {
            // create a list and insert in INSTANCES
            if(!strcmp(tokens[1], OBJECT_LIST))
            {
                list = (struct list*)malloc(sizeof(struct list));
                list_init(list);
                item = (struct list_item*)malloc(sizeof(struct list_item));
                list_item_init(item, list, tokens[2], 0);
                list_insert(list_end(instances), &item->elem);
            }
            // create a hashtable and insert in INSTANCES
            else if(!strcmp(tokens[1], OBJECT_HASHTABLE))
            {
                hashtable = (struct hash*)malloc(sizeof(struct hash));
                hash_init(hashtable, &hash_hash, &hash_less, NULL);
                item = (struct list_item*)malloc(sizeof(struct list_item));
                list_item_init(item, hashtable, tokens[2], 1);
                list_insert(list_end(instances), &item->elem);
            }
            // create a bitmap and insert in INSTANCES
            else if(!strcmp(tokens[1], OBJECT_BITMAP))
            {
                bitmap = bitmap_create(atoi(tokens[3]));
                item = (struct list_item*)malloc(sizeof(struct list_item));
                list_item_init(item, bitmap, tokens[2], 2);
                list_insert(list_end(instances), &item->elem);
            }
        }
        // This section deals commands that start with 'delete'
        else if(!strcmp(tokens[0], COMMAND_DELETE))
        {
            // Search for the instance which name is tokens[1]
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));

            // Remove the instance from INSTANCES
            list_remove(&ptr->elem);

            // Destroy the instance
            switch(ptr->type)
            {
                // 0 : List
                case 0:
                    list_delete(ptr->data);
                    break;
                // 1 : Hashtable
                case 1:
                    hash_destroy(ptr->data, hash_destroyer);
                    break;
                // 2 : Bitmap
                case 2:
                    bitmap_destroy(ptr->data);
                    break;
                default:
                    break;
            }
        }
        // This section deals commands that start with 'dumpdata'
        else if(!strcmp(tokens[0], COMMAND_DUMPDATA))
        {
            // Search for the instance which name is tokens[1]
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));

            // Print the data of the instance
            switch(ptr->type)
            {
                // 0 : List
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
                // 1 : Hashtable
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
                // 2 : Bitmap
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
        // This section deals commands that start with 'quit'
        else if(!strcmp(tokens[0], COMMAND_QUIT))
        {
            exit(EXIT_FAILURE);
        }
        // This section executes commands that start with list_front
        else if(!strcmp(tokens[0], LIST_FRONT))
        {
            // Search for the instance which name is tokens[1]
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));

            // Print the front element of the list
            fprintf(stdout, "%d\n", *(int*)list_entry(list_front(ptr->data), struct list_item, elem)->data);
        }
        // This section executes commands that start with list_back
        else if(!strcmp(tokens[0], LIST_BACK))
        {
            // Search for the instance which name is tokens[1]
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));

            // Print the back element of the list
            fprintf(stdout, "%d\n", *(int*)list_entry(list_back(ptr->data), struct list_item, elem)->data);
        }
        // This section executes commands that start with list_size
        else if(!strcmp(tokens[0], LIST_SIZE))
        {
            // Search for the instance which name is tokens[1]
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));

            // Print the size of the list
            fprintf(stdout, "%zu\n", list_size(ptr->data));
        }
        // This section executes commands that start with list_empty
        else if(!strcmp(tokens[0], LIST_EMPTY))
        {
            // Search for the instance which name is tokens[1]
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));

            // Print whether the list is empty or not
            if(list_empty(ptr->data))
            {
                fputs("true\n", stdout);
            }
            else
            {
                fputs("false\n", stdout);
            }
        }
        // This section executes commands that start with list_push_front
        else if(!strcmp(tokens[0], LIST_PUSH_FRONT))
        {
            // list_push_front list0 3
            // token[0]: list_push_front, token[1]: list0, token[2]: 3
            // Search for the instance which name is tokens[1]
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));

            // Create a new list item and push it to the front of the list
            item = (struct list_item*)malloc(sizeof(struct list_item));
            item->data = malloc(sizeof(int));
            *(int*)item->data = atoi(tokens[2]);
            list_push_front(ptr->data, &item->elem);
        }
        // This section executes commands that start with list_push_back
        else if(!strcmp(tokens[0], LIST_PUSH_BACK))
        {
            // list_push_back list0 4
            // token[0]: list_push_back, token[1]: list0, token[2]: 4
            // Search for the instance which name is tokens[1]
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));

            // Create a new list item and push it to the back of the list
            item = (struct list_item*)malloc(sizeof(struct list_item));
            item->data = malloc(sizeof(int));
            *(int*)item->data = atoi(tokens[2]);
            list_push_back(ptr->data, &item->elem);
        }
        // This section executes commands that start with list_pop_front
        else if(!strcmp(tokens[0], LIST_POP_FRONT))
        {
            // list_pop_front list0
            // token[0]: list_pop_front, token[1]: list0
            // Search for the instance which name is tokens[1]
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));

            // Pop the front element of the list
            list_pop_front(ptr->data);
        }
        // This section executes commands that start with list_pop_back
        else if(!strcmp(tokens[0], LIST_POP_BACK))
        {
            // list_pop_back list0
            // token[0]: list_pop_back, token[1]: list0
            // Search for the instance which name is tokens[1]
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));

            // Pop the back element of the list
            list_pop_back(ptr->data);
        }
        // This section executes commands that start with list_insert
        else if(!strcmp(tokens[0], LIST_INSERT))
        {
            // list_insert list0 0 1
            // token[0]: list_insert, token[1]: list0, token[2]: 0, token[3]: 1
            // Search for the instance which name is tokens[1]
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));

            // Insert a new list item to the list
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
        // This section executes commands that start with list_remove
        else if(!strcmp(tokens[0], LIST_REMOVE))
        {
            // list_remove list0 0
            // token[0]: list_remove, token[1]: list0, token[2]: 0
            // Search for the instance which name is tokens[1]
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));

            // Remove the element at the index of tokens[2]
            struct list_elem *e = list_begin(ptr->data);
            for(int i=0; i<atoi(tokens[2]); i++)
            {
                e = list_next(e);
            }
            list_remove(e);
        }
        // This section executes commands that start with list_shuffle
        else if(!strcmp(tokens[0], LIST_SHUFFLE))
        {
            // list_shuffle list0
            // token[0]: list_shuffle, token[1]: list0
            // Search for the instance which name is tokens[1]
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));

            // Shuffle the list
            list_shuffle(ptr->data);
        }
        // This section executes commands that start with list_sort
        else if(!strcmp(tokens[0], LIST_SORT))
        {
            // list_sort list0
            // token[0]: list_sort, token[1]: list0
            // Search for the instance which name is tokens[1]
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));

            // Sort the list
            list_sort(ptr->data, list_less, NULL);
        }
        // This section executes commands that start with list_reverse
        else if(!strcmp(tokens[0], LIST_REVERSE))
        {
            // list_reverse list0
            // token[0]: list_reverse, token[1]: list0
            // Search for the instance which name is tokens[1]
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));

            // Reverse the list
            list_reverse(ptr->data);
        }
        // This section executes commands that start with list_unique
        else if(!strcmp(tokens[0], LIST_UNIQUE))
        {
            // list_unique list0 list1
            // token[0]: list_unique, token[1]: list0, token[2]: list1
            // Search for the instance which name is tokens[2]
            struct list *list1 = NULL;
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[2]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));

             // list1이 유효한지 확인
            if (&ptr->elem != list_end(instances))
            {  
                list1 = ptr->data;
            }

            // Search for the instance which name is tokens[1]
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));

            // Remove duplicates from the list
            list_unique(ptr->data, list1, list_less, NULL);
        }
        // This section executes commands that start with list_max
        else if(!strcmp(tokens[0], LIST_MAX))
        {
            // Search for the instance which name is tokens[1]
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));

            // Print the maximum value of the list
            fprintf(stdout, "%d\n", *(int*)(list_entry(list_max(ptr->data, list_less, NULL), struct list_item, elem)->data));
        }
        // This section executes commands that start with list_min
        else if(!strcmp(tokens[0], LIST_MIN))
        {
            // Search for the instance which name is tokens[1]
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));

            // Print the minimum value of the list
            fprintf(stdout, "%d\n", *(int*)(list_entry(list_min(ptr->data, list_less, NULL), struct list_item, elem)->data));
        }
        // This section executes commands that start with list_insert_ordered
        else if(!strcmp(tokens[0], LIST_INSERT_ORDERED))
        {
            // list_insert_ordered list0 5
            // token[0]: list_insert_ordered, token[1]: list0, token[2]: 5
            // Search for the instance which name is tokens[1]
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));

            // Insert a new list item to the list in order
            item = (struct list_item*)malloc(sizeof(struct list_item));
            item->data = malloc(sizeof(int));
            *(int*)item->data = atoi(tokens[2]);
            list_insert_ordered(ptr->data, &item->elem, list_less, NULL);
        }
        // This section executes commands that start with list_splice
        else if(!strcmp(tokens[0], LIST_SPLICE))
        {
            // list_splice list0 2 list1 1 4
            // token[0]: list_splice, token[1]: list0, token[2]: 2, token[3]: list1, token[4]: 1, token[5]: 4
            struct list_elem *before = NULL, *first = NULL, *last = NULL;

            // Search for the instance which name is tokens[1]
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));
            
            before = list_begin(ptr->data);
            for(int i=0; i<atoi(tokens[2]); i++)
            {
                before = list_next(before);
            }

            // Search for the instance which name is tokens[1]
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

            // Splice the list
            list_splice(before, first, last);
        }
        // This section executes commands that start with list_swap
        else if(!strcmp(tokens[0], LIST_SWAP))
        {
            // list_swap list0 0 1
            // token[0]: list_swap, token[1]: list0, token[2]: 0, token[3]: 1
            struct list_elem *pos_a = NULL, *pos_b = NULL;

            // Search for the instance which name is tokens[1]
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));

            // Swap the elements at the index of tokens[2] and tokens[3]
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
        // This section executes commands that start with hash_insert
        else if(!strcmp(tokens[0], HASHTABLE_INSERT))
        {
            // hash_insert hash0 10
            // token[0]: hash_insert, token[1]: hash0, token[2]: 10
            // Search for the instance which name is tokens[1]
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));

            // Insert a new hash element to the hashtable
            struct hash_elem* e = (struct hash_elem*)malloc(sizeof(struct hash_elem));
            e->value = atoi(tokens[2]);
            hash_insert(ptr->data, e);
        }
        // This section executes commands that start with hash_delete
        else if(!strcmp(tokens[0], HASHTABLE_DELETE))
        {
            // Search for the instance which name is tokens[1]
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));

            // Delete the element from the hashtable
            struct hash_elem* e = (struct hash_elem*)malloc(sizeof(struct hash_elem));
            e->value = atoi(tokens[2]);
            hash_delete(ptr->data, e);
        }
        // This section executes commands that start with hash_find
        else if(!strcmp(tokens[0], HASHTABLE_FIND))
        {
            // hash_find hash0 10
            // token[0]: hash_find, token[1]: hash0, token[2]: 10
            // Search for the instance which name is tokens[1]
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));

            // Find the element from the hashtable
            struct hash_elem* e = (struct hash_elem*)malloc(sizeof(struct hash_elem));
            e->value = atoi(tokens[2]);
            if(hash_find(ptr->data, e))
            {
                fprintf(stdout, "%d\n", hash_find(ptr->data, e)->value);
            }
        }
        // This section executes commands that start with hash_replace
        else if(!strcmp(tokens[0], HASHTABLE_REPLACE))
        {
            // hash_replace hash0 10
            // token[0]: hash_replace, token[1]: hash0, token[2]: 10
            // Search for the instance which name is tokens[1]
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));

            // Replace the element from the hashtable
            struct hash_elem* e = (struct hash_elem*)malloc(sizeof(struct hash_elem));
            e->value = atoi(tokens[2]);
            hash_replace(ptr->data, e);
        }
        // This section executes commands that start with hash_apply
        else if(!strcmp(tokens[0], HASHTABLE_APPLY))
        {
            // hash_apply hash0 square
            // hash_apply hash0 triple
            // token[0]: hash_apply, token[1]: hash0, token[2]: square/triple
            // Search for the instance which name is tokens[1]
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));

            // Apply the function to the elements of the hashtable
            if(!strcmp(tokens[2], "square"))
            {
                hash_apply(ptr->data, &hash_square);
            }
            else if(!strcmp(tokens[2], "triple"))
            {
                hash_apply(ptr->data, &hash_triple);
            }
        }
        // This section executes commands that start with hash_empty
        else if(!strcmp(tokens[0], HASHTABLE_EMPTY))
        {
            // Search for the instance which name is tokens[1]
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));

            // Print whether the hashtable is empty or not
            if(hash_empty(ptr->data))
            {
               fputs("true\n", stdout); 
            }
            else
                fputs("false\n", stdout);
        }
        // This section executes commands that start with hash_size
        else if(!strcmp(tokens[0], HASHTABLE_SIZE))
        {
            // Search for the instance which name is tokens[1]
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));

            // Print the size of the hashtable
            fprintf(stdout, "%zu\n", hash_size(ptr->data));
        }
        // This section executes commands that start with hash_clear
        else if(!strcmp(tokens[0], HASHTABLE_CLEAR))
        {
            // Search for the instance which name is tokens[1]
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));

            // Clear the hashtable
            hash_clear(ptr->data, &hash_destroyer);
        }
        // This section executes commands that start with bitmap_mark
        else if(!strcmp(tokens[0], BITMAP_MARK))
        {
            // Search for the instance which name is tokens[1]
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));

            // Mark the bit of the bitmap
            bitmap_mark(ptr->data, atoi(tokens[2]));
        }
        // This section executes commands that start with bitmap_test
        else if(!strcmp(tokens[0], BITMAP_TEST))
        {
            // bitmap_test bm0 4
            // Search for the instance which name is tokens[1]
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));
            
            // Print whether the bit is true or false
            if(bitmap_test(ptr->data, atoi(tokens[2])))
            {
                fputs("true\n", stdout);
            }
            else
                fputs("false\n", stdout);
        }
        // This section executes commands that start with bitmap_size
        else if(!strcmp(tokens[0], BITMAP_SIZE))
        {
            // Search for the instance which name is tokens[1]
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));

            // Print the size of the bitmap
            fprintf(stdout, "%zu\n", bitmap_size(ptr->data));
        }
        // This section executes commands that start with bitmap_set
        else if(!strcmp(tokens[0], BITMAP_SET))
        {
            // bitmap_set bm0 0 false
            // Search for the instance which name is tokens[1]
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));

            // Set the bit of the bitmap
            if(!strcmp(tokens[3], "true"))
            {
                bitmap_set(ptr->data, atoi(tokens[2]), true);
            }
            else
                bitmap_set(ptr->data, atoi(tokens[2]), false);
        }
        // This section executes commands that start with bitmap_set_all
        else if(!strcmp(tokens[0], BITMAP_SET_ALL))
        {
            // bitmap_set_all bm0 true
            // Search for the instance which name is tokens[1]
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));

            // Set all the bits of the bitmap
            if(!strcmp(tokens[2], "true"))
            {
                bitmap_set_all(ptr->data, true);
            }
            else
                bitmap_set_all(ptr->data, false);
        }
        // This section executes commands that start with bitmap_set_multiple
        else if(!strcmp(tokens[0], BITMAP_SET_MULTIPLE))
        {
            // bitmap_set_multiple bm0 2 9 false
            // Search for the instance which name is tokens[1]
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));

            // Set multiple bits of the bitmap
            if(!strcmp(tokens[4], "true"))
            {
                bitmap_set_multiple(ptr->data, atoi(tokens[2]), atoi(tokens[3]), true);
            }
            else
                bitmap_set_multiple(ptr->data, atoi(tokens[2]), atoi(tokens[3]), false);
        }
        // This section executes commands that start with bitmap_scan
        else if(!strcmp(tokens[0], BITMAP_SCAN))
        {
            // Search for the instance which name is tokens[1]
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));

            // Scan the bitmap
            if(!strcmp(tokens[4], "true"))
            {
                fprintf(stdout, "%zu\n", bitmap_scan(ptr->data, atoi(tokens[2]), atoi(tokens[3]), true));
            }
            else if(!strcmp(tokens[4], "false"))
            {
                fprintf(stdout, "%zu\n", bitmap_scan(ptr->data, atoi(tokens[2]), atoi(tokens[3]), false));
            }
        }
        // This section executes commands that start with bitmap_scan_and_flip
        else if(!strcmp(tokens[0], BITMAP_SCAN_AND_FLIP))
        {
            // bitmap_scan_and_flip bm0 0 1 true
            // Search for the instance which name is tokens[1]
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));

            // Scan and flip the bitmap
            if(!strcmp(tokens[4], "true"))
            {
                fprintf(stdout, "%zu\n", bitmap_scan_and_flip(ptr->data, atoi(tokens[2]), atoi(tokens[3]), true));
            }
            else if(!strcmp(tokens[4], "false"))
            {
                fprintf(stdout, "%zu\n", bitmap_scan_and_flip(ptr->data, atoi(tokens[2]), atoi(tokens[3]), false));
            }
        }
        // This section executes commands that start with bitmap_reset
        else if(!strcmp(tokens[0], BITMAP_RESET))
        {
            // Search for the instance which name is tokens[1]
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));

            // Reset the bitmap
            bitmap_reset(ptr->data, atoi(tokens[2]));
        }
        // This section executes commands that start with bitmap_none
        else if(!strcmp(tokens[0], BITMAP_NONE))
        {
            // Search for the instance which name is tokens[1]
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));

            // Print if the bits in the range do not contain a specific value
            if(bitmap_none(ptr->data, atoi(tokens[2]), atoi(tokens[3])))
            {
                fputs("true\n", stdout);
            }
            else
                fputs("false\n", stdout);
        }
        // This section executes commands that start with bitmap_flip
        else if(!strcmp(tokens[0], BITMAP_FLIP))
        {
            // Search for the instance which name is tokens[1]
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));

            // Flip the bits of the bitmap
            bitmap_flip(ptr->data, atoi(tokens[2]));
        }
        // This section executes commands that start with bitmap_expand
        else if(!strcmp(tokens[0], BITMAP_EXPAND))
        {
            // Search for the instance which name is tokens[1]
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));

            // Expand the bitmap
            ptr->data = bitmap_expand(ptr->data, bitmap_size(ptr->data) + atoi(tokens[2]));
        }
        // This section executes commands that start with bitmap_contains
        else if(!strcmp(tokens[0], BITMAP_CONTAINS))
        {
            // Search for the instance which name is tokens[1]
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));

            // Print if the bits in the range contain a specific value
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
        // This section executes commands that start with bitmap_count
        else if(!strcmp(tokens[0], BITMAP_COUNT))
        {
            // bitmap_count bitmap 0 5 true
            // token[0]: bitmap_count, token[1]: bitmap, token[2]: 0, token[3]: 5, token[4]: true
            // Search for the instance which name is tokens[1]
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));

            // Count the number of bits in the range that contain a specific value
            if(!strcmp(tokens[4], "true"))
            {
                fprintf(stdout, "%zu\n", bitmap_count(ptr->data, atoi(tokens[2]), atoi(tokens[3]), true));
            }
            else if(!strcmp(tokens[4], "false"))
            {
                fprintf(stdout, "%zu\n", bitmap_count(ptr->data, atoi(tokens[2]), atoi(tokens[3]), false));
            }
        }
        // This section executes commands that start with bitmap_any
        else if(!strcmp(tokens[0], BITMAP_ANY))
        {
            // Search for the instance which name is tokens[1]
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));

            // Print if the bits in the range contain a specific value
            if(bitmap_any(ptr->data, atoi(tokens[2]), atoi(tokens[3])))
            {
                fputs("true\n", stdout);
            }
            else
                fputs("false\n", stdout);
        }
        // This section executes commands that start with bitmap_all
        else if(!strcmp(tokens[0], BITMAP_ALL))
        {
            // Search for the instance which name is tokens[1]
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));

            // Print if every bit in the range contains a specific value
            if(bitmap_all(ptr->data, atoi(tokens[2]), atoi(tokens[3])))
            {
                fputs("true\n", stdout);
            }
            else
                fputs("false\n", stdout);
        }
        // This section executes commands that start with bitmap_dump
        else if(!strcmp(tokens[0], BITMAP_DUMP))
        {
            // Search for the instance which name is tokens[1]
            for(ptr = list_entry(list_begin(instances), struct list_item, elem); (&ptr->elem != list_end(instances)) && strcmp(ptr->name, tokens[1]); ptr = list_entry(list_next(&ptr->elem), struct list_item, elem));

            // Dump the bitmaps
            bitmap_dump(ptr->data);
        }
    }
    // Free the memory for INSTANCES
    list_delete(instances);
    return 0;
}