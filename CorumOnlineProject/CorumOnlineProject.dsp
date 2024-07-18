# Microsoft Developer Studio Project File - Name="CorumOnlineProject" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=CorumOnlineProject - Win32 EngineTest
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CorumOnlineProject.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CorumOnlineProject.mak" CFG="CorumOnlineProject - Win32 EngineTest"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CorumOnlineProject - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "CorumOnlineProject - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "CorumOnlineProject - Win32 dev" (based on "Win32 (x86) Application")
!MESSAGE "CorumOnlineProject - Win32 JAPAN" (based on "Win32 (x86) Application")
!MESSAGE "CorumOnlineProject - Win32 JAPAN_DEV" (based on "Win32 (x86) Application")
!MESSAGE "CorumOnlineProject - Win32 CHINA" (based on "Win32 (x86) Application")
!MESSAGE "CorumOnlineProject - Win32 CHINA_DEV" (based on "Win32 (x86) Application")
!MESSAGE "CorumOnlineProject - Win32 TAIWAN" (based on "Win32 (x86) Application")
!MESSAGE "CorumOnlineProject - Win32 TAIWAN_DEV" (based on "Win32 (x86) Application")
!MESSAGE "CorumOnlineProject - Win32 JAPAN_ENCRYPE" (based on "Win32 (x86) Application")
!MESSAGE "CorumOnlineProject - Win32 dev_NEW_COMMAND" (based on "Win32 (x86) Application")
!MESSAGE "CorumOnlineProject - Win32 develop_Test" (based on "Win32 (x86) Application")
!MESSAGE "CorumOnlineProject - Win32 Korea_GMMode" (based on "Win32 (x86) Application")
!MESSAGE "CorumOnlineProject - Win32 EngineTest" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/CorumOnlineProject", VPAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "CorumOnlineProject - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MT /W4 /Gm /Gi /GX /Zi /Ot /Oa /Ow /Oi /Oy /Ob2 /Gf /Gy /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /c
# SUBTRACT CPP /WX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 libcmtd.lib libc.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib imm32.lib Winmm.lib WS2_32.lib d3d8.lib /nologo /subsystem:windows /machine:I386
# SUBTRACT LINK32 /incremental:yes /map /debug

!ELSEIF  "$(CFG)" == "CorumOnlineProject - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /GX /ZI /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_FOR_CORUM" /D "__PACKAGE_MODE__" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib imm32.lib Winmm.lib WS2_32.lib d3d8.lib mss32.lib version.lib /nologo /subsystem:windows /map /debug /machine:I386 /nodefaultlib:"LIBC.lib" /out:".\debug\CorumOnline\CorumOnline.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "CorumOnlineProject - Win32 dev"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "CorumOnlineProject___Win32_dev"
# PROP BASE Intermediate_Dir "CorumOnlineProject___Win32_dev"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "CorumOnlineProject___Win32_dev"
# PROP Intermediate_Dir "CorumOnlineProject___Win32_dev"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /GX /ZI /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_FOR_CORUM" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W4 /GX /ZI /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_FOR_CORUM" /D "GM_TOOL" /D "DEVELOP_MODE" /FR /YX /FD /GZ /c
# SUBTRACT CPP /WX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib imm32.lib Winmm.lib WS2_32.lib d3d8.lib mss32.lib /nologo /subsystem:windows /map /debug /machine:I386 /out:"Debug/CorumOnline.exe" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib imm32.lib Winmm.lib WS2_32.lib d3d8.lib mss32.lib CommonServer.lib /nologo /subsystem:windows /map /debug /machine:I386 /nodefaultlib:"LIBC.lib" /out:"Debug/CorumOnline.exe" /pdbtype:sept /libpath:"../CommonServer"

!ELSEIF  "$(CFG)" == "CorumOnlineProject - Win32 JAPAN"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "CorumOnlineProject___Win32_JAPAN"
# PROP BASE Intermediate_Dir "CorumOnlineProject___Win32_JAPAN"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "CorumOnlineProject___Win32_JAPAN"
# PROP Intermediate_Dir "CorumOnlineProject___Win32_JAPAN"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /GX /ZI /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_FOR_CORUM" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /GX /ZI /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_FOR_CORUM" /D "JAPAN_LOCALIZING" /D "__PACKET_ENCRYPTION" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib imm32.lib Winmm.lib WS2_32.lib d3d8.lib mss32.lib /nologo /subsystem:windows /map /debug /machine:I386 /out:"Debug/CorumOnline.exe" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib imm32.lib Winmm.lib WS2_32.lib d3d8.lib mss32.lib commonserver.lib /nologo /subsystem:windows /map /debug /machine:I386 /nodefaultlib:"libc.lib" /out:".\JAPAN\CorumOnline.exe" /pdbtype:sept /libpath:"..\commonserver\\"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "CorumOnlineProject - Win32 JAPAN_DEV"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "CorumOnlineProject___Win32_JAPAN_DEV"
# PROP BASE Intermediate_Dir "CorumOnlineProject___Win32_JAPAN_DEV"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "CorumOnlineProject___Win32_JAPAN_DEV"
# PROP Intermediate_Dir "CorumOnlineProject___Win32_JAPAN_DEV"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /GX /ZI /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_FOR_CORUM" /D "GM_TOOL" /D "DEVELOP_MODE" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /GX /ZI /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_FOR_CORUM" /D "GM_TOOL" /D "DEVELOP_MODE" /D "JAPAN_LOCALIZING" /D "__PACKET_ENCRYPTION" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib imm32.lib Winmm.lib WS2_32.lib d3d8.lib mss32.lib /nologo /subsystem:windows /map /debug /machine:I386 /out:"Debug/CorumOnline.exe" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib imm32.lib Winmm.lib WS2_32.lib d3d8.lib mss32.lib commonserver.lib /nologo /subsystem:windows /map /debug /machine:I386 /nodefaultlib:"libc.lib" /out:".\japan\CorumOnlineJJD.exe" /pdbtype:sept /libpath:"..\commonserver\\"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "CorumOnlineProject - Win32 CHINA"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "CorumOnlineProject___Win32_CHINA"
# PROP BASE Intermediate_Dir "CorumOnlineProject___Win32_CHINA"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "CorumOnlineProject___Win32_CHINA"
# PROP Intermediate_Dir "CorumOnlineProject___Win32_CHINA"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /GX /ZI /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_FOR_CORUM" /FR /YX /FD /GZ /c
# ADD CPP /nologo /W3 /GX /ZI /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_FOR_CORUM" /D "CHINA_LOCALIZING" /D "__PACKAGE_MODE__" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib imm32.lib Winmm.lib WS2_32.lib d3d8.lib mss32.lib /nologo /subsystem:windows /map /debug /machine:I386 /out:"Debug/CorumOnline.exe" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib imm32.lib Winmm.lib WS2_32.lib d3d8.lib mss32.lib commonserver.lib /nologo /subsystem:windows /map /debug /machine:I386 /out:"Debug/CorumOnline.exe" /pdbtype:sept /libpath:"..\commonserver\\"

