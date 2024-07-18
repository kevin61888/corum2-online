//==========================================================//
// Code by Jang.								2002.12.17	//
//==========================================================//
#include "UserInterface.h"
#include "CodeFun.h"
#include "ItemWnd.h"
#include "CharWnd.h"
#include "SkillWnd.h"
#include "GameMenuWnd.h"
#include "DSMiniMap.h"
#include "Interface.h"
#include "ChatWnd.h"
#include "SpriteManager.h"
#include "User.h"
#include "GameControl.h"
#include "DefResource.h"
#include "QuantityWnd.h"
#include "ItemWnd.h"
#include "ObjectManager.h"
#include "GMTool.h"
#include "GuildExWnd.h"
#include "GuildRankWnd.h"
#include "CorumResource.h"
#include "HashTable.h"
#include "dungeontable.h"
#include "dungeon_data_ex.h"
#include "message.h"
#include "GroupWnd.h"
#include "PlayerShopWnd.h"
#include "Chat.h"
#include "CDungeonInfoWnd.h"
#include "RequestMatchWnd.h"
#include "InputManager.h"
#include "ChatBackground.h"
#include "DungeonProcess.h"
#include "NetworkClient.h"
#include "Effect.h"
#include "InitGame.h"
#include "ItemTradeShopWnd.h"
#include "ItemNative.h"
#include "GuardianWnd.h"

//////////////////////////////////////////////////////////////////////////
// Temporary File for Interface Process
#include "InterfaceManager.h"
#include "InterfaceProcTemp.h"


CUserInterface* CUserInterface::c_pThis = NULL;



void GXClickProc(GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag)
{
	if(bFrameFlag==FRAME_FLAG_FINISHED || bFrameFlag == FRAME_FLAG_CHANGE_NEXT )	
		HideObject(handle);
}

void GXMoveProc(GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag)
{	
}

//==========================================================//
// Construction/Destruction.								//
//==========================================================//
CUserInterface::CUserInterface()
{ 
	for(int i = 0; i < 2; i++)
	{
		m_fEnIndex[i]	= 0;
		m_fEnSpeed[i]	= 0;
		m_fManaIndex[i]	= 0;
		m_fManaSpeed[i]	= 0;
		
		m_bEn[i]		= FALSE;
		m_bMana[i]		= FALSE;
	}	

	for(i = 0; i < 4; i++)
		m_bUIChk[i] = FALSE;

	m_dwMagicArray		= 0;
	m_byMoveType		= 0;
	m_byOrderCheck		= 0;
	m_byOrder			= 0;
	m_fExpIndex			= 0;
	m_fExpSpeed			= 0;
	m_fEnDecSpeed		= 0;
	m_fEnIncSpeed		= 0;	
	m_fManaIncDec		= 0;
	m_fManaDecSpeed		= 0;
	m_fManaIncSpeed		= 0;
	m_nChatChk			= 0;		
	m_nPK				= 0;
	m_wMaxSkillLevel	= __MAX_SKILL_UP__;
	m_wMaxMasteryLevel	= __MAX_MASTERYSKILL_UP__;
	m_byPointerType		= 255;
	m_bTrade			= FALSE;
	m_bMatch			= FALSE;
	m_bRGW				= FALSE;		// RivalGuildWar
	m_bCommand			= TRUE;
	m_bParty			= TRUE;
	m_bGuardian			= FALSE;
	m_bSoundChk			= FALSE;
	m_bUserInterface	= FALSE;
	m_bMouseIcon		= FALSE;
	m_bSmall			= FALSE;
	m_bManaDec			= FALSE;
	m_bManaInc			= FALSE;
	m_bManaDef			= FALSE;
	m_bActive			= FALSE;
	m_bExp				= FALSE;
	m_bEnDec			= FALSE;
	m_bEnInc			= FALSE;
	m_bEnDef			= FALSE;	
	m_bExpIncDef		= FALSE;
	m_bClick			= FALSE;
	m_bUp				= FALSE;
	m_bDown				= FALSE;	
	m_bSkillRender		= 0;
	m_bMonster			= FALSE;
	m_bMoveChr			= FALSE;
	m_fMonMaxHp			= 0;
	m_fMonHp			= 0;
	m_dwMonCurTime		= 0;
	m_dwMonPrevTime		= 0;
	m_dwUserCurTime		= 0;
	m_dwUserPrevTime		= 0;
	m_bMatchUserHPShowFlag = 0;
	m_bMatchUserHPShowEnable = FALSE;
	m_dwMagicArray	= 0;

	m_pMonster			= NULL;
	m_pUser				= NULL;	

	int nMousePos[__MAX_MOUSE_POINTER__*2] = 
	{
		2, 3, 2, 3, 2, 3, 10, 2, 10, 2, 15, 8, 15, 8, 15, 8, 1, 1, 1, 1, 1, 1, 7, 19, 7, 19, 10, 10, 10, 10, 10, 10, 16, 16, 16, 16
	};
	
	for(i = 0; i < __MAX_MOUSE_POINTER__*2; i++)
		m_nMousePos[i]	= nMousePos[i];	
	
	// Mouse Move Effect //
}

CUserInterface::~CUserInterface()
{
}
//==========================================================//
// Class Member Function.									//
//==========================================================//
BOOL CUserInterface::Init()
{
	m_pStatusWnd = CGuildWarStatusWnd::GetInstance();

	for(int i = 0; i < 2; i++)
	{
		m_fManaIndex[i]	= 0;
		m_fEnIndex[i]	= 0;		
		m_bEn[i]		= FALSE;
		m_bMana[i]		= FALSE;		
	}	

	m_fEnSpeed[0]	= 1.2f;
	m_fEnSpeed[1]	= 1.5f;
	m_fEnIncSpeed	= 1.0f;
	m_fEnDecSpeed	= 1.0f;
	m_fExpIndex		= 0.0f;
	m_fManaSpeed[0]	= 1.2f;
	m_fManaSpeed[1]	= 1.5f;
	m_fManaIncSpeed	= 0.6f;
	m_fManaDecSpeed	= 0.6f;
	m_fExpSpeed		= 1.3f;	
	m_bManaDec		= FALSE;
	m_bManaInc		= FALSE;
	m_bManaDef		= FALSE;	
	m_bExp			= FALSE;
	m_bEnDec		= FALSE;
	m_bEnInc		= FALSE;
	m_bEnDef		= FALSE;	
	m_bExpIncDef	= FALSE;
	m_bGuardian		= FALSE;
	m_dwMagicArray	= 0;
	
	InsertUIData();				// Insert User Interface Sprite Data

	SetRenderDefaultUI();		// Set Default User Interface Sprite to be Rendered


	SetScalingObjX(SPR_OBJ_CAST, 1023-(float)g_pMainPlayer->m_fCurCoolPoint/(float)g_pMainPlayer->m_fMaxCoolPoint*300);	

	// 버그 //	
	if(m_nPK==0)
		SetRender(BUTTON_OBJ_PKOFF1, TRUE);
	else
		SetRender(BUTTON_OBJ_PKON1, TRUE);	
		
	m_pUserClickHandle.pDesc				= AllocObjDesc();	
	m_pUserClickHandle.pDesc->ObjectFunc	= GXClickProc;
	m_pUserClickHandle.pHandle	= CreateHandleObject(g_pObjManager->GetFile(MOD_PLAYERCLICK), GXPlayerPROC, m_pUserClickHandle.pDesc, GXOBJECT_CREATE_TYPE_EFFECT);		
	HideObject(m_pUserClickHandle.pHandle);
				
	char szFile[0xff]={0,};
	
	lstrcpy(szFile, "e0121000.chr");
	m_pUserMouseHandle[0].pDesc				= AllocObjDesc();			
	m_pUserMouseHandle[0].pHandle			= CreateHandleObject(GetFile(szFile, DATA_TYPE_EFFECT), GXPlayerPROC, m_pUserMouseHandle[0].pDesc, GXOBJECT_CREATE_TYPE_EFFECT);
	HideObject(m_pUserMouseHandle[0].pHandle);

	lstrcpy(szFile, "e0125000.chr");
	m_pUserMouseHandle[1].pDesc				= AllocObjDesc();			
	m_pUserMouseHandle[1].pHandle			= CreateHandleObject(GetFile(szFile, DATA_TYPE_EFFECT), GXPlayerPROC, m_pUserMouseHandle[1].pDesc, GXOBJECT_CREATE_TYPE_EFFECT);
	HideObject(m_pUserMouseHandle[1].pHandle);

	lstrcpy(szFile, "e0128000.chr");
	m_pUserMouseHandle[2].pDesc				= AllocObjDesc();			
	m_pUserMouseHandle[2].pHandle			= CreateHandleObject(GetFile(szFile, DATA_TYPE_EFFECT), GXPlayerPROC, m_pUserMouseHandle[2].pDesc, GXOBJECT_CREATE_TYPE_EFFECT);
	HideObject(m_pUserMouseHandle[2].pHandle);

	m_pSprPlayerShop[0] = g_pRenderer->CreateSpriteObject(GetFile("menu_5.tga", DATA_TYPE_UI), 128, 48, 128, 19, 0);
	m_pSprPlayerShop[1] = g_pRenderer->CreateSpriteObject(GetFile("menu_5.tga", DATA_TYPE_UI), 128, 67, 128, 32, 0);			

	SetMousePointerType(__MOUSE_POINTER_DEFAULT__);
	SetMousePointerPos((float)g_Mouse.MousePos.x , (float)g_Mouse.MousePos.y);	
	
	m_pMonster	= NULL;
	m_pUser		= NULL;
	m_bInit		= TRUE;

	m_pStatusWnd->Init();
	return TRUE;
}

