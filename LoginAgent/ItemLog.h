#ifndef __ITEMLOG_H__
#define __ITEMLOG_H__

class	CItem;
/*
#define		ITEM_LOG_PICKUP					1		// 아이템을 줍다.
#define		ITEM_LOG_DROP					2		// 아이템을 떨구다.
#define		ITEM_LOG_USED					3		// 아이템 사용 
#define		ITEM_LOG_EXCHANGE				4		// 아이템을 교환으로 줍다.
#define		ITEM_LOG_SUM					5		// 아이템을 합쳤다.
#define		ITEM_LOG_SUB					6		// 아이템을 나누었다.
#define		ITEM_LOG_DELETE_BY_SUM			7		// 아이템 합치기에 의해 삭제 됐다.
#define		ITEM_LOG_CREATE_BY_SUB			8		// 아이템 나누기에 의해 생성됐다.
#define		ITEM_LOG_CREATE_BY_SHOP			9		// NPC에게 구입했다.
#define		ITEM_LOG_DELETE_BY_SHOP			10		// NPC에게 팔다.
#define		ITEM_LOG_UPGRADE				11		// 아이템 업그레이드
#define		ITEM_LOG_PLAYERSHOP				12		
#define		ITEM_LOG_TRADE					13
#define		ITEM_LOG_UPGRADE_FAIL			14		// 아이템 업그레이드 실패 
#define		ITEM_LOG_UPGRADE_FAIL_INSURANCE 15		// 아이템 업그레이트 실패 이지만 보험 처리
#define		ITEM_LOG_KARZ_DROP				16	 
#define		ITEM_LOG_KARZ_PICKUP			17
#define		ITEM_LOG_USER_INSURANCE			18
*/
#define		ITEM_LOG_ITEM_MALL				19		// 아이템몰에서 구입

BOOL	SetItemLog( CItem* pItem, BYTE bLogCode, WORD wDungeonID, char* szOwner, char* szSeller );

#endif // __ITEMLOG_H__