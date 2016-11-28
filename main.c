#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

void builtIn (char * command[]){
	if (strcmp(command[0],"cd") == 0 ) { 
		printf ("GOTTEM! CD HAS BEEN FOUND!\n");
		char s[50];
		getcwd(&s,50);
		printf("%s\n",s);
		chdir(command[1]);
		getcwd(&s,50);
		printf("%s\n",s);
	}
	if (strcmp(command[0],"exit") == 0 ) {
		printf ("OOF! GOTTE! EXIT HAS BEEN FOUND\n");
		exit(0);
	}
}

void semicolonHandler (char * str) {
	char * command;
	while (str) {
		command = strsep(&str,";");
		trimFrontEndWhite(command);
		removeExtraWhite(command);
		printf("Command to be run: %s\n", command);
		run(command);
	}
}

void run (char * str) {
	char * command[100];
	int i = 0;
	while(str){
		command[i] = strsep(&str," ");
		i++;
	}
	command[i] = NULL;
	builtIn(command);
	int f = fork();
	int status,r;
	if (f == 0) {
		printf("Hello I'm %d, Parent PID: %d\n", getpid(), getppid());
		execvp(command[0],command);
	}
	else{
		r = wait (&status);
		printf("Hello I'm %d\n", getpid());
	}
}

//execvp does not work when you feed it extra white space
//following two functions deal with this
void removeExtraWhite (char * str) {
	int i,x;
	for(i=x=1; str[i]; ++i)
	  if(!isspace(str[i]) || (i>0 && !isspace(str[i-1]))) {
	    str[x++] = str[i];
	  }
	str[x] = '\0';
}

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

int main(int argc, char *argv[]){

	printf("Insert Command Here: ");
	char line[100];
	fgets(line,sizeof(line),stdin);
	line[strlen(line) - 1] = '\0'; //gets rid of the new line
	char * s = line;
	semicolonHandler(s);
	/*
	char * command[100];
	int i = 0;
	while(s){
		command[i] = strsep(&s," ");
		i++;
	}
	command[i] = NULL;
	int f = fork();
	int status,r;
	if (f == 0) {
		printf("Hello I'm %d, Parent PID: %d\n", getpid(), getppid());
		execvp(command[0],command);
	}
	else{
		r = wait (&status);
		printf("Hello I'm %d\n", getpid());
	}
	*/
	return 0;
}
