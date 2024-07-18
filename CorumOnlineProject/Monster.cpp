//#include "GameSolution.h"
#ifdef SS3D_TEST
#include "./../SS3D_0719/4dyuchigx/4dyuchiGRX_common/IExecutive.h"
#else
#include "./../4dyuchiGRX_common/IExecutive.h"
#endif
#include "GameDefine.h"
#include "InitGame.h"
#include "GameControl.h"
#include "Monster.h"
#include "hashtable.h"
#include "userinterface.h"
#include "message.h"
#include "objectmanager.h"
#include "globalsoundHandles.h"
#include "EffectProcess.h"
#include "DungeonProcess.h"
#include "DamageNumber.h"
#include "chrinfo.h"
#include "Effect.h"
#include "Map.h"
#include "CorumResource.h"
#include "CodeFun.h"
#include "NetworkClient.h"

CMonster::CMonster()
{
	memset( this, 0, sizeof(CMonster) );
}

CMonster::~CMonster()
{

}
BOOL CMonster::IsNormalMonster()
{
	return m_dwMonsterKind == OBJECT_TYPE_MONSTER 
		&& !m_dwLordIndex
		&& !m_pEffectDesc[__SKILL_REDELEMENTAL__]
		&& !m_pEffectDesc[__SKILL_BLUEELEMENTAL__]
		&& !m_pEffectDesc[__SKILL_GREENELEMENTAL__];
}
void CMonster::SetDamageIndex( DWORD dwDamage )
{
	LPDAMAGE_NUMBER	pDamage;
	pDamage = AllocDamageNumber( &g_pDamageNumberPool[1] );
	if( !pDamage ) return;
	
	ClearDamageNumber( pDamage );

	int i=0, iStart = 0;
//	int iCnt = 0;
	VECTOR3	v3Pos;

	int	 iDamage[6] = {0,};
	char szDamage[6] = {0,};
	char szTmp[2] = {0,};
//	BOOL bFirstZero = FALSE;
//	if (dwDamage > 9999)
//		dwDamage = 9999;
	wsprintf( szDamage, "%05d", dwDamage );
	iStart = 4;
	for( i=0; i<5; i++ )
	{
		if( szDamage[i] != '0' ) 
		{
			iStart = i;
			break;
		}
	}
	
	MATRIX4 mtY;
	VECTOR3	v3CameraRad;
	int		iStartSpot = (5 - iStart)*23/2;

	g_pGeometry->GetCameraAngleRad( &v3CameraRad, 0 );
	SetRotationYMatrix( &mtY, (v3CameraRad.y)-3.141592f );
	
	szTmp[1] = '\0';
	for( i=iStart; i<5; i++ ) 
	{
		szTmp[0] = szDamage[i];
		iDamage[i] = atoi( szTmp );
		
		v3Pos.z	 = 0;
		v3Pos.y  = 0;
		v3Pos.x  = (float)( (5-i)*23 - iStartSpot );
		TransformVector3_VPTR1( &v3Pos, &mtY, 1 );
		v3Pos.x += m_v3CurPos.x;
		v3Pos.y += m_v3CurPos.y;
		v3Pos.z += m_v3CurPos.z;

		ShowObject( pDamage->m_hDamageNumber[ pDamage->m_pbUsedDamageNumber[iDamage[i]] ][iDamage[i]].pHandle );
		::SetAction( pDamage->m_hDamageNumber[ pDamage->m_pbUsedDamageNumber[iDamage[i]] ][iDamage[i]].pHandle, 1, 0, ACTION_ONCE );
		GXSetPosition( pDamage->m_hDamageNumber[ pDamage->m_pbUsedDamageNumber[iDamage[i]] ][iDamage[i]].pHandle, &v3Pos, FALSE );
		pDamage->m_hDamageNumber[ pDamage->m_pbUsedDamageNumber[iDamage[i]] ][iDamage[i]].pDesc->ObjectFunc = DamageNumberFunc;
		
		pDamage->m_hDamageNumber[ pDamage->m_pbUsedDamageNumber[iDamage[i]] ][iDamage[i]].pDesc->bType = OBJECT_TYPE_DAMAGE_MONSTER;
		pDamage->m_hDamageNumber[ pDamage->m_pbUsedDamageNumber[iDamage[i]] ][iDamage[i]].pDesc->pInfo = pDamage;

		pDamage->m_pbUsedDamageNumber[ iDamage[i] ]++;
	}
}

