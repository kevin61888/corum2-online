#ifndef __PACKET_ENCRYPT_H__
#define __PACKET_ENCRYPT_H__

#pragma once

class CUser;

#define ENCRYPT_KEY_LEN		16

void RandomKeyGenerator(LPBYTE btBuf);
void CalculateKey(LPBYTE btClientKey, LPBYTE btServerKey, LPBYTE btKey);


void SendToUserWithoutEncryption(CUser* pUser, char* pMsg, DWORD dwPacketSize, DWORD dwFlag);
void SendToUserWithoutEncryption(DWORD dwConnectionIndex, char* pMsg, DWORD dwPacketSize, DWORD dwFlag);

#ifdef __PACKET_ENCRYPTION
void SendToUserWithEncryption(CUser* pUser, char* pMsg, DWORD dwPacketSize, DWORD dwFlag);
void SendToUserWithEncryption(DWORD dwConnectionIndex, char* pMsg, DWORD dwPacketSize, DWORD dwFlag);
#endif

void SendToUser(CUser* pUser, char* pMsg, DWORD dwPacketSize, DWORD dwFlag);
void SendToUser(DWORD dwConnectionIndex, char* pMsg, DWORD dwPacketSize, DWORD dwFlag);
#ifdef __PACKET_ENCRYPTION
void DecryptReceivedMsg(CUser* pUser, char* pMsg, DWORD dwSize, char* pDec, WORD* pwSize);
void DecryptReceivedMsgWithBaseKey(char* pMsg, DWORD dwSize, char* pDec, WORD* pwSize);
#endif

#endif