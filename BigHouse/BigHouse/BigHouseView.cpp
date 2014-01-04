
// BigHouseView.cpp : implementation of the BigHouseView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "BigHouse.h"
#endif

#include "BigHouseDoc.h"
#include "BigHouseView.h"
#include "base.h"
#include "SettingRoomDlg.h"
#include "DlgSettingShelf.h"
#include "DlgProduction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// BigHouseView
#define LENGTH_AXIS 10000

IMPLEMENT_DYNCREATE(BigHouseView, CView)

BEGIN_MESSAGE_MAP(BigHouseView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &BigHouseView::OnFilePrintPreview)
	ON_COMMAND(ID_VIEW_FULLSCREEN, OnViewFullscreen)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONUP()
  ON_WM_SIZE()
  ON_WM_CREATE()
  ON_WM_KEYDOWN()
  ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
  ON_WM_MOUSEMOVE()
  ON_WM_MOUSEWHEEL()
  ON_WM_DESTROY()
END_MESSAGE_MAP()

// BigHouseView construction/destruction

BigHouseView::BigHouseView():
  eyeX_(0.0f),
  eyeY_(0.0f),
  eyeZ_(0.0f),
  centX_(0.0f),
  centY_(0.0f),
  centZ_(0.0f),
  x_position_(0.0f),
  y_position_(0.0f),
  z_zoom_(0.0f),
  angle_x_ea_(0.0f),
  angle_y_ea_(0.0f),
  angle_z_ea_(0.0f)
{
  m_PosIncr = 0.25f;
	m_AngIncr = 5.0f;
	m_AngleX = 90.0f;
  object_index_ = -1;
  pos[0] = 0.0f;
  pos[1] = 0.0f;
  pos[2] = 0.0f;
	move_shelf_.Set(0, 0, 0);
  m_OrthoRangeLeft = -1.5f;
	m_OrthoRangeRight = 1.5f;
	m_OrthoRangeBottom = -1.5f;
	m_OrthoRangeTop = 1.5f;
	m_OrthoRangeNear = -20.0f;
	m_OrthoRangeFar = 100.0f;
  rendering_rate_ = 0.5f;

  lef_pos_ = 0.0f;
  right_pos_ = 0.0f;
  bottom_pos = 0.0f;
  top_pos_ = 0.0f;

	red_color_ = 0.0f;
	green_color_ = 0.0f;
	blue_color_ = 0.0f;


  m_scaling = 1.0f;
	right_button_down_ = false;
	left_button_down_ = false;
	phi_ = 60;
	theta_ = 60;

  shelf_long_ = 0.0;
  shelf_width_ = 0.0;
  shelf_height_ = 0.0;
  number_shelf_ = 0;
  number_floor_ = 0;
	count_selected_ = -1;
	count_selected_ = -1;

  show_coordinate_ = false;

	room_size_.longs = 1500.0f;
	room_size_.width = 1500.0f;
	room_size_.height = 150.0f;
	room_size_.depth = 10.0f;
	left_point_down_2d_.SetPoint(- 1, -1);
	is_show_size_ = false;

	// Passion88 add
	bbmin_first.v[0]= 0.0;
	bbmin_first.v[1]= 0.0;
	bbmin_first.v[2]= 0.0;
	can_add_shelf = true;
	number_of_shelf_ = 0;
	number_of_product_ = 0;
}

BigHouseView::~BigHouseView()
{
}

BOOL BigHouseView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

  // Styles for Opengl requires
  cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	return CView::PreCreateWindow(cs);
}

// BigHouseView drawing

void BigHouseView::OnDraw(CDC* /*pDC*/)
{
	BigHouseDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

// Empty color buffer and depth buffer
  ::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(red_color_, green_color_, blue_color_, 1.0f);
  // Call render function 
 ::glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

  //glPushMatrix();
 	CClientDC clienDC(this);
	wglMakeCurrent(clienDC.m_hDC, m_hRC);
  SetViewFrustum();
  RenderScene();
	wglMakeCurrent(NULL, m_hRC);
  //glPopMatrix();
	glFlush();
  ::glFinish();
  ::SwapBuffers(m_pDC->GetSafeHdc());
}


// BigHouseView printing


void BigHouseView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL BigHouseView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void BigHouseView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void BigHouseView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


void BigHouseView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{ 
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// BigHouseView diagnostics

#ifdef _DEBUG
void BigHouseView::AssertValid() const
{
	CView::AssertValid();
}

void BigHouseView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

BigHouseDoc* BigHouseView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(BigHouseDoc)));
	return (BigHouseDoc*)m_pDocument;
}
#endif //_DEBUG


// BigHouseView message handlers
void BigHouseView::OnSize(UINT nType, int cx, int cy) {
  CView::OnSize(nType, cx, cy);
  if (cx <= 0 || cy <= 0)
    return;
  cx_ = cx;
  cy_ = cy;

	CClientDC clienDC(this);
	wglMakeCurrent(clienDC.m_hDC, m_hRC);

  // Define viewport = size window
  glViewport(0, 0, cx, cy);

  GLfloat aspect_ratio = (GLdouble)cx/(GLdouble)cy;
  ::glMatrixMode(GL_PROJECTION);

  //::gluPerspective(45.0f, aspect_ratio, 0.01f,-200.0f);
	// gluPerspective(0,aspect_ratio,0.01,200);

  ::glMatrixMode(GL_MODELVIEW);

  // Select MatrixModelView
	 wglMakeCurrent(NULL, NULL);
}

