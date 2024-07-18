//
// File: CStateManager.h
// Desc: StateManager class
//
// Auth: Standil (cpascal@nownuri.net)
// Adapted from Role Playing Games with DirectX book (Jim Adams)
//

#ifndef _LOWLIB_STATEMANAGEREX_H_
#define _LOWLIB_STATEMANAGEREX_H_

const LONG PURPOSE_NONE			= 0;
const LONG PURPOSE_INITIALIZE	= 1;			//초기화 프로시져(각종 데이터 준비 및 초기화)
const LONG PURPOSE_SHUTDOWN		= 2;			//프로시져 종료
const LONG PURPOSE_FRAME		= 3;			//실제 실행 프로시져
const LONG PURPOSE_USER			= 4;			//기타 용도의 프로시져

#define PSEXCMD_RETTYPE			void
#define PSEXCMD_ARGUMENT		void *pPointer, LONG lPurpose
#define PSEXCMD(name)			PSEXCMD_RETTYPE name( PSEXCMD_ARGUMENT )

typedef	PSEXCMD_RETTYPE (*PSExFunctionType)( PSEXCMD_ARGUMENT );

class CStateManagerEx
{
	// A structure that stores a function pointer and linked list
	typedef struct sState {
		PSEXCMD_RETTYPE (*Function)( PSEXCMD_ARGUMENT );
		sState *pNext;
	} sStateEx;

protected :
	sStateEx *m_StateParent;	// The top state in the stack
								// the head of the stack
public:
	CStateManagerEx( void );
	~CStateManagerEx( void );

	void Push( void (*Function)( void *pPointer, LONG dwPurpose ), void *pDataPointer = NULL );
	BOOL Pop( void *pDataPointer = NULL );
	void PopAll( void *pDataPointer = NULL );
	BOOL Process( void *pDataPointer = NULL, LONG lPurpose = PURPOSE_FRAME );
	void Set( PSEXCMD_RETTYPE (*Function)( PSEXCMD_ARGUMENT ), void *pDataPointer = NULL );
	PSExFunctionType GetState( void );
};

#endif /* _LOWLIB_STATEMANAGEREX_H_ */

