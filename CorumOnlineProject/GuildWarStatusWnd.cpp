#include ".\guildwarstatuswnd.h"
#include "GameDefine.h"
#include "GameControl.h"
#include "NetworkClient.h"
#include "HashTable.h"
#include "DUNGEON_DATA_EX.h"
#include "DungeonProcess_Sound.h"
#include "Interface.h"
#include "MouseLib.h"
#include "CodeFun.h"
#include "GuildWarFinalSettingWnd.h"
#include "./MessagePool.h"
#include "./Message.h"

extern	CMessagePool				g_Message;

//////////////////////////////////////////////////////////////////////////////////////////////////
extern CGuildListHash*				g_pGuildWarHash;
extern CUserHash*					g_pUserHash;
extern	CNetworkClient*				g_pNet;
extern I4DyuchiGXRenderer*			g_pRenderer;
extern DUNGEON_DATA_EX*				g_pThisDungeon;
extern I4DyuchiGXGeometry*			g_pGeometry;
extern CGuildInfoHash*				g_pGuildInfoHash;
CGuildWarStatusWnd* CGuildWarStatusWnd::c_pThis;

CGuildWarStatusWnd::CGuildWarStatusWnd()
{
}

CGuildWarStatusWnd::~CGuildWarStatusWnd()
{
}

