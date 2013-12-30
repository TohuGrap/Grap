
// MainFrm.h : interface of the MainFrame class
//

#pragma once
#include "FormBar.h"
#include "BigHouseView.h"
class MainFrame : public CFrameWndEx
{
	
protected: // create from serialization only
	MainFrame();
	DECLARE_DYNCREATE(MainFrame)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource,
												DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE,
												CWnd* pParentWnd = NULL,
												CCreateContext* pContext = NULL);
  BOOL OnCreateClient(LPCREATESTRUCT lpCreateStruct, CCreateContext *pContext);
	void OnViewFullscreen();
	void OnViewTop();
	void OnViewBottom();
	void OnViewLeft();
	void OnViewRight();
	void OnViewFront();
	void OnViewBack();
	void OnViewIso();
	void OnShowCoordinate();
	void OnUpdateShowCoordinate(CCmdUI * cmd);
	void ShowViewBar();
	void OnUpdateShowViewBar(CCmdUI* cmd);
	void HandleEscape();
	void ShowAndDockToolbar();
	void SetupRoom();
	void SetupShelf();
	void ClearAllShelf();
	void SetupProduction();

// Implementation
public:
	virtual ~MainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
  CMFCToolBar       m_wndViewBar;
	CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;
  BigHouseView *big_house_view_;
  FormBar *form_view_;
  bool is_full_screen_;
public:
  CSplitterWnd splitter_;

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()
};


