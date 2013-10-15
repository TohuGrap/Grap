// FormBar.cpp : implementation file
//

#include "stdafx.h"
#include "BigHouse.h"
#include "FormBar.h"
#include "resource.h"
#include "BigHouseView.h"
#include "MainFrm.h"
#include "BigHouse.h"
// FormBar

IMPLEMENT_DYNCREATE(FormBar, CFormView)

FormBar::FormBar()
	: CFormView(FormBar::IDD)
{
  cbitmap_.LoadBitmap(IDB_BITMAP_RECTANGLE);
  bitmap_index_ = 0;
  object_index_ = -1;
  str_x_pos_ = L"";
  str_y_pos_ = L"";
  str_z_pos_ = L"";
}

FormBar::~FormBar()
{
}

void FormBar::DoDataExchange(CDataExchange* pDX)
{
  CFormView::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_BITMAP_PICTURE, bitmap_image_ctrl);
  DDX_Text(pDX, IDC_EDIT_X_POS, edit_x_pos_);
  DDX_Text(pDX, IDC_EDIT_Y_POS, edit_y_pos_);
  DDX_Text(pDX, IDC_EDIT_Z_POS, edit_z_pos_);
}

BEGIN_MESSAGE_MAP(FormBar, CFormView)
  ON_COMMAND(IDC_OBJECT_NEXT, FormBar::OnBnNext)
  ON_COMMAND(IDC_OBJECT_OPTION, FormBar::OnOption)
  //ON_COMMAND(IDC_EDIT_X_POS, FormBar::OnEditChangeXpos)
  //ON_COMMAND(IDC_EDIT_Y_POS, FormBar::OnEditChangeYpos)
  //ON_COMMAND(IDC_EDIT_Z_POS, FormBar::OnEditChangeZpos)
  ON_WM_PAINT()
END_MESSAGE_MAP()


// FormBar diagnostics

#ifdef _DEBUG
void FormBar::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void FormBar::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// FormBar message handlers


void FormBar::OnInitialUpdate() {
  CFormView::OnInitialUpdate();
  bitmap_image_ctrl.SetBitmap((HBITMAP)cbitmap_);

  edit_x_pos_ = "0";
  edit_y_pos_ = "0";
  edit_z_pos_ = "0";
  UpdateData(FALSE);
}

void FormBar::OnPaint() {
  CFormView::OnPaint();
}


BigHouseView *FormBar::GetBigHouseView() {
  BigHouseApp *pApp= (BigHouseApp*)AfxGetApp();
  MainFrame *pMainFrame = (MainFrame*)pApp->m_pMainWnd;
  BigHouseView*pView = reinterpret_cast<BigHouseView*>(pMainFrame->splitter_.GetPane(0, 1));
  return pView;
}

void FormBar::OnBnNext() {
  bitmap_index_++;
  switch(bitmap_index_) {
  case 0: {
    CBitmap cbitmap;
    cbitmap.LoadBitmap(IDB_BITMAP_RECTANGLE);
    bitmap_image_ctrl.SetBitmap((HBITMAP)cbitmap);
    break;
  }
  case 1: {
    CBitmap cbitmap;
    cbitmap.LoadBitmap(IDB_BITMAP_CONE);
    bitmap_image_ctrl.SetBitmap((HBITMAP)cbitmap);
    break;
  }
  case 2: {
    CBitmap cbitmap;
    cbitmap.LoadBitmap(IDB_BITMAP_TRY_BA_PY);
    bitmap_image_ctrl.SetBitmap((HBITMAP)cbitmap);
    break;
  }
  case 3: {
    CBitmap cbitmap;
    cbitmap.LoadBitmap(IDB_BITMAP_SPHERE);
    bitmap_image_ctrl.SetBitmap((HBITMAP)cbitmap);
    break;
  }
  case 4: {
    CBitmap cbitmap;
    cbitmap.LoadBitmap(IDB_BITMAP_TEARPORT);
    bitmap_image_ctrl.SetBitmap((HBITMAP)cbitmap);
    break;
  }
  case 5: {
    CBitmap cbitmap;
    cbitmap.LoadBitmap(IDB_BITMAP_TOURS);
    bitmap_image_ctrl.SetBitmap((HBITMAP)cbitmap);
    break;
  }
  default : {
    bitmap_index_ = 0;
    CBitmap cbitmap;
    cbitmap.LoadBitmap(IDB_BITMAP_RECTANGLE);
    bitmap_image_ctrl.SetBitmap((HBITMAP)cbitmap);
    break;
   }
  }
}

void FormBar::OnOption() {
  GetBigHouseView()->SetIndexObject(bitmap_index_);

  float pos[3];

  UpdateData(TRUE);
  str_x_pos_ = edit_x_pos_;
  str_y_pos_ = edit_y_pos_;
  str_z_pos_ = edit_z_pos_;

  pos[0] = _tstof(str_x_pos_);
  pos[1] = _tstof(str_y_pos_);
  pos[2] = _tstof(str_z_pos_);

  GetBigHouseView()->SetPosValue(pos);
}

void FormBar::OnEditChangeXpos() {
  str_x_pos_ = edit_x_pos_;
}

void FormBar::OnEditChangeYpos() {
  UpdateData(TRUE);
  str_y_pos_ = edit_y_pos_;
}

void FormBar::OnEditChangeZpos() {
  UpdateData(TRUE);
  str_z_pos_ = edit_z_pos_;
}