# Microsoft Developer Studio Project File - Name="WorldServer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=WorldServer - Win32 Debug_with_NEW_COMMAND
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "WorldServer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "WorldServer.mak" CFG="WorldServer - Win32 Debug_with_NEW_COMMAND"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "WorldServer - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "WorldServer - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "WorldServer - Win32 JAPAN_LOCALIZING" (based on "Win32 (x86) Console Application")
!MESSAGE "WorldServer - Win32 CHINA_LOCALIZING" (based on "Win32 (x86) Console Application")
!MESSAGE "WorldServer - Win32 TAIWAN_LOCALIZING" (based on "Win32 (x86) Console Application")
!MESSAGE "WorldServer - Win32 CHINA_BILLING" (based on "Win32 (x86) Console Application")
!MESSAGE "WorldServer - Win32 CHINA_TEST_BILLING" (based on "Win32 (x86) Console Application")
!MESSAGE "WorldServer - Win32 Debug_with_NEW_COMMAND" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Develop/WorldServer", XANAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "WorldServer - Win32 Release"

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
# ADD CPP /nologo /MT /W4 /WX /GX /Ot /Oa /Ow /Oi /Oy /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Os
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib WS2_32.lib /nologo /subsystem:console /debug /machine:I386

!ELSEIF  "$(CFG)" == "WorldServer - Win32 Debug"

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
# ADD CPP /nologo /MTd /W4 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# SUBTRACT CPP /WX
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib WS2_32.lib /nologo /subsystem:console /debug /machine:I386 /out:"D:\Corum\INNER\2_WorldServer Configurations\WorldServer.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "WorldServer - Win32 JAPAN_LOCALIZING"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "WorldServer___Win32_JAPAN_LOCALIZING"
# PROP BASE Intermediate_Dir "WorldServer___Win32_JAPAN_LOCALIZING"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "WorldServer___Win32_JAPAN_LOCALIZING"
# PROP Intermediate_Dir "WorldServer___Win32_JAPAN_LOCALIZING"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /Ot /Oa /Ow /Oi /Oy /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /c
# SUBTRACT BASE CPP /Os
# ADD CPP /nologo /MT /W4 /WX /GX /ZI /Ot /Oa /Ow /Oi /Oy /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "JAPAN_LOCALIZING" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Os
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 WS2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib WS2_32.lib /nologo /subsystem:console /debug /machine:I386

!ELSEIF  "$(CFG)" == "WorldServer - Win32 CHINA_LOCALIZING"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "WorldServer___Win32_CHINA_LOCALIZING"
# PROP BASE Intermediate_Dir "WorldServer___Win32_CHINA_LOCALIZING"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "WorldServer___Win32_CHINA_LOCALIZING"
# PROP Intermediate_Dir "WorldServer___Win32_CHINA_LOCALIZING"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /Ot /Oa /Ow /Oi /Oy /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /c
# SUBTRACT BASE CPP /Os
# ADD CPP /nologo /MT /W4 /WX /GX /Ot /Oa /Ow /Oi /Oy /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "CHINA_LOCALIZING" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Os
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 WS2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib WS2_32.lib /nologo /subsystem:console /debug /machine:I386

!ELSEIF  "$(CFG)" == "WorldServer - Win32 TAIWAN_LOCALIZING"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "WorldServer___Win32_TAIWAN_LOCALIZING"
# PROP BASE Intermediate_Dir "WorldServer___Win32_TAIWAN_LOCALIZING"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "WorldServer___Win32_TAIWAN_LOCALIZING"
# PROP Intermediate_Dir "WorldServer___Win32_TAIWAN_LOCALIZING"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /Ot /Oa /Ow /Oi /Oy /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /c
# SUBTRACT BASE CPP /Os
# ADD CPP /nologo /MT /W4 /WX /GX /Ot /Oa /Ow /Oi /Oy /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "TAIWAN_LOCALIZING" /D "BILLING_CHECK" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Os
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 WS2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib WS2_32.lib /nologo /subsystem:console /debug /machine:I386

