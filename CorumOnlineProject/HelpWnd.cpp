//======================================================//
// Code by Jang.							2002.12.23	//
//======================================================//
#include "HelpWnd.h"
#include "GameControl.h"
#include "UserInterface.h"
#include "Interface.h"
#include "message.h"
#include "InitGame.h"
#include "HashTable.h"
#include "NetworkClient.h"
#include "CodeFun.h"

CHelpWnd* CHelpWnd::c_pThis = NULL;

//======================================================//
// Construction/Destrution.								//
//======================================================//
CHelpWnd::CHelpWnd()
{ 
	m_byType		= 0;	
	m_bySubType		= 0;		
	m_bPointer		= FALSE;
	m_bBtnChk[0]	= FALSE;
	m_bBtnChk[1]	= FALSE;
}

CHelpWnd::~CHelpWnd()
{
}
//======================================================//
// Class Member Function.								//
//======================================================//
BOOL CHelpWnd::Init()
{	
/*
	InsertData(SPR_OBJ_HELP_WND1, SPR_INTERFACE_DF_WND1, 0, 0, 1.0f, 1.0f, 0);
	InsertData(SPR_OBJ_HELP_WND2, SPR_INTERFACE_DF_WND2, 0, 32, 1.0f, 13.0f, 0);
	InsertData(SPR_OBJ_HELP_WND3, SPR_INTERFACE_DF_WND3, 0, 240, 1.0f, 1.0f, 0);
	InsertData(SPR_OBJ_HELP_CLOSE1, SPR_INTERFACE_CLOSE1, 242, 4, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_HELP_CLOSE2, SPR_INTERFACE_CLOSE2, 242, 4, 1.0f, 1.0f, 1);	
	InsertData(SPR_OBJ_HELP_NEXT1, SPR_INTERFACE_HELP_NEXT1, 84, 216, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_HELP_NEXT2, SPR_INTERFACE_HELP_NEXT2, 84, 216, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_HELP_NEXT3, SPR_INTERFACE_HELP_NEXT3, 84, 216, 1.0f, 1.0f, 2);	
	InsertData(SPR_OBJ_HELP_HP, SPR_INTERFACE_HELP_HP, 20, 85, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_HELP_SP, SPR_INTERFACE_HELP_SP, 140, 85, 1.0f, 1.0f, 2);					
	InsertData(SPR_OBJ_HELP_COOL, SPR_INTERFACE_HELP_COOL, 20, 115, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_HELP_EXP, SPR_INTERFACE_HELP_EXP, 140, 115, 1.0f, 1.0f, 2);		
	InsertData(SPR_OBJ_HELP_ITEM, SPR_INTERFACE_HELP_ITEM, 20, 135, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_HELP_CHAR, SPR_INTERFACE_HELP_CHAR, 20, 160, 1.0f, 1.0f, 2);		
	InsertData(SPR_OBJ_HELP_SKILL, SPR_INTERFACE_HELP_SKILL, 20, 185, 1.0f, 1.0f, 2);		
	InsertData(SPR_OBJ_HELP_GROUP, SPR_INTERFACE_HELP_GROUP, 20, 40, 1.0f, 1.0f, 2);			
	InsertData(SPR_OBJ_HELP_DUNGEON, SPR_INTERFACE_HELP_DUNGEON, 20, 65, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_HELP_CHAT, SPR_INTERFACE_HELP_CHAT, 20, 90, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_HELP_SHOP, SPR_INTERFACE_HELP_SHOP, 20, 115, 1.0f, 1.0f, 2);		
	InsertData(SPR_OBJ_HELP_MATCH, SPR_INTERFACE_HELP_MATCH, 128, 115, 1.0f, 1.0f, 2);	
	InsertData(SPR_OBJ_HELP_REST, SPR_INTERFACE_HELP_REST, 20, 140, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_HELP_TRADE, SPR_INTERFACE_HELP_TRADE, 128, 140, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_HELP_PK, SPR_INTERFACE_HELP_PK, 128, 165, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_HELP_OPTION, SPR_INTERFACE_HELP_OPTION, 128, 165, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_HELP_SLOT, SPR_INTERFACE_HELP_SLOT, 122, 37, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_HELP_SUPPLISE, SPR_INTERFACE_HELP_SUPPLISE, 8, 177, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_HELP_INTERFACE1, SPR_INTERFACE_HELP_INTERFACE1, 25, 150, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_HELP_UP1,  SPR_INTERFACE_HELP_UP1, 210, 99, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_HELP_UP2, SPR_INTERFACE_HELP_UP2, 58, 142, 1.0f, 1.0f, 2);		
	InsertData(SPR_OBJ_HELP_INTERFACE2, SPR_INTERFACE_HELP_INTERFACE2, 25, 160, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_HELP_INTERFACE3, SPR_INTERFACE_HELP_INTERFACE3, 128, 200, 1.0f, 1.0f, 2);

	InsertCheckInterface(242, 4, 256, 16, 2, CHECK_CLOSE);
	InsertCheckInterface(0, 0, 242, 20, 3, CHECK_MOVEWND);
	InsertCheckInterface(84, 216, 84+82, 216+12, 4, CHECK_BTN);
*/
	m_bInit	= TRUE;

	return TRUE;
}

void CHelpWnd::Remove()
{
	RemoveAllData();
	m_bActive	= FALSE;
	m_bInit		= FALSE;
}

void CHelpWnd::SetOrder()
{
}

void CHelpWnd::RenderText()
{	
	char szInfo[0xff] = {0,};

	lstrcpy(szInfo, g_Message[ETC_MESSAGE314].szMessage); // "도움말"
	
	int nSize = lstrlen(szInfo);
	RenderFont(szInfo, m_fPosX+22, m_fPosX+22+nSize*6, m_fPosZ+5, m_fPosZ+19, GetStartOrder()+1);

	RenderTextHelp();
}


