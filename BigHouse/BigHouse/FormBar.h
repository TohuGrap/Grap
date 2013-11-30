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
  enum ObjectShape {
    OBJ_TABLE = 0,
    OBJ_DESK = 1,
  };

protected:
  virtual void OnPaint();
  virtual void OnInitialUpdate();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  afx_msg void OnBnNext();
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg void OnOption();
  afx_msg void OnSelectShelf();
  afx_msg void ClearShelf();
  afx_msg void OnShowCoordinate();
  afx_msg void OnSettingRoom();
	afx_msg void ShowSizeRoom();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint pt);
	DECLARE_MESSAGE_MAP()
private:
  int object_index_;
protected:
	int show_size_;
};