!ELSEIF  "$(CFG)" == "CorumOnlineProject - Win32 CHINA_DEV"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "CorumOnlineProject___Win32_CHINA_DEV"
# PROP BASE Intermediate_Dir "CorumOnlineProject___Win32_CHINA_DEV"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "CorumOnlineProject___Win32_CHINA_DEV"
# PROP Intermediate_Dir "CorumOnlineProject___Win32_CHINA_DEV"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /GX /ZI /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_FOR_CORUM" /D "GM_TOOL" /D "DEVELOP_MODE" /FR /YX /FD /GZ /c
# ADD CPP /nologo /W3 /GX /ZI /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_FOR_CORUM" /D "GM_TOOL" /D "DEVELOP_MODE" /D "CHINA_LOCALIZING" /D "__WITH_EXCEPTION_FILTER" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib imm32.lib Winmm.lib WS2_32.lib d3d8.lib mss32.lib /nologo /subsystem:windows /map /debug /machine:I386 /out:"Debug/CorumOnline.exe" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib imm32.lib Winmm.lib WS2_32.lib d3d8.lib mss32.lib commonserver.lib /nologo /subsystem:windows /map /debug /machine:I386 /out:"Debug/CorumOnline.exe" /pdbtype:sept /libpath:"..\commonserver\\"

!ELSEIF  "$(CFG)" == "CorumOnlineProject - Win32 TAIWAN"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "CorumOnlineProject___Win32_TAIWAN"
# PROP BASE Intermediate_Dir "CorumOnlineProject___Win32_TAIWAN"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "CorumOnlineProject___Win32_TAIWAN"
# PROP Intermediate_Dir "CorumOnlineProject___Win32_TAIWAN"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /GX /ZI /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_FOR_CORUM" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /GX /ZI /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_FOR_CORUM" /D "TAIWAN_LOCALIZING" /D "__PACKAGE_MODE__" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x404 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib imm32.lib Winmm.lib WS2_32.lib d3d8.lib mss32.lib /nologo /subsystem:windows /map /debug /machine:I386 /out:"Debug/CorumOnline.exe" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib imm32.lib Winmm.lib WS2_32.lib d3d8.lib mss32.lib commonserver.lib /nologo /subsystem:windows /map /debug /machine:I386 /nodefaultlib:"libc.lib" /out:"Debug/CorumOnline.exe" /pdbtype:sept /libpath:"..\commonserver\\"

!ELSEIF  "$(CFG)" == "CorumOnlineProject - Win32 TAIWAN_DEV"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "CorumOnlineProject___Win32_TAIWAN_DEV"
# PROP BASE Intermediate_Dir "CorumOnlineProject___Win32_TAIWAN_DEV"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "CorumOnlineProject___Win32_TAIWAN_DEV"
# PROP Intermediate_Dir "CorumOnlineProject___Win32_TAIWAN_DEV"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /GX /ZI /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_FOR_CORUM" /D "GM_TOOL" /D "DEVELOP_MODE" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /GX /ZI /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_FOR_CORUM" /D "GM_TOOL" /D "DEVELOP_MODE" /D "TAIWAN_LOCALIZING" /D "__PACKET_ENCRYPTION" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x404 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib imm32.lib Winmm.lib WS2_32.lib d3d8.lib mss32.lib /nologo /subsystem:windows /map /debug /machine:I386 /out:"Debug/CorumOnline.exe" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib imm32.lib Winmm.lib WS2_32.lib d3d8.lib mss32.lib commonserver.lib /nologo /subsystem:windows /map /debug /machine:I386 /nodefaultlib:"LIBC.lib" /out:"D:\corum\CorumOnline TW\CorumOnline.exe" /pdbtype:sept /libpath:"..\commonserver\\"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "CorumOnlineProject - Win32 JAPAN_ENCRYPE"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "CorumOnlineProject___Win32_JAPAN_ENCRYPE"
# PROP BASE Intermediate_Dir "CorumOnlineProject___Win32_JAPAN_ENCRYPE"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "CorumOnlineProject___Win32_JAPAN_ENCRYPE"
# PROP Intermediate_Dir "CorumOnlineProject___Win32_JAPAN_ENCRYPE"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /GX /ZI /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_FOR_CORUM" /D "GM_TOOL" /D "DEVELOP_MODE" /D "JAPAN_LOCALIZING" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /GX /ZI /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "__PACKAGE_MODE__" /D "__PACKET_ENCRYPTION" /D "GM_TOOL" /D "DEVELOP_MODE" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib imm32.lib Winmm.lib WS2_32.lib d3d8.lib mss32.lib /nologo /subsystem:windows /map /debug /machine:I386 /out:"Debug/CorumOnline.exe" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib imm32.lib Winmm.lib WS2_32.lib d3d8.lib mss32.lib commonserver.lib /nologo /subsystem:windows /map /debug /machine:I386 /out:"Debug/CorumOnline.exe" /pdbtype:sept /libpath:"..\commonserver\\"

