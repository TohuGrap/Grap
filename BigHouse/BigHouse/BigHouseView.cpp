
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
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// BigHouseView
#define LENGTH_AXIS 10000

#define M_PI 3.1415
IMPLEMENT_DYNCREATE(BigHouseView, CView)

BEGIN_MESSAGE_MAP(BigHouseView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &BigHouseView::OnFilePrintPreview)
  ON_COMMAND(ID_VIEW_TOP, &BigHouseView::OnViewTop)
  ON_COMMAND(ID_VIEW_BOTTOM, &BigHouseView::OnViewBottom)
  ON_COMMAND(ID_VIEW_LEFT, &BigHouseView::OnViewLeft)
  ON_COMMAND(ID_VIEW_RIGHT, &BigHouseView::OnViewRight)
  ON_COMMAND(ID_VIEW_FRONT, &BigHouseView::OnViewFront)
  ON_COMMAND(ID_VIEW_BACK, &BigHouseView::OnViewBack)
  ON_COMMAND(ID_VIEW_ISO, &BigHouseView::OnViewIso)
  ON_COMMAND(ID_VIEW_COORDINATE, &BigHouseView::OnShowCoordinate)
  ON_UPDATE_COMMAND_UI(ID_VIEW_COORDINATE, &BigHouseView::OnUpdateShowCoordinate)
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

	is_show_size_ = false;
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

  // Clear backgroun by black color
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  // Clear depth
  glClearDepth(1.0f);
  // Enalbe depth test
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
		glPushMatrix();
	}

#ifdef ENABLE_DRAW_POINT
	glColor3f(1,0,0);
	glPointSize(5.0f);
	glBegin(GL_POINTS);
  glVertex3f(point_m_in_opengl_.v[0], point_m_in_opengl_.v[1], point_m_in_opengl_.v[2]);
  glEnd();
#endif

	glPushMatrix();
  DrawCad();
  DisableLight();
	glPopMatrix();
	DrawRoom();
}

