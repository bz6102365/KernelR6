# KernelR6
Pasted from Zer0Mem0ry(https://github.com/Zer0Mem0ry/KernelBhop) and make it work with X64. Include simple R6 cheat as demo.
Sign driver and disable secure boot to use it or make it compatible with a driver mapper(it must be detected with mapper, you should modify it a lot).

Methods with KeInterface:

GetTargetPid()
Get pid of the Image you want to RPM/WPM with. Return DWORD64.

GetClientModule()
Get target process' base address. Return 16-bytes address instead of 8-bytes address in Zer0Mem0ry's driver for X64. You can simply convert it to DWORD make it compatible with X86.

ReadVirtualMemory<type>(ULONG64 ProcessId, ULONG64 ReadAddress, SIZE_T Size)
Read max to 16 bytes from ReadAddress, and convert to type.

WriteVirtualMemory(ULONG64 ProcessId, ULONG64 WriteAddress, ULONG WriteValue, SIZE_T WriteSize)
Write max to 8 bytes to WriteAddress.

WriteVirtualMemory64(ULONG64 ProcessId, ULONG64 WriteAddress, ULONG64 WriteValue, SIZE_T WriteSize)
Write max to 16 bytes to WriteAddress. If you want to write float or something others to memory, use reinterpret_cast or just use it as what i do in Ring3console.

setModule(string *target, int size)
Set new target image name to driver. You can sign driver once and make it work with most of games.

If you have any question, contact me with bz6102365@live.com
