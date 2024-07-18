/////////////////////////////////////
//
//		 yakolla
//
/////////////////////////////////////
#include "stdafx.h"
#include "InitGame.h"
#include "effect.h"
#include "EffectProcess.h"
#include "GameControl.h"
#include "HashTable.h"
#include "ObjectManager.h"
#include "EffectInfo.h"
#include "CorumResource.h"
#include "message.h"
#include "gamemenuwnd.h"
#include "Chat.h"
#include "GlobalSoundHandles.h"
#include "DUNGEON_DATA_EX.h"
#include "map.h"
#include "Parsing.h"
#include "CodeFun.h"


void EffectLayer::Init(BOOL bChk)
{
	m_dwCount	= 0;
	memset(m_Effect, 0, sizeof(m_Effect));
	memset(g_sSkillInfoDP, 0, sizeof(g_sSkillInfoDP));

	LoadScript(bChk);	
}

void EffectLayer::LoadScript(BOOL bChk)
{
	// 스킬 데이터를 로드한다.
	Load(GetFile("skill.cdb", DATA_TYPE_MANAGER));
	
	char temp[100]={0,}; 
	for(DWORD i = 0; i < MAX_SKILL_COUNT; ++i)
	{
		m_nEffectSize[i] = (BYTE)lstrlen(m_Effect[i].szName);		
		
		if(bChk)
		{
			for(int joint = 0; joint < m_Effect[i].bJointEffect; ++joint)
			{
				lstrcpy(temp, g_pObjManager->GetFile(m_Effect[i].dwResourceID+joint));			
				g_pExecutive->PreLoadGXObject( temp );
			}
			
			if (m_Effect[i].dwStatusResourceID)
			{
				lstrcpy(temp, g_pObjManager->GetFile( m_Effect[i].dwStatusResourceID));
				g_pExecutive->PreLoadGXObject( temp );
			}
		}

		// 이펙트에 알맞는 펑션들 정의
		switch(m_Effect[i].bID)
		{
		case __SKILL_ICEMISSILE__:
		case __SKILL_FIREMISSILE__:
		case __SKILL_DETONATION__:
		case __SKILL_SOULBLASTER:
		case __SKILL_ROUNDRANGE__:
		case __SKILL_FLASHSHOCK__:
		//김영대 발산
		//case __SKILL_POSIONCLOUD__:
			m_Effect[i].SkillFunc = FireBallHeadFunctionBySlowboat;
			break;
		case __SKILL_MAGMAWALL__:
		case __SKILL_ICEWALL__:
			m_Effect[i].SkillFunc = EffectWall;
			break;
		case __SKILL_BLASTQUAKE__:
			m_Effect[i].SkillFunc = EffectBlastQuake;
			break;
		case __SKILL_CHAINATTACKER__:
			m_Effect[i].SkillFunc = EffectChainAttackerFunc;
			break;
		default:
			{
				if (m_Effect[i].bType == TYPE_TIMEZERO && (m_Effect[i].dwStatusResourceID==0) )
				{// 발동이펙트가 없다.
					switch(m_Effect[i].GetEffectPosition())
					{
					case EFFECT_TYPE_STATUS_TOP:
						m_Effect[i].SkillFunc = EffectSkillUserStatusTopFunc;	
						break;
					case EFFECT_TYPE_STATUS_CENTER:
						m_Effect[i].SkillFunc = EffectSkillUserStatusCenterFunc;	
						break;
					case EFFECT_TYPE_STATUS_BOTTOM:
						m_Effect[i].SkillFunc = EffectSkillUserStatusBottomFunc;	
						break;
					}
				}
				else
				{
					m_Effect[i].SkillFunc = EffectOnceAndRemoveFunc;	// 일단 한번 스킬 보이고 지운다.
				}
			}			
		}
				
		m_Effect[i].MonsterDamageFunc = SkillDamageFunc1; // 몬스터 맞는 펑션
		m_Effect[i].MonsterKillFunc = MonsterKillFunc; // 몬스터 죽는 펑션.

		switch(m_Effect[i].bID)
		{// 몬스터 죽기전에 액션
		case __SKILL_METEOR__:
			{
				m_Effect[i].MonsterKillWithAction  = MonsterKillWithAction2; // 몬스터 죽기전에 죽는 액션 펑션.				
			}break;
		case __SKILL_ICEWALL__:
			{
				m_Effect[i].MonsterKillWithAction  = MonsterKillWithAction3; // 몬스터 죽기전에 죽는 액션 펑션.
			}break;
		case __SKILL_BLASTQUAKE__:
			{
				m_Effect[i].MonsterKillWithAction  = MonsterKillWithAction3; // 몬스터 죽기전에 죽는 액션 펑션.
			}break;
		case __SKILL_FIREMISSILE__:
		case __SKILL_ICEMISSILE__:
		case __SKILL_SOULBLASTER:
		case __SKILL_FLASHSHOCK__:
		case __SKILL_POSIONCLOUD__:
		case __SKILL_DETONATION__:
			{
				m_Effect[i].MonsterKillWithAction  = MonsterKillWithAction;
			}break;
		default:
			m_Effect[i].MonsterKillWithAction  = MonsterKillWithAction;			
		}

		switch(m_Effect[i].bID)
		{// 몬스터 스킬맞으면서의 액션
		case __SKILL_EARTHQUAKE__:
			{
				m_Effect[i].MonsterSKillDamageWithAction = MonsterSKillDamageWithAction3;
			}break;
		case __SKILL_ICEWALL__:
			{
				m_Effect[i].MonsterSKillDamageWithAction = NULL;				
			}break;
		default:
			m_Effect[i].MonsterSKillDamageWithAction = MonsterSKillDamageWithAction1;
		}
		
		ParsingSkillDescription(m_Effect[i].bID);			
	}
}

