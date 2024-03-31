//----------------------------------------------------------------------------
// ObjectWindows
// Copyright (c) 1993, 1995 by Borland International, All Rights Reserved
//
//   Defines class TODListBox
//----------------------------------------------------------------------------
#if !defined(OWL_ODLISTBX_H)
#define OWL_ODLISTBX_H

#if !defined(OWL_CONTROL_H)
# include <owl/control.h>
#endif

class TODListBox : public TControl {
  public:

    struct ODItemInfo { // temporary information about current item
                   // the following are supplied to the GetItemInfo function:
      HDC    Hdc;       // display context for listbox
      int    Index;     // index of current item, set by Draw/MeasureItem
      void far* Data;   // user item data value,  set by Draw/MeasureItem
      uint   State;     // ODS_xxxx flags for new state, set by Draw/MeasureItem
      TRect  Bound;     // actual drawing area bounds
                   // the following are to be set by the GetItemInfo function:
      TSize  Extent;    // cell size to display entire data
      TPoint Offset;    // offset in rect of drawn data
      char far* Text;   // pointer to text data to draw
      int    TextLen;   // length of text data
    };     

    TODListBox(int id);
   ~TODListBox();

    // List box wrappers
    //
    virtual void  ClearList();
    virtual int   GetCount() const;
            int   GetTopIndex() const;
            int   SetTopIndex(int index);
            int   GetHorizontalExtent() const;
            void  SetHorizontalExtent(int horzExtent);
    virtual int   GetStringLen(int index) const;
    virtual int   GetString(char far* str, int index) const;
    virtual uint32 GetItemData(int index) const;
    virtual int   SetItemData(int index, uint32 itemData);
            int   GetItemRect(int index, TRect& rect) const;
    virtual int   GetItemHeight(int index) const;
    virtual int   SetItemHeight(int index, int height);
    virtual int   AddString(const char far* str);
    virtual int   InsertString(const char far* str, int index);
    virtual int   DeleteString(int index);
    virtual int   GetSelIndex() const;
    virtual int   SetSelIndex(int index);

    //
    // Override TWindow virtual member functions
    //
    char far     *GetClassName();

    virtual void ItemRedraw(int index);   // force item to be redrawn
  protected:
    int       MaxWidth; // maximum horizontal extent
    char far* BufTemp;  // temporary buffer for returning strings
    int       BufLen;      // allocated length of temporary buffer
   
    // Calls eminating from TControl for WM_DRAWITEM and WM_MEASUREITEM
    //
    virtual void  DrawItem (DRAWITEMSTRUCT far &);
    virtual void  MeasureItem (MEASUREITEMSTRUCT far &);

    // Calls from TODListBox::DrawItem() which must be overridden if not text
    //
    virtual bool GetItemInfo  (ODItemInfo& item); // fill Offset, Text, TextLen
    virtual void ChangeHilight(ODItemInfo& item);
    virtual void ChangeFocus  (ODItemInfo& item);
    virtual void DrawItemData (ODItemInfo& item);
  private:

    // Hidden to prevent accidental copying or assignment
    //
    TODListBox(const TODListBox&);
    TODListBox& operator=(const TODListBox&);

  DECLARE_STREAMABLE(, TODListBox, 1);
};

//----------------------------------------------------------------------------
// Inlines
//

inline TODListBox::~TODListBox() {
  if (BufTemp) delete BufTemp;
}

inline int TODListBox::GetTopIndex() const {
  return (int)CONST_CAST(TODListBox*,this)->SendMessage(LB_GETTOPINDEX);
}

inline int TODListBox::SetTopIndex(int index) {
  return (int)SendMessage(LB_SETTOPINDEX, index);
}

inline int TODListBox::GetHorizontalExtent() const {
  return (int)CONST_CAST(TODListBox*,this)->SendMessage(LB_GETHORIZONTALEXTENT);
}

inline void TODListBox::SetHorizontalExtent(int horzExtent) {
  SendMessage(LB_SETHORIZONTALEXTENT, horzExtent);
}

inline uint32 TODListBox::GetItemData(int index) const {
  return CONST_CAST(TODListBox*,this)->SendMessage(LB_GETITEMDATA, index);
}

inline int TODListBox::SetItemData(int index, uint32 itemData) {
  return (int)SendMessage(LB_SETITEMDATA, index, itemData);
}

inline int TODListBox::GetItemRect(int index, TRect& rect) const {
  return (int)CONST_CAST(TODListBox*,this)->
           SendMessage(LB_GETITEMRECT, index, TParam2((TRect FAR*)&rect));
}

inline int TODListBox::GetItemHeight(int index) const {
  return (int)CONST_CAST(TODListBox*,this)->
           SendMessage(LB_GETITEMHEIGHT, index);
}

inline int TODListBox::SetItemHeight(int index, int height) {
  return (int)SendMessage(LB_SETITEMHEIGHT, index, MkParam2(height, 0));
}

//
// Adds a string to an associated listbox
// Returns index of the string in the list(the first entry is at index 0),
// a negative if an error occurs.
//
inline int TODListBox::AddString(const char far* str) {
  return (int)SendMessage(LB_ADDSTRING, 0, TParam2(str));
}

//
// Inserts a string in the associated listbox at the passed index,
// returns the index of the string in the list, a negative if an error occurs
//
inline int TODListBox::InsertString(const char far* str, int index) {
  return (int)SendMessage(LB_INSERTSTRING, index, TParam2(str));
}

//
// Deletes the string at the passed index in the associated listbox
// Returns a count of the entries remaining in the list, a negative
// value if an error occurs
//
inline int TODListBox::DeleteString(int index) {
  return (int)SendMessage(LB_DELETESTRING, index);
}

//
// Clears all the entries in the associated listbox
//
inline void TODListBox::ClearList() {
  SendMessage(LB_RESETCONTENT);
}

//
// Returns the number of entries in the associated listbox, a negative
// value if an error occurs
//
inline int TODListBox::GetCount() const {
  return (int)CONST_CAST(TODListBox*,this)->SendMessage(LB_GETCOUNT);
}

//
// Retrieves the contents of the string at the passed index of the
// associated listbox. Returns the length of the string (in bytes
// excluding the terminating 0),  a negative if the passed index is not valid
//
// The buffer must be large enough for the string and the terminating 0
//
inline int TODListBox::GetString(char far* str, int index) const {
  return (int)CONST_CAST(TODListBox*,this)->
           SendMessage(LB_GETTEXT, index, TParam2(str));
}

//
// Returns the length of the string at the passed index in the associated
//
// listbox excluding the terminating 0, a negative if an error occurs
//
inline int TODListBox::GetStringLen(int index) const {
  return (int)CONST_CAST(TODListBox*,this)->SendMessage(LB_GETTEXTLEN, index);
}

inline int TODListBox::GetSelIndex() const {
  return (int)CONST_CAST(TODListBox*,this)->SendMessage(LB_GETCURSEL);
}

//
// selects the string at passed index in the associated listbox and
// forces the string into view
//
// clears selection when -1 is passed as the index. a negative value is
// returned if an error occurs
//
inline int TODListBox::SetSelIndex(int index) {
  return (int)SendMessage(LB_SETCURSEL, index);
}

#endif  // OWL_ODLISTBX_H