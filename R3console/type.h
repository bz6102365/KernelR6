#pragma once
#include<Windows.h>
typedef struct _KERNEL_READ_REQUEST
{
	ULONG64 ProcessId;

	ULONG64 Address;
	ULONG64 Response;
	ULONG64 Size;

} KERNEL_READ_REQUEST, * PKERNEL_READ_REQUEST;

typedef struct _KERNEL_WRITE_REQUEST
{
	ULONG64 ProcessId;

	ULONG64 Address;
	ULONG Value;
	ULONG64 Size;

} KERNEL_WRITE_REQUEST, * PKERNEL_WRITE_REQUEST;

typedef struct _KERNEL_SET
{
	int size;
	wchar_t target[256];
} KERNEL_SET, * PKERNEL_SET;


typedef struct _KERNEL_WRITE_REQUEST64
{
	ULONG64 ProcessId;

	ULONG64 Address;
	ULONG64 Value;
	ULONG64 Size;

} KERNEL_WRITE_REQUEST64, * PKERNEL_WRITE_REQUEST64;


typedef struct _KERNEL_INSTA_GET_PID
{
	int size;
	wchar_t target[256];
	HANDLE PID;
} KERNEL_INSTA_GET_PID, * PKERNEL_INSTA_GET_PID;

typedef struct _KERNEL_INSTA_GET_BASEADDR
{
	HANDLE PID;
	ULONG64 baseaddr;
} KERNEL_INSTA_GET_BASEADDR, * PKERNEL_INSTA_GET_BASEADDR;