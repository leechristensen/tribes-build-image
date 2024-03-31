//----------------------------------------------------------------------------
// ObjectWindows
// Copyright (c) 1995, 1997 by Borland International, All Rights Reserved
//
//$Revision:   10.20  $
//
// Implementation of class TRichEdit.
//----------------------------------------------------------------------------
#pragma hdrignore SECTION
#include <owl/pch.h>
#if !defined(OWL_WINDOW_H)
# include <owl/window.h>
#endif
#if !defined(OWL_CONTROL_H)
# include <owl/control.h>
#endif
#if !defined(OWL_EDIT_H)
# include <owl/edit.h>
#endif
#if !defined(OWL_EDITSEAR_H)
# include <owl/editsear.h>
#endif
#if !defined(OWL_EDITFILE_H)
# include <owl/editfile.h>
#endif
#if !defined(OWL_RICHEDIT_H)
# include <owl/richedit.h>
#endif
#if !defined(CLASSLIB_FILENAME_H)
# include <classlib/filename.h>
#endif

OWL_DIAGINFO;

// Range of the editor's font size
//
const int MinPointSize = 6;
const int MaxPointSize = 128;

// System DLL providing underlying support for RichEdit controls
//
const char RichEditDllName[]  = "RICHED32.DLL";

//
// Constructs a CharacterFormat structure from the current character
// attributes of a RICHEDIT control. NOTE: Specifying 'true' for selection
// will return the attribute of the character at the current location if
// there are no block of data selected in the control.
//
TCharFormat::TCharFormat(const TRichEdit& edit, bool selection, ulong mask)
{
  PRECONDITION((HWND)edit);

  memset(this, 0, sizeof(CHARFORMAT));

  cbSize = sizeof(CHARFORMAT);
  dwMask = mask;
  edit.GetCharFormat(*this, selection);
}

//
// Toggles the 'bold' character attribute according to boolean parameter
// specified.
//
void
TCharFormat::EnableBold(bool flag)
{
  dwMask |= CFM_BOLD;
  if (flag) {
    dwEffects |= CFE_BOLD;
  }
  else {
    dwEffects &= ~CFE_BOLD;
  }
}

//
// Toggles 'italic' character attribute based on the boolean parameter
// specified.
//
void
TCharFormat::EnableItalic(bool flag)
{
  dwMask |= CFM_ITALIC;
  if (flag) {
    dwEffects |= CFE_ITALIC;
  }
  else {
    dwEffects &= ~CFE_ITALIC;
  }
}

//
// Toggle the 'underline' character attribute based on the boolean parameter
// specified.
//
void
TCharFormat::EnableUnderline(bool flag)
{
  dwMask |= CFM_UNDERLINE;
  if (flag) {
    dwEffects |= CFE_UNDERLINE;
  }
  else {
    dwEffects &= ~CFE_UNDERLINE;
  }
}

//
// Toggle the 'strike-out' character attribute based on the boolean parameter
// specified.
//
void
TCharFormat::EnableStrikeOut(bool flag)
{
  dwMask |= CFM_STRIKEOUT;
  if (flag) {
    dwEffects |= CFE_STRIKEOUT;
  }
  else {
    dwEffects &= ~CFE_STRIKEOUT;
  }
}

//
// Toggle the 'protected' character attribute based on the boolean parameter
// specified.
//
void
TCharFormat::EnableProtected(bool flag)
{
  dwMask |= CFM_PROTECTED;
  if (flag) {
    dwEffects |= CFE_PROTECTED;
  }
  else {
    dwEffects &= ~CFE_PROTECTED;
  }
}

//
// Retrieve the character color stored in the CHARFORMAT structure.
// NOTE: Default to system text color of no explicit color was set
//       in the CHARFORMAT structure.
//
TColor
TCharFormat::GetTextColor() const
{
  if ((dwMask & CFM_COLOR) && !(dwEffects & CFE_AUTOCOLOR))
    return TColor(crTextColor);
  return TColor::SysWindowText;
}

//
// Update the CHARFORMAT structure with the specified color.
// NOTE: If 'TColor::None' is specified, enable the flag specifying that the
//       color should default to the system text color .
//
void
TCharFormat::SetTextColor(const TColor& color)
{
  dwMask |= CFM_COLOR;
  if (color == TColor::None) {
    dwEffects |= CFE_AUTOCOLOR;
  }
  else {
    dwEffects &= ~CFE_AUTOCOLOR;
    crTextColor = color;
  }
}

//
// Set the face name of the font.
//
void
TCharFormat::SetFaceName(const char far* name)
{
  strcpy(szFaceName, name);
}

//
// Set the character set of the font. Valid values include the following:
// ANSI_CHARSET, OEM_CHARSET and SYMBOL_CHARSET.
//
void
TCharFormat::SetCharSet(uint8 charSet)
{
  bCharSet = charSet;
}

//
// Set the pitch and family of the font. The two lower-bits specify the pitch
// of the font and can be one of the following values:
// DEFAULT_PITCH, FIXED_PITCH and VARIABLE_PITCH.

