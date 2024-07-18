#pragma once
#include "Define.h"
#include "dungeon_data_ex.h"
#define MAX_EVENT_DUNGEON_NUMBER				16
#define MAX_OCCUPIED_DUNGEON_NUM_PER_SERVERSET	400
#define MAX_BYGROUP	130
#define MAX_GROUPLIST	10
#define MAX_WORLD_LAND_NUM		2				//최대 월드맵 대륙갯수 
class COnlyList;
class CDungeonTable
{
	DWORD					m_dwMaxBucketNum;	
	DWORD					m_dwNum;	
	DUNGEON_DATA_EX**		m_ppInfoTable;	
	
	BOOL					Add(DUNGEON_DATA_EX* pDungeon);
	
	BYTE					m_bCurrentWorldMapID;		//현재 사용중인 또는 유저가 속한 월드맵 대륙번호를 나타냄 
public:	
	
	void					HideAllDungeonGroupEffect();
	void					ShowEffectDungeonGroup(WORD wGroupID, BOOL bShow);
	void					AddListDungeonGroup(DUNGEON_DATA_EX* pDungeon, WORD wGroupID);
	
	void					RemoveDungeonTable();

	COnlyList*				m_pInfoList[ MAX_WORLD_LAND_NUM + 1 ];		//월드맵 대륙번호별로 리스트 배열로 관리 0번 배열은 사용안함. 
	COnlyList*				m_pReceivedDungeonList;		//월드 서버로 부터 던전정보를 받은 점령던전의 리스트 
	COnlyList*				m_pGroupList[MAX_BYGROUP];		//그룹별로 리스트를 만든다.		

	DUNGEON_DATA_EX*		AllocDungeonInfo();
	DUNGEON_DATA_EX*		GetDungeonInfo(DWORD dwId);
	BOOL					AddDungeonInfo(DUNGEON_DATA_EX* pDungeon);

	void					Remove(DUNGEON_DATA_EX* pDungeon);
	DWORD					GetCount()	{	return m_dwNum;		}

	void					RemoveAllSprGuildMark();
	void					RemoveDungeonEffect(DUNGEON_DATA_EX* pDungeon);
	

	COnlyList*				m_pEventList;
	BOOL					AddEventDungeon(DUNGEON_DATA_EX* pDungeon);	
	BOOL					LoadEventDungeonDescription(char* pszFileName);

	void					SetCurrentWorldMapID( BYTE bWorldMapID )	{	m_bCurrentWorldMapID = bWorldMapID;	}
	BYTE					GetCurrentWorldMapID()						{	return m_bCurrentWorldMapID;	}

	void					DeleteEventDungeonResource();
	

	CDungeonTable(DWORD dwNum);
	~CDungeonTable();
};


