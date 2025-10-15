/* Represents the headers for my built-in shell functions */

// Only returns 1 when called
int builtin_exit();

// Prints the current directory
int builtin_pwd();

// Returns 0 meaning continue, returns -1 means error
int builtin_cd(char** argv);

// Lists current directories
int builtin_ls(char** argv);