void CHelpWnd::SetClear()
{
	SetRender(SPR_OBJ_HELP_HP, FALSE);
	SetRender(SPR_OBJ_HELP_SP, FALSE);
	SetRender(SPR_OBJ_HELP_EXP, FALSE);
	SetRender(SPR_OBJ_HELP_COOL, FALSE);
	SetRender(SPR_OBJ_HELP_ITEM, FALSE);
	SetRender(SPR_OBJ_HELP_CHAR, FALSE);
	SetRender(SPR_OBJ_HELP_SKILL, FALSE);
	SetRender(SPR_OBJ_HELP_NEXT1, FALSE);
	SetRender(SPR_OBJ_HELP_NEXT2, FALSE);
	SetRender(SPR_OBJ_HELP_NEXT3, FALSE);
	SetRender(SPR_OBJ_HELP_GROUP, FALSE);
	SetRender(SPR_OBJ_HELP_DUNGEON, FALSE);
	SetRender(SPR_OBJ_HELP_CHAT, FALSE);
	SetRender(SPR_OBJ_HELP_SHOP, FALSE);
	SetRender(SPR_OBJ_HELP_MATCH, FALSE);
	SetRender(SPR_OBJ_HELP_TRADE, FALSE);
	SetRender(SPR_OBJ_HELP_REST, FALSE);
	SetRender(SPR_OBJ_HELP_PK, FALSE);
	SetRender(SPR_OBJ_HELP_OPTION, FALSE);
	SetRender(SPR_OBJ_HELP_SLOT, FALSE);
	SetRender(SPR_OBJ_HELP_SUPPLISE, FALSE);
	SetRender(SPR_OBJ_HELP_INTERFACE1, FALSE);
	SetRender(SPR_OBJ_HELP_UP1, FALSE);
	SetRender(SPR_OBJ_HELP_UP2, FALSE);
	SetRender(SPR_OBJ_HELP_INTERFACE2, FALSE);
	SetRender(SPR_OBJ_HELP_INTERFACE3, FALSE);
}

