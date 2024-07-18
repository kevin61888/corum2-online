#include "stdafx.h"
#include "Develop.h"			//	__PACKET_ENCRYPTION 이 포함되었는지 판단하려면, 꼭 넣어야 한다.  minjin. 2004. 09. 22
#include "PacketEncrypt.h"
#include "NetworkClient.h"


DFXform g_xform;
BYTE g_byClientKey[10];

extern CNetworkClient*	g_pNet;

void RandomKeyGenerator(LPBYTE btBuf)
{
	srand( (unsigned) time( NULL ) );

	for(int i = 0; i < 10; i++)
	{
		btBuf[i] = BYTE(rand() % 256);
	}
}

void CalculateKey(LPBYTE btClientKey, LPBYTE btServerKey, LPBYTE btKey)
{
	memcpy(btKey, btClientKey, 6);
	
	for(int i = 0; i < 4; i++)
	{
		btKey[i+6] = BYTE(btClientKey[i+6] ^ btServerKey[i]);
	}

	memcpy(btKey+10, btServerKey+4, 6);
}

void SetClientKey(LPBYTE byClientKey)
{
	CopyMemory(g_byClientKey, byClientKey, 10);
}

LPBYTE GetClientKey()
{
	return g_byClientKey;
}

void SetEncryptionKey(LPBYTE btKey)
{
	g_xform.InitializeBlowfish(btKey, ENCRYPT_KEY_LEN);
}

void ResetEncryptionKey()
{
	g_xform.InitializeBlowfishWithBaseKey();
}

void SendMsgWithBlowfish(char* pMsg, DWORD dwSize, BYTE btServerType)
{
#ifdef __PACKET_ENCRYPTION

	char	pEncryptedMsg[4096] = {0,};
	WORD	wdEncSize = 0;

	pEncryptedMsg[0] = pMsg[0];
	pEncryptedMsg[1] = pMsg[1];

	g_xform.Encode(LPBYTE(pMsg + 2), WORD(dwSize - 2), LPBYTE(pEncryptedMsg+2), &wdEncSize);
	
	g_pNet->SendMsg(pEncryptedMsg, wdEncSize + 2, btServerType);

#else

	g_pNet->SendMsg(pMsg, dwSize, btServerType);

#endif
}

void DecryptReceivedMsgFromAgent(char* pMsg, DWORD dwSize, char* pDec, WORD* pwSize)
{
	pDec[0] = pMsg[0];
	pDec[1] = pMsg[1];

	g_xform.Decode((LPBYTE)pMsg+2, (WORD)(dwSize-2), (LPBYTE)pDec+2, pwSize);
	*pwSize += 2;
}