void BigHouseView::DrawCad() {
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	for(int i = 0; i < shelf_.size(); i ++) {
		Vector3D origin;
		shelf_.at(i)->GetOriginBody(origin);
		glPushMatrix();
		glTranslatef(origin.v[0], origin.v[1], origin.v[2]);
		if(count_selected_ == i && body_.second.empty()) {
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
		if(!body_.second.empty()) {
			shelf_.at(count_selected_)->SetCadToShelf(body_);
		} else {
			shelf_.at(count_selected_)->SetOriginBody(move_shelf_);
		}
	}
	count_selected_ = - 1;
}

void BigHouseView::OnRButtonDown(UINT nFlags, CPoint point) {
  mouse_down_point_ = point;
  SetCapture();
	right_button_down_ = true;

	// Clear creating production when right mouse is down
	body_.second.clear();

  //CView::OnLButtonDown(nFlags, point);
}

void BigHouseView::OnLButtonDown(UINT nFlags, CPoint point) { 
	if(body_.second.empty()) {
		Vector3D pos;
		ConvertScrenToOpengl(point, pos);
		Vector3D dir;
		GetVectorPerpendicularToThescreen(dir);
		Vector3D oz(0, 0, 1);
		Vector3D O(0, 0, 0);
		GetPointMouseOnPlane(dir, pos, oz, O, point_left_button_down_);// point click on plane
		count_selected_ = SelecteShelf(dir, pos);// 
		move_shelf_.Set(0, 0, 0);
	}
	left_button_down_ = true;
	CView::OnLButtonDown(nFlags, point);
}

void BigHouseView::ConvertScrenToOpengl(CPoint &point2D, Vector3D &point_3D) {
  GLint viewport[4];
  GLdouble modelview[16];
  GLdouble projection[16];
  GLdouble winX, winY, winZ;

  glGetDoublev( GL_PROJECTION_MATRIX, projection );
  glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
  glGetIntegerv( GL_VIEWPORT, viewport );

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
  	phi_ -= ((double)(point.x - mouse_down_point_.x))/3.60;
		if(phi_ > 360) {
			phi_ = phi_ - 360;
		} else if( phi_ < - 360) {
			phi_ = phi_ + 360;
		}

		theta_ -= ((double)(point.y - mouse_down_point_.y))/3.60;
		if(theta_ > 360) {
			theta_ = theta_ - 360;
		} else if( theta_ < - 360) {
			theta_ = theta_ + 360;
		}
	} else {
		Vector3D pos;
		ConvertScrenToOpengl(point, pos);
		Vector3D dir;
		GetVectorPerpendicularToThescreen(dir);
		Vector3D point_mouse;
		if(left_button_down_) {
			if(count_selected_ != -1 && body_.second.empty()) {
				Vector3D oz(0, 0, 1);
				Vector3D O(0, 0, 0);
			  GetPointMouseOnPlane(dir, pos, oz, O, point_mouse);
			  move_shelf_ = point_mouse - point_left_button_down_;
			}
		} else if (!body_.second.empty()) {
		  count_selected_ = SelecteShelf(dir, pos);
			if(count_selected_ != -1) {
				shelf_.at(count_selected_)->PointMouseOnFloor(dir, pos);
			}
		}
		if(old_count_selecte_ != - 1 && old_count_selecte_ != count_selected_) {
			if(shelf_.size() > old_count_selecte_) {
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

	double phi = phi_*M_PI/180.0;
	double theta = theta_*M_PI/180.0;
	Vector3D gradien;
	gradien.v[0] = cos(phi)*sin(theta);
	gradien.v[1] = sin(phi)*sin(theta);
	gradien.v[2] = cos(theta);

	Vector3D oz(0, 0, 1);	
	Vector3D cam_up;
	Vector3D temp;
	if((theta_ < 180 && theta_ > 0) || theta_ < -180){
		temp = oz*gradien;
	} else {
		temp = gradien*oz;
	}
	//gradient_ = gradient_ .Unit();
	if(temp.abs() < 0.01) {
	  cam_up.v[0] = - cos(phi);
		cam_up.v[1] =  - sin(phi);
		cam_up.v[2] = 0.0;
	
 	} else {
	  cam_up = gradien*temp;
	}
	cam_up = cam_up.Unit();
  gluLookAt(gradien.v[0], gradien.v[1], gradien.v[2], 0, 0, 0, cam_up.v[0], cam_up.v[1], cam_up.v[2]);
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
	//Processing Arrow Keys to Synchronise with Movement
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

	float longs = room_size_.longs;
	float width = room_size_.width;
	float height = room_size_.height;
	float depth = room_size_.depth;

  glColor3f(0.0f, 1.0f, 1.0f);
  DrawRectangleBox(longs, width, depth, 1);  // Bottom face

  glPushMatrix();  // left wall
  glTranslatef(0.0f, -1*(width/2), (height/2) - depth/2.0f);
  glRotatef(90.0, 1.0, 0.0, 0.0);
  DrawRectangleBox(longs, height, depth, 1);
  glPopMatrix();

  glPushMatrix();                    // right wall
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

void BigHouseView::GetVectorPerpendicularToThescreen(Vector3D &v_oz) {
	float m[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, m);
	Vector3D temp(m[2], m[6], m[10]);
	v_oz = temp.Unit();;
}

int BigHouseView::SelecteShelf(Vector3D &dir,Vector3D &pos) {
	double d;
	int count = -1;
	bool has_point = false;
	Vector3D point_on_bb;
	for(int i = 0; i < shelf_.size(); i ++) {
		Vector3D temp;
		if(shelf_.at(i)->IsLineCutBody(dir, pos, temp)) {
			if(!has_point) {
				point_on_bb = temp;
				count = i;
				has_point = true;
			} else {
				Vector3D u = point_on_bb - temp;
				if(u.scalar(dir) < 0) {
					point_on_bb = temp;
					count = i;
				} 
			}
		}
		
	}//
	return count;
}

void BigHouseView::GetPointMouseOnPlane(Vector3D &dir, Vector3D &pos, Vector3D &n, Vector3D &p, Vector3D &point) {
	if(dir.scalar(n) != 0) {
		double t = (p.scalar(n) - n.scalar(pos))/(n.scalar(dir));
		Vector3D u = dir*t;
		point = pos + u;
	} else {
		point.Set(0, 0, 0);
	}
}


void BigHouseView::ResetColor() {
	//for()
}

void BigHouseView::SetCadToView(std::pair<Floor, std::vector<Triangle3D*>> &body) {
	body_ = body;

}

void BigHouseView::MakeSimpleShelf(int width, int length, int height,
                             int count_floor, int count_shelf,
                             float angle) {

	RecShelf *rec_shelf = new RecShelf(width, length, height, count_floor , FONT);
	RenderShelf(rec_shelf);
}
	

void BigHouseView::MakeDoubleShelf(int width, int length, int height, int count_floor, int shelf_shape) {
  RecShelfFont_Back *double_shelf = new RecShelfFont_Back(width, length, height, count_floor ,LEFT_RIGHT);
	RenderShelf(double_shelf);
}

void BigHouseView::MakeCircleShelf(int radius, int height, int start_angle, int end_angle, int flat_angle, int floor) {
	CirclShelf *circle_shelf = new CirclShelf(radius, height, start_angle, end_angle, flat_angle, floor); //(100, 200,0, 360, 50, 4);
  RenderShelf(circle_shelf);
}

void BigHouseView::RenderShelf(Shelf* rec_shelf) {
	shelf_.push_back(rec_shelf);
	int size = shelf_.size();
	if (size > 1) {
		Vector3D bbmin;
		shelf_.at(size - 2)->GetOriginBody(bbmin);
		bbmin.v[1] = bbmin.v[1] + 100 + 50;
		shelf_.at(size - 1)->SetOriginBody(bbmin);

	} else {
	  Vector3D bbmin(-600, -600, 0);
		shelf_.at(size - 1)->SetOriginBody(bbmin);
	}
	InvalidateRect(NULL, true);
}

void BigHouseView::ClearShelf() {
  if (shelf_.empty() == false)
    shelf_.clear();
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
}

void BigHouseView::OnUpdateShowCoordinate(CCmdUI* cmd) {
  cmd->SetCheck(show_coordinate_ ? 1 : 0);
}

// draw label "X" "Y" "Z"
void BigHouseView::CreateOpenGLFont() {
  CFont m_font;
  m_font.CreateFont( -16,               // Height Of Font 
                       0,               // Width Of Font 
                       0,               // Angle Of Escapement 
                       0,               // Orientation Angle 
                       FW_NORMAL,       // Font Weight 
                       FALSE,           // Italic 
                       FALSE,           // Underline 
                       FALSE,           // Strikeout 
                       ANSI_CHARSET,              // Character Set Identifier 
                       OUT_TT_PRECIS,             // Output Precision 
                       CLIP_DEFAULT_PRECIS,       // Clipping Precision 
                       DEFAULT_QUALITY,           // Output Quality 
                       FF_DONTCARE|DEFAULT_PITCH, // Family And Pitch 
                       _T("Arial"));
  CFont* m_pOldFont = GetDC()->SelectObject(&m_font); 
  m_editCLTip = glGenLists(256);
  m_textTip = glGenLists(256);
  wglUseFontBitmaps(m_pDC->m_hDC, 0, 255, m_editCLTip);
  wglUseFontBitmaps(m_pDC->m_hDC, 0, 255, m_textTip);
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