#ifndef __SERVER_FUNCTION_H__
#define __SERVER_FUNCTION_H__


#include "ChinaBillingProc.h"
#include "WorldUser.h"


class CGuildWarTimeHash;


extern BOOL					g_bIsServerRunning;
extern COnlyList*			g_pGuildWarTimeList;
extern STMPOOL_HANDLE		g_pGuildWarTimePool;
extern SYSTEMTIME			g_CurDate;


#define UM_LISTEN_FOR_USERSIDE			WM_USER + 800
#define UM_LISTEN_FOR_SERVERSIDE		WM_USER + 801
#define UM_SERVER_RUNNING				WM_USER + 802
#define SCREEN_TEXT_SIZE_X				100
#define SCREEN_TEXT_SIZE_Y				100
#define CUSTOM_EVENT_DEF_NUM			26


#pragma pack(push, 1)
struct ServerRecord
{
	DWORD	ServerType;
	char	IPForUser[16];
	char	IPForServer[16];
	DWORD	Port;
};
#pragma pack(pop)


const int MAX_SERVER_MESSAGE_LENGTH		= 1024;
const int MAX_SERVER_MESSAGE_COUNT		= 256;
const int MAX_NOTICE_MESSAGE_LENGTH		= 1024;
const int MAX_NOTICE_COUNT				= 256;


#pragma pack(push, 1)
struct SERVER_MESSAGE
{
	long int			iId;
	char				szMessage[MAX_SERVER_MESSAGE_LENGTH];
};

struct NOTICE_MESSAGE
{
	long int			iNo;
	unsigned char		byImportant;
	unsigned char		byType;	
	char				szMessage[MAX_NOTICE_MESSAGE_LENGTH];
	unsigned char		byShow;
};
#pragma pack(pop)


extern void __stdcall WorldMatchProc(DWORD dwVal);//파티 매칭프로세스 : 050129 hwoarang
extern void __stdcall GetDBResult(DWORD dwVal);
extern void __stdcall TimerPerSec(DWORD dwVal);
extern void __stdcall EnableDoubleExp(DWORD dwVal);
extern void __stdcall DisableDoubleExp(DWORD dwVal);
extern void __stdcall IncreaseMagicDropRate(DWORD dwVal);
extern void __stdcall DecreaseMagicDropRate(DWORD dwVal);
extern void __stdcall IncreaseExpGain(DWORD dwVal);
extern void __stdcall DecreaseExpGain(DWORD dwVal);
extern void __stdcall IncreasePartyBonusExpGain(DWORD dwVal);
extern void __stdcall DecreasePartyBonusExpGain(DWORD dwVal);
extern void __stdcall GuildMaster(DWORD dwVal);
extern void __stdcall ShutDownServer(DWORD dwVal);
extern void __stdcall TerminateAllServer(DWORD dwVal);
extern void __stdcall ShowAllServerStatus(DWORD dwVal);
extern void __stdcall RefreshAllDungeonServerData(DWORD dwVal);
extern void __stdcall TimerPer5Minute(DWORD dwVal);
extern void __stdcall SendKeepAlivePacket(DWORD dwVal);
extern void __stdcall CheckKeepAlive(DWORD dwVal);
extern void __stdcall StartServerWithUserSide(DWORD dwEventIndex);
extern void __stdcall StartServerWithServerSide(DWORD dwEventIndex);
extern void __stdcall ShowCurrentUserCount(DWORD dwEventIndex);
extern void __stdcall SendServerAlive(DWORD dwEventIndex);
extern void __stdcall CheckServerAlive(DWORD dwEventIndex);
extern void __stdcall CreateEventJudgment(DWORD dwVal);


void	DisplayDBMessage(char* szMsg);
void	DisplayDBReport(char* szMsg);
void	SendNoticeMessageProcess(void);
void	InitOleDBThread();
void	TerminateServer();
void	ConnectToLoginAgentServer();
void	ListenForUserside();	
void	ListenForServerside();	
bool	ConnectToDBServer();
void	KeyHelp();
BOOL	IsDateExpired(DBTIMESTAMP *pEndDate);
void	CmdCheckConnection(CWorldUser* pUser ,char* pMsg, DWORD dwLength);
void	InitNetwork();
void	CreatePoolAndHash();
BOOL	InitGroupInformation();
BOOL	InitDungeonList();
void	QueryAllServer();
BOOL	QueryWorldmapFormation();
int		GetPlayTime(DWORD dwCurTick, DWORD dwStartTick, DWORD dwFlag);
BOOL	FindEmptyPosNearDungeon(BYTE bWorldID, VECTOR3* vpDungeonPos, VECTOR3 *vpNearPos, BOOL bVillage);
bool	UnloadNoticeMessage();
bool	LoadNoticeMessage();
void	UnLoadServerMessage();
bool	LoadServerMessage();
char*	GetServerMessage(int iCount);


#endif 