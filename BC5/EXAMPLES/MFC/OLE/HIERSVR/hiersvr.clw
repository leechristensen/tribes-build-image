; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CServerView
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "hiersvr.h"
VbxHeaderFile=hiersvr.h
VbxImplementationFile=hiersvr.cpp
LastPage=0

ClassCount=9
Class1=CServerApp
Class2=CAboutDlg
Class3=CInPlaceFrame
Class4=CMainFrame
Class5=CServerDoc
Class6=CChangeNameDlg
Class7=CAddNodeDlg
Class8=CServerView
Class9=CZoomDlg

ResourceCount=9
Resource1=IDR_HIERSVRTYPE
Resource2=IDD_CUSTZOOM
Resource3=IDR_MAINFRAME
Resource4=IDR_HIERSVRTYPE_INPLACE
Resource5=IDR_HIERSVRTYPE_EMBEDDED
Resource6=IDD_CHANGE_NAME
Resource7=IDD_ABOUTBOX
Resource8=IDD_ADD_NODE
Resource9=IDR_POPUPS

[CLS:CServerApp]
Type=0
HeaderFile=hiersvr.h
ImplementationFile=hiersvr.cpp

[CLS:CAboutDlg]
Type=0
HeaderFile=hiersvr.cpp
ImplementationFile=hiersvr.cpp

[CLS:CInPlaceFrame]
Type=0
HeaderFile=ipframe.h
ImplementationFile=ipframe.cpp

[CLS:CMainFrame]
Type=0
HeaderFile=mainfrm.h
ImplementationFile=mainfrm.cpp

[CLS:CServerDoc]
Type=0
HeaderFile=svrdoc.h
ImplementationFile=svrdoc.cpp

[CLS:CChangeNameDlg]
Type=0
HeaderFile=svritem.cpp
ImplementationFile=svritem.cpp

[CLS:CAddNodeDlg]
Type=0
HeaderFile=svritem.cpp
ImplementationFile=svritem.cpp

[CLS:CServerView]
Type=0
HeaderFile=svrview.h
ImplementationFile=svrview.cpp
LastObject=ID_TREE_EXPANDBRANCH

[CLS:CZoomDlg]
Type=0
HeaderFile=zoomdlg.h
ImplementationFile=zoomdlg.cpp

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDR_MAINFRAME,button,1342373889

[DLG:IDD_CHANGE_NAME]
Type=1
Class=CChangeNameDlg
ControlCount=8
Control1=IDC_STATIC,static,1342308354
Control2=IDC_EDIT1,edit,1350631552
Control3=IDC_STATIC,static,1342308354
Control4=IDC_EDIT2,edit,1350631552
Control5=IDC_STATIC,static,1342308354
Control6=IDC_COMBO1,combobox,1344339971
Control7=IDOK,button,1342242816
Control8=IDCANCEL,button,1342242816

[DLG:IDD_ADD_NODE]
Type=1
Class=CAddNodeDlg
ControlCount=8
Control1=IDC_STATIC,static,1342308354
Control2=IDC_EDIT1,edit,1350631552
Control3=IDC_STATIC,static,1342308354
Control4=IDC_EDIT2,edit,1350631552
Control5=IDC_STATIC,static,1342308354
Control6=IDC_COMBO1,combobox,1344339971
Control7=IDOK,button,1342242816
Control8=IDCANCEL,button,1342242816

[DLG:IDD_CUSTZOOM]
Type=1
Class=CZoomDlg
ControlCount=7
Control1=IDC_STATIC,button,1342177287
Control2=IDC_STATIC,static,1342308352
Control3=IDC_EDIT2,edit,1350631552
Control4=IDC_STATIC,static,1342308352
Control5=IDC_EDIT1,edit,1350631552
Control6=IDOK,button,1342242817
Control7=IDCANCEL,button,1342242816

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_MRU_FILE1
Command4=ID_APP_EXIT
Command5=ID_VIEW_TOOLBAR
Command6=ID_VIEW_STATUS_BAR
Command7=ID_HELP_INDEX
Command8=ID_HELP_USING
Command9=ID_CONTEXT_HELP
Command10=ID_APP_ABOUT
CommandCount=10

[MNU:IDR_HIERSVRTYPE]
Type=1
Class=CServerView
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_CLOSE
Command4=ID_FILE_SAVE
Command5=ID_FILE_SAVE_AS
Command6=ID_FILE_MRU_FILE1
Command7=ID_APP_EXIT
Command8=ID_EDIT_CUT
Command9=ID_EDIT_COPY
Command10=ID_EDIT_PASTE
Command11=ID_EDIT_CLEAR
Command12=ID_CHANGE_NAME
Command13=ID_ADD_NODE
Command14=ID_IMPORT_TEXT
Command15=ID_TREE_EXPANDONELEVEL
Command16=ID_TREE_EXPANDBRANCH
Command17=ID_TREE_EXPANDALL
Command18=ID_TREE_COLLAPSEBRANCH
Command19=ID_OPTIONS_FONT
Command20=ID_VIEW_TOOLBAR
Command21=ID_VIEW_STATUS_BAR
Command22=ID_VIEW_ZOOM25
Command23=ID_VIEW_ZOOM50
Command24=ID_VIEW_ZOOM75
Command25=ID_VIEW_ZOOM100
Command26=ID_VIEW_ZOOM150
Command27=ID_VIEW_ZOOM200
Command28=ID_VIEW_ZOOM300
Command29=ID_VIEW_ZOOMCUSTOM
Command30=ID_WINDOW_NEW
Command31=ID_WINDOW_CASCADE
Command32=ID_WINDOW_TILE_HORZ
Command33=ID_WINDOW_ARRANGE
Command34=ID_HELP_INDEX
Command35=ID_HELP_USING
Command36=ID_CONTEXT_HELP
Command37=ID_APP_ABOUT
CommandCount=37

