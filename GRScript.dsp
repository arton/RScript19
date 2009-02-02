# Microsoft Developer Studio Project File - Name="GRScript" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=GRScript - Win32 Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "GRScript.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "GRScript.mak" CFG="GRScript - Win32 Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "GRScript - Win32 Debug" ("Win32 (x86) Dynamic-Link Library" 用)
!MESSAGE "GRScript - Win32 Unicode Debug" ("Win32 (x86) Dynamic-Link Library" 用)
!MESSAGE "GRScript - Win32 Release MinSize" ("Win32 (x86) Dynamic-Link Library" 用)
!MESSAGE "GRScript - Win32 Release MinDependency" ("Win32 (x86) Dynamic-Link Library" 用)
!MESSAGE "GRScript - Win32 Unicode Release MinSize" ("Win32 (x86) Dynamic-Link Library" 用)
!MESSAGE "GRScript - Win32 Unicode Release MinDependency" ("Win32 (x86) Dynamic-Link Library" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "GRScript - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "GDebug"
# PROP Intermediate_Dir "GDebug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "c:\home\RUBY\lib\ruby\1.8\i386-mswin32\win32" /I "C:\home\RUBY-1.9\usr\include\ruby-1.9.1" /I "C:\home\RUBY-1.9\usr\include\ruby-1.9.1\i386-mswin32" /D "_DEBUG" /D "_MBCS" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "IMPORT" /D "NT" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 msvcrt-ruby191.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"GDebug/GRScript19.dll" /pdbtype:sept /libpath:"d:\ruby\lib" /libpath:"C:\home\ruby\lib"
# Begin Custom Build
OutDir=.\GDebug
TargetPath=.\GDebug\GRScript19.dll
InputPath=.\GDebug\GRScript19.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "GRScript - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "DebugU"
# PROP BASE Intermediate_Dir "DebugU"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "GDebugU"
# PROP Intermediate_Dir "GDebugU"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_UNICODE" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "D:\RUBY\lib\ruby\1.8\i386-mswin32" /I "D:\RUBY\lib\ruby\1.8\i386-mswin32\win32" /I "C:\home\RUBY-1.9\usr\include\ruby-1.9.1" /I "C:\home\RUBY-1.9\usr\include\ruby-1.9.1\i386-mswin32" /D "_DEBUG" /D "_UNICODE" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "IMPORT" /D "NT" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 msvcrt-ruby191.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"GDebugU/GRScript19.dll" /pdbtype:sept /libpath:"c:\home\ruby-1.9\usr\lib"
# Begin Custom Build
OutDir=.\GDebugU
TargetPath=.\GDebugU\GRScript19.dll
InputPath=.\GDebugU\GRScript19.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	if "%OS%"=="" goto NOTNT 
	if not "%OS%"=="Windows_NT" goto NOTNT 
	copy "$(TargetPath)" c:\home\ruby-1.9\usr\bin 
	regsvr32 /s /c "c:\home\ruby-1.9\usr\bin\GRScript19.dll" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	copy "$(OutDir)\*.pdb" c:\home\ruby-1.9\usr\bin 
	goto end 
	:NOTNT 
	echo Warning : Windows 95 Unicode DLL 
	:end 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "GRScript - Win32 Release MinSize"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseMinSize"
# PROP BASE Intermediate_Dir "ReleaseMinSize"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "GReleaseMinSize"
# PROP Intermediate_Dir "GReleaseMinSize"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_ATL_DLL" /D "_ATL_MIN_CRT" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O1 /I "D:\RUBY\lib\ruby\1.8\i386-mswin32" /I "D:\RUBY\lib\ruby\1.8\i386-mswin32\win32" /I "C:\home\RUBY-1.9\usr\include\ruby-1.9.1" /I "C:\home\RUBY-1.9\usr\include\ruby-1.9.1\i386-mswin32" /D "NDEBUG" /D "_MBCS" /D "_ATL_DLL" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "IMPORT" /D "NT" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 msvcrt-ruby191.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386 /out:"GReleaseMinSize/GRScript19.dll" /libpath:"d:\ruby\lib" /libpath:"C:\home\ruby\lib"
# Begin Custom Build
OutDir=.\GReleaseMinSize
TargetPath=.\GReleaseMinSize\GRScript19.dll
InputPath=.\GReleaseMinSize\GRScript19.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "GRScript - Win32 Release MinDependency"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseMinDependency"
# PROP BASE Intermediate_Dir "ReleaseMinDependency"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "GReleaseMinDependency"
# PROP Intermediate_Dir "GReleaseMinDependency"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_ATL_STATIC_REGISTRY" /D "_ATL_MIN_CRT" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O1 /I "C:\HOME\RUBY\lib\ruby\1.8\i386-mswin32\win32" /I "C:\home\RUBY-1.9\usr\include\ruby-1.9.1" /I "C:\home\RUBY-1.9\usr\include\ruby-1.9.1\i386-mswin32" /D "NDEBUG" /D "_MBCS" /D "_ATL_STATIC_REGISTRY" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "IMPORT" /D "NT" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 msvcrt-ruby191.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386 /out:"GReleaseMinDependency/GRScript19.dll" /libpath:"C:\home\ruby\lib"
# Begin Custom Build
OutDir=.\GReleaseMinDependency
TargetPath=.\GReleaseMinDependency\GRScript19.dll
InputPath=.\GReleaseMinDependency\GRScript19.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "GRScript - Win32 Unicode Release MinSize"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseUMinSize"
# PROP BASE Intermediate_Dir "ReleaseUMinSize"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "GReleaseUMinSize"
# PROP Intermediate_Dir "GReleaseUMinSize"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_UNICODE" /D "_ATL_DLL" /D "_ATL_MIN_CRT" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O1 /I "D:\RUBY\lib\ruby\1.8\i386-mswin32" /I "D:\RUBY\lib\ruby\1.8\i386-mswin32\win32" /I "C:\home\RUBY-1.9\usr\include\ruby-1.9.1" /I "C:\home\RUBY-1.9\usr\include\ruby-1.9.1\i386-mswin32" /D "NDEBUG" /D "_UNICODE" /D "_ATL_DLL" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "IMPORT" /D "NT" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 msvcrt-ruby191.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386 /out:"GReleaseUMinSize/GRScript19.dll" /libpath:"d:\ruby\lib" /libpath:"C:\home\ruby\lib"
# Begin Custom Build
OutDir=.\GReleaseUMinSize
TargetPath=.\GReleaseUMinSize\GRScript19.dll
InputPath=.\GReleaseUMinSize\GRScript19.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	if "%OS%"=="" goto NOTNT 
	if not "%OS%"=="Windows_NT" goto NOTNT 
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	goto end 
	:NOTNT 
	echo Warning : Windows 95 
	:end 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "GRScript - Win32 Unicode Release MinDependency"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseUMinDependency"
# PROP BASE Intermediate_Dir "ReleaseUMinDependency"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "GReleaseUMinDependency"
# PROP Intermediate_Dir "GReleaseUMinDependency"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_UNICODE" /D "_ATL_STATIC_REGISTRY" /D "_ATL_MIN_CRT" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O1 /I "C:\home\RUBY-1.9\usr\include\ruby-1.9.1" /I "C:\home\RUBY-1.9\usr\include\ruby-1.9.1\i386-mswin32" /D "NDEBUG" /D "_UNICODE" /D "_ATL_STATIC_REGISTRY" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "IMPORT" /D "NT" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 msvcrt-ruby191.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386 /out:"GReleaseUMinDependency/GRScript19.dll" /libpath:"c:\home\ruby-1.9\usr\lib"
# Begin Custom Build
OutDir=.\GReleaseUMinDependency
TargetPath=.\GReleaseUMinDependency\GRScript19.dll
InputPath=.\GReleaseUMinDependency\GRScript19.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	if "%OS%"=="" goto NOTNT 
	if not "%OS%"=="Windows_NT" goto NOTNT 
	copy "$(TargetPath)" c:\home\ruby-1.9\usr\bin 
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	goto end 
	:NOTNT 
	echo Warning : Windows 95 
	:end 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "GRScript - Win32 Debug"
# Name "GRScript - Win32 Unicode Debug"
# Name "GRScript - Win32 Release MinSize"
# Name "GRScript - Win32 Release MinDependency"
# Name "GRScript - Win32 Unicode Release MinSize"
# Name "GRScript - Win32 Unicode Release MinDependency"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\eventsink.cpp
# End Source File
# Begin Source File

SOURCE=.\GlobalRubyScript.cpp
# End Source File
# Begin Source File

SOURCE=.\GRScript.cpp
# End Source File
# Begin Source File

SOURCE=.\GRScript.def
# End Source File
# Begin Source File

SOURCE=.\GRScript.idl
# ADD MTL /tlb ".\GRScript.tlb" /h "GRScript.h" /iid "GRScript_i.c" /Oicf
# End Source File
# Begin Source File

SOURCE=.\GRScript.rc
# End Source File
# Begin Source File

SOURCE=.\ItemDisp.cpp
# End Source File
# Begin Source File

SOURCE=.\RScriptCore.cpp
# End Source File
# Begin Source File

SOURCE=.\RubyObject.cpp
# End Source File
# Begin Source File

SOURCE=.\ScrError.cpp
# End Source File
# Begin Source File

SOURCE=.\STAInitializer.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\win32oleex.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\eventsink.h
# End Source File
# Begin Source File

SOURCE=.\GlobalRubyScript.h
# End Source File
# Begin Source File

SOURCE=.\Initializer.h
# End Source File
# Begin Source File

SOURCE=.\ItemDisp.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\RScriptCore.h
# End Source File
# Begin Source File

SOURCE=.\RubyObject.h
# End Source File
# Begin Source File

SOURCE=.\ScrError.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\Win32Ole.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\GlobalRubyScript.rgs
# End Source File
# Begin Source File

SOURCE=.\icon1.ico
# End Source File
# End Group
# End Target
# End Project