void BigHouseView::SetViewFrustum() {
  double left_ = -(double)cx_ *0.5/ rendering_rate_;
  double right_ = (double)cx_ *0.5/ rendering_rate_;
  double top_ = (double)cy_ *0.5/ rendering_rate_;
  double bottom_ = -(double)cy_ *0.5/ rendering_rate_;

  double zfar = 2000/rendering_rate_;
  zfar = max(2000, rendering_rate_);
  glOrtho(left_, right_, bottom_, top_, -zfar, zfar);
}



int BigHouseView::OnCreate(LPCREATESTRUCT lpCreateStruct) {
  if (CView::OnCreate(lpCreateStruct) == -1)
    return -1;

  InitializeOpenGL();
  return 0;
}

void BigHouseView::OnDestroy() {
  theApp.FreePoint();
  CView::OnDestroy();
  ::wglDeleteContext(m_hRC);
  delete m_pDC;
}

BOOL BigHouseView::InitializeOpenGL() {
  m_pDC = new CClientDC(this);
  if (m_pDC == NULL)
    return FALSE;
  if (!SetupPixelFormat())
    return FALSE;

  m_hRC = ::wglCreateContext(m_pDC->GetSafeHdc());
  if ( 0 == m_hRC)
    return FALSE;
  if (FALSE == ::wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC))
    return FALSE;

  // Clear background by black color
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  // Clear depth
  glClearDepth(1.0f);
  // Enable depth test
	::glEnable(GL_DEPTH_TEST);

  // Enable color tracking
  ::glEnable(GL_COLOR_MATERIAL);
  ::glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

  ::glShadeModel(GL_SMOOTH);
  // Setup lighting and material

  //SetupLight();
  OnLoadTexture();
	CreateOpenGLFont();
  return TRUE;
}


void BigHouseView::SetColorForBackGround(float red_value, float green_value, float blue_value) {
	red_color_ = red_value;
	green_color_ = green_value;
	blue_color_ = blue_value;
	InvalidateRect(NULL, FALSE);
}


BOOL BigHouseView::SetupPixelFormat() {
  static PIXELFORMATDESCRIPTOR pfd = 
	{
    sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd
    1,                              // version number
    PFD_DRAW_TO_WINDOW |            // support window
      PFD_SUPPORT_OPENGL |          // support OpenGL
      PFD_DOUBLEBUFFER,             // double buffered
    PFD_TYPE_RGBA,                  // RGBA type
    24,                             // 24-bit color depth
    0, 0, 0, 0, 0, 0,               // color bits ignored
    0,                              // no alpha buffer
    0,                              // shift bit ignored
    0,                              // no accumulation buffer
    0, 0, 0, 0,                     // accum bits ignored
    16,                             // 16-bit z-buffer
    0,                              // no stencil buffer
    0,                              // no auxiliary buffer
    PFD_MAIN_PLANE,                 // main layer
    0,                              // reserved
    0, 0, 0                         // layer masks ignored
  };

  if ( 0 == (pixel_format_ = ::ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd)) ) {
    return FALSE;
	}

  if ( FALSE == ::SetPixelFormat(m_pDC->GetSafeHdc(), pixel_format_, &pfd) ) {
    return FALSE;
  }

  return TRUE;
}

void BigHouseView::SetupLight() {
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_NORMALIZE); 
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
	glEnable(GL_DIFFUSE);

  glEnable(GL_POINT_SMOOTH);
	//glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	//glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  //glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);
 // glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);

  GLfloat m_SceneAmbient1[]  = {0.2f,0.2f,0.2f,1.0f};
	GLfloat m_SceneDiffuse1[]  = {0.7f,0.7f,0.7f,1.0f};
	GLfloat m_SceneSpecular1[] = {0.2f,0.2f,0.2f,1.0f};
	//GLfloat m_ScenePosition1[] = {gradient_.v[0], gradient_.v[1], gradient_.v[2],0.0f};
	GLfloat m_ScenePosition1[] = {0.0f, 0.5f, -1.0f,0.0f};
	GLfloat m_SceneDirection1[]= {0.0f,0.0f,1.0f,0.0f};
	GLfloat whiteSpecularLight[] = {1.0, 1.0, 1.0}; 
	GLfloat blackAmbientLight[] = {0.0, 0.0, 0.0};
	GLfloat whiteDiffuseLight[] = {1.0, 1.0, 1.0};
	GLfloat mShininess[] = {60}; //set the shininess of the 

	glLightfv(GL_LIGHT0, GL_AMBIENT, m_SceneAmbient1); 
	glLightfv(GL_LIGHT0, GL_DIFFUSE, m_SceneDiffuse1); 
	glLightfv(GL_LIGHT0, GL_SPECULAR, m_SceneSpecular1); 
//	glLightfv(GL_LIGHT0, GL_POSITION, m_ScenePosition1);
 	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF,20.0f);

	GLfloat material_ambient[] = {1.0f ,1.0f , 1.0f ,1.0f};
	GLfloat material_specular[] = {1.0f ,1.0f ,1.0f ,1.0f};
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, material_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);

	glMaterialfv(GL_FRONT, GL_SHININESS, mShininess);
}

void BigHouseView::DisableLight() {
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
  glDisable(GL_POINT_SMOOTH);
  glDisable(GL_LINE_SMOOTH);
	glDisable(GL_BLEND);
  glDisable(GL_CULL_FACE);
  glDisable(GL_LIGHTING);
}

