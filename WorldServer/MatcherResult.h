#pragma once
#include "StadiumMgr.h"

class CMatcherResult
{
public:
	CMatcherResult(void);
	~CMatcherResult(void);
	
	class InsertResult
	{
		
	};

	class NotifyResult
	{
	public:
		CStadium *pStadium;
//		DUNGEON_MOVE_INFO DMI;
		FLAG_TYPE match_type;
		size_t	player_count;
		size_t	team_index;//
		DWORD	dwGuildWarFlag;
		FLAG_TYPE team_type;
	};
};
