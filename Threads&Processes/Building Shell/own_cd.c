#include "shell.h"

/**
 * own_cd - changes the working dir of the current shell executon env
 * @args: target directory
 *
 * Return: -1 one success, 0 otherwise.
 */
int own_cd(char **args)
{
    // TODO
    if (args[1] == NULL) {
        fprintf(stderr, "cd: no argument\n");
    }
    else {
        if (chdir(args[1])) {
            perror("ann error occurred in own_cd");
        }
        else {
            char *changeWorkDirectory;
            if (getcwd(changeWorkDirectory, sizeof(char)*1500) != NULL) {
                fprintf(stderr, "Now we are in %s \n", changeWorkDirectory);
            }
        }
    }
    return (-1);
}