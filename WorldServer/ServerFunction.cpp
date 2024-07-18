#include "stdafx.h"
#include "ServerFunction.h"
#include "ServerTable.h"
#include "WorldUserTable.h"
#include "recvmsg.h"
#include "GuildHash.h"
#include "PartyHash.h"
#include "MessengerHash.h"
#include "AcceptTable.h"
#include "DungeonTable.h"
#include "EventDungeonManager.h"
#include "Map.h"
#include "ConnectionDistributionTable.h"
#include <afxdisp.h>
#include "GuildHash.h"
#include "Guild.h"
#include "GlobalDefine.h"
#include "SchoolGuildDungeon.h"
#include "PartyMatcher.h"

extern CSchoolGuildDungeonHash*	g_pSchoolGuildDungeonTableHash;

DWORD				g_dwCurTick				= timeGetTime();
BOOL				g_bIsServerRunning		= TRUE;
SYSTEMTIME			g_CurDate;									//5분 주기로 갱신되는 현재 날짜참조용(빌링체크쪽에서 쓰임) TimerPer5Minute 함수에서 갱신됨
DWORD				g_dwNoticeMessageCount	= 0;
DWORD				g_dwServerMessageCount	= 0;
DWORD				g_dwLastNoticeTick		= 0;
const int			g_iNoticeMessageTime	= 1000 * 60 * 60;	// 10분마다. (정섭에선 1시간.)
NOTICE_MESSAGE*		g_pNoticeMessage		= NULL;
SERVER_MESSAGE*		g_pServerMessage		= NULL;

void __stdcall GetDBResult(DWORD dwVal)
{
	g_pDb->GetDBResult();
}

void DisplayDBMessage(char* szMsg)
{
	char szDBMsg[ 2048 ]={0,};
	wsprintf(szDBMsg, "[Msg for DB] %s", szMsg);
	Log(LOG_NORMAL, szDBMsg);
}

void DisplayDBReport(char* szMsg)
{
	Log(LOG_JUST_DISPLAY, szMsg);
}


void SendNoticeMessageProcess(void)
{
	// 자동 공지 메시지를 보낼 시간이 되었는지 검사한다.	
	DWORD dwTick = timeGetTime();
	
	if ((g_dwLastNoticeTick + g_iNoticeMessageTime) <= dwTick)
	{
		g_dwLastNoticeTick = dwTick;

		if (NULL == g_pNoticeMessage)
		{
			return;
		}

		int iRandom = rand() / (RAND_MAX / g_dwNoticeMessageCount);	
		
		// Send Notice message
		if (0 != g_pNoticeMessage[iRandom].byShow) // 보내야 할 메시지일 때.
		{			
			WSTDS_NOTICE_ACTION NoticeActionPacket;
			
			memset(NoticeActionPacket.szMessage, 0, 1024);	
			__lstrcpyn(NoticeActionPacket.szMessage, g_pNoticeMessage[iRandom].szMessage, 1024);	
			NoticeActionPacket.wMsgLen = (WORD)(strlen(g_pNoticeMessage[iRandom].szMessage) + 1);  // +1 : Null.			
			
			// 혹시라도 버퍼가 덮어졌는지 체크.
			if (1024 <= NoticeActionPacket.wMsgLen)
			{
				Log(LOG_IMPORTANT, "Dangerous. Notice array overscope. at CmdNotice function.");
				return;
			}
			
			// 각 던젼 서버들에게 보낸다.			
			g_pServerTable->BroadCast((char*)&NoticeActionPacket, NoticeActionPacket.GetPacketSize());	
			
			// 그리고 던젼에 접속해 있지 않은 월드에만 붙은 유저들에게 공지 메시지를 보내준다.			
			CTWS_GM_NOTICE SendMessage;
			memset(SendMessage.szMsg, 0, 1024);			
			__lstrcpyn(SendMessage.szMsg, g_pNoticeMessage[iRandom].szMessage, 1024);
			SendMessage.wMsgLen	= (WORD)(strlen(g_pNoticeMessage[iRandom].szMessage) + 1);		// +1 : Null 
			
			g_pUserTable->SendUserInWorld((char*)&SendMessage, SendMessage.GetPacketSize());		
		}
	}	
}

void InitOleDBThread()
{
	HRESULT hr = ::CoCreateInstance( CLSID_OLEDBTHREAD, NULL, CLSCTX_INPROC_SERVER, IID_OLEDBTHREAD, (void**)&g_pDb);

	if (FAILED(hr))
	{
		Log(LOG_NORMAL, "Cannot create IID_OLEDBTHREAD");
		return;
	}

	DB_INITIALIZE_DESC desc;

	desc.bUsingThread		= TRUE;				//Thread 모델의 사용 여부, FALSE일 경우 (접두어 TH~ 계열의 메소드는 사용불가!)
	desc.bUsingEventObject	= TRUE;				//TRUE  : Event Object 방식으로 쿼리결과를 받음
												//FALSE : Message 방식으로 쿼리 결과를 Callback 함수  	
	desc.ReceiveFunc = ReceivedFromDB;			//Event Object 방식일경우 결과값을 받을 Callback 함수 포인터
	desc.hWndToPostMessage = NULL;				//Message방식일 경우 Message를 받을 윈도우의 핸들값 
	desc.uMessage = NULL;						//Message방식일 경우의 Message
	
	desc.wMaxNumOfProcessMessage_Input = 1000;	//DLL에서 동시에 수용할수 있는 최대 Input 메세지 갯수..  (Input 메세지큐 메모리풀의 최대용량)
	desc.wMaxNumOfProcessMessage_Output = 1000;	//DLL에서 동시에 수용할수 있는 최대 Output 메세지 갯수..  (Output 메세지큐 메모리풀의 최대용량)

	desc.wMaxRowPerRead		= 50;				//DB에서 Data를 읽어드릴때 한번에 읽어드릴수 있는 최대 행의갯수 
												//예) 만약 쿼리로인해 Return되는 결과행수가 200행이고 
												//	  OpenRecordEx의 dwRowPerRead 인자값을 20으로 주었을경우
												//    한번에 20행씩 10번을 읽어서 200개를 받아온다.
												//	  이때 dwRowPerRead인자값이 가질수있는 최대범위가 50이라는 의미!!
	desc.bMaxParamNum		= 100;
	desc.wMaxReturnedRowNum = 50;				//쿼리문에서 반환될 수 있는 행의 최대 갯수
	desc.wMaxReturnedColNum = 128;				//쿼리문에서 반환될 수 있는 칼럼의 최대 갯수
	desc.dwMaxRowSize		= 2048;				//쿼리문에서 반환될 수 있는 한 행의 최대 사이즈	
	
	desc.OutputMessageFunc	= DisplayDBMessage;	//DLL내부의 메세지나 오류메세지를 출력받을 외부 함수 포인터
	desc.ReportFunc			= DisplayDBReport;		
	
	// DB모듈 초기화 
	if(g_pDb->InitDBModule(&desc) == FALSE)
	{
		Log(LOG_NORMAL, "Fail to initialize DB Module!");
		return;
	}

	// Event Object 방식일경우 Event 핸들 셋팅
	g_pDb->SetDBResultEvent( g_pNet->GetCustomEventHandle(0) );

	g_pDb->SetPerformanceReport(FALSE);	


//================================================================================================================
// LogDB..
	
	hr = ::CoCreateInstance(
			CLSID_OLEDBTHREAD,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_OLEDBTHREAD,
			(void**)&g_pLogDb);

	if (FAILED(hr))
	{
		Log(LOG_NORMAL, "Cannot create IID_OLEDBTHREAD(LogDB)");
		return;
	}

//	DB_INITIALIZE_DESC desc;

	desc.bUsingThread		= TRUE;					//Thread 모델의 사용 여부, FALSE일 경우 (접두어 TH~ 계열의 메소드는 사용불가!)
	desc.bUsingEventObject	= TRUE;					//TRUE  : Event Object 방식으로 쿼리결과를 받음
													//FALSE : Message 방식으로 쿼리 결과를 Callback 함수  
	
	desc.ReceiveFunc		= ReceivedFromDBLog;	//Event Object 방식일경우 결과값을 받을 Callback 함수 포인터
	
	desc.hWndToPostMessage	= NULL;					//Message방식일 경우 Message를 받을 윈도우의 핸들값 
	desc.uMessage			= NULL;					//Message방식일 경우의 Message

	desc.wMaxNumOfProcessMessage_Input = 1000;	//DLL에서 동시에 수용할수 있는 최대 Input 메세지 갯수..  (Input 메세지큐 메모리풀의 최대용량)
	desc.wMaxNumOfProcessMessage_Output = 1000;	//DLL에서 동시에 수용할수 있는 최대 Output 메세지 갯수..  (Output 메세지큐 메모리풀의 최대용량)

	desc.wMaxRowPerRead = 50;						//DB에서 Data를 읽어드릴때 한번에 읽어드릴수 있는 최대 행의갯수 
													//예) 만약 쿼리로인해 Return되는 결과행수가 200행이고 
													//	  OpenRecordEx의 dwRowPerRead 인자값을 20으로 주었을경우
													//    한번에 20행씩 10번을 읽어서 200개를 받아온다.
													//	  이때 dwRowPerRead인자값이 가질수있는 최대범위가 50이라는 의미!!
	
	desc.wMaxReturnedRowNum		= 50;				//쿼리문에서 반환될 수 있는 행의 최대 갯수
	desc.wMaxReturnedColNum		= 128;				//쿼리문에서 반환될 수 있는 칼럼의 최대 갯수
	desc.dwMaxRowSize			= 1024;				//쿼리문에서 반환될 수 있는 한 행의 최대 사이즈	
	desc.bMaxParamNum			= 100;
	
	desc.OutputMessageFunc		= DisplayDBMessage;	//DLL내부의 메세지나 오류메세지를 출력받을 외부 함수 포인터
	
	//DB모듈 초기화 
	if(g_pLogDb->InitDBModule(&desc) == FALSE)
	{
		Log(LOG_NORMAL, "Fail to initialize DB Module!(LogDB)");
		return;
	}

//	Log(LOG_NORMAL, "Log DB Size : %u Byte", g_pLogDb->GetOLEDBSize());

	//Event Object 방식일경우 Event 핸들 셋팅
	g_pLogDb->SetDBResultEvent( g_pNet->GetCustomEventHandle(8) );

	g_pLogDb->SetPerformanceReport(FALSE);
}