!ELSEIF  "$(CFG)" == "CorumOnlineProject - Win32 dev_NEW_COMMAND"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "CorumOnlineProject___Win32_dev_NEW_COMMAND0"
# PROP BASE Intermediate_Dir "CorumOnlineProject___Win32_dev_NEW_COMMAND0"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "CorumOnlineProject___Win32_develop_Test"
# PROP Intermediate_Dir "CorumOnlineProject___Win32_develop_Test"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /WX /GX /ZI /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_FOR_CORUM" /D "GM_TOOL" /D "DEVELOP_MODE" /FR /YX /FD /GZ /c
# ADD CPP /nologo /W3 /WX /GX /ZI /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_FOR_CORUM" /D "GM_TOOL" /D "DEVELOP_MODE" /D "NEW_COMMAND" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib imm32.lib Winmm.lib WS2_32.lib d3d8.lib mss32.lib /nologo /subsystem:windows /map /debug /machine:I386 /nodefaultlib:"LIBC.lib" /out:"Debug/CorumOnline.exe" /pdbtype:sept
# ADD LINK32 imm32.lib Winmm.lib WS2_32.lib d3d8.lib mss32.lib commonserver.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /map /debug /machine:I386 /nodefaultlib:"LIBC.lib" /out:"Debug/CorumOnline.exe" /pdbtype:sept /libpath:"..\commonserver\\"

!ELSEIF  "$(CFG)" == "CorumOnlineProject - Win32 develop_Test"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "CorumOnlineProject___Win32_develop_Test"
# PROP BASE Intermediate_Dir "CorumOnlineProject___Win32_develop_Test"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "CorumOnlineProject___Win32_develop_Test"
# PROP Intermediate_Dir "CorumOnlineProject___Win32_develop_Test"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /WX /GX /ZI /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_FOR_CORUM" /D "GM_TOOL" /D "DEVELOP_MODE" /D "NEW_COMMAND" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /WX /GX /ZI /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_FOR_CORUM" /D "GM_TOOL" /D "DEVELOP_MODE" /D "__PACKAGE_MODE__" /D "NEW_COMMAND" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib imm32.lib Winmm.lib WS2_32.lib d3d8.lib mss32.lib commonserver.lib /nologo /subsystem:windows /map /debug /machine:I386 /nodefaultlib:"LIBC.lib" /out:"Debug/CorumOnline.exe" /pdbtype:sept /libpath:"..\commonserver\\"
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib imm32.lib Winmm.lib WS2_32.lib d3d8.lib mss32.lib commonserver.lib /nologo /subsystem:windows /map /debug /machine:I386 /nodefaultlib:"LIBC.lib" /out:"CorumOnlineProject___Win32_develop_Test/CorumOnline.exe" /pdbtype:sept /libpath:"..\commonserver\\"

!ELSEIF  "$(CFG)" == "CorumOnlineProject - Win32 Korea_GMMode"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "CorumOnlineProject___Win32_Korea_GMMode"
# PROP BASE Intermediate_Dir "CorumOnlineProject___Win32_Korea_GMMode"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "CorumOnlineProject___Win32_Korea_GMMode"
# PROP Intermediate_Dir "CorumOnlineProject___Win32_Korea_GMMode"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W4 /GX /ZI /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_FOR_CORUM" /D "__PACKAGE_MODE__" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /GX /ZI /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_FOR_CORUM" /D "GM_TOOL" /D "DEVELOP_MODE" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib imm32.lib Winmm.lib WS2_32.lib d3d8.lib mss32.lib version.lib /nologo /subsystem:windows /map /debug /machine:I386 /nodefaultlib:"LIBC.lib" /out:"Debug/CorumOnline.exe" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib imm32.lib Winmm.lib WS2_32.lib d3d8.lib mss32.lib version.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib imm32.lib Winmm.lib WS2_32.lib d3d8.lib mss32.lib version.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib imm32.lib Winmm.lib WS2_32.lib d3d8.lib mss32.lib version.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib imm32.lib Winmm.lib WS2_32.lib d3d8.lib mss32.lib version.lib /nologo /subsystem:windows /map /debug /machine:I386 /nodefaultlib:"LIBC.lib" /out:".\debug\CorumOnlineWindow.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "CorumOnlineProject - Win32 EngineTest"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "CorumOnlineProject___Win32_EngineTest"
# PROP BASE Intermediate_Dir "CorumOnlineProject___Win32_EngineTest"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "CorumOnlineProject___Win32_EngineTest"
# PROP Intermediate_Dir "CorumOnlineProject___Win32_EngineTest"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /GX /ZI /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_FOR_CORUM" /D "GM_TOOL" /D "DEVELOP_MODE" /FR /YX /FD /GZ /c
# ADD CPP /nologo /G6 /MDd /W3 /Gi /GX /ZI /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_FOR_CORUM" /D "GM_TOOL" /D "DEVELOP_MODE" /D "SS3D_TEST" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib imm32.lib Winmm.lib WS2_32.lib d3d8.lib mss32.lib version.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib imm32.lib Winmm.lib WS2_32.lib d3d8.lib mss32.lib version.lib /nologo /subsystem:windows /map /debug /machine:I386 /nodefaultlib:"LIBC.lib" /out:"C:\Program Files\eSofNet\CorumOnlineTest\CorumOnlineWindow.exe" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib imm32.lib Winmm.lib WS2_32.lib d3d8.lib mss32.lib version.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib imm32.lib Winmm.lib WS2_32.lib d3d8.lib mss32.lib version.lib /nologo /subsystem:windows /map /debug /machine:I386 /nodefaultlib:"LIBC.lib" /out:"C:\Program Files\eSofNet\CorumOnlineTest\CorumOnlineEngineTest.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "CorumOnlineProject - Win32 Release"
# Name "CorumOnlineProject - Win32 Debug"
# Name "CorumOnlineProject - Win32 dev"
# Name "CorumOnlineProject - Win32 JAPAN"
# Name "CorumOnlineProject - Win32 JAPAN_DEV"
# Name "CorumOnlineProject - Win32 CHINA"
# Name "CorumOnlineProject - Win32 CHINA_DEV"
# Name "CorumOnlineProject - Win32 TAIWAN"
# Name "CorumOnlineProject - Win32 TAIWAN_DEV"
# Name "CorumOnlineProject - Win32 JAPAN_ENCRYPE"
# Name "CorumOnlineProject - Win32 dev_NEW_COMMAND"
# Name "CorumOnlineProject - Win32 develop_Test"
# Name "CorumOnlineProject - Win32 Korea_GMMode"
# Name "CorumOnlineProject - Win32 EngineTest"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ChrInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\CodeFun.cpp
# End Source File
# Begin Source File

