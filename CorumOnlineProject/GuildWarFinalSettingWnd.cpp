#include ".\guildwarfinalsettingwnd.h"
#include "GameDefine.h"
#include "GameControl.h"
#include "NetworkClient.h"
#include "MessagePool.h"
#include "./Message.h"
#include "UserInterface.h"
#include "DungeonProcess_Sound.h"
#include "Interface.h"
#include "User.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
extern	CNetworkClient*				g_pNet;
extern	I4DyuchiGXRenderer*			g_pRenderer;
extern	CMessagePool				g_Message;
const	DWORD						c_dwLineStep = 14;
//////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////
// 선택된 유저를 호출한다.
struct	PredCallSelectedUser : public unary_function< crListBoxItemInterface, void	>
{
	void	operator() ( crListBoxItemInterface& rhs )
	{
		if( rhs.m_bSelected )
		{
			GUILD_MATCHBEGIN_SETTING	Packet;
			Packet.eType = GMS_TYPE_REQUEST_USER_CALL;
			strcpy( Packet.Body.m_UserCall.szName, reinterpret_cast< CGuildWarFinalSettingWnd::MEMBERLIST_VALUE_TYPE* >(rhs.m_pData)->szName );
			
			CGuildWarFinalSettingWnd::GetInstance()->ProcessPacket( &Packet );
		}
	}
};

struct	PredKickSelectedUser : public unary_function< crListBoxItemInterface, void	>
{
	void	operator() ( crListBoxItemInterface& rhs )
	{
		if( rhs.m_bSelected )
		{
			GUILD_MATCHBEGIN_SETTING	Packet;
			Packet.eType = GMS_TYPE_REQUEST_USER_KICK;
			strcpy( Packet.Body.m_UserCall.szName, reinterpret_cast< CGuildWarFinalSettingWnd::MEMBERLIST_VALUE_TYPE* >(rhs.m_pData)->szName );
			
			CGuildWarFinalSettingWnd::GetInstance()->ProcessPacket( &Packet );
		}
	}
};

struct	PredSelectedCheck : public unary_function< crListBoxItemInterface, void	>
{
	void	operator() ( crListBoxItemInterface& rhs )
	{
		if( rhs.m_bSelected )
			CGuildWarFinalSettingWnd::GetInstance()->UpdateList_SelectedCheck( rhs );
	}
};

struct	PredUpdateCalledAndSelected : public unary_function< crListBoxItemInterface, void	>
{
	void	operator() ( crListBoxItemInterface& rhs )
	{
		if( rhs.m_bSelected )
			CGuildWarFinalSettingWnd::GetInstance()->UpdateList( rhs );
	}
};

struct	PredTransformCalledToLock : public unary_function< crListBoxItemInterface, void	>
{
	void	operator() ( crListBoxItemInterface& rhs )
	{
		if( reinterpret_cast< CGuildWarFinalSettingWnd::MEMBERLIST_VALUE_TYPE* >(rhs.m_pData)->byCalled )
		{
			rhs.m_bLock = TRUE;
			rhs.m_bSelected = FALSE;
		}
		else
		{
			rhs.m_bLock = FALSE;
		}
	}
};

struct	PredSortByLevel : public binary_function< crListBoxItemInterface, crListBoxItemInterface, bool >
{
    bool	operator() ( const crListBoxItemInterface& lhs, const crListBoxItemInterface& rhs ) const
	{
		return reinterpret_cast< CGuildWarFinalSettingWnd::MEMBERLIST_VALUE_TYPE* >(lhs.m_pData)->wLevel >
			reinterpret_cast< CGuildWarFinalSettingWnd::MEMBERLIST_VALUE_TYPE* >(rhs.m_pData)->wLevel;
	}
};

struct	PredCalledCount : public unary_function< crListBoxItemInterface, bool >
{
	bool	operator() ( const crListBoxItemInterface& rhs ) const
	{
		return (rhs.m_bLock ? true : false);
	}
};

struct	PredSelectedAndCalledCount : public unary_function< crListBoxItemInterface, bool >
{
	bool	operator() ( const crListBoxItemInterface& rhs ) const
	{
		return (rhs.m_bSelected || rhs.m_bLock ? true : false);
	}
};

VOID	FnLevelSort( void* pData )
{
	CGuildWarFinalSettingWnd*	pWnd = CGuildWarFinalSettingWnd::GetInstance();
	pWnd->ListSortByLevel();	
}

VOID	FnRefresh( void* pData )
{
	CGuildWarFinalSettingWnd*	pWnd = CGuildWarFinalSettingWnd::GetInstance();
	pWnd->Refresh();	
}

VOID	FnOverLevelSort( void* pData )
{
	CGuildWarFinalSettingWnd*	pWnd = CGuildWarFinalSettingWnd::GetInstance();
	pWnd->SetExplainText( CGuildWarFinalSettingWnd::ETT_LEVELSORT );
}

VOID	FnOverRefresh( void* pData )
{
	CGuildWarFinalSettingWnd*	pWnd = CGuildWarFinalSettingWnd::GetInstance();
	pWnd->SetExplainText( CGuildWarFinalSettingWnd::ETT_REFRESH );
}

VOID	FnOverInvitaion( void* pData )
{
	CGuildWarFinalSettingWnd*	pWnd = CGuildWarFinalSettingWnd::GetInstance();
	pWnd->SetExplainText( CGuildWarFinalSettingWnd::ETT_INVITATION );
}

VOID	FnKick( void* pData )
{
	CGuildWarFinalSettingWnd*	pWnd = CGuildWarFinalSettingWnd::GetInstance();
	pWnd->Kick();
}

VOID	FnOverKick( void* pData )
{
	CGuildWarFinalSettingWnd*	pWnd = CGuildWarFinalSettingWnd::GetInstance();
	pWnd->SetExplainText( CGuildWarFinalSettingWnd::ETT_KICK );
}

VOID	FnOverReady( void* pData )
{
	CGuildWarFinalSettingWnd*	pWnd = CGuildWarFinalSettingWnd::GetInstance();
	pWnd->SetExplainText( CGuildWarFinalSettingWnd::ETT_READY );
}

