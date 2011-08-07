/*
* CALCView is a Calc Block Editor
* Copyright (C) 2003
*
* Created by Tod Baudais
*
* This file is part of CALCView.
*
* CALCView is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as
* published by the Free Software Foundation, either version 3 of
* the License, or (at your option) any later version.
*
* CALCView is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with CALCView.  If not, see <http://www.gnu.org/licenses/>.
*/
// CALCViewWinView.cpp : implementation of the CCALCViewWinView class
//

#include "stdafx.h"
#include "CALCViewWin.h"

#include "CALCViewWinDoc.h"
#include "CALCViewWinView.h"
//#include ".\calcviewwinview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CCALCViewWinView

IMPLEMENT_DYNCREATE(CCALCViewWinView, CView)

BEGIN_MESSAGE_MAP(CCALCViewWinView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
//	ON_WM_TIMER()
//ON_UPDATE_COMMAND_UI(ID_FILE_PRINT, OnUpdateFilePrint)
END_MESSAGE_MAP()

// CCALCViewWinView construction/destruction

CCALCViewWinView::CCALCViewWinView()
{
	CCALCViewWinDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

}

CCALCViewWinView::~CCALCViewWinView()
{
}

BOOL CCALCViewWinView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CCALCViewWinView drawing

void CCALCViewWinView::OnDraw(CDC* /*pDC*/)
{
	CCALCViewWinDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CCALCViewWinView printing

BOOL CCALCViewWinView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CCALCViewWinView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CCALCViewWinView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CCALCViewWinView diagnostics

#ifdef _DEBUG
void CCALCViewWinView::AssertValid() const
{
	CView::AssertValid();
}

void CCALCViewWinView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCALCViewWinDoc* CCALCViewWinView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCALCViewWinDoc)));
	return (CCALCViewWinDoc*)m_pDocument;
}
#endif //_DEBUG


// CCALCViewWinView message handlers

BOOL CCALCViewWinView::OnCmdMsg	(UINT nID, int nCode, void *pExtra, AFX_CMDHANDLERINFO *pHandlerInfo)
{
	return CView::OnCmdMsg	(nID, nCode, pExtra, pHandlerInfo);
}

