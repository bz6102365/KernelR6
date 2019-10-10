#pragma once

#include <iostream>

#include "KeInterface64.h"

#include <stdio.h>

#include "Offsets.h"

#include <windows.h>

using namespace std;

KeInterface Driver("\\\\.\\mydriverver1");
DWORD64 ProcessId = Driver.GetTargetPid();
DWORD64 ClientAddress = Driver.GetClientModule();

void printBaseInfo()
{
	std::cout << "Found R6S Process Id: " << ProcessId << std::endl;
	std::cout << "Found R6S ClientBase: 0x" << std::uppercase
		<< std::hex << ClientAddress << std::endl;
}

void glow(float r, float g, float b, float a)
{
	DWORD64 glowManager = Driver.ReadVirtualMemory<DWORD64>(ProcessId, ClientAddress + OFFSET_GLOW_MANAGER, sizeof(DWORD64));
	DWORD64 glowBase = Driver.ReadVirtualMemory<DWORD64>(ProcessId, glowManager + 0xB8, sizeof(DWORD64));
	//printf("0x%p\n", glowBase);
	Driver.WriteVirtualMemory64(ProcessId, glowBase + 0x110, *(PULONG64)(&r), sizeof(float));
	Driver.WriteVirtualMemory64(ProcessId, glowBase + 0x114, *(PULONG64)(&g), sizeof(float));
	Driver.WriteVirtualMemory64(ProcessId, glowBase + 0x118, *(PULONG64)(&b), sizeof(float));
	Driver.WriteVirtualMemory64(ProcessId, glowBase + 0x130, *(PULONG64)(&a), sizeof(float));
}

WORD max_entities()
{
	DWORD64 gameManager = Driver.ReadVirtualMemory<DWORD64>(ProcessId, ClientAddress + OFFSET_GAME_MANAGER, sizeof(DWORD64));
	return Driver.ReadVirtualMemory<WORD>(ProcessId, gameManager + 0x1C8 + 0x8, sizeof(WORD));
}

DWORD64 entity(DWORD64 index)
{
	DWORD64 gameManager = Driver.ReadVirtualMemory<DWORD64>(ProcessId, ClientAddress + OFFSET_GAME_MANAGER, sizeof(DWORD64));
	DWORD64 entity_list = Driver.ReadVirtualMemory<DWORD64>(ProcessId, gameManager + 0x1C8, sizeof(DWORD64));
	return Driver.ReadVirtualMemory<DWORD64>(ProcessId, entity_list + index * sizeof(std::uintptr_t), sizeof(std::uintptr_t));
}

void enable_marker1(DWORD64 entity)
{
	DWORD64 entity_info = Driver.ReadVirtualMemory<DWORD64>(ProcessId, entity + 0x28, sizeof(DWORD64));
	DWORD64 component_list = Driver.ReadVirtualMemory<DWORD64>(ProcessId, entity_info + 0xD8, sizeof(DWORD64));
	for (DWORD64 current_component = 0x80; current_component < 0xf0; current_component += sizeof(std::uintptr_t))
	{
		DWORD64 component = Driver.ReadVirtualMemory<DWORD64>(ProcessId, component_list + current_component, sizeof(DWORD64));
		if (!component) {
			continue;
		}
		if (Driver.ReadVirtualMemory<DWORD64>(ProcessId, component, sizeof(DWORD64)) != (ClientAddress + VT_MARKER)) {
			continue;
		}
		std::uint8_t tmp = 1;
		Driver.WriteVirtualMemory64(ProcessId, component + 0x532, *(PULONG64)(&tmp), sizeof(std::uint8_t));
		//Driver.WriteVirtualMemory64(ProcessId, component + 0x534, *(PULONG64)(&tmp), sizeof(std::uint8_t));
	}
}