void	CUserInterface::CreateMenu(
		char*	pszName, 
		float	fWidth, 
		float	fHeight, 
		BYTE	byType, 
		int		nIndex, 
		BOOL	bActive, 
		BOOL	bScroll
		)
{
	m_pStatusWnd = CGuildWarStatusWnd::GetInstance();
	
	m_pStatusWnd->CreateMenu( 
		pszName, 
		fWidth, 
		fHeight, 
		byType, 
		nIndex, 
		bActive, 
		false );

	CMenu::CreateMenu( 
		pszName, 
		fWidth, 
		fHeight, 
		byType, 
		nIndex, 
		bActive, 
		bScroll );
}
void CUserInterface::InsertUIData()
{
	int Order = __ORDER_USERINTERFACE_START_;
	
	InsertData(SPR_OBJ_UI_LEFT, SPR_INTERFACE_UI_LEFT, 0, 640, 1.0f, 1.0f, Order, FALSE, FALSE, FALSE);
	InsertData(SPR_OBJ_UI_RIGHT, SPR_INTERFACE_UI_RIGHT, 512, 640, 1.0f, 1.0f, Order, FALSE, FALSE, FALSE);	
	InsertData(SPR_OBJ_EN2, SPR_INTERFACE_EN2, 0, 709, 0.0f, 1.0f,  Order+1,FALSE, FALSE, FALSE);
	InsertData(SPR_OBJ_EN1, SPR_INTERFACE_EN1, 0, 709, 0.0f, 1.0f, Order+2,FALSE, FALSE, FALSE);
	InsertData(SPR_OBJ_EXP, SPR_INTERFACE_EXP, 0, 725, 1.0f, 1.0f, Order+3, FALSE, FALSE, FALSE);		
	InsertData(SPR_OBJ_MANA2, SPR_INTERFACE_MANA2, 1023, 709, 0.0f, 1.0f, Order+1, FALSE, FALSE, FALSE);
	InsertData(SPR_OBJ_MANA1, SPR_INTERFACE_MANA1, 723, 709, 0.0f, 1.0f, Order+2, FALSE, FALSE, FALSE);		
	InsertData(SPR_OBJ_CAST, SPR_INTERFACE_CAST, 723, 725, (float)g_pMainPlayer->m_fCurCoolPoint/(float)g_pMainPlayer->m_fMaxCoolPoint*300/2, 1.0f, Order+3, FALSE, FALSE, FALSE);			
	
	InsertData(BUTTON_OBJ_ITEM1, BUTTON_INTERFACE_ITEM1, 83, 684, 1.0f, 1.0f, Order+1, FALSE, FALSE, FALSE);
	InsertData(BUTTON_OBJ_CHR1, BUTTON_INTERFACE_CHR1, 164, 684, 1.0f, 1.0f, Order+1, FALSE, FALSE, FALSE);
	InsertData(BUTTON_OBJ_SKILL1, BUTTON_INTERFACE_SKILL1, 243, 684, 1.0f, 1.0f, Order+1, FALSE, FALSE, FALSE);
	InsertData(BUTTON_OBJ_GUILD1, BUTTON_INTERFACE_GUILD1, 747, 684, 1.0f, 1.0f, Order+1, FALSE, FALSE, FALSE);
	InsertData(BUTTON_OBJ_PARTY1, BUTTON_INTERFACE_PARTY1, 827, 684, 1.0f, 1.0f, Order+1, FALSE, FALSE, FALSE);
	InsertData(BUTTON_OBJ_CHAT1, BUTTON_INTERFACE_CHAT1, 907, 684, 1.0f, 1.0f, Order+1, FALSE, FALSE, FALSE);	
	InsertData(BUTTON_OBJ_ITEM2, BUTTON_INTERFACE_ITEM2, 83, 684, 1.0f, 1.0f, Order+1, FALSE, FALSE, FALSE);
	InsertData(BUTTON_OBJ_CHR2, BUTTON_INTERFACE_CHR2, 164, 684, 1.0f, 1.0f, Order+1, FALSE, FALSE, FALSE);
	InsertData(BUTTON_OBJ_SKILL2, BUTTON_INTERFACE_SKILL2, 243, 684, 1.0f, 1.0f, Order+1, FALSE, FALSE, FALSE);
	InsertData(BUTTON_OBJ_GUILD2, BUTTON_INTERFACE_GUILD2, 747, 684, 1.0f, 1.0f, Order+1, FALSE, FALSE, FALSE);
	InsertData(BUTTON_OBJ_PARTY2, BUTTON_INTERFACE_PARTY2, 827, 684, 1.0f, 1.0f, Order+1, FALSE, FALSE, FALSE);
	InsertData(BUTTON_OBJ_CHAT2, BUTTON_INTERFACE_CHAT2, 907, 684, 1.0f, 1.0f, Order+1, FALSE, FALSE, FALSE);		
	InsertData(BUTTON_OBJ_SYSTEM1, BUTTON_INTERFACE_SYSTEM1, 659, 731, 1.0f, 1.0f, Order+1, FALSE, FALSE, FALSE);	
	InsertData(BUTTON_OBJ_SYSTEM2, BUTTON_INTERFACE_SYSTEM2, 659, 731, 1.0f, 1.0f, Order+1, FALSE, FALSE, FALSE);
	InsertData(BUTTON_OBJ_PKON1, BUTTON_INTERFACE_PKON1, 333, 731, 1.0f, 1.0f, Order+1, FALSE, FALSE, FALSE);
	InsertData(BUTTON_OBJ_PKON2, BUTTON_INTERFACE_PKON2, 333, 731, 1.0f, 1.0f, Order+1, FALSE, FALSE, FALSE);
	InsertData(BUTTON_OBJ_PKOFF1, BUTTON_INTERFACE_PKOFF1, 333, 731, 1.0f, 1.0f, Order+1, FALSE, FALSE, FALSE);
	InsertData(BUTTON_OBJ_PKOFF2, BUTTON_INTERFACE_PKOFF2, 333, 731, 1.0f, 1.0f, Order+1, FALSE, FALSE, FALSE);

	InsertData(SPR_OBJ_MONSTERHP_BAR1, SPR_INTERFACE_MONSTERHP_BAR1, 401, 0, 1.0f, 1.0f, Order, FALSE, FALSE, FALSE);
	InsertData(SPR_OBJ_MONSTERHP_BAR2, SPR_INTERFACE_MONSTERHP_BAR2, 598, 0, 1.0f, 1.0f, Order, FALSE, FALSE, FALSE);	
	InsertData(SPR_OBJ_MONSTER_GAGE1, SPR_INTERFACE_MONSTER_GAGE1, 412, 3, 1.0f, 1.0f, Order+2, FALSE, FALSE, FALSE);	
	InsertData(SPR_OBJ_MONSTER_GAGE2, SPR_INTERFACE_MONSTER_GAGE2, 412, 3, 1.0f, 1.0f, Order+1, FALSE, FALSE, FALSE);
	
	InsertData(SPR_OBJ_USERHP_BAR1, SPR_INTERFACE_USERHP_BAR1, 401, 0, 1.0f, 1.0f, Order, FALSE, FALSE, FALSE);
	InsertData(SPR_OBJ_USERHP_BAR2, SPR_INTERFACE_USERHP_BAR2, 598, 0, 1.0f, 1.0f, Order, FALSE, FALSE, FALSE);	
	InsertData(SPR_OBJ_USER_GAGE1, SPR_INTERFACE_USER_GAGE1, 412, 3, 1.0f, 1.0f, Order+2, FALSE, FALSE, FALSE);	
	InsertData(SPR_OBJ_USER_GAGE2, SPR_INTERFACE_USER_GAGE2, 412, 3, 1.0f, 1.0f, Order+1, FALSE, FALSE, FALSE);
	InsertData(SPR_OBJ_GUARDIAN_WND1, SPR_INTERFACE_GUARDIAN_WND1, 0, 0, 1.0f, 1.0f, Order, FALSE, FALSE, FALSE);
	InsertData(SPR_OBJ_GUARDIAN_EN, SPR_INTERFACE_GUARDIAN_EN, 0, 0, 1.0f, 1.0f, Order+1, FALSE, FALSE, FALSE);

	InsertData(SPR_OBJ_UI_TRADE1, SPR_INTERFACE_UI_TRADE1, 0, 661, 1.0f, 1.0f, Order+1, FALSE, FALSE, FALSE);	
	InsertData(SPR_OBJ_UI_TRADE2, SPR_INTERFACE_UI_TRADE2, 0, 661, 1.0f, 1.0f, Order+1, FALSE, FALSE, FALSE);	
	InsertData(SPR_OBJ_UI_TRADE3, SPR_INTERFACE_UI_TRADE3, 0, 661, 1.0f, 1.0f, Order+1, FALSE, FALSE, FALSE);		
	InsertData(SPR_OBJ_UI_SHOP1, SPR_INTERFACE_UI_SHOP1, 0, 679, 1.0f, 1.0f, Order+1, FALSE, FALSE, FALSE);	
	InsertData(SPR_OBJ_UI_SHOP2, SPR_INTERFACE_UI_SHOP2, 0, 679, 1.0f, 1.0f, Order+1, FALSE, FALSE, FALSE);	
	InsertData(SPR_OBJ_UI_SHOP3, SPR_INTERFACE_UI_SHOP3, 0, 679, 1.0f, 1.0f, Order+1, FALSE, FALSE, FALSE);		
	InsertData(SPR_OBJ_UI_REST1, SPR_INTERFACE_UI_REST1, 977, 661, 1.0f, 1.0f, Order+1, FALSE, FALSE, FALSE);	
	InsertData(SPR_OBJ_UI_REST2, SPR_INTERFACE_UI_REST2, 977, 661, 1.0f, 1.0f, Order+1, FALSE, FALSE, FALSE);	
	InsertData(SPR_OBJ_UI_REST3, SPR_INTERFACE_UI_REST3, 977, 661, 1.0f, 1.0f, Order+1, FALSE, FALSE, FALSE);	
	InsertData(SPR_OBJ_UI_MATCH1, SPR_INTERFACE_UI_MATCH1, 977, 679, 1.0f, 1.0f, Order+1, FALSE, FALSE, FALSE);	
	InsertData(SPR_OBJ_UI_MATCH2, SPR_INTERFACE_UI_MATCH2, 977, 679, 1.0f, 1.0f, Order+1, FALSE, FALSE, FALSE);	
	InsertData(SPR_OBJ_UI_MATCH3, SPR_INTERFACE_UI_MATCH3, 977, 679, 1.0f, 1.0f, Order+1, FALSE, FALSE, FALSE);		
	
	InsertData(SPR_OBJ_UI_QUICKSLOT_INABLE1, SPR_INTERFACE_UI_QUICKSLOT_INABLE, 727, 733, 1.0f, 1.0f, __ORDER_USER_BLET__+1, FALSE, FALSE, FALSE);
	InsertData(SPR_OBJ_UI_QUICKSLOT_INABLE2, SPR_INTERFACE_UI_QUICKSLOT_INABLE, 764, 733, 1.0f, 1.0f, __ORDER_USER_BLET__+1, FALSE, FALSE, FALSE);
	InsertData(SPR_OBJ_UI_QUICKSLOT_INABLE3, SPR_INTERFACE_UI_QUICKSLOT_INABLE, 801, 733, 1.0f, 1.0f, __ORDER_USER_BLET__+1, FALSE, FALSE, FALSE);
	InsertData(SPR_OBJ_UI_QUICKSLOT_INABLE4, SPR_INTERFACE_UI_QUICKSLOT_INABLE, 838, 733, 1.0f, 1.0f, __ORDER_USER_BLET__+1, FALSE, FALSE, FALSE);
	InsertData(SPR_OBJ_UI_QUICKSLOT_INABLE5, SPR_INTERFACE_UI_QUICKSLOT_INABLE, 875, 733, 1.0f, 1.0f, __ORDER_USER_BLET__+1, FALSE, FALSE, FALSE);
	InsertData(SPR_OBJ_UI_QUICKSLOT_INABLE6, SPR_INTERFACE_UI_QUICKSLOT_INABLE, 912, 733, 1.0f, 1.0f, __ORDER_USER_BLET__+1, FALSE, FALSE, FALSE);
	InsertData(SPR_OBJ_UI_QUICKSLOT_INABLE7, SPR_INTERFACE_UI_QUICKSLOT_INABLE, 949, 733, 1.0f, 1.0f, __ORDER_USER_BLET__+1, FALSE, FALSE, FALSE);
	InsertData(SPR_OBJ_UI_QUICKSLOT_INABLE8, SPR_INTERFACE_UI_QUICKSLOT_INABLE, 986, 733, 1.0f, 1.0f, __ORDER_USER_BLET__+1, FALSE, FALSE, FALSE);	

	// 가디언 스탯창 열기 버튼 : 최덕석 2005.3.29 
	InsertData(SPR_OBJ_UI_GUARDIAN_STATUS, SPR_INTERFACE_UI_GUARDIAN_STATUS, 0, 49, 1.0f, 1.0f, Order+1, FALSE, FALSE, FALSE);	
}

void CUserInterface::SetRenderDefaultUI()
{
	SetRender(SPR_OBJ_UI_TRADE1, TRUE);
	SetRender(SPR_OBJ_UI_SHOP1, TRUE);
	SetRender(SPR_OBJ_UI_REST1, TRUE);
	SetRender(SPR_OBJ_UI_MATCH1, TRUE);	
	SetRender(SPR_OBJ_UI_LEFT, TRUE);
	SetRender(SPR_OBJ_UI_RIGHT, TRUE);
	SetRender(SPR_OBJ_EN2, TRUE);
	SetRender(SPR_OBJ_EN1, TRUE);
	SetRender(SPR_OBJ_EXP, TRUE);
	SetRender(SPR_OBJ_MANA2, TRUE);
	SetRender(SPR_OBJ_MANA1, TRUE);
	SetRender(SPR_OBJ_CAST, TRUE);
}

void CUserInterface::SetPointerChk(BOOL bChk)
{
	m_bPointerChk = bChk;
}

BOOL CUserInterface::GetPointerChk()
{
	return m_bPointerChk;
}

void CUserInterface::SetOrder()
{
}

int	CUserInterface::GetChkOrderIndex()
{
	return m_byOrderCheck;
}

void CUserInterface::Remove()
{
	SetMousePointerType(__MOUSE_POINTER_DEFAULT__);

	RemoveAllData();
	
	if(m_pUserClickHandle.pHandle)
	{
		DeleteHandleObject(m_pUserClickHandle.pHandle);
		m_pUserClickHandle.pHandle = NULL;
	}
	if(m_pUserClickHandle.pDesc)	
	{
		FreeObjDesc( m_pUserClickHandle.pDesc);
		m_pUserClickHandle.pDesc = NULL;
	}
	
	if(m_pUserMouseHandle[0].pHandle)
	{
		DeleteHandleObject(m_pUserMouseHandle[0].pHandle);
		m_pUserMouseHandle[0].pHandle = NULL;
	}
	if(m_pUserMouseHandle[0].pDesc)	
	{
		FreeObjDesc(m_pUserMouseHandle[0].pDesc);
		m_pUserMouseHandle[0].pDesc = NULL;
	}
	
	if(m_pUserMouseHandle[1].pHandle)
	{
		DeleteHandleObject(m_pUserMouseHandle[1].pHandle);
		m_pUserMouseHandle[1].pHandle = NULL;
	}
	if(m_pUserMouseHandle[1].pDesc)	
	{
		FreeObjDesc(m_pUserMouseHandle[1].pDesc);
		m_pUserMouseHandle[1].pDesc = NULL;
	}
	
	if(m_pUserMouseHandle[2].pHandle)
	{
		DeleteHandleObject(m_pUserMouseHandle[2].pHandle);
		m_pUserMouseHandle[2].pHandle = NULL;
	}
	if(m_pUserMouseHandle[2].pDesc)	
	{
		FreeObjDesc(m_pUserMouseHandle[2].pDesc);
		m_pUserMouseHandle[2].pDesc = NULL;
	}

	if(m_pSprPlayerShop[0])
		m_pSprPlayerShop[0]->Release();
	if(m_pSprPlayerShop[1])
		m_pSprPlayerShop[1]->Release();

	m_byPointerType	= 255;
	
	m_bActive	= FALSE;
	m_bInit		= FALSE;

	
}


