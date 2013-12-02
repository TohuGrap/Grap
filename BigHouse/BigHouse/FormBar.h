#pragma once
#include "resource.h"
#include "afxwin.h"
#include "Struct.h"

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
  enum ObjectShape {
    OBJ_TABLE = 0,
    OBJ_DESK = 1,
  };

	enum ShelfType {
		SIMPLE_SHELF = 0,
		DOUBLE_SHELF = 1,
		CIRCLE_SHELF = 2
	};

protected:
  virtual void OnPaint();
  virtual void OnInitialUpdate();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  afx_msg void OnBnNext();
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg void OnGetProduction();
  afx_msg void OnSettingShelf();
  afx_msg void ClearShelf();
  afx_msg void OnShowCoordinate();
  afx_msg void OnSettingRoom();
	afx_msg void ShowSizeRoom();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint pt);
	afx_msg void OnBnShelfSelected();
	afx_msg void OnBnProductionSelected();
	DECLARE_MESSAGE_MAP()
private:
  int object_index_;
	int show_size_;
	CStatic background_color_;
	ShelfInfo shelf_info_;
	CString str_production_;
};


