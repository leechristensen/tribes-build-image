#-----------------------------------------------------------------------------
# ObjectWindows - (C) Copyright 1994 by Borland International
#   Non-Doc/View Ole tutorial application -- makefile.
#
# The following models may be built by defines on command line or environment:
#   16-bit OWL DLL model: make          (MODEL=d, SYSTEM=WIN16 are defaults)
#   16-bit large model:   make MODEL=l  (SYSTEM=WIN16 is the default)
# Additionally set DEBUG=1 to build with debug information
# Specify TARGET=owlole?.exe to build only one target step, rather than all
#
#-----------------------------------------------------------------------------

MODELS =ldf
EXERES = sampexe
EXEALL = owlole1.exe owlole2.exe owlole3.exe
EXEMAKE= $(owlole1) $(owlole2) $(owlole3)

owlole1 = $(EXERULE:sampexe=owlole1)
owlole2 = $(EXERULE:sampexe=owlole2)
owlole3 = $(EXERULE:sampexe=owlole3)

!include $(BCEXAMPLEDIR)\owlocfmk.gen
