#pragma once
#include "stdafx.h"

// DlgProduction dialog

class DlgProduction : public CDialogEx
{
	DECLARE_DYNAMIC(DlgProduction)

public:
	DlgProduction(CWnd* pParent = NULL);   // standard constructor
	virtual ~DlgProduction();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };
	void UpdateBitmap(int index_bitmap);
  int CheckBitmap(CString str);
	CString GetCurrentStrProduction() {return str_current_production_;}
protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnLBSelChange();
	DECLARE_MESSAGE_MAP()

protected:
	CListBox list_box_ctrl_;
	CString str_current_production_;
	CStatic bitmap_image_ctrl;
  CBitmap cbitmap_;
};
