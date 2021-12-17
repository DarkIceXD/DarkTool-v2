#include "offsets.h"
#include "netvar.h"
#include "../memory/memory.h"
#include <thread>

void offsets::initialize()
{
	memory::module client, engine, vstdlib_dll;
	do {
		client = memory::get_module(L"client.dll");
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	} while (!client.base);
	do {
		engine = memory::get_module(L"engine.dll");
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	} while (!engine.base);
	do {
		vstdlib_dll = memory::get_module(L"vstdlib.dll");
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	} while (!vstdlib_dll.base);
	dwClientState = memory::get_offset(engine, "\xA1\x00\x00\x00\x00\x33\xD2\x6A\x00\x6A\x00\x33\xC9\x89\xB0", "x????xxxxxxxxxx", 1);
	dwClientState_GetLocalPlayer = memory::get_offset(engine, "\x8B\x80\x00\x00\x00\x00\x40\xC3", "xx????xx", 2);
	dwClientState_State = memory::get_offset(engine, "\x83\xB8\x00\x00\x00\x00\x00\x0F\x94\xC0\xC3", "xx?????xxxx", 2);
	dwClientState_ViewAngles = memory::get_offset(engine, "\xF3\x0F\x11\x86\x00\x00\x00\x00\xF3\x0F\x10\x44\x24\x00\xF3\x0F\x11\x86", "xxxx????xxxxx?xxxx", 4);
	dwClientState_PlayerInfo = memory::get_offset(engine, "\x8B\x89\x00\x00\x00\x00\x85\xC9\x0F\x84\x00\x00\x00\x00\x8B\x01", "xx????xxxx????xx", 2);
	clientstate_delta_ticks = memory::get_offset(engine, "\xC7\x87\x00\x00\x00\x00\x00\x00\x00\x00\xFF\x15\x00\x00\x00\x00\x83\xC4\x08", "xx????????xx????xxx", 2);
	dwEntityList = memory::get_offset(client, "\xBB\x00\x00\x00\x00\x83\xFF\x01\x0F\x8C\x00\x00\x00\x00\x3B\xF8", "x????xxxxx????xx", 1);
	dwForceAttack = memory::get_offset(client, "\x89\x0D\x00\x00\x00\x00\x8B\x0D\x00\x00\x00\x00\x8B\xF2\x8B\xC1\x83\xCE\x04", "xx????xx????xxxxxx", 2);
	dwForceJump = memory::get_offset(client, "\x8B\x0D\x00\x00\x00\x00\x8B\xD6\x8B\xC1\x83\xCA\x02", "xx????xxxxxxx", 2);
	dwGlobalVars = memory::get_offset(engine, "\x68\x00\x00\x00\x00\x68\x00\x00\x00\x00\xFF\x50\x08\x85\xC0", "x????x????xxxxx", 1);
	dwGlowObjectManager = memory::get_offset(client, "\xA1\x00\x00\x00\x00\xA8\x01\x75\x4B", "x????xxxx", 1) + 4;
	dwLocalPlayer = memory::get_offset(client, "\x8D\x34\x85\x00\x00\x00\x00\x89\x15\x00\x00\x00\x00\x8B\x41\x08\x8B\x48\x04\x83\xF9\xFF", "xxx????xx????xxxxxxxxx", 3) + 4;
	dwPlayerResource = memory::get_offset(client, "\x8B\x3D\x00\x00\x00\x00\x85\xFF\x0F\x84\x00\x00\x00\x00\x81\xC7", "xx????xxxx????xx", 2);
	dwSetClanTag = memory::get_offset(engine, "\x53\x56\x57\x8B\xDA\x8B\xF9\xFF\x15", "xxxxxxxxx");
	dwViewMatrix = memory::get_offset(client, "\x0F\x10\x05\x00\x00\x00\x00\x8D\x85\x00\x00\x00\x00\xB9", "xxx????xx????x", 3) + 176;
	m_pStudioHdr = memory::get_offset(client, "\x8B\xB6\x00\x00\x00\x00\x85\xF6\x74\x05\x83\x3E\x00\x75\x02\x33\xF6\xF3\x0F\x10\x44\x24", "xx????xxxxxxxxxxxxxxxx", 2);
	m_bDormant = memory::get_offset(client, "\x8A\x81\x00\x00\x00\x00\xC3\x32\xC0", "xx????xxx", 2) + 8;
	model_ambient_min = memory::get_offset(engine, "\xF3\x0F\x10\x0D\x00\x00\x00\x00\xF3\x0F\x11\x4C\x24\x00\x8B\x44\x24\x20\x35\x00\x00\x00\x00\x89\x44\x24\x0C", "xxxx????xxxxx?xxxxx????xxxx", 4);
	dw_RevealRankFn = memory::get_offset(client, "\x55\x8B\xEC\x51\xA1\x00\x00\x00\x00\x85\xC0\x75\x37", "xxxxx????xxxx");
	m_dwModelPrecache = memory::get_offset(engine, "\x0C\x3B\x81\x00\x00\x00\x00\x75\x11\x8B\x45\x10\x83\xF8\x01\x7C\x09\x50\x83", "xxx????xxxxxxxxxxxx", 3);
	m_dwClientCmdFn = memory::get_offset(engine, "\x55\x8B\xEC\x8B\x55\x08\x33\xC9\x6A\x00\x6A\x00\xE8\x00\x00\x00\x00\x83\xC4", "xxxxxxxxxxxxx????xx");
	convar_name_hash_table = memory::get_offset(vstdlib_dll, "\x8B\x3C\x85", "xxx", 3);
	interface_engine_cvar = memory::get_offset(vstdlib_dll, "\x8B\x0D\x00\x00\x00\x00\xC7\x05", "xx????xx", 2);
	auto dwGetAllClasses = memory::get_offset(client, "\x44\x54\x5F\x54\x45\x57\x6F\x72\x6C\x64\x44\x65\x63\x61\x6C", "xxxxxxxxxxxxxxx");
	dwGetAllClasses = memory::get_offset(client, (char*)&dwGetAllClasses, "xxxx", 0x2B);
	m_OriginalOwnerXuidLow = netvar::find(dwGetAllClasses, "DT_BaseAttributableItem", "m_OriginalOwnerXuidLow");
	m_aimPunchAngle = netvar::find(dwGetAllClasses, "DT_BasePlayer", "m_aimPunchAngle");
	m_bGunGameImmunity = netvar::find(dwGetAllClasses, "DT_CSPlayer", "m_bGunGameImmunity");
	m_bHasDefuser = netvar::find(dwGetAllClasses, "DT_CSPlayer", "m_bHasDefuser");
	m_bIsScoped = netvar::find(dwGetAllClasses, "DT_CSPlayer", "m_bIsScoped");
	m_bSpottedByMask = netvar::find(dwGetAllClasses, "DT_BaseEntity", "m_bSpottedByMask");
	m_dwBoneMatrix = netvar::find(dwGetAllClasses, "DT_BaseAnimating", "m_nForceBone") + 28;
	m_fAccuracyPenalty = netvar::find(dwGetAllClasses, "DT_WeaponCSBase", "m_fAccuracyPenalty");
	m_fFlags = netvar::find(dwGetAllClasses, "DT_CSPlayer", "m_fFlags");
	m_flFallbackWear = netvar::find(dwGetAllClasses, "DT_BaseAttributableItem", "m_flFallbackWear");
	m_flC4Blow = netvar::find(dwGetAllClasses, "DT_PlantedC4", "m_flC4Blow");
	m_flFlashMaxAlpha = netvar::find(dwGetAllClasses, "DT_CSPlayer", "m_flFlashMaxAlpha");
	m_flNextPrimaryAttack = netvar::find(dwGetAllClasses, "DT_BaseCombatWeapon", "m_flNextPrimaryAttack");
	m_hActiveWeapon = netvar::find(dwGetAllClasses, "DT_BasePlayer", "m_hActiveWeapon");
	m_hMyWeapons = m_hActiveWeapon - 256;
	m_iAccountID = netvar::find(dwGetAllClasses, "DT_BaseAttributableItem", "m_iAccountID");
	m_iClip1 = netvar::find(dwGetAllClasses, "DT_BaseCombatWeapon", "m_iClip1");
	m_iCrosshairId = m_bHasDefuser + 92;
	m_iEntityQuality = netvar::find(dwGetAllClasses, "DT_BaseAttributableItem", "m_iEntityQuality");
	m_iFOV = netvar::find(dwGetAllClasses, "DT_CSPlayer", "m_iFOV");
	m_iHealth = netvar::find(dwGetAllClasses, "DT_BasePlayer", "m_iHealth");
	m_iItemDefinitionIndex = netvar::find(dwGetAllClasses, "DT_BaseCombatWeapon", "m_iItemDefinitionIndex");
	m_iItemIDHigh = netvar::find(dwGetAllClasses, "DT_BaseAttributableItem", "m_iItemIDHigh");
	m_iKills = netvar::find(dwGetAllClasses, "DT_CSPlayerResource", "m_iKills");
	m_iTeamNum = netvar::find(dwGetAllClasses, "DT_BasePlayer", "m_iTeamNum");
	m_nFallbackPaintKit = netvar::find(dwGetAllClasses, "DT_BaseAttributableItem", "m_nFallbackPaintKit");
	m_nFallbackSeed = netvar::find(dwGetAllClasses, "DT_BaseAttributableItem", "m_nFallbackSeed");
	m_nFallbackStatTrak = netvar::find(dwGetAllClasses, "DT_BaseAttributableItem", "m_nFallbackStatTrak");
	m_szCustomName = netvar::find(dwGetAllClasses, "DT_BaseAttributableItem", "m_szCustomName");
	m_vecOrigin = netvar::find(dwGetAllClasses, "DT_BasePlayer", "m_vecOrigin");
	m_vecViewOffset = netvar::find(dwGetAllClasses, "DT_CSPlayer", "m_vecViewOffset[0]");
	m_nModelIndex = netvar::find(dwGetAllClasses, "DT_BaseViewModel", "m_nModelIndex");
	m_hViewModel = netvar::find(dwGetAllClasses, "DT_BasePlayer", "m_hViewModel[0]");
	m_totalHitsOnServer = netvar::find(dwGetAllClasses, "DT_CSPlayer", "m_totalHitsOnServer");
}
