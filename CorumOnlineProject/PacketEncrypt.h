#ifndef __PACKET_ENCRYPTION_H__
#define __PACKET_ENCRYPTION_H__


#pragma once

#include "DFXForm.h"


#define ENCRYPT_KEY_LEN		16


void RandomKeyGenerator(LPBYTE btBuf);
void CalculateKey(LPBYTE btClientKey, LPBYTE btServerKey, LPBYTE btKey);
void SetClientKey(LPBYTE byClientKey);
LPBYTE GetClientKey();


void SetEncryptionKey(LPBYTE btKey);
void ResetEncryptionKey();


void SendMsgWithBlowfish(char* pMsg, DWORD dwSize, BYTE btServerType);
void DecryptReceivedMsgFromAgent(char* pMsg, DWORD dwSize, char* pDec, WORD* pwSize);


#endif // __PACKET_ENCRYPTION_H__