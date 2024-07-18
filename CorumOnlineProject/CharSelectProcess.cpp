#include "stdafx.h"
#include "CharSelectProcess.h"
#include "InitGame.h"
#include "GameControl.h"
#include "CorumResource.h"
#include "DefResource.h"
#include <crtdbg.h>
#include "ObjectManager.h"
#include "User.h"
#include "BlockWnd.h"
#include "Struct.h"
#include "HashTable.h"
#include "GMTool.h"
#include "Resource.h"
#include "Filter.h"
#include "GameDefaultWnd.h"
#include "message.h"
#include "GlobalSoundHandles.h"
#include "InputManager.h"
#include "IMEEdit.h"
#include "WorldWnd.h"
#include "NetworkClient.h"
#include "LoginProcess.h"
#include "CharMoveEditWnd.h"
#include "CodeFun.h"
#include "WorldProcess.h"
#include "CharSelectMsg.h"
#include "QuantityWnd.h"
#include "PacketEncrypt.h"
#include "Interface.h"
#include "GroupWnd.h"


BOOL g_bCharacterChk = FALSE;


//==================================================================//
// 캐릭터 셀렉트 상태에서 초기화 함수.								//
//==================================================================//
void InitPacketProcCharSelect()
{	
	PacketProc[ UPDATE_GAME_CHAR_SELECT ][ Protocol_CharSelect::CMD_CREATE_CHARACTER_FAIL ]		= CmdCreateCharacterFail;
	PacketProc[ UPDATE_GAME_CHAR_SELECT ][ Protocol_CharSelect::CMD_CREATE_CHARACTER_SUCCESS ]	= CmdCreateCharacterSuccess;
	PacketProc[ UPDATE_GAME_CHAR_SELECT ][ Protocol_CharSelect::CMD_CHARACTER_SELECT_FAIL ]		= CmdCharacterSelectFail;	
	PacketProc[ UPDATE_GAME_CHAR_SELECT ][ Protocol_CharSelect::CMD_WORLD_USER_INFO ]			= CmdWorldUserInfo;	
	PacketProc[ UPDATE_GAME_CHAR_SELECT ][ Protocol_CharSelect::CMD_WORLD_LOGIN_FAIL ]			= CmdWorldLoginFail;	
	PacketProc[ UPDATE_GAME_CHAR_SELECT ][ Protocol_CharSelect::ACMD_CONNECT_WORLD_SERVER ]		= AcmdConnectWorldServer;
	PacketProc[ UPDATE_GAME_CHAR_SELECT ][ Protocol_CharSelect::CMD_DELETE_CHARACTER_RESULT ]	= CmdDeleteCharacterResult;
	PacketProc[ UPDATE_GAME_CHAR_SELECT ][ Protocol_CharSelect::ACMD_ITEMMALL_INFO ]			= AcmdItemmallInfo;
	PacketProc[ UPDATE_GAME_CHAR_SELECT ][ Protocol_CharSelect::CMD_WARNING_TITLE ]				= AcmdWarningTitle;
	PacketProc[ UPDATE_GAME_CHAR_SELECT ][ Protocol_CharSelect::CMD_CHR_MOVE_CHK]				= CmdChrMoveChk;	
	PacketProc[ UPDATE_GAME_CHAR_SELECT ][ Protocol_CharSelect::CMD_MOVE_CHARACTER_SUCCESS]		= CmdMoveCharacterSuccess;	
	PacketProc[ UPDATE_GAME_CHAR_SELECT ][ Protocol_CharSelect::CMD_MOVE_CHARACTER_FAIL]		= CmdMoveCharacterFail;
}

