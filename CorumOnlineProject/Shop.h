//==============================================================//
// Code by Jang.									2003.08.01	//
// Object : 노점 관련 작업.										//
//==============================================================//
#ifndef		__SHOP_INC__
#define		__SHOP_INC__

#include	"stdafx.h"

//==============================================================//
// Function.													//
//==============================================================//
void	CmdShopFail(char* pMsg, DWORD dwLen);
void	CmdPlayerShopItem(char* pMsg, DWORD dwLen);
void	CmdPlayerShopItem_ReturnInv(char* pMsg, DWORD dwLen);
void	CmdPlayerShopSell(char* pMsg, DWORD dwLen);
void	CmdPlayerShopSoldOut(char* pMsg, DWORD dwLen);
void	CmdShopItem(char* pMsg, DWORD dwLen);
void	CmdPlayerShopOpen(char* pMsg, DWORD dwLen);
void	CmdPlayerShopClose(char* pMsg, DWORD dwLen);
void	CmdPlayerShopShowAll(char* pMsg, DWORD dwLen);

#endif
//==============================================================//
// End.															//
//==============================================================//

