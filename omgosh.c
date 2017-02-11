//
// Created by kfwong on 2/10/17.
//

#include <stdio.h>
#include <memory.h>
#include <malloc.h>
#include <zconf.h>
#include <wait.h>
#include <stdlib.h>
#include "omgosh.h"

int main() {
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
    pid_t status;

    printf("Starting omgosh Shell...\n");

    do {
        printf("> ");
        line = omgosh_read_line();
        args = omgosh_split_line(line);

        pid_t pid = fork();

        // see the return value from fork() to identify between parent and child
        // https://linux.die.net/man/2/fork
        if (pid == 0) {
            // child process
            pid_t child_process_id = getpid();
            pid_t parent_process_id = getppid();

            printf("Loading new process with id %d\n", child_process_id);
            printf("Parent pid: %d\n", parent_process_id);

            // about the exec() family
            // http://stackoverflow.com/questions/4204915/please-explain-exec-function-and-its-family
            // http://stackoverflow.com/questions/33598869/execve-not-taking-environment-parameters

            // checking execvp error
            // https://linux.die.net/man/3/explain_execvp
            if(execvp(args[0], args) < 0){
                fprintf(stderr, "%s\n", "Shell error: No such file or directory");
                exit(EXIT_FAILURE);
            };

        } else {
            // parent process

            // http://stackoverflow.com/questions/21248840/example-of-waitpid-in-use
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
    char *line = NULL;
    size_t buffer_size = 0;

    // info about getline() function
    // http://c-for-dummies.com/blog/?p=1112
    getline(&line, &buffer_size, stdin);

    return line;
}

char **omgosh_split_line(char *line) {
    // http://stackoverflow.com/questions/26597977/split-string-with-multiple-delimiters-using-strtok-in-c
    const char DELIMITER[] = " \t\r\n\v\f";
    const int BUFFER_SIZE_INCREMENT = 64;

    int buffer_size = 64; // initial buffer size
    int position = 0;
    char **tokens = malloc(buffer_size * sizeof(char *));

    // http://stackoverflow.com/questions/3889992/how-does-strtok-split-the-string-into-tokens-in-c
    for (char *token = strtok(line, DELIMITER); token != NULL; token = strtok(NULL, DELIMITER)) {
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