
// BigHouseView.h : interface of the BigHouseView class
//

#pragma once
#include "BigHouseDoc.h"

#define PI 3.1415926535
class BigHouseView : public CView
{
protected: // create from serialization only
	BigHouseView();
	DECLARE_DYNCREATE(BigHouseView)

// Attributes
public:
	BigHouseDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~BigHouseView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
  // Declare functions for opengl
public:
  BOOL InitializeOpenGL();
  BOOL SetupPixelFormat();
  void SetupLight();
  void DisableLight();
  void RenderScene();
  void DrawGround();
  void DrawCoordinate();
  void DrawRectangle(int length);
  // Variable for opengl
protected:
  int pixel_format_;
  HGLRC m_hRC;    // Render context variable
  CDC* m_pDC;     // Device context variable

  GLfloat eyeX_;
  GLfloat eyeY_;
  GLfloat eyeZ_;

  GLfloat centX_;
  GLfloat centY_;
  GLfloat centZ_;

  GLfloat m_PosIncr;
	GLfloat m_AngIncr;
	GLfloat m_AngleX;
// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
  afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in BigHouseView.cpp
inline BigHouseDoc* BigHouseView::GetDocument() const
   { return reinterpret_cast<BigHouseDoc*>(m_pDocument); }
#endif

