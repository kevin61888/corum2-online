#ifndef __LOCALIZING_H__
#define __LOCALIZING_H__

#pragma once

// 이하 deepdark에 의해 작성/수정됨.----------------------------------------------- 

#define __ENGLISH_MODE			0	// 영문판
#define __THAILAND_MODE			11	// 태국어판
#define __KOREAN_MODE			101	// 한국판, 2바이트 언어는 100번 이상으로 세팅.
#define __JAPANESE_MODE			102	// 일본판
#define __CHINESE_MODE			103	// 중국판
#define __TAIWAN_MODE			104	// 대만판

#define __INTERNATIONAL_MODE	200	// 인터내셔널


#ifdef			JAPAN_LOCALIZING	
#	define __LOCALIZING_MODE		__JAPANESE_MODE
#elif defined	CHINA_LOCALIZING	
#	define __LOCALIZING_MODE		__CHINESE_MODE
#elif defined	TAIWAN_LOCALIZING	
#	define __LOCALIZING_MODE		__TAIWAN_MODE
#elif defined	ENGLISH_LOCALIZNG
#	define __LOCALIZING_MODE		__ENGLISH_MODE
#elif defined	THAILAND_LOCALIZING
#	define __LOCALIZING_MODE		__THAILAND_MODE
#else 	
#	define __LOCALIZING_MODE		__KOREAN_MODE	// 기본값으로 한국어판!
#endif
 
// IMEEdit을 사용할 것인지 체크해서 정의한다.
#if (__LOCALIZING_MODE > 100)	//	__KOREAN_MODE)
#	define _USE_IME
#endif

#define IS_IME_VERSION()		(__LOCALIZING_MODE > 100)

#define IS_ENGLISH_LOCALIZING()	(__LOCALIZING_MODE == __ENGLISH_MODE)
#define IS_THAILAND_LOCALIZING()(__LOCALIZING_MODE == __THAILAND_MODE)
#define IS_KOREA_LOCALIZING()	(__LOCALIZING_MODE == __KOREAN_MODE)
#define IS_JAPAN_LOCALIZING()	(__LOCALIZING_MODE == __JAPANESE_MODE)
#define IS_CHINA_LOCALIZING()	(__LOCALIZING_MODE == __CHINESE_MODE)
#define IS_TAIWAN_LOCALIZING()	(__LOCALIZING_MODE == __TAIWAN_MODE)

#define IS_INTERNATIONAL()		(__LOCALIZING_MODE == __INTERNATIONAL_MODE)

// ---------------------------------------------------------------------------------

#endif // __LOCALIZING_H__