BOOL InitGameCharSelect()
{
	g_pExecutive->DeleteAllGXLights();
	g_byChkMessenger	= 0;
	g_byStatusMessenger	= 0;
	g_dwStatusTime		= timeGetTime();
	
	g_pGVLogin->pCharacterSelectBackGround = g_pObjManager->CreateObject(CHR_CHARACTER_SELECT, -100.0f, -300.0f, 500.0f, ACTION_LOOP, true, 0);
	
	VECTOR3 v3Scale = { 4.9f, v3Scale.x, 1};
	g_pExecutive->GXOSetScale(g_pGVLogin->pCharacterSelectBackGround->pHandle, &v3Scale);
	
	g_bCharMoveChk		= FALSE;
	
#ifdef _USE_IME

	GET_IMEEDIT()->EnableSpaceKey(FALSE);

#endif

	char szInfo[0xff] = {0,};
	
//	g_pExecutive->SetFramePerSec(40);
	g_pGeometry->DisableFog(0);
	

	g_pInputManager->SetFocusInput(0);
	g_nChatModeExt = 1;
	g_pSprManager->CreateBackground(SPR_CHAR_SELECT_BACKGROUND, 0.0f, 0.0f);
	
	
	V2_SPRITE* pTemp = g_pSprManager->CreateSprite(SPR_CHAR_SELECT_INFO_WINDOW, 1, 159);
	
	g_pSprManager->CreateSprite(SPR_CHAR_SELECT_CREATE_WINDOW, 1, float(159 + pTemp->dwHeight + 2) , FALSE);

	g_pSprManager->CreateSprite(BUTTON_CREATE, 53, 397, 192, 0, 64, 16, FALSE, 1);
	g_pSprManager->CreateSprite(BUTTON_CREATE_OVER,	53, 397, 192, 16, 64, 16, FALSE, 1);
	g_pSprManager->CreateSprite(BUTTON_CREATE_DOWN, 53, 397, 192, 32, 64, 16, FALSE, 1);
	g_pSprManager->CreateSprite(BUTTON_DELETE, 121, 397,  0, 48, 64, 16,TRUE, 1);
	g_pSprManager->CreateSprite(BUTTON_DELETE_OVER,	121, 397,  0, 64, 64, 16,FALSE, 1);
	g_pSprManager->CreateSprite(BUTTON_DELETE_DOWN, 121, 397,  0, 80, 64, 16,FALSE, 1);
	g_pSprManager->CreateSprite(BUTTON_EXIT, 188, 397, 128, 0, 64, 16, TRUE, 1);
	g_pSprManager->CreateSprite(BUTTON_EXIT_OVER, 188, 397, 128, 16, 64, 16, FALSE, 1);
	g_pSprManager->CreateSprite(BUTTON_EXIT_DOWN, 188, 397, 128, 32, 64, 16, FALSE, 1);
	g_pSprManager->CreateSprite(BUTTON_MODIFY, 82, 483, 64, 48, 64, 16, FALSE, 1);
	g_pSprManager->CreateSprite(BUTTON_MODIFY_OVER,	82, 483, 64, 64, 64, 16, FALSE, 1);
	g_pSprManager->CreateSprite(BUTTON_MODIFY_DOWN , 82, 483, 64, 80, 64, 16, FALSE, 1);
	g_pSprManager->CreateSprite(BUTTON_CLASS_WARRIOR, 26, 524, FALSE, 1);
	g_pSprManager->CreateSprite(BUTTON_CLASS_WARRIOR_DOWN, 26, 524, FALSE, 1);
	g_pSprManager->CreateSprite(BUTTON_CLASS_PRIEST, 68, 524, FALSE, 1);
	g_pSprManager->CreateSprite(BUTTON_CLASS_PRIEST_DOWN, 68, 524, FALSE, 1);
	g_pSprManager->CreateSprite(BUTTON_CLASS_SUMMONER, 113, 524, FALSE, 1);
	g_pSprManager->CreateSprite(BUTTON_CLASS_SUMMONER_DOWN, 113, 524, FALSE, 1);
	g_pSprManager->CreateSprite(BUTTON_CLASS_HUNTER, 155, 524, FALSE, 1);
	g_pSprManager->CreateSprite(BUTTON_CLASS_HUNTER_DOWN, 155, 524, FALSE, 1);
	g_pSprManager->CreateSprite(BUTTON_CLASS_WIZARD, 199, 524, FALSE, 1);
	g_pSprManager->CreateSprite(BUTTON_CLASS_WIZARD_DOWN, 199, 524, FALSE, 1);
	g_pSprManager->CreateSprite(BUTTON_LEFT, 553, 640, TRUE, 2);
	g_pSprManager->CreateSprite(BUTTON_LEFT_OVER, 553, 640, FALSE, 2);
	g_pSprManager->CreateSprite(BUTTON_LEFT_DOWN, 553, 640, FALSE, 2);
	g_pSprManager->CreateSprite(BUTTON_START, 585, 640, TRUE, 1);
	g_pSprManager->CreateSprite(BUTTON_START_OVER, 585, 640, FALSE, 2);
	g_pSprManager->CreateSprite(BUTTON_START_DOWN, 585, 640, FALSE, 2);
	g_pSprManager->CreateSprite(BUTTON_RIGHT, 713, 640, TRUE, 2);
	g_pSprManager->CreateSprite(BUTTON_RIGHT_OVER, 713, 640, FALSE, 2);
	g_pSprManager->CreateSprite(BUTTON_RIGHT_DOWN, 713, 640, FALSE, 2);
	g_pSprManager->CreateSprite(SPR_LOADING_BACK, 0, 0, FALSE, __ORDER_LOAD_INTERFACE__);
	//sung-han 2005-03-25 맵로딩시 게이지의 위치 설정부분(로딩 이미지 변경)
	//g_pSprManager->CreateSprite(SPR_LOADING_BLOCK_HEAD, 394, 515, FALSE, __ORDER_LOAD_INTERFACE__+2);
	g_pSprManager->CreateSprite(SPR_LOADING_BLOCK_HEAD, 394, 598, FALSE, __ORDER_LOAD_INTERFACE__+2);

	SPR(BUTTON_OK)->ShowSprite(FALSE);
	SPR(BUTTON_OK_OVER)->ShowSprite(FALSE);
	SPR(BUTTON_OK_DOWN)->ShowSprite(FALSE);
	SPR(BUTTON_CANCEL)->ShowSprite(FALSE);
	SPR(BUTTON_CANCEL_OVER)->ShowSprite(FALSE);
	SPR(BUTTON_CANCEL_DOWN)->ShowSprite(FALSE);

	//Loading Block Create...
	for(int j=0; j<20; j++)
	{		
		//sung-han 2005-03-25 맵로딩시 게이지의 위치 설정부분(로딩 이미지 변경)
		//g_pSprManager->CreateSprite(SPR_LOADING_BLOCK_1+j, float(422 + 9*j), 539, FALSE, __ORDER_LOAD_INTERFACE__+1 );
		g_pSprManager->CreateSprite(SPR_LOADING_BLOCK_1+j, float(422 + 9*j), 622, FALSE, __ORDER_LOAD_INTERFACE__+1 );
	}
	g_pObjManager->CreateObject(MOD_CHAR_SELECT_TORCHLIGHT, 0.0f, 0.0f, 0.0f, ACTION_LOOP, TRUE, 0);
	g_pSprManager->CreateSprite(SPR_SELECTCHAR_VILL1_1, 13, 583, 0, 96, 64, 16, FALSE, 1);
	g_pSprManager->CreateSprite(SPR_SELECTCHAR_VILL1_2, 13, 583, 0, 112, 64, 16, FALSE, 1);
	g_pSprManager->CreateSprite(SPR_SELECTCHAR_VILL1_3, 13, 583, 0, 128, 64, 16, FALSE, 1);
	g_pSprManager->CreateSprite(SPR_SELECTCHAR_VILL2_1, 80, 583, 64, 96, 64, 16, FALSE, 1);
	g_pSprManager->CreateSprite(SPR_SELECTCHAR_VILL2_2, 80, 583, 64, 112, 64, 16, FALSE, 1);
	g_pSprManager->CreateSprite(SPR_SELECTCHAR_VILL2_3, 80, 583, 64, 128, 64, 16, FALSE, 1);
	
	SetMousePointerType(__MOUSE_POINTER_DEFAULT__);

	g_pInputManager->ClearInput(0);
		
	RECT rt;
	rt.left = 83;	rt.right = 235;	rt.top = 449;	rt.bottom = 463;
	g_pInputManager->InitializeInput(INPUT_BUFFER_CHARACTER_NAME1, FONT_D3D_GAME, rt, 0xffffffff, 3);
	rt.left = 83;	rt.right = 235;	rt.top = 190;	rt.bottom = 204;
	g_pInputManager->InitializeInput(INPUT_BUFFER_CHARACTER_NAME2, FONT_D3D_GAME, rt, 0xffffffff, 3);
	rt.left = 83;	rt.right = 235;	rt.top = 209;	rt.bottom = 223;
	g_pInputManager->InitializeInput(INPUT_BUFFER_CHARACTER_RACE1, FONT_D3D_GAME, rt, 0xffffffff, 3);
	rt.left = 83;	rt.right = 236;	rt.top = 229;	rt.bottom = 243;
	g_pInputManager->InitializeInput(INPUT_BUFFER_CHARACTER_CLASS1, FONT_D3D_GAME, rt, 0xffffffff, 3);
	rt.left = 83;	rt.right = 235;	rt.top = 247;	rt.bottom = 261;
	g_pInputManager->InitializeInput(INPUT_BUFFER_CHARACTER_EXP, FONT_D3D_GAME, rt, 0xffffffff, 3);
	rt.left = 83;	rt.right = 131;	rt.top = 267;	rt.bottom = 281;
	g_pInputManager->InitializeInput(INPUT_BUFFER_CHARACTER_LEVEL, FONT_D3D_GAME, rt, 0xffffffff, 3);
	rt.left = 83;	rt.right = 131;	rt.top = 286;	rt.bottom = 300;
	g_pInputManager->InitializeInput(INPUT_BUFFER_CHARACTER_STR, FONT_D3D_GAME, rt, 0xffffffff, 3);
	rt.left = 83;	rt.right = 131;	rt.top = 306;	rt.bottom = 318;
	g_pInputManager->InitializeInput(INPUT_BUFFER_CHARACTER_VIT, FONT_D3D_GAME, rt, 0xffffffff, 3);
	rt.left = 83;	rt.right = 131;	rt.top = 325;	rt.bottom = 337;
	g_pInputManager->InitializeInput(INPUT_BUFFER_CHARACTER_DEX, FONT_D3D_GAME, rt, 0xffffffff, 3);
	rt.left = 83;	rt.right = 131;	rt.top = 344;	rt.bottom = 356;
	g_pInputManager->InitializeInput(INPUT_BUFFER_CHARACTER_INT, FONT_D3D_GAME, rt, 0xffffffff, 3);
	rt.left = 83;	rt.right = 131;	rt.top = 362;	rt.bottom = 374;
	g_pInputManager->InitializeInput(INPUT_BUFFER_CHARACTER_EGO, FONT_D3D_GAME, rt, 0xffffffff, 3);
	rt.left = 246;	rt.right = 235;	rt.top = 286;	rt.bottom = 298;
	g_pInputManager->InitializeInput(INPUT_BUFFER_12, FONT_D3D_GAME, rt, 0xffffffff, 3);
	rt.left = 246;	rt.right = 236;	rt.top = 305;	rt.bottom = 317;
	g_pInputManager->InitializeInput(INPUT_BUFFER_13, FONT_D3D_GAME, rt, 0xffffffff, 3);
	rt.left = 83;	rt.right = 235;	rt.top = 468;	rt.bottom = 480;
	g_pInputManager->InitializeInput(INPUT_BUFFER_CHARACTER_RACE2, FONT_D3D_GAME, rt, 0xffffffff, 3);
	rt.left = 83;	rt.right = 300;	rt.top = 507;	rt.bottom = 519;
	g_pInputManager->InitializeInput(INPUT_BUFFER_CHARACTER_CLASS2, FONT_D3D_GAME, rt, 0xffffffff, 3);
	
	if(IsEmptyString(g_pGVLogin->ChrSelectInfo[0].ReceivedChrInfo.szName))
	{
		SPR(BUTTON_DELETE)->ShowSprite(FALSE);
		SPR(BUTTON_CREATE)->ShowSprite(TRUE);
	}
	else
	{
		SPR(BUTTON_DELETE)->ShowSprite(TRUE);
		SPR(BUTTON_CREATE)->ShowSprite(FALSE);
	}
	
	VECTOR3 v3 = {0.0f,};
	
	//캐릭터 밑의 발판 기둥 부분 생성  
	for(int i=0; i<MAX_CHAR_SELECT_NUM; i++)
	{		
		g_pGVLogin->ChrSelectInfo[i].pDescGround = AllocObjDesc();
		memset(g_pGVLogin->ChrSelectInfo[i].pDescGround, 0, sizeof(OBJECT_DESC));

		g_pGVLogin->ChrSelectInfo[i].pDescSpotLight = AllocObjDesc();
		memset(g_pGVLogin->ChrSelectInfo[i].pDescSpotLight, 0, sizeof(OBJECT_DESC));

		g_pGVLogin->ChrSelectInfo[i].pDescGround->dwTempBuf[1] = i;		//Rotate할때 자신의 현재 위치 어디인가 기억할 변수 	
	
		g_pGVLogin->ChrSelectInfo[i].hGroundHandle = CreateHandleObject(FILE(MOD_CHAR_SELECT_GROUND), GXPlayerPROC, g_pGVLogin->ChrSelectInfo[i].pDescGround, NULL);
		//Spot Light 생성 
		g_pGVLogin->ChrSelectInfo[i].hSpotLightHandle = CreateHandleObject(FILE(MOD_CHAR_SELECT_LIGHT), GXPlayerPROC, g_pGVLogin->ChrSelectInfo[i].pDescSpotLight, NULL);
				
		switch(i)
		{
		case 0:		v3.x = 0;	v3.y = 0;	v3.z = 0;		break;
		case 1:		v3.x = 200;	v3.y = 0;	v3.z = 200;		break;
		case 2:		v3.x = 0;	v3.y = 0;	v3.z = 400;		break;
		case 3:		v3.x = -200;v3.y = 0;	v3.z = 200;		break;
		}
		GXSetPosition(g_pGVLogin->ChrSelectInfo[i].hGroundHandle, &v3, FALSE);
		GXSetPosition(g_pGVLogin->ChrSelectInfo[i].hSpotLightHandle, &v3, FALSE);
		HideObject(g_pGVLogin->ChrSelectInfo[i].hSpotLightHandle);
	}
	
	for(i = 0; i < MAX_CHAR_SELECT_NUM; i++)
	{	
		if(IsEmptyString(g_pGVLogin->ChrSelectInfo[i].ReceivedChrInfo.szName))
			continue;

		g_pGVLogin->ChrSelectInfo[i].pDesc	= AllocObjDesc();
		memset(g_pGVLogin->ChrSelectInfo[i].pDesc, 0, sizeof(OBJECT_DESC));		
		g_pGVLogin->ChrSelectInfo[i].pDesc->bType = OBJECT_TYPE_SELCHAR;				
		
		WORD wItemId[4] = {	g_pGVLogin->ChrSelectInfo[i].ReceivedChrInfo.wMailID,
							g_pGVLogin->ChrSelectInfo[i].ReceivedChrInfo.wRHandID,
							g_pGVLogin->ChrSelectInfo[i].ReceivedChrInfo.wLHandID,
							g_pGVLogin->ChrSelectInfo[i].ReceivedChrInfo.wHelmetID};

		if(wItemId[0]==0)	//갑옷이 없는경우 기본 몸뚱이 
		{				
			g_pGVLogin->ChrSelectInfo[i].hHandle = CreateHandleObject(GetFile(RESTYPE_BASE_BODY, g_pGVLogin->ChrSelectInfo[i].ReceivedChrInfo.wClass), GXPlayerPROC, g_pGVLogin->ChrSelectInfo[i].pDesc, 0);
		}
		else
		{
			ItemDataName(szInfo, wItemId[0], g_pGVLogin->ChrSelectInfo[i].ReceivedChrInfo.wClass-1);

			//갑옷이 있는 경우 //
			LP_ITEM_RESOURCE_EX lpItemResource = g_pItemResourceHash->GetData(wItemId[0]);

			if(lpItemResource)
			{
				if(lpItemResource->wModCount>0)
					g_pGVLogin->ChrSelectInfo[i].hHandle = CreateHandleObject(GetFile(szInfo, DATA_TYPE_CHARACTER), GXPlayerPROC, g_pGVLogin->ChrSelectInfo[i].pDesc, 0);
			}
		}		
	
		if(wItemId[1])	// R Hand
		{			
			LP_ITEM_RESOURCE_EX lpItemResource = g_pItemResourceHash->GetData(wItemId[1]);
			
			if(lpItemResource)
			{
				if(lpItemResource->wModCount>0)
				{	
//					LP_ITEM_TABLE pItemTable = g_pItemTableHash->GetData(wItemId[1]);
					CBaseItem* pItemTable = g_pItemTableHash->GetData(wItemId[1]);
				
					if(pItemTable)
					{
						if(pItemTable->BaseItem_Weapon.bModCount==2)
						{
							ItemAttach(wItemId[1]
								, &g_pGVLogin->ChrSelectInfo[i].hLeftHand
								, g_pGVLogin->ChrSelectInfo[i].pLItemDesc
								, &g_pGVLogin->ChrSelectInfo[i].hHandle
								, ITEM_ATTACH_TYPE_LHAND);
						}

						ItemAttach(wItemId[1]
							, &g_pGVLogin->ChrSelectInfo[i].hRightHand
							, g_pGVLogin->ChrSelectInfo[i].pRItemDesc
							, &g_pGVLogin->ChrSelectInfo[i].hHandle
							, ITEM_ATTACH_TYPE_RHAND);
					}
				}						
			}
		}		

		if(wItemId[2]) // L Hand
		{
			LP_ITEM_RESOURCE_EX lpItemResource = g_pItemResourceHash->GetData(wItemId[2]);
			
			if(lpItemResource)
			{			
				if(lpItemResource->wModCount>0)
					ItemAttach(wItemId[2], &g_pGVLogin->ChrSelectInfo[i].hLeftHand, g_pGVLogin->ChrSelectInfo[i].pLItemDesc, &g_pGVLogin->ChrSelectInfo[i].hHandle, ITEM_ATTACH_TYPE_LHAND);
			}
		}
		
		if(g_pGVLogin->ChrSelectInfo[i].ReceivedChrInfo.wClass < 4)
			g_pGVLogin->ChrSelectInfo[i].hHeadHandle = CreateHandleObject(GetFile(RESTYPE_HEAD_MALE, g_pGVLogin->ChrSelectInfo[i].ReceivedChrInfo.wHead), NULL, g_pGVLogin->ChrSelectInfo[i].pDesc, 0);
		else
			g_pGVLogin->ChrSelectInfo[i].hHeadHandle = CreateHandleObject(GetFile(RESTYPE_HEAD_FEMALE, g_pGVLogin->ChrSelectInfo[i].ReceivedChrInfo.wHead), NULL, g_pGVLogin->ChrSelectInfo[i].pDesc, 0);

		g_pExecutive->SetData(g_pGVLogin->ChrSelectInfo[i].hHeadHandle, g_pGVLogin->ChrSelectInfo[i].pDesc);
		g_pExecutive->GXOAttach(g_pGVLogin->ChrSelectInfo[i].hHeadHandle, g_pGVLogin->ChrSelectInfo[i].hHandle, "Bip01 Head");

		if(wItemId[3])	// Helmet
		{
			LP_ITEM_RESOURCE_EX lpItemResource = g_pItemResourceHash->GetData(wItemId[3]);

			if(lpItemResource)
			{
				if(lpItemResource->wModCount>0)
					ItemAttach(wItemId[3], &g_pGVLogin->ChrSelectInfo[i].hHelmetHandle, NULL, &g_pGVLogin->ChrSelectInfo[i].hHandle, ITEM_ATTACH_TYPE_HEAD);
			}
		}		
		
		g_pGVLogin->ChrSelectInfo[i].pDesc->dwTempBuf[0]	= i;												//캐릭터 선택 정보 구조체의 배열 인덱스 
		g_pGVLogin->ChrSelectInfo[i].pDesc->dwTempBuf[1]	= i;												//Rotate할때 자신의 현재 위치 어디인가 기억할 변수 	
		g_pGVLogin->ChrSelectInfo[i].pDesc->pInfo			= &g_pGVLogin->ChrSelectInfo[i].ReceivedChrInfo;	//선택할 캐릭터 정보 

		// Action //
		if(g_pGVLogin->ChrSelectInfo[i].ReceivedChrInfo.wRHandID==0)
			::SetAction(g_pGVLogin->ChrSelectInfo[i].hHandle, 1, 0, ACTION_LOOP);									
		else
		{
			int nIndex = g_pGVLogin->ChrSelectInfo[i].ReceivedChrInfo.wRHandID/ITEM_DISTRIBUTE;			
			::SetAction(g_pGVLogin->ChrSelectInfo[i].hHandle, WORD(nIndex*50+MOTION_TYPE_VILLAGESTAND), 0, ACTION_LOOP);
		}		
		
		switch(i)
		{
		case 0:		v3.x = 0;	v3.y = 0;	v3.z = 0;		break;
		case 1:		v3.x = 200;	v3.y = 0;	v3.z = 200;		break;
		case 2:		v3.x = 0;	v3.y = 0;	v3.z = 400;		break;
		case 3:		v3.x = -200;v3.y = 0;	v3.z = 200;		break;
		}

		GXSetPosition(g_pGVLogin->ChrSelectInfo[i].hHandle, &v3, FALSE);
		
		//선택된 캐릭터 (맨 앞자리의 캐릭터이면)
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_CHARSELSPOT, g_v3InterfaceSoundPos, FALSE); //로그인시 꽝하는 사운드 이펙트 안나오게 안한다. 김영대 2003.02.22
		
		if(i == 0)
		{
			DisplayCharacterInfo(&g_pGVLogin->ChrSelectInfo[i].ReceivedChrInfo);
			g_pGVLogin->dwCurCharIndex = i;

			ShowObject(g_pGVLogin->ChrSelectInfo[ i ].hSpotLightHandle);	//Spot Light On

			::SetAction( g_pGVLogin->ChrSelectInfo[ i ].hHandle, WORD((g_pGVLogin->ChrSelectInfo[ i ].bByItemType) * 50 + MOTION_TYPE_ATTACK1_1), 0, ACTION_LOOP );

			g_pGVLogin->ChrSelectInfo[ i ].pDesc->ObjectFunc = CharSelectHighlightMotion;
			g_pGVLogin->ChrSelectInfo[ i ].pDesc->dwTempBuf[10] = 1;
		}
	} 

	//캐릭터 생성할때 보여줄 모든 머리 모델 Load
	for(i = 0; i < MAX_KIND_OF_HEAD_MALE; i++)
	{
		g_pGVLogin->EnumCharHeadForCreate[1][i] = CreateHandleObject(GetFile(RESTYPE_HEAD_MALE, 1001 + i), NULL, NULL, NULL);
		HideObject(g_pGVLogin->EnumCharHeadForCreate[1][i]);
	}
	
	for(i = 0; i < MAX_KIND_OF_HEAD_FEMALE; i++)
	{
		g_pGVLogin->EnumCharHeadForCreate[0][i] = CreateHandleObject(GetFile(RESTYPE_HEAD_FEMALE, 1001 + i), NULL, NULL, NULL);
		HideObject(g_pGVLogin->EnumCharHeadForCreate[0][i]);
	}
	
	//캐릭터 생성할때 보여줄 모든 몸통 모델 Load
	LPObjectDesc pDesc;

	for(i = 0; i < MAX_KIND_OF_CLASS; i++)
	{
		pDesc = AllocObjDesc();		
		g_pGVLogin->EnumCharForCreate[i] = CreateHandleObject(GetFile(RESTYPE_BASE_BODY, i+1), GXPlayerPROC, pDesc, NULL);
		HideObject(g_pGVLogin->EnumCharForCreate[i]);
	}
	
	VECTOR3	From = {-100.0f, 350.0f, -900.0f};
	g_pGeometry->ResetCamera(&From, 100.0f, 80000.0f, DEG45, 0 );

	double dVal = atan2(From.y - 20.0f, 1000.0f);
	
	VECTOR3	Rotate = {float(-1.0f * dVal), 0.0f, 0.0f};
	g_pExecutive->GetGeometry()->RotateCameraRad(&Rotate, 0);	

	CInterface*	pInterface	= CInterface::GetInstance();
	pInterface->InitInterface(__SERVER_TYPE_CHARACTERSELECT__);	

	return TRUE;	
}


