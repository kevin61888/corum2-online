#pragma once

#include "Define.h"
#include "ObjectDefine.h"

#pragma pack(push,1)

struct NETMARBLE_LOGIN
{
	char	szIP[ MAX_IP_LENGTH ];
	DWORD	dwPort;
	BYTE	bLoging;			
	BYTE	bIsTryCorrectLogin;								// 제대로 인자가 넘어 왔나..
	char	szID[ MAX_ID_LENGTH ];
	char	szCode[ 40 ];
	char	szMakePassword[ 40 ];							//최종적으로 조합된 Password
	char	szCPCookie[ 1024*2 ];							// CP 쿠키 ;; 우리 게임에선 이것만 필요

	NETMARBLE_LOGIN()	{ memset(this, 0, sizeof(NETMARBLE_LOGIN));	}
};

#pragma pack(pop)

struct SERVER_SET_INFO
{
	char	szSetName[ MAX_SERVER_SET_NAME_LENGTH ];
	char	szIp[ MAX_IP_LENGTH ];
};

struct WORLD_CONNECT_INFO
{
	DWORD			 dwTotalSetNum;		
	DWORD			 dwCurMouseOverSet;	//현재 마우스 Over 된 셋
	SERVER_SET_INFO* pServerInfo;
};

struct CHR_SELECT_INFO
{
	RS_CharacterLogInfo	ReceivedChrInfo;	//월드서버로 부터 받은 모든 캐릭터 선택 정보 구조체
	GXOBJECT_HANDLE		hHandle;			//Client쪽에 표시할 Model Handle
	GXOBJECT_HANDLE		hHeadHandle;		//Client쪽에 표시할 Model head Handle		
	GXOBJECT_HANDLE		hGroundHandle;		//캐릭터 밑의 Ground 기둥 핸들
	GXOBJECT_HANDLE		hSpotLightHandle;	//캐릭터 선택 되었을때의 SpotLight 핸들 
	GXOBJECT_HANDLE		hHelmetHandle;		// 캐릭터 헬멧 핸드.
	GXOBJECT_HANDLE		hRightHand;			//0228
	GXOBJECT_HANDLE		hLeftHand;			//0228
	LPObjectDesc		pDesc;				//Model Desc
	LPObjectDesc		pDescGround;		//Ground Desc
	LPObjectDesc		pDescSpotLight;		//SpotLight Desc
	BYTE				bByItemType;		//장착하고 있는 아이템 종류 
	
	LPObjectDesc		pLItemDesc;
	LPObjectDesc		pRItemDesc;
};

typedef struct GlobalVariableLogin
{
	BYTE					bIsLoginInputMode;	
	BYTE					bIntroFlag;										//0이면 ServerSelect 모드1이면 동영상 상영중, 2이면 스프라이트 알파증가중, 
	BYTE					bConnectServerIndex;							//로그인할때 어떤 서버로 Connect할것인지를 나타내는 ServerSetInfo.pServerInfo[] 의 인덱스 번호 

	WORLD_CONNECT_INFO		ServerSetInfo;		
	CHR_SELECT_INFO			ChrSelectInfo[ MAX_CHAR_SELECT_NUM ];			//캐릭터 고를때 표시되는 정보들
	DWORD					dwCurTotalWorldUser;

	DWORD					dwCurCharIndex;
	GXOBJECT_HANDLE			EnumCharForCreate[ MAX_KIND_OF_CLASS ];			//캐릭터 Create 할때 선택할 모든 몸둥이 모델 핸들 (1번 인덱스 부터 사용하자)
	GXOBJECT_HANDLE			EnumCharHeadForCreate[ 2 ][ MAX_KIND_OF_HEAD ];	//캐릭터 Create 할때 선택할 모든 머리핸들 남자용 		[성별][머리번호]
	BYTE					bCurClassIndexForCreate;	
	DWORD					dwCurHeadIndexforCreate;						//반쪼개서 상위는 성별, 하위는 머리번호 
	DWORD					dwFlag;
	BYTE					bIsLoging;										//현재 로그인 결과를 기다리고 있음. 
	
	OBJECT_HANDLE_DESC*		pCharacterSelectBackGround;
	BOOL					LoadConnectionInfo();
	void					ReleaseConnectionInfo();

	DWORD					GetTotalSetNumber();
	DWORD					GetSelectedSet();
	SERVER_SET_INFO*		GetSelectedSetInfo();
	SERVER_SET_INFO*		GetSetInfo(DWORD dwIndex);
	void					SetCurSet(DWORD dwIndex);	
	
}GLOBAL_VARIABLE_LOGIN, *LPGlobalVariable_Login;