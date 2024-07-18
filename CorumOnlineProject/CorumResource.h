// Info =======================================================
// Built by eflmake (ver:EF010) at 2004/07/16-01:29:24
//
// Contained ERL files --------------------
// Resource3.erl
// Resource2.erl
// Resource1.erl
// Resource.erl
// ----------------------------------------
//
// ============================================================

// Types ======================================================
#define	RESTYPE_HEAD_MALE               	1           
#define	RESTYPE_HEAD_FEMALE             	2           
#define	RESTYPE_BASE_BODY               	3           
#define	RESTYPE_MAP_WORLD               	8           
#define	RESTYPE_TTB_WORLD               	9           
#define	RESTYPE_SPRITE                  	10          
#define	RESTYPE_BGM                     	11          
#define	RESTYPE_MOD                     	12          
#define	RESTYPE_BUTTON                  	13          
#define	RESTYPE_BUTTON_OVER             	14          
#define	RESTYPE_BUTTON_DOWN             	15          
#define	RESTYPE_SKILL_ICON3             	16          
#define	RESTYPE_ITEMICON                	17          
#define	RESTYPE_ITEMMOD                 	18          
#define	RESTYPE_IV                      	19          
#define	RESTYPE_ITEMMOD2                	20          
#define	RESTYPE_ITEMMOD3                	21          
#define	RESTYPE_ITEMMOD4                	22          
#define	RESTYPE_ITEMMOD5                	23          
#define	RESTYPE_CURIV1                  	24          
#define	RESTYPE_CURIV2                  	25          
#define	RESTYPE_CURIV3                  	26          
#define	RESTYPE_CURIV4                  	27          
#define	RESTYPE_CURIV5                  	28          
#define	RESTYPE_CURIV6                  	29          
#define	RESTYPE_MINIMAP                 	30          
#define	RESTYPE_IVBELT                  	31          
#define	RESTYPE_STORE                   	32          
#define	RESTYPE_BANK_ICON               	45          
#define	RESTYPE_ITEMTRADE               	50          
#define	RESTYPE_PLAYERSHOP              	51          
#define	RESTYPE_BUFF                    	52          
#define	RESTYPE_UP                      	53          
#define	RESTYPE_IV25                    	54          
#define	RESTYPE_IV26                    	55          
#define	RESTYPE_IV27                    	56          
#define	RESTYPE_IV28                    	57          
#define	RESTYPE_IV29                    	58          
#define	RESTYPE_IV30                    	59          
#define	RESTYPE_IV31                    	60          
#define	RESTYPE_IV32                    	61          
#define	RESTYPE_IV33                    	62          
#define	RESTYPE_IV34                    	63          
#define	RESTYPE_IV35                    	64          
#define	RESTYPE_IV36                    	65          
#define	RESTYPE_IV37                    	66          
#define	RESTYPE_IV38                    	67          
#define	RESTYPE_IV39                    	68          
#define	RESTYPE_IV40                    	69          
#define	RESTYPE_IV41                    	70          
#define	RESTYPE_IV42                    	71          
#define	RESTYPE_IVBELT1                 	72          
#define	RESTYPE_IVBELT2                 	73          
#define	RESTYPE_IVBELT3                 	74          
#define	RESTYPE_IVBELT4                 	75          
#define	RESTYPE_IVBELT5                 	76          
#define	RESTYPE_IVBELT6                 	77          
#define	RESTYPE_IVBELT7                 	78          
#define	RESTYPE_IVBELT8                 	79          
#define	RESTYPE_BANK_ICON1              	116         
#define	RESTYPE_BANK_ICON2              	117         
#define	RESTYPE_BANK_ICON3              	118         
#define	RESTYPE_BANK_ICON4              	119         
#define	RESTYPE_BANK_ICON5              	120         
#define	RESTYPE_BANK_ICON6              	121         
#define	RESTYPE_BANK_ICON7              	122         
#define	RESTYPE_BANK_ICON8              	123         
#define	RESTYPE_BANK_ICON9              	124         
#define	RESTYPE_BANK_ICON10             	125         
#define	RESTYPE_BANK_ICON11             	126         
#define	RESTYPE_BANK_ICON12             	127         
#define	RESTYPE_BANK_ICON13             	128         
#define	RESTYPE_BANK_ICON14             	129         
#define	RESTYPE_BANK_ICON15             	130         
#define	RESTYPE_BANK_ICON16             	131         
#define	RESTYPE_BANK_ICON17             	132         
#define	RESTYPE_BANK_ICON18             	133         
#define	RESTYPE_BANK_ICON19             	134         
#define	RESTYPE_BANK_ICON20             	135         
#define	RESTYPE_BANK_ICON21             	136         
#define	RESTYPE_BANK_ICON22             	137         
#define	RESTYPE_BANK_ICON23             	138         
#define	RESTYPE_BANK_ICON24             	139         
#define	RESTYPE_BANK_ICON25             	140         
#define	RESTYPE_BANK_ICON26             	141         
#define	RESTYPE_BANK_ICON27             	142         
#define	RESTYPE_BANK_ICON28             	143         
#define	RESTYPE_BANK_ICON29             	144         
#define	RESTYPE_BANK_ICON30             	145         
#define	RESTYPE_BANK_ICON31             	146         
#define	RESTYPE_BANK_ICON32             	147         
#define	RESTYPE_BANK_ICON33             	148         
#define	RESTYPE_BANK_ICON34             	149         
#define	RESTYPE_BANK_ICON35             	150         
#define	RESTYPE_BANK_ICON36             	151         
#define	RESTYPE_BANK_ICON37             	152         
#define	RESTYPE_BANK_ICON38             	153         
#define	RESTYPE_BANK_ICON39             	154         
#define	RESTYPE_BANK_ICON40             	155         
#define	RESTYPE_BANK_ICON41             	156         
#define	RESTYPE_BANK_ICON42             	157         
#define	RESTYPE_BANK_ICON43             	158         
#define	RESTYPE_BANK_ICON44             	159         
#define	RESTYPE_BANK_ICON45             	160         
#define	RESTYPE_BANK_ICON46             	161         
#define	RESTYPE_BANK_ICON47             	162         
#define	RESTYPE_BANK_ICON48             	163         
#define	RESTYPE_BANK_ICON49             	164         
#define	RESTYPE_BANK_ICON50             	165         
#define	RESTYPE_BANK_ICON51             	166         
#define	RESTYPE_BANK_ICON52             	167         
#define	RESTYPE_BANK_ICON53             	168         
#define	RESTYPE_BANK_ICON54             	169         
#define	RESTYPE_BANK_ICON55             	170         
#define	RESTYPE_BANK_ICON56             	171         
#define	RESTYPE_BANK_ICON57             	172         
#define	RESTYPE_BANK_ICON58             	173         
#define	RESTYPE_BANK_ICON59             	174         
#define	RESTYPE_BANK_ICON60             	175         
#define	RESTYPE_BANK_ICON61             	176         
#define	RESTYPE_BANK_ICON62             	177         
#define	RESTYPE_BANK_ICON63             	178         
#define	RESTYPE_BANK_ICON64             	179         
#define	RESTYPE_BANK_ICON65             	180         
#define	RESTYPE_BANK_ICON66             	181         
#define	RESTYPE_BANK_ICON67             	182         
#define	RESTYPE_BANK_ICON68             	183         
#define	RESTYPE_BANK_ICON69             	184         
#define	RESTYPE_BANK_ICON70             	185         
#define	RESTYPE_ITEMTRADE1              	190         
#define	RESTYPE_ITEMTRADE2              	191         
#define	RESTYPE_ITEMTRADE3              	192         
#define	RESTYPE_ITEMTRADE4              	193         
#define	RESTYPE_ITEMTRADE5              	194         
#define	RESTYPE_ITEMTRADE6              	195         
#define	RESTYPE_ITEMTRADE7              	196         
#define	RESTYPE_ITEMTRADE8              	197         
#define	RESTYPE_ITEMTRADE9              	198         
#define	RESTYPE_ITEMTRADE10             	199         
#define	RESTYPE_ITEMTRADE11             	200         
#define	RESTYPE_ITEMTRADE12             	201         
#define	RESTYPE_ITEMTRADE13             	202         
#define	RESTYPE_ITEMTRADE14             	203         
#define	RESTYPE_ITEMTRADE15             	204         
#define	RESTYPE_ITEMTRADE16             	205         
#define	RESTYPE_ITEMTRADE17             	206         
#define	RESTYPE_ITEMTRADE18             	207         
#define	RESTYPE_ITEMTRADE19             	208         
#define	RESTYPE_ITEMTRADE20             	209         
#define	RESTYPE_ITEMTRADE21             	210         
#define	RESTYPE_ITEMTRADE22             	211         
#define	RESTYPE_ITEMTRADE23             	212         
#define	RESTYPE_ITEMTRADE24             	213         
#define	RESTYPE_ITEMTRADE25             	214         
#define	RESTYPE_ITEMTRADE26             	215         
#define	RESTYPE_ITEMTRADE27             	216         
#define	RESTYPE_ITEMTRADE28             	217         
#define	RESTYPE_PLAYERSHOP1             	218         
#define	RESTYPE_PLAYERSHOP2             	219         
#define	RESTYPE_PLAYERSHOP3             	220         
#define	RESTYPE_PLAYERSHOP4             	221         
#define	RESTYPE_PLAYERSHOP5             	222         
#define	RESTYPE_PLAYERSHOP6             	223         
#define	RESTYPE_PLAYERSHOP7             	224         
#define	RESTYPE_PLAYERSHOP8             	225         
#define	RESTYPE_PLAYERSHOP9             	226         
#define	RESTYPE_PLAYERSHOP10            	238         
#define	RESTYPE_PLAYERSHOP11            	239         
#define	RESTYPE_PLAYERSHOP12            	240         
#define	RESTYPE_PLAYERSHOP13            	241         
#define	RESTYPE_PLAYERSHOP14            	242         
#define	RESTYPE_PLAYERSHOP15            	243         
#define	RESTYPE_PLAYERSHOP16            	244         
#define	RESTYPE_PLAYERSHOP17            	245         
#define	RESTYPE_PLAYERSHOP18            	246         
#define	RESTYPE_PLAYERSHOP19            	247         
#define	RESTYPE_PLAYERSHOP20            	248         
// ============================================================

