#pragma once
#include "resource.h"
#include "afxwin.h"

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
  enum ObjectShape {
    OBJ_TABLE = 0,
    OBJ_DESK = 1,
  };
protected:
  CStatic bitmap_image_ctrl;
  CBitmap cbitmap_;

  CString str_shelf_long_;
  CString str_shelf_width_;
  CString str_shelf_height_;
  CString str_number_of_shelf_;
  CString str_number_of_floor_;

protected:
  virtual void OnPaint();
  virtual void OnInitialUpdate();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  afx_msg void OnLBSelChange();
  afx_msg void OnBnNext();
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg void OnOption();
  afx_msg void OnEditShelfLong();
  afx_msg void OnEditShelfWidth();
  afx_msg void OnEditShelfHeight();
  afx_msg void OnNumberOfShelf();
  afx_msg void OnNumberOfFloor();
	DECLARE_MESSAGE_MAP()
private:
  int object_index_;
protected:
  CString edit_number_of_shelf_;
  CString edit_shelf_long_;
  CString edit_shelf_width_;
  CString edit_shelf_height_;
  CString edit_number_of_floor_;
  int is_turning_back_;
  CListBox list_box_ctrl_;
};


