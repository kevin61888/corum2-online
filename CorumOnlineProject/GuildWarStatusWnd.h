#pragma once

#include "crProxyMenuToDialog.h"
#include "crDialog.h"
#include "crImage.h"
#include "crImageList.h"

struct GUILD_MATCH_STATUS_INFO
{
	char	szName[20];
	DWORD	dwPoint;
};
struct GUILD_MATCH_STATUS_PREPARE
{
	DWORD m_dwTime;
    
	DWORD	dwSize;
	GUILD_MATCH_STATUS_INFO	Info;
};

struct GUILD_MATCH_STATUS_BEGIN
{
	DWORD m_dwTime;
};

struct GUILD_MATCH_STATUS_UPDATE
{
	DWORD	dwSize;
	GUILD_MATCH_STATUS_INFO	Info;
};

typedef struct tagGUILD_STATUS_ANG_IMAGE
{
	xstring			m_strGuildName;
	DWORD			m_dwGuildID;

	DWORD			m_dwImageIndex;
    vector< DWORD >	m_vecGuildUserID;    
} GUILD_STATUS_ANG_IMAGE;

/**
 * \ingroup 기타 파일
 *
 *
 * \par requirements
 * win98 or later\n
 * win2k or later\n
 * MFC\n
 *
 * \version 1.0
 * first version
 *
 * \date 2005-02-06
 *
 * \author 전병렬
 *
 * \par license
 * This code is absolutely free to use and modify. The code is provided "as is" with
 * no expressed or implied warranty. The author accepts no liability if it causes
 * any damage to your computer, causes your pet to fall ill, increases baldness
 * or makes your car start emitting strange noises when you start it up.
 * This code has no bugs, just undocumented features!
 * 
 * \todo 
 *
 * \bug 
 *
 */
class CGuildWarStatusWnd : public crProxyMenuToDialog
{
public:
	/**
	 *
	 * \return 
	 */
	virtual BOOL	Init();

	//////////////////////////////////////////////////////////////////////////////////////////
	/**
	*
	* \return 
	*/
	static CGuildWarStatusWnd*	GetInstance()		{ if(!c_pThis) c_pThis = new CGuildWarStatusWnd; return c_pThis; }
	/**
	*
	*/
	static void					DestroyInstance()	{ if(c_pThis) { delete c_pThis; c_pThis = NULL;} }
	static CGuildWarStatusWnd* c_pThis;
	//////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////////
	/**
	*
	* \param pPacket 
	*/
	void	ProcessPacket( GUILD_MATCH_STATUS* pPacket );
	void	ProcessPacket( GUILD_MATCH_STATUS_RESULT* pPacket );
	//////////////////////////////////////////////////////////////////////////////////////////

	/**
	*
	*/
	virtual void	RenderText();

	CGuildWarStatusWnd();
	virtual ~CGuildWarStatusWnd();

	/**
	 *
	 * \param bActive 
	 */
	virtual void	SetActive(BOOL bActive = TRUE);

	void	Draw();
	void	Win();
	void	Lose();
	void	WinLose_De()
	{
		m_imgTimeTenSecond.SetVisible( FALSE );
	}

	__inline	BOOL	IsPrepareTime()
	{
		return m_dwRemainBeginTime > 0;
	}

	__inline	VOID	SetMark_Start()
	{
		m_imgTimeTenSecond.SetVisible( TRUE );
		m_imgTimeTenSecond.SetCurIndex( 10 );		
		m_dwPVP_Start = ::timeGetTime();
	}
	__inline	VOID	SetMark_Ready()
	{		
		m_imgTimeTenSecond.SetVisible( FALSE );
		m_dwPVP_Start = 0xffffffff;
	}

private:
	void	UpdateBeginTime();
	void	UpdateEndTime();
	crImage			m_imgGuild[MAX_MATCH_TEAM];
	crImage			m_imgGuilMark[MAX_MATCH_TEAM];
	crStaticText	m_textGuildName[MAX_MATCH_TEAM];
	crStaticText	m_textGuildPoint[MAX_MATCH_TEAM];

//	crStaticText	m_textClock;
//	crStaticText	m_textClockEnd;

	crImage			m_imgTimeBack;
	crImage			m_imgTimeStamp;
	crImageList		m_imgTimeMinute1;
	crImageList		m_imgTimeMinute2;
	crImageList		m_imgTimeSecond1;
	crImageList		m_imgTimeSecond2;

	crImageList		m_imgTimeTenSecond;

	IDISpriteObject*	m_pSpr[4];	// 길드전

	DWORD			m_dwRemainBeginTime;
	DWORD			m_dwRemainEndTime;
	DWORD			m_dwRemainBeginTimeRecvTime;
	DWORD			m_dwRemainEndTimeRecvTime;

	list< GUILD_STATUS_ANG_IMAGE >	m_listGuidStatus;

	POINT			m_ptList[4];

	DWORD			m_dwPVP_Start;	// PVP 시작 시간
};