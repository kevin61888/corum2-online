// GuardianWnd.cpp: implementation of the CGuardianWnd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GuardianWnd.h"
#include "GameControl.h"
#include "User.h"
#include "UserInterface.h"
#include "Interface.h"
#include "message.h"
#include "InitGame.h"
#include "NetworkClient.h"
#include "CodeFun.h"

CGuardianWnd* CGuardianWnd::c_pThis = NULL;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGuardianWnd::CGuardianWnd()
{
	for(int i = 0; i < 5; i++)
		m_bBtn[i] = FALSE;
}

CGuardianWnd::~CGuardianWnd()
{

}


//======================================================//
// Class Member Function.								//
//======================================================//
BOOL CGuardianWnd::Init()
{
	m_bInit = TRUE;
	return TRUE;
}

void CGuardianWnd::Remove()
{
	RemoveAllData();
	m_bActive	= FALSE;
	m_bInit		= FALSE;
}

void CGuardianWnd::SetOrder()
{
}

void CGuardianWnd::RenderText()
{
	GuardianDisplay();
}

void CGuardianWnd::SetActive(BOOL bActive)
{
	m_bActive = bActive;		
	
	if(bActive==FALSE)
	{
		m_bSoundChk	= FALSE;
		ShowSpriteAll();
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDCLOSE, g_v3InterfaceSoundPos, FALSE);
	}
	else
	{
		CInterface::GetInstance()->SetWnd((BYTE)m_nIndex);

		SetRender(SPR_OBJ_GUARDIAN_WINDOWS1, TRUE);
		SetRender(SPR_OBJ_GUARDIAN_WINDOWS2, TRUE);
		SetRender(SPR_OBJ_GUARDIAN_CLOSE1, TRUE);
				
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDOPEN, g_v3InterfaceSoundPos, FALSE);
		ShowAllStatIncreseButton(FALSE);
	}
}

void CGuardianWnd::SpriteStatButtonEvent(GUARDIAN_STATUS_POINT_KIND enStatusPointKind, MOUSECHECK enMouseCheck)
{
	switch (enMouseCheck)
	{
	case MOUSE_DOWN:
		SetRender(BUTTON_OBJ_G_EGO_UP1+enStatusPointKind*2, FALSE);
		SetRender(BUTTON_OBJ_G_EGO_UP2+enStatusPointKind*2, TRUE);
		m_bBtn[enStatusPointKind] = TRUE;
		
		break;
	case MOUSE_UP:
		SetRender(BUTTON_OBJ_G_EGO_UP1+enStatusPointKind*2, TRUE);
		SetRender(BUTTON_OBJ_G_EGO_UP2+enStatusPointKind*2, FALSE);
		m_bBtn[enStatusPointKind] = FALSE;
		
		break;
	case MOUSE_OVER:
		
		break;
	}
}