SOURCE=.\Command.cpp
# End Source File
# Begin Source File

SOURCE=.\crBaseMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\crButton.cpp
# End Source File
# Begin Source File

SOURCE=.\crDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\crImage.cpp
# End Source File
# Begin Source File

SOURCE=.\crImageList.cpp
# End Source File
# Begin Source File

SOURCE=.\crListBox.cpp
# End Source File
# Begin Source File

SOURCE=.\crListBoxItemInterface.cpp
# End Source File
# Begin Source File

SOURCE=.\crProxyMenuToDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\crStaticText.cpp
# End Source File
# Begin Source File

SOURCE=.\crtList.cpp
# End Source File
# Begin Source File

SOURCE=.\crtNode.cpp
# End Source File
# Begin Source File

SOURCE=.\DamageNumber.cpp
# End Source File
# Begin Source File

SOURCE=.\DebugLog.cpp
# End Source File
# Begin Source File

SOURCE=.\DUNGEON_DATA.cpp
# End Source File
# Begin Source File

SOURCE=.\DUNGEON_DATA_EX.cpp
# End Source File
# Begin Source File

SOURCE=.\DungeonTable.cpp
# End Source File
# Begin Source File

SOURCE=.\GameControl.cpp
# End Source File
# Begin Source File

SOURCE=.\GuildWarFinalSettingWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\GuildWarInvite.cpp
# End Source File
# Begin Source File

SOURCE=.\GuildwarRequest.cpp
# End Source File
# Begin Source File

SOURCE=.\GuildWarStatusWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\GxDefault.cpp
# End Source File
# Begin Source File

SOURCE=.\IdentyGraphicCard.cpp
# End Source File
# Begin Source File

SOURCE=.\IMEEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\InitGame.cpp
# End Source File
# Begin Source File

SOURCE=.\InputManager.cpp
# End Source File
# Begin Source File

SOURCE=.\InterfaceInstance.cpp
# End Source File
# Begin Source File

SOURCE=.\LocalTimeObserver.cpp
# End Source File
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# Begin Source File

SOURCE=.\Map.cpp
# End Source File
# Begin Source File

SOURCE=.\MatchInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Monster.cpp
# End Source File
# Begin Source File

SOURCE=.\MonsterManager.cpp
# End Source File
# Begin Source File

SOURCE=.\MouseLib.cpp
# End Source File
# Begin Source File

SOURCE=.\MouseLib.h
# End Source File
# Begin Source File

SOURCE=.\NetworkClient.cpp
# End Source File
# Begin Source File

SOURCE=.\NetworkTimeObserver.cpp
# End Source File
# Begin Source File

SOURCE=.\ObjectManager.cpp
# End Source File
# Begin Source File

SOURCE=.\parsing.cpp
# End Source File
# Begin Source File

SOURCE=.\PartyMatchingWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\RivalGuildWarUI.cpp
# End Source File
# Begin Source File

SOURCE=.\Script.cpp
# End Source File
# Begin Source File

SOURCE=.\SpriteManager.cpp
# End Source File
# Begin Source File

SOURCE=.\system_info.cpp
# End Source File
# Begin Source File

SOURCE=.\TeamMatch.cpp
# End Source File
# Begin Source File

SOURCE=.\TokenCommand.cpp
# End Source File
# Begin Source File

SOURCE=.\User.cpp
# End Source File
# Begin Source File

SOURCE=.\UserHash.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BaseNetworkInterface.h
# End Source File
# Begin Source File

SOURCE=.\ChrInfo.h
# End Source File
# Begin Source File

SOURCE=.\cmd_message.h
# End Source File
# Begin Source File

SOURCE=.\CodeFun.h
# End Source File
# Begin Source File

SOURCE=.\Command.h
# End Source File
# Begin Source File

SOURCE=.\crBaseMenu.h
# End Source File
# Begin Source File

SOURCE=.\crBaseMenu.inl
# End Source File
# Begin Source File

SOURCE=.\crButton.h
# End Source File
# Begin Source File

SOURCE=.\crDialog.h
# End Source File
# Begin Source File

SOURCE=.\CResNameResolver.h
# End Source File
# Begin Source File

SOURCE=.\crImage.h
# End Source File
# Begin Source File

SOURCE=.\crImageList.h
# End Source File
# Begin Source File

SOURCE=.\crListBox.h
# End Source File
# Begin Source File

SOURCE=.\crListBoxItemInterface.h
# End Source File
# Begin Source File

SOURCE=.\crProxyMenuToDialog.h
# End Source File
# Begin Source File

SOURCE=.\crStaticText.h
# End Source File
# Begin Source File

SOURCE=.\crtList.h
# End Source File
# Begin Source File

SOURCE=.\crtNode.h
# End Source File
# Begin Source File

SOURCE=.\DamageNumber.h
# End Source File
# Begin Source File

SOURCE=.\DebugLog.h
# End Source File
# Begin Source File

SOURCE=.\Define.h
# End Source File
# Begin Source File

SOURCE=.\Develop.h
# End Source File
# Begin Source File

SOURCE=.\DUNGEON_DATA.h
# End Source File
# Begin Source File

SOURCE=.\DUNGEON_DATA_EX.h
# End Source File
# Begin Source File

SOURCE=.\DungeonTable.h
# End Source File
# Begin Source File

SOURCE=.\EffectInfo.h
# End Source File
# Begin Source File

SOURCE=.\GameControl.h
# End Source File
# Begin Source File

SOURCE=.\GameDefine.h
# End Source File
# Begin Source File

SOURCE=.\GetDxVer.h
# End Source File
# Begin Source File

SOURCE=.\GuildWarFinalSettingWnd.h
# End Source File
# Begin Source File

SOURCE=.\GuildWarInvite.h
# End Source File
# Begin Source File

SOURCE=.\GuildwarRequest.h
# End Source File
# Begin Source File

SOURCE=.\GuildWarStatusWnd.h
# End Source File
# Begin Source File

SOURCE=.\GXDefault.h
# End Source File
# Begin Source File

