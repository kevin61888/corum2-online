//======================================================//
// Code by Jang.							2003.07.21	// 
// Obejct : Group Window.								//
//======================================================//
#ifndef		__GROUP_INC__
#define		__GROUP_INC__

#include	"Menu.h"

#define SPR_OBJ_GROUPWND				0
#define SPR_OBJ_GROUPWND_CLOSE1			1	
#define SPR_OBJ_GROUPWND_CLOSE2			2
#define SPR_OBJ_GROUPWND_PARTY1			3
#define SPR_OBJ_GROUPWND_PARTY2			4
#define SPR_OBJ_GROUPWND_GUILD1			5
#define SPR_OBJ_GROUPWND_GUILD2			6
#define SPR_OBJ_GROUPWND_FRIEND1		7
#define SPR_OBJ_GROUPWND_FRIEND2		8
#define SPR_OBJ_GROUPWND_GUILDWAR1		9
#define SPR_OBJ_GROUPWND_GUILDWAR2		10
#define SPR_OBJ_GROUPWND_ADD1			11
#define SPR_OBJ_GROUPWND_ADD2			12
#define SPR_OBJ_GROUPWND_ADD3			13
#define SPR_OBJ_GROUPWND_DELETE1		14
#define SPR_OBJ_GROUPWND_DELETE2		15
#define SPR_OBJ_GROUPWND_DELETE3		16
#define SPR_OBJ_GROUPWND_INFO1			17
#define SPR_OBJ_GROUPWND_INFO2			18
#define SPR_OBJ_GROUPWND_INFO3			19
#define SPR_OBJ_GROUPWND_OUT1			20
#define SPR_OBJ_GROUPWND_OUT2			21
#define SPR_OBJ_GROUPWND_OUT3			22
#define SPR_OBJ_GROUPWND_MSG1			23
#define SPR_OBJ_GROUPWND_MSG2			24
#define SPR_OBJ_GROUPWND_MSG3			25
#define SPR_OBJ_GROUPWND_SELECT			26
#define SPR_OBJ_GROUPWND_GUILDWARADD1	27
#define SPR_OBJ_GROUPWND_GUILDWARADD2	28
#define SPR_OBJ_GROUPWND_GUILDWARADD3	29
#define SPR_OBJ_GROUPWND_GUILDWAREXIT1	30
#define SPR_OBJ_GROUPWND_GUILDWAREXIT2	31
#define SPR_OBJ_GROUPWND_GUILDWAREXIT3	32
#define SPR_OBJ_GUILD_PAGE				33
#define SPR_OBJ_GUILD_PAGEPREV1			34
#define SPR_OBJ_GUILD_PAGEPREV2			35
#define SPR_OBJ_GUILD_PAGEPREV3			36
#define SPR_OBJ_GUILD_PAGENEXT1			37
#define SPR_OBJ_GUILD_PAGENEXT2			38
#define SPR_OBJ_GUILD_PAGENEXT3			39
#define SPR_OBJ_GUILD_ONOFF1			40
#define SPR_OBJ_GUILD_ONOFF2			41
#define SPR_OBJ_GUILD_ONOFF3			42
#define SPR_OBJ_GUILD_INFO1				43
#define SPR_OBJ_GUILD_INFO2				44
#define SPR_OBJ_GUILD_INFO3				45
#define SPR_OBJ_GUILD_LEVEL1			46
#define SPR_OBJ_GUILD_LEVEL2			47
#define SPR_OBJ_GUILD_LEVEL3			48
#define SPR_OBJ_GUILD_LEVEL4			49
#define SPR_OBJ_GUILD_MARK				50
#define SPR_OBJ_GROUPWND_STATUS			51
#define SPR_OBJ_GROUP_MASTER1			52
#define SPR_OBJ_GROUP_MASTER2			53
#define SPR_OBJ_GROUP_MASTER3			54
#define SPR_OBJ_GUILDWAR_GUILD			55
#define SPR_OBJ_GROUPWND_PARTYBAR1		56
#define SPR_OBJ_GROUPWND_PARTYBAR2		57
#define SPR_OBJ_GROUPWND_PARTYBAR3		58
#define SPR_OBJ_GROUPWND_PARTYBAR4		59
#define SPR_OBJ_GROUPWND_PARTYBAR5		60
#define SPR_OBJ_GROUPWND_PARTYBAR6		61
#define SPR_OBJ_GROUPWND_PARTYBAR7		62
#define SPR_OBJ_GROUPWND_PARTYGAGE1		63
#define SPR_OBJ_GROUPWND_PARTYGAGE2		64
#define SPR_OBJ_GROUPWND_PARTYGAGE3		65
#define SPR_OBJ_GROUPWND_PARTYGAGE4		66
#define SPR_OBJ_GROUPWND_PARTYGAGE5		67
#define SPR_OBJ_GROUPWND_PARTYGAGE6		68
#define SPR_OBJ_GROUPWND_PARTYGAGE7		69
#define SPR_OBJ_GROUPWND_PBCREATE1		70
#define SPR_OBJ_GROUPWND_PBCREATE2		71
#define SPR_OBJ_GROUPWND_PBCREATE3		72
#define SPR_OBJ_GROUPWND_PBDESTROY1		73
#define SPR_OBJ_GROUPWND_PBDESTROY2		74
#define SPR_OBJ_GROUPWND_PBDESTROY3		75

