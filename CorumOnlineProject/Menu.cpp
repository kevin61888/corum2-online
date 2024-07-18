//==================================================//
// Code by Jang.						2002.12.9	//
//==================================================//
#include "Menu.h"
#include "SpriteManager.h"
#include "InitGame.h"
#include "GameControl.h"
#include "Interface.h"
#include "UserInterface.h"
#include "CodeFun.h"


//==================================================//
// Construction/Destruction.						//
//==================================================//
CMenu::CMenu()
{
	m_fPosX				= 0;
	m_fPosZ				= 0;
	m_fPrevPosX			= 0;
	m_fPrevPosZ			= 0;
	m_fMaxSizeWidth		= 0;
	m_fMaxSizeHeight	= 0;
	m_byOrder			= 0;	
	m_bySpriteDataCnt	= 0;
	m_byChkInterfaceCnt	= 0;
	m_byScrollChk		= 1;
	m_nCurScrollPos		= 0;
	m_nScrollPos		= 1;
	m_nIndex			= 0;
	m_byType			= __MENU_TYPE_DEFAULT__;
	m_bPointer			= FALSE;
	m_bInit				= FALSE;
	m_bMouseIcon		= FALSE;
	m_bDownChk			= FALSE;
	m_bActive			= FALSE;
	m_bClick			= FALSE;
	m_bUp				= FALSE;	
	m_bDown				= FALSE;	
	m_bUpChk			= FALSE;
	m_bClose			= FALSE;
	m_bSoundChk			= FALSE;
	m_bScCheckInterface	= FALSE;
	m_bScChk[0]			= FALSE;
	m_bScChk[1]			= FALSE;
	InitSpriteData();
	InitCheckInterface();
}

CMenu::~CMenu()
{

}


//==================================================//
// Class Member Function.							//
//==================================================//
void CMenu::InitSpriteData()
{
	for(int i = 0; i < MAX_SPRITE_DATA; i++)
	{
		memset(&m_sSprite_Data[i], 0, sizeof(m_sSprite_Data[i]));		
		m_sSprite_Data[i].dwId	= 0xff;
	}
}

void CMenu::InitCheckInterface()
{
	m_byChkInterfaceCnt = 0;

	for(int i = 0; i < MAX_CHECK_INTERFACE; i++)
	{
		m_rt[i].left	= 0;
		m_rt[i].right	= 0;
		m_rt[i].top		= 0;
		m_rt[i].bottom	= 0;
		m_rtType[i]		= CHECK_NONE;
	}
}

void CMenu::SetCheckInterfacePos(int nStartX, int nStartY, int nEndX, int nEndY, BYTE byReturn)
{
	m_rt[byReturn].left		= nStartX;
	m_rt[byReturn].right	= nEndX;
	m_rt[byReturn].top		= nStartY;
	m_rt[byReturn].bottom	= nEndY;
}

BOOL CMenu::InsertCheckInterface(int nStartX, int nStartY, int nEndX, int nEndY, BYTE byReturn
								 , CHECKTYPE sCheckType, BYTE byValue, BYTE byOrder)
{
	if(byReturn==0 || byReturn==MAX_CHECK_INTERFACE)
		return FALSE;

	m_rt[byReturn].left		= nStartX;
	m_rt[byReturn].right	= nEndX;
	m_rt[byReturn].top		= nStartY;
	m_rt[byReturn].bottom	= nEndY;		
	m_rtType[byReturn]		= sCheckType;
	m_byValue[byReturn]		= byValue;
	m_nOrder[byReturn]		= (int)byOrder;
	m_byChkInterfaceCnt++;

	if(sCheckType==CHECK_SCROLL)
		m_bScCheckInterface	= TRUE;

	return TRUE;
}

BOOL CMenu::InsertData(DWORD dwId, DWORD dwSprId, float fPosX, float fPosY, float fScalingX, float fScalingY
					   , int nOrder, BOOL bRender, BOOL bPos, BOOL bOrderChk, DWORD dwAlpha, float fRot)
{
	BYTE byIndex = 0;

	for(int i = 0; i < MAX_SPRITE_DATA; i++)
	{
		if(m_sSprite_Data[i].dwId==0xff)
		{
			byIndex = i; 
			break;
		}
	}

	if(m_bySpriteDataCnt<MAX_SPRITE_DATA && byIndex<MAX_SPRITE_DATA)
	{
		m_sSprite_Data[byIndex].dwId		= dwId;
		m_sSprite_Data[byIndex].dwSprId		= dwSprId;
		m_sSprite_Data[byIndex].fPosX		= fPosX;
		m_sSprite_Data[byIndex].fPosY		= fPosY;
		m_sSprite_Data[byIndex].nOrder		= nOrder;
		m_sSprite_Data[byIndex].bOrder		= bOrderChk;
		m_sSprite_Data[byIndex].dwAlpha		= dwAlpha;
		m_sSprite_Data[byIndex].bPos		= bPos;
		m_sSprite_Data[byIndex].fRot		= fRot;
		m_sSprite_Data[byIndex].bRender		= bRender;
		m_sSprite_Data[byIndex].v2Scaling.x	= fScalingX;
		m_sSprite_Data[byIndex].v2Scaling.y	= fScalingY;
		m_bySpriteDataCnt++;
		return TRUE;
	}		

	return FALSE;
}