!ELSEIF  "$(CFG)" == "WorldServer - Win32 CHINA_BILLING"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "WorldServer___Win32_CHINA_BILLING"
# PROP BASE Intermediate_Dir "WorldServer___Win32_CHINA_BILLING"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "WorldServer___Win32_CHINA_BILLING"
# PROP Intermediate_Dir "WorldServer___Win32_CHINA_BILLING"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /Ot /Oa /Ow /Oi /Oy /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /c
# SUBTRACT BASE CPP /Os
# ADD CPP /nologo /MT /W4 /WX /GX /Ot /Oa /Ow /Oi /Oy /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "CHINA_LOCALIZING" /D "CHINA_BILLING" /FR /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Os
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 WS2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib WS2_32.lib /nologo /subsystem:console /debug /machine:I386

!ELSEIF  "$(CFG)" == "WorldServer - Win32 CHINA_TEST_BILLING"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "WorldServer___Win32_CHINA_TEST_BILLING"
# PROP BASE Intermediate_Dir "WorldServer___Win32_CHINA_TEST_BILLING"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "WorldServer___Win32_CHINA_TEST_BILLING"
# PROP Intermediate_Dir "WorldServer___Win32_CHINA_TEST_BILLING"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W4 /WX /GX /Ot /Oa /Ow /Oi /Oy /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "CHINA_LOCALIZING" /D "CHINA_BILLING" /FR /Yu"stdafx.h" /FD /c
# SUBTRACT BASE CPP /Os
# ADD CPP /nologo /MT /W4 /WX /GX /Ot /Oa /Ow /Oi /Oy /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "CHINA_LOCALIZING" /D "CHINA_BILLING" /D "CHINA_TEST_BILLING" /FR /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Os
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 WS2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib WS2_32.lib /nologo /subsystem:console /debug /machine:I386

!ELSEIF  "$(CFG)" == "WorldServer - Win32 Debug_with_NEW_COMMAND"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "WorldServer___Win32_Debug_with_NEW_COMMAND0"
# PROP BASE Intermediate_Dir "WorldServer___Win32_Debug_with_NEW_COMMAND0"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "WorldServer___Win32_Debug_with_NEW_COMMAND0"
# PROP Intermediate_Dir "WorldServer___Win32_Debug_with_NEW_COMMAND0"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W4 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# SUBTRACT BASE CPP /WX
# ADD CPP /nologo /MTd /W4 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "NEW_COMMAND" /D "DEV_MODE" /FR /Yu"stdafx.h" /FD /GZ /c
# SUBTRACT CPP /WX
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib WS2_32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib WS2_32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "WorldServer - Win32 Release"
# Name "WorldServer - Win32 Debug"
# Name "WorldServer - Win32 JAPAN_LOCALIZING"
# Name "WorldServer - Win32 CHINA_LOCALIZING"
# Name "WorldServer - Win32 TAIWAN_LOCALIZING"
# Name "WorldServer - Win32 CHINA_BILLING"
# Name "WorldServer - Win32 CHINA_TEST_BILLING"
# Name "WorldServer - Win32 Debug_with_NEW_COMMAND"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AcceptTable.cpp
# End Source File
# Begin Source File

SOURCE=.\ChatLog.cpp
# End Source File
# Begin Source File

SOURCE=.\ConnectionDistributionTable.cpp
# End Source File
# Begin Source File

SOURCE=.\DBProc.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgProc.cpp
# End Source File
# Begin Source File

SOURCE=.\NameSearchTable.cpp
# End Source File
# Begin Source File

SOURCE=.\NTOS.CPP
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

SOURCE=.\ServerTable.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\WorldServer.cpp
# End Source File
# Begin Source File

SOURCE=.\WorldUser.cpp
# End Source File
# Begin Source File

SOURCE=.\WorldUserTable.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AcceptTable.h
# End Source File
# Begin Source File

SOURCE=.\ChatLog.h
# End Source File
# Begin Source File

SOURCE=.\ConnectionDistributionTable.h
# End Source File
# Begin Source File

