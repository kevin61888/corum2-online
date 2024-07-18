// TokenCommand.h: interface for the CTokenCommand class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOKENCOMMAND_H__2DAFD982_BDE2_4C1D_8D2C_3ADD147D1FB2__INCLUDED_)
#define AFX_TOKENCOMMAND_H__2DAFD982_BDE2_4C1D_8D2C_3ADD147D1FB2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "../CommonServer/GamePacket.h"
#include "../CommonServer/WorldPacket.h"
#include "StringFunc.h"
#include "Command.h"
#include "assert.h"


#if defined _DEBUG	
#	define DEBUG_LOG(string)	OutputDebugString(string); OutputDebugString("\n");
#else
#	define DEBUG_LOG(string)	
#endif

#define GET_TOKEN_COMMAND()		(CTokenCommand::GetInstance())


//
// CTokenCommand
//
class CTokenCommand  
{
public:
	enum { MAX_TOKEN_LENGTH = 1024 };

public:
	CTokenCommand();
	virtual ~CTokenCommand();

private:
	void	SendPacketGMCommand();
	int		ParseGMCommand();

	BOOL	IsValidCommand() const
	{
		return BOOL(m_bCorrectGrammarCommand && m_bIsCommand);
	}

	int		GetCommandMsgType() const;
	int		Get_GM_MsgType() const;

	//////////////////////////////////////////////////////////////////////////
	// static members
	static	void	SetCommandType(int iMsgType, int iCommandType);
	static	COMMAND_TOKEN_ENTRY 
			WhatKindOfGMMessage(char * pszString);
	//////////////////////////////////////////////////////////////////////////

public:
	void	Initialize();
	void	InputString(char * pszString);
	BOOL	Execute();

	BOOL	IsCommand()	const;

	static CTokenCommand *	GetInstance();


private:
	COMMAND_TOKEN_ENTRY	m_sCurrentTokenEntry;
	BOOL				m_bCorrectGrammarCommand;
	BOOL				m_bIsCommand;
	char				m_szTempString[MAX_TOKEN_LENGTH];

	static CTokenCommand * m_pTokenCommand;

	CTDS_NEW_GM_COMMAND	m_sDungeonGMCommand;
	CTWS_NEW_GM_COMMAND	m_sWorldGMCommand;

	DECLARE_COMMAND_TOKEN_ENTRY()
};

#endif // !defined(AFX_TOKENCOMMAND_H__2DAFD982_BDE2_4C1D_8D2C_3ADD147D1FB2__INCLUDED_)