SOURCE=.\IdentyGraphicCard.h
# End Source File
# Begin Source File

SOURCE=.\IMEEdit.h
# End Source File
# Begin Source File

SOURCE=.\InitGame.h
# End Source File
# Begin Source File

SOURCE=.\InputManager.h
# End Source File
# Begin Source File

SOURCE=.\InterfaceInstance.h
# End Source File
# Begin Source File

SOURCE=.\Localizing.h
# End Source File
# Begin Source File

SOURCE=.\LocalTimeObserver.h
# End Source File
# Begin Source File

SOURCE=.\LogWnd.h
# End Source File
# Begin Source File

SOURCE=.\Map.h
# End Source File
# Begin Source File

SOURCE=.\MatchInfo.h
# End Source File
# Begin Source File

SOURCE=.\message.h
# End Source File
# Begin Source File

SOURCE=.\Monster.h
# End Source File
# Begin Source File

SOURCE=.\MonsterManager.h
# End Source File
# Begin Source File

SOURCE=.\NetworkClient.h
# End Source File
# Begin Source File

SOURCE=.\NetworkTimeObserver.h
# End Source File
# Begin Source File

SOURCE=.\Npc.h
# End Source File
# Begin Source File

SOURCE=.\ObjectDefine.h
# End Source File
# Begin Source File

SOURCE=.\ObjectManager.h
# End Source File
# Begin Source File

SOURCE=.\Parsing.h
# End Source File
# Begin Source File

SOURCE=.\PartyMatchingWnd.h
# End Source File
# Begin Source File

SOURCE=.\RivalGuildWarUI.h
# End Source File
# Begin Source File

SOURCE=.\Script.h
# End Source File
# Begin Source File

SOURCE=.\special_effect.h
# End Source File
# Begin Source File

SOURCE=.\SpriteManager.h
# End Source File
# Begin Source File

SOURCE=.\StaticText.h
# End Source File
# Begin Source File

SOURCE=.\stdafx.h
# End Source File
# Begin Source File

SOURCE=.\struct.h
# End Source File
# Begin Source File

SOURCE=.\system_info.h
# End Source File
# Begin Source File

SOURCE=.\TokenCommand.h
# End Source File
# Begin Source File

SOURCE=.\User.h
# End Source File
# Begin Source File

SOURCE=.\UserHash.h
# End Source File
# Begin Source File

SOURCE=.\Version.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\corum.ico
# End Source File
# Begin Source File

SOURCE=.\Data\Cursor\m_attack.cur
# End Source File
# Begin Source File

SOURCE=.\Data\Cursor\m_attack_c.cur
# End Source File
# Begin Source File

SOURCE=.\Data\Cursor\m_button.cur
# End Source File
# Begin Source File

SOURCE=.\Data\Cursor\m_button_c.cur
# End Source File
# Begin Source File

SOURCE=.\Data\Cursor\m_common.cur
# End Source File
# Begin Source File

SOURCE=.\Data\Cursor\m_common_c.cur
# End Source File
# Begin Source File

SOURCE=.\Data\Cursor\m_dialog.cur
# End Source File
# Begin Source File

SOURCE=.\Data\Cursor\m_dialog_c.cur
# End Source File
# Begin Source File

SOURCE=.\Data\Cursor\m_item.cur
# End Source File
# Begin Source File

SOURCE=.\Data\Cursor\m_item_c.cur
# End Source File
# Begin Source File

SOURCE=.\Data\Cursor\m_target.cur
# End Source File
# Begin Source File

SOURCE=.\Data\Cursor\m_target_c.cur
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\Resource.rc
# End Source File
# End Group
# Begin Group "Intro"

# PROP Default_Filter "cpp;h;inl"
# Begin Source File

SOURCE=.\IntroProcess.cpp
# End Source File
# Begin Source File

SOURCE=.\IntroProcess.h
# End Source File
# End Group
# Begin Group "Login"

# PROP Default_Filter "cpp;h;inl"
# Begin Source File

SOURCE=.\GlobalVariableLogin.cpp
# End Source File
# Begin Source File

SOURCE=.\GlobalVariableLogin.h
# End Source File
# Begin Source File

SOURCE=.\LoginMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\LoginMsg.h
# End Source File
# Begin Source File

SOURCE=.\LoginProcess.cpp
# End Source File
# Begin Source File

SOURCE=.\LoginProcess.h
# End Source File
# End Group
# Begin Group "CharSelect"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CharSelectMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\CharSelectMsg.h
# End Source File
# Begin Source File

SOURCE=.\CharSelectProcess.cpp
# End Source File
# Begin Source File

SOURCE=.\CharSelectProcess.h
# End Source File
# Begin Source File

SOURCE=.\SpecialCharacterJPN.cpp
# End Source File
# End Group
# Begin Group "World"

# PROP Default_Filter "cpp;h;inl"
# Begin Group "World Msg"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\WorldMsg.h
# End Source File
# Begin Source File

SOURCE=.\WorldMsg_jjw6263.cpp
# End Source File
# Begin Source File

SOURCE=.\WorldMsg_jjw6263.h
# End Source File
# Begin Source File

SOURCE=.\WorldMsg_minbobo.cpp
# End Source File
# Begin Source File

SOURCE=.\WorldMsg_minbobo.h
# End Source File
# Begin Source File

SOURCE=.\WorldMsg_yak.cpp
# End Source File
# Begin Source File

SOURCE=.\WorldMsg_yak.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\WorldProcess.cpp
# End Source File
# Begin Source File

SOURCE=.\WorldProcess.h
# End Source File
# End Group
# Begin Group "Dungeon"

# PROP Default_Filter ""
# Begin Group "Dungeon Msg"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DungeonMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\DungeonMsg.h
# End Source File
# Begin Source File

SOURCE=.\DungeonMsg_deepdark.cpp
# End Source File
# Begin Source File

SOURCE=.\DungeonMsg_deepdark.h
# End Source File
# Begin Source File

SOURCE=.\DungeonMsg_jjw6263.cpp
# End Source File
# Begin Source File

SOURCE=.\DungeonMsg_jjw6263.h
# End Source File
# Begin Source File

