/* utilities.c */

#include "search.h"

#include <errno.h>
#include <string.h>
#include <time.h>

#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>
#include <math.h>
#include <sys/types.h>
/**
 * @brief Test whether or not a directory is empty.
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
    time_t modified_time = statbuff.st_mtime;
    return modified_time;
}
/**
 * @brief Get the permission bits of file
 * 
 * @param file char string for the filename
 * @return mode_t st_mode, whose 9 LSB are the permission bit.
 */
mode_t get_perm_mode(const char *file) {
    struct stat statbuff;
    lstat(file, &statbuff);
    return statbuff.st_mode;
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
        return 1;
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
 * @brief Test whether or not a path is empty using is_directory_empty() and is_file_empty()
 *
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

/**
 * @brief Test if a path is a directory
 *
 * @param path character string path
 * @return true if path points to a directory
 * @return false otherwise
 */
bool is_dir(const char *path)
{
    struct stat path_stat;
    if (stat(path, &path_stat) != 0)
    {
        perror("stat");
        return 1;
    }
    return S_ISDIR(path_stat.st_mode);
}
bool is_empty(const char *path) {
    return (!is_dir(path) && is_file_empty(path)) || (is_dir(path) && is_path_empty(path));
}
/**
 * @brief Return true if the string is a valid numberical string; false otherwise
 * 
 * @param str string expression to parse
 * @return true if str is numberical
 * @return false ottherwise
 */
bool is_numeric(char *str) {
    int i;
    for (i = 0; str[i] != '\0'; i++) {
        if (!isdigit(str[i])) {
            return false;
        }
    }
    return i < 4; // The fourth character (i == 3) must be the null character;
}

int to_decimal(char  *oct) {
    int len = strlen(oct), base = 8, dec = 0;
    for (int i = 0; i < len; i++) {
        int digit = oct[i] - '0';
        dec += digit * pow(base, len - i - 1);
    }
    return dec;
}
int get_access(const char *path, int flags) {
    return access(path, flags);
}
/* vim: set sts=4 sw=4 ts=8 expandtab ft=c: */
