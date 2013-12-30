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
	enum { IDD = IDD_DLG_PRODUCTION };
	CString GetCurrentStrProduction() {return str_current_production_;}
	void NewProduction();
	void NewImage();
	void ClearProduction();
	void UpdateProduct();
	void HandleBitmap(CString str_name);
	void LoadProductionBkgn();
	CString GetProductName() {return str_current_production_;}
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
	CString product_cad_path_;
	CString product_bitmap_path_;
	CString module_path_;
};
