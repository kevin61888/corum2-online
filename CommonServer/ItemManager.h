#pragma once


#include "CommonServer.h"
#include "CommonHeader.h"
#include "CommonGlobalDefine.h"


#pragma pack(push,1)
struct DECLSPECIFIER ITEM_OPTION_SERVER
{
	DWORD	dwID;
	char	szItemName[50];
	BYTE	pbOption[MAX_ITEM_KIND1_CODE];
};


struct DECLSPECIFIER ITEM_EVENT
{
	DWORD	dwid;
	WORD	witem_id;
	DWORD	dwitem_quantity;
	DWORD	dwmax_count;
	DWORD	dwnow_count;
};


typedef struct DECLSPECIFIER SET_ITEM_INFO
{
	WORD				wSetItemID;
	char				szItemName_Kor[50];
	char				szItemName_Eng[50];
	BYTE				bFullSet;
	WORD				pwSetID[7];	
	SET_ITEM_OPTION_2	pBonusOption[5][2];		// 총 5개까지 각각 2개씩 붙을수 있다.
	SET_ITEM_OPTION_2	pFullOption[8];			// Set를 보두 장착했을시에 얻을수 있는 풀옵 보너스.		
} *LPSET_ITEM_INFO;


typedef struct DECLSPECIFIER ITEM_MAKING_INFO
{
	WORD				wID;
	WORD				wItem_id1;
	WORD				wItem_id2;
	WORD				wComplete;
} *LPITEM_MAKING_INFO;


struct DECLSPECIFIER ITEM_UPGRADE
{
	DWORD	dwID;
	BYTE	bUpgradeLevel;
	BYTE	bCode;
	char	szUpgradeDesc[50];
	DWORD	dwMin;
	DWORD	dwMax;
	WORD	wLevel;
	WORD	wMinLevel;
	WORD	wMinGrade;
	WORD	wAddLevel;
	WORD	pwItemMax[ITEM_NUM_PER_OPTION];
};


typedef struct DECLSPECIFIER ITEM_COUNT
{
	WORD	wBaseItemID;		
	BYTE	bServerSet;
	DWORD	dwDungeonID;
} *LPITEM_COUNT;

typedef struct CHAT_LOG
{
	BYTE				bMessageType;		// enum CHAT_TYPE 참조 
	BYTE				bUserType;			// 일반, GM
	char				szSender[MAX_CHARACTER_NAME_LENGTH]; // 캐릭터 네임 
	char				szRecver[MAX_CHARACTER_NAME_LENGTH];
	WORD				wDungeonID;			// 해당 던전, 혹은 월드 ID
	char				szContents[0xff];	// 챗팅 내용
} *LPCHAT_LOG;

typedef struct DECLSPECIFIER ITEM_LOG
{
	ITEM_SERIAL			Serial;
	BYTE				bLogCode;
	BYTE				bServerSet;
	DWORD				dwDungeonID;
	CItem				Item;
	char				szOwnerName[20];		
	char				szSellerName[20];		

	ITEM_LOG()			{ memset(this,NULL,sizeof(ITEM_LOG)); }
} *LPITEM_LOG;


typedef struct DECLSPECIFIER KARZ_LOG
{
	ITEM_SERIAL			Serial;
	BYTE				bLogCode;
	BYTE				bServerSet;
	DWORD				dwDungeonID;
	CItem				Item;
	DWORD				dwMoney;
	char				szOwnerName[20];		
	char				szSellerName[20];		

	KARZ_LOG()			{ memset(this,NULL,sizeof(KARZ_LOG)); }
} *LPKARZ_LOG;


typedef struct DECLSPECIFIER SPEEDHACK_LOG
{
	char				szName[20];
	BYTE				bServerSet;
	DWORD				dwDungeonID;

	SPEEDHACK_LOG()		{ memset(this,NULL,sizeof(SPEEDHACK_LOG)); }
} *LPSPEEDHACK_LOG;


typedef struct DECLSPECIFIER INVALID_LOG_ITEM
{
	DWORD dwBaseItemID;
}* LPINVALID_LOG_ITEM;


struct DECLSPECIFIER USER_SET_ITEM_INFO
{
	DWORD	dwSetID;		// SetID.
	WORD	wCnt;			// 장착하고 있는 세트 아이템 개수.
	WORD	pwSetItemID[8];	// 장착하고 있는 세트 아이템 아이디.
	BOOL	isFullSet;
};


typedef struct DECLSPECIFIER ITEM_STORENATIVE
{
	DWORD	dwID;
	WORD	wItemId;
	BYTE	byType;	
	WORD	wMapId;
	
} *LP_ITEM_STORENATIVE;
#pragma pack(pop)


struct DECLSPECIFIER ITEM_CREATE
{
	void*				DelPos;
	VECTOR2				v2ItemPos;
	DWORD				dwCreateTick;		// 아이템이 생성될 당시의 틱.
	DWORD				dwSectionNum;
	DWORD				dwOwnerIndex;
	DWORD				dwPartyIndex;		// 파티 드롭 아이템 : 최덕석 2005.1.26
	CItem				Item;
	BYTE				bTemp[3];
	BYTE				bCreateByType;
	DWORD				dwOwnerLimitSec;    // 아이템소유권 유지되는 시간. : 재원 20050330
};

struct DECLSPECIFIER ITEM_SERVER
{
	VECTOR2				v2ItemPos;			// 아이템의 위치.
	DWORD				dwCurFramePerSec;	// 아이템의 사라지는 시간.
	DWORD				dwOwnerFramePerSec;	// 아이템소유권 유지한 시간.
	DWORD				dwOwnerLimitSec;    // 아이템소유권 유지되는 시간. : 재원 20050330
	DWORD				dwOwnerIndex;		// 주인의 인덱스번호.
	DWORD				dwPartyIndex;		// 파티 드롭 아이템 : 최덕석 2005.1.26
	DWORD				dwSectionNum;		// 현재 아이템이 있는 섹션.
	CItem				Item;				
};


DWORD		DECLSPECIFIER GetItemKind(WORD wItemID);
ITEM_TYPE	DECLSPECIFIER GetItemType(WORD wItemID);
void		DECLSPECIFIER SetCommonServerBaseItemHash(void* pBaseItemHash);









