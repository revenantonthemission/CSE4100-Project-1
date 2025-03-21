#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_OBJECT_NAME_LENGTH 20
#define MAX_COMMAND_LENGTH 20
#define COMMAND_CREATE "create"
#define COMMAND_DELETE "delete"
#define COMMAND_DUMPDATA "dumpdata"
#define COMMAND_QUIT "quit"
#define OBJECT_LIST "list"
#define OBJECT_HASHTABLE "hashtable"
#define OBJECT_BITMAP "bitmap"

int main() {
    char command[MAX_COMMAND_LENGTH] = {}, object_name[MAX_OBJECT_NAME_LENGTH] = {};
    int bit_cnt = 0;

    while (1) {
        printf("Enter command: ");
        scanf("%s", command);

        if (strcmp(command, COMMAND_CREATE) == 0) {
            scanf("%s", object_name);
            if (strcmp(object_name, OBJECT_LIST) == 0) {
                scanf("%s", object_name);
                //dummy
                printf("Creating list: %s\n", object_name);
            } else if (strcmp(object_name, OBJECT_HASHTABLE) == 0) {
                scanf("%s", object_name);
                //dummy
                printf("Creating hashtable: %s\n", object_name);
            } else if (strcmp(object_name, OBJECT_BITMAP) == 0) {
                scanf("%s %d", object_name, &bit_cnt);
                //dummy
                printf("Creating bitmap: %s with %d bits\n", object_name, bit_cnt);
            } else {
                printf("Error: Invalid create command\n");
            }
        } else if (strcmp(command, COMMAND_DELETE) == 0) {
            scanf("%s", object_name);
            //dummy
            printf("Deleting: %s\n", object_name);
        } else if (strcmp(command, COMMAND_DUMPDATA) == 0) {
            scanf("%s", object_name);
            //dummy
            printf("Dumping data for: %s\n", object_name);
        } else if (strcmp(command, COMMAND_QUIT) == 0) {
            printf("Exiting program.\n");
            break;
        } else {
            printf("Error: Invalid command\n");
        }
    }

    return 0;
}