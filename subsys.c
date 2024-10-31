#include "subsystem.h"
#include <string.h>

/*
    This file should contain the implementations for Subsystem specific functionality.
    Initialization, printing, setting and getting data and statuses.
*/
int subsys_init(Subsystem *subsystem, const char *name, char status){
    strncpy(subsystem->name, name, MAX_STR);
    subsystem->name[MAX_STR-1] = '\0';
    subsystem->status = status;
    subsystem->data = 0;
    return ERR_SUCCESS;
}
int subsys_print(Subsystem *subsystem){
    printf("\n-[Name: %16s, ", subsystem->name);
    subsys_status_print(subsystem);
    return ERR_SUCCESS;
}
int subsys_status_set(Subsystem *subsystem, unsigned char status, unsigned char value){
    if (value >= 2){
        if (status==0 || status == 2){ //Todo: when trying to edit populated 2-bit, ends up not depopulating correctly
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
        if (status==1 || status==3){
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
int subsys_status_print(const Subsystem *subsystem){
    printf("(PWR: %d|DATA: %d|ACT: %d|ERR: %d|PERF: %d|RES: %d)", 
    subsystem->status & (1<<STATUS_POWER),subsystem->status & (1<<STATUS_DATA),subsystem->status & (1<<STATUS_ACTIVITY),
    subsystem->status & (1<<STATUS_ERROR),subsystem->status & (1<<STATUS_PERFORMANCE+1)*2+subsystem->status & (1<<STATUS_PERFORMANCE),
    subsystem->status & (1<<STATUS_RESOURCE+1)*2+subsystem->status & (1<<STATUS_RESOURCE));
    return ERR_SUCCESS;
}
int subsys_data_set(Subsystem *subsystem, unsigned int new_data, unsigned int *old_data){
    if (old_data == NULL){
        subsystem->data = new_data;
        subsystem->status = subsystem->status | (1<<STATUS_DATA);
        return ERR_SUCCESS;
    }
    else{
        *old_data = subsystem->data;
        subsystem->data = new_data;
        subsystem->status = subsystem->status | (1<<STATUS_DATA);
        return ERR_SUCCESS;
    }
}
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