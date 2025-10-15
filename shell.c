#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include "builtins.h"

#define BUFFER_SIZE 1024

int handle_builtin(char** argv);
char** parse_input(char* buffer);

int main() {

    char buffer[BUFFER_SIZE];
    char* argv[10] = {NULL};
    int i;

    while (1) {

        // Prints current directory and flushes the output
        printf("\033[1;34m%s\033[0m mysh> ", getcwd(NULL, 0));
        fflush(stdout);

        // Gets input and checks for error, also removes newline
        if(fgets(buffer, BUFFER_SIZE, stdin) == NULL) 
            break;
        buffer[strcspn(buffer, "\n")] = 0;


        i = 0;
        char *buffer_ptr = buffer;
        while (*buffer_ptr && i < 9) {
            // Skips anys spaces
            while (*buffer_ptr == ' ')
                buffer_ptr++;
            // If we have reached the end 
            if (*buffer_ptr == '\0')
                break;

            // Goes through process of having an argument with ""
            if (*buffer_ptr == '"') {
                buffer_ptr++;
                argv[i] = buffer_ptr;

                while (*buffer_ptr != '"' && *buffer_ptr != '\0')
                    buffer_ptr++;
                
                if (*buffer_ptr == '"') {
                    *buffer_ptr = '\0';
                    buffer_ptr++;
                }
                
            }
            // Default case without a "" surrounding something
            else {
                argv[i] = buffer_ptr;

                while (*buffer_ptr != ' ' && *buffer_ptr != '\0')
                    buffer_ptr++;
                
                if (*buffer_ptr == ' ') {
                    *buffer_ptr = '\0';
                    buffer_ptr++;
                }
            }

            i++;
        }
        argv[i] = NULL;


        // Makes sure there is input
        if (argv[0] == NULL) {
            continue;
        }

        // Built in command (1 means exit, 0 means contiue, -1 means error, -2 not a builtin)
        int code = handle_builtin(argv);
        if (code == 1) {
            break;
        }
        else if (code == 0) {
            continue;
        }
        else if (code == -1) {
            continue;
        }

        
        // Forks the command into another process and then this program waits until it is finished to continue
        /*
        pid_t pid = fork();
        if (pid == 0) {
            execvp(argv[0], argv);
            perror("execvp");
            exit(EXIT_FAILURE);
        }
        else if (pid > 0) {
            int status;
            waitpid(pid, &status, 0);
        }
        else {
            perror("fork");
            continue;
        }*/
    }


    return 0;
}

// Handles the operation of the builtin shell functions declared in builtins.h
// Param: The argument array from the console
// Returns: -2 for not a command, -1 for error, 0 for worked perfect, 1 for exit command line
int handle_builtin(char** argv) {
    // exit command
    if (strcmp(argv[0], "exit") == 0) {
        return builtin_exit();
    }

    // cd command
    if (strcmp(argv[0], "cd") == 0) {
        return builtin_cd(argv);
    }

    // ls command
    if (strcmp(argv[0], "ls") == 0) {
        return builtin_ls(argv);
    }

    return -2;
}

char** parse_input(char* buffer) {

}