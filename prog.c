#include <stdio.h>
#include <zconf.h>
#include <wait.h>
#include <stdlib.h>

int main() {
    pid_t pid = fork();
    pid_t status = NULL;

    if(pid == 0){
        char *cmd = "/usr/bin/env";
        char *argv[2];

        argv[0] = "/usr/bin/env";
        argv[1] = NULL;

        execvp(cmd, argv);
    }else{

        waitpid(-1, &status, 0);

        printf("\n");

        char *shell_path = getenv("SHELL_PATH");

        if(shell_path){
            printf("SHELL_PATH value is %s\n", shell_path);
        }else{
            printf("SHELL_PATH value is UNKNOWN\n");
        }
    }

    return 0;
}