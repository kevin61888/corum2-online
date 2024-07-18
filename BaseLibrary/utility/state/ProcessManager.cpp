//
// File: CProcessManager.cpp
// Desc: ProcessManager class
//
// Auth: Standil (cpascal@nownuri.net)
// Adapted from Role Playing Games with DirectX book (Jim Adams)
//

#include "../../StdAFX.h"
#include "ProcessManager.h"

CProcessManager::CProcessManager() : m_ProcessParent( NULL )
{
}

CProcessManager::~CProcessManager()
{
	sProcess *ProcessPtr;

	// Remove all processes from the stack
	while( (ProcessPtr = m_ProcessParent) != NULL ) {
		m_ProcessParent = ProcessPtr->pNext;
		delete ProcessPtr;
	}
}

BOOL Pop( void );
void PopAll( void );

void CProcessManager::Push( void (*Process)() )
{
	// Don't push a NULL value
	if( Process != NULL ) {
		// Allocate a new process and push it on stack
		sProcess *ProcessPtr = new sProcess;
		ProcessPtr->Function = Process;
		ProcessPtr->pNext = m_ProcessParent;
		m_ProcessParent = ProcessPtr;
	}
}

BOOL CProcessManager::Pop( void )
{
	sProcess *ProcessPtr = m_ProcessParent;

	// Remove the head of stack (if any)
	if( ProcessPtr != NULL ) {
		m_ProcessParent = ProcessPtr->pNext;
		delete ProcessPtr;
	}

	// return TRUE if more states exist, FALSE otherwise
	if( m_ProcessParent == NULL )
		return FALSE;
	return TRUE;
}

void CProcessManager::PopAll( void )
{
	while( Pop() == TRUE );
}

void CProcessManager::Process( void )
{
	sProcess *ProcessPtr = m_ProcessParent;

	while( ProcessPtr != NULL ) {
		ProcessPtr->Function();
		ProcessPtr = ProcessPtr->pNext;
	}
}

/*

e.g.

CProcessManager PM;

// Macro to ease the use of MessageBox function
#define MB(s) MessageBox( NULL, s, s, MB_OK );

void Func1() { MB("1"); }
void Func2() { MB("2"); }
void Func3() { MB("3"); }

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrev, LPSTR szCmdLine, int nCmdShow )
{
	PM.Add( Func1 );
	PM.Add( Func2 );
	PM.Add( Func3 );
	PM.Process();
	PM.Process();
}

*/
