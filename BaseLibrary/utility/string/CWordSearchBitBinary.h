//
// File: CWordSearchBitBinary.h
// Desc: Æ®¸®¸¦ ÀÌ¿ëÇÑ ¿å¼³ ÇÊÅÍ¸µ ¾Ë°í¸®Áò
//
// Auth: Chan++ (chan78@esofnet.com)
// Auth: Standil (cpascal@nownuri.net)
//

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <tchar.h>

#include <set>

#define CS_MASK				0x07
#define CS_RESTBIT_COUNT	5
#define CS_BIT_COUNT		3
#define CS_NODE_COUNT		(CS_MASK + 1)

class CWordSearchBitBinary
{
private:
	typedef struct WordNode	{
		WordNode() : IndexSet(), bTermination(FALSE) { for( int i=0; i < CS_NODE_COUNT; i++ ) pNext[i] = NULL; };

		BOOL    bTermination;
		struct  WordNode *pNext[CS_NODE_COUNT];
		std::set<DWORD> IndexSet;
	} WORDNODE, *LPWORDNODE;
	DWORD		dwNumOfWords;
	LPWORDNODE	pRoot;
	void		ReleaseWordNode( LPWORDNODE pWordNode );
	static DWORD s_CRC32TABLE[256];

public:
	BOOL		AddWord( LPCTSTR szWord );

	TCHAR *		ReplaceString( LPTSTR szString );
	BOOL		JustSearchString( LPCTSTR szString );

	CWordSearchBitBinary();
	~CWordSearchBitBinary();
};

// CWordSearchBtBinary CurseFilter;

// CurseFilter.AddWord( "¿å1" );
// CurseFilter.AddWord( "¿å2" );
// CurseFilter.AddWord( "¿å3" );
