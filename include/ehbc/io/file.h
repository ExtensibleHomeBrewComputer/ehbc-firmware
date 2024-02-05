#ifndef _EHBC_IO_FILE_H__
#define _EHBC_IO_FILE_H__

#include <ehbc/types.h>
#include <ehbc/class.h>
#include <ehbc/hw/device.h>

extern const classdef_t _file_def;

#define File (&_file_def)

void memberof(File, open)(class_t* self);
int32_t memberof(File, seek)(class_t* self, int32_t offs, int whence);
ssize32_t memberof(File, read)(class_t* self, void* buf, size32_t count);
ssize32_t memberof(File, write)(class_t* self, const void* buf, size32_t count);
int memberof(File, flush)(class_t* self);

#endif  // _EHBC_IO_FILE_H__
