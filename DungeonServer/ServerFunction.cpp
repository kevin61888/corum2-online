#include "stdafx.h"
#include "ServerFunction.h"
#include "GameSystem.h"
#include "GameSystem_for_yak.h"
#include "OwnServer.h"
#include "DungeonTable.h"
#include "Linkedlist.h"
#include "Dungeon.h"
#include "DUNGEON_DATA_EX.h"
#include "recvmsg.h"
#include "MonsterManager.h"
#include "user.h"
#include "MonsterTable.h"
#include "Monster.h"
#include "TraderMonster.h"
#include "AcceptTable.h"
#include "../CommonServer/CommonHeader.h"
#include "ItemAttrLayer.h"
#include "DungeonLayer.h"
#include "cGDI.h"
#include "resource.h"
#include "Map.h"
#include "Section.h"
#include "ClassMemoryPool.h"
#include "ItemMove.h"
#include "ItemNative.h"
#include "DBProc.h"
#include "MsgProc_for_jjw6263.h"
#include "GameSystemNative.h"
#include "MsgProc.h"
//#include "STLsupport.h"


//========================================================
// 던전 서버의 전반적인 처리. 0.2초마다.
//========================================================
void __stdcall ProcessDungeon(DWORD dwVal)
{
	SRAND();

	if(!g_pDungeonTable)	return;

	POSITION_ DungeonPos = g_pDungeonTable->m_pDungeonList->GetHeadPosition();

	while( DungeonPos )
	{
		CDungeon* pDungeon = (CDungeon*)g_pDungeonTable->m_pDungeonList->GetNext( DungeonPos );	

		// 적어도 한던젼 마다는 다른 틱을 설정하자.
		g_dwTickCount = timeGetTime();	

		// 유저 업데이트!!!
		POSITION_ pos = pDungeon->GetDungeonUserList()->GetHeadPosition();	
		
		while( pos )
		{
			CUser* pUser = (CUser*)pDungeon->GetDungeonUserList()->GetNext(pos);
			pUser->Update();
		}

		// 이벤트 던전 처리
		if( DUNGEON_TYPE_EVENT == pDungeon->GetDungeonDataEx()->GetDungeonType() )
		{
			if ( pDungeon->GetEventClearTick() )
			{
				if ( g_dwTickCount >= pDungeon->GetEventClearTick() + EVENT_DUNGEON_RESET_DELAY )
				{
					pDungeon->BanAllUser();
					pDungeon->SetEventClearTick(0);
				}
			}
		}

		// 던젼 층의 처리
		for(int i = 0; i < pDungeon->GetTotalLayer(); i++ )
		{
			pDungeon->GetDungeonLayer(BYTE(i))->Process();
		}
		
		RemoveUserProcess(pDungeon);
	}
}


void __stdcall UpdateMonitor(DWORD dwVal)	// 일단 임시로 만드는것이다. 다음에 계속 쓸것이면 기능업..
{
	if( !g_hDlg )	return;
	if( !g_pGDI )	return;
	if( !g_bMonitor ) return;

	// 여기서 부터 코딩.
	g_pGDI->_clearSurface( 0x0 );
	
	HWND hCombo		= GetDlgItem(g_hDlg, IDC_COMBO_DUNGEONLAYER);
	int nIndex		= SendMessage(hCombo, CB_GETCURSEL, 0, 0);
	DWORD dwDungeon = SendMessage(hCombo, CB_GETITEMDATA, nIndex, 0);
	WORD wIndex		= WORD(dwDungeon % 100);
	dwDungeon		= dwDungeon / 100;

	if( g_pDungeonTable->GetDungeonInfo( (WORD)dwDungeon ) == NULL ) return;

	CMap* pMap = g_pDungeonTable->GetDungeonInfo( (WORD)dwDungeon )->GetDungeonLayer((BYTE)wIndex)->GetMap();

	DWORD	x=0, z=0;
	WORD	color = 0;
	WORD	wHeight	= WORD(pMap->m_dwTileNumHeight*2+2);	// 상하 반전을 위해서.

	for( z = 0; z<pMap->m_dwTileNumHeight; z++ )
	{
		for( x = 0; x<pMap->m_dwTileNumWidth; x++ )
		{
			if( ( pMap->GetMap(x, z)->wAttr.uOccupied ) == 0 )
				color = 0x0;
			else if( ( pMap->GetMap(x, z)->wAttr.uOccupied ) == TILE_OCCUPIED_DONT_MOVE ) 
				color = 0x1ce7;

			g_pGDI->DrawFillBox( x*2, wHeight-(z*2), x*2+2, wHeight-(z*2+2), color );
		}
	}
	
	POSITION_	pos;
	CUser*		pUser;
	CPcList*	pPcList = g_pDungeonTable->GetDungeonInfo( (WORD)dwDungeon )->GetDungeonLayer((BYTE)wIndex)->m_pPcList;
	
	pos = pPcList->GetHeadPosition();
	while( pos )
	{
		pUser = (CUser*)pPcList->GetNext( pos );
		g_pGDI->DrawFillBox( INT( pUser->GetCurPosition()->x / TILE_WIDTH ) * 2, INT(wHeight-( pUser->GetCurPosition()->y / TILE_HEIGHT * 2)), 
			INT( pUser->GetCurPosition()->x / TILE_WIDTH ) * 2 + 2, INT(wHeight-( pUser->GetCurPosition()->y / TILE_HEIGHT * 2 + 2)), 0xff00 );
	}
	
	// Monster
	CMonster*		pMonster;
	CMonsterList*	pMonsterList = g_pDungeonTable->GetDungeonInfo( (WORD)dwDungeon )->GetDungeonLayer((BYTE)wIndex)->m_pMonsterList;

	pos = pMonsterList->GetHeadPosition();
	while( pos )
	{
		pMonster = (CMonster*)pMonsterList->GetNext( pos );
		g_pGDI->DrawFillBox( INT( pMonster->GetCurPosition()->x / TILE_WIDTH ) * 2, INT(wHeight-( pMonster->GetCurPosition()->y / TILE_HEIGHT * 2)), 
			INT( pMonster->GetCurPosition()->x / TILE_WIDTH ) * 2 + 2, INT(wHeight-( pMonster->GetCurPosition()->y / TILE_HEIGHT * 2 + 2)), 0x07E0 );
	}

	// 활성화 섹션을 그린다.
	CSection*	pSection;
	CActiveSectionList*	pActiveList = g_pDungeonTable->GetDungeonInfo( (WORD)dwDungeon )->GetDungeonLayer((BYTE)wIndex)->m_pActiveSectionList;
	pos = pActiveList->GetHeadPosition();
	while( pos )
	{
		pSection = (CSection*)pActiveList->GetNext( pos );
		g_pGDI->DrawBox( pSection->x1*2, wHeight-(pSection->y1*2), pSection->x2 * 2+2, wHeight-( pSection->y2 * 2+2 ), 0xffff );
	}
		
	g_pGDI->Blt( 0, 0, 0 );
}

void __stdcall ShowAllDungeonStatus(DWORD dwVal)
{
	g_pDungeonTable->ShowAllDungeonStatus();
	Log(LOG_JUST_DISPLAY, "MonsterCount (%d)", g_pMonsterTable->GetCount());
}

void __stdcall ReconnectWorldServer(DWORD dwVal)
{
	Log(LOG_NORMAL, "Try to reconnect WorldServer! (IP:%s, Port:%d)", g_pThis->GetIPForWorldServer(), g_pThis->GetPortForWorldServer());
	ConnectToWorldServer();
}

void __stdcall DungeonEnteredCount(DWORD dwVal)
{
	return;

	char szQuery[1024]={0,};
	
	POSITION_ pos = g_pDungeonTable->m_pDungeonList->GetHeadPosition();
	CDungeon* pDungeon = NULL;
	
	while (pos)
	{
		pDungeon = (CDungeon*)g_pDungeonTable->m_pDungeonList->GetNext(pos);

		if (NULL == pDungeon)
		{
			continue;
		}

		if (NULL != pDungeon->GetDungeonDataEx())
		{
			memset(szQuery, 0, 1024);
			wsprintf(szQuery, "Insert into Dungeon_Entered_Count ([ID], [Name], UserCount, ServerSetNo) values(%d, '%s', %d, %d)"
				, pDungeon->GetDungeonDataEx()->m_dwID
				, pDungeon->GetDungeonDataEx()->m_szDungeonName
				, pDungeon->GetDungeonTotalUserCount()
				, g_pThis->GetServerSetNum());
			
			if(g_bEnableItemLog)
				g_pLogDb->THExecuteSQL(szQuery, FALSE, 0, 0, LOG_DB);
		}		
	}	
}

void __stdcall ToggleItemLog(DWORD dwVal)
{
	if(g_bEnableItemLog)
	{
		g_bEnableItemLog = 0;
		Log(LOG_NORMAL, "Itemlog Disabled!");
	}
	else
	{
		g_bEnableItemLog = 1;
		Log(LOG_NORMAL, "Itemlog Enabled!");
	}
}

void __stdcall ToggleSpeedHackLog(DWORD dwVal)
{
	if(g_bEnableSpeedHackLog)
	{
		g_bEnableSpeedHackLog = 0;
		Log(LOG_NORMAL, "Speedhacklog Disabled!");
	}
	else
	{
		g_bEnableSpeedHackLog = 1;
		Log(LOG_NORMAL, "Speedhacklog Enabled!");
	}
}

void __stdcall RefreshDungeonServerData(DWORD dwVal)
{
	Log(LOG_NORMAL, "Start reload dungeonserver data!");
	if(ReLoadDungeonServerData() == FALSE)
	{
		Log(LOG_NORMAL, "Fail to reload dungeonserver data!");
		return;
	}
	Log(LOG_NORMAL, "End reload dungeonserver data!");
}


