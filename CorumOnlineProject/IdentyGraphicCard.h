//==========================================================//
//															//
//==========================================================//
#ifndef		__IDENTYGRAPHICCARD_INC__
#define		__IDENTYGRAPHICCARD_INC__

enum eGRAPHIC_CARD_OPTION
{
	NORMAL				,	// nvidia / 이상없는 그래픽 카드 
	RADEON_7500_UNDER	,	// RADEON 7500 UNDER CHIP SERIES		
	MATROX				,	// MATROX CHIP
	VOODOO              ,	// VOODOO CHIP
	OPTION_NUM				// 옵션 개수 / 오류일때도 사용한다. 
};

#define NVIDIA_VENTDER_ID	4318
#define ATI_VENTDER_ID		4098
#define MATROX_VENTDER_ID	4139
#define VOODOO_VENDER_ID	4634

#define MAXPATH		512

enum eGRAPHIC_CARD_OPTION SpecGraphicCard();

#endif
//==========================================================//
//															//
//==========================================================//