//==================================================================//
// 캐릭터 셀렉트 상태에서 삭제함수.									//
//==================================================================//
void ReleaseGameCharSelect()
{	
	g_pSprManager->RemoveBackground();

	g_pInputManager->Initialize();	
	
	RELEASE_SPR(SPR_LOGIN_LOGIN_WINDOW);
	RELEASE_SPR(SPR_CHAR_SELECT_INFO_WINDOW);
	RELEASE_SPR(SPR_CHAR_SELECT_CREATE_WINDOW);
	RELEASE_SPR(BUTTON_CREATE);
	RELEASE_SPR(BUTTON_CREATE_OVER);
	RELEASE_SPR(BUTTON_CREATE_DOWN);
	RELEASE_SPR(BUTTON_DELETE);
	RELEASE_SPR(BUTTON_DELETE_OVER);
	RELEASE_SPR(BUTTON_DELETE_DOWN);
	RELEASE_SPR(BUTTON_EXIT);
	RELEASE_SPR(BUTTON_EXIT_OVER);
	RELEASE_SPR(BUTTON_EXIT_DOWN);
	RELEASE_SPR(BUTTON_MODIFY);
	RELEASE_SPR(BUTTON_MODIFY_OVER);
	RELEASE_SPR(BUTTON_MODIFY_DOWN);
	RELEASE_SPR(BUTTON_CLASS_WARRIOR);
	RELEASE_SPR(BUTTON_CLASS_WARRIOR_DOWN);
	RELEASE_SPR(BUTTON_CLASS_PRIEST);
	RELEASE_SPR(BUTTON_CLASS_PRIEST_DOWN);
	RELEASE_SPR(BUTTON_CLASS_SUMMONER);
	RELEASE_SPR(BUTTON_CLASS_SUMMONER_DOWN);
	RELEASE_SPR(BUTTON_CLASS_HUNTER	);
	RELEASE_SPR(BUTTON_CLASS_HUNTER_DOWN);
	RELEASE_SPR(BUTTON_CLASS_WIZARD	);
	RELEASE_SPR(BUTTON_CLASS_WIZARD_DOWN);
	RELEASE_SPR(BUTTON_LEFT);
	RELEASE_SPR(BUTTON_LEFT_OVER);
	RELEASE_SPR(BUTTON_LEFT_DOWN);
	RELEASE_SPR(BUTTON_START);
	RELEASE_SPR(BUTTON_START_OVER);
	RELEASE_SPR(BUTTON_START_DOWN);
	RELEASE_SPR(BUTTON_RIGHT);
	RELEASE_SPR(BUTTON_RIGHT_OVER);
	RELEASE_SPR(BUTTON_RIGHT_DOWN);
	RELEASE_SPR(BUTTON_OK);
	RELEASE_SPR(BUTTON_OK_OVER);
	RELEASE_SPR(BUTTON_OK_DOWN);
	RELEASE_SPR(BUTTON_CANCEL);
	RELEASE_SPR(BUTTON_CANCEL_OVER);
	RELEASE_SPR(BUTTON_CANCEL_DOWN);
	RELEASE_SPR(SPR_SELECTCHAR_VILL1_1);
	RELEASE_SPR(SPR_SELECTCHAR_VILL1_2);
	RELEASE_SPR(SPR_SELECTCHAR_VILL1_3);
	RELEASE_SPR(SPR_SELECTCHAR_VILL2_1);
	RELEASE_SPR(SPR_SELECTCHAR_VILL2_2);
	RELEASE_SPR(SPR_SELECTCHAR_VILL2_3);

	CInterface* pInterface = CInterface::GetInstance();
	pInterface->Remove();
	
	StopBGM();
	
//	WORD wSex		= HIWORD(g_pGVLogin->dwCurHeadIndexforCreate);	
//	WORD wHeadIndex = LOWORD(g_pGVLogin->dwCurHeadIndexforCreate);
	
	LPObjectDesc pDesc;
	
	//캐릭터 생성할때 보여줄 모든 몸통 모델 Release
	for(int i=0; i<MAX_KIND_OF_HEAD_MALE; i++)
		DeleteObj(&g_pGVLogin->EnumCharHeadForCreate[1][i]);
		 	
	for(i=0; i<MAX_KIND_OF_HEAD_FEMALE; i++)
		DeleteObj(&g_pGVLogin->EnumCharHeadForCreate[0][i]);

	//캐릭터 생성할때 보여줄 모든 몸통 모델 Delete		
	for(i = 0; i < MAX_KIND_OF_CLASS; i++)
	{
		pDesc = (LPObjectDesc)g_pExecutive->GetData(g_pGVLogin->EnumCharForCreate[i]);

		ObjDeleteDesc(&pDesc);
		DeleteObj(&g_pGVLogin->EnumCharForCreate[i]);
	}
	
	for(i=0; i<MAX_CHAR_SELECT_NUM; i++)
	{
		DeleteObj(&g_pGVLogin->ChrSelectInfo[i].hGroundHandle);
		DeleteObj(&g_pGVLogin->ChrSelectInfo[i].hSpotLightHandle);
		ObjDeleteDesc(&g_pGVLogin->ChrSelectInfo[i].pDescGround);
		
		ItemDeleteObject(&g_pGVLogin->ChrSelectInfo[i].hRightHand, &g_pGVLogin->ChrSelectInfo[i].hHandle);
		ObjDeleteDesc(&g_pGVLogin->ChrSelectInfo[i].pRItemDesc);

		ItemDeleteObject(&g_pGVLogin->ChrSelectInfo[i].hLeftHand, &g_pGVLogin->ChrSelectInfo[i].hHandle);
		ObjDeleteDesc(&g_pGVLogin->ChrSelectInfo[i].pLItemDesc);

		if(g_pGVLogin->dwFlag==CHAR_SELECT_STATUS_START_SUCCESS && g_pGVLogin->dwCurCharIndex == (DWORD)i)	
			continue;
		
		ItemDeleteObject(&g_pGVLogin->ChrSelectInfo[i].hHandle);
		ItemDeleteObject(&g_pGVLogin->ChrSelectInfo[i].hHeadHandle);
		ItemDeleteObject(&g_pGVLogin->ChrSelectInfo[i].hHelmetHandle);
		ObjDeleteDesc(&g_pGVLogin->ChrSelectInfo[i].pDesc);		
	}

	//GM인 경우 
	if(g_pGVLogin->ChrSelectInfo[ g_pGVLogin->dwCurCharIndex ].ReceivedChrInfo.wGrade == USER_GRADE_GM)
	{	
	}

	g_pObjManager->RemoveObject(CHR_CHARACTER_SELECT);
	g_pGVLogin->pCharacterSelectBackGround = NULL;

	memset(g_pGVLogin->ChrSelectInfo, 0, sizeof(g_pGVLogin->ChrSelectInfo));

	g_pGVLogin->dwCurCharIndex = 0;
	g_nChatModeExt = 0;
	
	delete g_pGVLogin;
	g_pGVLogin = NULL;	
}


//==================================================================//
// 캐릭터 셀렉트 상태에서 렌더함수.									//
//==================================================================//
DWORD __stdcall AfterRenderGameCharSelect()
{
	g_helpLoadingSprite.Show();

	g_pSprManager->RenderAllSprite();

	CInterface* pInterface = CInterface::GetInstance();
	pInterface->Render();
	
	//ID 폰트 출력
	for(int i=1; i<MAX_INPUT_BUFFER_NUM; i++)
	{		
		if(i==1)
		{
#ifndef _USE_IME
			g_pInputManager->RenderInputBuffer(i);
#endif
		}
		else
		{
			g_pInputManager->RenderInputBuffer(i);			
		}	
	}

#ifdef _USE_IME
	GET_IMEEDIT()->RenderSprite(5);
#endif
	
	return 0;
}

DWORD __stdcall BeforeRenderGameCharSelect()
{
	return 0;
}


//==================================================================//
// 캐릭터 셀렉트 상태에서 키입력 함수.								//
//==================================================================//
void OnCharCharSelect(BYTE bCh)
{	
	DWORD dwMaxLength = 0;

	CCharMoveEditWnd* pCharMoveEditWnd = CCharMoveEditWnd::GetInstance();

	if(bCh==VK_RETURN)
	{
		CInterface* pInterface = CInterface::GetInstance();

		for(int i = 0; i < CUR_INTERFACE; i++)
		{
			if(pInterface->m_pMenu[i]->m_nIndex==CHAR_MOVEEDIT_WND)
			{
				if(!(pCharMoveEditWnd->GetActive()==TRUE && pCharMoveEditWnd->m_bInputMode))
				{
					//캐릭터 생성 모드 이거나 삭제모드시 주민등록번호 입력할때만 입력하는 글자가 화면에 표시되어야 한다. 
					if(g_pGVLogin->dwFlag != CHAR_SELECT_STATUS_CHAR_MAKING && g_pGVLogin->dwFlag != CHAR_SELECT_STATUS_INPUT_JUMIN_NUM)
						return;		
				}				
			}
			else if(pInterface->m_pMenu[i]->m_nIndex==QUANTITY_WND)
			{
				if(pInterface->m_pMenu[i]->GetActive())
				{
					CQuantityWnd* pQuantityWnd = CQuantityWnd::GetInstance();										
					SendCharacterDeleteMsg();
					pQuantityWnd->SetActive(FALSE);
				}				
			}
			else
			{
				if(pInterface->m_pMenu[i]->GetActive())
					pInterface->m_pMenu[i]->SetActive(FALSE);
			}
		}		
	}
	
	switch(bCh)
	{
	case 9:	return;//Tab	
	case 8:	//BackSpace
		{
#ifdef _USE_IME
			if (GET_IMEEDIT()->IsActivated()) return;
#endif
			g_pInputManager->BackSpaceEvent();
			return;
		}
		break;	
	case VK_ESCAPE:
	case VK_SPACE:	
		::SendMessage(g_hMainWnd, WM_IME_ENDCOMPOSITION, 0, 0);			
		return;	
	case VK_RETURN:
		{
			switch(g_pGVLogin->dwFlag)
			{
			case CHAR_SELECT_STATUS_AWAITING_CREATE_RESULT:	
			case CHAR_SELECT_STATUS_CHAR_MAKING:				//캐릭터 만들고 있는중에 Enter
				{					
					return;
				}
				break;
			}
		}	
	}
	
	if(pCharMoveEditWnd->GetActive()==TRUE && pCharMoveEditWnd->m_bInputMode)
		dwMaxLength = MAX_ID_REAL_LENGTH;
	else if(g_pGVLogin->dwFlag == CHAR_SELECT_STATUS_CHAR_MAKING)
		dwMaxLength = MAX_ID_REAL_LENGTH;
	else
		dwMaxLength = 7;	//주민등록번호 뒷자리 7개

	if(g_pInputManager->GetInputBufferLength(g_pInputManager->GetCurFocusInputID()) >= dwMaxLength)
		return;

	g_pInputManager->InsertCharacter(g_pInputManager->GetCurFocusInputID(), (char*)&bCh, 1);
}

void OnKeyDownCharSelect(WPARAM wParam, LPARAM lParam)
{
}

void OnKeyUpCharSelect(WPARAM wParam, LPARAM lParam)
{
}


//==================================================================//
// 캐릭터 셀렉트 상태에서 마우스 함수.								//
//==================================================================//
void OnLButtonDownCharSelect(WPARAM wParam, LPARAM lParam)
{	
	CInterface*	 pInterface	= CInterface::GetInstance();	
		
	if(pInterface->InterfaceCheck()==-1)
		pInterface->m_bActiveChk = TRUE;

	CharSelectMousePointer(wParam, lParam);
	CharSelectMouseProcess(wParam, lParam, CHAR_SEL_MOUSE_DOWN);
}

void OnLButtonUpCharSelect(WPARAM wParam, LPARAM lParam)
{	
	CInterface*		pInterface		= CInterface::GetInstance();
	CQuantityWnd*	pQuantityWnd	= CQuantityWnd::GetInstance();
	
	pInterface->SetUp(TRUE);	
	pInterface->InterfaceCheck();
		
	if(pQuantityWnd->m_bQuantityType==__QUANTITY_JUMIN_NUMBER__)
	{
		if(pQuantityWnd->IsOk())
		{
			SendCharacterDeleteMsg();
		}
		else
		{
			//g_pGVLogin->dwFlag = CHAR_SELECT_STATUS_NORMAL;
			g_pInputManager->ClearInput(INPUT_BUFFER_CHARACTER_JUMIN);
		}
	}

	if(pInterface->GetClick())
		pInterface->SetClick(FALSE);

	pInterface->m_bActiveChk = FALSE;
	
	CharSelectMousePointer(wParam, lParam);
	CharSelectMouseProcess(wParam, lParam, CHAR_SEL_MOUSE_UP);	
}

void OnMouseMoveCharSelect(WPARAM wParam, LPARAM lParam)
{
	CInterface* pInterface	= CInterface::GetInstance();
	
	pInterface->SetUp(FALSE);
	
	if(pInterface->m_bActiveChk==FALSE)	
		pInterface->InterfaceCheck();	
	
	if(!g_Mouse.bLDown)	
		pInterface->m_bActiveChk = FALSE;

	CharSelectMousePointer(wParam, lParam);
	CharSelectMouseProcess(wParam, lParam, CHAR_SEL_MOUSE_OVER);
}

void OnRButtonDownCharSelect(WPARAM wParam, LPARAM lParam)
{
}

void OnRButtonUpCharSelect(WPARAM wParam, LPARAM lParam)
{
}

void OnMouseWheelCharSelect(WPARAM wParam, LPARAM lParam)
{
}
//==================================================================//
// 캐릭터 셀렉트 상태에서 마우스 처리 관련 함수.					//
//==================================================================//
void MouseType(BYTE byType)
{
	BYTE byMouseType = 0;

	if(g_Mouse.bLDown)
	{
		switch(byType)
		{
		case CHAR_SEL_MOUSE_BUTTON:		byMouseType = __MOUSE_POINTER_BUTTONCLICK__;	break;
		case CHAR_SEL_MOUSE_DEFAULT:	byMouseType = __MOUSE_POINTER_DEFAULTCLICK__;	break;
		}
	}
	else
	{
		switch(byType)
		{
		case CHAR_SEL_MOUSE_BUTTON:		byMouseType = __MOUSE_POINTER_BUTTON__;		break;
		case CHAR_SEL_MOUSE_DEFAULT:	byMouseType = __MOUSE_POINTER_DEFAULT__;	break;
		}
	}
	SetMousePointerType(byMouseType);
}

