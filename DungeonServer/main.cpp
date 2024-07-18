#include <crtdbg.h>
#include "stdafx.h"
#include "OwnServer.h"
#include "GameSystem.h"
#include "GameSystem_for_yak.h"
#include "ClassMemoryPool.h"
#include "DungeonTable.h"
#include "MonsterTable.h"
#include "AcceptTable.h"
#include "Effect.h"
#include "MonsterManager.h"
#include "User.h"
#include "cgdi.h"
#include "ServerFunction.h"
#include "resource.h"
#include "Dungeon.h"
#include "ItemMoveManager.h"
#include "ItemUsedManager.h"
#include "ItemAttrLayer.h"
#include "DBProc.h"
#include "EventDungeonManager.h"
#include "../BaseLibrary/Utility/Debugging/ExceptionReport.h" // MiniDump : jaewon 050120


CNTNetWork	*				g_pNet							= NULL;
IOleDBThread*				g_pDb							= NULL;
IOleDBThread*				g_pLogDb						= NULL;

HWND						g_hDlg							= NULL;
HANDLE						g_hIn							= NULL;
HANDLE						g_hOut							= NULL;
FILE*						g_fpLog							= NULL;
BYTE						GAME_DB;
BYTE						LOG_DB;
BOOL						g_bIsServerRunning				= TRUE;
BOOL						g_bMonitor						= FALSE;
DWORD						g_dwTickCount					= 0;
DWORD						g_pGuilRankId					= 0;
DWORD						g_dwPlayerShopKey				= 1;
DWORD						g_dwSpeedHackCheckPeriod		= 0;
DWORD						g_dwSpeedHackCheckError			= 0;
DWORD						g_dwScreenBufferLastRow			= 0;
int							g_ConsoleLogLevel				= 2;
int							g_FileLogLevel					= 7;
MACHINENAME					g_MachineName;
CRITICAL_SECTION			g_criLog;

COwnServer*					g_pThis;								
CDungeonTable*				g_pDungeonTable					= NULL;		
CMonsterTable*				g_pMonsterTable					= NULL;		
CAcceptTable*				g_pAcceptTable					= NULL;		
EffectLayer*				g_pEffectLayer					= NULL;		
BASEITEM_BOTARY*			g_pBaseBotary					= NULL;
BASEITEM_PRESENT_BOX*		g_pBasePresentBox				= NULL;
Character_Propensity		g_pCharacter_Propensity[MAX_CHARACTER_PROPENSITY];
cGDI*						g_pGDI							= NULL;
CPTable						g_pCPTable[MAX_CP_TABLE];
CItemMoveManager			g_ItemMoveManager;
CItemUsedManager			g_ItemUsedManager;
BASE_CLASS_INFO				g_pBaseClassInfo[MAX_CLASS+1];
BASE_CLASS_INFO				g_pBaseRaceInfo[MAX_RACE+1];
ITEM_OPTION_SERVER*			g_pItemOptionTable				= NULL;
CItemAttrLayer*				g_pItemAttrLayer				= NULL;
ITEM_UPGRADE*				g_pItemUpgradeTable[ MAX_ITEM_UPGRADE_LEVEL + 1 ];
NPC_TABLE					g_NPCTable[MAX_NPC_NUM_PER_VILLAGE];

DUNGEONPRODUCTIONITEMMINMAX g_DungeonProductionItemMinMax[MAX_DUNGEON_PROPERTY];

CClassMemoryPool*			g_pMonsterClassPool				= NULL;
CClassMemoryPool*			g_pTraderMonsterClassPool		= NULL;
CClassMemoryPool*			g_pUserClassPool				= NULL;

LPBASEMONSTER_HASH			g_pBaseMonsterHash				= NULL;
LPUSER_HASH					g_pUserHash						= NULL;
LPSET_ITEM_INFO_HASH		g_pSetItemInfoHash				= NULL;
LPBASEITEM_HASH				g_pBaseItemHash					= NULL;
LPITEM_MAKING_INFO_HASH		g_pItemMakingInfoHash			= NULL;
LPITEM_STORENATIVE_HASH		g_pItemStoreNativeHash			= NULL;
LPITEM_EVENT_HASH			g_pItemEventHash				= NULL;
LPUSER_SETITEM_HASH			g_pUserSetItemHash				= NULL;
LPINVALID_LOG_ITEM_HASH		g_pInvalidLogItemHash			= NULL;