int	CUserInterface::GetChk()
{
	CSkillWnd* pSkillWnd = CSkillWnd::GetInstance();
	CMonster *pGuardian = g_pMainPlayer->m_pGuardian[0];

	if(pSkillWnd->m_bySkillIndex!=0)
	{		
		if(	g_Mouse.MousePos.x>=116 &&
			g_Mouse.MousePos.x<=116+32 &&
			g_Mouse.MousePos.y>=646 &&
			g_Mouse.MousePos.y<=646+32)
		{
			switch(pSkillWnd->m_bySkillIndex)
			{
			case 1:
				return 11;
				break;
			case 2:
				return 12;
				break;
			case 3:
				return 22;
				break;
			}
		}
		else
		{
			if(pSkillWnd->m_bySkillIndex==1)
			{
				int nCount = 1;

				for(int i = 0; i < g_sSkillListManager.byLeftSkillCnt; i++)
				{
					if(g_pMainPlayer->GetSkillLevel(g_sSkillListManager.byLeftSkill[i])>0)
					{
						int nPosX = 116+32*(nCount%5);
						int nPosY = 646-32*(nCount/5);

						if( g_Mouse.MousePos.x>=nPosX &&
							g_Mouse.MousePos.x<=nPosX+32 &&
							g_Mouse.MousePos.y>=nPosY &&
							g_Mouse.MousePos.y<=nPosY+32)
						{
							return 11;
						}

						nCount++;
					}
				}
			}
			else if(pSkillWnd->m_bySkillIndex==2)
			{
				int nCount = 0;

				for(int i = 0; i < g_sSkillListManager.byRightSkillCnt; i++)
				{					
					if(g_pMainPlayer->GetSkillLevel(g_sSkillListManager.byRightSkill[i])>0)
					{
						int nPosX = 116+32*(nCount%5);
						int nPosY = 646-32*(nCount/5);

						if( g_Mouse.MousePos.x>=nPosX &&
							g_Mouse.MousePos.x<=nPosX+32 &&
							g_Mouse.MousePos.y>=nPosY &&
							g_Mouse.MousePos.y<=nPosY+32)
							return 12;

						nCount++;
					}					
				}
			}
			else if(pGuardian && pSkillWnd->m_bySkillIndex==3)
			{
				int nCount = 1;

				for(int i = 0; i < MAX_GUARDIAN_USE_SKILL; i++)
				{					
					if(pGuardian->GetSkillLevel(pGuardian->m_Skill[i].wSkill) > 0)
					{
						int nPosX = 116+32*(nCount%5);
						int nPosY = 646-32*(nCount/5);

						if( g_Mouse.MousePos.x>=nPosX &&
							g_Mouse.MousePos.x<=nPosX+32 &&
							g_Mouse.MousePos.y>=nPosY &&
							g_Mouse.MousePos.y<=nPosY+32)
							return 22;

						nCount++;
					}					
				}
			}
		}
	}

	if(m_bGuardian)
	{
		// 가디언 정보창 버튼
		if(g_Mouse.MousePos.x>=0 && g_Mouse.MousePos.x<=59 && g_Mouse.MousePos.y>=49 && g_Mouse.MousePos.y<=69)
			return 23;
	
		// 가디언 아이템
		if(g_Mouse.MousePos.x>=93 && g_Mouse.MousePos.x<=125 && g_Mouse.MousePos.y>=76 && g_Mouse.MousePos.y<=108)
			return 15;

		// 가디언 스킬 선택 : 최덕석 2005.3.3
		if(g_Mouse.MousePos.x>=209 && g_Mouse.MousePos.x<=209+32 && g_Mouse.MousePos.y>=733 && g_Mouse.MousePos.y<=733+32)
			return 21;	
	}

 
	if( (g_Mouse.MousePos.x>=0   && g_Mouse.MousePos.x<=340  && g_Mouse.MousePos.y>=661 && g_Mouse.MousePos.y<=768)	||
		(g_Mouse.MousePos.x>=341 && g_Mouse.MousePos.x<=370  && g_Mouse.MousePos.y>=723 && g_Mouse.MousePos.y<=768)	||
		(g_Mouse.MousePos.x>=653 && g_Mouse.MousePos.x<=682  && g_Mouse.MousePos.y>=723 && g_Mouse.MousePos.y<=768)	||
#ifndef VER_RIVAL_GUILD_WAR
		(g_Mouse.MousePos.x>=683 && g_Mouse.MousePos.x<=1024 && g_Mouse.MousePos.y>=661 && g_Mouse.MousePos.y<=768) )
#else
		(g_Mouse.MousePos.x>=683 && g_Mouse.MousePos.x<=1024 && g_Mouse.MousePos.y>=631 && g_Mouse.MousePos.y<=768) )	//kjk
#endif
	{	
		if(g_Mouse.MousePos.x>=83 && g_Mouse.MousePos.x<=83+32 && g_Mouse.MousePos.y>=684 && g_Mouse.MousePos.y<=684+16)
			return 2;
		else if(g_Mouse.MousePos.x>=164 && g_Mouse.MousePos.x<=164+32 && g_Mouse.MousePos.y>=684 && g_Mouse.MousePos.y<=684+16)
			return 3;
		else if(g_Mouse.MousePos.x>=243 && g_Mouse.MousePos.x<=243+32 && g_Mouse.MousePos.y>=684 && g_Mouse.MousePos.y<=684+16)
			return 4;
		else if(g_Mouse.MousePos.x>=747 && g_Mouse.MousePos.x<=747+32 && g_Mouse.MousePos.y>=684 && g_Mouse.MousePos.y<=684+16)
			return 5;
		else if(g_Mouse.MousePos.x>=827 && g_Mouse.MousePos.x<=827+32 && g_Mouse.MousePos.y>=684 && g_Mouse.MousePos.y<=684+16)
			return 6;
		else if(g_Mouse.MousePos.x>=907 && g_Mouse.MousePos.x<=907+32 && g_Mouse.MousePos.y>=684 && g_Mouse.MousePos.y<=684+16)
			return 7;
		else if(g_Mouse.MousePos.x>=659 && g_Mouse.MousePos.x<=659+32 && g_Mouse.MousePos.y>=731 && g_Mouse.MousePos.y<=731+32)
			return 8;
		else if(g_Mouse.MousePos.x>=132 && g_Mouse.MousePos.x<=132+32 && g_Mouse.MousePos.y>=733 && g_Mouse.MousePos.y<=733+32)
			return 9;
		else if(g_Mouse.MousePos.x>=245 && g_Mouse.MousePos.x<=245+32 && g_Mouse.MousePos.y>=733 && g_Mouse.MousePos.y<=733+32)
			return 10;
		else if(g_Mouse.MousePos.x>=333 && g_Mouse.MousePos.x<=333+32 && g_Mouse.MousePos.y>=731 && g_Mouse.MousePos.y<=731+32)
			return 13;
		else if(g_Mouse.MousePos.x>=727 && g_Mouse.MousePos.x<=1024 && g_Mouse.MousePos.y>=733 && g_Mouse.MousePos.y<=768)
			return 14;
		else if(g_Mouse.MousePos.x>=0 && g_Mouse.MousePos.x<=46 && g_Mouse.MousePos.y>=661 && g_Mouse.MousePos.y<=677)
			return 16;	// 교환.
		else if(g_Mouse.MousePos.x>=0 && g_Mouse.MousePos.x<=64 && g_Mouse.MousePos.y>=679 && g_Mouse.MousePos.y<=695)
			return 17;	// 앉기.
		else if(g_Mouse.MousePos.x>=977 && g_Mouse.MousePos.x<=1024 && g_Mouse.MousePos.y>=661 && g_Mouse.MousePos.y<=677)
			return 18;	// 샾.
		else if(g_Mouse.MousePos.x>=977 && g_Mouse.MousePos.x<=1024 && g_Mouse.MousePos.y>=679 && g_Mouse.MousePos.y<=695)
			return 19;	// 대결.
#ifdef VER_RIVAL_GUILD_WAR	//kjk
		else if(g_Mouse.MousePos.x>=977 && g_Mouse.MousePos.x<=1024 && g_Mouse.MousePos.y>=634 && g_Mouse.MousePos.y<=659)
			return 20;
#endif
		else
			return 1;
	}	

	return 0;
}


int CUserInterface::CheckInterface()
{
	m_bSkillRender	= 0;
	m_bMouseIcon	= FALSE;
		
	int nRt = GetChk();

//	if(nRt>=1 && nRt<=19)
	if(nRt>=1 && nRt<=23)
		m_bMouseIcon = TRUE;

	if(g_Mouse.bLDown && !m_bSoundChk)
	{		
		if(nRt>1)
		{
			if(nRt!=14)
				_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_BTNCLICK, g_Camera.v3CameraPos, FALSE);
		}			
		m_bSoundChk = TRUE;
	}

	// 대전중 
	if( (g_pMainPlayer->m_bMatching) || (g_pMainPlayer->m_dwGuildWarFlag == G_W_F_OBSERVER && g_pThisDungeon->IsStadium()) )
	{ 
		// 못쓰게 가려라.
		EnableQuickSlot(FALSE);	
	}
	else
	{
		EnableQuickSlot(TRUE);
	}

	if(m_bUIChk[0]==TRUE && nRt!=17)
	{
		SetRender(SPR_OBJ_UI_TRADE1, TRUE);
		SetRender(SPR_OBJ_UI_TRADE2, FALSE);
		SetRender(SPR_OBJ_UI_TRADE3, FALSE);
		m_bUIChk[0] = FALSE;
	}
	if(m_bUIChk[1]==TRUE && nRt!=18)
	{
		SetRender(SPR_OBJ_UI_REST1, TRUE);
		SetRender(SPR_OBJ_UI_REST2, FALSE);
		SetRender(SPR_OBJ_UI_REST3, FALSE);
		m_bUIChk[1] = FALSE;
	}
	if(m_bUIChk[2]==TRUE && nRt!=19)
	{
		SetRender(SPR_OBJ_UI_SHOP1, TRUE);
		SetRender(SPR_OBJ_UI_SHOP2, FALSE);
		SetRender(SPR_OBJ_UI_SHOP3, FALSE);
		m_bUIChk[2] = FALSE;
	}
	if(m_bUIChk[3]==TRUE && nRt!=20)
	{
		SetRender(SPR_OBJ_UI_MATCH1, TRUE);
		SetRender(SPR_OBJ_UI_MATCH2, FALSE);
		SetRender(SPR_OBJ_UI_MATCH3, FALSE);
		m_bUIChk[3] = FALSE;
	}
