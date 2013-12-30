#pragma once


// CustomListView
class FormBar;
class CustomListView : public CListCtrl
{
	DECLARE_DYNAMIC(CustomListView)

public:
	CustomListView();
	virtual ~CustomListView();
	void OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult);
	void SetFormBarShelf(FormBar* formbar) {form_bar_shelf = formbar;}
	void SetFormBarProduct(FormBar* formbar) {form_bar_product = formbar;}
protected:
		FormBar* form_bar_shelf;
		FormBar* form_bar_product;
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
