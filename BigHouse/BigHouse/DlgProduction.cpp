// DlgProduction.cpp : implementation file
//

#include "stdafx.h"
#include "BigHouse.h"
#include "DlgProduction.h"
#include "afxdialogex.h"
#include "base.h"
#include "FormBar.h"

// DlgProduction dialog

IMPLEMENT_DYNAMIC(DlgProduction, CDialogEx)

DlgProduction::DlgProduction(CWnd* pParent /*=NULL*/) {
	cad_info_.width = 50;
	cad_info_.lenght = 50;
	cad_info_.height = 50;
	cad_info_.count = 1;
	cad_info_.floor = 0;
	cad_info_.weight = 0;
	cad_info_.name_production = L"";
	cad_info_.type_commodity = TypeCommodity::COMMODITY;
	pos_list_view_ = -1;
}

DlgProduction::~DlgProduction() {

	//pParent_ = NULL;
}

void DlgProduction::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_FILE_OBJ, list_box_ctrl_);
	DDX_Control(pDX, IDC_BITMAP_PICTURE, bitmap_image_ctrl);
	DDX_Control(pDX, IDC_EDIT_CAD_LENGHT, edit_cad_lenght_);
	DDX_Control(pDX, IDC_EDIT_CAD_WIDTH, edit_cad_width_);
	DDX_Control(pDX, IDC_EDIT_CAD_HEIGHT, edit_cad_height_);
	DDX_Control(pDX, IDC_EDIT_COUNT_PRODUCTION , edit_count_commodity_);
	DDX_Control(pDX, IDC_EDIT_WEIGHT, edit_commodity_weight_);
	DDX_Control(pDX, IDC_EDIT_FLOOR, edit_selection_floor_);

}


BEGIN_MESSAGE_MAP(DlgProduction, CDialogEx)
	ON_LBN_SELCHANGE(IDC_LIST_FILE_OBJ, DlgProduction::OnLBSelChange)
	ON_BN_CLICKED(IDC_BTN_NEW_PRODUCT, DlgProduction::NewProduction)
	ON_BN_CLICKED(IDC_BTN_CLEAR, DlgProduction::ClearProduction)
	ON_BN_CLICKED(IDC_BTN_NEW_IMAGE, DlgProduction::NewImage)
	ON_EN_CHANGE(IDC_EDIT_CAD_LENGHT, &OnEditCadLenght)
	ON_EN_CHANGE(IDC_EDIT_CAD_WIDTH, &OnEditCadWidth)
	ON_EN_CHANGE(IDC_EDIT_CAD_HEIGHT, &OnEditCadHeight)
	ON_EN_CHANGE(IDC_EDIT_COUNT_PRODUCTION, &OnEditCountProduction)
	ON_EN_CHANGE(IDC_EDIT_WEIGHT, &OnEditWeighProduction)
	ON_EN_CHANGE(IDC_EDIT_FLOOR, &OnEditSelectionFloor)
	ON_COMMAND(IDOK, OnOk)

END_MESSAGE_MAP()


// DlgProduction message handlers
BOOL DlgProduction::OnInitDialog() {
	CDialogEx::OnInitDialog();
	UpdateProductImage();
	SetBitmap();
	DisplayInfoOnDialog();
	SetStatusDialog(TypeCommodity::COMMODITY);
	Invalidate();
	CButton *clear_btn = static_cast<CButton*>(GetDlgItem(IDC_BTN_CLEAR));
	clear_btn->EnableWindow(FALSE);
  return TRUE;
}

void DlgProduction::Create(CWnd* pParentWnd) {
	CDialog::Create(DlgProduction::IDD, pParentWnd);
	pParent_ = reinterpret_cast<FormBar*>(pParentWnd);
}

void DlgProduction::SetData(CadInfo &info) {
	cad_info_ = info;
	SetBitmap();
	DisplayInfoOnDialog();
	SetStatusDialog(cad_info_.type_commodity);
	UpdateData();
}