BOOL	CGuildWarStatusWnd::Init()
{
	if( GetProxyInit() == FALSE )
		SetProxyInit();
	else
	{
		return crProxyMenuToDialog::Init();
	}

	m_pDlg = &m_imgTimeBack;

	const float g_fScale = 0.4f;
	
	crBaseMenu_Status	SStatus;
	SStatus.m_bDynamicAllocated = FALSE;

	m_imgTimeBack.Initialize( 256-92, 32, 92, 42, _T("TimeBack"), 0xffffffff, SStatus );
	m_imgTimeBack.SetSprite( 
		g_pRenderer->CreateSpriteObject( GetFile( _T("guild_time.tif"), DATA_TYPE_UI ), 
		0, 
		0, 
		92, 
		42, 
		0 
		) );

	m_imgTimeBack.PushChild( &m_imgTimeStamp );
	m_imgTimeStamp.Initialize( 53-14, 10, 50*g_fScale, 54*g_fScale, _T("TimeStamp"), 0xffffffff, SStatus );
	m_imgTimeStamp.SetSprite( 
		g_pRenderer->CreateSpriteObject( GetFile( _T("guildnumber.tif"), DATA_TYPE_UI ), 
		0, 
		128, 
		50, 
		54, 
		0 
		) );
	
	IDISpriteObject*	pSpr[10];	

	POINT	ptNumber[13];

	ptNumber[0].x = 50;
	ptNumber[0].y = 64;

	ptNumber[1].x = 50;
	ptNumber[1].y = 64;
	
	ptNumber[2].x = 50;
	ptNumber[2].y = 64;
	
	ptNumber[3].x = 50;
	ptNumber[3].y = 64;

	ptNumber[4].x = 50;
	ptNumber[4].y = 64;

	ptNumber[5].x = 50;
	ptNumber[5].y = 64;

	ptNumber[6].x = 50;
	ptNumber[6].y = 64;

	ptNumber[7].x = 50;
	ptNumber[7].y = 64;

	ptNumber[8].x = 50;
	ptNumber[8].y = 64;

	ptNumber[9].x = 50;
	ptNumber[9].y = 64;

	ptNumber[10].x = 150;
	ptNumber[10].y = 54;

	ptNumber[11].x = 112;
	ptNumber[11].y = 54;

	ptNumber[12].x = 130;
	ptNumber[12].y = 54;

	pSpr[0] = g_pRenderer->CreateSpriteObject( GetFile( _T("guildnumber.tif"), DATA_TYPE_UI ), 
		0, 
		0, 
		ptNumber[0].x, 
		ptNumber[0].y,
		0 );
	pSpr[1] = g_pRenderer->CreateSpriteObject( GetFile( _T("guildnumber.tif"), DATA_TYPE_UI ), 
		50, 
		0, 
		ptNumber[1].x, 
		ptNumber[1].y,
		0 );
	pSpr[2] = g_pRenderer->CreateSpriteObject( GetFile( _T("guildnumber.tif"), DATA_TYPE_UI ), 
		100, 
		0, 
		ptNumber[2].x, 
		ptNumber[2].y,
		0 );
	pSpr[3] = g_pRenderer->CreateSpriteObject( GetFile( _T("guildnumber.tif"), DATA_TYPE_UI ), 
		150, 
		0, 
		ptNumber[3].x, 
		ptNumber[3].y,
		0 );
	pSpr[4] = g_pRenderer->CreateSpriteObject( GetFile( _T("guildnumber.tif"), DATA_TYPE_UI ), 
		200, 
		0, 
		ptNumber[4].x, 
		ptNumber[4].y,
		0 );	
	pSpr[5] = g_pRenderer->CreateSpriteObject( GetFile( _T("guildnumber.tif"), DATA_TYPE_UI ), 
		0, 
		64, 
		ptNumber[5].x, 
		ptNumber[5].y,
		0 );
	pSpr[6] = g_pRenderer->CreateSpriteObject( GetFile( _T("guildnumber.tif"), DATA_TYPE_UI ), 
		50, 
		64, 
		ptNumber[6].x, 
		ptNumber[6].y,
		0 );
	pSpr[7] = g_pRenderer->CreateSpriteObject( GetFile( _T("guildnumber.tif"), DATA_TYPE_UI ), 
		100, 
		64, 
		ptNumber[7].x, 
		ptNumber[7].y,
		0 );
	pSpr[8] = g_pRenderer->CreateSpriteObject( GetFile( _T("guildnumber.tif"), DATA_TYPE_UI ), 
		150, 
		64, 
		ptNumber[8].x, 
		ptNumber[8].y,
		0 );
	pSpr[9] = g_pRenderer->CreateSpriteObject( GetFile( _T("guildnumber.tif"), DATA_TYPE_UI ), 
		200, 
		64, 
		ptNumber[9].x, 
		ptNumber[9].y,
		0 );

	m_imgTimeMinute1.Initialize( 14-6, 8, 50*g_fScale, 64*g_fScale, _T("TimeMinute1"), 0xffffffff, SStatus );
	m_imgTimeMinute2.Initialize( 32-6, 8, 50*g_fScale, 64*g_fScale, _T("TimeMinute2"), 0xffffffff, SStatus );
	m_imgTimeSecond1.Initialize( 54-3, 8, 50*g_fScale, 64*g_fScale, _T("TimeSecond1"), 0xffffffff, SStatus );
	m_imgTimeSecond2.Initialize( 72-3, 8, 50*g_fScale, 64*g_fScale, _T("TimeSecond2"), 0xffffffff, SStatus );	

	m_imgTimeTenSecond.Initialize( ((1024-150)>>1)-256+92, ((768-50)>>1)-20, 150, 50, _T("TimeChecker"), 0xffffffff, SStatus );
	m_imgTimeTenSecond.SetImageAlign( crImageList::ILIA_CENTER );
	m_imgTimeTenSecond.SetSprite( 10, 
		g_pRenderer->CreateSpriteObject( 
			GetFile( _T("guildnumber.tif"), DATA_TYPE_UI ), 
			50, 
			128, 
			ptNumber[10].x, 
			ptNumber[10].y,
			0 ), ptNumber[10].x, ptNumber[10].y );
	m_imgTimeTenSecond.SetSprite( 11, 
		g_pRenderer->CreateSpriteObject( 
			GetFile( _T("guildnumber.tif"), DATA_TYPE_UI ), 
			0, 
			185, 
			ptNumber[11].x, 
			ptNumber[11].y,
			0 ), ptNumber[11].x, ptNumber[11].y );
	m_imgTimeTenSecond.SetSprite( 12, 
		g_pRenderer->CreateSpriteObject( 
			GetFile( _T("guildnumber.tif"), DATA_TYPE_UI ), 
			122, 
			185, 
			ptNumber[12].x, 
			ptNumber[12].y,
			0 ), ptNumber[12].x, ptNumber[12].y );

	m_imgTimeTenSecond.SetVisible( FALSE );	
	m_imgTimeTenSecond.SetScale( 3.0f );

	m_imgTimeStamp.SetScale( g_fScale );
	m_imgTimeMinute1.SetScale( g_fScale );
	m_imgTimeMinute2.SetScale( g_fScale );
	m_imgTimeSecond1.SetScale( g_fScale );
	m_imgTimeSecond2.SetScale( g_fScale );
	
	m_imgTimeBack.PushChild( &m_imgTimeMinute1 );
	m_imgTimeBack.PushChild( &m_imgTimeMinute2 );
	m_imgTimeBack.PushChild( &m_imgTimeSecond1 );
	m_imgTimeBack.PushChild( &m_imgTimeSecond2 );
	m_imgTimeBack.PushChild( &m_imgTimeTenSecond );

	for( DWORD dwIter = 0 ; dwIter < 10 ; ++dwIter )
	{
		m_imgTimeMinute1.SetSprite( dwIter, pSpr[dwIter], ptNumber[dwIter].x, ptNumber[dwIter].y, TRUE );
		m_imgTimeMinute2.SetSprite( dwIter, pSpr[dwIter], ptNumber[dwIter].x, ptNumber[dwIter].y, FALSE );
		m_imgTimeSecond1.SetSprite( dwIter, pSpr[dwIter], ptNumber[dwIter].x, ptNumber[dwIter].y, FALSE );
		m_imgTimeSecond2.SetSprite( dwIter, pSpr[dwIter], ptNumber[dwIter].x, ptNumber[dwIter].y, FALSE );

		m_imgTimeTenSecond.SetSprite( dwIter, pSpr[dwIter], ptNumber[dwIter].x, ptNumber[dwIter].y, FALSE );
	}		

	m_ptList[0].x = 0;
	m_ptList[0].y = 65;

	m_ptList[1].x = 0;
	m_ptList[1].y = 106;

	m_ptList[2].x = 128;
	m_ptList[2].y = 65;

	m_ptList[3].x = 128;
	m_ptList[3].y = 106;

	POINT	ptLsit2[4];

	ptLsit2[0].x = 0;
	ptLsit2[0].y = 0;

	ptLsit2[1].x = 0;
	ptLsit2[1].y = 33;

	ptLsit2[2].x = 128;
	ptLsit2[2].y = 0;

	ptLsit2[3].x = 128;
	ptLsit2[3].y = 33;

	for( dwIter = 0 ; dwIter < 4 ; ++dwIter )
	{
		crImage&	rImgGuild = m_imgGuild[dwIter];

        rImgGuild.Initialize( 256+dwIter*128-256+92, 0, 128, 42, _T("GuildPanel"), 0x00000000, SStatus );
		rImgGuild.SetSprite( 
			 g_pRenderer->CreateSpriteObject( 
				GetFile( _T("Guild_1.tga"), DATA_TYPE_UI ), 
				m_ptList[dwIter].x, 
				m_ptList[dwIter].y, 
				128, 
				42, 
				0 
				)
			 );
		m_textGuildName[dwIter].Initialize( 30, 7, 60, 14, _T("GuildName"), 0x00000000, SStatus );
		m_textGuildName[dwIter].SetAlign( crStaticText::STTA_CENTER );
		rImgGuild.PushChild( &m_textGuildName[dwIter] );
		m_textGuildPoint[dwIter].Initialize( 10, 27, 100, 14, _T("GuildPoint"), 0x00000000, SStatus );
		m_textGuildPoint[dwIter].SetAlign( crStaticText::STTA_CENTER );
		rImgGuild.PushChild( &m_textGuildPoint[dwIter] );

		m_imgTimeBack.PushChild( &rImgGuild );

		m_pSpr[dwIter] = g_pRenderer->CreateSpriteObject( GetFile( _T("Guild_1.tga"), DATA_TYPE_UI ), 
			ptLsit2[dwIter].x, 
			ptLsit2[dwIter].y, 
			128, 
			33, 
			0 
			);
	}	
	return crProxyMenuToDialog::Init();
}

