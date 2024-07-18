/////////////////////////////////////
// "version.lib" 추가해 주세요.
/////////////////////////////////////

#pragma once
#include "../../stdafx.h"

int GetFileVersion( char* aszFileName, char* szVerBuff ) 
{
	char achData[4096];
	char achFileName[128];
	if( aszFileName == NULL )	GetModuleFileName( NULL, achFileName, 128 );
	else						wsprintf( achFileName, aszFileName );

	if( !GetFileVersionInfo( achFileName, NULL, 4096, achData ) )
	{
		wsprintf( szVerBuff, "0" );
		return 0;
	}

	LPVOID			lpBuffer;
	unsigned int	dwBytes;
	int				nVersion;

	VerQueryValue( achData,
				   TEXT("\\StringFileInfo\\041204B0\\FileVersion"),		// FileVersion
//				   TEXT("\\StringFileInfo\\041204B0\\ProductVersion"),	// ProductVersion
				   &lpBuffer, 
				   &dwBytes); 

	wsprintf( szVerBuff, (char*)lpBuffer );

	char* pTemp = strtok( (char *)lpBuffer, "," );
	nVersion = atoi(pTemp) * 100;

	pTemp = strtok(NULL, ",");
	nVersion += atoi(pTemp) * 10;

	pTemp = strtok(NULL, ",");
	nVersion += atoi(pTemp) * 1;

	return nVersion;
}
