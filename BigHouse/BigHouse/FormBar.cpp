// FormBar.cpp : implementation file
//

#include "stdafx.h"
#include "BigHouse.h"
#include "FormBar.h"
#include "resource.h"
#include "BigHouseView.h"
#include "MainFrm.h"
#include "BigHouse.h"
#include "base.h"
// FormBar

IMPLEMENT_DYNCREATE(FormBar, CFormView)

FormBar::FormBar()
	: CFormView(FormBar::IDD)
{
  object_index_ = -1;
  str_shelf_long_ = L"";
  str_shelf_width_ = L"";
  str_shelf_height_ = L"";
}

FormBar::~FormBar()
{
}

void FormBar::DoDataExchange(CDataExchange* pDX)
{
  CFormView::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_BITMAP_PICTURE, bitmap_image_ctrl);
  DDX_Text(pDX, IDC_EDIT_NUMBER_SHELF, edit_number_of_shelf_);
  DDX_Text(pDX, IDC_EDIT_SHELF_LONG,  edit_shelf_long_);
  DDX_Text(pDX, IDC_EDIT_SHELF_WIDTH, edit_shelf_width_);
  DDX_Text(pDX, IDC_EDIT_SHELF_HEIGH, edit_shelf_height_);
  DDX_Text(pDX, IDC_EDIT_NUMBER_FLOOR, edit_number_of_floor_);
  DDX_Check(pDX, IDC_CHECK_IS_TURNNING_BACK, is_turning_back_);
  DDX_Control(pDX, IDC_LIST_FILE_OBJ, list_box_ctrl_);
}

BEGIN_MESSAGE_MAP(FormBar, CFormView)
  ON_COMMAND(IDC_OBJECT_NEXT, FormBar::OnBnNext)
  ON_COMMAND(IDC_OBJECT_OPTION, FormBar::OnOption)
  ON_EN_CHANGE(IDC_EDIT_NUMBER_SHELF, &FormBar::OnNumberOfShelf)
  ON_EN_CHANGE(IDC_EDIT_SHELF_LONG, &FormBar::OnEditShelfLong)
  ON_EN_CHANGE(IDC_EDIT_SHELF_WIDTH, &FormBar::OnEditShelfWidth)
  ON_EN_CHANGE(IDC_EDIT_SHELF_HEIGH, &FormBar::OnEditShelfHeight)
  ON_EN_CHANGE(IDC_EDIT_NUMBER_FLOOR, &FormBar::OnNumberOfFloor)
  ON_WM_PAINT()
  ON_WM_SIZE()
  ON_LBN_SELCHANGE(IDC_LIST_FILE_OBJ, FormBar::OnLBSelChange)
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

  edit_number_of_shelf_ = L"0";
  edit_shelf_long_ = L"0";
  edit_shelf_height_ = L"0";
  edit_shelf_width_ = L"0";
  edit_number_of_floor_ = L"0";
  is_turning_back_ = 0;

  CButton* btn = reinterpret_cast<CButton*>(GetDlgItem(IDC_OBJECT_NEXT));
  btn->EnableWindow(FALSE);
  UpdateData(FALSE);


  CString str = Base::GetPathModule();
  str = str + _T("\\cad\\");
  std::string str_cad = CStringA(str);
  std::string str_tye("*.stl");
  str_cad = str_cad + str_tye;

  std::vector<std::string> list_file = Base::ListFileInFolder(str_cad);

  for (int i =0; i < list_file.size(); i++) {
    CA2T str( list_file[i].c_str() );
    list_box_ctrl_.AddString(str);
  }
  list_box_ctrl_.SetSel(0, true);


  CButton *option_btn = reinterpret_cast<CButton*>(GetDlgItem(IDC_OBJECT_OPTION));
  int index = list_box_ctrl_.GetCurSel();
  if (index >= 0) {
    CString current_str;
    list_box_ctrl_.GetText(index, current_str);
    option_btn->EnableWindow(TRUE);
    int index_bitmap = CheckBitmap(current_str);
    UpdateBitmap(index_bitmap);
  } else {
     option_btn->EnableWindow(FALSE);
  }

  UpdateData(FALSE);
}

