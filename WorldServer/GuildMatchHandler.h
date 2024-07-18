#pragma once
#include "../CommonServer/CommonServer.h"
#include "../CommonServer/TeamMatchHandler.h"

#include "../CommonServer/STLSupport.h"

#include "windows.h"
#include "mmsystem.h"
#pragma comment (lib,"winmm.lib")

typedef size_t COUNT_TYPE;

const COUNT_TYPE g_aMatchTeamCount[] =  {2,3,4}; //반드시 순서대로 오름차순이 될 것
const COUNT_TYPE g_min_team_count = g_aMatchTeamCount[0];

// Remark
// 오토매칭 길드전을 하기 위한 인원 리스트
const COUNT_TYPE g_aBasePlayerCount[] =  
{
	6,
	9,
	12,
	15,
	18
}; //반드시 순서대로 오름차순이 될 것
// 정식 길드전을 하기 위한 최소인원
const COUNT_TYPE g_min_player_count = g_aBasePlayerCount[0];
// 오토매팅 길드전을 하기 위한 최소인원
#if defined(GUILDMATCH_TEST)
const COUNT_TYPE g_min_player_count_AutoMatch = 1;
#else
const COUNT_TYPE g_min_player_count_AutoMatch = 6;
#endif

const DWORD	g_dwGuildWarDelayTime = 5000;//1분 으로 고칠것 60000

extern bool MoveToStadium( CWorldUser *pUser, const DUNGEON_MOVE_INFO &rDMI , const size_t team_index, const DWORD dwGuildWarFlag = G_W_F_PLAYER, const bool bIsGMCClear =true);//기본적으로 플레이어다

class CPlayerInfo
{
public:
	CPlayerInfo(CWorldUser *pUser)
	{
		Name( pUser->m_szCharacterName );
//		Class( pUser->m_eClass );
//		LV( pUser->m_dwLevel );
		m_pUser = pUser;
//		m_bIsCalled = false;
	}

	virtual ~CPlayerInfo()
	{
		m_pUser = NULL;
	}

public:
//	int Index() const {return m_iIndex;}
//	void Index(const int iIndex) {m_iIndex = iIndex;}

	const char* Name() const {return m_strName.c_str();}
	void Name(const string &rstrName) {m_strName = rstrName;}

//	CLASS_TYPE	Class() const {return m_eClass;}
//	void Class(const CLASS_TYPE eCT) {m_eClass = eCT;}
	
//	DWORD	LV()const {return m_dwLv;}
//	void	LV(const DWORD dwLv){m_dwLv = dwLv;}

	WORD	Class() const {return m_pUser->m_wClass;}
	DWORD	LV()const {return m_pUser->m_dwLevel;}

//	void Call(const bool bIsCalled){m_bIsCalled = bIsCalled;}
//	bool IsCalled() const {return m_bIsCalled;}


	bool operator == (const CPlayerInfo& rRight)const 
	{
		if(	//this->Index() == rRight.Index()
		this->m_strName == rRight.m_strName )//Name() 으로 하면 포인터가 돌아온다.
		{
			return true;
		}
		return false;
	}

protected:	
	CWorldUser	*m_pUser;
	string		m_strName;
//	bool		m_bIsCalled;
//	CLASS_TYPE	m_eClass;
//	DWORD		m_dwLv;
};

