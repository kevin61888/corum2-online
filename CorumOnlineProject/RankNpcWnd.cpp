//======================================================//
// Code by Jang.							2004.3.10	//
//======================================================//
#include "RankNpcWnd.h"
#include "GameControl.h"
#include "UserInterface.h"
#include "Interface.h"
#include "message.h"
#include "InitGame.h"
#include "DSMiniMap.h"
#include "RankWnd.h"
#include "CodeFun.h"

CRankNpcWnd* CRankNpcWnd::c_pThis = NULL;

//======================================================//
// Construction/Destrution.								//
//======================================================//
CRankNpcWnd::CRankNpcWnd()
{ 
	m_byLine	= 0;
	m_bBtn		= FALSE;
}

CRankNpcWnd::~CRankNpcWnd()
{
}
//======================================================//
// Class Member Function.								//
//======================================================//
BOOL CRankNpcWnd::Init()
{	
/*
	InsertData(SPR_OBJ_RANKNPC_WND1, SPR_INTERFACE_DF_WND1, 0, 0, 1.0f, 1.0f, 0);
	InsertData(SPR_OBJ_RANKNPC_WND2, SPR_INTERFACE_DF_WND2, 0, 32, 1.0f, 13.0f, 0);
	InsertData(SPR_OBJ_RANKNPC_WND3, SPR_INTERFACE_DF_WND3, 0, 240, 1.0f, 1.0f, 0);
	InsertData(SPR_OBJ_RANKNPC_CLOSE1, SPR_INTERFACE_CLOSE1, 242, 4, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_RANKNPC_CLOSE2, SPR_INTERFACE_CLOSE2, 242, 4, 1.0f, 1.0f, 1);	
	InsertData(SPR_OBJ_RANKNPC_RANK1, SPR_INTERFACE_RANKNPC_RANK1, 176, 230, 1.0f, 1.0f, 2);	
	InsertData(SPR_OBJ_RANKNPC_RANK2, SPR_INTERFACE_RANKNPC_RANK2, 176, 230, 1.0f, 1.0f, 2);	
	InsertData(SPR_OBJ_RANKNPC_RANK3, SPR_INTERFACE_RANKNPC_RANK3, 176, 230, 1.0f, 1.0f, 2);	

	InsertCheckInterface(242, 4, 256, 16, 2, CHECK_CLOSE);
	InsertCheckInterface(0, 0, 242, 20, 3, CHECK_MOVEWND);
	InsertCheckInterface(176, 230, 240, 246, 4, CHECK_BTN);
*/
	m_bInit	= TRUE;

	return TRUE;
}

void CRankNpcWnd::Remove()
{
	RemoveAllData();
	m_bActive	= FALSE;
	m_bInit		= FALSE;
}

void CRankNpcWnd::SetOrder()
{
}

