#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "helper.h"

//execvp does not work when you feed it extra white space
//following two functions deal with this

/******************************************
 * removeExtraWhite: removes double occurences of white space 
 * Input: takes a string 
 * Output: removes the extra white space in place 
******************************************/

void removeExtraWhite (char * str) {
	int i,x;
	for(i=x=1; str[i]; ++i)
	  if(!isspace(str[i]) || (i>0 && !isspace(str[i-1]))) {
	    str[x++] = str[i];
	  }
	str[x] = '\0';
}

/******************************************
 * trimFrontEndWhite: removes white space at the beginning and end of the string 
 * Input: takes a string 
 * Output: removes white space at the beginning and end in place
******************************************/

void trimFrontEndWhite(char *str)
{
    int i;
    int begin = 0;
    int end = strlen(str) - 1;

    while (isspace((unsigned char) str[begin]))
        begin++;

    while ((end >= begin) && isspace((unsigned char) str[end]))
        end--;

    // Shift all characters back to the start of the string array.
    for (i = begin; i <= end; i++)
        str[i - begin] = str[i];

    str[i - begin] = '\0'; // Null terminate string.
}

/******************************************
 * find_str: takes two arrays and tries to find str inside command 
 * Input: char to be found and command to find the char in 
 * Output: returns the int place of the char or -1 if not found 
******************************************/

int find_str (char * command[], char str []) {
	int i; 
	for (i = 0; command[i] != NULL; i++) {
		if ( strcmp (command[i], str) == 0 ) {
			return i;
			//printf("command[%d]: %s\n", i, command[i]);
		}
	}
	return -1;
}