void CHelpWnd::SetActive(BOOL bActive)
{
	m_bActive	= bActive;

	if(bActive==FALSE)
	{
		if(g_pMainPlayer->m_byHelp[0]==6)
		{	
			for(int i = 0; i < MAX_INV_SMALL_POOL; i++)
			{
				int nValue = g_pMainPlayer->m_pInv_Small[i].GetID()/ITEM_DISTRIBUTE;

				if(nValue==ITEM_SUPPLIES_INDEX)
				{
					for(int j = 0; j < MAX_BELT_POOL; j++)
					{
						if(g_pMainPlayer->m_pBelt[j].GetID()/ITEM_DISTRIBUTE==0)
						{
							m_byType = 7;						
							g_pMainPlayer->m_byHelp[0] = 7;
							
#if IS_JAPAN_LOCALIZING()						
							SetPosObjX(SPR_OBJ_HELP_SLOT, 67);
#else
							SetPosObjX(SPR_OBJ_HELP_SLOT, 122);
#endif
							SetPosObjY(SPR_OBJ_HELP_SLOT, 37);
							SetPosObjX(SPR_OBJ_HELP_SUPPLISE, 8);
							SetPosObjY(SPR_OBJ_HELP_SUPPLISE, 177);

							SetRender(SPR_OBJ_HELP_ITEM, FALSE);
							SetRender(SPR_OBJ_HELP_SLOT, TRUE);
							SetRender(SPR_OBJ_HELP_SUPPLISE, TRUE);					
							SetRender(SPR_OBJ_HELP_WND1, TRUE);
							SetRender(SPR_OBJ_HELP_WND2, TRUE);
							SetRender(SPR_OBJ_HELP_WND3, TRUE);
							SetRender(SPR_OBJ_HELP_CLOSE1, TRUE);	
							m_bActive = TRUE;
						
							WSTC_HELP_INFO pHelpInfoPacket;
							pHelpInfoPacket.bHelpInfo[0] = g_pMainPlayer->m_byHelp[0];
							pHelpInfoPacket.bHelpInfo[1] = g_pMainPlayer->m_byHelp[1];
							pHelpInfoPacket.bHelpInfo[2] = g_pMainPlayer->m_byHelp[2];
							g_pNet->SendMsg((char*)&pHelpInfoPacket, pHelpInfoPacket.GetPacketSize(), SERVER_INDEX_WORLD);
						}
					}					
					return;
				}
			}						
		}
		m_bSoundChk	= FALSE;
	
		ShowSpriteAll();

		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDCLOSE, g_v3InterfaceSoundPos, FALSE);
	}
	else
	{
		CInterface::GetInstance()->SetWnd(m_nIndex);

		if(m_byType==2)
		{
			SetPosObjX(SPR_OBJ_HELP_NEXT1, 84);
			SetPosObjY(SPR_OBJ_HELP_NEXT1, 216);
			SetPosObjX(SPR_OBJ_HELP_NEXT2, 84);
			SetPosObjY(SPR_OBJ_HELP_NEXT2, 216);
			SetPosObjX(SPR_OBJ_HELP_NEXT3, 84);
			SetPosObjY(SPR_OBJ_HELP_NEXT3, 216);
			SetRender(SPR_OBJ_HELP_NEXT1, TRUE);
		}
		else if(m_byType==3)
		{
			if(m_bySubType==0)
			{
				SetPosObjX(SPR_OBJ_HELP_HP, 20);
				SetPosObjY(SPR_OBJ_HELP_HP, 85);
				SetPosObjX(SPR_OBJ_HELP_SP, 140);
				SetPosObjY(SPR_OBJ_HELP_SP, 85);								
				SetPosObjX(SPR_OBJ_HELP_EXP, 20);
				SetPosObjY(SPR_OBJ_HELP_EXP, 115);
				SetPosObjX(SPR_OBJ_HELP_COOL, 140);
				SetPosObjY(SPR_OBJ_HELP_COOL, 115);
				SetPosObjX(SPR_OBJ_HELP_ITEM, 20);
				SetPosObjY(SPR_OBJ_HELP_ITEM, 135);
				SetPosObjX(SPR_OBJ_HELP_CHAR, 20);
				SetPosObjY(SPR_OBJ_HELP_CHAR, 160);
				SetPosObjX(SPR_OBJ_HELP_SKILL, 20);
				SetPosObjY(SPR_OBJ_HELP_SKILL, 185);
				SetPosObjX(SPR_OBJ_HELP_NEXT1, 84);
				SetPosObjY(SPR_OBJ_HELP_NEXT1, 216);
				SetPosObjX(SPR_OBJ_HELP_NEXT2, 84);
				SetPosObjY(SPR_OBJ_HELP_NEXT2, 216);
				SetPosObjX(SPR_OBJ_HELP_NEXT3, 84);
				SetPosObjY(SPR_OBJ_HELP_NEXT3, 216);
				SetRender(SPR_OBJ_HELP_HP, TRUE);
				SetRender(SPR_OBJ_HELP_SP, TRUE);
				SetRender(SPR_OBJ_HELP_EXP, TRUE);
				SetRender(SPR_OBJ_HELP_COOL, TRUE);
				SetRender(SPR_OBJ_HELP_ITEM, TRUE);
				SetRender(SPR_OBJ_HELP_CHAR, TRUE);
				SetRender(SPR_OBJ_HELP_SKILL, TRUE);				
				SetRender(SPR_OBJ_HELP_NEXT1, TRUE);
			}			
			else if(m_bySubType==1)
			{
				SetPosObjX(SPR_OBJ_HELP_GROUP, 20);
				SetPosObjY(SPR_OBJ_HELP_GROUP, 40);					
				SetPosObjX(SPR_OBJ_HELP_DUNGEON, 20);
				SetPosObjY(SPR_OBJ_HELP_DUNGEON, 65);
				SetPosObjX(SPR_OBJ_HELP_CHAT, 20);
				SetPosObjY(SPR_OBJ_HELP_CHAT, 90);
				SetPosObjX(SPR_OBJ_HELP_SHOP, 20);
				SetPosObjY(SPR_OBJ_HELP_SHOP, 115);
				SetPosObjX(SPR_OBJ_HELP_MATCH, 128);
				SetPosObjY(SPR_OBJ_HELP_MATCH, 115);
				SetPosObjX(SPR_OBJ_HELP_TRADE, 20);
				SetPosObjY(SPR_OBJ_HELP_TRADE, 140);
				SetPosObjX(SPR_OBJ_HELP_REST, 128);
				SetPosObjY(SPR_OBJ_HELP_REST, 140);
				SetPosObjX(SPR_OBJ_HELP_PK, 20);
				SetPosObjY(SPR_OBJ_HELP_PK, 165);
				SetPosObjX(SPR_OBJ_HELP_OPTION, 128);
				SetPosObjY(SPR_OBJ_HELP_OPTION, 165);
				SetPosObjX(SPR_OBJ_HELP_NEXT1, 84);
				SetPosObjY(SPR_OBJ_HELP_NEXT1, 216);
				SetPosObjX(SPR_OBJ_HELP_NEXT2, 84);
				SetPosObjY(SPR_OBJ_HELP_NEXT2, 216);
				SetPosObjX(SPR_OBJ_HELP_NEXT3, 84);
				SetPosObjY(SPR_OBJ_HELP_NEXT3, 216);
				SetRender(SPR_OBJ_HELP_GROUP, TRUE);
				SetRender(SPR_OBJ_HELP_DUNGEON, TRUE);
				SetRender(SPR_OBJ_HELP_CHAT, TRUE);
				SetRender(SPR_OBJ_HELP_SHOP, TRUE);
				SetRender(SPR_OBJ_HELP_MATCH, TRUE);
				SetRender(SPR_OBJ_HELP_TRADE, TRUE);
				SetRender(SPR_OBJ_HELP_REST, TRUE);
				SetRender(SPR_OBJ_HELP_PK, TRUE);
				SetRender(SPR_OBJ_HELP_OPTION, TRUE);				
				SetRender(SPR_OBJ_HELP_NEXT1, TRUE);
			}
		}
		else if(m_byType==6)
		{
#if IS_JAPAN_LOCALIZING()
			SetPosObjX(SPR_OBJ_HELP_ITEM, 170);
#else
			SetPosObjX(SPR_OBJ_HELP_ITEM, 190);
#endif

			SetPosObjY(SPR_OBJ_HELP_ITEM, 37);
			SetRender(SPR_OBJ_HELP_ITEM, TRUE);
		}
		else if(m_byType==7)
		{
#if IS_JAPAN_LOCALIZING()
			SetPosObjX(SPR_OBJ_HELP_SLOT, 67);
#else
			SetPosObjX(SPR_OBJ_HELP_SLOT, 122);
#endif

			SetPosObjY(SPR_OBJ_HELP_SLOT, 37);
			SetPosObjX(SPR_OBJ_HELP_SUPPLISE, 8);
			SetPosObjY(SPR_OBJ_HELP_SUPPLISE, 177);
			SetRender(SPR_OBJ_HELP_SLOT, TRUE);
			SetRender(SPR_OBJ_HELP_SUPPLISE, TRUE);			
		}
		else if(m_byType==8)
		{
			if(g_pMainPlayer->m_byHelp[2]==0)
			{
				SetPosObjX(SPR_OBJ_HELP_NEXT1, 84);
				SetPosObjY(SPR_OBJ_HELP_NEXT1, 216);
				SetPosObjX(SPR_OBJ_HELP_NEXT2, 84);
				SetPosObjY(SPR_OBJ_HELP_NEXT2, 216);
				SetPosObjX(SPR_OBJ_HELP_NEXT3, 84);
				SetPosObjY(SPR_OBJ_HELP_NEXT3, 216);
				SetRender(SPR_OBJ_HELP_NEXT1, TRUE);
			}			
			SetPosObjX(SPR_OBJ_HELP_INTERFACE1, 25);
			SetPosObjY(SPR_OBJ_HELP_INTERFACE1, 150);
			SetRender(SPR_OBJ_HELP_INTERFACE1, TRUE);
		}
		else if(m_byType==9)
		{
			if(m_bySubType==0)
			{
				SetPosObjX(SPR_OBJ_HELP_HP, 20);
				SetPosObjY(SPR_OBJ_HELP_HP, 85);				
				SetPosObjX(SPR_OBJ_HELP_SP, 140);
				SetPosObjY(SPR_OBJ_HELP_SP, 85);								
				SetPosObjX(SPR_OBJ_HELP_EXP, 20);
				SetPosObjY(SPR_OBJ_HELP_EXP, 115);
				SetPosObjX(SPR_OBJ_HELP_COOL, 140);
				SetPosObjY(SPR_OBJ_HELP_COOL, 115);
				SetPosObjX(SPR_OBJ_HELP_ITEM, 20);
				SetPosObjY(SPR_OBJ_HELP_ITEM, 135);
				SetPosObjX(SPR_OBJ_HELP_CHAR, 20);
				SetPosObjY(SPR_OBJ_HELP_CHAR, 160);
				SetPosObjX(SPR_OBJ_HELP_SKILL, 20);
				SetPosObjY(SPR_OBJ_HELP_SKILL, 185);
				SetPosObjX(SPR_OBJ_HELP_NEXT1, 84);
				SetPosObjY(SPR_OBJ_HELP_NEXT1, 216);
				SetPosObjX(SPR_OBJ_HELP_NEXT2, 84);
				SetPosObjY(SPR_OBJ_HELP_NEXT2, 216);
				SetPosObjX(SPR_OBJ_HELP_NEXT3, 84);
				SetPosObjY(SPR_OBJ_HELP_NEXT3, 216);
				SetRender(SPR_OBJ_HELP_HP, TRUE);
				SetRender(SPR_OBJ_HELP_SP, TRUE);
				SetRender(SPR_OBJ_HELP_EXP, TRUE);
				SetRender(SPR_OBJ_HELP_COOL, TRUE);
				SetRender(SPR_OBJ_HELP_ITEM, TRUE);
				SetRender(SPR_OBJ_HELP_CHAR, TRUE);
				SetRender(SPR_OBJ_HELP_SKILL, TRUE);								
				SetRender(SPR_OBJ_HELP_NEXT1, TRUE);
			}			
			else if(m_bySubType==1)
			{
				SetPosObjX(SPR_OBJ_HELP_GROUP, 20);
				SetPosObjY(SPR_OBJ_HELP_GROUP, 40);
				SetPosObjX(SPR_OBJ_HELP_DUNGEON, 20);
				SetPosObjY(SPR_OBJ_HELP_DUNGEON, 65);
				SetPosObjX(SPR_OBJ_HELP_CHAT, 20);
				SetPosObjY(SPR_OBJ_HELP_CHAT, 90);
				SetPosObjX(SPR_OBJ_HELP_SHOP, 20);
				SetPosObjY(SPR_OBJ_HELP_SHOP, 115);
				SetPosObjX(SPR_OBJ_HELP_MATCH, 128);
				SetPosObjY(SPR_OBJ_HELP_MATCH, 115);
				SetPosObjX(SPR_OBJ_HELP_TRADE, 20);
				SetPosObjY(SPR_OBJ_HELP_TRADE, 140);
				SetPosObjX(SPR_OBJ_HELP_REST, 128);
				SetPosObjY(SPR_OBJ_HELP_REST, 140);
				SetPosObjX(SPR_OBJ_HELP_PK, 20);
				SetPosObjY(SPR_OBJ_HELP_PK, 165);
				SetPosObjX(SPR_OBJ_HELP_OPTION, 128);
				SetPosObjY(SPR_OBJ_HELP_OPTION, 165);
				SetRender(SPR_OBJ_HELP_GROUP, TRUE);
				SetRender(SPR_OBJ_HELP_DUNGEON, TRUE);
				SetRender(SPR_OBJ_HELP_CHAT, TRUE);
				SetRender(SPR_OBJ_HELP_SHOP, TRUE);
				SetRender(SPR_OBJ_HELP_MATCH, TRUE);
				SetRender(SPR_OBJ_HELP_TRADE, TRUE);
				SetRender(SPR_OBJ_HELP_REST, TRUE);
				SetRender(SPR_OBJ_HELP_PK, TRUE);
				SetRender(SPR_OBJ_HELP_OPTION, TRUE);				
			}
		}
		else if(m_byType==10)
		{
			SetPosObjX(SPR_OBJ_HELP_CHAR, 8);
			SetPosObjY(SPR_OBJ_HELP_CHAR, 35);

#if IS_JAPAN_LOCALIZING()
			SetPosObjX(SPR_OBJ_HELP_UP1, 195);
#else
			SetPosObjX(SPR_OBJ_HELP_UP1, 210);
#endif			
			SetPosObjY(SPR_OBJ_HELP_UP1, 99);
			SetRender(SPR_OBJ_HELP_CHAR, TRUE);
			SetRender(SPR_OBJ_HELP_UP1, TRUE);
		}
		else if(m_byType==11)
		{
			SetPosObjX(SPR_OBJ_HELP_SKILL, 10);
			SetPosObjY(SPR_OBJ_HELP_SKILL, 35);

#if IS_JAPAN_LOCALIZING()			
			SetPosObjX(SPR_OBJ_HELP_UP2, 202);
			SetPosObjY(SPR_OBJ_HELP_UP2, 128);
#else
			SetPosObjX(SPR_OBJ_HELP_UP2, 58);
			SetPosObjY(SPR_OBJ_HELP_UP2, 142);
#endif
			
			SetRender(SPR_OBJ_HELP_SKILL, TRUE);
			SetRender(SPR_OBJ_HELP_UP2, TRUE);
		}
		else if(m_byType==12)
		{
			SetPosObjX(SPR_OBJ_HELP_NEXT1, 84);
			SetPosObjY(SPR_OBJ_HELP_NEXT1, 216);
			SetPosObjX(SPR_OBJ_HELP_NEXT2, 84);
			SetPosObjY(SPR_OBJ_HELP_NEXT2, 216);
			SetPosObjX(SPR_OBJ_HELP_NEXT3, 84);
			SetPosObjX(SPR_OBJ_HELP_INTERFACE2, 25);
			SetPosObjY(SPR_OBJ_HELP_INTERFACE2, 160);
			SetPosObjY(SPR_OBJ_HELP_NEXT3, 216);
			SetRender(SPR_OBJ_HELP_NEXT1, TRUE);
			SetRender(SPR_OBJ_HELP_INTERFACE2, TRUE);
		}
		else if(m_byType==13)
		{
			SetPosObjX(SPR_OBJ_HELP_INTERFACE3, 128);
			SetPosObjY(SPR_OBJ_HELP_INTERFACE3, 200);
			SetRender(SPR_OBJ_HELP_INTERFACE3, TRUE);			
		}
		SetRender(SPR_OBJ_HELP_WND1, TRUE);
		SetRender(SPR_OBJ_HELP_WND2, TRUE);
		SetRender(SPR_OBJ_HELP_WND3, TRUE);
		SetRender(SPR_OBJ_HELP_CLOSE1, TRUE);				
		
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDOPEN, g_v3InterfaceSoundPos, FALSE);
	}
}

