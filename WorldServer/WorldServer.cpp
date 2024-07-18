#include "stdafx.h"
#include "ServerFunction.h"
#include "OwnServer.h"
#include "AcceptTable.h"
#include "WorldUserTable.h"
#include "DungeonTable.h"
#include "ConnectionDistributionTable.h"
#include "SchoolGuildDungeon.h"
#include "Guild.h"
#include "Party.h"
#include "Messenger.h"
#include "NTOS.h"


#include "RivalGUildWar.h"

#define CONSOLE_TITLE	"World Server - [Corum Online]"


#pragma comment(lib, "SS3DGFunc.lib")
#pragma comment(lib, "CommonServer.lib")


#ifdef DOUBLE_DB_CONNECTION	
DWORD				GAME_DB1 = 0;
DWORD				GAME_DB2 = 0;
#endif	
BYTE				GAME_DB		= 0;
BYTE				TOTAL_DB	= 0;
BYTE				MEMBER_DB	= 0;
// 대만은 귓말을 로그로 남긴다.
// 때문에 로그 디비가 필요하다.
BYTE				LOG_DB		= 0;

CNTNetWork	 *		g_pNet		= NULL;
IOleDBThread *		g_pDb		= NULL;
IOleDBThread *		g_pLogDb = NULL;
MACHINENAME			g_MachineName;

BOOL g_bEnableChatLog	= 0;

BOOL CtrlHandler(DWORD fdwCtrlType) 
{ 
    switch (fdwCtrlType) 
    { 
        // Handle the CTRL+C signal. 
        case CTRL_C_EVENT:    
			return TRUE; 
 
        // CTRL+CLOSE: confirm that the user wants to exit. 
        case CTRL_CLOSE_EVENT: 
			 //TerminateServer();	
			 //ExitProcess(0);
			SetEvent(g_pThis->m_hKeyEvent[6]);		//ShutDown
		     return FALSE; 
	 
        // Pass other signals to the next handler. 
        case CTRL_BREAK_EVENT: 
        case CTRL_LOGOFF_EVENT: 
        case CTRL_SHUTDOWN_EVENT: 
		default:       return FALSE; 
    } 
} 