void BigHouseView::RenderScene() {
	glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
	SetupLight();
	SetViewFrustum();

	glTranslatef(x_position_, y_position_, 0.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	ViewDirection();
  
  if (show_coordinate_ == true) {
    glPushMatrix();
		glTranslatef(0.0f, 0.0f, room_size_.depth/2 + 2);
    DrawCoordinate();
    glPopMatrix();
  }
	if (is_show_size_) {
		glPushMatrix();
		DrawSizeLine();
		glPopMatrix();
	}

#ifdef ENABLE_DRAW_POINT
	glColor3f(1,0,0);
	glPointSize(5.0f);
	glBegin(GL_POINTS);
  glVertex3f(point_m_in_opengl_.v[0], point_m_in_opengl_.v[1], point_m_in_opengl_.v[2]);
  glEnd();
#endif

	glPushMatrix();
  CreateShelf();
	glPopMatrix();
	DisableLight();
	DrawRoom();
}

void BigHouseView::CreateShelf() {
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	for(int i = 0; i < shelf_.size(); i ++) {
		Vector3D origin;
		// get position of shelf
		shelf_.at(i)->GetShelfPosition(origin);
		glPushMatrix();
		glTranslatef(origin.v[0], origin.v[1], origin.v[2]);

		// if shelf is moved by left mouse move
		if(count_selected_ == i && cad_info_.second.empty()) {
			glPushMatrix();
			glTranslatef(move_shelf_.v[0], move_shelf_.v[1], move_shelf_.v[2]);
			shelf_.at(i)->DrawShelf();
			glPopMatrix();
		} else {
	  	shelf_.at(i)->DrawShelf();
		}
	  glPopMatrix();
	}
}


void BigHouseView::DrawCoordinate() {
  glLineWidth(2.0f);
  glBegin(GL_LINES);
  glColor3f(1.0f, 0.0, 0.0);
  glVertex3f(0, 0.0, 0.0f);
  glVertex3f(LENGTH_AXIS, 0.0, 0.0f);

  glColor3f(0.0f, 1.0, 0.0);
  glVertex3f(0.0f, 0, 0.0f);
  glVertex3f(0.0f, LENGTH_AXIS, 0.0f);

  glColor3f(0.0f, 0.0, 1.0);
  glVertex3f(0.0f, 0.0, 0);
  glVertex3f(0.0f, 0.0f, LENGTH_AXIS);
  glEnd();
}

void BigHouseView::DrawSizeLine() {

	DisableLight();
	// width Size Line
	glPushMatrix();
	glTranslatef(0.0, room_size_.longs/2 + 100, 0.0);
  glLineWidth(3.0f);
  glBegin(GL_LINES);
  glColor3f(1.0f, 1.0, 0.0);
	glVertex3f(-room_size_.width/2, 0.0, 0.0f);
	glVertex3f(room_size_.width/2, 0.0, 0.0f);
	glEnd();
//	DrawTextList(room_size_.width);
	glPopMatrix();

	// long Size Line
	glPushMatrix();
	glTranslatef(room_size_.width/2 + 100, 0.0 , 0.0);
  glLineWidth(3.0f);
  glBegin(GL_LINES);
  glColor3f(1.0f, 1.0, 0.0);
	glVertex3f(0.0, -room_size_.width/2, 0.0f);
	glVertex3f(0.0, room_size_.width/2, 0.0f);
	glEnd();
	glPopMatrix();

	// make width left Arrow 
	glPushMatrix();
	glColor3ub(255, 0, 0);
	glTranslatef(room_size_.width/2 - 20, room_size_.longs/2 + 100, 0.0);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glutSolidCone(10, 40, 16, 100);
	glPopMatrix();

	// make width right Arrow 
	glPushMatrix();
	glColor3ub(255, 0, 0);
	glTranslatef(-room_size_.width/2 + 20, room_size_.longs/2 + 100, 0.0);
	glRotatef(-90.0, 0.0, 1.0, 0.0);
	glutSolidCone(10, 40, 16, 100);
	glPopMatrix();


	// make longs left Arrow 
	glPushMatrix();
	glColor3ub(255, 0, 0);
	glTranslatef(room_size_.width/2 + 100, -room_size_.longs/2 + 20, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glutSolidCone(10, 40, 16, 100);
	glPopMatrix();

	// make longs right Arrow 
	glPushMatrix();
	glColor3ub(255, 0, 0);
	glTranslatef(room_size_.width/2 + 100, room_size_.longs/2 - 20, 0.0);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	glutSolidCone(10, 40, 16, 100);
	glPopMatrix();

	// height Size Line
	glPushMatrix();
	glTranslatef(room_size_.width/2 + 100, room_size_.longs/2 + 100 , 0.0);
  glLineWidth(3.0f);
  glBegin(GL_LINES);
  glColor3f(1.0f, 1.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0f);
	glVertex3f(0.0, 0.0, room_size_.height);
	glEnd();
	glPopMatrix();

	// make height top Arrow 
	glPushMatrix();
	glColor3ub(255, 0, 0);
	glTranslatef(room_size_.width/2 + 100, room_size_.longs/2 + 100, room_size_.height-30);
	glRotatef(0.0, 1.0, 0.0, 0.0);
	glutSolidCone(10, 40, 16, 100);
	glPopMatrix();

	// make height bottom Arrow 
	glPushMatrix();
	glColor3ub(255, 0, 0);
	glTranslatef(room_size_.width/2 + 100, room_size_.longs/2 + 100, 30.0);
	glRotatef(180.0, 1.0, 0.0, 0.0);
	glutSolidCone(10, 40, 16, 100);
	glPopMatrix();


		//Draw LineSize
		CString str_longs;
		CString str_width;
		CString str_height;

		str_longs.Format(L"%.2f", room_size_.longs);
		str_width.Format(L"%.2f", room_size_.width);
		str_height.Format(L"%.2f", room_size_.height);

		char* ch_longs = Base::CStringToChar(str_longs);
		char* ch_width = Base::CStringToChar(str_width);
		char* ch_height = Base::CStringToChar(str_height);

		if (rendering_rate_ > 0.05) {
			glColor3ub(255, 255, 255);
			DrawStringAt(0.0, room_size_.longs/2 + 150, -100.0, ch_longs);
			DrawStringAt(room_size_.width/2 + 150, 0.0, -100.0, ch_width);
			DrawStringAt(room_size_.width/2 + 150, room_size_.longs/2 + 150, -50, ch_height);
		}
	SetupLight();
}
void BigHouseView::OnRButtonUp(UINT nFlags, CPoint point)
{
	//ClientToScreen(&point);
	//OnContextMenu(this, point);  // Create menu cut copy paste
  mouse_down_point_ = CPoint(0, 0);
  ReleaseCapture();
	right_button_down_ = false;
  //CView::OnRButtonUp(nFlags, point);
}

void BigHouseView::OnLButtonUp(UINT nFlags, CPoint point) {
	left_button_down_ = false;
	if(count_selected_ != -1) {
		if(!cad_info_.second.empty()) {
			shelf_.at(count_selected_)->SetCadToShelf(cad_info_);
		} else {
			shelf_.at(count_selected_)->SetShelfPosition(move_shelf_);
		}
	}
	count_selected_ = - 1;
}

void BigHouseView::OnRButtonDown(UINT nFlags, CPoint point) {
  mouse_down_point_ = point;
  SetCapture();
	right_button_down_ = true;
	//if(shelf_.size() > old_count_selecte_ && old_count_selecte_ >= 0) {
	//	shelf_.at(old_count_selecte_)->ReSetSelectFloor();
	//	old_count_selecte_ = -1;
	//}
	// Clear creating production when right mouse is down
	cad_info_.second.clear();

  //CView::OnLButtonDown(nFlags, point);
}

void BigHouseView::OnLButtonDown(UINT nFlags, CPoint point) { 
	// get coordinate windows left mouse
	left_point_down_2d_ = point;

	if(cad_info_.second.empty()) { // shelf only move when cad is not selected
		Vector3D opengl_point;
		Vector3D perpendicular_vector;
		Vector3D oz(0, 0, 1);
		Vector3D O(0, 0, 0);
		ConvertWindowToOpenGL(point, opengl_point);
		GetPerpendicularVectorWithScreen(perpendicular_vector);
		GetMousePointOnAnyPlane(perpendicular_vector,
														 opengl_point, oz, O,
														 point_left_button_down_);// point click on plane
		count_selected_ = SelecteShelf(perpendicular_vector, opengl_point);
		if(GetKeyState(VK_SHIFT) & 0x8000 && count_selected_ != -1) {
		  if(CalPointOnOZ(perpendicular_vector, opengl_point, point_oz_)) {
				shelf_.at(count_selected_)->GetHeightFloor(perpendicular_vector,
				                                          opengl_point,
																									selected_count_floor_,
																									height_floor_first_,
																									height_floor_second_);
			} else {
				count_selected_ = -1;
			}
		} 
	  move_shelf_.Set(0, 0, 0);
	}
	left_button_down_ = true;
	CView::OnLButtonDown(nFlags, point);
}

void BigHouseView::ConvertWindowToOpenGL(const CPoint &point2D, Vector3D &point_3D) {
  GLint viewport[4];
  GLdouble modelview[16];
  GLdouble projection[16];
  GLdouble winX, winY, winZ;

  glGetDoublev(GL_PROJECTION_MATRIX, projection);
  glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
  glGetIntegerv(GL_VIEWPORT, viewport );

  winX = (GLdouble)point2D.x;
  winY = (GLdouble)viewport[3] - (GLdouble)point2D.y;
  winZ = 0.1;//(GLdouble)screen_point.v[2];
  GLdouble v[3];
  gluUnProject(winX, winY, winZ,
               modelview, projection, viewport, v, v + 1, v + 2);
	point_3D.Set(v[0], v[1], v[2]);
}


void BigHouseView::OnMouseMove(UINT nFlags, CPoint point) {
	if(right_button_down_) {
		int deltax = point.x - mouse_down_point_.x;
		// convert deltax into o
		float anglex = deltax*180.0/cx_;
		// phi is angle of camera, it is not angle of model
		// when mouse move from left to right so camera move from right to left
  	phi_ -= anglex;
		if(phi_ > 360) {
			phi_ = phi_ - 360;
		} else if( phi_ < - 360) {
			phi_ = phi_ + 360;
		}

		int deltay = (double)(point.y - mouse_down_point_.y);
		// convert delta into o
		float angley = deltay*180.0/cy_;
		theta_ -= angley;
		if(theta_ > 360) {
			theta_ = theta_ - 360;
		} else if( theta_ < - 360) {
			theta_ = theta_ + 360;
		}
	} else {
		Vector3D opengl_point;
		Vector3D perpendicular_screen_vector;
		ConvertWindowToOpenGL(point, opengl_point);
		GetPerpendicularVectorWithScreen(perpendicular_screen_vector);
		Vector3D point_mouse;
		if(left_button_down_) {
			if(count_selected_ != -1 && cad_info_.second.empty()) { // if shelf is selected
				                                                  // and cad_production is empty
				if(count_selected_ != -1 && cad_info_.second.empty() && !(GetKeyState(VK_SHIFT) & 0x8000)) {
					Vector3D oz_unit(0, 0, 1);   // oz unit vector
					Vector3D O(0, 0, 0);   // origin coordinate (0 point)
					GetMousePointOnAnyPlane(perpendicular_screen_vector, opengl_point, oz_unit, O, point_mouse);
					move_shelf_ = point_mouse - point_left_button_down_;
				} else if (count_selected_ != -1 && cad_info_.second.empty()) {
				  Vector3D point_oz;
					if(count_selected_ != -1 && CalPointOnOZ(perpendicular_screen_vector, opengl_point, point_oz)) {
						//Vector3D temp = point_oz - point_oz_;
						int d = (int)(point_oz.v[2] - point_oz_.v[2]);
						d = d/3;
						d = 3*d;
						double d_f = d + height_floor_first_;
						double d_s = - d + height_floor_second_;
						shelf_.at(count_selected_)->SetHeightFloor(selected_count_floor_, d_f, d_s);
					}
				}
			} else if (count_selected_ != -1 && cad_info_.second.empty()) {
				Vector3D point_oz;
				if(count_selected_ != -1 && CalPointOnOZ(perpendicular_screen_vector, opengl_point, point_oz)) {
					//Vector3D temp = point_oz - point_oz_;
					double d = point_oz.v[2] - point_oz_.v[2];
					double d_f = d + height_floor_first_;
					double d_s = - d + height_floor_second_;
					shelf_.at(count_selected_)->SetHeightFloor(selected_count_floor_, d_f, d_s);
				}
			}
		} else if (!cad_info_.second.empty()) {
		  count_selected_ = SelecteShelf(perpendicular_screen_vector, opengl_point);
			if(count_selected_ != -1) {
				shelf_.at(count_selected_)->PointMouseOnFloor(perpendicular_screen_vector, opengl_point);
			}
		} 

		if(old_count_selecte_ != - 1 && old_count_selecte_ != count_selected_) {
			if(shelf_.size() > old_count_selecte_ && old_count_selecte_ >= 0) {
				shelf_.at(old_count_selecte_)->ReSetSelectFloor();
			}
		}
		old_count_selecte_ = count_selected_;
	}
  InvalidateRect(NULL,FALSE);
  CView::OnMouseMove(nFlags, point);
  if (GetCapture() == this) {
    mouse_down_point_ = point;
  }
}

void BigHouseView::ViewDirection() {
	Vector3D gradien;
	Vector3D cam_up;
	Vector3D temp;
	// unit vector oz
	Vector3D oz(0, 0, 1);

	// convert phi and theta to radian to speed is not fast
	double phi = phi_*M_PI/180.0;     // phi_ is x rotation
	double theta = theta_*M_PI/180.0; // theta_ is y rotation

	// At first time phi = 60 and theta = 60
	// oz replace position oy
	gradien.v[0] = cos(phi)*sin(theta);  // get eye_x when cam rotate
	gradien.v[1] = sin(phi)*sin(theta);  // get eye_y when cam rotate
	gradien.v[2] = cos(theta);           // get eye_z when cam rotate


	if((theta_ < 180 && theta_ > 0) || theta_ < -180) {
		temp = oz*gradien; // if  (oz->gradien) is same clock direction
		                   // direction of tem is top->bottom (- direction)
	} else {
		temp = gradien*oz; // else : direction of tem is bottom ->top (+ direction)
	}

	if(temp.abs() < 0.01) {
	  cam_up.v[0] = - cos(phi);
		cam_up.v[1] =  - sin(phi);
		cam_up.v[2] = 0.0;
 	} else {
	  cam_up = gradien*temp;
	}
	cam_up = cam_up.Unit();
  gluLookAt(gradien.v[0], gradien.v[1], gradien.v[2],
		         0, 0, 0,
						 cam_up.v[0], cam_up.v[1], cam_up.v[2]);
}


BOOL BigHouseView::OnMouseWheel(UINT nFlags, short zDetal, CPoint point) {
  BOOL ret = FALSE ;
  
  if (zDetal >= 0) {
		rendering_rate_ *= 1.50f;
    ret = TRUE ;
  } else {
		rendering_rate_ /= 1.5f;
    ret = TRUE ;
  }

  InvalidateRect(NULL,FALSE);
  //Set the mouse point
  mouse_down_point_ = point;
  CWnd::OnMouseWheel(nFlags, zDetal, point);
  return  ret;
}



void BigHouseView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	switch (nChar) {
	case VK_UP:
		y_position_ += 15.0f;
		InvalidateRect(NULL,FALSE);
		break;
	case VK_DOWN:
		y_position_ -= 15.0f;
		InvalidateRect(NULL,FALSE);
		break;
	case VK_LEFT:
		x_position_ -= 15.0f;
		InvalidateRect(NULL,FALSE);
		break;
	case VK_RIGHT:
		x_position_ += 15.0f;
		InvalidateRect(NULL,FALSE);
		break;
	case VK_DELETE: {
		DeleteShelf();
		InvalidateRect(NULL,FALSE);
		break;
		}
	case VK_ESCAPE : {
		MainFrame * main_frame = static_cast<MainFrame*>(AfxGetMainWnd());
		main_frame->HandleEscape();
		}
	case 82: { // R key button
		RotateShelf();
		InvalidateRect(NULL,FALSE);
		break;
					 }
	case VK_SHIFT:{
				InvalidateRect(NULL,FALSE);
				}
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void BigHouseView::OnLoadTexture() {
  //Create Texture Names
  glGenTextures(2, m_texture);
  CString str_path = Base::GetPathModule();
  
  LoadTexture(str_path + L"\\bitmap\\TAB.bmp",  0);
  //LoadTexture(str_path + L"\\bitmap\\TAB.bmp",  1);
}


void BigHouseView::LoadTexture(CString file_name, int text_name )
{
  // load texture 
  AUX_RGBImageRec *texture;
  int length = file_name.GetLength();
  LPWSTR lpstr = LPWSTR( file_name.GetBuffer(length));
  file_name.ReleaseBuffer();
  texture = auxDIBImageLoad(lpstr);
  if (!texture) {
    AfxMessageBox(L"Picture cout not be loaded");
    exit(1);
  }

  glBindTexture(GL_TEXTURE_2D, m_texture[text_name]);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

  gluBuild2DMipmaps(GL_TEXTURE_2D, 3, texture->sizeX,
                    texture->sizeY, GL_RGB, GL_UNSIGNED_BYTE,
                    texture->data);
}


void BigHouseView::DrawRectangleBox(float width, float height, float depth, char solid) {
  char i, j = 0;
  float x = width / 2.0, y = height / 2.0, z = depth / 2.0;
  for (i = 0; i < 4; i++) {
    glRotatef(90.0, 0.0, 0.0, 1.0);
    if (j) {
      if (!solid)
        glBegin(GL_LINE_LOOP);
      else
        glBegin(GL_QUADS);
      glNormal3f(-1.0, 0.0, 0.0);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(-x, y, z);
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(-x, -y, z);
      glTexCoord2f(1.0f, 1.0f);
      glVertex3f(-x, -y, -z);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(-x, y, -z);
      glEnd();
      if (solid) {
        glBegin(GL_TRIANGLES);
        glNormal3f(0.0, 0.0, 1.0);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(0.0, 0.0, z);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-x, y, z);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(-x, -y, z);
        glNormal3f(0.0, 0.0, -1.0);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(0.0, 0.0, -z);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-x, -y, -z);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(-x, y, -z);
        glEnd();
      }
      j = 0;
    } else {
      if (!solid)
        glBegin(GL_LINE_LOOP);
      else
        glBegin(GL_QUADS);
      glNormal3f(-1.0, 0.0, 0.0);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(-y, x, z);
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(-y, -x, z);
      glTexCoord2f(1.0f, 1.0f);
      glVertex3f(-y, -x, -z);
      glTexCoord2f(01.0f, 0.0f);
      glVertex3f(-y, x, -z);
      glEnd();
      if (solid) {
        glBegin(GL_TRIANGLES);
        glNormal3f(0.0, 0.0, 1.0);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(0.0, 0.0, z);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-y, x, z);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(-y, -x, z);
        glNormal3f(0.0, 0.0, -1.0);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(0.0, 0.0, -z);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-y, -x, -z);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(-y, x, -z);
        glEnd();
      }
      j = 1;
    }
  }
}

