
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
#define SIZE_GROUND 50
const float ROOM_LENGTH =1000.0f;
const	float ROOM_WIDTH = 1000.0f;
const float ROOM_HEIGHT = 0.0f;

#define M_PI 3.14
IMPLEMENT_DYNCREATE(BigHouseView, CView)

BEGIN_MESSAGE_MAP(BigHouseView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &BigHouseView::OnFilePrintPreview)
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
  eyeY_(0.5f),
  eyeZ_(2.5f),
  centX_(0.0f),
  centY_(0.5f),
  centZ_(2.25f),
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
	move_body_.Set(0, 0, 0);
  m_OrthoRangeLeft = -1.5f;
	m_OrthoRangeRight = 1.5f;
	m_OrthoRangeBottom = -1.5f;
	m_OrthoRangeTop = 1.5f;
	m_OrthoRangeNear = -20.0f;
	m_OrthoRangeFar = 100.0f;
  rendering_rate_ = 2.0f;
  m_scaling = 1.0f;
	right_button_down_ = false;
	left_button_down_ = false;
	phi_ = 0;
	theta_ = 45;

		 

  shelf_long_ = 0.0;
  shelf_width_ = 0.0;
  shelf_height_ = 0.0;
  number_shelf_ = 0;
  number_floor_ = 0;
  is_turnning_back_ = false;
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
  // Call render function 
 ::glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

  //glPushMatrix();
  SetViewFrustum();
  RenderScene();
  //glPopMatrix();

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
  // Define viewport = size window
  glViewport(0, 0, cx, cy);
  GLfloat aspect_ratio = (GLdouble)cx/(GLdouble)cy;

  // Select the projection matrix
  // here, option aspection view
  ::glMatrixMode(GL_PROJECTION);
  ::glLoadIdentity();
  //::gluPerspective(45.0f, aspect_ratio, 0.01f,-200.0f);
	 gluPerspective(0,aspect_ratio,0.01,200);
  // SetViewFrustum();

  // Select MatrixModelView
  ::glMatrixMode(GL_MODELVIEW);
  ::glLoadIdentity();
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
  //::glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

  ::glShadeModel(GL_SMOOTH);
  // Setup lighting and material

  //glEnable(GL_CULL_FACE);
  SetupLight();
  OnLoadTexture();
	gluLookAt(0, 0, 1, 0, 0, 0,0, 1, 0);
  return TRUE;
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
  glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);

  GLfloat m_SceneAmbient1[]  = {0.3f,0.4f,0.3f,1.0f};
	GLfloat m_SceneDiffuse1[]  = {0.4f,1.0f,0.5f,1.0f};
	GLfloat m_SceneSpecular1[] = {1.0f,1.0f,1.0f,1.0f};
	GLfloat m_ScenePosition1[] = {0.0f,0.0f, 200.0f,0.0f};
	GLfloat m_SceneDirection1[]= {0.0f,0.0f,1.0f,0.0f};
	GLfloat whiteSpecularLight[] = {1.0, 1.0, 1.0}; 
	GLfloat blackAmbientLight[] = {0.0, 0.0, 0.0};
	GLfloat whiteDiffuseLight[] = {1.0, 1.0, 1.0};
	GLfloat mShininess[] = {128}; //set the shininess of the 

	glLightfv(GL_LIGHT0, GL_AMBIENT, m_SceneAmbient1); 
	glLightfv(GL_LIGHT0, GL_DIFFUSE, m_SceneDiffuse1); 
	glLightfv(GL_LIGHT0, GL_SPECULAR, m_SceneSpecular1); 
	glLightfv(GL_LIGHT0, GL_POSITION, m_ScenePosition1);
 	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF,100.0f);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mShininess);
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
  glLoadIdentity();
  glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	::glMatrixMode(GL_MODELVIEW);
	SetViewFrustum();
	ViewDirection();
 // glPushMatrix();