VOID	FnOverCancel( void* pData )
{
	CGuildWarFinalSettingWnd*	pWnd = CGuildWarFinalSettingWnd::GetInstance();
	pWnd->SetExplainText( CGuildWarFinalSettingWnd::ETT_CANCEL );	

}

VOID	FnCall( void* /*pData*/ )
{
	CGuildWarFinalSettingWnd*	pWnd = CGuildWarFinalSettingWnd::GetInstance();
	pWnd->Call();
}

VOID	FnCancelYes( void* /*pData*/ )
{
	CGuildWarFinalSettingWnd*	pWnd = CGuildWarFinalSettingWnd::GetInstance();
	pWnd->CancelYes();
}

VOID	FnCancelNo( void* /*pData*/ )
{
	CGuildWarFinalSettingWnd*	pWnd = CGuildWarFinalSettingWnd::GetInstance();
	pWnd->CancelNo();
}

VOID	FnReady( void* pData )
{
	CGuildWarFinalSettingWnd*	pWnd = CGuildWarFinalSettingWnd::GetInstance();
	pWnd->ReadyToggle();	
}

VOID	FnInvitation( void* pData )
{
	CGuildWarFinalSettingWnd*	pWnd = CGuildWarFinalSettingWnd::GetInstance();
	pWnd->Invitation();	
}

VOID	FnOverInvitation( void* pData )
{
	CGuildWarFinalSettingWnd*	pWnd = CGuildWarFinalSettingWnd::GetInstance();
	pWnd->SetExplainText( CGuildWarFinalSettingWnd::ETT_INVITATION );
}

VOID	FnCancel( void* pData )
{
	CGuildWarFinalSettingWnd*	pWnd = CGuildWarFinalSettingWnd::GetInstance();
	pWnd->Cancel();
}

VOID	FnOverCall( void* pData )
{
	CGuildWarFinalSettingWnd*	pWnd = CGuildWarFinalSettingWnd::GetInstance();
	pWnd->SetExplainText( CGuildWarFinalSettingWnd::ETT_CALL );
}

VOID	FnListPrev( void* pData )
{
	CGuildWarFinalSettingWnd*	pWnd = CGuildWarFinalSettingWnd::GetInstance();
	pWnd->GetListBox().PrevPage();
	pWnd->GetPageCount().Format( "%u/%u", pWnd->GetListBox().GetPage(), pWnd->GetListBox().GetPageCount() );
}

VOID	FnListNext( void* pData )
{
	CGuildWarFinalSettingWnd*	pWnd = CGuildWarFinalSettingWnd::GetInstance();
	pWnd->GetListBox().NextPage();
	pWnd->GetPageCount().Format( "%u/%u", pWnd->GetListBox().GetPage(), pWnd->GetListBox().GetPageCount() );
}

VOID	FnUpdateSelectedItemCount( void* pData )
{
	CGuildWarFinalSettingWnd*	pWnd = CGuildWarFinalSettingWnd::GetInstance();
	pWnd->ListClick();	
}
//////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////
CGuildWarFinalSettingWnd* CGuildWarFinalSettingWnd::c_pThis = NULL;

CGuildWarFinalSettingWnd::CGuildWarFinalSettingWnd()
: m_dwLadderGrade(0), 
m_dwLadderPoint(0), 
m_dwSelectedAndCalledMemberAverageLevel(0),
m_dwMemberAverageLevel(0), 
m_dwSelectedAndCalledMemberCount(0), 
m_dwEnemyLadderGrade(0),
m_dwEnemyLadderPoint(0), 
m_dwEnemyWaitMemberAverageLevel(0), 
m_dwEnemyWaitMemberCount(0),
m_dwMaxMemberCount(0)
{
}

CGuildWarFinalSettingWnd::~CGuildWarFinalSettingWnd()
{
}
//////////////////////////////////////////////////////////////////////////////////////////////////