BYTE CMenu::GetChk(int nValue, BOOL bPos, BYTE byOrder)
{
	for(int i = 0; i < MAX_CHECK_INTERFACE; i++)
	{
		if(i==1 || i==0)
			continue;
		if( !byOrder )
		{
			if(!bPos)
			{
				if(m_byValue[i]==0)
				{
					if(	g_Mouse.MousePos.x>=m_fPosX+m_rt[i].left &&
						g_Mouse.MousePos.x<=m_fPosX+m_rt[i].right &&
						g_Mouse.MousePos.y>=m_fPosZ+m_rt[i].top && 
						g_Mouse.MousePos.y<=m_fPosZ+m_rt[i].bottom)
					{
						return i;
					}
				}			
				else if(m_byValue[i]==1)
				{
					if(	g_Mouse.MousePos.x>=m_fPosX+m_rt[i].left &&
						g_Mouse.MousePos.x<=m_fPosX+m_rt[i].right &&
						g_Mouse.MousePos.y>=m_fPosZ+m_rt[i].top+nValue &&
						g_Mouse.MousePos.y<=m_fPosZ+m_rt[i].bottom+nValue)
					{
						return i;
					}
				}
				else if(m_byValue[i]==2)
				{
					if(	g_Mouse.MousePos.x>=m_fPosX+m_rt[i].left &&
						g_Mouse.MousePos.x<=m_fPosX+m_rt[i].right &&
						g_Mouse.MousePos.y>=m_fPosZ+m_rt[i].top &&
						g_Mouse.MousePos.y<=m_fPosZ+m_rt[i].bottom+nValue)
					{
						return i;
					}
				}
			}
			else
			{
				if(m_byValue[i]==0)
				{
					if(	g_Mouse.MousePos.x>=m_rt[i].left &&
						g_Mouse.MousePos.x<=m_rt[i].right &&
						g_Mouse.MousePos.y>=m_rt[i].top && 
						g_Mouse.MousePos.y<=m_rt[i].bottom)
					{
						return i;
					}
				}
				else if(m_byValue[i]==1)
				{
					if(	g_Mouse.MousePos.x>=m_rt[i].left &&
						g_Mouse.MousePos.x<=m_rt[i].right && 
						g_Mouse.MousePos.y>=m_rt[i].top+nValue &&
						g_Mouse.MousePos.y<=m_rt[i].bottom+nValue)
					{
						return i;
					}
				}
				else if(m_byValue[i]==2)
				{
					if(	g_Mouse.MousePos.x>=m_rt[i].left &&
						g_Mouse.MousePos.x<=m_rt[i].right &&
						g_Mouse.MousePos.y>=m_rt[i].top &&
						g_Mouse.MousePos.y<=m_rt[i].bottom+nValue)
					{
						return i;
					}
				}
			}
		}
		else
		{
			if( m_nOrder[i] == byOrder )
			{
				if(	g_Mouse.MousePos.x>=m_fPosX+m_rt[i].left &&
					g_Mouse.MousePos.x<=m_fPosX+m_rt[i].right &&
					g_Mouse.MousePos.y>=m_fPosZ+m_rt[i].top && 
					g_Mouse.MousePos.y<=m_fPosZ+m_rt[i].bottom)
				{
					return i;
				}
			}
		}
	}

	if(!bPos)
	{
		if(	g_Mouse.MousePos.x>=m_fPosX &&
			g_Mouse.MousePos.x<=m_fPosX+m_fMaxSizeWidth &&
			g_Mouse.MousePos.y>=m_fPosZ &&
			g_Mouse.MousePos.y<=m_fPosZ+m_fMaxSizeHeight+nValue)
		{
			return 1;
		}
	}
	else
	{
		if(	g_Mouse.MousePos.x>=m_rt[1].left &&
			g_Mouse.MousePos.x<=m_rt[1].right &&
			g_Mouse.MousePos.y>=m_rt[1].top && 
			g_Mouse.MousePos.y<=m_rt[1].bottom)
		{
			return 1;
		}
	}

	return 0;
}