void DlgProduction::SetStatusDialog(TypeCommodity type) {
	if(type == TypeCommodity::COMMODITY) {
		edit_count_commodity_.EnableWindow(TRUE);
		edit_cad_lenght_.EnableWindow(TRUE);
		edit_cad_width_.EnableWindow(TRUE);
		edit_cad_height_.EnableWindow(TRUE);
		edit_commodity_weight_.EnableWindow(TRUE);
		edit_selection_floor_.EnableWindow(TRUE);
	} else {
		edit_count_commodity_.EnableWindow(TRUE);
		edit_cad_lenght_.EnableWindow(FALSE);
		edit_cad_width_.EnableWindow(FALSE);
		edit_cad_height_.EnableWindow(FALSE);
		edit_commodity_weight_.EnableWindow(FALSE);
		edit_selection_floor_.EnableWindow(TRUE);
	}
}

void DlgProduction::DisplayInfoOnDialog()	{
	CString str;
	str.Format(_T("%0.2f"),cad_info_.lenght);
	edit_cad_lenght_.SetWindowText(str);
	str.Format(_T("%0.2f"),cad_info_.width);
	edit_cad_width_.SetWindowText(str);
	str.Format(_T("%0.2f"),cad_info_.height);
	edit_cad_height_.SetWindowText(str);
	str.Format(_T("%d"),cad_info_.count);
	edit_count_commodity_.SetWindowText(str);
	str.Format(_T("%d"),cad_info_.floor);
	edit_selection_floor_.SetWindowTextW(str);
	str.Format(_T("%0.2f"),cad_info_.weight);
	edit_commodity_weight_.SetWindowText(str);
}

void DlgProduction::SetBitmap() {
	HandleBitmap(cad_info_.name_production);
	bitmap_image_ctrl.SetBitmap((HBITMAP)cbitmap_);
  UpdateData(FALSE);
}

void DlgProduction::UpdateProductImage() {
	CString str = Base::GetPathModule();
	str = str + _T("\\anh\\");
	std::string str_cad = CStringA(str);
	std::string str_tye("*.bmp");
	str_cad = str_cad + str_tye;

	std::vector<std::string> list_file = Base::ListFileInFolder(str_cad);
	int count = list_box_ctrl_.GetCount();
	for(int i = 0; i < count; i ++) {
		list_box_ctrl_.DeleteString(0);
	}
	for (int i = 0; i < list_file.size(); i++) {
		if(CheckProductionPic(list_file.at(i))) {
			std::string str_pic;
			str_pic = GetPicName(list_file.at(i));
			CA2T str(str_pic.c_str());
			if(i)
			list_box_ctrl_.AddString(str);
		}
	}
	list_box_ctrl_.SetSel(0, true);
  list_box_ctrl_.GetText(0, cad_info_.name_production);
}

std::string DlgProduction::GetPicName(std::string &str_name) {
	if(str_name.length() <= 6) {
		return "";
	}	
	std::string str = "";
	for(int i = 0; i < str_name.length() - 6; i ++) {
		str += str_name.at(i);
	}
	return str;
}

bool DlgProduction::CheckProductionPic(std::string str_name) {
	std::string str = "";
	if(str_name.length() <= 6) {
		return false;
	}
	for(int i = str_name.length() - 6; i < str_name.length(); i ++) {
		str += str_name.at(i);
	}
	if(str == "_1.bmp")
		return true;
	return false;
}

