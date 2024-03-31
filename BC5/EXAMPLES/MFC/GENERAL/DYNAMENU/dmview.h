// dmview.h
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1995 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.
//
// Purpose: interface of the CDynaMenuView class
//
// Functions:
//   Most of this file was generated by AppWizard.  The functions
//   which contain code specific to this sample are:
//
//      CDynaMenuView::OnDraw()             -- draw the view contents
//
//   These functions are implemented in dmview.cpp

class CDynaMenuView : public CView
{
protected: // create from serialization only
	CDynaMenuView();
	DECLARE_DYNCREATE(CDynaMenuView)

// Attributes
public:
	CDynaMenuDoc* GetDocument();

// Operations
public:

// Implementation
public:
	virtual ~CDynaMenuView();
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// Printing support
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Generated message map functions
protected:
	//{{AFX_MSG(CDynaMenuView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in dmview.cpp
inline CDynaMenuDoc* CDynaMenuView::GetDocument()
   { return (CDynaMenuDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////