//파티 강화 시스템 (정우창 2005.01.25)
#define SPR_OBJ_GROUPWND_PARTYUSERINFO1					76      //  파티창 6번째 버튼 비활성화 (사용안함) 
#define SPR_OBJ_GROUPWND_PARTYUSERINFO2					77		//	파티창 6번째 버튼 활성화 
#define SPR_OBJ_GROUPWND_PARTYUSERINFO3					78		//	파티창 6번째 버튼 버튼 클릭시 
#define SPR_OBJ_GROUPWND_EXPITEMCONDITION1				79		//  파티창 7번째 버튼 비활성화 
#define SPR_OBJ_GROUPWND_EXPITEMCONDITION2				80		//	파티창 7번째 버튼 활성화 
#define SPR_OBJ_GROUPWND_EXPITEMCONDITION3				81		//	파티창 7번째 버튼 버튼 클릭시 
#define SPR_OBJ_GROUPWND_PMATCHCONDITION1				82		//	파티창 8번째 버튼 비활성화 (사용안함)
#define SPR_OBJ_GROUPWND_PMATCHCONDITION2				83		//	파티창 8번째 버튼 활성화	
#define SPR_OBJ_GROUPWND_PMATCHCONDITION3				84		//	파티창 8번째 버튼 버튼 클릭시 
#define SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF			85		//	매칭시스템설정 후 확인 버튼 (비활성화)
#define SPR_OBJ_GROUPWND_PARTYFRIENDADD					86		//	파티 시 친구추가 버튼 
#define SPR_OBJ_GROUPWND_CONDITIONLIST_EXP1				87		//	매칭시스템설정 리스트 박스 오른쪽
#define SPR_OBJ_GROUPWND_CONDITIONLIST_EXP2				88		//	매칭시스템설정 리스트 박스 가운데 
#define SPR_OBJ_GROUPWND_CONDITIONLIST_EXP3				89		//	매칭시스템설정 리스트 박스 버튼 활성화 
#define SPR_OBJ_GROUPWND_CONDITIONLIST_EXP4				90		//	매칭시스템설정 리스트 박스 버튼 클릭 
#define SPR_OBJ_GROUPWND_CONDITIONLIST_EXP5				91		//	매칭시스템설정 리스트 박스 버튼 비활성화 
#define SPR_OBJ_GROUPWND_CONDITIONLIST_EXP6				92		//	매칭시스템설정 리스트 박스 백판 
#define SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM1			93		//	매칭시스템설정 리스트 박스 오른쪽
#define SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM2			94		//	매칭시스템설정 리스트 박스 가운데 
#define SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM3			95		//	매칭시스템설정 리스트 박스 버튼 활성화 
#define SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM4			96		//	매칭시스템설정 리스트 박스 버튼 클릭 
#define SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM5			97		//	매칭시스템설정 리스트 박스 버튼 비활성화 
#define SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM6			98		//	매칭시스템설정 리스트 박스 백판 
#define SPR_OBJ_GROUPWND_EXPITEMCONDITI_OK1             99      //  확인 버튼 비활성화 
#define SPR_OBJ_GROUPWND_EXPITEMCONDITI_OK2				100     //  확인 버튼 활성화  
#define SPR_OBJ_GROUPWND_EXPITEMCONDITI_OK3				101     //  확인 버튼 눌렸을때 
#define SPR_OBJ_GROUPWND_EXPITEMCONDITI_CANCEL1			102     //  취소 버튼 비활성화 
#define SPR_OBJ_GROUPWND_EXPITEMCONDITI_CANCEL2			103		//  취소 버튼 활성화 
#define SPR_OBJ_GROUPWND_EXPITEMCONDITI_CANCEL3			104     //  취소 버튼 눌렸을때 
//#define SPR_OBJ_GROUPWND_PMATCHCONDITION_OK1           105     //  검색할 파티의 조건 설정 확인 버튼 비활성화 
//#define SPR_OBJ_GROUPWND_PMATCHCONDITION_OK2			106     //  검색할 파티의 조건 설정확인 버튼 활성화  
//#define SPR_OBJ_GROUPWND_PMATCHCONDITION_OK3			107     //  검색할 파티의 조건 설정확인 버튼 눌렸을때 
//#define SPR_OBJ_GROUPWND_PMATCHCONDITION_CANCEL1		108     //  검색할 파티의 조건 설정취소 버튼 비활성화 
//#define SPR_OBJ_GROUPWND_PMATCHCONDITION_CANCEL2		109		//  검색할 파티의 조건 설정취소 버튼 활성화 
//#define SPR_OBJ_GROUPWND_PMATCHCONDITION_CANCEL3		110     //  검색할 파티의 조건 설정취소 버튼 눌렸을때 
#define SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_ON			111		//	매칭시스템설정 후 확인 버튼 (활성화)
#define SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV1				112		//	매칭시스템설정 리스트 박스 오른쪽
#define SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV2				113		//	매칭시스템설정 리스트 박스 가운데 
#define SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV3				114		//	매칭시스템설정 리스트 박스 버튼 활성화 
#define SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV4				115		//	매칭시스템설정 리스트 박스 버튼 클릭 
#define SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV5				116		//	매칭시스템설정 리스트 박스 버튼 비활성화 
#define SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV6				117		//	매칭시스템설정 리스트 박스 백판 
#define SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP1				118		//	매칭시스템설정 리스트 박스 오른쪽
#define SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP2				119		//	매칭시스템설정 리스트 박스 가운데 
#define SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP3				120		//	매칭시스템설정 리스트 박스 버튼 활성화 
#define SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP4				121		//	매칭시스템설정 리스트 박스 버튼 클릭 
#define SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP5				122		//	매칭시스템설정 리스트 박스 버튼 비활성화 
#define SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP6				123		//	매칭시스템설정 리스트 박스 백판 
#define SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM1				124		//	매칭시스템설정 리스트 박스 오른쪽
#define SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM2				125		//	매칭시스템설정 리스트 박스 가운데 
#define SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM3				126		//	매칭시스템설정 리스트 박스 버튼 활성화 
#define SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM4				127		//	매칭시스템설정 리스트 박스 버튼 클릭 
#define SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM5				128		//	매칭시스템설정 리스트 박스 버튼 비활성화 
#define SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM6				129		//	매칭시스템설정 리스트 박스 백판 
#define SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO1			130		//	매칭시스템설정 리스트 박스 오른쪽
#define SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO2			131		//	매칭시스템설정 리스트 박스 가운데 
#define SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO3			132		//	매칭시스템설정 리스트 박스 버튼 활성화 
#define SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO4			133		//	매칭시스템설정 리스트 박스 버튼 클릭 
#define SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO5			134		//	매칭시스템설정 리스트 박스 버튼 비활성화 
#define SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO6			135		//	매칭시스템설정 리스트 박스 백판 

