#include "subsystem.h"
#include <string.h>

/*
    This file should contain your documentation and implementations for SubsystemCollection 
    specific functionality; initialization, appending, removal, printing, finding, and filtering.
*/
/*
    Initializes a SubsystemCollection and sets it's size to 0.

    out: *subsystems, a struct containing Subsystem structs in an array with a size counter within the struct as well.

    Returns:
        ERR_SUCCESS
*/ 
int subsys_collection_init(SubsystemCollection *subsystems){
    subsystems->size = 0;
    return ERR_SUCCESS;
}
/*
    Appends a Subsystem to a given SubsystemCollection struct's array, and incrementing it's size value by 1

    in: *subsystem, an initialized Subsystem struct

    out: *subsystems, a SubsystemCollection cotaining Subsystem structs

    Returns:
        ERR_SUCCESS if the *subsystem was added successfully
        ERR_MAX_CAPACITY if the SubsystemCollection was already full
*/
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
/*
    Iterates through all elements within the array of a given SubsystemCollection and retrieves the index of the first matching element

    in:
        *subsystems, a SubsystemCollection cotaining Subsystem structs
        *name, a string the user entered to be compared to elements in the array to find the correct Subsystem
    
    Returns:
        ERR_SYS_NOT_FOUND if the name did not match any names of the Subsystems within
        i, the index of the first matching element within the array of the given SubsystemCollection
*/
int subsys_find(const SubsystemCollection *subsystems, const char *name){
    for (size_t i = 0; i<subsystems->size; i++){
        if (!strcmp(subsystems->subsystems[i].name, name)){
            return i;
        }
    }
    return ERR_SYS_NOT_FOUND;
    
}
/*
    Prints through every Subsystem in a SubsystemCollection

    in: *subsystems, a SubsystemCollection cotaining Subsystem structs

    Returns:
        ERR_SUCCESS if all elements were printed
        ERR_NO_DATA if there were no elements to be printed to begin with
*/
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
/*
    Removes a Subsystem from a given SubsytemCollection by index

    in: index, an integer entered by the user to be removed

    out: *subsystems, a SubsystemCollection cotaining Subsystem structs

    Returns:
        ERR_SUCCESS if the Subsystem at the index was correctly removed
        ERR_NO_DATA if the SubsystemCollection is empty
        ERR_INVALID_INDEX if the entered index was not within the bounds of the SubsystemCollection
*/
int subsys_remove(SubsystemCollection *subsystems, int index){
    if (subsystems->size >0){
        for (size_t i=index; i<(subsystems->size-1);i++){
            subsystems->subsystems[i] = subsystems->subsystems[i+1];
        }
        subsystems->size--;
        return ERR_SUCCESS;
    }
    else if (subsystems->size == 0){
        return ERR_NO_DATA;
    }
    else {
        return ERR_INVALID_INDEX;
    }
}
/*
    Creates a filter bitmask and a wildcard bitmask to compare to Subsystems in a SubsystemCollection.
    Comparison does a bitwise XOR operation with the bitmask and a Subsystem's status value, then takes the product of the operation
    and does a OR bitwise operation with the wildcard bitmask.
    If the value of the final product is equal to '11111111' in binary, or 0xFF in hexadecimal.

    in: 
        *src, a SubsystemCollection containing Subsystem structs
        *filter, a string of length 8 inputted by the user containing 0,1,or * as the values of each character
    
    out: *dest, a SubsystemCollection containing the Subsystems that passed the filter

    Returns:
        ERR_SUCCESS
*/
int subsys_filter(const SubsystemCollection *src, SubsystemCollection *dest, const unsigned char *filter){
    unsigned char filtering=0;
    unsigned char wildcard=0;
    for (size_t i =0; i<strlen(filter);i++){
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
