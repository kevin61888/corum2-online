#pragma once

#include <winsock2.h>
#include <windows.h>
#include <vector>


#define READ_DATA(FileClass, Column, Argument)		{ if (FALSE == ((CScriptFile)#FileClass).ReadData(Column, Argument)) { break; } }
#define READ_STRING(FileClass, Column, Argument)	{ if (FALSE == ((CScriptFile)#FileClass).ReadString(Column, Argument, CScriptFile::MAX_COLUMN_LENGHT)) { break; } }


typedef class CScriptFile*		LPCScriptFile;
class CScriptFile
{
public:
	enum { 
		MAX_LINE_LENGTH		= 8192,
		MAX_DELIMITER_NUM	= 32,
		MAX_COLUMN_LENGHT	= 64,
		DEFAULT_COLUMN_NUM	= 32
	};


	// MAX Delimiter num is 32;
	CScriptFile(const char* pszDelimiter = "\t");
	~CScriptFile(void);


private:
	HANDLE	m_hFile;
	DWORD	m_dwColumn;
	DWORD	m_dwColumnCount;
	CHAR	m_szLine[MAX_LINE_LENGTH];
	CHAR	m_szBackupLine[MAX_LINE_LENGTH];

	char	m_szDelimiter[MAX_DELIMITER_NUM];

	typedef std::vector<char*>	ColumnArray;
    ColumnArray	m_ColumnNames;

private:
	int		FindColumn(const char* szField);
	BOOL	GotoColumn(int nColumn);

public:
	//BOOL	Open(LPCSTR szFilename, BOOL bHeadLine, UINT nOpenFlags = modeRead);
	BOOL	Open(LPCSTR szFilename, BOOL bHeadLine, DWORD dwDesiredAccess, DWORD dwCreationDisposition);
	int		ReadLine(void);
	int		ReadString(void);

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

	BOOL	ReadData(const char* szField, double& dNumber);
	BOOL	ReadData(const char* szField, float& fNumber);
	BOOL	ReadData(const char* szField, unsigned long& fNumber);
	BOOL	ReadData(const char* szField, int& iNumber);	
	BOOL	ReadData(const char* szField, unsigned short& iNumber);
	BOOL	ReadData(const char* szField, short& iNumber);
	BOOL	ReadData(const char* szField, unsigned char& iNumber);
	BOOL	ReadData(const char* szField, char& iNumber);
	BOOL	ReadData(const char* szField, __int64& i64Number);
	BOOL	ReadData(const char* szField, DWORD64& i64Number);
	BOOL	ReadString(const char* szField, char* szString, DWORD dwSize);

    void	WriteData(const char* pFormat, ...);

	void	Terminate(void);

	char*	GetLine(void)		{ return (char*)m_szLine; }

};
