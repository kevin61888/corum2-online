// ComboBox.cpp: implementation of the CComboBox class.
//
//////////////////////////////////////////////////////////////////////
#include "ComboBox.h"
#include "SpriteManager.h"
#include "GameControl.h"
#include "InitGame.h"
#include "CodeFun.h"

#define REMOVE_SPRITE(x) if(x) { g_pSprManager->RemoveSprite(x); x = NULL;}
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CComboBox::CComboBox()
{
	m_pComboList = new CComboList;
	m_pComboDisplayFirst = NULL;
	m_pComboCurSel		= NULL;
	m_bScrollClick = 0;
	m_pSprSelectBar = NULL;
	m_bUpChk = false;
	
	
	m_dwComboListMaxDisplay = 0;
	
	m_dwScrollRecoverY = 0;		
	m_pSprParent = NULL;
	
	m_pSprBoard = NULL;
	m_pSprScrollButton = NULL;
	m_pSprButton[0] = NULL;
	m_pSprButton[1] = NULL;
}

CComboBox::~CComboBox()
{
	RemoveSprite();
	
	if (m_pComboList)
	{
		m_pComboList->RemoveAll();
		delete m_pComboList;
	}
	m_pComboList = NULL;
}

void CComboBox::AddTail(char *szName, void *pDesc)
{
	// 중복검사..
	POSITION_ pos = m_pComboList->GetHeadPosition();
	while(pos)
	{
		POSITION_ del = pos;
		COMBOBOX* pComboBox =  (COMBOBOX*)m_pComboList->GetNext(pos);
		if (!__strcmp(pComboBox->szComboName, szName))
		{
			if (m_pComboDisplayFirst == del)
				m_pComboDisplayFirst = pos;
			m_pComboList->RemoveAt(del);
			break;
			
		}
	}
	// 빈문자 삽입한것을 다시 지워줘야..
	if (GetCount() == 1)
	{
		COMBOBOX* pTemp = (COMBOBOX*)m_pComboList->GetTail();
		if(IsEmptyString(pTemp->szComboName))
		{
			RemoveHead();
		}
	}

	
	COMBOBOX* pComboBox = new COMBOBOX;
	pComboBox->pDesc = pDesc;
	memset(pComboBox->szComboName, 0, 30); // added by deepdark.
	__lstrcpyn(pComboBox->szComboName, szName, MAX_CHARACTER_NAME_REAL_LENGTH );
	m_pComboList->AddTail(pComboBox);
	pComboBox->pDesc = pDesc;
	
	pos = m_pComboList->GetHeadPosition();
	int nLine = 0;
	while(pos)
	{		
		COMBOBOX* pComboBox =  (COMBOBOX*)m_pComboList->GetNext(pos);
		pComboBox->dwIndex = nLine++;
	}
	if (!m_pComboDisplayFirst)
	{
		m_pComboDisplayFirst = m_pComboList->GetTailPosition();
	}
	m_pSprBoard->SetScaling((float)1, (float)1*min(m_dwComboListMaxDisplay, GetCount()));
	OnMove();
}
void CComboBox::RemoveHead()
{
	if (m_pComboDisplayFirst == m_pComboList->GetHeadPosition())
		m_pComboList->GetNext(m_pComboDisplayFirst);
	
	m_pComboList->RemoveHead();
	m_pSprBoard->SetScaling((float)1, (float)1*min(m_dwComboListMaxDisplay, GetCount()));

	OnMove();
}
int CComboBox::GetCount()
{
	return m_pComboList->GetCount();
}

void CComboBox::Initialize(V2_SPRITE *pSprParent, DWORD dwMaxDisplay)
{
	m_dwComboListMaxDisplay = dwMaxDisplay;
	m_pSprParent = pSprParent;
}

V2_SPRITE* CComboBox::CreateSpriteComboButtonNormal(DWORD dwSprID, float fRelativeX, float fRelativeY, int nStartX, int nStartY, int nWidth, int nHeight, bool bShow, int nOrder, BYTE bAlpha)
{
	m_pSprButton[0] = g_pSprManager->CreateSprite(dwSprID, m_pSprParent->vPos.x+fRelativeX, m_pSprParent->vPos.y+fRelativeY, nStartX, nStartY, nWidth, nHeight, bShow, nOrder, bAlpha);
	m_pSprButton[0]->vRelativePos.x = fRelativeX;
	m_pSprButton[0]->vRelativePos.y = fRelativeY;

	return m_pSprButton[0];
}