void CharSelectMousePointer(WPARAM wParam, LPARAM lParam)
{
	WORD dwX = LOWORD( lParam );
	WORD dwY = HIWORD( lParam );
	
	if(dwX>=556 && dwX<=580 && dwY>=643 && dwY<=668)
	{
		MouseType(CHAR_SEL_MOUSE_BUTTON);
	}
	else if(dwX>=602 && dwX<=695 && dwY>=643 && dwY<=668)
	{
		MouseType(CHAR_SEL_MOUSE_BUTTON);
	}
	else if(dwX>=716 && dwX<=740 && dwY>=643 && dwY<=668)
	{
		MouseType(CHAR_SEL_MOUSE_BUTTON);
	}
	else if(dwX>=53 && dwX<=116 && dwY>=397 && dwY<=412)
	{
		if(IsEmptyString(g_pGVLogin->ChrSelectInfo[ g_pGVLogin->dwCurCharIndex ].ReceivedChrInfo.szName))
			MouseType(CHAR_SEL_MOUSE_BUTTON);
		else
			MouseType(CHAR_SEL_MOUSE_DEFAULT);
	}
	else if(dwX>=121 && dwX<=184 && dwY>=397 && dwY<=412)
	{		
		if(IsEmptyString(g_pGVLogin->ChrSelectInfo[ g_pGVLogin->dwCurCharIndex ].ReceivedChrInfo.szName))
			MouseType(CHAR_SEL_MOUSE_DEFAULT);
		else
			MouseType(CHAR_SEL_MOUSE_BUTTON);
	}
	else if(dwX>=81 && dwX<=144 && dwY>=484 && dwY<=499)
	{
		if(g_bCharacterChk==TRUE)
			MouseType(CHAR_SEL_MOUSE_BUTTON);
		else
			MouseType(CHAR_SEL_MOUSE_DEFAULT);
	}
	else if(dwX>=26 && dwX<=58 && dwY>=524 && dwY<=556)
	{
		if(g_bCharacterChk==TRUE)
			MouseType(CHAR_SEL_MOUSE_BUTTON);
		else
			MouseType(CHAR_SEL_MOUSE_DEFAULT);
	}		
	else if(dwX>=69 && dwX<=101 && dwY>=524 && dwY<=556)
	{
		if(g_bCharacterChk==TRUE)
			MouseType(CHAR_SEL_MOUSE_BUTTON);
		else
			MouseType(CHAR_SEL_MOUSE_DEFAULT);
	}
	else if(dwX>=112 && dwX<=144 && dwY>=524 && dwY<=556)
	{
		// 서머너 선택 버튼 활성화 ( LOCAL04080701 과 일치화 )
		// ( 2004. 09. 06. minjin )
		if(g_bCharacterChk==TRUE)
			MouseType(CHAR_SEL_MOUSE_BUTTON);
		else
			MouseType(CHAR_SEL_MOUSE_DEFAULT);
	}
	else if(dwX>=155 && dwX<=187 && dwY>=524 && dwY<=556)
	{
		if(g_bCharacterChk==TRUE)
			MouseType(CHAR_SEL_MOUSE_BUTTON);
		else
			MouseType(CHAR_SEL_MOUSE_DEFAULT);
	}
	else if(dwX>=198 && dwX<=230 && dwY>=524 && dwY<=556)
	{
		if(g_bCharacterChk==TRUE)
			MouseType(CHAR_SEL_MOUSE_BUTTON);
		else
			MouseType(CHAR_SEL_MOUSE_DEFAULT);
	}
	else if(dwX>=123 && dwX<=186 && dwY>=654 && dwY<=669)
	{
		if(g_bCharacterChk==TRUE)
			MouseType(CHAR_SEL_MOUSE_BUTTON);
		else
			MouseType(CHAR_SEL_MOUSE_DEFAULT);
	}
	else if(dwX>=190 && dwX<=253 && dwY>=654 && dwY<=669)
	{
		if(g_bCharacterChk==TRUE)
			MouseType(CHAR_SEL_MOUSE_BUTTON);
		else
			MouseType(CHAR_SEL_MOUSE_DEFAULT);
	}
	else if(dwX>=189 && dwX<=251 && dwY>=397 && dwY<=412)
	{
		MouseType(CHAR_SEL_MOUSE_BUTTON);
	}
	else if(dwX>=13 && dwX<=77 && dwY>=583 && dwY<=599)
	{
		if(g_bCharacterChk)
			MouseType(CHAR_SEL_MOUSE_BUTTON);
		else
			MouseType(CHAR_SEL_MOUSE_DEFAULT);
	}
	else if(dwX>=80 && dwX<=144 && dwY>=583 && dwY<=599)
	{
		SetMousePointerType(__MOUSE_POINTER_BUTTONCLICK__);
	}
	else
		MouseType(CHAR_SEL_MOUSE_DEFAULT);
}

void CharSelectMouseProcess(WPARAM wParam, LPARAM lParam, BYTE byType)
{
	WORD dwX = LOWORD( lParam );
	WORD dwY = HIWORD( lParam );

	V2_SPRITE*	pSpr		= NULL;
	V2_SPRITE*	pSprDown	= NULL;
	V2_SPRITE*	pSprOver	= NULL;
	DWORD		dwBtnID		= 0 , dwOverBtnID = 0, dwDownBtnID = 0;

	for(DWORD i = 0; i < TOTAL_OVER_BUTTON_NUM; i++)
	{
		dwBtnID		= BUTTON_OK + i;
		dwOverBtnID = BUTTON_OK_OVER + i;
		dwDownBtnID = BUTTON_OK_DOWN + i;
		pSpr		= g_pSprManager->GetSprite(dwBtnID);
		pSprDown	= g_pSprManager->GetSprite(dwDownBtnID);
		pSprOver	= g_pSprManager->GetSprite(dwOverBtnID);
	
		if(byType==CHAR_SEL_MOUSE_UP)
		{
			if(pSprDown->PtInSpr(dwX, dwY) || pSprOver->PtInSpr(dwX, dwY) || pSpr->PtInSpr(dwX, dwY))
			{
				if(g_bCharMoveChk)
					break;
				
				pSprDown->ShowSprite(FALSE);
				pSprOver->ShowSprite(TRUE);
				OnSprButtonProcessCharSelect(dwBtnID);
				return;
			}
		}
		else if(byType==CHAR_SEL_MOUSE_DOWN)
		{
			if(pSprOver->PtInSpr(dwX, dwY))
			{
				pSprOver->ShowSprite(FALSE);
				pSprDown->ShowSprite(TRUE);
				return;
			}
		}
		else
		{
			if( pSpr)
			{
				if(pSpr->PtInSpr(dwX, dwY))
				{				
					_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_CHARSELBTNON, g_v3InterfaceSoundPos, FALSE);
					pSpr->ShowSprite(FALSE);
					pSprOver->ShowSprite(TRUE);
					return;
				}
				else if( !pSprOver->PtInSpr(dwX, dwY) && pSprOver->bRenderFlag )
				{
					pSpr->ShowSprite(TRUE);
					pSprOver->ShowSprite(FALSE);
				}
			}
		}		
	}

	for(i = 0; i < 5; i++)
	{
		dwBtnID		= BUTTON_CLASS_WARRIOR + i;
		dwDownBtnID = BUTTON_CLASS_WARRIOR_DOWN + i;
		pSpr		= g_pSprManager->GetSprite(dwBtnID);
		pSprDown	= g_pSprManager->GetSprite(dwDownBtnID);

		if(byType==CHAR_SEL_MOUSE_DOWN)
		{
			if(pSpr->PtInSpr(dwX, dwY))	
			{
/*#if IS_CHINA_LOCALIZING() || IS_JAPAN_LOCALIZING()	// !
				if (dwBtnID ==BUTTON_CLASS_SUMMONER)
				{ // 서머너 관련
					break;
				}
#endif*/
				pSpr->ShowSprite(FALSE);
				pSprDown->ShowSprite(TRUE);
				return;
			}		
		}
		else if(byType==CHAR_SEL_MOUSE_UP)
		{
			// modified by deepdark
			if(g_bCharacterChk)
			{
				if(g_bCharMoveChk)
					break;
				
				pSpr->ShowSprite(TRUE);
				pSprDown->ShowSprite(FALSE);
			}
			
			if( pSprDown->PtInSpr(dwX, dwY) || pSpr->PtInSpr(dwX, dwY))		
			{			
				if(g_bCharMoveChk)
					break;

				OnSprButtonProcessCharSelect(dwBtnID);
				return;
			}
		}
	}
	
	if(byType==CHAR_SEL_MOUSE_OVER)
	{
		if(dwX>=80 && dwX<=144 && dwY>=583 && dwY<=599)		
		{
			if(g_bCharacterChk)
			{
				SPR(SPR_SELECTCHAR_VILL1_2)->ShowSprite(FALSE);
				SPR(SPR_SELECTCHAR_VILL2_1)->ShowSprite(FALSE);
				
				if(g_bVilType==0)
				{
					SPR(SPR_SELECTCHAR_VILL1_1)->ShowSprite(FALSE);						
					SPR(SPR_SELECTCHAR_VILL1_3)->ShowSprite(TRUE);
					SPR(SPR_SELECTCHAR_VILL2_2)->ShowSprite(TRUE);										
					SPR(SPR_SELECTCHAR_VILL2_3)->ShowSprite(FALSE);				
				}
				else if(g_bVilType==1)
				{				
					SPR(SPR_SELECTCHAR_VILL1_1)->ShowSprite(TRUE);										
					SPR(SPR_SELECTCHAR_VILL1_3)->ShowSprite(FALSE);				
					SPR(SPR_SELECTCHAR_VILL2_2)->ShowSprite(FALSE);					
					SPR(SPR_SELECTCHAR_VILL2_3)->ShowSprite(TRUE);
				}
			}
		}
		else
		{
			if(g_bCharacterChk)
			{			
				SPR(SPR_SELECTCHAR_VILL1_2)->ShowSprite(FALSE);
				SPR(SPR_SELECTCHAR_VILL2_2)->ShowSprite(FALSE);

				if(g_bVilType==0)
				{
					SPR(SPR_SELECTCHAR_VILL1_1)->ShowSprite(FALSE);				
					SPR(SPR_SELECTCHAR_VILL1_3)->ShowSprite(TRUE);
					SPR(SPR_SELECTCHAR_VILL2_1)->ShowSprite(TRUE);				
					SPR(SPR_SELECTCHAR_VILL2_3)->ShowSprite(FALSE);
				}
				else if(g_bVilType==1)
				{
					SPR(SPR_SELECTCHAR_VILL1_1)->ShowSprite(TRUE);			
					SPR(SPR_SELECTCHAR_VILL1_3)->ShowSprite(FALSE);			
					SPR(SPR_SELECTCHAR_VILL2_1)->ShowSprite(FALSE);			
					SPR(SPR_SELECTCHAR_VILL2_3)->ShowSprite(TRUE);
				}
			}			
		}
	}
	else if(byType==CHAR_SEL_MOUSE_DOWN)
	{
		if(g_bCharacterChk)
		{
			if(dwX>=80 && dwX<=144 && dwY>=583 && dwY<=599)		
			{
				g_bVilType = 1;
			}
			else
			{
				g_bVilType = 0;
			}
			
			SPR(SPR_SELECTCHAR_VILL1_2)->ShowSprite(FALSE);
			SPR(SPR_SELECTCHAR_VILL2_2)->ShowSprite(FALSE);

			if(g_bVilType==0)
			{
				SPR(SPR_SELECTCHAR_VILL1_1)->ShowSprite(FALSE);
				SPR(SPR_SELECTCHAR_VILL1_3)->ShowSprite(TRUE);
				SPR(SPR_SELECTCHAR_VILL2_1)->ShowSprite(TRUE);
				SPR(SPR_SELECTCHAR_VILL2_3)->ShowSprite(FALSE);
			}
			else if(g_bVilType==1)
			{				
				SPR(SPR_SELECTCHAR_VILL1_1)->ShowSprite(TRUE);
				SPR(SPR_SELECTCHAR_VILL1_3)->ShowSprite(FALSE);
				SPR(SPR_SELECTCHAR_VILL2_1)->ShowSprite(FALSE);
				SPR(SPR_SELECTCHAR_VILL2_3)->ShowSprite(TRUE);
			}
		}
	}	
}

void OnSprButtonProcessCharSelect(DWORD dwSprID)
{
	CGameDefaultWnd* pGameDefaultWnd = CGameDefaultWnd::GetInstance();

	if(g_bCharMoveChk)
		return;

	switch(dwSprID)
	{
	case BUTTON_OK:
		{
			if(g_pGVLogin->dwFlag == CHAR_SELECT_STATUS_AWAITING_CREATE_RESULT)
			{
				// "CorumOnline", 
				// "캐릭터를 생성중입니다.", 
				// "잠시만 기다려 주세요", 
				pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, g_Message[ETC_MESSAGE691].szMessage, g_Message[ETC_MESSAGE692].szMessage, "", 502, 320, 2, TRUE);					
				pGameDefaultWnd->SetActive();	
				return;
			}

#ifdef _USE_IME
			LPSTR lpszName = (LPSTR)GET_IMEEDIT()->GetImeEditString();			
			g_pInputManager->ClearInput(INPUT_BUFFER_CHARACTER_NAME1);
			g_pInputManager->InsertCharacter(INPUT_BUFFER_CHARACTER_NAME1,lpszName, lstrlen(lpszName));	
#endif
			
			int nLen = g_pInputManager->GetInputBufferLength(INPUT_BUFFER_CHARACTER_NAME1);
			
			//길이 제한 
			if(nLen < 4)
			{
				// "CorumOnline"					//
				// "캐릭터 이름은 한글 2자",		//
				// "영문 4자 이상이어야 합니다."	//
				pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, g_Message[ETC_MESSAGE479].szMessage, g_Message[ETC_MESSAGE480].szMessage, "", 502, 320, 2); 
				pGameDefaultWnd->SetActive(TRUE);
				g_pInputManager->ClearInput(INPUT_BUFFER_CHARACTER_NAME1);				
				return;		
			}

			if(IsSpecialCharacter(g_pInputManager->GetInputBuffer(INPUT_BUFFER_CHARACTER_NAME1)))
			{				
				//"CorumOnline"								//
				// "특수문자나 한자는 사용할 수 없습니다."	//
				pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, g_Message[ETC_MESSAGE481].szMessage, "", "", 502, 320, 2, TRUE); 
				pGameDefaultWnd->SetActive(TRUE);
				g_pInputManager->ClearInput(INPUT_BUFFER_CHARACTER_NAME1);
				return;		
			}

			int nRt = ConvString(g_pInputManager->GetInputBuffer(INPUT_BUFFER_CHARACTER_NAME1));

			if(nRt==1)
			{	
				pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, g_Message[ETC_MESSAGE19].szMessage, g_Message[ETC_MESSAGE20].szMessage, "", 502, 320, 2, TRUE);		// MSG_ID : 19 ; 비속어나 욕설은 사용할, 20 ; 수 없습니다.
				pGameDefaultWnd->SetActive(TRUE);
				g_pInputManager->ClearInput(INPUT_BUFFER_CHARACTER_NAME1);
				return;
			}

			nRt = NoConvString(g_pInputManager->GetInputBuffer(INPUT_BUFFER_CHARACTER_NAME1));
					
			if(nRt==1)
			{						
				pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, g_Message[ETC_MESSAGE3].szMessage, g_Message[ETC_MESSAGE4].szMessage, "", 502, 320, 2, TRUE); // MSG_ID : 3 ; 금지 단어입니다. 사용할 수 , 4 ; 없습니다.
				pGameDefaultWnd->SetActive(TRUE);				
				g_pInputManager->ClearInput(INPUT_BUFFER_CHARACTER_NAME1);
				return;									
			}

			int ch = '\'';
			
			if(strchr(g_pInputManager->GetInputBuffer(INPUT_BUFFER_CHARACTER_NAME1), ch) )
			{
				pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, g_Message[ETC_MESSAGE5].szMessage, "", "", 502, 320, 1);	// MSG_ID : 5 ; 사용할수 없는 캐릭터 이름입니다.
				pGameDefaultWnd->SetActive(TRUE);				
				_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);								
				return;
			}

			if(g_pGVLogin->dwFlag != CHAR_SELECT_STATUS_CHAR_MAKING)		
				return;							
			
			g_bCharacterChk = FALSE;
			g_pInputManager->SetFocusInput(0);
						
			//여기서 캐릭터 생성한다는 패킷 날림 	
			
			// g_bVillType = 0 루, 1, 이다.
			CTWS_CREATE_NEW_CHARACTER	packet;
			packet.bChrNum = (BYTE)g_pGVLogin->dwCurCharIndex;
			packet.wHead = WORD(LOWORD(g_pGVLogin->dwCurHeadIndexforCreate) + 1);
			packet.bClass = g_pGVLogin->bCurClassIndexForCreate;

