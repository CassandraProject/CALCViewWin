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
// StepsView.cpp : implementation file
//

#include "stdafx.h"
#include "CALCViewWin.h"
#include "StepsView.h"
#include "CALCViewWinDoc.h"

#include <sstream>
#include <vector>
#include <functional>
#include <algorithm>
#include ".\stepsview.h"

// CStepsView

IMPLEMENT_DYNCREATE(CStepsView, CFormView)

CStepsView::CStepsView()
	: CFormView(CStepsView::IDD)
{
	// Load the image list
	CWinApp* pApp= AfxGetApp();
	
	// Create a masked image list large enough to hold the icons. 
    m_imagelist.Create(16, 16, ILC_COLOR8, 6, 0); 

    // Load the icon resources, and add the icons to the image list. 
    m_dot_error_icon = m_imagelist.Add(pApp->LoadIcon(IDR_ICON_DOT_ERROR));
    m_dot_active_icon = m_imagelist.Add(pApp->LoadIcon(IDR_ICON_DOT_ACTIVE)); 
    m_dot_inactive_icon = m_imagelist.Add(pApp->LoadIcon(IDR_ICON_DOT_INACTIVE));  
    m_stop_active_icon = m_imagelist.Add(pApp->LoadIcon(IDR_ICON_STOP_ACTIVE));  
    m_stop_inactive_icon = m_imagelist.Add(pApp->LoadIcon(IDR_ICON_STOP_INACTIVE)); 
	m_dot_cur_active_icon = m_imagelist.Add(pApp->LoadIcon(IDR_ICON_DOT_CUR_ACTIVE));
	
	int cols[] = {2};
	m_quicklist.Set_Edit_Columns(cols,1);
}

CStepsView::~CStepsView()
{
}

void CStepsView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STEPS_VIEW, m_quicklist);
}

BEGIN_MESSAGE_MAP(CStepsView, CFormView)
//	ON_WM_SIZING()
ON_WM_SIZE()
ON_MESSAGE(WM_QUICKLIST_GETLISTITEMDATA, OnGetListItem)
ON_MESSAGE(WM_QUICKLIST_CLICK, OnListClick)
ON_NOTIFY(LVN_ENDLABELEDIT, IDC_STEPS_VIEW, OnLvnEndLabelEdit)
ON_COMMAND(ID_EDIT_INSERT, OnEditInsert)
ON_COMMAND(ID_EDIT_DELETE, OnEditDelete)
ON_UPDATE_COMMAND_UI(ID_EDIT_INSERT, OnUpdateEditInsert)
ON_UPDATE_COMMAND_UI(ID_EDIT_DELETE, OnUpdateEditDelete)
//ON_WM_TIMER()
ON_WM_TIMER()
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)

END_MESSAGE_MAP()


// CStepsView diagnostics

#ifdef _DEBUG
void CStepsView::AssertValid() const
{
	CFormView::AssertValid();
}

void CStepsView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

// CStepsView message handlers

BOOL CStepsView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Add your specialized code here and/or call the base class
	cs.style |= LVS_OWNERDATA | LVS_REPORT | LVS_EDITLABELS;
	return CFormView::PreCreateWindow(cs);
}

void CStepsView::OnInitialUpdate()
{
	CCALCViewWinDoc *pDoc = dynamic_cast<CCALCViewWinDoc*>(GetDocument());
	ASSERT(pDoc);

	CFormView::OnInitialUpdate();
	
	// Delete all columns
	int nColumnCount = m_quicklist.GetHeaderCtrl()->GetItemCount();
	for (int i=0;i < nColumnCount;i++)
		m_quicklist.DeleteColumn(0);
	
	// Set up the style of the list
	ListView_SetExtendedListViewStyle(m_quicklist, LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	
	// Add the headers of the columns
	m_quicklist.InsertColumn(0, "BP", LVCFMT_LEFT, 32);
	m_quicklist.InsertColumn(1, "Step", LVCFMT_LEFT, 64);
	m_quicklist.InsertColumn(2, "Statement", LVCFMT_LEFT, 256);

	m_quicklist.SetItemCountEx(pDoc->StepsGetRowCount(), LVSICF_NOSCROLL);
	m_quicklist.Invalidate();
	
	SetTimer(	0,						// timer identifier 
				1000,					// 10-second interval 
				(TIMERPROC) NULL);		// no timer callback 

}

void CStepsView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	
	CRect rectView;
	GetWindowRect(&rectView);

	CWnd *wnd = GetWindow(GW_CHILD);
	if (wnd != NULL)
		wnd->MoveWindow(-2, -2, rectView.right - rectView.left + 4, rectView.bottom - rectView.top + 4); 
}