//#ifdef  __RIVALGUILD_WAR__	//kjk
//UI Insert
//#endif


	switch(nRt)
	{
	case 2:
		{
			SetRender(BUTTON_OBJ_ITEM1, TRUE);
			SetPointer(__MOUSE_POINTER_BUTTON__);
		}		
		break;
	case 3:
		{
			SetRender(BUTTON_OBJ_CHR1, TRUE);
			SetPointer(__MOUSE_POINTER_BUTTON__);
		}
		break;
	case 4:
		{
			SetRender(BUTTON_OBJ_SKILL1, TRUE);
			SetPointer(__MOUSE_POINTER_BUTTON__);
		}
		break;
	case 5:
		{
			SetRender(BUTTON_OBJ_GUILD1, TRUE);
			SetPointer(__MOUSE_POINTER_BUTTON__);
		}				
		break;
	case 6:
		{
			SetRender(BUTTON_OBJ_PARTY1, TRUE);
			SetPointer(__MOUSE_POINTER_BUTTON__);
		}		
		break;
	case 7:
		{
			SetRender(BUTTON_OBJ_CHAT1, TRUE);
			SetPointer(__MOUSE_POINTER_BUTTON__);
		}
		break;
	case 8:
		{
			SetRender(BUTTON_OBJ_SYSTEM1, TRUE);
			SetPointer(__MOUSE_POINTER_BUTTON__);
		}
		break;	
	case 9:
	case 10:
	case 21:
		if(g_Mouse.bLDown)		
			SetPointer(__MOUSE_POINTER_BUTTONCLICK__);	
		else
			SetPointer(__MOUSE_POINTER_BUTTON__);		
		break;
	case 11:
	case 12:
	case 22:
		if(g_Mouse.bLDown)
			SetPointer(__MOUSE_POINTER_BUTTONCLICK__);
		else
			SetPointer(__MOUSE_POINTER_BUTTON__);
		break;
	case 13:
		{
			SetPointer(__MOUSE_POINTER_BUTTON__);
			
			if(m_nPK==0)
			{
				SetRender(BUTTON_OBJ_PKOFF1, FALSE);
				SetRender(BUTTON_OBJ_PKOFF2, TRUE);
			}
			else
			{
				SetRender(BUTTON_OBJ_PKON1, FALSE);
				SetRender(BUTTON_OBJ_PKON2, TRUE);
			}
		}
		break;
	case 14:
		if( !g_pThisDungeon->IsStadium() || g_pMainPlayer->m_dwGuildWarFlag != G_W_F_OBSERVER )
			if ( !BeltProcForInterface() )	return 0;

		break;

	case 15: // 가디언 
		GuardianProcForInterface();
		break;
	case 23:	// 가디언 정보창 버튼
		if(g_Mouse.bLDown)
		{
			SetPointer(__MOUSE_POINTER_BUTTONCLICK__);
		}
		else
		{
			SetPointer(__MOUSE_POINTER_BUTTON__);
		}
		break;
	case 16:	// 트레이드 //
		{
			if(g_Mouse.bLDown)
			{
				m_bUIChk[0] = TRUE;
				SetRender(SPR_OBJ_UI_TRADE1, FALSE);
				SetRender(SPR_OBJ_UI_TRADE2, FALSE);
				SetRender(SPR_OBJ_UI_TRADE3, TRUE);
				SetPointer(__MOUSE_POINTER_BUTTONCLICK__);				
			}
			else if(!g_Mouse.bLDown && !m_bUp)
			{
				m_bUIChk[0] = TRUE;
				SetRender(SPR_OBJ_UI_TRADE1, FALSE);
				SetRender(SPR_OBJ_UI_TRADE2, TRUE);
				SetRender(SPR_OBJ_UI_TRADE3, FALSE);
				SetRender(SPR_OBJ_UI_SHOP1, TRUE);
				SetRender(SPR_OBJ_UI_SHOP2, FALSE);
				SetRender(SPR_OBJ_UI_SHOP3, FALSE);
				SetPointer(__MOUSE_POINTER_BUTTON__);
			}
		}
		break;
	case 17:	// 노점상 //
		{
			if(g_Mouse.bLDown)
			{
				m_bUIChk[2] = TRUE;
				SetRender(SPR_OBJ_UI_SHOP1, FALSE);
				SetRender(SPR_OBJ_UI_SHOP2, FALSE);
				SetRender(SPR_OBJ_UI_SHOP3, TRUE);
				SetPointer(__MOUSE_POINTER_BUTTONCLICK__);
			}
			else if(!g_Mouse.bLDown)
			{
				m_bUIChk[2] = TRUE;
				SetRender(SPR_OBJ_UI_SHOP1, FALSE);
				SetRender(SPR_OBJ_UI_SHOP2, TRUE);
				SetRender(SPR_OBJ_UI_SHOP3, FALSE);
				SetRender(SPR_OBJ_UI_TRADE1, TRUE);
				SetRender(SPR_OBJ_UI_TRADE2, FALSE);
				SetRender(SPR_OBJ_UI_TRADE3, FALSE);
				SetPointer(__MOUSE_POINTER_BUTTON__);
			}
		}
		break;
	case 18:	// 앉기 //
		{
			if(g_Mouse.bLDown)
			{
				m_bUIChk[1] = TRUE;
				SetRender(SPR_OBJ_UI_REST1, FALSE);
				SetRender(SPR_OBJ_UI_REST2, FALSE);
				SetRender(SPR_OBJ_UI_REST3, TRUE);
				SetPointer(__MOUSE_POINTER_BUTTONCLICK__);

#ifdef	DEVELOP_MODE

				if(g_pMainPlayer->GetStatus()==UNIT_STATUS_NORMAL)
				{
				}
#endif
			}
			else if(!g_Mouse.bLDown)
			{
				m_bUIChk[1] = TRUE;
				SetRender(SPR_OBJ_UI_REST1, FALSE);
				SetRender(SPR_OBJ_UI_REST2, TRUE);
				SetRender(SPR_OBJ_UI_REST3, FALSE);
				SetPointer(__MOUSE_POINTER_BUTTON__);
			}			
		}
		break;
	case 19:	// 1:1 대결 //
		{
			if(g_Mouse.bLDown)
			{
				m_bUIChk[3] = TRUE;
				SetRender(SPR_OBJ_UI_MATCH1, FALSE);
				SetRender(SPR_OBJ_UI_MATCH2, FALSE);
				SetRender(SPR_OBJ_UI_MATCH3, TRUE);
				SetPointer(__MOUSE_POINTER_BUTTONCLICK__);
			}
			else if(!g_Mouse.bLDown && !m_bUp)
			{
				m_bUIChk[3] = TRUE;
				SetRender(SPR_OBJ_UI_MATCH1, FALSE);
				SetRender(SPR_OBJ_UI_MATCH2, TRUE);
				SetRender(SPR_OBJ_UI_MATCH3, FALSE);
				SetPointer(__MOUSE_POINTER_BUTTON__);
			}
		}
#ifdef VER_RIVAL_GUILD_WAR	//kjk
	case 20:
		{
			if(g_Mouse.bLDown)
			{
				SetPointer(__MOUSE_POINTER_BUTTONCLICK__);
			}
			else if(!g_Mouse.bLDown && !m_bUp)
			{
				SetPointer(__MOUSE_POINTER_BUTTON__);
			}
		}
		break;
#endif
	case 0:
	case 1:

		if(nRt==1)
			if(!g_Mouse.bLDown)
				SetPointer(__MOUSE_POINTER_DEFAULT__);
					
		SetRender(BUTTON_OBJ_ITEM1, FALSE);
		SetRender(BUTTON_OBJ_CHR1, FALSE);
		SetRender(BUTTON_OBJ_SKILL1, FALSE);
		SetRender(BUTTON_OBJ_GUILD1, FALSE);
		SetRender(BUTTON_OBJ_PARTY1, FALSE);
		SetRender(BUTTON_OBJ_CHAT1, FALSE);
		SetRender(BUTTON_OBJ_SYSTEM1, FALSE);				

		if(m_nPK==0)
		{
			SetRender(BUTTON_OBJ_PKOFF1, TRUE);
			SetRender(BUTTON_OBJ_PKOFF2, FALSE);
			SetRender(BUTTON_OBJ_PKON1, FALSE);
			SetRender(BUTTON_OBJ_PKON2, FALSE);
		}
		else
		{
			SetRender(BUTTON_OBJ_PKOFF1, FALSE);
			SetRender(BUTTON_OBJ_PKOFF2, FALSE);
			SetRender(BUTTON_OBJ_PKON1, TRUE);
			SetRender(BUTTON_OBJ_PKON2, FALSE);
		}
		break;
	}

	SetRender(BUTTON_OBJ_ITEM2, FALSE);
	SetRender(BUTTON_OBJ_CHR2, FALSE);
	SetRender(BUTTON_OBJ_SKILL2, FALSE);
	SetRender(BUTTON_OBJ_GUILD2, FALSE);
	SetRender(BUTTON_OBJ_PARTY2, FALSE);
	SetRender(BUTTON_OBJ_CHAT2, FALSE);
	SetRender(BUTTON_OBJ_SYSTEM2, FALSE);

	m_nChatChk		= 0;
	m_byOrderCheck	= 0;
	
	if(m_bUp)
		m_bSoundChk	= FALSE;

	if( g_pThisDungeon->IsStadium() && g_pMainPlayer->m_dwGuildWarFlag == G_W_F_OBSERVER )
		goto STATUS_CHECK;

	if( m_bUp && m_bDown && nRt > 0 )
	{			
		m_bSmall = FALSE;
		
		if ( nRt == 2 )				// Item Inventory Wnd
		{			
			if (g_pMainPlayer->m_bMatching)
				return 0;
			if(g_pMainPlayer->GetStatus()==UNIT_STATUS_PORTAL_MOVING)
				return 0;

			CItemWnd* pItemWnd = CItemWnd::GetInstance();			
			pItemWnd->SetActive(!pItemWnd->GetActive());
			
			m_byOrderCheck = ITEM_WND;
		}
		else if ( nRt == 3 )		// Character Info Wnd
		{			
			CCharWnd* pCharWnd = CCharWnd::GetInstance();
			pCharWnd->SetActive(!pCharWnd->GetActive());			

			m_byOrderCheck = CHAR_WND;
		}
		else if ( nRt == 4 )		// Skill Info Wnd
		{			
			CSkillWnd* pSkillWnd = CSkillWnd::GetInstance();			
			pSkillWnd->SetActive(!pSkillWnd->GetActive());			

			m_byOrderCheck = SKILL_WND;
		}
		else if ( nRt == 5 )		// Group Wnd
		{
			CGroupWnd* pGroupWnd = CGroupWnd::GetInstance();
			pGroupWnd->SetActive(!pGroupWnd->GetActive());			
		}
		else if ( nRt == 6 )		// Dungeon Info Wnd
		{		
			DungeonInfoProcForInterface();
		}
		else if ( nRt == 7 )		// Chatting Wnd
		{	
			ChattingProcForInterface();
		}	
		else if ( nRt == 8 )		// Game Option Wnd
		{			
			CGameMenuWnd* pGameMenuWnd = CGameMenuWnd::GetInstance();			
			pGameMenuWnd->SetActive(!pGameMenuWnd->GetActive());
			
			if(pGameMenuWnd->GetActive()==TRUE)
				pGameMenuWnd->OpenWnd();						
		}
		else if ( nRt == 13 )		// PK MODE ON / OFF
		{		
			(m_nPK==0) ? SendPKMode( TRUE ) : SendPKMode( FALSE );
		}
		else if ( nRt == 16 )		// TRADE
		{
			if(!g_pMainPlayer->m_bMatching)
			{
				SetPointer(__MOUSE_POINTER_GUARDIAN__);
				SetPointerChk( TRUE );
				m_bTrade	= TRUE;						
			}
			else
			{
				// "대결 중에는 사용할 수 없습니다."
				DisplayMessageAdd(g_Message[ETC_MESSAGE501].szMessage, 0xFFFF2CFF); 
			}

		}
		else if ( nRt == 17 )		// PLAYER SHOP
		{
			if ( !PlayerShopProcForInterface() )	return 0;
		}
		else if ( nRt == 18 )		// REST
		{		
			if ( !RestProcForInterface() )			return 0;	
		}
		else if ( nRt == 19 )		// MATCH
		{
			if ( !MatchProcForInterface() )			return 0;
		}
#ifdef VER_RIVAL_GUILD_WAR
		else if ( nRt == 20 )		// RIVAL GUILD WAR
		{
			if(g_pThisDungeon->IsKaien())
			{
				// "카이엔의 집에서는 대결을 할 수 없습니다."
				DisplayMessageAdd(g_Message[ETC_MESSAGE1241].szMessage, 0xFFFF0000); 
				return 0;
			}
			
			if(g_pMainPlayer->GetStatus()==UNIT_STATUS_PLAYER_SHOP)
			{
				// "노점중에는 사용할 수 없습니다."
				DisplayMessageAdd(g_Message[ETC_MESSAGE798].szMessage, 0xFFFF0000); 
				return 0;
			}
			else if(g_pMainPlayer->GetStatus()==UNIT_STATUS_PLAYER_REST)
			{
				// "앉기중에는 사용할 수 없습니다."
				DisplayMessageAdd(g_Message[ETC_MESSAGE797].szMessage, 0xFFFF0000); 
				return 0;
			}
			
			if (g_pMainPlayer->m_bMatching )
			{
				// "대전중엔 대전신청을 하실 수 없습니다."
				DisplayMessageAdd(g_Message[ETC_MESSAGE664].szMessage, 0xFFFF0000); 
				return 0;
			}
			
			SetPointer(__MOUSE_POINTER_GUARDIAN__);
			SetPointerChk( TRUE );

			m_bRGW = TRUE;
		}
#endif
		else if(nRt==76)
		{
			
		}
		// 스킬 선택 아이콘, 가디언 스킬 선택 아이콘이면 : 최덕석 2005.3.3
 		else if(nRt==9 || nRt==10 || nRt==21)	
		{
			CSkillWnd* pSkillWnd = CSkillWnd::GetInstance();
			
			if(nRt==9)
				pSkillWnd->m_bySkillIndex = 1;
			else if(nRt==10)
				pSkillWnd->m_bySkillIndex = 2;
			else if(nRt==21)
				pSkillWnd->m_bySkillIndex = 3;
			
			SetPointer(__MOUSE_POINTER_BUTTONCLICK__);
		}			
		else if(nRt>=11 && nRt<=12)
		{			
			CSkillWnd* pSkillWnd = CSkillWnd::GetInstance();
			
			if(nRt==11)
			{		
				if( g_Mouse.MousePos.x>=116 &&
					g_Mouse.MousePos.x<=116+32 &&
					g_Mouse.MousePos.y>=646 &&
					g_Mouse.MousePos.y<=646+32)
				{
					for(int i = 0; i < 2; i++)
					{
						if(g_pMainPlayer->GetSkillKind(BYTE(i))==__SKILL_ATTACK__)
							g_pMainPlayer->SetSkillChangeLR(__SKILL_NONE_SELECT__, BYTE(i));
					}
									
					if(pSkillWnd->m_bySkillIndex==1)
						g_pMainPlayer->SetSkillChangeLR(__SKILL_ATTACK__, 0);										
					if(pSkillWnd->m_bySkillIndex==2)
						g_pMainPlayer->SetSkillChangeLR(__SKILL_ATTACK__, 1);

					pSkillWnd->m_bySkillIndex = 0;					
				}
				else
				{
					if(pSkillWnd->m_bySkillIndex==1)
					{
						int nCount = 1;

						for(int i = 0; i < g_sSkillListManager.byLeftSkillCnt; i++)
						{
							if(g_pMainPlayer->GetSkillLevel(g_sSkillListManager.byLeftSkill[i])>0)
							{
								int nPosX = 116+32*(nCount%5);
								int nPosY = 640-32*(nCount/5);

								if(	g_Mouse.MousePos.x>=nPosX &&
									g_Mouse.MousePos.x<=nPosX+32 &&
									g_Mouse.MousePos.y>=nPosY &&
									g_Mouse.MousePos.y<=nPosY+32)
								{									
									for(int j = 0; j < 2; j++)
									{
										if(g_pMainPlayer->GetSkillKind(BYTE(j))==g_sSkillListManager.byLeftSkill[i])
											g_pMainPlayer->SetSkillChangeLR(__SKILL_NONE_SELECT__, BYTE(j));
									}
									g_pMainPlayer->SetSkillChangeLR(g_sSkillListManager.byLeftSkill[i], 0);
									pSkillWnd->m_bySkillIndex = 0;								
									break;
								}
								nCount++;
							}
						}
					}		
				}				
			}
			else
			{
				/*if(g_Mouse.MousePos.x>=116 && g_Mouse.MousePos.x<=116+32 && g_Mouse.MousePos.y>=646 && g_Mouse.MousePos.y<=646+32)
				{
					for(int i = 0; i < 2; i++)
					{
						if(g_pMainPlayer->GetSkillKind(i)==__SKILL_ATTACK__)
							g_pMainPlayer->SetSkillChangeLR(-1, i);
					}
									
					if(pSkillWnd->m_bySkillIndex==1)
						g_pMainPlayer->SetSkillChangeLR(__SKILL_ATTACK__, 0);										
					if(pSkillWnd->m_bySkillIndex==2)
						g_pMainPlayer->SetSkillChangeLR(__SKILL_ATTACK__, 1);

					pSkillWnd->m_bySkillIndex = 0;
				}
				else*/
				{
					if(pSkillWnd->m_bySkillIndex==2)
					{
						int nCount = 0;

						for(int i = 0; i < g_sSkillListManager.byRightSkillCnt; i++)
						{
							if(g_pMainPlayer->GetSkillLevel(g_sSkillListManager.byRightSkill[i])>0)
							{
								int nPosX = 116+32*(nCount%5);
								int nPosY = 640-32*(nCount/5);

								if( g_Mouse.MousePos.x>=nPosX &&
									g_Mouse.MousePos.x<=nPosX+32 &&
									g_Mouse.MousePos.y>=nPosY &&
									g_Mouse.MousePos.y<=nPosY+32)
								{
									for(int j = 0; j < 2; j++)
									{
										if(g_pMainPlayer->GetSkillKind(BYTE(j))==g_sSkillListManager.byRightSkill[i])
											g_pMainPlayer->SetSkillChangeLR(__SKILL_NONE_SELECT__, BYTE(j));
									}										
									g_pMainPlayer->SetSkillChangeLR(g_sSkillListManager.byRightSkill[i], 1);										
									pSkillWnd->m_bySkillIndex = 0;
									break;
								}
								nCount++;
							}
						}
					}
				}
			}
		}
		// 가디언 스킬 선택 : 최덕석 2005.3.3
		else if(nRt==22)
		{
			CSkillWnd* pSkillWnd = CSkillWnd::GetInstance();
			CMonster *pGuardian = g_pMainPlayer->m_pGuardian[0];
			
			if(pGuardian && pSkillWnd->m_bySkillIndex==3)
			{
				int nCount = 1;

				if( g_Mouse.MousePos.x>=116 &&
					g_Mouse.MousePos.x<=116+32 &&
					g_Mouse.MousePos.y>=646 &&
					g_Mouse.MousePos.y<=646+32)
				{
					pGuardian->SetSelectedSkill(0);
					pSkillWnd->m_bySkillIndex = 0;
				}

				for(int i = 0; i < MAX_GUARDIAN_USE_SKILL; i++)
				{
					if(pGuardian->GetSkillLevel(pGuardian->m_Skill[i].wSkill)>0)
					{
						int nPosX = 116+32*(nCount%5);
						int nPosY = 640-32*(nCount/5);

						if( g_Mouse.MousePos.x>=nPosX &&
							g_Mouse.MousePos.x<=nPosX+32 &&
							g_Mouse.MousePos.y>=nPosY &&
							g_Mouse.MousePos.y<=nPosY+32)
						{										
							pGuardian->SetSelectedSkill(pGuardian->m_Skill[i].wSkill);
							pSkillWnd->m_bySkillIndex = 0;
							break;
						}
						nCount++;
					}
				}
			}
		}
		// 가디언 정보 창
		else if(nRt == 23)
		{
			CGuardianWnd* pGuardianWnd = CGuardianWnd::GetInstance();		
					
			if(pGuardianWnd->GetActive()==FALSE)
				pGuardianWnd->SetActive();
			else					
				pGuardianWnd->SetActive(FALSE);

			m_byOrderCheck = GUARDIAN_WND;
		}
		/*else if(nRt==77)
		{
			CTWS_PARTY_JOIN pPacket;
			memset(pPacket.szName, 0, sizeof(pPacket.szName));
			__lstrcpyn(pPacket.szName, m_szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);			
			pPacket.bErrCode	= 0;
			pPacket.dwUserIndex	= m_dwUserIndex;			
			g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_WORLD);
			
			char szInfo[0xff]={0,};
			wsprintf(szInfo, g_Message[ETC_MESSAGE387].szMessage, m_szCharacterName);	// MSG_ID : 387 ; %s 님에게 파티 메세지 보냈습니다.
			DisplayMessageAdd(szInfo, 0xFF00FF1E);

			UserClickInterface(FALSE, 0);
			CUserInterface::GetInstance()->m_bTimeChk		= FALSE;
			CUserInterface::GetInstance()->m_bUserInterface = FALSE;
		}*/
		/*else if(nRt==79)
		{
			if(g_pMainPlayer->m_byRank>=__GUILD_MASTER__ && g_pMainPlayer->m_byRank<=__GUILD_USER__)
			{
				char			szInfo[0xff]={0,};
				CGuildRankWnd*	pGuildRankWnd	= CGuildRankWnd::GetInstance();
				CGuildExWnd*	pGuildExWnd		= CGuildExWnd::GetInstance();

				if(pGuildRankWnd->GetActive()==FALSE && pGuildExWnd->GetActive()==FALSE)
				{				
					if(g_pGuildInfo.byCount<__MAX_GUILD_USER__)
					{
						CTWS_GUILDJOIN_MESSAGE	pGuildJoin;
						memset(pGuildJoin.szGuildName, 0, sizeof(pGuildJoin.szGuildName));

						pGuildJoin.bType = g_pMainPlayer->m_byType;
						__lstrcpyn(pGuildJoin.szGuildName, g_pMainPlayer->m_szGuildName, MAX_GUILD_NAME_REAL_LENGTH);
						__lstrcpyn(pGuildJoin.szCharacterName, m_szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);
						g_pNet->SendMsg((char*)&pGuildJoin, pGuildJoin.GetPacketSize(), SERVER_INDEX_WORLD);
						
						if(g_pMainPlayer->m_byType==__GCTYPE_GUILD__)
							wsprintf(szInfo, g_Message[ETC_MESSAGE388].szMessage, m_szCharacterName);	// MSG_ID : 388 ; %s 님에게 길드 가입 메세지를 보냈습니다.
						else
							wsprintf(szInfo, g_Message[ETC_MESSAGE389].szMessage, m_szCharacterName);	// MSG_ID : 389 ; %s 님에게 클랜 가입 메세지를 보냈습니다.

						DisplayMessageAdd(szInfo, 0xFFFFFF00);
					}
					else
					{
						wsprintf(szInfo, g_Message[ETC_MESSAGE390].szMessage);		// MSG_ID : 390 ; 더이상 가입을 받을수 없습니다.				
						DisplayMessageAdd(szInfo, 0xFFFFFF00);
						_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_Camera.v3CameraPos, FALSE);
					}
				}
				else
				{
					if(pGuildExWnd->m_byGuildType==__GCTYPE_GUILD__)
						wsprintf(szInfo, g_Message[ETC_MESSAGE391].szMessage);	// MSG_ID : 391 ; 길드를 생성중입니다.
					else
						wsprintf(szInfo, g_Message[ETC_MESSAGE392].szMessage);	// MSG_ID : 392 ; 클랜를 생성중입니다.
				}			
			}
			else
			{
				DisplayMessageAdd(g_Message[ETC_MESSAGE393].szMessage, 0xFFFFFF00);	// MSG_ID : 393 ; 권한이 없습니다.
				_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_Camera.v3CameraPos, FALSE);
			}			
			UserClickInterface(FALSE, 0);
			CUserInterface::GetInstance()->m_bTimeChk		= FALSE;
			CUserInterface::GetInstance()->m_bUserInterface = FALSE;
		}*/
		m_bUp	= FALSE;
		m_bDown = FALSE;
	}
	
	if(g_Mouse.bLDown && nRt>0)
	{
		if(nRt!=15)
		{
			CInterface::GetInstance()->SetIconChk(FALSE);		
			SetPointer(__MOUSE_POINTER_BUTTONCLICK__);
		}
			
		m_bClick	= TRUE;
		m_bDown		= TRUE;

		switch ( nRt )
		{
			case 2:		SetRender(BUTTON_OBJ_ITEM2, TRUE);			break;
			case 3:		SetRender(BUTTON_OBJ_CHR2, TRUE);			break;
			case 4:		SetRender(BUTTON_OBJ_SKILL2, TRUE);			break;
			case 5:		SetRender(BUTTON_OBJ_GUILD2, TRUE);			break;
			case 6:		SetRender(BUTTON_OBJ_PARTY2, TRUE);			break;
			case 7:		SetRender(BUTTON_OBJ_CHAT2, TRUE);			break;
			case 8:		SetRender(BUTTON_OBJ_SYSTEM2, TRUE);		break;
		}
/*		for more simple code
		if(nRt==2)
			SetRender(BUTTON_OBJ_ITEM2, TRUE);
		else if(nRt==3)
			SetRender(BUTTON_OBJ_CHR2, TRUE);	
		else if(nRt==4)
			SetRender(BUTTON_OBJ_SKILL2, TRUE);
		else if(nRt==5)
			SetRender(BUTTON_OBJ_GUILD2, TRUE);
		else if(nRt==6)
			SetRender(BUTTON_OBJ_PARTY2, TRUE);
		else if(nRt==7)
			SetRender(BUTTON_OBJ_CHAT2, TRUE);
		else if(nRt==8)
			SetRender(BUTTON_OBJ_SYSTEM2, TRUE);		//*/

		SetRender(BUTTON_OBJ_ITEM1, FALSE);
		SetRender(BUTTON_OBJ_CHR1, FALSE);
		SetRender(BUTTON_OBJ_SKILL1, FALSE);
		SetRender(BUTTON_OBJ_GUILD1, FALSE);
		SetRender(BUTTON_OBJ_PARTY1, FALSE);
		SetRender(BUTTON_OBJ_CHAT1, FALSE);
		SetRender(BUTTON_OBJ_SYSTEM1, FALSE);			
		return 1;
	}

