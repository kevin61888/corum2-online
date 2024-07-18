# Microsoft Developer Studio Project File - Name="LoginAgent" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=LoginAgent - Win32 CHINA_TEST_BILLING
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "LoginAgent.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "LoginAgent.mak" CFG="LoginAgent - Win32 CHINA_TEST_BILLING"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "LoginAgent - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "LoginAgent - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "LoginAgent - Win32 TAIWAN" (based on "Win32 (x86) Console Application")
!MESSAGE "LoginAgent - Win32 CHINA" (based on "Win32 (x86) Console Application")
!MESSAGE "LoginAgent - Win32 JAPAN" (based on "Win32 (x86) Console Application")
!MESSAGE "LoginAgent - Win32 CHINA_BILLING" (based on "Win32 (x86) Console Application")
!MESSAGE "LoginAgent - Win32 JAPAN_Release" (based on "Win32 (x86) Console Application")
!MESSAGE "LoginAgent - Win32 CHINA_TEST_BILLING" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/CorumOnline - LoginAgentServer", WABAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "LoginAgent - Win32 Release"

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
# ADD CPP /nologo /MT /W4 /WX /GX /Ot /Oa /Ow /Oi /Op /Oy /Ob1 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib cpssolib.lib /nologo /subsystem:console /incremental:yes /map /debug /machine:I386

!ELSEIF  "$(CFG)" == "LoginAgent - Win32 Debug"

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
# ADD CPP /nologo /MTd /W4 /WX /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 WS2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib cpssolib.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "LoginAgent - Win32 TAIWAN"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "LoginAgent___Win32_TAIWAN"
# PROP BASE Intermediate_Dir "LoginAgent___Win32_TAIWAN"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "LoginAgent___Win32_TAIWAN"
# PROP Intermediate_Dir "LoginAgent___Win32_TAIWAN"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /WX /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /WX /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "TAIWAN_LOCALIZING" /D "BILLING_CHECK" /D "__PACKET_ENCRYPTION" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib WS2_32.lib cpssolib.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 WS2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib cpssolib.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ELSEIF  "$(CFG)" == "LoginAgent - Win32 CHINA"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "LoginAgent___Win32_CHINA"
# PROP BASE Intermediate_Dir "LoginAgent___Win32_CHINA"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "LoginAgent___Win32_CHINA"
# PROP Intermediate_Dir "LoginAgent___Win32_CHINA"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /WX /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /WX /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "CHINA_LOCALIZING" /D "__PACKET_ENCRYPTION" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib WS2_32.lib cpssolib.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 WS2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib cpssolib.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ELSEIF  "$(CFG)" == "LoginAgent - Win32 JAPAN"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "LoginAgent___Win32_JAPAN"
# PROP BASE Intermediate_Dir "LoginAgent___Win32_JAPAN"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "LoginAgent___Win32_JAPAN"
# PROP Intermediate_Dir "LoginAgent___Win32_JAPAN"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /WX /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /WX /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "JAPAN_LOCALIZING" /D "__PACKET_ENCRYPTION" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib WS2_32.lib cpssolib.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 WS2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib cpssolib.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ELSEIF  "$(CFG)" == "LoginAgent - Win32 CHINA_BILLING"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "LoginAgent___Win32_CHINA_BILLING"
# PROP BASE Intermediate_Dir "LoginAgent___Win32_CHINA_BILLING"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "LoginAgent___Win32_CHINA_BILLING"
# PROP Intermediate_Dir "LoginAgent___Win32_CHINA_BILLING"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /WX /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "CHINA_LOCALIZING" /D "__PACKET_ENCRYPTION" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /WX /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "CHINA_LOCALIZING" /D "__PACKET_ENCRYPTION" /D "CHINA_BILLING" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib WS2_32.lib cpssolib.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 WS2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib cpssolib.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ELSEIF  "$(CFG)" == "LoginAgent - Win32 JAPAN_Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "LoginAgent___Win32_JAPAN_Release"
# PROP BASE Intermediate_Dir "LoginAgent___Win32_JAPAN_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "LoginAgent___Win32_JAPAN_Release"
# PROP Intermediate_Dir "LoginAgent___Win32_JAPAN_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /WX /GX /Ot /Oa /Ow /Oi /Op /Oy /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /WX /GX /Ot /Oa /Ow /Oi /Op /Oy /Ob1 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "JAPAN_LOCALIZING" /D "__PACKET_ENCRYPTION" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ws2_32.lib cpssolib.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib cpssolib.lib /nologo /subsystem:console /incremental:yes /map /debug /machine:I386

