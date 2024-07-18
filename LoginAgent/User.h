#ifndef __USER_H__
#define __USER_H__

#pragma once

#include "../CommonServer/CommonHeader.h"
#include "Linkedlist.h"

#include "ChinaBillingPacket.h"

#ifdef __PACKET_ENCRYPTION
#	include "PacketEncrypt.H"
//#	include "DFXForm.h" // 요넘 때매 컴파일이 안되넹 ;; 김영대 2004-12-30
#endif


#define MAX_INV_GUARDIAN			5				
#define MAX_BELT					8
#define MAX_INV_LARGE				21
#define MAX_INV_SMALL				42		
#define	MAX_BANK_SMALL				70
#define	MAX_BANK_LARGE				63

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


#define EVENT_NUM_RECOM_ADD_ITEM	1	//2003년 10월 28일부터 시작되는 이벤트 추천받아 생성된 캐릭터에는 기본 아이템 몇개를 더 넣어줌 
#define EVENT_NUM_PC_ROOM_EXP_TWICE	2	//2003년 12월 31일부터 시작되는 PC방 유저 경험치 2배 이벤트 

#define SERVER_POS_AGENT			0

#pragma pack(push,1)
struct ITEM_MALL
{
	CItem	sBankItemLarge[MAX_BANK_LARGE];
	CItem	sBankItemSmall[MAX_BANK_SMALL];
};


struct RS_CharacterLogInfo_Without_Equip
{
	DWORD	dwCharacterIndex;								// 캐릭터 Index
	char	szName[ MAX_CHARACTER_NAME_LENGTH ];			// 캐릭터 이름 
	WORD	wChrNum;										// 캐릭터 번호 (1 ~ 4)
	WORD	wHead;
	WORD	wClass;											// 클래스 
	WORD	wGrade;											// 그레이드 100이면 GM
	DWORD	dwLevel;										// 레벨 
	DWORD	dwExp;											// 경험치 
	
	DWORD	dwEgo;											// EGO
	DWORD	dwStr;											// STR
	DWORD	dwVit;											// VIT
	DWORD	dwDex;											// DEX.
	DWORD	dwInt;											// INT.

	DWORD	dwHonor;										// 명성
	DWORD	dwLuck;											// 행운
	DWORD	dwRecentWorldmapID;								//최근 종료한 WorldMap ID
};

struct RS_CharacterLogInfo_With_Equip : public RS_CharacterLogInfo_Without_Equip
{
	CItem	Equip[ MAX_EQUIP ];
	BYTE	byCurrentHand;	
	DWORD	dwGuildId;
	BYTE	byRank;	
	BYTE	byInvItemCheck;	
	DBTIMESTAMP	sCreateTime;
};

struct ITEMMALL_BOX
{
	DWORD dwIdx;
	DWORD dwItemID;	//Item ID
	DWORD dwCount;	//Item 갯수
};

struct ITEMMALL_SUPPLY
{
	DWORD	dwItemMallIndex;
};
#pragma pack(pop)


class CUser
{
public:
	
	DWORD					m_dwPropID;					//Chr_Log_Info 테이블에 있는 유저 고유값 
	DWORD					m_dwConnectionIndex;		//Server ConnectionIndex;
	DWORD					m_dwStatus;
	DWORD					m_dwCurWorldServerPos;		//유저가 있는 현재 서버의 위치(서버 ID)
	DWORD					m_dwSerialCode;				//월드서버 접속을 위한 SerialCode
	DWORD					m_dwChar_Index;				//유저가 선택한 Character의 고유값 			
	BYTE					m_bSelectedChrIndex;		//
	BYTE					m_bClass;
	BYTE					m_bTryCreateCharSlot;		//최근 캐릭터 생성을 시도했던 슬롯 번호 
	char					m_szLoginID[ MAX_ID_LENGTH ];
	char					m_szLoginPassword[ MAX_PASSWORD_LENGTH ];
	DWORD					m_dwKickUserPropID;			//로그인 중첩시 Kick할 대상 User의 PropID
	BYTE					m_bEventNum;				//이벤트용 번호 
	BYTE					m_bPCRoom;					//PC방 접속 유저 
	void*					m_pListPos;					
	DWORD					m_dwBillingStartTick;		//하나의 과금방식이 시작된 시점의 Tick
	DWORD					m_dwLastBillingCheckTick;	//마지막으로 Billing Check한 TickCount
	BYTE					m_bBillingType;				//결제 타입 
	DWORD					m_dwRemainTime;				//종량제 남은시간 
	DBTIMESTAMP				m_BillingEday;				//결제 끝나는시각 
	char					m_szIp[16];					//유저의 IP Address
	BYTE					m_bItemMallCount;			//아이템몰에서 도착한 선물의 갯수 
	ITEMMALL_BOX*			m_pItemMallBox;				//아이템몰 박스 포인터
	ITEMMALL_SUPPLY*		m_pItemMallSupply;			//아이템몰 총갯수중 지급된 아이템 리스트	//kjk(41030)
	WORD					m_wItemMallSupplyCount;		//아이템몰 총 지급된 갯수					//kjk(41030)

	SCHINA_BILLING_USER_DATA	m_sChinaBillingData;

	//재원 050323 
	DBTIMESTAMP				m_ID_RegDate; //계정생성일
	DWORD					m_dwCharCount ;


#ifdef __PACKET_ENCRYPTION
private:
	DFXform					m_xform;
#endif

public:
	BYTE					m_byItemMallType;

	BILLING_INFO			m_BillingInfo;				//결제 정보 

	DWORD	GetConnectionIndex()				{ return m_dwConnectionIndex;		}
	void	SetConnectionIndex(DWORD dwIndex)	{ m_dwConnectionIndex = dwIndex;	}
	

	DWORD	GetPropID()							{ return m_dwPropID;				}
	void	SetPropID(DWORD dwID)				{ m_dwPropID = dwID;				}
	
	DWORD	GetIP()								{ return atoi(m_szIp);				}
	LPCSTR	GetIPString()						{ return m_szIp;					}

	LPCSTR	GetUserID()							{ return m_szLoginID;				}
	void	SetUserID(LPCSTR szID)				{ lstrcpy(m_szLoginID, szID);		}

#ifdef __PACKET_ENCRYPTION
	void CUser::SetEncryptKey(LPBYTE pKey)			
	{ 
		m_xform.InitializeBlowfish(pKey, ENCRYPT_KEY_LEN);	
	}

	void CUser::SetBaseEncryptKey()					
	{ 
		m_xform.InitializeBlowfishWithBaseKey();	
	}

	void CUser::Encode(LPBYTE pSrc, WORD wSrcSize, LPBYTE pDes, LPWORD pwDesSize)	
	{
		m_xform.Encode(pSrc, wSrcSize, pDes, pwDesSize);	
	}

	void CUser::Decode(LPBYTE pSrc, WORD wSrcSize, LPBYTE pDes, LPWORD pwDesSize)
	{
		m_xform.Decode(pSrc, wSrcSize, pDes, pwDesSize);	
	}
#endif

	RS_CharacterLogInfo_With_Equip	m_EnumChar[ 4 ];
	RS_CharacterLogInfo_With_Equip*	GetSelectedCharacter()	{	return &m_EnumChar[ m_bSelectedChrIndex ];	}

	CUser*	m_pNextData;
	CUser*	m_pPrevData;

	bool	m_bIsMonitor;				// 모니터링 툴인가?

};

#endif 