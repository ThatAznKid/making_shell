#pragma once

//first of two whitespace functions 
//removes occurences of >1 whitespaces, these screw up exec otherwise
void removeExtraWhite (char * str);

//second of two whitespace functions
//removes all whitespace at the beginning and end of the command, same reason as before 
void trimFrontEndWhite (char * str);

//helper function 
//looks through array and returns first occurence of str
int find_str (char * command[], char str []);
