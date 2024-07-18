#ifndef _GDI_H
#define _GDI_H

#include <windows.h>
#include <stdio.h>

#define  SHADOW_555		0x7bde7bde
#define  SHADOW_565		0xf7def7de

class cGDI
{
private:
        HWND			m_hWnd;
        HDC				m_hDC;
        int				m_Width;
        int				m_Height;
        LPWORD			m_pSurface;

        BITMAPINFO *	  m_pBMInfo;
        BITMAPINFO *      pReduceBMInfo;

public:
        // Get Function!
        HWND    GetHWND()       { return m_hWnd; }
        int     GetWidth()      { return m_Width;}
        int     GetHeight()     { return m_Height;}
        LPWORD  GetSurface()    { return m_pSurface;}

public:
        void Create( HWND hWnd, int Width, int Height );
        void Destroy();

        cGDI( HWND hWnd, int Width, int Height );
        ~cGDI();

public:
        // Graphics Function!
        void _clearSurface( WORD color );
        void DrawXline( int x1, int y1, int len, WORD color);
		void DrawYline( int x1, int y1, int len, WORD color);
		void DrawBox( int x1, int y1, int x2, int y2, WORD color);
		void DrawFillBox( int x1, int y1, int x2, int y2, WORD color);
		void PutPixel( int x, int y, WORD color );
        void Blt( int nx, int ny, int eyes );
};

#endif