void CRankNpcWnd::SetText()
{
	char szName[0xff] = {0, };
	char szText[4][0xff];

	BYTE byIndex = 0;

	for(int i = 0; i < 4; i++)
		memset(szText[i], 0, sizeof(szText[i]));
	
	if(CMiniMapWnd::GetInstance()->m_wMapId==1)
	{
		lstrcpy(szName, g_Message[ETC_MESSAGE931].szMessage); // "한슨 레이크"

		if(m_dwGuildId[0]!=0)
		{
			int nRan = rand()%3;

			switch(nRan)
			{
			case 0:
				wsprintf(szText[0], g_Message[ETC_MESSAGE932].szMessage, szName); // "%s : 요즘 \"탈른\" 이 보이질 않는군."
				wsprintf(szText[1], g_Message[ETC_MESSAGE933].szMessage, m_szGuildName[0]); // "그 난봉꾼이 요즘 %s 길드가 이 일대를 잡고 있단 소리를 듣고는 잠잠하단말야."
				wsprintf(szText[2], g_Message[ETC_MESSAGE934].szMessage, m_szGuildName[0]); // "하여간 유명해~이 일대에선 %s 길드를 모르면 간첩이지. 자네 혹 간첩 아닌가? 하하핫"
				byIndex = 3;
				break;
			case 1:
				wsprintf(szText[0], g_Message[ETC_MESSAGE935].szMessage, szName, m_szGuildName[0]); // "%s : 요즘 이 근처에 몬스터들이 잘 보이지 않아.. 혹시 그 %s 길드 때문이 아닐까?"
				lstrcpy(szText[1], g_Message[ETC_MESSAGE936].szMessage); // "큰 몬스터들을 잡아주니 마을엔 정말 영웅 같은 존재라네."
				wsprintf(szText[2], g_Message[ETC_MESSAGE937].szMessage, m_szGuildName[0]); // "자네 혹 %s 길드의 전사들을 만나면 고맙다고 전해주게."
				lstrcpy(szText[3], g_Message[ETC_MESSAGE938].szMessage); // "난 일이 바빠서 말이야. 이곳을 뜰 수가 없거든."
				byIndex = 4;
				break;
			case 2:
				wsprintf(szText[0], g_Message[ETC_MESSAGE939].szMessage, szName, m_szGuildName[0]); // "%s : 애들이 그러는데.. %s 길드가.. 나는 다섯 쌍둥이의 장남이라네."
				lstrcpy(szText[1], g_Message[ETC_MESSAGE940].szMessage); // "내가 머리가 제일 좋.. 아니.. 그러니까.. 자꾸 삼천포로 빠지는군."
				lstrcpy(szText[2], g_Message[ETC_MESSAGE941].szMessage); // "음.. 한마디로 마을마다 뿔뿔이 흩어진 우리 형제들도 만날 때 마다 입에 올릴 정도로 유명한 길드라는 소린데.."
				lstrcpy(szText[3], g_Message[ETC_MESSAGE942].szMessage); // "내가 지금 무슨 말을 했더라..?"
				byIndex = 4;
				break;
			}
		}		
		else
		{
			wsprintf(szText[0], g_Message[ETC_MESSAGE1066].szMessage, szName); // "%s : 이런 난세에 세상을 구할 영웅은 없는 걸까? 나같이 미약한 힘으론"
			lstrcpy(szText[1], g_Message[ETC_MESSAGE1067].szMessage); // "마을을 지키는 것만으로도 벅찬데.. 휴..."
			byIndex = 2;
		}		
	}	
	else if(CMiniMapWnd::GetInstance()->m_wMapId==3)
	{
		lstrcpy(szName, g_Message[ETC_MESSAGE943].szMessage); // "두어 레이크"

		if(m_dwGuildId[0]!=0)
		{
			int nRan = rand()%3;

			switch(nRan)
			{
			case 0:
				wsprintf(szText[0], g_Message[ETC_MESSAGE944].szMessage, szName, m_szGuildName[0]); // "%s : 나라에서 주시하고 있다는 소문이 있더라구.. 뭘 말이냐구? %s 길드 말일세."
				lstrcpy(szText[1], g_Message[ETC_MESSAGE945].szMessage); // "너무 위세가 높으니까 경계 하더라구.. 일반인들로써는 몬스터도 잡아주고 치안유지에 도움이 되지만,"
				lstrcpy(szText[2], g_Message[ETC_MESSAGE946].szMessage); // "나라의 입장에서는 눈엣 가신가 봐. 뭐.. 유명하단 증거 아니겠어?"
				byIndex = 3;
				break;
			case 1:
				wsprintf(szText[0], g_Message[ETC_MESSAGE947].szMessage, szName, m_szGuildName[0]); // "%s : %s 길드의 무서움은 몬스터만이 알걸세. 아메리타트가 탑 안에 갇힌건 어쩌면 행운이야~"
				wsprintf(szText[1], g_Message[ETC_MESSAGE948].szMessage, m_szGuildName[0]); // "%s 길드의 무서움을 맛보지 않은 것만해도 그 드래곤의 천운이지. 근데 아메리타트가 뭐냐고?"
				lstrcpy(szText[2], g_Message[ETC_MESSAGE949].szMessage); // "아니 그 환영의 드래곤을 모른단 말인가~ 자네 왕초보로군~!!"
				byIndex = 3;
				break;
			case 2:
				wsprintf(szText[0], g_Message[ETC_MESSAGE950].szMessage, szName); // "%s : 형이 그러는데.. 우리 형은 한슨이라고 루디로스에 있어."
				wsprintf(szText[1], g_Message[ETC_MESSAGE951].szMessage, m_szGuildName[0]); // "근데 그 형이 엊그제 편지를 보내왔어. %s 길드의 사람들이 곧 이 마을을 들를거래~"
				lstrcpy(szText[2], g_Message[ETC_MESSAGE952].szMessage); // "정말 가슴 두근 거리는 일이야. 후훗. 얼른 다른 사람들에게도 알려줘야지."
				byIndex = 3;
				break;
			}
		}
		else
		{
			wsprintf(szText[0], g_Message[ETC_MESSAGE1068].szMessage, szName); // "%s : 사람들의 한숨 소리는 하늘에 닿지 않는 걸까? 신도 참 무심하시지."
			lstrcpy(szText[1], g_Message[ETC_MESSAGE1069].szMessage); // "아.. 말이 나와서 하는 소린데, 이런 시기에 생긴 신흥 종교는 조심하는 게 좋아."
			byIndex = 2;
		}		
	}
	else if(CMiniMapWnd::GetInstance()->m_wMapId==4)
	{
		lstrcpy(szName, g_Message[ETC_MESSAGE953].szMessage); // "네슈란 레이크"

		if(m_dwGuildId[0]!=0)
		{
			int nRan = rand()%3;

			switch(nRan)
			{
			case 0:
				wsprintf(szText[0], g_Message[ETC_MESSAGE954].szMessage, szName); // "%s : 아.. 정말 우리 셋째 형 때문에 못살겠어~"
				wsprintf(szText[1], g_Message[ETC_MESSAGE955].szMessage, m_szGuildName[0]); // "글쎄 웬일로 이 먼 곳까지 나를 찾아왔나 싶었더니 내 방에 있는 %s 길드 브로마이드를 다 가져갔잖아~!!!"
				lstrcpy(szText[2], g_Message[ETC_MESSAGE956].szMessage); // "싸인 받아놓은 것까지.. 어흐흑."
				wsprintf(szText[3], g_Message[ETC_MESSAGE957].szMessage, m_szGuildName[0]); // "정말 내가 우리 형 땜에 못살아~!!! %s 길드가 언제 이곳에 다시 올는지.."
				byIndex = 4;
				break;
			case 1:
				wsprintf(szText[0], g_Message[ETC_MESSAGE958].szMessage, szName, m_szGuildName[0]); // "%s : 우리 둘째 형도 정말 싫어. 내가 전에 %s 길드 길마랑 마주쳐서 악수를 받았는데,"
				lstrcpy(szText[1], g_Message[ETC_MESSAGE959].szMessage); // "너무 감격스러워서 한 달을 손을 안씻었거든. 근데 형이 더럽다고 나 자는 사이에 손을 씻겨 놓은 거야."
				lstrcpy(szText[2], g_Message[ETC_MESSAGE960].szMessage); // "난 내 꼬질꼬질한 손이 자랑이었다고~!! 정말..우리 형들은 제멋대로야~!!!"
				byIndex = 3;
				break;
			case 2:
				wsprintf(szText[0], g_Message[ETC_MESSAGE961].szMessage, szName, m_szGuildName[0]); // "%s : 우리 막내는 그래도 제일 괜찮아. %s 길드가 우리 마을 올 때 나한테 말해줘서 구경할 수 있었지."
				lstrcpy(szText[1], g_Message[ETC_MESSAGE962].szMessage); // "내 동생은 다다라고 하는데 시하 마을에서 일하고 있거든? 다 똑같이 생긴 거 같아도 자세히 보면 개가 제일 귀여워."
				wsprintf(szText[2], g_Message[ETC_MESSAGE963].szMessage, m_szGuildName[0]); // "%s 길드의 무용담 들어볼테야? 엥? 맨날 들어서 지겹다고~? 그래도 들어줘~"
				byIndex = 3;
				break;
			}			
		}		
		else
		{
			wsprintf(szText[0], g_Message[ETC_MESSAGE1070].szMessage, szName); // "%s : 어려울 때 도움을 줄 길드라도 있다면 좀 살기 쉬워질 텐데 말야."
			lstrcpy(szText[1], g_Message[ETC_MESSAGE1071].szMessage); // "당신, 용병인 거 같은데 한번 도전해봐."
			byIndex = 2;
		}
	}
	else if(CMiniMapWnd::GetInstance()->m_wMapId==5)
	{
		lstrcpy(szName, g_Message[ETC_MESSAGE964].szMessage); // "세난 레이크"

		if(m_dwGuildId[0]!=0)
		{
			int nRan = rand()%3;

			switch(nRan)
			{
			case 0:
				wsprintf(szText[0], g_Message[ETC_MESSAGE965].szMessage, szName, m_szGuildName[0]); // "%s : 말이야 바른 말이지. %s 길드가 아니면 몬스터들을 누가 잠 재웠겠는가?"
				lstrcpy(szText[1], g_Message[ETC_MESSAGE966].szMessage); // "그 옛날 마법사 륜 조차 몬스터들의 전멸 하지 못했잖은가?"
				wsprintf(szText[2], g_Message[ETC_MESSAGE967].szMessage, m_szGuildName[0]); // "%s 길드는 뛰어난 사람들이 많으니까 앞으로 몬스터가 소멸될 날도 머지 않은걸지도.."
				lstrcpy(szText[3], g_Message[ETC_MESSAGE968].szMessage); // "그렇게 생각하지 않아?"
				byIndex = 4;
				break;
			case 1:
				wsprintf(szText[0], g_Message[ETC_MESSAGE969].szMessage, szName, m_szGuildName[0]); // "%s : 우리 둘째 형이 그러는데 %s 길드의 길드원들을 나라에서 경계하고 있다더군."
				lstrcpy(szText[1], g_Message[ETC_MESSAGE970].szMessage); // "근데 형은 모르는게 있어~"
				lstrcpy(szText[2], g_Message[ETC_MESSAGE971].szMessage); // "이곳 주유에선 나라에서 스카우트 하려고 비밀리에 협의를 보고 있다는 소문이 돈다고.."
				wsprintf(szText[3], g_Message[ETC_MESSAGE972].szMessage, m_szGuildName[0]); // "뭐 어떤 게 사실이든 %s 길드의 주가가 올라가고 있는 건 사실이야."
				byIndex = 4;
				break;
			case 2:
				wsprintf(szText[0], g_Message[ETC_MESSAGE973].szMessage, szName, m_szGuildName[0]); // "%s : 힘이 있다고 다 최고는 아니지만 말야. %s 길드의 조직력과 파워는 탐낼 만 하겠더군."
				wsprintf(szText[1], g_Message[ETC_MESSAGE974].szMessage, m_szGuildName[0]); // "저번에 길가다 %s 길드의 길원을 만났는데 말야. 다른 사람과 같은 옷을 입어도 번쩍 하는거야."
				lstrcpy(szText[2], g_Message[ETC_MESSAGE975].szMessage); // "그런걸 위용이라 하는 거겠지?"
				byIndex = 3;
				break;
			}
		}
		else
		{
			wsprintf(szText[0], g_Message[ETC_MESSAGE1072].szMessage, szName); // "%s : 싸인 받을 만큼 유명한 영웅이 없다는 건 슬픈 일이야."
			wsprintf(szText[1], g_Message[ETC_MESSAGE1073].szMessage); // "세계도 많이 혼탁해지고, 이제 사람들은 무슨 낙으로 살지?"
			byIndex = 2;
		}
	}
	else if(CMiniMapWnd::GetInstance()->m_wMapId==6)
	{
		lstrcpy(szName, g_Message[ETC_MESSAGE976].szMessage); // "다다 레이크"

		if(m_dwGuildId[0]!=0)
		{
			int nRan = rand()%3;

			switch(nRan)
			{
			case 0:
				wsprintf(szText[0], g_Message[ETC_MESSAGE977].szMessage, szName, m_szGuildName[0]); // "%s : 아.. %s 길드라면 몇일 전에 이곳을 다녀갔어요."
				lstrcpy(szText[1], g_Message[ETC_MESSAGE978].szMessage); // "그들의 무용담은 멋지기도 하지만 왠지 간담이 서늘할 때도 있다니까요."
				lstrcpy(szText[2], g_Message[ETC_MESSAGE979].szMessage); // "하긴 그래서 사람들이 따르는 거겠죠. 카.리.스.마 그쵸?"
				byIndex = 3;
				break;
			case 1:
				wsprintf(szText[0], g_Message[ETC_MESSAGE980].szMessage, szName); // "%s : 눈이 올 때 스치는 검기 속에서 그 사람을 봤어요."
				wsprintf(szText[1], g_Message[ETC_MESSAGE981].szMessage, m_szGuildName[0]); // "%s 길드의 길드원 이라던데. 검은 머리가 인상적인 검객이었죠."
				lstrcpy(szText[2], g_Message[ETC_MESSAGE982].szMessage); // "다시 보게 되면 꼭 싸인 받아서 넷째 형에게 갖다 주기로 약속했어요."
				byIndex = 3;
				break;
			case 2:
				wsprintf(szText[0], g_Message[ETC_MESSAGE983].szMessage, szName, m_szGuildName[0]); // "%s : 시하의 지형상 고레벨의 분들을 자주 만나게 되는데, %s 길드 분들이 많아요."
				lstrcpy(szText[1], g_Message[ETC_MESSAGE984].szMessage); // "친하지는 않지만 가끔 인사는 하는 정도예요."
				lstrcpy(szText[2], g_Message[ETC_MESSAGE985].szMessage); // "그래서 저희 형들이 많이 부러워하죠."
				byIndex = 3;
				break;
			}
		}		
		else
		{
			wsprintf(szText[0], g_Message[ETC_MESSAGE1074].szMessage, szName); // "%s : 과거 란테 슐라인이 집권하던 시기엔 불만도 많았지만 주변 세력들간의"
			wsprintf(szText[1], g_Message[ETC_MESSAGE1075].szMessage); // "다툼이나 몬스터에 대한 위압감은 가질 필요가 없었죠."
			byIndex = 2;
		}
	}

	m_byLine = 0;
	
	for(i = 0; i < 20; i++)
		memset(m_szRenderText[i], 0, sizeof(m_szRenderText[i]));

	for(i = 0; i < byIndex; i++)
	{
		char* szTemp = NULL;

		int nSize = lstrlen(szText[i]);

		szTemp = szText[i];

		while(nSize>0)
		{
			if(nSize>36)
			{
				for(;;)
				{
					if(nSize > 40)
					{
						char* szPrev = CharPrevExA(0, szTemp, szTemp+40, 0);
						nSize = lstrlen(szTemp) - lstrlen(szPrev); 

						__lstrcpyn(m_szRenderText[m_byLine], szTemp, nSize);
						
						szTemp = szPrev; 
						m_byLine++;
						nSize = lstrlen(szTemp);
					}
					else
					{
						// 구한 문자열을 채팅 라인에 쓰고, 길이를 기록한다.
						lstrcpy(m_szRenderText[m_byLine], szTemp);
						m_byLine++;								
						nSize = 0;
						break;
					}
				}
			}
			else
			{
				lstrcpy(m_szRenderText[m_byLine], szTemp);				
				m_byLine++;
				nSize = 0;							
			}		
		}
	}
}

