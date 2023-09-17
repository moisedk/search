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
bool is_directory_empty(const char *path)
{

    DIR *dir;
    struct dirent *entry;

    if ((dir = opendir(path)) == NULL)
    {
        perror("opendir");
        return false;
    }
    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {
            closedir(dir);
            return false; // Directory is not empty
        }
    }
    closedir(dir);
    return true;
}

/**
 * Retrieve the modification time of the given file.
 * @param   path        Path to file of directory.
 * @return  The modification time of the given file.
 */
time_t get_mtime(const char *path)
{
    struct stat statbuff;
    lstat(path, &statbuff);
    time_t modified_time = statbuff.st_mtim.tv_sec;
    return modified_time;
}
/**
 * @brief Test if path is pointing to an empty file. Assumes path points to a file, not a directory
 *
 * @param path path to the file
 * @return true if path is empty
 * @return false otherwise
 */
bool is_file_empty(const char *path)
{
    FILE *file = fopen(path, "r");
    if (file == NULL)
    {
        perror("fopen");
        return -1;
    }
    fseek(file, 0, SEEK_END);     // Set the file position pointer to the end of the file then calculate the size of the file;
    long file_size = ftell(file); // Get the size of the file
    fclose(file);
    if (file_size == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
/**
Test whether or not a path is empty using is_directory_empty() and is_file_empty()
* @param path Path to the file or directory
* @return true if path is pointing to an empty file or an empty directory, false otherwise;
*/
bool is_path_empty(const char *path)
{
    // Check if path is pointing to a regular file or a directory. If neither, just pretend it's not empty
    if (is_dir(path))
    {
        return is_directory_empty(path);
    }

    return is_file_empty(path);
}
bool is_dir(const char *path)
{
    struct stat path_stat;
    if (stat(path, &path_stat) != 0)
    {
        perror("stat");
        return 1;
    }
    if (S_ISDIR(path_stat.st_mode))
    {
        return true;
    }
    return false;
}
/* vim: set sts=4 sw=4 ts=8 expandtab ft=c: */
