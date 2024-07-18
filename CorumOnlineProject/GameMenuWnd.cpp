//======================================================//
// Code by Jang.							2002.12.23	//
//======================================================//
#include "GameMenuWnd.h"
#include "SpriteManager.h"
#include "GameControl.h"
#include "../CommonServer/AbyssHash.h"
#include "HashTable.h"
#include "ExitWnd.h"
#include "UserInterface.h"
#include "Interface.h"
#include "message.h"
#include "initgame.h"
#include "Effect.h"
#include "DungeonProcess.h"

CGameMenuWnd* CGameMenuWnd::c_pThis = NULL;

//======================================================//
// Construction/Destrution.								//
//======================================================//
CGameMenuWnd::CGameMenuWnd()
{ 
	m_byEffectIndex		= __EFFECT_OPTION__MAX__;
	m_dwBGMId			= 0;
	m_bBtn				= FALSE;
	m_bBGMRepeat		= FALSE;
	m_bMoveChk			= FALSE;
	m_bTextFlag			= TRUE;
	m_bShadowFlag		= TRUE;
	m_bEffectFlag		= TRUE;
	m_bBGMFlag			= TRUE;
	m_bSystemMsgFlag	= TRUE;
	m_byChatRemainTime	= 1;
	m_bChatKindParty	= TRUE;
	m_bChatKindGuild	= TRUE;
	m_bChatKindWhisper	= TRUE;
	m_bChatKindFriend	= TRUE;
	m_bChatEmoticon		= TRUE;
	m_byKindNotich = 2;
}

CGameMenuWnd::~CGameMenuWnd()
{
}
//======================================================//
// Class Member Function.								//
//======================================================//
BOOL CGameMenuWnd::Init()
{	
	/*
	InsertData(SPR_OBJ_OPTIONWND, SPR_INTERFACE_OPTIONWND, 0, 0, 1.0f, 1.0f, 0);
	InsertData(SPR_OBJ_OPTION_SHADOW, SPR_INTERFACE_OPTION_CHECK, 45, 53, 1.0f, 1.0f, 1);	
	InsertData(SPR_OBJ_OPTION_BGM, SPR_INTERFACE_OPTION_CHECK, 199, 23, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_OPTION_TEXT, SPR_INTERFACE_OPTION_CHECK, 199, 54, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_OPTION_EFFECT, SPR_INTERFACE_OPTION_CHECK, 199, 38, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_OPTION_EFCHK, SPR_INTERFACE_OPTION_CHECK, 45, 68, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_SYSTEM_CLOSE1, SPR_INTERFACE_CLOSE1, 242, 4, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_SYSTEM_CLOSE2, SPR_INTERFACE_CLOSE2, 242, 4, 1.0f, 1.0f, 1);	
	InsertData(BUTTON_OBJ_EXIT1, BUTTON_INTERFACE_OPTION_EXIT1, 189, 110, 1.0f, 1.0f, 1);
	InsertData(BUTTON_OBJ_EXIT2, BUTTON_INTERFACE_OPTION_EXIT2, 189, 110, 1.0f, 1.0f, 1);
	InsertData(BUTTON_OBJ_EXIT3, BUTTON_INTERFACE_OPTION_EXIT3, 189, 110, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_OPTION_BGMBOL, SPR_INTERFACE_OPTION_VOL, 0, 0, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_OPTION_EFFECTBOL, SPR_INTERFACE_OPTION_VOL, 0, 0, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_OPTION_SYSTEMMSG, SPR_INTERFACE_OPTION_CHECK, 89, 83, 1.0f, 1.0f, 1);

	InsertCheckInterface(242, 4, 256, 16, 2, CHECK_CLOSE);
	InsertCheckInterface(0, 0, 242, 20, 3, CHECK_MOVEWND);
	InsertCheckInterface(46, 57, 56, 67, 4, CHECK_BTN);
	InsertCheckInterface(72, 57, 82, 67, 5, CHECK_BTN);
	InsertCheckInterface(46, 72, 56, 82, 6, CHECK_BTN);
	InsertCheckInterface(72, 72, 82, 82, 7, CHECK_BTN);
	InsertCheckInterface(200, 27, 210, 37, 8, CHECK_BTN);
	InsertCheckInterface(226, 27, 236, 37, 9, CHECK_BTN);
	InsertCheckInterface(200, 42, 210, 52, 10, CHECK_BTN);
	InsertCheckInterface(226, 42, 236, 52, 11, CHECK_BTN);
	InsertCheckInterface(103, 72, 113, 82, 12, CHECK_BTN);
	InsertCheckInterface(148, 72, 158, 82, 13, CHECK_BTN);
	InsertCheckInterface(180, 72, 190, 82, 14, CHECK_BTN);	
	InsertCheckInterface(189, 110, 253, 126, 15, CHECK_BTN);
	InsertCheckInterface(200, 57, 210, 67, 16, CHECK_BTN);
	InsertCheckInterface(226, 57, 236, 67, 17, CHECK_BTN);
	InsertCheckInterface(46, 32, 196, 40, 18, CHECK_SCROLL);
	InsertCheckInterface(46, 47, 196, 55, 19, CHECK_SCROLL);
	InsertCheckInterface(90, 88, 98, 96, 20, CHECK_BTN);
	InsertCheckInterface(118, 88, 126, 96, 21, CHECK_BTN);	
	*/
	m_bInit = TRUE;

	return TRUE;
}