CShopTileHash*				g_pShopTileHash					= NULL;
CItemShopHash*				g_pItemShopHash					= NULL;
CItemTradeHash*				g_pItemTradeHash				= NULL;
CItemStoreHash*				g_pItemStoreHash				= NULL;
CPlayerShopLogHash*			g_pPlayerShopLogHash			= NULL;
CGuildListHash*				g_pGuildListHash				= NULL;
CUserHash*					g_pUserChkHash					= NULL;

STMPOOL_HANDLE				g_pNodePool						= NULL;		
STMPOOL_HANDLE				g_pItemPool						= NULL;		
STMPOOL_HANDLE				g_pCreateItemPool				= NULL;
STMPOOL_HANDLE				g_pPortalBufferPool				= NULL;
STMPOOL_HANDLE				g_pVirtualLinkItemNativePool	= NULL;
STMPOOL_HANDLE				g_pPlayerShopLogPool			= NULL;
STMPOOL_HANDLE				g_pItemTradePool				= NULL;
STMPOOL_HANDLE				g_pItemStorePool				= NULL;
STMPOOL_HANDLE				g_pItemShopPool					= NULL;
STMPOOL_HANDLE				g_pShopTilePool					= NULL;
STMPOOL_HANDLE				g_pGuildListPool				= NULL;

#ifdef DEV_MODE
	BYTE					g_bEnableChatLog				= 1;
	BYTE					g_bEnableSpeedHackLog			= 0;
	BYTE					g_bEnableItemLog				= 1;
#else
	BYTE					g_bEnableChatLog				= 1;
	BYTE					g_bEnableSpeedHackLog			= 1;
	BYTE					g_bEnableItemLog				= 1;
#endif


BOOL CtrlHandler(DWORD fdwCtrlType);
BOOL APIENTRY DlgProc (HWND hdlg, UINT msg, WPARAM wParam, LPARAM lParam);

