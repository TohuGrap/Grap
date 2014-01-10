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
	module_path_ = Base::GetPathModule();
	product_cad_path_  = module_path_ + _T("\\cad\\");
  product_bitmap_path_ = module_path_ + _T("\\bitmap_cad\\");
  str_cad_lenght_ = L"50";
  str_cad_width_ = L"50";
  str_cad_height_ = L"50";
	str_cad_proportion_ = L"1";
	cad_info_.width = 50;
	cad_info_.lenght = 50;
	cad_info_.height = 50;
	cad_info_.proportion = 1;
	cad_info_.change_proportion = true;
}

DlgProduction::~DlgProduction()
{
}

void DlgProduction::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_FILE_OBJ, list_box_ctrl_);
	DDX_Control(pDX, IDC_BITMAP_PICTURE, bitmap_image_ctrl);
	DDX_Control(pDX, IDC_EDIT_CAD_LENGHT, edit_cad_lenght_);
	DDX_Control(pDX, IDC_EDIT_CAD_WIDTH, edit_cad_width_);
	DDX_Control(pDX, IDC_EDIT_CAD_HEIGHT, edit_cad_height_);
	DDX_Control(pDX, IDC_CHECKBOX_PROPORTION , check_proportion_);
	DDX_Control(pDX, IDC_EDIT_PROPORTION , edit_cad_proportion_);

}


BEGIN_MESSAGE_MAP(DlgProduction, CDialogEx)
	ON_LBN_SELCHANGE(IDC_LIST_FILE_OBJ, DlgProduction::OnLBSelChange)
	ON_BN_CLICKED(IDC_BTN_NEW_PRODUCT, DlgProduction::NewProduction)
	ON_BN_CLICKED(IDC_BTN_CLEAR, DlgProduction::ClearProduction)
	ON_BN_CLICKED(IDC_BTN_NEW_IMAGE, DlgProduction::NewImage)
	ON_BN_CLICKED(IDC_CHECKBOX_PROPORTION , OnCheckProportion)
	ON_EN_CHANGE(IDC_EDIT_CAD_LENGHT, &OnEditCadLenght)
	ON_EN_CHANGE(IDC_EDIT_CAD_WIDTH, &OnEditCadWidth)
	ON_EN_CHANGE(IDC_EDIT_CAD_HEIGHT, &OnEditCadHeight)
	ON_EN_CHANGE(IDC_EDIT_PROPORTION, &OnEditProportion)
END_MESSAGE_MAP()


// DlgProduction message handlers
BOOL DlgProduction::OnInitDialog() {
	CDialogEx::OnInitDialog();

	bitmap_image_ctrl.SetBitmap((HBITMAP)cbitmap_);

	UpdateProduct();

  int index = list_box_ctrl_.GetCurSel();
  if (index >= 0) {
    CString current_str;
    list_box_ctrl_.GetText(index, current_str);
		CString name_bitmap = Base::RemoveExtensionFile(current_str);
		HandleBitmap(name_bitmap);
  } else {
		cbitmap_.LoadBitmap(IDB_BITMAP_BKGN);
		bitmap_image_ctrl.SetBitmap((HBITMAP)cbitmap_);
  }

  UpdateData(FALSE);
	check_proportion_.SetCheck(1);
	edit_cad_lenght_.SetWindowText(str_cad_lenght_);
	edit_cad_width_.SetWindowText(str_cad_width_);
	edit_cad_height_.SetWindowText(str_cad_height_);
	edit_cad_proportion_.SetWindowText(str_cad_proportion_);

	edit_cad_lenght_.EnableWindow(false);
  edit_cad_width_.EnableWindow(false);
  edit_cad_height_.EnableWindow(false);
	edit_cad_proportion_.EnableWindow(true);


	CButton *clear_btn = static_cast<CButton*>(GetDlgItem(IDC_BTN_CLEAR));
	clear_btn->EnableWindow(FALSE);
  return TRUE;
}