void __stdcall PkDecreaseCountCheck(DWORD dwVal)
{
	struct tm when;
	time_t now;
	time(&now);
	when = *localtime(&now);
	
	CUser* pUser = 0;
	int nDecPKCount=0;

	if(!g_pThis) return;
	
	POSITION_ pos = g_pThis->m_pPKUserCheckList->GetHeadPosition();
	POSITION_ del;

	while(pos)
	{
		del = pos;
		pUser = (CUser*)g_pThis->m_pPKUserCheckList->GetNext(pos);

		if ((pUser->m_sPKDescInfo.m_bPKCount|pUser->m_sPKDescInfo.m_bPKRepeatCount) == 0)
		{
			// pk 없는놈들은 지워라.
			g_pThis->m_pPKUserCheckList->RemoveAt(del);
			pUser->m_sPKDescInfo.m_pPKList	 = NULL;
			continue;
		}

		nDecPKCount = (int)difftime(now, pUser->m_sPKDescInfo.m_dwPKTime)/PK_DEC_TIME;

		if (nDecPKCount)
		{
			// pk count 줄여준다.
			pUser->SetExchangePKCount(-nDecPKCount, now);
		}

		nDecPKCount = (int)difftime(now, pUser->m_sPKDescInfo.m_dwPKRepeatTime)/PKREPEAT_DEC_TIME;
		if (nDecPKCount)
		{
			// pk repeat count 줄여준다.
			pUser->SetExchangePKRepeatCount(-nDecPKCount, now);
		}	
	}
}

void __stdcall ShutDownServer(DWORD dwVal)
{
	TerminateServer();	
}


void __stdcall GetDBResult(DWORD dwVal)
{
	g_pDb->GetDBResult();
}

void __stdcall GetLogDBResult(DWORD dwVal)
{
	g_pLogDb->GetDBResult();
}


void LoadSpeedHackCheckParameter()
{
	g_dwSpeedHackCheckPeriod	= GetPrivateProfileInt("SPEEDHACK", "CheckPeriod", PERIOD_SPEED_HACK_CHECK, "./SpeedHack.ini" );
	g_dwSpeedHackCheckError		= GetPrivateProfileInt("SPEEDHACK", "CheckError", PERIOD_SPEED_HACK_ERROR, "./SpeedHack.ini" );
}


void KeyHelp()
{
	Log( LOG_JUST_DISPLAY, "													               " );
	Log( LOG_JUST_DISPLAY, "-------------------------------------------------------------------" );
	Log( LOG_JUST_DISPLAY, "                 World Server - Function Key Help	               " );
	Log( LOG_JUST_DISPLAY, "-------------------------------------------------------------------" );
	Log( LOG_JUST_DISPLAY, "[F1] : Funtion Key Help" );
	Log( LOG_JUST_DISPLAY, "[F2] : Show Current (User / Server) Connection Number" );
	Log( LOG_JUST_DISPLAY, "[F3] : Show Current All Dungeon Status" );
	Log( LOG_JUST_DISPLAY, "[F5] : Toggle User Accept Status" );
	Log( LOG_JUST_DISPLAY, "[ESC]: Quit Server");
	Log( LOG_JUST_DISPLAY, "-------------------------------------------------------------------" );
}


void DisplayDBMessage(char* szMsg)
{
	char szDBMsg[ 0xfff ]={0,};
	wsprintf(szDBMsg, "[Msg for DB] %s", szMsg);
	Log(LOG_NORMAL, szDBMsg);
}

void DisplayDBReport(char* szMsg)
{
	Log(LOG_JUST_DISPLAY, szMsg);
}


void InitOleDBThread()
{
	HRESULT hr = ::CoCreateInstance(CLSID_OLEDBTHREAD, NULL, CLSCTX_INPROC_SERVER, IID_OLEDBTHREAD, (void**)&g_pDb);

	if (FAILED(hr))
	{
		Log(LOG_NORMAL, "Cannot create IID_OLEDBTHREAD");
		return;
	}

	DB_INITIALIZE_DESC desc;

	desc.bUsingThread		= TRUE;					//Thread 모델의 사용 여부, FALSE일 경우 (접두어 TH~ 계열의 메소드는 사용불가!)
	desc.bUsingEventObject	= TRUE;					//TRUE  : Event Object 방식으로 쿼리결과를 받음
													//FALSE : Message 방식으로 쿼리 결과를 Callback 함수  	
	desc.ReceiveFunc		= ReceivedFromDB;		//Event Object 방식일경우 결과값을 받을 Callback 함수 포인터
	
	desc.hWndToPostMessage	= NULL;					//Message방식일 경우 Message를 받을 윈도우의 핸들값 
	desc.uMessage = NULL;							//Message방식일 경우의 Message	
#ifdef DEV_MODE
	desc.wMaxNumOfProcessMessage_Input	= 30;		//DLL에서 동시에 수용할수 있는 최대 Input 메세지 갯수..  (Input 메세지큐 메모리풀의 최대용량)
	desc.wMaxNumOfProcessMessage_Output = 30;		//DLL에서 동시에 수용할수 있는 최대 Output 메세지 갯수..  (Output 메세지큐 메모리풀의 최대용량)
#else
	desc.wMaxNumOfProcessMessage_Input	= 1000;		//DLL에서 동시에 수용할수 있는 최대 Input 메세지 갯수..  (Input 메세지큐 메모리풀의 최대용량)
	desc.wMaxNumOfProcessMessage_Output = 1000;		//DLL에서 동시에 수용할수 있는 최대 Output 메세지 갯수..  (Output 메세지큐 메모리풀의 최대용량)
#endif
	desc.wMaxRowPerRead		= 50;					//DB에서 Data를 읽어드릴때 한번에 읽어드릴수 있는 최대 행의갯수 
													//예) 만약 쿼리로인해 Return되는 결과행수가 200행이고 
													//	  OpenRecordEx의 dwRowPerRead 인자값을 20으로 주었을경우
													//    한번에 20행씩 10번을 읽어서 200개를 받아온다.
													//	  이때 dwRowPerRead인자값이 가질수있는 최대범위가 50이라는 의미!!
	desc.wMaxReturnedRowNum = 50;					//쿼리문에서 반환될 수 있는 행의 최대 갯수
	desc.wMaxReturnedColNum = 128;					//쿼리문에서 반환될 수 있는 칼럼의 최대 갯수
	desc.dwMaxRowSize		= 1024;					//쿼리문에서 반환될 수 있는 한 행의 최대 사이즈	
	desc.bMaxParamNum		= 100;	
	desc.OutputMessageFunc	= DisplayDBMessage;		//DLL내부의 메세지나 오류메세지를 출력받을 외부 함수 포인터
	desc.ReportFunc			= DisplayDBReport;		
	
	// DB모듈 초기화 
	if(g_pDb->InitDBModule(&desc) == FALSE)
	{
		Log(LOG_NORMAL, "Fail to initialize DB Module!");
		return;
	}
	
	// Event Object 방식일경우 Event 핸들 셋팅
	g_pDb->SetDBResultEvent( g_pNet->GetCustomEventHandle(5) );

	g_pDb->SetPerformanceReport(FALSE);	

	// LogDB..	
	hr = ::CoCreateInstance(CLSID_OLEDBTHREAD, NULL, CLSCTX_INPROC_SERVER, IID_OLEDBTHREAD, (void**)&g_pLogDb);

	if (FAILED(hr))
	{
		Log(LOG_NORMAL, "Cannot create IID_OLEDBTHREAD(LogDB)");
		return;
	}

	desc.bUsingThread		= TRUE;					//Thread 모델의 사용 여부, FALSE일 경우 (접두어 TH~ 계열의 메소드는 사용불가!)
	desc.bUsingEventObject	= TRUE;					//TRUE  : Event Object 방식으로 쿼리결과를 받음
													//FALSE : Message 방식으로 쿼리 결과를 Callback 함수  
	desc.ReceiveFunc		= ReceivedFromDBLog;	//Event Object 방식일경우 결과값을 받을 Callback 함수 포인터
	desc.hWndToPostMessage	= NULL;					//Message방식일 경우 Message를 받을 윈도우의 핸들값 
	desc.uMessage			= NULL;					//Message방식일 경우의 Message
	
#ifdef DEV_MODE
	desc.wMaxNumOfProcessMessage_Input	= 30;		//DLL에서 동시에 수용할수 있는 최대 Input 메세지 갯수..  (Input 메세지큐 메모리풀의 최대용량)
	desc.wMaxNumOfProcessMessage_Output = 30;		//DLL에서 동시에 수용할수 있는 최대 Output 메세지 갯수..  (Output 메세지큐 메모리풀의 최대용량)
#else
	desc.wMaxNumOfProcessMessage_Input	= 1000;		//DLL에서 동시에 수용할수 있는 최대 Input 메세지 갯수..  (Input 메세지큐 메모리풀의 최대용량)
	desc.wMaxNumOfProcessMessage_Output = 1000;		//DLL에서 동시에 수용할수 있는 최대 Output 메세지 갯수..  (Output 메세지큐 메모리풀의 최대용량)
#endif	
	desc.wMaxRowPerRead			= 50;				//DB에서 Data를 읽어드릴때 한번에 읽어드릴수 있는 최대 행의갯수 
													//예) 만약 쿼리로인해 Return되는 결과행수가 200행이고 
													//	  OpenRecordEx의 dwRowPerRead 인자값을 20으로 주었을경우
													//    한번에 20행씩 10번을 읽어서 200개를 받아온다.
													//	  이때 dwRowPerRead인자값이 가질수있는 최대범위가 50이라는 의미!!
	desc.wMaxReturnedRowNum		= 50;				//쿼리문에서 반환될 수 있는 행의 최대 갯수
	desc.wMaxReturnedColNum		= 128;				//쿼리문에서 반환될 수 있는 칼럼의 최대 갯수
	desc.dwMaxRowSize			= 1024;				//쿼리문에서 반환될 수 있는 한 행의 최대 사이즈	
	desc.bMaxParamNum			= 100;
	desc.OutputMessageFunc		= DisplayDBMessage;	//DLL내부의 메세지나 오류메세지를 출력받을 외부 함수 포인터
	
	// DB모듈 초기화 
	if(g_pLogDb->InitDBModule(&desc) == FALSE)
	{
		Log(LOG_NORMAL, "Fail to initialize DB Module!(LogDB)");
		return;
	}

	// Event Object 방식일경우 Event 핸들 셋팅
	g_pLogDb->SetDBResultEvent( g_pNet->GetCustomEventHandle(8) );

	g_pLogDb->SetPerformanceReport(FALSE);
}


