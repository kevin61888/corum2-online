#include "stdafx.h"
#include "inputmanager.h"
#include "InitGame.h"
#include "GameControl.h"
#include "CharMoveEditWnd.h"
#include "CodeFun.h"
#include "QuantityWnd.h"

extern UPDATE_GAME_STATUS	g_bGameStatus;

CInputManager::CInputManager()
{
	m_bDrawCursor = FALSE; // added by deepdark.
	m_dwCurFocusInputID = 0;
	memset(m_InputBuffer, 0, sizeof(m_InputBuffer));	
}

CInputManager::~CInputManager()
{
	
}

void CInputManager::Initialize()
{
	SetFocusInput(INPUT_BUFFER_0);
	memset(m_InputBuffer, 0, sizeof(m_InputBuffer));
}

void CInputManager::InitializeInput(DWORD dwInputID, BYTE bFontID, RECT& rt, DWORD dwColor, int nOrder, BYTE bPassword, BOOL bChk)
{
	if(bChk)
		ClearInput(dwInputID);

	m_InputBuffer[dwInputID].bFontIndex	= bFontID;
	m_InputBuffer[dwInputID].Pos		= rt;
	m_InputBuffer[dwInputID].nOrder		= __ORDER_USERINTERFACE_START_+nOrder;
	m_InputBuffer[dwInputID].dwColor	= dwColor;
	m_InputBuffer[dwInputID].bPassword  = bPassword;

	if((m_InputBuffer[dwInputID].Pos.left==0 && m_InputBuffer[dwInputID].Pos.right==0) || (m_InputBuffer[dwInputID].Pos.top==0 && m_InputBuffer[dwInputID].Pos.bottom==0))
	{
		// asm_int3();
	}
}

DWORD CInputManager::GetCurFocusInputID()
{
	return m_dwCurFocusInputID;
}

void CInputManager::InsertCharacter(DWORD dwInputID, char* szBuf, DWORD dwLen)
{
	if (m_InputBuffer[ dwInputID ].dwInputIndex+dwLen < MAX_INPUT_BUFFER_SIZE)
	{
		memcpy(m_InputBuffer[ dwInputID ].szBuffer + m_InputBuffer[ dwInputID ].dwInputIndex, szBuf, dwLen);
		m_InputBuffer[ dwInputID ].dwInputIndex+=dwLen;
	}	
}

void CInputManager::SetInput(DWORD dwInputID, char* pszBuffer)
{
	memset(m_InputBuffer[ dwInputID ].szBuffer, 0, sizeof(m_InputBuffer[ dwInputID ].szBuffer));		
	lstrcpy(m_InputBuffer[ dwInputID ].szBuffer, pszBuffer);
	m_InputBuffer[ dwInputID ].dwInputIndex	= lstrlen(m_InputBuffer[ dwInputID ].szBuffer);
}

void CInputManager::SetCharacter(DWORD dwInputID, DWORD dwLen)
{			
	memset(&m_InputBuffer[ dwInputID ].szBuffer[m_InputBuffer[ dwInputID ].dwInputIndex], 0, dwLen);		
}

DWORD CInputManager::GetInputBufferLength(DWORD dwInputID)
{
	return m_InputBuffer[ dwInputID ].dwInputIndex;
}

void CInputManager::RenderInputBufferAll()
{
	for(int i = 0; i < MAX_INPUT_BUFFER_NUM; ++i)
	{
		RenderInputBuffer(i);
		
	}
}

void CInputManager::RenderInputBuffer(DWORD dwInputID)
{	
	if (lstrlen(m_InputBuffer[dwInputID].szBuffer))
	{
		if(m_InputBuffer[dwInputID].bPassword)
		{
			RenderConvertToSpecialCharacter(dwInputID, '*');
		}
		else
		{
			if((m_InputBuffer[dwInputID].Pos.left==0 && m_InputBuffer[dwInputID].Pos.right==0) || (m_InputBuffer[dwInputID].Pos.top==0 && m_InputBuffer[dwInputID].Pos.bottom==0))
			{
#ifdef	DEVELOP_MODE 
				return;
#endif			
			}
			else
			{	
				if(GetGameStatus()==UPDATE_GAME_CHAR_SELECT)
				{
					CCharMoveEditWnd* pCharMoveEditWnd = CCharMoveEditWnd::GetInstance();

					if(dwInputID==1 && pCharMoveEditWnd->GetActive()==TRUE)
						return;
				}
				RenderFont(m_InputBuffer[dwInputID].szBuffer, 
					(float)m_InputBuffer[dwInputID].Pos.left, 
					(float)m_InputBuffer[dwInputID].Pos.right, 
					(float)m_InputBuffer[dwInputID].Pos.top, 
					(float)m_InputBuffer[dwInputID].Pos.bottom, 
					m_InputBuffer[dwInputID].nOrder, 
					m_InputBuffer[dwInputID].dwColor);				
			}
		}
	}
}

