#pragma once


///////////////// 각종 실수 상수 정의 /////////////////
#define PI					3.141592653589			//180도
#define PI2					6.283185307178			//360도
#define PIx2				PI2						//360도
#define PItime2				PI2						//360도
#define PIdiv2				1.570796326795			//90도
#define PIover2				PIdiv2					//90도
#define DEG90				PIdiv2					//90도
#define DEG45				0.7853981633974			//45도
#define DEG5				0.087266462599			//5도
#define DEG10				0.174532925199			//10도
#define DEG20				0.349065850398			//20도
#define DEG30				0.523598775597			//30도
#define DEG40				0.698131700796			//40도
#define DEG50				0.872664625995			//50도
#define DEG60				1.047197551194			//60도
#define DEG120				2.094395102388			//120도
#define DEG80				1.396263401592			//80도
#define DEG160				2.792526803191			//160도
#define DEG140				2.443460952792			//140도
#define SQRT2				1.4142135623731			//45*빗변
#define	SQRT_2				0.7071067811865			//1/SQRT2
#define logNE				2.71828182845904523536	//실제 log값
#define logN2				0.69314718055994530942	//2배 비율
#define logN10				2.30258509299404568402	//10배 비율
#define RADtoDEG			57.2957795132
#define DEGtoRAD			0.0174532925199

//////////////// 각종 수학함수 정의 ///////////////////
#define	ODD(a)				((a)&1)									//홀수인 경우 참(1)을 리턴
#define EVEN(a)				(!(a)&1)								//짝수인 경우 참(1)을 리턴
#define SGN(a)				(((a)<(0)) ? -1 : ((a)>(0)) ? 1 : 0)	//부호를 리턴
#define SQR(a)				((a)*(a))								//제곱
#define	CUBE(a)				((a)*(a)*(a))							//세제곱	
#define ABS(a)				(((a)<(0)) ? -(a) : (a))				//절대값
#define ROUND(a)			((a)>0 ? (int)(a+.5) : -(int)(.5-a))	//소수점이하값을 반올림
#define TRUNC(a)			(int)(a)								//소수점이하값을 버림
#define MIN(a,b)			(((a)<(b)) ? (a) : (b))					//두수중 적은수를 구함
#define MAX(a,b)			(((a)>(b)) ? (a) : (b))					//두수중 큰수를 구함
#define MIN3(a,b,c)			(MIN(MIN(a,b),c))						//세수중 적은수를 구함
#define MAX3(a,b,c)			(MAX(MAX(a,b),c))						//세수중 큰수를 구함
#define SWAP(a,b)			((a)^=(b)^=(a)^=(b))					//a xor b and b xor a and a xor b
#define SWAPPT(a,b)			(SWAP(a.y,b.y), SWAP(a.x,b.x))			//점(POINT)의 좌표를 바꿈
#define RECIP(a)			((1)/(a))								//a^-1 보수
#define NOT(a)				(~(a))									//not
#define XOR(a,b)			((a)^(b))								//배타적논리합
#define	ASL(a)				((a)<<1)								//좌로 1칸 쉬프트
#define ASL2(a,b)			((a)<<(b))								//좌로 b칸 만큼 쉬프트
#define ASR(a)				((a)>>1)								//우로 1칸 쉬프트
#define ASR2(a,b)			((a)>>(b))								//우로 b칸 만큼 쉬프트
#define LIMIT(val,l,h)		((val)<(l) ? (l) : (val)>(h) ? (h) : (val))		//수의 범위를 제한

#define TORAD(a)			((a)*(DEGtoRAD))						//각도값을 라디안값으로 변환
#define TODEG(a)			((a)*(RADtoDEG))						//라디안값을 각도값으로 변환

#define CSC(a)				(((a)==0) ? 0 : (1/sin(a)))				//코시컨트
#define SEC(a)				(((a)==0) ? 0 : (1/cos(a)))				//시컨트
#define COT(a)				(((a)==0) ? 0 : (1/tan(a)))				//코탄젠트
#define ARCSIN(a)			(atan((x)/sqrt(-(x)*(x)+1)))			//아크사인
#define ARCCOS(a)			(-atan((x)/sqrt(-(x)*(x)+1))+PIdiv2)	//아크코사인
#define AVG(a,b)			(((a)+(b))/2)							//평균
#define perppos(a)			((a)+PIdiv2)							//현재각의 시계반대방향의 수직각
#define perpneg(a)			((a)-PIdiv2)							//현재각의 시계방향의 수직각
#define SUM(a,b)			((a)+(b))								//합

