#--------------------------------------------------------------------------#
#                                                                          #
#   MAKEFILE for ARRAY examples                                            #
#                                                                          #
#   Copyright (c) 1994, 1996 Borland International                         #
#   All Rights Reserved                                                    #
#                                                                          #
#   Usage:                                                                 #
#       make MODEL=f                 for 32-bit Windows                    #
#       make MODEL={m|l}             for 16-bit Windows                    #
#       make MODEL={m|l}     -DDOS16 for 16-bit DOS                        #
#                                                                          #
#--------------------------------------------------------------------------#

!if $(SYSTEM)==WIN16 || $d(WIN16) || $(SYSTEM)==DOS16 || $d(DOS16) || !$d(SYSTEM)
 MODELS = mlh
!endif
SYSTEMS = DOS16 WIN16 CON32

EXE    = array
EXEALL = array.exe iarray.exe sarray.exe isarray.exe

EXEMAKE= $(ARRAY) $(IARRAY) $(SARRAY) $(ISARRAY)

RULES  = myclass.obj: ..\myclass.cpp

ARRAY=$(EXERULE:array.obj=array.obj myclass.obj)
IARRAY=$(ARRAY:array=iarray)

SARRAY=$(ARRAY:array=sarray)
ISARRAY=$(ARRAY:array=isarray)

!include $(BCEXAMPLEDIR)\bidsmake.gen

