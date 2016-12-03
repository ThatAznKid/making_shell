# making_shell
Project #1 Systems

by Jiawei Chen 

Features: 
    Forks and executes commands!
	Parses multiple commands on one line!
	Redirects using >, <!
    Can take commands with any amount of whitespace! 
    
Attempted: 
    Could not get pipes to work. :( 
    (can't tell if the issue is with me not understanding pipe() completely {which apparently we needed to use either that or temp files}, or there's a problem with the code itself)
    
Bugs/Limitations:
    Noticed this when working with < - 
        If I were to put a command into help.txt (like ls -l), there must be a space afterwards or else things get messed up (meaning "ls -l " not "ls -l"
 
        
Files and Function Headers: 

    helper.c: 
    
        /******************************************
         * removeExtraWhite: removes double occurences of white space 
         * Input: takes a string 
         * Output: removes the extra white space in place 
        ******************************************/
        Ex: "ls          " will still work as "ls"
        
        /******************************************
         * trimFrontEndWhite: removes white space at the beginning and end of the string 
         * Input: takes a string 
         * Output: removes white space at the beginning and end in place
        ******************************************/
        Ex: "     ls   " will still work as "ls"
        
        /******************************************
         * find_str: takes two arrays and tries to find str inside command 
         * Input: char to be found and command to find the char in 
         * Output: returns the int place of the char or -1 if not found 
        ******************************************/
        Ex: If you have command[] with command[0] = "ls", command[1] = ">", command[2] = "help.txt", you will find and return 1 when running find_str(command, ">")
        
    main.c: 
    
        /******************************************
         * builtIn: deals with the functions that we can't execvp 
         * Input: recieves the command and checks to see if it's cd or exit 
         * Output: if it is cd or exit, acts accordingly
        ******************************************/
        Ex: cd ..
        /home/ubuntu/workspace/making_shell -> /home/ubuntu/workspace
        
        /******************************************
         * semicolonHandler: parses by semicolon, removes any extraneous whitespace, then proceeds the run the commands
         * Input: command string
         * Output: runs whatever is given one by one 
        ******************************************/
        Ex: ls -l ; echo hello 
        Runs ls -l -> Runs echo hello
        
        /******************************************
         * handle_redirection: checks for ">" or "<" and acts accordingly 
         * Input: command in array form 
         * Output: acts accordingly after modifications 
        ******************************************/
        Ex: ls > help.txt  
        contents of ls now in help.txt 
        
        change contents of help.txt to only "ls -l " 
        ./a.out < help.txt 
        runs "ls -l" as expected
    
        /******************************************
         * handle_pipes: checks for "|" and acts accordingly
         * Input: command in array form 
         * Output: acts accordingly after modifications 
        ******************************************/
        :'(
        
        /******************************************
         * run: main function to be used, strseps the command to a readable state for exec and then forks and execs
         * Input: command str 
         * Output: whatever the command does 
        ******************************************/
        
FLOW: 
    Sort of an extra section to describe what I'm trying to do when I run the program 
    My shell takes one line of command to start off with, chops off the newline, and then passes it to the semicolonHandler(). 
    From there, it strseps the command by ; and then proceeds to run one command at a time, until there are none left. 
    For the first command (or if it's just one command with no ; to begin with), all the extraneous white space is gotten rid of and then passed to run(). 
    run() breaks the string into an exec friendly array and then runs builtIn() (to see if any of the commands are cd or exit), handle_redirection() (if there is a "<" or ">" detected), or handle_pipes() (if there is a "|" detected)
    From there it just forks and exec's the program. 
    If there are any other commands queued up from semicolonHandler(), then those will be run. Otherwise, the program finishes there.