int CHelpWnd::CheckInterface()
{
	m_bMouseIcon = FALSE;

	int nRt = GetChk();

	if(m_bBtnChk[0]==TRUE && nRt!=4)		
	{
		if(	m_byType==2										||
			(m_byType==3 && m_bySubType==0)					||
			(m_byType==3 && m_bySubType==1)					||
			(m_byType==8 && g_pMainPlayer->m_byHelp[2]==0)	||
			(m_byType==9 && m_bySubType==0)					||
			(m_byType==12))
		{
			SetRender(SPR_OBJ_HELP_NEXT1, TRUE);
			SetRender(SPR_OBJ_HELP_NEXT2, FALSE);
			SetRender(SPR_OBJ_HELP_NEXT3, FALSE);
			m_bBtnChk[0] = FALSE;
		}		
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
				SetRender(SPR_OBJ_HELP_CLOSE1, FALSE);
				SetRender(SPR_OBJ_HELP_CLOSE2, TRUE);
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
		{			
			if(m_byType==2 || (m_byType==3 && m_bySubType==0) || (m_byType==3 && m_bySubType==1) || (m_byType==8 && g_pMainPlayer->m_byHelp[2]==0) || (m_byType==9 && m_bySubType==0) || (m_byType==12))
			{
				if(eMouseCheck==MOUSE_UP)
				{
					m_bBtnChk[0] = TRUE;

					SetRender(SPR_OBJ_HELP_NEXT1, FALSE);
					SetRender(SPR_OBJ_HELP_NEXT2, FALSE);
					SetRender(SPR_OBJ_HELP_NEXT3, FALSE);

					if(m_byType==2)
					{
						if(g_pMainPlayer->m_byHelp[2]==0)
						{
							//if(g_pMainPlayer->m_byHelp[0]<=3 && g_pMainPlayer->m_byHelp[1]<=8)
							{
								// 인터페이스 설명을 안 했을 경우 //
								m_byType	= 3;
								m_bySubType	= 0;
								SetClear();
								SetActive();

								// 서버로 메세지 보내기 //
								g_pMainPlayer->m_byHelp[0]	= 3;
								g_pMainPlayer->m_byHelp[2]	= 1;
								
								WSTC_HELP_INFO pHelpInfoPacket;
								pHelpInfoPacket.bHelpInfo[0] = g_pMainPlayer->m_byHelp[0];
								pHelpInfoPacket.bHelpInfo[1] = g_pMainPlayer->m_byHelp[1];
								pHelpInfoPacket.bHelpInfo[2] = g_pMainPlayer->m_byHelp[2];
								g_pNet->SendMsg((char*)&pHelpInfoPacket, pHelpInfoPacket.GetPacketSize(), SERVER_INDEX_WORLD);
							}
						}					
						else
						{
							// 인터페이스 설명을 했을 경우 //
							m_byType	= 4;
							m_bySubType	= 0;

							g_pMainPlayer->m_byHelp[0]	= 4;
							
							WSTC_HELP_INFO pHelpInfoPacket;
							pHelpInfoPacket.bHelpInfo[0] = g_pMainPlayer->m_byHelp[0];
							pHelpInfoPacket.bHelpInfo[1] = g_pMainPlayer->m_byHelp[1];
							pHelpInfoPacket.bHelpInfo[2] = g_pMainPlayer->m_byHelp[2];
							g_pNet->SendMsg((char*)&pHelpInfoPacket, pHelpInfoPacket.GetPacketSize(), SERVER_INDEX_WORLD);

							SetClear();
							SetActive();
						}
					}
					else if(m_byType==3 && m_bySubType==0)
					{					
						m_byType	= 3;
						m_bySubType	= 1;

						g_pMainPlayer->m_byHelp[0]	= 3;
							
						WSTC_HELP_INFO pHelpInfoPacket;
						pHelpInfoPacket.bHelpInfo[0] = g_pMainPlayer->m_byHelp[0];
						pHelpInfoPacket.bHelpInfo[1] = g_pMainPlayer->m_byHelp[1];
						pHelpInfoPacket.bHelpInfo[2] = g_pMainPlayer->m_byHelp[2];
						g_pNet->SendMsg((char*)&pHelpInfoPacket, pHelpInfoPacket.GetPacketSize(), SERVER_INDEX_WORLD);

						SetClear();
						SetActive();
					}
					else if(m_byType==3 && m_bySubType==1)
					{
						// 인터페이스 설명을 했을 경우 // 					
						m_byType	= 4;
						m_bySubType	= 0;

						g_pMainPlayer->m_byHelp[0]	= 4;
							
						WSTC_HELP_INFO pHelpInfoPacket;
						pHelpInfoPacket.bHelpInfo[0] = g_pMainPlayer->m_byHelp[0];
						pHelpInfoPacket.bHelpInfo[1] = g_pMainPlayer->m_byHelp[1];
						pHelpInfoPacket.bHelpInfo[2] = g_pMainPlayer->m_byHelp[2];
						g_pNet->SendMsg((char*)&pHelpInfoPacket, pHelpInfoPacket.GetPacketSize(), SERVER_INDEX_WORLD);

						SetClear();
						SetActive();
					}
					else if(m_byType==8)
					{
						if(g_pMainPlayer->m_byHelp[2]==0)
						{						
							// 인터페이스 설명을 안 했을 경우 //
							m_byType	= 9;
							m_bySubType	= 0;
							SetClear();
							SetActive();

							// 서버로 메세지 보내기 //
							g_pMainPlayer->m_byHelp[1]	= 9;
							g_pMainPlayer->m_byHelp[2]	= 1;
							
							WSTC_HELP_INFO pHelpInfoPacket;
							pHelpInfoPacket.bHelpInfo[0] = g_pMainPlayer->m_byHelp[0];
							pHelpInfoPacket.bHelpInfo[1] = g_pMainPlayer->m_byHelp[1];
							pHelpInfoPacket.bHelpInfo[2] = g_pMainPlayer->m_byHelp[2];
							g_pNet->SendMsg((char*)&pHelpInfoPacket, pHelpInfoPacket.GetPacketSize(), SERVER_INDEX_WORLD);
						}
					}
					else if(m_byType==9 && m_bySubType==0)
					{
						m_byType	= 9;
						m_bySubType	= 1;

						g_pMainPlayer->m_byHelp[1]	= 9;
							
						WSTC_HELP_INFO pHelpInfoPacket;
						pHelpInfoPacket.bHelpInfo[0] = g_pMainPlayer->m_byHelp[0];
						pHelpInfoPacket.bHelpInfo[1] = g_pMainPlayer->m_byHelp[1];
						pHelpInfoPacket.bHelpInfo[2] = g_pMainPlayer->m_byHelp[2];
						g_pNet->SendMsg((char*)&pHelpInfoPacket, pHelpInfoPacket.GetPacketSize(), SERVER_INDEX_WORLD);

						SetClear();
						SetActive();
					}
					else if(m_byType==12)
					{
						m_byType	= 13;					
						g_pMainPlayer->m_byHelp[1]	= 13;
							
						WSTC_HELP_INFO pHelpInfoPacket;
						pHelpInfoPacket.bHelpInfo[0] = g_pMainPlayer->m_byHelp[0];
						pHelpInfoPacket.bHelpInfo[1] = g_pMainPlayer->m_byHelp[1];
						pHelpInfoPacket.bHelpInfo[2] = g_pMainPlayer->m_byHelp[2];
						g_pNet->SendMsg((char*)&pHelpInfoPacket, pHelpInfoPacket.GetPacketSize(), SERVER_INDEX_WORLD);

						SetClear();
						SetActive();
					}
				}
				else if(eMouseCheck==MOUSE_DOWN)
				{
					SetRender(SPR_OBJ_HELP_NEXT1, FALSE);
					SetRender(SPR_OBJ_HELP_NEXT2, TRUE);
					SetRender(SPR_OBJ_HELP_NEXT3, TRUE);
					m_bBtnChk[0]	= TRUE;
				}
				else if(eMouseCheck==MOUSE_OVER)
				{
					SetRender(SPR_OBJ_HELP_NEXT1, FALSE);
					SetRender(SPR_OBJ_HELP_NEXT2, TRUE);
					SetRender(SPR_OBJ_HELP_NEXT3, FALSE);
					m_bBtnChk[0] = TRUE;
				}
			}
			else
			{
				SetMouseCheck(nRt, eMouseCheck);
				return CInterface::GetInstance()->GetSetWnd();
			}
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;

	}
	
	if(MouseUp())
	{
		SetRender(SPR_OBJ_HELP_CLOSE1, TRUE);
		SetRender(SPR_OBJ_HELP_CLOSE2, FALSE);		
	}

	MouseChk(nRt);
	
	if(SetMoveWnd(nRt))		
	{			
		if((m_byType==2) || (m_byType==3 && m_bySubType==0) || (m_byType==3 && m_bySubType==1) || (m_byType==8 && g_pMainPlayer->m_byHelp[2]==0) || (m_byType==9 && m_bySubType==0) || (m_byType==12))
		{
			SetPosObjX(SPR_OBJ_HELP_NEXT1, 84);
			SetPosObjY(SPR_OBJ_HELP_NEXT1, 216);
			SetPosObjX(SPR_OBJ_HELP_NEXT2, 84);
			SetPosObjY(SPR_OBJ_HELP_NEXT2, 216);
			SetPosObjX(SPR_OBJ_HELP_NEXT3, 84);
			SetPosObjY(SPR_OBJ_HELP_NEXT3, 216);
		}
		
		if(m_byType==3)
		{
			if(m_bySubType==0)
			{
				SetPosObjX(SPR_OBJ_HELP_HP, 20);
				SetPosObjY(SPR_OBJ_HELP_HP, 85);				
				SetPosObjX(SPR_OBJ_HELP_SP, 140);
				SetPosObjY(SPR_OBJ_HELP_SP, 85);
				SetPosObjX(SPR_OBJ_HELP_EXP, 20);
				SetPosObjY(SPR_OBJ_HELP_EXP, 114);
				SetPosObjX(SPR_OBJ_HELP_COOL, 140);
				SetPosObjY(SPR_OBJ_HELP_COOL, 114);
				SetPosObjX(SPR_OBJ_HELP_ITEM, 20);
				SetPosObjY(SPR_OBJ_HELP_ITEM, 135);
				SetPosObjX(SPR_OBJ_HELP_CHAR, 20);
				SetPosObjY(SPR_OBJ_HELP_CHAR, 160);
				SetPosObjX(SPR_OBJ_HELP_SKILL, 20);
				SetPosObjY(SPR_OBJ_HELP_SKILL, 185);
			}
			else if(m_bySubType==1)
			{
				SetPosObjX(SPR_OBJ_HELP_GROUP, 20);
				SetPosObjY(SPR_OBJ_HELP_GROUP, 40);					
				SetPosObjX(SPR_OBJ_HELP_DUNGEON, 20);
				SetPosObjY(SPR_OBJ_HELP_DUNGEON, 65);
				SetPosObjX(SPR_OBJ_HELP_CHAT, 20);
				SetPosObjY(SPR_OBJ_HELP_CHAT, 90);
				SetPosObjX(SPR_OBJ_HELP_SHOP, 20);
				SetPosObjY(SPR_OBJ_HELP_SHOP, 115);
				SetPosObjX(SPR_OBJ_HELP_MATCH, 128);
				SetPosObjY(SPR_OBJ_HELP_MATCH, 115);
				SetPosObjX(SPR_OBJ_HELP_TRADE, 20);
				SetPosObjY(SPR_OBJ_HELP_TRADE, 140);
				SetPosObjX(SPR_OBJ_HELP_REST, 128);
				SetPosObjY(SPR_OBJ_HELP_REST, 140);
				SetPosObjX(SPR_OBJ_HELP_PK, 20);
				SetPosObjY(SPR_OBJ_HELP_PK, 165);
				SetPosObjX(SPR_OBJ_HELP_OPTION, 128);
				SetPosObjY(SPR_OBJ_HELP_OPTION, 165);
			}
		}
		else if(m_byType==6)
		{
#if IS_JAPAN_LOCALIZING()
			SetPosObjX(SPR_OBJ_HELP_ITEM, 170);
#else
			SetPosObjX(SPR_OBJ_HELP_ITEM, 190);
#endif
			SetPosObjY(SPR_OBJ_HELP_ITEM, 37);
		}
		else if(m_byType==7)
		{
#if IS_JAPAN_LOCALIZING()							
			SetPosObjX(SPR_OBJ_HELP_SLOT, 67);
#else
			SetPosObjX(SPR_OBJ_HELP_SLOT, 122);
#endif
			SetPosObjY(SPR_OBJ_HELP_SLOT, 37);
			SetPosObjY(SPR_OBJ_HELP_SUPPLISE, 8);
			SetPosObjY(SPR_OBJ_HELP_SUPPLISE, 177);
		}
		else if(m_byType==8)
		{
			SetPosObjX(SPR_OBJ_HELP_INTERFACE1, 25);
			SetPosObjY(SPR_OBJ_HELP_INTERFACE1, 150);
		}
		if(m_byType==9)
		{
			if(m_bySubType==0)
			{
				SetPosObjX(SPR_OBJ_HELP_HP, 20);
				SetPosObjY(SPR_OBJ_HELP_HP, 85);				
				SetPosObjX(SPR_OBJ_HELP_SP, 140);
				SetPosObjY(SPR_OBJ_HELP_SP, 85);
				SetPosObjX(SPR_OBJ_HELP_EXP, 20);
				SetPosObjY(SPR_OBJ_HELP_EXP, 114);
				SetPosObjX(SPR_OBJ_HELP_COOL, 140);
				SetPosObjY(SPR_OBJ_HELP_COOL, 114);
				SetPosObjX(SPR_OBJ_HELP_ITEM, 20);
				SetPosObjY(SPR_OBJ_HELP_ITEM, 135);
				SetPosObjX(SPR_OBJ_HELP_CHAR, 20);
				SetPosObjY(SPR_OBJ_HELP_CHAR, 160);
				SetPosObjX(SPR_OBJ_HELP_SKILL, 20);
				SetPosObjY(SPR_OBJ_HELP_SKILL, 185);
			}
			else if(m_bySubType==1)
			{
				SetPosObjX(SPR_OBJ_HELP_GROUP, 20);
				SetPosObjY(SPR_OBJ_HELP_GROUP, 40);					
				SetPosObjX(SPR_OBJ_HELP_DUNGEON, 20);
				SetPosObjY(SPR_OBJ_HELP_DUNGEON, 65);
				SetPosObjX(SPR_OBJ_HELP_CHAT, 20);
				SetPosObjY(SPR_OBJ_HELP_CHAT, 90);
				SetPosObjX(SPR_OBJ_HELP_SHOP, 20);
				SetPosObjY(SPR_OBJ_HELP_SHOP, 115);
				SetPosObjX(SPR_OBJ_HELP_MATCH, 128);
				SetPosObjY(SPR_OBJ_HELP_MATCH, 115);
				SetPosObjX(SPR_OBJ_HELP_TRADE, 20);
				SetPosObjY(SPR_OBJ_HELP_TRADE, 140);
				SetPosObjX(SPR_OBJ_HELP_REST, 128);
				SetPosObjY(SPR_OBJ_HELP_REST, 140);
				SetPosObjX(SPR_OBJ_HELP_PK, 20);
				SetPosObjY(SPR_OBJ_HELP_PK, 165);
				SetPosObjX(SPR_OBJ_HELP_OPTION, 128);
				SetPosObjY(SPR_OBJ_HELP_OPTION, 165);
			}				
		}
		else if(m_byType==10)
		{
			SetPosObjX(SPR_OBJ_HELP_CHAR, 10);
			SetPosObjY(SPR_OBJ_HELP_CHAR, 35);
#if IS_JAPAN_LOCALIZING()
			SetPosObjX(SPR_OBJ_HELP_UP1, 195);
#else
			SetPosObjX(SPR_OBJ_HELP_UP1, 210);
#endif			
			SetPosObjY(SPR_OBJ_HELP_UP1, 99);
		}
		else if(m_byType==11)
		{
			SetPosObjX(SPR_OBJ_HELP_SKILL, 10);
			SetPosObjY(SPR_OBJ_HELP_SKILL, 35);

#if IS_JAPAN_LOCALIZING()		
			SetPosObjX(SPR_OBJ_HELP_UP2, 202);
			SetPosObjY(SPR_OBJ_HELP_UP2, 128);
#else
			SetPosObjX(SPR_OBJ_HELP_UP2, 58);
			SetPosObjY(SPR_OBJ_HELP_UP2, 142);
#endif				
		}
		else if(m_byType==12)
		{
			SetPosObjX(SPR_OBJ_HELP_INTERFACE2, 25);
			SetPosObjY(SPR_OBJ_HELP_INTERFACE2, 160);
		}
		else if(m_byType==13)
		{
			SetPosObjX(SPR_OBJ_HELP_INTERFACE3, 128);
			SetPosObjY(SPR_OBJ_HELP_INTERFACE3, 200);			
		}
		return CInterface::GetInstance()->GetSetWnd();
	}
	
	if(IsReturn(nRt, eMouseCheck))
		return CInterface::GetInstance()->GetSetWnd();		

	return 0;
}

void CHelpWnd::RenderTextHelp()
{
	switch(m_byType)
	{
	case 0:		// 월드맵 기본 정보 //
		{
			for(int i = 1; i <= 11; i++)
				InterfaceTextRender(i);
			
		}
		break;
	case 2:
		{
			for(int i = 12; i <= 18; i++)
			{
				InterfaceTextRender(i);
				
			}			
		}
		break;
	case 3:
		{
			if(m_byType==3)
			{
				if(m_bySubType==0)
				{
					for(int i = 19; i <= 27; i++)
					{
						InterfaceTextRender(i);
						
					}
				}
				else if(m_bySubType==1)
				{
					for(int i = 28; i <= 36; i++)
					{
						InterfaceTextRender(i);
					}
				}
			}			
		}
		break;
	case 4:
		{
			for(int i = 37; i <= 48; i++)
			{
				InterfaceTextRender(i);
			}
		}
		break;
	case 5:
		{
			for(int i = 49; i <= 54; i++)
			{
				InterfaceTextRender(i);
			}
		}
		break;
	case 6:
		{
			for(int i = 55; i <= 65; i++)
			{
				InterfaceTextRender(i);
			}
		}
		break;
	case 7:
		{
			for(int i = 66; i <= 73; i++)
			{
				InterfaceTextRender(i);
			}			
		}
		break;
	case 8:
		{
			for(int i = 74; i <= 80; i++)
			{
				InterfaceTextRender(i);
			}			
		}
		break;
	case 9:
		{
			if(m_byType==9)
			{
				if(m_bySubType==0)
				{
					for(int i = 19; i <= 27; i++)
					{
						InterfaceTextRender(i);
					}
				}
				else if(m_bySubType==1)
				{
					for(int i = 28; i <= 36; i++)
					{
						InterfaceTextRender(i);
					}
				}
			}	
		}
		break;
	case 10:
		{
			for(int i = 81; i <= 91; i++)
			{
				InterfaceTextRender(i);
			}
		}
		break;
	case 11:
		{
			for(int i = 92; i <= 101; i++)
			{
				InterfaceTextRender(i);
			}
		}
		break;
	case 12:
		{
			for(int i = 102; i <= 109; i++)
			{
				InterfaceTextRender(i);
			}
		}
		break;
	case 13:
		{
			for(int i = 110; i <= 119; i++)
			{
				InterfaceTextRender(i);
			}
		}
		break;
	}
}
//======================================================//
// End.													//
//======================================================//