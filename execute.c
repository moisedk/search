/* expression.c */

#include "search.h"

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <dirent.h>
#include <sys/wait.h>
#include <unistd.h>

/**
 * Executes the -print or -exec expressions (or both) on the specified path.
 * @param   path        Path to a file or directory
 * @param   settings    Settings structure.
 * @return  Whether or not the execution was successful.
 */
int execute(const char *path, const Settings *settings)
{
    int rc = fork();
    if (rc < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (rc == 0) {
        char *exec_cmd = strdup(settings->exec_argv[0]);
        char *exec_args[50]; // Array of pointers to null-terminated strings for execvp()
        int i;
        for (i = 0; i < settings->exec_argc; ++i) {
            exec_args[i] = strdup(settings->exec_argv[i]);
        }
        exec_args[i++] = strdup(path);
        exec_args[i] = NULL;
        execvp(exec_cmd, exec_args);
        perror("exevp");
        exit(EXIT_FAILURE);
    }
    else {
        int status;
        wait(&status);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            printf("Done\n");
        }
    }
    return 0;
}

/* vim: set sts=4 sw=4 ts=8 expandtab ft=c: */