void BigHouseView::DrawRoom() { 
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_texture[0]);

	float longs = room_size_.longs;    // y
	float width = room_size_.width;    // x
	float height = room_size_.height;  // z
	float depth = room_size_.depth;

  glColor3f(0.0f, 1.0f, 1.0f);
  DrawRectangleBox(longs, width, depth, 1);  // Bottom face

  glPushMatrix();  // left wall
  glTranslatef(0.0f, -1*(width/2), (height/2) - depth/2.0f);
  glRotatef(90.0, 1.0, 0.0, 0.0);
  DrawRectangleBox(longs, height, depth, 1);
  glPopMatrix();

  glPushMatrix();    // right wall
  glTranslatef(0.0f, width/2, (height/2) - depth/2.0f);
  glRotatef(-90.0, 1.0, 0.0, 0.0);
  DrawRectangleBox(longs, height, depth, 1);
  glPopMatrix();

  glPushMatrix();    // Back wall
  glTranslatef(-1*(longs/2), 0.0, (height/2) - depth/2.0f);
  glRotatef(-90.0, 0.0, 1.0, 0.0);
  DrawRectangleBox(height, width + depth, depth, 1);
  glPopMatrix();


  glPushMatrix();    // Front wall
  glTranslatef((longs/2), 0.0, (height/2) - depth/2.0f);
  glRotatef(-90.0, 0.0, 1.0, 0.0);
  DrawRectangleBox(height, width + depth, depth, 1);
  glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

}

