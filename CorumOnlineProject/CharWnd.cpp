//======================================================//
// Code by Jang.							2002.12.23	//
//======================================================//
#include "CharWnd.h"
#include "GameControl.h"
#include "User.h"
#include "UserInterface.h"
#include "Interface.h"
#include "message.h"
#include "InitGame.h"
#include "NetworkClient.h"
#include "CodeFun.h"

CCharWnd* CCharWnd::c_pThis = NULL;

//======================================================//
// Construction/Destrution.								//
//======================================================//
CCharWnd::CCharWnd()
{	
	for(int i = 0; i < 5; i++)
		m_bBtn[i] = FALSE;
}

CCharWnd::~CCharWnd()
{
}
//======================================================//
// Class Member Function.								//
//======================================================//
BOOL CCharWnd::Init()
{
/*
	InsertData(SPR_OBJ_CHAR_WINDOWS1, SPR_INTERFACE_CHAR_WINDOWS1, 0, 0, 1.0f, 1.0f, 0);
	InsertData(SPR_OBJ_CHAR_WINDOWS2, SPR_INTERFACE_CHAR_WINDOWS2, 0, 256, 1.0f, 1.0f, 0);
	InsertData(SPR_OBJ_CHAR_CLOSE1, SPR_INTERFACE_CLOSE1, 242, 4, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_CHAR_CLOSE2, SPR_INTERFACE_CLOSE2, 242, 4, 1.0f, 1.0f, 1);			
	InsertData(BUTTON_OBJ_EGO_UP1, BUTTON_INTERFACE_UP1, 95, 164, 1.0f, 1.0f, 1);
	InsertData(BUTTON_OBJ_EGO_UP2, BUTTON_INTERFACE_UP2, 95, 164, 1.0f, 1.0f, 1);
	InsertData(BUTTON_OBJ_STR_UP1, BUTTON_INTERFACE_UP1, 95, 179, 1.0f, 1.0f, 1);
	InsertData(BUTTON_OBJ_STR_UP2, BUTTON_INTERFACE_UP2, 95, 179, 1.0f, 1.0f, 1);
	InsertData(BUTTON_OBJ_INT_UP1, BUTTON_INTERFACE_UP1, 95, 194, 1.0f, 1.0f, 1);
	InsertData(BUTTON_OBJ_INT_UP2, BUTTON_INTERFACE_UP2, 95, 194, 1.0f, 1.0f, 1);
	InsertData(BUTTON_OBJ_DEX_UP1, BUTTON_INTERFACE_UP1, 95, 209, 1.0f, 1.0f, 1);
	InsertData(BUTTON_OBJ_DEX_UP2, BUTTON_INTERFACE_UP2, 95, 209, 1.0f, 1.0f, 1);
	InsertData(BUTTON_OBJ_VIT_UP1, BUTTON_INTERFACE_UP1, 95, 224, 1.0f, 1.0f, 1);
	InsertData(BUTTON_OBJ_VIT_UP2, BUTTON_INTERFACE_UP2, 95, 224, 1.0f, 1.0f, 1);

	InsertCheckInterface(242, 4, 256, 16, 2, CHECK_CLOSE);
	InsertCheckInterface(0, 0, 242, 20, 3, CHECK_MOVEWND);
	InsertCheckInterface(95, 164, 103, 172, 4, CHECK_BTN);
	InsertCheckInterface(95, 179, 103, 187, 5, CHECK_BTN);
	InsertCheckInterface(95, 194, 103, 202, 6, CHECK_BTN);
	InsertCheckInterface(95, 209, 103, 217, 7, CHECK_BTN);
	InsertCheckInterface(95, 223, 103, 232, 8, CHECK_BTN);
		*/
	m_bInit = TRUE;
	m_enCurrentTabType = ENUM_TAB_TYPE_CHARACTERINFO;
	return TRUE;
}

void CCharWnd::Remove()
{
	RemoveAllData();
	m_bActive	= FALSE;
	m_bInit		= FALSE;
}

void CCharWnd::SetOrder()
{
}

