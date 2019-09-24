#include <Windows.h>
#define _CRT_SECURE_NO_WARNINGS
#include<locale>
#include<string>
using namespace std;
std::wstring toWstring(std::string str)
{
	using namespace std;
	wstring wstr;
	locale loc = locale();
	for (auto const& c : str)
	{
		wchar_t wchar = std::use_facet<ctype<wchar_t>>(loc).widen(c);
		wstr.push_back(wchar);
	}

	return wstr;
}

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


// interface for our driver
class KeInterface
{
public:
	HANDLE hDriver; // Handle to driver

					// Initializer
	KeInterface(LPCSTR RegistryPath)
	{
		hDriver = CreateFileA(RegistryPath, GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);
	}

	template <typename type>
	type ReadVirtualMemory(ULONG64 ProcessId, ULONG64 ReadAddress,
		SIZE_T Size)
	{
		if (hDriver == INVALID_HANDLE_VALUE)
			return (type)false;

		DWORD Return, Bytes;
		KERNEL_READ_REQUEST ReadRequest;

		ReadRequest.ProcessId = ProcessId;
		ReadRequest.Address = ReadAddress;
		ReadRequest.Size = Size;

		// send code to our driver with the arguments
		if (DeviceIoControl(hDriver, IO_READ_REQUEST, &ReadRequest,
			sizeof(ReadRequest), &ReadRequest, sizeof(ReadRequest), 0, 0))
			return (type)ReadRequest.Response;
		else
			return (type)false;
	}

	bool setModule(string *target, int size)
	{
		DWORD Bytes;
		KERNEL_SET temp;
		wstring temp1 = toWstring(*target);
		wcscpy(temp.target, temp1.c_str());
		temp.size = size;
		printf("%ws\n",temp.target);
		return(DeviceIoControl(hDriver, IO_GET_MODULE_SET, &temp, sizeof(temp),
			0, 0, &Bytes, NULL));
	}

	bool WriteVirtualMemory(ULONG64 ProcessId, ULONG64 WriteAddress,
		ULONG WriteValue, SIZE_T WriteSize)
	{
		if (hDriver == INVALID_HANDLE_VALUE)
			return false;
		DWORD Bytes;

		KERNEL_WRITE_REQUEST  WriteRequest;
		WriteRequest.ProcessId = ProcessId;
		WriteRequest.Address = WriteAddress;
		WriteRequest.Value = WriteValue;
		WriteRequest.Size = WriteSize;

		if (DeviceIoControl(hDriver, IO_WRITE_REQUEST, &WriteRequest, sizeof(WriteRequest),
			0, 0, &Bytes, NULL))
			return true;
		else
			return false;
	}

	bool WriteVirtualMemory64(ULONG64 ProcessId, ULONG64 WriteAddress,
		ULONG64 WriteValue, SIZE_T WriteSize)
	{
		if (hDriver == INVALID_HANDLE_VALUE)
			return false;
		DWORD Bytes;

		KERNEL_WRITE_REQUEST64  WriteRequest;
		WriteRequest.ProcessId = ProcessId;
		WriteRequest.Address = WriteAddress;
		WriteRequest.Value = WriteValue;
		WriteRequest.Size = WriteSize;

		if (DeviceIoControl(hDriver, IO_WRITE_REQUEST64, &WriteRequest, sizeof(WriteRequest),
			0, 0, &Bytes, NULL))
			return true;
		else
			return false;
	}

	DWORD64 GetTargetPid()
	{
		if (hDriver == INVALID_HANDLE_VALUE)
			return false;

		ULONG64 Id;
		DWORD Bytes;

		if (DeviceIoControl(hDriver, IO_GET_ID_REQUEST, &Id, sizeof(Id),
			&Id, sizeof(Id), &Bytes, NULL))
			return Id;
		else
			return false;
	}

	DWORD64 GetClientModule()
	{
		if (hDriver == INVALID_HANDLE_VALUE)
			return false;

		ULONG64 Address;
		DWORD Bytes;

		if (DeviceIoControl(hDriver, IO_GET_MODULE_REQUEST, &Address, sizeof(Address),
			&Address, sizeof(Address), &Bytes, NULL))
			return Address;
		else
			return false;
	}
};