void DlgProduction::OnLBSelChange() {
  int index = list_box_ctrl_.GetCurSel();
  if (index >= 0) {
		list_box_ctrl_.GetText(index, cad_info_.name_production);
		SetBitmap();
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
	CString str = Base::GetPathModule() + ("\\anh\\") + str_name + ("_1") + (".bmp");
	HBITMAP hbitmap =(HBITMAP)::LoadImage(AfxGetInstanceHandle(), str,
																				IMAGE_BITMAP, 200, 200,
																				LR_LOADFROMFILE  |
																				LR_CREATEDIBSECTION);
	if (hbitmap == NULL) {
		str = Base::GetPathModule() + _T("\\picture_default\\box.bmp");
	  hbitmap =(HBITMAP)::LoadImage(AfxGetInstanceHandle(), str,
																					IMAGE_BITMAP, 200, 200,
																					LR_LOADFROMFILE  |
																					LR_CREATEDIBSECTION);
		//MessageBox(L"Ảnh không tồn tại hoặc bị lỗi", L"Bitmap", MB_OK |MB_ICONERROR);
		//LoadProductionBkgn();
		//return;
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

		CString path_product;

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
	/*
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
	*/
}

void DlgProduction::NewImage() {
	CFileDialog dlg(TRUE, L"", L"",
		OFN_HIDEREADONLY | OFN_FILEMUSTEXIST,
		_T("Products File (*.bmp)|*.bmp||"));
	CString str_new_image(_T(""));
	if (dlg.DoModal() == IDOK) {
    str_new_image = dlg.GetPathName();

		CString des_str = /*product_pic_path_ + */dlg.GetFileName(); 
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
	CString str;
	edit_cad_lenght_.GetWindowText(str);
	cad_info_.lenght = _ttof(str);
}
void DlgProduction::OnEditCadWidth() {
	UpdateData(TRUE);
	CString str;
	edit_cad_width_.GetWindowText(str);
	cad_info_.width = _ttof(str);
}
void DlgProduction::OnEditCadHeight() {
	UpdateData(TRUE);
	CString str;
	edit_cad_height_.GetWindowText(str);
	cad_info_.height = _ttof(str);
}

void DlgProduction::GetCadInfor(CadInfo & cad_info) const{
	cad_info = cad_info_;
	//cad_info.name_production = str_current_production_;
}


void DlgProduction::OnEditCountProduction() {
	UpdateData(TRUE);
	CString str;
	edit_count_commodity_.GetWindowText(str);
	cad_info_.count = _ttoi(str);
}
void DlgProduction::OnEditWeighProduction() {
	UpdateData(TRUE);
	CString str;
	edit_commodity_weight_.GetWindowText(str);
	cad_info_.weight = _ttoi(str);
}

void DlgProduction::OnEditSelectionFloor() {
	UpdateData(TRUE);
	CString str;
	edit_selection_floor_.GetWindowText(str);
	cad_info_.floor = _ttoi(str);
}

void DlgProduction::OnOk() {
	pParent_->SetProductionList(cad_info_, pos_list_view_);
	//pParent_->SendMessage(IDC_MESAGER_PRODUCTION, (WPARAM)&cad_info_, 0);
	//SendMessage(WM_ICONERASEBKGND, (WPARAM)pParent_->GetSafeHwnd(), 0);
	//if(str_current_production_ == "" || list_box_ctrl_.GetCount() == 0) {
	//	if(list_box_ctrl_.GetCount() != 0) {
	//		CString current_str;
	//	
	//		list_box_ctrl_.GetText(0, current_str);
	//		// Get file cad Which is selected
	//		str_current_production_ = current_str;
	//		current_str += _T("_0");
	//	
	////		CString file_name = Base::RemoveExtensionFile(current_str);
	//		HandleBitmap(current_str);

	//		CButton *clear_btn = static_cast<CButton*>(GetDlgItem(IDC_BTN_CLEAR));
	//		clear_btn->EnableWindow(TRUE);
	//		CDialog::OnOK();
	//	} else {
	//		AfxMessageBox(_T("Xin vui lòng chọn ảnh"));
	//	}
	//} else {
		CDialog::OnOK();
	//}
}

void DlgProduction::InitCommodity() {
	cad_info_.type_commodity = TypeCommodity::COMMODITY;
	list_box_ctrl_.SetSel(0, true);
  list_box_ctrl_.GetText(0, cad_info_.name_production);
	SetBitmap();
	DisplayInfoOnDialog();
	SetStatusDialog(cad_info_.type_commodity);
	Invalidate();
}