void MainLoop()
{
	DWORD			dwResult=0;
	INPUT_RECORD	irBuffer;
	memset(&irBuffer, 0, sizeof(INPUT_RECORD));

	while ( g_bIsServerRunning )
	{
		GetConsoleKeyInput(&irBuffer, &dwResult);
	
		if (irBuffer.EventType == KEY_EVENT)
		{
			if (irBuffer.Event.KeyEvent.bKeyDown)
			{
				if (irBuffer.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE)
				{
					if( MessageBox( NULL, "Are you sure to DESTROY WorldMap Server?", "WorldServer Terminate Question", MB_YESNO ) == IDYES )
					{
						SetEvent(g_pThis->m_hKeyEvent[6]);		//ShutDown
					}
				}
				
				if(irBuffer.Event.KeyEvent.wVirtualKeyCode == VK_F1)
				{
					SetEvent(g_pThis->m_hKeyEvent[5]);
				}
				
				if (irBuffer.Event.KeyEvent.wVirtualKeyCode == VK_F3)
				{
					SetEvent(g_pThis->m_hKeyEvent[0]);	//Show Current All Server Status
				}

				if (irBuffer.Event.KeyEvent.wVirtualKeyCode == VK_F5)
				{
					SetEvent(g_pThis->m_hKeyEvent[3]);
				}

				if (irBuffer.Event.KeyEvent.wVirtualKeyCode == VK_F6)
				{
					SetEvent(g_pThis->m_hKeyEvent[4]);
				}

				if (irBuffer.Event.KeyEvent.wVirtualKeyCode == VK_F7)
				{
					if(g_pThis->ToggleUserAccept())			
						Log(LOG_JUST_DISPLAY, "@ User Accept Allowed!");
					else
						Log(LOG_JUST_DISPLAY, "@ User Accept Denied!");
				}

				if (irBuffer.Event.KeyEvent.wVirtualKeyCode == VK_F8)
				{
					if( MessageBox( NULL, "Are you sure to REFRESH All Other Dungeon Server?", "Refresh Data Question", MB_YESNO ) == IDYES )
					{
						SetEvent(g_pThis->m_hKeyEvent[2]);		//Refresh All dungeonserver Data!
					}
				}

				if (irBuffer.Event.KeyEvent.wVirtualKeyCode == VK_F9)
				{
					SetEvent(g_pThis->m_hKeyEvent[7]);		//Enable Double EXP
					Log(LOG_NORMAL, "Double Exp Enabled!");
				}

				if (irBuffer.Event.KeyEvent.wVirtualKeyCode == VK_F10)
				{
					SetEvent(g_pThis->m_hKeyEvent[8]);		//Disable Double Exp
					Log(LOG_NORMAL, "Double Exp Disabled!");
				}
				
				if (irBuffer.Event.KeyEvent.wVirtualKeyCode == VK_F12)
				{
					if( MessageBox( NULL, "Are you sure to DESTROY All Other Server?", "All Server Terminate Question", MB_YESNO ) == IDYES )
					{
						SetEvent(g_pThis->m_hKeyEvent[1]);	//Terminate All Other Server
					}
				}
				
				if (irBuffer.Event.KeyEvent.wVirtualKeyCode == 'A')
				{
					if( MessageBox( NULL, "Are you sure to Magic Item Drop Rate Increase", "Question", MB_YESNO ) == IDYES )
					{
						SetEvent(g_pThis->m_hKeyEvent[9]);	
					}
				}

				if (irBuffer.Event.KeyEvent.wVirtualKeyCode == 'Z')
				{
					if( MessageBox( NULL, "Are you sure to Magic Item Drop Rate Decrease", "Question", MB_YESNO ) == IDYES )
					{
						SetEvent(g_pThis->m_hKeyEvent[10]);	
					}
				}

				if (irBuffer.Event.KeyEvent.wVirtualKeyCode == 'S')
				{
					if( MessageBox( NULL, "Are you sure to Exp Increase", "Question", MB_YESNO ) == IDYES )
					{
						SetEvent(g_pThis->m_hKeyEvent[11]);	
					}
				}

				if (irBuffer.Event.KeyEvent.wVirtualKeyCode == 'X')
				{
					if( MessageBox( NULL, "Are you sure to Exp Decrease", "Question", MB_YESNO ) == IDYES )
					{
						SetEvent(g_pThis->m_hKeyEvent[12]);	
					}
				}

				// Just BONUS Exp for Party!!!		added by minjin.	2004. 12. 06.
				if (irBuffer.Event.KeyEvent.wVirtualKeyCode == 'D')
				{
					if( MessageBox( NULL, "Are you sure to Party Bonus Exp Increase", "Question", MB_YESNO ) == IDYES )
					{
						SetEvent(g_pThis->m_hKeyEvent[13]);	
					}
				}

				// Just BONUS Exp for Party!!!		added by minjin.	2004. 12. 06.
				if (irBuffer.Event.KeyEvent.wVirtualKeyCode == 'C')
				{
					if( MessageBox( NULL, "Are you sure to Party Bonus Exp Decrease", "Question", MB_YESNO ) == IDYES )
					{
						SetEvent(g_pThis->m_hKeyEvent[14]);	
					}
				}
				
				if (irBuffer.Event.KeyEvent.wVirtualKeyCode == 'N')
				{
					Log(LOG_NORMAL, "Load Notice message.");

					LoadNoticeMessage();

					Log(LOG_NORMAL, "Notice Message Loading complete!");
				}				
			}
		}
	}
}

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

		g_DBInfo.Set(DT_LOG_DB, DBIT_CATALOG, "Log");
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
		
		}break;
	case NC_TAIWAN:
		{
			g_DBInfo.Set(DT_GAME_DB, DBIT_CATALOG, "Corum");
			g_DBInfo.Set(DT_GAME_DB, DBIT_ID, "potato");
			g_DBInfo.Set(DT_GAME_DB, DBIT_PW, "8x0ozs1xos");

			g_DBInfo.Set(DT_TOTAL_DB, DBIT_CATALOG, "Total_DB");
			g_DBInfo.Set(DT_TOTAL_DB, DBIT_ID, "potato");
			g_DBInfo.Set(DT_TOTAL_DB, DBIT_PW, "8x0ozs1xos");

			g_DBInfo.Set(DT_MEMBER_DB, DBIT_CATALOG, "Member");
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
			g_DBInfo.Set(DT_GAME_DB, DBIT_PW, "vhrwnQkdqhdxn!");

			g_DBInfo.Set(DT_TOTAL_DB, DBIT_CATALOG, "Total_DB");
			g_DBInfo.Set(DT_TOTAL_DB, DBIT_ID, "GameSrv");
			g_DBInfo.Set(DT_TOTAL_DB, DBIT_PW, "vhrwnQkdqhdxn!");

			g_DBInfo.Set(DT_MEMBER_DB, DBIT_CATALOG, "Member");
			g_DBInfo.Set(DT_MEMBER_DB, DBIT_ID, "GameSrv");
			g_DBInfo.Set(DT_MEMBER_DB, DBIT_PW, "vhrwnQkdqhdxn!");

			g_DBInfo.Set(DT_LOG_DB, DBIT_CATALOG, "Total_DB");
			g_DBInfo.Set(DT_LOG_DB, DBIT_ID, "GameSrv");
			g_DBInfo.Set(DT_LOG_DB, DBIT_PW, "vhrwnQkdqhdxn!");
		}break;
	default:
		{
		}break;
	} // switch( MY_NATION_CODE )
}