/*
void SetDBInfo()
{
	if( IS_ABLE_SERVICE_TYPE(ST_DEVELOP) )
	{
		g_DBInfo.Set(DT_GAME_DB, DBIT_CATALOG, "Corum");
		g_DBInfo.Set(DT_GAME_DB, DBIT_ID, "potato");
		g_DBInfo.Set(DT_GAME_DB, DBIT_PW, "5378abc");

		g_DBInfo.Set(DT_TOTAL_DB, DBIT_CATALOG, "Corum_Total_DB");
		g_DBInfo.Set(DT_TOTAL_DB, DBIT_ID, "potato");
		g_DBInfo.Set(DT_TOTAL_DB, DBIT_PW, "5378abc");

		g_DBInfo.Set(DT_MEMBER_DB, DBIT_CATALOG, "member");
		g_DBInfo.Set(DT_MEMBER_DB, DBIT_ID, "potato");
		g_DBInfo.Set(DT_MEMBER_DB, DBIT_PW, "5378abc");

		g_DBInfo.Set(DT_LOG_DB, DBIT_CATALOG, "Log_Corum_");
		g_DBInfo.Set(DT_LOG_DB, DBIT_ID, "potato");
		g_DBInfo.Set(DT_LOG_DB, DBIT_PW, "5378abc");
		return;
	} // if( IS_ABLE_SERVICE_TYPE(ST_DEVELOP) )

	switch( MY_NATION_CODE )
	{
	case NC_JAPAN:
		{
			g_DBInfo.Set(DT_GAME_DB, DBIT_CATALOG, "Corum");
			g_DBInfo.Set(DT_GAME_DB, DBIT_ID, "potato");
			g_DBInfo.Set(DT_GAME_DB, DBIT_PW, "ajtwlssja");

			g_DBInfo.Set(DT_TOTAL_DB, DBIT_CATALOG, "Total_DB");
			g_DBInfo.Set(DT_TOTAL_DB, DBIT_ID, "potato");
			g_DBInfo.Set(DT_TOTAL_DB, DBIT_PW, "ajtwlssja");

			g_DBInfo.Set(DT_MEMBER_DB, DBIT_CATALOG, "CORUM");
			g_DBInfo.Set(DT_MEMBER_DB, DBIT_ID, "potato");
			g_DBInfo.Set(DT_MEMBER_DB, DBIT_PW, "ajtwlssja");

			g_DBInfo.Set(DT_LOG_DB, DBIT_CATALOG, "Log_Corum_");
			g_DBInfo.Set(DT_LOG_DB, DBIT_ID, "potato");
			g_DBInfo.Set(DT_LOG_DB, DBIT_PW, "ajtwlssja");
		}break;
	case NC_CHINA:
		{
			g_DBInfo.Set(DT_GAME_DB, DBIT_CATALOG, "Corum");
			g_DBInfo.Set(DT_GAME_DB, DBIT_ID, "potato");
			g_DBInfo.Set(DT_GAME_DB, DBIT_PW, "ajtwlssja");

			g_DBInfo.Set(DT_TOTAL_DB, DBIT_CATALOG, "Total_DB");
			g_DBInfo.Set(DT_TOTAL_DB, DBIT_ID, "potato");
			g_DBInfo.Set(DT_TOTAL_DB, DBIT_PW, "ajtwlssja");

			g_DBInfo.Set(DT_MEMBER_DB, DBIT_CATALOG, "CORUM");
			g_DBInfo.Set(DT_MEMBER_DB, DBIT_ID, "potato");
			g_DBInfo.Set(DT_MEMBER_DB, DBIT_PW, "ajtwlssja");

			g_DBInfo.Set(DT_LOG_DB, DBIT_CATALOG, "Log_Corum_");
			g_DBInfo.Set(DT_LOG_DB, DBIT_ID, "potato");
			g_DBInfo.Set(DT_LOG_DB, DBIT_PW, "ajtwlssja");		
		}break;
	case NC_TAIWAN:
		{
			g_DBInfo.Set(DT_GAME_DB, DBIT_CATALOG, "Corum");
			g_DBInfo.Set(DT_GAME_DB, DBIT_ID, "potato");
			g_DBInfo.Set(DT_GAME_DB, DBIT_PW, "8x0ozs1xos");

			g_DBInfo.Set(DT_TOTAL_DB, DBIT_CATALOG, "Total_DB");
			g_DBInfo.Set(DT_TOTAL_DB, DBIT_ID, "potato");
			g_DBInfo.Set(DT_TOTAL_DB, DBIT_PW, "8x0ozs1xos");

			g_DBInfo.Set(DT_MEMBER_DB, DBIT_CATALOG, "CORUM");
			g_DBInfo.Set(DT_MEMBER_DB, DBIT_ID, "potato");
			g_DBInfo.Set(DT_MEMBER_DB, DBIT_PW, "8x0ozs1xos");

			g_DBInfo.Set(DT_LOG_DB, DBIT_CATALOG, "Log_Corum_");
			g_DBInfo.Set(DT_LOG_DB, DBIT_ID, "potato");
			g_DBInfo.Set(DT_LOG_DB, DBIT_PW, "8x0ozs1xos");
		}break;
	case NC_KOREA:
		{
			g_DBInfo.Set(DT_GAME_DB, DBIT_CATALOG, "Corum");
			g_DBInfo.Set(DT_GAME_DB, DBIT_ID, "GameSrv");
			g_DBInfo.Set(DT_GAME_DB, DBIT_PW, "rnrwk3rotkaaksrncjsvkfqordnjs!");

			g_DBInfo.Set(DT_TOTAL_DB, DBIT_CATALOG, "Total_DB");
			g_DBInfo.Set(DT_TOTAL_DB, DBIT_ID, "GameSrv");
			g_DBInfo.Set(DT_TOTAL_DB, DBIT_PW, "rnrwk3rotkaaksrncjsvkfqordnjs!");

			g_DBInfo.Set(DT_MEMBER_DB, DBIT_CATALOG, "Member");
			g_DBInfo.Set(DT_MEMBER_DB, DBIT_ID, "GameSrv");
			g_DBInfo.Set(DT_MEMBER_DB, DBIT_PW, "rnrwk3rotkaaksrncjsvkfqordnjs!");

			g_DBInfo.Set(DT_LOG_DB, DBIT_CATALOG, "Log_Corum_");
			g_DBInfo.Set(DT_LOG_DB, DBIT_ID, "GameSrv");
			g_DBInfo.Set(DT_LOG_DB, DBIT_PW, "rnrwk3rotkaaksrncjsvkfqordnjs!");
		}break;
	default:
		{
		}break;
	} // switch( MY_NATION_CODE )
}
*/

