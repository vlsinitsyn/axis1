# Microsoft Developer Studio Project File - Name="libAxiscpp" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=libAxiscpp - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libAxiscpp.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libAxiscpp.mak" CFG="libAxiscpp - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libAxiscpp - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "libAxiscpp - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libAxiscpp - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\axiscpp\WEB-INF\classes"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LIBAXISCPP_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "../../common" /I "../../engine" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LIBAXISCPP_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 Engine.lib Common.lib Soap.lib wsdd.lib xerces-c_2.lib /nologo /dll /machine:I386 /libpath:"../../../lib/win32/axis" /libpath:"../../../lib/win32/xercesc"

!ELSEIF  "$(CFG)" == "libAxiscpp - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\axiscpp\WEB-INF\classes"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LIBAXISCPP_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../../common" /I "../../engine" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LIBAXISCPP_EXPORTS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 Engine_D.lib Common_D.lib Soap_D.lib wsdd_D.lib xerces-c_2D.lib /nologo /dll /debug /machine:I386 /pdbtype:sept /libpath:"../../../lib/win32/axis" /libpath:"../../../lib/win32/xercesc"

!ENDIF 

# Begin Target

# Name "libAxiscpp - Win32 Release"
# Name "libAxiscpp - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\libAxiscpp.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AxisCppContentHandler.h
# End Source File
# Begin Source File

SOURCE=.\libAxiscpp.h
# End Source File
# End Group
# Begin Group "Java Files"

# PROP Default_Filter "*.java"
# Begin Source File

SOURCE=.\AxisCppContentHandler.java

!IF  "$(CFG)" == "libAxiscpp - Win32 Release"

# Begin Custom Build
ProjDir=.
InputPath=.\AxisCppContentHandler.java
InputName=AxisCppContentHandler

"$(ProjDir)/$(InputName).class" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo javac -d axiscpp\WEB-INF\classes $(InputPath) 
	javac -d axiscpp\WEB-INF\classes $(InputPath) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "libAxiscpp - Win32 Debug"

# Begin Custom Build
ProjDir=.
InputPath=.\AxisCppContentHandler.java
InputName=AxisCppContentHandler

"$(ProjDir)/$(InputName).class" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo javac -d axiscpp\WEB-INF\classes $(InputPath) 
	javac -d axiscpp\WEB-INF\classes $(InputPath) 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\AxisCppServlet.java

!IF  "$(CFG)" == "libAxiscpp - Win32 Release"

# Begin Custom Build
ProjDir=.
InputPath=.\AxisCppServlet.java
InputName=AxisCppServlet

"$(ProjDir)/$(InputName).class" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo javac -d axiscpp\WEB-INF\classes $(InputPath) 
	javac -d axiscpp\WEB-INF\classes $(InputPath) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "libAxiscpp - Win32 Debug"

# Begin Custom Build
ProjDir=.
InputPath=.\AxisCppServlet.java
InputName=AxisCppServlet

"$(ProjDir)/$(InputName).class" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo javac -d axiscpp\WEB-INF\classes $(InputPath) 
	javac -d axiscpp\WEB-INF\classes $(InputPath) 
	
# End Custom Build

!ENDIF 

# End Source File
# End Group
# End Target
# End Project
