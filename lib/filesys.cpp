// This file is part of BOINC.
// http://boinc.berkeley.edu
// Copyright (C) 2025 University of California
//
// BOINC is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.

#ifndef _WIN32
#include "cpp.h"
#endif

#include "filesys.h"
#include "error_numbers.h"
#include "str_replace.h"
#include "util.h"

#include <filesystem>
#include <fstream>
#include <system_error>
#include <thread>
#include <chrono>

#ifdef _WIN32
#include "boinc_win.h"
#else
#include <fcntl.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

namespace fs = std::filesystem;

char boinc_failed_file[MAXPATHLEN];

// --- Helper for error code conversion ---
static int fs_error_to_boinc(const std::error_code& ec) {
    if (!ec) return 0;
    // Map common errors or return generic
    return ERR_FOPEN; // Simplified mapping
}

// --- File Existence / Type Checks ---

int is_file(const char* path) {
    std::error_code ec;
    return fs::is_regular_file(path, ec);
}

int is_dir(const char* path) {
    std::error_code ec;
    return fs::is_directory(path, ec);
}

int is_file_follow_symlinks(const char* path) {
    return is_file(path); // fs::is_regular_file follows symlinks by default
}

int is_dir_follow_symlinks(const char* path) {
    return is_dir(path);
}

int is_symlink(const char* path) {
    std::error_code ec;
    return fs::is_symlink(path, ec);
}

// --- Directory Operations ---

int boinc_mkdir(const char* path) {
    std::error_code ec;
    if (fs::exists(path, ec)) return 0;
    if (fs::create_directory(path, ec)) return 0;
    return ERR_MKDIR;
}

int boinc_rmdir(const char* name) {
    std::error_code ec;
    if (fs::remove(name, ec)) return 0;
    return ERR_RMDIR;
}

int boinc_make_dirs(const char* dirpath, const char* filepath) {
    fs::path p(filepath);
    fs::path d(dirpath);
    fs::path full = d / p;
    
    std::error_code ec;
    fs::create_directories(full.parent_path(), ec);
    return ec ? ERR_MKDIR : 0;
}

void boinc_getcwd(char* path) {
    try {
        std::string s = fs::current_path().string();
        safe_strcpy(path, s.c_str());
    } catch (...) {
        path[0] = 0;
    }
}

void relative_to_absolute(const char* relname, char* path) {
    try {
        fs::path p = fs::absolute(relname);
        safe_strcpy(path, p.string().c_str());
    } catch (...) {
        safe_strcpy(path, relname);
    }
}

bool is_path_absolute(const std::string path) {
    return fs::path(path).is_absolute();
}

int clean_out_dir(const char* dirpath) {
    std::error_code ec;
    for (const auto& entry : fs::directory_iterator(dirpath, ec)) {
        fs::remove_all(entry.path(), ec);
    }
    return 0;
}

int dir_size(const char* dirpath, double& size, bool recurse) {
    size = 0;
    std::error_code ec;
    if (!fs::exists(dirpath, ec) || !fs::is_directory(dirpath, ec)) return ERR_OPENDIR;

    if (recurse) {
        for (const auto& entry : fs::recursive_directory_iterator(dirpath, ec)) {
            if (fs::is_regular_file(entry.path(), ec)) {
                size += (double)fs::file_size(entry.path(), ec);
            }
        }
    } else {
        for (const auto& entry : fs::directory_iterator(dirpath, ec)) {
             if (fs::is_regular_file(entry.path(), ec)) {
                size += (double)fs::file_size(entry.path(), ec);
            }
        }
    }
    return 0;
}

int dir_size_alloc(const char* dirpath, double& size, bool recurse) {
    return dir_size(dirpath, size, recurse); // Approximate with standard size
}

// --- File Operations ---

int boinc_delete_file(const char* path) {
    std::error_code ec;
    if (!fs::exists(path, ec)) return 0;
    
    // Simple retry loop for Windows-like behavior
    for (int i=0; i<FILE_RETRY_INTERVAL; i++) {
        if (fs::remove(path, ec)) return 0;
        boinc_sleep(1.0);
    }
    safe_strcpy(boinc_failed_file, path);
    return ERR_UNLINK;
}

int boinc_touch_file(const char *path) {
    if (fs::exists(path)) return 0;
    std::ofstream ofs(path);
    return ofs.good() ? 0 : -1;
}

int boinc_copy(const char* orig, const char* newf) {
    std::error_code ec;
    fs::copy(orig, newf, fs::copy_options::overwrite_existing, ec);
    return ec ? ERR_FOPEN : 0;
}

int boinc_rename(const char* old, const char* newf) {
    std::error_code ec;
    for (int i=0; i<FILE_RETRY_INTERVAL; i++) {
        fs::rename(old, newf, ec);
        if (!ec) return 0;
        boinc_sleep(1.0);
    }
    return ERR_RENAME;
}

int file_size(const char* path, double& size) {
    std::error_code ec;
    size = (double)fs::file_size(path, ec);
    return ec ? ERR_STAT : 0;
}

int file_size_alloc(const char* path, double& size) {
    return file_size(path, size);
}

int boinc_truncate(const char* path, double size) {
    std::error_code ec;
    fs::resize_file(path, (std::uintmax_t)size, ec);
    return ec ? ERR_TRUNCATE : 0;
}

// --- READ FILE helpers ---

