//----------------------------------------------------------------------------
// ObjectWindows
// Copyright (c) 1992, 1997 by Borland International, All Rights Reserved
//
//$Revision:   10.6  $
//
// Implementation of class TEventHandler
//----------------------------------------------------------------------------
#include <owl/pch.h>
#if !defined(OWL_EVENTHAN_H)
# include <owl/eventhan.h>
#endif
#include <stdlib.h>

OWL_DIAGINFO;
DIAG_DEFINE_GROUP_INIT(OWL_INI, OwlMsg, 1, 0);
                             // diagnostic group for message tracing

#if defined(__TRACE) || defined(__WARN)

#if defined(BI_NAMESPACE)
namespace OWL {
#endif

  struct TWMStr {
    uint    msg;
    char*   str;
  };

  static  TWMStr  StrTab[] = {
    { 0x0000, "WM_NULL"                  },
    { 0x0001, "WM_CREATE"                },
    { 0x0002, "WM_DESTROY"               },
    { 0x0003, "WM_MOVE"                  },
    { 0x0004, "wm_sizewait"              },
    { 0x0005, "WM_SIZE"                  },
    { 0x0006, "WM_ACTIVATE"              },
    { 0x0007, "WM_SETFOCUS"              },
    { 0x0008, "WM_KILLFOCUS"             },
    { 0x0009, "wm_setvisible"            },
    { 0x000A, "WM_ENABLE"                },
    { 0x000B, "WM_SETREDRAW"             },
    { 0x000C, "WM_SETTEXT"               },
    { 0x000D, "WM_GETTEXT"               },
    { 0x000E, "WM_GETTEXTLENGTH"         },
    { 0x000F, "WM_PAINT"                 },
    { 0x0010, "WM_CLOSE"                 },
    { 0x0011, "WM_QUERYENDSESSION"       },
    { 0x0012, "WM_QUIT"                  },
    { 0x0013, "WM_QUERYOPEN"             },
    { 0x0014, "WM_ERASEBKGND"            },
    { 0x0015, "WM_SYSCOLORCHANGE"        },
    { 0x0016, "WM_ENDSESSION"            },
    { 0x0017, "wm_systemerror"           },
    { 0x0018, "WM_SHOWWINDOW"            },
    { 0x0019, "WM_CTLCOLOR"              },
    { 0x001A, "WM_SETTINGCHANGE"         },
    { 0x001B, "WM_DEVMODECHANGE"         },
    { 0x001C, "WM_ACTIVATEAPP"           },
    { 0x001D, "WM_FONTCHANGE"            },
    { 0x001E, "WM_TIMECHANGE"            },
    { 0x001F, "WM_CANCELMODE"            },
    { 0x0020, "WM_SETCURSOR"             },
    { 0x0021, "WM_MOUSEACTIVATE"         },
    { 0x0022, "WM_CHILDACTIVATE"         },
    { 0x0023, "WM_QUEUESYNC"             },
    { 0x0024, "WM_GETMINMAXINFO"         },
   // 0x0025
    { 0x0026, "WM_PAINTICON"             },
    { 0x0027, "WM_ICONERASEBKGND"        },
    { 0x0028, "WM_NEXTDLGCTL"            },
    { 0x0029, "wm_alttabactive"          },
    { 0x002A, "WM_SPOOLERSTATUS"         },
    { 0x002B, "WM_DRAWITEM"              },
    { 0x002C, "WM_MEASUREITEM"           },
    { 0x002D, "WM_DELETEITEM"            },
    { 0x002E, "WM_VKEYTOITEM"            },
    { 0x002F, "WM_CHARTOITEM"            },
    { 0x0030, "WM_SETFONT"               },
    { 0x0031, "WM_GETFONT"               },
    { 0x0032, "WM_SETHOTKEY"             },
    { 0x0033, "WM_GETHOTKEY"             },
    { 0x0034, "wm_filesyschange"         },
    { 0x0035, "wm_isactiveicon"          },
    { 0x0036, "wm_queryparkicon"         },
    { 0x0037, "WM_QUERYDRAGICON"         },
    { 0x0038, "wm_querysavestate"        },
    { 0x0039, "WM_COMPAREITEM"           },
    { 0x0040, "wm_testing"               },
    { 0x0041, "WM_COMPACTING"            },
    { 0x0042, "wm_otherwindowcreated"    },
    { 0x0043, "wm_otherwindowdestroyed"  },
    { 0x0044, "WM_COMMNOTIFY"            },
    { 0x0045, "wm_hotkeyevent"           },
    { 0x0046, "WM_WINDOWPOSCHANGING"     },
    { 0x0047, "WM_WINDOWPOSCHANGED"      },
    { 0x0048, "WM_POWER"                 },
    { 0x004A, "WM_COPYDATA"              },
    { 0x004B, "WM_CANCELJOURNAL"         },
   // 0x004C
   // 0x004D
    { 0x004E, "WM_NOTIFY"                },
   // 0x004F
    { 0x0050, "WM_INPUTLANGCHANGEREQUEST"},
    { 0x0051, "WM_INPUTLANGCHANGE"       },
    { 0x0052, "WM_TCARD"                 },
    { 0x0053, "WM_HELP"                  },
    { 0x0054, "WM_USERCHANGED"           },
    { 0x0055, "WM_NOTIFYFORMAT"          },
   // ...
    { 0x007B, "WM_CONTEXTMENU"           },
    { 0x007C, "WM_STYLECHANGING"         },
    { 0x007D, "WM_STYLECHANGED"          },
    { 0x007E, "WM_DISPLAYCHANGED"        },
    { 0x007F, "WM_GETICON"               },
    { 0x0080, "WM_SETICON"               },
    { 0x0081, "WM_NCCREATE"              },
    { 0x0082, "WM_NCDESTROY"             },
    { 0x0083, "WM_NCCALCSIZE"            },
    { 0x0084, "WM_NCHITTEST"             },
    { 0x0085, "WM_NCPAINT"               },
    { 0x0086, "WM_NCACTIVATE"            },
    { 0x0087, "WM_GETDLGCODE"            },
    { 0x0088, "wm_syncpaint"             },
    { 0x0089, "wm_synctask"              },
    { 0x00A0, "WM_NCMOUSEMOVE"           },
    { 0x00A1, "WM_NCLBUTTONDOWN"         },
    { 0x00A2, "WM_NCLBUTTONUP"           },
    { 0x00A3, "WM_NCLBUTTONDBLCLK"       },
    { 0x00A4, "WM_NCRBUTTONDOWN"         },
    { 0x00A5, "WM_NCRBUTTONUP"           },
    { 0x00A6, "WM_NCRBUTTONDBLCLK"       },
    { 0x00A7, "WM_NCMBUTTONDOWN"         },
    { 0x00A8, "WM_NCMBUTTONUP"           },
    { 0x00A9, "WM_NCMBUTTONDBLCLK"       },
   // ...
    { 0x0100, "WM_KEYDOWN"               },
    { 0x0101, "WM_KEYUP"                 },
    { 0x0102, "WM_CHAR"                  },
    { 0x0103, "WM_DEADCHAR"              },
    { 0x0104, "WM_SYSKEYDOWN"            },
    { 0x0105, "WM_SYSKEYUP"              },
    { 0x0106, "WM_SYSCHAR"               },
    { 0x0107, "WM_SYSDEADCHAR"           },
    { 0x0108, "wm_yomichar"              },
   // ...
    { 0x010A, "wm_convertrequest"        },
    { 0x010B, "wm_convertresult"         },
    { 0x010C, "wm_interim"               },
    { 0x010D, "WM_IME_STARTCOMPOSITION"  },
    { 0x010E, "WM_IME_ENDCOMPOSITION"    },
    { 0x010F, "WM_IME_COMPOSITION"       },
    { 0x010F, "WM_IME_KEYLAST"           },
    { 0x0110, "WM_INITDIALOG"            },
    { 0x0111, "WM_COMMAND"               },
    { 0x0112, "WM_SYSCOMMAND"            },
    { 0x0113, "WM_TIMER"                 },
    { 0x0114, "WM_HSCROLL"               },
    { 0x0115, "WM_VSCROLL"               },
    { 0x0116, "WM_INITMENU"              },
    { 0x0117, "WM_INITMENUPOPUP"         },
    { 0x0118, "wm_systimer"              },
   // ...
    { 0x011F, "WM_MENUSELECT"            },
    { 0x0120, "WM_MENUCHAR"              },
    { 0x0121, "WM_ENTERIDLE"             },
    { 0x0131, "wm_lbtrackpoint"          },
    { 0x0132, "WM_CTLCOLORMSGBOX"        },
    { 0x0133, "WM_CTLCOLOREDIT"          },
    { 0x0134, "WM_CTLCOLORLISTBOX"       },
    { 0x0135, "WM_CTLCOLORBTN"           },
    { 0x0136, "WM_CTLCOLORDLG"           },
    { 0x0137, "WM_CTLCOLORSCROLLBAR"     },
    { 0x0138, "WM_CTLCOLORSTATIC"        },
   // ...
    { 0x0200, "WM_MOUSEMOVE"             },
    { 0x0201, "WM_LBUTTONDOWN"           },
    { 0x0202, "WM_LBUTTONUP"             },
    { 0x0203, "WM_LBUTTONDBLCLK"         },
    { 0x0204, "WM_RBUTTONDOWN"           },
    { 0x0205, "WM_RBUTTONUP"             },
    { 0x0206, "WM_RBUTTONDBLCLK"         },
    { 0x0207, "WM_MBUTTONDOWN"           },
    { 0x0208, "WM_MBUTTONUP"             },
    { 0x0209, "WM_MBUTTONDBLCLK"         },
    { 0x0210, "WM_PARENTNOTIFY"          },
    { 0x0211, "WM_ENTERMENULOOP"         },
    { 0x0212, "WM_EXITMENULOOP"          },
    { 0x0213, "WM_NEXTMENU"              },
    { 0x0214, "WM_SIZING"                },
    { 0x0215, "WM_CAPTURECHANGED"        },
    { 0x0216, "WM_MOVING"                },
    { 0x0218, "WM_POWERBROADCAST"        },
    { 0x0219, "WM_DEVICECHANGE"          },
   // ...
    { 0x0220, "WM_MDICREATE"             },
    { 0x0221, "WM_MDIDESTROY"            },
    { 0x0222, "WM_MDIACTIVATE"           },
    { 0x0223, "WM_MDIRESTORE"            },
    { 0x0224, "WM_MDINEXT"               },
    { 0x0225, "WM_MDIMAXIMIZE"           },
    { 0x0226, "WM_MDITILE"               },
    { 0x0227, "WM_MDICASCADE"            },
    { 0x0228, "WM_MDIICONARRANGE"        },
    { 0x0229, "WM_MDIGETACTIVE"          },
    { 0x022A, "wm_dropobject"            },
    { 0x022B, "wm_querydropobject"       },
    { 0x022C, "wm_begindrag"             },
    { 0x022D, "wm_dragloop"              },
    { 0x022E, "wm_dragselect"            },
    { 0x022F, "wm_dragmove"              },
    { 0x0230, "WM_MDISETMENU"            },
    { 0x0231, "WM_ENTERSIZEMOVE"         },
    { 0x0232, "WM_EXITSIZEMOVE"          },
    { 0x0233, "WM_DROPFILES"             },
    { 0x0234, "WM_MDIREFRESHMENU"        },
   // ...
    { 0x0281, "WM_IME_SETCONTEXT"        },
    { 0x0282, "WM_IME_NOTIFY"            },
    { 0x0283, "WM_IME_CONTROL"           },
    { 0x0284, "WM_IME_COMPOSITIONFULL"   },
    { 0x0285, "WM_IME_SELECT"            },
    { 0x0286, "WM_IME_CHAR"              },
    { 0x0290, "WM_IME_KEYDOWN"           },
    { 0x0291, "WM_IME_KEYUP"             },
   // ...
    { 0x0300, "WM_CUT"                   },
    { 0x0301, "WM_COPY"                  },
    { 0x0302, "WM_PASTE"                 },
    { 0x0303, "WM_CLEAR"                 },
    { 0x0304, "WM_UNDO"                  },
    { 0x0305, "WM_RENDERFORMAT"          },
    { 0x0306, "WM_RENDERALLFORMATS"      },
    { 0x0307, "WM_DESTROYCLIPBOARD"      },
    { 0x0308, "WM_DRAWCLIPBOARD"         },
    { 0x0309, "WM_PAINTCLIPBOARD"        },
    { 0x030A, "WM_VSCROLLCLIPBOARD"      },
    { 0x030B, "WM_SIZECLIPBOARD"         },
    { 0x030C, "WM_ASKCBFORMATNAME"       },
    { 0x030D, "WM_CHANGECBCHAIN"         },
    { 0x030E, "WM_HSCROLLCLIPBOARD"      },
    { 0x030F, "WM_QUERYNEWPALETTE"       },
    { 0x0310, "WM_PALETTEISCHANGING"     },
    { 0x0311, "WM_PALETTECHANGED"        },
    { 0x0312, "WM_HOTKEY"                },
   // ...
    { 0x0317, "WM_PRINT"                 },
    { 0x0318, "WM_PRINTCLIENT"           },
   // ...
    { 0x0358, "WM_HANDHELDFIRST"         },
   // ...
    { 0x035F, "WM_HANDHELDLAST"          },
    { 0x0360, "WM_AFXFIRST"              },
   // ...
    { 0x037F, "WM_AFXLAST"               },
    { 0x0380, "WM_PENWINFIRST"           },
   // ...
    { 0x038F, "WM_PENWINLAST"            },
   // ...
    { 0x0400, "WM_USER"                  },
   // ...
    { 0x8000, "WM_APP"                   },
  };