V2_SPRITE* CComboBox::CreateSpriteComboButtonDown(DWORD dwSprID, float fRelativeX, float fRelativeY, int nStartX, int nStartY, int nWidth, int nHeight, bool bShow, int nOrder, BYTE bAlpha)
{
	m_pSprButton[1] = g_pSprManager->CreateSprite(dwSprID, m_pSprParent->vPos.x+fRelativeX, m_pSprParent->vPos.y+fRelativeY, nStartX, nStartY, nWidth, nHeight, bShow, nOrder, bAlpha);
	m_pSprButton[1]->vRelativePos.x = fRelativeX;
	m_pSprButton[1]->vRelativePos.y = fRelativeY;

	return m_pSprButton[1];
}

V2_SPRITE* CComboBox::CreateSpriteComboScrollButton(DWORD dwSprID, float fRelativeX, float fRelativeY, int nStartX, int nStartY, int nWidth, int nHeight, bool bShow, int nOrder, BYTE bAlpha)
{
	
	m_pSprScrollButton = g_pSprManager->CreateSprite(dwSprID, m_pSprParent->vPos.x+fRelativeX, m_pSprParent->vPos.y+fRelativeY, nStartX, nStartY, nWidth, nHeight, bShow, nOrder, bAlpha);
	m_pSprScrollButton->vRelativePos.x = fRelativeX;
	m_pSprScrollButton->vRelativePos.y = fRelativeY;

	return m_pSprScrollButton;
}

V2_SPRITE* CComboBox::CreateSpriteComboBoard(DWORD dwSprID, float fRelativeX, float fRelativeY, int nStartX, int nStartY, int nWidth, int nHeight, bool bShow, int nOrder, BYTE bAlpha)
{
	m_pSprBoard = g_pSprManager->CreateSprite(dwSprID, m_pSprParent->vPos.x+fRelativeX, m_pSprParent->vPos.y+fRelativeY, nStartX, nStartY, nWidth, nHeight, bShow, nOrder, bAlpha);
	m_pSprBoard->vRelativePos.x = fRelativeX;
	m_pSprBoard->vRelativePos.y = fRelativeY;

	return m_pSprBoard;
}

V2_SPRITE* CComboBox::CreateSpriteComboSelectBar(DWORD dwSprID, float fRelativeX, float fRelativeY, int nStartX, int nStartY, int nWidth, int nHeight, bool bShow, int nOrder, BYTE bAlpha)
{
	m_pSprSelectBar = g_pSprManager->CreateSprite(dwSprID, m_pSprParent->vPos.x+fRelativeX, m_pSprParent->vPos.y+fRelativeY, nStartX, nStartY, nWidth, nHeight, bShow, nOrder, bAlpha);
	m_pSprSelectBar->vRelativePos.x = fRelativeX;
	m_pSprSelectBar->vRelativePos.y = fRelativeY;

	return m_pSprSelectBar;
}

void CComboBox::SetOrder()
{
	m_pSprButton[0]->nZOrder = m_pSprParent->nZOrder+1;
	m_pSprButton[1]->nZOrder = m_pSprParent->nZOrder+1;
	m_pSprBoard->nZOrder = m_pSprParent->nZOrder+2;
	m_pSprSelectBar->nZOrder = m_pSprParent->nZOrder+2;
	m_pSprScrollButton->nZOrder = m_pSprParent->nZOrder+3;
}


char* CComboBox::GetSelectedString()
{
	COMBOBOX* p = (COMBOBOX*)m_pComboList->GetAt(m_pComboCurSel);
	return p->szComboName;
}

