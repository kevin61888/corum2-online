#ifndef _FILEUTILITY_H_
#define _FILEUTILITY_H_

extern HANDLE AdvCreateFile( LPCTSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile );

#endif /* _FILEUTILITY_H_ */