char* g_szDefaultGuildRank[4] = {0,};

void SetLocalData()
{
	if( IS_ABLE_NATION(NC_TAIWAN) )
	{
		g_bEnableChatLog = 1;
	}
	else
	{
		g_bEnableChatLog = 0;
	}

	switch( MY_NATION_CODE )
	{
	case NC_JAPAN:
		{
			g_szDefaultGuildRank[0] = "?긚??";
			g_szDefaultGuildRank[1] = "븲?긚??";
			g_szDefaultGuildRank[2] = "맫됵덒";
			g_szDefaultGuildRank[3] = "덇붗됵덒";
		}break;
	case NC_CHINA:
		{
			g_szDefaultGuildRank[0] = "뚠낀";
			g_szDefaultGuildRank[1] = "릴뚠낀";
			g_szDefaultGuildRank[2] = "攣삔逃";
			g_szDefaultGuildRank[3] = "寧겹삔逃";
		}break;
	case NC_TAIWAN:
		{
			g_szDefaultGuildRank[0] = "??";
			g_szDefaultGuildRank[1] = "걱??";
			g_szDefaultGuildRank[2] = "?─??";
			g_szDefaultGuildRank[3] = "????";
		}break;
	case NC_USA:
		{
			g_szDefaultGuildRank[0] = "Master";
			g_szDefaultGuildRank[1] = "ViceMaster";
			g_szDefaultGuildRank[2] = "Member1";
			g_szDefaultGuildRank[3] = "Member2";
		}break;
	case NC_KOREA:
		{
			g_szDefaultGuildRank[0] = "마스터";
			g_szDefaultGuildRank[1] = "부마스터";
			g_szDefaultGuildRank[2] = "정회원";
			g_szDefaultGuildRank[3] = "일반회원";
		}break;
	}
}

