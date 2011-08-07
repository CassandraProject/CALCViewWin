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
// RegistersView.cpp : implementation file
//

#include "stdafx.h"
#include "CALCViewWin.h"
#include "RegistersView.h"
#include ".\RegistersView.h"
#include "CALCViewWinDoc.h"

// CRegistersView

IMPLEMENT_DYNCREATE(CRegistersView, CFormView)

CRegistersView::CRegistersView()
	: CFormView(CRegistersView::IDD)
{
	// Load the image list
	CWinApp* pApp= AfxGetApp();

    // Create a masked image list large enough to hold the icons. 
    m_imagelist.Create(16, 16, ILC_COLOR8, 10, 0); 

    // Load the icon resources, and add the icons to the image list. 
    m_dot_active_icon = m_imagelist.Add(pApp->LoadIcon(IDR_ICON_DOT_ACTIVE)); 
    m_dot_inactive_icon = m_imagelist.Add(pApp->LoadIcon(IDR_ICON_DOT_INACTIVE));  
	
	m_check_active_red_icon = m_imagelist.Add(pApp->LoadIcon(IDR_CHECK_RED_ACTIVE)); 
    m_check_inactive_red_icon = m_imagelist.Add(pApp->LoadIcon(IDR_CHECK_RED_INACTIVE)); 
	m_check_active_yellow_icon = m_imagelist.Add(pApp->LoadIcon(IDR_CHECK_YELLOW_ACTIVE)); 
    m_check_inactive_yellow_icon = m_imagelist.Add(pApp->LoadIcon(IDR_CHECK_YELLOW_INACTIVE)); 
	m_check_active_cyan_icon = m_imagelist.Add(pApp->LoadIcon(IDR_CHECK_CYAN_ACTIVE)); 
    m_check_inactive_cyan_icon = m_imagelist.Add(pApp->LoadIcon(IDR_CHECK_CYAN_INACTIVE)); 
	m_check_active_grey_icon = m_imagelist.Add(pApp->LoadIcon(IDR_CHECK_GREY_ACTIVE)); 
    m_check_inactive_grey_icon = m_imagelist.Add(pApp->LoadIcon(IDR_CHECK_GREY_INACTIVE)); 
    
   	int cols[] = {5,6};
	m_quicklist.Set_Edit_Columns(cols,2);
}

CRegistersView::~CRegistersView()
{
}

void CRegistersView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STEPS_VIEW, m_quicklist);
}

BEGIN_MESSAGE_MAP(CRegistersView, CFormView)
//	ON_WM_SIZING()
ON_WM_SIZE()
ON_MESSAGE(WM_QUICKLIST_GETLISTITEMDATA, OnGetListItem)
ON_MESSAGE(WM_QUICKLIST_CLICK, OnListClick)
ON_NOTIFY(LVN_ENDLABELEDIT, IDC_STEPS_VIEW, OnLvnEndLabelEdit)
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)

END_MESSAGE_MAP()


// CRegistersView diagnostics

#ifdef _DEBUG
void CRegistersView::AssertValid() const
{
	CFormView::AssertValid();
}

void CRegistersView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

// CRegistersView message handlers

BOOL CRegistersView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Add your specialized code here and/or call the base class
	cs.style |= LVS_OWNERDATA | LVS_REPORT | LVS_EDITLABELS;
	return CFormView::PreCreateWindow(cs);
}

void CRegistersView::OnInitialUpdate()
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
	m_quicklist.InsertColumn(0, "OOS", LVCFMT_LEFT, 40);
	m_quicklist.InsertColumn(1, "BAD", LVCFMT_LEFT, 40);
	m_quicklist.InsertColumn(2, "ERR", LVCFMT_LEFT, 40);
	m_quicklist.InsertColumn(3, "SEC", LVCFMT_LEFT, 40);
	m_quicklist.InsertColumn(4, "Param", LVCFMT_LEFT, 96);
	m_quicklist.InsertColumn(5, "Value", LVCFMT_LEFT, 96);
	m_quicklist.InsertColumn(6, "Config", LVCFMT_LEFT, 96);

	m_quicklist.SetItemCountEx(pDoc->RegistersGetRowCount(), LVSICF_NOSCROLL);
	m_quicklist.Invalidate();
}

void CRegistersView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	
	CRect rectView;
	GetWindowRect(&rectView);

	CWnd *wnd = GetWindow(GW_CHILD);
	if (wnd != NULL)
		wnd->MoveWindow(-2, -2, rectView.right - rectView.left + 4, rectView.bottom - rectView.top + 4); 
}

