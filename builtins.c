#define _GNU_SOURCE // Fixes vscode error

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include "builtins.h"

int builtin_exit() {
    return 1; // 1 means that you must exit the function
}

int builtin_cd(char** argv) {
    // If there is no command move to home directory
    char *dir = argv[1];
    if (dir == NULL) {
        dir = getenv("HOME");
    }


    // Checks if there is an error and also runs the chdir command
    if (chdir(dir) != 0) {
        perror("cd");
        return -1;
    }     

    return 0;
}

int builtin_ls(char** argv) {
    DIR* dir_stream; // directory stream
    struct dirent* dir; // directory entry
    char* target_dir = ".";
    char* option = NULL;

    // Checks to see if there are arguments
    if (argv[1] != NULL) {

        // Checks to see if there is an option first or if it's only directory argument
        if (argv[1][0] == '-') {
            option = argv[1];

            if (argv[2] != NULL) {
                target_dir = argv[2];
            }
        }
        else {
            target_dir = argv[1];
        }
    }

    dir_stream = opendir(target_dir);

    // Checks if there is an error in opening the directory
    if (dir_stream == NULL) {
        perror("ls");
        return -1;
    }

    // Iterates through the stream printing out the directories
    while ((dir = readdir(dir_stream)) != NULL) {

        // If statements for all the possible options
        if (option == NULL) {
            if (dir->d_name[0] == '.')
                continue;
            
            // Prints out specific colors depening on their types
            if (dir->d_type == DT_DIR) {
                printf("\033[1;34m%s/\033[0m\n", dir->d_name);
            }
            else {
                printf("%s\n", dir->d_name);
            }
        }
        else if (strcmp(option, "-a") == 0) {
            printf("%s\n", dir->d_name);
        }
        
    }

    closedir(dir_stream);

    return 0;
}