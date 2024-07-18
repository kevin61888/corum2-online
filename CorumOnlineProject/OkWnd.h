//======================================================//
// Code by Jang.							2003.04.03	//
// Object : Ok Wnd.										//
//======================================================//
#ifndef		__OKWND_INC__
#define		__OKWND_INC__
//==============================================
//	0 bit ==>	포탈 탈지플래그
//	1 bit ==>	마을 스크롤
//	2 bit ==>	던전 포탈
//	3 bit ==>	포탈 좌표 세이브
//==============================================

#define	__OKWND_TYPE_POTAL_VILLAGE_USE	1		// 마을 스크롤 사용하기 위해서.
#define	__OKWND_TYPE_POTAL_DUNGEON_USE	2		// 던전 포탈 사용하기 위해서.
#define	__OKWND_TYPE_POTAL_SAVE			4		// 포탈 세이브를 위해서.
#define	__OKWND_TYPE_POTAL_ENTRANCE		8		// 입장료 물어보기 위해서.
#define __OKWND_TYPE_UNPACK_PRESENT_BOX	16		// 선물상자(태극기) 아이템 푸는것을 물어보기 위해서 
#define	__OKWND_TYPE_DUNGEON_ENTER		32		// 던전 입장할래?

#define SPR_OBJ_OK_CREATE_WND		0
#define SPR_OBJ_OK_CLOSE1			1
#define SPR_OBJ_OK_CLOSE2			2
#define SPR_OBJ_OK_OK1				3
#define SPR_OBJ_OK_OK2				4
#define SPR_OBJ_OK_OK3				5
#define SPR_OBJ_OK_CANCEL1			6
#define SPR_OBJ_OK_CANCEL2			7
#define SPR_OBJ_OK_CANCEL3			8

#include	"Menu.h"
class DUNGEON_DATA_EX;
class COkWnd : public CMenu
{
public:	
	
	BOOL	m_bBtn[2];

	char	m_szInfo[0xff];
	RECT	m_rtPos;
	
	BYTE	m_bZipCode;	
	BYTE	m_bType;	// BIT 체계를 쓴다.... 위에 있는거 참조 ㅡ.ㅡ
	BYTE	m_byRenderType;
	DWORD	m_dwDungeonID;

public:
	
	// Singleton Patten //
private:
	static COkWnd* c_pThis;

public:
	static COkWnd*	GetInstance()		{ if(!c_pThis) c_pThis = new COkWnd; return c_pThis; }
	static void		DestroyInstance()	{ if(c_pThis) { delete c_pThis; c_pThis = NULL;} }


	// 공통으로 사용되는 것들은 재정의 한다 //
	BOOL	Init();


	void	RenderText();	
	void	Remove();
	void	SetOrder();	
	void	SetActive(BOOL bActive = TRUE);
	
	int		CheckInterface();

	// 체크 관련 함수 //

	COkWnd();
	virtual ~COkWnd();
};

#endif
//======================================================//
// End.													//
//======================================================//