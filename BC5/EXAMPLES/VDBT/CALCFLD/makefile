#----------------------------------------------------------------------------
# Borland C++ - (C) Copyright 1996 by Borland International
#   examples\vdbt\calcfld\makefile
#----------------------------------------------------------------------------

SYSTEMS = WIN16 WIN32

!ifndef SYSTEM
SYSTEM = WIN32
!endif

!ifndef MODEL
MODEL = d
!endif

!if $(SYSTEM)==WIN16
MODELS = d l

!if $(MODEL)==d
USERLIBS = bidsi.lib bdto.lib bdtc.lib ole2w16.lib
!else
USERLIBS = bidsl.lib bdto.lib bdtc.lib ole2w16.lib
!endif
DEFEXE = calc16.def

!else

#WIN32 Macro definitions
MODELS = d f

!if $(MODEL)==d
USERLIBS = bidsfi.lib bdtof.lib bdtcf.lib ole2w32.lib
!else
USERLIBS = bidsf.lib bdtof.lib bdtcf.lib ole2w32.lib
!endif
DEFEXE = calc32.def

!endif

EXE = calcfld
USERLIBPATH = $(BCEXAMPLEDIR)\..\lib
!include $(BCEXAMPLEDIR)\makefile.gen
