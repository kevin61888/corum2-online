// DUNGEON_DATA_EX.h: interface for the DUNGEON_DATA_EX class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DUNGEON_DATA_EX_H__36AA0CD6_6522_4119_9CC7_866802C63C95__INCLUDED_)
#define AFX_DUNGEON_DATA_EX_H__36AA0CD6_6522_4119_9CC7_866802C63C95__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "dungeon_data.h"
#include "VoidList.h"
struct V2_SPRITE;
class CUser;


/// Constants ////////////////////////////////////////////////////////////////

#define EVENT_DUNGEON_EFFECT_INCRESEMENT	0xFFF00000
#define EVENT_DUNGEON_RELEASE_INCRESEMENT	0x7FF00000

//----------------------------------------------------------------------------
class DUNGEON_DATA_EX  : public DUNGEON_DATA
{

public:

	BYTE				m_byDungeonType;					// 던젼 타입.
	
	DWORD				m_dwRemainSiegeStartTick;		// 공성전 할려면 몇시간 남았는가?
	DWORD				m_dwRemainSiegeEndTick;			// 공성전 방어하기 몇시간 남았는가?
	DWORD				m_dwRemainLevelStartTick;		// 던전 레벨 (시간이 지남에 따라 레벨이 증가한다.)
	DWORD				m_dwStopLevelTick;				// 멈추었을때 레벨업글되기까지의 남은시간.
	DWORD				m_dwStopStartTick;

	WORD				m_wWorldMapID;					// 위치한 월드 번호
#if IS_JAPAN_LOCALIZING()
	char				m_szSchoolName[MAX_CHARACTER_NAME_LENGTH]; // 일본판의 경우, 소유 학교 길드 이름
#endif

	BYTE				bReceivedInfo;		//World 서버로 부터 Owner 정보 받은 던전인가 아닌가 (0이면 아직 안받았슴, 1이면 정보 받았슴, 2이면 정보 대기중) 
	VECTOR3				vPos;				//월드맵상의 던전 좌표 	
	V2_SPRITE*			pSprMark;			//소유길드 마크 
	BYTE				bDivision;			//던전 구역 

	POSITION_			pListPos;
	POSITION_			pReceivedDungeonPos;	
	DUNGEON_DATA_EX*	pNext;
	DUNGEON_DATA_EX*	pPrev;
	GXOBJECT_HANDLE		m_hOwnDungeon;		// 자기 던전 표시 이펙트
	GXOBJECT_HANDLE		m_hSiegingDungeon;	// 던전중인 던전 표시 이펙트
	GXOBJECT_HANDLE		m_hGroupDungeon;	// 같은 그룹끼리의 던전 표시 이펙트 

	GXOBJECT_HANDLE		m_hEventDungeonModel;	// 이벤트 던젼등의 특수한 경우, 월드맵에 생성될 입구의 데이터를 직접 갖고 있어야 한다.
	GXOBJECT_HANDLE		m_hEventDungeonEffect;	// 이벤트 던젼등의 특수한 경우, 월드맵에 생성될 이펙트 데이터를 직접 갖고 있어야 한다.		
	
	
	void				SetDungeonType(DWORD dwID);
	INT					GetDungeonType();
	WORD				GetLevel();								// 던전 레벨
	WORD				GetUpgradeTime();						// 던전 업그레이드 시간
	int					GetIdleTime();							// 공성전 할때까지의 휴식기간.
	void				SetSiegeStartDestTime(DWORD dwTick);	// 공성전 될 시간 셋팅한다.
	void				SetSiegeEndDestTime(DWORD dwTick);
	BOOL				StartSiegeWarTime();					// 공성전 시작해도 되는가?
	BOOL				LevelUpgrade();							// 던전 레벨 업그레이드하기.
	int					GetRemainTime(DWORD dwDestTime, DWORD dwCurTick, DWORD dwFlag);	// TIME_SEC, TIME_MIN, TIME_HOUR
	
	BOOL				IsConquer();
	BOOL				IsVillage()const{ return (DUNGEON_TYPE_VILLAGE == m_byDungeonType);}//마을 맵이냐?: hwoarang 050202
	BOOL				IsPathWay()const{ return (DUNGEON_TYPE_TUNNEL == m_byDungeonType);}//통로 맵이냐?: hwoarang 050202
	bool				IsStadium()const{ return (DUNGEON_TYPE_MATCH_STATIDUM == m_byDungeonType);}//길드전 맵이냐?: hwoarang 050202

	BOOL				IsStartWarTime();
	BOOL				IsEndWarTime();
	BOOL				IsLevelUpgradeTime();
	DWORD				GetCPTime();							// cp시간.
	void				SetOwner(DWORD dwOwnerIndex, DWORD dwGuildID, char* pOwnerName, char* pszGuildName);	
	DWORD				GetEntrancePay();
	DWORD				GetMaxEntrancePay();
	void				Initialize();
	void				SetOperationMode(BYTE bOperationMode);
	void				StopOperationTime();					// 공성전이 시작되면 생산모드, 레벨모드의 시간이 멈추게 된다.
	void				ResumeOperationTime();					// 공성전이 멈추게 되면 생산모드, 레벨모드의 시간이 다시 흐르게 된다.
	BOOL				IsDungeonOwner(CUser* pUser);

	BOOL				IsKaien();

	// 이 아래로 Event Dungeon 관련 부분.	
	BYTE						m_byEventAlphaStep;		// 알파블렌딩 스텝.
	BYTE						m_byEventDisplayState;	// 화면 표시를 위한 스텝.
	BOOL						m_bEventUsed;	
	BYTE						m_byEventType;			// 이벤트 던젼 타입.
	DWORD						m_dwEventEntered;		// 이벤트 던젼에 들어간 유저 수.
	DWORD						m_dwEventStartTick;		// 이벤트 던젼 시작된 시각.
	DWORD						m_dwEventEndTick;
	DWORD						m_dwEventOpenReadyTime;	// 이벤트 던젼 입구 열릴 시간.
	DWORD						m_dwEndOpenReadyTime;
	BYTE						m_bEventState;			// 이벤트 던전 상태.
	BYTE						m_byEventEnterMinLevel;	// 이벤트 던젼 입장 가능 최소 레벨.
	BYTE						m_byEventEnterMaxLevel;	// 이벤트 던젼 입장 가능 최대 레벨.	
	//BOOL						IsEvent(VOID);			// 이벤트 던젼인가?
	
	DUNGEON_DATA_EX();
	virtual ~DUNGEON_DATA_EX();
};


//----------------------------------------------------------------------------

#endif // !defined(AFX_DUNGEON_DATA_EX_H__36AA0CD6_6522_4119_9CC7_866802C63C95__INCLUDED_)

