#pragma once
#include <Windows.h>
/* IOCTL Codes needed for our driver */

// Request to read virtual user memory (memory of a program) from kernel space
#define IO_READ_REQUEST CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0701 /* Our Custom Code */, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

// Request to write virtual user memory (memory of a program) from kernel space
#define IO_WRITE_REQUEST CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0702 /* Our Custom Code */, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

// Request to retrieve the process id of csgo process, from kernel space
#define IO_GET_ID_REQUEST CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0703 /* Our Custom Code */, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

// Request to retrieve the base address of client.dll in csgo.exe from kernel space
#define IO_GET_MODULE_REQUEST CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0704 /* Our Custom Code */, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

// Set new process name
#define IO_GET_MODULE_SET CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0705/* Our Custom Code */, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

#define IO_WRITE_REQUEST64 CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0706 /* Our Custom Code */, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

// Instantly get PID of target process using enumerating all process.
#define IO_INSTA_GET_PID CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0707 /* Our Custom Code */, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

// Instantly get baseaddr of target process.
#define IO_INSTA_GET_BASEADDR CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0708 /* Our Custom Code */, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)