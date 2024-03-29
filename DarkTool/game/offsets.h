#pragma once
#include <cstdint>

namespace offsets {
	void initialize();
	inline uintptr_t dwClientState,
		dwClientState_GetLocalPlayer,
		dwClientState_PlayerInfo,
		dwClientState_State,
		dwClientState_ViewAngles,
		clientstate_delta_ticks,
		dwEntityList,
		dwForceAttack,
		dwForceJump,
		dwGlobalVars,
		dwGlowObjectManager,
		dwLocalPlayer,
		dwPlayerResource,
		dwSetClanTag,
		dwViewMatrix,
		m_pStudioHdr,
		m_bDormant,
		model_ambient_min,
		m_dwModelPrecache,
		dw_RevealRankFn,
		m_dwClientCmdFn,
		convar_name_hash_table,
		interface_engine_cvar,
		m_OriginalOwnerXuidLow,
		m_aimPunchAngle,
		m_bGunGameImmunity,
		m_bHasDefuser,
		m_bIsScoped,
		m_bSpottedByMask,
		m_dwBoneMatrix,
		m_fAccuracyPenalty,
		m_fFlags,
		m_flFallbackWear,
		m_flC4Blow,
		m_flFlashMaxAlpha,
		m_flNextPrimaryAttack,
		m_hActiveWeapon,
		m_hMyWeapons,
		m_iAccountID,
		m_iClip1,
		m_iCrosshairId,
		m_iEntityQuality,
		m_iFOV,
		m_iHealth,
		m_iItemDefinitionIndex,
		m_iItemIDHigh,
		m_iKills,
		m_iTeamNum,
		m_nFallbackPaintKit,
		m_nFallbackSeed,
		m_nFallbackStatTrak,
		m_szCustomName,
		m_vecOrigin,
		m_vecViewOffset,
		m_nModelIndex,
		m_hViewModel,
		m_totalHitsOnServer;
}