void CComboBox::RenderList(IDIFontObject *pFont, int nOrder)
{		
	POSITION_ pos = m_pComboDisplayFirst;
	int nCount = 0;
	
	if (m_pSprBoard->bRenderFlag)
	{
		int nStartX = m_pSprBoard->vPos.x+2;
		int nStartY = m_pSprBoard->vPos.y+4;

		while(pos)
		{
			// 콤보박스 목록을 디스플레이.
			COMBOBOX* pComboBox = (COMBOBOX*)m_pComboList->GetNext(pos);		
			
			int nPosX = (int)nStartX;
			int nPosY = (int)nStartY+nCount*m_pSprBoard->dwHeight;
			
			RenderFont(pComboBox->szComboName, nPosX, nPosX+100, nPosY, nPosY+m_pSprBoard->dwHeight, nOrder+3);
			
			if(++nCount == m_dwComboListMaxDisplay)
				break;
		}
	}
}


int CComboBox::MouseEventCheck()
{
	if (!m_bUpChk)
	{
		m_pSprSelectBar->ShowSprite(FALSE);
		m_pSprButton[0]->ShowSprite(true);
		m_pSprButton[1]->ShowSprite(false);
	}
	if(!g_Mouse.bLDown)
	{
		m_bUpChk	= false;
		m_bScrollClick = false;
	}

	if (m_bScrollClick)
	{
		OnScroll();
	}
	int nCurSelIndex = 0;
	if(GetCount())
	{	
		if (m_pSprBoard->PtInSpr(g_Mouse.MousePos.x, g_Mouse.MousePos.y))
		{// 마우스 무브중에 목록과 충돌하면 셀렉트바의 위치 알맞게 처리.
			DWORD dwHeight = m_pSprBoard->dwHeight* min(GetCount(), m_dwComboListMaxDisplay) /min(GetCount(), m_dwComboListMaxDisplay);//1칸의 사이즈를 구한다.
			DWORD dwY = m_pSprBoard->vPos.y;
			nCurSelIndex = min((g_Mouse.MousePos.y-dwY)/dwHeight, min(GetCount(), m_dwComboListMaxDisplay)-1);
			
			// 스크롤버튼도 따라 움직인다!! by deepdark.
			m_pSprScrollButton->vPos.y = dwY+nCurSelIndex*dwHeight;

			m_pSprSelectBar->vPos.y = dwY+nCurSelIndex*dwHeight;
			m_pSprSelectBar->ShowSprite(true);
			
		}
	}
	
	if(g_Mouse.bLDown && !m_bUpChk)
	{// 클릭
		
		if (m_pSprScrollButton->PtInSpr(g_Mouse.MousePos.x, g_Mouse.MousePos.y))
		{// 스크롤 버튼을 선택했따.
			m_bScrollClick = true;
			m_bUpChk = true;
			return 1;
		}
		else if (m_pSprBoard->PtInSpr(g_Mouse.MousePos.x, g_Mouse.MousePos.y))
		{// 목록을 선택했따.
			
			POSITION_ pos = m_pComboDisplayFirst;
			int nCount = 0;
			while(pos)
			{
				if (nCount++ == nCurSelIndex)
				{
					m_pComboCurSel = pos;
					break;
				}
				m_pComboList->GetNext(pos);
			}
			m_pSprBoard->ShowSprite(false);
			m_pSprScrollButton->ShowSprite(false);
			m_pSprSelectBar->ShowSprite(false);
						
			m_bUpChk = true;
			return 2;
		}
		else if (m_pSprButton[0]->PtInSpr(g_Mouse.MousePos.x, g_Mouse.MousePos.y))
		{//콤보박스 버튼을 클릭
			m_pSprButton[0]->ShowSprite(false);
			m_pSprButton[1]->ShowSprite(true);
			
			// 빈칸 보이기 위해서.
			if (GetCount()==0)
				AddTail("");

			m_pSprBoard->ShowSprite(!m_pSprBoard->bRenderFlag);
			m_pSprScrollButton->ShowSprite(!m_pSprScrollButton->bRenderFlag);
						

			
			OnMove();
			m_bUpChk = true;
			return 1;
		}
	}
	
	return 0;
}



