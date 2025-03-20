#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char command[20] = {}, object_name[20] = {};
    int bit_cnt = 0;

    while (1) {
        printf("Enter command: ");
        scanf("%s", command);

        if (strcmp(command, "create") == 0) {
            scanf("%s", object_name);
            if (strcmp(object_name, "list") == 0) {
                scanf("%s", object_name);
                //dummy
                printf("Creating list: %s\n", object_name);
            } else if (strcmp(object_name, "hashtable") == 0) {
                scanf("%s", object_name);
                //dummy
                printf("Creating hashtable: %s\n", object_name);
            } else if (strcmp(object_name, "bitmap") == 0) {
                scanf("%s %d", object_name, &bit_cnt);
                //dummy
                printf("Creating bitmap: %s with %d bits\n", object_name, bit_cnt);
            } else {
                printf("Error: Invalid create command\n");
            }
        } else if (strcmp(command, "delete") == 0) {
            scanf("%s", object_name);
            //dummy
            printf("Deleting: %s\n", object_name);
        } else if (strcmp(command, "dumpdata") == 0) {
            scanf("%s", object_name);
            //dummy
            printf("Dumping data for: %s\n", object_name);
        } else if (strcmp(command, "quit") == 0) {
            printf("Exiting program.\n");
            break;
        } else {
            printf("Error: Invalid command\n");
        }
    }

    return 0;
}