void CCharWnd::RenderText()
{

// Text Position Adjustment Code Refactored
// Codes had unnecessary repetitions...
// 2004. 08. 19.
// Begin - minjin

	(this->*m_fnDisplay)();
	
// End - minjin

/*
	int nLen	= 0;
	int nSize	= 0;
	int	nLeft	= 0;
	int nRight	= 0;
	
	// 캐릭터 이름 //	
	if(!IsEmptyString(g_pMainPlayer->m_szName))
	{		
#if IS_JAPAN_LOCALIZING()
		nLeft	= (LONG)m_fPosX+50;
#else
		nLeft	= (LONG)m_fPosX+55;
#endif
		
		RenderFont(g_pMainPlayer->m_szName, nLeft, (int)m_fPosX+55+g_pMainPlayer->m_nCharNameSize*7, (int)m_fPosZ+31, (int)m_fPosZ+45, GetStartOrder()+2);		
	}

	// 직업 //
#if IS_JAPAN_LOCALIZING()
	nLeft	= (LONG)m_fPosX+50;
#else
	nLeft	= (LONG)m_fPosX+55;
#endif	
	
	if(!IsEmptyString(g_pMainPlayer->m_szClassName))
		RenderFont(g_pMainPlayer->m_szClassName, nLeft, (int)m_fPosX+306, (int)m_fPosZ+47, (int)m_fPosZ+61, GetStartOrder()+2);
	
	// 종족 //
#if IS_JAPAN_LOCALIZING()
	nLeft	= (LONG)m_fPosX+50;
#else
	nLeft	= (LONG)m_fPosX+55;
#endif
	
	if(!IsEmptyString(g_pMainPlayer->m_szGroupName))
		RenderFont(g_pMainPlayer->m_szGroupName, nLeft, (int)m_fPosX+306, (int)m_fPosZ+62, (int)m_fPosZ+76, GetStartOrder()+2);

	// 길드 //	
	if(!IsEmptyString(g_pMainPlayer->m_szGuildName))
	{
#if IS_JAPAN_LOCALIZING()
		nLeft	= (LONG)m_fPosX+50;
#else
		nLeft	= (LONG)m_fPosX+55;
#endif
		
		RenderFont(g_pMainPlayer->m_szGuildName, nLeft, (int)m_fPosX+306, (int)m_fPosZ+77, (int)m_fPosZ+91, GetStartOrder()+2);
	}
	
	// Hp //
#if IS_JAPAN_LOCALIZING()
	nLeft	= (LONG)m_fPosX+50;
#else
	nLeft	= (LONG)m_fPosX+55;
#endif
	
	wsprintf(m_szInfo, "%d / %d", g_pMainPlayer->m_wHP, g_pMainPlayer->m_wMaxHP);
	RenderFont(m_szInfo, nLeft, (int)m_fPosX+306, (int)m_fPosZ+97, (int)m_fPosZ+111, GetStartOrder()+2);
	
	// Sp //
#if IS_JAPAN_LOCALIZING()
	nLeft	= (LONG)m_fPosX+50;
#else
	nLeft	= (LONG)m_fPosX+55;
#endif
	
	wsprintf(m_szInfo, "%d / %d", g_pMainPlayer->m_wMP, g_pMainPlayer->m_wMaxMP);
	RenderFont(m_szInfo, nLeft, (int)m_fPosX+306, (int)m_fPosZ+112, (int)m_fPosZ+126, GetStartOrder()+2);
	
	// Level //
#if IS_JAPAN_LOCALIZING()
	nLeft	= (LONG)m_fPosX+50;
#else
	nLeft	= (LONG)m_fPosX+55;
#endif
	
	wsprintf(m_szInfo, g_Message[ETC_MESSAGE863].szMessage, g_pMainPlayer->m_dwLevel); // "Lv %d"
	RenderFont(m_szInfo, nLeft, (int)m_fPosX+306, (int)m_fPosZ+127, (int)m_fPosZ+141, GetStartOrder()+2);
	
	// Exp //
#if IS_JAPAN_LOCALIZING()
	nLeft	= (LONG)m_fPosX+50;
#else
	nLeft	= (LONG)m_fPosX+55;
#endif
	
	wsprintf(m_szInfo, "%d / %d", g_pMainPlayer->m_dwExp, g_pMainPlayer->m_dwNextExp);
	RenderFont(m_szInfo, nLeft, (int)m_fPosX+306, (int)m_fPosZ+142, (int)m_fPosZ+156, GetStartOrder()+2);
	
	// EGO //		
	nSize = wsprintf(m_szInfo, "%d", g_pMainPlayer->m_dwEgo);		
	RenderFont(m_szInfo, m_fPosX+94-nSize*7, m_fPosX+318, m_fPosZ+163, m_fPosZ+177, GetStartOrder()+2);

	// STR //
	nSize = wsprintf(m_szInfo, "%d", g_pMainPlayer->m_dwStr);
	RenderFont(m_szInfo, m_fPosX+94-nSize*7, m_fPosX+318, m_fPosZ+178, m_fPosZ+192, GetStartOrder()+2);	
	
	// INT //	
	nSize = wsprintf(m_szInfo, "%d", g_pMainPlayer->m_dwInt);	
	RenderFont(m_szInfo, m_fPosX+94-nSize*7, m_fPosX+318, m_fPosZ+193, m_fPosZ+207, GetStartOrder()+2);	
	
	// Dex //	
	nSize = wsprintf(m_szInfo, "%d", g_pMainPlayer->m_dwDex);	
	RenderFont(m_szInfo, m_fPosX+94-nSize*7, m_fPosX+318, m_fPosZ+208, m_fPosZ+222, GetStartOrder()+2);	
	
	// VIT //
	nSize = wsprintf(m_szInfo, "%d", g_pMainPlayer->m_dwVit);	
	RenderFont(m_szInfo, m_fPosX+94-nSize*7, m_fPosX+318, m_fPosZ+223, m_fPosZ+237, GetStartOrder()+2);	

	// Physical //	
	nSize = wsprintf(m_szInfo, "%d%%", g_pMainPlayer->GetPhyResist());	
	RenderFont(m_szInfo, m_fPosX+248-nSize*7, m_fPosX+306, m_fPosZ+163, m_fPosZ+177, GetStartOrder()+2);	

	// Flame //	
	nSize = wsprintf(m_szInfo, "%d%%", g_pMainPlayer->m_wFireResist);	
	RenderFont(m_szInfo, m_fPosX+248-nSize*7, m_fPosX+306, m_fPosZ+178, m_fPosZ+192, GetStartOrder()+2);	
	
	// Ice //	
	nSize = wsprintf(m_szInfo, "%d%%", g_pMainPlayer->m_wIceResist);	
	RenderFont(m_szInfo, m_fPosX+248-nSize*7, m_fPosX+306, m_fPosZ+193, m_fPosZ+207, GetStartOrder()+2);	
	
	// Electric	//	
	nSize = wsprintf(m_szInfo, "%d%%", g_pMainPlayer->m_wLightResist);	
	RenderFont(m_szInfo, m_fPosX+248-nSize*7, m_fPosX+306, m_fPosZ+208, m_fPosZ+222, GetStartOrder()+2);

	// Poison //	
	nSize = wsprintf(m_szInfo, "%d%%", g_pMainPlayer->m_wPoiResist);	
	RenderFont(m_szInfo, m_fPosX+248-nSize*7, m_fPosX+306, m_fPosZ+223, m_fPosZ+237, GetStartOrder()+2);

	// Status Point //
	if(g_pMainPlayer->m_wPoint>0)
	{
		nSize = wsprintf(m_szInfo, "%d", g_pMainPlayer->m_wPoint);

		switch(nSize)
		{
		case 0:
			nLeft = m_fPosX+142-11;
			break;
		case 1:
			nLeft = m_fPosX+142-14;
			break;
		case 2:
			nLeft = m_fPosX+142-18;
			break;
		case 3:
			nLeft = m_fPosX+142-21;
			break;
		default:
			nLeft = m_fPosX+142-21;
			break;
		}			
		RenderFont(m_szInfo, nLeft, (int)m_fPosX+150, (int)m_fPosZ+191, (int)m_fPosZ+205, GetStartOrder()+2);
	}
	
	// 공격 정확도 //	
	nSize = wsprintf(m_szInfo, "%d", g_pMainPlayer->m_wAttackAcuracy);	
	RenderFont(m_szInfo, m_fPosX+124-nSize*7, m_fPosX+125, m_fPosZ+243, m_fPosZ+257, GetStartOrder()+2);	

	// 회피력 //	
	nSize = wsprintf(m_szInfo, "%d", g_pMainPlayer->m_wAvoid);	
	RenderFont(m_szInfo, m_fPosX+248-nSize*7, m_fPosX+250, m_fPosZ+243, m_fPosZ+257, GetStartOrder()+2);	

	// R 공격력 //	
	WORD wAttackDamage[2] = {0,};
	g_pMainPlayer->GetAttackDamage_R(wAttackDamage[0], wAttackDamage[1]);
	
	wsprintf(m_szInfo, g_Message[ETC_MESSAGE861].szMessage, wAttackDamage[0], wAttackDamage[1]); // "%d ~ %d"
	RenderFont(m_szInfo, m_fPosX+85, m_fPosX+230, m_fPosZ+259, m_fPosZ+273, GetStartOrder()+2);

	// L 공격력 //		
	g_pMainPlayer->GetAttackDamage_L(wAttackDamage[0], wAttackDamage[1]);

	wsprintf(m_szInfo, g_Message[ETC_MESSAGE861].szMessage, wAttackDamage[0], wAttackDamage[1]); // "%d ~ %d"
	RenderFont(m_szInfo, m_fPosX+85, m_fPosX+240, m_fPosZ+276, m_fPosZ+290, GetStartOrder()+2);
	
	// 성향 //	
	wsprintf(m_szInfo, "%d", g_pMainPlayer->m_wCriminalTime);

	nLen = lstrlen(m_szInfo);
	
	RenderFont(m_szInfo, m_fPosX+85, m_fPosX+85+nLen*7, m_fPosZ+298, m_fPosZ+312, GetStartOrder()+2);
	
	// 대전 전적 //
	wsprintf(m_szInfo, "%d/%d/%d", g_pMainPlayer->m_dwMatchRecords, g_pMainPlayer->m_dwMatchWin, g_pMainPlayer->m_dwMatchLose);	
	RenderFont(m_szInfo, m_fPosX+152, m_fPosX+306, m_fPosZ+47, m_fPosZ+61, GetStartOrder()+2);
//*/
}