void TerminateServer()
{
	char szTitle[ 0xff ] = {0,};
	wsprintf(szTitle,"[%d] CorumOnline World Server (%s)",g_pThis->GetPortForUser(),g_pThis->GetIPForUser());
	HWND hWnd	= NULL;
	hWnd		= FindWindow(NULL,szTitle);

	if(g_pGuildTableHash)
	{
		ListNode<GUILD_TABLE>* lpTablePos = g_pGuildTableHash->GetHead();

		while(lpTablePos)
		{
			LP_GUILD_TABLE lpGuildTable = lpTablePos->pData;

			if(lpGuildTable)
			{
				char szQuery[0xff]={0,};
				wsprintf(szQuery, "Update GuildInfo Set Level= %u, Exp = %u Where Id = %u", lpGuildTable->sGuild.wLevel, lpGuildTable->sGuild.dwExp, lpGuildTable->sGuild.dwGuildId);
				g_pDb->ExecuteSQL(szQuery, (BYTE)GAME_DB);
			}
			lpTablePos = lpTablePos->pNext;
		}
	}	

	g_pDb->THIgnoreQueryInput();

	if(g_pAcceptTable)
	{
		delete g_pAcceptTable;
		g_pAcceptTable = NULL;
	}
	
	if(g_pDungeonTable)		
	{
		delete g_pDungeonTable;
		g_pDungeonTable = NULL;
	}
	
	if (g_pEventDungeonManager)
	{
		delete g_pEventDungeonManager;
		g_pEventDungeonManager = NULL;
	}

	for(int i=0; i<MAX_WORLD_NUM_PER_SERVER; i++)			
	{
		if(g_pMap[i])
		{
			delete g_pMap[i];		
			g_pMap[i] = NULL;
		}
	}

#ifdef CHECK_KEEP_ALIVE
	if(g_pCDTable)
	{
		delete g_pCDTable;
		g_pCDTable = NULL;
	}
#endif

	if( IS_ABLE_NATION( NC_JAPAN) )
	{
		if(g_pSchoolGuildDungeonTableHash)
		{
			g_pSchoolGuildDungeonTableHash->Destroy(FALSE);
			delete g_pSchoolGuildDungeonTableHash;
			g_pSchoolGuildDungeonTableHash = NULL;
		}
	} 

	if(g_pGuildTableHash)										
	{	
		g_pGuildTableHash->Destroy(TRUE);
		delete g_pGuildTableHash;
		g_pGuildTableHash = NULL;
	}

	if(g_pPartyTableHash)						
	{
		g_pPartyTableHash->Destroy(TRUE);
		delete g_pPartyTableHash;
		g_pPartyTableHash = NULL;
	}

	if(g_pPartyBoardTableHash)
	{
		g_pPartyBoardTableHash->Destroy(TRUE);
		delete g_pPartyBoardTableHash;
		g_pPartyBoardTableHash = NULL;
	}

	UnloadNoticeMessage();
	UnLoadServerMessage();

	if(g_pGroupInfoTableHash)					
	{
		g_pGroupInfoTableHash->Destroy(TRUE);
		delete g_pGroupInfoTableHash;
		g_pGroupInfoTableHash= NULL;
	}

	if(g_pGuildOfflinePool)
	{
		ReleaseStaticMemoryPool(g_pGuildOfflinePool);
		g_pGuildOfflinePool	= NULL;
	}	

	if(g_pGroupTablePool)
	{
		ReleaseStaticMemoryPool(g_pGroupTablePool);
		g_pGroupTablePool = NULL;
	}

	if(g_pRankList)
	{
		g_pRankList->RemoveAll();
		delete g_pRankList;
		g_pRankList = NULL;
	}

	if(g_pRankListPool)
	{
		ReleaseStaticMemoryPool(g_pRankListPool	);
		g_pRankListPool	 = NULL;
	}
	
	if(g_pGuildWarTimeList)
	{
		g_pGuildWarTimeList->RemoveAll();
		delete g_pGuildWarTimeList;
		g_pGuildWarTimeList = NULL;
	}

	if(g_pGuildWarTimePool)
	{
		ReleaseStaticMemoryPool(g_pGuildWarTimePool);
		g_pGuildWarTimePool = NULL;
	}

	if(g_pMessengerTableHash)						
	{
		g_pMessengerTableHash->Destroy(TRUE);
		delete g_pMessengerTableHash;
		g_pMessengerTableHash = NULL;
	}

	if(g_pMessengerUserPool)						
	{
		ReleaseStaticMemoryPool(g_pMessengerUserPool);
		g_pMessengerUserPool = NULL;
	}
	
	if(g_pDungeonListHash)
	{
		g_pDungeonListHash->Destroy(TRUE);
		delete g_pDungeonListHash;
		g_pDungeonListHash = NULL;
	}

	if(g_pGuildWarHash)
	{
		g_pGuildWarHash->Destroy(TRUE);
		delete g_pGuildWarHash;
		g_pGuildWarHash = NULL;
	}

	if(g_pGuildWarPool)
	{
		ReleaseStaticMemoryPool(g_pGuildWarPool);
		g_pGuildWarPool = NULL;
	}
	
	if(g_pDungeonNodePool)
	{
		ReleaseStaticMemoryPool(g_pDungeonNodePool);
		g_pDungeonNodePool = NULL;		
	}

	if(g_pMessengerTablePool)					
	{
		ReleaseStaticMemoryPool(g_pMessengerTablePool);
		g_pMessengerTablePool = NULL;
	}
		
	if(g_pGuildTablePool)
	{
		ReleaseStaticMemoryPool(g_pGuildTablePool);
		g_pGuildTablePool = NULL;
	}

	if(g_pPartyTablePool)
	{
		ReleaseStaticMemoryPool(g_pPartyTablePool);
		g_pPartyTablePool = NULL;
	}

	if(g_pPartyBoardTablePool)
	{
		ReleaseStaticMemoryPool(g_pPartyBoardTablePool);
		g_pPartyBoardTablePool = NULL;
	}
	
	if(g_pUserTable)
		delete g_pUserTable;

	if(g_pServerTable)
		delete g_pServerTable;

	if(g_pThis)
		delete g_pThis;

	for(i=0;i<23;i++)
		g_pNet->PauseTimer(i);

	g_bIsServerRunning = FALSE;

	if(hWnd)
	{
		ShowWindow(hWnd,SW_MINIMIZE);
		ShowWindow(hWnd,SW_NORMAL);
	}	
}


void __stdcall TimerPerSec(DWORD dwVal)
{
	SRAND();

	START_FUNCTION_PROFILE();

	g_dwCurTick = timeGetTime();

	if(g_pDungeonTable)
	{
		// 공성전을 위한 처리.	
		POSITION_ pos = g_pDungeonTable->m_pDungeonList->GetHeadPosition();

		while(pos)
		{
			DUNGEON_DATA_EX* pDungeon = (DUNGEON_DATA_EX*)g_pDungeonTable->m_pDungeonList->GetNext(pos);
			
			if (pDungeon->IsConquer() && pDungeon->m_pServer)
			{	
				// 점령던전들 돌면서갱신해야할 유저들에게 던전 갱신 이벤트를 보내줘라.
				pDungeon->StartSiegeWarTime();
				
				if (!pDungeon->m_bSiege)
				{
					pDungeon->LevelUpgrade();							
				}
			}
		}
	}

	if(NULL != g_pEventDungeonManager)
	{
		g_pEventDungeonManager->Process();
	}
	
	SendNoticeMessageProcess();

	END_FUNCTION_PROFILE();
}

void __stdcall EnableDoubleExp(DWORD dwVal)
{
	WSTDS_SET_DOUBLE_EXP packet;
	packet.bEnable = 1;
	g_pServerTable->BroadCast((char*)&packet, packet.GetPacketSize() );
}

void __stdcall DisableDoubleExp(DWORD dwVal)
{
	WSTDS_SET_DOUBLE_EXP packet;
	packet.bEnable = 0;
	g_pServerTable->BroadCast((char*)&packet, packet.GetPacketSize() );
}


void __stdcall IncreaseMagicDropRate(DWORD dwVal)
{
	WSTDS_SET_MAGICITEM_DROPRATE packet;
	g_pThis->m_GLOBAL_MAGIC_FIND_PBT++;
	packet.wDropRate = (WORD)(g_pThis->m_GLOBAL_MAGIC_FIND_PBT * 100);
	g_pServerTable->BroadCast((char*)&packet, packet.GetPacketSize() );

	Log(LOG_JUST_DISPLAY, "Increase Magic Drop Rate : %d", packet.wDropRate);
}

void __stdcall DecreaseMagicDropRate(DWORD dwVal)
{
	WSTDS_SET_MAGICITEM_DROPRATE packet;
	if (g_pThis->m_GLOBAL_MAGIC_FIND_PBT > 0)
		g_pThis->m_GLOBAL_MAGIC_FIND_PBT--;

	packet.wDropRate = (WORD)(g_pThis->m_GLOBAL_MAGIC_FIND_PBT * 100);
	g_pServerTable->BroadCast((char*)&packet, packet.GetPacketSize() );

	Log(LOG_JUST_DISPLAY, "Decrease Magic Drop Rate : %d", packet.wDropRate);
}

void __stdcall IncreaseExpGain(DWORD dwVal)
{
	WSTDS_SET_EXPGAIN packet;
	g_pThis->m_dwGLOBAL_EXP_GAIN++;
	packet.wEXP = (WORD)(g_pThis->m_dwGLOBAL_EXP_GAIN * 100);
	g_pServerTable->BroadCast((char*)&packet, packet.GetPacketSize() );

	Log(LOG_JUST_DISPLAY, "Increase Exp Gain : %d", packet.wEXP);
}

void __stdcall DecreaseExpGain(DWORD dwVal)
{
	WSTDS_SET_EXPGAIN packet;
	if (g_pThis->m_dwGLOBAL_EXP_GAIN > 0)
		g_pThis->m_dwGLOBAL_EXP_GAIN--;

	packet.wEXP = (WORD)(g_pThis->m_dwGLOBAL_EXP_GAIN * 100);
	g_pServerTable->BroadCast((char*)&packet, packet.GetPacketSize() );

	Log(LOG_JUST_DISPLAY, "Decrease Exp Gain : %d", packet.wEXP);
}

void __stdcall IncreasePartyBonusExpGain(DWORD dwVal)
{
	WSTDS_SET_PARTY_BONUS_EXPGAIN packet;
	g_pThis->m_dwGLOBAL_PARTY_BONUS_EXP_GAIN++;
	packet.wEXP = (WORD)(g_pThis->m_dwGLOBAL_PARTY_BONUS_EXP_GAIN*100);
	g_pServerTable->BroadCast((char*)&packet, packet.GetPacketSize() );

	Log(LOG_JUST_DISPLAY, "Increase Party Bonus Exp Gain : %d", packet.wEXP);
}

void __stdcall DecreasePartyBonusExpGain(DWORD dwVal)
{
	WSTDS_SET_PARTY_BONUS_EXPGAIN packet;
	if (g_pThis->m_dwGLOBAL_PARTY_BONUS_EXP_GAIN > 0)
		g_pThis->m_dwGLOBAL_PARTY_BONUS_EXP_GAIN--;

	packet.wEXP = (WORD)(g_pThis->m_dwGLOBAL_PARTY_BONUS_EXP_GAIN*100);
	g_pServerTable->BroadCast((char*)&packet, packet.GetPacketSize() );

	Log(LOG_JUST_DISPLAY, "Decrease Party Bonus Exp Gain : %d", packet.wEXP);
}