void CComboBox::OnScroll()
{
	if (!m_pComboDisplayFirst)	return;
	
	V2_SPRITE* pSpr = m_pSprScrollButton;
	// 스크롤바 제한 영역설정.
	pSpr->vPos.y = min(max( m_pSprBoard->vPos.y+pSpr->vRelativePos.y, g_Mouse.MousePos.y), m_pSprBoard->vPos.y+pSpr->vRelativePos.y+m_pSprBoard->dwHeight*(min(GetCount(), m_dwComboListMaxDisplay))-pSpr->dwHeight);
	// 스크롤바의 위치로 목록 인덱스 알아내기.
	DWORD dwIndex = (pSpr->vPos.y-(m_pSprBoard->vPos.y+pSpr->vRelativePos.y))/((min(GetCount(), m_dwComboListMaxDisplay)*m_pSprBoard->dwHeight)/GetCount());
	m_dwScrollRecoverY = pSpr->vPos.y-m_pSprBoard->vPos.y;
	COMBOBOX* p = (COMBOBOX*)m_pComboList->GetAt(m_pComboDisplayFirst);
	if (p->dwIndex < dwIndex)
	{// 스크롤바가 기존에 선택된 것 보다 down
		if (GetCount() - p->dwIndex > m_dwComboListMaxDisplay)
			m_pComboList->GetNext(m_pComboDisplayFirst);
		
	}
	else if (p->dwIndex > dwIndex)
	{// 스크롤바가 기존에 선택된 것 보다 up
		m_pComboList->GetPrev(m_pComboDisplayFirst);
	}
}




void CComboBox::OnMove()
{
	// 부모 윈도우가 움직였다면 같이 움직여줘야지.
	m_pSprButton[0]->vPos.x = m_pSprParent->vPos.x+m_pSprButton[0]->vRelativePos.x;
	m_pSprButton[0]->vPos.y = m_pSprParent->vPos.y+m_pSprButton[0]->vRelativePos.y;
	m_pSprButton[1]->vPos.x = m_pSprParent->vPos.x+m_pSprButton[1]->vRelativePos.x;
	m_pSprButton[1]->vPos.y = m_pSprParent->vPos.y+m_pSprButton[1]->vRelativePos.y;

	// 위로 펼쳐질때 필요하다.
	m_pSprBoard->vPos.y = m_pSprParent->vPos.y+m_pSprBoard->vRelativePos.y-m_pSprBoard->dwHeight*(min(m_dwComboListMaxDisplay, GetCount()));
	m_pSprBoard->vPos.x = m_pSprParent->vPos.x+m_pSprBoard->vRelativePos.x;
	//m_pSprBoard->vPos.y = m_pSprParent->vPos.y+m_pSprBoard->vRelativePos.y;

	m_pSprScrollButton->vPos.x = m_pSprParent->vPos.x+m_pSprScrollButton->vRelativePos.x;
	m_pSprScrollButton->vPos.y = m_pSprBoard->vPos.y+m_dwScrollRecoverY;

	m_pSprSelectBar->vPos.x = m_pSprParent->vPos.x+m_pSprSelectBar->vRelativePos.x;
	m_pSprSelectBar->vPos.y = m_pSprBoard->vPos.y;//+m_pSprSelectBar->vRelativePos.y;
	m_bUpChk	= true;
			
}

void CComboBox::SetActive(BOOL bActive)
{
	// 보여줄까 말까.
	if(bActive)
		m_pSprButton[0]->ShowSprite(TRUE);
	else
	{
		m_pSprBoard->ShowSprite(FALSE);
		m_pSprButton[0]->ShowSprite(FALSE);
		m_pSprButton[1]->ShowSprite(FALSE);
		m_pSprScrollButton->ShowSprite(FALSE);
		m_pSprSelectBar->ShowSprite(FALSE);

	}
}

void CComboBox::RemoveSprite()
{
	REMOVE_SPRITE(m_pSprSelectBar)
	REMOVE_SPRITE(m_pSprBoard)
	REMOVE_SPRITE(m_pSprScrollButton)
	REMOVE_SPRITE(m_pSprScrollButton)
	REMOVE_SPRITE(m_pSprButton[0])
	REMOVE_SPRITE(m_pSprButton[1])
}	
