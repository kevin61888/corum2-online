#ifndef _CRESNAMERESOLVER_H
#define _CRESNAMERESOLVER_H


#include "stdafx.h"


// ================================================================================================
// Definitions
// ================================================================================================
const DWORD THISAPPLICATION_ID		=	0x000EF010;
const DWORD	THISAPPLICATION_VERINFO	=	0x00000001;
const DWORD MAX_PATH_LENGTH			=	256;

#define MAX_PATH_DEPTH			64
#define MAX_PATH_LENGTH			256
#define MAX_DRIVE_LENGTH		16
#define MAX_FILENAME_LENGTH		128
#define MAX_EXT_LENGTH			32

// ================================================================================================
// Type defines
// ================================================================================================
typedef struct tagSplittedResUID
{
	unsigned		uUniqID	:	24;
	unsigned		uTypeID	:	8;
} SPLITRESUID, *LPSPLITRESUID;

typedef struct tagResUID
{
	union
	{
		DWORD			dwResUID;
		SPLITRESUID		sResUID;
	};
} RESUID, *LPRESUID;

#pragma pack(push, 1)
typedef struct tagResListRecord
{
	RESUID		sResUID;
	WORD		wResNameLength;
	WORD		wResFileNameWithDirLength;
} RESLISTRECORD, *LPRESLISTRECORD;
#pragma pack(pop)

typedef struct tagRes
{
	DWORD		m_dwUID;
	DWORD		m_dwLength;
} RESDATAFORFILE, *LPRESDATAFORFILE;

typedef struct tagResSearchData
{
	DWORD		m_dwUID;
	char*		lpFileName;
} RESDATAFORSEARCH, *LPRESDATAFORSEARCH;

enum RNRVRESULT
{
	RNRVRESULT_OK					=	0,
	RNRVRESULT_FAILED_TO_OPEN_FILE	=	1,
	RNRVRESULT_FILE_IS_EMPTY		=	2,
	RNRVRESULT_ILLEGAL_FILE_FORMAT	=	3,
	RNRVRESULT_BASEPATH_IS_TOO_LONG	=	4
};



// ================================================================================================
// Function Prototypes
// ================================================================================================
int GetAppPath( char*pszDest, int iLength );
int GetPathOnly( char* pszSrc, char* pszDest, int iLength );
int GetFileNameOnly( char* pszSrc, char* pszDest, int iLength );
int ConvertAbPathToRlPath( char* pszTargetBuf, int iLength, char* pszCurPath, char* pszTargetPath);
int ConvertRlPathToAbPath( char* pszTargetBuf, int iLength, char* pszCurPath, char* pszTargetPath);



// ================================================================================================
// class CResNameResolver
// ================================================================================================
class CResNameResolver
{
protected:
	DWORD					m_dwNumOfResources;

	LPRESDATAFORSEARCH		m_lpResDataForSearch;
	LPTSTR					m_lpNameString;

	char					m_szBasePath[MAX_PATH_LENGTH];

public:
	RNRVRESULT				Init( const char* szDataFileName, const char* szBasePath );
	void					UnInit();

	char*					GetResFileName( DWORD dwUID )		const;
	char*					GetResFileNameOnly( DWORD dwUID )	const;

	CResNameResolver()		{ memset( this, NULL, sizeof(CResNameResolver) ); }
	~CResNameResolver()		{};
};


#endif