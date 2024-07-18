#pragma once


class CDungeon;


#define UM_LISTEN_FOR_USERSIDE		WM_USER + 800
#define UM_LISTEN_FOR_SERVERSIDE	WM_USER + 801
#define UM_SERVER_RUNNING			WM_USER + 802

#define SCREEN_TEXT_SIZE_X			100
#define SCREEN_TEXT_SIZE_Y			100

#define MAX_LOG_LENGTH				512
#define NUM_OF_LOG_TYPE				7


void LoadSpeedHackCheckParameter();
void InitNetwork();
void InitOleDBThread();
void InitMemoryPool();
BOOL InitHashTables();
bool ConnectToDBServer();
bool ConnectToDBServerDecodeCFG();
void ConnectToWorldServer();
BOOL LoadDungeonServerData();
void LoadItemTable();
void ListenForUserside();
void KeyHelp();
BOOL ReLoadDungeonServerData(); 
void RemoveUserProcess(CDungeon* pDungeon);
void TerminateServer();