void CRankNpcWnd::RenderText()
{	
	char szInfo[0xff] = {0, };

	lstrcpy(szInfo, g_Message[ETC_MESSAGE916].szMessage); // "랭킹 게시판"
	
	int nSize = lstrlen(szInfo);
	RenderFont(szInfo, m_fPosX+22, m_fPosX+22+nSize*7, m_fPosZ+5, m_fPosZ+19, GetStartOrder()+1);

	for(int i = 0; i < m_byLine; i++)
	{
		nSize = lstrlen(m_szRenderText[i]);
		RenderFont(m_szRenderText[i], m_fPosX+10, m_fPosX+10+nSize*7, m_fPosZ+35+i*15, m_fPosZ+49+i*15, GetStartOrder()+1);
	}		
}

void CRankNpcWnd::SetActive(BOOL bActive)
{
	m_bActive	= bActive;

	if(bActive==FALSE)
	{
		ShowSpriteAll();

		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDCLOSE, g_v3InterfaceSoundPos, FALSE);
	}
	else
	{
		CInterface::GetInstance()->SetWnd((BYTE)m_nIndex);

		SetRender(SPR_OBJ_RANKNPC_WND1, TRUE);
		SetRender(SPR_OBJ_RANKNPC_WND2, TRUE);
		SetRender(SPR_OBJ_RANKNPC_WND3, TRUE);
		SetRender(SPR_OBJ_RANKNPC_CLOSE1, TRUE);
		SetRender(SPR_OBJ_RANKNPC_RANK1, TRUE);			

		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDOPEN, g_v3InterfaceSoundPos, FALSE);		
	}
}

