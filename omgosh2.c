//
// Created by kfwong on 2/10/17.
//

#include <stdio.h>
#include <memory.h>
#include <malloc.h>
#include <zconf.h>
#include <wait.h>
#include <stdlib.h>
#include "omgosh2.h"

int main(){
    print_header();

    omgosh_loop();

    return 0;
}

void print_header() {
    printf("\n"
                   "██████╗ ███╗   ███╗ ██████╗  ██████╗ ███████╗██╗  ██╗\n"
                   "██╔═══██╗████╗ ████║██╔════╝ ██╔═══██╗██╔════╝██║  ██║\n"
                   "██║   ██║██╔████╔██║██║  ███╗██║   ██║███████╗███████║\n"
                   "██║   ██║██║╚██╔╝██║██║   ██║██║   ██║╚════██║██╔══██║\n"
                   "╚██████╔╝██║ ╚═╝ ██║╚██████╔╝╚██████╔╝███████║██║  ██║\n"
                   " ╚═════╝ ╚═╝     ╚═╝ ╚═════╝  ╚═════╝ ╚══════╝╚═╝  ╚═╝\n"
                   "\n");
}

void omgosh_loop() {
    char *line;
    char **args;
    int status;

    printf("Starting omgosh Shell...\n");

    do {
        printf("> ");
        line = omgosh_read_line();
        args = omgosh_split_line(line);

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

            if(execvp(args[0], args) < 0){
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

char *omgosh_read_line() {
    const int BUFFER_SIZE = 1024;
    char line[BUFFER_SIZE];

    fgets(line, BUFFER_SIZE, stdin);

    // http://www.java2s.com/Code/C/Console/Usefgetstoreadstringfromstandardinput.htm
    size_t last_char = strlen(line)-1;
    if(line[last_char] == '\n'){
        line[last_char] = '\0';
    }

    // http://stackoverflow.com/questions/14416759/return-char-string-from-a-function
    // need to return the char array...
    // potential memory leak... should caller free the mem?
    char *pline = (char *) malloc(sizeof(char) * 3);

    int i;
    for(i=0; i< BUFFER_SIZE; i++){
        pline[i] = line[i];
    }

    return pline;
}

char **omgosh_split_line(char *line) {
    // http://stackoverflow.com/questions/26597977/split-string-with-multiple-delimiters-using-strtok-in-c
    const char DELIMITER[] = " \t\r\n\v\f";
    const int BUFFER_SIZE_INCREMENT = 64;

    int buffer_size = 64; // initial buffer size
    int position = 0;
    char **tokens = malloc(buffer_size * sizeof(char *));

    // http://stackoverflow.com/questions/3889992/how-does-strtok-split-the-string-into-tokens-in-c
    char *token;
    for (token = strtok(line, DELIMITER); token != NULL; token = strtok(NULL, DELIMITER)) {
        tokens[position] = token;
        position++;

        // exceeded the buffer, need to reallocate memory & increase buffer size
        if (position >= buffer_size) {
            buffer_size += BUFFER_SIZE_INCREMENT;
            tokens = realloc(tokens, buffer_size * sizeof(char *));
        }
    }

    // Set the last item to NULL so the code knows when it's reached the end.
    tokens[position] = NULL;

    return tokens;
}