void enable_marker2(DWORD64 entity)
{
	DWORD64 entity_info = Driver.ReadVirtualMemory<DWORD64>(ProcessId, entity + 0x28, sizeof(DWORD64));
	DWORD64 component_list = Driver.ReadVirtualMemory<DWORD64>(ProcessId, entity_info + 0xD8, sizeof(DWORD64));
	for (DWORD64 current_component = 0x80; current_component < 0xf0; current_component += sizeof(std::uintptr_t))
	{
		DWORD64 component = Driver.ReadVirtualMemory<DWORD64>(ProcessId, component_list + current_component, sizeof(DWORD64));
		if (!component) {
			continue;
		}
		if (Driver.ReadVirtualMemory<DWORD64>(ProcessId, component, sizeof(DWORD64)) != (ClientAddress + VT_MARKER)) {
			continue;
		}
		std::uint8_t tmp = 1;
		//Driver.WriteVirtualMemory64(ProcessId, component + 0x532, *(PULONG64)(&tmp), sizeof(std::uint8_t));
		Driver.WriteVirtualMemory64(ProcessId, component + 0x534, *(PULONG64)(&tmp), sizeof(std::uint8_t));
	}
}

void ESP1()
{
	for (DWORD64 idx = 0u; idx <= max_entities(); idx++) {
		DWORD64 entitytmp = entity(idx);
		if (!entitytmp) {
			continue;
		}
		enable_marker1(entitytmp);
		//enable_marker2(entitytmp);
	}
	//Sleep(500);
}

void ESP2()
{
	for (DWORD64 idx = 0u; idx <= max_entities(); idx++) {
		DWORD64 entitytmp = entity(idx);
		if (!entitytmp) {
			continue;
		}
		//enable_marker1(entitytmp);
		enable_marker2(entitytmp);
	}
	//Sleep(500);
}

DWORD64 getLocalPlayer()
{
	DWORD64 ProfileManager = Driver.ReadVirtualMemory<DWORD64>(ProcessId, ClientAddress + OFFSET_LOCAL_PLAYER, sizeof(DWORD64));
	DWORD64 temp = Driver.ReadVirtualMemory<DWORD64>(ProcessId, ProfileManager + 0x68, sizeof(DWORD64));
	temp = Driver.ReadVirtualMemory<DWORD64>(ProcessId, temp, sizeof(DWORD64));
	DWORD64 LocalPlayer = Driver.ReadVirtualMemory<DWORD64>(ProcessId, temp + 0x28, sizeof(DWORD64));
	return LocalPlayer;
}

void norecoil_temp()
{
	DWORD64 LocalPlayer = getLocalPlayer();
	DWORD64 temp = Driver.ReadVirtualMemory<DWORD64>(ProcessId, LocalPlayer + 0x78, sizeof(DWORD64));
	temp = Driver.ReadVirtualMemory<DWORD64>(ProcessId, temp + 0xC8, sizeof(DWORD64));
	DWORD64 Weapon = Driver.ReadVirtualMemory<DWORD64>(ProcessId, temp + 0x208, sizeof(DWORD64));
	float a = 1.0f;
	Driver.WriteVirtualMemory64(ProcessId, Weapon + 0xC0, *(PULONG64)(&a), sizeof(float));
	//Driver.WriteVirtualMemory64(ProcessId, Weapon + 0x50, *(PULONG64)(&a), sizeof(float));
	//Sleep(500); 
}

void norecoil()
{
	uint64_t patch_address = ClientAddress + PATCH_DRONEFIX;
	BYTE fix_b[8] = { 0x0f, 0x2f, 0x3d, 0x36, 0xEA, 0x84, 0x02, 0x00 };
	Driver.WriteVirtualMemory64(ProcessId, patch_address, *(PULONG64)(&fix_b), 7 * sizeof(byte));
	float floatval = 1.0f;
	Driver.WriteVirtualMemory64(ProcessId, ClientAddress + RDATA_RECOIL, *(PULONG64)(&floatval), sizeof(float));
}

