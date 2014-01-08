#pragma once
#include "resource.h"
#include "Struct.h"
#include "afxwin.h"

// DlgSettingShelf dialog

class DlgSettingShelf : public CDialogEx
{
	DECLARE_DYNAMIC(DlgSettingShelf)

public:
	DlgSettingShelf(CWnd* pParent = NULL);   // standard constructor
	virtual ~DlgSettingShelf();

// Dialog Data
	enum { IDD = IDD_DLG_SETTING_SHELF };

	ShelfInfo GetShelfInfo() {return shelf_info_;}

public:
	void SetStatusInfoShelf(UINT shelf_type);
	void SetBitmapShelf(UINT index);

	enum ShelfType {
		SIMPLE_SHELF = 0,
		DOUBLE_SHELF = 1,
		CIRCLE_SHELF = 2
	};


protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnEditShelfLong();
  afx_msg void OnEditShelfWidth();
  afx_msg void OnEditShelfHeight();
  afx_msg void OnNumberOfShelf();
  afx_msg void OnNumberOfFloor();
	afx_msg void OnEditShelfRadius();
	afx_msg void OnEditShelfStartAngle();
	afx_msg void OnEditShelfEndAngle();
	afx_msg void OnEditShelfFlatAngle();
	afx_msg void OnComboxShelfType();
	afx_msg void OnEditShelfDrag();

	DECLARE_MESSAGE_MAP()
protected:
	CString str_shelf_long_;
  CString str_shelf_width_;
  CString str_shelf_height_;
	CString str_shelf_drag_;
  CString str_number_of_floor_;

	CString str_shelf_radius_;
	CString str_shelf_start_angle_;
	CString str_shelf_end_angle_;
	CString str_shelf_flat_angle_;


	CEdit edit_number_of_shelf_;
  CEdit edit_shelf_long_;
  CEdit edit_shelf_width_;
  CEdit edit_shelf_height_;
  CEdit edit_number_of_floor_;
  CEdit edit_shelf_angle_;
	CEdit edit_shelf_drag_;

	CEdit edit_shelf_radius_;
	CEdit edit_shelf_start_angle_;
	CEdit edit_shelf_end_angle_;
	CEdit edit_shelf_flat_angle_;

	ShelfInfo shelf_info_;
	CComboBox combox_shelf_type_;

	CStatic bitmap_shelf_;
	CBitmap cbitmap_;

};