void CCharWnd::SetActive(BOOL bActive)
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

		SetRender(SPR_OBJ_CHAR_WINDOWS1, TRUE);
		SetRender(SPR_OBJ_CHAR_WINDOWS2, TRUE);
		SetRender(SPR_OBJ_CHAR_CLOSE1, TRUE);
		
				
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDOPEN, g_v3InterfaceSoundPos, FALSE);
		SetTab(m_enCurrentTabType);
	}
}

void CCharWnd::SpriteStatButtonEvent(STATUS_POINT_KIND enStatusPointKind, MOUSECHECK enMouseCheck)
{
	switch (enMouseCheck)
	{
	case MOUSE_DOWN:
		SetRender(BUTTON_OBJ_EGO_UP1+enStatusPointKind*2, FALSE);
		SetRender(BUTTON_OBJ_EGO_UP2+enStatusPointKind*2, TRUE);
		m_bBtn[enStatusPointKind] = TRUE;
		
		break;
	case MOUSE_UP:
		SetRender(BUTTON_OBJ_EGO_UP1+enStatusPointKind*2, TRUE);
		SetRender(BUTTON_OBJ_EGO_UP2+enStatusPointKind*2, FALSE);
		m_bBtn[enStatusPointKind] = FALSE;
		
		break;
	case MOUSE_OVER:
		
		break;
	}
}