/*
#if IS_CHINA_LOCALIZING()
			// 서머너 선택 블로킹 : 대만은 일단 풀어준다.
			// 일본도 풀어준다 (2004. 08. 12. minjin)
			if (packet.bClass == 3)
			{ 
				// 서머너는 막는다.
				return;
			}
#endif*/
			packet.wStartMapID = WORD((g_bVilType) ? 3 : 1); // 이다:루디로스

			memset(packet.szCharacterName, 0, sizeof(packet.szCharacterName));
			__lstrcpyn(packet.szCharacterName, g_pInputManager->GetInputBuffer(INPUT_BUFFER_CHARACTER_NAME1), MAX_CHARACTER_NAME_REAL_LENGTH);

			//	by minjin. 2004. 10. 25.
			SendMsgWithBlowfish((char*)&packet, packet.GetPacketSize(), SERVER_INDEX_AGENT);
			//g_pNet->SendMsg((char*)&packet, packet.GetPacketSize(), SERVER_INDEX_AGENT );

			ShowObject(g_pGVLogin->ChrSelectInfo[ 0 ].hSpotLightHandle);	//Spot Light On
			
			g_pGVLogin->dwFlag = CHAR_SELECT_STATUS_AWAITING_CREATE_RESULT;									
		}
		break;
	case BUTTON_CANCEL:
		{
			g_bCharacterChk = FALSE;

			WORD wSex =		  HIWORD(g_pGVLogin->dwCurHeadIndexforCreate);
			WORD wHeadIndex = LOWORD(g_pGVLogin->dwCurHeadIndexforCreate);

			HideCreateWindow();
			g_pExecutive->GXODetach(g_pGVLogin->EnumCharForCreate[ g_pGVLogin->bCurClassIndexForCreate - 1 ], g_pGVLogin->EnumCharHeadForCreate[wSex][wHeadIndex]);
			HideObject(g_pGVLogin->EnumCharForCreate[ g_pGVLogin->bCurClassIndexForCreate - 1 ]);
			HideObject(g_pGVLogin->EnumCharHeadForCreate[wSex][wHeadIndex]);
			
			g_pInputManager->ClearInput(INPUT_BUFFER_CHARACTER_NAME1);
			g_pInputManager->SetFocusInput(0);
			g_pGVLogin->dwFlag = CHAR_SELECT_STATUS_NORMAL;
			
			SPR(SPR_SELECTCHAR_VILL1_1)->ShowSprite(FALSE);
			SPR(SPR_SELECTCHAR_VILL1_2)->ShowSprite(FALSE);
			SPR(SPR_SELECTCHAR_VILL1_3)->ShowSprite(FALSE);
			SPR(SPR_SELECTCHAR_VILL2_1)->ShowSprite(FALSE);
			SPR(SPR_SELECTCHAR_VILL2_2)->ShowSprite(FALSE);
			SPR(SPR_SELECTCHAR_VILL2_3)->ShowSprite(FALSE);

#ifdef _USE_IME
			GET_IMEEDIT()->DeactivateIME(TRUE);
#endif
		}	
		break;
	case BUTTON_CREATE:
		{
			if(g_bCharacterChk==TRUE)
				break;
			
			g_bCharacterChk = TRUE;

// 일본과 중국의 경우 기본 마을을 이다로 한다!
#if IS_JAPAN_LOCALIZING() || IS_CHINA_LOCALIZING()

			g_bVilType = 1;
			SPR(SPR_SELECTCHAR_VILL1_1)->ShowSprite(TRUE);
			SPR(SPR_SELECTCHAR_VILL1_2)->ShowSprite(FALSE);
			SPR(SPR_SELECTCHAR_VILL1_3)->ShowSprite(FALSE);
			SPR(SPR_SELECTCHAR_VILL2_1)->ShowSprite(FALSE);
			SPR(SPR_SELECTCHAR_VILL2_2)->ShowSprite(FALSE);
			SPR(SPR_SELECTCHAR_VILL2_3)->ShowSprite(TRUE);
#elif IS_TAIWAN_LOCALIZING()
			srand( g_dwCurTick );
			g_bVilType = GetRandom(2);
			if (g_bVilType)
			{
				// 이다 
				SPR(SPR_SELECTCHAR_VILL1_1)->ShowSprite(TRUE);
				SPR(SPR_SELECTCHAR_VILL1_2)->ShowSprite(FALSE);
				SPR(SPR_SELECTCHAR_VILL1_3)->ShowSprite(FALSE);
				SPR(SPR_SELECTCHAR_VILL2_1)->ShowSprite(FALSE);
				SPR(SPR_SELECTCHAR_VILL2_2)->ShowSprite(FALSE);
				SPR(SPR_SELECTCHAR_VILL2_3)->ShowSprite(TRUE);				
			}
			else
			{
				// 루디로스
				SPR(SPR_SELECTCHAR_VILL1_1)->ShowSprite(FALSE);
				SPR(SPR_SELECTCHAR_VILL1_2)->ShowSprite(FALSE);
				SPR(SPR_SELECTCHAR_VILL1_3)->ShowSprite(TRUE);
				SPR(SPR_SELECTCHAR_VILL2_1)->ShowSprite(TRUE);
				SPR(SPR_SELECTCHAR_VILL2_2)->ShowSprite(FALSE);
				SPR(SPR_SELECTCHAR_VILL2_3)->ShowSprite(FALSE);				
			}

#else
			g_bVilType = 0;
			SPR(SPR_SELECTCHAR_VILL1_1)->ShowSprite(FALSE);
			SPR(SPR_SELECTCHAR_VILL1_2)->ShowSprite(FALSE);
			SPR(SPR_SELECTCHAR_VILL1_3)->ShowSprite(TRUE);
			SPR(SPR_SELECTCHAR_VILL2_1)->ShowSprite(TRUE);
			SPR(SPR_SELECTCHAR_VILL2_2)->ShowSprite(FALSE);
			SPR(SPR_SELECTCHAR_VILL2_3)->ShowSprite(FALSE);
#endif
			SPR(SPR_CHAR_SELECT_CREATE_WINDOW)->ShowSprite(TRUE);
			SPR(BUTTON_OK)->ShowSprite(TRUE);
			SPR(BUTTON_CANCEL)->ShowSprite(TRUE);
			SPR(BUTTON_MODIFY)->ShowSprite(TRUE);
			
			SPR(BUTTON_CLASS_WARRIOR)->ShowSprite(TRUE);
			SPR(BUTTON_CLASS_PRIEST)->ShowSprite(TRUE);
			SPR(BUTTON_CLASS_SUMMONER)->ShowSprite(TRUE);
			SPR(BUTTON_CLASS_HUNTER)->ShowSprite(TRUE);
			SPR(BUTTON_CLASS_WIZARD)->ShowSprite(TRUE);

			SPR(BUTTON_DELETE)->ShowSprite(FALSE);

			g_pInputManager->SetFocusInput(INPUT_BUFFER_CHARACTER_NAME1);
			
			//첫번째 모델로 보여주고..
			ShowObject(g_pGVLogin->EnumCharForCreate[0]);
			ShowObject(g_pGVLogin->EnumCharHeadForCreate[1][0]);	//tt
			g_pExecutive->GXOAttach(g_pGVLogin->EnumCharHeadForCreate[1][0], g_pGVLogin->EnumCharForCreate[0], "Bip01 Head");

			g_pGVLogin->bCurClassIndexForCreate = CLASS_TYPE_WARRIOR;
			g_pGVLogin->dwCurHeadIndexforCreate = MAKELONG(0, 1);		//1이 남자, 0이 여자. .ㅡㅡ;;

			g_pInputManager->InsertCharacter(INPUT_BUFFER_CHARACTER_RACE2, g_Message[ETC_MESSAGE6].szMessage, lstrlen(g_Message[ETC_MESSAGE6].szMessage));	// MSG_ID : 6 ; HUMAN
			g_pInputManager->InsertCharacter(INPUT_BUFFER_CHARACTER_CLASS2, g_Message[ETC_MESSAGE7].szMessage, lstrlen(g_Message[ETC_MESSAGE7].szMessage));	// MSG_ID : 7 ; 파이터			
			g_pGVLogin->dwFlag = CHAR_SELECT_STATUS_CHAR_MAKING;		//1이면 캐릭터 생성 모드 
			
#ifdef _USE_IME

			POINT pChatPoint;
			pChatPoint.x	= 80;
			pChatPoint.y	= 449;
			GET_IMEEDIT()->ActivateIME(pChatPoint, 16, __ORDER_USERINTERFACE_START_, IMEOPTION_NORMAL);
#endif
		}
		break;
	case BUTTON_DELETE:
		{
			if(g_pGVLogin->dwFlag == CHAR_SELECT_STATUS_AWAITING_DELETE_RESULT)
			{
				// "CorumOnline"			//
				// "캐릭터를 삭제중입니다."	//
				// "잠시만 기다려 주세요"	//
				pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, g_Message[ETC_MESSAGE792].szMessage, g_Message[ETC_MESSAGE692].szMessage, "", 502, 320, 2, TRUE);					
				pGameDefaultWnd->SetActive();
				return;
			}			
			if( g_byCharacterAllCount <= 1)
			{
				pGameDefaultWnd->OpenWndEx(g_Message[ETC_MESSAGE248].szMessage, "캐릭터를 삭제하면 창고가 사라집니다.", "그래도 삭제하시겠습니까?", "", 502, 360, 2);
			}
			else
			{
				pGameDefaultWnd->OpenWndEx(g_Message[ETC_MESSAGE248].szMessage, g_Message[ETC_MESSAGE8].szMessage, "", "", 502, 320, 1);	// MSG_ID : 8 ; 삭제하시겠습니까?
			}
			
			pGameDefaultWnd->SetActive(TRUE);
		}
		break;
	case BUTTON_MODIFY:
		{			
			WORD wSex =		  HIWORD(g_pGVLogin->dwCurHeadIndexforCreate);
			WORD wHeadIndex = LOWORD(g_pGVLogin->dwCurHeadIndexforCreate);

			HideObject(g_pGVLogin->EnumCharHeadForCreate[wSex][wHeadIndex]);	//이전 머리 모양 Hide 
			g_pExecutive->GXODetach(g_pGVLogin->EnumCharForCreate[g_pGVLogin->bCurClassIndexForCreate-1], g_pGVLogin->EnumCharHeadForCreate[wSex][wHeadIndex]);
			
			wHeadIndex++;
			
			if(wSex && wHeadIndex >= MAX_KIND_OF_HEAD_MALE)
				wHeadIndex = 0;
			if(!wSex && wHeadIndex >= MAX_KIND_OF_HEAD_FEMALE)
				wHeadIndex = 0;

			ShowObject(g_pGVLogin->EnumCharHeadForCreate[wSex][wHeadIndex]);	//새로운 머리 모양 Show
			g_pExecutive->GXOAttach(g_pGVLogin->EnumCharHeadForCreate[wSex][wHeadIndex], g_pGVLogin->EnumCharForCreate[g_pGVLogin->bCurClassIndexForCreate-1], "Bip01 Head");
			g_pGVLogin->dwCurHeadIndexforCreate = MAKELONG(wHeadIndex, wSex);
		}
		break;
	case BUTTON_LEFT:
		{
			if(g_bCharMoveChk)
				break;
			
			if(g_pGVLogin->dwFlag>=CHAR_SELECT_STATUS_CHAR_MAKING)	//캐릭터 만들고 있는중 이거나 돌리고 있는중 
				return;			
			if(!CheckCompleteRotate())
				return;
			
			SPR(BUTTON_CREATE)->ShowSprite(FALSE);
			SPR(BUTTON_DELETE)->ShowSprite(FALSE);
			_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_CHARSELBTNCLICK, g_v3InterfaceSoundPos, FALSE);
			_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_CHARSELROTATE, g_v3InterfaceSoundPos, FALSE);

			for(int i=0; i<MAX_CHAR_SELECT_NUM; i++)
			{
				if(g_pGVLogin->ChrSelectInfo[i].hHandle)
				{
					g_pGVLogin->ChrSelectInfo[i].pDesc->ObjectFunc = CharSelectRotateLeft;
					g_pGVLogin->ChrSelectInfo[i].pDesc->bSkip = 1;	
					::SetAction( g_pGVLogin->ChrSelectInfo[i].hHandle, 1, 0, ACTION_LOOP );
					g_pGVLogin->ChrSelectInfo[i].pDesc->dwTempBuf[10] = 0;
				}
				//Spot Light 끄고 
				HideObject(g_pGVLogin->ChrSelectInfo[i].hSpotLightHandle);
				g_pGVLogin->ChrSelectInfo[i].pDescGround->ObjectFunc = CharSelectGroundLeft;
				g_pGVLogin->ChrSelectInfo[i].pDescGround->bSkip = 1;
			}
			g_pGVLogin->dwCurCharIndex++;
			
			if(g_pGVLogin->dwCurCharIndex >= 4)
				g_pGVLogin->dwCurCharIndex = 0;
			
			g_pGVLogin->dwFlag = CHAR_SELECT_STATUS_CHAR_ROTATING;	//캐릭터 돌리고 있는중 			
		}
		break;
	case BUTTON_RIGHT:
		{
			if(g_pGVLogin->dwFlag >= CHAR_SELECT_STATUS_CHAR_MAKING)	//캐릭터 만들고 있는중 이거나 돌리고 있는중 
				return;
			
			if(!CheckCompleteRotate())
				return;
			
			SPR(BUTTON_CREATE)->ShowSprite(FALSE);
			SPR(BUTTON_DELETE)->ShowSprite(FALSE);
			_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_CHARSELBTNCLICK, g_v3InterfaceSoundPos, FALSE);
			_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_CHARSELROTATE, g_v3InterfaceSoundPos, FALSE);

			for(int i=0; i<MAX_CHAR_SELECT_NUM; i++)
			{
				if(g_pGVLogin->ChrSelectInfo[i].hHandle)
				{
					g_pGVLogin->ChrSelectInfo[i].pDesc->ObjectFunc = CharSelectRotateRight;
					g_pGVLogin->ChrSelectInfo[i].pDesc->bSkip = 1;						
					::SetAction( g_pGVLogin->ChrSelectInfo[ i ].hHandle, 1, 0, ACTION_LOOP );
					g_pGVLogin->ChrSelectInfo[ i ].pDesc->dwTempBuf[10] = 0;
				}

				//Spot Light 끄고 
				HideObject(g_pGVLogin->ChrSelectInfo[ i ].hSpotLightHandle);
				g_pGVLogin->ChrSelectInfo[i].pDescGround->ObjectFunc = CharSelectGroundRight;
				g_pGVLogin->ChrSelectInfo[i].pDescGround->bSkip = 1;
			}

			g_pGVLogin->dwCurCharIndex--;
			
			if(g_pGVLogin->dwCurCharIndex > 65536 )
				g_pGVLogin->dwCurCharIndex = 3;

			g_pGVLogin->dwFlag = CHAR_SELECT_STATUS_CHAR_ROTATING;	//캐릭터 돌리고 있는중 			
		}
		break;
	case BUTTON_START:
		{
			//캐릭터 만들고 있는중 이거나 돌리고 있는 중이면 
			if(g_pGVLogin->dwFlag == CHAR_SELECT_STATUS_CHAR_MAKING				||
				g_pGVLogin->dwFlag == CHAR_SELECT_STATUS_AWAITING_CREATE_RESULT ||
				g_pGVLogin->dwFlag == CHAR_SELECT_STATUS_CHAR_ROTATING			||
				g_pGVLogin->dwFlag == CHAR_SELECT_STATUS_STARTED)	return;			
			
			_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_CHARSELBTNCLICK, g_v3InterfaceSoundPos, FALSE);

			if(IsEmptyString(g_pGVLogin->ChrSelectInfo[ g_pGVLogin->dwCurCharIndex ].ReceivedChrInfo.szName))
				return;
						
			CTWS_CHARACTER_SELECT packet;
			packet.bCharacterIndex = (BYTE)g_pGVLogin->dwCurCharIndex;

			//	by minjin. 2004. 10. 25.
			SendMsgWithBlowfish((char*)&packet, packet.GetPacketSize(), SERVER_INDEX_AGENT);
			//g_pNet->SendMsg((char*)&packet, packet.GetPacketSize(), SERVER_INDEX_AGENT);
						
			g_pSprManager->GetSprite(SPR_LOADING_BACK)->ShowSprite(TRUE);
			g_pInputManager->Initialize();
			g_pGVLogin->dwFlag = CHAR_SELECT_STATUS_STARTED;
			g_helpLoadingSprite.Load(10000);
		}
		break;
	case BUTTON_EXIT:
		{
			if(g_bCharacterChk==TRUE)
				return;

			g_pNet->DisconnectServer(SERVER_INDEX_AGENT);

#ifdef __PACKET_ENCRYPTION
			// 로그인 서버와의 연결을 끊으므로.. 키를 다시 리셋한다.
			// modified by minjin. 2004. 11. 05.
			ResetEncryptionKey();
#endif

			if(g_bExecuteType==EXECUTE_TYPE_NETMARBLE)
			{
				g_bIsRunning = FALSE;
				return;
			}
			SetGameStatus(UPDATE_GAME_LOGIN);
		}
		break;
	case BUTTON_CLASS_WARRIOR:	OnChangeClass(CLASS_TYPE_WARRIOR);		break;
	case BUTTON_CLASS_PRIEST:	OnChangeClass(CLASS_TYPE_PRIEST);		break;
	case BUTTON_CLASS_HUNTER:	OnChangeClass(CLASS_TYPE_HUNTER);		break;
	case BUTTON_CLASS_WIZARD:	OnChangeClass(CLASS_TYPE_WIZARD);		break;		
	case BUTTON_CLASS_SUMMONER:	
		{
//#if !IS_CHINA_LOCALIZING()
	// 서머너는 결과적으로 중국만 막는다.. ( 2004. 08. 12. minjin )
			OnChangeClass(CLASS_TYPE_SUMMONER);	
//#endif
			break;		
		}
	}	
}
//==================================================================//
// 캐릭터 셀렉트 상태에서 오브젝트(캐릭터생성, 캐릭터회전) 처리.	//
//==================================================================//
void OnChangeClass(BYTE bClassIndex)
{
	WORD wSex		= HIWORD(g_pGVLogin->dwCurHeadIndexforCreate);
	WORD wHeadIndex = LOWORD(g_pGVLogin->dwCurHeadIndexforCreate);

	g_pExecutive->GXODetach(g_pGVLogin->EnumCharForCreate[g_pGVLogin->bCurClassIndexForCreate-1], g_pGVLogin->EnumCharHeadForCreate[wSex][wHeadIndex]);
	HideObject(g_pGVLogin->EnumCharForCreate[g_pGVLogin->bCurClassIndexForCreate-1]);	//이전 몸 모양 Hide 
	
	g_pGVLogin->bCurClassIndexForCreate = bClassIndex;

	HideObject(g_pGVLogin->EnumCharHeadForCreate[wSex][wHeadIndex]);	//이전 머리 모양 Hide 
	g_pExecutive->GXODetach(g_pGVLogin->EnumCharForCreate[g_pGVLogin->bCurClassIndexForCreate-1], g_pGVLogin->EnumCharHeadForCreate[wSex][wHeadIndex]);
	wSex = WORD((bClassIndex == CLASS_TYPE_HUNTER || bClassIndex == CLASS_TYPE_WIZARD) ? 0 : 1);
	wHeadIndex = 0;
	g_pGVLogin->dwCurHeadIndexforCreate = MAKELONG(wHeadIndex, wSex);

	ShowObject(g_pGVLogin->EnumCharForCreate[g_pGVLogin->bCurClassIndexForCreate-1]);	//새로운 머리 모양 Show
	g_pExecutive->GXOAttach(g_pGVLogin->EnumCharHeadForCreate[wSex][wHeadIndex], g_pGVLogin->EnumCharForCreate[g_pGVLogin->bCurClassIndexForCreate-1], "Bip01 Head");

	g_pInputManager->ClearInput(INPUT_BUFFER_CHARACTER_CLASS2);
	
	switch(bClassIndex)
	{
	case CLASS_TYPE_WARRIOR:	g_pInputManager->InsertCharacter(INPUT_BUFFER_CHARACTER_CLASS2, g_Message[ETC_MESSAGE7].szMessage, lstrlen(g_Message[ETC_MESSAGE7].szMessage));	break;	// MSG_ID : 7
	case CLASS_TYPE_PRIEST:		g_pInputManager->InsertCharacter(INPUT_BUFFER_CHARACTER_CLASS2, g_Message[ETC_MESSAGE9].szMessage, lstrlen(g_Message[ETC_MESSAGE9].szMessage));	break;	// 9 
	case CLASS_TYPE_SUMMONER:	g_pInputManager->InsertCharacter(INPUT_BUFFER_CHARACTER_CLASS2, g_Message[ETC_MESSAGE10].szMessage, lstrlen(g_Message[ETC_MESSAGE10].szMessage));	break;	// 10
	case CLASS_TYPE_HUNTER:		g_pInputManager->InsertCharacter(INPUT_BUFFER_CHARACTER_CLASS2, g_Message[ETC_MESSAGE11].szMessage, lstrlen(g_Message[ETC_MESSAGE11].szMessage));	break;	// 11
	case CLASS_TYPE_WIZARD:		g_pInputManager->InsertCharacter(INPUT_BUFFER_CHARACTER_CLASS2, g_Message[ETC_MESSAGE12].szMessage, lstrlen(g_Message[ETC_MESSAGE12].szMessage));	break;	// 12
	}	
}