void CMonster::SetDamageChar( DWORD dwChar )
{
	LPDAMAGE_NUMBER	pDamage;
	pDamage = AllocDamageNumber( &g_pDamageNumberPool[1] );
	if( !pDamage ) return;
	
	ClearDamageNumber( pDamage );
	
	MATRIX4 mtY;
	VECTOR3	v3CameraRad, v3Pos;

	g_pGeometry->GetCameraAngleRad( &v3CameraRad, 0 );
	SetRotationYMatrix( &mtY, (v3CameraRad.y)-3.141592f );
	
	v3Pos.z	 = 0;
	v3Pos.y  = 0;
	v3Pos.x  = 0;//( (5-i)*23 - iStartSpot );
	TransformVector3_VPTR1( &v3Pos, &mtY, 1 );
	v3Pos.x += m_v3CurPos.x;
	v3Pos.y += m_v3CurPos.y;
	v3Pos.z += m_v3CurPos.z;

	ShowObject( pDamage->m_hDamageChar[dwChar].pHandle );
	::SetAction( pDamage->m_hDamageChar[dwChar].pHandle, 1, 0, ACTION_ONCE );
	GXSetPosition( pDamage->m_hDamageChar[dwChar].pHandle, &v3Pos, FALSE );
	pDamage->m_hDamageChar[dwChar].pDesc->ObjectFunc = DamageNumberFunc;
	pDamage->m_hDamageChar[dwChar].pDesc->bType = OBJECT_TYPE_DAMAGE_MONSTER;
	pDamage->m_hDamageChar[dwChar].pDesc->pInfo = pDamage;
}

//---------------------------------------------------------------------------//
// Get and Set Func
void CMonster::SetAction( WORD wAct, int nFrame /* = 0 */, BYTE bFlag /* = ACTION_LOOP  */)
{
	
	m_hMonster.pDesc->bActionFlag	= bFlag;
	GXOBJECT_HANDLE handle = m_hMonster.pHandle ;
	LPObjectDesc	pDesc = (LPObjectDesc)g_pExecutive->GetData( handle );

	if(!pDesc)	
	{	
		// MSG_ID : 280 ; Fail to ChangeAction!
		MessageBox(g_hMainWnd,g_Message[ETC_MESSAGE280].szMessage,NULL,NULL);	
		return;	
	}	
		
	DWORD			dwMotionIndex	= g_pExecutive->GXOGetCurrentMotionIndex( handle );
	DWORD			dwCurFrame = g_pExecutive->GXOGetCurrentFrame(handle);
	MOTION_DESC		MotionDesc;
	
	g_pExecutive->GXOGetMotionDesc( handle, &MotionDesc, dwMotionIndex, 0 );
	
	for(int i = (int)dwCurFrame; i < (int)MotionDesc.dwLastFrame; ++i)
	{
		if( !pDesc->ObjectFunc ) break;

		pDesc->nCurFrame = i;

		if (pDesc->nCurFrame == (int)MotionDesc.dwLastFrame)
		{
		
			if( pDesc->bActionFlag == ACTION_NEXT )
				(*pDesc->ObjectFunc)( handle, pDesc, pDesc->nCurFrame, FRAME_FLAG_CHANGE_NEXT );				
			else 
				(*pDesc->ObjectFunc)( handle, pDesc, pDesc->nCurFrame, FRAME_FLAG_FINISHED );
		}
		else
		{
			(*pDesc->ObjectFunc)( handle, pDesc, pDesc->nCurFrame, FRAME_FLAG_PLAYING );
		}	
	}	

	::SetAction(m_hMonster.pHandle, wAct, nFrame, bFlag );
}