void InitNetwork()
{
	CUSTOM_EVENT ev[12];
	DESC_NETWORK desc;
	memset(&desc, 0, sizeof(desc));

	ev[desc.dwCustomDefineEventNum].dwPeriodicTime	= 200;
	ev[desc.dwCustomDefineEventNum++].pEventFunc	= ProcessDungeon;	
	
	ev[desc.dwCustomDefineEventNum].dwPeriodicTime	= 200;
	ev[desc.dwCustomDefineEventNum++].pEventFunc	= UpdateMonitor;

	ev[desc.dwCustomDefineEventNum].dwPeriodicTime	= 10000;				//10초 간격 으로 재 접속 시도 
	ev[desc.dwCustomDefineEventNum++].pEventFunc	= ReconnectWorldServer;	

	ev[desc.dwCustomDefineEventNum].dwPeriodicTime	= 0;
	ev[desc.dwCustomDefineEventNum++].pEventFunc	= ShowAllDungeonStatus;

	ev[desc.dwCustomDefineEventNum].dwPeriodicTime	= 0;
	ev[desc.dwCustomDefineEventNum++].pEventFunc	= RefreshDungeonServerData;

	ev[desc.dwCustomDefineEventNum].dwPeriodicTime	= 0;
	ev[desc.dwCustomDefineEventNum++].pEventFunc	= GetDBResult;

	ev[desc.dwCustomDefineEventNum].dwPeriodicTime	= 0;
	ev[desc.dwCustomDefineEventNum++].pEventFunc	= ToggleItemLog;

	ev[desc.dwCustomDefineEventNum].dwPeriodicTime	= 0;
	ev[desc.dwCustomDefineEventNum++].pEventFunc	= ToggleSpeedHackLog;
	
	ev[desc.dwCustomDefineEventNum].dwPeriodicTime	= 0;
	ev[desc.dwCustomDefineEventNum++].pEventFunc	= GetLogDBResult;

	ev[desc.dwCustomDefineEventNum].dwPeriodicTime	= 0;					// 1시간 간격으로 던젼 입장객 수 저장.
	ev[desc.dwCustomDefineEventNum++].pEventFunc	= DungeonEnteredCount;	// 입장객 수 DB에 저장

	ev[desc.dwCustomDefineEventNum].dwPeriodicTime	= 0;					// 1시간 간격으로 던젼 입장객 수 저장.
	ev[desc.dwCustomDefineEventNum++].pEventFunc	= ShutDownServer;		// 입장객 수 DB에 저장
	
	ev[desc.dwCustomDefineEventNum].dwPeriodicTime	= 1000 * 60;			// 1분마다 pk 수를 줄여 준다.
	ev[desc.dwCustomDefineEventNum++].pEventFunc	= PkDecreaseCountCheck;	

	desc.OnAcceptUser						= OnAcceptUser;
	desc.OnAcceptServer						= OnAcceptServer;
	desc.OnDisconnectServer					= OnDisconnectServer;
	desc.OnDisconnectUser					= OnDisconnectUser;
	desc.dwMainMsgQueMaxBufferSize			= g_pThis->GetMainMsgQueMaxBufferSize();
	desc.dwMaxServerNum						= g_pThis->GetMaxServerNum();
	desc.dwMaxUserNum						= g_pThis->GetMaxUserConnection();
	desc.dwServerBufferSizePerConnection	= g_pThis->GetServerBufferSizePerConnection();
	desc.dwServerMaxTransferSize			= g_pThis->GetServerMaxTransferSize();
	desc.dwUserBufferSizePerConnection		= g_pThis->GetUserBufferSizePerConnection();
	desc.dwUserMaxTransferSize				= g_pThis->GetUserMaxTransferSize();
	desc.OnRecvFromServerTCP				= OnRecvFromServerTCP;
	desc.OnRecvFromUserTCP					= OnRecvFromUserTCP;
	desc.pEvent								= ev;
	desc.dwFlag								= 0;
	desc.dwConnectNumAtSameTime				= 20;
	
	HRESULT hr;
	//HRESULT hr = CoCreateInstance( CLSID_4DyuchiNET, NULL, CLSCTX_INPROC_SERVER, IID_4DyuchiNET, (void**)&g_pNet);
	// 패킷 암호화 g_pNet 구성 변경 
	// 2005.01.05 김영대
	//HRESULT hr;
	g_pNet = new CNTNetWork();
	hr = g_pNet->CreateInstance();

    /*
	hr = CoCreateInstance(
           CLSID_4DyuchiNET,
           NULL,
           CLSCTX_INPROC_SERVER,
           IID_4DyuchiNET,
           (void**)&g_pNet);
		   */
	
	if (FAILED(hr))
	{
		Log(LOG_NORMAL, "Can not initialize IID_4DyuchiNET!");
	}

	if (!g_pNet->CreateNetwork(&desc, 10, 0))
	{
		Log(LOG_NORMAL, "Fail to CreateNetwork!");
	}

	g_pNet->PauseTimer(2);

	g_pThis->m_hKeyEvent[0] = g_pNet->GetCustomEventHandle(3);
	g_pThis->m_hKeyEvent[1] = g_pNet->GetCustomEventHandle(4);
	g_pThis->m_hKeyEvent[2] = g_pNet->GetCustomEventHandle(6);	// Item Log
	g_pThis->m_hKeyEvent[3] = g_pNet->GetCustomEventHandle(7);	// Speed Hack Log
	g_pThis->m_hKeyEvent[4] = g_pNet->GetCustomEventHandle(10);	// ShutDownServer

	InitializePacketProc();		
}

	
void TerminateServer()
{
	char szTitle[ 0xff ]		= {0,};
	char szServerType[0xff]		= {0,};

	switch(g_pThis->GetServerType())
	{
		case SERVER_TYPE_OCCUPIED_DUNGEON:		lstrcpy(szServerType, "Occupied Dungeon");		break;
		case SERVER_TYPE_NON_OCCUPIED_DUNGEON:	lstrcpy(szServerType, "Non-Occupied Dungeon");	break;
		case SERVER_TYPE_VILLAGE:				lstrcpy(szServerType, "Village");				break;
		case SERVER_TYPE_PATHWAY_DUNGEON:		lstrcpy(szServerType, "Pathway Dungeon");		break;
	}

	wsprintf(szTitle, "[%d] CorumOnline %s Server (%s)", g_pThis->GetPortForUser(), szServerType, g_pThis->GetIPForUser());
	
	HWND hWnd	= NULL;
	hWnd		= FindWindow(NULL,szTitle);

	UpdateAllUserDB();
	
	g_pDb->THIgnoreQueryInput();

	for(int i = 0; i < 11; i++)
	{
		g_pNet->PauseTimer(i);
	}

	if (g_pItemAttrLayer)
	{
		delete g_pItemAttrLayer;
		g_pItemAttrLayer = NULL;
	}

	if (g_pBaseBotary)
	{
		delete [] g_pBaseBotary;
		g_pBaseBotary = NULL;
	}

	if(g_pBasePresentBox)
	{
		delete [] g_pBasePresentBox;
		g_pBasePresentBox = NULL;
	}
	
	if(g_pMonsterTable)
	{
		delete g_pMonsterTable;
		g_pMonsterTable = NULL;
	}

	if (g_pMonsterClassPool)
	{
		delete g_pMonsterClassPool;
		g_pMonsterClassPool = NULL;
	}
	
	if (g_pTraderMonsterClassPool)
	{
		delete g_pTraderMonsterClassPool;
		g_pTraderMonsterClassPool = NULL;
	}

	if(g_pItemPool)
	{
		ReleaseStaticMemoryPool( g_pItemPool );
		g_pItemPool = NULL;
	}
	
	if(g_pCreateItemPool)
	{
		ReleaseStaticMemoryPool( g_pCreateItemPool );
		g_pCreateItemPool = NULL;
	}
	
	if(g_pPortalBufferPool)
	{
		ReleaseStaticMemoryPool(g_pPortalBufferPool);
		g_pPortalBufferPool = NULL;
	}

	if(g_pVirtualLinkItemNativePool)
	{
		ReleaseStaticMemoryPool(g_pVirtualLinkItemNativePool);
		g_pVirtualLinkItemNativePool = NULL;
	}

 	if(g_pDungeonTable)
	{
		delete g_pDungeonTable;
		g_pDungeonTable = NULL;
	}

	if(g_pAcceptTable)
	{
		delete g_pAcceptTable;
		g_pAcceptTable = NULL;
	}	

	if( g_pSetItemInfoHash )
	{
		g_pSetItemInfoHash->Destroy( TRUE );
		delete g_pSetItemInfoHash;
		g_pSetItemInfoHash = NULL;
	}

	if( g_pItemMakingInfoHash )
	{
		g_pItemMakingInfoHash->Destroy( TRUE );
		delete g_pItemMakingInfoHash;
		g_pItemMakingInfoHash = NULL;
	}

	if( g_pBaseItemHash )
	{
		g_pBaseItemHash->Destroy( TRUE );
		delete g_pBaseItemHash;
		g_pBaseItemHash	= NULL;
	}

	for( i = 0; i < MAX_ITEM_UPGRADE_LEVEL + 1; ++i)
	{
		if (g_pItemUpgradeTable[i])
		{
			delete [] g_pItemUpgradeTable[i];
			g_pItemUpgradeTable[i] = NULL;
		}
	}

	if(g_pItemTradeHash)
	{
		g_pItemTradeHash->Destroy(TRUE);
		delete g_pItemTradeHash;
		g_pItemTradeHash = NULL;
	}

	if(g_pPlayerShopLogHash)
	{
		g_pPlayerShopLogHash->Destroy(TRUE);
		delete g_pPlayerShopLogHash;
		g_pPlayerShopLogHash = NULL;
	}

	if(g_pGuildListHash)
	{
		g_pGuildListHash->Destroy(TRUE);
		delete g_pGuildListHash;
		g_pGuildListHash = NULL;
	}

	if(g_pItemStoreHash)
	{
		g_pItemStoreHash->Destroy(TRUE);
		delete g_pItemStoreHash;
		g_pItemStoreHash = NULL;
	}

	if(g_pShopTileHash)
	{
		g_pShopTileHash->Destroy(TRUE);
		delete g_pShopTileHash;
		g_pShopTileHash = NULL;
	}

	if(g_pItemShopHash)
	{
		g_pItemShopHash->Destroy(TRUE);
		delete g_pItemShopHash;
		g_pItemShopHash = NULL;
	}

	if(g_pItemTradePool)
	{
		ReleaseStaticMemoryPool(g_pItemTradePool);
		g_pItemTradePool = NULL;
	}
	
	if(g_pPlayerShopLogPool)
	{
		ReleaseStaticMemoryPool(g_pPlayerShopLogPool);
		g_pPlayerShopLogPool = NULL;
	}
	
	if(g_pShopTilePool)
	{
		ReleaseStaticMemoryPool(g_pShopTilePool);
		g_pShopTilePool = NULL;
	}
	
	if (g_pItemOptionTable)
	{
		delete [] g_pItemOptionTable;
		g_pItemOptionTable = NULL;
	}

	if(g_pGuildListPool)
	{
		ReleaseStaticMemoryPool(g_pGuildListPool);
		g_pGuildListPool = NULL;
	}

	if(g_pItemShopPool)
	{
		ReleaseStaticMemoryPool(g_pItemShopPool);
		g_pItemShopPool = NULL;
	}

	if(g_pItemStorePool)
	{
		ReleaseStaticMemoryPool(g_pItemStorePool);
		g_pItemStorePool = NULL;
	}

	if( g_pBaseMonsterHash )
	{
		g_pBaseMonsterHash->Destroy( TRUE );
		delete g_pBaseMonsterHash;
		g_pBaseMonsterHash = NULL;
	}
		
	if( g_pUserSetItemHash )
	{
		g_pUserSetItemHash->Destroy( TRUE );
		delete g_pUserSetItemHash;
		g_pUserSetItemHash = NULL;
	}

	if( g_pInvalidLogItemHash )
	{
		g_pInvalidLogItemHash->Destroy( TRUE );
		delete g_pInvalidLogItemHash;
		g_pInvalidLogItemHash = NULL;
	}

	if( g_pItemStoreNativeHash )
	{
		g_pItemStoreNativeHash->Destroy( TRUE );
		delete g_pItemStoreNativeHash;
		g_pItemStoreNativeHash = NULL;
	}

	if( g_pItemEventHash )
	{
		g_pItemEventHash->Destroy( TRUE );
		delete g_pItemEventHash;
		g_pItemEventHash = NULL;
	}

	if (g_pEffectLayer) 
	{
		delete g_pEffectLayer;
		g_pEffectLayer = NULL;
	}
	
	if (g_pUserClassPool)
	{	
		delete g_pUserClassPool;
		g_pUserClassPool = NULL;
	}
	
	if( g_pUserHash )
	{
		g_pUserHash->Destroy();
		delete g_pUserHash;
		g_pUserHash = NULL;
	}

	if(g_pNodePool)
	{
		ReleaseStaticMemoryPool( g_pNodePool );
		g_pNodePool = NULL;
	}

	if(g_pUserChkHash)
	{
		g_pUserChkHash->Destroy();
		delete g_pUserChkHash;
		g_pUserChkHash = NULL;
	}
	
	if(g_pThis)
		delete g_pThis;
	
	g_pThis = NULL;
	g_bIsServerRunning = FALSE;
	
	if(hWnd)
	{
		ShowWindow(hWnd,SW_MINIMIZE);
		ShowWindow(hWnd,SW_NORMAL);
	}
}