LRESULT CStepsView::OnGetListItem(WPARAM wParam, LPARAM lParam)
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
    int item = data->GetItem() + 1;		// Note: Steps start at index 1
    int subItem = data->GetSubItem();
    
    

    //...insert information that is needed in "data"...


	if (subItem == 0) {
		data->m_button.m_style = DFCS_BUTTONCHECK;	
		data->m_allowEdit = false;
		data->m_image.m_imageList = &m_imagelist;
		data->m_image.m_imageID = pDoc->StepsGetBreak(item) ? m_stop_active_icon : m_stop_inactive_icon;
		data->m_image.m_center = true;
		data->m_image.m_noSelection = false;
	} else if (subItem == 1) {
		data->m_allowEdit = false;

		std::stringstream ss;
		ss << item;
   		data->m_text = _T(ss.str().c_str());
   		
		data->m_image.m_imageList = &m_imagelist;
		
		if (pDoc->StepsGetError(item))			data->m_image.m_imageID = m_dot_error_icon;
		else if (pDoc->StepsGetCurrent(item))	data->m_image.m_imageID = m_dot_cur_active_icon;
		else if (pDoc->StepsGetVisited(item))	data->m_image.m_imageID = m_dot_active_icon;
		else									data->m_image.m_imageID = m_dot_inactive_icon;
    
		data->m_image.m_noSelection = false;
	} else if (subItem == 2) {
		data->m_allowEdit = true;
		data->m_text = _T(pDoc->StepsGetValueAt(item).c_str());
	}

    return 0;
}

void CStepsView::OnLvnEndLabelEdit(NMHDR *pNMHDR, LRESULT *pResult)
{
	CCALCViewWinDoc *pDoc = dynamic_cast<CCALCViewWinDoc*>(GetDocument());
	ASSERT(pDoc);

	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

    // If pszText is NULL, editing was canceled
    if(pDispInfo->item.pszText != NULL)
    {
		int item = pDispInfo->item.iItem;		// Note: Steps start at index 1
		int step = pDispInfo->item.iItem + 1;		// Note: Steps start at index 1
		int subItem = pDispInfo->item.iSubItem;

		if (subItem == 1)
			pDoc->StepsToggleBreak(step);
		else if (subItem == 2)
			pDoc->StepsSetValueAt(step, LPCTSTR(pDispInfo->item.pszText) );
			
		m_quicklist.RedrawItems(item,item);
    }

    *pResult = 0;
}

LRESULT  CStepsView::OnListClick(WPARAM wParam, LPARAM lParam)
{
	CCALCViewWinDoc *pDoc = dynamic_cast<CCALCViewWinDoc*>(GetDocument());
	ASSERT(pDoc);

	//Make sure message comes from list box
    ASSERT( (HWND)wParam == m_quicklist.GetSafeHwnd() );

    CQuickList::CListHitInfo *hit= 
        (CQuickList::CListHitInfo*) lParam;

    //Get which item and subitem that is asked for.
    int step = hit->m_item + 1;		// Note: Steps start at index 1
    int subItem = hit->m_subitem;

    //Hit button?
    /*if(hit->m_onButton) {
        //...toggle check box in the database...

        //Redraw check box
        m_quicklist.RedrawCheckBoxs( hit->m_item, 
                    hit->m_item, 
                    hit->m_subitem);
                    
    } else*/ 
    
    if(hit->m_onImage) {
		if (subItem == 0) {
			pDoc->StepsToggleBreak(step);

			//Redraw image
			m_quicklist.RedrawImages(	hit->m_item, 
										hit->m_item, 
										hit->m_subitem);
		}
    }

    return 0;
}