void CInputManager::BlinkInputCaret()
{
	DWORD dwIndex = m_InputBuffer[ m_dwCurFocusInputID ].dwInputIndex;
	char* pBuffer = m_InputBuffer[ m_dwCurFocusInputID ].szBuffer;

	BYTE bLeadByte = BYTE(IsDBCSLeadByte(pBuffer[ dwIndex ]  ) + 1);
	
	if(bLeadByte == 2)
		dwIndex += 2;

	m_bDrawCursor = !m_bDrawCursor; // added by deepdark

#ifndef _USE_IME
	
	if(pBuffer[ dwIndex ] == '|')
		pBuffer[ dwIndex ] = '\0';
	else
		pBuffer[ dwIndex ] = '|';
	
#else

	CQuantityWnd* pQuantityWnd = CQuantityWnd::GetInstance();

	if(pQuantityWnd)
	{
		if(pQuantityWnd->GetActive())
		{
			if(pBuffer[ dwIndex ] == '|')
				pBuffer[ dwIndex ] = '\0';
			else
				pBuffer[ dwIndex ] = '|';
		}
	}

	if(g_bGameStatus==UPDATE_GAME_LOGIN)
	{
		if(pBuffer[ dwIndex ] == '|')
			pBuffer[ dwIndex ] = '\0';
		else
			pBuffer[ dwIndex ] = '|';
	}

	/*if(m_dwCurFocusInputID==INPUT_BUFFER_0 || m_dwCurFocusInputID==INPUT_BUFFER_5 || m_dwCurFocusInputID==INPUT_BUFFER_CHARACTER_JUMIN)
		return;

	if(pBuffer[ dwIndex ] == '|')
		pBuffer[ dwIndex ] = '\0';
	else
		pBuffer[ dwIndex ] = '|';*/
	
#endif
		
}

void CInputManager::SetFocusInput(DWORD dwInputID)
{	
	::KillTimer(g_hMainWnd, 1);
	if (dwInputID)
		::SetTimer(g_hMainWnd, 1, 500, NULL);
	
	GetInputBuffer(m_dwCurFocusInputID);
	m_dwCurFocusInputID = dwInputID;
}

void CInputManager::ClearInput(DWORD dwInputID)
{
	memset(m_InputBuffer[ dwInputID ].szBuffer, 0, sizeof(m_InputBuffer[ dwInputID ].szBuffer));
	m_InputBuffer[ dwInputID ].dwInputIndex = 0;
}

void CInputManager::SetInputColor(DWORD dwInputID, DWORD dwColor)
{
	m_InputBuffer[ dwInputID ].dwColor = dwColor;
}

void CInputManager::RenderConvertToSpecialCharacter(DWORD dwInputID, char c)
{
	char szBuffer[MAX_INPUT_BUFFER_SIZE];
	memset(szBuffer, 0, MAX_INPUT_BUFFER_SIZE);

	int nLen = lstrlen(m_InputBuffer[dwInputID].szBuffer) - 1;
	if (nLen < 0)	return;

	if(m_InputBuffer[dwInputID].szBuffer[nLen] == '|')
	{
		memset(szBuffer, c, nLen+1);
		szBuffer[ nLen ] = '|';
	}
	else
		memset(szBuffer, c, nLen+1);

	RenderFont(szBuffer, 
		(float)m_InputBuffer[dwInputID].Pos.left, 
		(float)m_InputBuffer[dwInputID].Pos.right, 
		(float)m_InputBuffer[dwInputID].Pos.top, 
		(float)m_InputBuffer[dwInputID].Pos.bottom, 
		m_InputBuffer[dwInputID].nOrder, 
		m_InputBuffer[dwInputID].dwColor);
}

char* CInputManager::GetInputBuffer(DWORD dwInputID, BOOL bChk)
{
	if(*m_InputBuffer[dwInputID].GetCurPos() == '|')
	{
		DWORD dwIndex = m_InputBuffer[ dwInputID ].dwInputIndex;
		char* pBuffer = m_InputBuffer[ dwInputID ].szBuffer;
		
		BYTE bLeadByte = BYTE(IsDBCSLeadByte(pBuffer[ dwIndex ]  ) + 1);
				
		if(bLeadByte == 2)
			dwIndex += 2;	

		if(!bChk)
			m_InputBuffer[dwInputID].szBuffer[dwIndex] = '\0';
	}

	return m_InputBuffer[dwInputID].szBuffer;
}

BOOL CInputManager::PtInRect(DWORD dwInputID, long x, long y)
{
	return m_InputBuffer[dwInputID].PtInRect(x, y);
}

void CInputManager::BackSpaceEvent()
{
	INPUT_BUFFER* pBuffer = &m_InputBuffer[ m_dwCurFocusInputID ];
	if(pBuffer->dwInputIndex == 0)		return;
	
	GetInputBuffer(m_dwCurFocusInputID);

	BOOL bParsing = FALSE;
	if(pBuffer->szBuffer[pBuffer->dwInputIndex-1] == '\n')
		bParsing = TRUE;

	BYTE bLeadByte = BYTE(IsDBCSLeadByte(pBuffer->szBuffer[ pBuffer->dwInputIndex - 1 ]) + 1);
	memset(pBuffer->szBuffer + pBuffer->dwInputIndex - bLeadByte, 0, bLeadByte );

	pBuffer->dwInputIndex -= bLeadByte;
	if (bParsing)
	{
		pBuffer->bLine--;
		BackSpaceEvent();
	}	
}

int CInputManager::GetInputOrder(DWORD dwInputID)
{
	return m_InputBuffer[ dwInputID ].nOrder;
}

RECT&	CInputManager::GetInputRect(DWORD dwInputID)
{
	return m_InputBuffer[ dwInputID ].Pos;
}

void CInputManager::SetInputOrder(DWORD dwInputID, int nOrder)
{
	m_InputBuffer[ dwInputID ].nOrder = nOrder;
}

// added by deepdark
BOOL CInputManager::IsDrawCursor()
{
	return m_bDrawCursor;
}