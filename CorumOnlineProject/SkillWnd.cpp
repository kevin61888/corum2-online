//======================================================//
// Code by Jang.							2002.12.23	//
//======================================================//
#include "SkillWnd.h"
#include "SpriteManager.h"
#include "GameControl.h"
#include "Interface.h"
#include "UserInterface.h"
#include "ChatListWnd.h"
#include "DefResource.h"
#include "message.h"
#include "CResNameResolver.h"
#include "InitGame.h"
#include "NetworkClient.h"
#include "effect.h"
#include "CodeFun.h"

CSkillWnd* CSkillWnd::c_pThis = NULL;

//======================================================//
// Construction/Destrution.								//
//======================================================//
CSkillWnd::CSkillWnd()
{ 	
	m_byBitClassType	= 0;
	m_bySkillIndex		= 0;
	m_bySkillType		= CLASS_TYPE_WARRIOR;		
	m_bBtnChk[0]		= FALSE;
	m_bBtnChk[1]		= FALSE;
	m_pLineSpr			= NULL;
}

CSkillWnd::~CSkillWnd()
{
}
//======================================================//
// Class Member Function.								//
//======================================================//
BOOL CSkillWnd::Init()
{		
	InitSpriteData();
	InitCheckInterface();
	
	InsertData(SPR_OBJ_SKILL_WND1, SPR_INTERFACE_SKILL_WND1, 0, 0, 1.0f, 1.0f, 0);
	InsertData(SPR_OBJ_SKILL_WND2, SPR_INTERFACE_SKILL_WND2, 64, 0, 1.0f, 1.0f, 0);
	InsertData(SPR_OBJ_SKILL_CLOSE1, SPR_INTERFACE_CLOSE1, 306, 4, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_SKILL_CLOSE2, SPR_INTERFACE_CLOSE2, 306, 4, 1.0f, 1.0f, 1);	
	InsertData(SPR_OBJ_SKILL_MANA1, SPR_INTERFACE_SKILL_MANA1, 7, 0, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_SKILL_MANA2, SPR_INTERFACE_SKILL_MANA2, 7, 0, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_SKILL_MANA3, SPR_INTERFACE_SKILL_MANA3, 7, 0, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_SKILL_AURA1, SPR_INTERFACE_SKILL_AURA1, 7, 0, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_SKILL_AURA2, SPR_INTERFACE_SKILL_AURA2, 7, 0, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_SKILL_AURA3, SPR_INTERFACE_SKILL_AURA3, 7, 0, 1.0f, 1.0f, 1);	
	InsertData(SPR_OBJ_SKILL_DIVINE1, SPR_INTERFACE_SKILL_DIVINE1, 7, 0, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_SKILL_DIVINE2, SPR_INTERFACE_SKILL_DIVINE2, 7, 0, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_SKILL_DIVINE3, SPR_INTERFACE_SKILL_DIVINE3, 7, 0, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_SKILL_SUMMON1, SPR_INTERFACE_SKILL_SUMMON1, 7, 0, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_SKILL_SUMMON2, SPR_INTERFACE_SKILL_SUMMON2, 7, 0, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_SKILL_SUMMON3, SPR_INTERFACE_SKILL_SUMMON3, 7, 0, 1.0f, 1.0f, 1);		
	InsertData(SPR_OBJ_SKILL_CHAKRA1, SPR_INTERFACE_SKILL_CHAKRA1, 7, 0, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_SKILL_CHAKRA2, SPR_INTERFACE_SKILL_CHAKRA2, 7, 0, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_SKILL_CHAKRA3, SPR_INTERFACE_SKILL_CHAKRA3, 7, 0, 1.0f, 1.0f, 1);	

	m_byBitClassType = (BYTE)pow( 2, ( g_pMainPlayer->m_wClass - 1 ) );	// modified by minjin. for more simple sentense
/*	switch(g_pMainPlayer->m_wClass)
	{
	case CLASS_TYPE_WARRIOR:
		m_byBitClassType = 1;
		break;
	case CLASS_TYPE_PRIEST:
		m_byBitClassType = 2;
		break;
	case CLASS_TYPE_SUMMONER:						
		m_byBitClassType = 4;
		break;
	case CLASS_TYPE_HUNTER:
		m_byBitClassType = 8;
		break;
	case CLASS_TYPE_WIZARD:
		m_byBitClassType = 16;
		break;
	}
	*/

	for(BYTE i = 0; i < 25; i++)
	{
		InsertData(SPR_OBJ_SKILL_UP1+i*2, SPR_INTERFACE_SKILL_UP1, m_fPosX, m_fPosZ, 1.0f, 1.0f, 3);
		InsertData(SPR_OBJ_SKILL_UP1+i*2+1, SPR_INTERFACE_SKILL_UP2, m_fPosX, m_fPosZ, 1.0f, 1.0f, 3);
	}

	SetTypePos();
	
	//	int nOrder = GetStartOrder();	Disabled by minjin. for Removing Unrefferneced Variable

	// Skill Icon Loading
	for(i = 0; i < 5; i++)
	{
		CreateSkillResourceSpr( g_sSkillListManager.pSkillList[i].pActiveList );		// Active Skill
		CreateSkillResourceSpr( g_sSkillListManager.pSkillList[i].pMasteryList );		// Mastery Skill
		CreateSkillResourceSpr( g_sSkillListManager.pSkillList[i].pPassiveList );		// Passive Skill
		CreateSkillResourceSpr( g_sSkillListManager.pSkillList[i].pOverDriveList );		// OverDrive Skill
		
/*		// modified by minjin. for more simple sentense
		// Active //
		POSITION_ pos = g_sSkillListManager.pSkillList[i].pActiveList->GetHeadPosition();	

		while(pos)
		{
			LP_SKILL_RESOURCE_EX lpSkillResourceNode = (LP_SKILL_RESOURCE_EX)g_sSkillListManager.pSkillList[i].pActiveList->GetNext(pos);
					
			nPosX = lpSkillResourceNode->wIndex%8*32;
			nPosY = lpSkillResourceNode->wIndex/8*32;
			lpSkillResourceNode->pSpr = g_pRenderer->CreateSpriteObject(GetFile(lpSkillResourceNode->szFileName, DATA_TYPE_UI), nPosX, nPosY, 32, 32, 0);
			nPosX = lpSkillResourceNode->wIndexAct%8*32;
			nPosY = lpSkillResourceNode->wIndexAct/8*32;			
			lpSkillResourceNode->pSprAct = g_pRenderer->CreateSpriteObject(GetFile(lpSkillResourceNode->szFileNameAct, DATA_TYPE_UI), nPosX, nPosY, 32, 32, 0);

			g_sSkillListManager.pSpr[lpSkillResourceNode->wId]	= lpSkillResourceNode->pSpr;
		}
		// Mastery //
		pos = g_sSkillListManager.pSkillList[i].pMasteryList->GetHeadPosition();	

		while(pos)
		{
			LP_SKILL_RESOURCE_EX lpSkillResourceNode = (LP_SKILL_RESOURCE_EX)g_sSkillListManager.pSkillList[i].pMasteryList->GetNext(pos);
					
			nPosX = lpSkillResourceNode->wIndex%8*32;
			nPosY = lpSkillResourceNode->wIndex/8*32;
			lpSkillResourceNode->pSpr = g_pRenderer->CreateSpriteObject(GetFile(lpSkillResourceNode->szFileName, DATA_TYPE_UI), nPosX, nPosY, 32, 32, 0);
			nPosX = lpSkillResourceNode->wIndexAct%8*32;
			nPosY = lpSkillResourceNode->wIndexAct/8*32;			
			lpSkillResourceNode->pSprAct = g_pRenderer->CreateSpriteObject(GetFile(lpSkillResourceNode->szFileNameAct, DATA_TYPE_UI), nPosX, nPosY, 32, 32, 0);													

			g_sSkillListManager.pSpr[lpSkillResourceNode->wId]	= lpSkillResourceNode->pSpr;
		}
		// Passive //
		pos = g_sSkillListManager.pSkillList[i].pPassiveList->GetHeadPosition();	

		while(pos)
		{
			LP_SKILL_RESOURCE_EX lpSkillResourceNode = (LP_SKILL_RESOURCE_EX)g_sSkillListManager.pSkillList[i].pPassiveList->GetNext(pos);
					
			nPosX = lpSkillResourceNode->wIndex%8*32;
			nPosY = lpSkillResourceNode->wIndex/8*32;
			lpSkillResourceNode->pSpr = g_pRenderer->CreateSpriteObject(GetFile(lpSkillResourceNode->szFileName, DATA_TYPE_UI), nPosX, nPosY, 32, 32, 0);
			nPosX = lpSkillResourceNode->wIndexAct%8*32;
			nPosY = lpSkillResourceNode->wIndexAct/8*32;			
			lpSkillResourceNode->pSprAct = g_pRenderer->CreateSpriteObject(GetFile(lpSkillResourceNode->szFileNameAct, DATA_TYPE_UI), nPosX, nPosY, 32, 32, 0);													

			g_sSkillListManager.pSpr[lpSkillResourceNode->wId]	= lpSkillResourceNode->pSpr;
		}
		// OverDrive //
		pos = g_sSkillListManager.pSkillList[i].pOverDriveList->GetHeadPosition();	

		while(pos)
		{
			LP_SKILL_RESOURCE_EX lpSkillResourceNode = (LP_SKILL_RESOURCE_EX)g_sSkillListManager.pSkillList[i].pOverDriveList->GetNext(pos);
					
			nPosX = lpSkillResourceNode->wIndex%8*32;
			nPosY = lpSkillResourceNode->wIndex/8*32;
			lpSkillResourceNode->pSpr = g_pRenderer->CreateSpriteObject(GetFile(lpSkillResourceNode->szFileName, DATA_TYPE_UI), nPosX, nPosY, 32, 32, 0);
			nPosX = lpSkillResourceNode->wIndexAct%8*32;
			nPosY = lpSkillResourceNode->wIndexAct/8*32;			
			lpSkillResourceNode->pSprAct = g_pRenderer->CreateSpriteObject(GetFile(lpSkillResourceNode->szFileNameAct, DATA_TYPE_UI), nPosX, nPosY, 32, 32, 0);													

			g_sSkillListManager.pSpr[lpSkillResourceNode->wId]	= lpSkillResourceNode->pSpr;
		}	//*/
	}
	
	InsertCheckInterface(306, 4, 318, 16, 2, CHECK_CLOSE);
	InsertCheckInterface(0, 0, 306, 20, 3, CHECK_MOVEWND);
	InsertCheckInterface(7, 99, 70, 116, 4, CHECK_BTN);
	InsertCheckInterface(7, 122, 70, 139, 5, CHECK_BTN);
	InsertCheckInterface(7, 145, 70, 162, 6, CHECK_BTN);
	InsertCheckInterface(7, 168, 70, 185, 7, CHECK_BTN);
	InsertCheckInterface(7, 191, 70, 208, 8, CHECK_BTN);
	InsertCheckInterface(61, 44, 93, 60, 9, CHECK_BTN);
	
	for(i = 0; i < 14; i++)
		InsertCheckInterface(112+(i/7)*82, 124+(i%7)*42, 144+(i/7)*82, 140+(i%7)*42, 10+i, CHECK_BTN);
	for(i = 0; i < 7; i++)
		InsertCheckInterface(276, 124+i*42, 308, 140+i*42, 24+i, CHECK_BTN);
	for(i = 0; i < 3; i++)
		InsertCheckInterface(112+i*82, 460, 144+i*82, 476, 31+i, CHECK_BTN);
				
	// 테두리 //
	m_pLineSpr		= g_pRenderer->CreateSpriteObject(GetFile("menu_4.tif", DATA_TYPE_UI), 71, 33, 71, 38, 0);
	m_pAttackSpr	= g_pRenderer->CreateSpriteObject(GetFile("skill_icon1.tga", DATA_TYPE_UI), 0, 0, 32, 32, 0);
	
	SetSkillUpPos();

	m_bInit	= TRUE;

	return TRUE;
}

// 스킬 스프라이트 정보를 등록한다
void CSkillWnd::CreateSkillResourceSpr( COnlyList* pSkillList )
{
	int nPosX = 0;
	int nPosY = 0;

	POSITION_ pos = pSkillList->GetHeadPosition();

	while(pos)
	{
		LP_SKILL_RESOURCE_EX lpSkillResourceNode = (LP_SKILL_RESOURCE_EX)pSkillList->GetNext(pos);
				
		nPosX = lpSkillResourceNode->wIndex % 8 * SKILL_ICON_SIZE;
		nPosY = lpSkillResourceNode->wIndex / 8 * SKILL_ICON_SIZE;
		lpSkillResourceNode->pSpr		= g_pRenderer->CreateSpriteObject( GetFile(lpSkillResourceNode->szFileName, DATA_TYPE_UI)
																			, nPosX, nPosY, SKILL_ICON_SIZE, SKILL_ICON_SIZE, 0 );

		nPosX = lpSkillResourceNode->wIndexAct % 8 * SKILL_ICON_SIZE;
		nPosY = lpSkillResourceNode->wIndexAct / 8 * SKILL_ICON_SIZE;			

		lpSkillResourceNode->pSprAct	= g_pRenderer->CreateSpriteObject( GetFile(lpSkillResourceNode->szFileNameAct, DATA_TYPE_UI)
																			, nPosX, nPosY, SKILL_ICON_SIZE, SKILL_ICON_SIZE, 0 );

		g_sSkillListManager.pSpr[lpSkillResourceNode->wId]	= lpSkillResourceNode->pSpr;
	}
}