  //
  // MsgCompare: this internal function is used by the qsort() and
  // bsearch() functions to compare two StrTab elements.
  //
  static int
  MsgCompare(const void* e1, const void* e2)
  {
    return ((TWMStr*)e1)->msg - ((TWMStr*)e2)->msg;
  }

  ostream& operator <<(ostream& os, const MsgName& msg)
  {
    TWMStr  key;
    TWMStr* item;

    // if it's a user message, then just show the offset value
    if (msg.Message >= WM_USER)
      return os << "WM_USER+" << hex << (msg.Message - WM_USER);

    // otherwise, search for it in the table
    else {
      // initialize the search key
      key.msg = msg.Message;

      // let bsearch() do the work
      item = (TWMStr*)bsearch(&key, StrTab, COUNTOF(StrTab), sizeof(TWMStr), MsgCompare);

      // if message found, then insert the name
      if (item)
        return os << item->str;

      // otherwise, just insert the message number
      else
        return os << "Message " << hex << msg.Message;
    }
  }

#if defined(BI_NAMESPACE)
} // namespace OWL
#endif

#endif // __TRACE || __WARN

//
// Searches the list of response table entries looking for a match
//
// Since class TEventHandler doesn't have any entries, this default
// implementation just returns false
//
bool
TEventHandler::Find(TEventInfo&, TEqualOperator)
{
  return false;
}

