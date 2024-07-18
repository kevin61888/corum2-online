//======================================================//
// Code by Jang.							2002.12.06	//
// Object : Script.										//
//======================================================//
#ifndef		__SCRIPT_INC__
#define		__SCRIPT_INC__

#include	<string.h>
#include	"define.h"
#include	"struct.h"

class CScript
{	
private:

	char	m_szId[MAX_ID_LENGTH];
	char	m_szText[__MAX_MESSGAE__];
	
public:
	
	char	m_szName[MAX_CHARACTER_NAME_LENGTH];
		
public:
	
	SSCRIPT			m_sScript;

	int				MsgCheck();
	int				ChkCommand();
	int				MsgExecute(int nServer);

	static CScript* GetInstance(char* pszText, BOOL bChk = FALSE)
	{
		static CScript cScript;		

		if(bChk==TRUE)
			return &cScript;

		lstrcpy(cScript.m_szText, pszText);
		return &cScript;
	};
	
	CScript();
	virtual ~CScript();
};

#endif
//======================================================//
// End.													//
//======================================================//