void CGuardianWnd::OnStatButtonEvent(GUARDIAN_STATUS_POINT_KIND enStatusPointKind, MOUSECHECK enMouseCheck)
{
	if (!IsStatPoint())
		return;

	SpriteStatButtonEvent(enStatusPointKind, enMouseCheck);
	if (MOUSE_UP == enMouseCheck)
	{
		Send_GuardianStatPoint(enStatusPointKind);
	}
	
}
int CGuardianWnd::CheckInterface()
{
	m_bMouseIcon = FALSE;

	int nRt = GetChk();
	
	for(int i = GUARDIAN_STATUS_POINT_KIND_EGO; i < GUARDIAN_STATUS_POINT_KIND_MAX; ++i)
	{
		if (m_bBtn[i])
			SpriteStatButtonEvent((GUARDIAN_STATUS_POINT_KIND)i, MOUSE_UP);
	}
	InitValue(nRt);
	
	MOUSECHECK eMouseCheck = GetMouseCheck(nRt);
	
	switch(nRt)
	{	
	case 1:
		{			
			SetMouseCheck(nRt, eMouseCheck);			
		}
		break;
	case 2:
		{
			if(eMouseCheck==MOUSE_UP)
				SetActive(FALSE);				
			else if(eMouseCheck==MOUSE_DOWN)
			{
				SetRender(SPR_OBJ_GUARDIAN_CLOSE1, FALSE);
				SetRender(SPR_OBJ_GUARDIAN_CLOSE2, TRUE);
			}						
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 3:			
		{
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;	
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
		{
			BYTE byType = 0;
			OnStatButtonEvent(GUARDIAN_STATUS_POINT_KIND(nRt-4), eMouseCheck);

			SetMouseCheck(nRt, eMouseCheck, byType);
		}
		break;
	}		
	
	if(MouseUp())
	{
		SetRender(SPR_OBJ_GUARDIAN_CLOSE1, TRUE);
		SetRender(SPR_OBJ_GUARDIAN_CLOSE2, FALSE);		
	}

	MouseChk(nRt);
	
	if(SetMoveWnd(nRt))	
		return CInterface::GetInstance()->GetSetWnd();
	
	if(IsReturn(nRt, eMouseCheck))
		return CInterface::GetInstance()->GetSetWnd();

	return 0;
}

BOOL CGuardianWnd::IsStatPoint()
{
	CMonster* pGuardian = GetMyGuardian();
	if (pGuardian)
		return pGuardian->m_wStatPoint;

	return 0;
}

CMonster* CGuardianWnd::GetMyGuardian()
{
	return g_pMainPlayer->m_pGuardian[ 0 ];
}

void CGuardianWnd::GuardianDisplay()
{
	CMonster* pGuardian = GetMyGuardian();
	if (!pGuardian)
		return;

	int nSize	= 0;
	int	nLeft	= 0;
	int nRight	= 0;
	int nOrder	= 0;

#if IS_JAPAN_LOCALIZING()
		nLeft	= (LONG)m_fPosX+50;
#else
		nLeft	= (LONG)m_fPosX+55;
#endif

	nOrder = GetStartOrder() + 2;

	if(__strcmp(pGuardian->m_szName, "")!=0)
		RenderFont(pGuardian->m_szName, (int)(nLeft), (int)(m_fPosX+55+lstrlen(pGuardian->m_szName) * GUARDIAN_INFOTEXT_CHAR_WIDTH), (int)(m_fPosZ+30), (int)(m_fPosZ+44), nOrder);

	// 직업, 종족, 길드, 기본스탯 (HP, SP, LEVEL, EXP)
	nRight = (int)m_fPosX + GUARDIAN_INFOTEXT_TOP_XPOS_RIGHT;

	// Hp //
	wsprintf(m_szInfo, "%d / %d", pGuardian->m_dwHP, pGuardian->m_dwMaxHP);
	RenderFont(m_szInfo, (int)(nLeft), (int)(nRight), (int)(m_fPosZ+96), (int)(m_fPosZ+110), nOrder);
	
	// Level //
	wsprintf(m_szInfo, g_Message[ETC_MESSAGE863].szMessage, pGuardian->m_dwLevel); // "Lv %d"
	RenderFont(m_szInfo, (int)(nLeft), (int)(nRight), (int)(m_fPosZ+46), (int)(m_fPosZ+60), nOrder);
	
	// Exp //
	// modified by minjin. 2004. 10. 28.
	wsprintf(m_szInfo, "%d / %d", pGuardian->m_dwExp, GetExpTableOfLevel(OBJECT_TYPE_MONSTER, GetGuardianItemLevel(pGuardian->m_dwExp)+1));
	RenderFont(m_szInfo, (int)(nLeft), (int)(nRight), (int)(m_fPosZ+61), (int)(m_fPosZ+75), nOrder);

	// 가디언 수명
	CItem*	pItem = &g_pMainPlayer->m_pInv_Guardian[0];
	char szItemInfo[50];
	ZeroMemory(szItemInfo, sizeof(szItemInfo));
	struct tm *when;
	DWORD dwDueDay = pItem->m_Item_Guardian.dwEndLife;
	when = localtime((time_t*)&dwDueDay);
	if(when)
	{
		wsprintf(szItemInfo, "%04d/%02d/%02d %02d:%02d", when->tm_year+1900, when->tm_mon+1 ,when->tm_mday, when->tm_hour, when->tm_min);
		RenderFont(szItemInfo, (int)(nLeft), (int)(nRight), (int)(m_fPosZ+76), (int)(m_fPosZ+90), nOrder);
	}
	
	// EGO, STR, INT, DEX, VIT
	nRight	= (int)m_fPosX + GUARDIAN_INFOTEXT_BASIC_STAT_XPOS_RIGHT;
	nLeft	= (int)m_fPosX + GUARDIAN_INFOTEXT_BASIC_STAT_XPOS_LEFT;

	// EGO //		
	nSize = wsprintf(m_szInfo, "%d", pGuardian->m_wEgo);		
	RenderFont(m_szInfo, (int)(GUARDIAN_THIS_TEXT_LEFT(nLeft, nSize)), (int)(nRight), (int)(m_fPosZ+132), (int)(m_fPosZ+146), nOrder);

	// STR //
	nSize = wsprintf(m_szInfo, "%d", pGuardian->m_wStr);
	RenderFont(m_szInfo, (int)(GUARDIAN_THIS_TEXT_LEFT(nLeft, nSize)), (int)(nRight), (int)(m_fPosZ+147), (int)(m_fPosZ+162), nOrder);	
	
	// INT //	
	nSize = wsprintf(m_szInfo, "%d", pGuardian->m_wInt);	
	RenderFont(m_szInfo, (int)(GUARDIAN_THIS_TEXT_LEFT(nLeft, nSize)), (int)(nRight), (int)(m_fPosZ+162), (int)(m_fPosZ+176), nOrder);	
	
	// Dex //	
	nSize = wsprintf(m_szInfo, "%d", pGuardian->m_wDex);	
	RenderFont(m_szInfo, (int)(GUARDIAN_THIS_TEXT_LEFT(nLeft, nSize)), (int)(nRight), (int)(m_fPosZ+177), (int)(m_fPosZ+191), nOrder);	
	
	// VIT //
	nSize = wsprintf(m_szInfo, "%d", pGuardian->m_wVit);	
	RenderFont(m_szInfo, (int)(GUARDIAN_THIS_TEXT_LEFT(nLeft, nSize)), (int)(nRight), (int)(m_fPosZ+192), (int)(m_fPosZ+206), nOrder);	

	
	// 저항력 스탯 (Physical, Flame, Ice, Electric, Poison)
	nRight	= (int)m_fPosX + GUARDIAN_INFOTEXT_RESIST_STAT_XPOS_RIGHT;
	nLeft	= (int)m_fPosX + GUARDIAN_INFOTEXT_RESIST_STAT_XPOS_LEFT;


	// Physical //	
	nSize = wsprintf(m_szInfo, "%d%%", pGuardian->m_wPhyResist);	
	RenderFont(m_szInfo, (int)(GUARDIAN_THIS_TEXT_LEFT(nLeft, nSize)), (int)(nRight), (int)(m_fPosZ+132), (int)(m_fPosZ+146), nOrder);	

	// Flame //	
	nSize = wsprintf(m_szInfo, "%d%%", pGuardian->m_wFireResist);	
	RenderFont(m_szInfo, (int)(GUARDIAN_THIS_TEXT_LEFT(nLeft, nSize)), (int)(nRight), (int)(m_fPosZ+147), (int)(m_fPosZ+162), nOrder);
	
	// Ice //	
	nSize = wsprintf(m_szInfo, "%d%%", pGuardian->m_wIceResist);	
	RenderFont(m_szInfo, (int)(GUARDIAN_THIS_TEXT_LEFT(nLeft, nSize)), (int)(nRight), (int)(m_fPosZ+162), (int)(m_fPosZ+176), nOrder);	
	
	// Electric	//	
	nSize = wsprintf(m_szInfo, "%d%%", pGuardian->m_wLightResist);	
	RenderFont(m_szInfo, (int)(GUARDIAN_THIS_TEXT_LEFT(nLeft, nSize)), (int)(nRight), (int)(m_fPosZ+177), (int)(m_fPosZ+191), nOrder);	

	// Poison //	
	nSize = wsprintf(m_szInfo, "%d%%", pGuardian->m_wPoiResist);	
	RenderFont(m_szInfo, (int)(GUARDIAN_THIS_TEXT_LEFT(nLeft, nSize)), (int)(nRight), (int)(m_fPosZ+192), (int)(m_fPosZ+206), nOrder);	

	// Status Point //
	if(pGuardian->m_wStatPoint>0)
	{
		nSize = wsprintf(m_szInfo, "%d", pGuardian->m_wStatPoint);

		switch(nSize)
		{
		case 0:
			nLeft = (int)m_fPosX + 142 - 11;
			break;
		case 1:
			nLeft = (int)m_fPosX + 142 - 14;
			break;
		case 2:
			nLeft = (int)m_fPosX + 142 - 18;
			break;
		case 3:
			nLeft = (int)m_fPosX + 142 - 21;
			break;
		default:
			nLeft = (int)m_fPosX + 142 - 21;
			break;
		}

		RenderFont(m_szInfo, (int)(nLeft), (int)(m_fPosX + 150), (int)(m_fPosZ + 161), (int)(m_fPosZ + 175), nOrder);

		ShowAllStatIncreseButton(TRUE);
	}
	
	
	// 공격 정확도 //	
	nSize = wsprintf(m_szInfo, "%d", pGuardian->m_wAttackAcuracy);	
	RenderFont(m_szInfo, (int)(m_fPosX + 124 - nSize * GUARDIAN_INFOTEXT_CHAR_WIDTH), (int)(m_fPosX+125), (int)(m_fPosZ+211), (int)(m_fPosZ+225), nOrder);	

	// 회피력 //	
	nSize = wsprintf(m_szInfo, "%d", pGuardian->m_wAvoid);	
	RenderFont(m_szInfo, (int)(m_fPosX + 248 - nSize * GUARDIAN_INFOTEXT_CHAR_WIDTH), (int)(m_fPosX+250), (int)(m_fPosZ+211), (int)(m_fPosZ+225), nOrder);	

	// L 공격력 //
	wsprintf(m_szInfo, g_Message[ETC_MESSAGE861].szMessage, pGuardian->m_pwAttackDamage_L[0], pGuardian->m_pwAttackDamage_L[1]); // "%d ~ %d"
	RenderFont(m_szInfo, (int)(m_fPosX + 85), (int)(m_fPosX + 240), (int)(m_fPosZ + 246), (int)(m_fPosZ + 270), nOrder);
}
void CGuardianWnd::HideAllStatButton()
{
	SetRender(BUTTON_OBJ_G_EGO_UP1, FALSE);
	SetRender(BUTTON_OBJ_G_STR_UP1, FALSE);
	SetRender(BUTTON_OBJ_G_INT_UP1, FALSE);
	SetRender(BUTTON_OBJ_G_DEX_UP1, FALSE);
	SetRender(BUTTON_OBJ_G_VIT_UP1, FALSE);		
	SetRender(BUTTON_OBJ_G_EGO_UP2, FALSE);
	SetRender(BUTTON_OBJ_G_STR_UP2, FALSE);
	SetRender(BUTTON_OBJ_G_INT_UP2, FALSE);
	SetRender(BUTTON_OBJ_G_DEX_UP2, FALSE);
	SetRender(BUTTON_OBJ_G_VIT_UP2, FALSE);
}
void CGuardianWnd::ShowAllStatIncreseButton(BOOL bShow)
{
	SetRender(BUTTON_OBJ_G_EGO_UP1, bShow);
	SetRender(BUTTON_OBJ_G_STR_UP1, bShow);
	SetRender(BUTTON_OBJ_G_INT_UP1, bShow);
	SetRender(BUTTON_OBJ_G_DEX_UP1, bShow);
	SetRender(BUTTON_OBJ_G_VIT_UP1, bShow);
}

void CGuardianWnd::SendStatPoint(GAME_OBJECT_TYPE enObjectType, GUARDIAN_STATUS_POINT_KIND enStatusPointKind)
{
	CTDS_CHAR_LEVELUP pPacket;				
	pPacket.byObjectType		= (BYTE)enObjectType;				
	pPacket.byStatusType		= (BYTE)enStatusPointKind;
	m_bBtn[enStatusPointKind]	= FALSE;
	
	g_pNet->SendMsg((char*)&pPacket, sizeof(CTDS_CHAR_LEVELUP), SERVER_INDEX_ZONE);					
}

void CGuardianWnd::Send_GuardianStatPoint(GUARDIAN_STATUS_POINT_KIND enStatusPointKind)
{
	CMonster* pGuardian = GetMyGuardian();
	if (!pGuardian)
		return;

	if(pGuardian->m_wStatPoint>0)
	{
		SendStatPoint(OBJECT_TYPE_MONSTER, enStatusPointKind);
	}
}
//======================================================//
// End.													//
//======================================================//