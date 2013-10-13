
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

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// BigHouseView

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
END_MESSAGE_MAP()

// BigHouseView construction/destruction

BigHouseView::BigHouseView():
  eyeX_(0.0f),
  eyeY_(0.5f),
  eyeZ_(2.5f),
  centX_(0.0f),
  centY_(0.5f),
  centZ_(2.25f)
{
  m_PosIncr = 0.25f;
	m_AngIncr = 5.0f;
	m_AngleX = 90.0f;

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
  glPushMatrix();
  RenderScene();
  glPopMatrix();

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

void BigHouseView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
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

  // Define viewport = size window
  glViewport(0, 0, cx, cy);
  GLfloat aspect_ratio = (GLdouble)cx/(GLdouble)cy;

  // Select the projection matrix
  // here, option aspection view
  ::glMatrixMode(GL_PROJECTION);
  ::glLoadIdentity();
  gluPerspective(60.0f, aspect_ratio, 0.01, 100.0);

  // Select MatrixModelView
  ::glMatrixMode(GL_MODELVIEW);
  ::glLoadIdentity();
}

int BigHouseView::OnCreate(LPCREATESTRUCT lpCreateStruct) {
  if (CView::OnCreate(lpCreateStruct) == -1)
    return -1;

  InitializeOpenGL();
  return 0;
}

void BigHouseView::OnDestroy() {
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
  SetupLight();

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

  // setup light mode
  glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);

  GLfloat m_SceneAmbient1[]  = {0.5f,0.5f,0.5f,1.0f};
	GLfloat m_SceneDiffuse1[]  = {1.0f,1.0f,1.0f,1.0f};
	GLfloat m_SceneSpecular1[] = {1.0f,1.0f,1.0f,1.0f};
	GLfloat m_ScenePosition1[] = {1.0f,1.5f,-1.0f,1.0f};
	GLfloat m_SceneDirection1[]= {0.0f,0.0f,-1.0f,1.0f};

	glLightfv(GL_LIGHT0,GL_AMBIENT,m_SceneAmbient1); 
	glLightfv(GL_LIGHT0,GL_DIFFUSE,m_SceneDiffuse1); 
	glLightfv(GL_LIGHT0,GL_SPECULAR,m_SceneSpecular1); 
	glLightfv(GL_LIGHT0,GL_POSITION,m_ScenePosition1);
	glLightf(GL_LIGHT0,GL_SPOT_CUTOFF,75.0f);
	glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION,m_SceneDirection1);
}

void BigHouseView::DisableLight() {
  glDisable(GL_LIGHTING);
}

void BigHouseView::RenderScene() {
  glLoadIdentity();
  gluLookAt(eyeX_, eyeY_, eyeZ_, centX_, centY_, centZ_, 0.0f, 1.0f, 0.0f);
  glTranslated(0.0f, 0.0f, 0.0f);
  glPushMatrix();
  DrawGround();
  glPopMatrix();

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
  glLineWidth(2.0f);
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
  glVertex3f(10.0, 0.0f, 10.0f);
  glVertex3f(10.0f, 0.0f, -10.0f);
  glVertex3f(-10.0f, 0.0f, -10.0f);
  glVertex3f(-10.0f, 0.0, 10.0f);
  glEnd();
}


void BigHouseView::DrawRectangle(int length) {
  glutSolidCube(length);
}

void BigHouseView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	// TODO: Add your message handler code here and/or call default
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
	
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}