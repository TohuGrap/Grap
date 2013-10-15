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
protected:
  CStatic bitmap_image_ctrl;
  CBitmap cbitmap_;

  CString str_x_pos_;
  CString str_y_pos_;
  CString str_z_pos_;

protected:
  virtual void OnPaint();
  virtual void OnInitialUpdate();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  afx_msg void OnBnNext();
  afx_msg void OnOption();
  afx_msg void OnEditChangeXpos();
  afx_msg void OnEditChangeYpos();
  afx_msg void OnEditChangeZpos();

	DECLARE_MESSAGE_MAP()
private:
  int bitmap_index_;
  int object_index_;
protected:
  CString edit_x_pos_;
  CString edit_y_pos_;
  CString edit_z_pos_;
};