// 스킬 스프라이트 정보를 제거한다
void CSkillWnd::ReleaseSkillResourceSpr( COnlyList* pSkillList )
{
	POSITION_ pos = pSkillList->GetHeadPosition();	

	while(pos)
	{
		LP_SKILL_RESOURCE_EX lpSkillResourceNode = (LP_SKILL_RESOURCE_EX)pSkillList->GetNext(pos);
				
		if(lpSkillResourceNode->pSpr)
		{
			lpSkillResourceNode->pSpr->Release();
			lpSkillResourceNode->pSpr = NULL;
		}
		if(lpSkillResourceNode->pSprAct)
		{
			lpSkillResourceNode->pSprAct->Release();
			lpSkillResourceNode->pSprAct = NULL;
		}
	}
}

void CSkillWnd::SetTypePos()
{
	switch(g_pMainPlayer->m_wClass)
	{
	case CLASS_TYPE_WARRIOR:
		SetPosObjY(SPR_OBJ_SKILL_AURA1, 99);
		SetPosObjY(SPR_OBJ_SKILL_AURA2, 99);
		SetPosObjY(SPR_OBJ_SKILL_AURA3, 99);
		m_bClassType[0] = CLASS_TYPE_WARRIOR;
		break;
	case CLASS_TYPE_PRIEST:
		SetPosObjY(SPR_OBJ_SKILL_DIVINE1, 99);
		SetPosObjY(SPR_OBJ_SKILL_DIVINE2, 99);
		SetPosObjY(SPR_OBJ_SKILL_DIVINE3, 99);
		m_bClassType[0] = CLASS_TYPE_PRIEST;
		break;
	case CLASS_TYPE_SUMMONER:
		SetPosObjY(SPR_OBJ_SKILL_SUMMON1, 99);
		SetPosObjY(SPR_OBJ_SKILL_SUMMON2, 99);
		SetPosObjY(SPR_OBJ_SKILL_SUMMON3, 99);
		m_bClassType[0] = CLASS_TYPE_SUMMONER;
		break;
	case CLASS_TYPE_HUNTER:		
		SetPosObjY(SPR_OBJ_SKILL_CHAKRA1, 99);
		SetPosObjY(SPR_OBJ_SKILL_CHAKRA2, 99);
		SetPosObjY(SPR_OBJ_SKILL_CHAKRA3, 99);
		m_bClassType[0] = CLASS_TYPE_HUNTER;
		break;
	case CLASS_TYPE_WIZARD:
		SetPosObjY(SPR_OBJ_SKILL_MANA1, 99);
		SetPosObjY(SPR_OBJ_SKILL_MANA2, 99);
		SetPosObjY(SPR_OBJ_SKILL_MANA3, 99);		
		m_bClassType[0] = CLASS_TYPE_WIZARD;
		break;
	}

	BYTE byIndex = 0;

	if(g_pMainPlayer->m_wClass!=CLASS_TYPE_WARRIOR)
	{
		SetPosObjY(SPR_OBJ_SKILL_AURA1, 122);
		SetPosObjY(SPR_OBJ_SKILL_AURA2, 122);
		SetPosObjY(SPR_OBJ_SKILL_AURA3, 122);		
		byIndex++;
		m_bClassType[byIndex] = CLASS_TYPE_WARRIOR;
	}
	if(g_pMainPlayer->m_wClass!=CLASS_TYPE_PRIEST)
	{
		SetPosObjY(SPR_OBJ_SKILL_DIVINE1, 122+byIndex*23);
		SetPosObjY(SPR_OBJ_SKILL_DIVINE2, 122+byIndex*23);
		SetPosObjY(SPR_OBJ_SKILL_DIVINE3, 122+byIndex*23);		
		byIndex++;
		m_bClassType[byIndex] = CLASS_TYPE_PRIEST;
	}
	if(g_pMainPlayer->m_wClass!=CLASS_TYPE_SUMMONER)
	{
		SetPosObjY(SPR_OBJ_SKILL_SUMMON1, 122+byIndex*23);
		SetPosObjY(SPR_OBJ_SKILL_SUMMON2, 122+byIndex*23);
		SetPosObjY(SPR_OBJ_SKILL_SUMMON3, 122+byIndex*23);
		byIndex++;
		m_bClassType[byIndex] = CLASS_TYPE_SUMMONER;
	}
	if(g_pMainPlayer->m_wClass!=CLASS_TYPE_HUNTER)
	{
		SetPosObjY(SPR_OBJ_SKILL_CHAKRA1, 122+byIndex*23);
		SetPosObjY(SPR_OBJ_SKILL_CHAKRA2, 122+byIndex*23);
		SetPosObjY(SPR_OBJ_SKILL_CHAKRA3, 122+byIndex*23);		
		byIndex++;
		m_bClassType[byIndex] = CLASS_TYPE_HUNTER;
	}
	if(g_pMainPlayer->m_wClass!=CLASS_TYPE_WIZARD)	
	{
		SetPosObjY(SPR_OBJ_SKILL_MANA1, 122+byIndex*23);
		SetPosObjY(SPR_OBJ_SKILL_MANA2, 122+byIndex*23);
		SetPosObjY(SPR_OBJ_SKILL_MANA3, 122+byIndex*23);		
		byIndex++;
		m_bClassType[byIndex] = CLASS_TYPE_WIZARD;
	}
}

void CSkillWnd::Remove()
{
	RemoveAllData();

	if(m_pLineSpr)
	{
		m_pLineSpr->Release();
		m_pLineSpr = NULL;
	}

	if(m_pAttackSpr)
	{
		m_pAttackSpr->Release();
		m_pAttackSpr = NULL;
	}
	
	
	for(int i = 0; i < 5; i++)
	{
		ReleaseSkillResourceSpr( g_sSkillListManager.pSkillList[i].pActiveList );		// Active Skill
		ReleaseSkillResourceSpr( g_sSkillListManager.pSkillList[i].pMasteryList );		// Mastery Skill
		ReleaseSkillResourceSpr( g_sSkillListManager.pSkillList[i].pPassiveList );		// Passive Skill
		ReleaseSkillResourceSpr( g_sSkillListManager.pSkillList[i].pOverDriveList );	// OverDrive Skill
		
/*		// modified by minjin. for more simple sentense
		// Active //
		POSITION_ pos = g_sSkillListManager.pSkillList[i].pActiveList->GetHeadPosition();	

		while(pos)
		{
			LP_SKILL_RESOURCE_EX lpSkillResourceNode = (LP_SKILL_RESOURCE_EX)g_sSkillListManager.pSkillList[i].pActiveList->GetNext(pos);
					
			if(lpSkillResourceNode->pSpr)
			{
				lpSkillResourceNode->pSpr->Release();
				lpSkillResourceNode->pSpr = NULL;
			}
			if(lpSkillResourceNode->pSprAct)
			{
				lpSkillResourceNode->pSprAct->Release();
				lpSkillResourceNode->pSprAct = NULL;
			}
		}
		// Mastery //
		pos = g_sSkillListManager.pSkillList[i].pMasteryList->GetHeadPosition();	

		while(pos)
		{
			LP_SKILL_RESOURCE_EX lpSkillResourceNode = (LP_SKILL_RESOURCE_EX)g_sSkillListManager.pSkillList[i].pMasteryList->GetNext(pos);
					
			if(lpSkillResourceNode->pSpr)
			{
				lpSkillResourceNode->pSpr->Release();
				lpSkillResourceNode->pSpr = NULL;
			}
			if(lpSkillResourceNode->pSprAct)
			{
				lpSkillResourceNode->pSprAct->Release();
				lpSkillResourceNode->pSprAct = NULL;
			}
		}
		// Passive //
		pos = g_sSkillListManager.pSkillList[i].pPassiveList->GetHeadPosition();	

		while(pos)
		{
			LP_SKILL_RESOURCE_EX lpSkillResourceNode = (LP_SKILL_RESOURCE_EX)g_sSkillListManager.pSkillList[i].pPassiveList->GetNext(pos);

			if(lpSkillResourceNode->pSpr)
			{
				lpSkillResourceNode->pSpr->Release();
				lpSkillResourceNode->pSpr = NULL;
			}
			if(lpSkillResourceNode->pSprAct)
			{
				lpSkillResourceNode->pSprAct->Release();
				lpSkillResourceNode->pSprAct = NULL;
			}
		}
		// OverDrive //
		pos = g_sSkillListManager.pSkillList[i].pOverDriveList->GetHeadPosition();	

		while(pos)
		{
			LP_SKILL_RESOURCE_EX lpSkillResourceNode = (LP_SKILL_RESOURCE_EX)g_sSkillListManager.pSkillList[i].pOverDriveList->GetNext(pos);					

			if(lpSkillResourceNode->pSpr)
			{
				lpSkillResourceNode->pSpr->Release();
				lpSkillResourceNode->pSpr = NULL;
			}
			if(lpSkillResourceNode->pSprAct)
			{
				lpSkillResourceNode->pSprAct->Release();
				lpSkillResourceNode->pSprAct = NULL;
			}
		}	//*/
	}
	
	m_bActive	= FALSE;
	m_bInit		= FALSE;
}

void CSkillWnd::SetOrder()
{
}

void CSkillWnd::RenderSkillIcon()
{
	int nOrder	= __ORDER_USERINTERFACE_START_ + 4;
	
	VECTOR2	vPos;					
		
	BYTE nLeft	= g_pMainPlayer->GetSkillKind(0);
	BYTE nRight	= g_pMainPlayer->GetSkillKind(1);

	BYTE nGuardianSkill = -1;
	
	CMonster *pGuardian = g_pMainPlayer->m_pGuardian[0];
	if(pGuardian)
	{
		nGuardianSkill = pGuardian->GetSelectedSkill();
	}
	
	if(nLeft!=__SKILL_NONE_SELECT__)
	{
		vPos.x	= 133;
		vPos.y	= 733;

		if(nLeft==__SKILL_ATTACK__)
		{
			g_pRenderer->RenderSprite(m_pAttackSpr, NULL, 0.0f, &vPos, NULL, 0xffffffff, nOrder, RENDER_TYPE_DISABLE_TEX_FILTERING);
		}
		else
		{
			if(g_sSkillListManager.pSpr[nLeft])
				g_pRenderer->RenderSprite(g_sSkillListManager.pSpr[nLeft], NULL, 0.0f, &vPos, NULL, 0xffffffff, nOrder, RENDER_TYPE_DISABLE_TEX_FILTERING);
		}
	}
	
	if(nRight!=-1)
	{
		vPos.x	= 248;
		vPos.y	= 733;

		if(nRight==__SKILL_ATTACK__)
		{
			g_pRenderer->RenderSprite(m_pAttackSpr, NULL, 0.0f, &vPos, NULL, 0xffffffff, nOrder, RENDER_TYPE_DISABLE_TEX_FILTERING);
		}
		else
		{
			if(g_sSkillListManager.pSpr[nRight])
				g_pRenderer->RenderSprite(g_sSkillListManager.pSpr[nRight], NULL, 0.0f, &vPos, NULL, 0xffffffff, nOrder, RENDER_TYPE_DISABLE_TEX_FILTERING);
		}
	}

	// 가디언이 소환중이면
	if(pGuardian)
	{
		vPos.x	= 209;
		vPos.y	= 733;

		// 선택된 스킬이 없다면
		if(nGuardianSkill == 0)
		{
			// 물리공격 혹은 렌덤스킬 스프라이트 출력
			g_pRenderer->RenderSprite(m_pAttackSpr, NULL, 0.0f, &vPos, NULL, 0xffaaffaa, nOrder, RENDER_TYPE_DISABLE_TEX_FILTERING);
		}
		// 선택된 스킬이 있다면
		else
		{
			// 선택된 스킬 스프라이트 출력
			if(g_sSkillListManager.pSpr[nGuardianSkill])
				g_pRenderer->RenderSprite(g_sSkillListManager.pSpr[nGuardianSkill], NULL, 0.0f, &vPos, NULL, 0xffaaffaa, nOrder, RENDER_TYPE_DISABLE_TEX_FILTERING);
		}
	}

	if(m_bySkillIndex==1)
	{
		int nCount = 1;

		vPos.x	= 116;
		vPos.y	= 640;
		g_pRenderer->RenderSprite(m_pAttackSpr, NULL, 0.0f, &vPos, NULL, 0xffffffff, nOrder, RENDER_TYPE_DISABLE_TEX_FILTERING);

		for(int i = 0; i < g_sSkillListManager.byLeftSkillCnt; i++)
		{
			if(g_pMainPlayer->GetSkillLevel(g_sSkillListManager.byLeftSkill[i])>0)
			{
				vPos.x	= 116+32*(nCount%5);
				vPos.y	= 640-32*(nCount/5);
				g_pRenderer->RenderSprite(g_sSkillListManager.pSpr[g_sSkillListManager.byLeftSkill[i]], NULL, 0.0f, &vPos, NULL, 0xffffffff, nOrder, RENDER_TYPE_DISABLE_TEX_FILTERING);
				nCount++;
			}
		}		
	}
	else if(m_bySkillIndex==2)
	{
		int nCount = 0;

		/*vPos.x	= 116;
		vPos.y	= 640;
		g_pRenderer->RenderSprite(m_pAttackSpr, NULL, 0.0f, &vPos, NULL, 0xffffffff, nOrder, RENDER_TYPE_DISABLE_TEX_FILTERING);*/
		
		for(int i = 0; i < g_sSkillListManager.byRightSkillCnt; i++)
		{
			if(g_pMainPlayer->GetSkillLevel(g_sSkillListManager.byRightSkill[i])>0)
			{
				vPos.x	= 116+32*(nCount%5);
				vPos.y	= 640-32*(nCount/5);
				g_pRenderer->RenderSprite(g_sSkillListManager.pSpr[g_sSkillListManager.byRightSkill[i]], NULL, 0.0f, &vPos, NULL, 0xffffffff, nOrder, RENDER_TYPE_DISABLE_TEX_FILTERING);
				nCount++;
			}
		}
	}
	// 가디언 스킬 선택 : 최덕석 2005.3.3
	else if(pGuardian && m_bySkillIndex == 3)
	{
		int nCount = 1;

		vPos.x	= 116;
		vPos.y	= 640;
		g_pRenderer->RenderSprite(m_pAttackSpr, NULL, 0.0f, &vPos, NULL, 0xffaaffaa, nOrder, RENDER_TYPE_DISABLE_TEX_FILTERING);
		
		for(int i = 0; i < MAX_GUARDIAN_USE_SKILL; i++)
		{
			WORD wSkill = pGuardian->m_Skill[i].wSkill;
			if(wSkill > 0 && pGuardian->GetSkillLevel(wSkill) > 0)
			{
				vPos.x	= 116+32*(nCount%5);
				vPos.y	= 640-32*(nCount/5);
				g_pRenderer->RenderSprite(g_sSkillListManager.pSpr[wSkill], NULL, 0.0f, &vPos, NULL, 0xffaaffaa, nOrder, RENDER_TYPE_DISABLE_TEX_FILTERING);
				nCount++;
			}
		}
	}

}