DWORD EffectLayer::Load(char *szFileName)
{
	BASESKILL baseskill[MAX_SKILL_COUNT];	memset(baseskill, 0, sizeof(baseskill));
	
	int nTotalSize = DecodeCDBData(szFileName, baseskill);
	int m_nMaxNode = nTotalSize / sizeof(BASESKILL);
	
	for(int i=0; i<m_nMaxNode; i++ )
	{
		memcpy(&m_Effect[baseskill[i].bID], &baseskill[i], sizeof(BASESKILL));		
	}
		
	return TRUE;
}

DWORD EffectLayer::ParsingSkillDescription(BYTE bSkillKind)
{	
	char *	pszToken = NULL;
	char	szTemp[0xff]={0,};
	char *	szSeps[7]	= {"[n]", "[min]", "[mn]", "[max]", "[dur]", "[pbt]", "[cps]"};	

	int nIndex	= 0;
	int nLen	= 0;
	
	__lstrcpyn(szTemp, m_Effect[bSkillKind].szDescription, sizeof(m_Effect[bSkillKind].szDescription));

	for(;;) 
	{
		pszToken = (char*)FindBeforeTokenStrings(szTemp+nLen, (char **)szSeps, 7);		
		
		if(pszToken==NULL) 
			break;

		g_sSkillInfoDP[bSkillKind].wSkillLevel	= (WORD)m_Effect[bSkillKind].dwMinMastery;
		g_sSkillInfoDP[bSkillKind].bActive		= TRUE;				
		lstrcat(g_sSkillInfoDP[bSkillKind].szInfo, pszToken);

		int	nRtLen = lstrlen(pszToken);

		if(SearchString(bSkillKind, nLen+nRtLen, nIndex)==FALSE)
			break;

		nLen += nRtLen;

		switch(g_sSkillInfoDP[bSkillKind].enSkillInfo[nIndex])
		{
			case SKILLINFO_LEVEL:	nLen += 3;	break;
			case SKILLINFO_SP:		nLen += 4;	break;
			case SKILLINFO_MIN:		nLen += 5;	break;
			case SKILLINFO_MAX:		nLen += 5;	break;
			case SKILLINFO_DUR:		nLen += 5;	break;
			case SKILLINFO_PBT:		nLen += 5;	break;
			case SKILLINFO_CPS:		nLen += 5;	break;
		}

		nIndex++;
		lstrcat(g_sSkillInfoDP[bSkillKind].szInfo, "%d");						
		g_sSkillInfoDP[bSkillKind].byIndex = (BYTE)nIndex;
	
	}
		
	return TRUE;
}

