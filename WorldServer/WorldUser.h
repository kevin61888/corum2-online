#ifndef __WORLD_USER_H__
#define __WORLD_USER_H__


#pragma once


#include "LinkedListEx.h"
#include "OnlyList.h"
#include "ChinaBillingProc.h"
#include "../CommonServer/Item.h"
#include "../CommonServer/CommonPacket.h"


#define MAX_BELT					8
#define MAX_EQUIP					21
#define MAX_INV_LARGE				21
#define MAX_INV_SMALL				42				


#define EQUIP_TYPE_RHAND1			0		//
#define EQUIP_TYPE_RHAND2			1		
#define EQUIP_TYPE_RIDE				2		//탈것.. 비공정자리 
#define EQUIP_TYPE_HELMET			3		//
#define EQUIP_TYPE_MANTLE			4		//망또
#define EQUIP_TYPE_LHAND1			5		//
#define EQUIP_TYPE_LHAND2			6		
#define EQUIP_TYPE_RGLOVE			7			
#define EQUIP_TYPE_ARMOR			8		//갑옷
#define EQUIP_TYPE_AMULE1			9
#define EQUIP_TYPE_AMULE2			10
#define EQUIP_TYPE_AMULE3			11
#define EQUIP_TYPE_BELT				12
#define EQUIP_TYPE_LGLOVE			13
#define EQUIP_TYPE_RRING1			14
#define EQUIP_TYPE_RRING2			15
#define EQUIP_TYPE_RRING3			16
#define EQUIP_TYPE_BOOTS			17
#define EQUIP_TYPE_LRING1			18
#define EQUIP_TYPE_LRING2			19
#define EQUIP_TYPE_LRING3			20


#define USER_TYPE_GM				1
#define USER_TYPE_NORMAL			0
#define USER_TYPE_PC_ROOM			2


#define MAX_BILLING_ORDER_COUNT		8


#define NTOS_INIT		0x1
#define NTOS_RUN		0x2


#pragma pack(push,1)
struct WORLD_USER_CHK
{
	DWORD dwUserIndex[4];
};

struct GUILD_CHK
{	
	BYTE	bType;	
	char	szName[MAX_CHARACTER_NAME_LENGTH];
};
#pragma pack(pop)


struct NTOS_INFO
{
	BOOL		bStatus;
	DWORD		dwSync;
	DWORD		dwSyncPrev;
	DWORD		dwTime;
};


class CWorldUser
{
public:
	