#define LO_BYTE(a)			((a)&=255)							//하위byte리턴
#define	HI_BYTE(a)			((a)&=65280)						//상위byte리턴

#define HI_LOSWAPshort(a)	{a=((((a)&0xff)<8) | (((a)&0xff00)>>8));}			//상위byte와 하위byte를 바꿈
#define HI_LOSWAPlong(a)	{a=((((a)&0xffff)<<16) | (((a)&0xffff0000)<<16));}	//상위word와 하위word를 바꿈
// BDH: Use Standard Math
#define DIST(a,b)			(int) (_hypot( (double) (a), (double) (b) ))
// #define DIST(a,b)			(float)(sqrt((float)((a)*(a))+((b)*(b))))					//두점의 거리 계산
#define ANGLE(x,y)			(float)((x!=0) ? (float)cos(x) : (float)sin(y))		//x와 y값을 가지고 각을 계산
#define TOCELSIUS(a)		(((a)-32/5*9)
#define TOFAREN(a)			(((a)*5/9)+32)
#define TOCENT(a)			((a)*2.54)
#define TOINCH(a)			((a)*.3937)
#define derive(a,x,b,c)		(((a)*(b))*pow((x),(b)-1))
#define VOLCYLINDER(r,h)	((h)*(PI*((r)*(r))))								//실린더의 용적
#define VOLCUBE(s)			((s)*(s)*(s))										//세제곱
#define VOLSPHERE(r)		(PI*((r)*(r)*(r)))									//구의용적
#define SURCYLINDER(r,h)	((h)*2*(r)*PI)										//표면적
#define	SURCUBE(s)			((s)*(s)*6)
#define SURSPHERE(s)		(3*PI*(r)*(r))
#define SURCONE(r,h)		(((PI*(r))*sqrt((r)*(r)+(h)*(h)))
#define AREARECT(x,y)		((x)*(y))
#define AREACONE(r,h)		(((PI*(r)*(r))*(h)/3)
#define AREACIRCLE(r)		((PI*(r)*(r))
#define AREATRIANGLE(b,h)	((b)*(h)*.5)
#define GALTOLITRE(a)		((a)*3.78541)
#define LITRETOGAL(a)		((a)*.264172)
#define LB_toKG(a)			((a)*.453592)
#define KG_toLB(a)			((a)*2.20462)
#define GRAVITY(m1,m2,dist)	((((m1)+(m2))*.0000000000667)/((dist)*dist))		//두값의 인력
#define ACCELERATION(f,m)	((f)/(m))
#define VALOCITY(acc,time)	((acc)/(time))
#define WORK(m,v)			((m)*(v)*(v))
#define NEWTON(kg,meter)	((kg)*(meter))


namespace BaseMath
{
	extern const unsigned short	m_FastHeToBi[0x100];
	extern const char*			PrefixOfHexadecimal;

	//// String을 값으로 바꿈(String은 0x가 붙은 Hex string이다.)
	//inline char			StrToHex08(char *szSrc);
	//inline unsigned short	StrToHex16(char *szSrc);
	//inline unsigned int	StrToHex32(char *szSrc);
	//inline __int64		StrToHex64(char *szSrc);

	// String을 값으로 바꿈(0x가 붙던지, 붙지 않던지간에 전부 바꿀 수 있다)
	inline char				Atoc(char *szSrc);
	inline unsigned short	Atos(char *szSrc);
	inline unsigned int		Atoi(char *szSrc);
	inline __int64			Atol64(char *szSrc);

	//// 값을 Hex String으로 바꿈
	//inline void			Hex08ToStr(char *szDest, BYTE hex);
	//inline void			Hex16ToStr(char *szDest, WORD hex);
	//inline void			Hex32ToStr(char *szDest, DWORD hex);
	//inline void			Hex64ToStr(char *szDest, DWORD64 hex);

	inline void Hex08ToStr( char *szDest, BYTE hex )
	{
		*((WORD *) szDest) = m_FastHeToBi[ hex ];
		szDest += 2;
		*(szDest) = '\0';
	}

	inline void Hex16ToStr( char *szDest, WORD hex )
	{
		LPBYTE pSrc = (LPBYTE) &hex;

	#ifdef BIG_ENDIAN
		*((WORD *) (szDest +  0)) = m_FastHeToBi[ *(pSrc++) ];
		*((WORD *) (szDest +  2)) = m_FastHeToBi[ *(pSrc++) ];
	#else
		*((WORD *) (szDest +  2)) = m_FastHeToBi[ *(pSrc++) ];
		*((WORD *) (szDest +  0)) = m_FastHeToBi[ *(pSrc++) ];
	#endif
		*(szDest + 4) = '\0';
	}

