//----------------------------------------------------------------------------
// ObjectWindows - (C) Copyright 1991, 1996 by Borland International
//   french.bas
//----------------------------------------------------------------------------

Fonction IntlDemo()
    AffecteR�f MonObjet = Cr�erObjet("Locale.Application")
    AffecteR�f MaFen�tre = MonObjet.Fen�tre
    MaFen�tre.CLocale
    MaFen�tre.LocaleAm�ricaine
    MaFen�tre.LocaleAnglaise
    MaFen�tre.LocaleFran�aise
    MaFen�tre.LocaleAllemande
    MaFen�tre.LangueAnglaise
    MaFen�tre.LangueFran�aise
    MaFen�tre.LangueAllemande
    AffecteR�f Fen�treEnfante = MaFen�tre.Fen�tre
    Fen�treEnfante.IsAlnum
    Fen�treEnfante.IsAlpha
    Fen�treEnfante.IsAscii
    Fen�treEnfante.IsCntrl
    Fen�treEnfante.IsDigit
    Fen�treEnfante.IsGraph
    Fen�treEnfante.IsLower
    Fen�treEnfante.IsPrint
    Fen�treEnfante.IsPunct
    Fen�treEnfante.IsSpace
    Fen�treEnfante.IsUpper
    Fen�treEnfante.IsXDigit
    MonObjet.Quitter
Fin Fonction