void CSkillWnd::SetSkillButtonPos()
{
	SetRender(SPR_OBJ_SKILL_AURA2, FALSE);
	SetRender(SPR_OBJ_SKILL_DIVINE2, FALSE);					
	SetRender(SPR_OBJ_SKILL_SUMMON2, FALSE);		
	SetRender(SPR_OBJ_SKILL_CHAKRA2, FALSE);		
	SetRender(SPR_OBJ_SKILL_MANA2, FALSE);
	SetRender(SPR_OBJ_SKILL_AURA3, FALSE);
	SetRender(SPR_OBJ_SKILL_DIVINE3, FALSE);					
	SetRender(SPR_OBJ_SKILL_SUMMON3, FALSE);		
	SetRender(SPR_OBJ_SKILL_CHAKRA3, FALSE);		
	SetRender(SPR_OBJ_SKILL_MANA3, FALSE);		
	SetRender(SPR_OBJ_SKILL_AURA1, TRUE);
	SetRender(SPR_OBJ_SKILL_DIVINE1, TRUE);					
	SetRender(SPR_OBJ_SKILL_SUMMON1, TRUE);		
	SetRender(SPR_OBJ_SKILL_CHAKRA1, TRUE);		
	SetRender(SPR_OBJ_SKILL_MANA1, TRUE);

	switch(m_bySkillType)
	{
	case CLASS_TYPE_WARRIOR:
		SetRender(SPR_OBJ_SKILL_AURA3, TRUE);
		SetRender(SPR_OBJ_SKILL_AURA1, FALSE);
		break;
	case CLASS_TYPE_PRIEST:
		SetRender(SPR_OBJ_SKILL_DIVINE3, TRUE);			
		SetRender(SPR_OBJ_SKILL_DIVINE1, FALSE);
		break;
	case CLASS_TYPE_SUMMONER:
		SetRender(SPR_OBJ_SKILL_SUMMON3, TRUE);
		SetRender(SPR_OBJ_SKILL_SUMMON1, FALSE);
		break;
	case CLASS_TYPE_HUNTER:
		SetRender(SPR_OBJ_SKILL_CHAKRA3, TRUE);
		SetRender(SPR_OBJ_SKILL_CHAKRA1, FALSE);
		break;
	case CLASS_TYPE_WIZARD:
		SetRender(SPR_OBJ_SKILL_MANA3, TRUE);
		SetRender(SPR_OBJ_SKILL_MANA1, FALSE);
		break;			
	}
}

void CSkillWnd::RenderText()
{
	int		nLeft = 0, nRight = 0;
	char	szInfo[0xff] = {0,};
	
	RenderSkill();
	RenderSkillInfo();
	
	// 스킬포인트 //
	if(g_pMainPlayer->m_wPointSkill>0)
	{
		wsprintf(szInfo, "%u", g_pMainPlayer->m_wPointSkill);
		
		int nSize = lstrlen(szInfo);

		if(nSize==3)
		{
			nLeft	= m_fPosX+267;
			nRight	= m_fPosX+267+nSize*7;
		}		
		else if(nSize==2)
		{
			nLeft	= m_fPosX+271;
			nRight	= m_fPosX+271+nSize*7;
		}
		else
		{
			nLeft	= m_fPosX+273;
			nRight	= m_fPosX+273+nSize*7;
		}
		RenderFont(szInfo, nLeft, nRight, (int)m_fPosZ+40, (int)m_fPosZ+54, GetStartOrder()+1);
	}	
}

BOOL CSkillWnd::CheckSkillIfno(int nIndex, int nPosX, int nPosY, int nPosX2, int nPosY2)
{
	if(CUserInterface::GetInstance()->InterfaceCollision(nIndex, nPosX, nPosX2, nPosY, nPosY2)==FALSE)
		return TRUE;				

	return FALSE;
}

void CSkillWnd::RenderSkillInfo()
{
	BYTE bySkillIndex = 0; 
	
	if(g_Mouse.bLDown)
		return;
	
	if ( m_bySkillType >=0 || m_bySkillType <= 5)
	{
		bySkillIndex = m_bySkillType % 5;
	}
	else
	{
		// "Invalid Skill Type"		by minjin.
		_asm int 3
	}
/*	for more simple code
	switch(m_bySkillType)
	{
	case CLASS_TYPE_WARRIOR:
		bySkillIndex = 1;
		break;
	case CLASS_TYPE_PRIEST:
		bySkillIndex = 2;
		break;
	case CLASS_TYPE_SUMMONER:						
		bySkillIndex = 3;
		break;
	case CLASS_TYPE_HUNTER:
		bySkillIndex = 4;
		break;
	case CLASS_TYPE_WIZARD:
		bySkillIndex = 0;
		break;
	}							//*/
	
	// 스킬 정보 //		
	if(g_Mouse.MousePos.x>=m_fPosX+28 && g_Mouse.MousePos.x<=m_fPosX+60 && g_Mouse.MousePos.y>=m_fPosZ+30 && g_Mouse.MousePos.y<=m_fPosZ+62)
	{
		POSITION_ pos = g_sSkillListManager.pSkillList[bySkillIndex].pMasteryList->GetHeadPosition();

		while(pos)
		{
			LP_SKILL_RESOURCE_EX lpSkillResourceEx = (LP_SKILL_RESOURCE_EX)g_sSkillListManager.pSkillList[bySkillIndex].pMasteryList->GetNext(pos);

			if(lpSkillResourceEx)
			{			
				SetRenderSkillInfo(lpSkillResourceEx, SKILL_WND, (int)m_fPosX+28, (int)m_fPosX+60, (int)m_fPosZ+30, (int)m_fPosZ+62);				
				return;
			}
		}
		
	}	

	for(int i = 0; i < 14; i++)
	{
		if(g_Mouse.MousePos.x>=m_fPosX+77+(i/7)*82 && g_Mouse.MousePos.x<=m_fPosX+109+(i/7)*82 && g_Mouse.MousePos.y>=m_fPosZ+108+(i%7)*42 && g_Mouse.MousePos.y<=m_fPosZ+140+(i%7)*42)
		{
			POSITION_ pos = g_sSkillListManager.pSkillList[bySkillIndex].pActiveList->GetHeadPosition();

			while(pos)
			{
				LP_SKILL_RESOURCE_EX lpSkillResourceEx = (LP_SKILL_RESOURCE_EX)g_sSkillListManager.pSkillList[bySkillIndex].pActiveList->GetNext(pos);

				if(lpSkillResourceEx)
				{
					if(lpSkillResourceEx->byTypeIndex==i)
					{
						SetRenderSkillInfo(lpSkillResourceEx, SKILL_WND, m_fPosX+77+(i/7)*82, m_fPosX+109+(i/7)*82, m_fPosZ+108+(i%7)*42, m_fPosZ+140+(i%7)*42);						
						return;
					}
				}
			}
		}
	}

	for(i = 0; i < 7; i++)
	{
		if(g_Mouse.MousePos.x>=m_fPosX+241 && g_Mouse.MousePos.x<=m_fPosX+273 && g_Mouse.MousePos.y>=m_fPosZ+108+i*42 && g_Mouse.MousePos.y<=m_fPosZ+140+i*42)
		{
			POSITION_ pos = g_sSkillListManager.pSkillList[bySkillIndex].pPassiveList->GetHeadPosition();

			while(pos)
			{
				LP_SKILL_RESOURCE_EX lpSkillResourceEx = (LP_SKILL_RESOURCE_EX)g_sSkillListManager.pSkillList[bySkillIndex].pPassiveList->GetNext(pos);

				if(lpSkillResourceEx)
				{
					if(lpSkillResourceEx->byTypeIndex==i)
					{
						SetRenderSkillInfo(lpSkillResourceEx, SKILL_WND, m_fPosX+241, m_fPosX+273, m_fPosZ+108+i*42, m_fPosZ+140+i*42);
						return;
					}
				}
			}
		}
	}

	for(i = 0; i < 3; i++)
	{				
		if(g_Mouse.MousePos.x>=m_fPosX+77+i*82 && g_Mouse.MousePos.x<=m_fPosX+109+i*82 && g_Mouse.MousePos.y>=m_fPosZ+444 && g_Mouse.MousePos.y<=m_fPosZ+476)
		{
			POSITION_ pos = g_sSkillListManager.pSkillList[bySkillIndex].pOverDriveList->GetHeadPosition();

			while(pos)
			{
				LP_SKILL_RESOURCE_EX lpSkillResourceEx = (LP_SKILL_RESOURCE_EX)g_sSkillListManager.pSkillList[bySkillIndex].pOverDriveList->GetNext(pos);

				if(lpSkillResourceEx)
				{
					if(lpSkillResourceEx->byTypeIndex==i)
					{
						SetRenderSkillInfo(lpSkillResourceEx, SKILL_WND, m_fPosX+77+i*82, m_fPosX+109+i*82 , m_fPosZ+444, m_fPosZ+476);
						return;
					}
				}
			}
		}
	}
}