STATUS_CHECK:	

	if(m_byOrderCheck!=0)
		return 1;

	m_pStatusWnd->CheckInterface();

	return 0;
}

void CUserInterface::Render()
{	
	if( g_pThisDungeon->IsStadium() && g_pMainPlayer->m_dwGuildWarFlag == G_W_F_OBSERVER )
		return;

	for(int i = 0; i < m_bySpriteDataCnt; i++)
	{
		if(m_sSprite_Data[i].dwId!=0xff && m_sSprite_Data[i].bRender)
		{
			LP_SPRITE_DATA lpSpriteData = g_pInterfaceSprHash->GetData(m_sSprite_Data[i].dwSprId);
			
			if(lpSpriteData)
			{	
				VECTOR2 vPos;

				if(!m_sSprite_Data[i].bPos)
				{
					vPos.x	= m_sSprite_Data[i].fPosX;
					vPos.y	= m_sSprite_Data[i].fPosY;
				}
				else
				{
					vPos.x	= m_fPosX + m_sSprite_Data[i].fPosX;
					vPos.y	= m_fPosZ + m_sSprite_Data[i].fPosY;
				}								

				int nOrder = 0;

				if(m_sSprite_Data[i].bOrder)	
					nOrder = __ORDER_INTERFACE_START__ + m_byOrder * 10 + m_sSprite_Data[i].nOrder;
				else
					nOrder = m_sSprite_Data[i].nOrder;

				if(m_sSprite_Data[i].v2Scaling.x==1.0f && m_sSprite_Data[i].v2Scaling.y==1.0f)
				{
					g_pRenderer->RenderSprite(lpSpriteData->pSpr, NULL
						, m_sSprite_Data[i].fRot, &vPos, NULL, m_sSprite_Data[i].dwAlpha, nOrder, 0);
				}
				else
				{
					g_pRenderer->RenderSprite(lpSpriteData->pSpr, &m_sSprite_Data[i].v2Scaling
						, m_sSprite_Data[i].fRot, &vPos, NULL, m_sSprite_Data[i].dwAlpha, nOrder, 0);				
				}
			}
		}
	}
}

void CUserInterface::PartyUserName()
{
	BYTE byIndex = 0;

	ListNode<PARTY_USER>* pNode = g_pPartyUserHash->GetHead();

	while(pNode)
	{
		LP_PARTY_USER sPartyNode = pNode->pData;

		CUser* pUser = g_pUserHash->GetData(sPartyNode->dwUserIndex);
		
		if(pUser)
		{
			int nSize	= lstrlen(pUser->m_szName);
			RenderFont(pUser->m_szName, 3, 3+nSize*7, 116+(byIndex*36)+4, 130+(byIndex*36)+4, __ORDER_USERINTERFACE_START_+3);								
			byIndex++;
		}

		pNode = pNode->pNext;
	}	
}