void CStepsView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	CFormView::OnUpdate(pSender, lHint, pHint);

	CCALCViewWinDoc *pDoc = dynamic_cast<CCALCViewWinDoc*>(GetDocument());
	ASSERT(pDoc);

	if (lHint == UPDATE_STEPS_TABLE_CONTENTS || lHint == UPDATE_ALL_TABLE_CONTENTS) {
		m_quicklist.Invalidate();
	} else if (lHint == UPDATE_STEPS_TABLE_SIZE) {
		m_quicklist.SetItemCountEx(pDoc->StepsGetRowCount(), LVSICF_NOSCROLL);
		m_quicklist.Invalidate();
	}
}

void CStepsView::OnEditInsert()
{
	std::vector<int> items;

	CCALCViewWinDoc *pDoc = dynamic_cast<CCALCViewWinDoc*>(GetDocument());
	ASSERT(pDoc);

	// Assumes m_quicklist is in focus.
	// Add all the items
	POSITION pos = m_quicklist.GetFirstSelectedItemPosition();

	while (pos) {
		int nItem = m_quicklist.GetNextSelectedItem(pos);
		items.push_back(nItem);
	}
	
	// Sort the list of items
	std::sort(items.begin(), items.end(), std::less<int>() );
	
	// Process the sorted items
	for (unsigned int i = 0; i < items.size(); ++i)
		pDoc->StepsInsertLine(items[i]);
}

void CStepsView::OnEditDelete()
{
	std::vector<int> items;

	CCALCViewWinDoc *pDoc = dynamic_cast<CCALCViewWinDoc*>(GetDocument());
	ASSERT(pDoc);

	// Assumes m_quicklist is in focus.
	// Add all the items
	POSITION pos = m_quicklist.GetFirstSelectedItemPosition();

	while (pos) {
		int nItem = m_quicklist.GetNextSelectedItem(pos);
		items.push_back(nItem);
	}
	
	// Sort the list of items
	std::sort(items.begin(), items.end(), std::greater<int>() );
	
	// Process the sorted items
	for (unsigned int i = 0; i < items.size(); ++i)
		pDoc->StepsDeleteLine(items[i]);
}

void CStepsView::OnUpdateEditInsert(CCmdUI *pCmdUI)
{
	if (GetFocus() == &m_quicklist) {
		pCmdUI->Enable(true);
	} else {
		pCmdUI->Enable(false);
	}
	// TODO: Add your command update UI handler code here
}

void CStepsView::OnUpdateEditDelete(CCmdUI *pCmdUI)
{
	if (GetFocus() == &m_quicklist) {
		pCmdUI->Enable(true);
	} else {
		pCmdUI->Enable(false);
	}
	// TODO: Add your command update UI handler code here
}

void CStepsView::OnTimer(UINT nIDEvent)
{
	CCALCViewWinDoc *pDoc = dynamic_cast<CCALCViewWinDoc*>(GetDocument());
	ASSERT(pDoc);
	
	pDoc->Timer_Fired();

	CView::OnTimer(nIDEvent);
	
	//SetTimer(	0,						// timer identifier 
	//			1000,					// 10-second interval 
	//			(TIMERPROC) NULL);		// no timer callback 

	CFormView::OnTimer(nIDEvent);
}
void CStepsView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: Add your specialized code here and/or call the base class
	
	CCALCViewWinDoc *pDoc = dynamic_cast<CCALCViewWinDoc*>(GetDocument());
	ASSERT(pDoc);
	
	pDoc->OnPrint(pDC, pInfo);
}

BOOL CStepsView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CStepsView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CStepsView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

