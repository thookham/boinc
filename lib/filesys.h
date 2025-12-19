// This file is part of BOINC.
// http://boinc.berkeley.edu
// Copyright (C) 2025 University of California
//
// BOINC is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.

#ifndef BOINC_FILESYS_H
#define BOINC_FILESYS_H

#include "boinc_stdio.h"
#include <string>

#ifdef _WIN32
#include "boinc_win.h"
#endif

#ifndef MAXPATHLEN
#define MAXPATHLEN 4096
#endif

#define DIR_LEN 2048
#define FILE_LEN 256
#define FILE_RETRY_INTERVAL 5

#ifdef __cplusplus
extern "C" {
#endif
    extern int boinc_delete_file(const char*);
    extern int boinc_touch_file(const char *path);
    extern FILE* boinc_fopen(const char* path, const char* mode);
    extern int boinc_copy(const char* orig, const char* newf);
    extern int boinc_rename(const char* old, const char* newf);
    extern int boinc_mkdir(const char*);
    extern int boinc_rmdir(const char*);
    extern void boinc_getcwd(char*);
    extern void relative_to_absolute(const char* relname, char* path);
    extern int boinc_make_dirs(const char*, const char*);
    extern char boinc_failed_file[MAXPATHLEN];
    extern int is_file(const char* path);
    extern int is_dir(const char* path);
    extern int is_file_follow_symlinks(const char* path);
    extern int is_dir_follow_symlinks(const char* path);
    extern int is_symlink(const char* path);
    extern int boinc_truncate(const char*, double);
    extern int boinc_file_exists(const char* path);
    extern int boinc_file_or_symlink_exists(const char* path);
#ifdef _WIN32
    extern FILE* boinc_temp_file(const char* dir, const char* prefix, char* temp_path, double size);
    extern int boinc_allocate_file(const char* path, double size);
#else
    extern FILE* boinc_temp_file(const char* dir, const char* prefix, char* temp_path);
#endif
    extern void boinc_path_to_dir(const char* path, char* dir);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

#if defined(__GNUC__) && (__GNUC__ < 8) && !defined(__clang__)
#include <experimental/filesystem>
namespace std { namespace filesystem = experimental::filesystem; }
#else
#include <filesystem>
#endif

extern int file_size(const char*, double&);
extern int file_size_alloc(const char*, double&);
extern int dir_size(const char* dirpath, double&, bool recurse=true);
extern int dir_size_alloc(const char* dirpath, double&, bool recurse=true);
extern int clean_out_dir(const char*);
extern int get_filesystem_info(double& total, double& free, char* path=const_cast<char *>("."));
extern bool is_path_absolute(const std::string path);

extern int read_file_malloc(const char* path, char*& result, size_t max_len=0, bool tail=false);
extern int read_file_string(const char* path, std::string& result, size_t max_len=0, bool tail=false);

// Modernized DIRREF as opaque pointer (implemented in cpp)
typedef void* DIRREF;

extern DIRREF dir_open(const char*);
extern int dir_scan(char*, DIRREF, int);
extern int dir_scan(std::string&, DIRREF);
extern void dir_close(DIRREF);

extern bool is_dir_empty(const char*);

class DirScanner {
    void* handle; // Opaque pointer to DirScannerImpl
public:
    DirScanner(std::string const& path);
    ~DirScanner();
    bool scan(std::string& name);
};

struct FILE_LOCK {
#ifdef _WIN32
    HANDLE handle;
#else
    int fd;
#endif
    bool locked;
    FILE_LOCK();
    ~FILE_LOCK();
    int lock(const char* filename);
    int unlock(const char* filename);
};

#endif // c++

#endif // BOINC_FILESYS_H