int main(int argc, char* argv[])
{
#ifdef _DEBUG
	_CrtSetReportFile( _CRT_WARN, _CRTDBG_FILE_STDERR );
//	_CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_FILE   );
//	_CrtSetReportFile( _CRT_WARN, _CRTDBG_FILE_STDOUT );
#endif
	// 발생하는 예외를 처리하도록 한다.
	// 예외 발생 시, 모든 유저의 정보를 저장하도록 한다.
	// 가디언은 글쎄...?
//	SET_GLOBAL_EXCEPTION_FILTER();// 엑셉션 필터 제거 : 050111 hwoarang
//	SetFuncForException(UpdateAllUserDB);



	// MiniDump : jaewon 050120
	g_CExceptionReport.SetProgramName();

	HWND hCombo;
	srand( timeGetTime() );

	AllocConsole();

	g_hIn	= GetStdHandle(STD_INPUT_HANDLE);
	g_hOut	= GetStdHandle(STD_OUTPUT_HANDLE);	

	DWORD dwMode = 0;
	GetConsoleMode(g_hIn, &dwMode);
	dwMode &= ~ENABLE_MOUSE_INPUT;
	SetConsoleMode(g_hIn, dwMode);

	INPUT_RECORD irBuffer;
	memset(&irBuffer, 0, sizeof(INPUT_RECORD));

	InitLog();
	
	BOOL bSuccess = SetConsoleCtrlHandler( (PHANDLER_ROUTINE) CtrlHandler, TRUE);             
	if(!bSuccess)	Log(LOG_NORMAL, "SetConsoleCtrlHandler Function Failed!");

	CoInitialize(NULL);	
	
	LoadSpeedHackCheckParameter();

	g_pThis = new COwnServer;

#if defined JAPAN_LOCALIZING
	g_pThis->InitializeDecodeCFG(".\\Server.cfg");
#elif defined CHINA_LOCALIZING
	g_pThis->InitializeDecodeCFG(".\\Server.cfg");
#elif defined TAIWAN_LOCALIZING
	g_pThis->InitializeDecodeCFG(".\\Server.cfg");
#elif defined ENGLISH_LOCALIZING
	g_pThis->InitializeDecodeCFG(".\\Server.cfg");
#else
	g_pThis->Initialize(".\\Server.ini", ".\\DungeonConfig.ini");
#endif	
	
	if(GKO_EXIST == CreateGlobalKernelObject(g_pThis->GetPortForUser()))
	{
		// 동일 서버의 중복실행 방지!
		Log(LOG_JUST_DISPLAY, "Same Server Already Running. Check out another terminal session!");
		MessageBox(NULL, "Same Server Already Running, Now Server Down!", "Not Unique Process", MB_OK|MB_ICONINFORMATION);

		delete g_pThis;
		FreeLog();
		FreeConsole();
		return 0;
	}	
	
	char szTitle[ 0xff ]={0,};
	if(g_pThis->GetServerType() == SERVER_TYPE_OCCUPIED_DUNGEON )
		wsprintf(szTitle, "[%d] CorumOnline Occupied Dungeon Server (%s)", g_pThis->GetPortForUser(), g_pThis->GetIPForUser());
	else if(g_pThis->GetServerType() == SERVER_TYPE_NON_OCCUPIED_DUNGEON )
		wsprintf(szTitle, "[%d] CorumOnline Non-Occupied Dungeon Server (%s)", g_pThis->GetPortForUser(), g_pThis->GetIPForUser());
	else if(g_pThis->GetServerType() == SERVER_TYPE_VILLAGE)
		wsprintf(szTitle, "[%d] CorumOnline Village Server (%s)", g_pThis->GetPortForUser(), g_pThis->GetIPForUser());
	else if(g_pThis->GetServerType() == SERVER_TYPE_PATHWAY_DUNGEON)
		wsprintf(szTitle, "[%d] CorumOnline Pathway Dungeon Server (%s)", g_pThis->GetPortForUser(), g_pThis->GetIPForUser());

	SetConsoleTitle(szTitle);

	GetComputerName(g_MachineName.szBuffer,&g_MachineName.dwSize);
	g_MachineName.wPort			= g_pThis->GetPortForUser();
	g_MachineName.bServerType	= g_pThis->GetServerType();
	wsprintf(g_MachineName.szIpForUser,g_pThis->GetIPForUser());	
	
	g_pDungeonTable		= new CDungeonTable(MAX_DUNGEON_PER_SERVER);
	g_pMonsterTable		= new CMonsterTable(10000);
	g_pAcceptTable		= new CAcceptTable(1024);	
	g_pItemAttrLayer	= new CItemAttrLayer;
	g_pBaseBotary		= new BASEITEM_BOTARY[MAX_ITEMBOTARY];
	g_pBasePresentBox   = new BASEITEM_PRESENT_BOX[MAX_ITEMPRESENT + 1];	

	for(int i = 0; i < MAX_ITEM_UPGRADE_LEVEL + 1; ++i)
	{
		g_pItemUpgradeTable[i] = new ITEM_UPGRADE[MAX_ITEM_KIND1_CODE];
	}

	g_pItemOptionTable = new ITEM_OPTION_SERVER[ITEM_NUM_PER_OPTION];

	InitNetwork();
	InitOleDBThread();
	InitMemoryPool();
	
#if defined JAPAN_LOCALIZING
	ConnectToDBServerDecodeCFG();
#elif defined CHINA_LOCALIZING
	ConnectToDBServerDecodeCFG();
#elif defined TAIWAN_LOCALIZING
	ConnectToDBServerDecodeCFG();
#elif defined ENGLISH_LOCALIZING
	ConnectToDBServerDecodeCFG();
#else
	if(!ConnectToDBServer())
	{
		return false;
	}
#endif	

	g_pThis->RequestDungeonInfo();

	if(!g_pThis->ResetItemSerialNumber())
	{
		goto lb_Exit;
	}

	InitHashTables();

	LoadDungeonServerData();	

	g_pThis->PreLoadDungeon();	


	g_pEffectLayer = new EffectLayer;
	g_pEffectLayer->Init();

	QueryEventDungeonProperties();
	
	g_pThis->ToggleUserAccept();	

	g_pNet->ResumeTimer(0);
	g_pNet->ResumeTimer(1);
		
	ConnectToWorldServer();		
	ListenForUserside();

	g_hDlg = CreateDialog( GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG1),NULL,DlgProc );
	g_pGDI = new cGDI( g_hDlg, 300, 300 );
	
	DWORD dwResult;
	{	
		hCombo = GetDlgItem(g_hDlg, IDC_COMBO_DUNGEONLAYER);
		POSITION_ pos = g_pDungeonTable->m_pDungeonList->GetHeadPosition();
		dwResult = 0;

		while(pos)
		{
			CDungeon* pDungeon = (CDungeon*)g_pDungeonTable->m_pDungeonList->GetNext(pos);

			for(int c = 0; c < pDungeon->GetTotalLayer(); ++c)
			{
				char szTemp[0xff]={0,};
				wsprintf(szTemp, "%s(%d)", pDungeon->GetDungeonName(), c);
				SendMessage(hCombo, CB_INSERTSTRING, dwResult, (LPARAM)szTemp);
				SendMessage(hCombo, CB_SETITEMDATA, dwResult++, pDungeon->GetID()*100+c);
			}
		}
	}

	ReportToCMListener(0, &g_MachineName);	

