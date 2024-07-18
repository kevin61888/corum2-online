#ifndef __THROW_QUERY_H__
#define __THROW_QUERY_H__

#pragma once

class CUser;

void ThrowLogoutQuery(CUser* pUser);
int ThrowLogoutAllQuery(BOOL bCallback = TRUE);
void ThrowLoginQuery(CUser* pUser, char* szID, char* szPassword);
void ThrowInsertWebBoardQuery(CUser* pUser, char* szName, WORD wdClass);
void ThrowDeleteWebBoardQuery(CUser* pUser, char* szCharName);
void ThrowInsertCharQuery(CUser* pUser, char* szCharName, BYTE btCharNum, WORD wdHead, BYTE btClass, WORD wdMapID);
void ThrowDeleteCharQuery(CUser* pUser, char* szCharName);
void ThrowItemMallQuery(CUser* pUser);
void ThrowCreateCharInitBinaryQuery(CUser* pUser);

void ThrowBlockTitleQuery(CUser* pUser, BOOL bBlock);

#endif // __THROW_QUERY_H__