//길드전 메뉴버튼 4개 *3 (활성, 눌렸을때, 비활성)
#define SPR_OBJ_GUILDWAR_RANK1					136
#define SPR_OBJ_GUILDWAR_RANK2					137
#define SPR_OBJ_GUILDWAR_RANK3					138
#define SPR_OBJ_GUILDWAR_LIST1					139
#define SPR_OBJ_GUILDWAR_LIST2					140
#define SPR_OBJ_GUILDWAR_LIST3					141
#define SPR_OBJ_GUILDWAR_MATCH1					142
#define SPR_OBJ_GUILDWAR_MATCH2					143
#define SPR_OBJ_GUILDWAR_MATCH3					144
#define SPR_OBJ_GUILDWAR_CHALLENGE1				145
#define SPR_OBJ_GUILDWAR_CHALLENGE2				146
#define SPR_OBJ_GUILDWAR_CHALLENGE3				147
//길드전 신청 버튼
#define SPR_OBJ_GUILDWAR_APPLY1					148
#define SPR_OBJ_GUILDWAR_APPLY2					149
#define SPR_OBJ_GUILDWAR_APPLY3					150
//관전 버튼
#define SPR_OBJ_GUILDWAR_OBSERVE1				151
#define SPR_OBJ_GUILDWAR_OBSERVE2				152
#define SPR_OBJ_GUILDWAR_OBSERVE3				153
//드롭다운 리소스 3종
#define DROPDOWN_OBJ_GUILDWAR_FORM_LEFT			154
#define DROPDOWN_OBJ_GUILDWAR_FORM_MIDDLE		155
#define DROPDOWN_OBJ_GUILDWAR_FORM_RIGHT1		156
#define DROPDOWN_OBJ_GUILDWAR_FORM_RIGHT2		157
#define DROPDOWN_OBJ_GUILDWAR_FORM_RIGHT3		158
#define DROPDOWN_OBJ_GUILDWAR_FORM_BOARD		159