SOURCE=.\DungeonMsg_minbobo.cpp
# End Source File
# Begin Source File

SOURCE=.\DungeonMsg_minbobo.h
# End Source File
# Begin Source File

SOURCE=.\DungeonMsg_seven.cpp
# End Source File
# Begin Source File

SOURCE=.\DungeonMsg_seven.h
# End Source File
# Begin Source File

SOURCE=.\DungeonMsg_yak.cpp
# End Source File
# Begin Source File

SOURCE=.\DungeonMsg_yak.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\DungeonProcess.cpp
# End Source File
# Begin Source File

SOURCE=.\DungeonProcess.h
# End Source File
# Begin Source File

SOURCE=.\DungeonProcess2nd.cpp
# End Source File
# Begin Source File

SOURCE=.\DungeonProcess2nd.h
# End Source File
# Begin Source File

SOURCE=.\DungeonProcess_Sound.cpp
# End Source File
# Begin Source File

SOURCE=.\DungeonProcess_Sound.h
# End Source File
# End Group
# Begin Group "PathFinding"

# PROP Default_Filter "cpp;h;inl"
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
# Begin Group "Resource"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\CorumResource.erd
# End Source File
# Begin Source File

SOURCE=.\CorumResource.h
# End Source File
# Begin Source File

SOURCE=.\default.rtl
# End Source File
# Begin Source File

SOURCE=.\default2.rtl
# End Source File
# Begin Source File

SOURCE=.\DefResource.erd
# End Source File
# Begin Source File

SOURCE=.\DefResource.erl
# End Source File
# Begin Source File

SOURCE=.\DefResource.h
# End Source File
# Begin Source File

SOURCE=.\Resource.erl
# End Source File
# Begin Source File

SOURCE=.\resource1.erl
# End Source File
# Begin Source File

SOURCE=.\resource2.erl
# End Source File
# Begin Source File

SOURCE=.\resource3.erl
# End Source File
# End Group
# Begin Group "Effect"

# PROP Default_Filter "cpp;h;inl"
# Begin Source File

SOURCE=.\Effect.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect.h
# End Source File
# Begin Source File

SOURCE=.\EffectCommand.cpp
# End Source File
# Begin Source File

SOURCE=.\EffectCommand.h
# End Source File
# Begin Source File

SOURCE=.\EffectProcess.cpp
# End Source File
# Begin Source File

SOURCE=.\EffectProcess.h
# End Source File
# End Group
# Begin Group "Interface"

# PROP Default_Filter ""
# Begin Group "BaloonChat"

# PROP Default_Filter "cpp;h"
# Begin Source File

SOURCE=.\BaloonChat.cpp
# End Source File
# Begin Source File

SOURCE=.\BaloonChat.h
# End Source File
# End Group
# Begin Group "BlockWnd"

# PROP Default_Filter "cpp;h"
# Begin Source File

SOURCE=.\BlockWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\BlockWnd.h
# End Source File
# End Group
# Begin Group "DungeonInfo Wnd"

# PROP Default_Filter "cpp;h"
# Begin Source File

SOURCE=.\CDungeonInfoWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\CDungeonInfoWnd.h
# End Source File
# End Group
# Begin Group "Character Change"

# PROP Default_Filter "cpp;h"
# Begin Source File

SOURCE=.\CharMoveEditWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\CharMoveEditWnd.h
# End Source File
# Begin Source File

SOURCE=.\CharMoveRtWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\CharMoveRtWnd.h
# End Source File
# Begin Source File

SOURCE=.\CharMoveWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\CharMoveWnd.h
# End Source File
# End Group
# Begin Group "Character Status Wnd"

# PROP Default_Filter "cpp;h"
# Begin Source File

SOURCE=.\CharWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\CharWnd.h
# End Source File
# End Group
# Begin Group "MiniMap Wnd"

# PROP Default_Filter "cpp;h"
# Begin Source File

SOURCE=.\DSMiniMap.cpp
# End Source File
# Begin Source File

SOURCE=.\DSMiniMap.h
# End Source File
# End Group
# Begin Group "Dungeon Siege Info Wnd"

# PROP Default_Filter "cpp;h"
# Begin Source File

SOURCE=.\DungeonSiegeInfoWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\DungeonSiegeInfoWnd.h
# End Source File
# End Group
# Begin Group "Event Dungeon"

# PROP Default_Filter "cpp;h"
# Begin Source File

SOURCE=.\EventDungeonWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\EventDungeonWnd.h
# End Source File
# Begin Source File

SOURCE=.\EventNPCSubWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\EventNPCSubWnd.h
# End Source File
# Begin Source File

SOURCE=.\EventNPCWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\EventNPCWnd.h
# End Source File
# End Group
# Begin Group "Guild"

# PROP Default_Filter "cpp; h"
# Begin Source File

SOURCE=.\GuildDeleteWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\GuildDeleteWnd.h
# End Source File
# Begin Source File

SOURCE=.\GuildEditWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\GuildEditWnd.h
# End Source File
# Begin Source File

SOURCE=.\GuildExWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\GuildExWnd.h
# End Source File
# Begin Source File

SOURCE=.\GuildJoinWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\GuildJoinWnd.h
# End Source File
# Begin Source File

SOURCE=.\GuildMasterWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\GuildMasterWnd.h
# End Source File
# Begin Source File

SOURCE=.\GuildPortalWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\GuildPortalWnd.h
# End Source File
# Begin Source File

SOURCE=.\GuildRankWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\GuildRankWnd.h
# End Source File
# Begin Source File

SOURCE=.\GuildRequestWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\GuildRequestWnd.h
# End Source File
# Begin Source File

SOURCE=.\GuildWarWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\GuildWarWnd.h
# End Source File
# Begin Source File

SOURCE=.\GuildWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\GuildWnd.h
# End Source File
# End Group
# Begin Group "Party"

# PROP Default_Filter "cpp;h"
# Begin Source File

SOURCE=.\PartyBoardInfoWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\PartyBoardInfoWnd.h
# End Source File
# Begin Source File

SOURCE=.\PartyBoardWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\PartyBoardWnd.h
# End Source File
# Begin Source File

SOURCE=.\PartyEntryWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\PartyEntryWnd.h
# End Source File
# Begin Source File

