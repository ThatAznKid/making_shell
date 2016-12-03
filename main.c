#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <errno.h>

#include "main.h"
#include "helper.h"

/******************************************
 * builtIn: deals with the functions that we can't execvp 
 * Input: recieves the command and checks to see if it's cd or exit 
 * Output: if it is cd or exit, acts accordingly
******************************************/
void builtIn (char * command[]){
	if (strcmp(command[0],"cd") == 0 ) { 
		//printf ("GOTTEM! CD HAS BEEN FOUND!\n");
		char s[50];
		getcwd(s,50);
		printf("%s\n",s);
		chdir(command[1]);
		getcwd(s,50);
		printf("%s\n",s);
	}
	if (strcmp(command[0],"exit") == 0 ) {
		//printf ("OOF! GOTTE! EXIT HAS BEEN FOUND\n");
		exit(0);
	}
}

/******************************************
 * semicolonHandler: parses by semicolon, removes any extraneous whitespace, then proceeds the run the commands
 * Input: command string
 * Output: runs whatever is given one by one 
******************************************/

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

/******************************************
 * handle_redirection: checks for ">" or "<" and acts accordingly 
 * Input: command in array form 
 * Output: acts accordingly after modifications 
******************************************/

void handle_redirection (char * command []) {
	//if statements to avoid seg faults
	if (find_str(command,"<") > 0 | find_str(command,">") > 0) {
		if (strcmp(command[1],">") == 0) {
			int fd = open(command[2], O_CREAT | O_TRUNC | O_WRONLY , 0644);  
			dup2(fd, STDOUT_FILENO); // redirect stdout to file_name
			close(fd);
			command[1] = '\0'; // null terminate
			//execvp(command[0], command);
		}
		else if (strcmp(command[1],"<") == 0) {
			int fd = open(command[2], O_RDONLY);
			if ( fd == -1 ) { // file does not exist
				printf("%s: No such file\n", command[2]);
				return;
			}
			dup2(fd, STDIN_FILENO); // redirect stdin to file_name
			close(fd);
			command[1] = 0; // null terminate
			//execvp(command[0], command);
		}
	}
}
 
/******************************************
 * handle_pipes: checks for "|" and acts accordingly
 * Input: command in array form 
 * Output: acts accordingly after modifications 
******************************************/
void handle_pipes (char * command []) {
	//if statement to avoid seg faults
	//printf("DID I MAKE IT HERE");
	if (find_str(command,"|") > 0) {
		if (strcmp(command[1],"|") == 0) {
			//printf("am i here");
			int fs[2];
			pipe(fs);
			int f1 = dup(STDOUT_FILENO);
			int f2 = dup(STDIN_FILENO);
			int stat;
			int i; 
			/*
			for (i = 0; command[i] != NULL; i++) {
				printf("command[%d]: %s\n", i, command[i]);
			}
			*/
			int f = fork();
			if (f == 0){
				//close(fs[0]); 
				dup2(fs[1], STDOUT_FILENO); 
				close(fs[0]);
				execute(command[2]);
				//exit(0);
			}
			else{
				wait(&stat);
				dup2(fs[0], STDIN_FILENO);
				close(fs[1]);
				execute(command[0]);
				dup2(f2, STDIN_FILENO);
			}
		}
	}
}

int execute(char *line){

  char **cmds = (char **)malloc(1000);
  char **args = (char **)malloc(256);

  if ( !(strcmp(args[0], "exit")) ) {
    printf("Exiting SeaShell...\n");
    exit(0);
  }

  if ( !(strcmp(args[0], "cd" )) ) {
    if (args[1] == NULL){
      args[1] = "~";
    }
    if ( !(strcmp(args[1], "~")) ) {
      chdir(getenv("HOME"));
    }
    else {
      if (!(chdir(args[1]) == 0)){
        printf("Failed to change directory!\n");
      }
    }
    free(args);
    free(cmds);
    return 0;
  }

  int pid = fork();

  if (pid == -1) {
    char *error = strerror(errno);
    printf("Process Error: %s\n", error);
    exit(0);
    free(args);
    free(cmds);
    return 0;
  }

  else if (pid == 0) {
    execvp(args[0], args);
    char * error = strerror(errno);
    printf("Command Error for %s: %s\n", args[0], error);
    exit(0);
    free(args);
    free(cmds);
    return 0;
  }

  else {
    int cstat;
    waitpid(pid, &cstat, 0);
    free(args);
    free(cmds);
    return 1;
  }
  free(args);
  free(cmds);
  return 1;
}

/******************************************
 * run: main function to be used, strseps the command to a readable state for exec and then forks and execs
 * Input: command str 
 * Output: whatever the command does 
******************************************/

void run (char * str) {
	char * command[100];
	int i = 0;
	//printf("AM I HERE");
	while(str){
		command[i] = strsep(&str," ");
		i++;
	}
	command[i] = '\0';
	builtIn(command);
	handle_redirection(command);
	handle_pipes(command);
	//printf("found the funnel at %d\n", find_str(command,">"));
	int f = fork();
	int status,r;
	if (f == 0) {
		//printf("Hello I'm %d, Parent PID: %d\n", getpid(), getppid());
		execvp(command[0],command);
	}
	else{
		r = wait (&status);
		//printf("Hello I'm %d\n", getpid());
	}
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