void __stdcall GuildMaster(DWORD dwVal)
{
	START_FUNCTION_PROFILE();

	char szQuery[0xff] = {0,};

	SYSTEMTIME SystemTime;
	GetLocalTime(&SystemTime);

	ListNode<GUILD_TABLE>* lpTablePos = g_pGuildTableHash->GetHead();

	while(lpTablePos)
	{
		LP_GUILD_TABLE lpGuildTable = lpTablePos->pData;

		if(!lpGuildTable)
		{
			lpTablePos = lpTablePos->pNext;
			continue;
		}

		ListNode<GUILD_OFFLINE_USER>* lpOfflineUserPos = lpGuildTable->pGuildOfflineHash->GetHead();

		while(lpOfflineUserPos)
		{
			LP_GUILD_OFFLINE_USER lpGuildOfflineUser = lpOfflineUserPos->pData;

			if(!lpGuildOfflineUser)
			{
				lpOfflineUserPos = lpOfflineUserPos->pNext;
				continue;
			}

			if(lpGuildOfflineUser->byRank == 1)
			{				
				COleDateTime PrevTime(
					lpGuildOfflineUser->sDate.year
					, lpGuildOfflineUser->sDate.month
					, lpGuildOfflineUser->sDate.day
					, lpGuildOfflineUser->sDate.hour
					, lpGuildOfflineUser->sDate.minute
					, lpGuildOfflineUser->sDate.second);
				
				if(lpGuildOfflineUser->sDate.year == 0)	
				{
					lpOfflineUserPos = lpOfflineUserPos->pNext;
					continue;
				}

				if(lpGuildOfflineUser->sDate.day == 0)
				{
					lpOfflineUserPos = lpOfflineUserPos->pNext;
					continue;
				}

				if(lpGuildOfflineUser->sDate.month == 0)
				{
					lpOfflineUserPos = lpOfflineUserPos->pNext;
					continue;
				}

				COleDateTime CurTime(SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour, SystemTime.wMinute, SystemTime.wSecond);
				
				COleDateTimeSpan ts = CurTime - PrevTime;
   
				DWORD dwDay = ts.GetDays();

				if(dwDay >= 7)
				{					
					// 길드 이양
					CWorldUser* pMasterUser = NULL;

					ListNode<CWorldUser>* lpGuildUserNode = lpGuildTable->pGuildUserHash->GetHead();		
													
					while(lpGuildUserNode)
					{
						CWorldUser* pGuildUser = lpGuildUserNode->pData;
						
						if(!pGuildUser)
						{
							lpGuildUserNode = lpGuildUserNode->pNext;
							continue;
						}

						if(pMasterUser==NULL)
						{
							pMasterUser = pGuildUser;
						}
						else
						{
							if(pMasterUser->m_byRank>pGuildUser->m_byRank)
							{
								pMasterUser = pGuildUser;
							}
							else if(pMasterUser->m_byRank==pGuildUser->m_byRank)
							{
								if(pMasterUser->m_dwLevel<pGuildUser->m_dwLevel)
									pMasterUser = pGuildUser;
							}
						}

						lpGuildUserNode = lpGuildUserNode->pNext;
					}						

					LP_GUILD_OFFLINE_USER lpMasterOfflineUser = NULL;
					
					ListNode<GUILD_OFFLINE_USER>* lpGuildOfflineUserNode = lpGuildTable->pGuildOfflineHash->GetHead();
					while(lpGuildOfflineUserNode)
					{
						LP_GUILD_OFFLINE_USER pOfflineGuildUser = lpGuildOfflineUserNode->pData;
						
						if(!pOfflineGuildUser)
						{
							lpGuildOfflineUserNode = lpGuildOfflineUserNode->pNext;
							continue;
						}

						if(pOfflineGuildUser->byRank==1)
						{
							lpGuildOfflineUserNode = lpGuildOfflineUserNode->pNext;
							continue;
						}

						if(lpMasterOfflineUser==NULL)
						{
							lpMasterOfflineUser = pOfflineGuildUser;
						}
						else
						{
							if(lpMasterOfflineUser->byRank>pOfflineGuildUser->byRank)
							{
								lpMasterOfflineUser = pOfflineGuildUser;
							}
							else if(lpMasterOfflineUser->byRank == pOfflineGuildUser->byRank &&
									lpMasterOfflineUser->dwLevel < pOfflineGuildUser->dwLevel)
							{
								lpMasterOfflineUser = pOfflineGuildUser;
							}
						}
						
						lpGuildOfflineUserNode = lpGuildOfflineUserNode->pNext;
					}						

					if(!pMasterUser && !lpMasterOfflineUser)
						break;

					if(pMasterUser)
					{						
						DUNGEON_DATA_EX* pMasterDungeon = NULL;

						if(pMasterUser)
						{
							pMasterDungeon = g_pDungeonTable->GetDungeonInfo((WORD)pMasterUser->m_dwDungeonID);	

							if(!pMasterDungeon)
								break;
						}
								
						lpGuildOfflineUser->byRank	= pMasterUser->m_byRank;
						pMasterUser->m_byRank		= 1;
						
						wsprintf(szQuery, "Update CharacterInfo Set Rank=%u Where Char_Index = %u", lpGuildOfflineUser->byRank, lpGuildOfflineUser->dwUserIndex);
						g_pDb->THExecuteSQL(szQuery, TRUE, QUERY_TYPE_GUILD_USER, NULL, (BYTE)GAME_DB);
						wsprintf(szQuery, "Update CharacterInfo Set Rank=%u Where Char_Index = %u", pMasterUser->m_byRank, pMasterUser->m_dwUserIndex);
						g_pDb->THExecuteSQL(szQuery, TRUE, QUERY_TYPE_GUILD_USER, NULL, (BYTE)GAME_DB);

						WSTDS_GUILD_INFO pDsServerPacket;
						memset(pDsServerPacket.szGuildName, 0, sizeof(pDsServerPacket.szGuildName));
						memset(pDsServerPacket.szNickName, 0, sizeof(pDsServerPacket.szNickName));
						pDsServerPacket.dwUserIndex		= pMasterUser->m_dwUserIndex;
						pDsServerPacket.dwGuildId		= pMasterUser->m_dwGuildId;
						pDsServerPacket.byRank			= pMasterUser->m_byRank;
						pDsServerPacket.byType			= pMasterUser->m_byType;
						__lstrcpyn(pDsServerPacket.szGuildName, pMasterUser->m_szGuildName, MAX_GUILD_REAL_NAME_LENGTH);
						__lstrcpyn(pDsServerPacket.szNickName, pMasterUser->m_szNickName, MAX_CHARACTER_REAL_NICKNAME_LENGTH);		
						g_pNet->SendToServer(pMasterDungeon->m_pServer->dwConnectionIndex, (char*)&pDsServerPacket, pDsServerPacket.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);

						WSTC_USER_GUILDMASTER pSendPacket; 
						memset(pSendPacket.szName, 0, sizeof(pSendPacket.szName));
						memset(pSendPacket.szMasterName, 0, sizeof(pSendPacket.szMasterName));
						__lstrcpyn(pSendPacket.szName, lpGuildOfflineUser->szName, MAX_CHARACTER_NAME_REAL_LENGTH);
						__lstrcpyn(pSendPacket.szMasterName, pMasterUser->m_szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);		
						pSendPacket.dwUserIndex			= lpGuildOfflineUser->dwUserIndex;
						pSendPacket.dwMasterUserIndex	= pMasterUser->m_dwUserIndex;
						pSendPacket.byRank				= lpGuildOfflineUser->byRank;
						pSendPacket.byMasterRank		= pMasterUser->m_byRank;

						ListNode<CWorldUser>* lpGuildUserNode = lpGuildTable->pGuildUserHash->GetHead();		
																
						while(lpGuildUserNode)
						{
							CWorldUser* pGuildUser = lpGuildUserNode->pData;

							if(!pGuildUser)
							{
								lpGuildUserNode = lpGuildUserNode->pNext;
								continue;
							}
							g_pNet->SendToUser(pGuildUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
							lpGuildUserNode = lpGuildUserNode->pNext;
						}
					}
					else if(lpMasterOfflineUser)
					{
						lpGuildOfflineUser->byRank	= lpMasterOfflineUser->byRank;
						lpMasterOfflineUser->byRank	= 1;
						
						wsprintf(szQuery, "CZP_GUILD_CHANGE 1, 0, %u, '', '', 0, %u", lpGuildOfflineUser->dwUserIndex, lpGuildOfflineUser->byRank);
						g_pDb->THExecuteSQL(szQuery, TRUE, QUERY_TYPE_GUILD_USER, NULL, (BYTE)GAME_DB);
						wsprintf(szQuery, "CZP_GUILD_CHANGE 1, 0, %u, '', '', 0, %u", lpMasterOfflineUser->dwUserIndex, lpMasterOfflineUser->byRank);
						g_pDb->THExecuteSQL(szQuery, TRUE, QUERY_TYPE_GUILD_USER, NULL, (BYTE)GAME_DB);
						
						WSTC_USER_GUILDMASTER pSendPacket; 
						memset(pSendPacket.szName, 0, sizeof(pSendPacket.szName));
						memset(pSendPacket.szMasterName, 0, sizeof(pSendPacket.szMasterName));
						__lstrcpyn(pSendPacket.szName, lpGuildOfflineUser->szName, MAX_CHARACTER_NAME_REAL_LENGTH);
						__lstrcpyn(pSendPacket.szMasterName, lpMasterOfflineUser->szName, MAX_CHARACTER_NAME_REAL_LENGTH);		
						pSendPacket.dwUserIndex			= lpGuildOfflineUser->dwUserIndex;
						pSendPacket.dwMasterUserIndex	= lpMasterOfflineUser->dwUserIndex;
						pSendPacket.byRank				= lpGuildOfflineUser->byRank;
						pSendPacket.byMasterRank		= lpMasterOfflineUser->byRank;

						ListNode<CWorldUser>* lpGuildUserNode = lpGuildTable->pGuildUserHash->GetHead();		
																
						while(lpGuildUserNode)
						{
							CWorldUser* pGuildUser = lpGuildUserNode->pData;

							if(!pGuildUser)
							{
								lpGuildUserNode = lpGuildUserNode->pNext;
								continue;
							}
							g_pNet->SendToUser(pGuildUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
							lpGuildUserNode = lpGuildUserNode->pNext;
						}
					}
				}

				break;
			}

			lpOfflineUserPos = lpOfflineUserPos->pNext;
		}

		lpTablePos = lpTablePos->pNext;
	}
	
	END_FUNCTION_PROFILE();
}

void __stdcall ShutDownServer(DWORD dwVal)
{
	TerminateServer();	
}


void ConnectToLoginAgentServer()
{
	g_pNet->ConnectToServerWithServerSide(g_pThis->GetIPForAgentServer(), g_pThis->GetPortForAgentServer(), SuccessToConnectLoginAgentServer, FailToConnectLoginAgentServer, NULL);
}


void ListenForUserside()
{
	if(g_pThis->m_bUserListening)
	{
		Log(LOG_JUST_DISPLAY, "@ Userside already listening!");
	}
	else
	{
		if(!g_pNet->StartServerWithUserSide(g_pThis->GetIPForUser(), g_pThis->GetPortForUser()))
		{
			Log(LOG_NORMAL, "@ Can Not Listen For Userside! IP=\"%s\", Port=%d",g_pThis->GetIPForUser(), g_pThis->GetPortForUser());
			return;
		}

		g_pThis->m_bUserListening = TRUE;
		Log(LOG_JUST_DISPLAY, "@ Now Listening... (For Userside(%d))", g_pThis->GetPortForUser());
		ReportToCMListener(2, &g_MachineName);
	}
}

void ListenForServerside()
{
	if(g_pThis->m_bServerListening)
	{
		Log(LOG_JUST_DISPLAY, "@ Serverside already listening!");
	}
	else
	{
		if(!g_pNet->StartServerWithServerSide(g_pThis->GetIPForServer(), g_pThis->GetPortForServer()))
		{
			Log(LOG_NORMAL, "@ Can Not Listen For Serverside! IP=\"%s\", Port=%d",g_pThis->GetIPForServer(), g_pThis->GetPortForServer());
			return;
		}

		g_pThis->m_bServerListening = TRUE;
		Log(LOG_JUST_DISPLAY, "@ Now Listening... (For Serverside(%d))", g_pThis->GetPortForServer());
		ConnectToLoginAgentServer();
	}		
}

bool ConnectToDBServer()
{
	char szDbName[255]={0,};

	// Game DB
	GAME_DB = (BYTE)g_pDb->CreateDBConnection();
	Log(LOG_JUST_DISPLAY, "@ Connecting SQL Server(Game DB)...");	

	wsprintf( szDbName, "%s%s", DEFAULT_GAME_DB, g_pThis->GetServerSetCode() );	

	if(g_pDb->Connect(	g_pThis->GetIPForGameDB(), szDbName, 
						g_DBInfo.Get(DT_GAME_DB,DBIT_ID), 
						g_DBInfo.Get(DT_GAME_DB,DBIT_PW), 20, /*10, FALSE,*/ (BYTE)GAME_DB) < 0)
	{
		Log(LOG_IMPORTANT, "@ Can not connect SQL Server(Game DB)! IP=\"%s\"", g_pThis->GetIPForGameDB());
		return false;
	}


	Log(LOG_JUST_DISPLAY, "@ SQL Server(Game DB) Connected!(%s)", g_pThis->GetIPForGameDB());

	g_pDb->SetDefaultDBIndex((BYTE)GAME_DB);	
	
	//Total DB
	TOTAL_DB = (BYTE)g_pDb->CreateDBConnection();
	Log(LOG_JUST_DISPLAY, "@ Connecting SQL Server(System DB)...");
	
	if(g_pDb->Connect(	g_pThis->GetIPForTotalDB(), 
						g_DBInfo.Get(DT_TOTAL_DB, DBIT_CATALOG),
						g_DBInfo.Get(DT_TOTAL_DB, DBIT_ID), 
						g_DBInfo.Get(DT_TOTAL_DB, DBIT_PW), 
						20, /*10, FALSE,*/ (BYTE)TOTAL_DB) < 0)
	{
		Log(LOG_IMPORTANT, "@ Can not connect SQL Server(TOTAL_DB)! IP=\"%s\"",g_pThis->GetIPForTotalDB());
		return false;
	}

	Log(LOG_JUST_DISPLAY, "@ SQL Server(Total DB) Connected!(%s)", g_pThis->GetIPForTotalDB());

	//Member DB
	MEMBER_DB = (BYTE)g_pDb->CreateDBConnection();

	Log(LOG_JUST_DISPLAY, "@ Connecting SQL Server(Member DB)...");
	
	if(g_pDb->Connect(g_pThis->GetIPForMemberDB(), 
		g_DBInfo.Get(DT_MEMBER_DB, DBIT_CATALOG),
		g_DBInfo.Get(DT_MEMBER_DB, DBIT_ID),
		g_DBInfo.Get(DT_MEMBER_DB, DBIT_PW),
		20, /*10, FALSE,*/ (BYTE)MEMBER_DB) < 0)
	{
		Log(LOG_IMPORTANT, "@ Can not connect SQL Server(MEMBER_DB)! IP=\"%s\"",g_pThis->GetIPForMemberDB());
		return false;
	}

	Log(LOG_JUST_DISPLAY, "@ SQL Server(Member DB) Connected!(%s)", g_pThis->GetIPForMemberDB());	

	if(	IS_ABLE_NATION( NC_TAIWAN ) )
	{
		if (TRUE == g_bEnableChatLog)
		{
			char szDBName[ 0xff ]={0,};

			wsprintf(szDBName, "%s%d", g_DBInfo.Get(DT_LOG_DB, DBIT_CATALOG), (BYTE)atoi(g_pThis->GetServerSetCode()) );

			//Log DB
			LOG_DB = (BYTE)g_pLogDb->CreateDBConnection();
			Log(LOG_JUST_DISPLAY, "@ Connecting SQL Server(Member DB)...");
			if(g_pLogDb->Connect(g_pThis->GetIPForLogDB(), szDBName, 
				g_DBInfo.Get(DT_LOG_DB, DBIT_ID),
				g_DBInfo.Get(DT_LOG_DB, DBIT_PW),
				20, 10, FALSE, LOG_DB) < 0)
			{
				Log(LOG_NORMAL, "@ Can not connect SQL Server(LOG_DB)! IP=\"%s\"",g_pThis->GetIPForLogDB());
				return 0;
			}
			Log(LOG_JUST_DISPLAY, "@ SQL Server(LOG DB) Connected!(%s)", g_pThis->GetIPForLogDB());
		}
	} // if(	IS_ABLE_NATION( NC_TAIWAN ) )
	
	return true;
}

void KeyHelp()
{
	Log( LOG_JUST_DISPLAY, "													               " );
	Log( LOG_JUST_DISPLAY, "-------------------------------------------------------------------" );
	Log( LOG_JUST_DISPLAY, "                World Server - Function Key Help	               " );
	Log( LOG_JUST_DISPLAY, "-------------------------------------------------------------------" );
	Log( LOG_JUST_DISPLAY, "[F1] : Funtion Key Help" );
	Log( LOG_JUST_DISPLAY, "[F2] : Show Current (Login / Connection) User Number" );
	Log( LOG_JUST_DISPLAY, "[F3] : Show Current All Server Status (Except Worldserver)" );
	Log( LOG_JUST_DISPLAY, "[F5] : Toggle User Accept Status" );
	Log( LOG_JUST_DISPLAY, "[F12]: Terminate All Other Server");
	Log( LOG_JUST_DISPLAY, "[ESC]: Quit Server");
	Log( LOG_JUST_DISPLAY, "-------------------------------------------------------------------" );
}

void __stdcall TerminateAllServer(DWORD dwVal)
{
	WSTS_SERVER_TERMINATE packet;
	g_pServerTable->BroadCast((char*)&packet, packet.GetPacketSize() );
}

void __stdcall ShowAllServerStatus(DWORD dwVal)
{
	g_pServerTable->ShowAllServerStatus();

	Log(LOG_JUST_DISPLAY, "Exp Gain : %d", g_pThis->m_dwGLOBAL_EXP_GAIN*100);	
	Log(LOG_JUST_DISPLAY, "Magic Item Drop Rate : %d", g_pThis->m_GLOBAL_MAGIC_FIND_PBT*100);
	Log(LOG_JUST_DISPLAY, "Party Bonus Exp Gain : %d", g_pThis->m_dwGLOBAL_PARTY_BONUS_EXP_GAIN * 100);
}

void __stdcall RefreshAllDungeonServerData(DWORD dwVal)
{
	WSTS_REFRESH_ALL_DUNGEON_SERVER_DATA packet;
	g_pServerTable->BroadCast((char*)&packet, packet.GetPacketSize() );

	Log(LOG_NORMAL, "Refresh Message Sended to All Dungeon Server!");
}

BOOL IsDateExpired(DBTIMESTAMP *pEndDate)
{
	if(pEndDate->year < g_CurDate.wYear)
		return TRUE;
	else if(pEndDate->month < g_CurDate.wMonth)
		return TRUE;
	else if(pEndDate->day < g_CurDate.wDay)
		return TRUE;
	else
		return FALSE;
}

BOOL ChangeBillingType(CWorldUser* pUser, BILLING_INFO* pBillInfo)
{
	pBillInfo->bCurBillingOrderIndex++;

	if(pBillInfo->bCurBillingOrderIndex >= MAX_BILLING_ORDER_COUNT)		return FALSE;
	if(!pBillInfo->BillingOrder[ pBillInfo->bCurBillingOrderIndex ])	return FALSE;	//더이상 결제할 수 있는 방법이 없다. 

	WSTC_BILLING_INFO	packet;
	memset(&packet.BillingEndDate, 0, sizeof(packet.BillingEndDate));	//종량제에서는 EndDate 안쓴다. 
	packet.dwRemainTime	= 0;

	packet.bInfoType = BILLING_INFO_START_BILLING;
	packet.bCurBillingType = pBillInfo->BillingOrder[ pBillInfo->bCurBillingOrderIndex ];

	switch(pUser->m_BillingInfo.BillingOrder[ pUser->m_BillingInfo.bCurBillingOrderIndex ])
	{
		case USER_BILLING_1HOUR_FREE:		packet.dwRemainTime = pBillInfo->dwTimeFor1HourFree;		break;
		case USER_BILLING_METER_RATE:		packet.dwRemainTime = pBillInfo->dwTimePerUserMeterRate;	break;
		case PCROOM_BILLING_METER_RATE:		packet.dwRemainTime = pBillInfo->dwTimeForPCRoomMeteRate;	break;
		case PCROOM_BILLING_FLAT_RATE:		packet.BillingEndDate = pBillInfo->PCFlatEndTime;			break;
		case USER_BILLING_FLAT_RATE:		packet.BillingEndDate = pBillInfo->UserFlatEndTime;			break;
	}

	g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
	
	return TRUE;	//결제타입변경 성공 
}

void BillingProcess()
{
	DWORD dwSec = 0;

	POSITION_ pos_ = g_pUserTable->m_pTotalUserList->GetHeadPosition();
	while(pos_)
	{
		CWorldUser* pUser = (CWorldUser*)g_pUserTable->m_pTotalUserList->GetNext(pos_);
		
		switch(pUser->m_BillingInfo.BillingOrder[ pUser->m_BillingInfo.bCurBillingOrderIndex ])
		{

		case PCROOM_BILLING_FLAT_RATE:
			{
				if( IsDateExpired(&pUser->m_BillingInfo.PCFlatEndTime) )
				{
					//더이상 결제할 수 있는 수단이 없다면 게임을 종료 시켜야된다. 
					if(!ChangeBillingType(pUser, &pUser->m_BillingInfo))
					{
						//check
					}
					pUser->m_dwBillingStartTick = g_dwCurTick;
				}
			}
			break;

		case USER_BILLING_FLAT_RATE:
			{
				if( IsDateExpired(&pUser->m_BillingInfo.UserFlatEndTime) )
				{
					//더이상 결제할 수 있는 수단이 없다면 게임을 종료 시켜야된다. 
					if(!ChangeBillingType(pUser, &pUser->m_BillingInfo))
					{
						//check
					}
					pUser->m_dwBillingStartTick = g_dwCurTick;
				}
			}
			break;		

		case USER_BILLING_1HOUR_FREE:
			{
				//과금 시작한 이후의 경과된 시간을 구함 
				if(g_dwCurTick < pUser->m_dwBillingStartTick)		
				{
					dwSec = 0xffffffff - pUser->m_dwBillingStartTick;	//tick이 4byte값 다 돌아간 경우 
					dwSec += g_dwCurTick;
				}
				else
				{
					dwSec = g_dwCurTick - pUser->m_dwBillingStartTick;	//정상적인 경우 
				}

				//과금 시작한지 5분이상 되었으면 5분을 차감하고, 
				//5분이 안되었을경우는 경과한 시간만큼만 차감 하자 
				if(dwSec >= 300000)	
					dwSec = 300000;

				if(pUser->m_BillingInfo.dwTimeFor1HourFree < dwSec)
				{
					pUser->m_BillingInfo.dwTimeFor1HourFree = 0;
					//더이상 결제할 수 있는 수단이 없다면 게임을 종료 시켜야된다. 
					if(!ChangeBillingType(pUser, &pUser->m_BillingInfo))
					{
						//check
					}
					pUser->m_dwBillingStartTick = g_dwCurTick;
				}
				else
				{
					pUser->m_BillingInfo.dwTimeFor1HourFree -= dwSec;
					
					if(pUser->m_BillingInfo.dwTimeFor1HourFree <= 600000)	//남은시간이 10분이하일때 
					{
						//몇분후에 종료된다는 정보를 클라이언트에게 날려준다. 
						WSTC_BILLING_INFO	packet;
						memset(&packet.BillingEndDate, 0, sizeof(packet.BillingEndDate));	//종량제에서는 EndDate 안쓴다. 

						packet.bCurBillingType = pUser->m_BillingInfo.BillingOrder[ pUser->m_BillingInfo.bCurBillingOrderIndex ];
						packet.bInfoType = BILLING_INFO_SMALL_TIME;
						packet.dwRemainTime = pUser->m_BillingInfo.dwTimeFor1HourFree;
						g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
					}
				}			
			}
			break;

		case USER_BILLING_METER_RATE:
			{
				//과금 시작한 이후의 경과된 시간을 구함 
				if(g_dwCurTick < pUser->m_dwBillingStartTick)		
				{
					dwSec = 0xffffffff - pUser->m_dwBillingStartTick;	//tick이 4byte값 다 돌아간 경우 
					dwSec += g_dwCurTick;
				}
				else
				{
					dwSec = g_dwCurTick - pUser->m_dwBillingStartTick;	//정상적인 경우 
				}

				//과금 시작한지 5분이상 되었으면 5분을 차감하고, 
				//5분이 안되었을경우는 경과한 시간만큼만 차감 하자 
				if(dwSec >= 300000)	
					dwSec = 300000;

				if(pUser->m_BillingInfo.dwTimePerUserMeterRate < dwSec)
				{
					pUser->m_BillingInfo.dwTimePerUserMeterRate = 0;
					//더이상 결제할 수 있는 수단이 없다면 게임을 종료 시켜야된다. 
					if(!ChangeBillingType(pUser, &pUser->m_BillingInfo))
					{
						//check
					}
					pUser->m_dwBillingStartTick = g_dwCurTick;
				}
				else
				{
					pUser->m_BillingInfo.dwTimePerUserMeterRate -= dwSec;

					if(pUser->m_BillingInfo.dwTimePerUserMeterRate <= 600000)	//남은시간이 10분이하일때 
					{
						//몇분후에 종료된다는 정보를 클라이언트에게 날려준다. 
						WSTC_BILLING_INFO	packet;
						memset(&packet.BillingEndDate, 0, sizeof(packet.BillingEndDate));	//종량제에서는 EndDate 안쓴다. 

						packet.bCurBillingType = pUser->m_BillingInfo.BillingOrder[ pUser->m_BillingInfo.bCurBillingOrderIndex ];
						packet.bInfoType = BILLING_INFO_SMALL_TIME;
						packet.dwRemainTime = pUser->m_BillingInfo.dwTimePerUserMeterRate;
						g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
					}
				}
			}
			break;

		case PCROOM_BILLING_METER_RATE:
			{
				//과금 시작한 이후의 경과된 시간을 구함 
				if(g_dwCurTick < pUser->m_dwBillingStartTick)		
				{
					dwSec = 0xffffffff - pUser->m_dwBillingStartTick;	//tick이 4byte값 다 돌아간 경우 
					dwSec += g_dwCurTick;
				}
				else
				{
					dwSec = g_dwCurTick - pUser->m_dwBillingStartTick;	//정상적인 경우
				}

				//과금 시작한지 5분이상 되었으면 5분을 차감하고, 
				//5분이 안되었을경우는 경과한 시간만큼만 차감 하자 
				if(dwSec >= 300000)	
					dwSec = 300000;

				if(pUser->m_BillingInfo.dwTimeForPCRoomMeteRate < dwSec)
				{
					pUser->m_BillingInfo.dwTimeForPCRoomMeteRate = 0;
					//더이상 결제할 수 있는 수단이 없다면 게임을 종료 시켜야된다. 
					if(!ChangeBillingType(pUser, &pUser->m_BillingInfo))
					{
						//check
					}
					pUser->m_dwBillingStartTick = g_dwCurTick;
				}
				else
				{
					pUser->m_BillingInfo.dwTimeForPCRoomMeteRate -= dwSec;

					if(pUser->m_BillingInfo.dwTimeForPCRoomMeteRate <= 600000)	//남은시간이 10분이하일때 
					{
						//몇분후에 종료된다는 정보를 클라이언트에게 날려준다. 
						WSTC_BILLING_INFO	packet;
						memset(&packet.BillingEndDate, 0, sizeof(packet.BillingEndDate));	//종량제에서는 EndDate 안쓴다. 

						packet.bCurBillingType = pUser->m_BillingInfo.BillingOrder[ pUser->m_BillingInfo.bCurBillingOrderIndex ];
						packet.bInfoType = BILLING_INFO_SMALL_TIME;
						packet.dwRemainTime = pUser->m_BillingInfo.dwTimeForPCRoomMeteRate;
						g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
					}
				}
			}
			break;

		case USER_BILLING_ABSOLUTELY_FREE:	continue;

		}
	}	
}

void __stdcall TimerPer5Minute(DWORD dwVal)
{
	START_FUNCTION_PROFILE();

	// 5분 주기로 전역변수 날짜를 갱신해 준다. 
	GetLocalTime(&g_CurDate);	
		
	//---------  Billing Check ---------------
	if( IS_ABLE_NATION(NC_TAIWAN) )//hwoarang
	{
		if( IS_ABLE_SERVICE_TYPE( ST_BILLING ) )
		{
			BillingProcess();
		}
	}
	
	// 쓰레기 Accept 정보 제거 작업  
	DWORD		dwCount		= 0;
	POSITION_	pos			= g_pAcceptTable->m_pAcceptedList->GetHeadPosition();
	DWORD		dwCurTick	= g_dwCurTick;
	DWORD		dwSecond	= 0;

	while(pos)
	{
		ACCEPT_USER* pAccept = (ACCEPT_USER*)g_pAcceptTable->m_pAcceptedList->GetNext(pos);
		
		if(dwCurTick < pAccept->dwAcceptedTick)		
		{
			// 4Byte 값 돌아간 경우 	
			dwSecond = 0xffffffff - pAccept->dwAcceptedTick;
			dwSecond += dwCurTick;
		}
		else
		{
			// 정상적인 경우 
			dwSecond = dwCurTick - pAccept->dwAcceptedTick;	
		}

		if( dwSecond > 60000 )	
		{
			// 1분이상 된 Accept이면 쓰레기로 간주 
			g_pAcceptTable->Remove(pAccept);
			Log(LOG_NORMAL, "GarbageAccept Removed! (Char_Index:%d)", pAccept->dwChar_Index);
			dwCount++;
		}
	}
	
	// 길드 경험치 저장 5분마다 저장.
	if(g_pGuildTableHash)
	{
		ListNode<GUILD_TABLE>* lpTablePos = g_pGuildTableHash->GetHead();

		while(lpTablePos)
		{
			LP_GUILD_TABLE lpGuildTable = lpTablePos->pData;

			if(NULL == lpGuildTable)	continue;

			if(lpGuildTable->dwPrevGuildExp != lpGuildTable->sGuild.dwExp)
			{
				char szQuery[0xff] = {0,};
				wsprintf(szQuery, "Update GuildInfo Set Level= %u, Exp = %u Where Id = %u"
					, lpGuildTable->sGuild.wLevel, lpGuildTable->sGuild.dwExp, lpGuildTable->sGuild.dwGuildId);

				g_pDb->THExecuteSQL(szQuery, FALSE, 0, NULL, GAME_DB);

				lpGuildTable->dwPrevGuildExp = lpGuildTable->sGuild.dwExp;
			}							

			lpTablePos = lpTablePos->pNext;
		}
	}
	
	// 길드전 자동 종료 
	pos = g_pGuildWarTimeList->GetHeadPosition();

	while(pos)
	{
		LP_GUILDWAR_TIME lpGuildWarTime = (LP_GUILDWAR_TIME)g_pGuildWarTimeList->GetNext(pos);

		if(NULL == lpGuildWarTime)	continue;

		struct tm	when;
		time_t		now;

		time(&now);
		when = *localtime(&now);

		int nTime	= (int)difftime(now, lpGuildWarTime->sTime);
		int nHou	= (nTime /60) / 60;

		if(nHou < GUILDWAR_TIME) break;

		LP_GUILD_LIST	lpGuildList		= g_pGuildWarHash->GetData(lpGuildWarTime->war_time.dwLowId);
		LP_GUILD_LIST	lpGuildListDst	= g_pGuildWarHash->GetData(lpGuildWarTime->war_time.dwHiId);

		LP_GUILD_TABLE	lpGuildTable	= g_pGuildTableHash->GetData(lpGuildWarTime->war_time.dwLowId);
		LP_GUILD_TABLE	lpGuildTableDst	= g_pGuildTableHash->GetData(lpGuildWarTime->war_time.dwHiId);

		char	szQuery[0xff] = {0,};
		char	szWinGuildName[MAX_GUILD_NAME_LENGTH] = {0,};
		char	szLoseGuildName[MAX_GUILD_NAME_LENGTH]= {0,};	

		memset(szWinGuildName, 0, sizeof(szWinGuildName));
		memset(szLoseGuildName, 0, sizeof(szWinGuildName));	
		
		if(NULL == lpGuildList || NULL == lpGuildListDst || NULL == lpGuildTable || NULL == lpGuildTableDst)
			continue;

		for(int i = 0; i < MAX_GUILD_WAR; i++)
		{
			if(lpGuildWarTime->war_time.dwHiId != lpGuildList->sGuildNode[i].dwGuildId)
				continue;

			for(int j = 0; j < MAX_GUILD_WAR; j++)
			{
				if(lpGuildListDst->sGuildNode[j].dwGuildId != lpGuildWarTime->war_time.dwLowId)
					continue;

				if(lpGuildList->sGuildNode[i].wPoint1 != lpGuildListDst->sGuildNode[j].wPoint2)
					continue;

				if(lpGuildList->sGuildNode[i].wPoint2 != lpGuildListDst->sGuildNode[j].wPoint1)
					continue;

				if(lpGuildList->sGuildNode[i].wPoint1 > lpGuildList->sGuildNode[i].wPoint2)
				{
					lpGuildTable->sGuild.dwPoint	+= 3;
					lpGuildTableDst->sGuild.dwPoint	+= 1;
					__lstrcpyn(szWinGuildName, lpGuildTable->sGuild.szGuildName, MAX_GUILD_REAL_NAME_LENGTH);
					__lstrcpyn(szLoseGuildName, lpGuildTableDst->sGuild.szGuildName, MAX_GUILD_REAL_NAME_LENGTH);
				}
				else if(lpGuildList->sGuildNode[i].wPoint1 < lpGuildList->sGuildNode[i].wPoint2)
				{
					lpGuildTable->sGuild.dwPoint	+= 1;
					lpGuildTableDst->sGuild.dwPoint	+= 3;
					__lstrcpyn(szLoseGuildName, lpGuildTable->sGuild.szGuildName, MAX_GUILD_REAL_NAME_LENGTH);
					__lstrcpyn(szWinGuildName, lpGuildTableDst->sGuild.szGuildName, MAX_GUILD_REAL_NAME_LENGTH);
				}
							
				POSITION_ pos = g_pRankList->GetHeadPosition();

				while(pos)
				{
					LP_GUILDWAR_RANK lpGuildWarRank = (LP_GUILDWAR_RANK)g_pRankList->GetNext(pos);

					if(lpGuildWarRank)
					{
						if(lpGuildWarRank->dwGuildId == lpGuildTable->sGuild.dwGuildId)
							lpGuildWarRank->dwPoint = lpGuildTable->sGuild.dwPoint;
						else if(lpGuildWarRank->dwGuildId == lpGuildTableDst->sGuild.dwGuildId)
							lpGuildWarRank->dwPoint = lpGuildTableDst->sGuild.dwPoint;
					}
				}
											
				// DB Update //
				wsprintf(szQuery, "CZP_GUILDLIST_POINT %u, %u", lpGuildTable->sGuild.dwGuildId, lpGuildTable->sGuild.dwPoint);
				g_pDb->THExecuteSQL(szQuery, FALSE, 0, NULL, (BYTE)GAME_DB);

				wsprintf(szQuery, "CZP_GUILDLIST_POINT %u, %u", lpGuildTableDst->sGuild.dwGuildId, lpGuildTableDst->sGuild.dwPoint);
				g_pDb->THExecuteSQL(szQuery, FALSE, 0, NULL, (BYTE)GAME_DB);									

				// 순위 재검색 //
				POSITION_ posRank = g_pRankList->GetHeadPosition();

				DWORD dwPrevGuildId = 0;

				while(posRank)
				{
					POSITION_ posPrev	= posRank;
					POSITION_ posDelete	= posRank;
					POSITION_ posChange	= NULL; 

					LP_GUILDWAR_RANK lpGuildWarRank = (LP_GUILDWAR_RANK)g_pRankList->GetNext(posRank);
					if(NULL == lpGuildWarRank)	continue;

					if(!dwPrevGuildId)
						dwPrevGuildId = lpGuildWarRank->dwGuildId;

					while(posPrev)
					{
						POSITION_ posGuild = posPrev;

						LP_GUILDWAR_RANK lpGuildWarPrev = (LP_GUILDWAR_RANK)g_pRankList->GetPrev(posPrev);
						if(NULL == lpGuildWarPrev)	continue;

						if(lpGuildWarPrev->dwGuildId != lpGuildWarRank->dwGuildId)
						{
							if(lpGuildWarPrev->dwPoint < lpGuildWarRank->dwPoint)
								posChange = posGuild;
							else if(lpGuildWarPrev->dwPoint == lpGuildWarRank->dwPoint && lpGuildWarPrev->dwExp < lpGuildWarRank->dwExp)
								posChange = posGuild;							
						}					
					}

					if(posChange)
					{
						g_pRankList->RemoveAt(posDelete);
						g_pRankList->InsertBefore(posChange, lpGuildWarRank);
					}					
				}										

				// 1등이 변경되면 던전서버에 알려준다 //
				posRank = g_pRankList->GetHeadPosition();
				if(NULL == posRank)	continue;

				LP_GUILDWAR_RANK lpGuildWarRank = (LP_GUILDWAR_RANK)g_pRankList->GetNext(posRank);
				if(lpGuildWarRank)
				{
					if(dwPrevGuildId != 0 && lpGuildWarRank->dwGuildId != dwPrevGuildId)
					{
						WSTDS_GUILD_RANK pRankPacket;
						pRankPacket.dwGuildId = lpGuildWarRank->dwGuildId;												
						g_pServerTable->BroadCast((char*)&pRankPacket, pRankPacket.GetPacketSize());												
					}
				}
				
				LP_GUILD_TABLE lpGuildTableRank = g_pGuildTableHash->GetData(dwPrevGuildId);
				if(lpGuildTableRank)
				{													
					ListNode<CWorldUser>* lpGuildUserRankNode = lpGuildTableRank->pGuildUserHash->GetHead();

					while(lpGuildUserRankNode)
					{
						CWorldUser* pGuildUser = lpGuildUserRankNode->pData;

						if(NULL == pGuildUser)	continue;

						WSTDS_GUILDUSER_RANK pGuildUserRankPacket;
						pGuildUserRankPacket.dwUserIndex	= pGuildUser->m_dwUserIndex;
						pGuildUserRankPacket.dwGuildId		= pGuildUser->m_dwGuildId;
						pGuildUserRankPacket.byGuildFlag	= 0;
						pGuildUserRankPacket.byChk			= 0;

						DUNGEON_DATA_EX* pDungeon = g_pDungeonTable->GetDungeonInfo((WORD)pGuildUser->m_dwDungeonID);

						if(pDungeon)
							g_pNet->SendToServer(pDungeon->m_pServer->dwConnectionIndex
							, (char*)&pGuildUserRankPacket, pGuildUserRankPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
						
						lpGuildUserRankNode = lpGuildUserRankNode->pNext;
					}
				}

				LP_GUILD_TABLE lpGuildTableRankDst = g_pGuildTableHash->GetData(lpGuildWarRank->dwGuildId);
				if(lpGuildTableRankDst)
				{													
					ListNode<CWorldUser>* lpGuildUserRankNode = lpGuildTableRankDst->pGuildUserHash->GetHead();

					while(lpGuildUserRankNode)
					{
						CWorldUser* pGuildUser = lpGuildUserRankNode->pData;

						if(NULL == pGuildUser) continue;
					
						WSTDS_GUILDUSER_RANK pGuildUserRankPacket;
						pGuildUserRankPacket.dwUserIndex	= pGuildUser->m_dwUserIndex;
						pGuildUserRankPacket.dwGuildId		= pGuildUser->m_dwGuildId;
						pGuildUserRankPacket.byGuildFlag	= 1;
						pGuildUserRankPacket.byChk			= 0;

						DUNGEON_DATA_EX* pDungeon = g_pDungeonTable->GetDungeonInfo((WORD)pGuildUser->m_dwDungeonID);

						if(pDungeon)
							g_pNet->SendToServer(pDungeon->m_pServer->dwConnectionIndex
							, (char*)&pGuildUserRankPacket, pGuildUserRankPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

						lpGuildUserRankNode = lpGuildUserRankNode->pNext;
					}
				}				
				
				// 종료 패킷 
				WSTC_GUILDWAR_EXIT pExitPacket;
				pExitPacket.dwGuildId	= lpGuildList->sGuildNode[i].dwGuildId;
				pExitPacket.dwPoint		= lpGuildTable->sGuild.dwPoint;									
				
				if(lpGuildList->sGuildNode[i].wPoint1 > lpGuildList->sGuildNode[i].wPoint2)
					pExitPacket.byType = 0;
				else if(lpGuildList->sGuildNode[i].wPoint1 < lpGuildList->sGuildNode[i].wPoint2)
					pExitPacket.byType = 1;
				else
					pExitPacket.byType = 2;

				ListNode<CWorldUser>* lpGuildUserNode = lpGuildTable->pGuildUserHash->GetHead();		

				while(lpGuildUserNode)
				{
					CWorldUser* pGuildUser = lpGuildUserNode->pData;
					if(NULL == pGuildUser) continue;

					g_pNet->SendToUser(pGuildUser->m_dwConnectionIndex, (char*)&pExitPacket, pExitPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
					
					lpGuildUserNode = lpGuildUserNode->pNext;
				}			

				pExitPacket.dwGuildId	= lpGuildListDst->sGuildNode[j].dwGuildId;
				pExitPacket.dwPoint		= lpGuildTableDst->sGuild.dwPoint;									

				if(lpGuildListDst->sGuildNode[j].wPoint1 > lpGuildListDst->sGuildNode[j].wPoint2)
					pExitPacket.byType = 0;
				else if(lpGuildListDst->sGuildNode[j].wPoint1 < lpGuildListDst->sGuildNode[j].wPoint2)
					pExitPacket.byType = 1;
				else
					pExitPacket.byType = 2;
				
				ListNode<CWorldUser>* lpGuildUserNodeSub = lpGuildTableDst->pGuildUserHash->GetHead();		

				while(lpGuildUserNodeSub)
				{
					CWorldUser* pGuildUser = lpGuildUserNodeSub->pData;
					if(NULL == pGuildUser) continue;

					g_pNet->SendToUser(pGuildUser->m_dwConnectionIndex, (char*)&pExitPacket, pExitPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
					
					lpGuildUserNodeSub = lpGuildUserNodeSub->pNext;
				}

				// 전체 공지 
				WSTC_DGCHAT_USER pAllUserPacket;	
				pAllUserPacket.nIndex = 5;
				memset(pAllUserPacket.szDstId,	0, sizeof(pAllUserPacket.szDstId));
				memset(pAllUserPacket.szId,		0, sizeof(pAllUserPacket.szId));
				memset(pAllUserPacket.szMessage, 0, sizeof(pAllUserPacket.szMessage));																		

				if(lpGuildList->sGuildNode[i].wPoint1 > lpGuildList->sGuildNode[i].wPoint2)
					wsprintf(pAllUserPacket.szMessage, g_szGuildBattleText[1], szWinGuildName, szLoseGuildName, szWinGuildName, szWinGuildName);
				else if(lpGuildList->sGuildNode[i].wPoint1 < lpGuildList->sGuildNode[i].wPoint2)
					wsprintf(pAllUserPacket.szMessage, g_szGuildBattleText[1], szWinGuildName, szLoseGuildName, szWinGuildName, szWinGuildName);									
				else
					wsprintf(pAllUserPacket.szMessage, g_szGuildBattleText[2], lpGuildTable->sGuild.szGuildName, lpGuildTableDst->sGuild.szGuildName);									

				g_pNet->BroadcastUser((char*)&pAllUserPacket, pAllUserPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);							
				
				// 초기화 
				WSTDS_GUILDWAR_DELETE pServerPacket;
				pServerPacket.dwGuildId		= lpGuildTable->sGuild.dwGuildId;
				pServerPacket.dwGuildIdDst	= lpGuildTableDst->sGuild.dwGuildId;																
		
				pos = g_pDungeonTable->m_pDungeonList->GetHeadPosition();
				
				while(pos)
				{
					DUNGEON_DATA_EX* pDungeonDataEx = (DUNGEON_DATA_EX*)g_pDungeonTable->m_pDungeonList->GetNext(pos);

					if(pDungeonDataEx && pDungeonDataEx->m_pServer)
					{									
						g_pNet->SendToServer(pDungeonDataEx->m_pServer->dwConnectionIndex
							, (char*)&pServerPacket, pServerPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
					}								
				}

				memset(&lpGuildList->sGuildNode[i], 0, sizeof(lpGuildList->sGuildNode[i]));
				memset(&lpGuildListDst->sGuildNode[j], 0, sizeof(lpGuildListDst->sGuildNode[j]));

				DBBINDING* pBinding = g_pDb->THCreateParamInfo(1);
					
				GUILD_NODE	sGuildNode[MAX_GUILD_WAR];									
				memset(szQuery, 0, sizeof(szQuery));
				memset(sGuildNode, 0, sizeof(sGuildNode));										
				memcpy(sGuildNode, lpGuildList->sGuildNode, sizeof(sGuildNode));
																						
				pBinding[0].obValue		= 0;
				pBinding[0].cbMaxLen	= sizeof(GUILD_NODE)*MAX_GUILD_WAR;
				pBinding[0].wType		= DBTYPE_BYTES;											
				wsprintf(szQuery, "CZP_GUILDLIST_UPDATE %u, ?", lpGuildTable->sGuild.dwGuildId);
				g_pDb->THExecuteSQLByParam(szQuery, pBinding, sGuildNode, 1, (BYTE)GAME_DB);

				DBBINDING* pBindingDst = g_pDb->THCreateParamInfo(1);	
				
				memset(szQuery, 0, sizeof(szQuery));
				memset(sGuildNode, 0, sizeof(sGuildNode));										
				memcpy(sGuildNode, lpGuildListDst->sGuildNode, sizeof(sGuildNode));
																						
				pBindingDst[0].obValue	= 0;
				pBindingDst[0].cbMaxLen	= sizeof(GUILD_NODE)*MAX_GUILD_WAR;
				pBindingDst[0].wType	= DBTYPE_BYTES;											
				wsprintf(szQuery, "CZP_GUILDLIST_UPDATE %u, ?", lpGuildTableDst->sGuild.dwGuildId);
				g_pDb->THExecuteSQLByParam(szQuery, pBindingDst, sGuildNode, 1, (BYTE)GAME_DB);									

				SGUILDWAR_TIME sGuildWarTime;				
				GuildWarTime(&sGuildWarTime, lpGuildTable->sGuild.dwGuildId, lpGuildTableDst->sGuild.dwGuildId);

				pos = g_pGuildWarTimeList->GetHeadPosition();

				while(pos)
				{
					LP_GUILDWAR_TIME lpGuildWarTime = (LP_GUILDWAR_TIME)g_pGuildWarTimeList->GetNext(pos);

					if(lpGuildWarTime && lpGuildWarTime->n64Id==lpGuildWarTime->n64Id)
					{
						g_pGuildWarTimeList->RemoveAt(pos);
						LALFree(g_pGuildWarTimePool, lpGuildWarTime);
						break;
					}
				}																							
			}			
		}				
	}

	END_FUNCTION_PROFILE();
}


void __stdcall SendKeepAlivePacket(DWORD dwVal)
{
	START_FUNCTION_PROFILE();

#ifdef CHECK_KEEP_ALIVE
	if(	! IS_ABLE_SERVICE_TYPE(ST_DEVELOP) 
	{
		if(!g_pCDTable)	return;
		g_pCDTable->SendKeepAlivePacket();
		g_pNet->ResumeTimer(10);
	}
#endif

	END_FUNCTION_PROFILE();
}

void __stdcall CheckKeepAlive(DWORD dwVal)
{
	START_FUNCTION_PROFILE();

#ifdef CHECK_KEEP_ALIVE
	if(!IS_ABLE_SERVICE_TYPE(ST_DEVELOP) 
	{
		if(!g_pCDTable)	return;
		g_pCDTable->CheckKeepAlive();
		g_pCDTable->SetNextList();
		g_pNet->PauseTimer(10);	
	}
#endif

	END_FUNCTION_PROFILE();
}

void __stdcall StartServerWithUserSide(DWORD dwEventIndex)
{
	ListenForUserside();	
}

void __stdcall StartServerWithServerSide(DWORD dwEventIndex)
{
	ListenForServerside();	
}

void __stdcall ShowCurrentUserCount(DWORD dwEventIndex)
{
	Log(LOG_JUST_DISPLAY, "@ Total %d/%d (Login/Connection) User Connected.", g_pUserTable->GetCount(), g_pNet->GetConnectedUserNum());
}

void CmdCheckConnection(CWorldUser* pUser ,char* pMsg, DWORD dwLength)
{
	ECHO_KEEP_ALIVE* pPacket = (ECHO_KEEP_ALIVE*)pMsg;
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->bGameStatus, pPacket->bHeader, dwLength);
		return;
	}

	if(pUser->m_bKeepAliveCheckFlag)
	{
		pUser->m_bKeepAliveCheckFlag = 0;
	}
	else
	{
		DWORD dwTick = g_dwCurTick - pUser->m_dwCheckSendedTick;
		Log(LOG_NORMAL, "Not Checked User Answered! (Name:%s, pUser->m_bCheckSended:%d, CheckGap:%d, MapLoading:%d)", pUser->m_szCharacterName, pUser->m_bCheckSended, dwTick, pUser->m_bIsMapLoading);
	}
}

void __stdcall SendServerAlive(DWORD dwEventIndex)
{

}

void __stdcall CheckServerAlive(DWORD dwEventIndex)
{
	dwEventIndex;	
}

void InitNetwork()
{
	CUSTOM_EVENT ev[CUSTOM_EVENT_DEF_NUM];

	ev[0].dwPeriodicTime	= 0;
	ev[0].pEventFunc		= GetDBResult;

	ev[1].dwPeriodicTime	= 0;
	ev[1].pEventFunc		= ShowAllServerStatus;

	ev[2].dwPeriodicTime	= 0;
	ev[2].pEventFunc		= TerminateAllServer;

	ev[3].dwPeriodicTime	= 0;
	ev[3].pEventFunc		= RefreshAllDungeonServerData;	
	
	ev[4].dwPeriodicTime	= 5 * 1000;	
	ev[4].pEventFunc		= WorldMatchProc;//hwoarang

	ev[5].dwPeriodicTime	= 500;
	ev[5].pEventFunc		= TimerPerSec;	

	ev[6].dwPeriodicTime	= 5 * 1000;				// 5 sec!
	ev[6].pEventFunc		= OnChinaKickOut;
	
	ev[7].dwPeriodicTime	= 60 * 5 * 1000;		//5분마다 	
	ev[7].pEventFunc		= TimerPer5Minute;

	ev[8].dwPeriodicTime	= 20 * 1000;
	ev[8].pEventFunc		= CreateEventJudgment;
	
	if( IS_ABLE_NATION(NC_TAIWAN) )//hwoarang
	{
		ev[9].dwPeriodicTime	= 45 * 1000;			//45초마다 KeepAlive Packet을 Send하고 
	}
	else
	{
		ev[9].dwPeriodicTime	= 30 * 1000;			//30초마다 KeepAlive Packet을 Send하고 
	}
	
	ev[9].pEventFunc		= SendKeepAlivePacket;

	if( IS_ABLE_NATION(NC_TAIWAN) )//hwoarang
	{
		ev[10].dwPeriodicTime	= 30 * 1000;		//30초후에 응답이 있는지 없는지를 체크 
	}
	else
	{
		ev[10].dwPeriodicTime	= 15 * 1000;		//15초후에 응답이 있는지 없는지를 체크 
	}

	ev[10].pEventFunc		= CheckKeepAlive;

	ev[11].dwPeriodicTime	= 0;
	ev[11].pEventFunc		= StartServerWithUserSide;

	ev[12].dwPeriodicTime	= 0;
	ev[12].pEventFunc		= StartServerWithServerSide;

	ev[13].dwPeriodicTime	= 0;
	ev[13].pEventFunc		= ShowCurrentUserCount;

	ev[14].dwPeriodicTime	= 60 * 1000;		//1분마다 Echo를 보내고 
	ev[14].pEventFunc		= SendServerAlive;

	ev[15].dwPeriodicTime	= 10 * 1000;		//10초후에 Echo Check
	ev[15].pEventFunc		= CheckServerAlive;

	ev[16].dwPeriodicTime	= 0;
	ev[16].pEventFunc		= ShutDownServer;

	ev[17].dwPeriodicTime	= 0;
	ev[17].pEventFunc		= EnableDoubleExp;

	ev[18].dwPeriodicTime	= 0;
	ev[18].pEventFunc		= DisableDoubleExp;

	ev[19].dwPeriodicTime	= 0;
	ev[19].pEventFunc		= IncreaseMagicDropRate;

	ev[20].dwPeriodicTime	= 0;
	ev[20].pEventFunc		= DecreaseMagicDropRate;
	
	ev[21].dwPeriodicTime	= 0;
	ev[21].pEventFunc		= IncreaseExpGain;

	ev[22].dwPeriodicTime	= 0;
	ev[22].pEventFunc		= DecreaseExpGain;

	ev[23].dwPeriodicTime	= 1000 * 60 * 60 * 24;
	ev[23].pEventFunc		= GuildMaster;

	ev[24].dwPeriodicTime	= 0;
	ev[24].pEventFunc		= IncreasePartyBonusExpGain;

	ev[25].dwPeriodicTime	= 0;
	ev[25].pEventFunc		= DecreasePartyBonusExpGain;
	
	DESC_NETWORK desc;
	memset(&desc,0,sizeof(DESC_NETWORK));
	desc.OnAcceptUser						= OnAcceptUser;
	desc.OnAcceptServer						= OnAcceptServer;
	desc.OnDisconnectServer					= OnDisconnectServer;
	desc.OnDisconnectUser					= OnDisconnectUser;

	desc.dwMainMsgQueMaxBufferSize			= 5120000;	// 최대 5메가의 메시지 버퍼(실제로는 더블버퍼링으로 10메가)	
	desc.dwMaxServerNum						= 50;		// 최대 50개의 서버 연결
	desc.dwServerBufferSizePerConnection	= 20480;	// 
	desc.dwServerMaxTransferSize			= 8192;	
	desc.dwMaxUserNum						= 4000;
	desc.dwUserBufferSizePerConnection		= 20480;	// 
	desc.dwUserMaxTransferSize				= 8192;		

	desc.OnRecvFromServerTCP				= OnRecvFromServerTCP;
	desc.OnRecvFromUserTCP					= OnRecvFromUserTCP;
	desc.dwConnectNumAtSameTime				= 10;
	desc.dwCustomDefineEventNum				= CUSTOM_EVENT_DEF_NUM;
	desc.pEvent								= ev;
	desc.dwFlag								= 0;
	
	HRESULT hr;
	//HRESULT hr = CoCreateInstance( CLSID_4DyuchiNET, NULL, CLSCTX_INPROC_SERVER, IID_4DyuchiNET, (void**)&g_pNet);
	// 패킷 암호화 작업중 g_pNet 구성 변경 
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

	if (!g_pNet->CreateNetwork(&desc,10, 0))
	{
		Log(LOG_NORMAL, "Fail to CreateNetwork!");
	}

	// 중국 빌링이 아니면, 타이머 중지시킨다. 안쓰니까!!!
	if(! IS_ABLE_SERVICE_TYPE( ST_CHINA_BILLING ) )
	{
		g_pNet->PauseTimer(6);
	}

	g_pNet->PauseTimer(10);

	g_pThis->m_hKeyEvent[0]		= g_pNet->GetCustomEventHandle(1);
	g_pThis->m_hKeyEvent[1]		= g_pNet->GetCustomEventHandle(2);
	g_pThis->m_hKeyEvent[2]		= g_pNet->GetCustomEventHandle(3);
	g_pThis->m_hKeyEvent[3]		= g_pNet->GetCustomEventHandle(11);
	g_pThis->m_hKeyEvent[4]		= g_pNet->GetCustomEventHandle(12);
	g_pThis->m_hKeyEvent[5]		= g_pNet->GetCustomEventHandle(13);
	g_pThis->m_hKeyEvent[6]		= g_pNet->GetCustomEventHandle(16);		//ServerShutDown;
	g_pThis->m_hKeyEvent[7]		= g_pNet->GetCustomEventHandle(17);		//Enable Double EXP	
	g_pThis->m_hKeyEvent[8]		= g_pNet->GetCustomEventHandle(18);		//Disable Double EXP	
	g_pThis->m_hKeyEvent[9]		= g_pNet->GetCustomEventHandle(19);		//Magic Item Drop Inc
	g_pThis->m_hKeyEvent[10]	= g_pNet->GetCustomEventHandle(20);		//Magic Item Drop Dec	
	g_pThis->m_hKeyEvent[11]	= g_pNet->GetCustomEventHandle(21);		//Exp Inc	
	g_pThis->m_hKeyEvent[12]	= g_pNet->GetCustomEventHandle(22);		//Exp Dec
	g_pThis->m_hKeyEvent[13]	= g_pNet->GetCustomEventHandle(24);		//Party Bonus Exp Gain Inc	
	g_pThis->m_hKeyEvent[14]	= g_pNet->GetCustomEventHandle(25);		//Party Bonus Exp Gain Dec

	InitializePacketProc();	
}

void QueryAllServer()
{
	if( !_CrtCheckMemory( ) ) __asm int 3

	Log(LOG_JUST_DISPLAY, "@ Loading All Server Informations...");
	
	// World서버에 접속할 서버 정보를 DB에서 쿼리 
	char szQuery[ 0xff ]={0,};
	wsprintf(szQuery, "Select ServerType, IPForUser, IPForServer, Port from ServerTable where ServerType <> 0");
		
	ServerRecord RecordSet[ 50 ];
	memset(RecordSet, 0, sizeof(ServerRecord));

	int nRet = g_pDb->OpenRecord(szQuery, RecordSet, 50, GAME_DB);

	if(nRet < 0)
	{
		Log(LOG_IMPORTANT, "Fail To Query Server Informations!");
		return;
	}

	for(int i =0; i<nRet; i++)
	{
		// Port가 ID
		SERVER_DATA* pServer = g_pServerTable->AllocNewServer(RecordSet[ i ].Port-10000);		

		pServer->dwServerStatus = SERVER_STATUS_NOT_CONNECTED;
		pServer->bServerType = (BYTE)RecordSet[ i ].ServerType;
		memset(pServer->szIPForServer, 0, sizeof(pServer->szIPForServer));
		__lstrcpyn(pServer->szIPForServer, RecordSet[ i ].IPForServer, MAX_IP_LENGTH);
		pServer->dwIPForUser = inet_addr(RecordSet[ i ].IPForUser);
		pServer->wPort = (WORD)RecordSet[i].Port;		

		g_pServerTable->Add(pServer);
	}

	Log(LOG_JUST_DISPLAY, "@ Loading All Map Informations...");
	
	// World서버가 가지고 있어야 할 맵 정보를 DB에서 쿼리 
	DUNGEON_DATA rs[ MAX_DUNGEON_NUM_PER_SERVERSET ];
	memset(rs,0, sizeof(rs));

	wsprintf(szQuery, "CZP_QUERY_DUNGEON_INFO_WORLD %d", g_pThis->GetServerID());	
	nRet = g_pDb->OpenRecord(szQuery, rs, MAX_DUNGEON_NUM_PER_SERVERSET, GAME_DB);

	if(nRet < 0)
	{
		Log(LOG_IMPORTANT, "Fail To Query Map Informations!");
		return;
	}

	Log(LOG_JUST_DISPLAY, "@ Calculating DungeonSiege Time...");

	struct tm present, when;
	time_t now;
	time(&now);
	present = *localtime(&now);

	// 분단위로 계산
	DWORD dwSubLocalTime = 0;

	for(i = 0; i < nRet; i++)
	{
		DUNGEON_DATA_EX* pDungeon = g_pDungeonTable->AllocNewDungeon( (WORD)rs[i].m_dwID );
		memcpy((DUNGEON_DATA*)pDungeon, &rs[i], sizeof(DUNGEON_DATA));
				
		pDungeon->m_pServer = g_pServerTable->GetServerInfo(rs[i].m_dwPort-10000);
		
		if (pDungeon->m_dwID > 2000 && pDungeon->m_dwID < 3000)
		{
			if (pDungeon->m_dwID%2==0)
			{
				DUNGEON_DATA_EX* pTempDungeon = g_pDungeonTable->GetDungeonInfo((WORD)pDungeon->m_dwID-1);
				pDungeon->m_pServer = pTempDungeon->m_pServer;
			}
		}

		DWORD dwSubTime = 0;
		
		if (pDungeon->m_dwPieceStartTime)
		{
			when = present;	
			
			dwSubLocalTime = pDungeon->m_dwPieceStartTime;
			
			when.tm_year = (dwSubLocalTime/100000000)+100;
			dwSubLocalTime -= (when.tm_year-100) *100000000;

			when.tm_mon = (dwSubLocalTime/1000000)-1;
			dwSubLocalTime -= (when.tm_mon+1) *1000000;

			when.tm_mday = (dwSubLocalTime/10000);
			dwSubLocalTime -= when.tm_mday *10000;
			
			when.tm_hour = (dwSubLocalTime/100%24);
			dwSubLocalTime -= when.tm_hour *100;	// 분으로 계산해라.

			when.tm_min = dwSubLocalTime;
			// 전에 시작한 시간을 분단위로.			
			
			time_t dwPrevTime = mktime(&when);
			dwSubTime = (DWORD)difftime(now, dwPrevTime);
			
			//tick으로 변환
			dwSubTime *= 1000;
			//050321 재원 공성주기 일정하게 맞추기 위한 코드.
			dwSubTime %= pDungeon->GetIdleTime();

		}
		
		// 공성시간 이어지는 처리.
		DWORD dwIdleTime = pDungeon->GetIdleTime();

		if (dwIdleTime >= dwSubTime)
		{		
			dwIdleTime -= dwSubTime;
		}
		else 
		{
			dwIdleTime = 0;// 공성시간으로 만들어라.			
		}

		if(	IS_ABLE_SERVICE_TYPE(ST_DEVELOP) )//hwoarang
		{
			//pDungeon->SetSiegeStartDestTime( 0 );
			pDungeon->SetSiegeStartDestTime( dwIdleTime );
		}
		else
		{
			pDungeon->SetSiegeStartDestTime( dwIdleTime );
		}

		//	레벨업시간 이어지는 처리
		dwIdleTime = pDungeon->GetLevelUpTickCount();
		
		if (dwIdleTime >= dwSubTime)
			dwIdleTime -= dwSubTime;
		else
			dwIdleTime = 0;

		pDungeon->m_dwRemainLevelStartTick = dwIdleTime;
		pDungeon->m_dwStartLevelStartTick = g_dwCurTick;
		g_pDungeonTable->Add(pDungeon);

		// 이벤트 던젼들의 리스트는 이벤트 던젼 메니져에 한번 더 등록하여 따로 처리 될 수 있게 한다.		
		if (DUNGEON_TYPE_EVENT == pDungeon->GetDungeonType())
		{
			if (NULL != g_pEventDungeonManager)
			{
				g_pEventDungeonManager->AddEventDungeon(pDungeon);
			}
		}	
	}	
	
	if( !_CrtCheckMemory( ) ) __asm int 3
	
}

BOOL QueryWorldmapFormation()
{
	Log(LOG_JUST_DISPLAY, "@ Loading Worldmap Formation...");

	memset(g_pMap, 0, sizeof(g_pMap));
	
	char	szQuery[ 1024 ]={0,};
	BYTE	bFormedID[ MAX_WORLD_NUM_PER_SERVER ];	memset(bFormedID, 0, sizeof(bFormedID));

	wsprintf(szQuery, "select MapID_01, MapID_02, MapID_03, MapID_04, MapID_05 from WorldServerTable where "
						"ServerSet = %s and WorldMapID=%d", g_pThis->GetServerSetCode(), g_pThis->GetServerID());

	int nRet = g_pDb->OpenRecord(szQuery, bFormedID, DEFAULT_RETURNED_MAX_ROWS, GAME_DB);

	if(nRet < 0)	
	{
		Log(LOG_IMPORTANT, "Fail to Query WorldMap Formation!");
		__asm int 3
		return FALSE;
	}

	for(int i=0; i<MAX_WORLD_NUM_PER_SERVER; i++)
	{
		if(bFormedID[i])
		{
			g_pMap[ bFormedID[i] ] = new CMap;
			g_pMap[ bFormedID[i] ]->Create( bFormedID[i] + 10000 );
		
			g_dwCurLandNum++;
		}			
	}

	return TRUE;
}

int GetPlayTime(DWORD dwCurTick, DWORD dwStartTick, DWORD dwFlag)
{
	if(dwCurTick  == dwStartTick)	return 0;
	
	DWORD dwSec = 0;
	if(dwCurTick < dwStartTick)		//4Byte 값 돌아간 경우 	
	{
		dwSec = 0xffffffff - dwStartTick;
		dwSec += dwCurTick;
	}
	else
	{
		dwSec = dwCurTick - dwStartTick;	//정상적인 경우 
	}
	
	dwSec = dwSec / 1000;		//초로 변환 

	int nHour, nMinute, nSec;	
	nHour	= dwSec / 3600;
	nMinute = ((dwSec % 3600) / 60)%60;
	nSec	= (dwSec - (3600*nHour + 60*nMinute))%60;

	switch(dwFlag)
	{
	case TIME_HOUR:
		return nHour;
		
	case TIME_MIN:
		return nMinute;
		
	case TIME_SEC:
		return nSec;

	case ONLY_SEC:
		return dwSec;
	}

	return -1;
}


BOOL FindEmptyPosNearDungeon(BYTE bWorldID, VECTOR3* vpDungeonPos, VECTOR3 *vpNearPos, BOOL bVillage)
{
	if(!g_pMap[ bWorldID ])
	{
		Log(LOG_IMPORTANT, "Invalid WorldID Entered at FindEmptyPosNearDungeon Function! (bWorldID:%d", bWorldID);
		return FALSE;
	}

	MAP_TILE*	pTile = 0;
	VECTOR3		vPos = { 0.f, 0.f, 0.f };
	DWORD		dwCount = 0;	

	while(dwCount < 4)
	{
		vPos = *vpDungeonPos;
		dwCount++;
		
		float fDistance = (bVillage) ? 5.0f : 2.0f;

		switch(dwCount)
		{
			case 1:		vPos.x -= (TILE_WIDTH * fDistance);	break;		//서쪽방향 
			case 2:		vPos.z -= (TILE_WIDTH * fDistance);	break;		//남쪽방향
			case 3:		vPos.z += (TILE_WIDTH * fDistance);	break;		//북쪽방향
			case 4:		vPos.x += (TILE_WIDTH * fDistance);	break;		//동쪽방향 
		}		

		pTile = g_pMap[bWorldID]->GetTile(vPos.x, vPos.z);	

		if(!pTile)	return FALSE;

		if(pTile->wAttr.uAttr != 1)
		{
			vpNearPos->x = vPos.x;
			vpNearPos->z = vPos.z;
			return TRUE;
		}
	}

	return FALSE;
}

void CreatePoolAndHash()
{
	g_pGuildTableHash		= new CGuildTableHash;
	g_pPartyTableHash		= new CPartyTableHash;
	g_pPartyBoardTableHash	= new CPartyBoardTableHash;
	g_pGroupInfoTableHash	= new CGroupInfoTableHash;
	g_pMessengerTableHash	= new CMessengerTableHash;
	g_pGuildWarHash			= new CGuildWarHash;
	g_pDungeonListHash		= new CDungeonListHash;
	g_pGuildWarTimeList		= new COnlyList(MAX_GUILD_COUNT);		
	g_pRankList				= new COnlyList(MAX_GUILD_COUNT);
	
	g_pRankListPool			= CreateStaticMemoryPool();			
	g_pGuildTablePool		= CreateStaticMemoryPool();	
	g_pPartyTablePool		= CreateStaticMemoryPool();	
	g_pPartyBoardTablePool	= CreateStaticMemoryPool();	
	g_pGroupTablePool		= CreateStaticMemoryPool();	
	g_pGuildOfflinePool		= CreateStaticMemoryPool();	
	g_pMessengerTablePool	= CreateStaticMemoryPool();
	g_pMessengerUserPool	= CreateStaticMemoryPool();
	g_pGuildWarPool			= CreateStaticMemoryPool();		
	g_pDungeonNodePool		= CreateStaticMemoryPool();	
	g_pGuildWarTimePool		= CreateStaticMemoryPool();	
	
	g_pDungeonListHash->InitializeHashTable(MAX_DUNGEON_NUM_PER_SERVERSET, MAX_DUNGEON_NUM_PER_SERVERSET);	
	g_pGuildTableHash->InitializeHashTable(MAX_GUILD_COUNT, MAX_GUILD_COUNT);	
	g_pGroupInfoTableHash->InitializeHashTable(HASH_SIZE_GROUP, HASH_SIZE_GROUP);
	g_pPartyTableHash->InitializeHashTable(HASH_SIZE_PARTY, HASH_SIZE_PARTY);	
	g_pMessengerTableHash->InitializeHashTable(HASH_SIZE_MESSENGERTABLE, HASH_SIZE_MESSENGERTABLE);		
	g_pGuildWarHash->InitializeHashTable(MAX_GUILD_COUNT, MAX_GUILD_COUNT);
	g_pPartyBoardTableHash->InitializeHashTable(HASH_SIZE_PARTYBOARD, HASH_SIZE_PARTYBOARD);
	
	InitializeStaticMemoryPool(g_pDungeonNodePool, sizeof(SDUNGEON_NODE), MAX_DUNGEON_NUM_PER_SERVERSET, MAX_DUNGEON_NUM_PER_SERVERSET);	
	InitializeStaticMemoryPool(g_pGuildWarTimePool, sizeof(SGUILDWAR_TIME), MAX_GUILD_COUNT, MAX_GUILD_COUNT);			
	InitializeStaticMemoryPool(g_pRankListPool, sizeof(GUILDWAR_RANK), MAX_GUILD_COUNT, MAX_GUILD_COUNT);	
	InitializeStaticMemoryPool(g_pGroupTablePool, sizeof(SGROUPINFO_TABLE), POOL_SIZE_GROUP, POOL_SIZE_GROUP);	
	InitializeStaticMemoryPool(g_pGuildTablePool, sizeof(GUILD_TABLE), MAX_GUILD_COUNT, MAX_GUILD_COUNT);			
	InitializeStaticMemoryPool(g_pPartyTablePool, sizeof(PARTY_TABLE), POOL_SIZE_PARTY, POOL_SIZE_PARTY);		
	InitializeStaticMemoryPool(g_pPartyBoardTablePool, sizeof(PARTYBOARD_TABLE), POOL_SIZE_PARTYBOARD, POOL_SIZE_PARTYBOARD);
	InitializeStaticMemoryPool(g_pGuildOfflinePool, sizeof(GUILD_OFFLINE_USER), POOL_SIZE_OFFLINEUSER, POOL_SIZE_OFFLINEUSER);	
	InitializeStaticMemoryPool(g_pMessengerTablePool, sizeof(MESSENGER_TABLE), POOL_SIZE_MESSENGERTABLE, POOL_SIZE_MESSENGERTABLE);	
	InitializeStaticMemoryPool(g_pMessengerUserPool, sizeof(MESSENGER_USER), POOL_SIZE_MESSENGERUSER, POOL_SIZE_MESSENGERUSER);	
	InitializeStaticMemoryPool(g_pGuildWarPool, sizeof(SGUILD_LIST), MAX_GUILD_COUNT, MAX_GUILD_COUNT);
}

BOOL InitDungeonList()
{	
#ifdef WORLD_MOVE_CHK
	SDUNGEON_NODE sDungeonList[1000];

	int nCount	= g_pDb->OpenRecord("Select * From DungeonList", sDungeonList, MAX_DUNGEON_NUM_PER_SERVERSET, (BYTE)GAME_DB);

	if(nCount<0)
		goto lb_fail;

	for(int i = 0; i < nCount; i++)
	{				
		LP_DUNGEON_NODE lpDungeonListNode = (LP_DUNGEON_NODE)LALAlloc(g_pDungeonNodePool);

		if(!lpDungeonListNode)
			goto lb_fail;

		lpDungeonListNode->byType		= sDungeonList[i].byType;
		lpDungeonListNode->dwDungeonId	= sDungeonList[i].dwDungeonId;

		g_pDungeonListHash->Insert(lpDungeonListNode, lpDungeonListNode->dwDungeonId);
	}

	goto lb_success;
	
lb_fail:
	Log(LOG_IMPORTANT, "Fail to Query DungeonList!");
	return FALSE;

lb_success:
	return TRUE;

#else
	return TRUE;
#endif
}

BOOL InitGroupInformation()
{
#define MAX_GROUP		500

	SGROUP_INFO			sGroup[MAX_GROUP];
	LP_GROUPINFO_TABLE	lpGroupInfo			= NULL;
	LP_GROUPINFO_TABLE	lpGroupInfoTable	= NULL;

	int nCount = g_pDb->OpenRecord("Select * From Group_Info", sGroup, MAX_GROUP, (BYTE)GAME_DB);
	
	if(nCount<0)
	{
		Log(LOG_IMPORTANT, "Fail to query GroupInfo!");
		return FALSE;
	}		
	
	for(int i = 0; i < nCount; i++)
	{
		lpGroupInfo = g_pGroupInfoTableHash->GetData(sGroup[i].byLevel);

		if(lpGroupInfo)
		{
			lpGroupInfo->sGroupInfo[sGroup[i].byType-1].byAura		= sGroup[i].byAura;
			lpGroupInfo->sGroupInfo[sGroup[i].byType-1].byLevel		= sGroup[i].byLevel;
			lpGroupInfo->sGroupInfo[sGroup[i].byType-1].byMake_Size	= sGroup[i].byMake_Size;
			lpGroupInfo->sGroupInfo[sGroup[i].byType-1].byType		= sGroup[i].byType;
			lpGroupInfo->sGroupInfo[sGroup[i].byType-1].dwExp		= sGroup[i].dwExp;
			lpGroupInfo->sGroupInfo[sGroup[i].byType-1].wMemeber[0]	= sGroup[i].wMemeber[0];
			lpGroupInfo->sGroupInfo[sGroup[i].byType-1].wMemeber[1]	= sGroup[i].wMemeber[1];
			lpGroupInfo->sGroupInfo[sGroup[i].byType-1].wMemeber[2]	= sGroup[i].wMemeber[2];
			lpGroupInfo->sGroupInfo[sGroup[i].byType-1].wMemeber[3]	= sGroup[i].wMemeber[3];
			lpGroupInfo->sGroupInfo[sGroup[i].byType-1].wMemeber[4]	= sGroup[i].wMemeber[4];
		}
		else
		{
			lpGroupInfoTable = (LP_GROUPINFO_TABLE)LALAlloc(g_pGroupTablePool);
			memset(lpGroupInfoTable, 0, sizeof(SGROUPINFO_TABLE));						
			lpGroupInfoTable->sGroupInfo[sGroup[i].byType-1].byAura			= sGroup[i].byAura;
			lpGroupInfoTable->sGroupInfo[sGroup[i].byType-1].byLevel		= sGroup[i].byLevel;
			lpGroupInfoTable->sGroupInfo[sGroup[i].byType-1].byMake_Size	= sGroup[i].byMake_Size;
			lpGroupInfoTable->sGroupInfo[sGroup[i].byType-1].byType			= sGroup[i].byType;
			lpGroupInfoTable->sGroupInfo[sGroup[i].byType-1].dwExp			= sGroup[i].dwExp;
			lpGroupInfoTable->sGroupInfo[sGroup[i].byType-1].wMemeber[0]	= sGroup[i].wMemeber[0];
			lpGroupInfoTable->sGroupInfo[sGroup[i].byType-1].wMemeber[1]	= sGroup[i].wMemeber[1];
			lpGroupInfoTable->sGroupInfo[sGroup[i].byType-1].wMemeber[2]	= sGroup[i].wMemeber[2];
			lpGroupInfoTable->sGroupInfo[sGroup[i].byType-1].wMemeber[3]	= sGroup[i].wMemeber[3];
			lpGroupInfoTable->sGroupInfo[sGroup[i].byType-1].wMemeber[4]	= sGroup[i].wMemeber[4];
			g_pGroupInfoTableHash->Insert(lpGroupInfoTable, sGroup[i].byLevel);
		}				
	}

	return TRUE;
}


void __stdcall CreateEventJudgment(DWORD dwVal)
{
	dwVal;
	
	START_FUNCTION_PROFILE()

	if(NULL != g_pEventDungeonManager)
	{
		g_pEventDungeonManager->ProcessRelationWithCreate();
	}

	END_FUNCTION_PROFILE();
}


bool UnloadNoticeMessage()
{
	if (NULL != g_pNoticeMessage)
	{
		delete[] g_pNoticeMessage;
		g_pNoticeMessage = NULL;
	}

	g_dwNoticeMessageCount = 0;

	return true;
}


bool LoadNoticeMessage()
{
	try
	{
		// Initialize require global variables.		
		UnloadNoticeMessage();			
		
		// Load Resource from database.
		NOTICE_MESSAGE aNoticeMessage[MAX_NOTICE_COUNT];
		
		int iCount = 0;
		iCount = g_pDb->OpenRecord("Select [No], [Important], [Type], [Message], [Show] From NOTICE_MESSAGE",
									aNoticeMessage,
									MAX_NOTICE_COUNT,
									(BYTE)TOTAL_DB);
		
		if (0 < iCount)
		{
			// Alloc require memory and copy notice message data to the buffer.			
			g_dwNoticeMessageCount = static_cast<DWORD>(iCount);
			
			// Exception process.
			g_pNoticeMessage = new NOTICE_MESSAGE[g_dwNoticeMessageCount];
			if (NULL == g_pNoticeMessage)
			{
				throw false;
			}

			memset(g_pNoticeMessage, 0, sizeof(NOTICE_MESSAGE) * g_dwNoticeMessageCount);

			// Actually copy data.
			memcpy(g_pNoticeMessage, aNoticeMessage, sizeof(NOTICE_MESSAGE) * g_dwNoticeMessageCount);

			throw true;			
		}
		else
		{
			// Load failure process.			
			g_dwNoticeMessageCount = 0;			
			throw false;
		}
	}
	catch (bool bResult)
	{
		return bResult;
	}	
}


void UnLoadServerMessage()
{
	if (NULL != g_pServerMessage)
	{
		delete[] g_pServerMessage;
		g_pServerMessage = NULL;
	}

	g_dwServerMessageCount = 0;	
}


bool LoadServerMessage()
{
	try
	{
		UnLoadServerMessage();

		SERVER_MESSAGE	aServerMessage[MAX_SERVER_MESSAGE_COUNT];

		int iCount = 0;
		iCount = g_pDb->OpenRecord("Select [Id], [Message] From ServerMessage ORDER by [Id]",
									aServerMessage,
									MAX_SERVER_MESSAGE_COUNT,
									(BYTE)TOTAL_DB);		
		if (0 < iCount)
		{
			// Alloc require memory and copy notice message data to the buffer.
			g_dwServerMessageCount = static_cast<DWORD>(iCount);
			
			// Exception process.
			g_pServerMessage = new SERVER_MESSAGE[g_dwServerMessageCount];
			if (NULL == g_pServerMessage)
			{
				throw false;
			}

			memset(g_pServerMessage, 0, sizeof(SERVER_MESSAGE) * g_dwServerMessageCount);

			// Actually copy data.
			memcpy(g_pServerMessage, aServerMessage, sizeof(SERVER_MESSAGE) * g_dwServerMessageCount);

			throw true;			
		}
		else
		{
			// Load failure process.
			g_dwServerMessageCount = 0;			
			throw false;
		}
	}
	catch (bool bResult)
	{
		return bResult;
	}
}


char* GetServerMessage(int iCount)
{
	if (NULL == g_pServerMessage)
	{
		return NULL;
	}

	if (0 > iCount || g_dwServerMessageCount <= (unsigned long int)iCount)
	{
		return NULL;
	}

#ifdef _DEBUG
	if (MAX_SERVER_MESSAGE_LENGTH <= strlen(g_pServerMessage[iCount].szMessage))
	{
		Log(LOG_NORMAL, "array overwrite at GetServerMessage Function");
		return NULL;
	}
#endif

	return g_pServerMessage[iCount].szMessage;
}
