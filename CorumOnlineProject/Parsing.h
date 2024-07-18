#ifndef _PARSING_H_
#define _PARSING_H_

#include <ASSERT.H>
#include <windows.h>
#include <STRING.H>

#define IN
#define OUT

char * FindBeforeTokenStrings(char * IN pszSrc, 
						char ** IN ppszSeps, 
						int iTokenStringCount);

int OneCharPerStrLen(const char * IN pszSrc);

BOOL IsThereString(const char * IN pszSrc, 
				   const char * IN pszDest);

char * GetToken(char * IN pszSrc, char * OUT pszGetToken, char cToken = ' ');

extern char g_szToken[0xff];

#endif
// _PARSING_H_