lb_Console:	
	while ( g_bIsServerRunning )
	{
		ReadConsoleInput(g_hIn,&irBuffer,1,&dwResult);
	
		if (irBuffer.EventType == KEY_EVENT)
		{
			if (irBuffer.Event.KeyEvent.bKeyDown)
			{
				if (irBuffer.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE)
				{
					char szCaption[0xff]={0,};
					wsprintf(szCaption,"DungeonServer Terminate Question [%d]",g_pThis->GetPortForUser());

					if( MessageBox( NULL, "Are you sure to DESTROY (Novice)Dungeon Server?", szCaption, MB_YESNO ) == IDYES )
					{
						SetEvent( g_pThis->m_hKeyEvent[4] );
					}
				}

				if(irBuffer.Event.KeyEvent.wVirtualKeyCode == VK_F1)
				{
					KeyHelp();
				}
				
				if (irBuffer.Event.KeyEvent.wVirtualKeyCode == VK_F2)
				{
					Log(LOG_JUST_DISPLAY, "@ Total %d/%d (User/Server) Connected.", g_pNet->GetConnectedUserNum(), g_pNet->GetConnectedServerNum());
				}

				if (irBuffer.Event.KeyEvent.wVirtualKeyCode == VK_F3)
				{
					// Show Current Dungeon Status
					SetEvent(g_pThis->m_hKeyEvent[0]);	
				}

				if (irBuffer.Event.KeyEvent.wVirtualKeyCode == VK_F4)
				{
					if (g_pThis->m_GLOBAL_EXP_GAIN_DOUBLE)
						Log(LOG_JUST_DISPLAY, "Now, Double Exp Enabled...");
					else
						Log(LOG_JUST_DISPLAY, "Now, Double Exp Disabled... ");
				}

				if (irBuffer.Event.KeyEvent.wVirtualKeyCode == VK_F6)
				{
					if( MessageBox( NULL, "Are you sure to REFRESH Dungeon Server Data?", "Refresh Data Question", MB_YESNO ) == IDYES )
					{
						// Refresh Dungeonserver data
						SetEvent(g_pThis->m_hKeyEvent[1]);	
					}
				}

				if (irBuffer.Event.KeyEvent.wVirtualKeyCode == VK_F11)
				{
					// Toggle Item Log
					SetEvent(g_pThis->m_hKeyEvent[2]);	
				}

				if (irBuffer.Event.KeyEvent.wVirtualKeyCode == VK_F12)
				{
					// Toggle Speedhack Log
					SetEvent(g_pThis->m_hKeyEvent[3]);	
				}
/*
				if (irBuffer.Event.KeyEvent.wVirtualKeyCode == VK_SPACE )
				{
					g_bMonitor = TRUE;
					if( g_bMonitor )
						ShowWindow(g_hDlg, SW_SHOW);

					SetFocus(g_hDlg);
					goto lb_Monitor;
				}				
				*/
			}
		}
	}
	
	if(!g_bIsServerRunning)	goto lb_Exit;

