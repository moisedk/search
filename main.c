/* driver.c */

#include "search.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/stat.h>
#include <unistd.h>

/* Functions */

void usage(const char *program_name, int status)
{
    fprintf(stderr, "Usage: %s PATH [OPTIONS] [EXPRESSION]\n", program_name);
    fprintf(stderr, "\nOptions:\n");
    fprintf(stderr, "    -executable     File is executable or directory is searchable to user\n");
    fprintf(stderr, "    -readable       File readable to user\n");
    fprintf(stderr, "    -writable       File is writable to user\n\n");
    fprintf(stderr, "    -type [f|d]     File is of type f for regular file or d for directory\n\n");
    fprintf(stderr, "    -empty          File or directory is empty\n\n");
    fprintf(stderr, "    -name  pattern  Base of file name matches shell pattern\n");
    fprintf(stderr, "    -path  pattern  Path of file matches shell pattern\n\n");
    fprintf(stderr, "    -perm  mode     File's permission bits are exactly mode (octal)\n");
    fprintf(stderr, "    -newer file     File was modified more recently than file\n\n");
    fprintf(stderr, "    -uid   n        File's numeric user ID is n\n");
    fprintf(stderr, "    -gid   n        File's numeric group ID is n\n");
    fprintf(stderr, "\nExpressions:\n\n");
    fprintf(stderr, "    -print          Display file path (default)\n");
    fprintf(stderr, "    -exec cmd {} ;  Execute command on path\n");
    exit(status);
}

/* Main Execution */

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        usage("search", 1);
    }
    // Most filtering setting is set to 0 or NULL by default to mean unacccounted for; 
    Settings settings = {
        .access = 0,
        .empty = false,
        .path = NULL,
        .name = NULL,
        .gid = -1,
        .uid = -1,
        .type = -1,
        .perm = NULL,
        .newer = 0,
        .gid = -1,
        .exec_argc = 0,
        .exec_argv = NULL,
        };
    // Parse the argument values
    char ** execargs;
    for (int i = 1; i < argc; i++)
    
    {
        // Access field specifiert
        if (streq(argv[i], "-executable"))
        {
            settings.access |= X_OK;
        }
        else if (streq(argv[i], "-readable"))
        {
            settings.access |= R_OK;
        }
        else if (streq(argv[i], "-writable"))
        {
            settings.access |= W_OK;
        }
        // File type specifier
        if (streq(argv[i], "-type"))
        {
            if (++i >= argc)
            {
                usage("search", 1);
            }
            if (streq(argv[i], "f"))
            {
                settings.type = 0; // Set the file type filter to regular file
            }
            else if (streq(argv[i], "d"))
            {
                settings.type = 1; // Set the file type filter to directory
            }
            else
            {
                usage("search", 1);
            }
            continue;
        }
        // Empty file specifier
        if (streq(argv[i], "-empty"))
        {
            settings.empty = 1;
        }
        // File name pattern specifier
        if (streq(argv[i], "-name"))
        {
            if (++i >= argc)
            {
                usage("search", 1);
            }
            char *name = strdup(argv[i]);
            if (name == NULL) {
                perror("strdup");
                return FAILURE;
            }
            settings.name = name;
            continue;
        }
        // File path pattern specifier
        if (streq(argv[i], "-path"))
        {
            if (++i >= argc)
            {
                usage("search", 1);
            }
            char* path = strdup(argv[i]);
            if (path == NULL) {
                perror("strdup");
                return FAILURE;
            }
            settings.path = path;
            continue;
        }
        // File permission mode specification
        if (streq(argv[i], "-perm"))
        {
            if (++i >= argc)
            {
                usage("search", 1);
            }
            char *perm = strdup(argv[i]);
            if (perm == NULL) {
                perror("strdup");
                return FAILURE;
            }
            if (is_numeric(perm)) { 
                settings.perm = perm;
            }
            else {
                fprintf(stderr, "Invalid permission value: aborting...");
                return ARG_ERR;
            }
        }
        if (streq(argv[i], "-newer")) {
            if (++i >= argc) {
                usage("search", 1);
            }
            char *oldfile = strdup(argv[i]);
            if (oldfile == NULL) {
                perror("strdup");
                return FAILURE;
            }
            settings.newer = get_mtime(oldfile);
        }
        if (streq(argv[i], "-uid")) {
            if (++i >= argc) {
                usage("search", 1);
            }
            char *userid = strdup(argv[i]);
            if (userid == NULL) {
                perror("strdup");
                return FAILURE;
            }
            if (is_numeric(userid)) {
                settings.uid = atoi(userid);
            }
            else {
                fprintf(stderr, "Invalid user id: aborting...");
                return ARG_ERR;
            }
        }
         if (streq(argv[i], "-gid")) {
            if (++i >= argc) {
                usage("search", 1);
            }
            char *groupid = strdup(argv[i]);
            if (groupid == NULL) {
                perror("strdup");
                return FAILURE;
            }
            if (is_numeric(groupid)) {
                settings.gid = atoi(groupid);
            }
            else {
                fprintf(stderr, "Invalid group id: aborting...");
                return ARG_ERR;
            }
        }
        if (streq(argv[i], "-print")) {
            settings.print = true;
            continue;
        }
        if (streq(argv[i], "-exec")) {
            ++i;
            int argnum = 0; // Get the number of arguments specifically passed to this program, as in "stat /home"
            while (i + argnum < argc && !streq(argv[i + argnum], "{}")) {
                ++argnum;
            }
            if (argnum == 0) {
                printf("Missing argument for exec: aborting...");
                return ARG_ERR;
            }
                // Dynamically allocate memory for execargs
            execargs = (char** )malloc((argnum + 1) * sizeof(char*));
            if (execargs == NULL) {
                perror("Failed to allocate memory for execargs");
                return ARG_ERR;
            }
            int p = 0; // To iterate from 1 to the end of the argument list ({})
            while (i < argc && !streq(argv[i], "{}")) {
                execargs[p] = argv[i];
                ++i;
                ++p;
            }
            execargs[argnum] = NULL;
            settings.exec_argc = argnum;
            settings.exec_argv = execargs;
            continue;
        }
    }

    search(argv[1], &settings);
    free(execargs);
    return SUCCESS;
}

/* vim: set sts=4 sw=4 ts=8 expandtab ft=c: */
