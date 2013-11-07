
// BigHouseView.h : interface of the BigHouseView class
//

#pragma once
#include "FormBar.h"
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
  void SetViewFrustum();
  BOOL InitializeOpenGL();
  BOOL SetupPixelFormat();
  void SetupLight();
  void DisableLight();
  void RenderScene();
  void DrawGround();
  void DrawCoordinate();
  void DrawRectangle(int length);
  void SetFormView(FormBar* form_bar) {form_bar_ = form_bar;}
  void DrawObject();
  void DrawSample();
  void SetIndexObject(int index) {object_index_ = index; InvalidateRect(NULL, FALSE);}
  void OnLoadTexture();
  void LoadTexture(CString file_name, int text_name);
	void DrawRoom();
  void SetPosValue(float p[3]) {
    pos[0] = p[0];
    pos[1] = p[1];
    pos[2] = p[2];
  }
  void DrawCad();
protected:
  float pos[3];

  int object_index_;
  int pixel_format_;
  FormBar* form_bar_;
  HGLRC m_hRC;    // Render context variable
  CDC* m_pDC;     // Device context variable

  GLfloat rendering_rate_;
  GLfloat eyeX_;
  GLfloat eyeY_;
  GLfloat eyeZ_;

  GLfloat centX_;
  GLfloat centY_;
  GLfloat centZ_;

  GLfloat m_PosIncr;
	GLfloat m_AngIncr;
	GLfloat m_AngleX;

  GLfloat x_position_;
  GLfloat y_position_;
  GLfloat z_zoom_;

  GLfloat angle_x_ea_;
  GLfloat angle_y_ea_;
  GLfloat angle_z_ea_;

  GLfloat m_OrthoRangeLeft;
	GLfloat m_OrthoRangeRight;
	GLfloat m_OrthoRangeTop;
	GLfloat m_OrthoRangeBottom;
	GLfloat m_OrthoRangeNear;
	GLfloat m_OrthoRangeFar;
  GLfloat m_scaling;

  GLuint m_texture[4];

  CPoint mouse_down_point_;

  int cx_; // size of window
  int cy_; 
  
// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
  afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
  afx_msg void CalculateRotatefAngle(CPoint point);
  afx_msg void OnDestroy();
  afx_msg void OnMouseMove(UINT nFlags, CPoint point);
  afx_msg BOOL OnMouseWheel(UINT nFlags, short zDetal, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in BigHouseView.cpp
inline BigHouseDoc* BigHouseView::GetDocument() const
   { return reinterpret_cast<BigHouseDoc*>(m_pDocument); }
#endif