// Bits 4 through 7 of the member specify the font family and can be
// one of the following values:
// FF_DECORATIVE, FF_DONTCARE, FF_MODERN, FF_ROMAN, FF_SCRIPT & FF_SWISS.
//
void
TCharFormat::SetPitchAndFamily(uint8 pitchFam)
{
  bPitchAndFamily = pitchFam;
}

//
// Set the character height
//
void
TCharFormat::SetHeight(long height)
{
  dwMask |= CFM_SIZE;
  yHeight = height;
}

//
// Set the character offset from the baseline. If the parameter is positive,
// the character is a superscript; if it is negative, the character is a
// subscript
//
void
TCharFormat::SetOffset(long offset)
{
  dwMask |= CFM_OFFSET;
  yOffset = offset;
}

//
// Transfer the information currently in the underlying CHARFORMAT structure
// to a LOGFONT structure.
// This is useful when changing the editor's font as initialized LOGFONT
// structure can subsequently be used when invoking the FONT Common
// Dialog, i.e. TChooseFontDialog
//
void
TCharFormat::SetFontInfo(LOGFONT& lf) const
{
  memset(&lf, 0, sizeof(LOGFONT));

  if (dwMask & CFM_SIZE)                //  (1 Point == 20 twips)
    lf.lfHeight = yHeight/-20;

  if (dwMask & CFM_BOLD)
    lf.lfWeight = dwEffects & CFE_BOLD ? FW_BOLD : FW_NORMAL;

  if (dwMask & CFM_ITALIC)
    lf.lfItalic = (uint8)(dwEffects & CFE_ITALIC ? TRUE : FALSE);

  if (dwMask & CFM_UNDERLINE)
    lf.lfUnderline = (uint8)(dwEffects & CFE_UNDERLINE ? TRUE : FALSE);

  if (dwMask & CFM_FACE) {
    lf.lfPitchAndFamily = bPitchAndFamily;
    strcpy(lf.lfFaceName, szFaceName);
  }

  lf.lfCharSet = DEFAULT_CHARSET;
  lf.lfQuality = DEFAULT_QUALITY;
}

//
// Initialize the underlying CHARFORMAT structure using the information
// stored in a LOGFONT structure.
//
void
TCharFormat::GetFontInfo(const LOGFONT& lf)
{
  dwMask = (CFM_SIZE | CFM_BOLD | CFM_ITALIC | CFM_OFFSET|
            CFM_STRIKEOUT | CFM_UNDERLINE | CFM_FACE);

  yHeight = lf.lfHeight * -20;

  if (FW_BOLD == lf.lfWeight)
    dwEffects |= CFE_BOLD;
  if (lf.lfItalic)
    dwEffects |= CFE_ITALIC;
  if (lf.lfStrikeOut)
    dwEffects |= CFE_STRIKEOUT;
  if (lf.lfUnderline)
    dwEffects |= CFE_UNDERLINE;

  bPitchAndFamily = lf.lfPitchAndFamily;
  strcpy(szFaceName, lf.lfFaceName);
}

//
// Construct a TFormatRange object initializing data members with the
// specified parameters.
//
TFormatRange::TFormatRange(HDC renderDC, HDC devDC, const TRect& renderArea,
                           const TRect& entireArea, const TCharRange& range)
{
  SetRenderDC(renderDC);
  SetTargetDC(devDC);
  SetRenderRect(renderArea);
  SetPageRect(entireArea);
  SetRange(range);
}

//
// Set the device context of the device to render to.
//
void
TFormatRange::SetRenderDC(HDC renderDC)
{
  hdc = renderDC;
}

//
// Set the device context of the target device to format for.
//
void
TFormatRange::SetTargetDC(HDC targetDC)
{
  hdcTarget = targetDC;
}

//
// Set the area to render to.
// NOTE: The specified units are in TWIPS.
//
void
TFormatRange::SetRenderRect(const TRect& renderRect)
{
  rc = renderRect;
}

//
// Set the entire area of the rendering device.
// NOTE: The specified units are in TWIPS.
//
void
TFormatRange::SetPageRect(const TRect& pgRect)
{
  rcPage = pgRect;
}

//
// Set the range of text to format.
//
void
TFormatRange::SetRange(const TCharRange& charRange)
{
  chrg = charRange;
}

//
// Set the range of text to format specifying the starting and ending
// character offsets.
//
void
TFormatRange::SetRange(long start, long end)
{
  chrg.cpMin = start;
  chrg.cpMax = end;
}

//
// Construct a default TParaFormat structure.
//
TParaFormat::TParaFormat(ulong mask)
{
  cbSize = sizeof(PARAFORMAT);
  dwMask = mask;
}

//
// Construct a TParaFormat structure whose members are initialized with the
// paragraph formatting attributes of the current selection of a rich edit
// control.
//
TParaFormat::TParaFormat(const TRichEdit& edit, ulong mask)
{
  PRECONDITION((HWND)edit);

  cbSize = sizeof(PARAFORMAT);
  dwMask = mask;
  edit.GetParaFormat(*this);
}