void ListenForUserside()
{
	if( !g_pNet->StartServerWithUserSide((char*)g_pThis->GetIPForUser(), g_pThis->GetPortForUser()) )
	{
		Log(LOG_NORMAL, "@ Can Not Listen For Userside! IP=\"%s\", Port=%d",g_pThis->GetIPForUser(), g_pThis->GetPortForUser());
		return;
	}

	Log(LOG_JUST_DISPLAY, "@ Now Listening... (For Userside(%d))", g_pThis->GetPortForUser());
	
	ReportToCMListener(2, &g_MachineName);
}


void ListenForServerside()
{
	if(!g_pNet->StartServerWithUserSide((char*)g_pThis->GetIPForServer(), g_pThis->GetPortForServer() ))
	{
		Log(LOG_NORMAL, "@ Can Not Listen For Serverside! IP=\"%s\", Port=%d",g_pThis->GetIPForServer(), g_pThis->GetPortForServer());
		return;
	}

	Log(LOG_JUST_DISPLAY, "@ Now Listening... (For Serverside(%d))", g_pThis->GetPortForServer());
}

void ConnectToWorldServer()
{
	g_pNet->ConnectToServerWithServerSide((char*)g_pThis->GetIPForWorldServer(), g_pThis->GetPortForWorldServer(), SuccessToConnectWorldServer, FailToConnectWorldServer, NULL);
}

bool __ConnectToDBServer(LPSTR szFile)
{
	char szIp[32]={0,};
	char szDbName[255]={0,};
	GetPrivateProfileString("EXTERNAL_SERVERINFO", "IP_FOR_DATABASE1", "", szIp, 32, szFile);

	GAME_DB = (BYTE)g_pDb->CreateDBConnection();
	
	wsprintf( szDbName, "%s%d", DEFAULT_GAME_DB, g_pThis->GetServerSetNum() );	

	Log(LOG_JUST_DISPLAY, "@ Connecting SQL Server(Game DB)...");
	OutputDebugString(DEFAULT_GAME_DB_ACCOUNT DEFAULT_GAME_DB_PASSWORD);


	if(g_pDb->Connect(szIp, szDbName, DEFAULT_GAME_DB_ACCOUNT, DEFAULT_GAME_DB_PASSWORD, 20, 10, FALSE, GAME_DB) < 0)
	{
		Log(LOG_NORMAL, "@ Can not connect SQL Server(Game DB)! IP=\"%s\"",szIp);
		return false;
	}
	Log(LOG_JUST_DISPLAY, "@ SQL Server(Game DB) Connected!(%s)", szIp);
	
	g_pDb->SetDefaultDBIndex(GAME_DB);	
	
	GetPrivateProfileString("EXTERNAL_SERVERINFO", "IP_FOR_DATABASE2", "", szIp, 32, szFile);

	LOG_DB = (BYTE)g_pLogDb->CreateDBConnection();
	Log(LOG_JUST_DISPLAY, "@ Connecting SQL Server(Total DB)...");

	if(g_bEnableItemLog)
	{
		char szDBName[ 0xff ]={0,};

		wsprintf(szDBName, "Log_Corum_%d", g_pThis->GetServerSetNum() );
	
		char szID[] = DEFAULT_LOG_DB_ACCOUNT;
		char szPW[] = DEFAULT_LOG_DB_PASSWORD;
		if(g_pLogDb->Connect(szIp, szDBName, szID, szPW, 20, 10, FALSE, LOG_DB) < 0)
		{
			Log(LOG_NORMAL, "@ Can not connect SQL Server(Log DB)! IP=\"%s\"",szIp);
			return false;
		}
		Log(LOG_JUST_DISPLAY, "@ SQL Server(Log DB) Connected!(%s)", szIp);
	}
	return true;
}

bool ConnectToDBServer()
{
	char* pFile = ".\\Server.ini";
	return __ConnectToDBServer(pFile);
}

bool ConnectToDBServerDecodeCFG()
{
	char* pFile = ".\\Server.cfg";
	char szFolderString[0xff]	= {0,};
	char szFileString[0xff]		= {0,};
	char szFolder[0xff]			= {0,};
	char szFile[0xff]			= {0,};
	
	do 
	{		
		CreateRandomString(szFolderString, 0xff);		
		wsprintf(szFolder,"c:\\%s",szFolderString);		
		
	}while(!CreateDirectory(szFolder,NULL));

	SetFileAttributes(szFolder, FILE_ATTRIBUTE_HIDDEN);

	CreateRandomString(szFileString, 0xff);
	lstrcat(szFileString,".ini");
	wsprintf(szFile,"%s\\%s",szFolder,szFileString);
	
	DecodeCFGFile(pFile,szFile,ENCODE_KEY,ENCODE_SUBKEY);

	const bool bRet = __ConnectToDBServer(szFile);

	DeleteFile(szFile);
	RemoveDirectory(szFolder);
	return bRet;
}


