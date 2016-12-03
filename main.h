#pragma once

//deals with the builtin functions we execvp can't cover (namely cd and exit)
void builtIn (char * command[]);

//lets us evaluate the commands one at a time, seperated by semicolons 
//Read and separate multiple commands on one line with ;
void semicolonHandler (char * str);

//one of the main functions 
//does the strsep'ing of the command to make it exec friendly  *** 
//runs other handlers and also forks + execs 
void run (char * str);

//deals with > and < 
//currently only deals with very basic redirection such as ls > help.txt 
//***POSSIBLE EXPANSION*** 
void handle_redirection (char * command[]);

//deals with | (aka pipes)
//***NEEDS TO BE UPDATED NOT WORKING RIGHT NOW
void handle_pipes (char * command[]); 

int execute(char *line);