void CSkillWnd::SetRenderSkillInfo(LP_SKILL_RESOURCE_EX lpSkillResourceEx, int nIndex, int nPosX, int nPosX2, int nPosY, int nPosY2)
{
	int		nLen		= 0;
	int		nSize		= 0;		
	int		nSkillId	= lpSkillResourceEx->wId;
	char*	pToken		= NULL;
	int		nVal[__MAX_SKILLINFO_VAL__] = {0,};
	BYTE	byIndex = 0;
	char	szInfo[0xff] = {0,};
//	RECT	Pos;
	
	if(!lpSkillResourceEx)
		return;
	
	if(CheckSkillIfno(nIndex, nPosX, nPosY, nPosX2, nPosY2)==TRUE)
		return;
	
	// 마스터리 //		
	int nLevel = g_pMainPlayer->GetSkillLevel(nSkillId);

	// 이름 //
	Effect* pEffect = g_pEffectLayer->GetEffectInfo(nSkillId);
	
	if(pEffect->bID==0)
		return;
		
	lstrcpy(szInfo, pEffect->szName);
		
	nLen = lstrlen(szInfo);
	RenderFont(szInfo, g_Mouse.MousePos.x, g_Mouse.MousePos.x+nLen*7, g_Mouse.MousePos.y+(byIndex*15)-7, g_Mouse.MousePos.y+(byIndex*15)+7, __ORDER_ITEM_DESC__+2, 0xffc8c8c8);	
	byIndex += 2;		

	if(nLen>nSize)
		nSize = nLen;			
			
	for(int i = 0; i < 2; i++)
	{
		int nLen	= lstrlen(pEffect->szDescription2[i]);
		
		if(nLen)
		{
			RenderFont(pEffect->szDescription2[i], g_Mouse.MousePos.x, g_Mouse.MousePos.x+nLen*7, g_Mouse.MousePos.y+(byIndex*15)-7, g_Mouse.MousePos.y+(byIndex*15)+7, __ORDER_ITEM_DESC__+2, 0xffc8c8c8);
			byIndex++;
			
			if(nLen>nSize)
				nSize = nLen;
		}
		
	}		
	byIndex++;

	if(nLevel==0)
	{
		char szInfo[0xff] = {0,};
		wsprintf(szInfo, g_Message[ETC_MESSAGE644].szMessage, g_sSkillInfoDP[nSkillId].wSkillLevel); // "필요한 마스터리 양 : %u"

		nLen = lstrlen(szInfo);
		RenderFont(szInfo, g_Mouse.MousePos.x, g_Mouse.MousePos.x+nLen*7, g_Mouse.MousePos.y+(byIndex*15)-7, g_Mouse.MousePos.y+(byIndex*15)+7, __ORDER_ITEM_DESC__+2, 0xffc8c8c8);
		byIndex += 2;		
		
// __deepdark		
		if(nLen>nSize)
			nSize = nLen;
// __deepdark
	}	

	if(nLevel>0)
	{
		// 현재 레벨 //
		for(int i = 0; i < g_sSkillInfoDP[nSkillId].byIndex; i++)
		{
			switch(g_sSkillInfoDP[nSkillId].enSkillInfo[i])
			{
			case SKILLINFO_LEVEL:	nVal[i] = g_pMainPlayer->GetSkillLevel(nSkillId);								break;				
			case SKILLINFO_MIN:		nVal[i] = g_pEffectLayer->m_Effect[nSkillId].Value[nLevel-1].nMin;				break;
			case SKILLINFO_MAX:		nVal[i] = g_pEffectLayer->m_Effect[nSkillId].Value[nLevel-1].nMax;				break;
			case SKILLINFO_DUR:		nVal[i] = g_pEffectLayer->m_Effect[nSkillId].Value[nLevel-1].nDuration/1000;	break;
			case SKILLINFO_PBT:		nVal[i] = g_pEffectLayer->m_Effect[nSkillId].Value[nLevel-1].nProbability;		break;
			case SKILLINFO_SP:		nVal[i] = g_pEffectLayer->m_Effect[nSkillId].Value[nLevel-1].nMana;				break;
			case SKILLINFO_CPS:		nVal[i] = g_pEffectLayer->m_Effect[nSkillId].Value[nLevel-1].nCompass;			break;
			}
			
			if(nVal[i]<0)
				nVal[i] = abs(nVal[i]);
		}

		switch(g_sSkillInfoDP[nSkillId].byIndex)
		{
			case 1: wsprintf(szInfo, g_sSkillInfoDP[nSkillId].szInfo, nVal[0]);													break;
			case 2:	wsprintf(szInfo, g_sSkillInfoDP[nSkillId].szInfo, nVal[0], nVal[1]);										break;
			case 3:	wsprintf(szInfo, g_sSkillInfoDP[nSkillId].szInfo, nVal[0], nVal[1], nVal[2]);								break;
			case 4: wsprintf(szInfo, g_sSkillInfoDP[nSkillId].szInfo, nVal[0], nVal[1], nVal[2], nVal[3]);						break;
			case 5: wsprintf(szInfo, g_sSkillInfoDP[nSkillId].szInfo, nVal[0], nVal[1], nVal[2], nVal[3], nVal[4]);				break;
			case 6: wsprintf(szInfo, g_sSkillInfoDP[nSkillId].szInfo, nVal[0], nVal[1], nVal[2], nVal[3], nVal[4], nVal[5]);	break;
		}							
		
		nLen = lstrlen(g_Message[ETC_MESSAGE645].szMessage); // "현재 레벨"
		RenderFont(g_Message[ETC_MESSAGE645].szMessage, g_Mouse.MousePos.x, g_Mouse.MousePos.x+nLen*7, g_Mouse.MousePos.y+(byIndex*15)-7, g_Mouse.MousePos.y+(byIndex*15)+7, __ORDER_ITEM_DESC__+2, 0xffc8c8c8);			
		byIndex++;

		pToken = strtok(szInfo, ",");

		if(pToken!=NULL)
		{
			nLen = lstrlen(pToken);

			if(!IsEmptyString(pToken))
			{
				RenderFont(pToken, g_Mouse.MousePos.x, g_Mouse.MousePos.x+nLen*7, g_Mouse.MousePos.y+(byIndex*15)-7, g_Mouse.MousePos.y+(byIndex*15)+7, __ORDER_ITEM_DESC__+2, 0xffc8c8c8);
				byIndex++;
// __deepdark		
				if(nLen>nSize)
					nSize = nLen;
// __deepdark
			}

			while(pToken!=NULL)
			{
				pToken = strtok(NULL, ",");

				if(pToken!=NULL)
				{
					int nDstSize = 0;

					nDstSize =lstrlen(pToken);
						
					if(nDstSize>nSize)
						nSize = nDstSize;

					if(!IsEmptyString(pToken))
					{				
						RenderFont(pToken, g_Mouse.MousePos.x, g_Mouse.MousePos.x+nDstSize*7+64, g_Mouse.MousePos.y+(byIndex*15)-7, g_Mouse.MousePos.y+(byIndex*15)+7, __ORDER_ITEM_DESC__+2, 0xffc8c8c8);						
						byIndex+=1;							
					}						
				}
			}
		}
		
		if(nLevel<CUserInterface::GetInstance()->m_wMaxMasteryLevel)
			byIndex+=1;
	}

	WORD wMaxLevel = 0;

	if(lpSkillResourceEx->wId%30==1)
		wMaxLevel = CUserInterface::GetInstance()->m_wMaxMasteryLevel;
	else
		wMaxLevel = CUserInterface::GetInstance()->m_wMaxSkillLevel;
	
	if(nLevel<wMaxLevel)
	{
		// 다음 레벨 //
		for(int i = 0; i < g_sSkillInfoDP[nSkillId].byIndex; i++)
		{
			switch(g_sSkillInfoDP[nSkillId].enSkillInfo[i])
			{
			case SKILLINFO_LEVEL:	nVal[i] = g_pMainPlayer->GetSkillLevel(nSkillId)+1;							break;				
			case SKILLINFO_MIN:		nVal[i] = g_pEffectLayer->m_Effect[nSkillId].Value[nLevel].nMin;			break;
			case SKILLINFO_MAX:		nVal[i] = g_pEffectLayer->m_Effect[nSkillId].Value[nLevel].nMax;			break;
			case SKILLINFO_DUR:		nVal[i] = g_pEffectLayer->m_Effect[nSkillId].Value[nLevel].nDuration/1000;	break;
			case SKILLINFO_PBT:		nVal[i] = g_pEffectLayer->m_Effect[nSkillId].Value[nLevel].nProbability;	break;
			case SKILLINFO_SP:		nVal[i] = g_pEffectLayer->m_Effect[nSkillId].Value[nLevel].nMana;			break;
			case SKILLINFO_CPS:		nVal[i] = g_pEffectLayer->m_Effect[nSkillId].Value[nLevel].nCompass;		break;
			}
			
			if(nVal[i]<0)
				nVal[i] = abs(nVal[i]);
		}

		switch(g_sSkillInfoDP[nSkillId].byIndex)
		{
			case 1: wsprintf(szInfo, g_sSkillInfoDP[nSkillId].szInfo, nVal[0]);													break;
			case 2:	wsprintf(szInfo, g_sSkillInfoDP[nSkillId].szInfo, nVal[0], nVal[1]);										break;
			case 3:	wsprintf(szInfo, g_sSkillInfoDP[nSkillId].szInfo, nVal[0], nVal[1], nVal[2]);								break;
			case 4: wsprintf(szInfo, g_sSkillInfoDP[nSkillId].szInfo, nVal[0], nVal[1], nVal[2], nVal[3]);						break;
			case 5: wsprintf(szInfo, g_sSkillInfoDP[nSkillId].szInfo, nVal[0], nVal[1], nVal[2], nVal[3], nVal[4]);				break;
			case 6: wsprintf(szInfo, g_sSkillInfoDP[nSkillId].szInfo, nVal[0], nVal[1], nVal[2], nVal[3], nVal[4], nVal[5]);	break;
		}							
		
		nLen = lstrlen(g_Message[ETC_MESSAGE646].szMessage); // "다음 레벨"
		RenderFont(g_Message[ETC_MESSAGE646].szMessage, g_Mouse.MousePos.x, g_Mouse.MousePos.x+nLen*7, g_Mouse.MousePos.y+(byIndex*15)-7, g_Mouse.MousePos.y+(byIndex*15)+7, __ORDER_ITEM_DESC__+2, 0xffc8c8c8);		
		byIndex++;

		pToken = strtok(szInfo, ",");

		if(pToken!=NULL)
		{	
			nLen = lstrlen(pToken);						
				
			if(!IsEmptyString(pToken))
			{
				RenderFont(pToken, g_Mouse.MousePos.x, g_Mouse.MousePos.x+nLen*7+64, g_Mouse.MousePos.y+(byIndex*15)-7, g_Mouse.MousePos.y+(byIndex*15)+7, __ORDER_ITEM_DESC__+2, 0xffc8c8c8);
				byIndex++;
// __deepdark		
				if(nLen>nSize)
					nSize = nLen;
// __deepdark
			}

			while(pToken!=NULL)
			{
				pToken = strtok(NULL, ",");

				if(pToken!=NULL)
				{
					int nDstSize = 0;
					
					nDstSize =lstrlen(pToken);
						
					if(nDstSize>nSize)
						nSize = nDstSize;

					if(!IsEmptyString(pToken))
					{
// Begin - minjin 2004. 08. 31.
// Debugging for Render font is Blocked
// (문자열 중간에 끊겨버리는거 수정.. -_-;;)

						//RenderFont(pToken, g_Mouse.MousePos.x, g_Mouse.MousePos.x+nLen*7+64, g_Mouse.MousePos.y+(byIndex*15)-7, g_Mouse.MousePos.y+(byIndex*15)+7, __ORDER_ITEM_DESC__+2, 0xffc8c8c8);
						RenderFont(pToken, g_Mouse.MousePos.x, g_Mouse.MousePos.x + nSize * 7 + 64, g_Mouse.MousePos.y + (byIndex * 15) - 7, g_Mouse.MousePos.y + (byIndex * 15) + 7, __ORDER_ITEM_DESC__ + 2, 0xffc8c8c8);

// End - minjin
						byIndex+=1;							
					}						
				}
			}
		}
	}
	CInterface::GetInstance()->RenderInfoBox(g_Mouse.MousePos.x-8, g_Mouse.MousePos.y-12, 2*(10+(nSize/2*6)), (int)(2*(3+byIndex*7.5f)), __ORDER_ITEM_DESC__);	
}

