# Project 1, CUinSPACE Rocket Status
This program takes user inputted commands to initialize and edit the subsystems of a rocket, each subsystem has a name stored as a string,
a status represented by a unsigned char with each part of the status being specific bits in the char, and a data value, represented by a 
single unsigned char. Bitwise operations are used to change the status of certain attributed within a Subsystem. The subsystems are managed
in a collection where they are stored in a array in which it can be added to, removed from, and filtered through.

## Building and Running
1. Open a terminal and navigate to the folder containing the program's files.
2. Type the command 'gcc -o p1 main.c subsys.c, subsys_collection,c, subsystem.h'.
3. If compiled correctly, the terminal should go to a new command line.
4. from the same directory, run './p1' to execute the project.

## Credits
    - Solely developped by Ivan Cheung (101312122)