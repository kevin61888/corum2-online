#pragma once

#include "crProxyMenuToDialog.h"
#include "crStaticText.h"
#include "crButton.h"
#include "crListBox.h"
#include "./../CommonServer/WorldPacket.h"
#include "crImage.h"

/**
 * \ingroup CorumOnlineProject
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
 * \date 2005-02-02
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
class CGuildWarFinalSettingWnd : public crProxyMenuToDialog
{
public:
	/**
	 *
	 */
	virtual void	RenderText();

	//////////////////////////////////////////////////////////////////////////////////////////
	/**
	 *
	 * \return 
	 */
	virtual BOOL	Init();

	CGuildWarFinalSettingWnd();
	virtual ~CGuildWarFinalSettingWnd();
	//////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////////
	/**
	 *
	 * \param pPacket 
	 */
	void	ProcessPacket( GUILD_MATCHBEGIN_SETTING * pPacket );
	//////////////////////////////////////////////////////////////////////////////////////////

	void	SetActive(BOOL bActive );

	//////////////////////////////////////////////////////////////////////////////////////////
	/**
	 *
	 * \return 
	 */
	static CGuildWarFinalSettingWnd*	GetInstance()		{ if(!c_pThis) c_pThis = new CGuildWarFinalSettingWnd; return c_pThis; }
	/**
	 *
	 */
	static void					DestroyInstance()	{ if(c_pThis) { delete c_pThis; c_pThis = NULL;} }
	static CGuildWarFinalSettingWnd* c_pThis;
	//////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////////
	/**
	 *
	 */
//	virtual void	Remove();
	////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////////
	enum	ExplainText_Type
	{
		ETT_DEFAULT = 0,
        ETT_LEVELSORT,
		ETT_REFRESH,
		ETT_CALL,
		ETT_KICK,
		ETT_SELECT_KICKEE,
		ETT_READY,
		ETT_CANCEL,
		ETT_INVITATION,
		ETT_MAX
	};
	/**
	 *
	 */
	void	SetExplainText( ExplainText_Type eType );
	////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////////
	typedef	GUILD_MATCHBEGIN_MEMBER				MEMBERLIST_VALUE_TYPE;
	typedef list< MEMBERLIST_VALUE_TYPE >		MEMBERLIST;
	typedef	MEMBERLIST::const_iterator			MEMBERLIST_CONST_ITER;
	typedef	MEMBERLIST::iterator				MEMBERLIST_ITER;
	////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////////
	__inline	crListBox&	GetListBox()
	{
		return m_listGuildMember;
	}
	////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////////
    /**
     *
     */
    void	ListSortByLevel();
	/**
	 *
	 */
	void	Refresh();
	void	Call();
	void	Kick();
	void	ReadyToggle();
	void	ListClick();
	void	Cancel();
	void	CancelYes();
	void	CancelNo();
	void	Invitation();

	void	UpdateListCount();
	
	void	UpdateList( const crListBoxItemInterface& rhs );
	void	UpdateList_SelectedCheck( crListBoxItemInterface& rhs );
	////////////////////////////////////////////////////////////////////////////////////

	__inline	crStaticText&	GetPageCount()
	{
		return m_textListGuildMemberCurPage;
	}

	void	SetTime( DWORD dwBegin, DWORD dwEnd );
private:
	crDialog		m_DlgOkCancel;
	crStaticText	m_textOkCancel;
	crButton		m_btnOkCancel_Yes;
	crButton		m_btnOkCancel_No;

	// 메인창, 프록시에 의해서 연결된다.
	crDialog		m_DlgMain;
		
	crStaticText	m_textGuildName[2];		//	길드이름 
	crStaticText	m_textLadderPoint[2];	//	레더점수
	crStaticText	m_textAverageLevel[2];	//	평균레벨
	crStaticText	m_textMemberCount[2];	//	인원수

	crStaticText	m_textLadder;
	crStaticText	m_textAverage;
	crStaticText	m_textMember;
	crStaticText	m_textMark[4];			//	: 표시

	crStaticText	m_textExplain[2];		// 현재행동 설명 텍스트
	crStaticText	m_textNumber[20];		// 이름앞, 번호 표시, Align Right
	crStaticText	m_textClock;			// 남은 시간 표시.

	crButton		m_btnLevelSort;			// 레벨정렬 버튼
	crButton		m_btnRefresh;			// 갱신 버튼
	crButton		m_btnCall;				// 멤버소환 버튼
	crButton		m_btnInvitation;		// 초대 버튼
	crButton		m_btnKick;				// 퇴장 버튼
    crButton		m_btnCancel;			// 경기 취소 버튼
//	crButton		m_btnReady;				// 준비 버튼

	crButton		m_btnPrev;				// 리스트박스 이전버튼
	crButton		m_btnNext;				// 리스트박스 다음버튼

	crListBox		m_listGuildMember;	
	crImage			m_imgListGuildMemberPage;
	crStaticText	m_textListGuildMemberCurPage;

	////////////////////////////////////////////////////////////////////////////////////////////////
	DWORD			m_dwLadderGrade;							// 레더 등수
	DWORD			m_dwLadderPoint;							// 레더 점수
	DWORD			m_dwSelectedAndCalledMemberAverageLevel;	// 선택인원과 호출인원의 레벨 평균값.	
	DWORD			m_dwMemberAverageLevel;						// 대기인원의 레벨 평균값
	DWORD			m_dwSelectedAndCalledMemberCount;			// 현재 선택된 인원과 호출된 인원의 수	

	DWORD			m_dwEnemyLadderGrade;						// 레더 등수
	DWORD			m_dwEnemyLadderPoint;						// 레더 점수
	DWORD			m_dwEnemyWaitMemberAverageLevel;			// 상대길드 전체대기멤버 평균레벨
	DWORD			m_dwEnemyWaitMemberCount;					// 상대길드 전체대기멤버 수
	
	DWORD			m_dwMaxMemberCount;							// 길드전 가능 최대 인원	
	////////////////////////////////////////////////////////////////////////////////////////////////
	
	MEMBERLIST		m_MemberList;
	MEMBERLIST		m_MemberList_TempRep;
	MEMBERLIST		m_MemberList_TempRep_Selected;

	BOOL			m_bReadyFlag;								// 현재 레디된 상태인지 나타냄.
	BOOL			m_bKickFlag;								// 현재 Kick할 상태인지 나타냄.

	DWORD			m_dwStartingSec;
	DWORD			m_dwMin;
	DWORD			m_dwSec;
	DWORD			m_dwRemainBeginTime;
	DWORD			m_dwRemainEndTime;
	DWORD			m_dwRemainBeginTimeRecvTime;
	DWORD			m_dwRemainEndTimeRecvTime;
};