void CCharWnd::OnStatButtonEvent(STATUS_POINT_KIND enStatusPointKind, MOUSECHECK enMouseCheck)
{
	if (!IsStatPoint())
		return;

	SpriteStatButtonEvent(enStatusPointKind, enMouseCheck);
	if (MOUSE_UP == enMouseCheck)
	{
		(this->*m_fnSendStatPoint)(enStatusPointKind);
	}
	
}
int CCharWnd::CheckInterface()
{
	m_bMouseIcon = FALSE;

	int nRt = GetChk();
	
	for(int i = STATUS_POINT_KIND_EGO; i < STATUS_POINT_KIND_MAX; ++i)
	{
		if (m_bBtn[i])
			SpriteStatButtonEvent((STATUS_POINT_KIND)i, MOUSE_UP);
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
				SetRender(SPR_OBJ_CHAR_CLOSE1, FALSE);
				SetRender(SPR_OBJ_CHAR_CLOSE2, TRUE);
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
			OnStatButtonEvent(STATUS_POINT_KIND(nRt-4), eMouseCheck);

			SetMouseCheck(nRt, eMouseCheck, byType);
		}
		break;
	case 9:// 가뎐 정보
		{
//			if(eMouseCheck==MOUSE_DOWN) //  : hwoarang 050120
//			{
//				SetTab(ENUM_TAB_TYPE_GUARDIANINFO);
//			}
		}break;
	case 10:// 캐릭터 정보
		{
			if(eMouseCheck==MOUSE_DOWN)
			{
				
				SetTab(ENUM_TAB_TYPE_CHARACTERINFO);
			}
		}break;
	}		
	
	if(MouseUp())
	{
		SetRender(SPR_OBJ_CHAR_CLOSE1, TRUE);
		SetRender(SPR_OBJ_CHAR_CLOSE2, FALSE);		
	}

	MouseChk(nRt);
	
	if(SetMoveWnd(nRt))	
		return CInterface::GetInstance()->GetSetWnd();
	
	if(IsReturn(nRt, eMouseCheck))
		return CInterface::GetInstance()->GetSetWnd();

	return 0;
}

BOOL CCharWnd::IsStatPoint()
{
	switch(m_enCurrentTabType)
	{
	case ENUM_TAB_TYPE_CHARACTERINFO:
		return g_pMainPlayer->m_wPoint;
	case ENUM_TAB_TYPE_GUARDIANINFO:
		{
			CMonster* pGuardian = GetMyGuardian();
			if (pGuardian)
				return pGuardian->m_wStatPoint;
		}
		break;
	}
	return 0;
}

void CCharWnd::SetTab(ENUM_TAB_TYPE eTabType)
{
	m_enCurrentTabType = eTabType;
	ShowAllStatIncreseButton(FALSE);
	switch(eTabType)
	{
	case ENUM_TAB_TYPE_CHARACTERINFO:
		m_fnDisplay = &CCharWnd::CharacterDisplay;
		m_fnSendStatPoint = &CCharWnd::Send_CharacterStatPoint;
		SetRender(SPR_OBJ_CHAR_TAB, FALSE);
		SetRender(SPR_OBJ_GUARDIAN_LIFE, FALSE);
		
		break;
	case ENUM_TAB_TYPE_GUARDIANINFO:
		m_fnDisplay = &CCharWnd::GuardianDisplay;
		m_fnSendStatPoint = &CCharWnd::Send_GuardianStatPoint;
		SetRender(SPR_OBJ_CHAR_TAB, TRUE);
		SetRender(SPR_OBJ_GUARDIAN_LIFE, TRUE);
		
		break;
	}
}

CMonster* CCharWnd::GetMyGuardian()
{
	return g_pMainPlayer->m_pGuardian[ 0 ];
}

void CCharWnd::GuardianDisplay()
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

// minjin.. Now Modifying...

	nOrder = GetStartOrder() + 2;

	// 캐릭터 이름 //		
	if(__strcmp(pGuardian->m_szName, "")!=0)
		RenderFont(pGuardian->m_szName, (int)(nLeft), (int)(m_fPosX+55+lstrlen(pGuardian->m_szName) * CHARINFOTEXT_CHAR_WIDTH), (int)(m_fPosZ+31), (int)(m_fPosZ+45), nOrder);

	
	// 직업, 종족, 길드, 기본스탯 (HP, SP, LEVEL, EXP)
	nRight = (int)m_fPosX + CHARINFOTEXT_TOP_XPOS_RIGHT;

/*
	// 직업 //
	if(__strcmp(g_pMainPlayer->m_szClassName, "")!=0)
		RenderFont(g_pMainPlayer->m_szClassName, (int)(nLeft), (int)(nRight), (int)(m_fPosZ+47), (int)(m_fPosZ+61), nOrder);
	
	// 종족 //
	if(__strcmp(g_pMainPlayer->m_szGroupName, "")!=0)	
		RenderFont(g_pMainPlayer->m_szGroupName, (int)(nLeft), (int)(nRight), (int)(m_fPosZ+62), (int)(m_fPosZ+76), nOrder);

	// 길드 //	
	if(__strcmp(g_pMainPlayer->m_szGuildName, "")!=0)
		RenderFont(g_pMainPlayer->m_szGuildName, (int)(nLeft), (int)(nRight), (int)(m_fPosZ+77), (int)(m_fPosZ+91), nOrder);
*/	
	// Hp //
	wsprintf(m_szInfo, "%d / %d", pGuardian->m_dwHP, pGuardian->m_dwMaxHP);
	RenderFont(m_szInfo, (int)(nLeft), (int)(nRight), (int)(m_fPosZ+97), (int)(m_fPosZ+111), nOrder);
	
	/*
	// Sp //
	wsprintf(m_szInfo, "%d / %d", pGuardian->m_dwMP, pGuardian->m_dwMaxMP);
	RenderFont(m_szInfo, (int)(nLeft), (int)(nRight), (int)(m_fPosZ+112), (int)(m_fPosZ+126), nOrder);
	*/
	
	// Level //
	wsprintf(m_szInfo, g_Message[ETC_MESSAGE863].szMessage, pGuardian->m_dwLevel); // "Lv %d"
	RenderFont(m_szInfo, (int)(nLeft), (int)(nRight), (int)(m_fPosZ+127), (int)(m_fPosZ+141), nOrder);
	
	// Exp //
	// modified by minjin. 2004. 10. 28.
	wsprintf(m_szInfo, "%d / %d", pGuardian->m_dwExp, GetExpTableOfLevel(OBJECT_TYPE_MONSTER, GetGuardianItemLevel(pGuardian->m_dwExp)+1));
