#include ".\crproxymenutodialog.h"
#include "DungeonProcess_Sound.h"
#include "Interface.h"

extern VECTOR3						g_v3InterfaceSoundPos;

crProxyMenuToDialog::crProxyMenuToDialog()
: m_bProxyInit(FALSE)
{
	m_pDlg = NULL;
}

crProxyMenuToDialog::~crProxyMenuToDialog()
{
}

void	crProxyMenuToDialog::CreateMenu(
						   char*	pszName, 
						   float	fWidth, 
						   float	fHeight, 
						   BYTE	byType, 
						   int		nIndex, 
						   BOOL	bActive, 
						   BOOL	bScroll
						   )
{	
	CMenu::CreateMenu( 
		pszName, 
		fWidth, 
		fHeight, 
		byType, 
		nIndex, 
		bActive, 
		bScroll 
		);

	Init();
}

BOOL	crProxyMenuToDialog::Init()
{
	m_bInit = TRUE;
	return m_bInit;
}

void	crProxyMenuToDialog::RenderText()
{
	if( GetProxyInit() == FALSE || m_bInit == FALSE || m_bActive == FALSE )
		return;

	// ·»´õ¸µ ÇÑ´Ù...
	if( m_pDlg )
		m_pDlg->Render( NULL, static_cast< DWORD >(/*m_fPosX*/0), static_cast< DWORD >(/*m_fPosZ*/0), 2002 );
}

void	crProxyMenuToDialog::SetActive(BOOL bActive )
{
	m_bActive = bActive;

	if( bActive == FALSE )
	{
		ShowSpriteAll();

		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDCLOSE, g_v3InterfaceSoundPos, FALSE);
	}
	else
	{
		CInterface::GetInstance()->SetWnd((BYTE)m_nIndex);

		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDOPEN, g_v3InterfaceSoundPos, FALSE);
	}
}

void	crProxyMenuToDialog::Remove()
{
	RemoveAllData();
	m_bActive	= FALSE;
	m_bInit		= FALSE;
}

void	crProxyMenuToDialog::SetOrder()
{
}

INT		crProxyMenuToDialog::CheckInterface()
{
	if( GetProxyInit() == FALSE || m_bInit == FALSE || m_bActive == FALSE )
		return FALSE;

	m_pDlg->ValidateState();
	m_pDlg->Update( NULL, 0, 0 );
    
	return CInterface::GetInstance()->GetSetWnd();
}