//
// Dispatch a message to an event handler given a found response table entry
//
TResult
TEventHandler::Dispatch(TEventInfo& eventInfo, TParam1 param1, TParam2 param2)
{
  PRECONDITION(eventInfo.Entry);
  return (*eventInfo.Entry->Dispatcher)(
           *eventInfo.Object,
           (TAnyPMF __RTFAR&)eventInfo.Entry->Pmf,
           param1, param2
         );
}

//
// Search for the event given the message and it and dispatch to the
// event handler if found.
//
TResult
TEventHandler::DispatchMsg(uint msg, uint id, TParam1 p1, TParam2 p2)
{
  TEventInfo eventInfo(msg, id);
  if (Find(eventInfo))
    return Dispatch(eventInfo, p1, p2);
  return 0;
}

//
// Low-level response table search function. Allows optional equal operator
// function to be provided
//
// Fills in Entry member of eventInfo and returns true if found.
//
bool
TEventHandler::SearchEntries(TGenericTableEntry __RTFAR* entries,
                             TEventInfo&         eventInfo,
                             TEqualOperator      equal)
{
  if (equal) {
    while (entries->Dispatcher != 0) {
      if (equal(*entries, eventInfo)) {
        eventInfo.Entry = entries;
        return true;
      }
      entries++;
    }
  }
  else {
    while (entries->Dispatcher != 0) {
      if (entries->Msg == eventInfo.Msg && entries->Id == eventInfo.Id) {
        eventInfo.Entry = entries;
        return true;
      }
      entries++;
    }
  }

  return false;
}