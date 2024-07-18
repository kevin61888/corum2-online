#pragma once
#include "Voidlist.h"
#ifdef SS3D_TEST
#include "./../SS3D_0719/4dyuchigx/4DyuchiGXGFunc/global.h"
#else
#include "./../4DyuchiGXGFunc/global.h"
#endif



//CVoidList를 상속받은 클래스로 실제 Data의 삭제 과정 없이..  
//연결만 해준다...     또다른 차이점은 Node를 풀로 생성 

class COnlyList : public CVoidList
{
	STMPOOL_HANDLE			m_pNodePool;
public:
	
	CNode*	NewNode(CNode* pPrev, CNode* pNext);
	void	FreeNode(CNode* pNode);

	void	RemoveAllData();

	void	(*DeleteData)();
	
	COnlyList(DWORD dwMaxNodePoolNum, void (*DeleteFun)() = NULL);
	~COnlyList();
};

