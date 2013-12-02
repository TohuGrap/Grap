
// BigHouseView.h : interface of the BigHouseView class
//

#pragma once
#include "FormBar.h"
#include "BigHouseDoc.h"
#include "Vector3D.h"
#include "Triangle3D.h"
#include "Shelf.h"
#include "RecShelf.h"
#include "RecShelfFont_Back.h"
#include "CircleShelf.h"
#include "Struct.h"

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
	void DrawSizeLine();
	void DrawStringAt(double x, double y, double z, char* s);
  void DrawRectangle(int length);
  void SetFormView(FormBar* form_bar) {form_bar_ = form_bar;}
  void SetIndexObject(int index) {object_index_ = index; InvalidateRect(NULL, FALSE);}
  void OnLoadTexture();
  void LoadTexture(CString file_name, int text_name);
  void DrawRectangleBox(float width, float height, float depth, char solid);
	void DrawRoom();
  void SetPosValue(float p[3]) {
    pos[0] = p[0];
    pos[1] = p[1];
    pos[2] = p[2];
  }
  void DrawCad();
	void ViewDirection();
	void GetVectorPerpendicularToThescreen(Vector3D &v_oz);
	int SelecteShelf(Vector3D &dir, Vector3D &pos);
	void ConvertScrenToOpengl(CPoint &point2D, Vector3D &point_3D);
	void GetPointMouseOnPlane(Vector3D &dir, Vector3D &pos, Vector3D &n, Vector3D &p, Vector3D &point);
	void ResetColor();
	void SetCadToView(std::pair<Floor, std::vector<Triangle3D*>> &body);

  void SetShefLong(float longs) {shelf_long_ = longs;};
  void SetShefWidth(float width) {shelf_width_ = width;};
  void SetShefHeight(float height) {shelf_height_ = height;};
  void SetNumberOfShelf(float nums) {number_shelf_ = nums;};
  void SetNumberOfFloor(float numf) {number_floor_ = numf;};
	void MakeShelf(int width, int length, int height, int count_floor, int count_shelf, float angle);
  void ClearShelf();

  void OnViewTop();
  void OnViewBottom();
  void OnViewLeft();
  void OnViewRight();
  void OnViewFront();
  void OnViewBack();
  void OnViewIso();
  void OnShowCoordinate();
  void OnUpdateShowCoordinate(CCmdUI* cmd);
	void SetRoomSize(RoomSize room_size) {room_size_ = room_size;}
	RoomSize GetRoomSize() {return room_size_;}
	void CreateOpenGLFont();
	void SetShowSize(bool is_show) {is_show_size_ = is_show; InvalidateRect(NULL, FALSE);} 
	void SetIsLbuttonDown(bool is_left_down) {left_button_down_ = is_left_down;}
	void SetColorForBackGround(float red_value, float green_value, float blue_value);
protected:
  float pos[3];

	float red_color_;
	float green_color_;
	float blue_color_;

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

  GLfloat lef_pos_;
  GLfloat right_pos_;
  GLfloat bottom_pos;
  GLfloat top_pos_;

  GLfloat m_scaling;

  GLuint m_texture[4];

  CPoint mouse_down_point_;

  int cx_; // size of window
  int cy_; 
  

  // variable about shelf
  float shelf_long_;
  float shelf_width_;
  float shelf_height_;
  int number_shelf_;
  int number_floor_;

  bool show_coordinate_;

	RoomSize room_size_;
	GLuint m_textTip;
	GLuint m_editCLTip;
	bool is_show_size_;
// Generated message map functions 
private:
	bool right_button_down_;
	bool left_button_down_;
	double phi_;
	double theta_;
	Vector3D point_m_in_opengl_;
	Vector3D dr_oz_;
	std::pair<Floor, std::vector<Triangle3D*>> body_;
	std::vector<Shelf*> shelf_;
	Vector3D point_left_button_down_;
	Vector3D move_shelf_;
	int old_count_selecte_;
	int count_selected_;

protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
  afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
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

