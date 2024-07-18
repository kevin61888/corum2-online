
#ifndef	__FILTER_INC__
#define	__FILTER_INC__

#pragma once


#define MAX_EMOTICON 20

BOOL	ConvString(char* szStr);
BOOL	NoConvString(char* szStr);
int		EmoticonString(char* szStr);


//-------------------------------------------------------------------------------------

BOOL CheckString(LPSTR szString, BOOL bCheckLimited = FALSE);

#endif // __FILTER_INC__