void CSkillWnd::RenderSkill()
{
	int nOrder	= __ORDER_INTERFACE_START__ + m_byOrder*10;

//	RECT	rt;
	VECTOR2	v2Scaling;
	VECTOR2	vPos;					
	char	szInfo[0xff] = {0,};

	v2Scaling.x	= 1.0f;
	v2Scaling.y	= 1.0f;
		
	BYTE bySkillIndex = 0; 
	
	if ( m_bySkillType >=0 || m_bySkillType <= 5)
	{
		bySkillIndex = m_bySkillType % 5;
	}
	else
	{
		// "Invalid Skill Type"		by minjin.
		_asm int 3
	}
/*	for more simple code
	switch(m_bySkillType)
	{
	case CLASS_TYPE_WARRIOR:
		byIndex = 1;
		break;
	case CLASS_TYPE_PRIEST:
		byIndex = 2;
		break;
	case CLASS_TYPE_SUMMONER:						
		byIndex = 3;
		break;
	case CLASS_TYPE_HUNTER:
		byIndex = 4;
		break;
	case CLASS_TYPE_WIZARD:
		byIndex = 0;
		break;
	}		//*/
		
	// Active //
	POSITION_ pos = g_sSkillListManager.pSkillList[bySkillIndex].pActiveList->GetHeadPosition();	

	while(pos)
	{
		LP_SKILL_RESOURCE_EX lpSkillResourceNode = (LP_SKILL_RESOURCE_EX)g_sSkillListManager.pSkillList[bySkillIndex].pActiveList->GetNext(pos);						

		if(!lpSkillResourceNode)
			return;

		int nPosX	= (int)m_fPosX+77+82*(lpSkillResourceNode->byTypeIndex/7);
		int nPosY	= (int)m_fPosZ+108+42*(lpSkillResourceNode->byTypeIndex%7);
			
		vPos.x	= (float)nPosX;
		vPos.y	= (float)nPosY;
			
		if(g_pMainPlayer->GetSkillLevel(lpSkillResourceNode->wId)>0)			
			g_pRenderer->RenderSprite(lpSkillResourceNode->pSpr, NULL, 0.0f, &vPos, NULL, 0xffffffff, nOrder+2, RENDER_TYPE_DISABLE_TEX_FILTERING);
		else
			g_pRenderer->RenderSprite(lpSkillResourceNode->pSprAct, NULL, 0.0f, &vPos, NULL, 0xffffffff, nOrder+2, RENDER_TYPE_DISABLE_TEX_FILTERING);

		vPos.x	= m_fPosX+75+82*(lpSkillResourceNode->byTypeIndex/7);
		vPos.y	= m_fPosZ+106+42*(lpSkillResourceNode->byTypeIndex%7);		
		g_pRenderer->RenderSprite(m_pLineSpr, NULL, 0.0f, &vPos, NULL, 0xffffffff, nOrder+1, RENDER_TYPE_DISABLE_TEX_FILTERING);

		if(g_pMainPlayer->GetSkillLevel(lpSkillResourceNode->wId)>0)
		{
			wsprintf(szInfo, g_Message[ETC_MESSAGE862].szMessage, g_pMainPlayer->GetSkillLevel(lpSkillResourceNode->wId));//"Lv%u"
		
			int nSize = lstrlen(szInfo);
			RenderFont(szInfo, nPosX+35, nPosX+49+nSize*6, nPosY+2, nPosY+16, nOrder+3);			
		}
	}
	// Mastery //
	pos = g_sSkillListManager.pSkillList[bySkillIndex].pMasteryList->GetHeadPosition();	

	while(pos)
	{		
		LP_SKILL_RESOURCE_EX lpSkillResourceNode = (LP_SKILL_RESOURCE_EX)g_sSkillListManager.pSkillList[bySkillIndex].pMasteryList->GetNext(pos);						
		
		vPos.x	= m_fPosX+26;
		vPos.y	= m_fPosZ+28;

		if(g_pMainPlayer->GetSkillLevel(lpSkillResourceNode->wId)>0)
			g_pRenderer->RenderSprite(lpSkillResourceNode->pSpr, NULL, 0.0f, &vPos, NULL, 0xffffffff, nOrder+2, RENDER_TYPE_DISABLE_TEX_FILTERING);
		else
			g_pRenderer->RenderSprite(lpSkillResourceNode->pSprAct, NULL, 0.0f, &vPos, NULL, 0xffffffff, nOrder+2, RENDER_TYPE_DISABLE_TEX_FILTERING);				
		
		if(g_pMainPlayer->GetSkillLevel(lpSkillResourceNode->wId)>0)
		{
			wsprintf(szInfo, g_Message[ETC_MESSAGE862].szMessage, g_pMainPlayer->GetSkillLevel(lpSkillResourceNode->wId));//"Lv%u"
		
			int nSize = lstrlen(szInfo);
			RenderFont(szInfo, m_fPosX+26+35, m_fPosX+26+49+nSize*6, m_fPosZ+28+2, m_fPosZ+28+16, nOrder+3);			
		}
					
		int nLevel = g_pMainPlayer->GetSkillLevel(lpSkillResourceNode->wId)-1;

		Effect* pEffect = g_pEffectLayer->GetEffectInfo(lpSkillResourceNode->wId);
		
		if(nLevel<0)
			lstrcpy(szInfo, pEffect->szName);
		else
			wsprintf(szInfo, "%s (%u)", pEffect->szName, g_pEffectLayer->m_Effect[lpSkillResourceNode->wId].GetMaxMastery(nLevel));
				
		int nSize = lstrlen(szInfo);
		RenderFont(szInfo, m_fPosX+99, m_fPosX+99+nSize*7, m_fPosZ+29, m_fPosZ+43, nOrder+3);		
	}
	// Passive //
	pos = g_sSkillListManager.pSkillList[bySkillIndex].pPassiveList->GetHeadPosition();	

	while(pos)
	{
		LP_SKILL_RESOURCE_EX lpSkillResourceNode = (LP_SKILL_RESOURCE_EX)g_sSkillListManager.pSkillList[bySkillIndex].pPassiveList->GetNext(pos);		

		int nPosX	= (int)m_fPosX+243;
		int nPosY	= (int)m_fPosZ+108+42*lpSkillResourceNode->byTypeIndex;
			
		vPos.x	= (float)nPosX;
		vPos.y	= (float)nPosY;		

		if(g_pMainPlayer->GetSkillLevel(lpSkillResourceNode->wId)>0)
			g_pRenderer->RenderSprite(lpSkillResourceNode->pSpr, NULL, 0.0f, &vPos, NULL, 0xffffffff, nOrder+2, RENDER_TYPE_DISABLE_TEX_FILTERING);		
		else
			g_pRenderer->RenderSprite(lpSkillResourceNode->pSprAct, NULL, 0.0f, &vPos, NULL, 0xffffffff, nOrder+2, RENDER_TYPE_DISABLE_TEX_FILTERING);

		vPos.x	= m_fPosX+241;
		vPos.y	= m_fPosZ+106+42*lpSkillResourceNode->byTypeIndex;
		g_pRenderer->RenderSprite(m_pLineSpr, NULL, 0.0f, &vPos, NULL, 0xffffffff, nOrder+1, RENDER_TYPE_DISABLE_TEX_FILTERING);

		if(g_pMainPlayer->GetSkillLevel(lpSkillResourceNode->wId)>0)
		{
			wsprintf(szInfo, g_Message[ETC_MESSAGE862].szMessage, g_pMainPlayer->GetSkillLevel(lpSkillResourceNode->wId));//"Lv%u"
		
			int nSize = lstrlen(szInfo);
			RenderFont(szInfo, nPosX+35, nPosX+49+nSize*6, nPosY+2, nPosY+16, nOrder+3);			
		}
	}
	// OverDrive //
	pos = g_sSkillListManager.pSkillList[bySkillIndex].pOverDriveList->GetHeadPosition();	

	while(pos)
	{
		LP_SKILL_RESOURCE_EX lpSkillResourceNode = (LP_SKILL_RESOURCE_EX)g_sSkillListManager.pSkillList[bySkillIndex].pOverDriveList->GetNext(pos);		

		if(!lpSkillResourceNode)
			return;

		int nPosX	= (int)m_fPosX+77+82*lpSkillResourceNode->byTypeIndex;
		int nPosY	= (int)m_fPosZ+444;
			
		vPos.x	= (float)nPosX;
		vPos.y	= (float)nPosY;
		
		if(g_pMainPlayer->GetSkillLevel(lpSkillResourceNode->wId)>0)
			g_pRenderer->RenderSprite(lpSkillResourceNode->pSpr, NULL, 0.0f, &vPos, NULL, 0xffffffff, nOrder+2, RENDER_TYPE_DISABLE_TEX_FILTERING);		
		else
			g_pRenderer->RenderSprite(lpSkillResourceNode->pSprAct, NULL, 0.0f, &vPos, NULL, 0xffffffff, nOrder+2, RENDER_TYPE_DISABLE_TEX_FILTERING);		

		vPos.x	= m_fPosX+75+82*lpSkillResourceNode->byTypeIndex;
		vPos.y	= m_fPosZ+442;
		g_pRenderer->RenderSprite(m_pLineSpr, NULL, 0.0f, &vPos, NULL, 0xffffffff, nOrder+1, RENDER_TYPE_DISABLE_TEX_FILTERING);

		if(g_pMainPlayer->GetSkillLevel(lpSkillResourceNode->wId)>0)
		{
			wsprintf(szInfo, g_Message[ETC_MESSAGE862].szMessage, g_pMainPlayer->GetSkillLevel(lpSkillResourceNode->wId)); // "Lv%u"
		
			int nSize = lstrlen(szInfo);

			RenderFont(szInfo, nPosX+35, nPosX+49+nSize*6, nPosY+2, nPosY+16, nOrder+3);			
		}
	}
}

void CSkillWnd::SetActive(BOOL bActive)
{
	m_bActive = bActive;

	if(bActive==FALSE)
	{
		ShowSpriteAll();

		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDCLOSE, g_v3InterfaceSoundPos, FALSE);
	}
	else
	{
		CInterface::GetInstance()->SetWnd(m_nIndex);

		m_bySkillType = g_pMainPlayer->m_wClass;		

		SetSkillUpBtn();
		SetSkillButtonPos();
		SetRender(SPR_OBJ_SKILL_WND1, TRUE);
		SetRender(SPR_OBJ_SKILL_WND2, TRUE);
		SetRender(SPR_OBJ_SKILL_CLOSE1, TRUE);

		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDOPEN, g_v3InterfaceSoundPos, FALSE);
	}
}

void CSkillWnd::SetSkillUpPos()
{	
	SetPosObj(SPR_OBJ_SKILL_UP1, 61, 44);	
	SetPosObj(SPR_OBJ_SKILL_UP1+1, 61, 44);
	
	// 액티브 //
	for(int i = 0; i < 14; i++)
	{		
		SetPosObj(SPR_OBJ_SKILL_UP1+2+i*2, 112+(i/7)*82, 124+(i%7)*42);				
		SetPosObj(SPR_OBJ_SKILL_UP1+3+i*2, 112+(i/7)*82, 124+(i%7)*42);		
	}
	// 패시브 //
	for(i = 0; i < 7; i++)
	{
		SetPosObj(SPR_OBJ_SKILL_UP1+30+i*2, 278, 124+i*42);
		SetPosObj(SPR_OBJ_SKILL_UP1+31+i*2, 278, 124+i*42);		
	}
	// 오버드라이브 //
	for(i = 0; i < 3; i++)
	{
		SetPosObj(SPR_OBJ_SKILL_UP1+44+i*2, 112+i*82, 460);
		SetPosObj(SPR_OBJ_SKILL_UP1+45+i*2, 112+i*82, 460);
	}
}

