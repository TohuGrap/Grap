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
#include "SetupRoomDlg.h"
// FormBar

IMPLEMENT_DYNCREATE(FormBar, CFormView)

FormBar::FormBar()
	: CFormView(FormBar::IDD)
{
  object_index_ = -1;
  size_room_.long_room = 600.0f;
  size_room_.width_room = 400.0f;
  size_room_.height_room = 120.0f;
}

FormBar::~FormBar()
{
}

void FormBar::DoDataExchange(CDataExchange* pDX)
{
  CFormView::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_BITMAP_PICTURE, bitmap_image_ctrl);
  DDX_Control(pDX, IDC_LIST_FILE_OBJ, list_box_ctrl_);
  DDX_Control(pDX, IDC_BUTTON_SETUP_ROOM, setup_room_btn_);
}

BEGIN_MESSAGE_MAP(FormBar, CFormView)
  ON_COMMAND(IDC_OBJECT_OPTION, FormBar::OnOption)
  ON_COMMAND(IDC_BUTTON_SETUP_ROOM, FormBar::OnOpenSetupRoomDlg)
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
     UpdateBitmap(-1);
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
BigHouseView *FormBar::GetBigHouseView() {
  BigHouseApp *pApp= (BigHouseApp*)AfxGetApp();
  MainFrame *pMainFrame = (MainFrame*)pApp->m_pMainWnd;
  BigHouseView*pView = reinterpret_cast<BigHouseView*>(pMainFrame->splitter_.GetPane(0, 1));
  return pView;
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
    CBitmap cbitmap;
    cbitmap.LoadBitmap(IDB_BITMAP_TABLE_1);
    bitmap_image_ctrl.SetBitmap((HBITMAP)cbitmap);
    break;
  }
  case 3: {
    CBitmap cbitmap;
    cbitmap.LoadBitmap(IDB_BITMAP_CHAIR);
    bitmap_image_ctrl.SetBitmap((HBITMAP)cbitmap);
    break;
  }
  case 4: {
    CBitmap cbitmap;
    cbitmap.LoadBitmap(IDB_BITMAP_LONG_TABLE);
    bitmap_image_ctrl.SetBitmap((HBITMAP)cbitmap);
    break;
  }
  case 5: {
    break;
  }
  default : {
    CBitmap cbitmap;
    cbitmap.LoadBitmap(IDB_BITMAP_BKGN);
    bitmap_image_ctrl.SetBitmap((HBITMAP)cbitmap);
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
}

int FormBar::CheckBitmap(CString str) {
  if (str == "table.STL") {
    return 0;
  } else if (str == "desk.STL") {
    return 1;
  } else if (str =="table_1.STL") {
    return 2;
  } else if (str == "chair.STL") {
    return 3;
  } else if (str == "long_table.STL") {
    return 4;
  } else {
    return -1;
  }
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

void FormBar::OnOpenSetupRoomDlg() {
  SetupRoomDlg dlg(size_room_.long_room, size_room_.width_room, size_room_.height_room);
  if (dlg.DoModal() == IDOK) {
    size_room_ = dlg.GetSizeRoom();
  }

  GetBigHouseView()->InvalidateRect(NULL, FALSE);
}