//////////////////////////////////////////////////////////////////////////////////////////
/**
*
* \param pPacket 
*/
void	CGuildWarStatusWnd::ProcessPacket( GUILD_MATCH_STATUS_RESULT* pPacket )
{
	DWORD	dwCount = 0;

	for( DWORD dwIter = 1 ; dwIter < MAX_MATCH_TEAM ; ++dwIter )
	{
		DWORD dwCurIndex = dwIter-1;
		crStaticText&	rGuildName = m_textGuildName[dwCurIndex];
		crStaticText&	rGuildPoint = m_textGuildPoint[dwCurIndex];
		crImage&		rImgGuild = m_imgGuild[dwCurIndex];

		if( pPacket->a_bIsUse[dwIter] )
		{			
			rGuildName.SetText( pPacket->szName[dwIter] );

			rGuildPoint.Format( "%d", pPacket->a_point[dwIter] );

			rImgGuild.SetPos( 256+dwCount*128-256+92, 0 );
			++dwCount;
		}

		rImgGuild.SetVisible( pPacket->a_bIsUse[dwIter] == true ? TRUE : FALSE );
	}

	m_dwRemainBeginTime = pPacket->dwRemainBeginTime;
	m_dwRemainEndTime = pPacket->dwRemainEndTime;

	m_dwRemainEndTimeRecvTime = m_dwRemainBeginTimeRecvTime= ::timeGetTime();

	UpdateBeginTime();
	UpdateEndTime();
}