int read_file_malloc(const char* path, char*& buf, size_t max_len, bool tail) {
    std::ifstream f(path, std::ios::binary | std::ios::ate);
    if (!f) return ERR_FOPEN;

    std::streamsize size = f.tellg();
    if (tail && max_len && size > (std::streamsize)max_len) {
        f.seekg(size - (std::streamsize)max_len);
        size = max_len;
    } else {
        f.seekg(0);
        if (max_len && size > (std::streamsize)max_len) size = max_len;
    }

    buf = (char*)malloc(size + 1);
    if (!buf) return ERR_MALLOC;

    f.read(buf, size);
    buf[size] = 0;
    return 0;
}

int read_file_string(const char* path, std::string& result, size_t max_len, bool tail) {
    char* buf;
    int retval = read_file_malloc(path, buf, max_len, tail);
    if (retval) return retval;
    result = buf;
    free(buf);
    return 0;
}

// --- Legacy Wrappers & Platform Specific ---

FILE* boinc_fopen(const char* path, const char* mode) {
    FILE* f = nullptr;
    // Retry loop for Windows reliability (and partial Unix support)
    int retries = 5;
    while (retries--) {
#ifdef _WIN32
        f = _fsopen(path, mode, _SH_DENYNO);
#else
        f = fopen(path, mode);
#endif
        if (f) break;
        if (errno == ENOENT && strchr(mode, 'r')) break; // Don't retry if reading non-existent
        boinc_sleep(0.2); // generic short sleep
    }
    
#ifndef _WIN32
    if (f) {
        fcntl(fileno(f), F_SETFD, FD_CLOEXEC);
    }
#endif
    return f;
}

// --- DirScanner Implementation (using fs::directory_iterator) ---

struct DirScannerImpl {
    fs::directory_iterator it;
    fs::directory_iterator end;
    
    DirScannerImpl(const std::string& path) {
        std::error_code ec;
        it = fs::directory_iterator(path, ec);
    }
};

DirScanner::DirScanner(std::string const& path) {
    handle = new DirScannerImpl(path); // reusing void* handle
}

DirScanner::~DirScanner() {
    delete (DirScannerImpl*)handle;
}

bool DirScanner::scan(std::string& name) {
    DirScannerImpl* impl = (DirScannerImpl*)handle;
    std::error_code ec;
    
    while (impl->it != impl->end) {
        const auto& entry = *impl->it;
        impl->it.increment(ec); // safe increment
        if (ec) return false;
        
        std::string filename = entry.path().filename().string();
        if (filename.empty() || filename[0] == '.') continue;
        
        name = filename;
        return true;
    }
    return false;
}

// --- Legacy C-style Dir Scan ---

DIRREF dir_open(const char* p) {
    try {
        return new DirScannerImpl(p);
    } catch (...) {
        return nullptr;
    }
}

void dir_close(DIRREF d) {
    delete (DirScannerImpl*)d;
}

int dir_scan(char* p, DIRREF d, int p_len) {
    DirScannerImpl* impl = (DirScannerImpl*)d;
    std::error_code ec;
    
    while (impl->it != impl->end) {
        const auto& entry = *impl->it;
        impl->it.increment(ec);
        if (ec) return ERR_READDIR;

        std::string filename = entry.path().filename().string();
        if (filename.empty() || filename == "." || filename == "..") continue;
        
        strlcpy(p, filename.c_str(), p_len);
        return 0;
    }
    return ERR_NOT_FOUND;
}

int dir_scan(std::string& s, DIRREF d) {
    char buf[MAXPATHLEN];
    int retval = dir_scan(buf, d, sizeof(buf));
    if (!retval) s = buf;
    return retval;
}

bool is_dir_empty(const char* p) {
    std::error_code ec;
    return fs::is_empty(p, ec);
}

// --- FILE_LOCK (Platform Specific) ---

FILE_LOCK::FILE_LOCK() {
    locked = false;
#ifdef _WIN32
    handle = INVALID_HANDLE_VALUE;
#else
    fd = -1;
#endif
}

FILE_LOCK::~FILE_LOCK() {
#ifdef _WIN32
    if (handle != INVALID_HANDLE_VALUE) CloseHandle(handle);
#else
    if (fd >= 0) close(fd);
#endif
}

int FILE_LOCK::lock(const char* filename) {
#ifdef _WIN32
    handle = CreateFileA(filename, GENERIC_WRITE, 0, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    if (handle == INVALID_HANDLE_VALUE) return GetLastError();
#else
    fd = open(filename, O_WRONLY|O_CREAT, 0666);
    if (fd < 0) return ERR_OPEN;
    if (flock(fd, LOCK_EX|LOCK_NB) != 0) return ERR_FCNTL;
#endif
    locked = true;
    return 0;
}

int FILE_LOCK::unlock(const char* filename) {
#ifdef _WIN32
    CloseHandle(handle);
    handle = INVALID_HANDLE_VALUE;
#else
    close(fd);
    fd = -1;
#endif
    locked = false;
    boinc_delete_file(filename);
    return 0;
}

#ifdef _WIN32
// Pre-allocate file space on Windows to reduce fragmentation
int boinc_allocate_file(const char* path, double size) {
    HANDLE hFile = CreateFileA(
        path,
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );
    if (hFile == INVALID_HANDLE_VALUE) return -1;
    
    LARGE_INTEGER li;
    li.QuadPart = (LONGLONG)size;
    if (!SetFilePointerEx(hFile, li, NULL, FILE_BEGIN)) {
        CloseHandle(hFile);
        return -1;
    }
    if (!SetEndOfFile(hFile)) {
        CloseHandle(hFile);
        return -1;
    }
    CloseHandle(hFile);
    return 0;
}
#endif