SOURCE=.\PartyHelpWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\PartyHelpWnd.h
# End Source File
# Begin Source File

SOURCE=.\PartyJoinWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\PartyJoinWnd.h
# End Source File
# Begin Source File

SOURCE=.\PartyPortalWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\PartyPortalWnd.h
# End Source File
# Begin Source File

SOURCE=.\PartyRequestWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\PartyRequestWnd.h
# End Source File
# End Group
# Begin Group "Item"

# PROP Default_Filter "cpp;h"
# Begin Source File

SOURCE=.\ItemMakingHelpWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemMakingHelpWnd.h
# End Source File
# Begin Source File

SOURCE=.\ItemMakingSubWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemMakingSubWnd.h
# End Source File
# Begin Source File

SOURCE=.\ItemShopWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemShopWnd.h
# End Source File
# Begin Source File

SOURCE=.\ItemUpgradeHelpWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemUpgradeHelpWnd.h
# End Source File
# Begin Source File

SOURCE=.\ItemUpgradeSubWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemUpgradeSubWnd.h
# End Source File
# End Group
# Begin Group "Login Wnd"

# PROP Default_Filter "cpp;h"
# Begin Source File

SOURCE=.\LoginWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\LoginWnd.h
# End Source File
# End Group
# Begin Group "Match"

# PROP Default_Filter "cpp;h"
# Begin Source File

SOURCE=.\RequestMatchWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\RequestMatchWnd.h
# End Source File
# End Group
# Begin Group "Skill Wnd"

# PROP Default_Filter "cpp;h"
# Begin Source File

SOURCE=.\SkillWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\SkillWnd.h
# End Source File
# End Group
# Begin Group "Wnds in Dungeon"

# PROP Default_Filter "cpp;h"
# Begin Source File

SOURCE=.\CharDieWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\CharDieWnd.h
# End Source File
# Begin Source File

SOURCE=.\ChatWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ChatWnd.h
# End Source File
# Begin Source File

SOURCE=.\ErrorWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ErrorWnd.h
# End Source File
# Begin Source File

SOURCE=.\ExitWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ExitWnd.h
# End Source File
# Begin Source File

SOURCE=.\NoticeWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\NoticeWnd.h
# End Source File
# End Group
# Begin Group "Guandina Status Wnd"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GuardianWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\GuardianWnd.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ChatListWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ChatListWnd.h
# End Source File
# Begin Source File

SOURCE=.\ComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\ComboBox.h
# End Source File
# Begin Source File

SOURCE=.\CommandWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\CommandWnd.h
# End Source File
# Begin Source File

SOURCE=.\DefaultWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\DefaultWnd.h
# End Source File
# Begin Source File

SOURCE=.\GameDefaultWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\GameDefaultWnd.h
# End Source File
# Begin Source File

SOURCE=.\GameMenuWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\GameMenuWnd.h
# End Source File
# Begin Source File

SOURCE=.\GameNoticeWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\GameNoticeWnd.h
# End Source File
# Begin Source File

SOURCE=.\GroupWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\GroupWnd.h
# End Source File
# Begin Source File

SOURCE=.\HelpWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\HelpWnd.h
# End Source File
# Begin Source File

SOURCE=.\InitItemWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\InitItemWnd.h
# End Source File
# Begin Source File

SOURCE=.\Interface.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface.h
# End Source File
# Begin Source File

SOURCE=.\Menu.cpp
# End Source File
# Begin Source File

SOURCE=.\Menu.h
# End Source File
# Begin Source File

SOURCE=.\MessengerDeleteWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\MessengerDeleteWnd.h
# End Source File
# Begin Source File

SOURCE=.\MessengerWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\MessengerWnd.h
# End Source File
# Begin Source File

SOURCE=.\NpcDungeonInfoWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\NpcDungeonInfoWnd.h
# End Source File
# Begin Source File

SOURCE=.\OkWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\OkWnd.h
# End Source File
# Begin Source File

SOURCE=.\RankNpcWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\RankNpcWnd.h
# End Source File
# Begin Source File

SOURCE=.\RankWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\RankWnd.h
# End Source File
# Begin Source File

SOURCE=.\TopDungeonHelpWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\TopDungeonHelpWnd.h
# End Source File
# Begin Source File

SOURCE=.\WorldWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\WorldWnd.h
# End Source File
# End Group
# Begin Group "GMTool"

# PROP Default_Filter "cpp;h;inl"
# Begin Source File

SOURCE=.\GMTool.cpp
# End Source File
# Begin Source File

SOURCE=.\GMTool.h
# End Source File
# End Group
# Begin Group "sound"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GlobalSoundHandles.cpp
# End Source File
# Begin Source File

SOURCE=.\GlobalSoundHandles.h
# End Source File
# Begin Source File

SOURCE=.\MSoundControl.cpp
# End Source File
# Begin Source File

SOURCE=.\MSoundControl.h
# End Source File
# Begin Source File

SOURCE=.\MSoundManager.cpp
# End Source File
# Begin Source File

SOURCE=.\MSoundManager.h
# End Source File
# Begin Source File

SOURCE=.\SoundControl.cpp
# End Source File
# Begin Source File

SOURCE=.\SoundControl.h
# End Source File
# Begin Source File

SOURCE=.\SoundLib.dll
# End Source File
# Begin Source File

SOURCE=.\SoundLib.h
# End Source File
# Begin Source File

SOURCE=.\SoundManager.h
# End Source File
# Begin Source File

SOURCE=.\SoundLib.lib
# End Source File
# End Group
# Begin Group "Minbobo"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Chat.cpp
# End Source File
# Begin Source File

SOURCE=.\Chat.h
# End Source File
# Begin Source File

SOURCE=.\Guild.cpp
# End Source File
# Begin Source File

SOURCE=.\Guild.h
# End Source File
# Begin Source File

SOURCE=.\Ini.cpp
# End Source File
# Begin Source File

SOURCE=.\Ini.h
# End Source File
# Begin Source File

SOURCE=.\Messenger.cpp
# End Source File
# Begin Source File

SOURCE=.\Messenger.h
# End Source File
# Begin Source File

SOURCE=.\MinboboWork.cpp
# End Source File
# Begin Source File