BOOL EffectLayer::SearchString(BYTE bSkillKind, int nLen, int nIndex)
{				
	Effect* pEffect = g_pEffectLayer->GetEffectInfo(bSkillKind);
	char szVal[0xf]={0,};

	memset(szVal, 0, sizeof(szVal));
	memcpy(szVal, pEffect->szDescription+nLen, 3);

	if(__strcmp(szVal, "[n]")==0)
		g_sSkillInfoDP[bSkillKind].enSkillInfo[nIndex] = SKILLINFO_LEVEL;
	
	if(g_sSkillInfoDP[bSkillKind].enSkillInfo[nIndex]==SKILLINFO_NONE)
	{
		memset(szVal, 0, sizeof(szVal));
		memcpy(szVal, pEffect->szDescription+nLen, 4);

		if(__strcmp(szVal, "[mn]")==0)
			g_sSkillInfoDP[bSkillKind].enSkillInfo[nIndex] = SKILLINFO_SP;
	}
	
	if(g_sSkillInfoDP[bSkillKind].enSkillInfo[nIndex]==SKILLINFO_NONE)
	{
		memset(szVal, 0, sizeof(szVal));
		memcpy(szVal, pEffect->szDescription+nLen, 5);

		if(__strcmp(szVal, "[min]")==0)		
			g_sSkillInfoDP[bSkillKind].enSkillInfo[nIndex] = SKILLINFO_MIN;

		if(g_sSkillInfoDP[bSkillKind].enSkillInfo[nIndex]==SKILLINFO_NONE)
		{
			memset(szVal, 0, sizeof(szVal));
			memcpy(szVal, pEffect->szDescription+nLen, 5);

			if(__strcmp(szVal, "[max]")==0)			
				g_sSkillInfoDP[bSkillKind].enSkillInfo[nIndex] = SKILLINFO_MAX;
		}

		if(g_sSkillInfoDP[bSkillKind].enSkillInfo[nIndex]==SKILLINFO_NONE)
		{
			memset(szVal, 0, sizeof(szVal));
			memcpy(szVal, pEffect->szDescription+nLen, 5);

			if(__strcmp(szVal, "[dur]")==0)
				g_sSkillInfoDP[bSkillKind].enSkillInfo[nIndex] = SKILLINFO_DUR;
		}

		if(g_sSkillInfoDP[bSkillKind].enSkillInfo[nIndex]==SKILLINFO_NONE)
		{
			memset(szVal, 0, sizeof(szVal));
			memcpy(szVal, pEffect->szDescription+nLen, 5);

			if(__strcmp(szVal, "[pbt]")==0)
				g_sSkillInfoDP[bSkillKind].enSkillInfo[nIndex] = SKILLINFO_PBT;
		}		

		if(g_sSkillInfoDP[bSkillKind].enSkillInfo[nIndex]==SKILLINFO_NONE)
		{
			memset(szVal, 0, sizeof(szVal));
			memcpy(szVal, pEffect->szDescription+nLen, 5);

			if(__strcmp(szVal, "[cps]")==0)
				g_sSkillInfoDP[bSkillKind].enSkillInfo[nIndex] = SKILLINFO_CPS;
		}
	}
	
	return (g_sSkillInfoDP[bSkillKind].enSkillInfo[nIndex]!=SKILLINFO_NONE);
}