void RemoveUserProcess_InMatch(CUser* pRemoveUser)
{
	// 대전중 전사하여 그 던전(현재는 마을)의 지상으로 나온다. 
	DSTC_END_MATCH_MAN2MAN	packet;
	packet.bJudgmentCode = 0;

	// 패자 
	memset(packet.szLoserName, 0, sizeof(packet.szLoserName));
	__lstrcpyn(packet.szLoserName, pRemoveUser->m_szName, sizeof(packet.szLoserName));

	// 승자
	CUser* pUser = g_pUserHash->GetData(pRemoveUser->m_sMatchDescInfo.m_dwMatchUserIndex);

	if(pUser)
	{
		MatchWinCount(pUser);	// 승리 카운트 

		memset(packet.szWinnerName, 0, sizeof(packet.szWinnerName));
		__lstrcpyn(packet.szWinnerName, pUser->m_szName, sizeof(packet.szWinnerName));
		pUser->GetCurDungeon()->GetDungeonLayer(0)->BroadCastMsg((char*)&packet, packet.GetPacketSize());	// 마을에다가 알린다.
		
		CDungeonLayer* pLayer = pUser->GetCurDungeonLayer();

		if(pLayer)
		{
			// 대결장소에다가 날린다.
			pLayer->BroadCastMsg((char*)&packet, packet.GetPacketSize());		
			pLayer->m_dwEndMatchJudgmentTick = 0;
			pLayer->m_dwMatchEndTime = g_dwTickCount + MATCH_JUDGMENT_TERM_TICK;
		}
	}		

	// 패배 카운트
	MatchLoseCount(pRemoveUser);		
	pRemoveUser->SetStatus(UNIT_STATUS_NORMAL, TRUE);
	
#if defined JAPAN_LOCALIZING
// 일본 서버에서는 사용자 되살아날때 전체 HP 의 50 % 회복
	if ( pRemoveUser->GetHP() == 0 )
	{
		DSTC_USER_STATUS UserStatus;
		UserStatus.bStatusMany	= 0;

		// 대전 후 HP 를 반땅 채워준다.
		pRemoveUser->SetStatus(UNIT_STATUS_NORMAL, TRUE);
		pRemoveUser->SetHP( pRemoveUser->GetMaxHP() / 2 );

		// HP 반땅 채워준다.(클라이언트에 알린다.)
		UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_HP;
		UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= pRemoveUser->GetMaxHP() / 2;
		
		NetSendToUser(pRemoveUser->m_dwConnectionIndex, (char*)&UserStatus,  UserStatus.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);
	}
#else
	if ( pRemoveUser->GetHP() == 0 )									
	{
		// 다른곳은 HP 1 로 되살아남
		pRemoveUser->SetHP(1);
	}
#endif
}