//
// Toggle the specified flag in the member which describes which information
// of the PARAFORMAT structures is valid.
//
void
TParaFormat::ToggleMaskBit(ulong flag)
{
  dwMask ^= flag;
}

//
// Set the numbering options. The only valid parameter is '0' or PFN_BULLET.
//
void
TParaFormat::SetNumbering(uint16 opt)
{
  PRECONDITION(opt == 0 || opt == PFN_BULLET);

  dwMask |= PFM_ALIGNMENT;
  wNumbering = opt;
}

//
// Set the indentation of the first line in the paragraph. If the paragraph
// formatting is being set and the 'relative' parameter is true, the 'start'
// value is  treated as a relative value that is added to the starting
// indentation of each affected paragraph.
//
void
TParaFormat::SetStartIndent(long start, bool relative)
{
  dwMask |= PFM_STARTINDENT;
  if (relative)
    dwMask |= PFM_OFFSETINDENT;
  else
    dwMask &= ~PFM_OFFSETINDENT;

  dxStartIndent = start;
}

//
// Set the size of the right identation, relative to the right margin.
//
void
TParaFormat::SetRightIndent(long indent)
{
  dwMask |= PFM_RIGHTINDENT;
  dxRightIndent = indent;
}

//
// Set the indentation of the second line and subsequent lines,
// relative to the starting indentation. The first line is indented if
// the 'offset' parameter is negative, or outdented if it is positive.
//
void
TParaFormat::SetOffset(long offset)
{
  dwMask |= PFM_OFFSET;
  dxOffset = offset;
}

//
// Set the alignment option. The 'opt' parameter can be one of the following
// values:
//    PFA_LEFT        Paragraphs are aligned with the left margin.
//    PFA_RIGHT       Paragraphs are aligned with the right margin.
//    PFA_CENTER      Paragraphs are centered.
//
void
TParaFormat::SetAlignment(uint16 opt)
{
  dwMask |= PFM_ALIGNMENT;
  wAlignment = opt;
}

//
// Set the number and absolute positions of the tab stops.
//
void
TParaFormat::SetTabCount(short cnt, long* tabs)
{
  PRECONDITION(tabs || !cnt);

  dwMask |= PFM_TABSTOPS;
  cTabCount = cnt;
  for (int i=0; i<cnt; i++)
    rgxTabs[i] = *tabs++;
}


DEFINE_RESPONSE_TABLE1(TRichEdit, TEditFile)
  EV_COMMAND(CM_EDITCUT, CmEditCut),
  EV_COMMAND(CM_EDITCOPY, CmEditCopy),
  EV_COMMAND(CM_EDITPASTE, CmEditPaste),
  EV_COMMAND(CM_EDITDELETE, CmEditDelete),
  EV_COMMAND(CM_EDITCLEAR, CmEditClear),
  EV_COMMAND(CM_EDITUNDO, CmEditUndo),
  EV_COMMAND_ENABLE(CM_EDITCUT, CeHasSelect),
  EV_COMMAND_ENABLE(CM_EDITCOPY, CeHasSelect),
  EV_COMMAND_ENABLE(CM_EDITDELETE, CeHasSelect),
  EV_COMMAND_ENABLE(CM_EDITPASTE, CeEditPaste),
  EV_COMMAND_ENABLE(CM_EDITCLEAR, CeEditClear),
  EV_COMMAND_ENABLE(CM_EDITUNDO, CeEditUndo),
  EV_WM_CHAR,
  EV_WM_KEYDOWN,
  EV_WM_GETDLGCODE,
  EV_WM_SETFOCUS,
  EV_WM_KILLFOCUS,
END_RESPONSE_TABLE;

//
// constructor for a TRichEdit object
//
// by default, edit control has a border and its text is left-justified
//
// multiline edit control has horizontal vertical scroll bars
//
TRichEdit::TRichEdit(TWindow*        parent,
                     int             id,
                     const char far* text,
                     int x, int y, int w, int h,
                     const char far* fileName,
                     TModule*        module)
  : TEditFile(parent, id, text, x, y, w, h, fileName, module)
{
  // Make sure the RichEdit DLL is available
  //
  if (!TRichEditDll::IsAvailable())
    TXCommCtrl::Raise();  

  // Default to RTF data format
  //
  SetFormat(SF_RTF);

  // Undo the styles specific to "EDIT" controls; add richedit styles
  //
  ModifyStyle(ES_LOWERCASE|ES_PASSWORD|ES_OEMCONVERT|
              ES_UPPERCASE|ES_AUTOHSCROLL,
              ES_LEFT | WS_BORDER | WS_TABSTOP);
}

//
// constructor for TRichEdit associated with a MS-Windows interface element
// created by MS-Windows from a resource definition
//
// by default, data transfer is enabled
//
TRichEdit::TRichEdit(TWindow*   parent,
                     int        resourceId,
                     TModule*   module)
  : TEditFile(parent, resourceId, module ? *module : *parent->GetModule())
{
  // Make sure the RichEdit DLL is available
  //
  if (!TRichEditDll::IsAvailable())
    TXCommCtrl::Raise();  
}