void	CGuildWarStatusWnd::ProcessPacket( GUILD_MATCH_STATUS* pPacket )
{
	GUILD_MATCH_GUILDINFO&	rInfo = pPacket->m_GuildInfo;
	DWORD	dwCount = 0;

	for( DWORD dwIter = 1 ; dwIter < MAX_MATCH_TEAM ; ++dwIter )
	{
		DWORD dwCurIndex = dwIter-1;
		crStaticText&	rGuildPoint = m_textGuildPoint[dwCurIndex];
		crImage&		rImgGuild = m_imgGuild[dwCurIndex];

		if( rInfo.a_bIsUse[dwIter] )
		{
			rGuildPoint.Format( "%d", rInfo.a_point[dwIter] );

			rImgGuild.SetPos( 256+dwCount*128-256+92, 0 );
			++dwCount;
		}

		rImgGuild.SetVisible( rInfo.a_bIsUse[dwIter] == true ? TRUE : FALSE );
	}

	m_dwRemainBeginTime = pPacket->m_GuildInfo.dwRemainBeginTime;
	m_dwRemainEndTime = pPacket->m_GuildInfo.dwRemainEndTime;

	m_dwRemainEndTimeRecvTime = m_dwRemainBeginTimeRecvTime= ::timeGetTime();

	UpdateBeginTime();
	UpdateEndTime();

	switch( rInfo.byState )
	{
	case TMS_RUN:
		if( CGuildWarFinalSettingWnd::GetInstance() && CGuildWarFinalSettingWnd::GetInstance()->GetActive() )
			CGuildWarFinalSettingWnd::GetInstance()->SetActive( FALSE );
		break;

	case TMS_PREPARE:
		if( CGuildWarFinalSettingWnd::GetInstance() && CGuildWarFinalSettingWnd::GetInstance()->GetActive() )
			CGuildWarFinalSettingWnd::GetInstance()->SetTime( m_dwRemainBeginTime, m_dwRemainEndTime );
		break;
	}
}
//////////////////////////////////////////////////////////////////////////////////////////