void BigHouseView::GetPerpendicularVectorWithScreen(Vector3D& perpendicular_screen_vector ){
	double mdview[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, mdview);
	Vector3D temp(mdview[2], mdview[6], mdview[10]);
	perpendicular_screen_vector = temp.Unit();
}

// Compare point mouse with points of shelf
int BigHouseView::SelecteShelf(Vector3D &perpendicular_vector,
															 Vector3D &opengl_point) {
	double d;
	int shelf_index = -1; // shelf selected element = -1 at first time
	bool has_point = false;
	Vector3D point_on_bb;
	for(int i = 0; i < shelf_.size(); ++i) {
		Vector3D temp;
		if(shelf_.at(i)->ObjectIsSelectedByLeftMouse(perpendicular_vector, opengl_point, temp)) {
			if(!has_point) {
				point_on_bb = temp;
				shelf_index = i;
				has_point = true;
			} else {
				Vector3D u = point_on_bb - temp;
				if(u.scalar(perpendicular_vector) < 0) {
					point_on_bb = temp;
					shelf_index = i;
				} 
			}
		}
	}
	return shelf_index;
}

void BigHouseView::GetMousePointOnAnyPlane( Vector3D &perpendicular_screen_vector, Vector3D &opengl_point, Vector3D &oz_unit, Vector3D &origin_coordinate_point, Vector3D &room_mouse_point )
{
	// if oz_vector and oz_unit is not Perpendicular 
  if (perpendicular_screen_vector.scalar(oz_unit) != 0) {
		double t = (origin_coordinate_point.scalar(oz_unit) - oz_unit.scalar(opengl_point))/(oz_unit.scalar(perpendicular_screen_vector));
		Vector3D u = perpendicular_screen_vector*t;
		room_mouse_point = opengl_point + u;
	} else {
		// when perpendicular line // room bottom face
		// shelf is moved to O point
		room_mouse_point.Set(0, 0, 0);
	}
}


