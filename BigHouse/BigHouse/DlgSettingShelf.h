#pragma once
#include "resource.h"
#include "Struct.h"

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
protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnEditShelfLong();
  afx_msg void OnEditShelfWidth();
  afx_msg void OnEditShelfHeight();
  afx_msg void OnNumberOfShelf();
  afx_msg void OnNumberOfFloor();
  afx_msg void OnEditShelfAngle();
	DECLARE_MESSAGE_MAP()

protected:
	CString str_shelf_long_;
  CString str_shelf_width_;
  CString str_shelf_height_;
  CString str_number_of_shelf_;
  CString str_number_of_floor_;
  CString str_shelf_angle_;

	CString edit_number_of_shelf_;
  CString edit_shelf_long_;
  CString edit_shelf_width_;
  CString edit_shelf_height_;
  CString edit_number_of_floor_;
  CString edit_shelf_angle_;

	ShelfInfo shelf_info_;
};