void	CGuildWarStatusWnd::RenderText()
{
	if( GetProxyInit() == FALSE || m_bInit == FALSE || m_bActive == FALSE )
		return;

	if( g_pThisDungeon->IsStadium() )//길드전 맵이고 : hwoarang 050202
	{
		CUser*	pUser;

		ListNode< CUser >* pUserNode = g_pUserHash->GetHead();
		
		while( pUserNode )
		{
			LP_GUILD_DATA lpGuildInfo;
			pUser = pUserNode->pData;

			if( pUser->m_dwGuildWarFlag != G_W_F_OBSERVER && pUser->m_dwGuildWarFlag != G_W_F_NONE )
			{
				VECTOR3	vPlayerHeadPos, vOutPos;

				vPlayerHeadPos.x	= pUser->m_v3CurPos.x;
				vPlayerHeadPos.y	= pUser->m_v3CurPos.y + 200.0f;
				vPlayerHeadPos.z	= pUser->m_v3CurPos.z;
				
				GetScreenXYFromXYZ(g_pGeometry, 0, &vPlayerHeadPos, &vOutPos);
				
				WORD	x = WORD( 1024.0f * vOutPos.x );
				WORD	y = WORD( 768.0f * vOutPos.y );
				
				if(vOutPos.x >= 1.0f || vOutPos.y >= 1.0f )			
					goto lb_Again;
				
				VECTOR2	vPos;
				vPos.x		= float(x - 64);
				vPos.y		= float(y - 16);
				g_pRenderer->RenderSprite( 
					m_pSpr[pUser->m_team_index-1], 
					NULL, 
					0.0f, 
					&vPos, 
					NULL, 
					0x80ffffff, 
					2000, 
					RENDER_TYPE_DISABLE_TEX_FILTERING|RENDER_TYPE_SPRITE_OPASITY
					);
				lpGuildInfo = g_pGuildInfoHash->GetData(pUser->m_dwGuildId);
				
				if(lpGuildInfo)
				{
					RenderFont( 
						const_cast< char* >(lpGuildInfo->szGuildName), 
						x - 60	+3,
						x + 60	+3, 
						y - 14	+1, 
						y		+1,
						2001, 
						0x80ffffff
						);
				}
				else if(g_pMainPlayer->m_dwUserIndex==pUser->m_dwUserIndex)
				{
					RenderFont( 
						const_cast< char* >(g_pMainPlayer->m_szGuildName), 
						x - 60	+3,
						x + 60	+3, 
						y - 14	+1, 
						y		+1,
						2001, 
						0x80ffffff
						);
				}
				
				RenderFont( 
					const_cast< char* >(pUser->m_szName), 
					x - 60	+3,
					x + 60	+3, 
					y		+1, 
					y+14	+1,
					2001, 
					0x80ffffff
					);
			}		
			
lb_Again:
			pUserNode = pUserNode->pNext;
		}

		// 렌더링 한다...
		if( m_pDlg )
		{			
			DWORD	dwCurTime = ::timeGetTime();		
			
			if( m_dwRemainBeginTime > 0 && dwCurTime > 1000 + m_dwRemainBeginTimeRecvTime )
			{
				m_dwRemainBeginTimeRecvTime += 1000;
				--m_dwRemainBeginTime;		
				UpdateBeginTime();
			}
			
			if( m_dwRemainEndTime > 0 && dwCurTime > 1000 + m_dwRemainEndTimeRecvTime )
			{
				m_dwRemainEndTimeRecvTime += 1000;
				--m_dwRemainEndTime;
				UpdateEndTime();				
			}			

			m_pDlg->Render( NULL, static_cast< DWORD >(/*m_fPosX*/0), static_cast< DWORD >(/*0m_fPosZ*/0), 2002 );
		}
	}
	else if( g_pMainPlayer->m_bMatching )
	{
		if( m_dwPVP_Start != 0xffffffff )
		{
			if( ::timeGetTime() - m_dwPVP_Start > 2000 )
				m_imgTimeTenSecond.SetVisible( FALSE );
		}

		m_imgTimeTenSecond.RenderOnly( NULL, 256-92, 32, 2003 );
	}
}

