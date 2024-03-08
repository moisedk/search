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
    if (!filter(root, settings))
    {
        // Print the file if requested explicitely, or if no argument is provided for exec();
        if (settings->print || !settings->exec_argv)
        {
            printf("%s\n", root);
        }
        if (settings->exec_argv)
        {
            execute(root, settings);
        }
    }
    if (!is_dir(root))
    {
        return EXIT_SUCCESS;
    }
    DIR *dir;
    struct dirent *entry;
    char path[500];

    dir = opendir(root);
    if (dir == NULL)
    {
        perror("opendir");
        return EXIT_FAILURE;
    }
    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }
        snprintf(path, sizeof(path), "%s/%s", root, entry->d_name);
        search(path, settings);
    }
    return EXIT_SUCCESS;
}

/* vim: set sts=4 sw=4 ts=8 expandtab ft=c: */
