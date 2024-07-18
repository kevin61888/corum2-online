#pragma once

#define SAFE_UNINIT(p)			{ if (p) { (p)->Uninitialize(); (p)->Release(); (p) = NULL;						} }
#define SAFE_RELEASE(p)			{ if (p) { (p)->Release(); (p) = NULL;											} }
#define SAFE_DELETE(p)			{ if (p) { delete (p); (p) = NULL;												} }
#define SAFE_DELETEOBJECT(p)	{ if (p) { ::DeleteObject(p); (p) = NULL;										} }
#define SAFE_DELETE_ARRAY(p)	{ if (p) { delete[] (p);   (p)=NULL;											} }
#define SAFE_FREE(p)			{ if (p) { free(p); (p) = NULL;													} }
#define SAFE_CLOSESOCKET(p)		{ if (p != INVALID_SOCKET) { closesocket(p); (p) = INVALID_SOCKET;				} }
#define SAFE_FCLOSE(p)			{ if (p) { fclose(p); (p) = NULL;												} }
#define SAFE_CLOSEHANDLE(p)		{ if (p != INVALID_HANDLE_VALUE) { CloseHandle(p); (p) = INVALID_HANDLE_VALUE;	} }
#define SAFE_DESTROYWINDOW(p)	{ if (p) { DestroyWindow(p); (p) = NULL;										} }
#define SAFE_DESTROYMENU(p)		{ if (p) { DestroyMenu(p); (p) = NULL;											} }

// constants definition
const int	DEFAULT_REFCOUNT		= 1;