//	glTranslated(0,0,01)
 // //glRotatef(angle_x_ea_ - 45.0, 1.0f, 0.0f, 0.0f);
 //// glRotatef(angle_z_ea_ -135, 0.0f, 0.0f, 1.0f);
	glDisable(GL_LIGHTING);
	glColor3f(1,0,0);
	glPointSize(5.0f);
	glBegin(GL_POINTS);
  	glVertex3f(point_m_in_opengl_.v[0], point_m_in_opengl_.v[1], point_m_in_opengl_.v[2]);
  glEnd();


	glEnable(GL_LIGHTING);
  //glScalef(m_scaling, m_scaling, m_scaling);
  //SetupLight();
	glPushMatrix();
	 DisableLight();
	 //Shelf shelf;
	 //SetupLight();
	 //shelf.DrawShelf();
  DrawCad();
	glPopMatrix();
  DisableLight();
	DrawRoom();

  //glPopMatrix();
}

void BigHouseView::DrawCad() {
	SetupLight();
  //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glColor3f(1.0, 1.0, 1.0);
 // std::vector<std::pair<RectBody, std::vector<Triangle3D*>>> all_body;
 // all_body = theApp.GetCadBoy();
	//if(all_body.size() != all_body_.size()) {
	//	int size_bd = all_body_.size();
	//	for(int i = size_bd; i < all_body.size(); i ++) {
	//		all_body_.push_back(all_body.at(i));
	//	}
	//}

	for(int i = 0; i <shelf_.size() /*all_body_.size()*/; i ++) {
		//std::vector<Triangle3D*> body = all_body_.at(i).second;
		Vector3D cen ;
		shelf_.at(i)->GetBBmin(cen);///*all_body_.at(i).first.bbmax + */all_body_.at(i).first.bbmin;
  //  cen = cen*0.5;

		glPushMatrix();
		glTranslatef(cen.v[0], cen.v[1], cen.v[2]);
		if(left_button_down_ && move_count_ == i) {
			glTranslatef (move_body_.v[0], move_body_.v[1], move_body_.v[2]);
		}
		SetupLight();
		shelf_.at(i)->DrawShelf();
		//glBegin(GL_TRIANGLES); 
		//for(int j = 0; j < body.size(); j ++) {
		//	glNormal3fv(body.at(j)->normal.v);
		//	glVertex3fv(body.at(j)->m_v0.v);
		//	glVertex3fv(body.at(j)->m_v1.v);
		//	glVertex3fv(body.at(j)->m_v2.v);
		//}
		//  glPopMatrix();
		glEnd();
		glPopMatrix();
	}
}



void BigHouseView::DrawObject() {

  if (object_index_ == 0) {
    glColor3f(1.0, 0.0, 0.0);
    glPushMatrix();
    glTranslated(pos[0], pos[1] + 0.11, pos[2]);
    DrawRectangle(20.0);
    glPopMatrix();
  }
  if (object_index_ == 1) {
    glPushMatrix();
    glTranslated(pos[0], pos[1], pos[2]);
    glRotatef(-90, 1.0, 0.0, 0.0);
    glColor3f(1.0, 0.0, 0.0);
    glutSolidCone(1.0, 2.0, 16, 100);
    glPopMatrix();
  }
  if (object_index_ == 2) {
    glPushMatrix();
    glTranslated(pos[0], pos[1], pos[2]);
    glColor3f(1.0, 0.0, 0.0);
    glRotatef(-190, 1.0, 0.0, 0.0);
    glutSolidTetrahedron();
    glPopMatrix();
  }
  

  if (object_index_ == 3) {
    glPushMatrix();
    glTranslated(pos[0], pos[1], pos[2]);
    glColor3f(1.0, 1.0, 0.0);
    glRotatef(-90, 1.0, 0.0, 0.0);
    glutWireSphere(0.5, 16, 100);
    glPopMatrix();
  }

  if (object_index_ == 4) {
    glPushMatrix();
    glTranslated(pos[0], pos[1], pos[2]);
    glColor3f(1.0, 1.0, 0.0);
    glutWireTeapot(0.5);
    glPopMatrix();
  }

  if (object_index_ == 5) {
    glPushMatrix();
    glTranslated(pos[0], pos[1], pos[2]);
    glColor3f(1.0, 1.0, 0.0);
    glRotatef(-90, 1.0, 0.0, 0.0);
    glutWireTorus(0.5, 1.0, 16, 100);
    glPopMatrix();
  }
}