//
// Retrieve the current character formatting in an edit control. If
// 'selection' parameter is 'true', the attribute of the current selection is
// retrieved. Otherwise, the default formatting attribute is retrieved.
//
ulong
TRichEdit::GetCharFormat(TCharFormat far& cf, bool selection) const
{
  return CONST_CAST(TRichEdit*,this)->SendMessage(
                         EM_GETCHARFORMAT, selection ? SCF_SELECTION : 0,
                                                  TParam2(&cf));
}

//
// Retrieve the paragraph formatting of the current selection of the rich
// edit control.
//
// NOTE: If more than one paragraph is selected, the structure
//       receives the attributes of the first paragraph, and the dwMask
//       member specifies which attributes are consistent throughout the
//       entire selection.
//
ulong
TRichEdit::GetParaFormat(TParaFormat far& pf) const
{
  return CONST_CAST(TRichEdit*,this)->SendMessage(EM_GETPARAFORMAT, 0,
                                                    TParam2(&pf));
}

//
// Set the character formatting of a rich edit control. The 'flags' parameter
// can be one of the following:
//
//    SCF_SELECTION  Applies the formatting to the current selection, or sets
//                   the default formatting if the selection is empty.
//    SCF_WORD       Applies the formatting to the selected word or words. If
//                   the selection is empty but the insertion point is inside
//                   a word, the formatting is applied to the word. This value
//                   must be used in conjunction with the SCF_SELECTION value.
//
bool
TRichEdit::SetCharFormat(const TCharFormat far& cf, uint flags)
{
  return SendMessage(EM_SETCHARFORMAT, TParam1(flags), TParam2(&cf)) != 0;
}

//
// Set the paragraph formatting of the current selection of the rich edit
// control.
//
bool
TRichEdit::SetParaFormat(const TParaFormat far& pf)
{
  return SendMessage(EM_SETPARAFORMAT, 0, TParam2(&pf)) != 0;
}

//
// Set the background color of the rich edit control.
// NOTE: If 'TColor::None' is specified, the color is set to the window
//       background system color.
//
TColor
TRichEdit::SetBkgndColor(const TColor& bkColor)
{
  TParam1 p1 = bkColor == TColor::None ? true : false;
  TParam2 p2 = bkColor == TColor::None ? 0    : (COLORREF)bkColor;
  return TColor((COLORREF)SendMessage(EM_SETBKGNDCOLOR, p1, p2));
}

//
// Return whether the current selection has a particular attribute.
// The 'mask' identifies the attribute we're interested in.
// The 'effects' contains the state of the attributes.
// The function returns one of the following values:
//
//    TFmtStatus::Yes           The attribute is enabled
//    TFmtStatus::No            The attribute is absent
//    TFmtStatus::Partly        The attribute is partly present
//
uint
TRichEdit::HasCharAttribute(ulong mask, ulong effects)
{
  TCharFormat cf(*this);
  if (cf.dwMask & mask) {
    if (cf.dwEffects & effects)
      return Yes;
    else
      return No;
  }
  else
    return Partly;
}

//
// Toggle a set of character attributes. The 'mask' identifies
// the attributes we're interested in while 'effects' identifies the state
// of the attributes.
//
bool
TRichEdit::ToggleCharAttribute(ulong mask, ulong effects)
{
  TCharFormat cf(*this);
  cf.dwMask = mask;
  cf.ToggleEffectsBit(effects);
  return SetCharFormat(cf);
}

//
// Increase or decrease (using a positive or negative value respectively) the
// point size of the current selection.
//
bool
TRichEdit::ChangeCharPointSize(int pointSizeDelta)
{
  TCharFormat cf(*this);
  cf.dwMask = CFM_SIZE;
  if (((cf.yHeight + 20*pointSizeDelta) <= (MaxPointSize*20)) &&
      ((cf.yHeight + 20*pointSizeDelta) >= (MinPointSize*6))) {
    cf.yHeight += 20*pointSizeDelta;
    return SetCharFormat(cf);
  }
  return false;
}

//
// Return true if the rich edit control has an active selection. Return false
// otherwise.
//
bool
TRichEdit::HasSelection() const
{
  TCharRange chRange;
  GetSelRange(chRange);
  return chRange.cpMin != chRange.cpMax;
}

//
// Retrieve the starting and ending character position of the selection
// in the rich edit control.
//
void
TRichEdit::GetSelection(uint& startPos, uint& endPos) const
{
  TCharRange cr;
  GetSelRange(cr);
  startPos = cr.cpMin;
  endPos = cr.cpMax;
}

//
// Retrieve the starting and ending character positions of the selection of
// the richedit control.
//
void
TRichEdit::GetSelRange(TCharRange far& cr) const
{
  CONST_CAST(TRichEdit*,this)->SendMessage(EM_EXGETSEL, 0, TParam2(&cr));
}

