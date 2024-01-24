#ifndef _EHBC_HW_DEVICE_H__
#define _EHBC_HW_DEVICE_H__

#include <ehbc/types.h>
#include <ehbc/hw/bus.h>
#include <ehbc/class.h>

extern classdef_t class_device_def;

#define Device (&class_device_def)

#endif  // _EHBC_HW_DEVICE_H__