void CMonster::SetActionNext( WORD wAct, WORD wNextAct, BYTE bNextFlag /* = ACTION_LOOP */, int nNextFrame /* = 0  */)
{		
	m_hMonster.pDesc->bActionFlag	= ACTION_NEXT;
	m_hMonster.pDesc->bNextFlag		= bNextFlag;
	m_hMonster.pDesc->wNextAct		= wNextAct;
	m_hMonster.pDesc->nNextFrame	= nNextFrame;
	GXOBJECT_HANDLE handle = m_hMonster.pHandle ;
	LPObjectDesc	pDesc = (LPObjectDesc)g_pExecutive->GetData( handle );

	DWORD			dwMotionIndex	= g_pExecutive->GXOGetCurrentMotionIndex( handle );
	DWORD			dwCurFrame = g_pExecutive->GXOGetCurrentFrame(handle);
	MOTION_DESC		MotionDesc;
	
	g_pExecutive->GXOGetMotionDesc( handle, &MotionDesc, dwMotionIndex, 0 );
					
	for(int i = (int)dwCurFrame; i < (int)MotionDesc.dwLastFrame; ++i)
	{
		if( !pDesc->ObjectFunc ) break;

		pDesc->nCurFrame = i;
		if (pDesc->nCurFrame == (int)MotionDesc.dwLastFrame)
		{
			if( pDesc->bActionFlag == ACTION_NEXT )
				(*pDesc->ObjectFunc)( handle, pDesc, pDesc->nCurFrame, FRAME_FLAG_CHANGE_NEXT );
			else 
				(*pDesc->ObjectFunc)( handle, pDesc, pDesc->nCurFrame, FRAME_FLAG_FINISHED );		
		}
		else
		{
			(*pDesc->ObjectFunc)( handle, pDesc, pDesc->nCurFrame, FRAME_FLAG_PLAYING );
		}
	}
	
	::SetActionNext(m_hMonster.pHandle, wAct, wNextAct, bNextFlag, nNextFrame);
}
//---------------------------------------------------------------------------//

BYTE CMonster::GetStatus()
{
	return m_bStatus;
}

void CMonster::SetStatus(BYTE bStatus, void (__cdecl *ObjectFunc)(GXOBJECT_HANDLE,OBJECT_DESC *,DWORD,BYTE))
{
//	EffectDesc* pEffectDesc = (EffectDesc*)m_hMonster.pDesc->pInfo;
	
	if (m_bStatus == UNIT_STATUS_DAMAGING)
	{
		m_dwTemp[MONSTER_TEMP_FRAMECOUNT]	= 0;
	}	

	m_bStatus = bStatus;
	m_hMonster.pDesc->ObjectFunc = ObjectFunc;
	WithActionFunc = NULL;
	if (bStatus == UNIT_STATUS_DEAD)
	{
		if( m_dwMonsterIndex == g_pMainPlayer->m_dwTemp[USER_TEMP_AUTO_TARGET_INDEX] )
		{
			g_pMainPlayer->m_dwTemp[USER_TEMP_AUTO_TARGET_INDEX] = 0;
			g_pMainPlayer->m_dwTemp[USER_TEMP_LASTATTACKTICK] = g_dwCurTick;
		}
	}
}

BOOL CMonster::UsingStatusSkill(BYTE bSkillKind)
{
	return m_pEffectDesc[bSkillKind] != NULL;	
}

void CMonster::AttachSkill(BYTE bSkillKind, BYTE bSkillLevel, DWORD dwEndTime)
{	
	Effect* pEffect = g_pEffectLayer->GetEffectInfo(bSkillKind);
	
	EffectDesc*	pEffectDesc = m_pEffectDesc[bSkillKind];
		
	if (!pEffectDesc)
	{// 어쭈? 새로 만들어야 하는군.
		if (bSkillKind == __SKILL_MINDCONTROL__)
			return;

		pEffectDesc = g_pEffectLayer->CreateStatusEffect(bSkillKind, 0, 0);
		
		pEffectDesc->dwCount = 1;
		pEffectDesc->byTargetObjectType[0] = OBJECT_TYPE_MONSTER;
		pEffectDesc->dwTargetIndex[0] = m_dwMonsterIndex;
					
		::SetAction( pEffectDesc->hEffect.pHandle, 1, 0, ACTION_LOOP );
		pEffectDesc->pUsingStatus = m_pUsingStatusEffectList->AddTail(pEffectDesc);
		m_pEffectDesc[bSkillKind] = pEffectDesc;
					
		switch(pEffectDesc->pEffect->GetEffectPosition())
		{
		case EFFECT_TYPE_STATUS_TOP:
			pEffectDesc->hEffect.pDesc->ObjectFunc = EffectSkillMonsterStatusTopFunc;
			break;
		case EFFECT_TYPE_STATUS_CENTER:
			pEffectDesc->hEffect.pDesc->ObjectFunc = EffectSkillMonsterStatusCenterFunc;	
			break;
		case EFFECT_TYPE_STATUS_BOTTOM:
			pEffectDesc->hEffect.pDesc->ObjectFunc = EffectSkillMonsterStatusBottomFunc;
			switch(pEffectDesc->pEffect->bID)
			{
			case __SKILL_REDELEMENTAL__:
			case __SKILL_BLUEELEMENTAL__:
			case __SKILL_GREENELEMENTAL__:
				::SetAction( pEffectDesc->hEffect.pHandle, 1, 0, ACTION_ONCE );
				break;
			}break;
		default:
			::SetAction( pEffectDesc->hEffect.pHandle, 1, 0, ACTION_ONCE_HIDE );
			pEffectDesc->hEffect.pDesc->ObjectFunc = EffectSkillMonsterStatusTopFunc;
		}
		pEffectDesc->dwTemp[SKILL_TEMP_EFFECT_TIME_1SEC] = g_dwCurTick;
		if (pEffect->bType == TYPE_DRIVE)
		{
			++m_bOverDriveCount;
			::SetAction(pEffectDesc->hEffect.pHandle, (WORD)min(m_bOverDriveCount, 3), 0, ACTION_LOOP );
		}
	}
//	m_sUseEffectDesc[bSkillKind].m_nRefCount++;
	pEffectDesc->dwTemp[SKILL_TEMP_EFFECT_TIME_FINISH] = 0;	// 상태 마법 끝나기 위한 플래그.
	ShowObject(pEffectDesc->hEffect.pHandle);
	pEffectDesc->dwSkillLevel = bSkillLevel;
	pEffectDesc->hEffect.pDesc->dwDestTime  = g_dwCurTick+dwEndTime;
	pEffectDesc->hEffect.pDesc->dwStartTick = g_dwCurTick;		
}

