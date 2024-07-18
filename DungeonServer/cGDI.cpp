#include "stdafx.h"
#include "cGDI.h"


cGDI::cGDI( HWND hWnd, int Width, int Height )
{
    m_hWnd          = NULL;
    m_Width         = 0;
    m_Height        = 0;
    m_hDC           = NULL;
    m_pSurface      = NULL;

    Create( hWnd, Width, Height );
}


void cGDI::Create( HWND hWnd, int Width, int Height )
{
    // 값들을 세팅하고
    m_hWnd  = hWnd;
    m_Width = Width;
    m_Height= Height;
    m_hDC   = GetDC( m_hWnd );

    // 값에 의해 스크린을 만들고
    m_pSurface = new WORD[ m_Width * m_Height ];
    memset( m_pSurface, 0x7ff, m_Width * m_Height * 2 );

    // 화면 블리트를 위해 비트맵 정보를 생성한다.
    m_pBMInfo = new BITMAPINFO;

    BITMAPINFOHEADER* pBMIHeader = (BITMAPINFOHEADER*) m_pBMInfo;
    ZeroMemory(pBMIHeader, sizeof(BITMAPINFOHEADER));

    pBMIHeader->biSize     = sizeof(BITMAPINFOHEADER);
    pBMIHeader->biWidth    = m_Width;
    pBMIHeader->biHeight   = -m_Height;
    pBMIHeader->biPlanes   = 1;
    pBMIHeader->biBitCount = 16;
}


cGDI::~cGDI()
{
	Destroy();
}


void cGDI::Destroy()
{
    if( m_pBMInfo )
	{
		delete m_pBMInfo;
		m_pBMInfo = NULL;
	}

    if( m_hDC )
    {
            ReleaseDC( m_hWnd, m_hDC );
            m_hDC   = NULL;
    }

    if( m_pSurface )
    {
            delete[] m_pSurface;
            m_pSurface = NULL;
    }
}


// 스크린 버퍼를 지정한 색으로 채운다.
void cGDI::_clearSurface( WORD color )
{
    memset( m_pSurface, color, m_Width * m_Height * 2 );
}


// 백스크린을 메인 화면으로 블리트 한다.
void cGDI::Blt( int nx, int ny, int eyes )
{
    if( !eyes )
    {
            SetDIBitsToDevice(m_hDC ,
                            nx , ny ,
                            m_Width ,
                            m_Height ,
                            0 ,
                            m_Height ,
                            m_Height ,
                            m_Height ,
                            m_pSurface ,
                            (BITMAPINFO*) m_pBMInfo,
                            DIB_RGB_COLORS);
    }
    else
    {
            StretchDIBits(m_hDC ,
                         nx , ny ,
                         m_Width ,
                         m_Height ,
                         eyes ,
                         eyes,
                         m_Width - (eyes * 2) ,
                         m_Height - (eyes *2),
                         m_pSurface,
                         (BITMAPINFO*)m_pBMInfo ,
                         DIB_RGB_COLORS , SRCCOPY);
    }
}


// 가로 라인을 그린다.
void cGDI::DrawXline( int x1, int y1, int len, WORD color)
{
    // 클리핑...
	if( y1 < 0 || y1 >= m_Height) return;

	if( x1 < 0 ) {
		len += x1;
		x1 = 0;
		if(len <= 0) return;
	}

	if( x1+len >= m_Width ) {
		len -= (x1 + len) - m_Width;
		if(len <= 0 ) return;
	}

	WORD*	dest;
	dest = m_pSurface;
	dest += y1 * m_Width+x1;
	for(int i=0; i<len; i++)
		dest[i] = color;
}


void cGDI::DrawYline( int x1, int y1, int len, WORD color)
{
    if( x1 < 0 || x1 >= m_Width) return;

	if( y1 < 0 )
	{
		len += y1;
		y1 = 0;
		if(len <= 0) return;
	}
	if( y1+len >= m_Height )
	{
		len -= (y1 + len) - m_Height;
		if(len <= 0 ) return;
	}

	WORD*	dest;
	dest = m_pSurface;
	dest += y1*m_Width+x1;
	for(int i=0; i<len; i++)
		dest[m_Width*i] = color;
}

void cGDI::DrawBox( int x1, int y1, int x2, int y2, WORD color)
{
    int xMin, xMax;
	int yMin, yMax;

	xMin = min(x1, x2);
	xMax = max(x1, x2);
	yMin = min(y1, y2);
	yMax = max(y1, y2);
	
	x1 = xMin;
	x2 = xMax;
	y1 = yMin;
	y2 = yMax;

	DrawXline(x1, y1, x2-x1, color);
	DrawXline(x1, y2, x2-x1, color);
	DrawYline(x1, y1, y2-y1, color);
	DrawYline(x2, y1, y2-y1, color);
}

void cGDI::DrawFillBox( int x1, int y1, int x2, int y2, WORD color)
{
    int i;
	
	int xMin, xMax;
	int yMin, yMax;

	xMin = min(x1, x2);
	xMax = max(x1, x2);
	yMin = min(y1, y2);
	yMax = max(y1, y2);
	
	x1 = xMin;
	x2 = xMax;
	y1 = yMin;
	y2 = yMax;

	for( i=0; i< y2-y1; i++ )
	{
		DrawXline( x1, y1+i, x2-x1, color );
	}
}

void cGDI::PutPixel( int x, int y, WORD color )
{
	m_pSurface[y*m_Width+x] = color;
}