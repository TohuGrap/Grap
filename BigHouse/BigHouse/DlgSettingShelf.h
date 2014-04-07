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
		CIRCLE_SHELF = 2,
		CONTAINER = 3
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
	afx_msg void OnComboxShelfType();
	afx_msg void OnEditShelfDrag();
	afx_msg void OnEditShelfDisWall();
	afx_msg void OnEditNameProject();
	afx_msg void OnEditDragWall();
	afx_msg void OnRadioArrange0();
	afx_msg void OnRadioArrange1();
	afx_msg void OnOk();

	DECLARE_MESSAGE_MAP()
protected:
	CString str_shelf_long_;
  CString str_shelf_width_;
  CString str_shelf_height_;
	CString str_shelf_drag_;
	CString str_drag_wall_;
  CString str_number_of_floor_;

	CString str_shelf_radius_;

	CEdit edit_number_of_shelf_;
  CEdit edit_shelf_long_;
  CEdit edit_shelf_width_;
  CEdit edit_shelf_height_;
  CEdit edit_number_of_floor_;
	CEdit edit_shelf_drag_;
	CEdit edit_drag_wall_;

	CEdit edit_shelf_radius_;
	CEdit edit_name_shelf_project_;
	CButton radio_arrange_0_;
	CButton radio_arrange_1_;

	ShelfInfo shelf_info_;
	CComboBox combox_shelf_type_;

	CStatic bitmap_shelf_;
	CBitmap cbitmap_;

};
