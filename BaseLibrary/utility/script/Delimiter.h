#pragma once

#include <windows.h>


class CDelimiter
{
public:
	enum { 
		MAX_LINE_LENGTH		= 8192,
		MAX_DELIMITER_NUM	= 32,
	};

	// constructor and destructor
	CDelimiter(char* szLine, char* pszDelimiter = "\t");	// MAX Delimiter num is 32;
	~CDelimiter(void);


private:
	CHAR*	m_pHeadPtr;
	CHAR*	m_pCurrPtr;
	CHAR	m_szDelimiter[MAX_DELIMITER_NUM];


public:
	BOOL	ReadData(bool& bNumber);
	BOOL	ReadData(double& dNumber);
	BOOL	ReadData(float& fNumber);
	BOOL	ReadData(unsigned long &fNumber);
	BOOL	ReadData(int& iNumber);	
	BOOL	ReadData(unsigned short& iNumber);
	BOOL	ReadData(short& iNumber);
	BOOL	ReadData(unsigned char& iNumber);
	BOOL	ReadData(char& iNumber);
	BOOL	ReadData(__int64& i64Number);
	BOOL	ReadData(DWORD64& i64Number);
	BOOL	ReadString(char *szString, DWORD dwSize);
	BOOL	ReadString(char *szString, DWORD dwSize, OUT CHAR* pcToken);

	void	Terminate(void);

	char*	GetLine(void)		{ return m_pCurrPtr;	}
};