void CGameMenuWnd::Remove()
{
	RemoveAllData();
	m_bActive	= FALSE;
	m_bInit		= FALSE;
}

void CGameMenuWnd::SetOrder()
{
}

void CGameMenuWnd::RenderText()
{
}

void CGameMenuWnd::OpenWnd()
{
	// Background //
	if(m_bBGMFlag)
		SetPosObjX(SPR_OBJ_OPTION_BGM, 199);
	else
		SetPosObjX(SPR_OBJ_OPTION_BGM, 225);
	// Effect Sound //
	if(m_bEffectFlag==TRUE)
		SetPosObjX(SPR_OBJ_OPTION_EFFECT, 199);
	else
		SetPosObjX(SPR_OBJ_OPTION_EFFECT, 225);		
	// Shadow //
	if(m_bShadowFlag==TRUE)
		SetPosObjX(SPR_OBJ_OPTION_SHADOW, 45);			
	else
		SetPosObjX(SPR_OBJ_OPTION_SHADOW, 73);	

#if !IS_CHINA_LOCALIZING()		
	// Text //
	if(m_bTextFlag==TRUE)
		SetPosObjX(SPR_OBJ_OPTION_TEXT, 199);
	else
		SetPosObjX(SPR_OBJ_OPTION_TEXT, 225);	
#endif

	// System Message //
	if(m_bSystemMsgFlag==TRUE)
		SetPosObjX(SPR_OBJ_OPTION_SYSTEMMSG, 84);
	else
		SetPosObjX(SPR_OBJ_OPTION_SYSTEMMSG, 128);
	
	switch(m_byEffectIndex)
	{
	case __EFFECT_OPTION__OFF__:
		SetPosObjX(SPR_OBJ_OPTION_EFCHK, 45);
		break;
	case __EFFECT_OPTION__LOW__:
		SetPosObjX(SPR_OBJ_OPTION_EFCHK, 73);
		break;
	case __EFFECT_OPTION__MIDIUM__:
		SetPosObjX(SPR_OBJ_OPTION_EFCHK, 102);
		break;
	case __EFFECT_OPTION__HIGH__:
		SetPosObjX(SPR_OBJ_OPTION_EFCHK, 147);
		break;
	case __EFFECT_OPTION__MAX__:
		SetPosObjX(SPR_OBJ_OPTION_EFCHK, 179);
		break;
	}
	
	SetPosObjY(SPR_OBJ_OPTION_SHADOW, 53);
	SetPosObjY(SPR_OBJ_OPTION_BGM, 23);
#if !IS_CHINA_LOCALIZING()
	SetPosObjY(SPR_OBJ_OPTION_TEXT, 54);
#endif
	SetPosObjY(SPR_OBJ_OPTION_EFFECT, 38);
	SetPosObjY(SPR_OBJ_OPTION_EFCHK, 68);
	
	// BGM Volume //
	SetPosObjX(SPR_OBJ_OPTION_BGMBOL, 46+g_fBGMVolume*142);
	SetPosObjY(SPR_OBJ_OPTION_BGMBOL, 32);
	SetRender(SPR_OBJ_OPTION_BGMBOL, TRUE);
	// Effect Volume //
	SetPosObjX(SPR_OBJ_OPTION_EFFECTBOL, 46+g_fEffectVolume*142);
	SetPosObjY(SPR_OBJ_OPTION_EFFECTBOL, 47);
	SetRender(SPR_OBJ_OPTION_EFFECTBOL, TRUE);
		
	SetRender(SPR_OBJ_OPTIONWND, TRUE);
	SetRender(SPR_OBJ_OPTION_SYSTEMMSG, TRUE);
	SetRender(SPR_OBJ_OPTION_SHADOW, TRUE);
	SetRender(SPR_OBJ_OPTION_BGM, TRUE);
	SetRender(SPR_OBJ_OPTION_EFFECT, TRUE);
#if !IS_CHINA_LOCALIZING()
	SetRender(SPR_OBJ_OPTION_TEXT, TRUE);
#endif
	SetRender(BUTTON_OBJ_EXIT1, TRUE);
	SetRender(SPR_OBJ_OPTION_EFCHK, TRUE);

	// sung-han 2005-04-01 ¿É¼ÇÃ¢¿¡¼­ VÇ¥ À§Ä¡
	if(m_byChatRemainTime==1)
		SetPosObjX(SPR_OBJ_OPTION_TEXTSPEED, 84);
	else if(m_byChatRemainTime==2)
		SetPosObjX(SPR_OBJ_OPTION_TEXTSPEED, 128);
	else
		SetPosObjX(SPR_OBJ_OPTION_TEXTSPEED, 180);
	SetRender(SPR_OBJ_OPTION_TEXTSPEED, TRUE);


	if(g_byChatLineCnt ==1)
		SetPosObjX(SPR_OBJ_OPTION_TEXTLINE, 84);
	else if(g_byChatLineCnt ==2)
		SetPosObjX(SPR_OBJ_OPTION_TEXTLINE, 128);
	else
		SetPosObjX(SPR_OBJ_OPTION_TEXTLINE, 180);
	SetRender(SPR_OBJ_OPTION_TEXTLINE, TRUE);


	if(m_byKindNotich==1)
		SetPosObjX(SPR_OBJ_OPTION_NOTICE, 84);
	else if(m_byKindNotich==2)
		SetPosObjX(SPR_OBJ_OPTION_NOTICE, 128);
	else
		SetPosObjX(SPR_OBJ_OPTION_NOTICE, 180);
	SetRender(SPR_OBJ_OPTION_NOTICE, TRUE);


	if(m_bChatEmoticon==TRUE)
		SetPosObjX(SPR_OBJ_OPTION_EMOTICON, 84);
	else
		SetPosObjX(SPR_OBJ_OPTION_EMOTICON, 128);
	SetRender(SPR_OBJ_OPTION_EMOTICON, TRUE);


	if(m_bChatKindWhisper==TRUE)
		SetPosObjX(SPR_OBJ_OPTION_WHISPER, 84);
	else
		SetPosObjX(SPR_OBJ_OPTION_WHISPER, 128);
	SetRender(SPR_OBJ_OPTION_WHISPER, TRUE);


	if(m_bChatKindGuild==TRUE)
		SetPosObjX(SPR_OBJ_OPTION_GUILD, 84);
	else
		SetPosObjX(SPR_OBJ_OPTION_GUILD, 128);
	SetRender(SPR_OBJ_OPTION_GUILD, TRUE);


	if(m_bChatKindParty==TRUE)
		SetPosObjX(SPR_OBJ_OPTION_PARTY, 84);
	else
		SetPosObjX(SPR_OBJ_OPTION_PARTY, 128);
	SetRender(SPR_OBJ_OPTION_PARTY, TRUE);


	if(m_bChatKindFriend==TRUE)
		SetPosObjX(SPR_OBJ_OPTION_FRIEND, 84);
	else
		SetPosObjX(SPR_OBJ_OPTION_FRIEND, 128);
	SetRender(SPR_OBJ_OPTION_FRIEND, TRUE);
}