LRESULT CRegistersView::OnGetListItem(WPARAM wParam, LPARAM lParam)
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
		case 0:		data->m_allowEdit = false;
					data->m_image.m_imageList = &m_imagelist;
					data->m_image.m_imageID = pDoc->RegistersGetOOS(item) ? m_check_active_cyan_icon : m_check_inactive_cyan_icon;
					data->m_colors.m_backColor = pDoc->RegistersGetOOS(item) ? RGB(0,255,255) : RGB(255,255,255);
					data->m_image.m_center = true;
					data->m_image.m_noSelection = false;
					data->m_noSelection = false;
					break;
		case 1:		data->m_allowEdit = false;
					data->m_image.m_imageList = &m_imagelist;
					data->m_image.m_imageID = pDoc->RegistersGetBAD(item) ? m_check_active_red_icon : m_check_inactive_red_icon;
					data->m_colors.m_backColor = pDoc->RegistersGetBAD(item) ? RGB(255,0,0) : RGB(255,255,255);
					data->m_image.m_center = true;
					data->m_image.m_noSelection = false;
					data->m_noSelection = false;
					break;
		case 2:		data->m_allowEdit = false;
					data->m_image.m_imageList = &m_imagelist;
					data->m_image.m_imageID = pDoc->RegistersGetERR(item) ? m_check_active_yellow_icon : m_check_inactive_yellow_icon;
					data->m_colors.m_backColor = pDoc->RegistersGetERR(item) ? RGB(255,255,0) : RGB(255,255,255);
					data->m_image.m_center = true;
					data->m_image.m_noSelection = false;
					data->m_noSelection = false;
					break;
		case 3:		data->m_allowEdit = false;
					data->m_image.m_imageList = &m_imagelist;
					data->m_image.m_imageID = pDoc->RegistersGetSEC(item) ? m_check_active_grey_icon : m_check_inactive_grey_icon;
					data->m_colors.m_backColor = pDoc->RegistersGetSEC(item) ? RGB(192,192,192) : RGB(255,255,255);
					data->m_image.m_center = true;
					data->m_image.m_noSelection = false;
					data->m_noSelection = false;
					break;
		case 4:		data->m_allowEdit = false;
					data->m_image.m_imageList = &m_imagelist;
					data->m_image.m_imageID = pDoc->RegistersGetModified(item) ? m_dot_active_icon : m_dot_inactive_icon;
					data->m_text = _T(pDoc->RegistersGetParam(item).c_str());
					break;
		case 5:		data->m_allowEdit = true;
					data->m_text = _T(pDoc->RegistersGetValue(item).c_str());
					break;
		case 6:		data->m_allowEdit = true;
					data->m_text = _T(pDoc->RegistersGetConfig(item).c_str());
					break;
	};

    return 0;
}

void CRegistersView::OnLvnEndLabelEdit(NMHDR *pNMHDR, LRESULT *pResult)
{
	CCALCViewWinDoc *pDoc = dynamic_cast<CCALCViewWinDoc*>(GetDocument());
	ASSERT(pDoc);

	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

    // If pszText is NULL, editing was canceled
    if(pDispInfo->item.pszText != NULL)
    {
		int item = pDispInfo->item.iItem;		// Note: Steps start at index 1
		int subItem = pDispInfo->item.iSubItem;
		
		if (subItem == 5)
			pDoc->RegistersSetValue(item, LPCTSTR(pDispInfo->item.pszText) );
		else if (subItem == 6)
			pDoc->RegistersSetConfig(item, LPCTSTR(pDispInfo->item.pszText) );
			
		m_quicklist.RedrawItems(item,item);
    }

    *pResult = 0;
}

LRESULT  CRegistersView::OnListClick(WPARAM wParam, LPARAM lParam)
{
	CCALCViewWinDoc *pDoc = dynamic_cast<CCALCViewWinDoc*>(GetDocument());
	ASSERT(pDoc);

	//Make sure message comes from list box
    ASSERT( (HWND)wParam == m_quicklist.GetSafeHwnd() );

    CQuickList::CListHitInfo *hit= 
        (CQuickList::CListHitInfo*) lParam;

    //Get which item and subitem that is asked for.
    int item = hit->m_item;		// Note: Steps start at index 1
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
    
		if (subItem == 0)
			pDoc->RegistersToggleOOS(item);
		else if (subItem == 1)
			pDoc->RegistersToggleBAD(item);
		else if (subItem == 2)
			pDoc->RegistersToggleERR(item);
		else if (subItem == 3)
			pDoc->RegistersToggleSEC(item);
    
		//Redraw image
		m_quicklist.RedrawImages(	hit->m_item, 
									hit->m_item, 
									hit->m_subitem);
		
    }

    return 0;
}

void CRegistersView::OnUpdate(CView* /*pSender*/, LPARAM lHint, CObject* /*pHint*/)
{
	CCALCViewWinDoc *pDoc = dynamic_cast<CCALCViewWinDoc*>(GetDocument());
	ASSERT(pDoc);

	if (lHint == UPDATE_REGISTER_TABLE_CONTENTS || lHint == UPDATE_ALL_TABLE_CONTENTS) {
		m_quicklist.Invalidate();
	} else if (lHint == UPDATE_REGISTER_TABLE_SIZE) {
		m_quicklist.SetItemCountEx(pDoc->RegistersGetRowCount(), LVSICF_NOSCROLL);
		m_quicklist.Invalidate();
	}

}

void CRegistersView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: Add your specialized code here and/or call the base class
	
	CCALCViewWinDoc *pDoc = dynamic_cast<CCALCViewWinDoc*>(GetDocument());
	ASSERT(pDoc);
	
	pDoc->OnPrint(pDC, pInfo);
}

BOOL CRegistersView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CRegistersView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CRegistersView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}