void BigHouseView::ResetColor() {
}

void BigHouseView::SetCadInfo(std::pair<Floor, std::vector<Triangle3D*>> &cad_info) {
	// body include information about production 
	cad_info_ = cad_info;
}

void BigHouseView::MakeSimpleShelf( float width, float length, float height, int count_floor )
{
	RectShelf *rec_shelf = new RectShelf(width, length, height, count_floor);
	UINT space_distance_length = length;
	UINT space_distance_width = width;
	RenderShelf(rec_shelf, space_distance_length, space_distance_width);
}
	

void BigHouseView::MakeDoubleShelf( float width, float length, float height, int count_floor )
{
  
  RectShelfFront_Back *double_shelf = new RectShelfFront_Back(width, length, height,
																													count_floor);
	UINT space_distance_length = length;
	UINT space_distance_width = 2*width;
	RenderShelf(double_shelf, space_distance_length, space_distance_width);
}

void BigHouseView::MakeCircleShelf(float radius, float height,
	                                 float start_angle, float end_angle,
																	 float flat_angle, int floor )
{
	CircleShelf *circle_shelf = new CircleShelf (radius, height, start_angle, end_angle, flat_angle, floor);
	UINT space_distance_length = 2*radius;
	UINT space_distance_width = 2*radius;
	RenderShelf(circle_shelf, space_distance_length, space_distance_width);
}