void RemoveUserProcess(CDungeon* pDungeon)
{
	// 던전에서 떠날 유저를 떠나게 해줘라.
	POSITION_ posRemove = pDungeon->GetRemovePCList()->GetHeadPosition();	
	
	while(posRemove)
	{
		POSITION_	del			= posRemove;
		CUser*		pRemoveUser = (CUser*)pDungeon->GetRemovePCList()->GetNext(posRemove);

		if (pRemoveUser->m_dwTemp[USER_TEMP_FLAG_UPDATEUSERFORAI])
		{
			pRemoveUser->m_dwTemp[USER_TEMP_FLAG_UPDATEUSERFORAI] = 0;
			::UpdateUserForAI( pRemoveUser );				
		}

		if (pRemoveUser->m_dwTemp[USER_TEMP_USERDYINGSTARTTICK] &&
			pRemoveUser->m_dwTemp[USER_TEMP_USERDYINGSTARTTICK] <= g_dwTickCount)
		{
			pRemoveUser->m_dwTemp[USER_TEMP_USERDYINGSTARTTICK] = 0;

			BOOL bDisconnect = FALSE;

			switch(pRemoveUser->m_dwTemp[USER_TEMP_GOTOWORLD_OUTSTAUS])
			{
			case GOTOWORLD_STATUS_UPPER_LAYER:
				if (pRemoveUser->GetCurLayerIndex() != INVALID_DUNGEON_LAYER)
				{
					pRemoveUser->SetStatus(UNIT_STATUS_NORMAL, TRUE);
					pRemoveUser->GetCurDungeon()->ChangeLayerUser(pRemoveUser, pRemoveUser->GetCurLayerIndex(), 0);
				}
				break;

			case GOTOWORLD_STATUS_GOTOWORLD:
				{					
					DSTWS_GOTOWORLD packet;
					packet.dwUserIndex		= pRemoveUser->GetID();
					packet.dwMoney			= pRemoveUser->m_dwMoney;
					packet.wCriminalTime	= pRemoveUser->m_wCriminalPropensity;
					packet.wToDungeonID		= WORD(pDungeon->GetID() + pRemoveUser->m_bDestWorldSpot);
					packet.wNowDungeonID	= pRemoveUser->GetCurDungeonID();

					if(pDungeon->GetEventDungeonProperty() != NULL)
					{
						if(pRemoveUser->m_wRecentJoinDungeonID > 0)
						{
							packet.wToDungeonID = pRemoveUser->m_wRecentJoinDungeonID;
						}							
					}

					g_pNet->SendToServer( WSINDEX, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
					bDisconnect = TRUE;
				}
				break;
			case GOTOWORLD_STATUS_GOTOWORLD_OUTSTATUS_GUILDWAR:
				{					
					DSTWS_GOTOWORLD packet;
					packet.dwUserIndex		= pRemoveUser->GetID();
					packet.dwMoney			= pRemoveUser->m_dwMoney;
					packet.wCriminalTime	= pRemoveUser->m_wCriminalPropensity;
					packet.wToDungeonID		= WORD(pRemoveUser->m_wRecentJoinDungeonID + pRemoveUser->m_bDestWorldSpot);
					packet.wNowDungeonID	= pRemoveUser->GetCurDungeonID();
/*
					if(pDungeon->GetEventDungeonProperty() != NULL)
					{
						if(pRemoveUser->m_wRecentJoinDungeonID > 0)
						{
							packet.wToDungeonID = pRemoveUser->m_wRecentJoinDungeonID;
						}							
					}
*/
					g_pNet->SendToServer( WSINDEX, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
					bDisconnect = TRUE;
				}
				break;
			}
			
			if(pRemoveUser->m_sMatchDescInfo.m_bMatching == TRUE )	
			{
				RemoveUserProcess_InMatch(pRemoveUser);
			}
			
			pRemoveUser->m_posRemoveList = NULL;
			pDungeon->GetRemovePCList()->RemoveAt(del);

			if (bDisconnect)
			{
				pRemoveUser->CallBackCompulsiveDisconnectUser();
			}
		}			
	}
}


void InitMemoryPool()
{	
	g_pItemTradePool = CreateStaticMemoryPool();
	InitializeStaticMemoryPool(g_pItemTradePool, sizeof(ITEM_TRADE), g_pThis->GetPoolSizeItemTrade(), g_pThis->GetPoolSizeItemTrade());

	g_pItemStorePool = CreateStaticMemoryPool();
	InitializeStaticMemoryPool(g_pItemStorePool, sizeof(ITEM_STORE), g_pThis->GetPoolSizeItemStore(), g_pThis->GetPoolSizeItemStore());
	
	g_pItemShopPool = CreateStaticMemoryPool();
	InitializeStaticMemoryPool(g_pItemShopPool, sizeof(ITEM_SHOP), g_pThis->GetPoolSizeItemShop(), g_pThis->GetPoolSizeItemShop());

	g_pShopTilePool = CreateStaticMemoryPool();
	InitializeStaticMemoryPool(g_pShopTilePool, sizeof(SHOP_TILE), g_pThis->GetPoolSizeItemTitle(), g_pThis->GetPoolSizeItemTitle());	

	g_pGuildListPool = CreateStaticMemoryPool();
	InitializeStaticMemoryPool(g_pGuildListPool, sizeof(GUILD_NODE), g_pThis->GetPoolSizeGuildList(), g_pThis->GetPoolSizeGuildList());

	g_pPlayerShopLogPool = CreateStaticMemoryPool();
	InitializeStaticMemoryPool(g_pPlayerShopLogPool, sizeof(PLAYERSHOP_LOG), g_pThis->GetPoolSizeShopLog(), g_pThis->GetPoolSizeShopLog());	
	
	// 50000개도 모자란다. -_-
	g_pNodePool = CreateStaticMemoryPool();
	InitializeStaticMemoryPool( g_pNodePool, sizeof( CNode ), g_pThis->GetPoolSizeNode(), g_pThis->GetPoolSizeNode()); 

	g_pTraderMonsterClassPool = new CClassMemoryPool;
	g_pTraderMonsterClassPool->CreateMemoryPool(sizeof(CTraderMonster), g_pThis->GetPoolSizeTraderMonster());
	
	g_pMonsterClassPool = new CClassMemoryPool;
	g_pMonsterClassPool->CreateMemoryPool(sizeof(CMonster), g_pThis->GetPoolSizeMonster());
	
	g_pUserClassPool = new CClassMemoryPool;
	g_pUserClassPool->CreateMemoryPool(sizeof(CUser), g_pThis->GetPoolSizeUser());	

	g_pItemPool = CreateStaticMemoryPool();
	InitializeStaticMemoryPool(g_pItemPool, sizeof(ITEM_SERVER), g_pThis->GetPoolSizeItem(), g_pThis->GetPoolSizeItem() );
	
	g_pCreateItemPool = CreateStaticMemoryPool();
	InitializeStaticMemoryPool(g_pCreateItemPool, sizeof(ITEM_CREATE), g_pThis->GetPoolSizeCreateItem(), g_pThis->GetPoolSizeCreateItem());

	g_pPortalBufferPool = CreateStaticMemoryPool();
	InitializeStaticMemoryPool(g_pPortalBufferPool, sizeof(PORTAL_BUCKET), g_pThis->GetPoolSizePortal(), g_pThis->GetPoolSizePortal() );
	
	g_pVirtualLinkItemNativePool = CreateStaticMemoryPool();
	InitializeStaticMemoryPool(g_pVirtualLinkItemNativePool, sizeof(CItem), g_pThis->GetPoolSizeCreateItem(), g_pThis->GetPoolSizeCreateItem());
}


BOOL InitHashTables()
{
	g_pSetItemInfoHash = new SET_ITEM_INFO_HASH;
	g_pSetItemInfoHash->InitializeHashTable(g_pThis->GetHashSizeSetItemInfo(), g_pThis->GetHashSizeSetItemInfo());

	g_pItemMakingInfoHash = new ITEM_MAKING_INFO_HASH;
	g_pItemMakingInfoHash->InitializeHashTable(g_pThis->GetHashSizeMakeItemInfo(), g_pThis->GetHashSizeMakeItemInfo());

	g_pBaseItemHash = new BASEITEM_HASH;
	g_pBaseItemHash->InitializeHashTable(g_pThis->GetHashSizeBaseItem(), g_pThis->GetHashSizeBaseItem());

	g_pItemTradeHash = new CItemTradeHash;
	g_pItemTradeHash->InitializeHashTable(g_pThis->GetHashSizeItemTrade(), g_pThis->GetHashSizeItemTrade());

	g_pItemShopHash = new CItemShopHash;
	g_pItemShopHash->InitializeHashTable(g_pThis->GetHashSizeItemShop(), g_pThis->GetHashSizeItemShop());

	g_pPlayerShopLogHash = new CPlayerShopLogHash;
	g_pPlayerShopLogHash->InitializeHashTable(g_pThis->GetHashSizeShopLog(), g_pThis->GetHashSizeShopLog());

	g_pGuildListHash = new CGuildListHash;
	g_pGuildListHash->InitializeHashTable(g_pThis->GetHashSizeGuildList(), g_pThis->GetHashSizeGuildList());

	g_pShopTileHash = new CShopTileHash;
	g_pShopTileHash->InitializeHashTable(g_pThis->GetHashSizeItemTitle(), g_pThis->GetHashSizeItemTitle());

	g_pItemStoreHash = new CItemStoreHash;
	g_pItemStoreHash->InitializeHashTable(g_pThis->GetHashSizeItemStore(), g_pThis->GetHashSizeItemStore());

	g_pBaseMonsterHash	= new BASEMONSTER_HASH;
	g_pBaseMonsterHash->InitializeHashTable(g_pThis->GetHashSizeBaseMonster(), g_pThis->GetHashSizeBaseMonster());

	g_pUserChkHash	= new CUserHash;
	g_pUserChkHash->InitializeHashTable(g_pThis->GetHashSizeUser(), g_pThis->GetHashSizeUser());

	g_pUserHash = new USER_HASH;
	g_pUserHash->InitializeHashTable(g_pThis->GetHashSizeUser(), g_pThis->GetHashSizeUser());	
	
	g_pUserSetItemHash = new USER_SETITEM_HASH;
	g_pUserSetItemHash->InitializeHashTable( g_pThis->GetHashSizeSetItem(), g_pThis->GetHashSizeSetItem());
	
	g_pInvalidLogItemHash = new INVALID_LOG_ITEM_HASH;
	g_pInvalidLogItemHash->InitializeHashTable(g_pThis->GetHashSizeInvalidLog(), g_pThis->GetHashSizeInvalidLog());

	g_pItemStoreNativeHash = new ITEM_STORENATIVE_HASH;
	g_pItemStoreNativeHash->InitializeHashTable(g_pThis->GetHashSizeStoreNative(), g_pThis->GetHashSizeStoreNative());

	g_pItemEventHash = new ITEM_EVENT_HASH;
	g_pItemEventHash->InitializeHashTable(g_pThis->GetHashSizeInvalidLog(), g_pThis->GetHashSizeInvalidLog());

	return TRUE;
}


void LoadItemTable()
{
	DBRECEIVEDATA*	pData		= NULL;
	CBaseItem*		pBaseItem	= NULL;

	struct size_all{           //modified by Crazy 1.30.2008
	WORD	wID;
	char	szItemName_Kor[50];		
	char	szItemName_Eng[35];
	DWORD	dwCode_ID;
	BYTE	bCode_Type;
	BYTE	bRandItem;
	BYTE	bMovable;
	};

	int size_t = sizeof(size_all);
	
	
	g_pBaseItemHash->Destroy( TRUE );
	pData = g_pDb->OpenRecordEx("Select * from Item_Weapon", 500, DEFAULT_ROWS_PER_READ, GAME_DB );
	for( DWORD i = 0; i < pData->Query.select.dwRowCount; i++)
	{
		pBaseItem = new CBaseItem;
		memset( pBaseItem, 0, sizeof(CBaseItem) );
		memcpy( pBaseItem, pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ),size_t );
		memcpy( &pBaseItem->BaseItem_Weapon, pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ) + size_t, sizeof(BASEITEM_WEAPON));
		g_pBaseItemHash->Insert( pBaseItem, pBaseItem->wID );
	}
	
	g_pDb->ReleaseRecordset(pData);
	CHECK_MEMORY();

	pData = g_pDb->OpenRecordEx("Select * from Item_Armor", 1000, DEFAULT_ROWS_PER_READ, GAME_DB );
	for( i = 0; i < pData->Query.select.dwRowCount; i++ )
	{
		pBaseItem = new CBaseItem;
		memset( pBaseItem, 0, sizeof(CBaseItem) );
		memcpy( pBaseItem, pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ), size_t);//pData->Query.select.dwRowSize ); 
		memcpy( &pBaseItem->BaseItem_Armor, pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ) + size_t, sizeof(BASEITEM_ARMOR));
		g_pBaseItemHash->Insert( pBaseItem, pBaseItem->wID );
	}

	g_pDb->ReleaseRecordset(pData);
	CHECK_MEMORY();
	
	pData = g_pDb->OpenRecordEx("Select * from Item_Bag", 500, DEFAULT_ROWS_PER_READ, GAME_DB );
	for( i = 0; i < pData->Query.select.dwRowCount; i++)
	{
		pBaseItem = new CBaseItem;
		memset( pBaseItem, 0, sizeof(CBaseItem) );
		memcpy( pBaseItem, pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ), size_t);//pData->Query.select.dwRowSize ); 
		memcpy( &pBaseItem->BaseItem_Bag, pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ) + size_t, sizeof(BASEITEM_BAG));
		g_pBaseItemHash->Insert( pBaseItem, pBaseItem->wID );
	}
	
	g_pDb->ReleaseRecordset(pData);
	CHECK_MEMORY();

	pData = g_pDb->OpenRecordEx("Select * from Item_Consumable", 500, DEFAULT_ROWS_PER_READ, GAME_DB );
	for( i = 0; i < pData->Query.select.dwRowCount; i++ )
	{
		pBaseItem = new CBaseItem;
		memset( pBaseItem, 0, sizeof(CBaseItem) );
		memcpy( pBaseItem, pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ), size_t);//pData->Query.select.dwRowSize ); 
		memcpy( &pBaseItem->BaseItem_Consumable, pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ) + size_t, sizeof(BASEITEM_CONSUMABLE));
		g_pBaseItemHash->Insert( pBaseItem, pBaseItem->wID );
	}

	g_pDb->ReleaseRecordset(pData);
	CHECK_MEMORY();

	pData = g_pDb->OpenRecordEx("Select * from Item_Supplies", 500, DEFAULT_ROWS_PER_READ, GAME_DB );
	for( i = 0; i < pData->Query.select.dwRowCount; i++ )
	{
		pBaseItem = new CBaseItem;
		memset( pBaseItem, 0, sizeof(CBaseItem) );
		memcpy( pBaseItem, pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ), size_t);//pData->Query.select.dwRowSize ); 
		memcpy( &pBaseItem->BaseItem_Supplies, pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ) + size_t, sizeof(BASEITEM_SUPPLIES));
		g_pBaseItemHash->Insert( pBaseItem, pBaseItem->wID );
	}

	g_pDb->ReleaseRecordset(pData);
	CHECK_MEMORY();

	pData = g_pDb->OpenRecordEx("Select * from Item_Zodiac", 500, DEFAULT_ROWS_PER_READ, GAME_DB );
	for( i = 0; i < pData->Query.select.dwRowCount; i++ )
	{
		pBaseItem = new CBaseItem;
		memset( pBaseItem, 0, sizeof(CBaseItem) );
		memcpy( pBaseItem, pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ), size_t);//pData->Query.select.dwRowSize ); 
		memcpy( &pBaseItem->BaseItem_Zodiac, pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ) + size_t, sizeof(BASEITEM_ZODIAC));
		g_pBaseItemHash->Insert( pBaseItem, pBaseItem->wID );
	}
	
	g_pDb->ReleaseRecordset(pData);
	CHECK_MEMORY();

	pData = g_pDb->OpenRecordEx("Select * from Item_Ride", 500, DEFAULT_ROWS_PER_READ, GAME_DB );
	for( i = 0; i < pData->Query.select.dwRowCount; i++ )
	{
		pBaseItem = new CBaseItem;
		memset( pBaseItem, 0, sizeof(CBaseItem) );
		memcpy( pBaseItem, pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ), size_t);//pData->Query.select.dwRowSize ); 
		memcpy( &pBaseItem->BaseItem_Ride, pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ) + size_t, sizeof(BASEITEM_RIDE));
		g_pBaseItemHash->Insert( pBaseItem, pBaseItem->wID );
	}
	
	g_pDb->ReleaseRecordset(pData);
	CHECK_MEMORY();

	pData = g_pDb->OpenRecordEx("Select * from Item_Special", 500, DEFAULT_ROWS_PER_READ, GAME_DB );
	for( i = 0; i < pData->Query.select.dwRowCount; i++ )
	{
		pBaseItem = new CBaseItem;
		memset( pBaseItem, 0, sizeof(CBaseItem) );
		memcpy( pBaseItem, pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ), size_t);//pData->Query.select.dwRowSize ); 
		memcpy( &pBaseItem->BaseItem_Special, pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ) + size_t, sizeof(BASEITEM_SPECIAL));
		g_pBaseItemHash->Insert( pBaseItem, pBaseItem->wID );
	}

	g_pDb->ReleaseRecordset(pData);
	CHECK_MEMORY();

	pData = g_pDb->OpenRecordEx("Select * from Item_Guardian", 500, DEFAULT_ROWS_PER_READ, GAME_DB );
	for( i = 0; i < pData->Query.select.dwRowCount; i++ )
	{
		pBaseItem = new CBaseItem;
		memset( pBaseItem, 0, sizeof(CBaseItem) );
		memcpy( pBaseItem, pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ), size_t);//pData->Query.select.dwRowSize ); 
		memcpy( &pBaseItem->BaseItem_Guardian, pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ) + size_t, sizeof(BASEITEM_GUARDIAN));
		g_pBaseItemHash->Insert( pBaseItem, pBaseItem->wID );
	}

	g_pDb->ReleaseRecordset(pData);
	CHECK_MEMORY();

	pData = g_pDb->OpenRecordEx("Select * from Item_Magic_Array", 500, DEFAULT_ROWS_PER_READ, GAME_DB );
	for( i = 0; i < pData->Query.select.dwRowCount; i++ )
	{
		pBaseItem = new CBaseItem;
		memset( pBaseItem, 0, sizeof(CBaseItem) );
		memcpy( pBaseItem, pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ), size_t);//pData->Query.select.dwRowSize ); 
		memcpy( &pBaseItem->BaseItem_MagicArray, pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ) + size_t, sizeof(BASEITEM_MAGICARRAY));
		g_pBaseItemHash->Insert( pBaseItem, pBaseItem->wID );
	}

	g_pDb->ReleaseRecordset(pData);
	CHECK_MEMORY();

	pData = g_pDb->OpenRecordEx("Select * from Item_Materials", 500, DEFAULT_ROWS_PER_READ, GAME_DB );
	for( i = 0; i < pData->Query.select.dwRowCount; i++ )
	{
		pBaseItem = new CBaseItem;
		memset( pBaseItem, 0, sizeof(CBaseItem) );
		memcpy( pBaseItem, pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ), size_t);//pData->Query.select.dwRowSize ); 
		memcpy( &pBaseItem->BaseItem_Materials, pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ) + size_t, sizeof(BASEITEM_MATERIALS));
		g_pBaseItemHash->Insert( pBaseItem, pBaseItem->wID );
	}

	g_pDb->ReleaseRecordset(pData);
	CHECK_MEMORY();

	pData = g_pDb->OpenRecordEx("Select * from Item_Mix_Upgrade", 500, DEFAULT_ROWS_PER_READ, GAME_DB );
	for( i = 0; i < pData->Query.select.dwRowCount; i++ )
	{
		pBaseItem = new CBaseItem;
		memset( pBaseItem, 0, sizeof(CBaseItem) );
		memcpy( pBaseItem, pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ), size_t);//pData->Query.select.dwRowSize ); 
		memcpy( &pBaseItem->BaseItem_MixUpgrade, pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ) + size_t, sizeof(BASEITEM_MIX_UPGRADE));
		g_pBaseItemHash->Insert( pBaseItem, pBaseItem->wID );
	}
	
	g_pDb->ReleaseRecordset(pData);
	CHECK_MEMORY();

	pData = g_pDb->OpenRecordEx("Select * from Item_MagicField_Array", 500, DEFAULT_ROWS_PER_READ, GAME_DB );
	for( i = 0; i < pData->Query.select.dwRowCount; i++ )
	{
		pBaseItem = new CBaseItem;
		memset( pBaseItem, 0, sizeof(CBaseItem) );
		memcpy( pBaseItem, pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ), size_t);//pData->Query.select.dwRowSize ); 
		memcpy( &pBaseItem->BaseItem_MagicFieldArray, pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ) + size_t, sizeof(BASEITEM_MAGICFIELD_ARRAY));
		g_pBaseItemHash->Insert( pBaseItem, pBaseItem->wID );
	}
	
	g_pDb->ReleaseRecordset(pData);
	CHECK_MEMORY();

	pData = g_pDb->OpenRecordEx("Select * from Item_Upgrade",500,DEFAULT_ROWS_PER_READ,(BYTE)GAME_DB);
	for( i = 0; i < pData->Query.select.dwRowCount; i++)
	{
		pBaseItem = new CBaseItem;
		memset( pBaseItem, 0, sizeof(CBaseItem) );
		memcpy( pBaseItem, pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ), size_t);//pData->Query.select.dwRowSize ); 
		memcpy( &pBaseItem->BaseItem_UpGrade, pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ) + size_t, sizeof(BASEITEM_UPGRADE));
		g_pBaseItemHash->Insert( pBaseItem, pBaseItem->wID );
	}

	g_pDb->ReleaseRecordset(pData);
	CHECK_MEMORY();

	pData = g_pDb->OpenRecordEx("Select * from Item_liquid",500,DEFAULT_ROWS_PER_READ,(BYTE)GAME_DB);
	for( i = 0; i < pData->Query.select.dwRowCount; i++)
	{
		pBaseItem = new CBaseItem;
		memset( pBaseItem, 0, sizeof(CBaseItem) );
		memcpy( pBaseItem, pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ), size_t);//pData->Query.select.dwRowSize ); 
		memcpy( &pBaseItem->BaseItem_LiQuid, pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ) + size_t, sizeof(BASEITEM_LIQUID));
		g_pBaseItemHash->Insert( pBaseItem, pBaseItem->wID );
	}

	g_pDb->ReleaseRecordset(pData);
	CHECK_MEMORY();

	pData = g_pDb->OpenRecordEx("Select * from Item_Edition",500,DEFAULT_ROWS_PER_READ,(BYTE)GAME_DB);
	for( i = 0; i < pData->Query.select.dwRowCount; i++)
	{
		pBaseItem = new CBaseItem;
		memset( pBaseItem, 0, sizeof(CBaseItem) );
		memcpy( pBaseItem, pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ), size_t);//pData->Query.select.dwRowSize ); 
		memcpy( &pBaseItem->BaseItem_Edition, pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ) + size_t, sizeof(BASEITEM_EDITION));
		g_pBaseItemHash->Insert( pBaseItem, pBaseItem->wID );
	}

	g_pDb->ReleaseRecordset(pData);
	CHECK_MEMORY();

	SetCommonServerBaseItemHash(g_pBaseItemHash);
}


