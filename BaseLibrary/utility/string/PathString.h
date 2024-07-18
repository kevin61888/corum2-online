#ifndef _PATHSTRING_H_
#define _PATHSTRING_H_

extern void AdvSplitPath( register TCHAR *szFullPath, TCHAR *szPath, TCHAR *szFilename );
extern void AdvSplitPath( register TCHAR *szFullPath, TCHAR *szPath, TCHAR *szFilename, TCHAR *szExtName );
extern int AdvGetAppPath( LPTSTR lpszPath, DWORD dwLength );

#endif