void CSkillWnd::SetSkillUpBtn()
{
	for(int i = 0; i < 25; i++)
	{
		SetRender(SPR_OBJ_SKILL_UP1+i*2, FALSE);
		SetRender(SPR_OBJ_SKILL_UP1+i*2+1, FALSE);
	}

	if(g_pMainPlayer->m_wPointSkill>0)
	{
		BYTE bySkillIndex = 0; 
	
		if ( m_bySkillType >=0 || m_bySkillType <= 5)
		{
			bySkillIndex = m_bySkillType % 5;
		}
		else
		{
			// "Invalid Skill Type"		by minjin.
			_asm int 3
		}
	/*	for more simple code
		switch(m_bySkillType)
		{
		case CLASS_TYPE_WARRIOR:
			byIndex = 1;
			break;
		case CLASS_TYPE_PRIEST:
			byIndex = 2;
			break;
		case CLASS_TYPE_SUMMONER:						
			byIndex = 3;
			break;
		case CLASS_TYPE_HUNTER:
			byIndex = 4;
			break;
		case CLASS_TYPE_WIZARD:
			byIndex = 0;
			break;
		}		//*/

		
		POSITION_ pos;
#ifdef __SKILL_MASTERY_ENABLE
		// 마스터리 //
		pos = g_sSkillListManager.pSkillList[bySkillIndex].pMasteryList->GetHeadPosition();	

		while(pos)
		{	
			LP_SKILL_RESOURCE_EX lpSkillResourceEx = (LP_SKILL_RESOURCE_EX)g_sSkillListManager.pSkillList[bySkillIndex].pMasteryList->GetNext(pos);

			if(lpSkillResourceEx)
			{
				Effect* pEffect = g_pEffectLayer->GetEffectInfo(lpSkillResourceEx->wId);
				
				if(pEffect->bAbleClass&m_byBitClassType)
				{
					if(g_pMainPlayer->sSkillTable[lpSkillResourceEx->wId].bSKillLevel<CUserInterface::GetInstance()->m_wMaxMasteryLevel)
						SetRender(SPR_OBJ_SKILL_UP1, TRUE);					
				}
			}
		}
#endif
		// 액티브 //
		pos = g_sSkillListManager.pSkillList[bySkillIndex].pActiveList->GetHeadPosition();	

		while(pos)
		{	
			LP_SKILL_RESOURCE_EX lpSkillResourceEx = (LP_SKILL_RESOURCE_EX)g_sSkillListManager.pSkillList[bySkillIndex].pActiveList->GetNext(pos);

			if(lpSkillResourceEx)
			{
				Effect* pEffect = g_pEffectLayer->GetEffectInfo(lpSkillResourceEx->wId);
				
				if(pEffect->bAbleClass&m_byBitClassType)
				{
					if(g_pMainPlayer->sSkillTable[lpSkillResourceEx->wId].bSKillLevel<CUserInterface::GetInstance()->m_wMaxSkillLevel)
					{
						int		nMaxLevel		= 0;
						BYTE	byMastery		= g_pEffectLayer->GetSkillMasteryKind(lpSkillResourceEx->wId);
						BYTE	byLevel			= g_pMainPlayer->sSkillTable[byMastery].bSKillLevel;
						Effect* pEffect			= g_pEffectLayer->GetEffectInfo(lpSkillResourceEx->wId);
						Effect* pMasteryEffect	= g_pEffectLayer->GetEffectInfo(byMastery);

						if(byLevel!=0)
							nMaxLevel = pMasteryEffect->GetMaxMastery(byLevel-1);

						if(nMaxLevel>=(int)pEffect->dwMinMastery)
							SetRender(SPR_OBJ_SKILL_UP1+2+lpSkillResourceEx->byTypeIndex*2, TRUE);						
					}
				}
			}
		}
		// 패시브 //
		pos = g_sSkillListManager.pSkillList[bySkillIndex].pPassiveList->GetHeadPosition();	

		while(pos)
		{	
			LP_SKILL_RESOURCE_EX lpSkillResourceEx = (LP_SKILL_RESOURCE_EX)g_sSkillListManager.pSkillList[bySkillIndex].pPassiveList->GetNext(pos);

			if(lpSkillResourceEx)
			{
				Effect* pEffect = g_pEffectLayer->GetEffectInfo(lpSkillResourceEx->wId);
				
				if(pEffect->bAbleClass&m_byBitClassType)
				{				
					if(g_pMainPlayer->sSkillTable[lpSkillResourceEx->wId].bSKillLevel<CUserInterface::GetInstance()->m_wMaxSkillLevel)
					{
						int		nMaxLevel		= 0;
						BYTE	byMastery		= g_pEffectLayer->GetSkillMasteryKind(lpSkillResourceEx->wId);
						BYTE	byLevel			= g_pMainPlayer->sSkillTable[byMastery].bSKillLevel;
						Effect* pEffect			= g_pEffectLayer->GetEffectInfo(lpSkillResourceEx->wId);
						Effect* pMasteryEffect	= g_pEffectLayer->GetEffectInfo(byMastery);

						if(byLevel!=0)
							nMaxLevel = pMasteryEffect->GetMaxMastery(byLevel-1);

						if(nMaxLevel>=(int)pEffect->dwMinMastery)
							SetRender(SPR_OBJ_SKILL_UP1+30+lpSkillResourceEx->byTypeIndex*2, TRUE);
					}				
				}
			}
		}
		// 오버드라이브 //
		pos = g_sSkillListManager.pSkillList[bySkillIndex].pOverDriveList->GetHeadPosition();	

		while(pos)
		{	
			LP_SKILL_RESOURCE_EX lpSkillResourceEx = (LP_SKILL_RESOURCE_EX)g_sSkillListManager.pSkillList[bySkillIndex].pOverDriveList->GetNext(pos);

			if(lpSkillResourceEx)
			{
				Effect* pEffect = g_pEffectLayer->GetEffectInfo(lpSkillResourceEx->wId);
				
				if(pEffect->bAbleClass&m_byBitClassType)
				{
					if(g_pMainPlayer->sSkillTable[lpSkillResourceEx->wId].bSKillLevel<CUserInterface::GetInstance()->m_wMaxSkillLevel)
					{
						int		nMaxLevel		= 0;
						BYTE	byMastery		= g_pEffectLayer->GetSkillMasteryKind(lpSkillResourceEx->wId);
						BYTE	byLevel			= g_pMainPlayer->sSkillTable[byMastery].bSKillLevel;
						Effect* pEffect			= g_pEffectLayer->GetEffectInfo(lpSkillResourceEx->wId);
						Effect* pMasteryEffect	= g_pEffectLayer->GetEffectInfo(byMastery);

						if(byLevel!=0)
							nMaxLevel = pMasteryEffect->GetMaxMastery(byLevel-1);

						if(nMaxLevel>=(int)pEffect->dwMinMastery)
							SetRender(SPR_OBJ_SKILL_UP1+44+lpSkillResourceEx->byTypeIndex*2, TRUE);						
					}				
				}
			}
		}
	}
}