!ELSEIF  "$(CFG)" == "LoginAgent - Win32 CHINA_TEST_BILLING"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "LoginAgent___Win32_CHINA_TEST_BILLING"
# PROP BASE Intermediate_Dir "LoginAgent___Win32_CHINA_TEST_BILLING"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "LoginAgent___Win32_CHINA_TEST_BILLING"
# PROP Intermediate_Dir "LoginAgent___Win32_CHINA_TEST_BILLING"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /WX /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "CHINA_LOCALIZING" /D "__PACKET_ENCRYPTION" /D "CHINA_BILLING" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /WX /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "CHINA_LOCALIZING" /D "__PACKET_ENCRYPTION" /D "CHINA_BILLING" /D "CHINA_TEST_BILLING" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib WS2_32.lib cpssolib.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 WS2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib cpssolib.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "LoginAgent - Win32 Release"
# Name "LoginAgent - Win32 Debug"
# Name "LoginAgent - Win32 TAIWAN"
# Name "LoginAgent - Win32 CHINA"
# Name "LoginAgent - Win32 JAPAN"
# Name "LoginAgent - Win32 CHINA_BILLING"
# Name "LoginAgent - Win32 JAPAN_Release"
# Name "LoginAgent - Win32 CHINA_TEST_BILLING"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\DBProc.cpp
# End Source File
# Begin Source File

SOURCE=.\DBProcItemMall.cpp
# End Source File
# Begin Source File

SOURCE=.\Filter.cpp
# End Source File
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgProc.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgProcForServer.cpp
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

SOURCE=.\ThrowQuery.cpp
# End Source File
# Begin Source File

SOURCE=.\UserTable.cpp
# End Source File
# Begin Source File

SOURCE=.\WorldServerTable.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\DBProc.h
# End Source File
# Begin Source File

SOURCE=.\DBProcItemMall.h
# End Source File
# Begin Source File

SOURCE=.\Filter.h
# End Source File
# Begin Source File

SOURCE=.\GlobalDefine.h
# End Source File
# Begin Source File

SOURCE=.\MsgProc.h
# End Source File
# Begin Source File

SOURCE=.\MsgProcForServer.h
# End Source File
# Begin Source File

SOURCE=.\OwnServer.h
# End Source File
# Begin Source File

SOURCE=.\recvmsg.h
# End Source File
# Begin Source File

SOURCE=.\ServerFunction.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\ThrowQuery.h
# End Source File
# Begin Source File

SOURCE=.\User.h
# End Source File
# Begin Source File

SOURCE=.\UserTable.h
# End Source File
# Begin Source File

SOURCE=.\Version.h
# End Source File
# Begin Source File

SOURCE=.\WorldServerTable.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "Netmarble"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CPSSOLib.h
# End Source File
# Begin Source File

SOURCE=.\libcomservice2.dll
# End Source File
# Begin Source File

SOURCE=.\CPSSOLib.lib
# End Source File
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

SOURCE=.\OnlyList.cpp
# End Source File
# Begin Source File

SOURCE=.\OnlyList.h
# End Source File
# End Group
# Begin Group "ChinaBilling"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ChinaBillingPacket.h
# End Source File
# Begin Source File

SOURCE=.\ChinaBillingProc.cpp
# End Source File
# Begin Source File

SOURCE=.\ChinaBillingProc.h
# End Source File
# End Group
# Begin Group "Accessory"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\PacketEncrypt.cpp
# End Source File
# Begin Source File

SOURCE=.\PacketEncrypt.h
# End Source File
# End Group
# Begin Group "Licensing"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\LicensingPacket.h
# End Source File
# Begin Source File

SOURCE=.\LicensingProc.cpp
# End Source File
# Begin Source File

SOURCE=.\LicensingProc.h
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
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
