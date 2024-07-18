#ifndef __TEXT_RESOURCE_MESSAGE_POOL_H__
#define __TEXT_RESOURCE_MESSAGE_POOL_H__

#pragma once

//---------------------------------------------------------------------------------
//
//		Coded by deepdark. 2004.01.16
//			the Text Resource Pool for Effective Memory Management
//
//---------------------------------------------------------------------------------

#define MAX_TEXT_LEN	255

#ifndef DECODE_KEY
#	define DECODE_KEY			"具昞秀天才,骨淫萬萬歲"
#endif

#ifndef DECODE_SUBKEY
#	define DECODE_SUBKEY		2
#endif

////////////////////////////////////////////////////////////////////////////////////////

#pragma  pack(push, 1)

// DB로부터 가져오는 버퍼 역할을 할 구조체.
// DB에 저장된 포맷 그대로.
typedef struct 
{
	DWORD	dwTextID;					// TEXT 리소스 ID
	char	szText[MAX_TEXT_LEN];		// TEXT 리소스 데이터.
} TEXT_RESOURCE, *LPTEXT_RESOURCE;
#pragma  pack(pop)


// 텍스트 리소스 파일의 헤더.
typedef struct 
{
	char	szSign[4];					// TEXT 리소스 Identifier
	DWORD	dwResourceCount;			// TEXT 리소스 갯수.
	DWORD	dwTextArraySize;			// TEXT DATA ARRAY SIZE
} TEXT_RESOURCE_HEADER, *LPTEXT_RESOURCE_HEADER;

// 텍스트 리소스 파일의 각 리소스 정보.
typedef struct 
{
	DWORD	dwTextID;					// 텍스트 리소스 ID
	DWORD	dwTextPos;					// 텍스트 리소스 위치(포인터 계산에 사용됨)
} TEXT_RESOURCE_INFO, *LPTEXT_RESOURCE_INFO;



////////////////////////////////////////////////////////////////////////////////////////
/*
	TEXT 리소스 파일의 구조(n개 텍스트 리소스).
	+--------+-------------------------+---------------------------------------------+
	| Header | Info[n] Array ...       | TextData Array  ...                         |
	+--------+-------------------------+---------------------------------------------+

	; 텍스트는 text data array에 연이어 저장된다.
*/

typedef struct __MESSAGEx
{
	DWORD	dwId;		// MessageID
	LPSTR	szMessage;	// Message!
} MESSAGEx, *LPMESSAGEx;


class CMessagePool
{
public:
	CMessagePool();
	~CMessagePool();

	BOOL	LoadTextResource(char* szSourceFile, char* szDecodeKey = DECODE_KEY, int nDecodeSubKey = DECODE_SUBKEY);
	MESSAGEx operator [] (DWORD dwIndex);
	LPSTR	GetMessage(DWORD dwIndex);
	DWORD	GetCount() { return m_dwTextResourceCount;	}

	DWORD	GetFirstMessageIdInString(LPCSTR szString);
	DWORD	GetLastMessageIdInString(LPCSTR szString);
	BOOL	IsThereAnyMessageInString(LPCSTR szString);
	LPCSTR	GetFirstMessageInString(LPCSTR szString);

	// for emoticon
	int		CompareRoutineEmoticon(LPCSTR szString);
	int		FilterEmoticonString(LPCSTR szString);

private:
	BOOL	VerifySourceFile(LPTEXT_RESOURCE_HEADER pHeader, DWORD dwFileSize);
	BOOL	DecodeData4Text(char* szLoadFile,  void* pReturnValue, char* szDecodeKey, int nDecodeSubKey);
	BOOL	AllocResourceInfo(DWORD dwCount, LPVOID pData = NULL);
	BOOL	AllocResourceStringBuffer(DWORD dwSize, LPVOID pData = NULL);
	void	ClearPool();

	BOOL	CompareRoutine(LPCSTR szString, LPCSTR szConv);

	DWORD					m_dwTextResourceCount;
	LPTEXT_RESOURCE_INFO	m_pTextResourceInfo;
	LPBYTE					m_pTextResourceString;
};

#endif // __TEXT_RESOURCE_MESSAGE_POOL_H__