#define DROPDOWN_OBJ_GUILDWAR_NUMBER_LEFT		160
#define DROPDOWN_OBJ_GUILDWAR_NUMBER_MIDDLE		161
#define DROPDOWN_OBJ_GUILDWAR_NUMBER_RIGHT1		162
#define DROPDOWN_OBJ_GUILDWAR_NUMBER_RIGHT2		163
#define DROPDOWN_OBJ_GUILDWAR_NUMBER_RIGHT3		164
#define DROPDOWN_OBJ_GUILDWAR_NUMBER_BOARD		165

#define DROPDOWN_OBJ_GUILDWAR_TYPE_LEFT			166
#define DROPDOWN_OBJ_GUILDWAR_TYPE_MIDDLE		167
#define DROPDOWN_OBJ_GUILDWAR_TYPE_RIGHT1		168
#define DROPDOWN_OBJ_GUILDWAR_TYPE_RIGHT2		169
#define DROPDOWN_OBJ_GUILDWAR_TYPE_RIGHT3		170
#define DROPDOWN_OBJ_GUILDWAR_TYPE_BOARD		171
//오토매칭 버튼
#define SPR_OBJ_GUILDWAR_AUTOMATCHING1			172
#define SPR_OBJ_GUILDWAR_AUTOMATCHING2			173
#define SPR_OBJ_GUILDWAR_AUTOMATCH_V			174
//좌우 페치지 넘기기(많이:10)
#define SPR_OBJ_GUILDWAR_PAGEPREV_DOUBLE1		175
#define SPR_OBJ_GUILDWAR_PAGEPREV_DOUBLE2		176
#define SPR_OBJ_GUILDWAR_PAGEPREV_DOUBLE3		177
#define SPR_OBJ_GUILDWAR_PAGENEXT_DOUBLE1		178
#define SPR_OBJ_GUILDWAR_PAGENEXT_DOUBLE2		179
#define SPR_OBJ_GUILDWAR_PAGENEXT_DOUBLE3		180
//랭킹, 리스트, 매칭, 도전 타이틀바
#define SPR_GUILDWAR_TITLEBAR_RANK				181
#define SPR_GUILDWAR_TITLEBAR_LIST				182
#define SPR_GUILDWAR_TITLEBAR_MATCH				183
//파티 시스템 친구 추가 (정우창 2005.02.22)
#define SPR_OBJ_GROUPWND_FRIENDADD_OFF1			184
#define SPR_OBJ_GROUPWND_FRIENDADD_OFF2			185
#define SPR_OBJ_GROUPWND_FRIENDADD_OFF3			186
#define SPR_OBJ_GROUPWND_FRIENDADD_OFF4			187
#define SPR_OBJ_GROUPWND_FRIENDADD_OFF5			188
#define SPR_OBJ_GROUPWND_FRIENDADD_OFF6			189
#define SPR_OBJ_GROUPWND_FRIENDADD_OFF7			190
#define SPR_OBJ_GROUPWND_FRIENDADD_ON1			191
#define SPR_OBJ_GROUPWND_FRIENDADD_ON2			192
#define SPR_OBJ_GROUPWND_FRIENDADD_ON3			193
#define SPR_OBJ_GROUPWND_FRIENDADD_ON4			194
#define SPR_OBJ_GROUPWND_FRIENDADD_ON5			195
#define SPR_OBJ_GROUPWND_FRIENDADD_ON6			196
#define SPR_OBJ_GROUPWND_FRIENDADD_ON7			197