int CRankNpcWnd::CheckInterface()
{
	m_bMouseIcon = FALSE;

	int nRt = GetChk();
		
	if(nRt!=4 && m_bBtn)
	{
		m_bBtn = TRUE;
		SetRender(SPR_OBJ_RANKNPC_RANK1, TRUE);
		SetRender(SPR_OBJ_RANKNPC_RANK2, FALSE);
		SetRender(SPR_OBJ_RANKNPC_RANK3, FALSE);
	}

	InitValue(nRt);

	MOUSECHECK eMouseCheck = GetMouseCheck(nRt);
	
	switch(nRt)
	{		
	case 1:
		{			
			SetMouseCheck(nRt, eMouseCheck);						
		}
		break;
	case 2:
		{			
			if(eMouseCheck==MOUSE_UP)			
				SetActive(FALSE);	
			else if(eMouseCheck==MOUSE_DOWN)
			{
				SetRender(SPR_OBJ_RANKNPC_CLOSE1, FALSE);
				SetRender(SPR_OBJ_RANKNPC_CLOSE2, TRUE);
			}						
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 3:			
		{
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 4:
		{			
			if(eMouseCheck==MOUSE_UP)
			{
				CRankWnd* pRankWnd = CRankWnd::GetInstance();

				SetRender(SPR_OBJ_RANKNPC_RANK1, FALSE);
				SetRender(SPR_OBJ_RANKNPC_RANK2, TRUE);
				SetRender(SPR_OBJ_RANKNPC_RANK3, FALSE);
				m_bBtn = TRUE;

				for(int i = 0; i < 4; i++)
				{
					pRankWnd->m_dwGuildId[i]	= m_dwGuildId[i];
					pRankWnd->m_dwPoint[i]	= m_dwPoint[i];
					memset(pRankWnd->m_szGuildName[i], 0, sizeof(pRankWnd->m_szGuildName[i]));
					__lstrcpyn(pRankWnd->m_szGuildName[i], m_szGuildName[i], MAX_GUILD_NAME_REAL_LENGTH);
				}
				pRankWnd->m_dwRank	= m_dwRank;
				pRankWnd->SetActive();
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{							
				m_bBtn = TRUE;
				SetRender(SPR_OBJ_RANKNPC_RANK1, FALSE);
				SetRender(SPR_OBJ_RANKNPC_RANK2, FALSE);
				SetRender(SPR_OBJ_RANKNPC_RANK3, TRUE);				
			}
			else if(eMouseCheck==MOUSE_OVER)
			{
				m_bBtn = TRUE;
				SetRender(SPR_OBJ_RANKNPC_RANK1, FALSE);
				SetRender(SPR_OBJ_RANKNPC_RANK2, TRUE);
				SetRender(SPR_OBJ_RANKNPC_RANK3, FALSE);				
			}
			SetMouseCheck(nRt, eMouseCheck);
		}		
		break;
	}		
	
	if(MouseUp())
	{
		SetRender(SPR_OBJ_RANKNPC_CLOSE1, TRUE);
		SetRender(SPR_OBJ_RANKNPC_CLOSE2, FALSE);		
	}

	MouseChk(nRt);
	
	if(SetMoveWnd(nRt))	
		return 1;
	
	if(IsReturn(nRt, eMouseCheck))
		return 1;		

	return 0;
}
//======================================================//
// End.													//
//======================================================//