void BigHouseView::RenderShelf( Shelf* sh, UINT space_distance_length, UINT space_distance_width )
{
	if (can_add_shelf == false) {
		AfxMessageBox(_T("Kh?ng th? th?m du?c n?a"));
		return;
	}

	shelf_.push_back(sh);
	UINT size = shelf_.size();
	if (size > 1) {
		Vector3D bbmin;
		shelf_.at(size - 2)->GetShelfPosition(bbmin);
		if (bbmin.v[1] + space_distance_length + 50 < room_size_.width/2 - space_distance_length - room_size_.depth) {
			 bbmin.v[1] = bbmin.v[1] + space_distance_length + 50;
			} else {
			if (bbmin.v[0] + 2*space_distance_width + 50 > room_size_.longs/2 - room_size_.depth) {
				AfxMessageBox(_T("Kh?ng th? th?m du?c n?a"));
				shelf_.pop_back();
				can_add_shelf = false;
				return;
			} else {
				bbmin = bbmin_first;
				bbmin.v[0] = bbmin.v[0] + space_distance_width + 50;
				bbmin_first = bbmin;
			}
		}
		shelf_.at(size - 1)->SetShelfPosition(bbmin);
	} else {
		if (1/*sh->IsCircleShelf() == true*/) {
			Vector3D bbmin(-(room_size_.longs/2 - space_distance_length), -(room_size_.width/2 - space_distance_width), 0);
			bbmin_first = bbmin;
			shelf_.at(size - 1)->SetShelfPosition(bbmin);
		} else {
			Vector3D bbmin(-(room_size_.longs/2 - 25), -(room_size_.width/2 - 25), 0);
			bbmin_first = bbmin;
			shelf_.at(size - 1)->SetShelfPosition(bbmin);
		}
	}
	InvalidateRect(NULL, FALSE);
}


void BigHouseView::ClearShelf() {
  if (!shelf_.empty())
    shelf_.clear();

	// Passion88 add
	bbmin_first.v[0]= 0.0;
	bbmin_first.v[1]= 0.0;
	bbmin_first.v[2]= 0.0;
	can_add_shelf = true;
	InvalidateRect(NULL, FALSE);
}

void BigHouseView::OnViewTop() { 
	phi_ = 0.0;
  theta_ = 0.0;
  ::glMatrixMode( GL_MODELVIEW );
  ::glLoadIdentity();
	InvalidateRect(NULL, FALSE);
}

void BigHouseView::OnViewBottom() {
	phi_ = 0.0;
  theta_ = 180.0;
  ::glMatrixMode( GL_MODELVIEW );
  ::glLoadIdentity();
	InvalidateRect(NULL, FALSE);
}

void BigHouseView::OnViewLeft() {
  phi_ = -90.0;
  theta_ = 90.0;
  ::glMatrixMode( GL_MODELVIEW );
  ::glLoadIdentity();
	InvalidateRect(NULL, FALSE);
}

void BigHouseView::OnViewRight() {
	phi_ = 90.0;
  theta_ = 90.0;
  ::glMatrixMode( GL_MODELVIEW );
  ::glLoadIdentity();
	InvalidateRect(NULL, FALSE);
}

void BigHouseView::OnViewFront() {
  phi_ = 0.0;
  theta_ = 90.0;
  ::glMatrixMode( GL_MODELVIEW );
  ::glLoadIdentity();
	InvalidateRect(NULL, FALSE);
}

void BigHouseView::OnViewBack() {
	phi_ = -180.0;
  theta_ = 90.0;
  ::glMatrixMode( GL_MODELVIEW );
  ::glLoadIdentity();
	InvalidateRect(NULL, FALSE);
}

void BigHouseView::OnViewIso() {
  phi_ = 60.0;
  theta_ = 60.0;
  ::glMatrixMode( GL_MODELVIEW );
  ::glLoadIdentity();
	InvalidateRect(NULL, FALSE);
}

void BigHouseView::OnShowCoordinate() {
  if (show_coordinate_ == true)
    show_coordinate_ = false;
  else
    show_coordinate_ = true;
	InvalidateRect(NULL, FALSE);
}

void BigHouseView::OnUpdateShowCoordinate(CCmdUI* cmd) {
  cmd->SetCheck(show_coordinate_ ? 1 : 0);
}