//
// Select a range of characters in the rich edit control.
//
bool
TRichEdit::SetSelection(uint startPos, uint endPos)
{
  TCharRange cr(startPos, endPos);
  return SetSelRange(cr) >= 0;
}

//
// Select a range of characters in the rich edit control.
//
int
TRichEdit::SetSelRange(const TCharRange far& cr)
{
  return (int)SendMessage(EM_EXSETSEL, 0, TParam2(&cr));
}

//
// Show or hide the selection in the rich edit control.
// The 'hide' parameter specifies whether to hide or show the selection. If the
// is 'false' the selection is shown. Otherwise, the selection is hidden.
// The 'changeStyle' parameter specifies whether to change the control's
// ES_NOHIDESEL window style. If this parameter is 'false', the selection is
// temporarily shown or hidden. Otherwise, the style is changed. If this
// parameter is 'true' and the control has the focus, the selection is hidden
// or shown as appropriate.
//
void
TRichEdit::HideSelection(bool hide, bool changeStyle)
{
  SendMessage(EM_HIDESELECTION, TParam1(hide), TParam2(changeStyle));
}

//
// Return the selection type of the rich edit control. Returns SEL_EMPTY if
// the selection is empty, or one or more of the following values:
//
//    Value                 Contents of the selection
//    -----                 ------------------------
//    SEL_TEXT              Text
//    SEL_OBJECT            At least one OLE object
//    SEL_MULTICHAR         More than one character of text
//    SEL_MULTIOBJECT       More than one OLE object
//
ulong
TRichEdit::GetSelectionType() const
{
  return CONST_CAST(TRichEdit*,this)->SendMessage(EM_SELECTIONTYPE);
}

//
// Retrieve a specified range of text from the rich edit control
//
int
TRichEdit::GetTextRange(TTextRange far& tr) const
{
  return (int)CONST_CAST(TRichEdit*,this)->SendMessage(EM_GETTEXTRANGE, 0,
                                                         TParam2(&tr));
}

//
// Retrieve a specified range of text from the rich edit control
//
int
TRichEdit::GetTextRange(const TCharRange far& cr, char far* buffer) const
{
  TTextRange tr(cr, buffer);
  return GetTextRange(tr);
}

//
// Retrieve a specified range of text from the rich edit control
//
void
TRichEdit::GetSubText(char far* str, uint startPos, uint endPos) const
{
  TTextRange tr(startPos, endPos, str);
  GetTextRange(tr);
}

//
// Retrieve the currently selected text of the rich edit control.
//
int
TRichEdit::GetSelectedText(char far* buffer) const
{
  return (int)CONST_CAST(TRichEdit*,this)->SendMessage(EM_GETSELTEXT, 0,
                                                         TParam2(buffer));
}

//
// Set an upper limit to the amount of text in the richedit control.
//
void
TRichEdit::LimitText(long max)
{
  SendMessage(EM_EXLIMITTEXT, 0, TParam2(max));
}

//
// Find text within the rich edit control. The 'flags' parameter can be a
// combination of the following values:
// i.   FT_MATCHCASE  - Perform a case sensitiv search
// ii.  FT_MATCHWORD  - Match whole words
//
int
TRichEdit::FindText(uint flags, const TFindText far& ft)
{
  return (int)SendMessage(EM_FINDTEXT, TParam1(flags), TParam2(&ft));
}

//
// Find text within the rich edit control. The 'flags' parameter can be a
// combination of the following values:
// i.   FT_MATCHCASE  - Perform a case sensitiv search
// ii.  FT_MATCHWORD  - Match whole words
//
int
TRichEdit::FindText(uint flags, const TCharRange far& cr, const char far* text)
{
  TFindText ft(cr, text);
  return FindText(flags, ft);
}

//
// Search for the specified text in the rich edit control. If found, select
// the text and return the offset of the text. Otherwise, return -1.
//
// NOTE: If the 'startPos' is -1, it is assumed that the starting position is
// the end [or beginning, depending on the direction parameter, 'up'] of the
// current selection
//
int
TRichEdit::Search(uint startPos, const char far* text, bool caseSensitive,
                  bool wholeWord, bool up)
{
  if (!text || !text[0])
    return -1;

  if (startPos == (uint)-1) {
    uint sBeg, sEnd;
    GetSelection(sBeg, sEnd);
    startPos = up ? sBeg : sEnd;
  }

  // The current docs. mention the FT_MATCHCASE and FT_WHOLEWORD flags which 
  // are not defined currently. I suspect they meant the FR_xxxx flags (used 
  // in CommDlg API).
  //
  TFindText findText(startPos, -1, text);
  uint flags = (caseSensitive ? FR_MATCHCASE : 0) |
               (wholeWord ? FR_WHOLEWORD : 0);
  int index = FindText(flags, findText);

  //
  // If we've got a match, select the text
  //
  if (index >= 0) {
    int len = strlen(text);
    SetSelection(index+len, index);
  }
  return index;
}

