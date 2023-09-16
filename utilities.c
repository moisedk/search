/* utilities.c */

#include "search.h"

#include <errno.h>
#include <string.h>
#include <time.h>

#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

/**
 * Test whether or not a directory is empty.
 * @param   path        Path to directory.
 * @return  Whether or not a directory is empty.
 */
bool is_directory_empty(const char *path) {

    DIR *dir;
    struct dirent *entry;

    if ((dir = opendir(path) == NULL)) {
        perror("opendir");
    }
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") !=  0 && strcmp(entry->d_name, "..") != 0) {
            closedir(dir);
            return false; // Directory is not empty
        }
    }
    return true;
}

/**
 * Retrieve the modification time of the given file.
 * @param   path        Path to file of directory.
 * @return  The modification time of the given file.
 */
time_t get_mtime(const char *path) {
    struct stat statbuff;
    lstat(path, &statbuff);
    time_t modified_time = statbuff.st_mtim.tv_sec;
    return modified_time;
}

/* vim: set sts=4 sw=4 ts=8 expandtab ft=c: */
