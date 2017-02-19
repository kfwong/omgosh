
/*
 * Name: Wong Kang Fei
 * Matric No: A0138862W
 *
 * Name: Chew Xing Kai
 * Matric No: A0107476B
 *
 */

#include <stdio.h>
#include <memory.h>
#include <zconf.h>
#include <wait.h>
#include <stdlib.h>
#include "omgosh2.h"

#define BUFFER_SIZE 1024

int main(){
    print_header();

    omgosh_loop();

    return 0;
}

void print_header() {
    printf("      ___           ___           ___           ___           ___           ___     \n"
                   "     /\\  \\         /\\__\\         /\\  \\         /\\  \\         /\\  \\         /\\__\\    \n"
                   "    /::\\  \\       /::|  |       /::\\  \\       /::\\  \\       /::\\  \\       /:/  /    \n"
                   "   /:/\\:\\  \\     /:|:|  |      /:/\\:\\  \\     /:/\\:\\  \\     /:/\\ \\  \\     /:/__/     \n"
                   "  /:/  \\:\\  \\   /:/|:|__|__   /:/  \\:\\  \\   /:/  \\:\\  \\   _\\:\\~\\ \\  \\   /::\\  \\ ___ \n"
                   " /:/__/ \\:\\__\\ /:/ |::::\\__\\ /:/__/_\\:\\__\\ /:/__/ \\:\\__\\ /\\ \\:\\ \\ \\__\\ /:/\\:\\  /\\__\\\n"
                   " \\:\\  \\ /:/  / \\/__/~~/:/  / \\:\\  /\\ \\/__/ \\:\\  \\ /:/  / \\:\\ \\:\\ \\/__/ \\/__\\:\\/:/  /\n"
                   "  \\:\\  /:/  /        /:/  /   \\:\\ \\:\\__\\    \\:\\  /:/  /   \\:\\ \\:\\__\\        \\::/  / \n"
                   "   \\:\\/:/  /        /:/  /     \\:\\/:/  /     \\:\\/:/  /     \\:\\/:/  /        /:/  /  \n"
                   "    \\::/  /        /:/  /       \\::/  /       \\::/  /       \\::/  /        /:/  /   \n"
                   "     \\/__/         \\/__/         \\/__/         \\/__/         \\/__/         \\/__/    \n"
                   "\nFirst Assignment for CS2106 Operating System v0.1\n");
}

void omgosh_loop() {
    int status;

    printf("Starting omgosh Shell...\n");

    do {
        printf("> ");
        char line[BUFFER_SIZE];
        char *splitted_line[BUFFER_SIZE];

        omgosh_read_line(line);
        omgosh_split_line(line, splitted_line);

        int pid = fork();

        // see the return value from fork() to identify between parent and child
        // https://linux.die.net/man/2/fork
        if (pid == 0) {
            // child process
            int child_process_id = getpid();
            int parent_process_id = getppid();

            printf("Loading new process with id %d\n", child_process_id);
            printf("Parent pid: %d\n", parent_process_id);

            // about the exec() family
            // http://stackoverflow.com/questions/4204915/please-explain-exec-function-and-its-family
            // http://stackoverflow.com/questions/33598869/execve-not-taking-environment-parameters

            // checking execvp error
            // https://linux.die.net/man/3/explain_execvp

            char cwd[1024];
            getcwd(cwd, sizeof(cwd));

            setenv("SHELL_PATH", cwd, 1);

            if(execvp(splitted_line[0], splitted_line) < 0){
                perror("Shell error");
                exit(EXIT_FAILURE);
            }else{
                exit(EXIT_SUCCESS);
            };

        } else {
            // parent process

            // http://stackoverflow.com/questions/21248840/example-of-waitpid-in-use
            // wait for all children to terminate
            waitpid(-1, &status, 0);

        }

        // for debug: print out the array of args
        /*
        for(int i = 0; args[i] != NULL; i++){
            printf(args[i]);
            printf("\n");
        }
         */
    } while (1);

}

void omgosh_read_line(char buffer[]) {

    if(fgets(buffer, BUFFER_SIZE, stdin)){
        // http://www.java2s.com/Code/C/Console/Usefgetstoreadstringfromstandardinput.htm
        size_t last_char = strlen(buffer)-1;
        if(buffer[last_char] == '\n'){
            buffer[last_char] = '\0';
        }
    }

}

void omgosh_split_line(char buffer[], char *result[]) {
    // http://stackoverflow.com/questions/26597977/split-string-with-multiple-delimiters-using-strtok-in-c
    const char DELIMITER[] = " \t\r\n\v\f";

    int position = 0;

    // http://stackoverflow.com/questions/3889992/how-does-strtok-split-the-string-into-tokens-in-c
    char *token;
    for (token = strtok(buffer, DELIMITER); token != NULL; token = strtok(NULL, DELIMITER)) {
        result[position] = token;
        position++;
    }

    // Set the last item to NULL so the code knows when it's reached the end.
    result[position] = NULL;
}