void CharSelectRotateRight(GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag)
{	
	float fRad=0.f; 
	VECTOR3 vPos;	vPos.y = 0;
	RS_CharacterLogInfo* pChrInfo = (RS_CharacterLogInfo*)pData->pInfo;
	
	switch(pData->dwTempBuf[1])		//Rotate할때 현재 위치 
	{
	case 0:		fRad = DEG270 + (DEG01 * pData->dwTempBuf[2]);		break;
	case 1:		fRad = (DEG01 * pData->dwTempBuf[2]);				break;
	case 2:		fRad = DEG90 + (DEG01 * pData->dwTempBuf[2]);		break;
	case 3:		fRad = DEG180 + (DEG01 * pData->dwTempBuf[2]);		break;
	}
	
	vPos.x = float(0 + ( 200 * cos( fRad ) ));
	vPos.z = float(200 + ( 200 * sin( fRad ) ));
		
	pData->dwTempBuf[2] += 3;
	if(pData->dwTempBuf[2] >= 90)
	{
		pData->dwTempBuf[2] = 0;

		pData->dwTempBuf[1]++;		
		if(pData->dwTempBuf[1] >= 4)	pData->dwTempBuf[1] = 0;
	
		pData->ObjectFunc = NULL;
		
		switch(pData->dwTempBuf[1])
		{
		case 0:	vPos.x = 0;		vPos.z = 0;		break;
		case 1:	vPos.x = 200;	vPos.z = 200;	break;
		case 2:	vPos.x = 0;		vPos.z = 400;	break;
		case 3:	vPos.x = -200;	vPos.z = 200;	break;
		}
		
		if(pData->dwTempBuf[1] != 0)
		{
			if(g_pGVLogin->ChrSelectInfo[ g_pGVLogin->dwCurCharIndex ].hHandle == NULL && 
			   !__strcmp(pChrInfo->szName, g_pInputManager->GetInputBuffer(INPUT_BUFFER_CHARACTER_NAME2)) )
			{
				DisplayCharacterInfo(NULL);
				g_pSprManager->GetSprite(BUTTON_DELETE)->ShowSprite(FALSE);				
			}
		}
		else
		{
		//	VECTOR3 v3Pos = {0.f, };
			_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_CHARSELSPOT, g_v3InterfaceSoundPos, FALSE);
			DisplayCharacterInfo(pChrInfo);
			g_pSprManager->GetSprite(BUTTON_DELETE)->ShowSprite(TRUE);

			//선택된 캐릭터 
			ShowObject(g_pGVLogin->ChrSelectInfo[ pData->dwTempBuf[1] ].hSpotLightHandle);
			
			::SetAction( g_pGVLogin->ChrSelectInfo[ pData->dwTempBuf[0] ].hHandle, 
			    		WORD((g_pGVLogin->ChrSelectInfo[ pData->dwTempBuf[0] ].bByItemType) * 50 + MOTION_TYPE_ATTACK1_1), 0, ACTION_LOOP );

			g_pGVLogin->ChrSelectInfo[ pData->dwTempBuf[0] ].pDesc->ObjectFunc = CharSelectHighlightMotion;
			g_pGVLogin->ChrSelectInfo[ pData->dwTempBuf[0] ].pDesc->dwTempBuf[10] = 1;			
		}				
		GXSetPosition(handle, &vPos, FALSE);	
		return;
	}
	GXSetPosition(handle, &vPos, FALSE);	
}

void CharSelectRotateLeft(GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag)
{
	VECTOR3 vPos;	vPos.y = 0;
	RS_CharacterLogInfo* pChrInfo = (RS_CharacterLogInfo*)pData->pInfo;	

	float fRad = 0.f; 
	switch(pData->dwTempBuf[1])
	{
	case 0:		fRad = DEG270 - (DEG01 * pData->dwTempBuf[2]);		break;
	case 1:		fRad = DEG360 - (DEG01 * pData->dwTempBuf[2]);		break;
	case 2:		fRad = DEG90 - (DEG01 * pData->dwTempBuf[2]);		break;
	case 3:		fRad = DEG180 - (DEG01 * pData->dwTempBuf[2]);		break;
	}

	vPos.x = float(0 + ( 200 * cos( fRad ) ));
	vPos.z = float(200 + ( 200 * sin( fRad ) ));
		
	pData->dwTempBuf[2] += 3;
	if(pData->dwTempBuf[2] >= 90)
	{
		pData->dwTempBuf[2] = 0;

		pData->dwTempBuf[1]--;		
		if(pData->dwTempBuf[1] > 32767 )	pData->dwTempBuf[1] = 3;
		
		pData->ObjectFunc = NULL;
		
		switch(pData->dwTempBuf[1])
		{
		case 0:	vPos.x = 0;		vPos.z = 0;		break;
		case 1:	vPos.x = 200;	vPos.z = 200;	break;
		case 2:	vPos.x = 0;		vPos.z = 400;	break;
		case 3:	vPos.x = -200;	vPos.z = 200;	break;
		}

		//중앙 맨 앞자리가 아닐때 
		if(pData->dwTempBuf[1] != 0)
		{
			//맨 앞자리에 만들어진 캐릭터가 없을때 
			if(g_pGVLogin->ChrSelectInfo[ g_pGVLogin->dwCurCharIndex ].hHandle == NULL && 
			   !__strcmp(pChrInfo->szName, g_pInputManager->GetInputBuffer(INPUT_BUFFER_CHARACTER_NAME2)) )
			{
				DisplayCharacterInfo(NULL);
				g_pSprManager->GetSprite(BUTTON_DELETE)->ShowSprite(FALSE);
			}
		}
		else
		{
		//	VECTOR3		v3Pos = {0.f, };
			_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_CHARSELSPOT, g_v3InterfaceSoundPos, FALSE);
			DisplayCharacterInfo(pChrInfo);
			g_pSprManager->GetSprite(BUTTON_DELETE)->ShowSprite(TRUE);

			//선택된 캐릭터 
			ShowObject(g_pGVLogin->ChrSelectInfo[ pData->dwTempBuf[1] ].hSpotLightHandle);

			if(g_pGVLogin->ChrSelectInfo[ pData->dwTempBuf[0] ].hHandle)
			{
				::SetAction( g_pGVLogin->ChrSelectInfo[ pData->dwTempBuf[0] ].hHandle, 
			    		WORD((g_pGVLogin->ChrSelectInfo[ pData->dwTempBuf[0] ].bByItemType) * 50 + MOTION_TYPE_ATTACK1_1), 0, ACTION_LOOP );
				
				g_pGVLogin->ChrSelectInfo[ pData->dwTempBuf[0] ].pDesc->ObjectFunc = CharSelectHighlightMotion;
				g_pGVLogin->ChrSelectInfo[ pData->dwTempBuf[0] ].pDesc->dwTempBuf[10] = 1;
			}
		}
		GXSetPosition(handle, &vPos, FALSE);
		return;
	}
	GXSetPosition(handle, &vPos, FALSE);	
}

