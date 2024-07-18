#pragma once

#include "stdafx.h"

class CDebugLog
{
private:
	FILE*		m_fpLogFile;

public:
	BOOL		InitDubugLog( char* szFileName );
	BOOL		Log( char* szLog );
	BOOL		Save();
	
public:
	CDebugLog();
	~CDebugLog();
};