void FormBar::OnPaint() {
  CFormView::OnPaint();
}

void FormBar::OnSize(UINT nType, int cx, int cy) {
  EnableScrollBarCtrl(SB_VERT, FALSE);
  CFormView::OnSize(nType, cx, cy);
}
// set to view
BigHouseView *FormBar::GetBigHouseView() {
  BigHouseApp *pApp= (BigHouseApp*)AfxGetApp();
  MainFrame *pMainFrame = (MainFrame*)pApp->m_pMainWnd;
  BigHouseView*pView = reinterpret_cast<BigHouseView*>(pMainFrame->splitter_.GetPane(0, 1));
  return pView;
}

void FormBar::OnBnNext() {
}

void FormBar::UpdateBitmap(int index_bitmap) {
  switch(index_bitmap) {
  case 0: {
    CBitmap cbitmap;
    cbitmap.LoadBitmap(IDB_BITMAP_TABLE);
    bitmap_image_ctrl.SetBitmap((HBITMAP)cbitmap);
    break;
  }
  case 1: {
    CBitmap cbitmap;
    cbitmap.LoadBitmap(IDB_BITMAP_DESK);
    bitmap_image_ctrl.SetBitmap((HBITMAP)cbitmap);
    break;
  }
  case 2: {
    break;
  }
  case 3: {
    break;
  }
  case 4: {
    break;
  }
  case 5: {
    break;
  }
  default : {
    break;
   }
  }
}

void FormBar::OnOption() {
  int current_index = list_box_ctrl_.GetCurSel();
  CString current_str;
  list_box_ctrl_.GetText(current_index, current_str);
  theApp.LoadFileCad(current_str);
  int index_bitmap = CheckBitmap(current_str);
  UpdateBitmap(index_bitmap);

  // Get info from fomrview to view
  float longs = _ttof(edit_shelf_long_);
  float height = _ttof(str_shelf_height_);
  float width = _ttof(str_shelf_width_);
  float nums = _ttof(str_number_of_shelf_);
  float numf = _ttof(str_number_of_floor_);
  bool is_tb = false;


  GetBigHouseView()->SetShefLong(longs);
  GetBigHouseView()->SetShefWidth(width);
  GetBigHouseView()->SetShefHeight(height);
  GetBigHouseView()->SetNumberOfShelf(nums);
  GetBigHouseView()->SetNumberOfFloor(numf);

  UpdateData(TRUE);
  if (is_turning_back_ == 1) 
    is_tb = true;
  else 
    is_tb = false;
  GetBigHouseView()->SetIsTurningBack(is_tb);
}

int FormBar::CheckBitmap(CString str) {
  if (str == "table.stl") {
    return 0;
  } else if (str == "desk.stl") {
    return 1;
  } else {
    return -1;
  }
}

void FormBar::OnNumberOfFloor() {
  UpdateData(TRUE);
  str_number_of_floor_ = edit_number_of_floor_;
}

void FormBar::OnNumberOfShelf() {
  UpdateData(TRUE);
  str_number_of_shelf_ = edit_number_of_shelf_;
}

void FormBar::OnEditShelfLong() {
  UpdateData(TRUE);
  str_shelf_long_ = edit_shelf_long_;
}

void FormBar::OnEditShelfWidth() {
  UpdateData(TRUE);
  str_shelf_width_ = edit_shelf_width_;
}

void FormBar::OnEditShelfHeight() {
  UpdateData(TRUE);
  str_shelf_height_ = edit_shelf_height_;
}

void FormBar::OnLBSelChange() {
  CButton *option_btn = reinterpret_cast<CButton*>(GetDlgItem(IDC_OBJECT_OPTION));
  int index = list_box_ctrl_.GetCurSel();
  if (index >= 0) {
    CString current_str;
    list_box_ctrl_.GetText(index, current_str);
    option_btn->EnableWindow(TRUE);
    int index_bitmap = CheckBitmap(current_str);
    UpdateBitmap(index_bitmap);
  } else {
     option_btn->EnableWindow(FALSE);
  }
}