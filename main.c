#include <stdio.h>
#include <stdlib.h>
#include "subsystem.h"

#define MENU_ADD 1
#define MENU_PRINT 2
#define MENU_PRINTALL 3
#define MENU_STATUS 4
#define MENU_REMOVE 5
#define MENU_FILTER 6
#define MENU_DATA 7
#define MENU_EXIT 0
#define MENU_MIN 0
#define MENU_MAX 7

int print_menu(int *choice);

int main(void) {
    // Use a while loop to print the menu for the user and call the appropriate functions.
    // The definitions above are provided to assist with this.
    SubsystemCollection collection;
    subsys_collection_init (&collection);
    Subsystem subsys;
    SubsystemCollection FilterColl;
    subsys_collection_init(&FilterColl);
    char name[MAX_STR];
    unsigned char status;
    unsigned int choice = 1; // This is assigned a number because it would otherwise default to 0, and would immediately exit the while loop since MENU_EXIT is also 0
    unsigned char value;
    unsigned int find;
    unsigned int data;
    unsigned char filter;
    int index;
    int remove;
    while (choice != MENU_EXIT){
        if (print_menu(&choice)==ERR_SUCCESS){
            switch (choice){
                case MENU_ADD:
                    printf("Enter subsystem name (no spaces): ");
                    scanf("%s", name);
                    subsys_init(&subsys, name, 0);
                    if (subsys_append(&collection, &subsys)==ERR_MAX_CAPACITY){
                        printf("The subsystem collection has reached maximum capacity");
                    }
                    break;
                case MENU_PRINT:
                    printf("Enter the name of the subsystem to be printed: ");
                    scanf("%s",name);
                    find = subsys_find(&collection, name);
                    if (find != ERR_SYS_NOT_FOUND){                
                        subsys_print(&subsys);                    
                    }
                    else {
                        printf("No such subsystem found");
                    }
                    break;
                case MENU_PRINTALL:
                    subsys_collection_print(&collection);
                    break;
                case MENU_STATUS:
                    printf("Enter: <Subsystem name> <Status ID:7,6,5,4,2,0> <New value(0-3)>:");
                    scanf("%s %hhu %hhu", name, &status, &value);
                    find = subsys_find(&collection, name);
                    if (find != ERR_SYS_NOT_FOUND && subsys_status_set(&collection.subsystems[find], status, value) != ERR_INVALID_STATUS){
                        printf("Status updated successfully");
                    }
                    else{
                        printf("No such subsystem found");
                    }
                    break;
                case MENU_DATA:
                    printf("Enter: <Subsystem Name> <Hex value without 0X>:");
                    scanf("%s, %x", name, &data);
                    find = subsys_find(&collection, name);
                    if (find != ERR_SYS_NOT_FOUND){                
                        subsys_data_set(&collection.subsystems[find], data, &collection.subsystems[find].data);
                        printf("Data entered successfully");         
                    }
                    else {
                        printf("No such subsystem found");
                    }
                case MENU_REMOVE:
                    printf("Enter the index of a subsystem to be removed:");
                    scanf("%d", &index);
                    remove=subsys_remove(&collection, index);
                    if (remove == ERR_INVALID_INDEX){
                        printf ("Invalid index entered");
                    }
                    else if (remove == ERR_NO_DATA){
                        printf ("No subsystems in the collection to be removed");
                    }
                    else{
                        printf ("Subsystem removed successfully");
                    }
                case MENU_FILTER:
                    printf("Enter a string of length 8 (Valid characters are: 0,1,*):");
                    scanf("%s", &filter);
                    subsys_filter(&collection, &FilterColl, &filter);
                    printf("Subsystems filtered: %d", FilterColl.size);
                    subsys_collection_print(&FilterColl);
                    break;
                default:
                    break;
            }
        }
    }
    return 0;
}

/* 
    Prints a menu to the user and prompts them until they enter a valid menu option.

    out: *choice, an integer corresponding to a MENU_ definition defining the choice made by the user.

    Returns: 
        - ERR_SUCCESS if there is no error
        - ERR_NULL_POINTER if `choice` is NULL

*/
int print_menu(int *choice) {
    if (choice == NULL) {
        return ERR_NULL_POINTER;
    }

    *choice = 0;

    do {
        printf("\n--- Subsystem Management Menu ---\n");
        printf("%d. Add Subsystem\n", MENU_ADD);
        printf("%d. Print Subsystem\n", MENU_PRINT);
        printf("%d. Print All Subsystems\n", MENU_PRINTALL);
        printf("%d. Set Subsystem Status\n", MENU_STATUS);
        printf("%d. Remove Subsystem\n", MENU_REMOVE);
        printf("%d. Filter Subsystems\n", MENU_FILTER);
        printf("%d. Set Subsystem Data\n", MENU_DATA);
        printf("%d. Exit\n", MENU_EXIT);
        printf("Select an option: ");
        scanf("%d", choice);
        while (getchar() != '\n');

        if (*choice < MENU_MIN || *choice > MENU_MAX) {
            printf("Error: Invalid choice. Please try again.\n");
        }
    } while (*choice < MENU_MIN || *choice > MENU_MAX);

    return ERR_SUCCESS;
}