void CMonster::CreateResource()
{
	m_pEffectList = new COnlyList(50);
	m_pUsingStatusEffectList = new COnlyList(50);
	m_pDyingList = NULL;
	
	m_pSoundControl = new CSoundControl;
}

void CMonster::RemoveResource()
{
	RemoveAllStatusEffect();	

	if(m_pEffectList)
		delete m_pEffectList;
	m_pEffectList = NULL;	

	if(m_pUsingStatusEffectList)
		delete m_pUsingStatusEffectList;
	m_pUsingStatusEffectList = NULL;

	if (m_pDyingList)
		g_pDyingMonsterList->RemoveAt(m_pDyingList);
	m_pDyingList = NULL;

	// Sound Release
	m_pSoundControl->Release();
	delete m_pSoundControl;		// 함수로 뺄것이다.
}

void CMonster::RemoveAllStatusEffect()
{	
	m_pEffectList->RemoveAll();
	POSITION_ pos = m_pUsingStatusEffectList->GetHeadPosition();
	
	while( pos )
	{
		POSITION_ delPos = pos;
		EffectDesc* pEffectDesc = (EffectDesc*)m_pUsingStatusEffectList->GetNext(pos);		
		
		RemoveEffectDesc(pEffectDesc);
		m_pUsingStatusEffectList->RemoveAt(delPos);			
	}

	m_bOverDriveCount = 0;
	memset(m_pEffectDesc, 0, sizeof(m_pEffectDesc));
}

