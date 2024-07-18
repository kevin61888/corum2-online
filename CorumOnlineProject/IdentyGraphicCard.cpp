//==========================================================//
//															//
//==========================================================//
#define DIRECTINPUT_VERSION 0x0800


#include "identygraphiccard.h"
#include <ddraw.h>
#include <dinput.h>
#include <dmusici.h>
#include <string.h>
#include <stdio.h>
#include <D3d8types.h>
#include <d3d8.h>
#include <iostream>
#include <memory.h>
#include <string.h>


enum eGRAPHIC_CARD_OPTION SpecGraphicCard()
{
	char			szBuf[MAXPATH] = {0,};
//	D3DDEVTYPE		d3dType = D3DDEVTYPE_HAL;
//	D3DCAPS8		d3dCaps;
	LPDIRECT3D8		lpD3D;

	// D3D 객체 얻음 
	if( NULL == ( lpD3D = Direct3DCreate8( D3D_SDK_VERSION ) ) )
	{
		MessageBox(NULL, "Direct3D object create error", NULL, MB_OK);
		return OPTION_NUM;	
	}

	// 어뎁터(그래픽 카드)의 정보 얻기 
	D3DADAPTER_IDENTIFIER8  Adapter;
    if( FAILED( lpD3D->GetAdapterIdentifier(0, D3DENUM_NO_WHQL_LEVEL, &Adapter) ) )
	{	
		MessageBox(NULL, "AdapterIdentifer error", NULL, MB_OK);
		lpD3D->Release();
        return OPTION_NUM;
	}	

	int		i=0;
	char *	pString=0;
	bool	bRadeon8500MoreThan = FALSE;
	switch(Adapter.VendorId)
	{
	case NVIDIA_VENTDER_ID:
		lpD3D->Release();
		return NORMAL;		

	case ATI_VENTDER_ID:
		for (i = 0; i < 10; i++)
		{ // 숫자가 나오면 무조건 검사한다. 
			itoa(i, szBuf, 10);
			pString = strstr(Adapter.Description, szBuf);

			if (!pString)
				continue;

			if ( strstr(Adapter.Description, "7000") || 
				 strstr(Adapter.Description, "7200") ||
				 strstr(Adapter.Description, "7500") )
			{
				 bRadeon8500MoreThan = FALSE;
				 break;
			}
			else if ( strstr(Adapter.Description, "8500") )
			{
				bRadeon8500MoreThan = TRUE;
				break;
			}
			else if ( strstr(Adapter.Description, "9000") )
			{
				bRadeon8500MoreThan = TRUE;
				break;
			}
			else if ( strstr(Adapter.Description, "9100") )		
			{
				bRadeon8500MoreThan = TRUE;
				break;
			}
			else if ( strstr(Adapter.Description, "9200") )		
			{
				bRadeon8500MoreThan = TRUE;
				break;
			}
			else if ( strstr(Adapter.Description, "9700") )		
			{
				bRadeon8500MoreThan = TRUE;
				break;
			}
			else if ( strstr(Adapter.Description, "9800") )		
			{
				bRadeon8500MoreThan = TRUE;
				break;
			}
			else
			{
				bRadeon8500MoreThan = FALSE;
			}
		}

		if ( strstr(Adapter.Description, "VE") )		
		{
			bRadeon8500MoreThan = TRUE;
		}

		if (bRadeon8500MoreThan)
		{
			lpD3D->Release();
			return NORMAL;
		}
		else
		{
			lpD3D->Release();
			return RADEON_7500_UNDER;
		}

		break;
	case MATROX_VENTDER_ID:
		lpD3D->Release();
		return MATROX;
		
	case VOODOO_VENDER_ID:
		lpD3D->Release();
		return VOODOO;
	}

	// release object
	lpD3D->Release();		
	return NORMAL;
}
//==========================================================//
//															//
//==========================================================//