BOOL CMenu::GetRender(DWORD dwId)
{
	for(int i = 0; i < m_bySpriteDataCnt; i++)
	{
		if(m_sSprite_Data[i].dwId==dwId)		
			return m_sSprite_Data[i].bRender;
	}

	return FALSE;
}

BOOL CMenu::SetRotObj(DWORD dwId, float fRot)
{
	for(int i = 0; i < m_bySpriteDataCnt; i++)
	{
		if(m_sSprite_Data[i].dwId==dwId)
		{			
			m_sSprite_Data[i].fRot = fRot;			
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CMenu::SetScalingObjX(DWORD dwId, float fScalingX)
{
	for(int i = 0; i < m_bySpriteDataCnt; i++)
	{
		if(m_sSprite_Data[i].dwId==dwId)
		{			
			m_sSprite_Data[i].v2Scaling.x = fScalingX;			
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CMenu::SetScalingObj(DWORD dwId, float fScalingX, float fScalingY)
{
	for(int i = 0; i < m_bySpriteDataCnt; i++)
	{
		if(m_sSprite_Data[i].dwId==dwId)
		{			
			m_sSprite_Data[i].v2Scaling.x = fScalingX;
			m_sSprite_Data[i].v2Scaling.y = fScalingY;			
			return TRUE;
		}
	}
	
	return FALSE;
}

BOOL CMenu::SetScalingObjY(DWORD dwId, float fScalingY)
{
	for(int i = 0; i < m_bySpriteDataCnt; i++)
	{
		if(m_sSprite_Data[i].dwId==dwId)
		{			
			m_sSprite_Data[i].v2Scaling.y = fScalingY;			
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CMenu::SetPosObj(DWORD dwId, float fPosX, float fPosY)
{
	for(int i = 0; i < m_bySpriteDataCnt; i++)
	{
		if(m_sSprite_Data[i].dwId==dwId)
		{		
			m_sSprite_Data[i].fPosX	= fPosX;
			m_sSprite_Data[i].fPosY	= fPosY;			
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CMenu::SetPosObjX(DWORD dwId, int nPosX)
{
	return SetPosObjX(dwId, (float)nPosX);
}

BOOL CMenu::SetPosObjX(DWORD dwId, float fPosX)
{
	for(int i = 0; i < m_bySpriteDataCnt; i++)
	{
		if(m_sSprite_Data[i].dwId==dwId)
		{			
			m_sSprite_Data[i].fPosX	= fPosX;
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CMenu::SetPosObjY(DWORD dwId, int nPosY)
{
	return SetPosObjY(dwId, (float)nPosY);
}

BOOL CMenu::SetPosObjY(DWORD dwId, float fPosY)
{
	for(int i = 0; i < m_bySpriteDataCnt; i++)
	{
		if(m_sSprite_Data[i].dwId==dwId)
		{			
			m_sSprite_Data[i].fPosY	= fPosY;
			return TRUE;
		}
	}

	return FALSE;
}

void CMenu::RemoveAllData()
{
	for(int i = 0; i < MAX_SPRITE_DATA; i++)
	{
		m_sSprite_Data[i].dwId			= 0xff;
		m_sSprite_Data[i].dwSprId		= 0;
		m_sSprite_Data[i].fPosX			= 0;
		m_sSprite_Data[i].fPosY			= 0;
		m_sSprite_Data[i].dwAlpha		= 0;
		m_sSprite_Data[i].nOrder		= 0;
		m_sSprite_Data[i].fRot			= 0;
		m_sSprite_Data[i].bOrder		= FALSE;
		m_sSprite_Data[i].bPos			= FALSE;
		m_sSprite_Data[i].bRender		= FALSE;
		m_sSprite_Data[i].v2Scaling.x	= 0;
		m_sSprite_Data[i].v2Scaling.y	= 0;		
	}

	m_bySpriteDataCnt = 0;
}

void CMenu::RemoveData(DWORD dwSprId)
{
	for(int i = 0; i < m_bySpriteDataCnt; i++)
	{		
		if(dwSprId==m_sSprite_Data[i].dwId)
		{			
			m_sSprite_Data[i].dwId			= 0xff;
			m_sSprite_Data[i].dwSprId		= 0;
			m_sSprite_Data[i].fPosX			= 0;
			m_sSprite_Data[i].fPosY			= 0;
			m_sSprite_Data[i].dwAlpha		= 0;
			m_sSprite_Data[i].nOrder		= 0;
			m_sSprite_Data[i].fRot			= 0;
			m_sSprite_Data[i].bOrder		= FALSE;
			m_sSprite_Data[i].bPos			= FALSE;
			m_sSprite_Data[i].bRender		= FALSE;
			m_sSprite_Data[i].v2Scaling.x	= 0;
			m_sSprite_Data[i].v2Scaling.y	= 0;
			m_bySpriteDataCnt--;
			return;
		}
	}
}

void CMenu::SetRender(DWORD dwId, BOOL bRender)
{
	for(int i = 0; i < m_bySpriteDataCnt; i++)
	{		
		if(dwId==m_sSprite_Data[i].dwId)
		{
			m_sSprite_Data[i].bRender = bRender;
			return;
		}
	}
}

LP_SPR_RENDER_DATA CMenu::GetData(DWORD dwId)
{
	LP_SPR_RENDER_DATA lpSprRenderData = NULL;

	for(int i = 0; i < m_bySpriteDataCnt; i++)
	{		
		if(dwId==m_sSprite_Data[i].dwId)
		{
			lpSprRenderData = &m_sSprite_Data[i];
			return lpSprRenderData;		
		}
	}
	return lpSprRenderData;
}

void CMenu::Render()
{	
	for(int i = 0; i < m_bySpriteDataCnt; i++)
	{
		if(m_sSprite_Data[i].dwId!=0xff && m_sSprite_Data[i].bRender)
		{
			LP_SPRITE_DATA lpSpriteData = g_pInterfaceSprHash->GetData(m_sSprite_Data[i].dwSprId);
			
			if(lpSpriteData)
			{	
				VECTOR2 vPos;

				if(!m_sSprite_Data[i].bPos)
				{
					vPos.x	= m_sSprite_Data[i].fPosX;
					vPos.y	= m_sSprite_Data[i].fPosY;
				}
				else
				{
					vPos.x	= m_fPosX + m_sSprite_Data[i].fPosX;
					vPos.y	= m_fPosZ + m_sSprite_Data[i].fPosY;
				}								

				int nOrder = 0;

				if(m_sSprite_Data[i].bOrder)	
					nOrder = __ORDER_INTERFACE_START__ + m_byOrder * 10 + m_sSprite_Data[i].nOrder;
				else
					nOrder = m_sSprite_Data[i].nOrder;

				if(m_sSprite_Data[i].v2Scaling.x==1.0f && m_sSprite_Data[i].v2Scaling.y==1.0f)
				{
					g_pRenderer->RenderSprite(lpSpriteData->pSpr, NULL
						, m_sSprite_Data[i].fRot, &vPos, NULL, m_sSprite_Data[i].dwAlpha, nOrder, 0);
				}
				else
				{
					g_pRenderer->RenderSprite(lpSpriteData->pSpr, &m_sSprite_Data[i].v2Scaling
						, m_sSprite_Data[i].fRot, &vPos, NULL, m_sSprite_Data[i].dwAlpha, nOrder, 0);				
				}
			}
		}
	}
}

void CMenu::ShowSpriteAll()
{
	for(int i = 0; i < m_bySpriteDataCnt; i++)
	{
		if(m_sSprite_Data[i].dwId==0xff)
			continue;

		m_sSprite_Data[i].bRender = FALSE;
	}
}

void CMenu::SetPos(float fPosX, float fPosZ)
{
	if(fPosX<0)
	{
		m_fPosX		= 0;
		m_fPrevPosX = 0;
	}
	else if(fPosX>__GAME_SIZE_X__-m_fMaxSizeWidth)
	{
		m_fPosX		= __GAME_SIZE_X__-m_fMaxSizeWidth;
		m_fPrevPosX = __GAME_SIZE_X__-m_fMaxSizeWidth; 
	}
	else
	{
		m_fPosX		= fPosX;
		m_fPrevPosX = fPosX; 
	}

	if(fPosZ<0)
	{				
		m_fPosZ		= 0;
		m_fPrevPosZ	= 0;
	}
	else if(fPosZ>__GAME_SIZE_Y__-m_fMaxSizeHeight)
	{		
		m_fPosZ		= __GAME_SIZE_Y__-m_fMaxSizeHeight;
		m_fPrevPosZ	= __GAME_SIZE_Y__-m_fMaxSizeHeight;
	}
	else
	{
		m_fPosZ		= fPosZ;
		m_fPrevPosZ = fPosZ;
	}		
}
	
WORD CMenu::GetStartOrder()
{
	return __ORDER_INTERFACE_START__ + m_byOrder*10;
}

void CMenu::SetMaxSize(float fWidth, float fHeight)
{
	m_fMaxSizeWidth		= fWidth;
	m_fMaxSizeHeight	= fHeight;
}

int	CMenu::GetPosX()
{
	return (int)m_fPosX;
}

int	CMenu::GetPosY()
{
	return (int)m_fPosZ;
}

BOOL CMenu::GetClick()
{
	return m_bClick;
}

void CMenu::SetClick(BOOL bClick)
{
	m_bClick = bClick;
}

BOOL CMenu::GetActive()
{
	return m_bActive;
}

void CMenu::GetMouseIcon(BOOL bMouseIcon)
{
	m_bMouseIcon = bMouseIcon;
}

BOOL CMenu::GetMouseIcon()
{
	return m_bMouseIcon;
}

void CMenu::SetUp(BOOL bUp)
{
	m_bUp = bUp;
}

void CMenu::SetPosX(float fPosX)
{
	m_fPosX		= fPosX;
	m_fPrevPosX	= m_fPosX;
}

void CMenu::SetPosY(float fPosY)
{
	m_fPosZ		= fPosY;
	m_fPrevPosZ	= m_fPosZ;
}

int	CMenu::GetOrderIndex()
{
	return m_byOrder;
}

void CMenu::SetOrderIndex(BYTE byOrder)
{	
	m_byOrder = byOrder;
}

void CMenu::CreateMenu(char* pszName, float fWidth, float fHeight, BYTE byType, int nIndex, BOOL bActive, BOOL bScroll)
{
	m_bFirst	= bActive;
	m_byType	= byType;
	m_bScroll	= bScroll;
	m_nIndex	= nIndex;
	memset(m_szMenu, 0, sizeof(m_szMenu));
	lstrcpy(m_szMenu, pszName);
	SetMaxSize(fWidth, fHeight);	
}

void CMenu::InitValue(int nRt)
{	
	if(nRt>=1)
		m_bMouseIcon = TRUE;

	if(!g_Mouse.bLDown)
	{
		m_bUpChk	= FALSE;
		m_bDownChk	= FALSE;
		m_bScChk[0]	= FALSE;
		m_bScChk[1]	= FALSE;
	}
}

BOOL CMenu::SetMoveWnd(int nRt, int nValue)
{
	if(g_Mouse.bLDown)
	{
		if(m_fPrevPosX != m_fPosX || m_fPrevPosZ != m_fPosZ)
		{
			m_fPosX		+= g_Mouse.MousePos.x - m_fPrevPosX;
			m_fPosZ		+= g_Mouse.MousePos.y - m_fPrevPosZ;
			m_fPrevPosX = (float)g_Mouse.MousePos.x;
			m_fPrevPosZ = (float)g_Mouse.MousePos.y;

			if(m_fPosX<0)
				m_fPosX = 0;
			else if(m_fPosX>__GAME_SIZE_X__-m_fMaxSizeWidth)
				m_fPosX = __GAME_SIZE_X__-m_fMaxSizeWidth;
			if(m_fPosZ<0)
				m_fPosZ = 0;
			else if(m_fPosZ>__GAME_SIZE_Y__-m_fMaxSizeHeight+nValue)
				m_fPosZ = __GAME_SIZE_Y__-m_fMaxSizeHeight+nValue;
						
			m_bUpChk = TRUE;
			return TRUE;
		}
		
		if(nRt>=1)	
		{
			m_bClick	= TRUE;
			m_bDown		= TRUE;
			m_bUpChk	= TRUE;
			return TRUE;
		}
		
		if(g_pMainPlayer)
		{
			if(g_ItemMoveManager.GetNewItemMoveMode())
			{

			}
			else
			{
				if(g_pMainPlayer->m_MouseItem.GetID()==0)
				{
					CUserInterface::GetInstance()->SetPointer(__MOUSE_POINTER_DEFAULTCLICK__);
				}
			}		
		}		
	}
	
	return FALSE;
}

BOOL CMenu::IsReturn(int nRt, MOUSECHECK eMouseCheck)
{
	if(nRt==1)
	{
		if(eMouseCheck==MOUSE_DOWN || eMouseCheck==MOUSE_UP)
			return TRUE;
	}
	
	switch(m_rtType[nRt])
	{
	case CHECK_CLOSE:
	case CHECK_BTN:
	case CHECK_PUSHBTN:
		{			
			if(eMouseCheck==MOUSE_DOWN || eMouseCheck==MOUSE_UP)
				return TRUE;		
		}
		break;	
	}	
	return FALSE;
}

BOOL CMenu::MouseUp()
{
	if(m_bUp)
	{
		m_byScrollChk	= 1;
		m_fPrevPosX		= m_fPosX;
		m_fPrevPosZ		= m_fPosZ;		
		m_bClose		= FALSE;
		m_bMoveChk		= FALSE;
		m_bSoundChk		= FALSE;
		return TRUE;
	}
	return FALSE;
}

BOOL CMenu::MouseChk(int nRt)
{
	if(m_bUp && m_bDown && nRt>0)
	{	
		m_bUp	= FALSE;
		m_bDown = FALSE;
		return TRUE;
	}
	return FALSE;
}

BOOL CMenu::MouseScrollChk(int nRt, int nScrollNum)
{
	if(m_bScChk[0] && nRt!=nScrollNum)
	{
		if(g_Mouse.bLDown)
		{
			if(m_byScrollChk==1 || m_byScrollChk==2)
			{
				float fMousePos = (float)g_Mouse.MousePos.y;

				if(fMousePos<m_fPosZ+m_fStartSize)
					fMousePos = m_fPosZ+m_fStartSize;
				else if(fMousePos>=m_fPosZ+m_fEndSize)
					fMousePos = m_fPosZ+m_fEndSize-1;			
				
				float fPos = fMousePos-(m_fPosZ+m_fStartSize);
			
				if(m_nScrollPos>0)
				{				
					m_nCurScrollPos	= (int)(fPos / (float)(m_fScrollSize/(float)(m_nScrollPos+1)));
					fPos = (float)(fMousePos-8);
										
					if(fPos>m_fPosZ+m_fEndSize-16)
						fPos = m_fPosZ+m_fEndSize-16;
					else if(fPos<m_fPosZ+m_fStartSize)
						fPos = m_fPosZ+m_fStartSize;

					m_fSCTemp		= fPos - (m_fPosZ+m_fStartSize);
					m_fScrollPos	= fPos;
				}
				m_byScrollChk = 2;
			}
		}

		m_bClick	= TRUE;
		m_bDown		= TRUE;
		m_bUpChk	= TRUE;

		if(m_nScrollPos>0)
			return TRUE;
		else
			return FALSE;
	}
	return FALSE;
}

void CMenu::SetMouseCheck(int nRt, MOUSECHECK eMouseCheck, BYTE byChk, BOOL bEx)
{
	BYTE bType = 0xff;

	switch(m_rtType[nRt])
	{
	case CHECK_NONE:		
		bType = MOUSE_TYPE_DEFAULT;
		break;
	case CHECK_MOVEWND:
		bType = MOUSE_TYPE_DEFAULT;
		break;
	case CHECK_CLOSE:
	case CHECK_BTN:
	case CHECK_PUSHBTN:
	case CHECK_SCROLL:	
		bType = MOUSE_TYPE_BTN;
		break;	
	case CHECK_ITEM:
		bType = MOUSE_TYPE_ITEM;
		break;
	case CHECK_ATTACK:
		bType = MOUSE_TYPE_ATTACK;
		break;
	case CHECK_NPC:
		bType = MOUSE_TYPE_NPC;
		break;
	case CHECK_GUARDIAN:
		bType = MOUSE_TYPE_GUARDIAN;
		break;
	}

	if(nRt==1)
		bType = MOUSE_TYPE_DEFAULT;

	if(eMouseCheck==MOUSE_DOWN)
	{
		if(m_rtType[nRt]==CHECK_CLOSE)
		{
			m_bClose	= TRUE;
			m_bClick	= TRUE;
			m_bDown		= TRUE;
			m_bUpChk	= TRUE;			
			m_bDownChk	= TRUE;							
		}
		else if(m_rtType[nRt]==CHECK_MOVEWND)			
		{
			if(g_pMainPlayer)
			{
				if(g_pMainPlayer->m_MouseItem.GetID()==0)
				{
					if(m_fPrevPosX==m_fPosX && m_fPrevPosZ==m_fPosZ)
					{
						m_fPrevPosX	= (float)g_Mouse.MousePos.x;
						m_fPrevPosZ	= (float)g_Mouse.MousePos.y;
						m_bClick	= TRUE;
						m_bDown		= TRUE;
						m_bUpChk	= TRUE;
					}
				}
			}			
		}
		else if(m_rtType[nRt]==CHECK_BTN || m_rtType[nRt]==CHECK_PUSHBTN || m_rtType[nRt]==CHECK_ITEM)
		{
			m_bClick	= TRUE;
			m_bDown		= TRUE;
			m_bUpChk	= TRUE;		

			if(m_rtType[nRt]==CHECK_ITEM && m_bScCheckInterface)
			{
				if(m_byScrollChk==1 && !bEx)
					m_byScrollChk = 0;
			}
		}
		else if(m_rtType[nRt]==CHECK_SCROLL)
		{
			if(g_pMainPlayer)
			{
				if(g_pMainPlayer->m_MouseItem.GetID()==0)
				{
					if(!bEx)
					{
						m_bValueScrollChk = FALSE;

						if(m_byScrollChk==1 || m_byScrollChk==2)
						{
							float fPos = g_Mouse.MousePos.y-(m_fPosZ+m_fStartSize);
						
							if(m_nScrollPos>0)
							{				
								m_nCurScrollPos	= (int)(fPos / (float)(m_fScrollSize/(float)(m_nScrollPos+1)));
								
								if(m_nCurScrollPos==m_byMaxScroll+1)
									m_nCurScrollPos = m_byMaxScroll;

								fPos = (float)(g_Mouse.MousePos.y-8);
								
								if(fPos>m_fPosZ+m_fEndSize-16)
									fPos = m_fPosZ+m_fEndSize-16;
								else if(fPos<m_fPosZ+m_fStartSize)
									fPos = m_fPosZ+m_fStartSize;

								m_fSCTemp			= fPos - (m_fPosZ+m_fStartSize);
								m_fScrollPos		= fPos;
								m_bValueScrollChk	= TRUE;
							}						
							m_bScChk[0]		= TRUE;				
							m_bScChk[1]		= TRUE;				
							m_bClick		= TRUE;
							m_bDown			= TRUE;
							m_bUpChk		= TRUE;
							m_byScrollChk	= 2;
						}
					}
				}			
			}
		}
	}
	else if(eMouseCheck==MOUSE_UP)
	{	
		if(m_rtType[nRt]==CHECK_CLOSE)
		{
			m_bClose = FALSE;			
		}
	}
	
	if(byChk!=0)
	{
		CUserInterface::GetInstance()->SetPointer(byChk);
		return;	
	}

	if(eMouseCheck==MOUSE_OVER)
	{
		if(bType==MOUSE_TYPE_DEFAULT)
			CUserInterface::GetInstance()->SetPointer(__MOUSE_POINTER_DEFAULT__);
		else if(bType==MOUSE_TYPE_BTN)
			CUserInterface::GetInstance()->SetPointer(__MOUSE_POINTER_BUTTON__);
		else if(bType==MOUSE_TYPE_ITEM)
			CUserInterface::GetInstance()->SetPointer(__MOUSE_POINTER_ITEM__);
		else if(bType==MOUSE_TYPE_ATTACK)
			CUserInterface::GetInstance()->SetPointer(__MOUSE_POINTER_ATTACK__);
		else if(bType==MOUSE_TYPE_NPC)
			CUserInterface::GetInstance()->SetPointer(__MOUSE_POINTER_NPC__);
		else if(bType==MOUSE_TYPE_GUARDIAN)
			CUserInterface::GetInstance()->SetPointer(__MOUSE_POINTER_GUARDIAN__);
	}
	else if(eMouseCheck==MOUSE_DOWN)
	{
		if(bType==MOUSE_TYPE_DEFAULT)
			CUserInterface::GetInstance()->SetPointer(__MOUSE_POINTER_DEFAULTCLICK__);
		else if(bType==MOUSE_TYPE_BTN)
			CUserInterface::GetInstance()->SetPointer(__MOUSE_POINTER_BUTTONCLICK__);
		else if(bType==MOUSE_TYPE_ITEM)
			CUserInterface::GetInstance()->SetPointer(__MOUSE_POINTER_ITEMCLICK__);
		else if(bType==MOUSE_TYPE_ATTACK)
			CUserInterface::GetInstance()->SetPointer(__MOUSE_POINTER_ATTACKCLICK__);
		else if(bType==MOUSE_TYPE_NPC)
			CUserInterface::GetInstance()->SetPointer(__MOUSE_POINTER_NPCCLICK__);
		else if(bType==MOUSE_TYPE_GUARDIAN)
			CUserInterface::GetInstance()->SetPointer(__MOUSE_POINTER_GUARDIANCLICK__);						
	}
	else if(eMouseCheck==MOUSE_UP)
	{
		if(bType==MOUSE_TYPE_DEFAULT)
			CUserInterface::GetInstance()->SetPointer(__MOUSE_POINTER_DEFAULT__);
		else if(bType==MOUSE_TYPE_BTN)
			CUserInterface::GetInstance()->SetPointer(__MOUSE_POINTER_BUTTON__);
		else if(bType==MOUSE_TYPE_ITEM)
			CUserInterface::GetInstance()->SetPointer(__MOUSE_POINTER_ITEM__);
		else if(bType==MOUSE_TYPE_ATTACK)
			CUserInterface::GetInstance()->SetPointer(__MOUSE_POINTER_ATTACK__);
		else if(bType==MOUSE_TYPE_NPC)
			CUserInterface::GetInstance()->SetPointer(__MOUSE_POINTER_NPC__);
		else if(bType==MOUSE_TYPE_GUARDIAN)
			CUserInterface::GetInstance()->SetPointer(__MOUSE_POINTER_GUARDIAN__);
	}
}

BOOL CMenu::GetCheckScroll()
{
	return m_bValueScrollChk;
}

void CMenu::SetScroll(BYTE byMaxScroll, float fStartSize, float fEndSize)
{	
	m_byMaxScroll	= byMaxScroll;
	m_fStartSize	= fStartSize;
	m_fEndSize		= fEndSize;
	m_fScrollSize	= fEndSize-fStartSize;
}

MOUSECHECK CMenu::GetMouseCheck(int nRt)
{
	MOUSECHECK eMouseCheck = MOUSE_NONE;

	switch(m_rtType[nRt])
	{	
	case CHECK_NONE:
		eMouseCheck = MOUSE_NONE;
		break;
	case CHECK_CLOSE:
		{
			if(m_bUp && m_bDown && !m_bUpChk && m_bClose)
				eMouseCheck = MOUSE_UP;
			else if(g_Mouse.bLDown && !m_bUpChk)
				eMouseCheck = MOUSE_DOWN;
			else if(!g_Mouse.bLDown && !m_bUpChk)
				eMouseCheck = MOUSE_OVER;
		}
		break;
	case CHECK_ITEM:
	case CHECK_BTN:
		{
			if(m_bUp && m_bDown && !m_bUpChk)
				eMouseCheck = MOUSE_UP;
			else if(g_Mouse.bLDown && !m_bUpChk)
				eMouseCheck = MOUSE_DOWN;
			else if(!g_Mouse.bLDown && !m_bUpChk)
				eMouseCheck = MOUSE_OVER;
		}
		break;
	case CHECK_PUSHBTN:
		{
		}
		break;
	case CHECK_MOVEWND:
		{
			if(m_bScroll)
			{
				if(g_Mouse.bLDown && !m_bDownChk && !m_bScChk[0] && !m_bUpChk && !m_bScChk[1] )
					eMouseCheck = MOUSE_DOWN;
				else if(!g_Mouse.bLDown && !m_bUpChk)
					eMouseCheck = MOUSE_OVER;
			}
			else
			{
				if(g_Mouse.bLDown && !m_bDownChk && !m_bUpChk)
					eMouseCheck = MOUSE_DOWN;
				else if(!g_Mouse.bLDown && !m_bUpChk)
					eMouseCheck = MOUSE_OVER;
			}			
		}
		break;
	case CHECK_SCROLL:
		{
			if(g_Mouse.bLDown && !m_bMoveChk)
				eMouseCheck = MOUSE_DOWN;
			else if(!g_Mouse.bLDown && !m_bUpChk)
				eMouseCheck = MOUSE_OVER;
		}
		break;
	}

	if(nRt==1)
	{
		if(m_bUp && m_bDown && !m_bUpChk)
			eMouseCheck = MOUSE_UP;
		else if(g_Mouse.bLDown && !m_bUpChk)
			eMouseCheck = MOUSE_DOWN;
		else if(!g_Mouse.bLDown && !m_bUpChk)
			eMouseCheck = MOUSE_OVER;
	}
	return eMouseCheck;	
}

void CMenu::InterfaceTextRender(WORD wID, DWORD dwColor)
{
	LP_HELP_INFO lpHelpInfo = (LP_HELP_INFO)g_pHelpInfoHash->GetData(wID);
	int nSize = 0;	
	if(lpHelpInfo)
	{
		
		nSize = lstrlen(lpHelpInfo->szText);
		if(nSize==0)
			return;

		RenderFont(lpHelpInfo->szText, 
			m_fPosX+lpHelpInfo->wLeft, 
			m_fPosX+lpHelpInfo->wLeft+(float)nSize*6.5f, 
			m_fPosZ+lpHelpInfo->wTop, 
			m_fPosZ+lpHelpInfo->wTop+15, GetStartOrder()+1, dwColor);
	}	
}
//==================================================//
// End.												//
//==================================================//