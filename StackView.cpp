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
// StackView.cpp : implementation file
//

#include "stdafx.h"
#include "CALCViewWin.h"
#include "StackView.h"
#include ".\StackView.h"
#include "CALCViewWinDoc.h"

// CStackView

IMPLEMENT_DYNCREATE(CStackView, CFormView)

CStackView::CStackView()
	: CFormView(CStackView::IDD)
{
	int cols[] = {0};
	m_quicklist.Set_Edit_Columns(cols,1);
}

CStackView::~CStackView()
{
}

void CStackView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STEPS_VIEW, m_quicklist);
}

BEGIN_MESSAGE_MAP(CStackView, CFormView)
//	ON_WM_SIZING()
ON_WM_SIZE()
ON_MESSAGE(WM_QUICKLIST_GETLISTITEMDATA, OnGetListItem)
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)

END_MESSAGE_MAP()


// CStackView diagnostics

#ifdef _DEBUG
void CStackView::AssertValid() const
{
	CFormView::AssertValid();
}

void CStackView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

// CStackView message handlers

BOOL CStackView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Add your specialized code here and/or call the base class
	cs.style |= LVS_OWNERDATA | LVS_REPORT | LVS_EDITLABELS;
	return CFormView::PreCreateWindow(cs);
}

void CStackView::OnInitialUpdate()
{
	CCALCViewWinDoc *pDoc = dynamic_cast<CCALCViewWinDoc*>(GetDocument());
	ASSERT(pDoc);

	CFormView::OnInitialUpdate();
	
	// Delete all columns
	int nColumnCount = m_quicklist.GetHeaderCtrl()->GetItemCount();
	for (int i=0;i < nColumnCount;i++)
		m_quicklist.DeleteColumn(0);
	
	// Set up the style of the list
	//m_quicklist.ModifyStyle(LVS_TYPEMASK, LVS_REPORT, 0);
	ListView_SetExtendedListViewStyle(m_quicklist, LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	
	// Add the headers of the columns
	m_quicklist.InsertColumn(0, "Stack Contents", LVCFMT_LEFT, 128);

	m_quicklist.SetItemCountEx(pDoc->StackGetRowCount(), LVSICF_NOSCROLL);
	m_quicklist.Invalidate();
}

void CStackView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	
	CRect rectView;
	GetWindowRect(&rectView);

	CWnd *wnd = GetWindow(GW_CHILD);
	if (wnd != NULL)
		wnd->MoveWindow(-2, -2, rectView.right - rectView.left + 4, rectView.bottom - rectView.top + 4); 
}

LRESULT CStackView::OnGetListItem(WPARAM wParam, LPARAM lParam)
{
	CCALCViewWinDoc *pDoc = dynamic_cast<CCALCViewWinDoc*>(GetDocument());
	ASSERT(pDoc);
	
	//wParam is a handler to the list
    //Make sure message comes from list box
    ASSERT( (HWND)wParam == m_quicklist.GetSafeHwnd() );

    //lParam is a pointer to the data that 
    //is needed for the element
    CQuickList::CListItemData* data = 
        (CQuickList::CListItemData*) lParam;

    //Get which item and subitem that is asked for.
    int item = data->GetItem();
    int subItem = data->GetSubItem();

    //...insert information that is needed in "data"...
	
	switch (subItem) {
		case 0:		data->m_text = _T(pDoc->StackGetValueAt(item).c_str());	
					data->m_allowEdit = false;
					data->m_textStyle.m_textPosition = DT_CENTER | DT_SINGLELINE | DT_END_ELLIPSIS;
					break;
	};

    return 0;
}
void CStackView::OnUpdate(CView* /*pSender*/, LPARAM lHint, CObject* /*pHint*/)
{
	CCALCViewWinDoc *pDoc = dynamic_cast<CCALCViewWinDoc*>(GetDocument());
	ASSERT(pDoc);

	if (lHint == UPDATE_STACK_TABLE_CONTENTS || lHint == UPDATE_ALL_TABLE_CONTENTS) {
		m_quicklist.Invalidate();
	} else if (lHint == UPDATE_STACK_TABLE_SIZE) {
		m_quicklist.SetItemCountEx(pDoc->StackGetRowCount(), LVSICF_NOSCROLL);
		m_quicklist.Invalidate();
	}

}

void CStackView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: Add your specialized code here and/or call the base class
	
	CCALCViewWinDoc *pDoc = dynamic_cast<CCALCViewWinDoc*>(GetDocument());
	ASSERT(pDoc);
	
	pDoc->OnPrint(pDC, pInfo);
}

BOOL CStackView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CStackView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CStackView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

