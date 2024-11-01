#include "subsystem.h"
#include <string.h>

/*
    This file should contain the implementations for Subsystem specific functionality.
    Initialization, printing, setting and getting data and statuses.
*/
/*
    Initializes a Subsystem, assigning a name to it and giving it a status

    in: 
        *name, a string entered by the user, of max length 32
        status, a value in which it can be decoded with bitwise operations to see the status of different elements in a Subsystem
    
    out: *subsystem, an initialized struct, containing a name (string) and status (unsigned char) value, with data (unsigned char) equal to 0 

    Returns:
        ERR_SUCCESS
*/
int subsys_init(Subsystem *subsystem, const char *name, char status){
    strncpy(subsystem->name, name, MAX_STR);
    subsystem->name[MAX_STR-1] = '\0';
    subsystem->status = status;
    subsystem->data = 0;
    return ERR_SUCCESS;
}
/*
    Prints out the name of a subsystem and it's statuses

    in: *subsystem, a Struct containing a name (string), status (unsigned char), and data (unsigned char)

    Returns:
        ERR_SUCCESS
*/
int subsys_print(Subsystem *subsystem){
    printf("\n-[Name: %16s, ", subsystem->name);
    subsys_status_print(subsystem);
    return ERR_SUCCESS;
}
/*
    Sets a part of the status value within a Subsystem through bitwise operations

    in: 
        value, an integer between 0-3, representing the state of certain parts of the Subsystem
        status, an integer value between 0-7, representing different parts of a Subsystem's status to be changed
    
    out:
        subsystem->status, the status value of the provided Subsystem, which has had bitwise operations done to change some values

    Returns:
        ERR_SUCCESS if the values were changed successfully
        ERR_INVALID_STATUS if the status value entered were invalid e.g. 1 or 3 or if the value entered 
                           was 2 or 3 and the entered status value was not 0 or 2
*/
int subsys_status_set(Subsystem *subsystem, unsigned char status, unsigned char value){
    if (value >= 2){
        if (status==STATUS_PERFORMANCE || status == STATUS_RESOURCE){ //Todo: when trying to edit populated 2-bit, ends up not depopulating correctly
                                       //Solved: missed case of value < 2, therefore going to else statement, therefore only editing one bit
            subsystem->status = subsystem->status & ~(1<<status);
            subsystem->status = subsystem->status & ~(1<<(status+1));
            subsystem->status = subsystem->status | (value<<status);
            return ERR_SUCCESS;
        }
        else{
            return ERR_INVALID_STATUS;
        }
    }
    else{
        if (status==(STATUS_PERFORMANCE+1) || status==(STATUS_RESOURCE+1)){
            return ERR_INVALID_STATUS;
        }
        else if (status==0 || status == 2){
            subsystem->status = subsystem->status & ~(1<<status);
            subsystem->status = subsystem->status & ~(1<<(status+1));
            subsystem->status = subsystem->status | (value<<status);
            return ERR_SUCCESS;
        }
        else{
            subsystem->status = subsystem->status | (value<<status);
            return ERR_SUCCESS;
        }
    }
}
/*
    Prints the status of a given Subsystem

    in: *subsystem, a Struct containing a name (string), status (unsigned char), and data (unsigned char)

    Returns:
        ERR_SUCCESS
*/
int subsys_status_print(const Subsystem *subsystem){
    printf("(PWR: %d|DATA: %d|ACT: %d|ERR: %d|PERF: %d|RES: %d)", 
    subsystem->status & (1<<STATUS_POWER),subsystem->status & (1<<STATUS_DATA),subsystem->status & (1<<STATUS_ACTIVITY),
    subsystem->status & (1<<STATUS_ERROR),subsystem->status & (1<<STATUS_PERFORMANCE+1)*2+subsystem->status & (1<<STATUS_PERFORMANCE),
    subsystem->status & (1<<STATUS_RESOURCE+1)*2+subsystem->status & (1<<STATUS_RESOURCE));
    return ERR_SUCCESS;
}
/*
    Sets the value of data within a Subsystem to a new data, and optionally stores the old data to an int address, sets the data bit in status to 1

    in: new_data, an unsigned integer entered by the user

    out:
        subsystem->data, the data field within the given Subsystem, now populated with the new data
        *old_data, an unsigned int containing the data within the Subsystem before it was overwritten, if it was NULL upon entry, the value is 0
    
    Returns:
        ERR_SUCCESS
*/
int subsys_data_set(Subsystem *subsystem, unsigned int new_data, unsigned int *old_data){
    if (old_data != NULL){
        *old_data = subsystem->data;
    }
    subsystem->data = new_data;
    subsys_status_set(subsystem, STATUS_DATA, 1);
    return ERR_SUCCESS;
}
/*
    Puts the data within a given Subsystem to an unsigned int and clears that Subsystem's data field and sets the data bit in status to 0

    in: *subsystem, a struct containing a name (string), status (unsigned char), and data (unsigned char)

    out: *dest, an unsigned int contianing the data form a given Subsystem

    Returns:
        ERR_SUCCESS if the data was retrieved successfully
        ERR_NO_DATA of the data bit in status was not 1

*/
int subsys_data_get(Subsystem *subsystem, unsigned int *dest){
    if (subsystem->status & (1<<STATUS_DATA)){
        *dest = subsystem->data;
        subsystem->status = subsystem->status & ~(1<<STATUS_DATA);
        subsystem->data = 0;
        return ERR_SUCCESS;
    }
    else {
        *dest = 0;
        return ERR_NO_DATA;
    }
}