void CUserInterface::RenderText()
{
	m_pStatusWnd->RenderText();

	if( g_pThisDungeon->IsStadium() && g_pMainPlayer->m_dwGuildWarFlag == G_W_F_OBSERVER )
		return;

	DengeonEnStart();
	DengeonExpStart();
	DengeonManaStart();

	DengeonHpDec();
	DengeonHpInc();
	DengeonHpDef();

	DengeonManaDec();
	DengeonManaInc();
	DengeonManaDef();

	DengeonExpDefInc();

	DengeonCastingDef();

	RenderMonsterBar();
	RenderUserBar();
	
	PartyUserName();

	RenderGuardianItem();

	CItemWnd::GetInstance()->RenderItemBelt();
	CSkillWnd::GetInstance()->RenderSkillIcon();

	if(g_pMainPlayer->m_GuardianItem.m_wItemID!=0)
	{		
		char	szInfo[0xff]={0,};
		WORD	wCnt	= 10;
		int		nIndex	= 0;

		for(int j = 0; j < 2; j++)
		{
			if(g_pMainPlayer->m_GuardianItem.m_Item_Supplies.bQuantity>=wCnt)
			{
				nIndex++;
				wCnt=WORD(wCnt*10);
			}			
		}
		
		if(g_ItemMoveManager.GetNewItemMoveMode())
		{
			if(	g_ItemMoveManager.GetNativeSrc() == ITEM_NATIVE_GUARDIAN )
			{
				
			}
		}
		else
		{
			wsprintf(szInfo, "%d", g_pMainPlayer->m_GuardianItem.GetQuantity());							

			if(!IsEmptyString(szInfo))
			{
				RenderFont(szInfo, 93+21-(nIndex*7), 93+21+7, 96, 110, __ORDER_USERINTERFACE_START_+2);
			}
		}		
	}

	if(m_bGuardian==TRUE)
	{		
		if(!IsEmptyString(m_szGuardianName))
		{
 			RenderFont(m_szGuardianName, 2, 100, 85, 99, __ORDER_USERINTERFACE_START_+2);
		}
	}

	if(CSkillWnd::GetInstance()->m_bySkillIndex==0)
	{				
		if(g_Mouse.MousePos.x>=132 && g_Mouse.MousePos.x<=132+32 && g_Mouse.MousePos.y>=733 && g_Mouse.MousePos.y<=733+32)
		{			
			if(InterfaceCollision(INTERFACE_USER, 132, 164, 733, 765)==FALSE)
				return;
							
			if(g_pMainPlayer->GetSkillKind(0)==__SKILL_ATTACK__)
			{
				RenderFont(g_Message[ETC_MESSAGE377].szMessage, g_Mouse.MousePos.x-15, g_Mouse.MousePos.x+100, g_Mouse.MousePos.y-10, g_Mouse.MousePos.y+4, __ORDER_ITEM_DESC__+2);
				
				CInterface::GetInstance()->RenderInfoBox(float(g_Mouse.MousePos.x-22-5), float(g_Mouse.MousePos.y-16), 44+10, 24, __ORDER_ITEM_DESC__);
			}
			else if(g_pMainPlayer->GetSkillKind(0)!=__SKILL_NONE_SELECT__)
			{				
				if(g_pEffectLayer->m_Effect[g_pMainPlayer->GetSkillKind(0)].wProperty!=0)
				{				
					if(g_pEffectLayer->m_Effect[g_pMainPlayer->GetSkillKind(0)].bSkillType!=0)
					{
						if(!IsEmptyString(g_pEffectLayer->m_Effect[g_pMainPlayer->GetSkillKind(0)].szName))
						{
							RenderFont(g_pEffectLayer->m_Effect[g_pMainPlayer->GetSkillKind(0)].szName, 
								g_Mouse.MousePos.x-g_pEffectLayer->m_nEffectSize[g_pMainPlayer->GetSkillKind(0)]/2*6, 
								g_Mouse.MousePos.x+100, 
								g_Mouse.MousePos.y-10, 
								g_Mouse.MousePos.y+4, 
								__ORDER_ITEM_DESC__+2);

							CInterface::GetInstance()->RenderInfoBox(float(g_Mouse.MousePos.x-g_pEffectLayer->m_nEffectSize[g_pMainPlayer->GetSkillKind(0)]/2*7-5), float(g_Mouse.MousePos.y-16), float(g_pEffectLayer->m_nEffectSize[g_pMainPlayer->GetSkillKind(0)]/2*8*2+10), 24, __ORDER_ITEM_DESC__);
						}												
					}
				}								
			}				
		}
		else if(g_Mouse.MousePos.x>=245 && g_Mouse.MousePos.x<=245+32 && g_Mouse.MousePos.y>=733 && g_Mouse.MousePos.y<=733+32)
		{
			if(InterfaceCollision(INTERFACE_USER, 245, 277, 733, 765)==FALSE)
				return;

			if(g_pMainPlayer->GetSkillKind(1)==__SKILL_ATTACK__)
			{
				RenderFont(g_Message[ETC_MESSAGE377].szMessage, g_Mouse.MousePos.x-12, g_Mouse.MousePos.x+100, g_Mouse.MousePos.y-10, g_Mouse.MousePos.y+4, __ORDER_ITEM_DESC__+2);
				
				CInterface::GetInstance()->RenderInfoBox(float(g_Mouse.MousePos.x-22-5), float(g_Mouse.MousePos.y-16), 44+10, 24, __ORDER_ITEM_DESC__);								
			}			
			else if(g_pMainPlayer->GetSkillKind(1)!=__SKILL_NONE_SELECT__)
			{
				if(g_pEffectLayer->m_Effect[g_pMainPlayer->GetSkillKind(1)].wProperty!=0)
				{				
					if(g_pEffectLayer->m_Effect[g_pMainPlayer->GetSkillKind(1)].bSkillType!=0)
					{
						if(!IsEmptyString(g_pEffectLayer->m_Effect[g_pMainPlayer->GetSkillKind(1)].szName))
						{						
							RenderFont(g_pEffectLayer->m_Effect[g_pMainPlayer->GetSkillKind(1)].szName, 
								g_Mouse.MousePos.x-g_pEffectLayer->m_nEffectSize[g_pMainPlayer->GetSkillKind(1)]/2*6, 
								g_Mouse.MousePos.x+100, 
								g_Mouse.MousePos.y-10, 
								g_Mouse.MousePos.y+4, __ORDER_ITEM_DESC__+2);
							
							CInterface::GetInstance()->RenderInfoBox(float(g_Mouse.MousePos.x-g_pEffectLayer->m_nEffectSize[g_pMainPlayer->GetSkillKind(1)]/2*7-5), float(g_Mouse.MousePos.y-16), float(g_pEffectLayer->m_nEffectSize[g_pMainPlayer->GetSkillKind(1)]/2*8*2+10), 24, __ORDER_ITEM_DESC__);
						}																		
					}
				}				
			}
		}
		else		
			return;
	}
	else
	{
//		RECT Pos;				

		int nX	= (g_Mouse.MousePos.x-116)/33;
		int nY	= (678-g_Mouse.MousePos.y)/33;

		BOOL bError = FALSE;
		
		if(InterfaceCollision(INTERFACE_USER, nX*33+116, nX*33+149, 678-nY*33, 710-nY*32)==FALSE)
			return;

		if(678-g_Mouse.MousePos.y<0)
			bError = TRUE;
		if(g_Mouse.MousePos.x-116<0 && nY==0)
			bError = TRUE;
		if(nX<0)
			bError = TRUE;
		if(nX>=5)
			bError = TRUE;
		if(nY<0)
			bError = TRUE;

		if(bError==TRUE)
		{			
			return;
		}		

		if((nY*5+nX==0) && (CSkillWnd::GetInstance()->m_bySkillIndex==1))
		{
			RenderFont(g_Message[ETC_MESSAGE377].szMessage, g_Mouse.MousePos.x-12, g_Mouse.MousePos.x+100, g_Mouse.MousePos.y-10, g_Mouse.MousePos.y+4, __ORDER_ITEM_DESC__+2);

			CInterface::GetInstance()->RenderInfoBox(float(g_Mouse.MousePos.x-22-5), float(g_Mouse.MousePos.y-16), 44+10, 24, __ORDER_ITEM_DESC__);
			return;
		}
		
		if(CSkillWnd::GetInstance()->m_bySkillIndex==1)
		{		
			BYTE	bySkillValue	= 0;
			int		nCount			= 0;
			
			for(int i = 1; i < g_sSkillListManager.byLeftSkillCnt+1; i++)
			{
				if(g_pMainPlayer->GetSkillLevel(g_sSkillListManager.byLeftSkill[i])>0)
				{	
					nCount++;

					if(nY*5+nX==nCount)
						bySkillValue = g_sSkillListManager.byLeftSkill[i];					
				}
			}
			
			if(nCount>=nY*5+nX)
			{
				if(!IsEmptyString(g_pEffectLayer->m_Effect[bySkillValue].szName))
				{
					RenderFont(g_pEffectLayer->m_Effect[bySkillValue].szName, g_Mouse.MousePos.x-g_pEffectLayer->m_nEffectSize[bySkillValue]/2*6, g_Mouse.MousePos.x+100, g_Mouse.MousePos.y-10, g_Mouse.MousePos.y+4, __ORDER_ITEM_DESC__+2);
					
					CInterface::GetInstance()->RenderInfoBox(float(g_Mouse.MousePos.x-g_pEffectLayer->m_nEffectSize[bySkillValue]/2*7-5), float(g_Mouse.MousePos.y-16), float(g_pEffectLayer->m_nEffectSize[bySkillValue]/2*8*2+10), 24, __ORDER_ITEM_DESC__);
				}								
				return;
			}			
		}
		else if(CSkillWnd::GetInstance()->m_bySkillIndex==2)
		{
			BYTE	bySkillValue	= 1;
			int		nCount			= 0;
			
			for(int i = 1; i < g_sSkillListManager.byRightSkillCnt; i++)
			{
				if(g_pMainPlayer->GetSkillLevel(g_sSkillListManager.byRightSkill[i])>0)
				{						
					if(nY*5+nX==nCount)
						bySkillValue = g_sSkillListManager.byRightSkill[i];

					nCount++;
				}
			}
			
			int nSize = 0;

			if(nCount>nY*5+nX)
			{
				if(!IsEmptyString(g_pEffectLayer->m_Effect[bySkillValue].szName))
				{
					nSize = lstrlen(g_pEffectLayer->m_Effect[bySkillValue].szName);

					RenderFont(g_pEffectLayer->m_Effect[bySkillValue].szName, g_Mouse.MousePos.x-nSize/2*6, g_Mouse.MousePos.x+100, g_Mouse.MousePos.y-10, g_Mouse.MousePos.y+4, __ORDER_ITEM_DESC__+2);
															
					CInterface::GetInstance()->RenderInfoBox(float(g_Mouse.MousePos.x-g_pEffectLayer->m_nEffectSize[bySkillValue]/2*7-5), float(g_Mouse.MousePos.y-16), float(g_pEffectLayer->m_nEffectSize[bySkillValue]/2*8*2+10), 24, __ORDER_ITEM_DESC__);
				}								
				return;
			}		
		}
		else if(CSkillWnd::GetInstance()->m_bySkillIndex==3 && g_pMainPlayer->m_pGuardian[0])
		{
			CMonster *pGuardian = g_pMainPlayer->m_pGuardian[0];
			BYTE	bySkillValue	= 0;
			int		nCount			= 1;
			
			for(int i = 0; i < MAX_GUARDIAN_USE_SKILL; i++)
			{
				if(pGuardian->GetSkillLevel(pGuardian->m_Skill[i].wSkill) > 0)
				{						
					if(nY*5+nX==nCount)
						bySkillValue = pGuardian->m_Skill[i].wSkill;

					nCount++;
				}
			}
			
			int nSize = 0;

			if(nCount>nY*5+nX)
			{
				if(!IsEmptyString(g_pEffectLayer->m_Effect[bySkillValue].szName))
				{
					char szTmp[100];
					sprintf(szTmp, "%s Lv. %d", g_pEffectLayer->m_Effect[bySkillValue].szName, pGuardian->GetSkillLevel(bySkillValue));
					nSize = lstrlen(szTmp);

					RenderFont(szTmp, g_Mouse.MousePos.x-nSize/2*6, g_Mouse.MousePos.x+100, g_Mouse.MousePos.y-10, g_Mouse.MousePos.y+4, __ORDER_ITEM_DESC__+2);
															
					CInterface::GetInstance()->RenderInfoBox(float(g_Mouse.MousePos.x-g_pEffectLayer->m_nEffectSize[bySkillValue]/2*7-5) - 15, float(g_Mouse.MousePos.y-16), float(g_pEffectLayer->m_nEffectSize[bySkillValue]/2*8*2+10) + 35, 24, __ORDER_ITEM_DESC__);
				}
				return;
			}		
		}
		return;	
	}
}

void CUserInterface::DengeonEnStart()
{
	for(int i = 0; i < 2; i++)
	{
		if(!m_bEn[i]) 
		{	
			float fSize = (g_pMainPlayer->m_wHP>g_pMainPlayer->m_wMaxHP) ?
				1.0f : (float)g_pMainPlayer->m_wHP/(float)g_pMainPlayer->m_wMaxHP;

			if(m_fEnIndex[i]<fSize*300)
			{
				if(m_fEnIndex[i]+m_fEnSpeed[i]>fSize*300)
					m_fEnIndex[i] = fSize*300;
				else
					m_fEnIndex[i] += m_fEnSpeed[i];
				
				if(i==0)
					SetScalingObj(SPR_OBJ_EN1, m_fEnIndex[i]/2, 1.0);
				else
					SetScalingObj(SPR_OBJ_EN2, m_fEnIndex[i]/2, 1.0);
			}
			else
			{
				if(i==1)
					m_fEnIncDec = m_fEnIndex[i];

				m_bEn[i] = TRUE;
			}
		}
	}
}

void CUserInterface::DengeonExpStart()
{
	if(!m_bExp) //kjk(41115)
	{	
//		float fMaxExp = (float)GetExpTableOfLevel(OBJECT_TYPE_PLAYER, g_pMainPlayer->m_dwLevel+1)-(float)GetExpTableOfLevel(OBJECT_TYPE_PLAYER, g_pMainPlayer->m_dwLevel-1);
//		float fCurExp = (float)GetExpTableOfLevel(OBJECT_TYPE_PLAYER, g_pMainPlayer->m_dwLevel)-(float)GetExpTableOfLevel(OBJECT_TYPE_PLAYER, g_pMainPlayer->m_dwLevel-1);
		float fMaxExp = (float)GetExpTableOfLevel(OBJECT_TYPE_PLAYER, g_pMainPlayer->m_dwLevel+1)-(float)GetExpTableOfLevel(OBJECT_TYPE_PLAYER, g_pMainPlayer->m_dwLevel);
		float fCurExp = (float)g_pMainPlayer->m_dwExp - (float)GetExpTableOfLevel(OBJECT_TYPE_PLAYER, g_pMainPlayer->m_dwLevel);
		
		if(fCurExp>fMaxExp)
			fCurExp = fMaxExp;

		if(m_fExpIndex<fCurExp/fMaxExp*300)
		{
			if(m_fExpIndex+m_fExpSpeed>fCurExp/fMaxExp*300)				
				m_fExpIndex = fCurExp/fMaxExp*300;
			else
				m_fExpIndex += m_fExpSpeed;						
			
			SetScalingObj(SPR_OBJ_EXP, m_fExpIndex/2.0f, 1.0);
		}
		else
		{
			if(fCurExp/fMaxExp*300==0)
				SetRender(SPR_OBJ_EXP, FALSE);

			m_bExp = TRUE;
		}
	}
}