void CharSelectHighlightMotion(GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag)
{	
	if(bFrameFlag == FRAME_FLAG_FINISHED || bFrameFlag == FRAME_FLAG_CHANGE_NEXT )
	{
		pData->dwTempBuf[10]++;

		switch(pData->dwTempBuf[10])
		{
		case 2:
			{
				::SetAction( g_pGVLogin->ChrSelectInfo[ pData->dwTempBuf[0] ].hHandle, 
						   	WORD((g_pGVLogin->ChrSelectInfo[ pData->dwTempBuf[0] ].bByItemType) * 50 + MOTION_TYPE_ATTACK1_2), 0, ACTION_LOOP );
			}
			break;
		case 3:
			{
				::SetAction( g_pGVLogin->ChrSelectInfo[ pData->dwTempBuf[0] ].hHandle, 
						   	WORD((g_pGVLogin->ChrSelectInfo[ pData->dwTempBuf[0] ].bByItemType) * 50 + MOTION_TYPE_ATTACK1_3), 0, ACTION_LOOP );
			}
			break;
		case 4:
			{				
				::SetAction( g_pGVLogin->ChrSelectInfo[ pData->dwTempBuf[0] ].hHandle, 
						   	WORD((g_pGVLogin->ChrSelectInfo[ pData->dwTempBuf[0] ].bByItemType) * 50 + MOTION_TYPE_ATTACK1_1), 0, ACTION_LOOP );
				pData->dwTempBuf[10] = 1;	
			}
			break;
		}
	}	
}

void CharSelectGroundRight(GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag)
{	
	VECTOR3 vPos;	vPos.y = 0;

	float fRad=0.f; 
	switch(pData->dwTempBuf[1])		//Rotate할때 현재 위치 
	{
	case 0:		fRad = DEG270 + (DEG01 * pData->dwTempBuf[2]);		break;
	case 1:		fRad = (DEG01 * pData->dwTempBuf[2]);				break;
	case 2:		fRad = DEG90 + (DEG01 * pData->dwTempBuf[2]);		break;
	case 3:		fRad = DEG180 + (DEG01 * pData->dwTempBuf[2]);		break;
	}
	
	vPos.x = float(0 + ( 200 * cos( fRad ) ));
	vPos.z = float(200 + ( 200 * sin( fRad ) ));
		
	pData->dwTempBuf[2] += 3;
	
	if(pData->dwTempBuf[2] >= 90)
	{
		pData->dwTempBuf[2] = 0;

		pData->dwTempBuf[1]++;		
		
		if(pData->dwTempBuf[1] >= 4)
			pData->dwTempBuf[1] = 0;
	
		pData->ObjectFunc = NULL;
		
		switch(pData->dwTempBuf[1])
		{
		case 0:	vPos.x = 0;		vPos.z = 0;		break;
		case 1:	vPos.x = 200;	vPos.z = 200;	break;
		case 2:	vPos.x = 0;		vPos.z = 400;	break;
		case 3:	vPos.x = -200;	vPos.z = 200;	break;
		}

		//맨앞자리이고 생성된 캐릭터가 없으면 
		if(pData->dwTempBuf[1] == 0 && !g_pGVLogin->ChrSelectInfo[ g_pGVLogin->dwCurCharIndex ].hHandle)
			SPR(BUTTON_CREATE)->ShowSprite(TRUE);
		
		GXSetPosition(handle, &vPos, FALSE);
		g_pGVLogin->dwFlag = CHAR_SELECT_STATUS_NORMAL;	
		return;
	}
	GXSetPosition(handle, &vPos, FALSE);	
}

void CharSelectGroundLeft(GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag)
{
	VECTOR3 vPos;	vPos.y = 0;
	
	float fRad=0.f; 
	switch(pData->dwTempBuf[1])
	{
	case 0:		fRad = DEG270 - (DEG01 * pData->dwTempBuf[2]);		break;
	case 1:		fRad = DEG360 - (DEG01 * pData->dwTempBuf[2]);		break;
	case 2:		fRad = DEG90 - (DEG01 * pData->dwTempBuf[2]);		break;
	case 3:		fRad = DEG180 - (DEG01 * pData->dwTempBuf[2]);		break;
	}

	vPos.x = float(0 + ( 200 * cos( fRad ) ));
	vPos.z = float(200 + ( 200 * sin( fRad ) ));
		
	pData->dwTempBuf[2] += 3;

	if(pData->dwTempBuf[2] >= 90)
	{
		pData->dwTempBuf[2] = 0;

		pData->dwTempBuf[1]--;		
		if(pData->dwTempBuf[1] > 32767 )	pData->dwTempBuf[1] = 3;
		
		pData->ObjectFunc = NULL;
		
		switch(pData->dwTempBuf[1])
		{
		case 0:	vPos.x = 0;		vPos.z = 0;		break;
		case 1:	vPos.x = 200;	vPos.z = 200;	break;
		case 2:	vPos.x = 0;		vPos.z = 400;	break;
		case 3:	vPos.x = -200;	vPos.z = 200;	break;
		}
		
		// 맨앞자리이고 생성된 캐릭터가 없으면 
		if(pData->dwTempBuf[1] == 0 && !g_pGVLogin->ChrSelectInfo[ g_pGVLogin->dwCurCharIndex ].hHandle)
			SPR(BUTTON_CREATE)->ShowSprite(TRUE);		

		GXSetPosition(handle, &vPos, FALSE);
		g_pGVLogin->dwFlag = CHAR_SELECT_STATUS_NORMAL;	
		return;
	}
	GXSetPosition(handle, &vPos, FALSE);	
}

BOOL CheckCompleteRotate()
{	
	BYTE bRemainFunc = 0;
	for(int i=0; i<MAX_CHAR_SELECT_NUM; i++)
	{
		if(g_pGVLogin->ChrSelectInfo[i].hHandle)
		{
			if(g_pGVLogin->ChrSelectInfo[i].pDesc->ObjectFunc == CharSelectRotateRight ||
				g_pGVLogin->ChrSelectInfo[i].pDesc->ObjectFunc == CharSelectRotateLeft )
				bRemainFunc++;
		}
	}
	return !(bRemainFunc > 0);
}

void HideCreateWindow()
{	
	SPR(BUTTON_OK)->ShowSprite(FALSE);
	SPR(BUTTON_OK_OVER)->ShowSprite(FALSE);
	SPR(BUTTON_OK_DOWN)->ShowSprite(FALSE);

	SPR(BUTTON_CANCEL)->ShowSprite(FALSE);
	SPR(BUTTON_CANCEL_OVER)->ShowSprite(FALSE);
	SPR(BUTTON_CANCEL_DOWN)->ShowSprite(FALSE);

	SPR(BUTTON_MODIFY)->ShowSprite(FALSE);

	SPR(BUTTON_CLASS_WARRIOR)->ShowSprite(FALSE);
	SPR(BUTTON_CLASS_PRIEST)->ShowSprite(FALSE);
	SPR(BUTTON_CLASS_SUMMONER)->ShowSprite(FALSE);
	SPR(BUTTON_CLASS_HUNTER)->ShowSprite(FALSE);
	SPR(BUTTON_CLASS_WIZARD)->ShowSprite(FALSE);

	g_pInputManager->ClearInput(INPUT_BUFFER_CHARACTER_RACE2);
	g_pInputManager->ClearInput(INPUT_BUFFER_CHARACTER_CLASS2);
	g_pInputManager->ClearInput(INPUT_BUFFER_CHARACTER_NAME1);
	
	SPR(SPR_CHAR_SELECT_CREATE_WINDOW)->ShowSprite(FALSE);
}
//==================================================================//
// 캐릭터 셀렉트 상태에서 장착 아이템 처리.							//
//==================================================================//
void SetRHandChangeChr(CUser* pUser, DWORD dwId)
{
	int nIndex = dwId/ITEM_DISTRIBUTE;

	ItemDeleteObject(&pUser->m_hPlayerRHand.pHandle, &pUser->m_hPlayerRHand.pHandle);
	ObjDeleteDesc(&pUser->m_hPlayerRHand.pDesc);

	if(dwId)
	{		
		LP_ITEM_RESOURCE_EX lpItemResource = g_pItemResourceHash->GetData(dwId);

		if(lpItemResource)
		{
			if(lpItemResource->wModCount>0)
				ItemAttach(WORD(dwId), &pUser->m_hPlayerRHand.pHandle, pUser->m_hPlayerRHand.pDesc, &pUser->m_hPlayer.pHandle, ITEM_ATTACH_TYPE_RHAND, 0);
		}
	}
		
	if(!dwId)
	{
		pUser->m_byItemType = 0;		
		pUser->SetAction(MOTION_TYPE_VILLAGESTAND, 0, ACTION_LOOP);
	}			
	else
	{		
		pUser->m_byItemType = BYTE(nIndex+1);		
		pUser->SetAction(WORD(nIndex*50+MOTION_TYPE_VILLAGESTAND), 0, ACTION_LOOP);
	}	
}

void SetLHandChangeChr(CUser* pUser, DWORD dwId)
{
	ItemDeleteObject(&pUser->m_hPlayerLHand.pHandle, &pUser->m_hPlayerRHand.pHandle);
	ObjDeleteDesc(&pUser->m_hPlayerLHand.pDesc);

	if(dwId)
	{
		LP_ITEM_RESOURCE_EX lpItemResource = g_pItemResourceHash->GetData(dwId);
			
		if(lpItemResource)
		{
			if(lpItemResource->wModCount>0)
				ItemAttach(WORD(dwId), &pUser->m_hPlayerLHand.pHandle, pUser->m_hPlayerLHand.pDesc, &pUser->m_hPlayer.pHandle, ITEM_ATTACH_TYPE_LHAND, 0);
		}
	}	
}