void BigHouseView::DrawSample() {
  // Draw Regtangle
  glPushMatrix();
  glTranslated(1.0, 0.0, 0.0);
  glColor3f(1.0, 0.0, 0.0);
  glScalef(1.0, 1.0, 1.0);
  DrawRectangle(1.5);
  glPopMatrix();

  glPushMatrix();
  glTranslated(-1.0, 0.0, 0.0);
  glColor3f(0.0, 1.0, 0.0);
  glScalef(1.0, 1.0, 1.0);
  DrawRectangle(1.0);
  glPopMatrix();

  glPushMatrix();
  glTranslated(0.0, 0.0, -5.0);
  glColor3f(0.0, 0.0, 1.0);
  glScalef(1.0, 1.0, 1.0);
  DrawRectangle(1.5);
  glPopMatrix();

  glPushMatrix();
  glTranslated(2.0, 0.0, -5.0);
  glColor3f(0.0, 1.0, 1.0);
  glScalef(1.0, 3.0, 1.0);
  DrawRectangle(1.0);
  glPopMatrix();

  glPushMatrix();
  glTranslated(-2.0, 0.0, -3.0);
  glColor3f(1.0, 0.0, 1.0);
  glScalef(1.0, 2.0, 1.0);
  DrawRectangle(1.0);
  glPopMatrix();
}


void BigHouseView::DrawCoordinate() {
  glLineWidth(4.0f);
  glBegin(GL_LINES);
  
  glColor3f(1.0f, 0.0, 0.0);
  glVertex3f(-100.0, 0.0, 0.0f);
  glVertex3f(100.0, 0.0, 0.0f);
  
  glColor3f(0.0f, 1.0, 0.0);
  glVertex3f(0.0f, -100.0f, 0.0f);
  glVertex3f(0.0f, 100.0f, 0.0f);
  
  glColor3f(0.0f, 0.0, 1.0);
  glVertex3f(0.0f, 0.0f, -100.0f);
  glVertex3f(0.0f, 0.0f, 100.0f);
  
  glEnd();
}

void BigHouseView::DrawGround() {
  glColor3f(1.0, 0.5f, 0.0f);
  glBegin(GL_POLYGON);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(SIZE_GROUND, 0.0f, 2*SIZE_GROUND);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(SIZE_GROUND, 0.0f, -1*SIZE_GROUND);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(-1*SIZE_GROUND, 0.0f, -1*SIZE_GROUND);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(-1*SIZE_GROUND, 0.0, 2*SIZE_GROUND);
  glEnd();
}


void BigHouseView::DrawRectangle(int length) {
  glutSolidCube(length);
}


void BigHouseView::OnRButtonUp(UINT nFlags, CPoint point)
{
	//ClientToScreen(&point);
	//OnContextMenu(this, point);  // Create menu cut copy paste
  mouse_down_point_ = CPoint(0, 0);
  ReleaseCapture();
	right_button_down_ = false;
  //CView::OnLButtonUp(nFlags, point);
}

void BigHouseView::OnLButtonUp(UINT nFlags, CPoint point) {
	left_button_down_ = false;
	if(!shelf_.empty() && move_count_ != - 1) {
		if(!body_.second.empty()) {
			shelf_.at(move_count_)->SetCadToShelf(body_);
			body_.second.clear();
		} else {
			Vector3D bbmin;
			shelf_.at(move_count_)->GetBBmin(bbmin);
			Vector3D temp1 = bbmin + move_body_;
			shelf_.at(move_count_)->SetBoundingBox(temp1);
		}
	}
	move_count_ = - 1;
}

void BigHouseView::OnRButtonDown(UINT nFlags, CPoint point) {
  mouse_down_point_ = point;
  SetCapture();
	right_button_down_ = true;
  //CView::OnLButtonDown(nFlags, point);
}

