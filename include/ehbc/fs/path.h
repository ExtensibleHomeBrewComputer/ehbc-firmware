#ifndef _EHBC_FS_PATH_H__
#define _EHBC_FS_PATH_H__

#include <ehbc/types.h>


/*
 * Path format:
 * Absolute: $bus$drive$partition:/dir/(...)/file_or_dir
 * Relative: ../../dir/(...)/file_or_dir
 *
 * Examples:
 * - $ata0$master$0:/foo/bar.baz
 * - ../
 *
 */

static inline int path_get_file_name(const char* path, char* buf, size32_t len);
static inline int path_get_file_extension(const char* path, char* buf, size32_t len);
static inline int path_get_file_name_only(const char* path, char* buf, size32_t len);
static inline int path_get_device_name(const char* path, char* buf, size32_t len);
static inline int path_get_directory_name(const char* path, char* buf, size32_t len);
static inline int path_get_next_directory_hierarchy(const char* path, char* buf, size32_t len, size32_t* next);

#endif  // _EHBC_FS_PATH_H__