#define PARTY_FRIEND_MAX_PAGE					3    //파티현황 (정우창 2005. 03.04) 최근 파티 현황 MAX 페이지 


//한 패이지당 나오는 리스트 갯수(텍스트)
#define MAXLINE_PER_PAGE						15
//신청, 관전버튼
#define GUILDWAR_CHECK_RETURN_APPLY				56
//자동매칭, 55로 바뀌면서 안쓴다.
#define GUILDWAR_CHECK_RETURN_MATCH				57
//드롭다운 버튼에 대한 리턴값
#define GUILDWAR_CHECK_DROPDOWN_BUTTON_FORM		58
#define GUILDWAR_CHECK_DROPDOWN_BUTTON_NUMBER	59
#define GUILDWAR_CHECK_DROPDOWN_BUTTON_TYPE		60
//드롭다운 리스트 체크 리턴 값(리스트가 추가되면 번호를 죽 밀려줘야 한다.)
#define GUILDWAR_CHECK_DROPDOWN_FORM			61
#define GUILDWAR_CHECK_DROPDOWN_NUMBER			69
#define GUILDWAR_CHECK_DROPDOWN_TYPE			77
//글자 찍혀있는 부분 체크(15줄)
#define GUILDWAR_CHECK_TEXT_LINE_NUMBER			85
//페이지 넘기기 많이(10)
#define GUILDWAR_CHAGE_DOUBLE_PAGEPREV			100
#define GUILDWAR_CHAGE_DOUBLE_PAGENEXT			101


#define GROUP_RENDER_TEXT__GUILDWAR_RANK		41
#define GROUP_RENDER_TEXT__GUILDWAR_LIST		42
#define GROUP_RENDER_TEXT__GUILDWAR_MATCH		43
#define GROUP_RENDER_TEXT__GUILDWAR_CHALLENGE	44


class CGroupWnd : public CMenu
{
//파티 강화 시스템(정우창 2005.01.28)
public:
	PARTY_CONFIG_EXP	m_nExpDistrib;				//파티 탭 메뉴 7번째 버튼에서 첫번째 드롭다운 
	PARTY_CONFIG_ITEM	m_nItemRouting;				//파티 탭 메뉴 7번째 버튼에서 두번째 드롭다운 