	inline void Hex32ToStr( char *szDest, DWORD hex )
	{
		LPBYTE pSrc = (LPBYTE) &hex;

	#ifdef BIG_ENDIAN
		*((WORD *) (szDest +  0)) = m_FastHeToBi[ *(pSrc++) ];
		*((WORD *) (szDest +  2)) = m_FastHeToBi[ *(pSrc++) ];
		*((WORD *) (szDest +  4)) = m_FastHeToBi[ *(pSrc++) ];
		*((WORD *) (szDest +  6)) = m_FastHeToBi[ *(pSrc++) ];
	#else
		*((WORD *) (szDest +  6)) = m_FastHeToBi[ *(pSrc++) ];
		*((WORD *) (szDest +  4)) = m_FastHeToBi[ *(pSrc++) ];
		*((WORD *) (szDest +  2)) = m_FastHeToBi[ *(pSrc++) ];
		*((WORD *) (szDest +  0)) = m_FastHeToBi[ *(pSrc++) ];
	#endif
		*(szDest + 8) = '\0';
	}

	inline void Hex64ToStr( char *szDest, DWORD64 hex )
	{
		LPBYTE pSrc = (LPBYTE) &hex;

	#ifdef BIG_ENDIAN
		*((WORD *) (szDest +  0)) = m_FastHeToBi[ *(pSrc++) ];
		*((WORD *) (szDest +  2)) = m_FastHeToBi[ *(pSrc++) ];
		*((WORD *) (szDest +  4)) = m_FastHeToBi[ *(pSrc++) ];
		*((WORD *) (szDest +  6)) = m_FastHeToBi[ *(pSrc++) ];
		*((WORD *) (szDest +  8)) = m_FastHeToBi[ *(pSrc++) ];
		*((WORD *) (szDest + 10)) = m_FastHeToBi[ *(pSrc++) ];
		*((WORD *) (szDest + 12)) = m_FastHeToBi[ *(pSrc++) ];
		*((WORD *) (szDest + 14)) = m_FastHeToBi[ *(pSrc++) ];
	#else
		*((WORD *) (szDest + 14)) = m_FastHeToBi[ *(pSrc++) ];
		*((WORD *) (szDest + 12)) = m_FastHeToBi[ *(pSrc++) ];
		*((WORD *) (szDest + 10)) = m_FastHeToBi[ *(pSrc++) ];
		*((WORD *) (szDest +  8)) = m_FastHeToBi[ *(pSrc++) ];
		*((WORD *) (szDest +  6)) = m_FastHeToBi[ *(pSrc++) ];
		*((WORD *) (szDest +  4)) = m_FastHeToBi[ *(pSrc++) ];
		*((WORD *) (szDest +  2)) = m_FastHeToBi[ *(pSrc++) ];
		*((WORD *) (szDest +  0)) = m_FastHeToBi[ *(pSrc++) ];
	#endif
		*(szDest + 16) = '\0';
	}

	// this function returns the equivalent binary value for an individual character specified in the ascii format
	inline UCHAR BiToHe( char cBin )
	{
		if ( (cBin >= '0') && (cBin <= '9') )
			return ( cBin - '0' );
		else if ( (cBin >= 'A') && (cBin <= 'F') )
			return ( cBin - 'A' + 0xA );
		if ( (cBin >= 'a') && (cBin <= 'f') )
			return ( cBin -'a' + 0xA );

		return cBin;
	}

	inline char StrToHex08( char *szSrc )
	{
		char	*pStart		= szSrc + 2;
		char	cHex		= 0;

		for ( int i=0; i<1; i++ ) {
			char	c1		= BiToHe(*pStart++);
			c1 <<= (8*(7-i)+4);
			char	c2		= BiToHe(*pStart++);
			c2 <<= (8*(7-i));
			char	cRet	=  c1  + c2;
			cHex	+= cRet;

		}
		return cHex;
	}

	inline unsigned short StrToHex16( char *szSrc )
	{
		char	*pStart		= szSrc + 2;
		unsigned short	sHex		= 0;

		for ( int i=0; i<2; i++ ) {
			
			unsigned short	s1	= BiToHe(*pStart++);
			s1 <<= (8*(1-i)+4);
			unsigned short	s2	= BiToHe(*pStart++);
			s2 <<= (8*(1-i));
			unsigned short	sRet =  s1  + s2;

			sHex	+= (unsigned short) sRet;
		}

		return sHex;
	}