void BigHouseView::OnLButtonDown(UINT nFlags, CPoint point) { 
	Vector3D pos;
	ConvertScrenToOpengl(point, pos);
	Vector3D dir;
	GetVectorPerpendicularToThescreen(dir);
	move_body_.Set(0, 0 ,0);
	move_count_ = MoveBody( dir, pos, l_point_button_down_);
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
	//*************huu.nv***************
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
			Vector3D point_plane;
		if(left_button_down_) {
		 	MoveBody(dir, pos, point_plane);
			move_body_ = point_plane - l_point_button_down_;
			point_m_in_opengl_ = pos;
			//if(move_count_ != -1) {
			//	shelf_.at(move_count_)->PointMouseOnFloor(dir, pos);
			//}
		//} else {
		//
		} else if(!body_.second.empty()) {
		  move_count_ = MoveBody(dir, pos, point_plane);
			if(move_count_ != -1) {
				shelf_.at(move_count_)->PointMouseOnFloor(dir, pos);
			}
		}
	}

  InvalidateRect(NULL,FALSE);

  //**********************************
  CView::OnMouseMove(nFlags, point);
  if (GetCapture() == this) {
    ////Increment the object rotation angles
    //angle_x_ea_ += (point.y - mouse_down_point_.y)/3.6;
    //angle_z_ea_ += (point.x - mouse_down_point_.x)/3.6;
    //  //Redraw the view
    //InvalidateRect(NULL, FALSE);
    //  //Set the mouse point
    mouse_down_point_ = point;
  }
}

void BigHouseView::ViewDirection() {
	double phi = phi_*3.1415/180.0;
	double theta = theta_*3.1415/180.0;
	Vector3D gradien;
	gradien.v[0] = cos(phi)*sin(theta);
	gradien.v[1] = sin(phi)*sin(theta);
	gradien.v[2] = cos(theta);
	Vector3D oz(0,0,1);	
	Vector3D cam_up;
	Vector3D temp;
	if((theta_ < 180 && theta_ > 0) || theta_ < -180){
		temp = oz*gradien;
	} else {
		temp = gradien*oz;
	}
	if(temp.abs() < 0.01) {
	  cam_up.v[0] = - cos(phi);
		cam_up.v[1] =  - sin(phi);
		cam_up.v[2] = 0.0;
	
 	} else {
	  cam_up = gradien*temp;
	}
	cam_up = cam_up.Unit();
	gluLookAt(gradien.v[0], gradien.v[1], gradien.v[2], 0, 0, 0, cam_up.v[0],cam_up.v[1],cam_up.v[2]);
}



BOOL BigHouseView::OnMouseWheel(UINT nFlags, short zDetal, CPoint point) {
  BOOL ret = FALSE ;
  //if (zDetal >=0) {
  //  m_scaling *= 1.25f;
  //  ret = TRUE ;
  //}
  //else {
  //  m_scaling /= 1.25f;
  //  ret = TRUE ;
  //}
	//**************huu.nv***************
  if (zDetal >=0) {
		rendering_rate_ *= 1.50f;
    ret = TRUE ;
  }
  else {
		rendering_rate_ /= 1.5f;
    ret = TRUE ;
  }

	//***********************************
  InvalidateRect(NULL,FALSE);
  //Set the mouse point
  mouse_down_point_ = point;
  CWnd::OnMouseWheel(nFlags, zDetal, point);
  return  ret;
}



void BigHouseView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
  return;
	// TODO: Add your message handler code here and/or call default
#if 0
	switch (nChar) {
	//Processing Arrow Keys to Synchronise with Movement
	case VK_UP:
    centZ_ = eyeZ_ - 0.5f*(GLfloat)sin(m_AngleX*PI/180.0f);
		eyeZ_ = eyeZ_ - m_PosIncr*(GLfloat)sin(m_AngleX*PI/180.0f);
    centX_ = eyeX_ - 0.5f*(GLfloat)cos(m_AngleX*PI/180.0f);
		eyeX_ = eyeX_ - m_PosIncr*(GLfloat)cos(m_AngleX*PI/180.0f);
		InvalidateRect(NULL,FALSE);
		break;
	case VK_DOWN:
		centZ_ = eyeZ_ - 0.5f*(GLfloat)sin(m_AngleX*PI/180.0f);
		eyeZ_ = eyeZ_ + m_PosIncr*(GLfloat)sin(m_AngleX*PI/180.0f);
		centX_ = eyeX_ - 0.5f*(GLfloat)cos(m_AngleX*PI/180.0f);
		eyeX_ = eyeX_ + m_PosIncr*(GLfloat)cos(m_AngleX*PI/180.0f);
		InvalidateRect(NULL,FALSE);
		break;
	case VK_LEFT:
		m_AngleX = m_AngleX - m_AngIncr;
		centZ_ = eyeZ_ - 0.5f*(GLfloat)sin(m_AngleX*PI/180.0f);
		centX_ = eyeX_ - 0.5f*(GLfloat)cos(m_AngleX*PI/180.0f);
		InvalidateRect(NULL,FALSE);
		break;
	case VK_RIGHT:
		m_AngleX = m_AngleX + m_AngIncr;	
		centZ_ = eyeZ_ - 0.5f*(GLfloat)sin(m_AngleX*PI/180.0f);
		centX_ = eyeX_ - 0.5f*(GLfloat)cos(m_AngleX*PI/180.0f);
		InvalidateRect(NULL,FALSE);
		break;
	case 'A':
    eyeY_ = eyeY_ + m_PosIncr;
		centY_ = eyeY_;
		InvalidateRect(NULL,FALSE);
		break;
	case 'Z':
		eyeY_ = eyeY_ - m_PosIncr;
		centY_ = eyeY_;
		InvalidateRect(NULL,FALSE);
		break;

  case 'S':
    eyeX_ = eyeX_ - m_PosIncr;
    centX_ = eyeX_;
    InvalidateRect(NULL, FALSE);
    break;
  case 'D':
    eyeX_ = eyeX_ + m_PosIncr;
    centX_ = eyeX_;
    InvalidateRect(NULL, FALSE);
    break;
	}
