# Microsoft Developer Studio Project File - Name="RScript" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=RScript - Win32 Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "RScript.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "RScript.mak" CFG="RScript - Win32 Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "RScript - Win32 Debug" ("Win32 (x86) Dynamic-Link Library" 用)
!MESSAGE "RScript - Win32 Unicode Debug" ("Win32 (x86) Dynamic-Link Library" 用)
!MESSAGE "RScript - Win32 Release MinSize" ("Win32 (x86) Dynamic-Link Library" 用)
!MESSAGE "RScript - Win32 Release MinDependency" ("Win32 (x86) Dynamic-Link Library" 用)
!MESSAGE "RScript - Win32 Unicode Release MinSize" ("Win32 (x86) Dynamic-Link Library" 用)
!MESSAGE "RScript - Win32 Unicode Release MinDependency" ("Win32 (x86) Dynamic-Link Library" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "RScript - Win32 Debug"

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
# ADD BASE CPP /nologo /MTd /W3 /Gm /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "C:\Home\RUBY-1.9\usr\include\ruby-1.9.1" /I "C:\Home\RUBY-1.9\usr\include\ruby-1.9.1\i386-mswin32" /D "_DEBUG" /D "_MBCS" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_MERGE_PROXYSTUB" /D "IMPORT" /D "NT" /D "MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD MTL /Oicf
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 msvcrt-ruby191.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /version:1.0 /subsystem:windows /dll /debug /machine:I386 /out:"Debug/RScript19.dll" /pdbtype:sept /libpath:"C:\home\ruby\lib" /libpath:"c:\home\ruby-1.9\usr\lib"
# Begin Custom Build - Registering
OutDir=.\Debug
TargetPath=.\Debug\RScript19.dll
InputPath=.\Debug\RScript19.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "RScript - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "DebugU"
# PROP BASE Intermediate_Dir "DebugU"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugU"
# PROP Intermediate_Dir "DebugU"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_UNICODE" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "C:\Home\RUBY-1.9\usr\include\ruby-1.9.1" /I "C:\Home\RUBY-1.9\usr\include\ruby-1.9.1\i386-mswin32" /D "_DEBUG" /D "_UNICODE" /D "TARGET_IS_NT40_OR_LATER" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_MERGE_PROXYSTUB" /D "IMPORT" /D "NT" /Yu"stdafx.h" /FD /GZ /c
# ADD MTL /Oicf
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 msvcrt-ruby191.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /version:1.0 /subsystem:windows /dll /debug /machine:I386 /out:"DebugU/RScript19.dll" /pdbtype:sept /libpath:"d:\ruby\lib" /libpath:"c:\home\ruby-1.9\usr\lib"
# Begin Custom Build - Registering
OutDir=.\DebugU
TargetPath=.\DebugU\RScript19.dll
InputPath=.\DebugU\RScript19.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	if "%OS%"=="" goto NOTNT 
	if not "%OS%"=="Windows_NT" goto NOTNT 
	copy "$(TargetPath)" "c:\home\ruby-1.9\usr\bin" 
	copy "$(OutDir)\*.pdb" "c:\home\ruby-1.9\usr\bin" 
	regsvr32 /s /c "c:\home\ruby-1.9\usr\bin\RScript19.dll" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	goto end 
	:NOTNT 
	echo Windows 9x doesn't register Unicode DLL 
	:end 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "RScript - Win32 Release MinSize"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseMinSize"
# PROP BASE Intermediate_Dir "ReleaseMinSize"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseMinSize"
# PROP Intermediate_Dir "ReleaseMinSize"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_ATL_DLL" /D "_ATL_MIN_CRT" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O1 /I "C:\Home\RUBY-1.9\usr\include\ruby-1.9.1" /I "C:\Home\RUBY-1.9\usr\include\ruby-1.9.1\i386-mswin32" /D "NDEBUG" /D "_MBCS" /D "_ATL_DLL" /D "TARGET_IS_NT40_OR_LATER" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_MERGE_PROXYSTUB" /D "IMPORT" /D "NT" /D "MBCS" /Yu"stdafx.h" /FD /c
# ADD MTL /Oicf
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 msvcrt-ruby191.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /version:1.0 /subsystem:windows /dll /machine:I386 /out:"ReleaseMinSize/RScript19.dll" /libpath:"C:\home\ruby\lib" /libpath:"c:\home\ruby-1.9\usr\lib"
# Begin Custom Build - Registering
OutDir=.\ReleaseMinSize
TargetPath=.\ReleaseMinSize\RScript19.dll
InputPath=.\ReleaseMinSize\RScript19.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "RScript - Win32 Release MinDependency"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseMinDependency"
# PROP BASE Intermediate_Dir "ReleaseMinDependency"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseMinDependency"
# PROP Intermediate_Dir "ReleaseMinDependency"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_ATL_STATIC_REGISTRY" /D "_ATL_MIN_CRT" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /G6 /MD /W3 /GX /Zi /O1 /I "C:\Home\RUBY-1.9\usr\include\ruby-1.9.1" /I "C:\Home\RUBY-1.9\usr\include\ruby-1.9.1\i386-mswin32" /D "NDEBUG" /D "_MBCS" /D "_ATL_STATIC_REGISTRY" /D "TARGET_IS_NT40_OR_LATER" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_MERGE_PROXYSTUB" /D "IMPORT" /D "NT" /D "MBCS" /Yu"stdafx.h" /FD /c
# ADD MTL /Oicf
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 msvcrt-ruby191.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /version:1.0 /subsystem:windows /dll /debug /machine:I386 /out:"ReleaseMinDependency/RScript19.dll" /libpath:"C:\Home\ruby\lib" /libpath:"c:\home\ruby-1.9\usr\lib"
# Begin Custom Build - Registering
OutDir=.\ReleaseMinDependency
TargetPath=.\ReleaseMinDependency\RScript19.dll
InputPath=.\ReleaseMinDependency\RScript19.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "RScript - Win32 Unicode Release MinSize"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseUMinSize"
# PROP BASE Intermediate_Dir "ReleaseUMinSize"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseUMinSize"
# PROP Intermediate_Dir "ReleaseUMinSize"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_UNICODE" /D "_ATL_DLL" /D "_ATL_MIN_CRT" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O1 /I "C:\Home\RUBY-1.9\usr\include\ruby-1.9.1" /I "C:\Home\RUBY-1.9\usr\include\ruby-1.9.1\i386-mswin32" /D "NDEBUG" /D "_UNICODE" /D "_ATL_DLL" /D "_ATL_MIN_CRT" /D "TARGET_IS_NT40_OR_LATER" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_MERGE_PROXYSTUB" /D "IMPORT" /D "NT" /Yu"stdafx.h" /FD /c
# ADD MTL /Oicf
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 msvcrt-ruby191.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /version:1.0 /subsystem:windows /dll /machine:I386 /out:"ReleaseUMinSize/RScript19.dll" /libpath:"d:\ruby\lib" /libpath:"c:\home\ruby-1.9\usr\lib"
# Begin Custom Build - Registering
OutDir=.\ReleaseUMinSize
TargetPath=.\ReleaseUMinSize\RScript19.dll
InputPath=.\ReleaseUMinSize\RScript19.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	if "%OS%"=="" goto NOTNT 
	if not "%OS%"=="Windows_NT" goto NOTNT 
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	goto end 
	:NOTNT 
	echo Windows 9x doesn't register Unicode DLL 
	:end 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "RScript - Win32 Unicode Release MinDependency"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseUMinDependency"
# PROP BASE Intermediate_Dir "ReleaseUMinDependency"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseUMinDependency"
# PROP Intermediate_Dir "ReleaseUMinDependency"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_UNICODE" /D "_ATL_STATIC_REGISTRY" /D "_ATL_MIN_CRT" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O1 /I "C:\Home\RUBY-1.9\usr\include\ruby-1.9.1" /I "C:\Home\RUBY-1.9\usr\include\ruby-1.9.1\i386-mswin32" /D "NDEBUG" /D "_UNICODE" /D "_ATL_STATIC_REGISTRY" /D "TARGET_IS_NT40_OR_LATER" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_MERGE_PROXYSTUB" /D "IMPORT" /D "NT" /Yu"stdafx.h" /FD /c
# ADD MTL /Oicf
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 msvcrt-ruby191.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /version:1.0 /subsystem:windows /dll /machine:I386 /out:"ReleaseUMinDependency/RScript19.dll" /libpath:"d:\ruby\lib" /libpath:"c:\home\ruby-1.9\usr\lib"
# Begin Custom Build - Registering
OutDir=.\ReleaseUMinDependency
TargetPath=.\ReleaseUMinDependency\RScript19.dll
InputPath=.\ReleaseUMinDependency\RScript19.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	if "%OS%"=="" goto NOTNT 
	if not "%OS%"=="Windows_NT" goto NOTNT 
	copy "$(TargetPath)" c:\home\ruby-1.9\usr\bin 
	regsvr32 /s /c "c:\home\ruby-1.9\usr\bin\RScript19.dll" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	goto end 
	:NOTNT 
	echo Windows 9x doesn't register Unicode DLL 
	:end 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "RScript - Win32 Debug"
# Name "RScript - Win32 Unicode Debug"
# Name "RScript - Win32 Release MinSize"
# Name "RScript - Win32 Release MinDependency"
# Name "RScript - Win32 Unicode Release MinSize"
# Name "RScript - Win32 Unicode Release MinDependency"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\dlldatax.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\eventsink.cpp
# End Source File
# Begin Source File

SOURCE=.\Initializer.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemDisp.cpp
# End Source File
# Begin Source File

SOURCE=.\RScript.cpp
# End Source File
# Begin Source File

SOURCE=.\RScript.def
# End Source File
# Begin Source File

SOURCE=.\RScript.idl
# ADD MTL /tlb ".\RScript.tlb" /h "RScript.h" /iid "RScript_i.c" /Oicf
# End Source File
# Begin Source File

SOURCE=.\RScript.rc
# End Source File
# Begin Source File

SOURCE=.\RScriptCore.cpp
# End Source File
# Begin Source File

SOURCE=.\Rubyize.cpp
# End Source File
# Begin Source File

SOURCE=.\RubyObject.cpp
# End Source File
# Begin Source File

SOURCE=.\RubyScript.cpp
# End Source File
# Begin Source File

SOURCE=.\RubyWrapper.cpp
# End Source File
# Begin Source File

SOURCE=.\ScrError.cpp
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

SOURCE=.\dlldatax.h
# PROP Exclude_From_Scan -1
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\eventsink.h
# End Source File
# Begin Source File

SOURCE=.\giplip.h
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

SOURCE=.\Rubyize.h
# End Source File
# Begin Source File

SOURCE=.\RubyObject.h
# End Source File
# Begin Source File

SOURCE=.\RubyScript.h
# End Source File
# Begin Source File

SOURCE=.\RubyWrapper.h
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
# Begin Source File

SOURCE=.\Rubyize.rgs
# End Source File
# Begin Source File

SOURCE=.\RubyScript.rgs
# End Source File
# Begin Source File

SOURCE=.\RubyWrapper.rgs
# End Source File
# End Group
# End Target
# End Project
