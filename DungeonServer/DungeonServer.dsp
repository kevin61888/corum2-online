# Microsoft Developer Studio Project File - Name="DungeonServer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=DungeonServer - Win32 Dev
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "DungeonServer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "DungeonServer.mak" CFG="DungeonServer - Win32 Dev"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "DungeonServer - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "DungeonServer - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "DungeonServer - Win32 CHINA_LOCALIZING" (based on "Win32 (x86) Console Application")
!MESSAGE "DungeonServer - Win32 JAPAN_LOCALIZING" (based on "Win32 (x86) Console Application")
!MESSAGE "DungeonServer - Win32 TAIWAN_LOCALIZING" (based on "Win32 (x86) Console Application")
!MESSAGE "DungeonServer - Win32 Debug_with_New_Command" (based on "Win32 (x86) Console Application")
!MESSAGE "DungeonServer - Win32 Dev" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/CorumOnline - DungeonServer", JCBAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "DungeonServer - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W4 /GX /Ot /Oa /Ow /Oi /Op /Oy /Ob1 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# SUBTRACT CPP /WX
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib WS2_32.lib commonserver.lib /nologo /subsystem:console /debug /machine:I386 /libpath:"..\commonserver\\"

!ELSEIF  "$(CFG)" == "DungeonServer - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W4 /WX /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 CommonServer.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib WS2_32.lib commonserver.lib /nologo /subsystem:console /map /debug /machine:I386 /nodefaultlib:"libc.lib" /pdbtype:sept /libpath:"..\commonserver\\"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "DungeonServer - Win32 CHINA_LOCALIZING"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "DungeonServer___Win32_CHINA_LOCALIZING"
# PROP BASE Intermediate_Dir "DungeonServer___Win32_CHINA_LOCALIZING"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "DungeonServer___Win32_CHINA_LOCALIZING"
# PROP Intermediate_Dir "DungeonServer___Win32_CHINA_LOCALIZING"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /Ot /Oa /Ow /Oi /Op /Oy /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /Ot /Oa /Ow /Oi /Op /Oy /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "CHINA_LOCALIZING" /YX /FD /c
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 WS2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib WS2_32.lib commonserver.lib /nologo /subsystem:console /debug /machine:I386 /libpath:"..\commonserver\\"

!ELSEIF  "$(CFG)" == "DungeonServer - Win32 JAPAN_LOCALIZING"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "DungeonServer___Win32_JAPAN_LOCALIZING"
# PROP BASE Intermediate_Dir "DungeonServer___Win32_JAPAN_LOCALIZING"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "DungeonServer___Win32_JAPAN_LOCALIZING"
# PROP Intermediate_Dir "DungeonServer___Win32_JAPAN_LOCALIZING"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /Ot /Oa /Ow /Oi /Op /Oy /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /ZI /Ot /Oa /Ow /Oi /Op /Oy /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "JAPAN_LOCALIZING" /YX /FD /c
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 WS2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib WS2_32.lib commonserver.lib /nologo /subsystem:console /debug /machine:I386 /libpath:"..\commonserver\\"

!ELSEIF  "$(CFG)" == "DungeonServer - Win32 TAIWAN_LOCALIZING"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "DungeonServer___Win32_TAIWAN_LOCALIZING"
# PROP BASE Intermediate_Dir "DungeonServer___Win32_TAIWAN_LOCALIZING"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "DungeonServer___Win32_TAIWAN_LOCALIZING"
# PROP Intermediate_Dir "DungeonServer___Win32_TAIWAN_LOCALIZING"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /Ot /Oa /Ow /Oi /Op /Oy /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /WX /GX /Ot /Oa /Ow /Oi /Op /Oy /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "TAIWAN_LOCALIZING" /YX /FD /c
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 WS2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib WS2_32.lib commonserver.lib /nologo /subsystem:console /debug /machine:I386 /libpath:"..\commonserver\\"