#endif
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void BigHouseView::OnLoadTexture() {
  //Create Texture Names
  glGenTextures(2, m_texture);
  CString str_path = Base::GetPathModule();
  
  LoadTexture(str_path + L"\\bitmap\\Floor.bmp",  0);
  LoadTexture(str_path + L"\\bitmap\\TAB.bmp",  1);
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

void BigHouseView::DrawRoom() { 
	glPushMatrix();
  glTranslatef(-1*ROOM_LENGTH /2.0, -1*ROOM_WIDTH/2.0, 0);

	//glDisable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_texture[0]);

	// A
	//glPushMatrix();
	glBegin(GL_POLYGON);
	//glNormal3f(0, 1.0, 0.0);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0, 0.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
  glVertex3f(0.0f, 0.0f, ROOM_HEIGHT);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(ROOM_LENGTH, 0.0f, ROOM_HEIGHT);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(ROOM_LENGTH, 0.0f, 0.0f);
	glEnd();
	//glPopMatrix();
	//glPushMatrix();
	// B
	glBegin(GL_POLYGON);
	//glNormal3f(0, - 1.0, 0.0);
	glTexCoord2f(0.0f, 0.0f);
  glVertex3f(0.0, ROOM_WIDTH, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0f, ROOM_WIDTH, ROOM_HEIGHT);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(ROOM_LENGTH, ROOM_WIDTH, ROOM_HEIGHT);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(ROOM_LENGTH, ROOM_WIDTH, 0.0f);
	glEnd();
	// C
	//glPopMatrix();
	//glPushMatrix();
	glBegin(GL_POLYGON);
	//glNormal3f(1.0,  0.0, 0.0);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, ROOM_HEIGHT);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.0f, ROOM_WIDTH, ROOM_HEIGHT);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0f, ROOM_WIDTH, 0.0f);
	glEnd();
	//D
	//glPopMatrix();
	//glPushMatrix();
	glBegin(GL_POLYGON);
	// plane
	//glNormal3f( - 1.0, 0.0, 0.0);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(ROOM_LENGTH, 0.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
  glVertex3f(ROOM_LENGTH, 0.0f, ROOM_HEIGHT);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(ROOM_LENGTH, ROOM_WIDTH, ROOM_HEIGHT);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(ROOM_LENGTH, ROOM_WIDTH, 0.0f);
	glEnd();
	//glPopMatrix();

	//glPushMatrix();
	glBegin(GL_POLYGON);
	//glNormal3f(0, 0, 0.0);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0, 0.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(ROOM_LENGTH, 0.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(ROOM_LENGTH, ROOM_WIDTH, 0.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0f, ROOM_WIDTH, 0.0f);
	glEnd();
	//glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void BigHouseView::GetVectorPerpendicularToThescreen(Vector3D &v_oz) {
	float m[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, m);
	Vector3D temp(m[2], m[6], m[10]);
	v_oz = temp.Unit();;
}

int BigHouseView::MoveBody(Vector3D &dir,Vector3D &pos, Vector3D &point_m_on_plane) {
	double d;
	int count = -1;
	bool has_point = false;
	Vector3D point_on_bb;
	for(int i = 0; i < /*all_body_*/shelf_.size(); i ++) {
		Vector3D bbmin ;//= all_body_.at(i).first.bbmin;
		Vector3D bbmax ;//= all_body_.at(i).first.bbmax;
		shelf_.at(i)->GetBoundingBox(bbmin,bbmax);
		Vector3D temp;
		if(LineCutBoundingBox(dir, pos, bbmin, bbmax, temp)) {
			if(!has_point) {
				point_on_bb = temp;
				count = i;
				has_point = true;
			} else {
				Vector3D u = point_on_bb - temp;
				if(u.scalar(dir) > 0) {
					point_on_bb = temp;
					count = i;
				} 
			}
		}
		//Vector3D M = all_body_.at(i).first.bbmax + all_body_.at(i).first.bbmin;
		//M = M*0.5;
		//Vector3D temp = M - pos;
		//Vector3D temp1 = temp*dir;
		//if(i == 0) {
		//	d = temp1.abs()/dir.abs();
		//} else {
		//	double dis = temp1.abs()/dir.abs();
		//	if(d > dis) {
		//		d = dis;
		//		count = i;
		//	}
		//}
		
	}//
	Vector3D oz(0, 0, 1);
	Vector3D O(0, 0, 0);
	if(dir.scalar(oz) != 0) {
		double t = (O.scalar(oz) - oz.scalar(pos))/(oz.scalar(dir));
		Vector3D u = dir*t;
		point_m_on_plane = pos + u;
	return count;
	}
}

bool BigHouseView::LineCutBoundingBox(Vector3D &dir, Vector3D &pos, Vector3D &bbmin, Vector3D &bbmax, Vector3D &p_on_bb) {
	Vector3D normal;
	Vector3D E;
	bool has_a_point = false;
	for(int i = 0; i < 3; i++) {
		Vector3D p = bbmax;
		p.v[i] = bbmin.v[i];
		normal.Set(0, 0, 0);
		normal.v[i] = 1;
		if(LineCutSurface(dir, pos, normal, bbmin, p, E)) {
			if(!has_a_point) {
				p_on_bb = E;
			  has_a_point = true;
			} else {
				Vector3D temp = p_on_bb - E;
				if(temp.scalar(dir) > 0) {
					p_on_bb = E;
				}
			}
		}
	}
	for(int i = 0; i < 3; i++) {
		Vector3D p = bbmin;
		p.v[i] = bbmax.v[i];
		normal.Set(0, 0, 0);
		normal.v[i] = 1;
		if(LineCutSurface(dir, pos, normal, p, bbmax, E)) {
			if(!has_a_point) {
				p_on_bb = E;
			  has_a_point = true;
			} else {
				Vector3D temp = p_on_bb - E;
				if(temp.scalar(dir) > 0) {
					p_on_bb = E;
				}
			}
		}
	}
	return has_a_point;
}

bool BigHouseView::LineCutSurface(Vector3D &dir, Vector3D &pos, Vector3D &n, Vector3D &A, Vector3D &B, Vector3D &E) {
	if(n.scalar(dir) == 0 ) {
		return false;
	}
	double t = (n.scalar(A) - n.scalar(pos))/(n.scalar(dir));
	Vector3D temp = dir*t;
  E = pos + temp;
	for(int i = 0; i < 3; i ++) {
		if(n.v[i] == 0) {
			if(!(A.v[i] < E.v[i] && E.v[i] < B.v[i])) {
				return false;
			}
		}
	}
	return true;
}

void BigHouseView::ResetColor() {
	//for()
}

void BigHouseView::SetCadToView(std::pair<Floor, std::vector<Triangle3D*>> &body) {
	body_ = body;

}

void BigHouseView::MakeShelf(int width, int length, int height, int count_floor) {
	Shelf *shelf = new Shelf(width, length, height, count_floor);
	shelf_.push_back(shelf);
	int size = shelf_.size();
	if(size > 1) {
		Vector3D bbmin;
		shelf_.at(size - 2)->GetBBmin(bbmin);
		bbmin.v[0] = bbmin.v[0] + length;
		shelf_.at(size - 1)->SetBoundingBox(bbmin);
	}
}