	BYTE				m_bLevelMatch;				//파티 탭 메뉴 8번째 버튼에서 첫번째 드롭다운 
	BYTE				m_bExpMatch;				//파티 탭 메뉴 8번째 버튼에서 두번째 드롭다운 
	BYTE				m_bItemMatch;				//파티 탭 메뉴 8번째 버튼에서 세번째 드롭다운 
	BYTE				m_bMasterMatch;				//파티 탭 메뉴 8번째 버튼에서 네번째 드롭다운 
//파티 강화 시스템(정우창 2005.02.01)	
	BYTE				m_byBackBoard;				//백판 초기값:0
	BYTE		    	m_byPMatchStatus;           //매치 상태 초기값:0     매칭 상태일 경우 0, 매칭 상태가 아닐 경우 1
	int					m_nPMatchResult;            //대기자 등록 결과 받아 오기 0:성공 아니면 에러코드 
	int                 m_nPMatchEscapeResult;	    //대기자 탈퇴 결과 받아 오기 0:성공 아니면 에러코드 
	BYTE                m_bMasterMatchStatus;       //파티원,파티장 판단 
	PARTY_FRIEND		m_nPartyFriend[20];				//파티현황(정우창 2005.03.03)
	DWORD				m_dwPartyCount;				//파티현황(정우창 2005. 03.03)
	
 	// sung-han 2005-01-29 길드전랭킹 리스트
	//GUILD_LADDER_DATA m_LadderList[MAX_LADDER_LIST];//길드전랭킹 리스트에 출력할 리스트
	GuildLadderInfoPage m_pGuildWarRankListInfoPage;//길드전랭킹 리스트에 출력할 리스트
	int 		m_nGuildWarRankListPage;//길드전랭킹 리스트의 페이지 (초기값:1)
	int			m_nGuildWarRankListSelect;//길드전랭킹 리스트에서 선택한 라인 (초기값:-1)
	int			m_nGuildWarRankListTotalCount;//길드전랭킹 리스트의 갯수 (초기값:0)
	int			m_nGuildWarRankListMaxPage;//길드전랭킹 리스트의 페이지 최대값 (초기값:0)
	BYTE		m_byBtnChallange;//도전버튼 상태(초기값:0)
	BYTE		m_byBtnLeftDouble;//페이지 왼쪽 두개버튼(초기값:0)
	BYTE		m_byBtnRightDouble;//페이지 오른쪽 두개버튼(초기값:0)

	// sung-han 2005-01-29 길드전 리스트
	GUILD_WAR_DATA m_GuildWarList[MAX_LADDER_LIST];//길드전 리스트에 출력할 리스트
	int 		m_nGuildWarListPage;//길드전 리스트의 페이지 (초기값:1)
	int			m_nGuildWarListSelect;//길드전 리스트에서 선택한 라인 (초기값:-1)
	int			m_nGuildWarListTotalCount;//길드전 리스트의 갯수 (초기값:0)
	int			m_nGuildWarListMaxPage;//길드전 리스트의 페이지 최대값 (초기값:0)
	BYTE		m_byBtnOvserve;//관전버튼 상태(초기값:0)

	// sung-han 2005-01-29 길드전 매칭 옵션
	BYTE		m_byGuildWarDropDownEnableNum;//현재 열려있는 드롭다운 박스(초기값:0)
	BYTE		m_bynGuildWarForm;//경기방식(초기값:0, 첫번째것이 선택된 상태)
	BYTE		m_byGuildWarNumber;//참여길드 수(초기값:0, 첫번째것이 선택된 상태)
	BYTE		m_byGuildWarType;//경기타입(초기값:0, 첫번째것이 선택된 상태)
	BOOL		m_bGuildWarMatchEnable;//매칭 작동중인가(초기값:FALSE)
	MOUSECHECK 	m_byGuildWarDropDownButtonStatus;//드롭다운 버튼의 상태, 세군데 드롭다운에서 공통으로 사용(초기값:0)
	char		m_szGuildWarMatch[4][TEXT_RENDER_BUFFER];
	char		m_szGuildWarNumber[4][TEXT_RENDER_BUFFER];
	char		m_szGuildWarType[4][TEXT_RENDER_BUFFER];

