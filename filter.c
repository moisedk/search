/* filter.c */

#include "search.h"

#include <errno.h>
#include <fcntl.h>
#include <string.h>

#include <dirent.h>
#include <fnmatch.h>
#include <sys/stat.h>
#include <unistd.h>
#include <libgen.h>
#include <fnmatch.h>

/**
 * Test whether or not the given path should be filtered out of the output.
 * @param   path        Path to file or directory.
 * @param   settings    Settings structure.
 * @return  Whether or not the path should be excluded from the output (true to
 * exclude, false to include).
 */
bool filter(const char *path, const Settings *settings)
{
    char *base_name = basename(path);

    bool _type = (is_dir(path) && settings->type == 0) || (!is_dir(path) && settings->type == 1); // path points to dir but file was searched for, or vice versa
    bool _empty = !is_empty(path) && settings->empty;
    bool _name = settings->name && fnmatch(settings->name, base_name, FNM_NOESCAPE) != 0;
    bool _path = settings->path && fnmatch(settings->path, path, FNM_NOESCAPE) != 0;
    bool _perm = settings->perm && ((to_decimal(settings->perm) & 0777) != (get_perm_mode(path) & 0777));
    bool _access = settings->access && get_access(path, settings->access);
    bool _mtime = settings->newer && (settings->newer > get_mtime(path));
    bool _uid = (settings->uid != -1) && (getuid() != settings->uid);
    bool _gid = (settings->uid != -1) && (getgid() != settings->gid);
    return _type || _empty || _name || _path || _perm | _access || _mtime || _uid || _gid;
}