//
// Find the next work break before or after the specified character position,
// or retrieve information about the character at that position. The 'code'
// parameter can be one of the following:
//
//  WB_CLASSIFY       Returns the character class and word break flags of the
//                    character at the specified position.
//  WB_ISDELIMITER    Returns TRUE if the character at the specified position
//                    is a delimiter, or FALSE otherwise.
//  WB_LEFT           Finds the nearest character before the specified
//                    position that begins a word.
//  WB_LEFTBREAK      Finds the next word end before the specified position.
//  WB_MOVEWORDLEFT   Finds the next character that begins a word before the
//                    specified position. This value is used during CTRL+LEFT key processing.
//  WB_MOVEWORDRIGHT  Finds the next character that begins a word after the
//                    specified position. This value is used during
//                    CTRL+RIGHT key processing.
//  WB_RIGHT          Finds the next character that begins a word after the
//                    specified position.
//  WB_RIGHTBREAK     Finds the next end-of-word delimiter after the
//                    specified position.
//
// The return value is the character index of the word break, unless the
// 'code' parameter is WB_CLASSIFY or WB_ISDELIMETER
int
TRichEdit::FindWordBreak(uint code, long start)
{
  return (int)SendMessage(EM_FINDWORDBREAK, TParam1(code), TParam2(start));
}

//
// Determine which line contains the specified character in the richedit
// control.
// NOTE: The return value is zero-based.
//
int
TRichEdit::GetLineFromPos(uint charPos) const
{
  return (int)CONST_CAST(TRichEdit*,this)->SendMessage(EM_EXLINEFROMCHAR,
                                                         0, TParam2(charPos));
}

//
// Return true if the richedit can paste the specified clipboard format, or
// false otherwise.
//
bool
TRichEdit::CanPaste(uint format) const
{
  return CONST_CAST(TRichEdit*,this)->SendMessage(EM_CANPASTE,
                                                    TParam1(format)) != 0;
}

//
// Paste the specified clipboard format in the rich edit control.
//
void
TRichEdit::PasteSpecial(uint format)
{
  SendMessage(EM_PASTESPECIAL, TParam1(format));
}

//
// Paste a compatible clipboard format in the rich edit control.
//
void
TRichEdit::Paste()
{
  SendMessage(WM_PASTE);

}

//
// Replace the contents of the rich edit control with the specified data
// stream. The 'format' parameter can be one of the following data formats,
// optionally combined with the SFF_SELECTION flag:
//
//    Value               Meaning
//    -----               -------
//    SF_TEXT             Text
//    SF_RTF              Rich-text format
//
// If the SFF_SELECTION flag is specified, the stream replaces the contents of
// the current selection. Otherwise, the stream replaces the entire contents
// of the control.
//
ulong
TRichEdit::StreamIn(uint format, TEditStream far& es)
{
  return SendMessage(EM_STREAMIN, TParam1(format), TParam2(&es));
}

//
// Write the contents of the rich edit control to the specified data stream.
// The 'format' parameter can be one of the following values, optionally
// combined with the SFF_SELECTION flag:
//
//    Value               Meaning
//    -----               -------
//    SF_TEXT             Text with spaces in place of OLE objects
//    SF_RTF              Rich-text format (RTF)
//    SF_RTFNOOBJS        RTF with spaces in place of OLE object.
//    SF_TEXTIZED         Text with a text representation of OLE objects.
//
// NOTE: The SF_RTFNOOBJS option is useful if an application stores OLE
//       objects itself, as RTF representation of OLE objects is not very
//       compact.
//       If the SFF_SELECTION flag is specified, only the contents of the
//       current selection are streamed out. Otherwise, the entire contents of
//       the control are streamed out.
//
ulong
TRichEdit::StreamOut(uint format, TEditStream far& es)
{
  return SendMessage(EM_STREAMOUT, TParam1(format), TParam2(&es));
}

//
// Display a portion of the richedit control's content within the specified
// rectangle.
// NOTE: The content of the control must first be formatted via a call to the
//       'FormatRange' method.
//
bool
TRichEdit::DisplayBand(TRect far& rc)
{
  return SendMessage(EM_DISPLAYBAND, 0, TParam2(&rc)) != 0;
}

//
// Formats a range of text (specified via the 'chrg' member of the
// specified TFormatRange) for the device(s) specified via the 'hdcTarget'
// and 'hdc' members of the TFormatRange structure.
//
int
TRichEdit::FormatRange(const TFormatRange far& fr, bool render)
{
  return (int)SendMessage(EM_FORMATRANGE, TParam1(render), TParam2(&fr));
}

//
// Frees the Formatting information cached by the RichEdit control...
//
int
TRichEdit::FormatRange()
{
  return (int)SendMessage(EM_FORMATRANGE, TParam1(TRUE), 0);
}

//
// Set the target device and line width used for WYSIWYG (what you see is
// what you get) formatting of the rich edit control.
//
bool
TRichEdit::SetTargetDevice(HDC dc, int lineWidth)
{
  return SendMessage(EM_SETTARGETDEVICE, TParam1(dc),
                       TParam2(lineWidth)) != 0;
}