// draw label "X" "Y" "Z"
void BigHouseView::CreateOpenGLFont() {
  CFont m_font;
	GLYPHMETRICSFLOAT agmf[256]; 

  m_font.CreateFont( -160,               // Height Of Font 
                       0,               // Width Of Font 
                       0,               // Angle Of Escapement 
                       0,               // Orientation Angle 
                       FW_BOLD,       // Font Weight 
                       FALSE,           // Italic 
                       FALSE,           // Underline 
                       FALSE,           // Strikeout 
                       ANSI_CHARSET,              // Character Set Identifier 
                       OUT_TT_PRECIS,             // Output Precision 
                       CLIP_DEFAULT_PRECIS,       // Clipping Precision 
                       ANTIALIASED_QUALITY,           // Output Quality 
                       FF_DONTCARE|DEFAULT_PITCH, // Family And Pitch 
                       _T("Arial"));
  CFont* m_pOldFont = m_pDC->SelectObject(&m_font); 
  m_editCLTip = glGenLists(256);
  m_textTip = glGenLists(256);
	m_3DTextList = glGenLists(256);


  wglUseFontBitmaps(m_pDC->m_hDC, 0, 255, m_editCLTip);
  wglUseFontBitmaps(m_pDC->m_hDC, 0, 255, m_textTip);
	wglUseFontOutlines(m_pDC->GetSafeHdc(), 0, 255, m_3DTextList, 0.0f, 0.01f, WGL_FONT_POLYGONS, agmf); 



  GetDC()->SelectObject(m_pOldFont); 
} 

void BigHouseView::DrawStringAt(double x, double y, double z, char* s) {
  size_t length = strlen(s);
  if (length > 0 && s) {
    glRasterPos3d(x, y , z);
    glListBase(m_textTip);
    glCallLists(length, GL_UNSIGNED_BYTE, (const GLvoid*)s);
  }
}

void BigHouseView::DeleteShelf() {
	Vector3D pos;
	ConvertWindowToOpenGL(left_point_down_2d_, pos);
	Vector3D dir;
	GetPerpendicularVectorWithScreen(dir);
	int count = SelecteShelf(dir, pos);
	if(!shelf_.empty() && count != -1 && left_point_down_2d_.x != - 1) {
		shelf_.erase(shelf_.begin() + count);
	}
	left_point_down_2d_.SetPoint(- 1, -1);
}

void BigHouseView::RotateShelf() {
	Vector3D pos;
	ConvertWindowToOpenGL(left_point_down_2d_, pos);
	Vector3D dir;
	GetPerpendicularVectorWithScreen(dir);
	int count = SelecteShelf(dir, pos);
	if(!shelf_.empty() && count != -1 && left_point_down_2d_.x != - 1) {
		shelf_.at(count)->RotateShelf();
	}
	//left_point_down_2d_.SetPoint(- 1, -1);
}


void BigHouseView::SetupRoom() {
	SettingRoomDlg dlg(room_size_, is_show_size_);
	if (IDOK == dlg.DoModal()) {
		room_size_ = dlg.GetRoomSize();
		is_show_size_ = dlg.IsShowRoomSize();
	}
}

void BigHouseView::SetupShelf()
{
	DlgSettingShelf dlg;
	ShelfInfo shelf_info;
	bool is_exist = false;
	if (dlg.DoModal() == IDOK) {
		shelf_info = dlg.GetShelfInfo();
		form_bar_->SetShelfInfoList(shelf_info, is_exist);
		if (is_exist == false) {
		  number_of_shelf_++;
		  form_bar_->SetDataForListShelf(number_of_shelf_);
		} else {
			::MessageBox(NULL, _T("Lo?i K? N?y ?? T?n T?i Trong List R?i"), _T("Th?ng B?o"), MB_OK | MB_ICONINFORMATION);
			return;
		}
	}
}

void BigHouseView::ClearAllShelf() {
	ClearShelf();
	form_bar_->DisableLoadProduct();
}

void BigHouseView::SetupProduction()
{
	bool is_exist = false;
	DlgProduction dlg;
	CString str;
	if (dlg.DoModal() == IDOK) {
		str = dlg.GetProductName();
		form_bar_->SetProductionList(str, is_exist);
		if (is_exist == false) {
			number_of_product_++;
			form_bar_->SetDataForListProduct(number_of_product_);
		} else {
			::MessageBox(NULL, _T("S?n ph?m n?y d? t?n t?i"), _T("Th?ng b?o"), MB_OK | MB_ICONINFORMATION);
			return;
		}
	}
}

void BigHouseView::OnViewFullscreen()
{
	MainFrame* main_frame = static_cast<MainFrame*>(AfxGetMainWnd());
	main_frame->OnViewFullscreen();
}


void BigHouseView::DrawTextList(double size) {
	glPushMatrix();
	CString str = _T("");
	str.Format(_T("%.3f"), size);
	glColor3f(1, 1, 1);
	glListBase(m_3DTextList);
	int length = str.GetLength();
	glScalef(100,100,100);
	glCallLists(length, GL_SHORT , str);
	glPopMatrix();
}

bool BigHouseView::CalPointOnOZ(Vector3D &dir, Vector3D &pos, Vector3D &Point_oz) {
	Vector3D oz(0, 0, 1);
	Vector3D O(0, 0, 0);
	Vector3D n = oz*dir;
	if(n.abs() == 0) {
		return false;
	}
	Vector3D u = n*dir;
	double t = (u.scalar(pos) - u.scalar(O))/(u.scalar(oz));
	Point_oz = oz*t;
	Point_oz = Point_oz + O;
	return true;
}