BOOL	CGuildWarFinalSettingWnd::Init()
{
	if( GetProxyInit() == FALSE )
		SetProxyInit();
	else
	{
		m_bReadyFlag = FALSE;
		m_bKickFlag = FALSE;
		return crProxyMenuToDialog::Init();
	}

	m_pDlg = &m_DlgMain;

	crBaseMenu_Status	SStatus;

	SStatus.m_bDynamicAllocated = FALSE;

	m_DlgMain.Initialize( 20, 100, 256, 432, g_Message[ETC_MESSAGE1318].szMessage, 0x00000000, SStatus );	

	m_DlgMain.PushChild( &m_DlgOkCancel );

	m_DlgOkCancel.Initialize( 300, 100, 256, 93, _T("Message"), 0x00000000, SStatus );	
	m_DlgOkCancel.SetVisible( FALSE );

	m_textOkCancel.Initialize( 0, (93-14)>>1, 256, 14, _T("OkCancelText"), 0x00000000, SStatus );
	m_textOkCancel.SetAlign( crStaticText::STTA_CENTER );
	m_textOkCancel.SetText( g_Message[ETC_MESSAGE1337].szMessage );
	m_DlgOkCancel.PushChild( &m_textOkCancel );
	m_btnOkCancel_Yes.Initialize( 128, 93-17, 64, 16, _T("OkCancel_Yes"), 0x00000000, SStatus );
	m_DlgOkCancel.PushChild( &m_btnOkCancel_Yes );
	m_btnOkCancel_Yes.SetEventFunction( FnCancelYes );
	int iter =0;
	for( iter = 0 ; iter < 3 ; ++iter )
		m_btnOkCancel_Yes.SetSprite( 
			iter, 
			g_pRenderer->CreateSpriteObject( GetFile( _T("menu_3.tga"), DATA_TYPE_UI ), 0, 16*iter, 64, 16, 0 )
			);	
	m_btnOkCancel_No.Initialize( 192, 93-17, 64, 16, _T("OkCancel_No"), 0x00000000, SStatus );
	m_DlgOkCancel.PushChild( &m_btnOkCancel_No );
	m_btnOkCancel_No.SetEventFunction( FnCancelNo );
	for( iter = 0 ; iter < 3 ; ++iter )
		m_btnOkCancel_No.SetSprite( 
			iter, 
			g_pRenderer->CreateSpriteObject( GetFile( _T("menu_3.tga"), DATA_TYPE_UI ), 64, 16*iter, 64, 16, 0 )
			);

	DWORD	dwFirstPos = 26;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	m_textGuildName[0].Initialize( 40, dwFirstPos, 124, c_dwLineStep, _T("GuildName1"), 0x00000000, SStatus );
	m_textGuildName[0].SetAlign( crStaticText::STTA_LEFT );
	m_textGuildName[1].Initialize( 146, dwFirstPos, 124, c_dwLineStep, _T("GuildName2"), 0x00000000, SStatus );
	m_textGuildName[1].SetAlign( crStaticText::STTA_LEFT );
	m_textMark[0].Initialize( 134, dwFirstPos, 10, c_dwLineStep, _T("Mark1"), 0x00000000, SStatus );
	m_textMark[0].SetAlign( crStaticText::STTA_CENTER );
	m_textMark[0].SetText( _T(":") );
	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	dwFirstPos += c_dwLineStep;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	m_textLadderPoint[0].Initialize( 40, dwFirstPos, 124, c_dwLineStep, _T("LadderPoint1"), 0x00000000, SStatus );
	m_textLadderPoint[0].SetAlign( crStaticText::STTA_LEFT );
	m_textLadderPoint[1].Initialize( 146, dwFirstPos, 124, c_dwLineStep, _T("LadderPoint2"), 0x00000000, SStatus );
	m_textLadderPoint[1].SetAlign( crStaticText::STTA_LEFT );
	m_textMark[1].Initialize( 134, dwFirstPos, 10, c_dwLineStep, _T("Mark2"), 0x00000000, SStatus );
	m_textMark[1].SetAlign( crStaticText::STTA_CENTER );
	m_textMark[1].SetText( _T(":") );
	m_textLadder.Initialize( 10, dwFirstPos, 30, c_dwLineStep, _T("Ladder"), 0x00000000, SStatus );
	m_textLadder.SetText( g_Message[ETC_MESSAGE1319].szMessage );
	m_textLadder.SetAlign( crStaticText::STTA_LEFT );
	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	dwFirstPos += c_dwLineStep;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	m_textAverageLevel[0].Initialize( 40, dwFirstPos, 124, c_dwLineStep, _T("AverageLevel1"), 0x00000000, SStatus );
	m_textAverageLevel[0].SetAlign( crStaticText::STTA_LEFT );
	m_textAverageLevel[1].Initialize( 146, dwFirstPos, 124, c_dwLineStep, _T("AverageLevel2"), 0x00000000, SStatus );
	m_textAverageLevel[1].SetAlign( crStaticText::STTA_LEFT );
	m_textMark[2].Initialize( 134, dwFirstPos, 10, c_dwLineStep, _T("Mark3"), 0x00000000, SStatus );
	m_textMark[2].SetAlign( crStaticText::STTA_CENTER );
	m_textMark[2].SetText( _T(":") );
	m_textAverage.Initialize( 10, dwFirstPos, 30, c_dwLineStep, _T("Average"), 0x00000000, SStatus );
	m_textAverage.SetText( g_Message[ETC_MESSAGE1320].szMessage );
	m_textAverage.SetAlign( crStaticText::STTA_LEFT );
	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	dwFirstPos += c_dwLineStep;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	m_textMemberCount[0].Initialize( 40, dwFirstPos, 124, c_dwLineStep, _T("MemberCount1"), 0x00000000, SStatus );
	m_textMemberCount[0].SetAlign( crStaticText::STTA_LEFT );
	m_textMemberCount[1].Initialize( 146, dwFirstPos, 124, c_dwLineStep, _T("MemberCount2"), 0x00000000, SStatus );
	m_textMemberCount[1].SetAlign( crStaticText::STTA_LEFT );
	m_textMark[3].Initialize( 134, dwFirstPos, 10, c_dwLineStep, _T("Mark4"), 0x00000000, SStatus );
	m_textMark[3].SetAlign( crStaticText::STTA_CENTER );
	m_textMark[3].SetText( _T(":") );
	m_textMember.Initialize( 10, dwFirstPos, 30, c_dwLineStep, _T("Member"), 0x00000000, SStatus );
	m_textMember.SetText( g_Message[ETC_MESSAGE1321].szMessage );
	m_textMember.SetAlign( crStaticText::STTA_LEFT );
	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	dwFirstPos += c_dwLineStep;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	m_textExplain[0].Initialize( 10, dwFirstPos, 180, c_dwLineStep, _T("ExplainText1"), 0x00000000, SStatus );
	m_textExplain[0].SetAlign( crStaticText::STTA_LEFT );
	m_textExplain[0].SetTextColor( 0xffC0C0C0 );
	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	dwFirstPos += c_dwLineStep;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	m_textExplain[1].Initialize( 10, dwFirstPos, 180, c_dwLineStep, _T("ExplainText2"), 0x00000000, SStatus );
	m_textExplain[1].SetAlign( crStaticText::STTA_LEFT );
	m_textExplain[1].SetTextColor( 0xffC0C0C0 );
	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	dwFirstPos += c_dwLineStep;	

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	m_listGuildMember.Initialize( 40, dwFirstPos, 100, c_dwLineStep*20, _T("MemberList"), 0x00000000, SStatus );
	m_listGuildMember.SetLineStep( c_dwLineStep );
	m_listGuildMember.SetMaxVisbleLine( 20 );
	m_listGuildMember.SetPage( 0 );
	m_listGuildMember.SetTextColor( 0, 0xffffffff );
	m_listGuildMember.SetTextColor( 1, 0xff00ffff );
	m_listGuildMember.SetTextColor( 2, 0xffff0000 );
	m_listGuildMember.SetTextColor( 3, 0xffffff00 );
	m_listGuildMember.SetEventFunction( FnUpdateSelectedItemCount );
	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	m_btnLevelSort.Initialize( 256-64-10, dwFirstPos, 64, 16, _T("LevelSort"), 0x00000000, SStatus );	
	for( iter = 0 ; iter < 3 ; ++iter )
		m_btnLevelSort.SetSprite( 
			iter, 
			g_pRenderer->CreateSpriteObject( GetFile( _T("menu_7.dds"), DATA_TYPE_UI ), 0, 128+16*iter, 64, 16, 0 )
			);
	m_btnLevelSort.SetEventFunction( FnLevelSort );
	m_btnLevelSort.SetEventOverFunction( FnOverLevelSort );
	m_DlgMain.PushChild( &m_btnLevelSort );
	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	TCHAR	strBuffer[0xff];

	for( iter = 0 ; iter < 20 ; ++iter )
	{
		sprintf( strBuffer, _T("%u."), iter + 1 );
		m_textNumber[iter].Initialize( 
			10, 
			dwFirstPos+iter*c_dwLineStep, 
			20, 
			c_dwLineStep, 
			strBuffer, 
			0x00000000, 
			SStatus 
			);
		m_textNumber[iter].SetAlign( crStaticText::STTA_RIGHT );
		m_textNumber[iter].SetText( strBuffer );

		m_DlgMain.PushChild( &m_textNumber[iter] );
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	dwFirstPos += c_dwLineStep;	

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 갱신버튼
	m_btnRefresh.Initialize( 256-64-10, dwFirstPos, 64, 16, _T("LevelSort"), 0x00000000, SStatus );
	for( iter = 0 ; iter < 3 ; ++iter )
		m_btnRefresh.SetSprite( 
		iter, 
		g_pRenderer->CreateSpriteObject( GetFile( _T("menu_7.dds"), DATA_TYPE_UI ), 0, 208+16*iter, 64, 16, 0 )
		);
	m_btnRefresh.SetEventFunction( FnRefresh );
	m_btnRefresh.SetEventOverFunction( FnOverRefresh );
	m_DlgMain.PushChild( &m_btnRefresh );
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	dwFirstPos += c_dwLineStep;	

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 갱신버튼
	m_btnCall.Initialize( 256-64-10, dwFirstPos, 64, 16, _T("Call"), 0x00000000, SStatus );
	for( iter = 0 ; iter < 3 ; ++iter )
		m_btnCall.SetSprite( 
		iter, 
		g_pRenderer->CreateSpriteObject( GetFile( _T("menu_7.dds"), DATA_TYPE_UI ), 64, 128+16*iter, 64, 16, 0 )
		);
	m_btnCall.SetEventFunction( FnCall );
	m_btnCall.SetEventOverFunction( FnOverCall );
	m_DlgMain.PushChild( &m_btnCall );
	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	dwFirstPos += c_dwLineStep;	

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 갱신버튼
	m_btnKick.Initialize( 256-64-10, dwFirstPos, 64, 16, _T("Kick"), 0x00000000, SStatus );
	for( iter = 0 ; iter < 3 ; ++iter )
		m_btnKick.SetSprite( 
		iter, 
		g_pRenderer->CreateSpriteObject( GetFile( _T("menu_7.dds"), DATA_TYPE_UI ), 128, 128+16*iter, 64, 16, 0 )
		);
	m_btnKick.SetEventFunction( FnKick );
	m_btnKick.SetEventOverFunction( FnOverKick );
	m_DlgMain.PushChild( &m_btnKick );
	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	dwFirstPos += c_dwLineStep;	
	dwFirstPos += c_dwLineStep;	

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	m_textClock.Initialize( 160, dwFirstPos, 80, c_dwLineStep, _T("Clock"), 0x00000000, SStatus );
	m_textClock.SetAlign( crStaticText::STTA_LEFT );
	m_textClock.Format( g_Message[ETC_MESSAGE1338].szMessage, 3, 0 );
	m_DlgMain.PushChild( &m_textClock );
	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	dwFirstPos += c_dwLineStep;	

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 준비

/*	m_btnReady.Initialize( 160, dwFirstPos, 64, 16, _T("Ready"), 0x00000000, SStatus );
	for( iter = 0 ; iter < 3 ; ++iter )
		m_btnReady.SetSprite( 
		iter, 
		g_pRenderer->CreateSpriteObject( GetFile( _T("menu_7.dds"), DATA_TYPE_UI ), 64, 208+16*iter, 64, 16, 0 )
		);
	m_btnReady.SetEventFunction( FnReady );
	m_btnReady.SetEventOverFunction( FnOverReady );
	m_DlgMain.PushChild( &m_btnReady );
	m_btnReady.SetVisible( FALSE );	// 임시로 닫음.*/
	m_btnInvitation.Initialize( 256-64-10, dwFirstPos, 64, 16, _T("Invitation"), 0x00000000, SStatus );
	for( iter = 0 ; iter < 3 ; ++iter )
		m_btnInvitation.SetSprite( 
		iter, 
		g_pRenderer->CreateSpriteObject( GetFile( _T("menu_6.dds"), DATA_TYPE_UI ), 64, 144+16*iter, 64, 16, 0 )
		);
	m_btnInvitation.SetEventFunction( FnInvitation );
	m_btnInvitation.SetEventOverFunction( FnOverInvitation );
	m_DlgMain.PushChild( &m_btnInvitation );	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	dwFirstPos += c_dwLineStep;	

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 준비
	m_btnCancel.Initialize( 256-64-10, dwFirstPos, 64, 16, _T("Cancel"), 0x00000000, SStatus );
	for( iter = 0 ; iter < 3 ; ++iter )
		m_btnCancel.SetSprite( 
		iter, 
		g_pRenderer->CreateSpriteObject( GetFile( _T("menu_6.dds"), DATA_TYPE_UI ), 0, 144+16*iter, 64, 16, 0 )
		);
	m_btnCancel.SetEventFunction( FnCancel );
	m_btnCancel.SetEventOverFunction( FnOverCancel );
	m_DlgMain.PushChild( &m_btnCancel );
	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	dwFirstPos += c_dwLineStep;	

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 준비
	m_btnPrev.Initialize( 78-m_listGuildMember.GetRect().left, 400-m_listGuildMember.GetRect().top, 18, 18, _T("PrevList"), 0x00000000, SStatus );
	for( iter = 0 ; iter < 3 ; ++iter )
		m_btnPrev.SetSprite( 
		iter, 
		g_pRenderer->CreateSpriteObject( GetFile( _T("menu_1.dds"), DATA_TYPE_UI ), 201+18*iter, 52, 18, 18, 0 )
		);
	m_btnPrev.SetEventFunction( FnListPrev );
	m_listGuildMember.PushChild( &m_btnPrev );
	
	m_btnNext.Initialize( 
		155-m_listGuildMember.GetRect().left, 
		400-m_listGuildMember.GetRect().top, 
		18, 
		18, 
		_T("NextList"), 
		0x00000000, 
		SStatus 
		);
	for( iter = 0 ; iter < 3 ; ++iter )
		m_btnNext.SetSprite( 
		iter, 
		g_pRenderer->CreateSpriteObject( GetFile( _T("menu_1.dds"), DATA_TYPE_UI ), 201+18*iter, 70, 18, 18, 0 )
		);
	m_btnNext.SetEventFunction( FnListNext );
	m_listGuildMember.PushChild( &m_btnNext );
		
	m_imgListGuildMemberPage.Initialize( 
		78-m_listGuildMember.GetRect().left+m_btnPrev.GetWidth(),
		400-m_listGuildMember.GetRect().top-2,
		64, 
		20, 
		_T("PageBack"),
		0x00000000,
		SStatus
		);
	m_imgListGuildMemberPage.SetSprite( 
		g_pRenderer->CreateSpriteObject( 
			GetFile( 
				_T("menu_1.dds"), 
				DATA_TYPE_UI 
				), 
			192, 
			88, 
			64, 
			20, 
			0 
			)
		);
	m_listGuildMember.PushChild( &m_imgListGuildMemberPage );

	m_textListGuildMemberCurPage.Initialize( 0, 3, 64, c_dwLineStep, _T("PageBackText"), 0x00000000, SStatus );
	m_textListGuildMemberCurPage.SetAlign( crStaticText::STTA_CENTER );
	m_textListGuildMemberCurPage.SetText( _T("0/0") );
	m_imgListGuildMemberPage.PushChild( &m_textListGuildMemberCurPage );

	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	m_DlgMain.PushChild( &m_listGuildMember );

	m_DlgMain.PushChild( &m_textGuildName[0] );
	m_DlgMain.PushChild( &m_textGuildName[1] );
	m_DlgMain.PushChild( &m_textLadderPoint[0] );
	m_DlgMain.PushChild( &m_textLadderPoint[1] );
	m_DlgMain.PushChild( &m_textAverageLevel[0] );
	m_DlgMain.PushChild( &m_textAverageLevel[1] );
	m_DlgMain.PushChild( &m_textMemberCount[0] );
	m_DlgMain.PushChild( &m_textMemberCount[1] );

	m_DlgMain.PushChild( &m_textExplain[0] );
	m_DlgMain.PushChild( &m_textExplain[1] );	

	m_DlgMain.PushChild( &m_textLadder );
	m_DlgMain.PushChild( &m_textAverage );
	m_DlgMain.PushChild( &m_textMember );
	m_DlgMain.PushChild( &m_textMark[0] );
	m_DlgMain.PushChild( &m_textMark[1] );
	m_DlgMain.PushChild( &m_textMark[2] );
	m_DlgMain.PushChild( &m_textMark[3] );	

	SetExplainText( CGuildWarFinalSettingWnd::ETT_DEFAULT );
	return crProxyMenuToDialog::Init();
}

void	CGuildWarFinalSettingWnd::ProcessPacket( GUILD_MATCHBEGIN_SETTING* pPacket )
{
	assert( pPacket );

	const	char*	const szClassID[] =
	{
		"X",
		"F",
		"P",
		"M",
		"R",
		"S"
	};
	switch( pPacket->eType )
	{
	case GMS_TYPE_LADDER_INFO:
		{
			GUILD_MATCHBEGIN_LADDER_INFO&	rLadderInfo = pPacket->Body.m_LadderInfo;

			DWORD	dwMyIndex = 0xffffffff;
			DWORD	dwYourIndex = 0xffffffff;

			bool bIsMySet = false;
			for( DWORD dwIter = 0 ; dwIter < MAX_MATCH_TEAM ; ++dwIter )
			{
				if( rLadderInfo.a_bIsUse[dwIter] )
				{
/*					if( T_T_RANDOM == rLadderInfo.a_team_type[dwIter] )
					{//랜덤 팀이면
						if( !bIsMySet )
						{
							m_textGuildName[0].SetText( g_Message[ETC_MESSAGE1381].szMessage  );
							dwMyIndex = dwIter;
							bIsMySet  = true;
						}
						else
						{
							m_textGuildName[1].SetText( g_Message[ETC_MESSAGE1382].szMessage );
							dwYourIndex = dwIter;
						}
					}
					else
*/					{
						// rLadderInfo.m_szGuildName[dwIter]가 
						if( 0 == ::strcmp( g_pMainPlayer->m_szGuildName, rLadderInfo.m_szGuildName[dwIter] ) )
						{
							dwMyIndex = dwIter;
							m_textGuildName[0].SetText( rLadderInfo.m_szGuildName[dwMyIndex] );
						}
						else if( dwYourIndex == 0xffffffff )
						{
							dwYourIndex = dwIter;
							m_textGuildName[1].SetText( rLadderInfo.m_szGuildName[dwYourIndex] );
						}
					}
				}
			}

			m_textGuildName[0].SetText( rLadderInfo.m_szGuildName[dwMyIndex] );
			m_textGuildName[1].SetText( rLadderInfo.m_szGuildName[dwYourIndex] );

			m_dwLadderGrade = rLadderInfo.m_dwLadderGrade[dwMyIndex];
			m_dwLadderPoint = rLadderInfo.m_dwLadderPoint[dwMyIndex];
			m_textLadderPoint[0].Format( _T("%u%s(%u%s)"), m_dwLadderGrade, g_Message[ETC_MESSAGE1380].szMessage, m_dwLadderPoint, g_Message[ETC_MESSAGE1371].szMessage );

			m_dwEnemyLadderGrade = rLadderInfo.m_dwLadderGrade[dwYourIndex];
			m_dwEnemyLadderPoint = rLadderInfo.m_dwLadderPoint[dwYourIndex];
			m_textLadderPoint[1].Format( _T("%u%s(%u%s)"), m_dwEnemyLadderGrade, g_Message[ETC_MESSAGE1380].szMessage, m_dwEnemyLadderPoint, g_Message[ETC_MESSAGE1371].szMessage);	

			m_dwEnemyWaitMemberAverageLevel = rLadderInfo.m_dwEnemyWaitMemberAverageLevel;	
			m_textAverageLevel[1].Format( _T("Lv \?\?\?/Lv %u"), m_dwEnemyWaitMemberAverageLevel );

			m_dwEnemyWaitMemberCount = rLadderInfo.m_dwEnemyWaitMemberCount;
			m_dwMaxMemberCount = rLadderInfo.m_dwMaxMemberCount;

			m_textMemberCount[1].Format( _T("%u%s(\?\?\?/%u)"), m_dwEnemyWaitMemberCount, g_Message[ETC_MESSAGE1379].szMessage, m_dwMaxMemberCount );	

			m_dwRemainBeginTime = rLadderInfo.dwRemainBeginTime;
			m_dwRemainEndTime = rLadderInfo.dwRemainEndTime;

			m_dwRemainBeginTimeRecvTime = m_dwRemainEndTimeRecvTime = ::timeGetTime();
		}		
		break;

	case GMS_TYPE_MEMBER_LIST:
		{
			GUILD_MATCHBEGIN_MEMBER_LIST&	rMemberList = pPacket->Body.m_MemberList;

			for( WORD iter = 0 ; iter < rMemberList.m_wListSize ; ++iter )
			{
				MEMBERLIST_VALUE_TYPE&	rData = rMemberList.m_aMemberList[iter];
				m_MemberList_TempRep.push_back( rData );
			}

			// 마지막 리스트라면.
			if( rMemberList.m_wIsEnd )
			{
				// 기존의 선택되어있던 아이템이 있을것이다.
				// 그것을 유지하기 위해서 선택되었던 아이템들중, 현재 갱신된
				// 데이터와 같은것이 있으면 선택리스트에 넣어준다.
				for_each( 
					m_listGuildMember.GetItemList().begin(), 
					m_listGuildMember.GetItemList().end(), 
					PredUpdateCalledAndSelected() 
					);
//				m_listGuildMember.ForEach< PredUpdateCalledAndSelectedStruct >();

				m_MemberList.clear();
				m_listGuildMember.DeleteAllItems();

				MEMBERLIST_CONST_ITER	iter_begin  = m_MemberList_TempRep.begin();
				MEMBERLIST_CONST_ITER	iter_end =  m_MemberList_TempRep.end();

				while( iter_begin != iter_end )
				{
					MEMBERLIST_ITER	iter_list = m_MemberList.insert( m_MemberList.end(), (*iter_begin) );

					m_listGuildMember.AddItemFromFormat( 
						(void*)&(*iter_list), 
						"Lv %u %s %s", (*iter_list).wLevel, szClassID[(*iter_list).byClass], (*iter_list).szName );

					++iter_begin;
				}

				for_each( 
					m_listGuildMember.GetItemList().begin(), 
					m_listGuildMember.GetItemList().end(), 
					PredTransformCalledToLock() 
					);
//				m_listGuildMember.ForEach< FnCalledToLockStruct >();

				iter_begin  = m_MemberList_TempRep_Selected.begin();
				iter_end =  m_MemberList_TempRep_Selected.end();

				for_each( 
					m_listGuildMember.GetItemList().begin(), 
					m_listGuildMember.GetItemList().end(), 
					PredSelectedCheck() 
					);
//				m_listGuildMember.ForEach< FnSelectedCheckStruct >();		

				// 임시저장소를 클리어해준다.
				m_MemberList_TempRep.clear();
				m_MemberList_TempRep_Selected.clear();

				UpdateListCount();
			}
		}		
		break;

	case GMS_TYPE_REQUEST_USER_CALL:
	case GMS_TYPE_REQUEST_LADDER_INFO:
	case GMS_TYPE_REQUEST_MEMBER_LIST:
	case GMS_TYPE_REQUEST_SET_READY:
	case GMS_TYPE_REQUEST_RESET_READY:
	case GMS_TYPE_REQUEST_USER_KICK:
	case GMS_TYPE_REQUEST_CANCEL:
	case GMS_TYPE_REQUEST_INVITATION:
		{
            g_pNet->SendMsg( (char*)pPacket, pPacket->GetPacketSize(), SERVER_INDEX_WORLD );
		}
		break;

	default:
		assert( !"Error" );
		break;
	}	
}

void	CGuildWarFinalSettingWnd::SetExplainText( ExplainText_Type eType )
{
	DWORD	dw1;
	DWORD	dw2;

	if( m_bReadyFlag )
	{
		dw1 = ETC_MESSAGE1332;
		dw2 = ETC_MESSAGE1333;
	}
	else if( m_bKickFlag )
	{
		dw1 = ETC_MESSAGE1329;
		dw2 = 0xffffffff;
	}
	else
	{
		switch( eType )
		{
		case ETT_DEFAULT:
			dw1 = ETC_MESSAGE1322;
			dw2 = ETC_MESSAGE1323;
			break;
		case ETT_LEVELSORT:
			dw1 = ETC_MESSAGE1324;
			dw2 = 0xffffffff;
			break;
		case ETT_REFRESH:
			dw1 = ETC_MESSAGE1325;
			dw2 = 0xffffffff;
			break;
		case ETT_CALL:
			dw1 = ETC_MESSAGE1326;
			dw2 = 0xffffffff;
			break;
		case ETT_KICK:
			dw1 = ETC_MESSAGE1327;
			dw2 = ETC_MESSAGE1328;
			break;
		case ETT_SELECT_KICKEE:
			dw1 = 0xffffffff;
			dw2 = 0xffffffff;
			break;
		case ETT_READY:
			if( m_dwMaxMemberCount != count_if( 
				m_listGuildMember.GetItemList().begin(), 
				m_listGuildMember.GetItemList().end(), 
				PredCalledCount() 
				) )
			{
				dw1 = ETC_MESSAGE1334;
				dw2 = ETC_MESSAGE1335;
			}
			else
			{
				dw1 = ETC_MESSAGE1330;
				dw2 = ETC_MESSAGE1331;
			}
			break;
		case ETT_CANCEL:
			dw1 = ETC_MESSAGE1336;
			dw2 = 0xffffffff;
			break;
		case ETT_INVITATION:
			dw1 = ETC_MESSAGE1396;
			dw2 = 0xffffffff;
		default:
			return;
		}
	}

	if( dw1 == 0xffffffff )
		m_textExplain[0].SetText( _T(" ") );
	else
		m_textExplain[0].SetText( g_Message[dw1].szMessage );

	if( dw2 == 0xffffffff )
		m_textExplain[1].SetText( _T(" ") );
	else
		m_textExplain[1].SetText( g_Message[dw2].szMessage );	
}


void	CGuildWarFinalSettingWnd::ListSortByLevel()
{
	sort( m_listGuildMember.GetItemList().begin(), m_listGuildMember.GetItemList().end(), PredSortByLevel() );
//	m_listGuildMember.SortBy< FnLevelSortStruct >();
}

void	CGuildWarFinalSettingWnd::Refresh()
{
	// 모든 데이터를 갱신한다.
	// 보내야 할 패킷을 보내준다.
	// 호출된 전체 인원.
	GUILD_MATCHBEGIN_SETTING	Packet;
	Packet.eType = GMS_TYPE_REQUEST_LADDER_INFO;
	ProcessPacket( &Packet );
	Packet.eType = GMS_TYPE_REQUEST_MEMBER_LIST;
	ProcessPacket( &Packet );
}

void	CGuildWarFinalSettingWnd::UpdateListCount()
{
	m_dwSelectedAndCalledMemberCount 
		= count_if( 
			m_listGuildMember.GetItemList().begin(), 
			m_listGuildMember.GetItemList().end(), 
			PredSelectedAndCalledCount() 
			);

//	m_dwSelectedAndCalledMemberCount = GetListBox().CountBy< FnCountStruct >();

    m_textMemberCount[0].Format( 
		_T("%u%s(%u/%u)"), 
		m_MemberList.size(), 
		g_Message[ETC_MESSAGE1379].szMessage,	
		m_dwSelectedAndCalledMemberCount, 
		m_dwMaxMemberCount 
		);

	DWORD	dwSize = m_listGuildMember.GetItemSize();

	m_dwSelectedAndCalledMemberAverageLevel = 0;
	m_dwMemberAverageLevel = 0;

	DWORD	dwSelCount = 0;

	for( DWORD dwIter = 0 ; dwIter < dwSize ; ++dwIter )
	{
		crListBoxItemInterface*	pItem = m_listGuildMember.GetItem( dwIter );

		MEMBERLIST_VALUE_TYPE*	pVal = reinterpret_cast< MEMBERLIST_VALUE_TYPE* >(pItem->m_pData);

		if( pItem->m_bSelected || pItem->m_bLock )
		{
			m_dwSelectedAndCalledMemberAverageLevel += pVal->wLevel;
			++dwSelCount;
		}

		m_dwMemberAverageLevel += pVal->wLevel;
	}	

	m_dwSelectedAndCalledMemberAverageLevel = 0;
	m_dwMemberAverageLevel = 0;
	
	if( dwSelCount )
		m_dwSelectedAndCalledMemberAverageLevel /= dwSelCount;
		
	if( dwSize )
		m_dwMemberAverageLevel /= dwSize;

	// 레벨 평균이 산출되었으므로 컨트롤의 텍스트를 갱신한다.
	m_textAverageLevel[0].Format( _T("Lv %u/Lv %u"), m_dwSelectedAndCalledMemberAverageLevel, m_dwMemberAverageLevel );

	GetPageCount().Format( "%u/%u", GetListBox().GetPage(), GetListBox().GetPageCount() );
}

void	CGuildWarFinalSettingWnd::UpdateList( const crListBoxItemInterface& rhs )
{
	MEMBERLIST_ITER	iter_begin = m_MemberList_TempRep.begin();
	MEMBERLIST_CONST_ITER	iter_end = m_MemberList_TempRep.end();

	while( iter_end != iter_begin )
	{
		// 받아들인 리스트에서 
		// 기존의 리스트에서 선택되었거나, 호출되었던 것들이 새로운 리스트에서 발견되는지 알아본다.
		if( 0 == strcmp( (*iter_begin).szName, reinterpret_cast< MEMBERLIST_VALUE_TYPE* >(rhs.m_pData)->szName ) )
		{
			// 이름이 똑같은 객체가 발견되었다.
			m_MemberList_TempRep_Selected.push_back( (*iter_begin) );			
			break;
		}

		++iter_begin;
	}
}

void	CGuildWarFinalSettingWnd::UpdateList_SelectedCheck( crListBoxItemInterface& rhs )
{
	MEMBERLIST_ITER	iter_begin = m_MemberList_TempRep_Selected.begin();
	MEMBERLIST_CONST_ITER	iter_end = m_MemberList_TempRep_Selected.end();

	while( iter_end !=  iter_begin )
	{
		// 받아들인 리스트에서 
		// 기존의 리스트에서 선택되었거나, 호출되었던 것들이 새로운 리스트에서 발견되는지 알아본다.
		if( 0 == strcmp( (*iter_begin).szName, reinterpret_cast< MEMBERLIST_VALUE_TYPE* >(rhs.m_pData)->szName ) )
		{
			// 이름이 똑같은 객체가 발견되었다.
			rhs.m_bSelected = TRUE;
			m_MemberList_TempRep_Selected.erase( iter_begin );
			break;
		}
	}
}

void	CGuildWarFinalSettingWnd::Call()
{
	for_each( m_listGuildMember.GetItemList().begin(), m_listGuildMember.GetItemList().end(), PredCallSelectedUser() );
//	m_listGuildMember.ForEach< FnSelectedCall >();

	GUILD_MATCHBEGIN_SETTING	Packet;
	Packet.eType = GMS_TYPE_REQUEST_MEMBER_LIST;
	ProcessPacket( &Packet );
}

void	CGuildWarFinalSettingWnd::Kick()
{	
	if( m_bReadyFlag )
		return;

	if( m_bKickFlag == FALSE )
	{
		CUserInterface::GetInstance()->SetMousePointerType( __MOUSE_POINTER_BUTTON__ );
	}
	else
	{
		CUserInterface::GetInstance()->SetMousePointerType( __MOUSE_POINTER_DEFAULT__ );
	}

	m_bKickFlag = 1-m_bKickFlag;
	/*

	for_each( m_listGuildMember.GetItemList().begin(), m_listGuildMember.GetItemList().end(), PredKickSelectedUser() );
//	m_listGuildMember.ForEach< FnSelectedCall >();

	

	GUILD_MATCHBEGIN_SETTING	Packet;
	Packet.eType = GMS_TYPE_REQUEST_MEMBER_LIST;
	ProcessPacket( &Packet );
	*/
}

void	CGuildWarFinalSettingWnd::ReadyToggle()
{
	if( m_bKickFlag )
		return;

	if( !m_bReadyFlag )
	{
		// 인원체크
		DWORD dwCount
			= count_if( 
				m_listGuildMember.GetItemList().begin(), 
				m_listGuildMember.GetItemList().end(), 
				PredCalledCount() 
				);

		if( m_dwMaxMemberCount != dwCount )
			return;
	}

	GUILD_MATCHBEGIN_SETTING	Packet;

	if( !m_bReadyFlag )
		Packet.eType = GMS_TYPE_REQUEST_SET_READY;
	else
		Packet.eType = GMS_TYPE_REQUEST_RESET_READY;

	// 임시로
	m_bReadyFlag = 1-m_bReadyFlag;
	ProcessPacket( &Packet );
}

void	CGuildWarFinalSettingWnd::ListClick()
{
	if( m_bKickFlag )
	{
		crListBoxItemInterface&	rData = m_listGuildMember.GetLastSelected();

		if( rData.m_bLock == TRUE )
		{
			// 이넘 킥 해준다.
			GUILD_MATCHBEGIN_SETTING	Packet;
			Packet.eType = GMS_TYPE_REQUEST_USER_KICK;
			strcpy( Packet.Body.m_UserCall.szName, reinterpret_cast< CGuildWarFinalSettingWnd::MEMBERLIST_VALUE_TYPE* >(rData.m_pData)->szName );			
			ProcessPacket( &Packet );

			Packet.eType = GMS_TYPE_REQUEST_MEMBER_LIST;
			ProcessPacket( &Packet );

			Kick();			
		}
	}
	else if( m_bReadyFlag )
	{
	}
	else
		UpdateListCount();

	// Kick 모드일때, 선택한놈이 락이라면 그놈 퇴장시킨다.
}

void	CGuildWarFinalSettingWnd::Cancel()
{
	// 차일드에 넣고
	// 확인 받고.
	// 땡
	m_DlgOkCancel.SetPos( g_Mouse.MousePos.x - m_DlgMain.GetRect().left, g_Mouse.MousePos.y - m_DlgMain.GetRect().top );
	m_DlgOkCancel.SetVisible( TRUE );	
}

void	CGuildWarFinalSettingWnd::CancelYes()
{
	m_DlgOkCancel.SetVisible( FALSE );

	GUILD_MATCHBEGIN_SETTING	Packet;
	Packet.eType = GMS_TYPE_REQUEST_CANCEL;
	ProcessPacket( &Packet );

	SetActive( FALSE );
}

void	CGuildWarFinalSettingWnd::CancelNo()
{
	m_DlgOkCancel.SetVisible( FALSE );
}

void	CGuildWarFinalSettingWnd::RenderText()
{
	if( GetProxyInit() == FALSE || m_bInit == FALSE || m_bActive == FALSE )
		return;

	// 렌더링 한다...
	if( m_pDlg )
	{
		DWORD	dwCurTime = ::timeGetTime();		
		
		if( m_dwRemainBeginTime > 0 && dwCurTime - m_dwRemainBeginTimeRecvTime > 1000 )
		{
			m_dwRemainBeginTimeRecvTime += 1000;
			--m_dwRemainBeginTime;
			DWORD dwMin = m_dwRemainBeginTime/60;
			DWORD dwSec = m_dwRemainBeginTime-dwMin*60;

			m_textClock.Format( g_Message[ETC_MESSAGE1338].szMessage, dwMin, dwSec );
		}
		
		m_pDlg->Render( NULL, static_cast< DWORD >(/*m_fPosX*/0), static_cast< DWORD >(/*m_fPosZ*/0), 2002 );
	}
}

void	CGuildWarFinalSettingWnd::SetActive(BOOL bActive )
{
	m_bActive = bActive;

	if( bActive == FALSE )
	{
		ShowSpriteAll();

		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDCLOSE, g_v3InterfaceSoundPos, FALSE);
	}
	else if( m_dwRemainBeginTime == 0 )
	{
		CInterface::GetInstance()->SetWnd((BYTE)m_nIndex);

		m_dwStartingSec = ::timeGetTime();
		m_dwMin = 0xffffffff;
		m_dwMin = 0xffffffff;

		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDOPEN, g_v3InterfaceSoundPos, FALSE);
	}
}

void	CGuildWarFinalSettingWnd::Invitation()
{
	GUILD_MATCHBEGIN_SETTING	Packet;
	Packet.eType = GMS_TYPE_REQUEST_INVITATION;
	ProcessPacket( &Packet );	
}

void	CGuildWarFinalSettingWnd::SetTime( DWORD dwBegin, DWORD dwEnd )
{
	m_dwRemainBeginTime = dwBegin;
	m_dwRemainEndTime = dwEnd;
	
	m_dwRemainBeginTimeRecvTime = m_dwRemainEndTimeRecvTime = ::timeGetTime();
	
	DWORD dwMin = m_dwRemainBeginTime/60;
	DWORD dwSec = m_dwRemainBeginTime-dwMin*60;
	
	m_textClock.Format( g_Message[ETC_MESSAGE1338].szMessage, dwMin, dwSec );
}