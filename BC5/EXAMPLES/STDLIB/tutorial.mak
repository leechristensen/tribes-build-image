#==========================================================================
#
#  makefile - makefile for examples
#
#==========================================================================
#
#  (c) Copyright 1994, 1995 Rogue Wave Software, Inc.
#  ALL RIGHTS RESERVED
#
#  The software and information contained herein are proprietary to, and
#  comprise valuable trade secrets of, Rogue Wave Software, Inc., which
#  intends to preserve as trade secrets such software and information.
#  This software is furnished pursuant to a written license agreement and
#  may be used, copied, transmitted, and stored only in accordance with
#  the terms of such license and with the inclusion of the above copyright
#  notice.  This software and information or any other copies thereof may
#  not be provided or otherwise made available to any other person.
#
#  Notwithstanding any other lease or license that may pertain to, or
#  accompany the delivery of, this computer software and information, the
#  rights of the Government regarding its use, reproduction and disclosure
#  are as set forth in Section 52.227-19 of the FARS Computer
#  Software-Restricted Rights clause.
#
#  Use, duplication, or disclosure by the Government is subject to
#  restrictions as set forth in subparagraph (c)(1)(ii) of the Rights in
#  Technical Data and Computer Software clause at DFARS 252.227-7013.
#  Contractor/Manufacturer is Rogue Wave Software, Inc.,
#  P.O. Box 2328, Corvallis, Oregon 97339.
#
#  This computer software and information is distributed with "restricted
#  rights."  Use, duplication or disclosure is subject to restrictions as
#  set forth in NASA FAR SUP 18-52.227-79 (April 1985) "Commercial
#  Computer Software-Restricted Rights (April 1985)."  If the Clause at
#  18-52.227-74 "Rights in Data General" is specified in the contract,
#  then the "Alternate III" clause applies.
#
#==========================================================================
#
#  Makefile for the Rogue Wave Standard Library package, using
#  Borland C++ with a Win32 target.
#
#  Note ONLY the Borland-supplied "make" command
#  should be used.
#
#==========================================================================

!include makeincl.inc

########################   Targets    #################################

all : alg1.exe alg2.exe alg3.exe alg4.exe alg5.exe alg6.exe \
      alg7.exe calc.exe complx.exe concord.exe exceptn.exe graph.exe \
      icecream.exe radix.exe sieve.exe spell.exe tele.exe widwork.exe

clean:
   if exist *.obj del *.obj
   if exist *.exe del *.exe

###########################  Implicit Rules #######################
.cpp.obj:
    $(CPP) $(CPPFLAGS) -c {$< }

.obj.exe:
    $(CPP) $(CPPFLAGS) $<


