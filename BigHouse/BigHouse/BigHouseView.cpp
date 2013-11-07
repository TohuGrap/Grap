
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
const float ROOM_LENGTH = 100.0f;
const	float ROOM_WIDTH = 100.0f;
const float ROOM_HEIGHT = 20.0f;

#define M_PI 3.14
IMPLEMENT_DYNCREATE(BigHouseView, CView)

BEGIN_MESSAGE_MAP(BigHouseView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &BigHouseView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
  ON_WM_SIZE()
  ON_WM_CREATE()
  ON_WM_KEYDOWN()
  ON_WM_RBUTTONDOWN()
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

  m_OrthoRangeLeft = -1.5f;
	m_OrthoRangeRight = 1.5f;
	m_OrthoRangeBottom = -1.5f;
	m_OrthoRangeTop = 1.5f;
	m_OrthoRangeNear = -20.0f;
	m_OrthoRangeFar = 100.0f;
  rendering_rate_ = 2.0f;
  m_scaling = 1.0f;
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
  // ::gluPerspective(45.0f, aspect_ratio, .01f, 200.0f);

  SetViewFrustum();

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
	//::glEnable(GL_DEPTH_TEST);

  // Enable color tracking
  ::glEnable(GL_COLOR_MATERIAL);
  ::glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

  ::glShadeModel(GL_SMOOTH);
  // Setup lighting and material

  //glEnable(GL_CULL_FACE);
  //SetupLight();
  OnLoadTexture();
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
  // Enable lighting
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_NORMALIZE); 
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
 // // setup light mode
 // glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);
 // glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);

 // GLfloat m_SceneAmbient1[]  = {0.2f,0.3f,0.3f,1.0f};
	//GLfloat m_SceneDiffuse1[]  = {0.2f,0.4f,0.5f,1.0f};
	//GLfloat m_SceneSpecular1[] = {1.0f,1.0f,1.0f,1.0f};
	//GLfloat m_ScenePosition1[] = {0.0f,0.0f,0.0f,1.0f};
	//GLfloat m_SceneDirection1[]= {0.0f,0.0f,-1.0f,1.0f};

	//glLightfv(GL_LIGHT0, GL_AMBIENT, m_SceneAmbient1); 
	//glLightfv(GL_LIGHT0, GL_DIFFUSE, m_SceneDiffuse1); 
	//glLightfv(GL_LIGHT0, GL_SPECULAR, m_SceneSpecular1); 
	//glLightfv(GL_LIGHT0, GL_POSITION, m_ScenePosition1);
	//glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 45.0f);
	//glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION,m_SceneDirection1);
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

  glPushMatrix();
  glTranslatef(x_position_, y_position_, - 100.0f);
  glRotatef(angle_x_ea_ - 45.0, 1.0f, 0.0f, 0.0f);
  glRotatef(angle_z_ea_ -135, 0.0f, 0.0f, 1.0f);

  glScalef(m_scaling, m_scaling, m_scaling);

  SetupLight();
  DrawCad();
  DisableLight();

	DrawRoom();

  glPopMatrix();
}

void BigHouseView::DrawCad() {
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  int a = theApp.GetNumberOfPoint();
  glColor3f(1.0, 1.0, 0.0);
  for (unsigned long i = 0; i < theApp.GetNumberOfPoint(); i = i+3) {
    glBegin(GL_POLYGON);
    glNormal3f(theApp.GetNormalVector()[i/3][0], theApp.GetNormalVector()[i/3][1], theApp.GetNormalVector()[i/3][2]); 
    //glTexCoord2f(0.0f, 0.0f);
	  glVertex3f(theApp.GetTrianglePoint()->Vertex[i][0], theApp.GetTrianglePoint()->Vertex[i][1], theApp.GetTrianglePoint()->Vertex[i][2]);
    //glTexCoord2f(1.0f, 0.0f);
	  glVertex3f(theApp.GetTrianglePoint()->Vertex[i+1][0], theApp.GetTrianglePoint()->Vertex[i+1][1], theApp.GetTrianglePoint()->Vertex[i+1][2]);
    //glTexCoord2f(0.0f, 1.0f);
	  glVertex3f(theApp.GetTrianglePoint()->Vertex[i+2][0], theApp.GetTrianglePoint()->Vertex[i+2][1], theApp.GetTrianglePoint()->Vertex[i+2][2]);
	glEnd();
  
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
  //CView::OnLButtonUp(nFlags, point);
}

void BigHouseView::OnRButtonDown(UINT nFlags, CPoint point) {
   mouse_down_point_ = point;
  SetCapture();
  //CView::OnLButtonDown(nFlags, point);
}


void BigHouseView::OnMouseMove(UINT nFlags, CPoint point) {
  // TODO: Add your message handler code here and/or call default
  // Check if we have captured the mouse
  CView::OnMouseMove(nFlags, point);
  if (GetCapture() == this) {
    //Increment the object rotation angles
    angle_x_ea_ += (point.y - mouse_down_point_.y)/3.6;
    angle_z_ea_ += (point.x - mouse_down_point_.x)/3.6;
      //Redraw the view
    InvalidateRect(NULL, FALSE);
      //Set the mouse point
    mouse_down_point_ = point;
  }
}

BOOL BigHouseView::OnMouseWheel(UINT nFlags, short zDetal, CPoint point) {
  BOOL ret = FALSE ;
  if (zDetal >=0) {
    m_scaling *= 1.25f;
    ret = TRUE ;
  }
  else {
    m_scaling /= 1.25f;
    ret = TRUE ;
  }
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