void DlgProduction::UpdateProduct() {
	CString str = product_cad_path_;
	std::string str_cad = CStringA(str);
	std::string str_tye("*.stl");
	str_cad = str_cad + str_tye;

	std::vector<std::string> list_file = Base::ListFileInFolder(str_cad);

	for (int i =0; i < list_file.size(); i++) {
		CA2T str( list_file[i].c_str() );
		list_box_ctrl_.AddString(str);
	}
	list_box_ctrl_.SetSel(0, true);
}

void DlgProduction::OnLBSelChange() {
  int index = list_box_ctrl_.GetCurSel();
  if (index >= 0) {
    CString current_str;
    list_box_ctrl_.GetText(index, current_str);
		// Get file cad Which is selected
		str_current_production_ = current_str;
		CString file_name = Base::RemoveExtensionFile(current_str);
    HandleBitmap(file_name);

		CButton *clear_btn = static_cast<CButton*>(GetDlgItem(IDC_BTN_CLEAR));
		clear_btn->EnableWindow(TRUE);
  }
}
void DlgProduction::LoadProductionBkgn() {
	cbitmap_.Detach();
	cbitmap_.LoadBitmap(IDB_BITMAP_BKGN);
	bitmap_image_ctrl.SetBitmap((HBITMAP)cbitmap_);
}


void DlgProduction::HandleBitmap(CString str_name) {
	cbitmap_.Detach();
	CString str = product_bitmap_path_ + str_name + (".bmp");

	HBITMAP hbitmap =(HBITMAP)::LoadImage(AfxGetInstanceHandle(), str,
																				IMAGE_BITMAP, 0, 0,
																				LR_LOADFROMFILE  |
																				LR_CREATEDIBSECTION);
	if (hbitmap == NULL) {
		MessageBox(L"Ảnh không tồn tại hoặc bị lỗi", L"Bitmap", MB_OK |MB_ICONERROR);
		LoadProductionBkgn();
		return;
	}
	cbitmap_.Attach(hbitmap); 
	bitmap_image_ctrl.SetBitmap((HBITMAP)cbitmap_);
}

void DlgProduction::NewProduction() {
  CFileDialog dlg(TRUE, L"", L"",
		              OFN_HIDEREADONLY | OFN_FILEMUSTEXIST,
									_T("Products File (*.stl)|*.stl||"));
	CString file_path = _T("");
	CString file_name = _T("");

	if (dlg.DoModal() == IDOK) {
	  file_path = dlg.GetPathName();
		file_name = dlg.GetFileName();

		CString path_product = product_cad_path_ + file_name;

		const char * src_path = Base::CStringToChar(file_path);

		const char * des_path = Base::CStringToChar(path_product);

		bool ret = Base::CopyFileIntoFolder(src_path, des_path);

		if (ret) {
			MessageBox(L"Thêm Mới Thành Công", L"Thêm Mới", MB_OK|MB_ICONINFORMATION); 
			list_box_ctrl_.AddString(file_name);
		} else {	
			MessageBox(L"Thêm Mới Thất Bại", L"Thêm Mới", MB_OK|MB_ICONERROR); 
		}
	}
}

void DlgProduction::ClearProduction() {
  int index = list_box_ctrl_.GetCurSel();
	CString str;
	list_box_ctrl_.GetText(index, str);
	
	CString str_bitmap = Base::RemoveExtensionFile(str);
	CString clear_file = product_cad_path_ + str;
	CString clear_bitmap_file = product_bitmap_path_ + str_bitmap + (".bmp");

  const char* clear_fl = Base::CStringToChar(clear_file);
	const char* clear_bm = Base::CStringToChar(clear_bitmap_file);

	bool ret = Base::RemoveFile(clear_fl);
	bool ret2 = Base::RemoveFile(clear_bm);

	if (ret == true) {
		MessageBox(L"Xóa File Thành Công", L"Xóa File", MB_OK |MB_ICONINFORMATION);
		list_box_ctrl_.DeleteString(index);
		int index2 = list_box_ctrl_.GetCurSel();
		CString str_cur;
		if (index2 < 0) {
			LoadProductionBkgn();
			return;
		} else {
			list_box_ctrl_.GetText(index2, str_cur);
			HandleBitmap(Base::RemoveExtensionFile(str_cur));
		}
	} else {
    MessageBox(L"Xóa File Thât Bại", L"Xóa File", MB_OK |MB_ICONERROR);
	}
}