void CMonster::InitDie(CUser* pKiller, BOOL bOwn, DWORD dwPower, DWORD dwStartKillFrame)
{
//	OutputDebugString("InitDie\n");

	if (bOwn)
	{// 자기가 때려 죽였을때..
		CUserInterface* pUserInterface = CUserInterface::GetInstance();
		
		pUserInterface->m_bMonster = FALSE;

		pUserInterface->SetRender(SPR_OBJ_MONSTERHP_BAR1, FALSE);
		pUserInterface->SetRender(SPR_OBJ_MONSTERHP_BAR2, FALSE);
		pUserInterface->SetRender(SPR_OBJ_MONSTER_GAGE1, FALSE);
		pUserInterface->SetRender(SPR_OBJ_MONSTER_GAGE2, FALSE);
	}	
	
	g_pExecutive->GXOGetDirection(m_hMonster.pHandle, &m_vecTemp, &m_fDir);
	m_vecTemp = m_v3CurPos;
	m_dwTemp[MONSTER_TEMP_PUSHPOWER] = dwPower;
	m_dwTemp[MONSTER_TEMP_PUSHPOWER1]	= 70;
	m_dwTemp[MONSTER_TEMP_FRAMECOUNT]	= 0;
	m_dwTemp[MONSTER_TEMP_FINISHDYINGFRAME] = dwStartKillFrame+10;
	if (pKiller)
		m_dwTemp[MONSTER_TEMP_KILLERINDEX]	= pKiller->m_dwUserIndex;
	
	SetStatus(UNIT_STATUS_DEAD, NULL);
	g_pMap->SetTileOccupied( m_pCurTile->wIndex_X, m_pCurTile->wIndex_Z, TILE_EMPTY, this );
	g_pExecutive->DisablePick( m_hMonster.pHandle );	// 더이상 픽킹이 되지 않도록 만든다.	
	if (!m_pDyingList)
		m_pDyingList = g_pDyingMonsterList->AddTail((void*)m_dwMonsterIndex);
	
	if( g_pMainPlayer->m_dwUserIndex == m_dwLordIndex )
		__asm nop;
}
void CMonster::ModelScaleByLevel()
{
	float fAlphaScale = 0.f;
	// 마지막으로 가디언이라면 주인 유저에게 연결해준다.( 지금은 일단 메인 케릭터에만. )
	if( m_dwMonsterKind == OBJECT_TYPE_GUARDIAN
		&& m_dwLordIndex )
	{
		fAlphaScale = (m_dwLevel*0.25f)/10.f;
	}

	// 스케일 조정.
	VECTOR3 v3Scale;
	
	v3Scale.x	= (float)(m_byBaseScale+fAlphaScale) / 10.f;
	v3Scale.y	= (float)(m_byBaseScale+fAlphaScale) / 10.f;
	v3Scale.z	= (float)(m_byBaseScale+fAlphaScale) / 10.f;

	g_pExecutive->GXOSetScale( m_hMonster.pHandle, &v3Scale );
	
}

// 사운드 파일 핸들만 날린다.
// 이팩트 헨들은 쓰레기통에서 관리한다.
void CMonster::ReleaseSoundHandle()
{
	// sound file handle release 
	for (int i = 0; i < 10; i++)
	{
//		SAFE_REL_SOUND_FILE( m_pSound[i] );
		SAFE_REL_SOUND_FILE( m_hSoundFile[i] );
	}
}

void CMonster::SetActionSkill(BYTE bSkillKind)
{
	// 인자값은 현재 무시된다.
	// 이유라면, 몬스터는 어떤 스킬을 쓰던간에 동작이 1개이기 때문이렷다.	
	switch(bSkillKind)
	{
	case __SKILL_DRAGONICFIREBLAST__:
		SetAction(MON_MOTION_TYPE_ATTACK4, 0, ACTION_ONCE);	
		break;
	case __SKILL_DRAGONICVIBRATION__:
		SetAction(MON_MOTION_TYPE_ATTACK3, 0, ACTION_ONCE);	
		break;
	default:
		SetAction(MON_MOTION_TYPE_ATTACK3, 0, ACTION_ONCE);	
	}	
}

BYTE CMonster::GetSkillLevel(const BYTE bySkillKind) const
{
	for(int i = 0; i < MAX_GUARDIAN_USE_SKILL; ++i)
	{
		if (m_Skill[i].wSkill == bySkillKind)
		{		
			return m_Skill[i].bSkillLevel;
		}
	}
	
	return 0;
}

void CMonster::SetSkillLevel(const BYTE bySkillKind, const BYTE bySkillLevel)
{
	int nIndex = 0;
	for(int i = MAX_GUARDIAN_USE_SKILL; i >= 0; --i)
	{
		// 이미 배웟다면 그곳에 들어가기.
		if (m_Skill[i].wSkill == bySkillKind)
		{
			nIndex = i;
			break;
		}
		
		// 들어 갈수 있는 자리 알아내기.
		if (m_Skill[i].wSkill == 0)
		{
			nIndex = i;
		}
	}

	m_Skill[nIndex].wSkill = bySkillKind;
	m_Skill[nIndex].bSkillLevel = bySkillLevel;
}

BYTE CMonster::GetSelectedSkill() const
{
	return m_bSelectedSkill;
}

void CMonster::SetSelectedSkill(BYTE bSkill)
{
	if(GetSkillLevel(bSkill) > 0)
		m_bSelectedSkill = bSkill;
	else
		m_bSelectedSkill = 0;

	CTDS_GUARDIAN_SKILL_SELECT packet;
	packet.bSkill = bSkill;
	g_pNet->SendMsg((char*)&packet, packet.GetPacketSize(), SERVER_INDEX_ZONE);
}