//==================================================================//
// 기타 함수들.														//
//==================================================================//
BOOL CreateMainPlayer(WSTC_WORLD_USER_INFO* pInfo)
{	
	if(!pInfo)
		return FALSE;

	CHR_SELECT_INFO* pSelectChr = &g_pGVLogin->ChrSelectInfo[ g_pGVLogin->dwCurCharIndex ];		//선택한 캐릭터 포인터 

#ifdef __RANDOM_MEMORY_ALLOCATION
	DWORD dwRand = GetRandom(1024);
	char* pRandMemBlock = new char [dwRand*4];

	CMainUser* pTemp[16] = {0,};

	for(DWORD i = 0; i < 16; i++)
	{
		pTemp[i] = new CMainUser;
	}
	
	dwRand = GetRandom(16);

	for(i = 0; i < 16; i++)
	{
		if(i == dwRand)
		{
			g_pMainPlayer = pTemp[i];
		}
		else
		{
			delete pTemp[i];
		}
	}

	delete [] pRandMemBlock;

#else
	g_pMainPlayer = new CMainUser;
#endif // __RANDOM_MEMORY_ALLOCATION

	g_pMainPlayer->m_hPlayer.pHandle = pSelectChr->hHandle;				//g_pGVLogin->ChrSelectInfo[ g_pGVLogin->dwCurCharIndex ].hHandle;

	if(!g_pMainPlayer->m_hPlayer.pHandle)
		asm_int3();
	
	g_pMainPlayer->m_hPlayer.pDesc			= (LPObjectDesc)g_pExecutive->GetData(g_pMainPlayer->m_hPlayer.pHandle);
	g_pMainPlayer->m_hPlayer.pDesc->bType	= OBJECT_TYPE_MAINPLAYER;
	g_pMainPlayer->m_hPlayer.pDesc->pInfo	= g_pMainPlayer;
	g_pMainPlayer->m_hPlayerHead.pHandle	= pSelectChr->hHeadHandle;	//g_pGVLogin->ChrSelectInfo[ g_pGVLogin->dwCurCharIndex ].hHeadHandle;
	g_pMainPlayer->m_hPlayerHelmet.pHandle	= pSelectChr->hHelmetHandle;
	g_pMainPlayer->m_wClass					= pSelectChr->ReceivedChrInfo.wClass;
	g_pMainPlayer->m_dwRecentOutMap			= pInfo->dwRecentOutMap;
	g_pMainPlayer->m_dwRecentOutPos			= pInfo->dwRecentOutPos;
	g_pMainPlayer->m_wCriminalTime			= pInfo->wCriminalTime;
	g_pMainPlayer->m_dwMoney				= pInfo->dwMoney;
	g_pMainPlayer->m_byHelp[0]				= pInfo->byHelpTypeA;
	g_pMainPlayer->m_byHelp[1]				= pInfo->byHelpTypeB;				
	g_pMainPlayer->m_byHelp[2]				= pInfo->byHelpInterface;
	
	if(!g_pMainPlayer->m_dwRecentOutMap)	
		g_pMainPlayer->m_dwRecentOutMap = 1;
	
	if(!g_pMainPlayer->m_hPlayerHead.pHandle)
		asm_int3();
	
	g_pMainPlayer->m_dwUserIndex = pInfo->dwUserIndex;
	__lstrcpyn(g_pMainPlayer->m_szName, pSelectChr->ReceivedChrInfo.szName, MAX_CHARACTER_NAME_REAL_LENGTH); 
	
	g_pMainPlayer->m_dwDex			= pSelectChr->ReceivedChrInfo.dwDex;
	g_pMainPlayer->m_dwEgo			= pSelectChr->ReceivedChrInfo.dwEgo;
	g_pMainPlayer->m_dwExp			= pSelectChr->ReceivedChrInfo.dwExp;
	g_pMainPlayer->m_dwHonor		= pSelectChr->ReceivedChrInfo.dwHonor;
	g_pMainPlayer->m_dwInt			= pSelectChr->ReceivedChrInfo.dwInt;
	g_pMainPlayer->m_dwLevel		= pSelectChr->ReceivedChrInfo.dwLevel;
	g_pMainPlayer->m_dwLuck			= pSelectChr->ReceivedChrInfo.dwLuck;
	g_pMainPlayer->m_dwStr			= pSelectChr->ReceivedChrInfo.dwStr;
	g_pMainPlayer->m_dwVit			= pSelectChr->ReceivedChrInfo.dwVit;
	g_pMainPlayer->m_wGrade			= pSelectChr->ReceivedChrInfo.wGrade;
	g_pMainPlayer->m_bCurrnetHand	= pInfo->byCurrentHand;
	
	BYTE byLHand = 0;
	BYTE byRHand = 0;

	if(g_pMainPlayer->m_bCurrnetHand==1)
	{
		byLHand = EQUIP_TYPE_LHAND1;
		byRHand = EQUIP_TYPE_RHAND1;
	}
	else
	{
		byLHand = EQUIP_TYPE_LHAND2;
		byRHand = EQUIP_TYPE_RHAND2;
	}

	g_pMainPlayer->m_wClass = pInfo->wClass;
	g_pMainPlayer->m_wHead = pInfo->wHead;
	g_pMainPlayer->m_wHelmet = pInfo->Equip[ EQUIP_TYPE_HELMET ].m_wItemID;
	g_pMainPlayer->m_wHandL = pInfo->Equip[ byLHand ].m_wItemID;
	g_pMainPlayer->m_wHandR = pInfo->Equip[ byRHand ].m_wItemID;

	//-- 기타 필요한 이펙트들을 로드한다. --//
	g_pMainPlayer->m_hMoveStartEffect.pDesc = AllocObjDesc();
	g_pMainPlayer->m_hMoveStopEffect.pDesc	= AllocObjDesc();
	g_pMainPlayer->m_hPlayerShop.pDesc		= AllocObjDesc();
	
	g_pMainPlayer->m_hMoveStartEffect.pDesc->bType		= OBJECT_TYPE_EFFECT;
	g_pMainPlayer->m_hMoveStartEffect.pDesc->pInfo		= (void*)g_pMainPlayer;
	g_pMainPlayer->m_hMoveStartEffect.pDesc->ObjectFunc = NULL;
	g_pMainPlayer->m_hMoveStopEffect.pDesc->bType		= OBJECT_TYPE_EFFECT;
	g_pMainPlayer->m_hMoveStopEffect.pDesc->pInfo		= (void*)g_pMainPlayer;
	g_pMainPlayer->m_hMoveStopEffect.pDesc->ObjectFunc	= NULL;	
	g_pMainPlayer->m_hPlayerShop.pDesc->bType			= OBJECT_TYPE_EFFECT;
	g_pMainPlayer->m_hPlayerShop.pDesc->pInfo			= (void*)g_pMainPlayer;
	g_pMainPlayer->m_hPlayerShop.pDesc->ObjectFunc		= NULL;
	
	//0228
	memcpy(g_pMainPlayer->m_pEquip, pInfo->Equip, sizeof(pInfo->Equip));		//현재 장착도구들 Copy

	if(g_pMainPlayer->m_pEquip[byRHand].m_wItemID==0)
	{
		SetLHandChangeChr(g_pMainPlayer, g_pMainPlayer->m_pEquip[byLHand].m_wItemID);
		SetRHandChangeChr(g_pMainPlayer, g_pMainPlayer->m_pEquip[byRHand].m_wItemID);		
	}
	else
	{
//		LP_ITEM_TABLE pItemTable = g_pItemTableHash->GetData(g_pMainPlayer->m_pEquip[byRHand].m_wItemID);		
		CBaseItem* pItemTable = g_pItemTableHash->GetData(g_pMainPlayer->m_pEquip[byRHand].m_wItemID);		

		if(pItemTable)
		{
			if(pItemTable->BaseItem_Weapon.bModCount==2)			
				SetLHandChangeChr(g_pMainPlayer, g_pMainPlayer->m_pEquip[byRHand].m_wItemID);
			else		
				SetLHandChangeChr(g_pMainPlayer, g_pMainPlayer->m_pEquip[byLHand].m_wItemID);

			SetRHandChangeChr(g_pMainPlayer, g_pMainPlayer->m_pEquip[byRHand].m_wItemID);
		}
	}

	// 달릴때 시작 이펙트와 정지 이펙트.
	g_pMainPlayer->m_hMoveStartEffect.pHandle = 
		CreateHandleObject( g_pObjManager->GetFile( SKILL_DUST1 ), GXPlayerPROC, g_pMainPlayer->m_hMoveStartEffect.pDesc, 0 );

	g_pMainPlayer->m_hMoveStopEffect.pHandle = 
		CreateHandleObject( g_pObjManager->GetFile( SKILL_DUST2 ), GXPlayerPROC, g_pMainPlayer->m_hMoveStopEffect.pDesc, 0 );

	g_pMainPlayer->m_hPlayerShop.pHandle = 
		CreateHandleObject(GetFile("playershop_sign.chr", DATA_TYPE_ITEM), GXPlayerPROC, g_pMainPlayer->m_hPlayerShop.pDesc, 0 );
	
	//Self Portal Effect Create
	if(!g_pMainPlayer->m_hSelfPortalEffect.pHandle)
	{
		g_pMainPlayer->m_hSelfPortalEffect.pDesc = AllocObjDesc();
		memset(g_pMainPlayer->m_hSelfPortalEffect.pDesc, 0, sizeof(OBJECT_DESC));
		g_pMainPlayer->m_hSelfPortalEffect.pDesc->bType = OBJECT_TYPE_EFFECT;
		g_pMainPlayer->m_hSelfPortalEffect.pDesc->pInfo = (void*)g_pMainPlayer;
		g_pMainPlayer->m_hSelfPortalEffect.pHandle = CreateHandleObject( FILE(MOD_SELF_PORTAL_EFFECT) , GXPlayerPROC, 
														g_pMainPlayer->m_hSelfPortalEffect.pDesc, GXOBJECT_CREATE_TYPE_EFFECT);
		HideObject(g_pMainPlayer->m_hSelfPortalEffect.pHandle);
	}
	//--
	
	//우선 MainPlayer의 World Start Position을 셋팅하고 y 좌표는 추후 map 생성후 하이트 필드의 위치에 맞춰 셋팅한다.
	VECTOR3 vPos;
	vPos.x = pInfo->fStartPos_X;
	vPos.y = 0.0f;
	vPos.z = pInfo->fStartPos_Z;
	GXSetPosition(g_pMainPlayer->m_hPlayer.pHandle, &vPos, FALSE);
	
	HideObject( g_pMainPlayer->m_hMoveStartEffect.pHandle );
	HideObject( g_pMainPlayer->m_hMoveStopEffect.pHandle );
	HideObject( g_pMainPlayer->m_hPlayerShop.pHandle );

	InitChrInfo( GetFile( "pm01000.cdt", DATA_TYPE_CDT ), CLASS_TYPE_WARRIOR-1, OBJECT_TYPE_PLAYER );
	InitChrInfo( GetFile( "pm02000.cdt", DATA_TYPE_CDT ), CLASS_TYPE_PRIEST-1, OBJECT_TYPE_PLAYER );
	InitChrInfo( GetFile( "pm03000.cdt", DATA_TYPE_CDT ), CLASS_TYPE_SUMMONER-1, OBJECT_TYPE_PLAYER );
	InitChrInfo( GetFile( "pm04000.cdt", DATA_TYPE_CDT ), CLASS_TYPE_HUNTER-1, OBJECT_TYPE_PLAYER );
	InitChrInfo( GetFile( "pm05000.cdt", DATA_TYPE_CDT ), CLASS_TYPE_WIZARD-1, OBJECT_TYPE_PLAYER );
	
	g_pMainPlayer->RegistItemNativeInfo();
	// sung-han 2005-02-15 길드전 매칭 관련 초기화
	CGroupWnd::GetInstance()->InitGuildWarMatching();
	// 파티 강화 시스템 추가 (정우창 2005.02.18 파티매칭 초기화 )
	CGroupWnd::GetInstance()->m_byPMatchStatus = 0;
	CGroupWnd::GetInstance()->m_nPMatchResult = -1;  
			
	return TRUE;	
}

BOOL SendCharacterDeleteMsg()
{
	CTWS_DELETE_CHARACTER packet;
	
	memset(packet.szCharacterName, 0, sizeof(packet.szCharacterName));
	packet.bChrNum = (BYTE)g_pGVLogin->dwCurCharIndex;

	int nLen = lstrlen(g_pGVLogin->ChrSelectInfo[ g_pGVLogin->dwCurCharIndex ].ReceivedChrInfo.szName);

	if(nLen==0)
		return FALSE;

	if(g_pInputManager->GetInputBufferLength(INPUT_BUFFER_CHARACTER_JUMIN) == 0 || g_pInputManager->GetInputBufferLength(INPUT_BUFFER_CHARACTER_JUMIN) < 7)	
		return FALSE;
	
	__lstrcpyn(packet.szJumin, g_pInputManager->GetInputBuffer(INPUT_BUFFER_CHARACTER_JUMIN), 7);
	__lstrcpyn(packet.szCharacterName, g_pGVLogin->ChrSelectInfo[ g_pGVLogin->dwCurCharIndex ].ReceivedChrInfo.szName, MAX_CHARACTER_NAME_REAL_LENGTH);
	
	//	by minjin. 2004. 10. 25.
	SendMsgWithBlowfish((char*)&packet, packet.GetPacketSize(), SERVER_INDEX_AGENT);
	//g_pNet->SendMsg((char*)&packet, packet.GetPacketSize(), SERVER_INDEX_AGENT);

	g_pGVLogin->dwFlag = CHAR_SELECT_STATUS_AWAITING_DELETE_RESULT;
	g_pInputManager->ClearInput(INPUT_BUFFER_CHARACTER_JUMIN);
	return TRUE;
}

void UpdateGameCharSelect()
{
}

void OnTimerEventCharSelect(DWORD dwTimerIndex)
{
	switch(dwTimerIndex)
	{
	case 1:
		{
			g_pInputManager->BlinkInputCaret();			
		}
		break;
	}	
}

void DisplayCharacterInfo(RS_CharacterLogInfo* pInfo)
{	
	if(pInfo)
	{
		for(int i=2; i<16; i++)
			g_pInputManager->ClearInput(i);

		char szBuf[ 0xff ] = {0,};
		g_pInputManager->InsertCharacter(INPUT_BUFFER_CHARACTER_NAME2, pInfo->szName ,lstrlen(pInfo->szName) );	//이름
		g_pInputManager->InsertCharacter(INPUT_BUFFER_CHARACTER_RACE1, g_Message[ETC_MESSAGE6].szMessage , lstrlen(g_Message[ETC_MESSAGE6].szMessage) );	//종족	// MSG_ID : 6 ; HUMAN
		
		//Class
		char* szClass[6] = {"",g_Message[ETC_MESSAGE7].szMessage, g_Message[ETC_MESSAGE9].szMessage, g_Message[ETC_MESSAGE10].szMessage, g_Message[ETC_MESSAGE11].szMessage, g_Message[ETC_MESSAGE12].szMessage	};	// MSG_ID : 파이터(7), 프리스트(9), 서머너(10), 레인저(11), 소서리스(12)
		g_pInputManager->InsertCharacter(INPUT_BUFFER_CHARACTER_CLASS1, szClass[ pInfo->wClass ], lstrlen(szClass[ pInfo->wClass ]) );
		
		//Exp
		wsprintf(szBuf, "%ld", pInfo->dwExp);
		g_pInputManager->InsertCharacter(INPUT_BUFFER_CHARACTER_EXP, szBuf, lstrlen(szBuf) );
		
		//Level
		wsprintf(szBuf, "%d", pInfo->dwLevel);
		g_pInputManager->InsertCharacter(INPUT_BUFFER_CHARACTER_LEVEL, szBuf, lstrlen(szBuf) );
		
		//Str
		wsprintf(szBuf, "%d", pInfo->dwStr);
		g_pInputManager->InsertCharacter(INPUT_BUFFER_CHARACTER_STR, szBuf, lstrlen(szBuf) );
		
		//Vit
		wsprintf(szBuf, "%ld", pInfo->dwVit);
		g_pInputManager->InsertCharacter(INPUT_BUFFER_CHARACTER_VIT, szBuf, lstrlen(szBuf) );

		//Dex
		wsprintf(szBuf, "%ld", pInfo->dwDex);
		g_pInputManager->InsertCharacter(INPUT_BUFFER_CHARACTER_DEX, szBuf, lstrlen(szBuf) );

		//Int
		wsprintf(szBuf, "%ld", pInfo->dwInt);
		g_pInputManager->InsertCharacter(INPUT_BUFFER_CHARACTER_INT, szBuf, lstrlen(szBuf) );

		//Ego
		wsprintf(szBuf, "%ld", pInfo->dwEgo);
		g_pInputManager->InsertCharacter(INPUT_BUFFER_CHARACTER_EGO, szBuf, lstrlen(szBuf) );

		SPR(SPR_CHAR_SELECT_CREATE_WINDOW)->ShowSprite(FALSE);
		SPR(BUTTON_OK)->ShowSprite(FALSE);
		SPR(BUTTON_CANCEL)->ShowSprite(FALSE);
		SPR(BUTTON_MODIFY)->ShowSprite(FALSE);
		SPR(BUTTON_CREATE)->ShowSprite(FALSE);
	}
	else
	{
		for(int i=2; i<16; i++)
			g_pInputManager->ClearInput(i);

		SPR(BUTTON_CREATE)->ShowSprite(TRUE);
	}
}
//==================================================================//
//																	//
//==================================================================//