!ELSEIF  "$(CFG)" == "DungeonServer - Win32 Debug_with_New_Command"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "DungeonServer___Win32_Debug_with_New_Command"
# PROP BASE Intermediate_Dir "DungeonServer___Win32_Debug_with_New_Command"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DungeonServer___Win32_Debug_with_New_Command"
# PROP Intermediate_Dir "DungeonServer___Win32_Debug_with_New_Command"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /WX /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /WX /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "DEV_MODE" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib WS2_32.lib /nologo /subsystem:console /map /debug /machine:I386 /nodefaultlib:"libc.lib" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib WS2_32.lib commonserver.lib /nologo /subsystem:console /map /debug /machine:I386 /nodefaultlib:"libc.lib" /pdbtype:sept /libpath:"..\commonserver\\"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "DungeonServer - Win32 Dev"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "DungeonServer___Win32_Dev"
# PROP BASE Intermediate_Dir "DungeonServer___Win32_Dev"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DungeonServer___Win32_Dev"
# PROP Intermediate_Dir "DungeonServer___Win32_Dev"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W4 /WX /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "DEV_MODE" /FR /YX /FD /GZ /c
# SUBTRACT CPP /WX
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 CommonServer.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib WS2_32.lib commonserver.lib /nologo /subsystem:console /map /debug /machine:I386 /nodefaultlib:"libc.lib" /pdbtype:sept /libpath:"..\commonserver\\"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 CommonServer.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib WS2_32.lib commonserver.lib /nologo /subsystem:console /map /debug /machine:I386 /nodefaultlib:"libc.lib" /out:"..\..\inner\3_DungeonServer Configurations_VILLAGE\DungeonServer.exe" /pdbtype:sept /libpath:"..\commonserver\\" /mapinfo:lines
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "DungeonServer - Win32 Release"
# Name "DungeonServer - Win32 Debug"
# Name "DungeonServer - Win32 CHINA_LOCALIZING"
# Name "DungeonServer - Win32 JAPAN_LOCALIZING"
# Name "DungeonServer - Win32 TAIWAN_LOCALIZING"
# Name "DungeonServer - Win32 Debug_with_New_Command"
# Name "DungeonServer - Win32 Dev"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AcceptTable.cpp
# End Source File
# Begin Source File

SOURCE=.\cGDI.cpp
# End Source File
# Begin Source File

SOURCE=.\DBProc.cpp
# End Source File
# Begin Source File

SOURCE=.\DungeonServer.rc
# End Source File
# Begin Source File

SOURCE=.\GameSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\GameSystem_for_yak.cpp
# End Source File
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# Begin Source File

SOURCE=.\OwnServer.cpp
# End Source File
# Begin Source File

SOURCE=.\recvmsg.cpp
# End Source File
# Begin Source File

SOURCE=.\ServerFunction.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\IndexCreator.lib
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AcceptTable.h
# End Source File
# Begin Source File

SOURCE=.\cGDI.h
# End Source File
# Begin Source File

SOURCE=.\DBProc.h
# End Source File
# Begin Source File

SOURCE=.\GameSystem.h
# End Source File
# Begin Source File

SOURCE=.\GameSystem_for_yak.h
# End Source File
# Begin Source File

SOURCE=.\GlobalDefine.h
# End Source File
# Begin Source File

SOURCE=.\OwnServer.h
# End Source File
# Begin Source File

SOURCE=.\RecordSet.h
# End Source File
# Begin Source File

SOURCE=.\recvmsg.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\ServerFunction.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\cursor1.cur
# End Source File
# End Group
# Begin Group "PathFinding"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\PathFinding\BlockLineManager.cpp
# End Source File
# Begin Source File

SOURCE=.\PathFinding\BlockLineManager.h
# End Source File
# Begin Source File

SOURCE=.\PathFinding\BlockManager.cpp
# End Source File
# Begin Source File

SOURCE=.\PathFinding\BlockManager.h
# End Source File
# Begin Source File

SOURCE=.\PathFinding\ControlStruct.cpp
# End Source File
# Begin Source File

SOURCE=.\PathFinding\ControlStruct.h
# End Source File
# Begin Source File

SOURCE=.\PathFinding\DirectionManager.cpp
# End Source File
# Begin Source File

SOURCE=.\PathFinding\DirectionManager.h
# End Source File
# Begin Source File

SOURCE=.\PathFinding\MemoryBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\PathFinding\MemoryBlock.h
# End Source File
# Begin Source File

SOURCE=.\PathFinding\PriorityQue.cpp
# End Source File
# Begin Source File

SOURCE=.\PathFinding\PriorityQue.h
# End Source File
# Begin Source File

SOURCE=.\PathFinding\SearchModule_Astar.cpp
# End Source File
# Begin Source File

SOURCE=.\PathFinding\SearchModule_Astar.h
# End Source File
# Begin Source File

SOURCE=.\PathFinding\VirtualBlockManager.cpp
# End Source File
# Begin Source File

SOURCE=.\PathFinding\VirtualBlockManager.h
# End Source File
# End Group
# Begin Group "Effect"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Effect.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect.h
# End Source File
# End Group
# Begin Group "Accessary"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\TrafficLog.cpp
# End Source File
# Begin Source File

SOURCE=.\TrafficLog.h
# End Source File
# End Group
# Begin Group "Container"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\LinkedList.cpp
# End Source File
# Begin Source File

SOURCE=.\Linkedlist.h
# End Source File
# Begin Source File

SOURCE=.\Voidlist.cpp
# End Source File
# Begin Source File

SOURCE=.\Voidlist.h
# End Source File
# End Group
# Begin Group "MsgProc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MsgProc.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgProc.h
# End Source File
# Begin Source File