[MNU:IDR_POPUPS]
Type=1
Class=CServerView
Command1=ID_EDIT_CUT
Command2=ID_EDIT_COPY
Command3=ID_EDIT_PASTE
Command4=ID_EDIT_CLEAR
Command5=ID_CHANGE_NAME
Command6=ID_ADD_NODE
CommandCount=6

[MNU:IDR_HIERSVRTYPE_EMBEDDED]
Type=1
Class=CServerView
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_CLOSE
Command4=ID_FILE_UPDATE
Command5=ID_FILE_SAVE_COPY_AS
Command6=ID_FILE_MRU_FILE1
Command7=ID_APP_EXIT
Command8=ID_EDIT_CUT
Command9=ID_EDIT_COPY
Command10=ID_EDIT_PASTE
Command11=ID_EDIT_CLEAR
Command12=ID_CHANGE_NAME
Command13=ID_ADD_NODE
Command14=ID_IMPORT_TEXT
Command15=ID_TREE_EXPANDONELEVEL
Command16=ID_TREE_EXPANDBRANCH
Command17=ID_TREE_EXPANDALL
Command18=ID_TREE_COLLAPSEBRANCH
Command19=ID_OPTIONS_FONT
Command20=ID_VIEW_TOOLBAR
Command21=ID_VIEW_STATUS_BAR
Command22=ID_VIEW_ZOOM25
Command23=ID_VIEW_ZOOM50
Command24=ID_VIEW_ZOOM75
Command25=ID_VIEW_ZOOM100
Command26=ID_VIEW_ZOOM150
Command27=ID_VIEW_ZOOM200
Command28=ID_VIEW_ZOOM300
Command29=ID_VIEW_ZOOMCUSTOM
Command30=ID_WINDOW_NEW
Command31=ID_WINDOW_CASCADE
Command32=ID_WINDOW_TILE_HORZ
Command33=ID_WINDOW_ARRANGE
Command34=ID_HELP_INDEX
Command35=ID_HELP_USING
Command36=ID_APP_ABOUT
CommandCount=36

[MNU:IDR_HIERSVRTYPE_INPLACE]
Type=1
Class=CServerView
Command1=ID_EDIT_CUT
Command2=ID_EDIT_COPY
Command3=ID_EDIT_PASTE
Command4=ID_EDIT_CLEAR
Command5=ID_CHANGE_NAME
Command6=ID_ADD_NODE
Command7=ID_IMPORT_TEXT
Command8=ID_TREE_EXPANDONELEVEL
Command9=ID_TREE_EXPANDBRANCH
Command10=ID_TREE_EXPANDALL
Command11=ID_TREE_COLLAPSEBRANCH
Command12=ID_OPTIONS_FONT
Command13=ID_HELP_INDEX
Command14=ID_HELP_USING
Command15=ID_APP_ABOUT
CommandCount=15

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_TREE_EXPANDBRANCH
Command2=ID_TREE_EXPANDONELEVEL
Command3=ID_TREE_COLLAPSEBRANCH
Command4=ID_TREE_EXPANDALL
Command5=ID_EDIT_COPY
Command6=ID_FILE_NEW
Command7=ID_FILE_OPEN
Command8=ID_FILE_SAVE
Command9=ID_EDIT_PASTE
Command10=ID_EDIT_CLEAR
Command11=ID_EDIT_CUT
Command12=ID_HELP
Command13=ID_CONTEXT_HELP
Command14=ID_EDIT_COPY
Command15=ID_EDIT_PASTE
Command16=ID_EDIT_CUT
CommandCount=16

[ACL:IDR_HIERSVRTYPE_EMBEDDED]
Type=1
Class=CServerView
Command1=ID_TREE_EXPANDBRANCH
Command2=ID_TREE_EXPANDONELEVEL
Command3=ID_TREE_COLLAPSEBRANCH
Command4=ID_TREE_EXPANDALL
Command5=ID_EDIT_COPY
Command6=ID_FILE_NEW
Command7=ID_FILE_OPEN
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_CLEAR
Command10=ID_EDIT_CUT
Command11=ID_HELP
Command12=ID_CONTEXT_HELP
Command13=ID_EDIT_COPY
Command14=ID_EDIT_PASTE
Command15=ID_EDIT_CUT
CommandCount=15

[ACL:IDR_HIERSVRTYPE_INPLACE]
Type=1
Class=CServerView
Command1=ID_TREE_EXPANDBRANCH
Command2=ID_TREE_EXPANDONELEVEL
Command3=ID_TREE_COLLAPSEBRANCH
Command4=ID_TREE_EXPANDALL
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_EDIT_CLEAR
Command8=ID_EDIT_CUT
Command9=ID_HELP
Command10=ID_CONTEXT_HELP
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_EDIT_CUT
CommandCount=13

