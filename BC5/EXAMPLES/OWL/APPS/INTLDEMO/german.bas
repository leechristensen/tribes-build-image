//----------------------------------------------------------------------------
// ObjectWindows - (C) Copyright 1991, 1996 by Borland International
//   german.bas
//----------------------------------------------------------------------------

Funktion IntlDemo()
    Setze AnwendungObjekt = ErstelleObjekt("Locale.Application")
    Setze HauptFenster = AnwendungObjekt.Fenster
    HauptFenster.CLocale
    HauptFenster.AmerikanischeLocale
    HauptFenster.BritischeLocale
    HauptFenster.Franz�sischeLocale
    HauptFenster.DeutscheLocale
    HauptFenster.EnglischeSprache
    HauptFenster.Franz�sischeSprache
    HauptFenster.DeutscheSprache
    Setze ChildFenster = HauptFenster.Fenster
    ChildFenster.IsAlnum
    ChildFenster.IsAlpha
    ChildFenster.IsAscii
    ChildFenster.IsCntrl
    ChildFenster.IsDigit
    ChildFenster.IsGraph
    ChildFenster.IsLower
    ChildFenster.IsPrint
    ChildFenster.IsPunct
    ChildFenster.IsSpace
    ChildFenster.IsUpper
    ChildFenster.IsXDigit
    AnwendungObjekt.Beenden
Ende Funktion