SOURCE=.\MinboboWork.h
# End Source File
# Begin Source File

SOURCE=.\Party.cpp
# End Source File
# Begin Source File

SOURCE=.\Party.h
# End Source File
# Begin Source File

SOURCE=.\Shop.cpp
# End Source File
# Begin Source File

SOURCE=.\Shop.h
# End Source File
# Begin Source File

SOURCE=.\Trade.cpp
# End Source File
# Begin Source File

SOURCE=.\Trade.h
# End Source File
# End Group
# Begin Group "SS3DEngine"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SS3DExecutiveForCorum.dll
# End Source File
# Begin Source File

SOURCE=.\SS3DFileStorage.dll
# End Source File
# Begin Source File

SOURCE=.\SS3DGeometryForCorum.dll
# End Source File
# Begin Source File

SOURCE=.\SS3DGFunc.dll
# End Source File
# Begin Source File

SOURCE=.\SS3DGFuncN.dll
# End Source File
# Begin Source File

SOURCE=.\SS3DGFuncSSE.dll
# End Source File
# Begin Source File

SOURCE=.\SS3DRendererForCorum.dll
# End Source File
# Begin Source File

SOURCE=.\SS3DGFunc.lib
# End Source File
# End Group
# Begin Group "Netmarble"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\NMCrypt.h
# End Source File
# Begin Source File

SOURCE=.\NMCrypt.lib
# End Source File
# End Group
# Begin Group "Interface Sprite"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\InterfaceSpr.cpp
# End Source File
# Begin Source File

SOURCE=.\InterfaceSpr.h
# End Source File
# Begin Source File

SOURCE=.\InterfaceSprDef.cpp
# End Source File
# Begin Source File

SOURCE=.\InterfaceSprDef.h
# End Source File
# End Group
# Begin Group "DeepDark"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ChatBackground.cpp
# End Source File
# Begin Source File

SOURCE=.\ChatBackground.h
# End Source File
# Begin Source File

SOURCE=.\ChinaHackToolBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\ChinaHackToolBlock.h
# End Source File
# Begin Source File

SOURCE=.\ClientTrafficLog.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientTrafficLog.h
# End Source File
# Begin Source File

SOURCE=.\DFXForm.cpp
# End Source File
# Begin Source File

SOURCE=.\DFXForm.h
# End Source File
# Begin Source File

SOURCE=.\Filter.cpp
# End Source File
# Begin Source File

SOURCE=.\Filter.h
# End Source File
# Begin Source File

SOURCE=.\MessagePool.cpp
# End Source File
# Begin Source File

SOURCE=.\MessagePool.h
# End Source File
# Begin Source File

SOURCE=.\NativeOsCheck.cpp
# End Source File
# Begin Source File

SOURCE=.\PacketEncrypt.cpp
# End Source File
# Begin Source File

SOURCE=.\PacketEncrypt.h
# End Source File
# Begin Source File

SOURCE=.\SafeMemory.h
# End Source File
# Begin Source File

SOURCE=.\StringFunc.h
# End Source File
# End Group
# Begin Group "Seven"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SevenWork.cpp
# End Source File
# Begin Source File

SOURCE=.\SevenWork.h
# End Source File
# End Group
# Begin Group "JJW6263"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\EventDungeonProcess.cpp
# End Source File
# Begin Source File

SOURCE=.\EventDungeonProcess.h
# End Source File
# End Group
# Begin Group "Hash & List"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\HashTable.cpp
# End Source File
# Begin Source File

SOURCE=.\HashTable.h
# End Source File
# Begin Source File

SOURCE=.\ListTable.cpp
# End Source File
# Begin Source File

SOURCE=.\ListTable.h
# End Source File
# Begin Source File

SOURCE=.\OnlyList.cpp
# End Source File
# Begin Source File

SOURCE=.\OnlyList.h
# End Source File
# Begin Source File

SOURCE=.\Voidlist.cpp
# End Source File
# Begin Source File

SOURCE=.\VoidList.h
# End Source File
# End Group
# Begin Group "ChinaBilling"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ChinaBillingRemainTime.cpp
# End Source File
# Begin Source File

SOURCE=.\ChinaBillingRemainTime.h
# End Source File
# End Group
# Begin Group "ItemNative"

# PROP Default_Filter ""
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

SOURCE=.\ItemUsed.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemUsed.h
# End Source File
# Begin Source File

SOURCE=.\ItemUsedManager.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemUsedManager.h
# End Source File
# End Group
# Begin Group "ItemNativeWnd"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BankWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\BankWnd.h
# End Source File
# Begin Source File

SOURCE=.\CDungeonManagerWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\CDungeonManagerWnd.h
# End Source File
# Begin Source File

SOURCE=.\ItemMakingWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemMakingWnd.h
# End Source File
# Begin Source File

SOURCE=.\ItemTradeShopWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemTradeShopWnd.h
# End Source File
# Begin Source File

SOURCE=.\ItemTradeWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemTradeWnd.h
# End Source File
# Begin Source File

SOURCE=.\ItemUpgrade.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemUpgrade.h
# End Source File
# Begin Source File

SOURCE=.\ItemWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemWnd.h
# End Source File
# Begin Source File

SOURCE=.\PlayerShopWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\PlayerShopWnd.h
# End Source File
# Begin Source File

SOURCE=.\QuantityWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\QuantityWnd.h
# End Source File
# Begin Source File

SOURCE=.\StoreWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\StoreWnd.h
# End Source File
# Begin Source File

SOURCE=.\UserInterface.cpp
# End Source File
# Begin Source File

SOURCE=.\UserInterface.h
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
# Begin Group "minjin"

# PROP Default_Filter "cpp;h"
# Begin Source File

SOURCE=.\InterfaceManager.cpp
# End Source File
# Begin Source File

SOURCE=.\InterfaceManager.h
# End Source File
# Begin Source File

SOURCE=.\InterfaceProcTemp.cpp
# End Source File
# Begin Source File

SOURCE=.\InterfaceProcTemp.h
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

SOURCE=.\CorumOnlineProject.dsp
# End Source File
# Begin Source File

SOURCE=.\CorumVerInfo.ver
# End Source File
# End Target
# End Project
