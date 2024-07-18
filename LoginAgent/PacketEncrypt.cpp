#include "stdafx.h"
#include "PacketEncrypt.h"
#include "User.h"
#include "UserTable.h"


extern CUserTable* g_pUserTable;


#ifdef __PACKET_ENCRYPTION
void SendToUserWithEncryption(CUser* pUser, char* pMsg, DWORD dwPacketSize, DWORD dwFlag)
{
	// 상위 2 바이트는 헤더이므로, 암호화하지 않는다.
	WORD	wdEncSize = 0;
	BYTE	pEncryptedMsg[4096] = {0xdd,};	

	pEncryptedMsg[0] = pMsg[0];
	pEncryptedMsg[1] = pMsg[1];

	pUser->Encode(LPBYTE(pMsg + 2), WORD(dwPacketSize - 2), pEncryptedMsg+2, &wdEncSize);
	if(pEncryptedMsg[2] > 8)
	{
		Log(LOG_IMPORTANT, "Illegal Encryption Padding Size In Send!");
		return;
	}

	g_pNet->SendToUser(pUser->GetConnectionIndex(), (char*)pEncryptedMsg, wdEncSize+2, dwFlag);
}


void SendToUserWithEncryption(DWORD dwConnectionIndex, char* pMsg, DWORD dwPacketSize, DWORD dwFlag)
{
	// 상위 2 바이트는 헤더이므로, 암호화하지 않는다.
	static DFXform XF;

	BYTE	pEncryptedMsg[4096] = {0,};
	WORD	wdEncSize = 0;

	pEncryptedMsg[0] = pMsg[0];
	pEncryptedMsg[1] = pMsg[1];

	CUser* pUser = g_pUserTable->GetUserInfo(dwConnectionIndex);
	if(pUser)
	{
		pUser->Encode(LPBYTE(pMsg + 2), WORD(dwPacketSize - 2), pEncryptedMsg+2, &wdEncSize);
	}
	else
	{
		XF.Encode(LPBYTE(pMsg + 2), WORD(dwPacketSize - 2), pEncryptedMsg+2, &wdEncSize);
	}	

	if(pEncryptedMsg[2] > 8)
	{
		Log(LOG_IMPORTANT, "Illegal Encryption Padding Size In Send!");
		return;
	}

	g_pNet->SendToUser(dwConnectionIndex, (char*)pEncryptedMsg, wdEncSize+2, dwFlag);
}


void DecryptReceivedMsg(CUser* pUser, char* pMsg, DWORD dwSize, char* pDec, WORD* pwSize)
{
	if((BYTE)pMsg[2] > 8)
	{
		Log(LOG_IMPORTANT, "Illegal Encryption Padding Size In Receive!, PaddingSize: %u", pDec[2]);
		return;
	}

	pDec[0] = pMsg[0];
	pDec[1] = pMsg[1];

	pUser->Decode((LPBYTE)pMsg+2, (WORD)dwSize-2, (LPBYTE)pDec+2, pwSize);
	*pwSize += 2;
}


void DecryptReceivedMsgWithBaseKey(char* pMsg, DWORD dwSize, char* pDec, WORD* pwSize)
{
	static DFXform XF;

	if((BYTE)pMsg[2] > 8)
	{
		Log(LOG_IMPORTANT, "Illegal Encryption Padding Size In Receive!, PaddingSize: %u", pDec[2]);
		return;
	}

	pDec[0] = pMsg[0];
	pDec[1] = pMsg[1];
	
	XF.Decode((LPBYTE)pMsg+2, (WORD)dwSize-2, (LPBYTE)pDec+2, pwSize);
	*pwSize += 2;
}
#endif 


void RandomKeyGenerator(LPBYTE btBuf)
{
	srand( (unsigned)time( NULL ) );

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

void SendToUserWithoutEncryption(CUser* pUser, char* pMsg, DWORD dwPacketSize, DWORD dwFlag)
{
	SendToUserWithoutEncryption(pUser->GetConnectionIndex(), pMsg, dwPacketSize, dwFlag);
}

void SendToUserWithoutEncryption(DWORD dwConnectionIndex, char* pMsg, DWORD dwPacketSize, DWORD dwFlag)
{
	g_pNet->SendToUser(dwConnectionIndex, pMsg, dwPacketSize, dwFlag);
}


void SendToUser(CUser* pUser, char* pMsg, DWORD dwPacketSize, DWORD dwFlag)
{
#ifdef __PACKET_ENCRYPTION
	SendToUserWithEncryption(pUser, pMsg, dwPacketSize, dwFlag);
#else
	SendToUserWithoutEncryption(pUser, pMsg, dwPacketSize, dwFlag);
#endif 
}

void SendToUser(DWORD dwConnectionIndex, char* pMsg, DWORD dwPacketSize, DWORD dwFlag)
{
#ifdef __PACKET_ENCRYPTION
	SendToUserWithEncryption(dwConnectionIndex, pMsg, dwPacketSize, dwFlag);
#else
	SendToUserWithoutEncryption(dwConnectionIndex, pMsg, dwPacketSize, dwFlag);
#endif 
}

