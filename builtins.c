#define _GNU_SOURCE // Fixes vscode error

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h> // Use for struct stat and prining ls -l
#include "builtins.h"

int builtin_exit() {
    return 1; // 1 means that you must exit the function
}

int builtin_pwd() {
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("pwd");
        return -1;
    }

    printf("%s\n", cwd);
    return 0;
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

// TODO
// prints permissinos for a file/ directory
void print_permissions() {

}

void print_filename(struct dirent* dir) {
    if (dir->d_type == DT_DIR) {
        printf("\033[1;34m%s\033[0m\n", dir->d_name);
    }
    else {
        printf("%s\n", dir->d_name);
    }
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

    // Different printing options depedning on the option
    if (option == NULL) {
        while ((dir = readdir(dir_stream)) != NULL) {
            if (dir->d_name[0] == '.')
                continue;

            print_filename(dir);
        }
    }
    else if (strcmp(option, "-a") == 0) { // Displays all the files
        while ((dir = readdir(dir_stream)) != NULL) {
            print_filename(dir);
        }
    }
    else if (strcmp(option, "-l") == 0) { // Displays long version of the file
        while ((dir = readdir(dir_stream)) != NULL) {
            // 1. print_permissions();
            // 2. print hard links
            // 3. print owner of the file
            // 4. print group that owns the file
            // 5. print file size in bytes
            // 6. print when file was lost modified (Jan 1 06:00)
            // 7. print name of the file
            print_filename(dir);
        }
    }


    closedir(dir_stream);

    return 0;
}