//lb_Monitor:
	MSG msg;
	while(GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		
		if( !g_bMonitor )
			goto lb_Console;
	}

lb_Exit:

	g_pNet->BreakMainThread();

	delete	g_pGDI;
	g_pGDI = NULL;

	if(g_hDlg)	DestroyWindow(g_hDlg);

	if (g_pNet)
	{
		g_pNet->Release();
		g_pNet = NULL;
	}

	if(g_pLogDb)
	{
		g_pLogDb->Release();
		g_pLogDb = NULL;
	}

	if(g_pDb)
	{
		g_pDb->Release();
		g_pDb = NULL;
	}

	


	FreeLog();
	CoFreeUnusedLibraries();
	CoUninitialize();
	FreeConsole();

	DeleteGlobalKernelObject();
	
	ReportToCMListener(4, &g_MachineName);

	
// jaewon 050120
#ifdef _DEBUG
	int nRet = _CrtDumpMemoryLeaks();
#endif

	return 0;
}


BOOL CtrlHandler(DWORD fdwCtrlType) 
{ 
    switch (fdwCtrlType) 
    { 
        // Handle the CTRL+C signal. 
        case CTRL_C_EVENT:    
			return TRUE; 
 
        // CTRL+CLOSE: confirm that the user wants to exit. 
        case CTRL_CLOSE_EVENT: 
			  SetEvent( g_pThis->m_hKeyEvent[4] );
		     return FALSE; 
	 
        // Pass other signals to the next handler. 
        case CTRL_BREAK_EVENT: 
        case CTRL_LOGOFF_EVENT: 
        case CTRL_SHUTDOWN_EVENT: 
		default:       return FALSE; 
    } 
} 


BOOL APIENTRY DlgProc (HWND hdlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) 
	{
		case WM_RBUTTONDOWN:
		{
			g_bMonitor = FALSE;
			ShowWindow(g_hDlg, SW_HIDE);
		}
		break;

		case WM_CHAR:
		{
			int		nVirtKey = ( int )wParam;	// virtual-key code 

			switch(nVirtKey)
			{
				case VK_ESCAPE:
				{
					g_bMonitor = FALSE;
					ShowWindow(g_hDlg, SW_HIDE);
				}
				break;
			}
		}
		break;

		case WM_INITDIALOG:	break;
		case WM_ACTIVATE:	break;

		case WM_LBUTTONDOWN:
		{
			PostMessage(hdlg, WM_NCLBUTTONDOWN, HTCAPTION, lParam);
		}
		break;
	}
	
	return 0;
}