void	CGuildWarStatusWnd::UpdateBeginTime()
{		
	DWORD dwRemainBeginTime = (m_dwRemainBeginTime > 0 ?  m_dwRemainBeginTime - 1 : 0);
	m_imgTimeMinute1.SetCurIndex( dwRemainBeginTime/600 );
	m_imgTimeMinute2.SetCurIndex( dwRemainBeginTime/60 );
	m_imgTimeSecond1.SetCurIndex( (dwRemainBeginTime%60)/10 );
	m_imgTimeSecond2.SetCurIndex( (dwRemainBeginTime%60)%10 );
	
	if( m_dwRemainBeginTime <= 10 )
	{
		if( m_dwRemainBeginTime == 10 && m_imgTimeTenSecond.IsVisible() == FALSE )
		{
			m_imgTimeTenSecond.SetVisible( TRUE );
			m_imgTimeTenSecond.SetCurIndex( 9 );
		}
		else if( m_dwRemainBeginTime == 0 )
		{						
			m_imgTimeTenSecond.SetVisible( FALSE );
		}
		else if( m_dwRemainBeginTime == 1 )
		{
			m_imgTimeTenSecond.SetCurIndex( 10 );
			_PlaySound( 0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_GUILDWAR_START, g_v3InterfaceSoundPos, FALSE );
		}
		else
		{
			m_imgTimeTenSecond.SetCurIndex( m_dwRemainBeginTime-1 );
			_PlaySound( 0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_GUILDWAR_COUNT, g_v3InterfaceSoundPos, FALSE );
		}
	}
}

void	CGuildWarStatusWnd::UpdateEndTime()
{	
	if( m_dwRemainBeginTime == 0 )					
	{
		m_imgTimeMinute1.SetCurIndex( m_dwRemainEndTime/600 );
		m_imgTimeMinute2.SetCurIndex( (m_dwRemainEndTime/60)%10 );
		m_imgTimeSecond1.SetCurIndex( (m_dwRemainEndTime%60)/10 );
		m_imgTimeSecond2.SetCurIndex( (m_dwRemainEndTime%60)%10 );
	}
}

void	CGuildWarStatusWnd::SetActive(BOOL bActive )
{
	m_bActive = bActive;

	if( m_bActive == FALSE )
		m_imgTimeTenSecond.SetVisible( FALSE );
}

void	CGuildWarStatusWnd::Draw()
{
	m_imgTimeTenSecond.SetVisible( TRUE );
	m_imgTimeTenSecond.SetCurIndex( 12 );
}

void	CGuildWarStatusWnd::Win()
{
	m_imgTimeTenSecond.SetVisible( TRUE );
	m_imgTimeTenSecond.SetCurIndex( 11 );
	_PlaySound( 0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_GUILDWAR_WIN, g_v3InterfaceSoundPos, FALSE );
/*
	if( 0 == ::strcmp( g_pMainPlayer->m_szGuildName, WinTeam ) )
	{
		m_imgTimeTenSecond.SetCurIndex( 11 );
		_PlaySound( 0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_GUILDWAR_WIN, g_v3InterfaceSoundPos, FALSE );
	}
	else
	{
		m_imgTimeTenSecond.SetCurIndex( 12 );
		_PlaySound( 0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_GUILDWAR_LOSE, g_v3InterfaceSoundPos, FALSE );
	}
	*/
}

void	CGuildWarStatusWnd::Lose()
{
	m_imgTimeTenSecond.SetVisible( TRUE );
	m_imgTimeTenSecond.SetCurIndex( 12 );
	_PlaySound( 0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_GUILDWAR_LOSE, g_v3InterfaceSoundPos, FALSE );
}