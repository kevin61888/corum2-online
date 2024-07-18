#pragma once


#include "Menu.h"


#define SIEGEINFOWND_TYPE_SIEGE				0
#define SIEGEINFOWND_TYPE_EVENT_TRAINING	1
#define SIEGEINFOWND_TYPE_EVENT_SURVIVAL	2


#define SPR_OBJ_DUNGEONSIEGEINFOWND1		0
#define SPR_OBJ_DUNGEONSIEGEINFOWND2		1
#define SPR_OBJ_DUNGEONSIEGEINFOWND3		2


class CDungeonSiegeInfoWnd : public CMenu  
{
public:

	char	m_szInfo[TEXT_RENDER_BUFFER];
	RECT	m_rtPos;			

public:	
private:
	static CDungeonSiegeInfoWnd* c_pThis;

public:
	static CDungeonSiegeInfoWnd*	GetInstance()		{ if(!c_pThis) c_pThis = new CDungeonSiegeInfoWnd; return c_pThis; }
	static void						DestroyInstance()	{ if(c_pThis) { delete c_pThis; c_pThis = NULL;} }


	// 공통으로 사용되는 것들은 재정의 한다 //
	BOOL	Init();
		
	void	RenderText();
	void	Remove();
	void	SetOrder();
	void	SetActive(BOOL bActive = TRUE);
	
	int		CheckInterface();

	// 체크 관련 함수 //	

	INT		m_iType;
	INT		m_iTotalMonsterCount;
	INT		m_iCurrenMonsterCount;

	INT		m_iHour;
	INT		m_iMin;
	INT		m_iSec;

	INT		m_iDungeonTotalUserNumber;
	INT		m_iLayerTotalUserNumber;
	
	INT		m_iNeedKillMonsterNumber;
	INT		m_iNeedUserNumber;

	INT		m_iCurrentKillMonsterCount;


	DWORD	m_dwTotalKillMonSterLayerCount;
	DWORD	m_dwCurrentKillMonsterLayerCount;
	
	DWORD	m_dwCurrentMonterLayerCount;
	DWORD	m_dwPartyCurrentKillMonsterLayerCount;
	DWORD	m_dwKillMonsterLayerCount;
	
	BYTE	m_bPartySameDungeonLayerUserCount;
	
public:
	
	CDungeonSiegeInfoWnd();
	~CDungeonSiegeInfoWnd();

private:
	
	void	OnMove();
	
};