class CGuildMatchHandler
	: public CTeamMatchHandler< CPlayerInfo >// 유저명
{
public:
	CGuildMatchHandler(void)
	{
		WaitTime(0);
		ReadyPlayerCount(0);
	}
	
	virtual ~CGuildMatchHandler(void)
	{
	}

public:	
	virtual bool Open()
	{
		cout << "Open Called[ "<< typeid(this).name()<<" ]"<< endl;
		return true;
	}
	virtual void OnDestroy(){return;}

	virtual bool handle_insert()//insert 전 
	{
		return true;
	}

	virtual bool handle_remove()//insert 후
	{
		return true;
	}

	virtual void OnInsert()//insert 완료
	{//한명이 추가 될 때, 타입별 인원수에 맞는 사람이 들어오면 기다리는 시간을 추가해준다.
		cout << "OnInsert Called[ "<< typeid(this).name() <<" ]"<<  endl;
		cout << "Now Size [ "<< List().size() <<" ]"<<  endl;
				
		const size_t array_count = sizeof(g_aBasePlayerCount)/sizeof(COUNT_TYPE);

		const size_t prev_ready_count = ReadyPlayerCount();
		const size_t now_real_count = List().size();

		for(size_t i = 0; array_count > i; ++i)
		{
			const size_t &r_base_count = g_aBasePlayerCount[i];
			if(	prev_ready_count < r_base_count
			&&	now_real_count >= r_base_count )
			{
				ReadyPlayerCount( r_base_count );
				break;
			}
		} // for(size_t i = 0; array_count > i; ++i)

		if( prev_ready_count != ReadyPlayerCount() )//다음단계로 셋팅 될때마다 wait 타임이 걸린다.
		{
			WaitTime( ::timeGetTime() + g_dwGuildWarDelayTime );
		}
	}

	virtual void OnRemove()//remove 완료
	{
		cout << "OnRemove Called[ "<< typeid(this).name()<<" ]"<< endl;
		cout << "Now Size [ "<< List().size() <<" ]"<<  endl;

		const size_t array_count = sizeof(g_aBasePlayerCount)/sizeof(COUNT_TYPE);

		const size_t now_count = List().size();

		ReadyPlayerCount(0);//0으로 초기화 해야만 min 보다 작을 경우 게임이 진행 된다.

		for(size_t i = 0; array_count > i; ++i)
		{
			const size_t &r_base_count = g_aBasePlayerCount[i];
			if(	now_count >= r_base_count )
			{
				ReadyPlayerCount( r_base_count );
				break;
			}
		} // for(size_t i = 0; array_count > i; ++i)
	}
	
	virtual bool IsReady( size_t minplayerCount ) CONST
	{
		if( WaitTime() <= ::timeGetTime() )//웨이트 타임이 걸리지 않고
		{
			if( minplayerCount <= ReadyPlayerCount() )
			{
				return true;
			}
		}
		return false;
	}

	virtual bool OnMatchNotify( const CMatcherResult::NotifyResult& rNR )
	{//
		cout << "!!! OnMatchNotify Called[ "<< typeid(this).name()<<" ]"<< "TeamIndex[" << rNR.team_index<< "]" <<endl;
		const size_t target_count = rNR.player_count;//몇명짜리 전투냐.
		size_t count = 0;

		ELEMENT_LIST::iterator Itor = List().begin();
		
		while( List().end() != Itor)
		{
			T_PLAYER rPlayer = (*Itor);

			CWorldUser* pUser = g_pUserTable->GetUserInfoByName( rPlayer.Name() );

			if(pUser)
			{//유저 이동
				DUNGEON_MOVE_INFO DMI;
				DMI.wDungeonID = rNR.pStadium->wDungeonID;
				DMI.byLayerIndex = rNR.pStadium->byLayerIndex;
				
				DMI.wX = static_cast< WORD >(rNR.pStadium->aPos[rNR.team_index].x);
				DMI.wY = static_cast< WORD >(rNR.pStadium->aPos[rNR.team_index].y);

				STADIUM_PLAYER S_P(rNR.match_type, DMI,rNR.team_index, rNR.dwGuildWarFlag, rNR.team_type );
				rNR.pStadium->InsertUser( pUser, S_P );
//				rNR.pStadium->PlayerHash().Insert(rPlayer.Name(), S_P );//운동장에 사람 넣고
				if( ::MoveToStadium(pUser, DMI, rNR.team_index , rNR.dwGuildWarFlag) )
				{
					Itor = List().erase(Itor);//Erase 던전 이동 가능하면 지운다
				}
			} // if(pUser)
			else
			{
				++Itor;
			}

			if( target_count == (++count) )
			{
				break;
			}
		} // while( List().end() != Itor)

		OnRemove();

		return true;
	}//유저를 강제 이동 시키는 부분

	DWORD WaitTime() CONST 
	{
		return m_dwWaitTime;
	}
	void WaitTime(const DWORD dwWaitTime){m_dwWaitTime = dwWaitTime;}
	
	size_t ReadyPlayerCount() const {return m_ready_player;}
	void ReadyPlayerCount(const size_t ready_player)
	{
		cout << "ReadyPlayerCount [ "<< ready_player <<" ]"<< endl;
		m_ready_player = ready_player;
	}
private:
	DWORD m_dwWaitTime;
	size_t m_ready_player;
};