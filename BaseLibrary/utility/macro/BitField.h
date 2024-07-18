#pragma once

#ifndef _BITFIELD_H_
#define _BITFIELD_H_

// Bit
#define BIT_CHECK( p, n )	((p) & (n))							// 비트 검사
#define BIT_SET( p, n )		((p) |= (n))						// 비트 설정
#define BIT_REMOVE( p, n )	((p) &= ~(n))						// 비트 해제
#define BIT_ZERO( p )		((p) = 0)							// 전체비트 해제

// Bit fields
typedef long				BitField;
#define MYNBBY				8
#define MYNFDBITS			(sizeof(BitField) * MYNBBY)
#define MAX_BITSFIELD(x)	(((x)+((MYNFDBITS)-1))/(MYNFDBITS))

#define BITF_SET( p, n )	((p)[(n)/MYNFDBITS] |=  (1 << ((n) % MYNFDBITS)))
#define BITF_REMOVE( p, n)	((p)[(n)/MYNFDBITS] &= ~(1 << ((n) % MYNFDBITS)))
#define BITF_CHECK( p, n)	((p)[(n)/MYNFDBITS] &   (1 << ((n) % MYNFDBITS)))
#define BITF_ZERO( p )		memset( p, 0, sizeof( *p ) );

// BitField m_bfQuest[ MAX_BITSFIELD( 1000 ) ];			// BitField 선언 (1000개)

// BITF_ZERO( m_bfQuest );								// BitField 초기화
// BITF_CHECK( m_bfQuest, 100 );						// 100번째 비트 확인 (TRUE/FALSE 반환)
// BITF_REMOVE( m_bfQuest, 100 );						// 100번째 비트 제거
// BITF_SET( m_bfQuest, 100 );							// 100번째 비트 셋팅

#endif /* _BITFIELD_H_ */
