#pragma once
#include "resource.h"
#include "afxwin.h"
#include "struct.h"
class BigHouseView;
class BigHouseDoc;

// FormBar form view

class FormBar : public CFormView
{
	DECLARE_DYNCREATE(FormBar)

public:
	FormBar();           // protected constructor used by dynamic creation
	virtual ~FormBar();

public:
	enum { IDD = IDD_FORMBAR };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

public:
  int ObjectIndex() {return object_index_;}
  BigHouseView* GetBigHouseView();
  void UpdateBitmap(int index_bitmap);
  int CheckBitmap(CString str);
  SizeRoom GetSizeRoom() {return size_room_;}
  enum ObjectShape {
    OBJ_TABLE = 0,
    OBJ_DESK = 1,
  };
protected:
  CStatic bitmap_image_ctrl;
  CBitmap cbitmap_;
  SizeRoom size_room_;
protected:
  virtual void OnPaint();
  virtual void OnInitialUpdate();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  afx_msg void OnLBSelChange();
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg void OnOption();
  afx_msg void OnOpenSetupRoomDlg();
	DECLARE_MESSAGE_MAP()
private:
  int object_index_;
protected:
  CListBox list_box_ctrl_;
  CButton setup_room_btn_;
};