SOURCE=.\DBProc.h
# End Source File
# Begin Source File

SOURCE=.\GlobalDefine.h
# End Source File
# Begin Source File

SOURCE=.\MsgProc.h
# End Source File
# Begin Source File

SOURCE=.\NameSearchTable.h
# End Source File
# Begin Source File

SOURCE=.\NTOS.H
# End Source File
# Begin Source File

SOURCE=.\OwnServer.h
# End Source File
# Begin Source File

SOURCE=.\recvmsg.h
# End Source File
# Begin Source File

SOURCE=.\Section.h
# End Source File
# Begin Source File

SOURCE=.\ServerFunction.h
# End Source File
# Begin Source File

SOURCE=.\ServerTable.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\WorldUser.h
# End Source File
# Begin Source File

SOURCE=.\WorldUserTable.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "Container"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Linkedlist.cpp
# End Source File
# Begin Source File

SOURCE=.\Linkedlist.h
# End Source File
# Begin Source File

SOURCE=.\LinkedListEx.cpp
# End Source File
# Begin Source File

SOURCE=.\LinkedListEx.h
# End Source File
# Begin Source File

SOURCE=.\OnlyList.cpp
# End Source File
# Begin Source File

SOURCE=.\OnlyList.h
# End Source File
# End Group
# Begin Group "ChinaBilling"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ChinaBillingProc.cpp
# End Source File
# Begin Source File

SOURCE=.\ChinaBillingProc.h
# End Source File
# End Group
# Begin Group "Party&Guild&Messenger"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Guild.cpp
# End Source File
# Begin Source File

SOURCE=.\Guild.h
# End Source File
# Begin Source File

SOURCE=.\GuildDef.h
# End Source File
# Begin Source File

SOURCE=.\GuildHash.cpp
# End Source File
# Begin Source File

SOURCE=.\GuildHash.h
# End Source File
# Begin Source File

SOURCE=.\Messenger.cpp
# End Source File
# Begin Source File

SOURCE=.\Messenger.h
# End Source File
# Begin Source File

SOURCE=.\MessengerDef.h
# End Source File
# Begin Source File

SOURCE=.\MessengerHash.cpp
# End Source File
# Begin Source File

SOURCE=.\MessengerHash.h
# End Source File
# Begin Source File

SOURCE=.\Party.cpp
# End Source File
# Begin Source File

SOURCE=.\Party.h
# End Source File
# Begin Source File

SOURCE=.\PartyDef.h
# End Source File
# Begin Source File

SOURCE=.\PartyHash.cpp
# End Source File
# Begin Source File

SOURCE=.\PartyHash.h
# End Source File
# End Group
# Begin Group "RivalGuildWar"

# PROP Default_Filter "cpp;h"
# Begin Source File

SOURCE=.\RivalGuildWar.cpp
# End Source File
# Begin Source File

SOURCE=.\RivalGuildWar.h
# End Source File
# Begin Source File

SOURCE=.\RivalGuildWarLib.cpp
# End Source File
# Begin Source File

SOURCE=.\RivalGuildWarLib.h
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

SOURCE=.\MsgProcForServerFromAgent.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgProcForServerFromAgent.h
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
# Begin Group "CommonServer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CommonServer.dll
# End Source File
# Begin Source File

SOURCE=.\CommonServer.lib
# End Source File
# End Group
# Begin Group "Dungeon"

# PROP Default_Filter ""
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

SOURCE=.\DungeonTable.cpp
# End Source File
# Begin Source File

SOURCE=.\DungeonTable.h
# End Source File
# Begin Source File

SOURCE=.\EventDungeonManager.cpp
# End Source File
# Begin Source File

SOURCE=.\EventDungeonManager.h
# End Source File
# Begin Source File

SOURCE=.\Map.cpp
# End Source File
# Begin Source File

SOURCE=.\Map.h
# End Source File
# Begin Source File

SOURCE=.\SchoolGuildDungeon.cpp
# End Source File
# Begin Source File

SOURCE=.\SchoolGuildDungeon.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