void CUserInterface::DengeonManaStart()
{
	for(int i = 0; i < 2; i++)
	{
		if(!m_bMana[i]) 
		{				
			float fSize = (g_pMainPlayer->m_wMP>g_pMainPlayer->m_wMaxMP) ?
				1.0f : (float)g_pMainPlayer->m_wMP/(float)g_pMainPlayer->m_wMaxMP;

			if(m_fManaIndex[i]<fSize*300)
			{
				if(m_fManaIndex[i]+m_fManaSpeed[i]>fSize*300)
					m_fManaIndex[i] = fSize*300;
				else
					m_fManaIndex[i] += m_fManaSpeed[i];
				
				if(i==0)
				{
					SetPosObjX(SPR_OBJ_MANA1, 1023-m_fManaIndex[i]);
					SetScalingObj(SPR_OBJ_MANA1, m_fManaIndex[i]/2, 1.0);
				}
				else
				{
					SetPosObjX(SPR_OBJ_MANA2, 1023-m_fManaIndex[i]);
					SetScalingObj(SPR_OBJ_MANA2, m_fManaIndex[i]/2, 1.0);
				}
			}
			else
			{
				if(i==1)
					m_fManaIncDec = m_fManaIndex[i];

				m_bMana[i] = TRUE;
			}
		}
	}
}

void CUserInterface::DengeonCastingDef()
{
	if((float)g_pMainPlayer->m_fCurCoolPoint<=(float)g_pMainPlayer->m_fMaxCoolPoint)
	{
		SetScalingObj(SPR_OBJ_CAST, (float)g_pMainPlayer->m_fCurCoolPoint/(float)g_pMainPlayer->m_fMaxCoolPoint*300/2, 1.0);
		SetPosObjX(SPR_OBJ_CAST, 1023-(float)g_pMainPlayer->m_fCurCoolPoint/(float)g_pMainPlayer->m_fMaxCoolPoint*300);
	}

	if(g_pMainPlayer->m_fCurCoolPoint==0)
		SetRender(SPR_OBJ_CAST, FALSE);
	else if(g_pMainPlayer->m_fCurCoolPoint>=0)
		SetRender(SPR_OBJ_CAST, TRUE);
}

void CUserInterface::SetCool()
{
	float fSize = (g_pMainPlayer->m_fCurCoolPoint>g_pMainPlayer->m_fMaxCoolPoint) ?
		1.0f : (float)g_pMainPlayer->m_fCurCoolPoint/(float)g_pMainPlayer->m_fMaxCoolPoint;

	SetScalingObj(SPR_OBJ_CAST, fSize *300/2, 1.0);
	SetPosObjX(SPR_OBJ_CAST, 1023-fSize *300);
}

void CUserInterface::DengeonHpDec()
{		
	if(m_bEnDec)
	{	
		float fSize = (g_pMainPlayer->m_wHP>g_pMainPlayer->m_wMaxHP) ? 
			1.0f : (float)g_pMainPlayer->m_wHP/(float)g_pMainPlayer->m_wMaxHP;

		SetScalingObj(SPR_OBJ_EN1, fSize*300/2, 1.0);

		if(m_fEnIncDec>fSize*300)
		{
			if(m_fEnIncDec-m_fEnDecSpeed<fSize*300)
				m_fEnIncDec = fSize*300;
			else
				m_fEnIncDec -= m_fEnDecSpeed;

			SetScalingObj(SPR_OBJ_EN2, m_fEnIncDec/2.0f, 1.0);
		}
		else
		{
			m_fEnIncDec	= fSize*300;
			m_bEnDec	= FALSE;
		}
	}
}

void CUserInterface::DengeonHpInc()
{
	if(m_bEnInc)
	{	
		float fSize = (g_pMainPlayer->m_wHP>g_pMainPlayer->m_wMaxHP) ?
			1.0f : (float)g_pMainPlayer->m_wHP/(float)g_pMainPlayer->m_wMaxHP;

		SetScalingObj(SPR_OBJ_EN2, fSize*300/2, 1.0);

		if(m_fEnIncDec<fSize*300)
		{
			if(m_fEnIncDec+m_fEnDecSpeed>fSize*300)
				m_fEnIncDec = fSize*300;
			else
				m_fEnIncDec += m_fEnDecSpeed;

			SetScalingObj(SPR_OBJ_EN1, m_fEnIncDec/2.0f, 1.0);
		}
		else
		{
			m_fEnIncDec	= fSize*300;
			m_bEnInc	= FALSE;
		}
		SetRender(SPR_OBJ_EN1, TRUE);
		SetRender(SPR_OBJ_EN2, TRUE);
	}
}

void CUserInterface::DengeonHpDef()
{
	if(m_bEnDef)
	{
		float fSize = (g_pMainPlayer->m_wHP>g_pMainPlayer->m_wMaxHP) ?
			1.0f : (float)g_pMainPlayer->m_wHP/(float)g_pMainPlayer->m_wMaxHP;

		SetScalingObj(SPR_OBJ_EN1, fSize*300/2, 1.0);
		SetScalingObj(SPR_OBJ_EN2, fSize*300/2, 1.0);
		m_fEnIncDec = fSize*300;
		m_bEnDef	= FALSE;
	}
}

void CUserInterface::DengeonManaDec()
{
	if(m_bManaDec)
	{	
		float fSize = (g_pMainPlayer->m_wMP>g_pMainPlayer->m_wMaxMP) ?
			1.0f : (float)g_pMainPlayer->m_wMP/(float)g_pMainPlayer->m_wMaxMP;

		SetScalingObj(SPR_OBJ_MANA1, fSize*300/2, 1.0);
		SetPosObjX(SPR_OBJ_MANA1, 1023-fSize*300);

		if(m_fManaIncDec>fSize*300)
		{
			if(m_fManaIncDec-m_fManaDecSpeed<fSize*300)
				m_fManaIncDec = fSize*300;
			else
				m_fManaIncDec -= m_fManaDecSpeed;

			SetScalingObj(SPR_OBJ_MANA2, m_fManaIncDec/2.0f, 1.0);
			SetPosObjX(SPR_OBJ_MANA2, 1023-m_fManaIncDec);
		}
		else
		{
			m_fManaIncDec	= fSize*300;
			m_bManaDec		= FALSE;			
		}
	}
}

void CUserInterface::DengeonManaInc()
{
	if(m_bManaInc)
	{	
		float fSize = (g_pMainPlayer->m_wMP>g_pMainPlayer->m_wMaxMP) ?
			1.0f : (float)g_pMainPlayer->m_wMP/(float)g_pMainPlayer->m_wMaxMP;

		SetScalingObj(SPR_OBJ_MANA2, fSize*300/2, 1.0);
		SetPosObjX(SPR_OBJ_MANA2, 1023-fSize*300);

		if(m_fManaIncDec<fSize*300)
		{
			if(m_fManaIncDec+m_fManaDecSpeed>fSize*300)
				m_fManaIncDec = fSize*300;
			else
				m_fManaIncDec += m_fManaDecSpeed;

			SetScalingObj(SPR_OBJ_MANA1, m_fManaIncDec/2.0f, 1.0);
			SetPosObjX(SPR_OBJ_MANA1, 1023-m_fManaIncDec);
		}
		else
		{
			m_fManaIncDec	= fSize*300;
			m_bManaInc		= FALSE;
		}
		SetRender(SPR_OBJ_MANA1, TRUE);
		SetRender(SPR_OBJ_MANA2, TRUE);
	}
}

void CUserInterface::DengeonManaDef()
{
	if(m_bManaDef)
	{
		float fSize = (g_pMainPlayer->m_wMP>g_pMainPlayer->m_wMaxMP) ?
			1.0f : (float)g_pMainPlayer->m_wMP/(float)g_pMainPlayer->m_wMaxMP;

		SetScalingObj(SPR_OBJ_MANA1, fSize*300/2, 1.0);
		SetScalingObj(SPR_OBJ_MANA2, fSize*300/2, 1.0);
		SetPosObjX(SPR_OBJ_MANA1, 1023-fSize*300);
		SetPosObjX(SPR_OBJ_MANA2, 1023-fSize*300);
		m_fManaIncDec = fSize*300;
		m_bManaDef	= FALSE;
	}
}

void CUserInterface::DengeonExpDefInc()
{
	if(m_bExpIncDef)
	{
		float fMaxExp = 
			(float)GetExpTableOfLevel(OBJECT_TYPE_PLAYER, g_pMainPlayer->m_dwLevel+1) - 
			(float)GetExpTableOfLevel(OBJECT_TYPE_PLAYER, g_pMainPlayer->m_dwLevel);
//		float fCurExp = (float)g_pMainPlayer->m_dwExp;
		float fCurExp = (float)g_pMainPlayer->m_dwExp - (float)GetExpTableOfLevel(OBJECT_TYPE_PLAYER, g_pMainPlayer->m_dwLevel);
//			(float)GetExpTableOfLevel(OBJECT_TYPE_PLAYER, g_pMainPlayer->m_dwLevel) -
//			(float)GetExpTableOfLevel(OBJECT_TYPE_PLAYER, g_pMainPlayer->m_dwLevel-1);	

		if(fMaxExp < fCurExp)
		{
			fCurExp = fMaxExp;
		}
		
		SetScalingObj(SPR_OBJ_EXP, fCurExp / fMaxExp * 300/2, 1.0);
		SetRender(SPR_OBJ_EXP, TRUE);
		m_bExpIncDef = FALSE;
	}	
}

void CUserInterface::SetDengeonHp(WORD wHp)
{
	if(g_pMainPlayer->m_wHP>wHp)
		SetDengeonHpDec();
	else
		SetDengeonHpInc();

	if(wHp>g_pMainPlayer->m_wMaxHP)	
		g_pMainPlayer->m_wHP = g_pMainPlayer->m_wMaxHP;
	else if(wHp<0)
		g_pMainPlayer->m_wHP = 0;
	else
		g_pMainPlayer->m_wHP = wHp;
}

void CUserInterface::SetDengeonMp(WORD wMp)
{
	if(g_pMainPlayer->m_wMP>wMp)
		SetDengeonManaDec();
	else
		SetDengeonManaInc();

	if(wMp>g_pMainPlayer->m_wMaxMP)	
		g_pMainPlayer->m_wMP = g_pMainPlayer->m_wMaxMP;
	else if(wMp<0)
		g_pMainPlayer->m_wMP = 0;
	else
		g_pMainPlayer->m_wMP = wMp;
}

void CUserInterface::SetDengeonExpDefInc()
{
	m_bExpIncDef = TRUE;
}

void CUserInterface::SetDengeonHpDef()
{
	m_bEnDef = TRUE;
}

void CUserInterface::SetDengeonHpDec()
{
	m_bEnDec = TRUE;
}

void CUserInterface::SetDengeonHpInc()
{
	m_bEnInc = TRUE;
}

void CUserInterface::SetDengeonManaDec()
{
	m_bManaDec = TRUE;
}

void CUserInterface::SetDengeonManaInc()
{
	m_bManaInc = TRUE;
}

void CUserInterface::SetDengeonManaDef()
{
	m_bManaDef = TRUE;
}

void CUserInterface::SetActive(BOOL bActive)
{
	m_bActive =  bActive;


}

void CUserInterface::RenderMonsterBar()
{
	//	sung-han 2004-12-30 : pvp 에서 캐릭터 에너지바와 엘레맨탈 에너지 바가 겹치는 부분 처리
	//-----------------------------------------------------------------------------------------
	if( m_pMatchUser && m_bMatchUserHPShowEnable )
		return;
	//-----------------------------------------------------------------------------------------

	if(m_bMonster)
	{
		m_dwMonCurTime	= timeGetTime();

		if(m_dwMonCurTime-m_dwMonPrevTime>3000)
		{
			SetRender(SPR_OBJ_MONSTERHP_BAR1, FALSE);
			SetRender(SPR_OBJ_MONSTERHP_BAR2, FALSE);
			SetRender(SPR_OBJ_MONSTER_GAGE1, FALSE);
			SetRender(SPR_OBJ_MONSTER_GAGE2, FALSE);
			m_bMonster = FALSE;
			return;
		}
		
		if(!IsEmptyString(m_szMosterName))
		{
			int nSize = lstrlen(m_szMosterName);
			RenderFont(m_szMosterName, 421, 421+(nSize*7)+10, 4, 18, __ORDER_USERINTERFACE_START_+3);			
		}
	}	
}

void CUserInterface::RenderUserBar()
{
	if( m_pMatchUser && m_bMatchUserHPShowEnable )
	{
		SetRender(SPR_OBJ_USERHP_BAR1, TRUE);
		SetRender(SPR_OBJ_USERHP_BAR2, TRUE);
		SetRender(SPR_OBJ_USER_GAGE1, TRUE);
		SetRender(SPR_OBJ_USER_GAGE2, TRUE);
		
		if(!IsEmptyString(m_szMatchUserName))
		{	
			int nSize = lstrlen(m_szMatchUserName);
			RenderFont(m_szMatchUserName, 421, 421+(nSize*7)+10, 4, 18, __ORDER_USERINTERFACE_START_+3);			
		}
	}
	else
	{
		CloseUserBar();
	}
}