	DWORD					m_dwPropID;				
	DWORD					m_dwConnectionIndex;							// Server side connection index
	DWORD					m_dwStatus;										// 현재 상태 
	DWORD					m_dwCurServerPos;								// (월드서버 제외하고)현재 접속하고 있는 서버 ID	
	BYTE					m_bUserType;									// USER_TYPE_GM, USER_TYPE_NORMAL
//	CLASS_TYPE				m_eClass;//클래스 타입 : hwoarang 050205
	char					m_szID[ MAX_ID_LENGTH ];						// ID	
	DWORD					m_dwUserIndex;
	char					m_szCharacterName[ MAX_CHARACTER_NAME_LENGTH  ];// 캐릭터 Name	
	char					m_szGuildName[MAX_GUILD_NAME_LENGTH];
	char					m_szNickName[MAX_GUILD_NAME_LENGTH];
	BYTE					m_byType;
	BYTE					m_byRank;
	DWORD					m_dwGuildId;
	DWORD					m_dwPartyId;
	DWORD					m_dwDungeonID;									// 최근 로그아웃한 던전 번호로 사용됨 
	BYTE					m_bWorldId;										// 최근 들어갔던 월드맵 번호(ID)	1번부터 	
	BYTE					m_bySkillKey[16];
	BYTE					m_byPosKey[16];
	BYTE					m_byHelp[3];	
	BYTE					m_byCurrentHand;	
	BYTE					m_byMessenger;
	DBTIMESTAMP				m_sGuildJoinTime;
	DWORD					m_dwPrevGuildId;
	BYTE					m_byMessengerStatus;
	GUILD_CHK				m_sGuildChk[4];	
	DWORD					m_dwPrevDungeonId;
	BOOL					m_bCommand;
	BOOL					m_bParty;
	BOOL					m_bConnectGameRoom;								// 게임방에서 접속했다.
	short					m_wCriminalTime;								// 범죄 타임	
	BYTE					m_bDestWorldSpot;								// 통로던전일때 들어왔던곳을 스폿으로 기억한다. 예) 0 일때는 2001, 1일때는 2002로 나가야 하므로..	
	DWORD					m_dwStartTick;									// 접속해서 게임 시작한 시간. 
	time_t					m_sTime;
	BYTE					m_byLeader;	
	WORD					m_wHp;
	WORD					m_wMaxHp;
	DWORD					m_dwLevel;
	WORD					m_wHead;										// 머리 번호.
	WORD					m_wClass;				
	DWORD					m_dwMoney;
	CItem					m_pEquip[ MAX_EQUIP ];					
	VECTOR3					m_v3CurWorldPos;								// 월드맵에서 현재 유저가 있는 위치 
	POSITION_				m_ListPosInWorld;								// 월드에 있는 유저 리스트 Position
	POSITION_				m_ListTotalUser;								// 전체 유저 리스트용 Position	
	BYTE					m_bIsMapLoading;								// 맵 로딩중인 Flag
	BYTE					m_bCheckSended;									// Check Packet을 보냈다 
	BYTE					m_bKeepAliveCheckFlag;			
	DWORD					m_dwCDTableBucketID;							// ConnectionDistributeTable의 BucketID
	POSITION_				m_ListPosCDTable;								// ConnectionDistributeTable의 Node Position
	DWORD					m_dwCheckSendedTick;							// For Test
	BYTE					m_bKicked;										// 중복로그인으로 Kick당하는 거면 
	BYTE					m_bBillingType;									// 결제 타입 
	DWORD					m_dwRemainTime;									// 종량제 남은시간 
	DBTIMESTAMP				m_BillingEday;									// 결제 끝나는시각 
	BYTE					m_byPortal;	
	CWorldUser*				m_pNextData;
	CWorldUser*				m_pPrevData;	
	CDugneonRefreshList*	m_pListRefreshFromDungeon;						// 갱신값을 받아야 할 던전리스트들.
	COwnDugneonList*		m_pListOwnDungeon;								// 자기 던전리스트
	char					m_szSearchName[MAX_CHARACTER_NAME_LENGTH];		// 캐릭터 Name
	BOOL					m_bIsMonitor;									// 모니터링 툴인가?
	NTOS_INFO				m_NTOS;

	DWORD					m_dwBillingStartTick;							// 과금을 시작한 시간의 Tick
	BILLING_INFO			m_BillingInfo;									// 결제 정보
	SCHINA_BILLING_INFO		m_sChinaBillingInfo;

	// 파티 매칭 : 최덕석 2005.1.21
	PMATCH_CONDITION		m_PMatchCondition;
	DWORD					m_dwPMatchIndex;
	PARTY_CONFIG			m_PartyConfig;
	BYTE					m_bLevelDist;
	int						m_iSign;

	BOOL					m_bGuildWarRequestApply;						// 길드전 수락 여부
	GUILD_WAR_REQUEST		m_RequestGuildWarRequest;						// 길드전 요청 데이타

	// Guild Macth Condition Default; //TEAM_MATCH : hwoarang 050129
	G_MC_DEFAULT			m_GMC;
	BYTE					m_byLayerIndex;//길드전시에만 갱신 됩니다. 평상시에는 사용 하지 마세요
	DWORD					m_dwGuildWarFlag;//G_W_F_xxxx
	size_t					m_team_index;//

	bool IsHaveGMC();
	void ClearGuildWarData();

public:

	void					SetCurDungeon(WORD wDungeonID);
	void					SetExchangePKCount(int nCount, DWORD dwPKEndTime);
	void					SetExchangePKRepeatCount(int nCount, DWORD dwPKRepeatEndTime);
	void					GuildIdChange();
	void					GuildIdPoint();	
	BOOL					IsGMUser() const	{	return BOOL(USER_TYPE_GM == m_bUserType);	}

	CWorldUser()
	{
		m_byLayerIndex = 0;
		m_dwGuildWarFlag = 0;
		m_team_index = 0;
	};

	~CWorldUser(){};

};

#endif