	// convert string to hexadecimal value
	inline unsigned int StrToHex32( char *szSrc )
	{
		char			*pStart		= szSrc + 2;
		unsigned int	nHex		= 0;

		for ( int i=0; i<4; i++ ) {
			unsigned int n1		= BiToHe(*pStart++);
			n1 <<= (8*(3-i)+4);
			unsigned int n2		= BiToHe(*pStart++);
			n2 <<= (8*(3-i));

			unsigned int nRet	=  n1  + n2;

			nHex += nRet;
		}

		return nHex;
	}

	// convert string to hexadecimal value
	inline __int64 StrToHex64( char *szSrc )
	{
		char	*pStart		= szSrc + 2;
		__int64	dlHex		= 0;

		for ( int i=0; i<8; i++ ) {
			__int64	dl1		= BiToHe(*pStart++);
			dl1 <<= (8*(7-i)+4);
			__int64	dl2		= BiToHe(*pStart++);
			dl2 <<= (8*(7-i));

			__int64	dlRet	=  dl1  + dl2;

			dlHex	+= dlRet;

		}
		return dlHex;
	}


	// convert string to value
	inline DWORD CommaStrToDWORD( char *szSrc )
	{
		char	*pStart			= szSrc;
		DWORD	dwValue			= 0;
		DWORD	dwFactor		= 1;

		int nLen	= static_cast<int>(strlen(pStart));
		for ( int i=(nLen-1); i>=0; --i) {
			if ( ( ' ' == pStart[i] ) || ( '\"' == pStart[i] ) || ( ',' == pStart[i] ) )
				continue;

			dwValue	+= ( BiToHe(pStart[i]) * dwFactor );
			dwFactor	*= 10;
		}

		return dwValue;
	}
};


