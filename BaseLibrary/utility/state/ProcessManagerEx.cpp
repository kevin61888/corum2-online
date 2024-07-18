//
// File: CProcessManagerEx.cpp
// Desc: ProcessManager class
//
// Auth: Standil (cpascal@nownuri.net)
// Adapted from Role Playing Games with DirectX book (Jim Adams)
//

#include "../../StdAFX.h"
#include "ProcessManagerEx.h"

CProcessManagerEx::CProcessManagerEx( void ) : m_ProcessParent( NULL )
{
}

CProcessManagerEx::~CProcessManagerEx( void )
{
	PopAll();
}

void CProcessManagerEx::Push( PSEXCMD_RETTYPE (*Process)( PSEXCMD_ARGUMENT ), void *pDataPointer )
{
	if( Process == NULL )	// Don't push a NULL value
		return;
	// Allocate a new process and push it on stack
	sProcess *ProcessPtr	= new sProcess;
	ProcessPtr->Function	= Process;
	ProcessPtr->pNext		= m_ProcessParent;
	m_ProcessParent			= ProcessPtr;
	ProcessPtr->Function( pDataPointer, PURPOSE_INITIALIZE );
}

void CProcessManagerEx::PushBack( PSEXCMD_RETTYPE (*Process)( PSEXCMD_ARGUMENT ), void *pDataPointer )
{
	if( Process == NULL )	// Don't push a NULL value
		return;
	if( m_ProcessParent == NULL ) {
		Push( Process, pDataPointer );
		return;
	}

	sProcess *LastProcessPtr;
	for( LastProcessPtr = m_ProcessParent; LastProcessPtr && (LastProcessPtr->pNext != NULL); LastProcessPtr = LastProcessPtr->pNext );

	// Allocate a new process and push_back it on stack
	sProcess *ProcessPtr	= new sProcess;
	ProcessPtr->Function	= Process;
	ProcessPtr->pNext		= NULL;
	LastProcessPtr->pNext	= ProcessPtr;
	ProcessPtr->Function( pDataPointer, PURPOSE_INITIALIZE );
}

BOOL CProcessManagerEx::Pop( void *pDataPointer )
{
	sProcess *ProcessPtr = m_ProcessParent;

	// Remove the head of stack (if any)
	if( ProcessPtr != NULL ) {
		ProcessPtr->Function( pDataPointer, PURPOSE_SHUTDOWN );
		m_ProcessParent = ProcessPtr->pNext;
		delete ProcessPtr;
	}

	// return TRUE if more states exist, FALSE otherwise
	if( m_ProcessParent == NULL )
		return FALSE;
	return TRUE;
}

void CProcessManagerEx::PopAll( void *pDataPointer )
{
	while( Pop( pDataPointer ) == TRUE );
}

void CProcessManagerEx::Process( void *pDataPointer, LONG lPurpose )
{
	sProcess *ProcessPtr = m_ProcessParent;

	while( ProcessPtr != NULL ) {
		ProcessPtr->Function( pDataPointer, lPurpose );
		ProcessPtr = ProcessPtr->pNext;
	}
}

/*

e.g.

CProcessManagerEx PM;

// Macro to ease the use of MessageBox function
#define MB(s) MessageBox( NULL, s, s, MB_OK );

void Func1() { MB("1"); }
void Func2() { MB("2"); }
void Func3() { MB("3"); }

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrev, LPSTR szCmdLine, int nCmdShow )
{
	PM.Push( Func1 );
	PM.Add( Func2 );
	PM.Add( Func3 );
	PM.Process();
	PM.Process();
}

*/
