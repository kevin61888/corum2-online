#pragma once

#include <map>
#include <deque>
#include <algorithm>

using namespace std;


template<typename T_CONDITION, typename T_USER, class T_ACTION>
class CMatcher
{
//////////////
// 제네릭 매쳐 : 최덕석 2005.1.29
//
// 대상들을 로비로 입장시키고
// 대상에 맞는 상대를 찾으면 그들의 대기실을 만들어준다
// 매칭이 성공하면 대기실의 파장만 로비로 돌려보내고 대기실을 없앤다.
// 매칭이 실패하면 그들을 로비로 돌려보내고 대기실을 없앤다.

	typedef deque<T_USER> V_USER;
	typedef map<T_CONDITION, V_USER> M_LOBBY;

	struct SReadyRoom
	{
		T_CONDITION cndOwn;
		T_USER usrOwn;
		T_CONDITION cndGuest;
		T_USER usrGuest;
		DWORD dwCreateTime;
	};

	typedef map<int, SReadyRoom> M_READYROOM;

protected:
	M_LOBBY m_Lobby;					// 상대를 기다리는 로비
	M_READYROOM m_ReadyRoom;		// 매칭 수락을 기다리는 대기실
	T_ACTION m_Handler;				// 매칭자

public:
	// 로비로 입장
	int Insert(T_CONDITION& cnd, T_USER& usr, bool bMatching = true)
	{
		T_CONDITION cndOwn;
		T_USER usrOwn;
		
		// 로비 등록하면서 바로 상대를 찾으라면
		if(bMatching && Search(cnd, usr, cndOwn, usrOwn))
		{
			// 상대와 매칭을 주선
			Match(cndOwn, usrOwn, cnd, usr);
			return true;
		}
		
		// 원하는 조건의 로비가 없다면
		M_LOBBY::iterator i = m_Lobby.find(cnd);
		if(i == m_Lobby.end())
		{
			// 새 로비를 만들고
			i = m_Lobby.insert(m_Lobby.end(), make_pair(cnd, deque<T_USER>()));
		}

		// 로비에 대상을 등록
		i->second.push_back(usr);
		
		return true;
	}

	// 로비에서 나감
	bool Remove(T_CONDITION& cnd, T_USER& usr)
	{
		M_LOBBY::iterator i =  m_Lobby.find(cnd);

		// 조건의 로비가 없다면
		if(i == m_Lobby.end())
		{
			// 실패
			return false;
		}

		// 로비에서 대상을 찾음
		V_USER& vecUser = i->second;
		V_USER::iterator j = find(vecUser.begin(), vecUser.end(), usr);
		
		// 대상이 있다면
		if(j != vecUser.end())
		{
			// 로비에서 대상을 탈퇴
			vecUser.erase(j);
			return true;
		}
		// 대상이 없다면
		else
		{
			// 실패
			return false;
		}
	}

	// 매칭 상대를 찾음
	bool Search(T_CONDITION& cndGuest, T_USER& usrGuest, T_CONDITION& cndOwn, T_USER& usrOwn/*out*/)
	{
		M_LOBBY::iterator i;
		T_CONDITION cnd;
		
		// 비슷한 조건들 모두에 대해
		while(m_Handler.MakeCondition(usrGuest, cndGuest, cnd))
		{
			// 비슷한 조건의 로비를 찾아라
			i = m_Lobby.find(cnd);

			// 로비가 있고 상대도 있다면
			if(i != m_Lobby.end() && i->second.size() > 0 && m_Handler.CanMatch(i->second.front(), usrGuest))
			{
				// 찾은 로비와 상대를 알려줌
				cndOwn = cnd;
				usrOwn = i->second.front();
				
				return true;
			}
		}

		// 찾지 못했음
		return false;
	}

