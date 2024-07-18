#include ".\crdialog.h"
#include <cassert>
#include "GameDefine.h"
#include "GameControl.h"

extern I4DyuchiGXRenderer*			g_pRenderer;

crDialog::crDialog()
{
	m_pSpr[0] = NULL;
	m_pSpr[1] = NULL;
	m_pSpr[2] = NULL;
}

crDialog::~crDialog()
{
}

BOOL	crDialog::Render( crBaseMenu*	pParentMenu, INT	iX, INT iY, DWORD dwZOrder )
{
	crBaseMenu*	pMenu;

	BOOL	bVal = TRUE;

	if( IsVisible() )
	{
		assert( m_pSpr[0] > 0 && m_pSpr[1] > 0 && m_pSpr[2] > 0 );
		
		VECTOR2 vPos;
		
		vPos.x = GetRect().left + iX;
		vPos.y = GetRect().top + iY;
		
		INT	iHeight = GetHeight() - m_SSpriteRenderInfo[0].m_iHeight - m_SSpriteRenderInfo[2].m_iHeight;
		
		g_pRenderer->RenderSprite(
			m_pSpr[0], 
			&m_SSpriteRenderInfo[0].m_vScale ,
			m_SSpriteRenderInfo[0].m_fRot, 
			&vPos, 
			NULL, 
			m_SSpriteRenderInfo[0].m_dwColor, 
			dwZOrder, 
			0
			);
		
		vPos.y += m_SSpriteRenderInfo[0].m_iHeight;
		
		while( iHeight > 0 )
		{
			g_pRenderer->RenderSprite(
				m_pSpr[1], 
				&m_SSpriteRenderInfo[1].m_vScale ,
				m_SSpriteRenderInfo[1].m_fRot, 
				&vPos, 
				NULL, 
				m_SSpriteRenderInfo[1].m_dwColor, 
				dwZOrder, 
				0
				);
			
			vPos.y += m_SSpriteRenderInfo[1].m_iHeight;
			
			iHeight -= m_SSpriteRenderInfo[1].m_iHeight;
		}	
		
		g_pRenderer->RenderSprite(
			m_pSpr[2], 
			&m_SSpriteRenderInfo[2].m_vScale ,
			m_SSpriteRenderInfo[2].m_fRot, 
			&vPos, 
			NULL, 
			m_SSpriteRenderInfo[2].m_dwColor, 
			dwZOrder, 
			0
			);

		pMenu = GetChild();
		
		if( pMenu )
			bVal &= pMenu->Render( this, iX + GetRect().left, iY + GetRect().top, dwZOrder+1 );
	}	

	pMenu = GetNext();

	if( pMenu )
		bVal &= pMenu->Render( pParentMenu, iX, iY, dwZOrder );

	return bVal;	
}

INT		crDialog::ProcessMessgae()
{
	return -1;
}

BOOL	crDialog::Initialize(
						   INT		iX,
						   INT		iY,
						   INT		iWidth,
						   INT		iHeight,
						   LPCTSTR	lpMenuName,
						   DWORD	dwBackgroundColor,
						   const crBaseMenu_Status&	SStatus
						   )
{
//	m_btnClose.Initialize( )
	if( SStatus.m_bExistClose == TRUE )
	{
		PushChild( &m_btnClose );
	}
    
	crBaseMenu_Status	CloseStatus;
	CloseStatus.m_bDynamicAllocated = FALSE;
	m_btnClose.Initialize( iWidth - 2 - 10, 2, 10, 10, _T("Close"), 0xff000000, CloseStatus );

	m_textName.Initialize( 10, 4, 150, 15, lpMenuName, 0xffffffff, CloseStatus );
	m_textName.SetAlign( crStaticText::STTA_LEFT );
	m_textName.SetText( lpMenuName );

	PushChild( &m_textName );

	m_pSpr[0] = g_pRenderer->CreateSpriteObject( 
		GetFile( _T("df_window1.tif"), DATA_TYPE_UI ),
		0
		);
	assert( m_pSpr[0] );

	m_pSpr[1] = g_pRenderer->CreateSpriteObject( 
		GetFile( _T("df_window2.tif"), DATA_TYPE_UI ),
		0
		);
	assert( m_pSpr[1] );

	m_pSpr[2] = g_pRenderer->CreateSpriteObject( 
		GetFile( _T("df_window3.tif"), DATA_TYPE_UI ),
		0
		);
	assert( m_pSpr[2] );

	IMAGE_HEADER	SImageHeader[3];

	for( DWORD dwIter = 0 ; dwIter < 3 ; ++dwIter )
	{
		m_pSpr[dwIter]->GetImageHeader( &SImageHeader[dwIter], 0 );
	}

	VECTOR2	v2;
	v2.x = 1.0f;
    v2.y = 1.0f;

	m_SSpriteRenderInfo[0].m_dwColor = 0xffffffff;
	m_SSpriteRenderInfo[0].m_fRot = 0.0f;
	m_SSpriteRenderInfo[0].m_vScale = v2;
	m_SSpriteRenderInfo[0].m_iWidth = SImageHeader[0].dwWidth;
	m_SSpriteRenderInfo[0].m_iHeight = SImageHeader[0].dwHeight;

	m_SSpriteRenderInfo[1].m_dwColor = 0xffffffff;
	m_SSpriteRenderInfo[1].m_fRot = 0.0f;
	m_SSpriteRenderInfo[1].m_vScale = v2;
	m_SSpriteRenderInfo[1].m_iWidth = SImageHeader[1].dwWidth;
	m_SSpriteRenderInfo[1].m_iHeight = SImageHeader[1].dwHeight;

	m_SSpriteRenderInfo[2].m_dwColor = 0xffffffff;
	m_SSpriteRenderInfo[2].m_fRot = 0.0f;
	m_SSpriteRenderInfo[2].m_vScale = v2;
	m_SSpriteRenderInfo[2].m_iWidth = SImageHeader[2].dwWidth;
	m_SSpriteRenderInfo[2].m_iHeight = SImageHeader[2].dwHeight;

	return crBaseMenu::Initialize( 
		iX,
		iY,
		iWidth,
		iHeight,
		lpMenuName,
		dwBackgroundColor,
		SStatus
		);
}

INT		crDialog::Destroy()
{
	for( INT iter = 0 ; iter < 3 ; ++iter )
	{
		if( m_pSpr[iter] )
		{
			m_pSpr[iter]->Release();
			m_pSpr[iter] = NULL;
		}
	}
	return TRUE;
}