int CSkillWnd::CheckInterface()
{
	m_bMouseIcon = FALSE;

	int nRt = GetChk();

	InitValue(nRt);

	if(m_bBtnChk[0]==TRUE && !(nRt>=4 && nRt<=8))
	{
		SetSkillButtonPos();
		m_bBtnChk[0] = FALSE;
	}

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
				SetRender(SPR_OBJ_SKILL_CLOSE1, FALSE);
				SetRender(SPR_OBJ_SKILL_CLOSE2, TRUE);
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
			if(eMouseCheck==MOUSE_DOWN)
			{
				if(m_bySkillType!=m_bClassType[nRt-4])
				m_bySkillType = m_bClassType[nRt-4];

				SetSkillButtonPos();
				SetSkillUpBtn();
			}
			else if(eMouseCheck==MOUSE_OVER)
			{
				SetSkillButtonPos();
		
				if(m_bySkillType!=m_bClassType[nRt-4])
				{				
					switch(m_bClassType[nRt-4])
					{
						case CLASS_TYPE_WARRIOR:
							SetRender(SPR_OBJ_SKILL_AURA2, TRUE);
							SetRender(SPR_OBJ_SKILL_AURA1, FALSE);
							break;
						case CLASS_TYPE_PRIEST:
							SetRender(SPR_OBJ_SKILL_DIVINE2, TRUE);
							SetRender(SPR_OBJ_SKILL_DIVINE1, FALSE);
							break;
						case CLASS_TYPE_SUMMONER:
							SetRender(SPR_OBJ_SKILL_SUMMON2, TRUE);
							SetRender(SPR_OBJ_SKILL_SUMMON1, FALSE);
							break;
						case CLASS_TYPE_HUNTER:
							SetRender(SPR_OBJ_SKILL_CHAKRA2, TRUE);
							SetRender(SPR_OBJ_SKILL_CHAKRA1, FALSE);
							break;
						case CLASS_TYPE_WIZARD:
							SetRender(SPR_OBJ_SKILL_MANA2, TRUE);
							SetRender(SPR_OBJ_SKILL_MANA1, FALSE);
							break;
					}
				}
				m_bBtnChk[0] = TRUE;
			}
			SetMouseCheck(nRt, eMouseCheck);
		}		
		break;
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
	case 16:
	case 17:
	case 18:
	case 19:
	case 20:
	case 21:
	case 22:
	case 23:
	case 24:
	case 25:
	case 26:
	case 27:
	case 28:
	case 29:
	case 30:
	case 31:
	case 32:
	case 33:
		{
			BOOL bChk	= FALSE;
			BYTE byMousePosChk	= 0;

			if(eMouseCheck==MOUSE_UP)
			{
				BYTE byIndex = 0; 
	
				if ( m_bySkillType >=0 || m_bySkillType <= 5)
				{
					byIndex = m_bySkillType % 5;
				}
				else
				{
					// "Invalid Skill Type"		by minjin.
					_asm int 3
				}
			/*	for more simple code
				switch(m_bySkillType)
				{
				case CLASS_TYPE_WARRIOR:
					byIndex = 1;
					break;
				case CLASS_TYPE_PRIEST:
					byIndex = 2;
					break;
				case CLASS_TYPE_SUMMONER:						
					byIndex = 3;
					break;
				case CLASS_TYPE_HUNTER:
					byIndex = 4;
					break;
				case CLASS_TYPE_WIZARD:
					byIndex = 0;
					break;
				}		//*/


				if(nRt==9)
				{
#ifdef __SKILL_MASTERY_ENABLE
					POSITION_ pos = g_sSkillListManager.pSkillList[byIndex].pMasteryList->GetHeadPosition();

					while(pos)
					{	
						LP_SKILL_RESOURCE_EX lpSkillResourceEx = (LP_SKILL_RESOURCE_EX)g_sSkillListManager.pSkillList[byIndex].pMasteryList->GetNext(pos);

						if(lpSkillResourceEx)
						{
							if(g_pMainPlayer->sSkillTable[lpSkillResourceEx->wId].bSKillLevel<CUserInterface::GetInstance()->m_wMaxMasteryLevel)
							{
								Effect* pEffect = g_pEffectLayer->GetEffectInfo(lpSkillResourceEx->wId);
					
								if(pEffect->bAbleClass&m_byBitClassType)
								{
									if ( g_pMainPlayer->m_wPointSkill <= 0 )
									{
										SetRender(SPR_OBJ_SKILL_UP1, TRUE);
										SetRender(SPR_OBJ_SKILL_UP1+1, FALSE);																				
										CTDS_SKILL_LEVELUP pPacket;										
										pPacket.nSkillIndex	= lpSkillResourceEx->wId;
										g_pNet->SendMsg((char*)&pPacket, sizeof(CTDS_SKILL_LEVELUP), SERVER_INDEX_ZONE);
										CUserInterface::GetInstance()->SetPointer(__MOUSE_POINTER_BUTTONCLICK__);										
									}
									else
										byMousePosChk = __MOUSE_POINTER_DEFAULT__;										
								}
								else
									byMousePosChk = __MOUSE_POINTER_DEFAULT__;
							}
							else
								byMousePosChk = __MOUSE_POINTER_DEFAULT__;
						}
					}
#else
					byMousePosChk = __MOUSE_POINTER_DEFAULT__;
#endif
				}
				else if(nRt>=10 && nRt<=23)
				{
 					POSITION_ pos = g_sSkillListManager.pSkillList[byIndex].pActiveList->GetHeadPosition();	

					while(pos)
					{	
						LP_SKILL_RESOURCE_EX lpSkillResourceEx = (LP_SKILL_RESOURCE_EX)g_sSkillListManager.pSkillList[byIndex].pActiveList->GetNext(pos);

						if(nRt-10==lpSkillResourceEx->byTypeIndex)
						{
							if(lpSkillResourceEx)
							{
								Effect* pEffect = g_pEffectLayer->GetEffectInfo(lpSkillResourceEx->wId);
					
								if(pEffect->bAbleClass&m_byBitClassType)
								{
									if(g_pMainPlayer->sSkillTable[lpSkillResourceEx->wId].bSKillLevel<CUserInterface::GetInstance()->m_wMaxSkillLevel)
									{
										int		nMaxLevel		= 0;
										BYTE	byMastery		= g_pEffectLayer->GetSkillMasteryKind(lpSkillResourceEx->wId);
										BYTE	byLevel			= g_pMainPlayer->sSkillTable[byMastery].bSKillLevel;
										Effect* pEffect			= g_pEffectLayer->GetEffectInfo(lpSkillResourceEx->wId);
										Effect* pMasteryEffect	= g_pEffectLayer->GetEffectInfo(byMastery);

										if(byLevel!=0)
											nMaxLevel = pMasteryEffect->GetMaxMastery(byLevel-1);

										if(nMaxLevel>=(int)pEffect->dwMinMastery)
										{
											if(g_pMainPlayer->m_wPointSkill>0)
											{
												SetRender(SPR_OBJ_SKILL_UP1+2+lpSkillResourceEx->byTypeIndex*2, TRUE);
												SetRender(SPR_OBJ_SKILL_UP1+3+lpSkillResourceEx->byTypeIndex*2, FALSE);
												
												CTDS_SKILL_LEVELUP pPacket;												
												pPacket.nSkillIndex	= lpSkillResourceEx->wId;
												g_pNet->SendMsg((char*)&pPacket, sizeof(CTDS_SKILL_LEVELUP), SERVER_INDEX_ZONE);												
											}
											else
												byMousePosChk = __MOUSE_POINTER_DEFAULT__;
										}
										else
											byMousePosChk = __MOUSE_POINTER_DEFAULT__;
									}
									else
										byMousePosChk = __MOUSE_POINTER_DEFAULT__;
								}
								else
									byMousePosChk = __MOUSE_POINTER_DEFAULT__;
							}
							break;
						}					
					}
				}
				else if(nRt>=24 && nRt<=30)
				{		
					POSITION_ pos = g_sSkillListManager.pSkillList[byIndex].pPassiveList->GetHeadPosition();	

					while(pos)
					{	
						LP_SKILL_RESOURCE_EX lpSkillResourceEx = (LP_SKILL_RESOURCE_EX)g_sSkillListManager.pSkillList[byIndex].pPassiveList->GetNext(pos);

						if(nRt-24==lpSkillResourceEx->byTypeIndex)
						{
							if(lpSkillResourceEx)
							{
								Effect* pEffect = g_pEffectLayer->GetEffectInfo(lpSkillResourceEx->wId);
					
								if(pEffect->bAbleClass&m_byBitClassType)
								{
									if(g_pMainPlayer->sSkillTable[lpSkillResourceEx->wId].bSKillLevel<CUserInterface::GetInstance()->m_wMaxSkillLevel)
									{
										int		nMaxLevel		= 0;
										BYTE	byMastery		= g_pEffectLayer->GetSkillMasteryKind(lpSkillResourceEx->wId);
										BYTE	byLevel			= g_pMainPlayer->sSkillTable[byMastery].bSKillLevel;
										Effect* pEffect			= g_pEffectLayer->GetEffectInfo(lpSkillResourceEx->wId);
										Effect* pMasteryEffect	= g_pEffectLayer->GetEffectInfo(byMastery);

										if(byLevel!=0)
											nMaxLevel = pMasteryEffect->GetMaxMastery(byLevel-1);
										
										if(nMaxLevel>=(int)pEffect->dwMinMastery)
										{
											if(g_pMainPlayer->m_wPointSkill>0)
											{
												SetRender(SPR_OBJ_SKILL_UP1+30+lpSkillResourceEx->byTypeIndex*2, TRUE);
												SetRender(SPR_OBJ_SKILL_UP1+31+lpSkillResourceEx->byTypeIndex*2, FALSE);

												CTDS_SKILL_LEVELUP pPacket;
												pPacket.nSkillIndex	= lpSkillResourceEx->wId;
												g_pNet->SendMsg((char*)&pPacket, sizeof(CTDS_SKILL_LEVELUP), SERVER_INDEX_ZONE);												
											}
											else
												byMousePosChk = __MOUSE_POINTER_DEFAULT__;
										}
										else
											byMousePosChk = __MOUSE_POINTER_DEFAULT__;
									}
									else
										byMousePosChk = __MOUSE_POINTER_DEFAULT__;
								}
								else
									byMousePosChk = __MOUSE_POINTER_DEFAULT__;
							}
							break;
						}
					}
				}
				else if(nRt>=31 && nRt<=33)
				{
					POSITION_ pos = g_sSkillListManager.pSkillList[byIndex].pOverDriveList->GetHeadPosition();	

					while(pos)
					{	
						LP_SKILL_RESOURCE_EX lpSkillResourceEx = (LP_SKILL_RESOURCE_EX)g_sSkillListManager.pSkillList[byIndex].pOverDriveList->GetNext(pos);

						if(nRt-31==lpSkillResourceEx->byTypeIndex)
						{
							if(lpSkillResourceEx)
							{
								Effect* pEffect = g_pEffectLayer->GetEffectInfo(lpSkillResourceEx->wId);
					
								if(pEffect->bAbleClass&m_byBitClassType)
								{
									if(g_pMainPlayer->sSkillTable[lpSkillResourceEx->wId].bSKillLevel<CUserInterface::GetInstance()->m_wMaxSkillLevel)
									{
										int		nMaxLevel		= 0;
										BYTE	byMastery		= g_pEffectLayer->GetSkillMasteryKind(lpSkillResourceEx->wId);
										BYTE	byLevel			= g_pMainPlayer->sSkillTable[byMastery].bSKillLevel;
										Effect* pEffect			= g_pEffectLayer->GetEffectInfo(lpSkillResourceEx->wId);
										Effect* pMasteryEffect	= g_pEffectLayer->GetEffectInfo(byMastery);

										if(byLevel!=0)
											nMaxLevel = pMasteryEffect->GetMaxMastery(byLevel-1);

										if(nMaxLevel>=(int)pEffect->dwMinMastery)
										{
											if(g_pMainPlayer->m_wPointSkill>0)
											{
												SetRender(SPR_OBJ_SKILL_UP1+44+lpSkillResourceEx->byTypeIndex*2, TRUE);
												SetRender(SPR_OBJ_SKILL_UP1+45+lpSkillResourceEx->byTypeIndex*2, FALSE);

												CTDS_SKILL_LEVELUP pPacket;												
												pPacket.nSkillIndex	= lpSkillResourceEx->wId;
												g_pNet->SendMsg((char*)&pPacket, sizeof(CTDS_SKILL_LEVELUP), SERVER_INDEX_ZONE);												
											}
											else
												byMousePosChk = __MOUSE_POINTER_DEFAULT__;
										}
										else
											byMousePosChk = __MOUSE_POINTER_DEFAULT__;
									}
									else
										byMousePosChk = __MOUSE_POINTER_DEFAULT__;
								}
								else
									byMousePosChk = __MOUSE_POINTER_DEFAULT__;
							}
							break;
						}
					}
				}				
			}	
			else if(eMouseCheck==MOUSE_DOWN)
			{			
				BYTE byIndex = 0; 
		
				switch(m_bySkillType)
				{
				case CLASS_TYPE_WARRIOR:
					byIndex = 1;
					break;
				case CLASS_TYPE_PRIEST:
					byIndex = 2;
					break;
				case CLASS_TYPE_SUMMONER:
					byIndex = 3;
					break;
				case CLASS_TYPE_HUNTER:
					byIndex = 4;
					break;
				case CLASS_TYPE_WIZARD:
					byIndex = 0;
					break;
				}
				
				if(nRt==9)
				{
#ifdef __SKILL_MASTERY_ENABLE
					POSITION_ pos = g_sSkillListManager.pSkillList[byIndex].pMasteryList->GetHeadPosition();

					while(pos)
					{	
						LP_SKILL_RESOURCE_EX lpSkillResourceEx = (LP_SKILL_RESOURCE_EX)g_sSkillListManager.pSkillList[byIndex].pMasteryList->GetNext(pos);

						if(lpSkillResourceEx)
						{
							Effect* pEffect = g_pEffectLayer->GetEffectInfo(lpSkillResourceEx->wId);
					
							if(pEffect->bAbleClass&m_byBitClassType)
							{
								if(g_pMainPlayer->sSkillTable[lpSkillResourceEx->wId].bSKillLevel<CUserInterface::GetInstance()->m_wMaxMasteryLevel)
								{
									if(g_pMainPlayer->m_wPointSkill>0)
									{
										SetRender(SPR_OBJ_SKILL_UP1, FALSE);
										SetRender(SPR_OBJ_SKILL_UP1+1, TRUE);
									}
									else
										byMousePosChk = __MOUSE_POINTER_DEFAULTCLICK__;										
								}
								else
									byMousePosChk = __MOUSE_POINTER_DEFAULTCLICK__;

								if(!byMousePosChk)
									bChk = TRUE;
							}
							else
								byMousePosChk = __MOUSE_POINTER_DEFAULTCLICK__;
						}
					}

					if(byMousePosChk==0 && !bChk)
						byMousePosChk = __MOUSE_POINTER_DEFAULTCLICK__;
#else 
					if(byMousePosChk==0 && !bChk)
						byMousePosChk = __MOUSE_POINTER_DEFAULTCLICK__;
#endif					
				}
				else if(nRt>=10 && nRt<=23)
				{
					POSITION_ pos = g_sSkillListManager.pSkillList[byIndex].pActiveList->GetHeadPosition();	

					while(pos)
					{	
						LP_SKILL_RESOURCE_EX lpSkillResourceEx = (LP_SKILL_RESOURCE_EX)g_sSkillListManager.pSkillList[byIndex].pActiveList->GetNext(pos);

						if(nRt-10==lpSkillResourceEx->byTypeIndex)
						{
							if(lpSkillResourceEx)
							{
								Effect* pEffect = g_pEffectLayer->GetEffectInfo(lpSkillResourceEx->wId);
					
								if(pEffect->bAbleClass&m_byBitClassType)
								{
									if(g_pMainPlayer->sSkillTable[lpSkillResourceEx->wId].bSKillLevel<CUserInterface::GetInstance()->m_wMaxSkillLevel)
									{
										int		nMaxLevel		= 0;
										BYTE	byMastery		= g_pEffectLayer->GetSkillMasteryKind(lpSkillResourceEx->wId);
										BYTE	byLevel			= g_pMainPlayer->sSkillTable[byMastery].bSKillLevel;
										Effect* pEffect			= g_pEffectLayer->GetEffectInfo(lpSkillResourceEx->wId);
										Effect* pMasteryEffect	= g_pEffectLayer->GetEffectInfo(byMastery);

										if(byLevel!=0)
											nMaxLevel = pMasteryEffect->GetMaxMastery(byLevel-1);

										if(nMaxLevel>=(int)pEffect->dwMinMastery)
										{
											if(g_pMainPlayer->m_wPointSkill>0)
											{
												SetRender(SPR_OBJ_SKILL_UP1+2+lpSkillResourceEx->byTypeIndex*2, FALSE);
												SetRender(SPR_OBJ_SKILL_UP1+3+lpSkillResourceEx->byTypeIndex*2, TRUE);												
											}									
											else
												byMousePosChk = __MOUSE_POINTER_DEFAULTCLICK__;
										}
										else
											byMousePosChk = __MOUSE_POINTER_DEFAULTCLICK__;

										if(!byMousePosChk)
											bChk = TRUE;
									}
									else
										byMousePosChk = __MOUSE_POINTER_DEFAULTCLICK__;
								}
								else
									byMousePosChk = __MOUSE_POINTER_DEFAULTCLICK__;
							}
							break;
						}					
					}

					if(byMousePosChk==0 && !bChk)
						byMousePosChk = __MOUSE_POINTER_DEFAULTCLICK__;
				}
				else if(nRt>=24 && nRt<=30)
				{		
					POSITION_ pos = g_sSkillListManager.pSkillList[byIndex].pPassiveList->GetHeadPosition();	

					while(pos)
					{	
						LP_SKILL_RESOURCE_EX lpSkillResourceEx = (LP_SKILL_RESOURCE_EX)g_sSkillListManager.pSkillList[byIndex].pPassiveList->GetNext(pos);

						if(nRt-24==lpSkillResourceEx->byTypeIndex)
						{
							if(lpSkillResourceEx)
							{
								Effect* pEffect = g_pEffectLayer->GetEffectInfo(lpSkillResourceEx->wId);
					
								if(pEffect->bAbleClass&m_byBitClassType)
								{
									if(g_pMainPlayer->sSkillTable[lpSkillResourceEx->wId].bSKillLevel<CUserInterface::GetInstance()->m_wMaxSkillLevel)
									{
										int		nMaxLevel		= 0;
										BYTE	byMastery		= g_pEffectLayer->GetSkillMasteryKind(lpSkillResourceEx->wId);
										BYTE	byLevel			= g_pMainPlayer->sSkillTable[byMastery].bSKillLevel;
										Effect* pEffect			= g_pEffectLayer->GetEffectInfo(lpSkillResourceEx->wId);
										Effect* pMasteryEffect	= g_pEffectLayer->GetEffectInfo(byMastery);

										if(byLevel!=0)
											nMaxLevel = pMasteryEffect->GetMaxMastery(byLevel-1);
										
										if(nMaxLevel>=(int)pEffect->dwMinMastery)
										{
											if(g_pMainPlayer->m_wPointSkill>0)
											{
												SetRender(SPR_OBJ_SKILL_UP1+30+lpSkillResourceEx->byTypeIndex*2, FALSE);
												SetRender(SPR_OBJ_SKILL_UP1+31+lpSkillResourceEx->byTypeIndex*2, TRUE);												
											}									
											else
												byMousePosChk = __MOUSE_POINTER_DEFAULTCLICK__;
										}
										else
											byMousePosChk = __MOUSE_POINTER_DEFAULTCLICK__;

										if(!byMousePosChk)
											bChk = TRUE;
									}
									else
										byMousePosChk = __MOUSE_POINTER_DEFAULTCLICK__;
								}
								else
									byMousePosChk = __MOUSE_POINTER_DEFAULTCLICK__;
							}
							break;
						}
					}

					if(byMousePosChk==0 && !bChk)
						byMousePosChk = __MOUSE_POINTER_DEFAULTCLICK__;
				}
				else if(nRt>=31 && nRt<=33)
				{
					POSITION_ pos = g_sSkillListManager.pSkillList[byIndex].pOverDriveList->GetHeadPosition();	

					while(pos)
					{	
						LP_SKILL_RESOURCE_EX lpSkillResourceEx = (LP_SKILL_RESOURCE_EX)g_sSkillListManager.pSkillList[byIndex].pOverDriveList->GetNext(pos);

						if(nRt-31==lpSkillResourceEx->byTypeIndex)
						{
							if(lpSkillResourceEx)
							{
								Effect* pEffect = g_pEffectLayer->GetEffectInfo(lpSkillResourceEx->wId);
					
								if(pEffect->bAbleClass&m_byBitClassType)
								{
									if(g_pMainPlayer->sSkillTable[lpSkillResourceEx->wId].bSKillLevel<CUserInterface::GetInstance()->m_wMaxSkillLevel)
									{
										int		nMaxLevel		= 0;
										BYTE	byMastery		= g_pEffectLayer->GetSkillMasteryKind(lpSkillResourceEx->wId);
										BYTE	byLevel			= g_pMainPlayer->sSkillTable[byMastery].bSKillLevel;
										Effect* pEffect			= g_pEffectLayer->GetEffectInfo(lpSkillResourceEx->wId);
										Effect* pMasteryEffect	= g_pEffectLayer->GetEffectInfo(byMastery);

										if(byLevel!=0)
											nMaxLevel = pMasteryEffect->GetMaxMastery(byLevel-1);

										if(nMaxLevel>=(int)pEffect->dwMinMastery)
										{
											if(g_pMainPlayer->m_wPointSkill>0)
											{
												SetRender(SPR_OBJ_SKILL_UP1+44+lpSkillResourceEx->byTypeIndex*2, FALSE);
												SetRender(SPR_OBJ_SKILL_UP1+45+lpSkillResourceEx->byTypeIndex*2, TRUE);
											}																		
											else
												byMousePosChk = __MOUSE_POINTER_DEFAULTCLICK__;
										}
										else
											byMousePosChk = __MOUSE_POINTER_DEFAULTCLICK__;

										if(!byMousePosChk)
											bChk = TRUE;
									}
									else
										byMousePosChk = __MOUSE_POINTER_DEFAULTCLICK__;
								}
								else
									byMousePosChk = __MOUSE_POINTER_DEFAULTCLICK__;
							}
							break;
						}
					}

					if(byMousePosChk==0 && !bChk)
						byMousePosChk = __MOUSE_POINTER_DEFAULTCLICK__;
				}
			}
			else if(eMouseCheck==MOUSE_OVER)
			{
				BYTE byIndex = 0; 
		
				switch(m_bySkillType)
				{
				case CLASS_TYPE_WARRIOR:
					byIndex = 1;
					break;
				case CLASS_TYPE_PRIEST:
					byIndex = 2;
					break;
				case CLASS_TYPE_SUMMONER:
					byIndex = 3;
					break;
				case CLASS_TYPE_HUNTER:
					byIndex = 4;
					break;
				case CLASS_TYPE_WIZARD:
					byIndex = 0;
					break;
				}
				
				if(nRt==9)
				{

#ifdef __SKILL_MASTERY_ENABLE

					POSITION_ pos = g_sSkillListManager.pSkillList[byIndex].pMasteryList->GetHeadPosition();

					while(pos)
					{	
						LP_SKILL_RESOURCE_EX lpSkillResourceEx = (LP_SKILL_RESOURCE_EX)g_sSkillListManager.pSkillList[byIndex].pMasteryList->GetNext(pos);

						if(lpSkillResourceEx)
						{
							Effect* pEffect = g_pEffectLayer->GetEffectInfo(lpSkillResourceEx->wId);
					
							if(pEffect->bAbleClass&m_byBitClassType)
							{
								if(g_pMainPlayer->sSkillTable[lpSkillResourceEx->wId].bSKillLevel>=CUserInterface::GetInstance()->m_wMaxMasteryLevel)								
									byMousePosChk = __MOUSE_POINTER_DEFAULT__;
							}
							else
								byMousePosChk = __MOUSE_POINTER_DEFAULT__;

							if(!byMousePosChk)
								bChk = TRUE;
						}
					}

					if(byMousePosChk==0 && !bChk)
						byMousePosChk = __MOUSE_POINTER_DEFAULT__;
#else
					byMousePosChk = __MOUSE_POINTER_DEFAULT__;
#endif

				}
				else if(nRt>=10 && nRt<=23)
				{
					POSITION_ pos = g_sSkillListManager.pSkillList[byIndex].pActiveList->GetHeadPosition();	

					while(pos)
					{	
						LP_SKILL_RESOURCE_EX lpSkillResourceEx = (LP_SKILL_RESOURCE_EX)g_sSkillListManager.pSkillList[byIndex].pActiveList->GetNext(pos);

						if(nRt-10==lpSkillResourceEx->byTypeIndex)
						{
							if(lpSkillResourceEx)
							{
								Effect* pEffect = g_pEffectLayer->GetEffectInfo(lpSkillResourceEx->wId);
					
								if(pEffect->bAbleClass&m_byBitClassType)
								{
									if(g_pMainPlayer->sSkillTable[lpSkillResourceEx->wId].bSKillLevel<CUserInterface::GetInstance()->m_wMaxSkillLevel)
									{
										int		nMaxLevel		= 0;
										BYTE	byMastery		= g_pEffectLayer->GetSkillMasteryKind(lpSkillResourceEx->wId);
										BYTE	byLevel			= g_pMainPlayer->sSkillTable[byMastery].bSKillLevel;
										Effect* pEffect			= g_pEffectLayer->GetEffectInfo(lpSkillResourceEx->wId);
										Effect* pMasteryEffect	= g_pEffectLayer->GetEffectInfo(byMastery);

										if(byLevel!=0)
											nMaxLevel = pMasteryEffect->GetMaxMastery(byLevel-1);

										if(nMaxLevel>=(int)pEffect->dwMinMastery)
										{
											if(g_pMainPlayer->m_wPointSkill<=0)												
												byMousePosChk = __MOUSE_POINTER_DEFAULT__;
										}
										else
											byMousePosChk = __MOUSE_POINTER_DEFAULT__;

										if(!byMousePosChk)
											bChk = TRUE;
									}
								}
								else
									byMousePosChk = __MOUSE_POINTER_DEFAULT__;
							}
							break;
						}					
					}

					if(byMousePosChk==0 && !bChk)
						byMousePosChk = __MOUSE_POINTER_DEFAULT__;
				}
				else if(nRt>=24 && nRt<=30)
				{		
					POSITION_ pos = g_sSkillListManager.pSkillList[byIndex].pPassiveList->GetHeadPosition();	

					while(pos)
					{	
						LP_SKILL_RESOURCE_EX lpSkillResourceEx = (LP_SKILL_RESOURCE_EX)g_sSkillListManager.pSkillList[byIndex].pPassiveList->GetNext(pos);

						if(nRt-24==lpSkillResourceEx->byTypeIndex)
						{
							if(lpSkillResourceEx)
							{
								Effect* pEffect = g_pEffectLayer->GetEffectInfo(lpSkillResourceEx->wId);
					
								if(pEffect->bAbleClass&m_byBitClassType)
								{
									if(g_pMainPlayer->sSkillTable[lpSkillResourceEx->wId].bSKillLevel<CUserInterface::GetInstance()->m_wMaxSkillLevel)
									{
										int		nMaxLevel		= 0;
										BYTE	byMastery		= g_pEffectLayer->GetSkillMasteryKind(lpSkillResourceEx->wId);
										BYTE	byLevel			= g_pMainPlayer->sSkillTable[byMastery].bSKillLevel;
										Effect* pEffect			= g_pEffectLayer->GetEffectInfo(lpSkillResourceEx->wId);
										Effect* pMasteryEffect	= g_pEffectLayer->GetEffectInfo(byMastery);

										if(byLevel!=0)
											nMaxLevel = pMasteryEffect->GetMaxMastery(byLevel-1);
										
										if(byLevel>=(int)pEffect->dwMinMastery)
										{		
											if(g_pMainPlayer->m_wPointSkill>0)
												CUserInterface::GetInstance()->SetPointer(__MOUSE_POINTER_BUTTON__);
											else
												byMousePosChk = __MOUSE_POINTER_DEFAULT__;
										}
										else
											byMousePosChk = __MOUSE_POINTER_DEFAULT__;

										if(!byMousePosChk)
											bChk = TRUE;
									}
								}
								else
									byMousePosChk = __MOUSE_POINTER_DEFAULT__;
							}
							break;
						}
					}
					
					if(byMousePosChk==0 && !bChk)
						byMousePosChk = __MOUSE_POINTER_DEFAULT__;
				}
				else if(nRt>=31 && nRt<=33)
				{
					POSITION_ pos = g_sSkillListManager.pSkillList[byIndex].pOverDriveList->GetHeadPosition();	

					while(pos)
					{	
						LP_SKILL_RESOURCE_EX lpSkillResourceEx = (LP_SKILL_RESOURCE_EX)g_sSkillListManager.pSkillList[byIndex].pOverDriveList->GetNext(pos);

						if(nRt-31==lpSkillResourceEx->byTypeIndex)
						{
							if(lpSkillResourceEx)
							{
								Effect* pEffect = g_pEffectLayer->GetEffectInfo(lpSkillResourceEx->wId);
					
								if(pEffect->bAbleClass&m_byBitClassType)
								{							
									if(g_pMainPlayer->sSkillTable[lpSkillResourceEx->wId].bSKillLevel<CUserInterface::GetInstance()->m_wMaxSkillLevel)
									{
										int		nMaxLevel		= 0;
										BYTE	byMastery		= g_pEffectLayer->GetSkillMasteryKind(lpSkillResourceEx->wId);
										BYTE	byLevel			= g_pMainPlayer->sSkillTable[byMastery].bSKillLevel;
										Effect* pEffect			= g_pEffectLayer->GetEffectInfo(lpSkillResourceEx->wId);
										Effect* pMasteryEffect	= g_pEffectLayer->GetEffectInfo(byMastery);

										if(byLevel!=0)
											nMaxLevel = pMasteryEffect->GetMaxMastery(byLevel-1);

										if(nMaxLevel>=(int)pEffect->dwMinMastery)
										{
											if(g_pMainPlayer->m_wPointSkill<=0)												
												byMousePosChk = __MOUSE_POINTER_DEFAULT__;
										}
										else
											byMousePosChk = __MOUSE_POINTER_DEFAULT__;

										if(!byMousePosChk)
											bChk = TRUE;
									}
								}
								else
									byMousePosChk = __MOUSE_POINTER_DEFAULT__;
							}
							break;
						}
					}

					if(byMousePosChk==0 && !bChk)
						byMousePosChk = __MOUSE_POINTER_DEFAULT__;
				}			
			}
			SetMouseCheck(nRt, eMouseCheck, byMousePosChk);
		}
		break;				
	}	
	
	if(MouseUp())
	{
		SetRender(SPR_OBJ_SKILL_CLOSE1, TRUE);
		SetRender(SPR_OBJ_SKILL_CLOSE2, FALSE);
	}

	MouseChk(nRt);
	
	if(SetMoveWnd(nRt))		
		return 1;

	if(IsReturn(nRt, eMouseCheck))
		return 1;		

	return 0;
}