void CUserInterface::CloseUserBar()
{
	SetRender(SPR_OBJ_USERHP_BAR1, FALSE);
	SetRender(SPR_OBJ_USERHP_BAR2, FALSE);
	SetRender(SPR_OBJ_USER_GAGE1, FALSE);
	SetRender(SPR_OBJ_USER_GAGE2, FALSE);
}

void CUserInterface::OpenUserBar(char *pszName, DWORD dwHP, DWORD dwMaxHP)
{
	// 데이터 //
	__lstrcpyn(m_szMatchUserName, pszName, lstrlen(pszName));
	m_fUserHp		= (float)dwHP;
	m_fUserMaxHp	= (float)dwMaxHP;
//	m_bMonster		= FALSE;
	m_dwUserPrevTime= timeGetTime();
	m_dwUserCurTime	= timeGetTime();
	// 리소트 //
	SetScalingObj(SPR_OBJ_USER_GAGE1, (m_fUserHp/m_fUserMaxHp)*100.0f, 1.0f);
	SetScalingObj(SPR_OBJ_USER_GAGE2, 100.0f, 1.0f);

	SetRender(SPR_OBJ_USERHP_BAR1, TRUE);
	SetRender(SPR_OBJ_USERHP_BAR2, TRUE);
	SetRender(SPR_OBJ_USER_GAGE1, TRUE);
	SetRender(SPR_OBJ_USER_GAGE2, TRUE);
	
}

void CUserInterface::OpenMonsterBar(char* pszName, DWORD dwHp, DWORD dwMaxHp)
{
//	sung-han 2004-12-30 : pvp 에서 캐릭터 에너지바와 엘레맨탈 에너지 바가 겹치는 부분 처리
//-----------------------------------------------------------------------------------------
	if( m_pMatchUser && m_bMatchUserHPShowEnable ) 
	{
		CloseUserBar();
		m_bMatchUserHPShowEnable = FALSE;
	}
//-----------------------------------------------------------------------------------------
	// 데이터 //
	lstrcpy(m_szMosterName, pszName);
	m_fMonHp		= (float)dwHp;
	m_fMonMaxHp		= (float)dwMaxHp;
	m_bMonster		= TRUE;
	m_dwMonPrevTime	= timeGetTime();
	m_dwMonCurTime	= timeGetTime();
	// 리소트 //
	SetScalingObj(SPR_OBJ_MONSTER_GAGE1, (m_fMonHp/m_fMonMaxHp)*100.0f, 1.0f);
	SetScalingObj(SPR_OBJ_MONSTER_GAGE2, 100.0f, 1.0f);

	SetRender(SPR_OBJ_MONSTERHP_BAR1, TRUE);
	SetRender(SPR_OBJ_MONSTERHP_BAR2, TRUE);
	SetRender(SPR_OBJ_MONSTER_GAGE1, TRUE);
	SetRender(SPR_OBJ_MONSTER_GAGE2, TRUE);	
}

void CUserInterface::RenderGuardianItem()
{		
	if(g_pMainPlayer->m_GuardianItem.m_wItemID!=0)
	{
		if(g_ItemMoveManager.GetNewItemMoveMode())
		{
			if(	g_ItemMoveManager.GetNativeSrc() == ITEM_NATIVE_GUARDIAN )
			{
				return;
			}
		}

		LP_ITEM_RESOURCE_EX lpItemResourceEx = 
			g_pItemResourceHash->GetData(g_pMainPlayer->m_GuardianItem.m_wItemID);

		if(lpItemResourceEx)
		{
			VECTOR2	vPos = {93.f, 76.f};
			g_pRenderer->RenderSprite(lpItemResourceEx->pSpr, NULL, 0.0f, &vPos, NULL, 0xffffffff, __ORDER_USERINTERFACE_START_+1, RENDER_TYPE_DISABLE_TEX_FILTERING);
		}
	}
}

void CUserInterface::SetMousePointerPos(float fPosX, float fPosZ)
{	

}


void CUserInterface::SetPKMode(BOOL bPK)
{
	m_nPK = bPK;
	if(bPK)
	{		
		SetRender(BUTTON_OBJ_PKOFF1, FALSE);
		SetRender(BUTTON_OBJ_PKOFF2, FALSE);
		SetRender(BUTTON_OBJ_PKON2, TRUE);	
	}
	else
	{		
		SetRender(BUTTON_OBJ_PKON1, FALSE);
		SetRender(BUTTON_OBJ_PKON1, FALSE);
		SetRender(BUTTON_OBJ_PKOFF2, TRUE);
	}
}

void CUserInterface::SetPointer(BYTE byPointerType)
{
	if(CInterface::GetInstance()->GetIconChk() == FALSE|| GetGameStatus() == UPDATE_GAME_PLAY)
	{

		if(  (g_pThisDungeon != NULL) &&  (g_pThisDungeon->IsStadium() && g_pMainPlayer->m_dwGuildWarFlag == G_W_F_OBSERVER ) )
			SetMousePointerType( __MOUSE_POINTER_DEFAULT__ );
		else
			SetMousePointerType(byPointerType);

		CInterface::GetInstance()->SetIconChk(TRUE);
	}
	
	if(GetGameStatus()==UPDATE_GAME_CHAR_SELECT || GetGameStatus()==UPDATE_GAME_WORLD )
	{
		SetMousePointerType(byPointerType);
	}
}


void CUserInterface::SetMousePointerType(BYTE byPointerType)
{
	if(m_bPointerChk==TRUE)
	{
		if(byPointerType!=__MOUSE_POINTER_GUARDIAN__ && byPointerType!=__MOUSE_POINTER_GUARDIANCLICK__)
		{
			return;			
		}
	}

	m_byPointerType = byPointerType;

	if(!g_hCursor[ byPointerType ] || byPointerType >= __MAX_MOUSE_POINTER__)	
	{
		asm_int3();
		return;
	}

	if(GetCursor() != g_hCursor[ byPointerType ])
	{
		if(g_ItemMoveManager.GetNewItemMoveMode() )
			if( g_ItemMoveManager.GetMouseItemID() )
				if( byPointerType != __MOUSE_POINTER_ITEMCLICK__ )
					byPointerType = __MOUSE_POINTER_ITEMCLICK__;		

		SetCursor( g_hCursor[ byPointerType ] );					
	}
	
	CInterface::GetInstance()->SetIconChk(TRUE);
}


BOOL CUserInterface::PosCollision(int nPosX1, int nPosX2, int nPosY1, int nPosY2)
{
	CInterface* pInterface = CInterface::GetInstance();	
	
	for(int i = 0; i < CUR_INTERFACE; i++)
	{
		if(i==COMMAND_WND || i==INTERFACE_USER)
			continue;	
	
		if(pInterface->m_pMenu[i]->GetActive()==TRUE)
		{
			int nStartPosX	= pInterface->m_pMenu[i]->GetPosX();
			int nStartPosY	= pInterface->m_pMenu[i]->GetPosY();
			int nEndPosX	= pInterface->m_pMenu[i]->GetPosX() + pInterface->m_nSize[i*2];
			int nEndPosY	= pInterface->m_pMenu[i]->GetPosY() + pInterface->m_nSize[i*2+1];
						
			if((nPosX1>=nStartPosX && nPosX1<=nEndPosX) || (nPosX2>=nStartPosX && nPosX2<=nEndPosX))
			{		
				if((nPosY1>=nStartPosY && nPosY1<=nEndPosY) || (nPosY2>=nStartPosY && nPosY2<=nEndPosY))
					return FALSE;
			}			
		}
	}	
	return TRUE;
}

BOOL CUserInterface::InterfaceCollision(int nIndex, int nPosX1, int nPosX2, int nPosY1, int nPosY2)
{
	CInterface* pInterface = CInterface::GetInstance();	
	
	for(int i = 0; i < CUR_INTERFACE; i++)
	{
		int nOrderDst = pInterface->m_pMenu[i]->GetOrderIndex();
		int nOrderSrc = pInterface->m_pMenu[nIndex]->GetOrderIndex();

		if(i==nIndex)
			continue;
		if(i==COMMAND_WND || i==INTERFACE_USER)
			continue;
		if(nOrderDst<nOrderSrc)
			continue;		
	
		if(pInterface->m_pMenu[i]->GetActive()==TRUE)
		{
			int nStartPosX	= pInterface->m_pMenu[i]->GetPosX();
			int nStartPosY	= pInterface->m_pMenu[i]->GetPosY();
			int nEndPosX	= pInterface->m_pMenu[i]->GetPosX() + pInterface->m_nSize[i*2];
			int nEndPosY	= pInterface->m_pMenu[i]->GetPosY() + pInterface->m_nSize[i*2+1];
						
			if((nPosX1>=nStartPosX && nPosX1<=nEndPosX) || (nPosX2>=nStartPosX && nPosX2<=nEndPosX))
			{		
				if((nPosY1>=nStartPosY && nPosY1<=nEndPosY) || (nPosY2>=nStartPosY && nPosY2<=nEndPosY))
					return FALSE;
			}			
		}
	}	
	return TRUE;
}

// seven
// 대중중일때 퀵슬롯의 쓰지못하게 가린다.
// 이는 가리는 효과만 있다.
void CUserInterface::EnableQuickSlot(BOOL bEnAble)
{
	if (!bEnAble)
	{
		// 가려라.
		SetRender(SPR_OBJ_UI_QUICKSLOT_INABLE1, TRUE);
		SetRender(SPR_OBJ_UI_QUICKSLOT_INABLE2, TRUE);
		SetRender(SPR_OBJ_UI_QUICKSLOT_INABLE3, TRUE);
		SetRender(SPR_OBJ_UI_QUICKSLOT_INABLE4, TRUE);
		SetRender(SPR_OBJ_UI_QUICKSLOT_INABLE5, TRUE);
		SetRender(SPR_OBJ_UI_QUICKSLOT_INABLE6, TRUE);
		SetRender(SPR_OBJ_UI_QUICKSLOT_INABLE7, TRUE);
		SetRender(SPR_OBJ_UI_QUICKSLOT_INABLE8, TRUE);
	}
	else
	{
		// 보여라.
		SetRender(SPR_OBJ_UI_QUICKSLOT_INABLE1, FALSE);
		SetRender(SPR_OBJ_UI_QUICKSLOT_INABLE2, FALSE);
		SetRender(SPR_OBJ_UI_QUICKSLOT_INABLE3, FALSE);
		SetRender(SPR_OBJ_UI_QUICKSLOT_INABLE4, FALSE);
		SetRender(SPR_OBJ_UI_QUICKSLOT_INABLE5, FALSE);
		SetRender(SPR_OBJ_UI_QUICKSLOT_INABLE6, FALSE);
		SetRender(SPR_OBJ_UI_QUICKSLOT_INABLE7, FALSE);
		SetRender(SPR_OBJ_UI_QUICKSLOT_INABLE8, FALSE);
	}
}


void CUserInterface::OpenGuardianDescriptionWnd(INT64 i64ID
											, char* pszName
											, DWORD dwGauge
											, DWORD dwMaxGauge)
{
	// Interface update		
	
	m_bGuardian = TRUE;
	m_i64ActiveGuardianItemID = i64ID;
	lstrcpy(m_szGuardianName, pszName);			

	SetPosObj(SPR_OBJ_GUARDIAN_WND1, 0, 69);			
	SetRender(SPR_OBJ_GUARDIAN_WND1, TRUE);			
	SetGauge(dwGauge, dwMaxGauge);

	SetPosObj(SPR_OBJ_GUARDIAN_EN, 0, 97);
	SetRender(SPR_OBJ_GUARDIAN_EN, TRUE);

	CItemWnd* pItemWnd = CItemWnd::GetInstance();
	pItemWnd->m_bGuarInterface = TRUE;

	SetRender(SPR_OBJ_UI_GUARDIAN_STATUS, TRUE);

	if(pItemWnd->GetActive())
	{
		pItemWnd->SetRender(SPR_ITEMWND_GUARDIAN_ON1, TRUE);
		pItemWnd->SetRender(SPR_ITEMWND_GUARDIAN_ON2, FALSE);
		pItemWnd->SetRender(SPR_ITEMWND_GUARDIAN_OFF1, FALSE);
		pItemWnd->SetRender(SPR_ITEMWND_GUARDIAN_OFF2, FALSE);			
	}		
}


void CUserInterface::CloseGuardianDescriptionWnd()
{	
	m_bGuardian = FALSE;
	m_i64ActiveGuardianItemID = 0;
	
	SetRender(SPR_OBJ_GUARDIAN_EN, FALSE);
	SetRender(SPR_OBJ_GUARDIAN_WND1, FALSE);

	CItemWnd* pItemWnd = CItemWnd::GetInstance();
	pItemWnd->m_bGuarInterface = FALSE;

	SetRender(SPR_OBJ_UI_GUARDIAN_STATUS, FALSE);

	if(pItemWnd->GetActive())
	{
		pItemWnd->SetRender(SPR_OBJ_ITEMWND_GUARDIAN_ON1, FALSE);
		pItemWnd->SetRender(SPR_OBJ_ITEMWND_GUARDIAN_ON2, FALSE);
		pItemWnd->SetRender(SPR_OBJ_ITEMWND_GUARDIAN_OFF1, TRUE);
		pItemWnd->SetRender(SPR_OBJ_ITEMWND_GUARDIAN_OFF2, FALSE);
	}
}


void CUserInterface::SetGauge(DWORD dwGauge, DWORD dwMaxGauge)
{
	SetScalingObj(SPR_OBJ_GUARDIAN_EN, ((float)dwGauge/(float)dwMaxGauge)*46.0f, 1.0f);
}
// --
//==========================================================//
// End.														//
//==========================================================//