EffectDesc* EffectLayer::CreateEffect(BYTE bSkillKind, BYTE bJoint, BOOL bOwn)
{		
	Effect* pEffect = GetEffectInfo(bSkillKind );
	char* szFileName = g_pObjManager->GetFile(pEffect->dwResourceID+bJoint);

	if( SKILL_METEOR == pEffect->dwResourceID+bJoint )
	{
		int	a = 0;
	}

	EffectDesc* pEffectDesc = CreateGXObject(szFileName, bOwn, bSkillKind);

	pEffectDesc->bEffectInfoNum	= bSkillKind;
	pEffectDesc->bJoint			= bJoint;
	pEffectDesc->pEffect		= pEffect;	

	//if (!bJoint)//주석처리
	{		
	//	SOUND_FILE_HANDLE	pHandle;
		char szBase[255] = {0, };
		char szSoundFile[0xff]={0,};
		char* p = NULL;

		if (g_pSoundLib)
		{
			for(int c = 0; c < 8; ++c)
			{
				if (pEffect->wSoundNum[c])
				{
					szBase[0] = 0;
					p = GetFile(szBase, DATA_TYPE_SOUND);
					memcpy(szBase, p, lstrlen(p));

					wsprintf(szSoundFile, "%s%d.wav", szBase, pEffect->wSoundNum[c], pEffectDesc->vecOwn);
					GetSoundEffectFile(&pEffectDesc->m_hSoundFile[c], szSoundFile);
					GetSoundEffect(&pEffectDesc->m_pSound[c], pEffectDesc->m_hSoundFile[c], pEffectDesc->vecOwn);					
				}				
			}
		}
	}
	
	return pEffectDesc;
}

BOOL EffectLayer::IsEffectShow(EffectDesc* pEffectDesc)
{
	DWORD dwEffectOptionLevel = CGameMenuWnd::GetInstance()->m_byEffectIndex;
	
	BOOL bShow	= 1;
	
	if(dwEffectOptionLevel > __EFFECT_OPTION__OFF__)
	{
		if(dwEffectOptionLevel == __EFFECT_OPTION__LOW__)
			bShow = pEffectDesc->bOwn;
		
		return bShow;
	}	
	return 0;

/*
	DWORD dwEffectOptionLevel = CGameMenuWnd::GetInstance()->m_byEffectIndex;
	BOOL bShow	= 1;
	if (dwEffectOptionLevel > __EFFECT_OPTION__OFF__ )
	{
		if (dwEffectOptionLevel == __EFFECT_OPTION__LOW__ )
		{
			
			bShow = pEffectDesc->bOwn;
		}
	}
	else bShow = 0;
	
	return bShow;
*/
}

EffectDesc* EffectLayer::CreateStatusEffect(BYTE bSkillKind, BYTE bJoint, BOOL bOwn)
{
	Effect* pEffect = GetEffectInfo(bSkillKind );
	EffectDesc* effectDesc = CreateGXObject(g_pObjManager->GetFile(pEffect->dwStatusResourceID+bJoint), bOwn, bSkillKind);
	
	effectDesc->bEffectInfoNum			= bSkillKind;
	effectDesc->bJoint					= bJoint;
	effectDesc->hEffect.pDesc->bType	= OBJECT_TYPE_EFFECT;
	effectDesc->hEffect.pDesc->pInfo	= effectDesc;
	effectDesc->pEffect					= pEffect;
		
	return effectDesc;
}

EffectDesc* EffectLayer::CreateGXObject(char *szFile, BOOL bOwn, WORD wChrNum)
{
	EffectDesc*	pEffectDesc = (EffectDesc*)LALAlloc( g_pEffectPool );
	memset(pEffectDesc, 0, sizeof(EffectDesc));

	pEffectDesc->hEffect.pDesc				= AllocObjDesc();	
	pEffectDesc->hEffect.pHandle			= CreateHandleObject( szFile, GXPlayerPROC, pEffectDesc->hEffect.pDesc,0);//GXOBJECT_CREATE_TYPE_EFFECT );
	pEffectDesc->hEffect.pDesc->bType		= OBJECT_TYPE_EFFECT;
	pEffectDesc->hEffect.pDesc->pInfo		= pEffectDesc;
	pEffectDesc->wChrNum					= wChrNum;

	g_pExecutive->DisablePick(pEffectDesc->hEffect.pHandle);
	g_pExecutive->DisableUpdateShading(pEffectDesc->hEffect.pHandle);
	g_pExecutive->DisableSendShadow(pEffectDesc->hEffect.pHandle);
	
	pEffectDesc->dwHashKey					= ++m_dwCount;
	pEffectDesc->bOwn						= bOwn;
	
	g_pEffectHash->Insert(pEffectDesc, m_dwCount);

	if (wChrNum != __CHR_EFFECT_NONE__)
	{		
		char szFileName[0xff]={0,};
		lstrcpy(szFileName, szFile);
		lstrcpy(&szFileName[lstrlen(szFileName)-3], "cdt");
//		InitChrInfo( szFileName, wChrNum,OBJECT_TYPE_EFFECT );
		InitChrInfo( GetFile( szFileName, DATA_TYPE_CDT ),wChrNum,OBJECT_TYPE_EFFECT );		
	}	

	if (!IsEffectShow(pEffectDesc)) g_pExecutive->DisableRender(pEffectDesc->hEffect.pHandle);

	return pEffectDesc;
}

