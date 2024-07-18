// GuildWarInvite.h: interface for the CGuildWarInvite class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GUILDWARINVITE_H__4CB513B5_46BD_4601_8F82_AEFC4F63989A__INCLUDED_)
#define AFX_GUILDWARINVITE_H__4CB513B5_46BD_4601_8F82_AEFC4F63989A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Menu.h"


#define SPR_OBJ_GUILDWAR_INVITE_WND			0
#define SPR_OBJ_GUILDWAR_INVITE_CLOSE1		1
#define SPR_OBJ_GUILDWAR_INVITE_CLOSE2		2
#define SPR_OBJ_GUILDWAR_INVITE_OK1			3
#define SPR_OBJ_GUILDWAR_INVITE_OK2			4
#define SPR_OBJ_GUILDWAR_INVITE_OK3			5
#define SPR_OBJ_GUILDWAR_INVITE_CANCEL1		6
#define SPR_OBJ_GUILDWAR_INVITE_CANCEL2		7
#define SPR_OBJ_GUILDWAR_INVITE_CANCEL3		8

class CGuildWarInvite : public CMenu  
{
public:
	BOOL	m_bBtn[2];

private:
	static CGuildWarInvite* c_pThis;

public:
	static CGuildWarInvite*	GetInstance()		{ if(!c_pThis) c_pThis = new CGuildWarInvite; return c_pThis; }
	static void		DestroyInstance()	{ if(c_pThis) { delete c_pThis; c_pThis = NULL;} }


	BOOL	Init();
			
	void	RenderText();	
	void	Remove();
	void	SetOrder();	
	void	SetActive(BOOL bActive = TRUE);	

	int		CheckInterface();

public:
	CGuildWarInvite();
	virtual ~CGuildWarInvite();

};

#endif // !defined(AFX_GUILDWARINVITE_H__4CB513B5_46BD_4601_8F82_AEFC4F63989A__INCLUDED_)
