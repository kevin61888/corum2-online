// ComboBox.h: interface for the CComboBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMBOBOX_H__5B6692F1_D30C_444E_ABCE_226AC4106BD4__INCLUDED_)
#define AFX_COMBOBOX_H__5B6692F1_D30C_444E_ABCE_226AC4106BD4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "voidlist.h"
struct COMBOBOX
{
	char szComboName[30];	// 콤보 리스트에 나타날 이름.
	void* pDesc;			// 데이타 포인터.
	DWORD dwIndex;			// 콤보박스에 등록된 리스트 인덱스. 
	
};
class CComboList : public CVoidList
{
	void FreeData(void* pData)
	{
		if (pData)
			delete (COMBOBOX*)pData;
		pData = NULL;
	}
};
struct V2_SPRITE;
interface IDIFontObject;
class CComboBox  
{
	CComboList*			m_pComboList;				// 콤보박스에 등록이 되어잇는 리스트
	POSITION_			m_pComboDisplayFirst;		// 디스플레이될 첫번째 리스트.
	DWORD				m_dwComboListMaxDisplay;	// 디스플레이될 최대 목록개수.
	POSITION_			m_pComboCurSel;				// 현재 선택된 목록
	DWORD				m_dwScrollRecoverY;				// 스크롤 버튼 y좌표. 창 움직일때 필요하다.
	V2_SPRITE*			m_pSprParent;
	V2_SPRITE*			m_pSprSelectBar;			
	V2_SPRITE*			m_pSprBoard;
	V2_SPRITE*			m_pSprScrollButton;
	V2_SPRITE*			m_pSprButton[2];

	BOOL				m_bScrollClick;
	BOOL				m_bUpChk;
public:
	void RemoveSprite();							// 콤보박스에 필요했던 스프라이트를 지워라.
	void SetActive(BOOL bActive);					// 활성화 처리.
	void OnMove();									// 부모 윈도우가 움직였을때 처리.
	int MouseEventCheck();							// 프레임 마다 마우스상태를 체크하고, 콤보윈도우와 충돌하면 리턴값은 1
	void RenderList(IDIFontObject* pFont, int nOrder); // 콤보박스 목록을 디스플레이하라.
	char* GetSelectedString();						// 현재 선택되어진 콤보박스목록의 스트링 값을 얻어와라.
	void SetOrder();								// 스프라이트 뿌려질때의 순서를 지정한다.
	
	V2_SPRITE* CreateSpriteComboButtonDown(DWORD dwSprID, float fRelativeX, float fRelativeY, int nStartX, int nStartY, int nWidth, int nHeight, bool bShow, int nOrder, BYTE bAlpha = 0xff); // 콤보박스 버튼
	V2_SPRITE* CreateSpriteComboSelectBar(DWORD dwSprID, float fRelativeX, float fRelativeY, int nStartX, int nStartY, int nWidth, int nHeight, bool bShow, int nOrder, BYTE bAlpha = 0xff);	// 콤보박스 목록셀렉트바
	V2_SPRITE* CreateSpriteComboBoard(DWORD dwSprID, float fRelativeX, float fRelativeY, int nStartX, int nStartY, int nWidth, int nHeight, bool bShow, int nOrder, BYTE bAlpha = 0xff);	// 콤보박스 목록바
	V2_SPRITE* CreateSpriteComboScrollButton(DWORD dwSprID, float fRelativeX, float fRelativeY, int nStartX, int nStartY, int nWidth, int nHeight, bool bShow, int nOrder, BYTE bAlpha = 0xff);	// 콤보박스 스크롤 버튼
	V2_SPRITE* CreateSpriteComboButtonNormal(DWORD dwSprID, float fRelativeX, float fRelativeY, int nStartX, int nStartY, int nWidth, int nHeight, bool bShow, int nOrder, BYTE bAlpha = 0xff); // 콤보박스 버튼
	void Initialize(V2_SPRITE* pSprParent, DWORD dwMaxDisplay);	// pSprParent 부모가 될 창윈도우 포인터, dwMaxDisplay 콤보박스를 열었을때 화면에 나타날수 있는 최대크기.
	int GetCount();		// 리스트에 등록되어 있는 갯수.
	void AddTail(char* szName, void* pDesc = NULL);	// 콤보박스에 추가
	void RemoveHead();
	CComboBox();
	~CComboBox();
private:
	void OnScroll();								// 스크롤 처리.
};

#endif // !defined(AFX_COMBOBOX_H__5B6692F1_D30C_444E_ABCE_226AC4106BD4__INCLUDED_)