BOOL EffectLayer::IsEffectUse(BYTE bSkillKind, VECTOR3* vecTarget, DWORD dwStartSkillTick)
{
	if (g_pThisDungeon->GetDungeonType() == DUNGEON_TYPE_VILLAGE 
		&& !g_pMainPlayer->m_bCurLayer )
	{
		// "마을에선 스킬사용이 안됩니다."
		DisplayMessageAdd(g_Message[ETC_MESSAGE548].szMessage, 0xFFFFC309);	// MSG_ID : 147 ; 사정거리 안에 없습니다.
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_Camera.CameraDesc.v3From, FALSE);
		return FALSE;
	}
	
	char bSkillLevel = char(g_pMainPlayer->GetSkillLevel(bSkillKind)-1);	
	if (bSkillLevel < 0)
	{
		DisplayMessageAdd(g_Message[ETC_MESSAGE146].szMessage, 0xFFFFC309);	// MSG_ID : 146 ; 스킬 레벨이 낮아 사용할수 없습니다.
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_Camera.CameraDesc.v3From, FALSE);
		return FALSE;
	}

	MAP_TILE* pTile = g_pMap->GetTile(vecTarget->x, vecTarget->z);
	if (pTile && !pTile->wAttr.uSection)
	{
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_Camera.CameraDesc.v3From, FALSE);
		return FALSE;
	}

	float fDist = CalcDistance( &g_pMainPlayer->m_v3CurPos, vecTarget );
	Effect* pEffect = GetEffectInfo(bSkillKind);

	if( pEffect->dwRange && fDist > pEffect->dwRange )
	{
		DisplayMessageAdd(g_Message[ETC_MESSAGE147].szMessage, 0xFFFFC309);	// MSG_ID : 147 ; 사정거리 안에 없습니다.
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);
		return FALSE;
	}
	
	int nMana = g_pMainPlayer->m_wMP - pEffect->Value[bSkillLevel].nMana;	
	if(nMana<0)
	{
		DisplayMessageAdd(g_Message[ETC_MESSAGE148].szMessage, 0xFFFFC309);		// MSG_ID : 148 ; 마나가 없습니다.
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);
		return FALSE;
	}
		
	if (g_dwCurTick - dwStartSkillTick < pEffect->dwCoolTime)
	{
		int nCool = int(g_pMainPlayer->m_fCurCoolPoint*1000-pEffect->dwCoolTime);
	
		if(nCool<0)
		{
			DisplayMessageAdd(g_Message[ETC_MESSAGE149].szMessage, 0xFFFFC309);	// MSG_ID : 149 ; 쿨게이지가 부족합니다.
			_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);
			return FALSE;
		}
		
		g_pMainPlayer->m_fCurCoolPoint = (float)max(nCool/1000.f, 0.1);		
	}
	
	return TRUE;
}