SOURCE=.\MsgProc_for_jjw6263.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgProc_for_jjw6263.h
# End Source File
# Begin Source File

SOURCE=.\MsgProc_for_minbobo.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgProc_for_minbobo.h
# End Source File
# Begin Source File

SOURCE=.\MsgProc_for_y2jin.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgProc_for_y2jin.h
# End Source File
# Begin Source File

SOURCE=.\MsgProc_for_yak.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgProc_for_yak.h
# End Source File
# End Group
# Begin Group "ItemNative"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GameSystemNative.cpp
# End Source File
# Begin Source File

SOURCE=.\GameSystemNative.h
# End Source File
# Begin Source File

SOURCE=.\ItemAttrLayer.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemAttrLayer.h
# End Source File
# Begin Source File

SOURCE=.\ItemMove.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemMove.h
# End Source File
# Begin Source File

SOURCE=.\ItemMoveManager.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemMoveManager.h
# End Source File
# Begin Source File

SOURCE=.\ItemNative.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemNative.h
# End Source File
# Begin Source File

SOURCE=.\ItemNativeManager.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemNativeManager.h
# End Source File
# Begin Source File

SOURCE=.\ItemUsedManager.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemUsedManager.h
# End Source File
# End Group
# Begin Group "RivalGuildWar"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\RivalGuildWar.cpp
# End Source File
# Begin Source File

SOURCE=.\RivalGuildWar.h
# End Source File
# End Group
# Begin Group "EventDungeon"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\EventDungeonManager.cpp
# End Source File
# Begin Source File

SOURCE=.\EventDungeonManager.h
# End Source File
# End Group
# Begin Group "CommonServer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CommonServer.dll
# End Source File
# Begin Source File

SOURCE=.\CommonServer.lib
# End Source File
# End Group
# Begin Group "Unit"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ClassMemoryPool.cpp
# End Source File
# Begin Source File

SOURCE=.\ClassMemoryPool.h
# End Source File
# Begin Source File

SOURCE=.\Monster.cpp
# End Source File
# Begin Source File

SOURCE=.\Monster.h
# End Source File
# Begin Source File

SOURCE=.\MonsterManager.cpp
# End Source File
# Begin Source File

SOURCE=.\MonsterManager.h
# End Source File
# Begin Source File

SOURCE=.\MonsterTable.cpp
# End Source File
# Begin Source File

SOURCE=.\MonsterTable.h
# End Source File
# Begin Source File

SOURCE=.\TraderMonster.cpp
# End Source File
# Begin Source File

SOURCE=.\TraderMonster.h
# End Source File
# Begin Source File

SOURCE=.\Unit.cpp
# End Source File
# Begin Source File

SOURCE=.\Unit.h
# End Source File
# Begin Source File

SOURCE=.\User.cpp
# End Source File
# Begin Source File

SOURCE=.\User.h
# End Source File
# Begin Source File

SOURCE=.\UserItemSystem.cpp
# End Source File
# End Group
# Begin Group "Dungeon"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DunGeon.cpp
# End Source File
# Begin Source File

SOURCE=.\Dungeon.h
# End Source File
# Begin Source File

SOURCE=.\DUNGEON_DATA.h
# End Source File
# Begin Source File

SOURCE=.\DUNGEON_DATA_EX.cpp
# End Source File
# Begin Source File

SOURCE=.\DUNGEON_DATA_EX.h
# End Source File
# Begin Source File

SOURCE=.\DungeonLayer.cpp
# End Source File
# Begin Source File

SOURCE=.\DungeonLayer.h
# End Source File
# Begin Source File

SOURCE=.\DungeonTable.cpp
# End Source File
# Begin Source File

SOURCE=.\DungeonTable.h
# End Source File
# Begin Source File

SOURCE=.\LayerFormation.h
# End Source File
# Begin Source File

SOURCE=.\Map.cpp
# End Source File
# Begin Source File

SOURCE=.\Map.h
# End Source File
# Begin Source File

SOURCE=.\Section.cpp
# End Source File
# Begin Source File

SOURCE=.\Section.h
# End Source File
# End Group
# Begin Group "MsgProcForServer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MsgProcForServer.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgProcForServer.h
# End Source File
# Begin Source File

SOURCE=.\MsgProcForServerEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgProcForServerEvent.h
# End Source File
# Begin Source File

SOURCE=.\MsgProcForServerGuild.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgProcForServerGuild.h
# End Source File
# Begin Source File

SOURCE=.\MsgProcForServerParty.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgProcForServerParty.h
# End Source File
# Begin Source File

SOURCE=.\MsgProcForServerPortal.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgProcForServerPortal.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\DungeonConfig.ini
# End Source File
# Begin Source File

SOURCE=.\DungeonServer.dsp
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
