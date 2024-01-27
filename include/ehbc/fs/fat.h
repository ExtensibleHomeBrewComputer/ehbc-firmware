#ifndef _EHBC_FS_FAT_H__
#define _EHBC_FS_FAT_H__

#include <ehbc/types.h>
#include <ehbc/class.h>
#include <ehbc/hw/device.h>

extern const classdef_t _fs_fat_def;

#define FileSystemFAT (&_fs_fat_def)

int methodof(FileSystemFAT, initialize_from_blkdev)(class_t* self, class_t* blkdev);

const char* methodof(FileSystemFAT, get_filesystem_name)(class_t* self);

int methodof(FileSystemFAT, create_directory)(class_t* self, const char* path);
int methodof(FileSystemFAT, remove_directory)(class_t* self, const char* path);
int methodof(FileSystemFAT, open_directory)(class_t* self, const char* path);
int methodof(FileSystemFAT, close_directory)(class_t* self);
int methodof(FileSystemFAT, list_directory)(class_t* self, const char* path, char** buf, uint32_t count);

int methodof(FileSystemFAT, create_file)(class_t* self, const char* path);
int methodof(FileSystemFAT, remove_file)(class_t* self, const char* path);
int methodof(FileSystemFAT, open_file)(class_t* self, const char* path, const char* mode);
int methodof(FileSystemFAT, close_file)(class_t* self, int fd);
int methodof(FileSystemFAT, read_file)(class_t* self, int fd);
int methodof(FileSystemFAT, write_file)(class_t* self, int fd);
int methodof(FileSystemFAT, seek_file)(class_t* self, int fd);

int methodof(FileSystemFAT, rename)(class_t* self, const char* oldname, const char* newname);
int methodof(FileSystemFAT, copy)(class_t* self, const char* orig, const char* tgt);

#endif  // _EHBC_FS_FAT_H__
