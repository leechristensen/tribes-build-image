# This is a part of the Microsoft Foundation Classes C++ library.
# Copyright (C) 1992-1995 Microsoft Corporation
# All rights reserved.
#
# This source code is only intended as a supplement to the
# Microsoft Foundation Classes Reference and related
# electronic documentation provided with the library.
# See these sources for detailed information regarding the
# Microsoft Foundation Classes product.

OBJS= circ2.obj circ2ctl.obj circ2ppg.obj stdafx.obj

USES_OLE=1
AFXDLL=1
USRDLL=1
PROJ = circ2

All: MainRule
        @-if exist $(PROJ).ocx del $(PROJ).ocx
	@-ren $(PROJ).dll $(PROJ).ocx

!include <$(MAKEDIR)\..\include\mfc\mfcsamps.mak>

