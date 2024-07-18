#include "stdafx.h"
#include "CommonServer.h"
#include "CommonDungeon.h"

DUNGEON_TYPE DECLSPECIFIER DungeonIDtoType(const DWORD dwID)
{
	if ( (1 <= dwID) && (10 >= dwID) )				
	{	// 마을
		return DUNGEON_TYPE_VILLAGE;
	}
	else if ( (2000 <= dwID) && (3000 > dwID) ) 
	{	// 터널 던젼
		return DUNGEON_TYPE_TUNNEL;
	}
	else if ( (3000 <= dwID) && (4000 > dwID) ) 
	{	// 비점령 던젼	
		return DUNGEON_TYPE_NOT_CONQUER;
	}
	else if ( (4000 <= dwID) && (5000 > dwID ) ) 
	{	// 점령 던젼
		return DUNGEON_TYPE_CONQUER;
	}
	else if ( (5000 <= dwID) && (6000 > dwID ) )  
	{	// 이벤트 던젼.
		return DUNGEON_TYPE_EVENT;
	}
	else if ( (6000 <= dwID) && (7000 > dwID ) )  
	{	// 라이벌 길드전 던젼 
		return DUNGEON_TYPE_MATCH_STATIDUM;
	}
	
	return DUNGEON_TYPE_NOT_CONQUER;//기본적으로 비점령던전이 된다.
}