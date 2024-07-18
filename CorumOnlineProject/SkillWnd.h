//======================================================//
// Code by Jang.							2002.12.23	//
// Obejct : Skill Window.								//
//======================================================//
#ifndef		__SKILL_WINDOW_INC__
#define		__SKILL_WINDOW_INC__

#include	"Menu.h"

#define SPR_OBJ_SKILL_WND1				0
#define SPR_OBJ_SKILL_WND2				1
#define SPR_OBJ_SKILL_CLOSE1			2	
#define SPR_OBJ_SKILL_CLOSE2			3
#define SPR_OBJ_SKILL_MANA1				4
#define SPR_OBJ_SKILL_MANA2				5
#define SPR_OBJ_SKILL_MANA3				6
#define SPR_OBJ_SKILL_AURA1				7
#define SPR_OBJ_SKILL_AURA2				8
#define SPR_OBJ_SKILL_AURA3				9
#define SPR_OBJ_SKILL_DIVINE1			10
#define SPR_OBJ_SKILL_DIVINE2			11
#define SPR_OBJ_SKILL_DIVINE3			12
#define SPR_OBJ_SKILL_SUMMON1			13
#define SPR_OBJ_SKILL_SUMMON2			14
#define SPR_OBJ_SKILL_SUMMON3			15
#define SPR_OBJ_SKILL_CHAKRA1			16
#define SPR_OBJ_SKILL_CHAKRA2			17
#define SPR_OBJ_SKILL_CHAKRA3			18

#define SPR_OBJ_SKILL_UP1				19
#define SPR_OBJ_SKILL_UP2				20
#define SPR_OBJ_SKILL_UP3				21
#define SPR_OBJ_SKILL_UP4				22
#define SPR_OBJ_SKILL_UP5				23
#define SPR_OBJ_SKILL_UP6				24
#define SPR_OBJ_SKILL_UP7				25
#define SPR_OBJ_SKILL_UP8				26
#define SPR_OBJ_SKILL_UP9				27
#define SPR_OBJ_SKILL_UP10				28
#define SPR_OBJ_SKILL_UP11				29
#define SPR_OBJ_SKILL_UP12				30
#define SPR_OBJ_SKILL_UP13				31
#define SPR_OBJ_SKILL_UP14				32
#define SPR_OBJ_SKILL_UP15				33
#define SPR_OBJ_SKILL_UP16				34
#define SPR_OBJ_SKILL_UP17				35
#define SPR_OBJ_SKILL_UP18				36
#define SPR_OBJ_SKILL_UP19				37
#define SPR_OBJ_SKILL_UP20				38
#define SPR_OBJ_SKILL_UP21				39
#define SPR_OBJ_SKILL_UP22				40
#define SPR_OBJ_SKILL_UP23				41
#define SPR_OBJ_SKILL_UP24				42
#define SPR_OBJ_SKILL_UP25				43
#define SPR_OBJ_SKILL_UP26				44
#define SPR_OBJ_SKILL_UP27				45
#define SPR_OBJ_SKILL_UP28				46
#define SPR_OBJ_SKILL_UP29				47
#define SPR_OBJ_SKILL_UP30				48
#define SPR_OBJ_SKILL_UP31				49
#define SPR_OBJ_SKILL_UP32				50
#define SPR_OBJ_SKILL_UP33				51
#define SPR_OBJ_SKILL_UP34				52
#define SPR_OBJ_SKILL_UP35				53
#define SPR_OBJ_SKILL_UP36				54
#define SPR_OBJ_SKILL_UP37				55
#define SPR_OBJ_SKILL_UP38				56
#define SPR_OBJ_SKILL_UP39				57
#define SPR_OBJ_SKILL_UP40				58
#define SPR_OBJ_SKILL_UP41				59
#define SPR_OBJ_SKILL_UP42				60
#define SPR_OBJ_SKILL_UP43				61
#define SPR_OBJ_SKILL_UP45				62
#define SPR_OBJ_SKILL_UP46				63
#define SPR_OBJ_SKILL_UP47				64
#define SPR_OBJ_SKILL_UP48				65
#define SPR_OBJ_SKILL_UP49				66
#define SPR_OBJ_SKILL_UP50				67

#define SKILL_ICON_SIZE					32

class CSkillWnd : public CMenu
{
public:

	BYTE				m_byBitClassType;

	BYTE				m_bySkillType;
	BYTE				m_bySkillIndex;
	BYTE				m_bClassType[5];
	
	BOOL				m_bMoveChk;		
	BOOL				m_bBtnChk[2];
	
	IDISpriteObject*	m_pLineSpr;	
	IDISpriteObject*	m_pAttackSpr;

public:

	// Singleton Patten //
private:
	static CSkillWnd* c_pThis;

public:
	static CSkillWnd*	GetInstance()		{ if(!c_pThis) c_pThis = new CSkillWnd; return c_pThis; }
	static void		DestroyInstance()	{ if(c_pThis) { delete c_pThis; c_pThis = NULL;} }


	// 공통으로 사용되는 것들은 재정의 한다 //
	BOOL	Init();
	

	void	CreateSkillResourceSpr( COnlyList* pSkillList );
	void	ReleaseSkillResourceSpr( COnlyList* pSkillList );

	void	RenderText();	
	void	Remove();	
	void	SetOrder();	
	void	SetActive(BOOL bActive = TRUE);	
	
	int		CheckInterface();

	// 체크 관련 함수 //			
	void	SetTypePos();
	void	SetSkillButtonPos();
	void	SetSkillUpBtn();
	void	SetSkillUpPos();
	void	RenderSkill();
	void	RenderSkillInfo();
	void	SetRenderSkillInfo(LP_SKILL_RESOURCE_EX lpSkillResourceEx, int nIndex, int nPosX, int nPosX2, int nPosY, int nPosY2);	

	BOOL	CheckSkillIfno(int nIndex, int nPosX, int nPosY, int nPosX2, int nPosY2);

	void	RenderSkillIcon();
	void	RenderUsing();

	CSkillWnd();
	virtual ~CSkillWnd();
};

#endif
//======================================================//
// End.													//
//======================================================//