	// sung-han 2005-01-29 길드전 도전
	BOOL		m_bGuildWarChallenge;//길드전 도전 버튼을 클릭한 상태인가(초기값:FALSE)

	// sung-han 2005-01-30 길드전 패킷 보낸것 체크
	BOOL		m_bGuildWarPacketSendEnable;//초기값:TRUE

public:

	BOOL		m_bMoveChk;
	
	BYTE		m_byRenderType;
	BYTE		m_byOrder;
	BYTE		m_byGroupTab;
	BYTE		m_byStatus;	
	
	BYTE		m_byColor; //파티 강화 시스템 (정우창 2005.01.26) 드롭박스 텍스트 색깔 

	char		m_szInfo[2][TEXT_RENDER_BUFFER];
	RECT		m_rtPos;

	BYTE		m_byMsgCnt;
	
	BOOL		m_bPartyMove;
	BOOL		m_bGuildMove;
	BOOL		m_bMessengerMove;
	BOOL		m_bGuildWarMove;

	int			m_nPartyUserIndex;
	
	BYTE		m_byLayerIndex;
	BOOL		m_bInfo;
	DWORD		m_dwPrevTime;
	DWORD		m_dwCurTime;
	WORD		m_wDungeonID;

	char		m_szName[MAX_CHARACTER_NAME_LENGTH];
	
	BYTE		m_byCurPage;
	BYTE		m_byMaxPage;

	BYTE		m_byUserType;
	BYTE		m_byMessengerType;

	BOOL		m_bPartyBtn[6];
	BOOL		m_bGuildBtn[7];
	BOOL		m_bFriednBtn[4];
	BOOL		m_bGuildWarBtn;
	BOOL		m_bPartyMsg;
	BOOL		m_bGuildMsg;	

	BYTE		m_byGuildLevel;
	
	ListNode<GUILD_USER>*			m_pGuildUserPos;	
	ListNode<GUILD_OFFLINE_USER>*	m_pGuildOfflineUserPos;
	
	ListNode<MESSENGER_USER>*		m_pMessengerOnlinePos;	
	ListNode<MESSENGER_USER>*		m_pMessengerOfflinePos;	
	
	IDISpriteObject*				m_pSprStatus[4];




public:
	
private:
	static CGroupWnd* c_pThis;

public:
	static CGroupWnd*	GetInstance()		{ if(!c_pThis) c_pThis = new CGroupWnd; return c_pThis; }
	static void			DestroyInstance()	{ if(c_pThis) { delete c_pThis; c_pThis = NULL;} }


	// 공통으로 사용되는 것들은 재정의 한다 //
	BOOL	GetClick();	
	BOOL	Init();
		
	void	RenderText();	
	void	Remove();
	void	SetOrder();		
	void	SetActive(BOOL bActive = TRUE);
		
	int		CheckInterface();

	// 체크 관련 함수 //			
	void	OpenTab();
	void	SetStatus(BYTE byStatus);

	void	SetPartyUI();
	void	SetPosGroup();

	DWORD	GetBarResource(BYTE byIndex);
	DWORD	GetGageResource(BYTE byIndex);

	void	SetPrevNext(BYTE byType);
	
	void	RenderGuildInfo();
	void	RenderPartyInfo();
	void	RenderFriendInfo();	
	void	RenderGuildWarInfo();
	void	RenderGuildWarInfoNew();

	//sung-han 2005-01-29 라이벌 길드전
	void	RenderTextGuildWarRankList();
	void	RenderTextGuildWarList();
	void	SetGuildWarRenderStatus();
	void	ReSetGuildWarRenderStatus();
	void	TempSetRandomData();
	void	InitGuildWarMatching();

	CGroupWnd();
	~CGroupWnd();
};

#endif
//======================================================//
// End.													//
//======================================================//

