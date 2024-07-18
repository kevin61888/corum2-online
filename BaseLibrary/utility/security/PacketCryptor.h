#pragma once

#include <BaseLibrary/Pattern/Singleton.hpp>
#include <time.h>

#define	MAX_PACKET_CIPHER_KEY_SIZE	13

class CPacketCryptor //: public CSingleton<CPacketCryptor>
{
private:
	int		m_nSendKeyOffset;
	int		m_nRecvKeyOffset;
	BYTE	m_szKeyBuffer[ MAX_PACKET_CIPHER_KEY_SIZE + 1 ];
	BYTE	m_szGeneratedKeyBuffer[ MAX_PACKET_CIPHER_KEY_SIZE + 1 ]; 

	void SwapBit(BYTE &b, bool is_encrypt)
	{
		int  t = 0;
		BYTE p[4];

		p[0] = (b & 0xC0) >> 6;
		p[1] = (b & 0x30) >> 4;
		p[2] = (b & 0x0C) >> 2;
		p[3] = (b & 0x03);

		t |= p[1] << 6;
		t |= p[0] << 4;
		t |= p[3] << 2;
		t |= p[2];

		b = t & 0xFF;
		if(is_encrypt) _asm ror b,3; else _asm rol b,3;
	}

	void Encrypt( LPBYTE pSrc, int size, int& nKeyOffset )
	{
		assert( m_szKeyBuffer != NULL );

		for(int i=0;i<size;i++)
		{
			SwapBit(pSrc[i],true);
			pSrc[i] ^= m_szKeyBuffer[nKeyOffset % MAX_PACKET_CIPHER_KEY_SIZE];
		}
	}

	void Decrypt( LPBYTE pSrc, int size, int& nKeyOffset )
	{
		assert( m_szKeyBuffer != NULL );

		for(int i=0;i<size;i++)
		{
			pSrc[i] ^= m_szKeyBuffer[nKeyOffset % MAX_PACKET_CIPHER_KEY_SIZE];
			SwapBit(pSrc[i], false);
		}
	}


	//inline void Cipher( LPBYTE pTargetBuff, const int nTargetLen, int& nKeyOffset )//, const LPBYTE pKeyBuff )
	//{
	//	assert( m_szKeyBuffer != NULL );
	//	//int key_off = 0;
	//	int val = 0;
	//	int XST = 0;
	//	int SA = 1;
	//	int SB = 2;

	//	for(int i = 0; i < nTargetLen; ++i)
	//	{
	//		if(nKeyOffset >= MAX_PACKET_CIPHER_KEY_SIZE)
	//		{
	//			nKeyOffset = 0;
	//		}

	//		val = m_szKeyBuffer[nKeyOffset];
	//		//val += i;
	//		val += nKeyOffset;
	//		SB += val;
	//		val *= SA;
	//		SA = val + 1;
	//		val = SA;
	//		val *= SB;
	//		XST += val;
	//		pTargetBuff[i] ^= XST;

	//		++nKeyOffset;
	//	}
	//}

public:
	//CPacketCryptor( LPBYTE pDefaultKey, int nSize, bool bSeed = false ) 
	CPacketCryptor( bool bSeed = false )
	{		
		if( bSeed )
			srand(unsigned(time(NULL)));

		memset( m_szKeyBuffer, 0, MAX_PACKET_CIPHER_KEY_SIZE + 1 );
		//ChangeKey( pDefaultKey, nSize );
	}

	~CPacketCryptor()
	{
	}

	/**
	 *
	 *	@param pKeyBuff 키가 세팅될 버퍼. 최소한 MAX_PACKET_CIPHER_KEY_SIZE 보다 커야한다.
	 */
	void GenerateKey()
	{
		for(int i = 0; i < MAX_PACKET_CIPHER_KEY_SIZE; ++i)
		{
			m_szGeneratedKeyBuffer[i] = rand()%256;
		}
		m_szGeneratedKeyBuffer[ MAX_PACKET_CIPHER_KEY_SIZE ] = '\0';
	}

	/**
	 *	GenerateKey() 를 통해 생성된 키를 사용하겠다는 의미.
	 */
	inline void ActivateGenKey()
	{
		SetKeyBuffer( m_szGeneratedKeyBuffer, MAX_PACKET_CIPHER_KEY_SIZE );
	}

	LPBYTE GetGenCryptKey()
	{
		return m_szGeneratedKeyBuffer;
	}

	LPBYTE GetCryptKey()
	{
		return m_szKeyBuffer;
	}

	void SetKeyBuffer( LPBYTE pNewKey, int nSize )
	{
		if( MAX_PACKET_CIPHER_KEY_SIZE < nSize )
			nSize = MAX_PACKET_CIPHER_KEY_SIZE;

		memcpy( m_szKeyBuffer, pNewKey, nSize );
		m_szKeyBuffer[ MAX_PACKET_CIPHER_KEY_SIZE ] = '\0';

		m_nSendKeyOffset = m_nRecvKeyOffset = 0;
	}

	void CipherSend( LPBYTE pTargetBuff, const int nTargetLen )
	{
		//Cipher( pTargetBuff, nTargetLen, m_nSendKeyOffset );
		Encrypt( pTargetBuff, nTargetLen, m_nSendKeyOffset );
	}

	void CipherRecv( LPBYTE pTargetBuff, const int nTargetLen )
	{
		//Cipher( pTargetBuff, nTargetLen, m_nRecvKeyOffset );
		Decrypt( pTargetBuff, nTargetLen, m_nRecvKeyOffset );
	}

};