void nospread()
{
	DWORD64 LocalPlayer = getLocalPlayer();
	DWORD64 temp = Driver.ReadVirtualMemory<DWORD64>(ProcessId, LocalPlayer + 0x78, sizeof(DWORD64));
	temp = Driver.ReadVirtualMemory<DWORD64>(ProcessId, temp + 0xC8, sizeof(DWORD64));
	DWORD64 Weapon = Driver.ReadVirtualMemory<DWORD64>(ProcessId, temp + 0x208, sizeof(DWORD64));
	float a = 0.0f;
	Driver.WriteVirtualMemory64(ProcessId, Weapon + 0xB0, *(PULONG64)(&a), sizeof(float));
	Driver.WriteVirtualMemory64(ProcessId, Weapon + 0x50, *(PULONG64)(&a), sizeof(float));
	//Sleep(500);
}

void outlineESP_Drone()
{
	DWORD64 gameManager = Driver.ReadVirtualMemory<DWORD64>(ProcessId, ClientAddress + OFFSET_GAME_MANAGER, sizeof(DWORD64));
	for (int i = 0; i < 11; i++) {
		DWORD64 tmp = Driver.ReadVirtualMemory<DWORD64>(ProcessId, gameManager + 0x1D8, sizeof(DWORD64));
		tmp = Driver.ReadVirtualMemory<DWORD64>(ProcessId, tmp + i * 0x8, sizeof(DWORD64));
		tmp = Driver.ReadVirtualMemory<DWORD64>(ProcessId, tmp + 0xA70, sizeof(DWORD64));
		tmp = Driver.ReadVirtualMemory<DWORD64>(ProcessId, tmp + 0x30, sizeof(DWORD64));
		tmp = Driver.ReadVirtualMemory<DWORD64>(ProcessId, tmp + 0x48, sizeof(DWORD64));
		tmp = Driver.ReadVirtualMemory<DWORD64>(ProcessId, tmp + 0x110, sizeof(DWORD64));
		byte a = 1;
		Driver.WriteVirtualMemory64(ProcessId, tmp + 0x50, *(PULONG64)(&a), sizeof(byte));
	}
}

void outlineESP_EXIT()
{
	byte a = 4;
	DWORD64 tmp = Driver.ReadVirtualMemory<DWORD64>(ProcessId, ClientAddress + OFFSET_OUTLINE_MANAGER, sizeof(DWORD64));
	tmp = Driver.ReadVirtualMemory<DWORD64>(ProcessId, tmp + 0x90, sizeof(DWORD64));
	tmp = Driver.ReadVirtualMemory<DWORD64>(ProcessId, tmp + 0x38, sizeof(DWORD64));
	byte comp = Driver.ReadVirtualMemory<DWORD64>(ProcessId, tmp + 0x128, sizeof(DWORD64));
	if (comp == 3)
	{
		Driver.WriteVirtualMemory64(ProcessId, tmp + 0x128, *(PULONG64)(&a), sizeof(byte));
	}
	tmp = Driver.ReadVirtualMemory<DWORD64>(ProcessId, ClientAddress + OFFSET_OUTLINE_MANAGER, sizeof(DWORD64));
	tmp = Driver.ReadVirtualMemory<DWORD64>(ProcessId, tmp + 0x88, sizeof(DWORD64));
	tmp = Driver.ReadVirtualMemory<DWORD64>(ProcessId, tmp + 0x38, sizeof(DWORD64));
	tmp = Driver.ReadVirtualMemory<DWORD64>(ProcessId, tmp + 0x68, sizeof(DWORD64));
	comp = Driver.ReadVirtualMemory<DWORD64>(ProcessId, tmp + 0x20, sizeof(DWORD64));
	if (comp == 3)
	{
		Driver.WriteVirtualMemory64(ProcessId, tmp + 0x20, *(PULONG64)(&a), sizeof(byte));
	}
}

int getStatus() {
	uint64_t chain;
	chain = Driver.ReadVirtualMemory<DWORD64>(ProcessId, ClientAddress + ROUNDMANAGER, sizeof(DWORD64));
	int res = Driver.ReadVirtualMemory<int>(ProcessId, chain + 0x2E8, sizeof(int));
	return res;
}
