#pragma once

class CRandom
{
public:
	inline void SRand( DWORD dwSeed );
	inline int Rand( void );
	inline int Rand( int iLast );
	inline int Rand( int iFirst, int iLast );
	inline int SimpleRand( void );
	inline int SimpleRand( int iLast );
	inline int SimpleRand( int iFirst, int iLast );
	inline int ComplexRand( void );
	inline int ComplexRand( int iLast );
	inline int ComplexRand( int iFirst, int iLast );
private:
	DWORD	m_HoldRand;
};

void CRandom::SRand( DWORD dwSeed )
{
	m_HoldRand = dwSeed;
}

int CRandom::Rand( void )
{
	return( ((m_HoldRand = m_HoldRand * 214013L + 2531011L) >> 16) & 0x7FFF );
}

int CRandom::Rand( int iLast )
{
	return( (((m_HoldRand = m_HoldRand * 214013L + 2531011L) >> 16) & 0x7FFF) % iLast );
}

int CRandom::Rand( int iFirst, int iLast )
{
	return( iFirst + ((((m_HoldRand = m_HoldRand * 214013L + 2531011L) >> 16) & 0x7FFF) % (iLast - iFirst)) );
}

int CRandom::SimpleRand( void )
{
	return( ((m_HoldRand = m_HoldRand + 2531011L) >> 3) & 0x7FFF );
}

int CRandom::SimpleRand( int iLast )
{
	return( (((m_HoldRand = m_HoldRand + 2531011L) >> 3) & 0x7FFF) % iLast );
}

int CRandom::SimpleRand( int iFirst, int iLast )
{
	return( iFirst + ((((m_HoldRand = m_HoldRand + 2531011L) >> 3) & 0x7FFF) % (iLast - iFirst)) );
}

int CRandom::ComplexRand( void )
{
	DWORD dwNext = m_HoldRand;
	int iResult;

	dwNext *= 1103515245;
	dwNext += 12345;
	iResult = (DWORD) (dwNext / 65536) % 2048;

	dwNext *= 1103515245;
	dwNext += 12345;
	iResult <<= 10;
	iResult ^= (DWORD) (dwNext / 65536) % 1024;

	dwNext *= 1103515245;
	dwNext += 12345;
	iResult <<= 10;
	iResult ^= (DWORD) (dwNext / 65536) % 1024;

	m_HoldRand = dwNext;
	return iResult;
}

int CRandom::ComplexRand( int iLast )
{
	return ( ComplexRand() % iLast );
}

int CRandom::ComplexRand( int iFirst, int iLast )
{
	return ( iFirst + (ComplexRand() % (iLast - iFirst)) );
}