/*
// FastMath.h: interface for the CFastMath class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include <winsock2.h>
#include <windows.h>
#include <cmath>
#include <tchar.h>
#include <BaseLibrary/Pattern/Singleton.hpp>


#define FP_BITS(fp) (*(DWORD*)&(fp))

class CFastMath : public CSingleton<CFastMath> {
	
private:

	union FastSqrtUnion {
		float f;
		unsigned int i;
	};

	unsigned int m_FastSqrtTable[0x10000];

	static long m_HoldRand;

public:
	static const float	m_fPI;
	static const double	m_dPI;

	// constructor and destructor
	CFastMath();	
	virtual ~CFastMath();

	// -------------------------------------------------------------------------------------------------
	// 빠른 제곱근 처리 함수 - 빠르긴 뭐가 빠르냐 --;;;; deprecated by Sparrowhawk : 2002/11/08 5:22

	// void InitSqrt();	// 빠른 제곱근 연산을 위한 테이블 초기화
	// inline float FastSqrt(float n);
	
	// -------------------------------------------------------------------------------------------------
	// 문자열 변환 함수들
	
	// SPX, IPX 주소 계산을 위해 사용하는 함수
	static unsigned char	BiToHe(char cBin);	
	static void				AcToHe(char *szDst, char *szSrc, int iCount);

	// -------------------------------------------------------------------------------------------------
	// String-to-Hash 함수들 : http://www.cs.yorku.ca/~oz/hash.html 에서 참고

	static inline unsigned long djb2Hash(const unsigned char *str);	// first reported by dan bernstein 
	static inline unsigned long sdbmHash(const unsigned char *str);	// this is one of the algorithms used in berkeley db
	static inline unsigned long looseHash(const unsigned char *str);	// 가장 간단한 해쉬. 그냥 모두 더함.
};

inline float CFastMath::FastSqrt(float n) 
{
	if (FP_BITS(n) == 0) { return 0.0f; } // check for square root of 0
	FP_BITS(n) = m_FastSqrtTable[(FP_BITS(n) >> 8) & 0xFFFF] | ((((FP_BITS(n) - 0x3F800000) >> 1) + 0x3F800000) & 0x7F800000);
	return n;
};

inline unsigned long CFastMath::djb2Hash(const unsigned char *str)
{
    unsigned long hash = 5381;
    int c;

	while (c = *str++) { hash = ((hash << 5) + hash) + c; } //hash * 33 + c
    return hash;
}


inline unsigned long CFastMath::sdbmHash(const unsigned char *str)
{
    unsigned long hash = 0;
    int c;

	while (c = *str++) { hash = c + (hash << 6) + (hash << 16) - hash; }
    return hash;
}

inline unsigned long CFastMath::looseHash(const unsigned char *str)
{
	unsigned int hash = 0;
	int c;

	while (c = *str++) { hash += c; }
	return hash;
}

// FastMath.cpp: implementation of the CFastMath class.
//
//////////////////////////////////////////////////////////////////////

#include "../StdAfx.h"
#include "FastMath.h"

CFastMath	g_FastMath;


long CFastMath::m_HoldRand = 1L;

CFastMath::CFastMath()
{
//	InitSqrt();
}

CFastMath::~CFastMath()
{

}

void CFastMath::InitSqrt()
{
	unsigned int	i;
	FastSqrtUnion	s;

	for(i = 0; i <= 0x7FFF; ++i) {
		s.i = (i << 8) | (0x7F << 23);
		s.f = (float)sqrt(s.f);
		m_FastSqrtTable[i+0x8000]=(s.i&0x7FFFFF);
		s.i = (i << 8) | (0x80 << 23);
		s.f = (float)sqrt(s.f);
		m_FastSqrtTable[i]=(s.i&0x7FFFFF);
	}
}

//
// this function returns the equivalent binary value for an individual character specified in the ascii format
UCHAR CFastMath::BiToHe( char cBin )
{
	if ( (cBin >= '0') && (cBin <= '9') ) {
		return ( cBin - '0' );
	} else 	if ( (cBin >= 'A') && (cBin <= 'F') ) {
		return ( cBin - 'A' + 0xA );
	} if ( (cBin >= 'a') && (cBin <= 'f') ) {
		return ( cBin -'a' + 0xA );
	}
	return cBin;
}

void CFastMath::AcToHe(char *szDst, char *szSrc, int iCount)
{
	while( iCount-- ) {
		*szDst = BiToHe(*szSrc) << 4;
		*szSrc++;
		*szDst += BiToHe(*szSrc);
		*szSrc++;
		*szDst++;
	}

닷넷 릴리즈 버전에서 이 루틴으로 컴파일하면 제대로 작동하지 않습니다(szDst에 아무것도 복사가 안됨). 
다른 분들은 괜찮은가요? --장진영 (2002-08-17)

//	while (iCount--) {
//		*szDst++ = (BiToHe(*szSrc++) << 4) + BiToHe(*szSrc++); 
//	}
	return;
}

char CFastMath::Atoc( char *szSrc )
{
	if ( strncmp( szSrc, BaseMath::PrefixOfHexadecimal, strlen(BaseMath::PrefixOfHexadecimal) ) == 0 ) {
		return StrToHex08( szSrc );
	} else {
		return (char) _ttol( szSrc );
	}
}

//
// TCHAR *로 고칩시다! (By Standil)
//
unsigned short CFastMath::Atos( char *szSrc )
{
	if ( strncmp( szSrc, BaseMath::PrefixOfHexadecimal, strlen(BaseMath::PrefixOfHexadecimal) ) == 0 ) {
		return StrToHex16( szSrc );
	} else {
		return _ttoi( szSrc );
	}
}

//
// TCHAR *로 고칩시다! (By Standil)
//
unsigned int CFastMath::Atoi( char *szSrc )
{
	if ( strncmp( szSrc, , BaseMath::PrefixOfHexadecimal, strlen(BaseMath::PrefixOfHexadecimal) ) == 0 ) {
		return StrToHex32( szSrc );
	} else {
		return _ttol( szSrc );
	}
}

//
// TCHAR *로 고칩시다! (By Standil)
//
__int64 CFastMath::Atol64( char *szSrc )
{
	if ( strncmp( szSrc, , BaseMath::PrefixOfHexadecimal, strlen(BaseMath::PrefixOfHexadecimal) ) == 0 ) {
		return StrToHex32( szSrc );
	} else {
		return _ttoi64( szSrc );
	}
}

const float		CFastMath::m_fPI = 3.14159265358979323846f;
const double	CFastMath::m_dPI = 3.14159265358979323846;

const float CFastMath::m_fSinTable[360] = {
	0.000000f, 0.017452f, 0.034899f, 0.052336f, 0.069756f, 
	0.087156f, 0.104528f, 0.121869f, 0.139173f, 0.156434f, 
	0.173648f, 0.190809f, 0.207912f, 0.224951f, 0.241922f, 
	0.258819f, 0.275637f, 0.292372f, 0.309017f, 0.325568f, 
	0.342020f, 0.358368f, 0.374607f, 0.390731f, 0.406737f, 
	0.422618f, 0.438371f, 0.453990f, 0.469472f, 0.484810f, 
	0.500000f, 0.515038f, 0.529919f, 0.544639f, 0.559193f, 
	0.573576f, 0.587785f, 0.601815f, 0.615661f, 0.629320f, 
	0.642788f, 0.656059f, 0.669131f, 0.681998f, 0.694658f, 
	0.707107f, 0.719340f, 0.731354f, 0.743145f, 0.754710f, 
	0.766044f, 0.777146f, 0.788011f, 0.798636f, 0.809017f, 
	0.819152f, 0.829038f, 0.838671f, 0.848048f, 0.857167f, 
	0.866025f, 0.874620f, 0.882948f, 0.891007f, 0.898794f, 
	0.906308f, 0.913545f, 0.920505f, 0.927184f, 0.933580f, 
	0.939693f, 0.945519f, 0.951057f, 0.956305f, 0.961262f, 
	0.965926f, 0.970296f, 0.974370f, 0.978148f, 0.981627f, 
	0.984808f, 0.987688f, 0.990268f, 0.992546f, 0.994522f, 
	0.996195f, 0.997564f, 0.998630f, 0.999391f, 0.999848f, 
	1.000000f, 0.999848f, 0.999391f, 0.998630f, 0.997564f, 
	0.996195f, 0.994522f, 0.992546f, 0.990268f, 0.987688f, 
	0.984808f, 0.981627f, 0.978148f, 0.974370f, 0.970296f, 
	0.965926f, 0.961262f, 0.956305f, 0.951057f, 0.945519f, 
	0.939693f, 0.933580f, 0.927184f, 0.920505f, 0.913545f, 
	0.906308f, 0.898794f, 0.891007f, 0.882948f, 0.874620f, 
	0.866025f, 0.857167f, 0.848048f, 0.838671f, 0.829038f, 
	0.819152f, 0.809017f, 0.798636f, 0.788011f, 0.777146f, 
	0.766044f, 0.754710f, 0.743145f, 0.731354f, 0.719340f, 
	0.707107f, 0.694658f, 0.681998f, 0.669131f, 0.656059f, 
	0.642788f, 0.629320f, 0.615661f, 0.601815f, 0.587785f, 
	0.573576f, 0.559193f, 0.544639f, 0.529919f, 0.515038f, 
	0.500000f, 0.484810f, 0.469472f, 0.453990f, 0.438371f, 
	0.422618f, 0.406737f, 0.390731f, 0.374607f, 0.358368f, 
	0.342020f, 0.325568f, 0.309017f, 0.292372f, 0.275637f, 
	0.258819f, 0.241922f, 0.224951f, 0.207912f, 0.190809f, 
	0.173648f, 0.156434f, 0.139173f, 0.121869f, 0.104528f, 
	0.087156f, 0.069756f, 0.052336f, 0.034899f, 0.017452f, 
	0.000000f, -0.017452f, -0.034899f, -0.052336f, -0.069756f, 
	-0.087156f, -0.104528f, -0.121869f, -0.139173f, -0.156434f, 
	-0.173648f, -0.190809f, -0.207912f, -0.224951f, -0.241922f, 
	-0.258819f, -0.275637f, -0.292372f, -0.309017f, -0.325568f, 
	-0.342020f, -0.358368f, -0.374607f, -0.390731f, -0.406737f, 
	-0.422618f, -0.438371f, -0.453990f, -0.469472f, -0.484810f, 
	-0.500000f, -0.515038f, -0.529919f, -0.544639f, -0.559193f, 
	-0.573576f, -0.587785f, -0.601815f, -0.615661f, -0.629320f, 
	-0.642788f, -0.656059f, -0.669131f, -0.681998f, -0.694658f, 
	-0.707107f, -0.719340f, -0.731354f, -0.743145f, -0.754710f, 
	-0.766044f, -0.777146f, -0.788011f, -0.798636f, -0.809017f, 
	-0.819152f, -0.829038f, -0.838671f, -0.848048f, -0.857167f, 
	-0.866025f, -0.874620f, -0.882948f, -0.891007f, -0.898794f, 
	-0.906308f, -0.913545f, -0.920505f, -0.927184f, -0.933580f, 
	-0.939693f, -0.945519f, -0.951057f, -0.956305f, -0.961262f, 
	-0.965926f, -0.970296f, -0.974370f, -0.978148f, -0.981627f, 
	-0.984808f, -0.987688f, -0.990268f, -0.992546f, -0.994522f, 
	-0.996195f, -0.997564f, -0.998630f, -0.999391f, -0.999848f, 
	-1.000000f, -0.999848f, -0.999391f, -0.998630f, -0.997564f, 
	-0.996195f, -0.994522f, -0.992546f, -0.990268f, -0.987688f, 
	-0.984808f, -0.981627f, -0.978148f, -0.974370f, -0.970296f, 
	-0.965926f, -0.961262f, -0.956305f, -0.951057f, -0.945519f, 
	-0.939693f, -0.933580f, -0.927184f, -0.920505f, -0.913545f, 
	-0.906308f, -0.898794f, -0.891007f, -0.882948f, -0.874620f, 
	-0.866025f, -0.857167f, -0.848048f, -0.838671f, -0.829038f, 
	-0.819152f, -0.809017f, -0.798636f, -0.788011f, -0.777146f, 
	-0.766044f, -0.754710f, -0.743145f, -0.731354f, -0.719340f, 
	-0.707107f, -0.694658f, -0.681998f, -0.669131f, -0.656059f, 
	-0.642788f, -0.629320f, -0.615661f, -0.601815f, -0.587785f, 
	-0.573576f, -0.559193f, -0.544639f, -0.529919f, -0.515038f, 
	-0.500000f, -0.484810f, -0.469472f, -0.453990f, -0.438371f, 
	-0.422618f, -0.406737f, -0.390731f, -0.374607f, -0.358368f, 
	-0.342020f, -0.325568f, -0.309017f, -0.292372f, -0.275637f, 
	-0.258819f, -0.241922f, -0.224951f, -0.207912f, -0.190809f, 
	-0.173648f, -0.156434f, -0.139173f, -0.121869f, -0.104528f, 
	-0.087156f, -0.069756f, -0.052336f, -0.034899f, -0.017452f
};

const float CFastMath::m_fCosTable[360] = {
	1.000000f, 0.999848f, 0.999391f, 0.998630f, 0.997564f, 
	0.996195f, 0.994522f, 0.992546f, 0.990268f, 0.987688f, 
	0.984808f, 0.981627f, 0.978148f, 0.974370f, 0.970296f, 
	0.965926f, 0.961262f, 0.956305f, 0.951057f, 0.945519f, 
	0.939693f, 0.933580f, 0.927184f, 0.920505f, 0.913545f, 
	0.906308f, 0.898794f, 0.891007f, 0.882948f, 0.874620f, 
	0.866025f, 0.857167f, 0.848048f, 0.838671f, 0.829038f, 
	0.819152f, 0.809017f, 0.798636f, 0.788011f, 0.777146f, 
	0.766044f, 0.754710f, 0.743145f, 0.731354f, 0.719340f, 
	0.707107f, 0.694658f, 0.681998f, 0.669131f, 0.656059f, 
	0.642788f, 0.629320f, 0.615661f, 0.601815f, 0.587785f, 
	0.573576f, 0.559193f, 0.544639f, 0.529919f, 0.515038f, 
	0.500000f, 0.484810f, 0.469472f, 0.453990f, 0.438371f, 
	0.422618f, 0.406737f, 0.390731f, 0.374607f, 0.358368f, 
	0.342020f, 0.325568f, 0.309017f, 0.292372f, 0.275637f, 
	0.258819f, 0.241922f, 0.224951f, 0.207912f, 0.190809f, 
	0.173648f, 0.156434f, 0.139173f, 0.121869f, 0.104528f, 
	0.087156f, 0.069756f, 0.052336f, 0.034899f, 0.017452f, 
	0.000000f, -0.017452f, -0.034899f, -0.052336f, -0.069756f, 
	-0.087156f, -0.104528f, -0.121869f, -0.139173f, -0.156434f, 
	-0.173648f, -0.190809f, -0.207912f, -0.224951f, -0.241922f, 
	-0.258819f, -0.275637f, -0.292372f, -0.309017f, -0.325568f, 
	-0.342020f, -0.358368f, -0.374607f, -0.390731f, -0.406737f, 
	-0.422618f, -0.438371f, -0.453990f, -0.469472f, -0.484810f, 
	-0.500000f, -0.515038f, -0.529919f, -0.544639f, -0.559193f, 
	-0.573576f, -0.587785f, -0.601815f, -0.615661f, -0.629320f, 
	-0.642788f, -0.656059f, -0.669131f, -0.681998f, -0.694658f, 
	-0.707107f, -0.719340f, -0.731354f, -0.743145f, -0.754710f, 
	-0.766044f, -0.777146f, -0.788011f, -0.798636f, -0.809017f, 
	-0.819152f, -0.829038f, -0.838671f, -0.848048f, -0.857167f, 
	-0.866025f, -0.874620f, -0.882948f, -0.891007f, -0.898794f, 
	-0.906308f, -0.913545f, -0.920505f, -0.927184f, -0.933580f, 
	-0.939693f, -0.945519f, -0.951057f, -0.956305f, -0.961262f, 
	-0.965926f, -0.970296f, -0.974370f, -0.978148f, -0.981627f, 
	-0.984808f, -0.987688f, -0.990268f, -0.992546f, -0.994522f, 
	-0.996195f, -0.997564f, -0.998630f, -0.999391f, -0.999848f, 
	-1.000000f, -0.999848f, -0.999391f, -0.998630f, -0.997564f, 
	-0.996195f, -0.994522f, -0.992546f, -0.990268f, -0.987688f, 
	-0.984808f, -0.981627f, -0.978148f, -0.974370f, -0.970296f, 
	-0.965926f, -0.961262f, -0.956305f, -0.951057f, -0.945519f, 
	-0.939693f, -0.933580f, -0.927184f, -0.920505f, -0.913545f, 
	-0.906308f, -0.898794f, -0.891007f, -0.882948f, -0.874620f, 
	-0.866025f, -0.857167f, -0.848048f, -0.838671f, -0.829038f, 
	-0.819152f, -0.809017f, -0.798636f, -0.788011f, -0.777146f, 
	-0.766044f, -0.754710f, -0.743145f, -0.731354f, -0.719340f, 
	-0.707107f, -0.694658f, -0.681998f, -0.669131f, -0.656059f, 
	-0.642788f, -0.629320f, -0.615661f, -0.601815f, -0.587785f, 
	-0.573576f, -0.559193f, -0.544639f, -0.529919f, -0.515038f, 
	-0.500000f, -0.484810f, -0.469472f, -0.453990f, -0.438371f, 
	-0.422618f, -0.406737f, -0.390731f, -0.374607f, -0.358368f, 
	-0.342020f, -0.325568f, -0.309017f, -0.292372f, -0.275637f, 
	-0.258819f, -0.241922f, -0.224951f, -0.207912f, -0.190809f, 
	-0.173648f, -0.156434f, -0.139173f, -0.121869f, -0.104528f, 
	-0.087156f, -0.069756f, -0.052336f, -0.034899f, -0.017452f, 
	-0.000000f, 0.017452f, 0.034899f, 0.052336f, 0.069756f, 
	0.087156f, 0.104528f, 0.121869f, 0.139173f, 0.156434f, 
	0.173648f, 0.190809f, 0.207912f, 0.224951f, 0.241922f, 
	0.258819f, 0.275637f, 0.292372f, 0.309017f, 0.325568f, 
	0.342020f, 0.358368f, 0.374607f, 0.390731f, 0.406737f, 
	0.422618f, 0.438371f, 0.453990f, 0.469472f, 0.484810f, 
	0.500000f, 0.515038f, 0.529919f, 0.544639f, 0.559193f, 
	0.573576f, 0.587785f, 0.601815f, 0.615661f, 0.629320f, 
	0.642788f, 0.656059f, 0.669131f, 0.681998f, 0.694658f, 
	0.707107f, 0.719340f, 0.731354f, 0.743145f, 0.754710f, 
	0.766044f, 0.777146f, 0.788011f, 0.798636f, 0.809017f, 
	0.819152f, 0.829038f, 0.838671f, 0.848048f, 0.857167f, 
	0.866025f, 0.874620f, 0.882948f, 0.891007f, 0.898794f, 
	0.906308f, 0.913545f, 0.920505f, 0.927184f, 0.933580f, 
	0.939693f, 0.945519f, 0.951057f, 0.956305f, 0.961262f, 
	0.965926f, 0.970296f, 0.974370f, 0.978148f, 0.981627f, 
	0.984808f, 0.987688f, 0.990268f, 0.992546f, 0.994522f, 
	0.996195f, 0.997564f, 0.998630f, 0.999391f, 0.999848f
};


// -90 - 90
int CFastMath::as_asin(float as)
{
	double	nearest_diff = 2.0;
	int		nearest_index;

	for(int i = -90; i <=90; ++i) {
		int index = i;
		if (index < 0) {
			index += 360;
		}

		if (fabsf(m_fSinTable[index]-as) < nearest_diff) {
			nearest_diff = fabsf(m_fSinTable[index]-as);
			nearest_index = i;
		}
	}

	return nearest_index;
}


// 0 - 180
int CFastMath::as_acos(float ac)
{
	double	nearest_diff = 2.0;
	int		nearest_index;

	for(int i = 0; i <= 180; ++i) {
		if (fabsf(m_fCosTable[i]-ac) < nearest_diff) {
			nearest_diff = fabsf(m_fCosTable[i]-ac);
			nearest_index = i;
		}
	}

	return nearest_index;
}

inline float CFastMath::as_sin(int degree)
{
	while(degree < 0) {
		degree += 360;
	}

	degree = degree % 360;
	return m_fSinTable[degree];
}

inline float CFastMath::as_cos(int degree)
{
	while(degree < 0) {
		degree += 360;
	}

	degree = degree%360;
	return m_fCosTable[degree];
}

*/