	// 두 대상을 매칭 시켜라
	int Match(T_CONDITION& cndOwn, T_USER& usrOwn, T_CONDITION& cndGuest, T_USER& usrGuest)
	{
		static int matchId = 1;

		// 새 대기실 번호를 만듬
		matchId++;

		// 대기실을 만들고 두 대상을 입장 시킴
		SReadyRoom room = {cndOwn, usrOwn, cndGuest, usrGuest, g_dwCurTick};
		m_ReadyRoom.insert(make_pair(matchId, room));

		// 매칭을 시키고
		m_Handler.DoMatch(usrOwn, usrGuest, matchId);

		// 로비에서 두 대상을 탈퇴
		Remove(cndOwn, usrOwn);
		Remove(cndGuest, usrGuest);

		return matchId;
	}

	// 대기실 정보를 알아냄
	bool GetReadyDB(int matchId, T_USER& usrOwn, T_USER& usrGuest)
	{
		M_READYROOM::iterator i = m_ReadyRoom.find(matchId);
		
		// 대기실이 없다면
		if(i == m_ReadyRoom.end())
		{
			// 실패
			return false;
		}
		
		// 대기실 정보를 알려줌
		SReadyRoom& room = i->second;
		usrOwn = room.usrOwn;
		usrGuest = room.usrGuest;

		return true;
	}

	// 매칭 완료
	bool MatchSuccess(int matchId)
	{
		M_READYROOM::iterator i = m_ReadyRoom.find(matchId);
		
		// 대기실이 없다면
		if(i == m_ReadyRoom.end())
		{
			// 실패
			return false;
		}

		// 파장을 로비로 보내고 대기실을 없앰
		SReadyRoom& room = i->second;
		Insert(room.cndOwn, room.usrOwn, false);
		m_ReadyRoom.erase(i);

		return true;
	}

	// 매칭 실패
	bool MatchFail(int matchId)
	{
		M_READYROOM::iterator i = m_ReadyRoom.find(matchId);
		
		// 대기실이 없다면
		if(i == m_ReadyRoom.end())
		{
			// 실패
			return false;
		}


		// 대기실의 두 대상을 로비로 보내고 대기실을 없앰
		SReadyRoom& room = i->second;
		Insert(room.cndOwn, room.usrOwn, false);
		Insert(room.cndGuest, room.usrGuest, false);

		m_Handler.CancelMatch(room.usrOwn, room.usrGuest);

		m_ReadyRoom.erase(i);

		return true;
	}

	// 자동 매칭
	void Process(int iCount)
	{
		M_READYROOM::iterator r;
		M_LOBBY::iterator i;
		static int flag = 0;
	
		
		//// 응답 없는 대기실, 취소 시키기
		r = m_ReadyRoom.begin();
		// 모든 대기실에 대해
		for(;r != m_ReadyRoom.end();r++)
		{
			SReadyRoom& room = r->second;
			
			// 대기실이 만들어 진지 10초가 지났다면
			if(g_dwCurTick - room.dwCreateTime > 10000)
			{
                // 매칭을 취소 시킴
				MatchFail(r->first);

				// 이터레이터가 깨지므로 다시 만듬
				r = m_ReadyRoom.begin();
			}
		}

		i = m_Lobby.begin();

		for(int n = 0;n<flag && i != m_Lobby.end();n++)
			i++;

		for(;i != m_Lobby.end();i++, flag++)
		{
			if(i->second.size() == 0)
				continue;

			T_CONDITION cnd, cndOwn;
			T_USER usr, usrOwn;

			cnd = i->first;
			usr = i->second.front();
			i->second.pop_front();

			if(Search(cnd, usr, cndOwn, usrOwn))
			{
				// 상대와 매칭을 주선
				Match(cndOwn, usrOwn, cnd, usr);
			}
			else
			{
				// 뒤로 보냄, 기회의 균등
				i->second.push_back(usr);
			}

			iCount--;

			if(iCount == 0)
				break;
		}

		if(i == m_Lobby.end())
		{
			flag = 0;
		}
	}
};