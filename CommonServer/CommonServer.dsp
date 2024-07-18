# Microsoft Developer Studio Project File - Name="CommonServer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=CommonServer - Win32 dev
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CommonServer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CommonServer.mak" CFG="CommonServer - Win32 dev"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CommonServer - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "CommonServer - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "CommonServer - Win32 JAPAN_LOCALIZING_DBG" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "CommonServer - Win32 JAPAN_LOCALIZING_RRS" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "CommonServer - Win32 CHINA_LOCALIZING_DBG" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "CommonServer - Win32 CHINA_LOCALIZING_RRS" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "CommonServer - Win32 TAIWAN_LOCALIZING_DBG" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "CommonServer - Win32 TAIWAN_LOCALIZING_RRS" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "CommonServer - Win32 dev" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Develop/CommonServer", OEPAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "CommonServer - Win32 Release"

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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "COMMONSERVER_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "COMMONSERVER_EXPORTS" /D "NEW_COMMAND" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib WS2_32.lib /nologo /dll /machine:I386

!ELSEIF  "$(CFG)" == "CommonServer - Win32 Debug"

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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "COMMONSERVER_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W4 /WX /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "COMMONSERVER_EXPORTS" /D "NEW_COMMAND" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib WS2_32.lib /nologo /dll /pdb:"CommonServer.pdb" /debug /machine:I386 /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "CommonServer - Win32 JAPAN_LOCALIZING_DBG"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "CommonServer___Win32_JAPAN_LOCALIZING_DBG"
# PROP BASE Intermediate_Dir "CommonServer___Win32_JAPAN_LOCALIZING_DBG"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "CommonServer___Win32_JAPAN_LOCALIZING_DBG"
# PROP Intermediate_Dir "CommonServer___Win32_JAPAN_LOCALIZING_DBG"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W4 /WX /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "COMMONSERVER_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W4 /WX /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "COMMONSERVER_EXPORTS" /D "JAPAN_LOCALIZING" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /pdb:"CommonServer.pdb" /debug /machine:I386 /out:"..\CommonServer\CommonServer.dll" /implib:"CommonServer.lib" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib WS2_32.lib /nologo /dll /pdb:"CommonServer.pdb" /debug /machine:I386 /out:"..\CommonServer\CommonServer.dll" /implib:"CommonServer.lib" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "CommonServer - Win32 JAPAN_LOCALIZING_RRS"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "CommonServer___Win32_JAPAN_LOCALIZING_RRS"
# PROP BASE Intermediate_Dir "CommonServer___Win32_JAPAN_LOCALIZING_RRS"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "CommonServer___Win32_JAPAN_LOCALIZING_RRS"
# PROP Intermediate_Dir "CommonServer___Win32_JAPAN_LOCALIZING_RRS"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "COMMONSERVER_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "COMMONSERVER_EXPORTS" /D "JAPAN_LOCALIZING" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib WS2_32.lib /nologo /dll /machine:I386

!ELSEIF  "$(CFG)" == "CommonServer - Win32 CHINA_LOCALIZING_DBG"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "CommonServer___Win32_CHINA_LOCALIZING_DBG"
# PROP BASE Intermediate_Dir "CommonServer___Win32_CHINA_LOCALIZING_DBG"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "CommonServer___Win32_CHINA_LOCALIZING_DBG"
# PROP Intermediate_Dir "CommonServer___Win32_CHINA_LOCALIZING_DBG"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W4 /WX /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "COMMONSERVER_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W4 /WX /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "COMMONSERVER_EXPORTS" /D "CHINA_LOCALIZING" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /pdb:"CommonServer.pdb" /debug /machine:I386 /out:"..\CommonServer\CommonServer.dll" /implib:"CommonServer.lib" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib WS2_32.lib /nologo /dll /pdb:"CommonServer.pdb" /debug /machine:I386 /out:"..\CommonServer\CommonServer.dll" /implib:"CommonServer.lib" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "CommonServer - Win32 CHINA_LOCALIZING_RRS"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "CommonServer___Win32_CHINA_LOCALIZING_RRS"
# PROP BASE Intermediate_Dir "CommonServer___Win32_CHINA_LOCALIZING_RRS"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "CommonServer___Win32_CHINA_LOCALIZING_RRS"
# PROP Intermediate_Dir "CommonServer___Win32_CHINA_LOCALIZING_RRS"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "COMMONSERVER_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "COMMONSERVER_EXPORTS" /D "CHINA_LOCALIZING" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib WS2_32.lib /nologo /dll /machine:I386

!ELSEIF  "$(CFG)" == "CommonServer - Win32 TAIWAN_LOCALIZING_DBG"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "CommonServer___Win32_TAIWAN_LOCALIZING_DBG"
# PROP BASE Intermediate_Dir "CommonServer___Win32_TAIWAN_LOCALIZING_DBG"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "CommonServer___Win32_TAIWAN_LOCALIZING_DBG"
# PROP Intermediate_Dir "CommonServer___Win32_TAIWAN_LOCALIZING_DBG"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W4 /WX /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "COMMONSERVER_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W4 /WX /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "COMMONSERVER_EXPORTS" /D "TAIWAN_LOCALIZING" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /pdb:"CommonServer.pdb" /debug /machine:I386 /out:"..\CommonServer\CommonServer.dll" /implib:"CommonServer.lib" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib WS2_32.lib /nologo /dll /pdb:"CommonServer.pdb" /debug /machine:I386 /out:"..\CommonServer\CommonServer.dll" /implib:"CommonServer.lib" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "CommonServer - Win32 TAIWAN_LOCALIZING_RRS"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "CommonServer___Win32_TAIWAN_LOCALIZING_RRS"
# PROP BASE Intermediate_Dir "CommonServer___Win32_TAIWAN_LOCALIZING_RRS"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "CommonServer___Win32_TAIWAN_LOCALIZING_RRS"
# PROP Intermediate_Dir "CommonServer___Win32_TAIWAN_LOCALIZING_RRS"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "COMMONSERVER_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "COMMONSERVER_EXPORTS" /D "TAIWAN_LOCALIZING" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib WS2_32.lib /nologo /dll /machine:I386

