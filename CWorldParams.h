#pragma once

#include "pch.h"
#include "Registry.h"
#include "../Dots/CUtil.h"
#include "InstructionSet.h"


#define REG_ROOT_KEY					_T("Software\\Darquidsoft\\Koslet")
#define REG_VALUE_G						_T("G")
#define REG_VALUE_STEP_LENGTH			_T("StepLength")
#define REG_VALUE_COMMON				_T("Common")
#define REG_VALUE_GIANT_STAR			_T("GiantStar")
#define REG_VALUE_BLACKHOLE				_T("Blackhole")
#define REG_VALUE_ANTI_G_COMMON			_T("AntiGCommon")
#define REG_VALUE_ANTI_G_GIANT_STAR		_T("AntiGGiantStar")
#define REG_VALUE_ANTI_G_BLACKHOLE		_T("AntiGBlackhole")
#define REG_VALUE_TRAIL					_T("Trail")
#define REG_VALUE_EXPLOSION				_T("Explosion")
#define REG_VALUE_SAVE_SESSION			_T("SaveSession")
#define REG_VALUE_OSD					_T("OSD")


class CWorldParams
{
public:
	DWORD dwG{ 300u };
	DWORD dwStepLength{ 2u };
	DWORD dwCommon{ 500u };
	DWORD dwGiantStar{ 0u };
	DWORD dwBlackhole{ 0u };
	DWORD dwAntiGCommon{ 0u };
	DWORD dwAntiGGiantStar{ 0u };
	DWORD dwAntiGBlackhole{ 0u };
	DWORD dwTrail{ 3u };
	DWORD dwExplosion{ 2u };
	DWORD dwSaveSession{ FALSE };
	DWORD dwOSD{ FALSE };


	void SetDefault(void)
	{
		dwG = 300u;
		dwStepLength = 2u;
		dwCommon = 500u;
		dwGiantStar = 0u;
		dwBlackhole = 0u;
		dwAntiGCommon = dwAntiGGiantStar = dwAntiGBlackhole = 0u;
	}
	

	bool SaveToRegistry(void) const
	{
		CRegistry reg;

		if (reg.CreateKey(reg.currentUser, REG_ROOT_KEY))
		{
			reg.SetValue(REG_VALUE_G, dwG);
			reg.SetValue(REG_VALUE_STEP_LENGTH, dwStepLength);
			reg.SetValue(REG_VALUE_COMMON, dwCommon);
			reg.SetValue(REG_VALUE_GIANT_STAR, dwGiantStar);
			reg.SetValue(REG_VALUE_BLACKHOLE, dwBlackhole);
			reg.SetValue(REG_VALUE_ANTI_G_COMMON, dwAntiGCommon);
			reg.SetValue(REG_VALUE_ANTI_G_GIANT_STAR, dwAntiGGiantStar);
			reg.SetValue(REG_VALUE_ANTI_G_BLACKHOLE, dwAntiGBlackhole);
			reg.SetValue(REG_VALUE_TRAIL, dwTrail);
			reg.SetValue(REG_VALUE_EXPLOSION, dwExplosion);
			reg.SetValue(REG_VALUE_SAVE_SESSION, dwSaveSession);
			reg.SetValue(REG_VALUE_OSD, dwOSD);
			reg.CloseKey();
			return true;
		}
		else
			return false;
	}

	bool LoadFromRegistry(void)
	{
		CRegistry reg;
		if (reg.OpenKey(reg.currentUser, REG_ROOT_KEY))
		{
			reg.GetValue(REG_VALUE_G, dwG);
			reg.GetValue(REG_VALUE_STEP_LENGTH, dwStepLength);
			reg.GetValue(REG_VALUE_COMMON, dwCommon);
			reg.GetValue(REG_VALUE_GIANT_STAR, dwGiantStar);
			reg.GetValue(REG_VALUE_BLACKHOLE, dwBlackhole);
			reg.GetValue(REG_VALUE_ANTI_G_COMMON, dwAntiGCommon);
			reg.GetValue(REG_VALUE_ANTI_G_GIANT_STAR, dwAntiGGiantStar);
			reg.GetValue(REG_VALUE_ANTI_G_BLACKHOLE, dwAntiGBlackhole);
			reg.GetValue(REG_VALUE_TRAIL, dwTrail);
			reg.GetValue(REG_VALUE_EXPLOSION, dwExplosion);
			reg.GetValue(REG_VALUE_SAVE_SESSION, dwSaveSession);
			reg.GetValue(REG_VALUE_OSD, dwOSD);
			reg.CloseKey();
			Normalize();
			return true;
		}
		else
			return false;
	}


	void Normalize(void)
	{
		dwG = dwG % 4001u;
		dwG = dwG == 0u ? 1u : dwG;
		dwStepLength = dwStepLength % 5u;
		dwCommon = dwCommon % 5001u;
		dwGiantStar = dwGiantStar % 257u;
		dwBlackhole = dwBlackhole % 17u;
		dwAntiGCommon = dwAntiGCommon % 5001u;
		dwAntiGGiantStar = dwAntiGGiantStar % 257u;
		dwAntiGBlackhole = dwAntiGBlackhole % 17u;
		if (dwCommon + dwGiantStar + dwBlackhole + dwAntiGCommon + dwAntiGGiantStar + dwAntiGBlackhole < 2u)
			dwCommon = 500u;
		dwTrail = dwTrail % 4u;
		dwExplosion = dwExplosion % 4u;
	}


	bool IsMatch(int nG, int ntc, int nc, int ng, int nb, int nac, int nag, int nab) const
	{
		bool bRes = true;
		bRes = bRes && (nG > 0 && nG < 4001);
		bRes = bRes && (ntc == (nc + ng + nb + nac + nag + nab));
		bRes = bRes && (ntc > 1);
		bRes = bRes && (dwCommon == nc && dwGiantStar == ng && dwBlackhole == nb &&
			dwAntiGCommon == nac && dwAntiGGiantStar == nag && dwAntiGBlackhole == nab);
		return bRes;
	}


	void MakePreviewValues(void)
	{
		dwG = dwG == 0u ? 1u : dwG;
		dwG = dwG > 64u ? 64u : dwG;
		dwCommon = dwCommon > 8u ? 8u : dwCommon;
		dwGiantStar = dwGiantStar > 3u ? 3u : dwGiantStar;
		dwBlackhole = dwBlackhole > 1u ? 1u : dwBlackhole;
		dwAntiGCommon = dwAntiGCommon > 8u ? 8u : dwAntiGCommon;
		dwAntiGGiantStar = dwAntiGGiantStar > 3u ? 3u : dwAntiGGiantStar;
		dwAntiGBlackhole = dwAntiGBlackhole > 1u ? 1u : dwAntiGBlackhole;
		if (dwCommon + dwGiantStar + dwBlackhole + dwAntiGCommon + dwAntiGGiantStar + dwAntiGBlackhole < 2u)
			dwCommon = 8u;
	}


	int GetThreadCount(void) const
	{
		DWORD dw = Total();
		if (dw > 1000u)
			return CUtil::GetThreadCount();
		else
			return 1;
	}


	bool SIMD(void) const
	{
		return InstructionSet::AVX2() && Total() > 200u;
	}


	DWORD Total(void) const
	{
		return dwCommon + dwGiantStar + dwBlackhole + dwAntiGCommon + dwAntiGGiantStar + dwAntiGBlackhole;
	}
};

