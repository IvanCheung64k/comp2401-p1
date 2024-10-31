#include "subsystem.h"
#include <string.h>

/*
    This file should contain your documentation and implementations for SubsystemCollection 
    specific functionality; initialization, appending, removal, printing, finding, and filtering.
*/
int subsys_collection_init(SubsystemCollection *subsystems){
    subsystems->size = 0;
    return ERR_SUCCESS;
}
int subsys_append(SubsystemCollection *subsystems, const Subsystem *subsystem){
    if (subsystems->size < MAX_ARR){
        subsystems->subsystems[subsystems->size] = *subsystem;
        subsystems->size++;
        return ERR_SUCCESS;
    }
    else{
        return ERR_MAX_CAPACITY;
    }
}
int subsys_find(const SubsystemCollection *subsystems, const char *name){
    for (size_t i = 0; i<subsystems->size; i++){
        if (!strcmp(subsystems->subsystems[i].name, name)){
            return i;
        }
    }
    return ERR_SYS_NOT_FOUND;
}
int subsys_collection_print(SubsystemCollection *subsystems){
    if (subsystems->size >0){
        for (size_t i = 0; i<=subsystems->size; i++){
            subsys_print(&subsystems->subsystems[i]);
        }
        return ERR_SUCCESS;
    }
    else {
        return ERR_NO_DATA;
    }
}
int subsys_remove(SubsystemCollection *subsystems, int index){
    if (subsystems->size >0){
        for (size_t i=index; i<(subsystems->size-1);i++){
            subsystems->subsystems[i] = subsystems->subsystems[i+1];
        }
        subsystems->size--;
        return ERR_SUCCESS;
    }
    else {
        return ERR_NO_DATA;
    }
}
int subsys_filter(const SubsystemCollection *src, SubsystemCollection *dest, const unsigned char *filter){
    unsigned char filtering=0;
    unsigned char wildcard=0;
    for (size_t i =0; i<8;i++){
        if (filter[i]=='1'){
            filtering = filtering | (1 << 7-i);
        }
        else if (filter[i]=='*'){
            wildcard = wildcard | (1<<7-i);
        }
    }
    filtering = ~filtering;
    for (size_t i=0;i<src->size;i++){
        if (((src->subsystems[i].status ^ filtering) | wildcard)==0xFF){
            subsys_append(dest, &src->subsystems[i]);
        }
    }
    return ERR_SUCCESS;
}