!ELSEIF  "$(CFG)" == "CommonServer - Win32 dev"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "CommonServer___Win32_dev"
# PROP BASE Intermediate_Dir "CommonServer___Win32_dev"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "CommonServer___Win32_dev"
# PROP Intermediate_Dir "CommonServer___Win32_dev"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W4 /WX /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "COMMONSERVER_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W4 /WX /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "COMMONSERVER_EXPORTS" /D "DEV_MODE" /D "NEW_COMMAND" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib WS2_32.lib /nologo /dll /pdb:"CommonServer.pdb" /debug /machine:I386 /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib WS2_32.lib /nologo /dll /pdb:"CommonServer.pdb" /map /debug /machine:I386 /pdbtype:sept
# SUBTRACT LINK32 /pdb:none /nodefaultlib

!ENDIF 

# Begin Target

# Name "CommonServer - Win32 Release"
# Name "CommonServer - Win32 Debug"
# Name "CommonServer - Win32 JAPAN_LOCALIZING_DBG"
# Name "CommonServer - Win32 JAPAN_LOCALIZING_RRS"
# Name "CommonServer - Win32 CHINA_LOCALIZING_DBG"
# Name "CommonServer - Win32 CHINA_LOCALIZING_RRS"
# Name "CommonServer - Win32 TAIWAN_LOCALIZING_DBG"
# Name "CommonServer - Win32 TAIWAN_LOCALIZING_RRS"
# Name "CommonServer - Win32 dev"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CommonServer.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ComInterface.h
# End Source File
# Begin Source File

SOURCE=.\CommonAccessory.h
# End Source File
# Begin Source File

SOURCE=.\CommonGlobalDefine.h
# End Source File
# Begin Source File

SOURCE=.\CommonHash.h
# End Source File
# Begin Source File

SOURCE=.\CommonHeader.h
# End Source File
# Begin Source File

SOURCE=.\CommonPacket.h
# End Source File
# Begin Source File

SOURCE=.\CommonServer.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "Packet"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AgentServerPacket.h
# End Source File
# Begin Source File

SOURCE=.\CharSelectPacket.h
# End Source File
# Begin Source File

SOURCE=.\DungeonPacket.h
# End Source File
# Begin Source File

SOURCE=.\GamePacket.h
# End Source File
# Begin Source File

SOURCE=.\General_Command.h
# End Source File
# Begin Source File

SOURCE=.\GM_Command.h
# End Source File
# Begin Source File

SOURCE=.\GMPacket.h
# End Source File
# Begin Source File

SOURCE=.\LoginPacket.h
# End Source File
# Begin Source File

SOURCE=.\ProtocolDefinition.h
# End Source File
# Begin Source File

SOURCE=.\RivalGuildWarPacket.h
# End Source File
# Begin Source File

SOURCE=.\ServerPacket.h
# End Source File
# Begin Source File

SOURCE=.\WorldPacket.h
# End Source File
# End Group
# Begin Group "Common"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AbyssHash.h
# End Source File
# Begin Source File

SOURCE=.\AbyssHash64.h
# End Source File
# Begin Source File

SOURCE=.\Account.h
# End Source File
# Begin Source File

SOURCE=.\DFXForm.cpp
# End Source File
# Begin Source File

SOURCE=.\DFXForm.h
# End Source File
# Begin Source File

SOURCE=.\Encode.cpp
# End Source File
# Begin Source File

SOURCE=.\Encode.h
# End Source File
# Begin Source File

SOURCE=.\IllegalPacketLog.h
# End Source File
# Begin Source File

SOURCE=.\Log.cpp
# End Source File
# Begin Source File

SOURCE=.\Log.h
# End Source File
# Begin Source File

SOURCE=.\Profile.h
# End Source File
# Begin Source File

SOURCE=.\ReportCMListener.cpp
# End Source File
# Begin Source File

SOURCE=.\ReportCMListener.h
# End Source File
# Begin Source File

SOURCE=.\SizeReporter.h
# End Source File
# Begin Source File

SOURCE=.\StringFunc.h
# End Source File
# Begin Source File

SOURCE=.\UniqueProcess.cpp
# End Source File
# Begin Source File

SOURCE=.\UniqueProcess.h
# End Source File
# End Group
# Begin Group "COM Interface"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DBStruct.h
# End Source File
# Begin Source File

SOURCE=.\global.h
# End Source File
# Begin Source File

SOURCE=.\inetwork.h
# End Source File
# Begin Source File

SOURCE=.\IOleDBThread.h
# End Source File
# Begin Source File

SOURCE=.\network_guid.h
# End Source File
# Begin Source File

SOURCE=.\typedef.h
# End Source File
# End Group
# Begin Group "Item"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BaseItem.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseItem.h
# End Source File
# Begin Source File

SOURCE=.\Item.cpp
# End Source File
# Begin Source File

SOURCE=.\Item.h
# End Source File
# Begin Source File

SOURCE=.\ItemManager.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemManager.h
# End Source File
# Begin Source File

SOURCE=.\ItemManagerDefine.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