//
// Force the rich edit control to send an EN_REQUESTRESIZE notification
// message to its parent window.
//
// NOTE: This message is useful during WM_SIZE processing for the parent of a
//       bottomless rich edit control.
//
void
TRichEdit::RequestResize()
{
  SendMessage(EM_REQUESTRESIZE);
}

//
// Retrieve an IRichEditOle object that a client can use to access a rich edit
// control's OLE functionality. Returns 'true' if successful, or false
// otherwise.
//
bool
TRichEdit::GetOleInterface(IRichEditOle far* &pInterface) const
{
  return CONST_CAST(TRichEdit*,this)->SendMessage(EM_GETOLEINTERFACE,
                                0, TParam2((void far* far*)&pInterface)) != 0;
}

//
// Set an IRichEditOleCallback object that the rich edit control uses to get
// OLE-related resources and information from the client. Returns 'true' if
// successful, or false otherwise.
//
bool
TRichEdit::SetOleInterface(IRichEditOleCallback far* /*pInterface*/)
{
#if 0
  // EM_SETOLEINTERFACE: Documented but missing...
  return SendMessage(EM_SETOLEINTERFACE, 0, TParam2(pInterface)) != 0;
#else
  return false;
#endif
}

//
// Retrieve the event mask for the rich edit control. The event mask specifies
// which notification messages the control sends to its parent window.
//
ulong
TRichEdit::GetEventMask() const
{
  return CONST_CAST(TRichEdit*,this)->SendMessage(EM_GETEVENTMASK);
}

//
// Set the event mask for a rich edit control. The event mask specifies which
// notification messages the control sends to its parent window. The 'msk'
// parameter can be zero or more of the following values:
//
//  Value               Meaning
//  -----               -------
//  ENM_CHANGE          Sends EN_CHANGE notifications.
//  ENM_CORRECTTEXT     Sends EN_CORRECTTEXT notifications.
//  ENM_DROPFILES       Sends EN_DROPFILES notifications.
//  ENM_KEYEVENTS       Sends EN_MSGFILTER  notifications for keyboard events.
//  ENM_MOUSEEVENTS     Sends EN_MSGFILTER notifications for mouse events.
//  ENM_PROTECTED       Sends EN_PROTECTED notifications.
//  ENM_RESIZEREQUEST   Sends EN_REQUESTRESIZE notifications.
//  ENM_SCROLL          Sends EN_HSCROLL notifications.
//  ENM_SELCHANGE       Sends EN_SELCHANGE notifications.
//  ENM_UPDATE          Sends EN_UPDATE notifications
//
ulong
TRichEdit::SetEventMask(ulong msk)
{
  return SendMessage(EM_SETEVENTMASK, 0, TParam2(msk));
}

//
// WM_GETDLGCODE handler to bypass TEdit's handler which caters for
// validators.
//
uint
TRichEdit::EvGetDlgCode(MSG far* msg)
{
  return TWindow::EvGetDlgCode(msg);
}

//
// WM_CHAR handler to bypass TEdit's handler which caters for
// validators.
//
void
TRichEdit::EvChar(uint /*key*/, uint /*repeatCount*/, uint /*flags*/)
{
  DefaultProcessing();
}

//
// WM_KEYDOWN handler to bypass TEdit's handler which caters for
// validators.
//
void
TRichEdit::EvKeyDown(uint /*key*/, uint /*repeatCount*/, uint /*flags*/)
{
  DefaultProcessing();
}

//
// WM_KILLFOCUS handler to bypass TEdit's handler which caters for
// validators.
//
void
TRichEdit::EvKillFocus(THandle hGetFocus)
{
  TControl::EvKillFocus(hGetFocus);
}

//
// WM_SETFOCUS handler to bypass TEdit's handler which caters for
// validators.
//
void
TRichEdit::EvSetFocus(THandle /*hLostFocus*/)
{
  DefaultProcessing();
}

//
// This function is called for Cut/Copy/Delete menu items to determine
// whether or not the item is enabled.
//
void
TRichEdit::CeHasSelect(TCommandEnabler& commandHandler)
{
  uint sPos, ePos;

  GetSelection(sPos, ePos);
  commandHandler.Enable(sPos != ePos);
}

//
// This function is called for the Paste menu item to determine whether or
// not the item is enabled.
//
void
TRichEdit::CeEditPaste(TCommandEnabler& ce)
{
  TClipboard clip(*this);
  if (clip.IsClipboardFormatAvailable(CF_TEXT) ||
      clip.IsClipboardFormatAvailable(CF_OEMTEXT))
    ce.Enable(true);
  else
    ce.Enable(false);
}

//
// This function is called for the Clear menu item to determine whether or
// not the item is enabled.
//
void
TRichEdit::CeEditClear(TCommandEnabler& commandHandler)
{
  commandHandler.Enable(!(GetNumLines() == 1 && GetLineLength(0) == 0));
}

//
// This function is called for the Undo menu item to determine whether or
// not the item is enabled.
//
void
TRichEdit::CeEditUndo(TCommandEnabler& commandHandler)
{
  commandHandler.Enable(CanUndo());
}