void CGameMenuWnd::SetActive(BOOL bActive)
{
	m_bActive	= bActive;

	if(bActive==FALSE)
	{
		m_bSoundChk = FALSE;
		ShowSpriteAll();
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDCLOSE, g_v3InterfaceSoundPos, FALSE);
	}
	else
	{
		CInterface::GetInstance()->SetWnd((BYTE)m_nIndex);
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDOPEN, g_v3InterfaceSoundPos, FALSE);
	}
}

int CGameMenuWnd::CheckInterface()
{
	m_bMouseIcon = FALSE;

	int nRt = GetChk();

	InitValue(nRt);

	if(m_bBtn && nRt!=15)
	{
		SetRender(BUTTON_OBJ_EXIT1, TRUE);
		SetRender(BUTTON_OBJ_EXIT2, FALSE);
		SetRender(BUTTON_OBJ_EXIT3, FALSE);
		m_bBtn	= FALSE;		
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
				SetRender(SPR_OBJ_SYSTEM_CLOSE1, FALSE);
				SetRender(SPR_OBJ_SYSTEM_CLOSE2, TRUE);
			}
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 3:		
		{
			SetMouseCheck(nRt, eMouseCheck);			
		}		
		break;
	case 4:// ±×¸²ÀÚ ÄÔ
		{			
			if(eMouseCheck==MOUSE_DOWN)
			{
				m_bShadowFlag = TRUE;				
				SetPosObjX(SPR_OBJ_OPTION_SHADOW, 45);
				g_pExecutive->GetGeometry()->SetShadowFlag(m_bShadowFlag);

				if(g_pUserHash)
				{
					CUser*				pUser;	
					ListNode<CUser>*	pUserNode = g_pUserHash->GetHead();
		
					while(pUserNode)
					{
						pUser = pUserNode->pData;												
						HideObject(pUser->m_hShadowHandle);
						pUserNode = pUserNode->pNext;
					}
				}

				if(g_pMonsterHash)
				{
					CMonster*			pMonster;	
					ListNode<CMonster>*	pMonsterNode = g_pMonsterHash->GetHead();
		
					while(pMonsterNode)
					{
						pMonster = pMonsterNode->pData;
						HideObject(pMonster->m_hShadowHandle);
						pMonsterNode = pMonsterNode->pNext;
					}
				}
			}
			SetMouseCheck(nRt, eMouseCheck);
				
		}
		break;
	case 5:// ±×¸²ÀÚ ²û
		{
			if(eMouseCheck==MOUSE_DOWN)
			{
				m_bShadowFlag	= FALSE;				
				SetPosObjX(SPR_OBJ_OPTION_SHADOW, 73);
				g_pExecutive->GetGeometry()->SetShadowFlag(m_bShadowFlag);

				if(g_pUserHash)
				{
					CUser*				pUser;	
					ListNode<CUser>*	pUserNode = g_pUserHash->GetHead();
		
					while(pUserNode)
					{
						pUser = pUserNode->pData;		
						ShowObject(pUser->m_hShadowHandle);					
						pUserNode = pUserNode->pNext;
					}
				}

				if(g_pMonsterHash)
				{
					CMonster*			pMonster;	
					ListNode<CMonster>*	pMonsterNode = g_pMonsterHash->GetHead();
		
					while(pMonsterNode)
					{
						pMonster = pMonsterNode->pData;
						ShowObject(pMonster->m_hShadowHandle);
						pMonsterNode = pMonsterNode->pNext;
					}
				}
			}
			SetMouseCheck(nRt, eMouseCheck);
		}		
		break;
	case 6://ÀÌÆåÆ® off
		{			
			if(eMouseCheck==MOUSE_DOWN)
			{
				SetPosObjX(SPR_OBJ_OPTION_EFCHK, 45);
				m_byEffectIndex = __EFFECT_OPTION__OFF__;				
				SetEffect();
			}
			SetMouseCheck(nRt, eMouseCheck);
		}		
		break;		
	case 7:// ÀÌÆåÆ® low
		{
			if(eMouseCheck==MOUSE_DOWN)
			{
				SetPosObjX(SPR_OBJ_OPTION_EFCHK, 73);
				m_byEffectIndex = __EFFECT_OPTION__LOW__;			
				SetEffect();
			}
			SetMouseCheck(nRt, eMouseCheck);			
		}
		break;
	case 8:// ¹è°æÀ½ ÄÔ
		{
			if(eMouseCheck==MOUSE_DOWN)
			{
				SetPosObjX(SPR_OBJ_OPTION_BGM, 198);
				m_bBGMFlag = TRUE;
//				PlayBGM(FILE(m_dwBGMId));
				PlayBGM(m_szLast_BGM_Name);
			}
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 9:// ¹è°æÀ½ ²û
		{
			if(eMouseCheck==MOUSE_DOWN)
			{
				SetPosObjX(SPR_OBJ_OPTION_BGM, 225);
				m_bBGMFlag	= FALSE;						
				StopBGM();
			}
			SetMouseCheck(nRt, eMouseCheck);
		}		
		break;
	case 10:// È¿°úÀ½ ²û
		{			
			if(eMouseCheck==MOUSE_DOWN)
			{
				SetPosObjX(SPR_OBJ_OPTION_EFFECT, 198);
				m_bEffectFlag = TRUE;
			}
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 11:// È¿°úÀ½ ²û
		{			
			if(eMouseCheck==MOUSE_DOWN)
			{
				SetPosObjX(SPR_OBJ_OPTION_EFFECT, 225);
				m_bEffectFlag	= FALSE;			
			}
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 12:// ÀÌÆåÆ® ¹Ìµð¾ö
		{			
			if(eMouseCheck==MOUSE_DOWN)
			{
				SetPosObjX(SPR_OBJ_OPTION_EFCHK, 102);
				m_byEffectIndex = __EFFECT_OPTION__MIDIUM__;			
				SetEffect();
			}
			SetMouseCheck(nRt, eMouseCheck);
		}		
		break;
	case 13:// ÀÌÆåÆ® ÇÏÀÌ
		{			
			if(eMouseCheck==MOUSE_DOWN)
			{
				SetPosObjX(SPR_OBJ_OPTION_EFCHK, 147);
				m_byEffectIndex = __EFFECT_OPTION__HIGH__;			
				SetEffect();
			}
			SetMouseCheck(nRt, eMouseCheck);
		}		
		break;
	case 14:// ÀÌÆåÆ® ¸ß½º
		{			
			if(eMouseCheck==MOUSE_DOWN)
			{
				SetPosObjX(SPR_OBJ_OPTION_EFCHK, 179);
				m_byEffectIndex = __EFFECT_OPTION__MAX__;
				SetEffect();
			}
			SetMouseCheck(nRt, eMouseCheck);
		}		
		break;
	case 15:// °ÔÀÓÁ¾·á ¹öÆ°
		{			
			if(eMouseCheck==MOUSE_UP)
			{
				SetRender(BUTTON_OBJ_EXIT1, FALSE);
				SetRender(BUTTON_OBJ_EXIT2, TRUE);
				SetRender(BUTTON_OBJ_EXIT3, FALSE);
				m_bBtn	= TRUE;				
				
				// Exit Window //
				CExitWnd*		pExitWnd		= CExitWnd::GetInstance();						
				CInterface*		pInterface		= CInterface::GetInstance();	
							
				pExitWnd->SetActive(!pExitWnd->GetActive());			

				// Order //
				if(pExitWnd->GetActive()!=FALSE)
					pInterface->SetWnd(EXIT_WND);				
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{
				SetRender(BUTTON_OBJ_EXIT1, FALSE);
				SetRender(BUTTON_OBJ_EXIT2, FALSE);
				SetRender(BUTTON_OBJ_EXIT3, TRUE);			
				m_bBtn	= TRUE;				
			}
			else if(eMouseCheck==MOUSE_OVER)
			{
				SetRender(BUTTON_OBJ_EXIT1, FALSE);
				SetRender(BUTTON_OBJ_EXIT2, TRUE);
				SetRender(BUTTON_OBJ_EXIT3, FALSE);
				m_bBtn	= TRUE;				
			}
			SetMouseCheck(nRt, eMouseCheck);
		}		
		break;	
	case 16://ÅØ½ºÆ® °¡¼Ó ÄÔ
		{			
			if(eMouseCheck==MOUSE_DOWN)
			{
#if! IS_CHINA_LOCALIZING()
				SetPosObjX(SPR_OBJ_OPTION_TEXT, 199);
				m_bTextFlag		= TRUE;				
				g_dwFontFlag	= SS3D_FONT;
#endif
			}			
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 17:// ÅØ½ºÆ® °¡¼Ó ²û
		{
			if(eMouseCheck==MOUSE_DOWN)
			{
#if !IS_CHINA_LOCALIZING()
				SetPosObjX(SPR_OBJ_OPTION_TEXT, 225);
				m_bTextFlag		= FALSE;				
				g_dwFontFlag	= D3DX_FONT;
#endif
			}	
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 18:// ¹è°æÀ½ À½·®
		{
			if(eMouseCheck==MOUSE_DOWN)
			{
				int nVolume = g_Mouse.MousePos.x-4;

				if(g_Mouse.MousePos.x-4<m_fPosX+46)
					nVolume = int(m_fPosX+46);
				else if(g_Mouse.MousePos.x-4>m_fPosX+188)
					nVolume = int(m_fPosX+188);
			
				SetPosObjX(SPR_OBJ_OPTION_BGMBOL, float(nVolume-m_fPosX));

				// BGM Volume ¼³Á¤ //
				nVolume -= int(m_fPosX+46);
				g_fBGMVolume = (float)nVolume/142.0f;

				if(g_fBGMVolume>1.0f)
					g_fBGMVolume = 1.0f;
				else if(g_fBGMVolume<0.0f)
					g_fBGMVolume = 0.0f;
				
				if(m_bBGMFlag)
				{
					SetBGMVolume(g_fBGMVolume);
				}
				
				m_bScChk[0]		= TRUE;
			}			
			SetMouseCheck(nRt, eMouseCheck, TRUE);
		}
		break;
	case 19:// È¿°úÀ½ À½·®
		{
			if(eMouseCheck==MOUSE_DOWN)
			{
				int nVolume = g_Mouse.MousePos.x-4;

				if(g_Mouse.MousePos.x-4<m_fPosX+46)
					nVolume = int(m_fPosX+46);
				else if(g_Mouse.MousePos.x-4>m_fPosX+188)
					nVolume = int(m_fPosX+188);
			
				SetPosObjX(SPR_OBJ_OPTION_EFFECTBOL, float(nVolume-m_fPosX));

				// EFFECT Volume ¼³Á¤ //
				nVolume -= int(m_fPosX+46);
				g_fEffectVolume = (float)nVolume/142.0f;//ÉãÏñ»úÊÓ½Ç				
				
				m_bScChk[1]		= TRUE;
			}			
			SetMouseCheck(nRt, eMouseCheck, TRUE);
		}
		break;
	case 20:// ½Ã½ºÅÛ ¸Þ¼¼Áö ÄÔ
		{
			if(eMouseCheck==MOUSE_DOWN)
			{
				m_bSystemMsgFlag = TRUE;			
				SetPosObjX(SPR_OBJ_OPTION_SYSTEMMSG, 84);
			}
			SetMouseCheck(nRt, eMouseCheck);
		}		
		break;
	case 21:// ½Ã½ºÅÛ ¸Þ¼¼Áö ²û
		{			
			if(eMouseCheck==MOUSE_DOWN)
			{
				m_bSystemMsgFlag	= FALSE;			
				SetPosObjX(SPR_OBJ_OPTION_SYSTEMMSG, 128);
			}
			SetMouseCheck(nRt, eMouseCheck);
		}				
		break;
	case 22:// ÅØ½ºÆ® ¼Óµµ ºü¸§
		{
			if(eMouseCheck==MOUSE_DOWN)
			{
				m_byChatRemainTime = 1;
				SetPosObjX(SPR_OBJ_OPTION_TEXTSPEED, 84);
			}
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 23:// ÅØ½ºÆ® ¼Óµµ Áß°£
		{
			if(eMouseCheck==MOUSE_DOWN)
			{
				m_byChatRemainTime = 2;
				SetPosObjX(SPR_OBJ_OPTION_TEXTSPEED, 128);
			}
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 24:// ÅØ½ºÆ® ¼Óµµ ´À¸²
		{
			if(eMouseCheck==MOUSE_DOWN)
			{
				m_byChatRemainTime = 3;
				SetPosObjX(SPR_OBJ_OPTION_TEXTSPEED, 180);
			}
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 25:// ÅØ½ºÆ® ¶óÀÎ 5
		{
			if(eMouseCheck==MOUSE_DOWN)
			{
				g_byChatLineCnt = 1;
				SetPosObjX(SPR_OBJ_OPTION_TEXTLINE, 84);
			}
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 26:// ÅØ½ºÆ® ¶óÀÎ 10
		{
			if(eMouseCheck==MOUSE_DOWN)
			{
				g_byChatLineCnt = 2;
				SetPosObjX(SPR_OBJ_OPTION_TEXTLINE, 128);
			}
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 27:// ÅØ½ºÆ® ¶óÀÎ 15
		{
			if(eMouseCheck==MOUSE_DOWN)
			{
				g_byChatLineCnt = 3;
				SetPosObjX(SPR_OBJ_OPTION_TEXTLINE, 180);
			}
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 28:// °øÁö ¸Þ¼¼Áö ³ë¸»
		{
			if(eMouseCheck==MOUSE_DOWN)
			{
				m_byKindNotich = 1;
				SetPosObjX(SPR_OBJ_OPTION_NOTICE, 84);
			}
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 29:// °øÁö ¸Þ¼¼Áö À©µµ¿ì
		{
			if(eMouseCheck==MOUSE_DOWN)
			{
				m_byKindNotich = 2;
				SetPosObjX(SPR_OBJ_OPTION_NOTICE, 128);
			}
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 30:// °øÁö ¸Þ¼¼Áö ¸ðµÎ
		{
			if(eMouseCheck==MOUSE_DOWN)
			{
				m_byKindNotich = 3;
				SetPosObjX(SPR_OBJ_OPTION_NOTICE, 180);
			}
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 31:// ÀÚµ¿ ÀÌ¸ðÆ¼ÄÜ ÄÔ
		{
			if(eMouseCheck==MOUSE_DOWN)
			{
				m_bChatEmoticon = TRUE;
				SetPosObjX(SPR_OBJ_OPTION_EMOTICON, 84);
			}
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 32:// ÀÚµ¿ ÀÌ¸ðÆ¼ÄÜ ²û
		{
			if(eMouseCheck==MOUSE_DOWN)
			{
				m_bChatEmoticon = FALSE;
				SetPosObjX(SPR_OBJ_OPTION_EMOTICON, 128);
			}
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 33:// ±Ó¼Ó¸» ÄÔ
		{
			if(eMouseCheck==MOUSE_DOWN)
			{
				m_bChatKindWhisper = TRUE;
				SetPosObjX(SPR_OBJ_OPTION_WHISPER, 84);
			}
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 34:// ±Ó¼Ó¸» ²û
		{
			if(eMouseCheck==MOUSE_DOWN)
			{
				m_bChatKindWhisper = FALSE;
				SetPosObjX(SPR_OBJ_OPTION_WHISPER, 128);
			}
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 35://±æµå ÄÔ
		{
			if(eMouseCheck==MOUSE_DOWN)
			{
				m_bChatKindGuild = TRUE;
				SetPosObjX(SPR_OBJ_OPTION_GUILD, 84);
			}
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 36:// ±æµå ²û
		{
			if(eMouseCheck==MOUSE_DOWN)
			{
				m_bChatKindGuild = FALSE;
				SetPosObjX(SPR_OBJ_OPTION_GUILD, 128);
			}
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 37:// ÆÄÆ¼ ÄÔ
		{
			if(eMouseCheck==MOUSE_DOWN)
			{
				m_bChatKindParty = TRUE;
				SetPosObjX(SPR_OBJ_OPTION_PARTY, 84);
			}
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 38:// ÆÄÆ¼ ²û
		{
			if(eMouseCheck==MOUSE_DOWN)
			{
				m_bChatKindParty = FALSE;
				SetPosObjX(SPR_OBJ_OPTION_PARTY, 128);
			}
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 39:// Ä£±¸ ÄÔ
		{
			if(eMouseCheck==MOUSE_DOWN)
			{
				m_bChatKindFriend = TRUE;
				SetPosObjX(SPR_OBJ_OPTION_FRIEND, 84);
			}
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 40:// Ä£±¸ ²û
		{
			if(eMouseCheck==MOUSE_DOWN)
			{
				m_bChatKindFriend = FALSE;
				SetPosObjX(SPR_OBJ_OPTION_FRIEND, 128);
			}
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	}		
	
	if(MouseUp())
	{
		SetRender(SPR_OBJ_SYSTEM_CLOSE1, TRUE);
		SetRender(SPR_OBJ_SYSTEM_CLOSE2, FALSE);
	}

	MouseChk(nRt);
	
	if(SetMoveWnd(nRt))		
		return CInterface::GetInstance()->GetSetWnd();

	if(m_bScChk[0] && nRt!=18)
	{
		if(g_Mouse.bLDown)
		{		
			int nVolume = g_Mouse.MousePos.x-4;

			if(g_Mouse.MousePos.x-4<m_fPosX+46)
				nVolume = int(m_fPosX+46);
			else if(g_Mouse.MousePos.x-4>m_fPosX+188)
				nVolume = int(m_fPosX+188);
		
			SetPosObjX(SPR_OBJ_OPTION_BGMBOL, float(nVolume-m_fPosX));

			// BGM Volume ¼³Á¤ //
			nVolume -= int(m_fPosX+46);
			g_fBGMVolume = (float)nVolume/142.0f;

			if(g_fBGMVolume>1.0f)
				g_fBGMVolume = 1.0f;
			else if(g_fBGMVolume<0.0f)
				g_fBGMVolume = 0.0f;
			
			if(m_bBGMFlag)
				SetBGMVolume(g_fBGMVolume);

			CUserInterface::GetInstance()->SetPointer(__MOUSE_POINTER_BUTTONCLICK__);					
		}
		m_bClick		= TRUE;
		m_bDown			= TRUE;
		m_bUpChk		= TRUE;
		m_bScChk[0]		= TRUE;
		return CInterface::GetInstance()->GetSetWnd();
	}

	if(m_bScChk[1] && nRt!=19)
	{
		if(g_Mouse.bLDown)
		{
			int nVolume = g_Mouse.MousePos.x-4;

			if(g_Mouse.MousePos.x-4<m_fPosX+46)
				nVolume = int(m_fPosX+46);
			else if(g_Mouse.MousePos.x-4>m_fPosX+188)
				nVolume = int(m_fPosX+188);
		
			SetPosObjX(SPR_OBJ_OPTION_EFFECTBOL, float(nVolume-m_fPosX));

			// EFFECT Volume ¼³Á¤ //
			nVolume -= int(m_fPosX+46);
			g_fEffectVolume = (float)nVolume/142.0f;			

			CUserInterface::GetInstance()->SetPointer(__MOUSE_POINTER_BUTTONCLICK__);					
		}
		m_bClick		= TRUE;
		m_bDown			= TRUE;
		m_bUpChk		= TRUE;
		m_bScChk[1]		= TRUE;
		return CInterface::GetInstance()->GetSetWnd();
	}

	if(IsReturn(nRt, eMouseCheck))
		return CInterface::GetInstance()->GetSetWnd();		

	return 0;
}

void CGameMenuWnd::SetEffect()
{
	// ÀÌÆåÆ® ¸ðµÎ ²û
	ListNode<EffectDesc> *pNode;
	pNode = g_pEffectHash->GetHead();
	while( pNode )
	{
		GXOBJECT_HANDLE handle = pNode->pData->hEffect.pHandle;
		if (handle && pNode->pData->bOwn != 0xff)
			g_pExecutive->DisableRender(handle);
		pNode = pNode->pNext;							
	}

	// ÁÄÁÄ ¸ðµÎ ²û
	ListNode<CUser> *pUserNode =  g_pUserHash->GetHead();
	while (pUserNode)
	{
		SetEffectIndexAll(pUserNode->pData, 0xffffffff);
		pUserNode = pUserNode->pNext;
	}

	g_wEffectLevel = m_byEffectIndex;

	switch(m_byEffectIndex)
	{
	case __EFFECT_OPTION__MAX__:
		// Å¸ÀÎÀÇ ÀÌÆåÆ® ÄÔ
		{
			ListNode<EffectDesc>*		pNode;
			pNode = g_pEffectHash->GetHead();
			while( pNode )
			{
				GXOBJECT_HANDLE handle = pNode->pData->hEffect.pHandle;
				if (handle && pNode->pData->bOwn == 0)
					g_pExecutive->DisableRender(handle);
				pNode = pNode->pNext;
			}	
		}
	case __EFFECT_OPTION__HIGH__:
		// Å¸ÀÎÀÇ ÁÄÁÄ ÄÔ
		{
			ListNode<CUser> *pUserNode =  g_pUserHash->GetHead();
			while (pUserNode)
			{
				if(pUserNode->pData->m_byUpgrade != 0)
				{
					SetEffectIndexAll(pUserNode->pData, pUserNode->pData->m_byUpgrade-6+1);
				}

				pUserNode = pUserNode->pNext;
			}
		}
	case __EFFECT_OPTION__MIDIUM__:
		// ÀÚ½ÅÀÇ ÀÌÆåÆ® ÄÔ
		{
			ListNode<EffectDesc>*		pNode;
			pNode = g_pEffectHash->GetHead();
			while( pNode )
			{
				GXOBJECT_HANDLE handle = pNode->pData->hEffect.pHandle;
				if (handle && (pNode->pData->bOwn == 1 || pNode->pData->bOwn == 2))
				{
					g_pExecutive->EnableRender(handle);
				}
				pNode = pNode->pNext;
			}	
		}
	case __EFFECT_OPTION__LOW__:
		// ÀÚ½ÅÀÇ ÁÄÁÄ ÄÔ
		{
			if(g_pMainPlayer && g_pMainPlayer->m_byUpgrade != 0)
			{
				SetEffectIndexAll(g_pMainPlayer, g_pMainPlayer->m_byUpgrade-6+1);
			}
		}
	}
}
//======================================================//
// End.													//
//======================================================//
