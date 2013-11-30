// DlgProduction.cpp : implementation file
//

#include "stdafx.h"
#include "BigHouse.h"
#include "DlgProduction.h"
#include "afxdialogex.h"
#include "base.h"

// DlgProduction dialog

IMPLEMENT_DYNAMIC(DlgProduction, CDialogEx)

DlgProduction::DlgProduction(CWnd* pParent /*=NULL*/)
	: CDialogEx(DlgProduction::IDD, pParent)
{

}

DlgProduction::~DlgProduction()
{
}

void DlgProduction::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_FILE_OBJ, list_box_ctrl_);
	DDX_Control(pDX, IDC_BITMAP_PICTURE, bitmap_image_ctrl);
}


BEGIN_MESSAGE_MAP(DlgProduction, CDialogEx)
	ON_LBN_SELCHANGE(IDC_LIST_FILE_OBJ, DlgProduction::OnLBSelChange)
END_MESSAGE_MAP()


// DlgProduction message handlers
BOOL DlgProduction::OnInitDialog() {
	CDialogEx::OnInitDialog();

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


  int index = list_box_ctrl_.GetCurSel();
  if (index >= 0) {
    CString current_str;
    list_box_ctrl_.GetText(index, current_str);
    int index_bitmap = CheckBitmap(current_str);
    UpdateBitmap(index_bitmap);
  } else {
    CBitmap cbitmap;
    cbitmap.LoadBitmap(IDB_BITMAP_BKGN);
    bitmap_image_ctrl.SetBitmap((HBITMAP)cbitmap);
  }

  UpdateData(FALSE);

  return TRUE;
}

void DlgProduction::OnLBSelChange() {
  int index = list_box_ctrl_.GetCurSel();
  if (index >= 0) {
    CString current_str;
    list_box_ctrl_.GetText(index, current_str);
		str_current_production_ = current_str;
    int index_bitmap = CheckBitmap(current_str);
    UpdateBitmap(index_bitmap);
  }
}

int DlgProduction::CheckBitmap(CString str) {
  if (str == "table.stl" || str == "table.STL") {
    return 0;
  } else if (str == "desk.stl" || str == "desk.STL") {
    return 1;
  } else {
    return -1;
  }
}

void DlgProduction::UpdateBitmap(int index_bitmap) {
  switch(index_bitmap) {
  case 0: {
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
    CBitmap cbitmap;
    cbitmap.LoadBitmap(IDB_BITMAP_BKGN);
    bitmap_image_ctrl.SetBitmap((HBITMAP)cbitmap);
    break;
    break;
   }
  }
}