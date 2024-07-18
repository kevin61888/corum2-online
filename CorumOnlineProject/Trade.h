//==============================================================//
// Code by Jang.									2003.08.01	//
// Object : 교환 관련 작업.										//
//==============================================================//
#ifndef		__TRADE_INC__
#define		__TRADE_INC__

#include	"stdafx.h"


class CItem;


//==============================================================//
// Function.													//
//==============================================================//
void CmdTradeRequest(char* pMsg, DWORD dwLen);
void CmdTradeResult(char* pMsg, DWORD dwLen);
void CmdTradeCancel(char* pMsg, DWORD dwLen);
void CmdTradeOk( char* pMsg, DWORD dwLen );
void CmdTradeItem( char* pMsg, DWORD dwLen );
void CmdTradeTrade( char* pMsg, DWORD dwLen );
void CmdTradeRetrun( char* pMsg, DWORD dwLen );
void CmdTradeData( char* pMsg, DWORD dwLen );
void CmdTradeCancelFail( char* pMsg, DWORD dwLen );
void CmdTradeCancelReturnInv(char* pMsg, DWORD dwLen);


void Trade_SmallItem(CItem* pItem);

#endif
//==============================================================//
// End.															//
//==============================================================//