//	wsprintf(m_szInfo, "%d / %d", GetExpTableOfLevel(OBJECT_TYPE_PLAYER, g_pMainPlayer->m_dwLevel), GetExpTableOfLevel(OBJECT_TYPE_PLAYER, g_pMainPlayer->m_dwLevel+1));
	RenderFont(m_szInfo, (int)(nLeft), (int)(nRight), (int)(m_fPosZ+142), (int)(m_fPosZ+156), nOrder);

	
	// EGO, STR, INT, DEX, VIT
	nRight	= (int)m_fPosX + CHARINFOTEXT_BASIC_STAT_XPOS_RIGHT;
	nLeft	= (int)m_fPosX + CHARINFOTEXT_BASIC_STAT_XPOS_LEFT;


	// EGO //		
	nSize = wsprintf(m_szInfo, "%d", pGuardian->m_wEgo);		
	RenderFont(m_szInfo, (int)(THIS_TEXT_LEFT(nLeft, nSize)), (int)(nRight), (int)(m_fPosZ+163), (int)(m_fPosZ+177), nOrder);

	// STR //
	nSize = wsprintf(m_szInfo, "%d", pGuardian->m_wStr);
	RenderFont(m_szInfo, (int)(THIS_TEXT_LEFT(nLeft, nSize)), (int)(nRight), (int)(m_fPosZ+178), (int)(m_fPosZ+192), nOrder);	
	
	// INT //	
	nSize = wsprintf(m_szInfo, "%d", pGuardian->m_wInt);	
	RenderFont(m_szInfo, (int)(THIS_TEXT_LEFT(nLeft, nSize)), (int)(nRight), (int)(m_fPosZ+193), (int)(m_fPosZ+207), nOrder);	
	
	// Dex //	
	nSize = wsprintf(m_szInfo, "%d", pGuardian->m_wDex);	
	RenderFont(m_szInfo, (int)(THIS_TEXT_LEFT(nLeft, nSize)), (int)(nRight), (int)(m_fPosZ+208), (int)(m_fPosZ+222), nOrder);	
	
	// VIT //
	nSize = wsprintf(m_szInfo, "%d", pGuardian->m_wVit);	
	RenderFont(m_szInfo, (int)(THIS_TEXT_LEFT(nLeft, nSize)), (int)(nRight), (int)(m_fPosZ+223), (int)(m_fPosZ+237), nOrder);	

	
	// 저항력 스탯 (Physical, Flame, Ice, Electric, Poison)
	nRight	= (int)m_fPosX + CHARINFOTEXT_RESIST_STAT_XPOS_RIGHT;
	nLeft	= (int)m_fPosX + CHARINFOTEXT_RESIST_STAT_XPOS_LEFT;


	// Physical //	
	nSize = wsprintf(m_szInfo, "%d%%", pGuardian->m_wPhyResist);	
	RenderFont(m_szInfo, (int)(THIS_TEXT_LEFT(nLeft, nSize)), (int)(nRight), (int)(m_fPosZ+163), (int)(m_fPosZ+177), nOrder);	

	// Flame //	
	nSize = wsprintf(m_szInfo, "%d%%", pGuardian->m_wFireResist);	
	RenderFont(m_szInfo, (int)(THIS_TEXT_LEFT(nLeft, nSize)), (int)(nRight), (int)(m_fPosZ+178), (int)(m_fPosZ+192), nOrder);	
	
	// Ice //	
	nSize = wsprintf(m_szInfo, "%d%%", pGuardian->m_wIceResist);	
	RenderFont(m_szInfo, (int)(THIS_TEXT_LEFT(nLeft, nSize)), (int)(nRight), (int)(m_fPosZ+193), (int)(m_fPosZ+207), nOrder);	
	
	// Electric	//	
	nSize = wsprintf(m_szInfo, "%d%%", pGuardian->m_wLightResist);	
	RenderFont(m_szInfo, (int)(THIS_TEXT_LEFT(nLeft, nSize)), (int)(nRight), (int)(m_fPosZ+208), (int)(m_fPosZ+222), nOrder);

	// Poison //	
	nSize = wsprintf(m_szInfo, "%d%%", pGuardian->m_wPoiResist);	
	RenderFont(m_szInfo, (int)(THIS_TEXT_LEFT(nLeft, nSize)), (int)(nRight), (int)(m_fPosZ+223), (int)(m_fPosZ+237), nOrder);

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

		RenderFont(m_szInfo, (int)(nLeft), (int)(m_fPosX + 150), (int)(m_fPosZ + 191), (int)(m_fPosZ + 205), nOrder);

		ShowAllStatIncreseButton(TRUE);
	}
	
	
	// 공격 정확도 //	
	nSize = wsprintf(m_szInfo, "%d", pGuardian->m_wAttackAcuracy);	
	RenderFont(m_szInfo, (int)(m_fPosX + 124 - nSize * CHARINFOTEXT_CHAR_WIDTH), (int)(m_fPosX+125), (int)(m_fPosZ+243), (int)(m_fPosZ+257), nOrder);	

	// 회피력 //	
	nSize = wsprintf(m_szInfo, "%d", pGuardian->m_wAvoid);	
	RenderFont(m_szInfo, (int)(m_fPosX + 248 - nSize * CHARINFOTEXT_CHAR_WIDTH), (int)(m_fPosX+250), (int)(m_fPosZ+243), (int)(m_fPosZ+257), nOrder);	

	// L 공격력 //
	wsprintf(m_szInfo, g_Message[ETC_MESSAGE861].szMessage, pGuardian->m_pwAttackDamage_L[0], pGuardian->m_pwAttackDamage_L[1]); // "%d ~ %d"
	RenderFont(m_szInfo, (int)(m_fPosX + 85), (int)(m_fPosX + 240), (int)(m_fPosZ + 276), (int)(m_fPosZ + 290), nOrder);

	/*
	// R 공격력 //	
	g_pMainPlayer->GetAttackDamage_R(wAttackDamage[0], wAttackDamage[1]);
	
	wsprintf(m_szInfo, g_Message[ETC_MESSAGE861].szMessage, wAttackDamage[0], wAttackDamage[1]); // "%d ~ %d"
	RenderFont(m_szInfo, (int)(m_fPosX + 85), (int)(m_fPosX + 230), (int)(m_fPosZ + 259), (int)(m_fPosZ + 273), nOrder);

	
	// 성향 //	
	wsprintf(m_szInfo, "%d", g_pMainPlayer->m_wCriminalTime);

	nLen = lstrlen(m_szInfo);
	
	RenderFont(m_szInfo, (int)(m_fPosX + 85), (int)(m_fPosX + 85 + nLen * CHARINFOTEXT_CHAR_WIDTH), (int)(m_fPosZ + 298), (int)(m_fPosZ + 312), nOrder);
	
	// 대전 전적 //
	wsprintf(m_szInfo, "%d/%d/%d", g_pMainPlayer->m_dwMatchRecords, g_pMainPlayer->m_dwMatchWin, g_pMainPlayer->m_dwMatchLose);	
	RenderFont(m_szInfo, (int)(m_fPosX + 152), (int)(m_fPosX + 306), (int)(m_fPosZ + 47), (int)(m_fPosZ + 61), nOrder);
*/
}
void CCharWnd::HideAllStatButton()
{
	SetRender(BUTTON_OBJ_EGO_UP1, FALSE);
	SetRender(BUTTON_OBJ_STR_UP1, FALSE);
	SetRender(BUTTON_OBJ_INT_UP1, FALSE);
	SetRender(BUTTON_OBJ_DEX_UP1, FALSE);
	SetRender(BUTTON_OBJ_VIT_UP1, FALSE);		
	SetRender(BUTTON_OBJ_EGO_UP2, FALSE);
	SetRender(BUTTON_OBJ_STR_UP2, FALSE);
	SetRender(BUTTON_OBJ_INT_UP2, FALSE);
	SetRender(BUTTON_OBJ_DEX_UP2, FALSE);
	SetRender(BUTTON_OBJ_VIT_UP2, FALSE);
}
void CCharWnd::ShowAllStatIncreseButton(BOOL bShow)
{
	SetRender(BUTTON_OBJ_EGO_UP1, bShow);
	SetRender(BUTTON_OBJ_STR_UP1, bShow);
	SetRender(BUTTON_OBJ_INT_UP1, bShow);
	SetRender(BUTTON_OBJ_DEX_UP1, bShow);
	SetRender(BUTTON_OBJ_VIT_UP1, bShow);
}
void CCharWnd::CharacterDisplay()
{
	
	int nLen	= 0;
	int nSize	= 0;
	int	nLeft	= 0;
	int nRight	= 0;
	int nOrder	= 0;

#if IS_JAPAN_LOCALIZING()
		nLeft	= (LONG)m_fPosX+50;
#else
		nLeft	= (LONG)m_fPosX+55;
#endif

// minjin.. Now Modifying...

	nOrder = GetStartOrder() + 2;

	// 캐릭터 이름 //		
	if(__strcmp(g_pMainPlayer->m_szName, "")!=0)
		RenderFont(g_pMainPlayer->m_szName, (int)(nLeft), (int)(m_fPosX+55+g_pMainPlayer->m_nCharNameSize * CHARINFOTEXT_CHAR_WIDTH), (int)(m_fPosZ+31), (int)(m_fPosZ+45), nOrder);		

	
	// 직업, 종족, 길드, 기본스탯 (HP, SP, LEVEL, EXP)
	nRight = (int)m_fPosX + CHARINFOTEXT_TOP_XPOS_RIGHT;


	// 직업 //
	if(__strcmp(g_pMainPlayer->m_szClassName, "")!=0)
		RenderFont(g_pMainPlayer->m_szClassName, (int)(nLeft), (int)(nRight), (int)(m_fPosZ+47), (int)(m_fPosZ+61), nOrder);
	
	// 종족 //
	if(__strcmp(g_pMainPlayer->m_szGroupName, "")!=0)	
		RenderFont(g_pMainPlayer->m_szGroupName, (int)(nLeft), (int)(nRight), (int)(m_fPosZ+62), (int)(m_fPosZ+76), nOrder);

	// 길드 //	
	if(__strcmp(g_pMainPlayer->m_szGuildName, "")!=0)
		RenderFont(g_pMainPlayer->m_szGuildName, (int)(nLeft), (int)(nRight), (int)(m_fPosZ+77), (int)(m_fPosZ+91), nOrder);
	
	// Hp //
	wsprintf(m_szInfo, "%d / %d", g_pMainPlayer->m_wHP, g_pMainPlayer->m_wMaxHP);
	RenderFont(m_szInfo, (int)(nLeft), (int)(nRight), (int)(m_fPosZ+97), (int)(m_fPosZ+111), nOrder);
	
	// Sp //
	wsprintf(m_szInfo, "%d / %d", g_pMainPlayer->m_wMP, g_pMainPlayer->m_wMaxMP);
	RenderFont(m_szInfo, (int)(nLeft), (int)(nRight), (int)(m_fPosZ+112), (int)(m_fPosZ+126), nOrder);
	
	// Level //
	wsprintf(m_szInfo, g_Message[ETC_MESSAGE863].szMessage, g_pMainPlayer->m_dwLevel); // "Lv %d"
	RenderFont(m_szInfo, (int)(nLeft), (int)(nRight), (int)(m_fPosZ+127), (int)(m_fPosZ+141), nOrder);
	
	// Exp //
	// modified by minjin. 2004. 10. 28.
	wsprintf(m_szInfo, "%d / %d", g_pMainPlayer->m_dwExp, GetExpTableOfLevel(OBJECT_TYPE_PLAYER, g_pMainPlayer->m_dwLevel+1));
//	wsprintf(m_szInfo, "%d / %d", GetExpTableOfLevel(OBJECT_TYPE_PLAYER, g_pMainPlayer->m_dwLevel), GetExpTableOfLevel(OBJECT_TYPE_PLAYER, g_pMainPlayer->m_dwLevel+1));
	RenderFont(m_szInfo, (int)(nLeft), (int)(nRight), (int)(m_fPosZ+142), (int)(m_fPosZ+156), nOrder);

	
	// EGO, STR, INT, DEX, VIT
	nRight	= (int)m_fPosX + CHARINFOTEXT_BASIC_STAT_XPOS_RIGHT;
	nLeft	= (int)m_fPosX + CHARINFOTEXT_BASIC_STAT_XPOS_LEFT;


	// EGO //		
	nSize = wsprintf(m_szInfo, "%d", g_pMainPlayer->m_dwEgo);		
	RenderFont(m_szInfo, (int)(THIS_TEXT_LEFT(nLeft, nSize)), (int)(nRight), (int)(m_fPosZ+163), (int)(m_fPosZ+177), nOrder);

	// STR //
	nSize = wsprintf(m_szInfo, "%d", g_pMainPlayer->m_dwStr);
	RenderFont(m_szInfo, (int)(THIS_TEXT_LEFT(nLeft, nSize)), (int)(nRight), (int)(m_fPosZ+178), (int)(m_fPosZ+192), nOrder);	
	
	// INT //	
	nSize = wsprintf(m_szInfo, "%d", g_pMainPlayer->m_dwInt);	
	RenderFont(m_szInfo, (int)(THIS_TEXT_LEFT(nLeft, nSize)), (int)(nRight), (int)(m_fPosZ+193), (int)(m_fPosZ+207), nOrder);	
	
	// Dex //	
	nSize = wsprintf(m_szInfo, "%d", g_pMainPlayer->m_dwDex);	
	RenderFont(m_szInfo, (int)(THIS_TEXT_LEFT(nLeft, nSize)), (int)(nRight), (int)(m_fPosZ+208), (int)(m_fPosZ+222), nOrder);	
	
	// VIT //
	nSize = wsprintf(m_szInfo, "%d", g_pMainPlayer->m_dwVit);	
	RenderFont(m_szInfo, (int)(THIS_TEXT_LEFT(nLeft, nSize)), (int)(nRight), (int)(m_fPosZ+223), (int)(m_fPosZ+237), nOrder);	

	
	// 저항력 스탯 (Physical, Flame, Ice, Electric, Poison)
	nRight	= (int)m_fPosX + CHARINFOTEXT_RESIST_STAT_XPOS_RIGHT;
	nLeft	= (int)m_fPosX + CHARINFOTEXT_RESIST_STAT_XPOS_LEFT;


	// Physical //	
	nSize = wsprintf(m_szInfo, "%d%%", g_pMainPlayer->GetPhyResist());	
	RenderFont(m_szInfo, (int)(THIS_TEXT_LEFT(nLeft, nSize)), (int)(nRight), (int)(m_fPosZ+163), (int)(m_fPosZ+177), nOrder);	

	// Flame //	
	nSize = wsprintf(m_szInfo, "%d%%", g_pMainPlayer->m_wFireResist);	
	RenderFont(m_szInfo, (int)(THIS_TEXT_LEFT(nLeft, nSize)), (int)(nRight), (int)(m_fPosZ+178), (int)(m_fPosZ+192), nOrder);	
	
	// Ice //	
	nSize = wsprintf(m_szInfo, "%d%%", g_pMainPlayer->m_wIceResist);	
	RenderFont(m_szInfo, (int)(THIS_TEXT_LEFT(nLeft, nSize)), (int)(nRight), (int)(m_fPosZ+193), (int)(m_fPosZ+207), nOrder);	
	
	// Electric	//	
	nSize = wsprintf(m_szInfo, "%d%%", g_pMainPlayer->m_wLightResist);	
	RenderFont(m_szInfo, (int)(THIS_TEXT_LEFT(nLeft, nSize)), (int)(nRight), (int)(m_fPosZ+208), (int)(m_fPosZ+222), nOrder);

	// Poison //	
	nSize = wsprintf(m_szInfo, "%d%%", g_pMainPlayer->m_wPoiResist);	
	RenderFont(m_szInfo, (int)(THIS_TEXT_LEFT(nLeft, nSize)), (int)(nRight), (int)(m_fPosZ+223), (int)(m_fPosZ+237), nOrder);

	// Status Point //
	if(g_pMainPlayer->m_wPoint>0)
	{
		nSize = wsprintf(m_szInfo, "%d", g_pMainPlayer->m_wPoint);

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

		RenderFont(m_szInfo, (int)(nLeft), (int)(m_fPosX + 150), (int)(m_fPosZ + 191), (int)(m_fPosZ + 205), nOrder);

		ShowAllStatIncreseButton(TRUE);
		
	}
	
	// 공격 정확도 //	
	nSize = wsprintf(m_szInfo, "%d", g_pMainPlayer->m_wAttackAcuracy);	
	RenderFont(m_szInfo, (int)(m_fPosX + 124 - nSize * CHARINFOTEXT_CHAR_WIDTH), (int)(m_fPosX+125), (int)(m_fPosZ+243), (int)(m_fPosZ+257), nOrder);	

	// 회피력 //	
	nSize = wsprintf(m_szInfo, "%d", g_pMainPlayer->m_wAvoid);	
	RenderFont(m_szInfo, (int)(m_fPosX + 248 - nSize * CHARINFOTEXT_CHAR_WIDTH), (int)(m_fPosX+250), (int)(m_fPosZ+243), (int)(m_fPosZ+257), nOrder);	

	// R 공격력 //	
	WORD wAttackDamage[2] = {0,};
	g_pMainPlayer->GetAttackDamage_R(wAttackDamage[0], wAttackDamage[1]);
	
	wsprintf(m_szInfo, g_Message[ETC_MESSAGE861].szMessage, wAttackDamage[0], wAttackDamage[1]); // "%d ~ %d"
	RenderFont(m_szInfo, (int)(m_fPosX + 85), (int)(m_fPosX + 230), (int)(m_fPosZ + 259), (int)(m_fPosZ + 273), nOrder);

	// L 공격력 //		
	g_pMainPlayer->GetAttackDamage_L(wAttackDamage[0], wAttackDamage[1]);

	wsprintf(m_szInfo, g_Message[ETC_MESSAGE861].szMessage, wAttackDamage[0], wAttackDamage[1]); // "%d ~ %d"
	RenderFont(m_szInfo, (int)(m_fPosX + 85), (int)(m_fPosX + 240), (int)(m_fPosZ + 276), (int)(m_fPosZ + 290), nOrder);
	
	// 성향 //	
	wsprintf(m_szInfo, "%d", g_pMainPlayer->m_wCriminalTime);

	nLen = lstrlen(m_szInfo);
	
	RenderFont(m_szInfo, (int)(m_fPosX + 85), (int)(m_fPosX + 85 + nLen * CHARINFOTEXT_CHAR_WIDTH), (int)(m_fPosZ + 298), (int)(m_fPosZ + 312), nOrder);
	
	// 대전 전적 //
	wsprintf(m_szInfo, "%d/%d/%d", g_pMainPlayer->m_dwMatchRecords, g_pMainPlayer->m_dwMatchWin, g_pMainPlayer->m_dwMatchLose);	
	RenderFont(m_szInfo, (int)(m_fPosX + 152), (int)(m_fPosX + 306), (int)(m_fPosZ + 47), (int)(m_fPosZ + 61), nOrder);

}
void CCharWnd::SendStatPoint(GAME_OBJECT_TYPE enObjectType, STATUS_POINT_KIND enStatusPointKind)
{
	CTDS_CHAR_LEVELUP pPacket;				
	pPacket.byObjectType		= (BYTE)enObjectType;				
	pPacket.byStatusType		= (BYTE)enStatusPointKind;
	m_bBtn[enStatusPointKind]	= FALSE;
	
	g_pNet->SendMsg((char*)&pPacket, sizeof(CTDS_CHAR_LEVELUP), SERVER_INDEX_ZONE);					
}
void CCharWnd::Send_CharacterStatPoint(STATUS_POINT_KIND enStatusPointKind)
{
	if(g_pMainPlayer->m_wPoint>0)
	{
		SendStatPoint(OBJECT_TYPE_PLAYER, enStatusPointKind);
	}				

//	if(g_pMainPlayer->m_wPoint==0)
//		byType = __MOUSE_POINTER_DEFAULT__;	
}
void CCharWnd::Send_GuardianStatPoint(STATUS_POINT_KIND enStatusPointKind)
{
	CMonster* pGuardian = GetMyGuardian();
	if (!pGuardian)
		return;

	if(pGuardian->m_wStatPoint>0)
	{
		SendStatPoint(OBJECT_TYPE_MONSTER, enStatusPointKind);
	}				

//	if(pGuardian->m_wStatPoint==0)
//		byType = __MOUSE_POINTER_DEFAULT__;	
}
//======================================================//
// End.													//
//======================================================//