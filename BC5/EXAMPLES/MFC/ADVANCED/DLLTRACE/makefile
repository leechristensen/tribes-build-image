# This is a part of the Microsoft Foundation Classes C++ library.
# Copyright (C) 1992-1995 Microsoft Corporation
# All rights reserved.
#
# This source code is only intended as a supplement to the
# Microsoft Foundation Classes Reference and related
# electronic documentation provided with the library.
# See these sources for detailed information regarding the
# Microsoft Foundation Classes product.
#

# Build dlltrace application and Trace DLL
#
# Options to NMAKE:
#     "OPT" => use to set custom compile options

BORINCL=$(MAKEDIR)\..\include
BORLIB=$(MAKEDIR)\..\lib

!if "$(DEBUG)" == ""
DEBUG=1
!endif
!if "$(DEBUG)" != "0"
DEBUG_SUFFIX=d
!endif

LIBRARIES=nafxcw$(DEBUG_SUFFIX).lib $(OTHERLIBS) cw32mt.lib import32.lib

#CPPFLAGS=/c /W3 /GX /MT$(DEBUG_SUFFIX) $(DEBUGFLAGS) $(OPT)
CPPFLAGS=/c /w- $(DEBUG_FLAG) /I$(BORINCL);$(BORINCL)\mfc /D_RTLDLL
#LINKFLAGS=/subsystem:windows
LINKFLAGS=/aa /w-inq /n /s /L$(BORLIB)
!if "$(DEBUG)" == "0"
#CPPFLAGS=$(CPPFLAGS) /O1
CPPFLAGS=$(CPPFLAGS) /v-
LINKFLAGS=$(LINKFLAGS) /v-
!else
#CPPFLAGS=$(CPPFLAGS) /Od /D_DEBUG
CPPFLAGS=$(CPPFLAGS) /D_DEBUG /v /vi
LINKFLAGS=$(LINKFLAGS) /v
!endif
!if "$(UNICODE)" != "1"
#CPPFLAGS=$(CPPFLAGS) /D_MBCS
CPPFLAGS=$(CPPFLAGS)
!else
CPPFLAGS=$(CPPFLAGS) /D_UNICODE
#LINKFLAGS=$(LINKFLAGS) /entry:wWinMainCRTStartup
!endif

!if "$(CODEVIEW)" != "0"
#CPPFLAGS=$(CPPFLAGS) /Z7
CPPFLAGS=$(CPPFLAGS)
#LINKFLAGS=$(LINKFLAGS) /debug:full /debugtype:cv
!endif

!if "$(NO_PDB)" == "1"
LINKFLAGS=$(LINKFLAGS) /pdb:none
PDB_NONE=/pdb:none
!endif

goal: dlltrace.exe trace.dll

#############################################################################

dlltrace.obj: dlltrace.cpp resource.h traceapi.h
#       cl $(CPPFLAGS) dlltrace.cpp
	bcc32 $(CPPFLAGS) dlltrace.cpp

dlltrace.exe: dlltrace.obj dlltrace.res trace.lib
#       link $(LINKFLAGS) /out:dlltrace.exe dlltrace.obj trace.lib dlltrace.res
	tlink32 /Tpe @&&~
$(LINKFLAGS) +
$(BORLIB)\c0w32.obj dlltrace.obj
dlltrace.exe
dlltrace.map
trace.lib $(LIBRARIES)

dlltrace.res
~

dlltrace.res: dlltrace.rc resource.h hello.ico
#       rc /r dlltrace.rc
	brc32 -I$(BORINCL);$(BORINCL)\mfc -r dlltrace.rc

#############################################################################

trace.obj: trace.cpp traceapi.h
#       cl $(CPPFLAGS) /D_WINDLL /D_USRDLL trace.cpp
	bcc32 $(CPPFLAGS) /D_WINDLL /D_USRDLL trace.cpp

trace.dll: trace.obj trace.res
#       link /debug:full /debugtype:cv /dll $(PDB_NONE) \
#       /out:trace.dll /def:trace.def /implib:trace.lib /map:trace.map \
#       trace.obj trace.res
	tlink32 /Tpd @&&~
$(LINKFLAGS) +
$(BORLIB)\c0d32.obj trace.obj
trace.dll
trace.map
$(LIBRARIES)
trace.def
trace.res
~

trace.lib: trace.dll
	implib trace.lib trace.dll

trace.res: trace.rc traceres.h
#       rc /r trace.rc
	brc32 -I$(BORINCL);$(BORINCL)\mfc -r trace.rc

#############################################################################

clean::
	if exist *.exe erase *.exe
	if exist *.dll erase *.dll
	if exist *.aps erase *.aps
	if exist *.pch erase *.pch
	if exist *.map erase *.map
	if exist *.obj erase *.obj
	if exist *.pdb erase *.pdb
	if exist *.map erase *.map
	if exist *.lib erase *.lib
	if exist *.res erase *.res

#############################################################################
