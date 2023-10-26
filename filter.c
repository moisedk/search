/* filter.c */

#include "search.h"

#include <errno.h>
#include <fcntl.h>
#include <string.h>

#include <dirent.h>
#include <fnmatch.h>
#include <sys/stat.h>
#include <unistd.h>

/**
 * Test whether or not the given path should be filtered out of the output.
 * @param   path        Path to file or directory.
 * @param   settings    Settings structure.
 * @return  Whether or not the path should be excluded from the output (true to
 * exclude, false to include).
 */
bool filter(const char *path, const Settings *settings)
{
    // Filtering the file type
    bool type= (is_dir(path) && settings->type == 0) || (!is_dir(path) && settings->type == 1); // path points to dir but file was searched for, or vice versa
    bool empty = ((!is_dir(path) && is_file_empty(path)) || (is_dir(path) && is_path_empty(path))) && settings->empty == true;
    if (type || !empty)
    {
        return true;
    }
    return false;
}

/* vim: set sts=4 sw=4 ts=8 expandtab ft=c: */