void DlgProduction::NewImage() {
	CFileDialog dlg(TRUE, L"", L"",
		OFN_HIDEREADONLY | OFN_FILEMUSTEXIST,
		_T("Products File (*.bmp)|*.bmp||"));
	CString str_new_image(_T(""));
	if (dlg.DoModal() == IDOK) {
    str_new_image = dlg.GetPathName();

		CString des_str = product_bitmap_path_ + dlg.GetFileName(); 
	  const wchar_t* src_wstr = str_new_image.GetString();
		const wchar_t* des_wstr = des_str.GetString();

		// check info of bitmap
		HBITMAP hbitmap =(HBITMAP)::LoadImage(AfxGetInstanceHandle(), str_new_image,
																					IMAGE_BITMAP, 0, 0,
																					LR_LOADFROMFILE  |
																					LR_CREATEDIBSECTION);
		if(hbitmap == NULL) {
      MessageBox(L"File ảnh này bị lỗi", L"Bitmap", MB_OK |MB_ICONERROR);
			return;
		}
		CBitmap cbit;
		cbit.Attach(hbitmap);
		BITMAP bitmap;
		cbit.GetBitmap(&bitmap);
		int width = bitmap.bmWidth;
		int height = bitmap.bmHeight;
		if (width != 200 && height != 200) {
		  MessageBox(L"Kích thước phải là 200 x200", L"Bitmap", MB_OK |MB_ICONERROR);
			return;
		}
		bool ret = Base::CopyFiles(src_wstr, des_wstr);
		if (ret == true) {
      MessageBox(L"Thêm Ảnh Thành Công", L"Copy File", MB_OK |MB_ICONINFORMATION);

			int index2 = list_box_ctrl_.GetCurSel();
			CString str_cur;
			if (index2 < 0) {
				LoadProductionBkgn();
				return;
			} else {
				list_box_ctrl_.GetText(index2, str_cur);
				HandleBitmap(Base::RemoveExtensionFile(str_cur));
			}
		} else {
      MessageBox(L"Thêm Ảnh Thất Bại", L"Copy File", MB_OK |MB_ICONERROR);
		}
	}
}

void DlgProduction::OnEditCadLenght() {
	UpdateData(TRUE);
	edit_cad_lenght_.GetWindowText(str_cad_lenght_);
	cad_info_.lenght = _ttof(str_cad_lenght_);
}
void DlgProduction::OnEditCadWidth() {
	UpdateData(TRUE);
	edit_cad_width_.GetWindowText(str_cad_width_);
	cad_info_.width = _ttof(str_cad_width_);
}
void DlgProduction::OnEditCadHeight() {
	UpdateData(TRUE);
	edit_cad_height_.GetWindowText(str_cad_height_);
	cad_info_.height = _ttof(str_cad_height_);
}

void DlgProduction::GetCadInfor(CadInfo & cad_info) const{
	cad_info = cad_info_;
	cad_info.production = str_current_production_;
}

void DlgProduction::OnCheckProportion() {
	UpdateData(true);
	if(check_proportion_.GetCheck() == 0) {
		cad_info_.change_proportion = false;

		edit_cad_lenght_.EnableWindow(true);
    edit_cad_width_.EnableWindow(true);
    edit_cad_height_.EnableWindow(true);
		edit_cad_proportion_.EnableWindow(false);
	} else {
		edit_cad_lenght_.EnableWindow(false);
    edit_cad_width_.EnableWindow(false);
    edit_cad_height_.EnableWindow(false);
		edit_cad_proportion_.EnableWindow(true);

		cad_info_.change_proportion = true;
	}
	UpdateData(false);
}

void DlgProduction::OnEditProportion() {
	UpdateData(TRUE);
	edit_cad_proportion_.GetWindowText(str_cad_proportion_);
	cad_info_.proportion = _ttof(str_cad_proportion_);
}