//
// Return name of predefined Windows edit class
//
char far*
TRichEdit::GetClassName()
{
  return "RICHEDIT";
}

//
// Updates the list of filters describing files which can be opened by
// the rich edit control.
//
//
void
TRichEdit::SetupWindow()
{
  TEditFile::SetupWindow();

  //Load RichEdit's filter
  //
  FileData.SetFilter(GetModule()->LoadString(IDS_RICHEDITFILTER).c_str());
}

//
// Callback used when reading data from a stream into a rich edit control.
//
DWORD CALLBACK
RichEditStrmInWithIStream(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
  PRECONDITION(dwCookie);

  istream& is = *((istream*)dwCookie);

  // Return 0 if transfer is complete
  //
  if (is.eof()) {
   *pcb = 0;
    return 0;
  }

  // Read data in buffer
  //
  is.read(pbBuff, cb);

  // Indicate amount of data read
  //
  *pcb = is.gcount();

  // Docs says to return the amount of bytes read in but that does not seem
  // to be correct!!! Returning 0, on the other side, seems to work - so
  // stick with it until...
  //
  return 0;
}

//
// Callback used when writing out the contents of a rich edit control to
// a data stream.
//
DWORD CALLBACK
RichEditStrmOutWithOstream(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
  PRECONDITION(dwCookie);

  ostream& os = *((ostream*)dwCookie);

  // Save current stream location and write data to buffer
  //
  streampos pCnt = os.tellp();
  os.write(pbBuff, cb);

  // Indicate the number of bytes written to the file
  //
  *pcb = os.tellp() - pCnt;
  return 0;
}

//
// Overriden to bypass TEdit's 'Transfer' method.
// NOTE: There's no transfer-support for rich edit controls.
//
uint
TRichEdit::Transfer(void* /*buffer*/, TTransferDirection /*direction*/)
{
  // NOTE: No transfer support for rich edit control
  //
  return 0;
}

//
// Read the data from the specified stream into the rich edit control. The
// 'fmt' parameter can be one of the following data formats, optionally
// combined with the SFF_SELECTION flag:
//
//    Value               Meaning
//    -----               -------
//    SF_TEXT             Text
//    SF_RTF              Rich-text format
//
// If the SFF_SELECTION flag is specified, the stream replaces the contents of
// the current selection. Otherwise, the stream replaces the entire contents
// of the control.
//
bool
TRichEdit::ReadFromStream(istream& is, uint fmt)
{
  TEditStream edStrm(DWORD((istream*)&is), RichEditStrmInWithIStream);
  StreamIn(fmt, edStrm);
  return edStrm.dwError == 0;
}

//
// Write the contents of the rich edit control to the specified data stream.
// The 'fmt' parameter can be one of the following values, optionally
// combined with the SFF_SELECTION flag:
//
//    Value               Meaning
//    -----               -------
//    SF_TEXT             Text with spaces in place of OLE objects
//    SF_RTF              Rich-text format (RTF)
//    SF_RTFNOOBJS        RTF with spaces in place of OLE object.
//    SF_TEXTIZED         Text with a text representation of OLE objects.
//
// NOTE: The SF_RTFNOOBJS option is useful if an application stores OLE
//       objects itself, as RTF representation of OLE objects is not very
//       compact.
//       If the SFF_SELECTION flag is specified, only the contents of the
//       current selection are streamed out. Otherwise, the entire contents of
//       the control are streamed out.
//
bool
TRichEdit::WriteToStream(ostream& os, uint fmt)
{
  TEditStream edStrm(DWORD((ostream*)&os), RichEditStrmOutWithOstream);
  StreamOut(fmt, edStrm);
  return edStrm.dwError == 0;
}

//
// Read the contents of the specified file in the rich edit control. Returns
// 'true' if successful, or false otherwise.
//
bool
TRichEdit::Read(const char far* fileName)
{
  if (!fileName)
    if (FileName)
      fileName = FileName;
    else
      return false;

  ifstream ifs(fileName, ios::in|ios::binary);
  if (ifs) {
    // Could check for a valid file (eg. FileSize != 0)
    // before proceeding with a call to Clear() here.
    //
    Clear();

    // Stream in data from file
    //
    if (ReadFromStream(ifs, Format)) {
      ClearModify();
      return true;
    }
  }
  return false;
}

//
// Write the contents of the edit control to the specified file. Returns
// 'true' if successful, or false otherwise.
//
bool
TRichEdit::Write(const char far* fileName)
{
  if (!fileName)
    if (FileName)
      fileName = FileName;
    else
      return false;

  ofstream ofs(fileName, ios::out|ios::binary);
  if (ofs) {
    if (WriteToStream(ofs, Format)) {
      ClearModify();
      return true;
    }
  }
  return false;
}

//
// Object wrapper which loads the RichEdit DLL
//
TRichEditModule::TRichEditModule() 
             :TModule(RichEditDllName, true, true)
{
}

