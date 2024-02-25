/* search.c */

#include "search.h"

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <dirent.h>

/**
 * Recursively search through the provided root directory
 * @param   root        Path to directory
 * @param   settings    Settings structure
 * @return  Whether or not the search was successful.
 */
int search(const char *root, const Settings *settings)
{
    DIR *dir;
    struct dirent *entry;
    char path[500];

    dir = opendir(root);
    if (dir == NULL)
    {
        perror(" seach opendir");
        return EXIT_FAILURE;
    }
    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }
        snprintf(path, sizeof(path), "%s/%s", root, entry->d_name);
        // printf("%s\n", path);
        if (!filter(path, settings))
        {
            // Print the file if requested explicitely, or if no argument is provided for exec();
            if (settings->print || !settings->exec_argc)
            {
                printf("%s\n", path);
            }
            if (settings->exec_argc)
            {
                execute(path, settings);
            }
        }
        if (is_dir(path))
        {
            search(path, settings);
        }
    }
    return EXIT_SUCCESS;
}

/* vim: set sts=4 sw=4 ts=8 expandtab ft=c: */