// ==== =================================== =============== ===============================
// IDX  Name                                UID             FileName
// ---- ----------------------------------- --------------- -------------------------------
#define	SPR_INTRO_ESOFNET               	167772161   	// .\Data\UI\esofnet.tga
#define	SPR_INTRO_PSTEAM                	167772162   	// .\Data\UI\psteam.tga
#define	SPR_LOGIN_BACKGROUND            	167772163   	// .\Data\UI\title.tif
#define	SPR_LOGIN_LOGIN_WINDOW          	167772164   	// .\Data\UI\wdw0001.tif
#define	SPR_LOGIN_SERVERSELECT_WINDOW   	167772165   	// .\Data\UI\wdw0002.tif
#define	SPR_CHAR_SELECT_BACKGROUND      	167772167   	// .\Data\UI\select_back.tga
#define	SPR_CHAR_SELECT_INFO_WINDOW     	167772169   	// .\Data\UI\wdw0003.tif
#define	SPR_CHAR_SELECT_CREATE_WINDOW   	167772170   	// .\Data\UI\wdw0004.tif
#define	SPR_WORLD_UI_TOP                	167772171   	// .\Data\UI\wib001.tif
#define	SPR_WORLD_UI_MENU               	167772172   	// .\Data\UI\wib003.tif
#define	SPR_WORLD_UI_SMALL_MAP          	167772173   	// .\Data\UI\worldminimap_10001.tif
#define	SPR_WORLD_UI_RIGHT              	167772174   	// .\Data\UI\wib004.tif
#define	SPR_WORLD_UI_LEFT               	167772175   	// .\Data\UI\wib005.tif
#define	SPR_WORLD_UI_BOTTOM             	167772176   	// .\Data\UI\wib006.tif
#define	SPR_WORLD_UI_CHAT               	167772177   	// .\Data\UI\wib007.tif
#define	SPR_WORLD_RED_DOT               	167772178   	// .\Data\UI\reddot.tga
#define	SPR_SHOES_MARK                  	167772179   	// .\Data\UI\temp_shoes.tga
#define	SPR_MANDOO_MARK                 	167772180   	// .\Data\UI\temp_mandoo.tga
#define	SPR_SATAFE_MARK                 	167772181   	// .\Data\UI\temp_abyss.tga
#define	SPR_CS_BLACK_LINE               	167772182   	// .\Data\UI\cs_blackline.tga
#define	SPR_LOADING_BACK                	167772183   	// .\Data\UI\loading.tga
#define	SPR_LOADING_BLOCK_1             	167772184   	// .\Data\UI\loading_sprite_a.tga
#define	SPR_LOADING_BLOCK_2             	167772185   	// .\Data\UI\loading_sprite_a.tga
#define	SPR_LOADING_BLOCK_3             	167772186   	// .\Data\UI\loading_sprite_a.tga
#define	SPR_LOADING_BLOCK_4             	167772187   	// .\Data\UI\loading_sprite_a.tga
#define	SPR_LOADING_BLOCK_5             	167772188   	// .\Data\UI\loading_sprite_a.tga
#define	SPR_LOADING_BLOCK_6             	167772189   	// .\Data\UI\loading_sprite_a.tga
#define	SPR_LOADING_BLOCK_7             	167772190   	// .\Data\UI\loading_sprite_a.tga
#define	SPR_LOADING_BLOCK_8             	167772191   	// .\Data\UI\loading_sprite_a.tga
#define	SPR_LOADING_BLOCK_9             	167772192   	// .\Data\UI\loading_sprite_a.tga
#define	SPR_LOADING_BLOCK_10            	167772193   	// .\Data\UI\loading_sprite_a.tga
#define	SPR_LOADING_BLOCK_11            	167772194   	// .\Data\UI\loading_sprite_a.tga
#define	SPR_LOADING_BLOCK_12            	167772195   	// .\Data\UI\loading_sprite_a.tga
#define	SPR_LOADING_BLOCK_13            	167772196   	// .\Data\UI\loading_sprite_a.tga
#define	SPR_LOADING_BLOCK_14            	167772197   	// .\Data\UI\loading_sprite_a.tga
#define	SPR_LOADING_BLOCK_15            	167772198   	// .\Data\UI\loading_sprite_a.tga
#define	SPR_LOADING_BLOCK_16            	167772199   	// .\Data\UI\loading_sprite_a.tga
#define	SPR_LOADING_BLOCK_17            	167772200   	// .\Data\UI\loading_sprite_a.tga
#define	SPR_LOADING_BLOCK_18            	167772201   	// .\Data\UI\loading_sprite_a.tga
#define	SPR_LOADING_BLOCK_19            	167772202   	// .\Data\UI\loading_sprite_a.tga
#define	SPR_LOADING_BLOCK_20            	167772203   	// .\Data\UI\loading_sprite_a.tga
#define	SPR_LOADING_BLOCK_HEAD          	167772204   	// .\Data\UI\loading_sprite_b.tif
#define	SPR_WORLD_UI_SMALL_MAP2         	167772205   	// .\Data\UI\worldminimap_10002.tif
#define	SPR_UI_LEFT                     	167778161   	// .\Data\UI\interface_Left.tif
#define	SPR_EN1                         	167778162   	// .\Data\UI\menu_1.tga
#define	SPR_EN2                         	167778163   	// .\Data\UI\menu_1.tga
#define	SPR_UI_RIGHT                    	167778164   	// .\Data\UI\interface_Right.tif
#define	SPR_EXP                         	167778165   	// .\Data\UI\menu_1.tga
#define	SPR_MINIMAP                     	167778166   	// .\Data\UI\minimap.tif
#define	SPR_MANA1                       	167778167   	// .\Data\UI\menu_1.tga
#define	SPR_MANA2                       	167778168   	// .\Data\UI\menu_1.tga
#define	SPR_CAST                        	167778169   	// .\Data\UI\menu_1.tga
#define	SPR_ITEMWINDOW                  	167778170   	// .\Data\UI\itemwindow.tif
#define	SPR_CHAR_WINDOWS1               	167778171   	// .\Data\UI\Chrwindow1.tif
#define	SPR_SKILLWINDOW                 	167778172   	// .\Data\UI\skillwindow.tif
#define	SPR_OPTIONWND                   	167778173   	// .\Data\UI\optionwindow.tif
#define	SPR_DFWND1                      	167778174   	// .\Data\UI\df_window1.tif
#define	SPR_DFWND2                      	167778175   	// .\Data\UI\df_window2.tif
#define	SPR_DFWNDSC                     	167778176   	// .\Data\UI\menu_1.tga
#define	SPR_DFWND3                      	167778177   	// .\Data\UI\df_window3.tif
#define	SPR_CHAT_SC                     	167778178   	// .\Data\UI\menu_1.tga
#define	SPR_ITEM_CLOSE1                 	167778179   	// .\Data\UI\menu_1.tga
#define	SPR_ITEM_CLOSE2                 	167778180   	// .\Data\UI\menu_1.tga
#define	SPR_CHAR_CLOSE1                 	167778181   	// .\Data\UI\menu_1.tga
#define	SPR_CHAR_CLOSE2                 	167778182   	// .\Data\UI\menu_1.tga
#define	SPR_SKILL_CLOSE1                	167778183   	// .\Data\UI\menu_1.tga
#define	SPR_SKILL_CLOSE2                	167778184   	// .\Data\UI\menu_1.tga
#define	SPR_SYSTEM_CLOSE2               	167778185   	// .\Data\UI\menu_1.tga
#define	SPR_SYSTEM_CLOSE1               	167778186   	// .\Data\UI\menu_1.tga
#define	SPR_ITEM_CHK1                   	167778187   	// .\Data\UI\item_menu1.tif
#define	SPR_COMMAND_WND1                	167778188   	// .\Data\UI\df_window4.tif
#define	SPR_ITEM_CHK2                   	167778189   	// .\Data\UI\item_menu2.tif
#define	SPR_COMMAND_WND2                	167778190   	// .\Data\UI\df_window5.tif
#define	SPR_COMMAND_WND3                	167778191   	// .\Data\UI\df_window6.tif
#define	SPR_COMMAND_CLOSE1              	167778192   	// .\Data\UI\menu_1.tga
#define	SPR_COMMAND_CLOSE2              	167778193   	// .\Data\UI\menu_1.tga
#define	SPR_CHAT_WND                    	167778194   	// .\Data\UI\chat_window.tif
#define	SPR_CHAT_CLOSE1                 	167778200   	// .\Data\UI\menu_1.tga
#define	SPR_CHAT_CLOSE2                 	167778201   	// .\Data\UI\menu_1.tga
#define	SPR_COMMAND_SC                  	167778202   	// .\Data\UI\menu_1.tga
#define	SPR_SPEAKING_BOX                	167778203   	// .\Data\UI\speaking_box.tif
#define	SPR_DFCMD                       	167778204   	// .\Data\UI\menu_1.tga
#define	SPR_CHATLIST_WND1               	167778205   	// .\Data\UI\Chatlist1.tif
#define	SPR_CHATLIST_WND2               	167778206   	// .\Data\UI\Chatlist2.tif
#define	SPR_CHATLIST_WND3               	167778207   	// .\Data\UI\Chatlist3.tif
#define	SPR_CHATLIST_CLOSE1             	167778208   	// .\Data\UI\menu_1.tga
#define	SPR_CHATLIST_CLOSE2             	167778209   	// .\Data\UI\menu_1.tga
#define	SPR_CHATLIST_SC                 	167778210   	// .\Data\UI\menu_1.tga
#define	SPR_CHAR_WINDOWS2               	167778211   	// .\Data\UI\Chrwindow2.tif
#define	SPR_OPTION_SHADOW               	167778214   	// .\Data\UI\checkv1.tif
#define	SPR_OPTION_BGM                  	167778215   	// .\Data\UI\checkv1.tif
#define	SPR_OPTION_EFFECT               	167778216   	// .\Data\UI\checkv1.tif
#define	SPR_ITEMSHOPWND                 	167778217   	// .\Data\UI\itemshopwnd.tif
#define	SPR_ITEMSHOPWND_CLOSE1          	167778218   	// .\Data\UI\menu_1.tga
#define	SPR_ITEMSHOPWND_CLOSE2          	167778219   	// .\Data\UI\menu_1.tga
#define	SPR_OPTION_EFCHK                	167778249   	// .\Data\UI\checkv1.tif
#define	SPR_CHATLIST_NAME               	167778250   	// .\Data\UI\menu_1.tga
#define	SPR_NPCDUNGEONINFO_WND1         	167778251   	// .\Data\UI\df_window1.tif
#define	SPR_NPCDUNGEONINFO_WND2         	167778252   	// .\Data\UI\df_window2.tif
#define	SPR_NPCDUNGEONINFO_WND3         	167778253   	// .\Data\UI\df_window3.tif
#define	SPR_NPCDUNGEONINFO_NAME         	167778254   	// .\Data\UI\menu_1.tga
#define	SPR_NPCDUNGEONINFO_CLOSE1       	167778255   	// .\Data\UI\menu_1.tga
#define	SPR_NPCDUNGEONINFO_CLOSE2       	167778256   	// .\Data\UI\menu_1.tga
#define	SPR_QUANTITY_WND                	167778270   	// .\Data\UI\quantitywindow.tif
#define	SPR_QUANTITY_CLOSE1             	167778271   	// .\Data\UI\menu_1.tga
#define	SPR_QUANTITY_CLOSE2             	167778272   	// .\Data\UI\menu_1.tga
#define	SPR_MONEY_BUTTON1               	167778273   	// .\Data\UI\menu_2.tga
#define	SPR_MONEY_BUTTON2               	167778274   	// .\Data\UI\menu_2.tga
#define	SPR_MONEY_BUTTON3               	167778275   	// .\Data\UI\menu_2.tga
#define	SPR_NAME_TRUE1                  	167778276   	// .\Data\UI\menu_3.tga
#define	SPR_NAME_TRUE2                  	167778277   	// .\Data\UI\menu_3.tga
#define	SPR_NAME_TRUE3                  	167778278   	// .\Data\UI\menu_3.tga
#define	SPR_NAME_FALSE1                 	167778279   	// .\Data\UI\menu_3.tga
#define	SPR_NAME_FALSE2                 	167778280   	// .\Data\UI\menu_3.tga
#define	SPR_NAME_FALSE3                 	167778281   	// .\Data\UI\menu_3.tga
#define	SPR_EXIT_WND1                   	167778282   	// .\Data\UI\df_window1.tif
#define	SPR_EXIT_WND2                   	167778283   	// .\Data\UI\df_window2.tif
#define	SPR_EXIT_WND3                   	167778284   	// .\Data\UI\df_window3.tif
#define	SPR_EXIT_CLOSE1                 	167778285   	// .\Data\UI\menu_1.tga
#define	SPR_EXIT_CLOSE2                 	167778286   	// .\Data\UI\menu_1.tga
#define	SPR_STORE_WND1                  	167778287   	// .\Data\UI\df_window1.tif
#define	SPR_STORE_WND2                  	167778288   	// .\Data\UI\df_window2.tif
#define	SPR_STORE_WND3                  	167778289   	// .\Data\UI\df_window3.tif
#define	SPR_STORE_CLOSE1                	167778290   	// .\Data\UI\menu_1.tga
#define	SPR_STORE_CLOSE2                	167778291   	// .\Data\UI\menu_1.tga
#define	SPR_STORE_TAB                   	167778292   	// .\Data\UI\itembank1.tif
#define	SPR_STORE_CHK1                  	167778293   	// .\Data\UI\item_menu1.tif
#define	SPR_STORE_CHK2                  	167778294   	// .\Data\UI\item_menu2.tif
#define	SPR_BANK_BTN1                   	167778295   	// .\Data\UI\menu_3.tga
#define	SPR_BANK_BTN2                   	167778296   	// .\Data\UI\menu_3.tga
#define	SPR_BANK_BTN3                   	167778297   	// .\Data\UI\menu_3.tga
#define	SPR_BANK_WND1                   	167778298   	// .\Data\UI\df_window1.tif
#define	SPR_BANK_WND2                   	167778299   	// .\Data\UI\df_window2.tif
#define	SPR_BANK_WND3                   	167778300   	// .\Data\UI\itembank2.tif
#define	SPR_BANK_CHK1                   	167778301   	// .\Data\UI\item_menu1.tif
#define	SPR_BANK_CHK2                   	167778302   	// .\Data\UI\item_menu2.tif
#define	SPR_BANK_BAR                    	167778303   	// .\Data\UI\itembank1.tif
#define	SPR_DUNGEON_WND1                	167778304   	// .\Data\UI\dungeon_window1.tif
#define	SPR_DUNGEON_WND2                	167778305   	// .\Data\UI\dungeon_window2.tif
#define	SPR_DUNGEON_WND3                	167778306   	// .\Data\UI\dungeon_window3.tif
#define	SPR_DUNGEONMANAGER_WND          	167778307   	// .\Data\UI\dungeon_manage1.tif
#define	SPR_DUNGEONMANAGER_CLOSE1       	167778308   	// .\Data\UI\menu_1.tga
#define	SPR_DUNGEONMANAGER_CLOSE2       	167778309   	// .\Data\UI\menu_1.tga
#define	SPR_BANK_CLOSE1                 	167778310   	// .\Data\UI\menu_1.tga
#define	SPR_BANK_CLOSE2                 	167778311   	// .\Data\UI\menu_1.tga
#define	SPR_DUNGEONINFO_CLOSE1          	167778312   	// .\Data\UI\menu_1.tga
#define	SPR_DUNGEONINFO_CLOSE2          	167778313   	// .\Data\UI\menu_1.tga
#define	SPR_DUNGEONINFO_WND1            	167778314   	// .\Data\UI\dungeon_window1.tif
#define	SPR_DUNGEONINFO_WND2            	167778315   	// .\Data\UI\dungeon_window2.tif
#define	SPR_DUNGEONINFO_WND3            	167778316   	// .\Data\UI\dungeon_window2.tif
#define	SPR_DUNGEONINFO_WND4            	167778317   	// .\Data\UI\dungeon_window2.tif
#define	SPR_DUNGEONINFO_WND5            	167778318   	// .\Data\UI\dungeon_window2.tif
#define	SPR_DUNGEONINFO_WND6            	167778319   	// .\Data\UI\dungeon_window2.tif
#define	SPR_DUNGEONINFO_WND7            	167778320   	// .\Data\UI\dungeon_window2.tif
#define	SPR_DUNGEONINFO_WND8            	167778321   	// .\Data\UI\dungeon_window2.tif
#define	SPR_DUNGEONINFO_WND10           	167778322   	// .\Data\UI\dungeon_window3.tif
#define	SPR_SC_BG                       	167778340   	// .\Data\UI\menu_1.tga
#define	SPR_BANK_SC                     	167778341   	// .\Data\UI\menu_1.tga
#define	SPR_GUILD_CREATE_WND            	167778342   	// .\Data\UI\okwindow.tif
#define	SPR_GUILD_OK1                   	167778343   	// .\Data\UI\menu_3.tga
#define	SPR_GUILD_OK2                   	167778344   	// .\Data\UI\menu_3.tga
#define	SPR_GUILD_OK3                   	167778345   	// .\Data\UI\menu_3.tga
#define	SPR_GUILD_CANCEL1               	167778346   	// .\Data\UI\menu_3.tga
#define	SPR_GUILD_CANCEL2               	167778347   	// .\Data\UI\menu_3.tga
#define	SPR_GUILD_CANCEL3               	167778348   	// .\Data\UI\menu_3.tga
#define	SPR_GUILD_CLOSE1                	167778349   	// .\Data\UI\menu_1.tga
#define	SPR_GUILD_CLOSE2                	167778350   	// .\Data\UI\menu_1.tga
#define	SPR_GUILDEX_WND                 	167778351   	// .\Data\UI\okwindow.tif
#define	SPR_GUILDEX_CLOSE1              	167778352   	// .\Data\UI\menu_1.tga
#define	SPR_GUILDEX_CLOSE2              	167778353   	// .\Data\UI\menu_1.tga
#define	SPR_GUILDEX_OK1                 	167778354   	// .\Data\UI\menu_3.tga
#define	SPR_GUILDEX_OK2                 	167778355   	// .\Data\UI\menu_3.tga
#define	SPR_GUILDEX_OK3                 	167778356   	// .\Data\UI\menu_3.tga
#define	SPR_GUILDEX_CANCEL1             	167778357   	// .\Data\UI\menu_3.tga
#define	SPR_GUILDEX_CANCEL2             	167778358   	// .\Data\UI\menu_3.tga
#define	SPR_GUILDEX_CANCEL3             	167778359   	// .\Data\UI\menu_3.tga
#define	SPR_BANK_MONEY1                 	167778360   	// .\Data\UI\menu_2.tga
#define	SPR_BANK_MONEY2                 	167778361   	// .\Data\UI\menu_2.tga
#define	SPR_BANK_MONEY3                 	167778362   	// .\Data\UI\menu_2.tga
#define	SPR_MOUSE_POINTER1              	167778376   	// .\Data\UI\cursor.TIF
#define	SPR_MOUSE_POINTER2              	167778377   	// .\Data\UI\cursor.TIF
#define	SPR_MOUSE_POINTER3              	167778378   	// .\Data\UI\cursor.TIF
#define	SPR_MOUSE_POINTER4              	167778379   	// .\Data\UI\cursor.TIF
#define	SPR_MOUSE_POINTER5              	167778380   	// .\Data\UI\cursor.TIF
#define	SPR_MOUSE_POINTER6              	167778381   	// .\Data\UI\cursor.TIF
#define	SPR_MOUSE_POINTER7              	167778382   	// .\Data\UI\cursor.TIF
#define	SPR_MOUSE_POINTER8              	167778383   	// .\Data\UI\cursor.TIF
#define	SPR_MOUSE_POINTER9              	167778384   	// .\Data\UI\cursor.TIF
#define	SPR_MOUSE_POINTER10             	167778385   	// .\Data\UI\cursor.TIF
#define	SPR_MOUSE_POINTER11             	167778386   	// .\Data\UI\cursor.TIF
#define	SPR_MOUSE_POINTER12             	167778387   	// .\Data\UI\cursor.TIF
#define	SPR_MOUSE_POINTER13             	167778388   	// .\Data\UI\cursor.TIF
#define	SPR_MOUSE_POINTER14             	167778389   	// .\Data\UI\cursor.TIF
#define	SPR_MOUSE_POINTER15             	167778390   	// .\Data\UI\cursor.TIF
#define	SPR_MOUSE_POINTER16             	167778391   	// .\Data\UI\cursor.TIF
#define	SPR_DUNGEONMANAGE_COMBOBAR_OWNER	167778458   	// .\Data\UI\menu_4.tif
#define	SPR_DUNGEONMANAGE_COMBOSELECT_OWNER	167778459   	// .\Data\UI\menu_4.tif
#define	SPR_GUILDJOIN_CREATEWND         	167778462   	// .\Data\UI\okwindow.tif
#define	SPR_GUILDJOIN_OK1               	167778463   	// .\Data\UI\menu_3.tga
#define	SPR_GUILDJOIN_OK2               	167778464   	// .\Data\UI\menu_3.tga
#define	SPR_GUILDJOIN_OK3               	167778465   	// .\Data\UI\menu_3.tga
#define	SPR_GUILDJOIN_CANCEL1           	167778466   	// .\Data\UI\menu_3.tga
#define	SPR_GUILDJOIN_CANCEL2           	167778467   	// .\Data\UI\menu_3.tga
#define	SPR_GUILDJOIN_CANCEL3           	167778468   	// .\Data\UI\menu_3.tga
#define	SPR_GUILDJOIN_CLOSE1            	167778469   	// .\Data\UI\menu_1.tga
#define	SPR_GUILDJOIN_CLOSE2            	167778470   	// .\Data\UI\menu_1.tga
#define	SPR_ITEMWINDOW2                 	167778471   	// .\Data\UI\itemwindow1.tif
#define	SPR_GUARDIAN_WND1               	167778472   	// .\Data\UI\guardian_gauge.tif
#define	SPR_GUARDIAN_WND2               	167778473   	// .\Data\UI\guardian_gauge.tif
#define	SPR_GUARDIAN_WND3               	167778474   	// .\Data\UI\guardian_gauge.tif
#define	SPR_GUARDIAN_WND4               	167778475   	// .\Data\UI\guardian_gauge.tif
#define	SPR_GUARDIAN_WND5               	167778476   	// .\Data\UI\guardian_gauge.tif
#define	SPR_GUILDRANK_WND               	167778477   	// .\Data\UI\okwindow.tif
#define	SPR_GUILDRANK_OK1               	167778478   	// .\Data\UI\menu_3.tga
#define	SPR_GUILDRANK_OK2               	167778479   	// .\Data\UI\menu_3.tga
#define	SPR_GUILDRANK_OK3               	167778480   	// .\Data\UI\menu_3.tga
#define	SPR_GUILDRANK_CANCEL1           	167778481   	// .\Data\UI\menu_3.tga
#define	SPR_GUILDRANK_CANCEL2           	167778482   	// .\Data\UI\menu_3.tga
#define	SPR_GUILDRANK_CANCEL3           	167778483   	// .\Data\UI\menu_3.tga
#define	SPR_GUILDRANK_CLOSE1            	167778484   	// .\Data\UI\menu_1.tga
#define	SPR_GUILDRANK_CLOSE2            	167778485   	// .\Data\UI\menu_1.tga
#define	SPR_GUILD_PAGE                  	167778486   	// .\Data\UI\menu_1.tga
#define	SPR_GUILD_PAGEPREV1             	167778487   	// .\Data\UI\menu_1.tga
#define	SPR_GUILD_PAGEPREV2             	167778488   	// .\Data\UI\menu_1.tga
#define	SPR_GUILD_PAGEPREV3             	167778489   	// .\Data\UI\menu_1.tga
#define	SPR_GUILD_PAGENEXT1             	167778490   	// .\Data\UI\menu_1.tga
#define	SPR_GUILD_PAGENEXT2             	167778491   	// .\Data\UI\menu_1.tga
#define	SPR_GUILD_PAGENEXT3             	167778492   	// .\Data\UI\menu_1.tga
#define	SPR_GUILDEDIT_WND               	167778493   	// .\Data\UI\okwindow.tif
#define	SPR_GUILDEDIT_OK1               	167778494   	// .\Data\UI\menu_3.tga
#define	SPR_GUILDEDIT_OK2               	167778495   	// .\Data\UI\menu_3.tga
#define	SPR_GUILDEDIT_OK3               	167778496   	// .\Data\UI\menu_3.tga
#define	SPR_GUILDEDIT_CANCEL1           	167778497   	// .\Data\UI\menu_3.tga
#define	SPR_GUILDEDIT_CANCEL2           	167778498   	// .\Data\UI\menu_3.tga
#define	SPR_GUILDEDIT_CANCEL3           	167778499   	// .\Data\UI\menu_3.tga
#define	SPR_GUILDEDIT_BAR               	167778500   	// .\Data\UI\guile_edit1.tif
#define	SPR_GUILDEDIT_CLOSE1            	167778501   	// .\Data\UI\menu_1.tga
#define	SPR_GUILDEDIT_CLOSE2            	167778502   	// .\Data\UI\menu_1.tga
#define	SPR_GUILDEDIT_CHK               	167778503   	// .\Data\UI\checkv1.tif
#define	SPR_PARTYJOIN_WND               	167778504   	// .\Data\UI\okwindow.tif
#define	SPR_PARTYJOIN_OK1               	167778505   	// .\Data\UI\menu_3.tga
#define	SPR_PARTYJOIN_OK2               	167778506   	// .\Data\UI\menu_3.tga
#define	SPR_PARTYJOIN_OK3               	167778507   	// .\Data\UI\menu_3.tga
#define	SPR_PARTYJOIN_CANCEL1           	167778508   	// .\Data\UI\menu_3.tga
#define	SPR_PARTYJOIN_CANCEL2           	167778509   	// .\Data\UI\menu_3.tga
#define	SPR_PARTYJOIN_CANCEL3           	167778510   	// .\Data\UI\menu_3.tga
#define	SPR_PARTYJOIN_CLOSE1            	167778511   	// .\Data\UI\menu_1.tga
#define	SPR_PARTYJOIN_CLOSE2            	167778512   	// .\Data\UI\menu_1.tga
#define	SPR_MOUSE_POINTER17             	167778522   	// .\Data\UI\cursor.TIF
#define	SPR_MOUSE_POINTER18             	167778523   	// .\Data\UI\cursor.TIF
#define	SPR_MONSTERHP_BAR1              	167778524   	// .\Data\UI\menu_4.tif
#define	SPR_MONSTER_GAGE1               	167778525   	// .\Data\UI\menu_1.tga
#define	SPR_MONSTER_GAGE2               	167778526   	// .\Data\UI\menu_4.tif
#define	SPR_MONSTERHP_BAR2              	167778527   	// .\Data\UI\menu_4.tif
#define	SPR_STOREWND_PAGE               	167778528   	// .\Data\UI\menu_1.tga
#define	SPR_STOREWND_PREV1              	167778529   	// .\Data\UI\menu_1.tga
#define	SPR_STOREWND_PREV2              	167778530   	// .\Data\UI\menu_1.tga
#define	SPR_STOREWND_PREV3              	167778531   	// .\Data\UI\menu_1.tga
#define	SPR_STOREWND_NEXT1              	167778532   	// .\Data\UI\menu_1.tga
#define	SPR_STOREWND_NEXT2              	167778533   	// .\Data\UI\menu_1.tga
#define	SPR_STOREWND_NEXT3              	167778534   	// .\Data\UI\menu_1.tga
#define	SPR_ITEMWND_GUARDIAN_ON1        	167778557   	// .\Data\UI\menu_1.tga
#define	SPR_ITEMWND_GUARDIAN_ON2        	167778558   	// .\Data\UI\menu_1.tga
#define	SPR_ITEMWND_GUARDIAN_OFF1       	167778559   	// .\Data\UI\menu_1.tga
#define	SPR_ITEMWND_GUARDIAN_OFF2       	167778560   	// .\Data\UI\menu_1.tga
#define	SPR_GUARDIAN_EN                 	167778571   	// .\Data\UI\menu_1.tga
#define	SPR_DEFAULT_WND                 	167778572   	// .\Data\UI\okwindow.tif
#define	SPR_DEFAULT_CLOSE1              	167778573   	// .\Data\UI\menu_1.tga
#define	SPR_DEFAULT_CLOSE2              	167778574   	// .\Data\UI\menu_1.tga
#define	SPR_DEFAULT_OK1                 	167778575   	// .\Data\UI\menu_3.tga
#define	SPR_DEFAULT_OK2                 	167778576   	// .\Data\UI\menu_3.tga
#define	SPR_DEFAULT_OK3                 	167778577   	// .\Data\UI\menu_3.tga
#define	SPR_DEFAULT_CANCEL1             	167778578   	// .\Data\UI\menu_3.tga
#define	SPR_DEFAULT_CANCEL2             	167778579   	// .\Data\UI\menu_3.tga
#define	SPR_DEFAULT_CANCEL3             	167778580   	// .\Data\UI\menu_3.tga
#define	SPR_GAMEDEFAULT_WND             	167778581   	// .\Data\UI\okwindow2.tif
#define	SPR_GAMEDEFAULT_CLOSE1          	167778582   	// .\Data\UI\menu_1.tga
#define	SPR_GAMEDEFAULT_CLOSE2          	167778583   	// .\Data\UI\menu_1.tga
#define	SPR_GAMEDEFAULT_OK1             	167778584   	// .\Data\UI\menu_3.tga
#define	SPR_GAMEDEFAULT_OK2             	167778585   	// .\Data\UI\menu_3.tga
#define	SPR_GAMEDEFAULT_OK3             	167778586   	// .\Data\UI\menu_3.tga
#define	SPR_HELP_WND1                   	167778587   	// .\Data\UI\df_window1.tif
#define	SPR_HELP_WND2                   	167778588   	// .\Data\UI\df_window2.tif
#define	SPR_HELP_WND3                   	167778589   	// .\Data\UI\df_window3.tif
#define	SPR_HELP_CLOSE1                 	167778590   	// .\Data\UI\menu_1.tga
#define	SPR_HELP_CLOSE2                 	167778591   	// .\Data\UI\menu_1.tga
#define	SPR_ERROR_WND                   	167778592   	// .\Data\UI\okwindow2.tif
#define	SPR_ERROR_OK1                   	167778593   	// .\Data\UI\menu_3.tga
#define	SPR_ERROR_OK2                   	167778594   	// .\Data\UI\menu_3.tga
#define	SPR_ERROR_OK3                   	167778595   	// .\Data\UI\menu_3.tga
#define	SPR_LOGIN_EXIT1                 	167778596   	// .\Data\UI\menu_3.tga
#define	SPR_LOGIN_EXIT2                 	167778597   	// .\Data\UI\menu_3.tga
#define	SPR_LOGIN_EXIT3                 	167778598   	// .\Data\UI\menu_3.tga
#define	SPR_CP_GUAGEBAR                 	167778599   	// .\Data\UI\menu_1.tga
#define	SPR_CP_GUAGEBOARD               	167778600   	// .\Data\UI\menu_1.tga
#define	SPR_GAMEDEFAULT_CANCEL1         	167778601   	// .\Data\UI\menu_3.tga
#define	SPR_GAMEDEFAULT_CANCEL2         	167778602   	// .\Data\UI\menu_3.tga
#define	SPR_GAMEDEFAULT_CANCEL3         	167778603   	// .\Data\UI\menu_3.tga
#define	SPR_ITEMTRADE_WND               	167778605   	// .\Data\UI\okwindow.tif
#define	SPR_ITEMTRADE_CLOSE1            	167778607   	// .\Data\UI\menu_1.tga
#define	SPR_ITEMTRADE_CLOSE2            	167778608   	// .\Data\UI\menu_1.tga
#define	SPR_ITEMTRADE_OK1               	167778609   	// .\Data\UI\menu_3.tga
#define	SPR_ITEMTRADE_OK2               	167778610   	// .\Data\UI\menu_3.tga
#define	SPR_ITEMTRADE_OK3               	167778611   	// .\Data\UI\menu_3.tga
#define	SPR_ITEMTRADE_CANCEL1           	167778612   	// .\Data\UI\menu_3.tga
#define	SPR_ITEMTRADE_CANCEL2           	167778613   	// .\Data\UI\menu_3.tga
#define	SPR_ITEMTRADE_CANCEL3           	167778614   	// .\Data\UI\menu_3.tga
#define	SPR_ITEMTRADESHOP_WND1          	167778615   	// .\Data\UI\TradeWindow.tif
#define	SPR_ITEMTRADESHOP_WND2          	167778616   	// .\Data\UI\TradeWindow.tif
#define	SPR_ITEMTRADESHOP_CLOSE1        	167778617   	// .\Data\UI\menu_1.tga
#define	SPR_ITEMTRADESHOP_CLOSE2        	167778618   	// .\Data\UI\menu_1.tga
#define	SPR_ITEMTRADESHOP_TRADE1        	167778619   	// .\Data\UI\menu_4.tga
#define	SPR_ITEMTRADESHOP_TRADE2        	167778620   	// .\Data\UI\menu_4.tga
#define	SPR_ITEMTRADESHOP_TRADE3        	167778621   	// .\Data\UI\menu_4.tga
#define	SPR_ITEMTRADESHOP_TRADE4        	167778622   	// .\Data\UI\menu_4.tga
#define	SPR_ITEMTRADESHOP_OK1_1         	167778623   	// .\Data\UI\menu_4.tga
#define	SPR_ITEMTRADESHOP_OK1_2         	167778624   	// .\Data\UI\menu_3.tga
#define	SPR_ITEMTRADESHOP_OK1_3         	167778625   	// .\Data\UI\menu_3.tga
#define	SPR_ITEMTRADESHOP_OK1_4         	167778626   	// .\Data\UI\menu_3.tga
#define	SPR_ITEMTRADESHOP_CANCEL1       	167778627   	// .\Data\UI\menu_3.tga
#define	SPR_ITEMTRADESHOP_CANCEL2       	167778628   	// .\Data\UI\menu_3.tga
#define	SPR_ITEMTRADESHOP_CANCEL3       	167778629   	// .\Data\UI\menu_3.tga
#define	SPR_ITEMTRADESHOP_OK2_1         	167778633   	// .\Data\UI\menu_4.tga
#define	SPR_ITEMTRADESHOP_OK2_2         	167778634   	// .\Data\UI\menu_3.tga
#define	SPR_ITEMTRADESHOP_OK2_3         	167778635   	// .\Data\UI\menu_3.tga
#define	SPR_ITEMTRADESHOP_OK2_4         	167778636   	// .\Data\UI\menu_3.tga
#define	SPR_DSMAP_USER                  	167778637   	// .\Data\UI\menu_4.tif
#define	SPR_SPEAKING_BOX1               	167778653   	// .\Data\UI\speaking_box.tif
#define	SPR_OK_CREATE_WND               	167778654   	// .\Data\UI\okwindow.tif
#define	SPR_OK_OK1                      	167778655   	// .\Data\UI\menu_3.tga
#define	SPR_OK_OK2                      	167778656   	// .\Data\UI\menu_3.tga
#define	SPR_OK_OK3                      	167778657   	// .\Data\UI\menu_3.tga
#define	SPR_OK_CANCEL1                  	167778658   	// .\Data\UI\menu_3.tga
#define	SPR_OK_CANCEL2                  	167778659   	// .\Data\UI\menu_3.tga
#define	SPR_OK_CANCEL3                  	167778660   	// .\Data\UI\menu_3.tga
#define	SPR_OK_CLOSE1                   	167778661   	// .\Data\UI\menu_1.tga
#define	SPR_OK_CLOSE2                   	167778662   	// .\Data\UI\menu_1.tga
#define	SPR_BELT_INFO                   	167778668   	// .\Data\UI\speaking_box.tif
#define	SPR_BELT_WIDTH1                 	167778669   	// .\Data\UI\menu_4.tif
#define	SPR_BELT_WIDTH2                 	167778670   	// .\Data\UI\menu_4.tif
#define	SPR_BELT_HEIGHT1                	167778671   	// .\Data\UI\menu_4.tif
#define	SPR_BELT_HEIGHT2                	167778672   	// .\Data\UI\menu_4.tif
#define	SPR_PARTY_RED                   	167778673   	// .\Data\UI\menu_1.tga
#define	SPR_PARTY_BLUE                  	167778674   	// .\Data\UI\menu_1.tga
#define	SPR_PARTY_WHITE                 	167778675   	// .\Data\UI\menu_1.tga
#define	SPR_PARTY_GREEN                 	167778676   	// .\Data\UI\menu_1.tga
#define	SPR_PARTY_BORA                  	167778677   	// .\Data\UI\menu_1.tga
#define	SPR_PARTY_YELLOW                	167778678   	// .\Data\UI\menu_1.tga
#define	SPR_PARTY_ORANGE                	167778679   	// .\Data\UI\menu_1.tga
#define	SPR_PARTY_PINK                  	167778680   	// .\Data\UI\menu_1.tga
#define	SPR_CP_MINIMAPDOT1              	167778681   	// .\Data\UI\menu_1.tga
#define	SPR_CP_MINIMAPDOT2              	167778682   	// .\Data\UI\menu_1.tga
#define	SPR_CP_MINIMAPDOT3              	167778683   	// .\Data\UI\menu_1.tga
#define	SPR_CP_MINIMAPDOT4              	167778684   	// .\Data\UI\menu_1.tga
#define	SPR_CP_MINIMAPDOT5              	167778685   	// .\Data\UI\menu_1.tga
#define	SPR_CP_MINIMAPDOT6              	167778686   	// .\Data\UI\menu_1.tga
#define	SPR_CP_MINIMAPDOT7              	167778687   	// .\Data\UI\menu_1.tga
#define	SPR_CP_MINIMAPDOT8              	167778688   	// .\Data\UI\menu_1.tga
#define	SPR_CP_MINIMAPDOT9              	167778689   	// .\Data\UI\menu_1.tga
#define	SPR_CP_MINIMAPDOT10             	167778690   	// .\Data\UI\menu_1.tga
#define	SPR_CP_MINIMAPDOT11             	167778691   	// .\Data\UI\menu_1.tga
#define	SPR_CP_MINIMAPDOT12             	167778692   	// .\Data\UI\menu_1.tga
#define	SPR_CP_MINIMAPDOT13             	167778693   	// .\Data\UI\menu_1.tga
#define	SPR_CP_MINIMAPDOT14             	167778694   	// .\Data\UI\menu_1.tga
#define	SPR_CP_MINIMAPDOT15             	167778695   	// .\Data\UI\menu_1.tga
#define	SPR_CP_MINIMAPDOT16             	167778696   	// .\Data\UI\menu_1.tga
#define	SPR_CP_MINIMAPDOT17             	167778697   	// .\Data\UI\menu_1.tga
#define	SPR_CP_MINIMAPDOT18             	167778698   	// .\Data\UI\menu_1.tga
#define	SPR_CP_MINIMAPDOT19             	167778699   	// .\Data\UI\menu_1.tga
#define	SPR_CP_MINIMAPDOT20             	167778700   	// .\Data\UI\menu_1.tga
#define	SPR_GROUPWND                    	167778701   	// .\Data\UI\group_window.tif
#define	SPR_GROUPWND_CLOSE1             	167778702   	// .\Data\UI\menu_1.tga
#define	SPR_GROUPWND_CLOSE2             	167778703   	// .\Data\UI\menu_1.tga
#define	SPR_GROUPWND_PARTY1             	167778704   	// .\Data\UI\menu_4.tif
#define	SPR_GROUPWND_PARTY2             	167778705   	// .\Data\UI\menu_4.tif
#define	SPR_GROUPWND_GUILD1             	167778706   	// .\Data\UI\menu_4.tif
#define	SPR_GROUPWND_GUILD2             	167778707   	// .\Data\UI\menu_4.tif
#define	SPR_GROUPWND_FRIEND1            	167778708   	// .\Data\UI\menu_4.tif
#define	SPR_GROUPWND_FRIEND2            	167778709   	// .\Data\UI\menu_4.tif
#define	SPR_GROUPWND_ADD1               	167778710   	// .\Data\UI\menu_4.tga
#define	SPR_GROUPWND_ADD2               	167778711   	// .\Data\UI\menu_4.tga
#define	SPR_GROUPWND_DELETE1            	167778712   	// .\Data\UI\menu_4.tga
#define	SPR_GROUPWND_DELETE2            	167778713   	// .\Data\UI\menu_4.tga
#define	SPR_GROUPWND_INFO1              	167778714   	// .\Data\UI\menu_4.tga
#define	SPR_GROUPWND_INFO2              	167778715   	// .\Data\UI\menu_4.tga
#define	SPR_GROUPWND_CREATE1            	167778716   	// .\Data\UI\menu_4.tga
#define	SPR_GROUPWND_CREATE2            	167778717   	// .\Data\UI\menu_4.tga
#define	SPR_GROUPWND_OUT1               	167778718   	// .\Data\UI\menu_4.tga
#define	SPR_GROUPWND_OUT2               	167778719   	// .\Data\UI\menu_4.tga
#define	SPR_GROUPWND_ADD3               	167778720   	// .\Data\UI\menu_4.tga
#define	SPR_GROUPWND_DELETE3            	167778721   	// .\Data\UI\menu_4.tga
#define	SPR_GROUPWND_INFO3              	167778722   	// .\Data\UI\menu_4.tga
#define	SPR_GROUPWND_CREATE3            	167778723   	// .\Data\UI\menu_4.tga
#define	SPR_GROUPWND_OUT3               	167778724   	// .\Data\UI\menu_4.tga
#define	SPR_GROUPWND_SELECT             	167778725   	// .\Data\UI\menu_4.tif
#define	SPR_PLAYER_SHOP_WND             	167778726   	// .\Data\UI\playershop_a.tif
#define	SPR_GROUPWND_PARTYBAR1          	167778727   	// .\Data\UI\menu_4.tif
#define	SPR_GROUPWND_PARTYBAR2          	167778728   	// .\Data\UI\menu_4.tif
#define	SPR_GROUPWND_PARTYBAR3          	167778729   	// .\Data\UI\menu_4.tif
#define	SPR_GROUPWND_PARTYBAR4          	167778730   	// .\Data\UI\menu_4.tif
#define	SPR_GROUPWND_PARTYBAR5          	167778731   	// .\Data\UI\menu_4.tif
#define	SPR_GROUPWND_PARTYBAR6          	167778732   	// .\Data\UI\menu_4.tif
#define	SPR_GROUPWND_PARTYBAR7          	167778733   	// .\Data\UI\menu_4.tif
#define	SPR_GROUPWND_PARTYGAGE1         	167778734   	// .\Data\UI\menu_1.tga
#define	SPR_GROUPWND_PARTYGAGE2         	167778735   	// .\Data\UI\menu_1.tga
#define	SPR_GROUPWND_PARTYGAGE3         	167778736   	// .\Data\UI\menu_1.tga
#define	SPR_GROUPWND_PARTYGAGE4         	167778737   	// .\Data\UI\menu_1.tga
#define	SPR_GROUPWND_PARTYGAGE5         	167778738   	// .\Data\UI\menu_1.tga
#define	SPR_GROUPWND_PARTYGAGE6         	167778739   	// .\Data\UI\menu_1.tga
#define	SPR_GROUPWND_PARTYGAGE7         	167778740   	// .\Data\UI\menu_1.tga
#define	SPR_PARTYPORTAL_WND             	167778741   	// .\Data\UI\okwindow.tif
#define	SPR_PARTYPORTAL_CLOSE1          	167778742   	// .\Data\UI\menu_1.tga
#define	SPR_PARTYPORTAL_CLOSE2          	167778743   	// .\Data\UI\menu_1.tga
#define	SPR_PARTYPORTAL_OK1             	167778744   	// .\Data\UI\menu_3.tga
#define	SPR_PARTYPORTAL_OK2             	167778745   	// .\Data\UI\menu_3.tga
#define	SPR_PARTYPORTAL_OK3             	167778746   	// .\Data\UI\menu_3.tga
#define	SPR_PARTYPORTAL_CANCEL1         	167778747   	// .\Data\UI\menu_3.tga
#define	SPR_PARTYPORTAL_CANCEL2         	167778748   	// .\Data\UI\menu_3.tga
#define	SPR_PARTYPORTAL_CANCEL3         	167778749   	// .\Data\UI\menu_3.tga
#define	SPR_UI_TRADE1                   	167778750   	// .\Data\UI\menu_1.tga
#define	SPR_UI_TRADE2                   	167778751   	// .\Data\UI\menu_1.tga
#define	SPR_UI_TRADE3                   	167778752   	// .\Data\UI\menu_1.tga
#define	SPR_UI_REST1                    	167778753   	// .\Data\UI\menu_1.tga
#define	SPR_UI_REST2                    	167778754   	// .\Data\UI\menu_1.tga
#define	SPR_UI_REST3                    	167778755   	// .\Data\UI\menu_1.tga
#define	SPR_UI_SHOP1                    	167778756   	// .\Data\UI\menu_1.tga
#define	SPR_UI_SHOP2                    	167778757   	// .\Data\UI\menu_1.tga
#define	SPR_UI_SHOP3                    	167778758   	// .\Data\UI\menu_1.tga
#define	SPR_UI_MATCH1                   	167778759   	// .\Data\UI\menu_1.tga
#define	SPR_UI_MATCH2                   	167778760   	// .\Data\UI\menu_1.tga
#define	SPR_UI_MATCH3                   	167778761   	// .\Data\UI\menu_1.tga
#define	SPR_REQUESTMATCH_WND            	167778762   	// .\Data\UI\okwindow.tif
#define	SPR_REQUESTMATCH_CLOSE1         	167779193   	// .\Data\UI\menu_1.tga
#define	SPR_REQUESTMATCH_CLOSE2         	167779194   	// .\Data\UI\menu_1.tga
#define	SPR_REQUESTMATCH_OK1            	167779195   	// .\Data\UI\menu_3.tga
#define	SPR_REQUESTMATCH_OK2            	167779196   	// .\Data\UI\menu_3.tga
#define	SPR_REQUESTMATCH_OK3            	167779197   	// .\Data\UI\menu_3.tga
#define	SPR_REQUESTMATCH_CANCEL1        	167779198   	// .\Data\UI\menu_3.tga
#define	SPR_REQUESTMATCH_CANCEL2        	167779199   	// .\Data\UI\menu_3.tga
#define	SPR_REQUESTMATCH_CANCEL3        	167779200   	// .\Data\UI\menu_3.tga
#define	SPR_CHAT_MESSAGE1               	167779206   	// .\Data\UI\menu_4.tga
#define	SPR_CHAT_MESSAGE2               	167779207   	// .\Data\UI\menu_4.tga
#define	SPR_CHAT_MESSAGE3               	167779208   	// .\Data\UI\menu_4.tga
#define	SPR_CHAT_CHAT1                  	167779209   	// .\Data\UI\menu_4.tga
#define	SPR_CHAT_CHAT2                  	167779210   	// .\Data\UI\menu_4.tga
#define	SPR_CHAT_CHAT3                  	167779211   	// .\Data\UI\menu_4.tga
#define	SPR_CHAT_ALL1                   	167779212   	// .\Data\UI\menu_4.tga
#define	SPR_CHAT_ALL2                   	167779213   	// .\Data\UI\menu_4.tga
#define	SPR_CHAT_ALL3                   	167779214   	// .\Data\UI\menu_4.tga
#define	SPR_CHAT_PARTY1                 	167779215   	// .\Data\UI\menu_4.tga
#define	SPR_CHAT_PARTY2                 	167779216   	// .\Data\UI\menu_4.tga
#define	SPR_CHAT_PARTY3                 	167779217   	// .\Data\UI\menu_4.tga
#define	SPR_CHAT_GUILD1                 	167779218   	// .\Data\UI\menu_4.tga
#define	SPR_CHAT_GUILD2                 	167779219   	// .\Data\UI\menu_4.tga
#define	SPR_CHAT_GUILD3                 	167779220   	// .\Data\UI\menu_4.tga
#define	SPR_CHAT_WHISPER1               	167779221   	// .\Data\UI\menu_4.tga
#define	SPR_CHAT_WHISPER2               	167779222   	// .\Data\UI\menu_4.tga
#define	SPR_CHAT_WHISPER3               	167779223   	// .\Data\UI\menu_4.tga
#define	SPR_CHAT_FRIEND1                	167779224   	// .\Data\UI\menu_4.tga
#define	SPR_CHAT_FRIEND2                	167779225   	// .\Data\UI\menu_4.tga
#define	SPR_CHAT_FRIEND3                	167779226   	// .\Data\UI\menu_4.tga
#define	SPR_CHAT_LIST1                  	167779227   	// .\Data\UI\menu_4.tga
#define	SPR_CHAT_LIST2                  	167779228   	// .\Data\UI\menu_4.tga
#define	SPR_CHAT_LIST3                  	167779229   	// .\Data\UI\menu_4.tga
#define	SPR_CHATLIST_UPDOWN1            	167779230   	// .\Data\UI\menu_1.tga
#define	SPR_CHATLIST_UPDOWN2            	167779231   	// .\Data\UI\menu_1.tga
#define	SPR_CHATLIST_ALL1               	167779232   	// .\Data\UI\menu_4.tga
#define	SPR_CHATLIST_ALL2               	167779233   	// .\Data\UI\menu_4.tga
#define	SPR_CHATLIST_ALL3               	167779234   	// .\Data\UI\menu_4.tga
#define	SPR_CHATLIST_PARTY1             	167779235   	// .\Data\UI\menu_4.tga
#define	SPR_CHATLIST_PARTY2             	167779236   	// .\Data\UI\menu_4.tga
#define	SPR_CHATLIST_PARTY3             	167779237   	// .\Data\UI\menu_4.tga
#define	SPR_CHATLIST_GUILD1             	167779238   	// .\Data\UI\menu_4.tga
#define	SPR_CHATLIST_GUILD2             	167779239   	// .\Data\UI\menu_4.tga
#define	SPR_CHATLIST_GUILD3             	167779240   	// .\Data\UI\menu_4.tga
#define	SPR_CHATLIST_FRIEND1            	167779241   	// .\Data\UI\menu_4.tga
#define	SPR_CHATLIST_FRIEND2            	167779242   	// .\Data\UI\menu_4.tga
#define	SPR_CHATLIST_FRIEND3            	167779243   	// .\Data\UI\menu_4.tga
#define	SPR_CHATLIST_WHISPER1           	167779244   	// .\Data\UI\menu_4.tga
#define	SPR_CHATLIST_WHISPER2           	167779245   	// .\Data\UI\menu_4.tga
#define	SPR_CHATLIST_WHISPER3           	167779246   	// .\Data\UI\menu_4.tga
#define	SPR_GROUPWND_MSG1               	167779247   	// .\Data\UI\menu_4.tga
#define	SPR_GROUPWND_MSG2               	167779248   	// .\Data\UI\menu_4.tga
#define	SPR_GROUPWND_MSG3               	167779249   	// .\Data\UI\menu_4.tga
#define	SPR_CHATLIST_SC1                	167779250   	// .\Data\UI\menu_1.tga
#define	SPR_CHATLIST_SC2                	167779251   	// .\Data\UI\menu_1.tga
#define	SPR_CHATLIST_SC3                	167779252   	// .\Data\UI\menu_1.tga
#define	SPR_CHATLIST_SC4                	167779253   	// .\Data\UI\menu_1.tga
#define	SPR_UI_QUICKSLOT_INABLE1        	167779254   	// .\Data\UI\menu_4.tif
#define	SPR_UI_QUICKSLOT_INABLE2        	167779255   	// .\Data\UI\menu_4.tif
#define	SPR_UI_QUICKSLOT_INABLE3        	167779256   	// .\Data\UI\menu_4.tif
#define	SPR_UI_QUICKSLOT_INABLE4        	167779257   	// .\Data\UI\menu_4.tif
#define	SPR_UI_QUICKSLOT_INABLE5        	167779258   	// .\Data\UI\menu_4.tif
#define	SPR_UI_QUICKSLOT_INABLE6        	167779259   	// .\Data\UI\menu_4.tif
#define	SPR_UI_QUICKSLOT_INABLE7        	167779260   	// .\Data\UI\menu_4.tif
#define	SPR_UI_QUICKSLOT_INABLE8        	167779261   	// .\Data\UI\menu_4.tif
#define	SPR_DUNGEONINFOEX_WND1          	167779262   	// .\Data\UI\dungeon_1.tif
#define	SPR_DUNGEONINFOEX_WND2          	167779263   	// .\Data\UI\dungeon_2.tif
#define	SPR_DUNGEONINFOEX_WND3          	167779264   	// .\Data\UI\dungeon_3.tif
#define	SPR_DUNGEONINFOEX_WND4          	167779265   	// .\Data\UI\dungeon_4.tif
#define	SPR_DUNGEONINFOEX_SELECTBAR     	167779266   	// .\Data\UI\menu_5.tga
#define	SPR_DUNGEONINFOEX_PAGEBOX       	167779267   	// .\Data\UI\menu_1.tga
#define	SPR_UI_LIFELINK                 	167779268   	// .\Data\UI\item_icon01.tga
#define	SPR_UI_AMBUSH                   	167779269   	// .\Data\UI\item_icon02.tga
#define	SPR_UI_ST                       	167779270   	// .\Data\UI\item_icon02.tga
#define	SPR_SKILL_WND1                  	167779271   	// .\Data\UI\skillwindow.tif
#define	SPR_SKILL_WND2                  	167779272   	// .\Data\UI\skillwindow_a.tif
#define	SPR_SKILL_MANA1                 	167779273   	// .\Data\UI\menu_4.tif
#define	SPR_SKILL_MANA2                 	167779274   	// .\Data\UI\menu_4.tif
#define	SPR_SKILL_MANA3                 	167779275   	// .\Data\UI\menu_4.tif
#define	SPR_SKILL_AURA1                 	167779276   	// .\Data\UI\menu_4.tif
#define	SPR_SKILL_AURA2                 	167779277   	// .\Data\UI\menu_4.tif
#define	SPR_SKILL_AURA3                 	167779278   	// .\Data\UI\menu_4.tif
#define	SPR_SKILL_DIVINE1               	167779279   	// .\Data\UI\menu_4.tif
#define	SPR_SKILL_DIVINE2               	167779280   	// .\Data\UI\menu_4.tif
#define	SPR_SKILL_DIVINE3               	167779281   	// .\Data\UI\menu_4.tif
#define	SPR_SKILL_SUMMON1               	167779282   	// .\Data\UI\menu_4.tif
#define	SPR_SKILL_SUMMON2               	167779283   	// .\Data\UI\menu_4.tif
#define	SPR_SKILL_SUMMON3               	167779284   	// .\Data\UI\menu_4.tif
#define	SPR_SKILL_CHAKRA1               	167779285   	// .\Data\UI\menu_4.tif
#define	SPR_SKILL_CHAKRA2               	167779286   	// .\Data\UI\menu_4.tif
#define	SPR_SKILL_CHAKRA3               	167779287   	// .\Data\UI\menu_4.tif
#define	SPR_HELP_NEXT1                  	167779288   	// .\Data\UI\menu_5.tga
#define	SPR_HELP_NEXT2                  	167779289   	// .\Data\UI\menu_5.tga
#define	SPR_HELP_NEXT3                  	167779290   	// .\Data\UI\menu_5.tga
#define	SPR_HELP_HP                     	167779291   	// .\Data\UI\help01.tga
#define	SPR_HELP_SP                     	167779292   	// .\Data\UI\help01.tga
#define	SPR_HELP_EXP                    	167779293   	// .\Data\UI\help01.tga
#define	SPR_HELP_COOL                   	167779294   	// .\Data\UI\help01.tga
#define	SPR_HELP_ITEM                   	167779295   	// .\Data\UI\help01.tga
#define	SPR_HELP_CHAR                   	167779296   	// .\Data\UI\help01.tga
#define	SPR_HELP_SKILL                  	167779297   	// .\Data\UI\help01.tga
#define	SPR_HELP_GROUP                  	167779298   	// .\Data\UI\help01.tga
#define	SPR_HELP_DUNGEON                	167779299   	// .\Data\UI\help01.tga
#define	SPR_HELP_CHAT                   	167779300   	// .\Data\UI\help01.tga
#define	SPR_HELP_MATCH                  	167779301   	// .\Data\UI\help01.tga
#define	SPR_HELP_TRADE                  	167779302   	// .\Data\UI\help01.tga
#define	SPR_HELP_REST                   	167779303   	// .\Data\UI\help01.tga
#define	SPR_HELP_PK                     	167779304   	// .\Data\UI\help01.tga
#define	SPR_HELP_OPTION                 	167779305   	// .\Data\UI\help01.tga
#define	SPR_HELP_SHOP                   	167779306   	// .\Data\UI\help01.tga
#define	SPR_HELP_SLOT                   	167779307   	// .\Data\UI\help01.tga
#define	SPR_HELP_SUPPLISE               	167779308   	// .\Data\UI\help01.tga
#define	SPR_HELP_INTERFACE1             	167779309   	// .\Data\UI\help01.tga
#define	SPR_HELP_UP1                    	167779310   	// .\Data\UI\help01.tga
#define	SPR_HELP_UP2                    	167779311   	// .\Data\UI\help01.tga
#define	SPR_HELP_INTERFACE2             	167779312   	// .\Data\UI\help01.tga
#define	SPR_HELP_INTERFACE3             	167779313   	// .\Data\UI\help01.tga
#define	SPR_DUNGEONSIEGEINFOWND1        	167779314   	// .\Data\UI\df_window7.tif
#define	SPR_DUNGEONSIEGEINFOWND2        	167779315   	// .\Data\UI\df_window8.tif
#define	SPR_DUNGEONSIEGEINFOWND3        	167779316   	// .\Data\UI\df_window9.tif
#define	SPR_OPTION_TEXT                 	167779317   	// .\Data\UI\checkv1.tif
#define	SPR_ITEMUPGRADEWND              	167779318   	// .\Data\UI\strength_window.tif
#define	SPR_ITEMUPGRADE_HELPWND         	167779319   	// .\Data\UI\window_help.tif
#define	SPR_OPTION_BGMBOL               	167779325   	// .\Data\UI\menu_1.tga
#define	SPR_OPTION_EFFECTBOL            	167779326   	// .\Data\UI\menu_1.tga
#define	SPR_SELECTCHAR_VILL1_1          	167779327   	// .\Data\UI\menu_5.tga
#define	SPR_SELECTCHAR_VILL1_2          	167779328   	// .\Data\UI\menu_5.tga
#define	SPR_SELECTCHAR_VILL1_3          	167779329   	// .\Data\UI\menu_5.tga
#define	SPR_SELECTCHAR_VILL2_1          	167779330   	// .\Data\UI\menu_5.tga
#define	SPR_SELECTCHAR_VILL2_2          	167779331   	// .\Data\UI\menu_5.tga
#define	SPR_SELECTCHAR_VILL2_3          	167779332   	// .\Data\UI\menu_5.tga
#define	SPR_EVENTDUNGEON_WND            	167779333   	// .\Data\UI\okwindow2.tif
#define	SPR_EVENTDUNGEON_OK1            	167779334   	// .\Data\UI\menu_3.tga
#define	SPR_EVENTDUNGEON_OK2            	167779335   	// .\Data\UI\menu_3.tga
#define	SPR_EVENTDUNGEON_OK3            	167779336   	// .\Data\UI\menu_3.tga
#define	SPR_NOTICE_WND1                 	167779337   	// .\Data\UI\df_window1.tif
#define	SPR_NOTICE_WND2                 	167779338   	// .\Data\UI\df_window2.tif
#define	SPR_NOTICE_WND3                 	167779339   	// .\Data\UI\df_window3.tif
#define	SPR_NOTICE_CLOSE1               	167779340   	// .\Data\UI\menu_1.tga
#define	SPR_NOTICE_CLOSE2               	167779341   	// .\Data\UI\menu_1.tga
#define	SPR_MESSAGE_BOX                 	167779342   	// .\Data\UI\speaking_box.tif
#define	SPR_GUILD_ONOFF1                	167779343   	// .\Data\UI\menu_4.tga
#define	SPR_GUILD_ONOFF2                	167779344   	// .\Data\UI\menu_4.tga
#define	SPR_GUILD_ONOFF3                	167779345   	// .\Data\UI\menu_4.tga
#define	SPR_GUILD_INFO1                 	167779346   	// .\Data\UI\menu_4.tga
#define	SPR_GUILD_INFO2                 	167779347   	// .\Data\UI\menu_4.tga
#define	SPR_GUILD_INFO3                 	167779348   	// .\Data\UI\menu_4.tga
#define	SPR_PARTYREQUEST_WND            	167779349   	// .\Data\UI\okwindow.tif
#define	SPR_PARTYREQUEST_CLOSE1         	167779350   	// .\Data\UI\menu_1.tga
#define	SPR_PARTYREQUEST_CLOSE2         	167779351   	// .\Data\UI\menu_1.tga
#define	SPR_PARTYREQUEST_OK1            	167779352   	// .\Data\UI\menu_3.tga
#define	SPR_PARTYREQUEST_OK2            	167779353   	// .\Data\UI\menu_3.tga
#define	SPR_PARTYREQUEST_OK3            	167779354   	// .\Data\UI\menu_3.tga
#define	SPR_PARTYREQUEST_CANCEL1        	167779355   	// .\Data\UI\menu_3.tga
#define	SPR_PARTYREQUEST_CANCEL2        	167779356   	// .\Data\UI\menu_3.tga
#define	SPR_PARTYREQUEST_CANCEL3        	167779357   	// .\Data\UI\menu_3.tga
#define	SPR_OPTION_SYSTEMMSG            	167779358   	// .\Data\UI\checkv1.tif
#define	SPR_USERHP_BAR1                 	167779359   	// .\Data\UI\menu_4.tif
#define	SPR_USER_GAGE1                  	167779360   	// .\Data\UI\menu_1.tga
#define	SPR_USER_GAGE2                  	167779361   	// .\Data\UI\menu_4.tif
#define	SPR_USERHP_BAR2                 	167779362   	// .\Data\UI\menu_4.tif
#define	SPR_INITITEM_WND                	167779363   	// .\Data\UI\okwindow.tif
#define	SPR_INITITEM_CLOSE1             	167779364   	// .\Data\UI\menu_1.tga
#define	SPR_INITITEM_CLOSE2             	167779365   	// .\Data\UI\menu_1.tga
#define	SPR_INITITEM_OK1                	167779366   	// .\Data\UI\menu_3.tga
#define	SPR_INITITEM_OK2                	167779367   	// .\Data\UI\menu_3.tga
#define	SPR_INITITEM_OK3                	167779368   	// .\Data\UI\menu_3.tga
#define	SPR_INITITEM_CANCEL1            	167779369   	// .\Data\UI\menu_3.tga
#define	SPR_INITITEM_CANCEL2            	167779370   	// .\Data\UI\menu_3.tga
#define	SPR_INITITEM_CANCEL3            	167779371   	// .\Data\UI\menu_3.tga
#define	SPR_GUILDREQUEST_WND            	167779372   	// .\Data\UI\okwindow.tif
#define	SPR_GUILDREQUEST_CLOSE1         	167779373   	// .\Data\UI\menu_1.tga
#define	SPR_GUILDREQUEST_CLOSE2         	167779374   	// .\Data\UI\menu_1.tga
#define	SPR_GUILDREQUEST_OK1            	167779375   	// .\Data\UI\menu_3.tga
#define	SPR_GUILDREQUEST_OK2            	167779376   	// .\Data\UI\menu_3.tga
#define	SPR_GUILDREQUEST_OK3            	167779377   	// .\Data\UI\menu_3.tga
#define	SPR_GUILDREQUEST_CANCEL1        	167779378   	// .\Data\UI\menu_3.tga
#define	SPR_GUILDREQUEST_CANCEL2        	167779379   	// .\Data\UI\menu_3.tga
#define	SPR_GUILDREQUEST_CANCEL3        	167779380   	// .\Data\UI\menu_3.tga
#define	SPR_GUILD_LEVEL1                	167779381   	// .\Data\UI\menu_4.tga
#define	SPR_GUILD_LEVEL2                	167779382   	// .\Data\UI\menu_4.tga
#define	SPR_GUILD_LEVEL3                	167779383   	// .\Data\UI\menu_4.tga
#define	SPR_GUILD_LEVEL4                	167779384   	// .\Data\UI\menu_4.tga
#define	SPR_GUILD_MARK                  	167779385   	// .\Data\UI\menu_1.tga
#define	SPR_OPTION_GRAPHIC              	167779386   	// .\Data\UI\checkv1.tif
#define	SPR_MESSENGER_WND               	167779387   	// .\Data\UI\okwindow.tif
#define	SPR_MESSENGER_CLOSE1            	167779388   	// .\Data\UI\menu_1.tga
#define	SPR_MESSENGER_CLOSE2            	167779389   	// .\Data\UI\menu_1.tga
#define	SPR_MESSENGER_OK1               	167779390   	// .\Data\UI\menu_3.tga
#define	SPR_MESSENGER_OK2               	167779391   	// .\Data\UI\menu_3.tga
#define	SPR_MESSENGER_OK3               	167779392   	// .\Data\UI\menu_3.tga
#define	SPR_MESSENGER_CANCEL1           	167779393   	// .\Data\UI\menu_3.tga
#define	SPR_MESSENGER_CANCEL2           	167779394   	// .\Data\UI\menu_3.tga
#define	SPR_MESSENGER_CANCEL3           	167779395   	// .\Data\UI\menu_3.tga
#define	SPR_GUILDDELETE_WND             	167779396   	// .\Data\UI\okwindow.tif
#define	SPR_GUILDDELETE_CLOSE1          	167779397   	// .\Data\UI\menu_1.tga
#define	SPR_GUILDDELETE_CLOSE2          	167779398   	// .\Data\UI\menu_1.tga
#define	SPR_GUILDDELETE_OK1             	167779399   	// .\Data\UI\menu_3.tga
#define	SPR_GUILDDELETE_OK2             	167779400   	// .\Data\UI\menu_3.tga
#define	SPR_GUILDDELETE_OK3             	167779401   	// .\Data\UI\menu_3.tga
#define	SPR_GUILDDELETE_CANCEL1         	167779402   	// .\Data\UI\menu_3.tga
#define	SPR_GUILDDELETE_CANCEL2         	167779403   	// .\Data\UI\menu_3.tga
#define	SPR_GUILDDELETE_CANCEL3         	167779404   	// .\Data\UI\menu_3.tga
#define	SPR_MESSENGERDELETE_WND         	167779405   	// .\Data\UI\okwindow.tif
#define	SPR_MESSENGERDELETE_CLOSE1      	167779406   	// .\Data\UI\menu_1.tga
#define	SPR_MESSENGERDELETE_CLOSE2      	167779407   	// .\Data\UI\menu_1.tga
#define	SPR_MESSENGERDELETE_OK1         	167779408   	// .\Data\UI\menu_3.tga
#define	SPR_MESSENGERDELETE_OK2         	167779409   	// .\Data\UI\menu_3.tga
#define	SPR_MESSENGERDELETE_OK3         	167779410   	// .\Data\UI\menu_3.tga
#define	SPR_MESSENGERDELETE_CANCEL1     	167779411   	// .\Data\UI\menu_3.tga
#define	SPR_MESSENGERDELETE_CANCEL2     	167779412   	// .\Data\UI\menu_3.tga
#define	SPR_MESSENGERDELETE_CANCEL3     	167779413   	// .\Data\UI\menu_3.tga
#define	SPR_GROUPWND_STATUS             	167779414   	// .\Data\UI\menu_5.tga
#define	SPR_GROUP_MASTER1               	167779415   	// .\Data\UI\menu_5.tga
#define	SPR_GROUP_MASTER2               	167779416   	// .\Data\UI\menu_5.tga
#define	SPR_GROUP_MASTER3               	167779417   	// .\Data\UI\menu_5.tga
#define	SPR_GUILDMASTER_WND             	167779418   	// .\Data\UI\okwindow.tif
#define	SPR_GUILDMASTER_CLOSE1          	167779419   	// .\Data\UI\menu_1.tga
#define	SPR_GUILDMASTER_CLOSE2          	167779420   	// .\Data\UI\menu_1.tga
#define	SPR_GUILDMASTER_OK1             	167779421   	// .\Data\UI\menu_3.tga
#define	SPR_GUILDMASTER_OK2             	167779422   	// .\Data\UI\menu_3.tga
#define	SPR_GUILDMASTER_OK3             	167779423   	// .\Data\UI\menu_3.tga
#define	SPR_GUILDMASTER_CANCEL1         	167779424   	// .\Data\UI\menu_3.tga
#define	SPR_GUILDMASTER_CANCEL2         	167779425   	// .\Data\UI\menu_3.tga
#define	SPR_GUILDMASTER_CANCEL3         	167779426   	// .\Data\UI\menu_3.tga
#define	SPR_WORLD_WND1                  	167779427   	// .\Data\UI\df_window1.tif
#define	SPR_WORLD_WND2                  	167779428   	// .\Data\UI\df_window2.tif
#define	SPR_WORLD_WND3                  	167779429   	// .\Data\UI\df_window3.tif
#define	SPR_WORLD_CLOSE1                	167779430   	// .\Data\UI\menu_1.tga
#define	SPR_WORLD_CLOSE2                	167779431   	// .\Data\UI\menu_1.tga
#define	SPR_DUNGEONMANAGER_MOF1         	167779432   	// .\Data\UI\menu_3.tga
#define	SPR_DUNGEONMANAGER_MOF2         	167779433   	// .\Data\UI\menu_3.tga
#define	SPR_DUNGEONMANAGER_MOF3         	167779434   	// .\Data\UI\menu_3.tga
#define	SPR_CHARDIE_WND                 	167779435   	// .\Data\UI\okwindow2.tif
#define	SPR_CHARDIE_OK1                 	167779436   	// .\Data\UI\menu_3.tga
#define	SPR_CHARDIE_OK2                 	167779437   	// .\Data\UI\menu_3.tga
#define	SPR_CHARDIE_OK3                 	167779438   	// .\Data\UI\menu_3.tga
#define	SPR_GUILDPORTAL_WND             	167779439   	// .\Data\UI\okwindow.tif
#define	SPR_GUILDPORTAL_CLOSE1          	167779440   	// .\Data\UI\menu_1.tga
#define	SPR_GUILDPORTAL_CLOSE2          	167779441   	// .\Data\UI\menu_1.tga
#define	SPR_GUILDPORTAL_OK1             	167779442   	// .\Data\UI\menu_3.tga
#define	SPR_GUILDPORTAL_OK2             	167779443   	// .\Data\UI\menu_3.tga
#define	SPR_GUILDPORTAL_OK3             	167779444   	// .\Data\UI\menu_3.tga
#define	SPR_GUILDPORTAL_CANCEL1         	167779445   	// .\Data\UI\menu_3.tga
#define	SPR_GUILDPORTAL_CANCEL2         	167779446   	// .\Data\UI\menu_3.tga
#define	SPR_GUILDPORTAL_CANCEL3         	167779447   	// .\Data\UI\menu_3.tga
#define	SPR_GROUPWND_GUILDWAR1          	167779448   	// .\Data\UI\menu_4.tif
#define	SPR_GROUPWND_GUILDWAR2          	167779449   	// .\Data\UI\menu_4.tif
#define	SPR_GROUPWND_GUILDWARADD1       	167779450   	// .\Data\UI\menu_5.tga
#define	SPR_GROUPWND_GUILDWAREXIT1      	167779451   	// .\Data\UI\menu_5.tga
#define	SPR_GROUPWND_GUILDWARADD2       	167779452   	// .\Data\UI\menu_5.tga
#define	SPR_GROUPWND_GUILDWARADD3       	167779453   	// .\Data\UI\menu_5.tga
#define	SPR_GROUPWND_GUILDWAREXIT2      	167779454   	// .\Data\UI\menu_5.tga
#define	SPR_GROUPWND_GUILDWAREXIT3      	167779455   	// .\Data\UI\menu_5.tga
#define	SPR_GUILDWAR_WND                	167779456   	// .\Data\UI\okwindow.tif
#define	SPR_GUILDWAR_CLOSE1             	167779457   	// .\Data\UI\menu_1.tga
#define	SPR_GUILDWAR_CLOSE2             	167779458   	// .\Data\UI\menu_1.tga
#define	SPR_GUILDWAR_OK1                	167779459   	// .\Data\UI\menu_3.tga
#define	SPR_GUILDWAR_OK2                	167779460   	// .\Data\UI\menu_3.tga
#define	SPR_GUILDWAR_OK3                	167779461   	// .\Data\UI\menu_3.tga
#define	SPR_GUILDWAR_CANCEL1            	167779462   	// .\Data\UI\menu_3.tga
#define	SPR_GUILDWAR_CANCEL2            	167779463   	// .\Data\UI\menu_3.tga
#define	SPR_GUILDWAR_CANCEL3            	167779464   	// .\Data\UI\menu_3.tga
#define	SPR_EDIT_BKGR                   	167779465   	// .\Data\UI\speaking_box.tif
#define	SPR_GUILDWAR_GUILD              	167779466   	// .\Data\UI\menu_5.tga
#define	SPR_RANKNPC_WND1                	167779467   	// .\Data\UI\df_window1.tif
#define	SPR_RANKNPC_WND2                	167779468   	// .\Data\UI\df_window2.tif
#define	SPR_RANKNPC_WND3                	167779469   	// .\Data\UI\df_window3.tif
#define	SPR_RANKNPC_CLOSE1              	167779470   	// .\Data\UI\menu_1.tga
#define	SPR_RANKNPC_CLOSE2              	167779471   	// .\Data\UI\menu_1.tga
#define	SPR_RANKNPC_RANK1               	167779472   	// .\Data\UI\menu_5.tga
#define	SPR_RANKNPC_RANK2               	167779473   	// .\Data\UI\menu_5.tga
#define	SPR_RANKNPC_RANK3               	167779474   	// .\Data\UI\menu_5.tga
#define	SPR_RANK_WND1                   	167779475   	// .\Data\UI\df_window1.tif
#define	SPR_RANK_WND2                   	167779476   	// .\Data\UI\df_window2.tif
#define	SPR_RANK_WND3                   	167779477   	// .\Data\UI\df_window3.tif
#define	SPR_RANK_CLOSE1                 	167779478   	// .\Data\UI\menu_1.tga
#define	SPR_RANK_CLOSE2                 	167779479   	// .\Data\UI\menu_2.tga
#define	BGM_TITLE                       	184549377   	// .\Data\Sound\Main_Intro.mp3
#define	BGM_WORLDMAP                    	184549378   	// .\Data\Sound\Map_World1.mp3
#define	BGM_PLANE                       	184549379   	// .\Data\Sound\Map_World1_Fly.mp3
#define	BGM_VILLAGE                     	184549380   	// .\Data\Sound\Town_World1.mp3
#define	BGM_DUNGEON_ENTRANCE            	184549381   	// .\Data\Sound\Dungeon_Hold1.mp3
#define	BGM_DUNGEON_LEVEL1              	184549382   	// .\Data\Sound\Dungeon_Hold1.mp3
#define	BGM_DUNGEON_LEVEL2              	184549383   	// .\Data\Sound\Dungeon_Hold3.mp3
#define	BGM_DUNGEON_LEVEL3              	184549384   	// .\Data\Sound\Dungeon_Hold3.mp3
#define	MOD_CHAR_SELECT_GROUND          	201326593   	// .\Data\UI\chrs001.chr
#define	MOD_WORLD_WATER                 	201326595   	// .\Data\Map\wwater001.chr
#define	MOD_WORLD_PLANE                 	201326596   	// .\Data\Object\wair001.chr
#define	MOD_CHAR_SELECT_LIGHT           	201326597   	// .\Data\UI\chrs002.chr
#define	MOD_CHAR_SELECT_TORCHLIGHT      	201326598   	// .\Data\UI\chrs003.chr
#define	CHR_MAIN_TITLE                  	201326599   	// .\Data\UI\corum_title.chr
#define	MOD_SELF_PORTAL_EFFECT          	201326600   	// .\Data\Effect\e0118000.chr
#define	CHR_TRAP_DUNGEON_MODEL          	201326602   	// .\Data\Effect\e0127000.chr
#define	CHR_TRAP_DUNGEON_EFFECT         	201326603   	// .\Data\Effect\e0126000.chr
#define	CHR_CHARACTER_SELECT            	201326604   	// .\Data\UI\chr_select.MOD
#define	MOD_ITEM_BOTARI                 	201328592   	// .\Data\Item\itembotary.chr
#define	MOD_ITEM_MONEY                  	201328593   	// .\Data\Item\money_item.chr
#define	MOD_CHARACTER_SHADOW            	201333093   	// .\Data\Character\df_shadow02.MOD
#define	MOD_PLAYERCLICK                 	201333094   	// .\Data\UI\click_a.chr
#define	MOD_MONSTER_SHADOW1             	201333095   	// .\Data\Character\df_shadow01.MOD
#define	MOD_MONSTER_SHADOW2             	201333096   	// .\Data\Character\df_shadow02.MOD
#define	MOD_MONSTER_SHADOW3             	201333097   	// .\Data\Character\df_shadow03.MOD
#define	MOD_MONSTER_SHADOW4             	201333098   	// .\Data\Character\df_shadow04.MOD
#define	CHR_EVENT_DUNGEON_MODEL_TYPE_1  	201333099   	// .\Data\Map\e0127000.CHR
#define	CHR_EVENT_DUNGEON_MODEL_TYPE_2  	201333100   	// .\Data\Map\e0127001.CHR
#define	CHR_EVENT_DUNGEON_MODEL_TYPE_3  	201333101   	// .\Data\Map\e0127002.CHR
#define	CHR_EVENT_DUNGEON_MODEL_TYPE_4  	201333102   	// .\Data\Map\e0127003.CHR
#define	CHR_EVENT_DUNGEON_MODEL_TYPE_5  	201333103   	// .\Data\Map\e0127004.CHR
#define	CHR_EVENT_DUNGEON_MODEL_TYPE_6  	201333104   	// .\Data\Map\e0127005.CHR
#define	MOD_ITEM_POTION                 	201333105   	// .\Data\Item\itempotion_1.chr
#define	MOD_ITEM_MAGICBOTARI            	201333106   	// .\Data\Item\itembotary_magic.chr
#define	MOD_ITEM_SETBOTARI              	201333107   	// .\Data\Item\itembox_set.chr
#define	MOD_ITEM_EXTERAMBOTARI          	201333108   	// .\Data\Item\itembox_unique.chr
#define	MOD_ITEM_MONEY_BOXA             	201333109   	// .\Data\Item\moneybox_a.chr
#define	MOD_ITEM_MONEY_BOXB             	201333110   	// .\Data\Item\moneybox_b.chr
#define	MOD_ITEM_MONEY_BOXC             	201333111   	// .\Data\Item\moneybox_c.chr
#define	MOD_ITEM_EVENT                  	201333112   	// .\Data\Item\event_item1.chr
#define	BUTTON_OK                       	218103809   	// .\Data\UI\menu_3.tga
#define	BUTTON_CANCEL                   	218103810   	// .\Data\UI\menu_3.tga
#define	BUTTON_EXIT                     	218103811   	// .\Data\UI\menu_3.tga
#define	BUTTON_CREATE                   	218103812   	// .\Data\UI\menu_3.tga
#define	BUTTON_DELETE                   	218103813   	// .\Data\UI\menu_3.tga
#define	BUTTON_MODIFY                   	218103814   	// .\Data\UI\menu_3.tga
#define	BUTTON_START                    	218103815   	// .\Data\Button\wdb0007a.tga
#define	BUTTON_LEFT                     	218103816   	// .\Data\Button\wdb0008a.tga
#define	BUTTON_RIGHT                    	218103817   	// .\Data\Button\wdb0009a.tga
#define	BUTTON_CLASS_WARRIOR            	218103818   	// .\Data\Button\wdj0001a.tga
#define	BUTTON_CLASS_PRIEST             	218103819   	// .\Data\Button\wdj0002a.tga
#define	BUTTON_CLASS_SUMMONER           	218103820   	// .\Data\Button\wdj0003a.tga
#define	BUTTON_CLASS_HUNTER             	218103821   	// .\Data\Button\wdj0004a.tga
#define	BUTTON_CLASS_WIZARD             	218103822   	// .\Data\Button\wdj0005a.tga
#define	BUTTON_ITEM1                    	218110487   	// .\Data\UI\menu_1.tga
#define	BUTTON_CHR1                     	218110488   	// .\Data\UI\menu_1.tga
#define	BUTTON_SKILL1                   	218110489   	// .\Data\UI\menu_1.tga
#define	BUTTON_GUILD1                   	218110490   	// .\Data\UI\menu_1.tga
#define	BUTTON_PARTY1                   	218110491   	// .\Data\UI\menu_1.tga
#define	BUTTON_CHAT1                    	218110492   	// .\Data\UI\menu_1.tga
#define	BUTTON_CHR2                     	218110493   	// .\Data\UI\menu_1.tga
#define	BUTTON_SKILL2                   	218110494   	// .\Data\UI\menu_1.tga
#define	BUTTON_ITEM2                    	218110495   	// .\Data\UI\menu_1.tga
#define	BUTTON_GUILD2                   	218110496   	// .\Data\UI\menu_1.tga
#define	BUTTON_PARTY2                   	218110497   	// .\Data\UI\menu_1.tga
#define	BUTTON_CHAT2                    	218110498   	// .\Data\UI\menu_1.tga
#define	BUTTON_SYSTEM1                  	218110499   	// .\Data\UI\menu_1.tga
#define	BUTTON_SYSTEM2                  	218110500   	// .\Data\UI\menu_1.tga
#define	BUTTON_WDCHAT1                  	218110501   	// .\Data\UI\menu_1.tga
#define	BUTTON_WDCHAT2                  	218110502   	// .\Data\UI\menu_1.tga
#define	BUTTON_WDCHAT3                  	218110503   	// .\Data\UI\menu_1.tga
#define	BUTTON_AURA                     	218110504   	// .\Data\UI\menu_1.tga
#define	BUTTON_MANA                     	218110505   	// .\Data\UI\menu_1.tga
#define	BUTTON_DIVINE                   	218110506   	// .\Data\UI\menu_1.tga
#define	BUTTON_SUMMON                   	218110507   	// .\Data\UI\menu_1.tga
#define	BUTTON_CHAKRA                   	218110508   	// .\Data\UI\menu_1.tga
#define	BUTTON_ATTACK2                  	218110713   	// .\Data\UI\skill_icon1.tga
#define	BUTTON_ATTACK3                  	218110714   	// .\Data\UI\skill_icon1.tga
#define	BUTTON_MAKE1                    	218110715   	// .\Data\UI\menu_3.tga
#define	BUTTON_MAKE2                    	218110716   	// .\Data\UI\menu_3.tga
#define	BUTTON_MAKE3                    	218110717   	// .\Data\UI\menu_3.tga
#define	BUTTON_STR1                     	218110718   	// .\Data\UI\menu_3.tga
#define	BUTTON_STR2                     	218110719   	// .\Data\UI\menu_3.tga
#define	BUTTON_STR3                     	218110720   	// .\Data\UI\menu_3.tga
#define	BUTTON_TRADE1                   	218110721   	// .\Data\UI\menu_3.tga
#define	BUTTON_TRADE2                   	218110722   	// .\Data\UI\menu_3.tga
#define	BUTTON_TRADE3                   	218110723   	// .\Data\UI\menu_3.tga
#define	BUTTON_EGO_UP1                  	218110724   	// .\Data\UI\menu_1.tga
#define	BUTTON_EGO_UP2                  	218110725   	// .\Data\UI\menu_1.tga
#define	BUTTON_STR_UP1                  	218110726   	// .\Data\UI\menu_1.tga
#define	BUTTON_STR_UP2                  	218110727   	// .\Data\UI\menu_1.tga
#define	BUTTON_INT_UP1                  	218110728   	// .\Data\UI\menu_1.tga
#define	BUTTON_INT_UP2                  	218110729   	// .\Data\UI\menu_1.tga
#define	BUTTON_DEX_UP1                  	218110730   	// .\Data\UI\menu_1.tga
#define	BUTTON_DEX_UP2                  	218110731   	// .\Data\UI\menu_1.tga
#define	BUTTON_VIT_UP1                  	218110732   	// .\Data\UI\menu_1.tga
#define	BUTTON_VIT_UP2                  	218110733   	// .\Data\UI\menu_1.tga
#define	BUTTON_PKON1                    	218110734   	// .\Data\UI\menu_1.tga
#define	BUTTON_PKON2                    	218110735   	// .\Data\UI\menu_1.tga
#define	BUTTON_PKOFF1                   	218110736   	// .\Data\UI\menu_1.tga
#define	BUTTON_PKOFF2                   	218110737   	// .\Data\UI\menu_1.tga
#define	BUTTON_INFO1                    	218110738   	// .\Data\UI\menu_3.tga
#define	BUTTON_INFO2                    	218110739   	// .\Data\UI\menu_3.tga
#define	BUTTON_INFO3                    	218110740   	// .\Data\UI\menu_3.tga
#define	BUTTON_EXIT1                    	218110741   	// .\Data\UI\menu_3.tga
#define	BUTTON_EXIT2                    	218110742   	// .\Data\UI\menu_3.tga
#define	BUTTON_EXIT3                    	218110743   	// .\Data\UI\menu_3.tga
#define	BUTTON_GAME_EXIT1               	218110744   	// .\Data\UI\menu_2.tga
#define	BUTTON_GAME_EXIT2               	218110745   	// .\Data\UI\menu_2.tga
#define	BUTTON_GAME_EXIT3               	218110746   	// .\Data\UI\menu_2.tga
#define	BUTTON_CHAR_SELECT1             	218110747   	// .\Data\UI\menu_2.tga
#define	BUTTON_CHAR_SELECT2             	218110748   	// .\Data\UI\menu_2.tga
#define	BUTTON_CHAR_SELECT3             	218110749   	// .\Data\UI\menu_2.tga
#define	BUTTON_GAME_RETURN1             	218110750   	// .\Data\UI\menu_2.tga
#define	BUTTON_GAME_RETURN2             	218110751   	// .\Data\UI\menu_2.tga
#define	BUTTON_GAME_RETURN3             	218110752   	// .\Data\UI\menu_2.tga
#define	BUTTON_DUNGEONMANAGERMENTMODE_DEFENSE1	218110756   	// .\Data\UI\menu_3.tga
#define	BUTTON_DUNGEONMANAGERMENTMODE_DEFENSE3	218110757   	// .\Data\UI\menu_3.tga
#define	BUTTON_DUNGEONMANAGERMENTMODE_LEVELUP1	218110758   	// .\Data\UI\menu_3.tga
#define	BUTTON_DUNGEONMANAGERMENTMODE_LEVELUP3	218110759   	// .\Data\UI\menu_3.tga
#define	BUTTON_DUNGEONMANAGERMENTMODE_PRODUCTION1	218110760   	// .\Data\UI\menu_3.tga
#define	BUTTON_DUNGEONMANAGERMENTMODE_PRODUCTION3	218110761   	// .\Data\UI\menu_3.tga
#define	BUTTON_DUNGEONMANAGEMENT1       	218110764   	// .\Data\UI\menu_3.tga
#define	BUTTON_DUNGEONMANAGEMENT2       	218110765   	// .\Data\UI\menu_3.tga
#define	BUTTON_DUNGEONMANAGEMENT3       	218110766   	// .\Data\UI\menu_3.tga
#define	BUTTON_ACQUISITION1_1           	218110767   	// .\Data\UI\menu_3.tga
#define	BUTTON_ACQUISITION1_2           	218110768   	// .\Data\UI\menu_3.tga
#define	BUTTON_ACQUISITION1_3           	218110769   	// .\Data\UI\menu_3.tga
#define	BUTTON_ACQUISITION2_1           	218110770   	// .\Data\UI\menu_3.tga
#define	BUTTON_ACQUISITION2_2           	218110771   	// .\Data\UI\menu_3.tga
#define	BUTTON_ACQUISITION2_3           	218110772   	// .\Data\UI\menu_3.tga
#define	BUTTON_DUNGEONINFOCOMBOBOX1_1   	218110773   	// .\Data\UI\menu_1.tga
#define	BUTTON_DUNGEONINFOCOMBOBOX1_3   	218110774   	// .\Data\UI\menu_1.tga
#define	BUTTON_DUNGEONINFOCOMBOBOX2_1   	218110775   	// .\Data\UI\menu_1.tga
#define	BUTTON_DUNGEONINFOCOMBOBOX2_3   	218110776   	// .\Data\UI\menu_1.tga
#define	BUTTON_DUNGEONINFOCOMBOBOX3_1   	218110777   	// .\Data\UI\menu_1.tga
#define	BUTTON_DUNGEONINFOCOMBOBOX3_3   	218110778   	// .\Data\UI\menu_1.tga
#define	BUTTON_DUNGEONINFOCOMBOBOX4_1   	218110779   	// .\Data\UI\menu_1.tga
#define	BUTTON_DUNGEONINFOCOMBOBOX4_3   	218110780   	// .\Data\UI\menu_1.tga
#define	BUTTON_DUNGEONINFOCOMBOBOX5_1   	218110781   	// .\Data\UI\menu_1.tga
#define	BUTTON_DUNGEONINFOCOMBOBOX5_3   	218110782   	// .\Data\UI\menu_1.tga
#define	BUTTON_DUNGEONINFOCOMBOBOX6_1   	218110783   	// .\Data\UI\menu_1.tga
#define	BUTTON_DUNGEONINFOCOMBOBOX6_3   	218110784   	// .\Data\UI\menu_1.tga
#define	BUTTON_DUNGEONINFOCOMBOBOX7_1   	218110785   	// .\Data\UI\menu_1.tga
#define	BUTTON_DUNGEONINFOCOMBOBOX7_3   	218110786   	// .\Data\UI\menu_1.tga
#define	BUTTON_DUNGEONINFOCOMBOBOX8_1   	218110787   	// .\Data\UI\menu_1.tga
#define	BUTTON_DUNGEONINFOCOMBOBOX8_3   	218110788   	// .\Data\UI\menu_1.tga
#define	BUTTON_DUNGEONINFO_MANAGEMENT1_1	218110791   	// .\Data\UI\menu_3.tga
#define	BUTTON_DUNGEONINFO_MANAGEMENT1_2	218110792   	// .\Data\UI\menu_3.tga
#define	BUTTON_DUNGEONINFO_MANAGEMENT1_3	218110793   	// .\Data\UI\menu_3.tga
#define	BUTTON_DUNGEONINFO_MANAGEMENT2_1	218110794   	// .\Data\UI\menu_3.tga
#define	BUTTON_DUNGEONINFO_MANAGEMENT2_2	218110795   	// .\Data\UI\menu_3.tga
#define	BUTTON_DUNGEONINFO_MANAGEMENT2_3	218110796   	// .\Data\UI\menu_3.tga
#define	BUTTON_DUNGEONINFO_MANAGEMENT3_1	218110797   	// .\Data\UI\menu_3.tga
#define	BUTTON_DUNGEONINFO_MANAGEMENT3_2	218110798   	// .\Data\UI\menu_3.tga
#define	BUTTON_DUNGEONINFO_MANAGEMENT3_3	218110799   	// .\Data\UI\menu_3.tga
#define	BUTTON_DUNGEONINFO_MANAGEMENT4_1	218110800   	// .\Data\UI\menu_3.tga
#define	BUTTON_DUNGEONINFO_MANAGEMENT4_2	218110801   	// .\Data\UI\menu_3.tga
#define	BUTTON_DUNGEONINFO_MANAGEMENT4_3	218110802   	// .\Data\UI\menu_3.tga
#define	BUTTON_DUNGEONINFO_MANAGEMENT5_1	218110803   	// .\Data\UI\menu_3.tga
#define	BUTTON_DUNGEONINFO_MANAGEMENT5_2	218110804   	// .\Data\UI\menu_3.tga
#define	BUTTON_DUNGEONINFO_MANAGEMENT5_3	218110805   	// .\Data\UI\menu_3.tga
#define	BUTTON_DUNGEONINFO_MANAGEMENT6_1	218110806   	// .\Data\UI\menu_3.tga
#define	BUTTON_DUNGEONINFO_MANAGEMENT6_2	218110807   	// .\Data\UI\menu_3.tga
#define	BUTTON_DUNGEONINFO_MANAGEMENT6_3	218110808   	// .\Data\UI\menu_3.tga
#define	BUTTON_DUNGEONINFO_MANAGEMENT7_1	218110809   	// .\Data\UI\menu_3.tga
#define	BUTTON_DUNGEONINFO_MANAGEMENT7_2	218110810   	// .\Data\UI\menu_3.tga
#define	BUTTON_DUNGEONINFO_MANAGEMENT7_3	218110811   	// .\Data\UI\menu_3.tga
#define	BUTTON_MANAGERMENT1             	218110812   	// .\Data\UI\menu_1.tga
#define	BUTTON_MANAGERMENT3             	218110813   	// .\Data\UI\menu_1.tga
#define	BUTTON_DUNGEONINFO_MANAGEMENT_OK1	218110814   	// .\Data\UI\menu_3.tga
#define	BUTTON_DUNGEONINFO_MANAGEMENT_OK2	218110815   	// .\Data\UI\menu_3.tga
#define	BUTTON_DUNGEONINFO_MANAGEMENT_OK3	218110816   	// .\Data\UI\menu_3.tga
#define	BUTTON_DUNGEONINFO_MANAGEMENT_CANCEL1	218110817   	// .\Data\UI\menu_3.tga
#define	BUTTON_DUNGEONINFO_MANAGEMENT_CANCEL2	218110818   	// .\Data\UI\menu_3.tga
#define	BUTTON_DUNGEONINFO_MANAGEMENT_CANCEL3	218110819   	// .\Data\UI\menu_3.tga
#define	BUTTON_DUNGEONINFO_SCROLL       	218110827   	// .\Data\UI\menu_1.tga
#define	BUTTON_DUNGONEINFO_MANAGEMENT_PULLDOWN_SCROLL	218110828   	// .\Data\UI\menu_1.tga
#define	BUTTON_PLAYERSHOP_OPEN1         	218110829   	// .\Data\UI\menu_4.tga
#define	BUTTON_PLAYERSHOP_OPEN2         	218110830   	// .\Data\UI\menu_4.tga
#define	BUTTON_PLAYERSHOP_OPEN3         	218110831   	// .\Data\UI\menu_4.tga
#define	BUTTON_PLAYERSHOP_CLOSE1        	218110832   	// .\Data\UI\menu_4.tga
#define	BUTTON_PLAYERSHOP_CLOSE2        	218110833   	// .\Data\UI\menu_4.tga
#define	BUTTON_PLAYERSHOP_CLOSE3        	218110834   	// .\Data\UI\menu_4.tga
#define	BUTTON_PLAYERSHOPWND_CANCEL1    	218110835   	// .\Data\UI\menu_4.tga
#define	BUTTON_PLAYERSHOPWND_CANCEL2    	218110836   	// .\Data\UI\menu_4.tga
#define	BUTTON_PLAYERSHOPWND_CANCEL3    	218110837   	// .\Data\UI\menu_4.tga
#define	BUTTON_PLAYERSHOP_EXIT1         	218110838   	// .\Data\UI\menu_1.tga
#define	BUTTON_PLAYERSHOP_EXIT2         	218110839   	// .\Data\UI\menu_1.tga
#define	BUTTON_PLAYERSHOP_EXIT3         	218110840   	// .\Data\UI\menu_1.tga
#define	BUTTON_DUNGEONINFOEX_STATUS1    	218110841   	// .\Data\UI\menu_5.tga
#define	BUTTON_DUNGEONINFOEX_STATUS2    	218110842   	// .\Data\UI\menu_5.tga
#define	BUTTON_DUNGEONINFOEX_STATUS3    	218110843   	// .\Data\UI\menu_5.tga
#define	BUTTON_DUNGEONINFOEX_INFORMATION1	218110844   	// .\Data\UI\menu_5.tga
#define	BUTTON_DUNGEONINFOEX_INFORMATION2	218110845   	// .\Data\UI\menu_5.tga
#define	BUTTON_DUNGEONINFOEX_INFORMATION3	218110846   	// .\Data\UI\menu_5.tga
#define	BUTTON_DUNGEONINFOEX_PRODUCTION1	218110847   	// .\Data\UI\menu_5.tga
#define	BUTTON_DUNGEONINFOEX_PRODUCTION2	218110848   	// .\Data\UI\menu_5.tga
#define	BUTTON_DUNGEONINFOEX_PRODUCTION3	218110849   	// .\Data\UI\menu_5.tga
#define	BUTTON_DUNGEONINFOEX_MANAGEMENT1	218110850   	// .\Data\UI\menu_3.tga
#define	BUTTON_DUNGEONINFOEX_MANAGEMENT2	218110851   	// .\Data\UI\menu_3.tga
#define	BUTTON_DUNGEONINFOEX_MANAGEMENT3	218110852   	// .\Data\UI\menu_3.tga
#define	BUTTON_DUNGEONINFOEX_PAGENEXT1  	218110853   	// .\Data\UI\menu_1.tga
#define	BUTTON_DUNGEONINFOEX_PAGENEXT2  	218110854   	// .\Data\UI\menu_1.tga
#define	BUTTON_DUNGEONINFOEX_PAGENEXT3  	218110855   	// .\Data\UI\menu_1.tga
#define	BUTTON_DUNGEONINFOEX_PAGEPRE1   	218110856   	// .\Data\UI\menu_1.tga
#define	BUTTON_DUNGEONINFOEX_PAGEPRE2   	218110857   	// .\Data\UI\menu_1.tga
#define	BUTTON_DUNGEONINFOEX_PAGEPRE3   	218110858   	// .\Data\UI\menu_1.tga
#define	BUTTON_ITEMUPGRADE_CLOSE1       	218110859   	// .\Data\UI\menu_1.tga
#define	BUTTON_ITEMUPGRADE_CLOSE2       	218110860   	// .\Data\UI\menu_1.tga
#define	BUTTON_ITEMUPGRADE1             	218110861   	// .\Data\UI\menu_3.tga
#define	BUTTON_ITEMUPGRADE2             	218110862   	// .\Data\UI\menu_3.tga
#define	BUTTON_ITEMUPGRADE3             	218110863   	// .\Data\UI\menu_3.tga
#define	BUTTON_ACQUISITION3_1           	218110864   	// .\Data\UI\menu_3.tga
#define	BUTTON_ACQUISITION3_2           	218110865   	// .\Data\UI\menu_3.tga
#define	BUTTON_ACQUISITION3_3           	218110866   	// .\Data\UI\menu_3.tga
#define	BUTTON_OK_OVER                  	234881025   	// .\Data\UI\menu_3.tga
#define	BUTTON_CANCEL_OVER              	234881026   	// .\Data\UI\menu_3.tga
#define	BUTTON_EXIT_OVER                	234881027   	// .\Data\UI\menu_3.tga
#define	BUTTON_CREATE_OVER              	234881028   	// .\Data\UI\menu_3.tga
#define	BUTTON_DELETE_OVER              	234881029   	// .\Data\UI\menu_3.tga
#define	BUTTON_MODIFY_OVER              	234881030   	// .\Data\UI\menu_3.tga
#define	BUTTON_START_OVER               	234881031   	// .\Data\Button\wdb0007b.tga
#define	BUTTON_LEFT_OVER                	234881032   	// .\Data\Button\wdb0008b.tga
#define	BUTTON_RIGHT_OVER               	234881033   	// .\Data\Button\wdb0009b.tga
#define	BUTTON_OK_DOWN                  	251658241   	// .\Data\UI\menu_3.tga
#define	BUTTON_CANCEL_DOWN              	251658242   	// .\Data\UI\menu_3.tga
#define	BUTTON_EXIT_DOWN                	251658243   	// .\Data\UI\menu_3.tga
#define	BUTTON_CREATE_DOWN              	251658244   	// .\Data\UI\menu_3.tga
#define	BUTTON_DELETE_DOWN              	251658245   	// .\Data\UI\menu_3.tga
#define	BUTTON_MODIFY_DOWN              	251658246   	// .\Data\UI\menu_3.tga
#define	BUTTON_START_DOWN               	251658247   	// .\Data\Button\wdb0007c.tga
#define	BUTTON_LEFT_DOWN                	251658248   	// .\Data\Button\wdb0008c.tga
#define	BUTTON_RIGHT_DOWN               	251658249   	// .\Data\Button\wdb0009c.tga
#define	BUTTON_CLASS_WARRIOR_DOWN       	251658250   	// .\Data\Button\wdj0001b.tga
#define	BUTTON_CLASS_PRIEST_DOWN        	251658251   	// .\Data\Button\wdj0002b.tga
#define	BUTTON_CLASS_SUMMONER_DOWN      	251658252   	// .\Data\Button\wdj0003b.tga
#define	BUTTON_CLASS_HUNTER_DOWN        	251658253   	// .\Data\Button\wdj0004b.tga
#define	BUTTON_CLASS_WIZARD_DOWN        	251658254   	// .\Data\Button\wdj0005b.tga
#define	DUNGEON_MAGICARRAY              	788529153   	// .\Data\UI\item_icon01.tga
#define	SKILL_FIREMISSILE1              	1677724600  	// .\Data\Effect\e0401102.chr
#define	SKILL_FIREMISSILE2              	1677724601  	// .\Data\Effect\e0401103.chr
#define	SKILL_FIREMISSILE3              	1677724602  	// .\Data\Effect\e0401104.chr
#define	SKILL_FIREMISSILE4              	1677724603  	// .\Data\Effect\e0401105.chr
#define	SKILL_FIREMISSILE5              	1677724604  	// .\Data\Effect\e0401106.chr
#define	SKILL_FIREMISSILE6              	1677724605  	// .\Data\Effect\e0401107.chr
#define	SKILL_FIREMISSILE7              	1677724606  	// .\Data\Effect\e0401108.chr
#define	SKILL_ICEMISSILE1               	1677724607  	// .\Data\Effect\e0401202.chr
#define	SKILL_ICEMISSILE2               	1677724608  	// .\Data\Effect\e0401203.chr
#define	SKILL_ICEMISSILE3               	1677724609  	// .\Data\Effect\e0401204.chr
#define	SKILL_ICEMISSILE4               	1677724610  	// .\Data\Effect\e0401205.chr
#define	SKILL_ICEMISSILE5               	1677724611  	// .\Data\Effect\e0401206.chr
#define	SKILL_ICEMISSILE6               	1677724612  	// .\Data\Effect\e0401207.chr
#define	SKILL_ICEMISSILE7               	1677724613  	// .\Data\Effect\e0401208.chr
#define	SKILL_LANDINGSUPPORT1           	1677724614  	// .\Data\Effect\e0401301.chr
#define	SKILL_LIGHTING1                 	1677724615  	// .\Data\Effect\e0401401.chr
#define	SKILL_LIGHTING2                 	1677724616  	// .\Data\Effect\e0401402.chr
#define	SKILL_MANAEXPAND                	1677724617  	// .\Data\Effect\e0401501.chr
#define	SKILL_SPEEDCASTING              	1677724618  	// .\Data\Effect\e0402501.chr
#define	SKILL_EXPLOSION                 	1677724619  	// .\Data\Effect\e0403501.chr
#define	SKILL_FIREBLAZE                 	1677724620  	// .\Data\Effect\e0402101.chr
#define	SKILL_ICEWAVE                   	1677724621  	// .\Data\Effect\e0402201.chr
#define	SKILL_EARTHQUAKE                	1677724622  	// .\Data\Effect\e0402301.chr
#define	SKILL_AIREALCOAT                	1677724623  	// .\Data\Effect\e0402401.chr
#define	SKILL_MAGMAWALL1                	1677724624  	// .\Data\Effect\e0403101.chr
#define	SKILL_MAGMAWALL2                	1677724625  	// .\Data\Effect\e0403102.chr
#define	SKILL_MAGMAWALL3                	1677724626  	// .\Data\Effect\e0403103.chr
#define	SKILL_ICEWALL1                  	1677724627  	// .\Data\Effect\e0403201.chr
#define	SKILL_ICEWALL2                  	1677724628  	// .\Data\Effect\e0403202.chr
#define	SKILL_ICEWALL3                  	1677724629  	// .\Data\Effect\e0403203.chr
#define	SKILL_METEOR                    	1677724630  	// .\Data\Effect\e0403301.chr
#define	SKILL_THUNDERSTORM              	1677724631  	// .\Data\Effect\e0403401.chr
#define	SKILL_LANDINGSUPPORT2           	1677724632  	// .\Data\Effect\e0401302.chr
#define	SKILL_DRAGONICVIBRATION         	1677724633  	// .\Data\Effect\e100020.chr
#define	SKILL_DRAGONICFIREBLAST         	1677724634  	// .\Data\Effect\e100010.chr
#define	SKILL_FLASHSHOCK1               	1677724635  	// .\Data\Effect\e0404401.chr
#define	SKILL_FLASHSHOCK2               	1677724636  	// .\Data\Effect\e0404402.chr
#define	SKILL_FLASHSHOCK3               	1677724637  	// .\Data\Effect\e0404403.chr
#define	SKILL_FLASHSHOCK4               	1677724638  	// .\Data\Effect\e0404404.chr
#define	SKILL_FLASHSHOCK5               	1677724639  	// .\Data\Effect\e0404405.chr
#define	SKILL_FLASHSHOCK6               	1677724640  	// .\Data\Effect\e0404406.chr
#define	SKILL_FLASHSHOCK7               	1677724641  	// .\Data\Effect\e0404407.chr
#define	SKILL_AURARECHARGE              	1694501816  	// .\Data\Effect\e0501000.chr
#define	SKILL_ZEAL                      	1694501817  	// .\Data\Effect\e0502000.chr
#define	SKILL_BASH                      	1694501818  	// .\Data\Effect\e0503000.chr
#define	SKILL_DASH                      	1694501819  	// .
#define	SKILL_RISINGSWORD               	1694501820  	// .\Data\Effect\e0505000.chr
#define	SKILL_BLASTQUAKE                	1694501821  	// .\Data\Effect\e0506000.chr
#define	SKILL_LIGHTNINGBREAK            	1694501822  	// .\Data\Effect\e0507000.chr
#define	SKILL_SILENTBRANDISH            	1694501823  	// .\Data\Effect\e0508000.chr
#define	SKILL_LIFEEXPLOSION             	1694501824  	// .\Data\Effect\e0509000.chr
#define	SKILL_ANTAGONIZE                	1694501825  	// .
#define	SKILL_AURABATTLER               	1694501826  	// .\Data\Effect\e0511000.chr
#define	SKILL_LIFEFORCE                 	1694501827  	// .\Data\Effect\e0512000.chr
#define	SKILL_AURAMASTERY               	1694501828  	// .
#define	SKILL_REGENERATION              	1694501829  	// .
#define	SKILL_WEAPONMASTERY             	1694501830  	// .
#define	SKILL_DEFENCEMASTERY            	1694501831  	// .
#define	SKILL_HEAL                      	1711279032  	// .\Data\Effect\e0601000.chr
#define	SKILL_PRAY                      	1711279033  	// .\Data\Effect\e0602000.chr
#define	SKILL_BLESSING                  	1711279034  	// .\Data\Effect\e0603000.chr
#define	SKILL_WINDTAILS                 	1711279035  	// .\Data\Effect\e0604000.chr
#define	SKILL_HOLYSHIELD                	1711279036  	// .\Data\Effect\e0607000.chr
#define	SKILL_WINDFORCE                 	1711279037  	// .\Data\Effect\e0605000.chr
#define	SKILL_TRIAGION                  	1711279038  	// .\Data\Effect\e0606000.chr
#define	SKILL_DRIVEAMPLIFIER            	1711279039  	// .\Data\Effect\e0608000.chr
#define	SKILL_HOLYPRESSURE              	1711279040  	// .\Data\Effect\e0609000.chr
#define	SKILL_ABSOLUTEMAGICBARRIER      	1711279041  	// .\Data\Effect\e0601000.chr
#define	SKILL_SPELLBINDING              	1711279042  	// .\Data\Effect\e0611000.chr
#define	SKILL_DIVINEMASTERY             	1711279043  	// .
#define	SKILL_DETECT                    	1711279044  	// .
#define	SKILL_AROUSE                    	1711279045  	// .
#define	SKILL_HOLYDEVINE                	1711279046  	// .
#define	SKILL_ORDSLUM                   	1711279047  	// .\Data\Effect\e0609000.chr
#define	SKILL_SUITSLUM                  	1711279048  	// .\Data\Effect\e0609000.chr
#define	SKILL_DETECTION                 	1711279049  	// .\Data\Effect\e0613000.chr
#define	SKILL_REMEDY                    	1711279050  	// .\Data\Effect\e0614000.chr
#define	SKILL_ROUNDRANGE1               	1711279051  	// .\Data\Effect\e0616000.chr
#define	SKILL_ROUNDRANGE2               	1711279052  	// .\Data\Effect\e0616001.chr
#define	SKILL_ROUNDRANGE3               	1711279053  	// .\Data\Effect\e0616002.chr
#define	SKILL_ROUNDRANGE4               	1711279054  	// .\Data\Effect\e0616003.chr
#define	SKILL_ROUNDRANGE5               	1711279055  	// .\Data\Effect\e0616004.chr
#define	SKILL_ROUNDRANGE6               	1711279056  	// .\Data\Effect\e0616005.chr
#define	SKILL_ROUNDRANGE7               	1711279057  	// .\Data\Effect\e0616006.chr
#define	SKILL_ORDRISING                 	1711279058  	// .\Data\Effect\e0601000.chr
#define	SKILL_SUITRISING                	1711279059  	// .\Data\Effect\e0610000.chr
#define	SKILL_CALMDOWN                  	1728056248  	// .
#define	SKILL_SLEEP                     	1728056249  	// .\Data\Effect\e0702000.chr
#define	SKILL_CUNFUSE                   	1728056250  	// .\Data\Effect\e0703000.chr
#define	SKILL_MINDCONTROL               	1728056251  	// .\Data\Effect\e0704000.chr
#define	SKILL_RAGE                      	1728056252  	// .\Data\Effect\e0706000.chr
#define	SKILL_SUMMONING                 	1728056253  	// .\Data\Effect\e0705000.chr
#define	SKILL_CONVENANT                 	1728056254  	// .\Data\Effect\e0707000.chr
#define	SKILL_MINDEXPLOSION             	1728056255  	// .\Data\Effect\e0708000.chr
#define	SKILL_SOULEATER                 	1728056256  	// .\Data\Effect\e0709000.chr
#define	SKILL_SUMMONACCELERATOR         	1728056257  	// .\Data\Effect\e0710000.chr
#define	SKILL_SUMMONMASTERY             	1728056258  	// .
#define	SKILL_TAMINGMASTERY             	1728056259  	// .
#define	SKILL_DOMINATION                	1728056260  	// .
#define	SKILL_BEYONDLIMIT               	1728056261  	// .
#define	SKILL_SOULBLAST1                	1728056262  	// .\Data\Effect\e0708000.chr
#define	SKILL_SOULBLAST2                	1728056263  	// .\Data\Effect\e0708001.chr
#define	SKILL_SOULBLAST3                	1728056264  	// .\Data\Effect\e0708002.chr
#define	SKILL_SOULBLAST4                	1728056265  	// .\Data\Effect\e0708003.chr
#define	SKILL_SOULBLAST5                	1728056266  	// .\Data\Effect\e0708004.chr
#define	SKILL_SOULBLAST6                	1728056267  	// .\Data\Effect\e0708005.chr
#define	SKILL_SOULBLAST7                	1728056268  	// .\Data\Effect\e0708006.chr
#define	SKILL_DETONATION1               	1728056269  	// .\Data\Effect\e0709000.chr
#define	SKILL_DETONATION2               	1728056270  	// .\Data\Effect\blank_effect.chr
#define	SKILL_DETONATION3               	1728056271  	// .\Data\Effect\blank_effect.chr
#define	SKILL_DETONATION4               	1728056272  	// .\Data\Effect\blank_effect.chr
#define	SKILL_DETONATION5               	1728056273  	// .\Data\Effect\blank_effect.chr
#define	SKILL_DETONATION6               	1728056274  	// .\Data\Effect\blank_effect.chr
#define	SKILL_DETONATION7               	1728056275  	// .\Data\Effect\blank_effect.chr
#define	SKILL_DETONATION_MONSTER        	1728056276  	// .\Data\Effect\e0710000.chr
#define	SKILL_TURNOVER                  	1728056277  	// .\Data\Effect\e0711000.chr
#define	SKILL_ILLUSION                  	1744833464  	// .\Data\Effect\e0801000.chr
#define	SKILL_POISONING                 	1744833465  	// .\Data\Effect\e0802000.chr
#define	SKILL_CHAINATTACKER1            	1744833466  	// .\Data\Effect\e0803000.chr
#define	SKILL_CHAINATTACKER2            	1744833467  	// .\Data\Effect\e0803001.chr
#define	SKILL_CHAINATTACKER3            	1744833468  	// .\Data\Effect\e0803002.chr
#define	SKILL_CHAINATTACKER4            	1744833469  	// .\Data\Effect\e0803003.chr
#define	SKILL_CHAINATTACKER5            	1744833470  	// .\Data\Effect\e0803004.chr
#define	SKILL_IRONSKIN                  	1744833472  	// .\Data\Effect\e0804000.chr
#define	SKILL_SNIPING                   	1744833473  	// .
#define	SKILL_SHADOWSQUARE              	1744833474  	// .
#define	SKILL_HIDING                    	1744833475  	// .
#define	SKILL_SPELLTRAP                 	1744833476  	// .\Data\Effect\e0807000.chr
#define	SKILL_PRESSURE                  	1744833477  	// .\Data\Effect\e0808000.chr
#define	SKILL_SYLPHID                   	1744833478  	// .\Data\Effect\e0809000.chr
#define	SKILL_SHAUT                     	1744833479  	// .\Data\Effect\e0810000.chr
#define	SKILL_BERSERK                   	1744833480  	// .\Data\Effect\e0811000.chr
#define	SKILL_CHAKRAMASTERY             	1744833481  	// .
#define	SKILL_SPEEDOVER                 	1744833482  	// .
#define	SKILL_DODGE                     	1744833483  	// .
#define	SKILL_TRAINING                  	1744833484  	// .
#define	SKILL_BLIND                     	1744833485  	// .\Data\Effect\e0810000.chr
#define	SKILL_PARALYSIS                 	1744833486  	// .\Data\Effect\e0809000.chr
#define	SKILL_POSIONCLOUD1              	1744833487  	// .\Data\Effect\e0811000.chr
#define	SKILL_POSIONCLOUD2              	1744833488  	// .\Data\Effect\e0811001.chr
#define	SKILL_POSIONCLOUD3              	1744833489  	// .\Data\Effect\e0811002.chr
#define	SKILL_POSIONCLOUD4              	1744833490  	// .\Data\Effect\e0811003.chr
#define	SKILL_POSIONCLOUD5              	1744833491  	// .\Data\Effect\e0811004.chr
#define	SKILL_POSIONCLOUD6              	1744833492  	// .\Data\Effect\e0811005.chr
#define	SKILL_POSIONCLOUD7              	1744833493  	// .\Data\Effect\e0811006.chr
#define	EFFECT_DUNGEONDEFENSESUCCESS_WORLD	1761607681  	// .\Data\Effect\e0124000.chr
#define	SKILL_DAMAGE1                   	1761610680  	// .\Data\Effect\e0103000.chr
#define	SKILL_DAMAGE2                   	1761610681  	// .\Data\Effect\e0105000.chr
#define	SKILL_CRITICAL                  	1761610682  	// .\Data\Effect\e0106000.chr
#define	SKILL_RECOVER                   	1761610683  	// .\Data\Effect\e0107000.chr
#define	SKILL_LEVELUP                   	1761610684  	// .\Data\Effect\e0108000.chr
#define	SKILL_DUST1                     	1761610685  	// .\Data\Effect\e0201001.chr
#define	SKILL_DUST2                     	1761610686  	// .\Data\Effect\e0201002.chr
#define	SKILL_DUST3                     	1761610687  	// .\Data\Effect\e0201003.chr
#define	SKILL_MAGICARRAY_FIRE           	1761610688  	// .\Data\Effect\e0301100.chr
#define	SKILL_MAGICARRAY_WATER          	1761610689  	// .\Data\Effect\e0301200.chr
#define	SKILL_MAGICARRAY_EARTH          	1761610690  	// .\Data\Effect\e0301300.chr
#define	SKILL_MAGICARRAY_WIND           	1761610691  	// .\Data\Effect\e0301400.chr
#define	SKILL_MAGICARRAY_SKY            	1761610692  	// .\Data\Effect\e0301500.chr
#define	SKILL_NONEEFFECT                	1761610693  	// .\Data\Effect\blank_effect.chr
#define	EFFECT_TELEPORT_POSITION        	1761610694  	// .\Data\Map\teleport_1.chr
#define	EFFECT_CP_POSITION              	1761610695  	// .\Data\Effect\cp.chr
#define	EFFECT_CP_RELEASE1              	1761610696  	// .\Data\Effect\e0112000.chr
#define	EFFECT_CP_RELEASE2              	1761610697  	// .\Data\Effect\e0113000.chr
#define	EFFECT_GUARDIAN_DYING           	1761610698  	// .\Data\Effect\e0114000.chr
#define	EFFECT_DUNGEON_CAPTURE          	1761610699  	// .\Data\Effect\e0111000.chr
#define	EFFECT_DUNGEON_GUARD_SUCCESS    	1761610700  	// .\Data\Effect\e0115000.chr
#define	EFFECT_DUNGEON_GUARDIAN         	1761610701  	// .\Data\Effect\e0116000.chr
#define	EFFECT_PK_COUNT                 	1761610702  	// .\Data\Effect\e0117000.chr
#define	EFFECT_DUNGEON_OWNER            	1761610703  	// .\Data\Effect\e0116001.chr
#define	EFFECT_DEFENCE_TEAM             	1761610704  	// .\Data\Effect\e0122000.chr
#define	EFFECT_OFFENSE_TEAM             	1761610705  	// .\Data\Effect\e0123000.chr
#define	EFFECT_HPPOTION                 	1761610706  	// .\Data\Effect\e0129000.CHR
#define	EFFECT_SNOW                     	1761610707  	// .\Data\Effect\env_snow1.chr
#define	EFFECT_USING_DOUBLEEXP_ITEM     	1761610708  	// .\Data\Effect\e0131000.chr
#define	EFFECT_CHAIN_EXPLODE            	1761610709  	// .\Data\Effect\e0803005.chr
#define	EFFECT_PLUS                     	1761610710  	// .\Data\Effect\e0611000.chr
#define	EFFECT_MINUS                    	1761610712  	// .\Data\Effect\e0612000.chr
#define	SKILL_MEDITATION                	1778387896  	// .
#define	SKILL_GUIDING                   	1778387897  	// .
#define	SKILL_MULTICASTING              	1778387898  	// .
#define	SKILL_CONCENTRATION             	1778387899  	// .
#define	SKILL_MANAMASTERY               	1778387900  	// .
#define	SKILLSTATUS_SPELL_MONSTER       	1929382840  	// .\Data\Effect\e0700000.chr
#define	SKILLSTATUS_SPELL_USER          	1929382841  	// .\Data\Effect\e0302000.chr
#define	SKILLSTATUS_OVERDRIVE1          	1929382842  	// .\Data\Effect\e0401501.chr
#define	SKILLSTATUS_OVERDRIVE2          	1929382843  	// .\Data\Effect\e0402501.chr
#define	SKILLSTATUS_OVERDRIVE3          	1929382844  	// .\Data\Effect\e0403501.chr
#define	DUNGEON_OWNS                    	-1174405119 	// .\Data\Effect\dungeon_effect_1.chr
#define	DUNGEON_SIEGING                 	-1174405118 	// .\Data\Effect\dungeon_effect_2.chr
#define	DUNGEON_GROUP                   	-1174405117 	// .\Data\Effect\dungeon_effect_3.chr
// ==== =================================== =============== ===============================
