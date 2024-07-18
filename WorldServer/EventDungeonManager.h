#ifndef _EVENT_DUNGEON_MANAGER_
#define _EVENT_DUNGEON_MANAGER_


#pragma once


#include "DUNGEON_DATA_EX.h"


#define EVENT_DUNGEON_POS_TYPE_NORMAL	0
#define EVENT_DUNGEON_POS_TYPE_SPECIAL	1
#define MAX_EVENT_DUNGEON_NUMBER		16
#define MAX_EVENT_POS_NUMBER			128
#define MAX_EVENT_DUNGEON_KIND			16

class DUNGEON_DATA_EX;


#pragma pack(push, 1)
struct EVENT_DUNGEON_PROPERTY
{
	DWORD		dwDungeonNumber;		// 맵 테이블에서의 던젼 번호.
	BYTE		byDungeonType;			// 던젼의 타입.
	BYTE		byEnterLevelMin;		// 입장 최소 레벨
	BYTE		byEnterLevelMax;		// 입장 최대 레벨
	DWORD		dwOpenTime;				// 닫혀 있는 던젼이 오픈되기 위한 딜레이.
	DWORD		dwReadyOpenTime;		// '입구 열기 위해 준비중인' 던젼이 '입구를 열기'위한 딜레이.
	DWORD		dwOpenTimeUpTime;		// '입구 열림'던젼이 '입구 닫기'위한 딜레이.
	DWORD		dwFirstCloseTime;		// 생존던전일때 1차이벤트 종료 딜레이 
	DWORD		dwCloseTime;			// '입구 열림'던젼이 '닫히기'위한 (입구가 아님) 딜레이.
	BYTE		byMaxEnterNo;			// 최대 입장 인원수.
	BYTE		byPositionType;			// 생성 위치 데이터 참조 타입.
};
#pragma pack(pop)


class CEventDungeonManager
{

public:

	CEventDungeonManager();
	~CEventDungeonManager();

	BOOL					AddEventDungeon(DUNGEON_DATA_EX* pDungeon);
	EVENT_DUNGEON_PROPERTY* GetEventProperty(DWORD dwDungeonNumber) const;
	void					Process();						
	void					ProcessRelationWithCreate();
	BOOL					QueryEventDungeonProperties();

	COnlyList*				m_pEventDungeonList;
	CVoidDontDeleteList*	m_pPropertyList;

private:

	void					Clear();	
	BOOL					ClearPropertyInfo();	
};


extern CEventDungeonManager* g_pEventDungeonManager;


#endif 