EffectDesc* EffectLayer::CreateMagicArray(BYTE bSkillKind, VECTOR3* vecStart, BOOL bOwn)
{
	Effect* pEffect = GetEffectInfo(bSkillKind);
	
	EffectDesc*	pEffectDesc = NULL;
	switch(pEffect->wProperty%100-pEffect->wProperty%10)
	{
	case 70:
	case 10:
		pEffectDesc = CreateGXObject(g_pObjManager->GetFile(SKILL_MAGICARRAY_FIRE), bOwn, __CHR_EFFECT_NONE__);
		SetAction(pEffectDesc->hEffect.pHandle, 1, 0, ACTION_ONCE);
		break;
	case 20:
		pEffectDesc = CreateGXObject(g_pObjManager->GetFile(SKILL_MAGICARRAY_WATER), bOwn, __CHR_EFFECT_NONE__);
		SetAction(pEffectDesc->hEffect.pHandle, 1, 0, ACTION_ONCE);
		break;
	case 30:
		pEffectDesc = CreateGXObject(g_pObjManager->GetFile(SKILL_MAGICARRAY_EARTH), bOwn, __CHR_EFFECT_NONE__);
		SetAction(pEffectDesc->hEffect.pHandle, 1, 0, ACTION_ONCE);
		break;
	case 40:
		pEffectDesc = CreateGXObject(g_pObjManager->GetFile(SKILL_MAGICARRAY_WIND), bOwn, __CHR_EFFECT_NONE__);
		SetAction(pEffectDesc->hEffect.pHandle, 1, 0, ACTION_ONCE);
		break;
	case 50:
		pEffectDesc = CreateGXObject(g_pObjManager->GetFile(SKILL_MAGICARRAY_SKY), bOwn, __CHR_EFFECT_NONE__);
		SetAction(pEffectDesc->hEffect.pHandle, 1, 0, ACTION_ONCE);
		break;
	case 60:
		pEffectDesc = CreateGXObject(g_pObjManager->GetFile(SKILL_AURARECHARGE), bOwn, __CHR_EFFECT_NONE__);
		SetAction(pEffectDesc->hEffect.pHandle, 1, 0, ACTION_LOOP);
		break;	
	}

	if (!pEffectDesc)
		return FALSE;

	pEffectDesc->hEffect.pDesc->ObjectFunc	= NULL;
	pEffectDesc->hEffect.pDesc->pInfo		= pEffectDesc;	
	
	GXSetPosition(pEffectDesc->hEffect.pHandle, vecStart, FALSE);	
	
	char szBase[255] = {0, };
	char szSoundFile[0xff]={0,};
	if (g_pSoundLib)
	{
		lstrcpy(szBase, GetFile(szBase, DATA_TYPE_SOUND));

		wsprintf(szSoundFile, "%s%d.wav", szBase, pEffect->wSoundNum[SOUND_EFFECT_CASTING1]);
		GetSoundEffectFile(&pEffectDesc->m_hSoundFile[SOUND_EFFECT_CASTING1], szSoundFile);
		GetSoundEffect(&pEffectDesc->m_pSound[SOUND_EFFECT_CASTING1], pEffectDesc->m_hSoundFile[SOUND_EFFECT_CASTING1],  pEffectDesc->vecOwn);
		PlaySoundEffect(pEffectDesc->m_pSound[SOUND_EFFECT_CASTING1], vecStart, TRUE);
	}

	return pEffectDesc;
}

void EffectLayer::AttachLight(EffectDesc* pEffectDesc, BYTE bLightNum, void(*LightFunc)(EffectDesc*))
{	
	pEffectDesc->m_sLightDescEx.m_handle = CreateLight((LIGHT_DESC*)&pEffectDesc->m_sLightDescEx, &pEffectDesc->vecBasePosion, bLightNum);		

	if (pEffectDesc->m_sLightDescEx.m_handle)
	{
		g_pExecutive->GXOAttachLight(pEffectDesc->hEffect.pHandle,"plane02",pEffectDesc->m_sLightDescEx.m_handle, NULL, ATTACH_TYPE_ATTACH);
		pEffectDesc->m_sLightDescEx.LightFunc = LightFunc;
	}

	_CHECK_MEMORY();
}

void EffectLayer::DetachLight(EffectDesc* pEffectDesc)
{
	g_pExecutive->GXODetachLight(pEffectDesc->hEffect.pHandle, pEffectDesc->m_sLightDescEx.m_handle);
	g_pExecutive->DeleteGXLight(pEffectDesc->m_sLightDescEx.m_handle);
	memset(&pEffectDesc->m_sLightDescEx, 0, sizeof(pEffectDesc->m_sLightDescEx));	
}
