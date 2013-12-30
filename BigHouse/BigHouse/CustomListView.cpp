// CustomListView.cpp : implementation file
//

#include "stdafx.h"
#include "BigHouse.h"
#include "CustomListView.h"
#include "FormBar.h"

// CustomListView

IMPLEMENT_DYNAMIC(CustomListView, CListCtrl)

CustomListView::CustomListView()
{
  form_bar_product = NULL;
	form_bar_shelf = NULL;
}

CustomListView::~CustomListView()
{
}


BEGIN_MESSAGE_MAP(CustomListView, CListCtrl)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_SIZE()
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, OnItemchanged)
END_MESSAGE_MAP()



// CustomListView message handlers


void CustomListView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
  CListCtrl::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CustomListView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
  CListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CustomListView::OnSize(UINT nType, int cx, int cy) {
  CListCtrl::OnSize(nType, cx, cy);
}

void CustomListView::OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult) {
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int item = pNMListView->iItem;
	if (form_bar_shelf != NULL) {
		form_bar_shelf->HandleListViewShelf(item);
	}

	if (form_bar_product != NULL) {
		form_bar_product->HandleListViewProduct(item);
	}
}