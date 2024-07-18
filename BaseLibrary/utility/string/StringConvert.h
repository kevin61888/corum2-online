/* ==========================================================================
  File : StringConvert.h
  Vers.: 0.5
  Plat.: Windows 98 or above
  Desc.: String conversion functions
  Auth.: DongHo Byun (cpascal@nownuri.net)

  Adapted from VC++ 6.0 CRT's source
========================================================================== */

#pragma once

#ifdef  __cplusplus
extern "C" {
#endif

#define ICVT_BUFFER_SIZE	256

inline int AdvAtoi( char *numstr )
{
	int ret_num = 0;
	int multiplier = 0;

	if( numstr == NULL || !*numstr )
		return 0;
	for( ; isspace( *numstr ); numstr++ );
	if( !isdigit( *numstr ) )
		return 0;
	while( isdigit( *numstr ) )
		ret_num = (ret_num * 10) + (*numstr++ - '0');
	switch( *numstr ) {
		case 'k'  :
		case 'K'  :
			ret_num *= (multiplier = 1000);
			numstr++;
			break;
		case 'm'  :
		case 'M'  :
			ret_num *= (multiplier = 1000000);
			numstr++;
			break;
		case 'g'  :
		case 'G'  :
			ret_num *= (multiplier = 1000000000);
			numstr++;
			break;
		case '\0' :
			break;
		default   :
			return 0;
	}
	while( isdigit( *numstr ) && multiplier > 1 ) {
		multiplier /= 10;
		ret_num = ret_num + ((*numstr++ - '0') * multiplier);
	}
	if( *numstr && !isdigit( *numstr ) )
		return 0;
	return ret_num;
}

inline __int64 AdvAtoi64( char *numstr )
{
	__int64 ret_num = 0;
	__int64 multiplier = 0;

	if( numstr == NULL || !*numstr )
		return 0;
	for( ; isspace( *numstr ); numstr++ );
	if( !isdigit( *numstr ) )
		return 0;
	while( isdigit( *numstr ) )
		ret_num = (ret_num * 10) + (*numstr++ - '0');
	switch( *numstr ) {
		case 'k'  :
		case 'K'  :
			ret_num *= (multiplier = 1000);
			numstr++;
			break;
		case 'm'  :
		case 'M'  :
			ret_num *= (multiplier = 1000000);
			numstr++;
			break;
		case 'g'  :
		case 'G'  :
			ret_num *= (multiplier = 1000000000);
			numstr++;
			break;
		case '\0' :
			break;
		default   :
			return 0;
	}
	while( isdigit( *numstr ) && multiplier > 1 ) {
		multiplier /= 10;
		ret_num = ret_num + ((*numstr++ - '0') * multiplier);
	}
	if( *numstr && !isdigit( *numstr ) )
		return 0;
	return ret_num;
}

inline int BaseIcvt( int nValue, char *szString )
{
	char szBuffer[ICVT_BUFFER_SIZE], *pszText;
	int nLocValue, cDigit, nTextLen;
	bool bSigned;

	if( szString == NULL )
		return 0;
	if( nValue == 0 ) {
		*(szString+0) = '0';
		*(szString+1) = '\0';
		return 1;
	} else if( nValue < 0 ) {
		nLocValue = -nValue;
		bSigned = TRUE;
	} else {
		bSigned = FALSE;
		nLocValue = nValue;
	}

	pszText = &szBuffer[ICVT_BUFFER_SIZE - 1];
	while( nLocValue != 0 ) {
		cDigit = (int) (nLocValue % 10) + '0';
		nLocValue /= 10;
		*pszText-- = (char) cDigit;
	}
	if( bSigned )
		*pszText-- = '-';
	nTextLen = (int)((char *) &szBuffer[ICVT_BUFFER_SIZE - 1] - pszText);
	pszText++;
	memcpy( szString, pszText, nTextLen );
	*(szString + nTextLen) = '\0';
	return nTextLen;
}

inline int BaseUcvt( unsigned int nValue, char *szString )
{
	char szBuffer[ICVT_BUFFER_SIZE], *pszText;
	unsigned int nLocValue;
	int cDigit, nTextLen;

	if( szString == NULL )
		return 0;
	if( nValue == 0 ) {
		*(szString+0) = '0';
		*(szString+1) = '\0';
		return 1;
	}
	nLocValue = nValue;

	pszText = &szBuffer[ICVT_BUFFER_SIZE - 1];
	while( nLocValue != 0 ) {
		cDigit = (int) (nLocValue % 10) + '0';
		nLocValue /= 10;
		*pszText-- = (char) cDigit;
	}
	nTextLen = (int)((char *) &szBuffer[ICVT_BUFFER_SIZE - 1] - pszText);
	pszText++;
	memcpy( szString, pszText, nTextLen );
	*(szString + nTextLen) = '\0';
	return nTextLen;
}

#undef KILO_SIZE
#undef MEGA_SIZE
#undef GIGA_SIZE
#undef TERA_SIZE
#undef PETA_SIZE
#undef EXA_SIZE
#define KILO_SIZE	(1000)
#define MEGA_SIZE	(1000 * KILO_SIZE)
#define GIGA_SIZE	(1000 * MEGA_SIZE)
#define TERA_SIZE	(LONGLONG)((LONGLONG)1000 * GIGA_SIZE)
#define PETA_SIZE	(LONGLONG)((LONGLONG)1000 * TERA_SIZE)
#define EXA_SIZE	(LONGLONG)((LONGLONG)1000 * PETA_SIZE)

inline int AdvIcvt( int nValue, CHAR *szString )
{
	int nInt;

	if( nValue >= GIGA_SIZE ) {
		nInt = nValue / GIGA_SIZE;
		strcpy( szString + BaseIcvt( nInt, szString ), "G" );
	} else if( nValue >= MEGA_SIZE ) {
		nInt = nValue / MEGA_SIZE;
		strcpy( szString + BaseIcvt( nInt, szString ), "M" );
	} else if( nValue >= KILO_SIZE ) {
		nInt = nValue / KILO_SIZE;
		sprintf( szString, "%dK", nInt );
		strcpy( szString + BaseIcvt( nInt, szString ), "K" );
	} else {
		BaseIcvt( nValue, szString );
	}
	return (int) strlen( szString );
}

inline int AdvI64cvt( __int64 nValue, CHAR *szString )
{
	__int64 nInt;

	if( nValue >= EXA_SIZE ) {
		nInt = nValue / EXA_SIZE;
		strcpy( szString + BaseIcvt( (int) nInt, szString ), "E" );
	} else if( nValue >= PETA_SIZE ) {
		nInt = nValue / PETA_SIZE;
		strcpy( szString + BaseIcvt( (int) nInt, szString ), "P" );
	} else if( nValue >= TERA_SIZE ) {
		nInt = nValue / TERA_SIZE;
		strcpy( szString + BaseIcvt( (int) nInt, szString ), "T" );
	} else if( nValue >= GIGA_SIZE ) {
		nInt = nValue / GIGA_SIZE;
		strcpy( szString + BaseIcvt( (int) nInt, szString ), "G" );
	} else if( nValue >= MEGA_SIZE ) {
		nInt = nValue / MEGA_SIZE;
		strcpy( szString + BaseIcvt( (int) nInt, szString ), "M" );
	} else if( nValue >= KILO_SIZE ) {
		nInt = nValue / KILO_SIZE;
		strcpy( szString + BaseIcvt( (int) nInt, szString ), "K" );
	} else {
		BaseIcvt( (int) nValue, szString );
	}
	return (int) strlen( szString );
}

#undef KILO_SIZE
#undef MEGA_SIZE
#undef GIGA_SIZE
#undef TERA_SIZE
#undef PETA_SIZE
#undef EXA_SIZE
#define KILO_SIZE	(1024)
#define MEGA_SIZE	(1024 * KILO_SIZE)
#define GIGA_SIZE	(1024 * MEGA_SIZE)
#define TERA_SIZE	(LONGLONG)((LONGLONG)1024 * GIGA_SIZE)
#define PETA_SIZE	(LONGLONG)((LONGLONG)1024 * TERA_SIZE)
#define EXA_SIZE	(LONGLONG)((LONGLONG)1024 * PETA_SIZE)

inline int AdvIcvt1024( int nValue, CHAR *szString )
{
	int nInt;

	if( nValue >= GIGA_SIZE ) {
		nInt = nValue / GIGA_SIZE;
		strcpy( szString + BaseIcvt( nInt, szString ), "G" );
	} else if( nValue >= MEGA_SIZE ) {
		nInt = nValue / MEGA_SIZE;
		strcpy( szString + BaseIcvt( nInt, szString ), "M" );
	} else if( nValue >= KILO_SIZE ) {
		nInt = nValue / KILO_SIZE;
		sprintf( szString, "%dK", nInt );
		strcpy( szString + BaseIcvt( nInt, szString ), "K" );
	} else {
		BaseIcvt( nValue, szString );
	}
	return (int) strlen( szString );
}

inline int AdvI64cvt1024( __int64 nValue, CHAR *szString )
{
	__int64 nInt;

	if( nValue >= EXA_SIZE ) {
		nInt = nValue / EXA_SIZE;
		strcpy( szString + BaseIcvt( (int) nInt, szString ), "E" );
	} else if( nValue >= PETA_SIZE ) {
		nInt = nValue / PETA_SIZE;
		strcpy( szString + BaseIcvt( (int) nInt, szString ), "P" );
	} else if( nValue >= TERA_SIZE ) {
		nInt = nValue / TERA_SIZE;
		strcpy( szString + BaseIcvt( (int) nInt, szString ), "T" );
	} else if( nValue >= GIGA_SIZE ) {
		nInt = nValue / GIGA_SIZE;
		strcpy( szString + BaseIcvt( (int) nInt, szString ), "G" );
	} else if( nValue >= MEGA_SIZE ) {
		nInt = nValue / MEGA_SIZE;
		strcpy( szString + BaseIcvt( (int) nInt, szString ), "M" );
	} else if( nValue >= KILO_SIZE ) {
		nInt = nValue / KILO_SIZE;
		strcpy( szString + BaseIcvt( (int) nInt, szString ), "K" );
	} else {
		BaseIcvt( (int) nValue, szString );
	}
	return (int) strlen( szString );
}

#ifdef  __cplusplus
}
#endif

