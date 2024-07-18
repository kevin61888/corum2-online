// PartyMatchingWnd.h: interface for the CPartyMatchingWnd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARTYMATCHINGWND_H__F3BBDF13_13E2_44BE_B87A_68FC7AD1C3CF__INCLUDED_)
#define AFX_PARTYMATCHINGWND_H__F3BBDF13_13E2_44BE_B87A_68FC7AD1C3CF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Menu.h"


#define SPR_OBJ_PARTYMATCHAING_WND			0
#define SPR_OBJ_PARTYMATCHAING_CLOSE1		1
#define SPR_OBJ_PARTYMATCHAING_CLOSE2		2
#define SPR_OBJ_PARTYMATCHAING_OK1			3
#define SPR_OBJ_PARTYMATCHAING_OK2			4
#define SPR_OBJ_PARTYMATCHAING_OK3			5
#define SPR_OBJ_PARTYMATCHAING_CANCEL1		6
#define SPR_OBJ_PARTYMATCHAING_CANCEL2		7
#define SPR_OBJ_PARTYMATCHAING_CANCEL3		8

enum eParty_Matching_Wnd_Kind
{
	PMWK_APPLY = 0,					//기존의 파티에 참여 
	PMWK_TAKE = 1,					//자신이 파장으로 
};

struct PARTY_MATCHING_DATA
{
	BYTE byKind;
	BYTE byClass;
	int nLevel;
	char szName[0xff];
};

class CPartyMatchingWnd : public CMenu  
{
public:
	RECT	m_rtPos;
	DWORD	m_dwUserIndex;
	BOOL	m_bBtn[2];
	char	m_szInfo[2][0xff];
	BYTE	m_byPartyMatchKind;
	BYTE	m_byPMatchResultTrue;            //매칭의뢰 결과   0:성공 
	BYTE	m_byPMatchResultFalse;           //매칭의뢰 결과   1:실패 

private:
	static CPartyMatchingWnd* c_pThis;

public:
	static CPartyMatchingWnd*	GetInstance()		{ if(!c_pThis) c_pThis = new CPartyMatchingWnd; return c_pThis; }
	static void		DestroyInstance()	{ if(c_pThis) { delete c_pThis; c_pThis = NULL;} }


	BOOL	Init();
			
	void	RenderText();	
	void	Remove();
	void	SetOrder();	
	void	SetActive(BOOL bActive = TRUE);	

	int		CheckInterface();

	void	SetPartyMatchingWndData(PARTY_MATCHING_DATA pMatchData);

public:
	CPartyMatchingWnd();
	virtual ~CPartyMatchingWnd();



};

#endif // !defined(AFX_PARTYMATCHINGWND_H__F3BBDF13_13E2_44BE_B87A_68FC7AD1C3CF__INCLUDED_)

/*
CPartyMatchingWnd* pPartMatchingWnd = CPartyMatchingWnd::GetInstance();
PARTY_MATCHING_DATA PartyMatchData;
ZeroMemory(PartyMatchData.szName, sizeof(PartyMatchData.szName));
PartyMatchData.byKind = PMWK_APPLY;
PartyMatchData.byClass = CLASS_TYPE_WARRIOR;
PartyMatchData.nLevel = 100;
lstrcpy(PartyMatchData.szName, "이름");
pPartMatchingWnd->SetPartyMatchingWndData(PartyMatchData);
pPartMatchingWnd->SetActive(TRUE);
*/