void CSkillWnd::RenderUsing()
{
	
	POSITION_ pos =  g_pMainPlayer->m_pUsingStatusEffectList->GetHeadPosition();

	char	szInfo[0xff] = {0,};
//	RECT	rtPos;
	BYTE	bySkillIndex = 0; 
	int		nSize = 0;
		
	VECTOR2 vPos;

	while(pos)
	{
		EffectDesc* pEffectDesc = (EffectDesc*)g_pMainPlayer->m_pUsingStatusEffectList->GetNext(pos);		

		if(!g_sSkillListManager.pSpr[pEffectDesc->pEffect->bID])
			continue;

		int nPosX	= (int)10+(bySkillIndex*35);
		int nPosY	= (int)10;
			
		vPos.x	= (float)nPosX;
		vPos.y	= (float)nPosY;
												
		g_pRenderer->RenderSprite(g_sSkillListManager.pSpr[pEffectDesc->pEffect->bID], NULL, 0.0f, &vPos, NULL, 0xffffffff, __ORDER_USERINTERFACE_START_, RENDER_TYPE_DISABLE_TEX_FILTERING);
					
		wsprintf(szInfo, "%u", pEffectDesc->GetRemainTime(g_dwCurTick));

		nSize = lstrlen(szInfo);
		RenderFont(szInfo, nPosX+30-(nSize*7), nPosX+30, nPosY+17, nPosY+31, __ORDER_USERINTERFACE_START_+1, 0xffff8383);		
		bySkillIndex++;
		
	}	

	if(CUserInterface::GetInstance()->m_dwMagicArray!=0)
	{
		LP_ITEM_RESOURCE_EX lpItemResource = g_pItemResourceHash->GetData(CUserInterface::GetInstance()->m_dwMagicArray);		

		if(lpItemResource)
		{			
			int nPosX	= (int)10+(bySkillIndex*35);
			int nPosY	= (int)10;
				
			vPos.x	= (float)nPosX;
			vPos.y	= (float)nPosY;
			
			g_pRenderer->RenderSprite(lpItemResource->pSpr, NULL, 0.0f, &vPos, NULL, 0xffffffff, __ORDER_USERINTERFACE_START_, RENDER_TYPE_DISABLE_TEX_FILTERING);
		}
	}
}
//======================================================//
// End.													//
//======================================================//