int main(int argc, char* argv[])
{
	if(!g_Local.LoadFromINI("./Local.ini"))// Local Info 셋팅 : 050102 hwoarang
	{
		assert(NULL && "local.ini not exist");
		return 0;
	} // if(!g_Local.LoadFromINI("./Local.ini"))// Local Info 셋팅 : 050102 hwoarang

	g_GuildWarLog.Init( OUTPUT_JUST_FILE, "./GuildWarLog", "Result.txt" );//050221 : hwoarang 

	SetLocalData();
	SetDBInfo();// DB 계정 정보 셋팅 : 050103 hwoarang

	srand((unsigned int)timeGetTime());
	GetLocalTime(&g_CurDate);	//서버 날짜 참조용 

	AllocConsole();

	InitLog();

	SetConsoleTitle(CONSOLE_TITLE);
	
	BOOL bSuccess = SetConsoleCtrlHandler( (PHANDLER_ROUTINE) CtrlHandler, TRUE);             
	if(!bSuccess)	Log(LOG_NORMAL, "SetConsoleCtrlHandler Function Failed!");

	g_pThis = new COwnServer;

	switch( MY_NATION_CODE )
	{
	case NC_KOREA:
		{
			g_pThis->Initialize(".\\Server.ini");
		}break;
	default:
		{
			g_pThis->InitializeDecodeCFG(".\\Server.cfg");
		}break;
	}

	if(GKO_EXIST == CreateGlobalKernelObject(g_pThis->GetPortForUser()))
	{
		// 동일 서버의 중복실행 방지!
		Log(LOG_JUST_DISPLAY, "Same Server is Already Running. Check out another terminal session!");
		MessageBox(NULL, "Same Server is Already Running, Now Server Down!", "Not Unique Process", MB_OK|MB_ICONINFORMATION);

		delete g_pThis;
		FreeLog();
		FreeConsole();
		return 0;
	}
		 
	char szTitle[ 0xff ]={0,};
	wsprintf(szTitle,"[%d] CorumOnline World Server (%s)",g_pThis->GetPortForUser(),g_pThis->GetIPForUser());
	SetConsoleTitle(szTitle);

	g_pAcceptTable			= new CAcceptTable(512,2048);
	g_pUserTable			= new CWorldUserTable(1024,4096);	
	g_pServerTable			= new CServerTable(5000);
	g_pDungeonTable			= new CDungeonTable(2000);	
	g_pEventDungeonManager	= new CEventDungeonManager;
	g_pNtos					= new CNetworkTimeObserverServer;

	g_pNtos->Initialize();
	g_pNtos->SetMaxOverTick( g_pThis->GetNTOSOverTick() );		
	g_pNtos->SetInterval( g_pThis->GetNTOSInterval() );
	g_pNtos->Begin();	
	
#ifdef CHECK_KEEP_ALIVE	
	g_pCDTable = new CConnectionDistributionTable(20,500,2);	
#endif
	
	Log(LOG_NORMAL, "########### CorumOnline - World Server Started! ###########");
		
	CoInitialize(NULL);		
	
	InitNetwork();		
	InitOleDBThread();	
	if( !ConnectToDBServer() )
	{
		return 0;
	}
	QueryAllServer();	
	QueryWorldmapFormation();	
	
	if( IS_ABLE_NATION( NC_JAPAN) )
	{
		//일본판을 위한 비점령 던전의 학교 길드 이름 부여!
		InitSchoolGuildDungeon();
	}

	if(g_pDb->OpenRecord( "CZP_QUERY_BASECLASSINFO", g_pUserTable->GetBaseClassInfo(1), MAX_CLASS, GAME_DB ) < 0)
	{
		Log(LOG_IMPORTANT, "Fail to query BaseClassInfo!");
		goto lb_Exit;
	}

	// party, guild를 위한 pool과 hash 생성 초기화.
	CreatePoolAndHash();

	// Group Info //
	if(!InitGroupInformation())
	{
		goto lb_Exit;
	}

	// Guild Info //
	if(!InitGuildInformations())
	{
		goto lb_Exit;
	}
	
	// Dungeon List //
	if(!InitDungeonList())
	{
		goto lb_Exit;
	}

	g_pDb->ExecuteSQL("CZP_MESSENGER_INIT", (BYTE)GAME_DB);

	// 길드레더정보관련 체크
//	g_pDb->ExecuteSQL("SP_CHECK_GUILDINFO_UPDATED_050405", (BYTE)GAME_DB);


	// 이벤트 던젼 '속성'들을 DB로부터 불러온다.
	g_pEventDungeonManager->QueryEventDungeonProperties();	

	// 공지 메시지를 DB로부터 읽어온다.
	LoadNoticeMessage();
	LoadServerMessage();

	g_pThis->ToggleUserAccept();	

	GetComputerName(g_MachineName.szBuffer,&g_MachineName.dwSize);
	g_MachineName.wPort			= g_pThis->GetPortForUser();
	g_MachineName.bServerType	= g_pThis->GetServerType();
	wsprintf(g_MachineName.szIpForUser,g_pThis->GetIPForUser());
	
	ReportToCMListener(0, &g_MachineName);

	MainLoop();

lb_Exit:

	//	deathrain - Add 2004. 7. 14
	if( g_pNtos)
	{
		g_pNtos->End();
		delete g_pNtos;
		g_pNtos = NULL;
	}

	Log(LOG_NORMAL, "Start! - Release BaseNetwork!");
	if (g_pNet)
	{
		g_pNet->Release();
		g_pNet = NULL;
	}
	Log(LOG_NORMAL, "End! - Release BaseNetwork!");

	Log(LOG_NORMAL, "Start! - Release DB Component!");

	if(g_pDb)
	{
		g_pDb->Release();
		g_pDb = NULL;
	}

	if(g_pLogDb)
	{
		g_pLogDb->Release();
		g_pLogDb = NULL;
	}

	Log(LOG_NORMAL, "End! - Release DB Component!");

	Log(LOG_NORMAL, "Start! - CoUninitialize!");

	FreeLog();
	CoFreeUnusedLibraries();
	CoUninitialize();	

	FreeConsole();
	DeleteGlobalKernelObject();

	ReportToCMListener(4, &g_MachineName);

	return 0;
}