BOOL LoadDungeonServerData()
{
	DBRECEIVEDATA*			pData					= NULL;
	LPBASEMONSTER			pBaseMonster			= NULL;
	LPSET_ITEM_INFO			pSetItemInfo			= NULL;
	LPITEM_MAKING_INFO		pItemMakingInfo			= NULL;
	LPINVALID_LOG_ITEM		pInvalidLogItem			= NULL;
	LP_ITEM_STORENATIVE		pItemStoreNative		= NULL;
	ITEM_EVENT*				pItemEvent				= NULL;
	ITEM_STORE*				pItemStore				= NULL;
	BASEITEM_BOTARY*		pBaseBotary				= NULL;
	BASEITEM_PRESENT_BOX*	pBasePresent			= NULL;
	Character_Propensity*	pCharacter_Propensity	= NULL;
	
	if(g_pDb->OpenRecord( "Select * from BaseClassInfo", &g_pBaseClassInfo[1], MAX_CLASS, GAME_DB ) < 0)
		return FALSE;
	
	if(g_pDb->OpenRecord( "Select * from BaseRaceInfo", &g_pBaseRaceInfo[1], MAX_RACE, GAME_DB ) < 0)
		return FALSE;
	
	if(g_pDb->OpenRecord( "Select [EXP] from LevelExp", &g_pdwExp[1], MAX_LEVEL, GAME_DB ) < 0)
		return FALSE;

	if(g_pDb->OpenRecord( "Select [EXP] from Guardian_Exp", &g_pdwGuardianExp[1], MAX_LEVEL, GAME_DB ) < 0)
		return FALSE;
	
	if(g_pDb->OpenRecord( "Select * from Item_Upgrade_Formula_A", g_pItemOptionTable, ITEM_NUM_PER_OPTION, GAME_DB ) < 0 )
		return FALSE;
	
	ITEM_UPGRADE item_Upgrade[MAX_ITEM_UPGRADE_LEVEL * MAX_ITEM_KIND1_CODE];
	if(g_pDb->OpenRecord( "Select * from Item_Upgrade_Formula_B ORDER BY ID", &item_Upgrade, MAX_ITEM_UPGRADE_LEVEL * MAX_ITEM_KIND1_CODE, GAME_DB ) < 0 )
		return FALSE;

	for(DWORD i = 0; i < MAX_ITEM_UPGRADE_LEVEL; ++i)
		for(DWORD j = 0; j < MAX_ITEM_KIND1_CODE; ++j)
			memcpy(&g_pItemUpgradeTable[i+1][j], &item_Upgrade[i*MAX_ITEM_KIND1_CODE+j], sizeof(ITEM_UPGRADE));

	if(g_pDb->OpenRecord( "Select id, class, rate, apply_time, party, type_id_1, value_1, type_id_2, value_2, type_id_3, value_3, type_id_4, value_4, type_id_5, value_5 from cptable", &g_pCPTable, MAX_CP_TABLE, GAME_DB ) < 0)
		return FALSE;


	if(g_pDb->OpenRecord("Select Status From Char_Status", &g_byStstus, MAX_LEVEL, GAME_DB ) < 0)
		return FALSE;
	
	if(g_pDb->OpenRecord("Select Status From Guardian_Status", &g_byGuardianStatus, MAX_LEVEL, GAME_DB ) < 0)
		return FALSE;
	
	if(g_pDb->OpenRecord("Select * From DungeonProductionItemMinMax", &g_DungeonProductionItemMinMax, MAX_DUNGEON_PROPERTY, GAME_DB ) < 0)
		return FALSE;

	g_pSetItemInfoHash->Destroy( TRUE );
	pData = g_pDb->OpenRecordEx( "Select * from Item_SET_INFO", 500, DEFAULT_ROWS_PER_READ, GAME_DB );		
	for( i = 0; i < pData->Query.select.dwRowCount; i++ )
	{
		pSetItemInfo = new SET_ITEM_INFO;
		memset( pSetItemInfo, 0, sizeof( SET_ITEM_INFO ) );
		memcpy( pSetItemInfo, pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ), pData->Query.select.dwRowSize );
		g_pSetItemInfoHash->Insert( pSetItemInfo, pSetItemInfo->wSetItemID );
	}

	g_pDb->ReleaseRecordset(pData);	
	CHECK_MEMORY();

	g_pItemMakingInfoHash->Destroy( TRUE );
	pData = g_pDb->OpenRecordEx( "Select * from Item_making", 500, DEFAULT_ROWS_PER_READ, GAME_DB );
	for( i = 0; i < pData->Query.select.dwRowCount; i++ )
	{
		pItemMakingInfo = new ITEM_MAKING_INFO;
		memset( pItemMakingInfo, 0, sizeof( ITEM_MAKING_INFO ) );
		memcpy( pItemMakingInfo, pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ), pData->Query.select.dwRowSize );
		g_pItemMakingInfoHash->Insert( pItemMakingInfo,pItemMakingInfo->wID );
	}

	g_pDb->ReleaseRecordset(pData);
	CHECK_MEMORY();	

	pData = g_pDb->OpenRecordEx("select [id], itemid, [type], mapid from Item_Store order by ItemId", 2000 , DEFAULT_ROWS_PER_READ, (BYTE)GAME_DB );
	for( i = 0; i < pData->Query.select.dwRowCount; i++)
	{
		pItemStoreNative = new ITEM_STORENATIVE;
		memset( pItemStoreNative, 0, sizeof(ITEM_STORENATIVE) );
		memcpy( pItemStoreNative, pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ), pData->Query.select.dwRowSize );
		g_pItemStoreNativeHash->Insert( pItemStoreNative, pItemStoreNative->dwID );
	}

	g_pDb->ReleaseRecordset(pData);	
	CHECK_MEMORY();

	pData = g_pDb->OpenRecordEx("Select * From Item_Event_MaxCount Order by id", 500, DEFAULT_ROWS_PER_READ, (BYTE)GAME_DB );
	for( i = 0; i < pData->Query.select.dwRowCount; i++)
	{
		pItemEvent = new ITEM_EVENT;
		memset( pItemEvent, 0, sizeof(ITEM_EVENT) );
		memcpy( pItemEvent, pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ), pData->Query.select.dwRowSize );
		g_pItemEventHash->Insert( pItemEvent, pItemEvent->witem_id );
	}

	g_pDb->ReleaseRecordset(pData);	
	CHECK_MEMORY();

	g_pBaseMonsterHash->Destroy(TRUE);
	pData = g_pDb->OpenRecordEx("Select * from BaseMonsterInfo", 2000, DEFAULT_ROWS_PER_READ, GAME_DB );
	for( i = 0; i < pData->Query.select.dwRowCount; i++ )
	{
		pBaseMonster = new BASEMONSTER;
		memset( pBaseMonster, 0, sizeof(BASEMONSTER) );
		memcpy( pBaseMonster, pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ), pData->Query.select.dwRowSize );
		g_pBaseMonsterHash->Insert( pBaseMonster, pBaseMonster->dwID );
	}

	g_pDb->ReleaseRecordset(pData);
	CHECK_MEMORY();

	if(g_bEnableItemLog)
	{
		g_pInvalidLogItemHash->Destroy( TRUE );
		pData = g_pLogDb->OpenRecordEx("Select * from InvalidLogItem", 200, DEFAULT_ROWS_PER_READ, LOG_DB );
		for( i = 0; i < pData->Query.select.dwRowCount; i++ )
		{
			pInvalidLogItem = new INVALID_LOG_ITEM;
			memcpy( pInvalidLogItem, pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ), pData->Query.select.dwRowSize );
			g_pInvalidLogItemHash->Insert( pInvalidLogItem, pInvalidLogItem->dwBaseItemID );
		}

		g_pDb->ReleaseRecordset(pData);
		CHECK_MEMORY();
	}
	g_pItemStoreHash->Destroy(TRUE);
	pData = g_pDb->OpenRecordEx("Select ItemId From Item_Store Group by ItemId", 1500, DEFAULT_ROWS_PER_READ, GAME_DB);	
	for( i = 0; i < pData->Query.select.dwRowCount; i++ )
	{
		pItemStore = (ITEM_STORE*)LALAlloc(g_pItemStorePool);
		memcpy(pItemStore, pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ), pData->Query.select.dwRowSize );	
		g_pItemStoreHash->Insert(pItemStore, pItemStore->wItemId);
	}

	g_pDb->ReleaseRecordset(pData);
	CHECK_MEMORY();

	memset(g_pBaseBotary, 0, sizeof(BASEITEM_BOTARY) * MAX_ITEMBOTARY);
	pData = g_pDb->OpenRecordEx("Select * from basebotaryitem", MAX_ITEMBOTARY, DEFAULT_ROWS_PER_READ, GAME_DB);
	for( i = 0; i < pData->Query.select.dwRowCount; i++ )
	{
		pBaseBotary = (BASEITEM_BOTARY*)(pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ));
		memcpy(&g_pBaseBotary[pBaseBotary->dwID-BASE_ITEMBOTARY_INDEX], pBaseBotary, pData->Query.select.dwRowSize );
	}

	g_pDb->ReleaseRecordset(pData);
	CHECK_MEMORY();

	memset(g_pBasePresentBox, 0, sizeof(BASEITEM_PRESENT_BOX) * (MAX_ITEMPRESENT+1));
	pData = g_pDb->OpenRecordEx("Select [Item_Id],[Rate],[Quantity] from BasePresentBox", MAX_ITEMPRESENT, DEFAULT_ROWS_PER_READ, GAME_DB);
	for( i = 0; i < pData->Query.select.dwRowCount; i++ )
	{
		pBasePresent = (BASEITEM_PRESENT_BOX*)(pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ));
		memcpy(&g_pBasePresentBox[1].ItemRate[i], pBasePresent, pData->Query.select.dwRowSize );
	}

	g_pDb->ReleaseRecordset(pData);
	CHECK_MEMORY();

	memset(g_pCharacter_Propensity, 0, sizeof(Character_Propensity) * MAX_CHARACTER_PROPENSITY);
	pData = g_pDb->OpenRecordEx("Select * from Character_Propensity", MAX_CHARACTER_PROPENSITY, DEFAULT_ROWS_PER_READ, GAME_DB);
	for( i = 0; i < pData->Query.select.dwRowCount; i++ )
	{
		pCharacter_Propensity = (Character_Propensity*)(pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ));
		memcpy(&g_pCharacter_Propensity[pCharacter_Propensity->dwID], pCharacter_Propensity, pData->Query.select.dwRowSize );		
	}

	g_pDb->ReleaseRecordset(pData);
	CHECK_MEMORY();

	g_pItemAttrLayer->QueryItemAttr();
	CHECK_MEMORY();
	
	memset(g_NPCTable,0,sizeof(NPC_TABLE));
	if(	g_pDb->OpenRecord("Select [ID],[Type] From NPCTable ORDER BY ID", g_NPCTable, MAX_NPC_NUM_PER_VILLAGE, GAME_DB) < 0 ) 
		return FALSE;

	LoadItemTable();
	
	return TRUE;
}

BOOL ReLoadDungeonServerData()
{
	DBRECEIVEDATA*	pData = g_pDb->OpenRecordEx("Select * from BaseMonsterInfo", 2000, DEFAULT_ROWS_PER_READ, GAME_DB );
	BASEMONSTER		MonsterInfo;
	LPBASEMONSTER	pMonsterInfo = NULL;

	for( DWORD i = 0; i < pData->Query.select.dwRowCount; i++ )
	{
		memcpy( &MonsterInfo, pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ), pData->Query.select.dwRowSize );
		
		pMonsterInfo = g_pBaseMonsterHash->GetData( MonsterInfo.dwID );

		if(pMonsterInfo)
		{
			memcpy(pMonsterInfo, &MonsterInfo, pData->Query.select.dwRowSize);
		}
		else
		{
			LPBASEMONSTER pMonster = new BASEMONSTER;

			memcpy( pMonster, pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ), pData->Query.select.dwRowSize );
			g_pBaseMonsterHash->Insert( pMonster, pMonster->dwID );
		}
	}

	g_pDb->ReleaseRecordset(pData);

	CHECK_MEMORY();
	
	return TRUE;
}



