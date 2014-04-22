// DlgSettingShelf.cpp : implementation file
//

#include "stdafx.h"
#include "BigHouse.h"
#include "DlgSettingShelf.h"
#include "afxdialogex.h"
#include "FormBar.h"


// DlgSettingShelf dialog

IMPLEMENT_DYNAMIC(DlgSettingShelf, CDialogEx)

DlgSettingShelf::DlgSettingShelf(CWnd* pParent /*=NULL*/) {
	shelf_info_.longs = 200;
	shelf_info_.width = 200;
	shelf_info_.height = 400;
	shelf_info_.dis_drag = 4;
	shelf_info_.numf = 5;
	shelf_info_.dis_wall = 0;
	shelf_info_.shelf_radius = 100;
	shelf_info_.shelf_type = ShelfType::SIMPLE_SHELF;
	shelf_info_.type_arrange = TypeArrange::SEQUENTIAL;
	position_item_ = -1;
}

DlgSettingShelf::~DlgSettingShelf() {

}

void DlgSettingShelf::DoDataExchange(CDataExchange* pDX) {
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_SHELF_LONG,  edit_shelf_long_);
	DDX_Control(pDX, IDC_EDIT_SHELF_WIDTH, edit_shelf_width_);
	DDX_Control(pDX, IDC_EDIT_SHELF_HEIGH, edit_shelf_height_);
	DDX_Control(pDX, IDC_EDIT_NUMBER_FLOOR, edit_number_of_floor_);


	DDX_Control(pDX, IDC_EDIT_SHELF_RADIUS, edit_shelf_radius_);
	DDX_Control(pDX, IDC_EDIT_SHELF_DRAG, edit_shelf_drag_);
	DDX_Control(pDX, IDC_EDIT_DRAG_WALL, edit_drag_wall_);
	DDX_Control(pDX, IDC_STR_NAME_PROJECT, edit_name_shelf_project_);
	DDX_Control(pDX, IDC_RADIO_ARRANGE_0, radio_arrange_0_);
	DDX_Control(pDX, IDC_RADIO_ARRANGE_1, radio_arrange_1_);


	DDX_Control(pDX, IDC_COMBO_SHELF_TYPE, combox_shelf_type_);

  DDX_Control(pDX, IDC_BITMAP_SHELF, bitmap_shelf_);
}


BEGIN_MESSAGE_MAP(DlgSettingShelf, CDialogEx)
  ON_EN_CHANGE(IDC_EDIT_SHELF_LONG, &DlgSettingShelf::OnEditShelfLong)
  ON_EN_CHANGE(IDC_EDIT_SHELF_WIDTH, &DlgSettingShelf::OnEditShelfWidth)
  ON_EN_CHANGE(IDC_EDIT_SHELF_HEIGH, &DlgSettingShelf::OnEditShelfHeight)
  ON_EN_CHANGE(IDC_EDIT_NUMBER_FLOOR, &DlgSettingShelf::OnNumberOfFloor)
	ON_EN_CHANGE(IDC_EDIT_SHELF_RADIUS, &DlgSettingShelf::OnEditShelfRadius)
	ON_CBN_SELENDOK(IDC_COMBO_SHELF_TYPE, &DlgSettingShelf::OnComboxShelfType)
	ON_EN_CHANGE(IDC_EDIT_SHELF_DRAG, &DlgSettingShelf::OnEditShelfDrag)
	ON_EN_CHANGE(IDC_STR_NAME_PROJECT, &DlgSettingShelf::OnEditNameProject)
	ON_EN_CHANGE(IDC_EDIT_DRAG_WALL, &DlgSettingShelf::OnEditDragWall)
	ON_COMMAND(IDC_RADIO_ARRANGE_0, &DlgSettingShelf::OnRadioArrange0)
	ON_COMMAND(IDC_RADIO_ARRANGE_1, &DlgSettingShelf::OnRadioArrange1)
	ON_COMMAND(IDOK, OnOk)

END_MESSAGE_MAP()

BOOL  DlgSettingShelf::OnInitDialog() {
	CDialogEx::OnInitDialog();
	combox_shelf_type_.AddString(_T("Kệ Đơn"));
	combox_shelf_type_.AddString(_T("Kệ Đôi"));
	combox_shelf_type_.AddString(_T("Kệ Tròn"));
	combox_shelf_type_.AddString(_T("Thùng chứa"));
  UpdateInfo();
	SetBitmapShelf(shelf_info_.shelf_type);
	SetStatusInfoShelf(shelf_info_.shelf_type);
	SetStatusCombobox(shelf_info_.shelf_type);
	SetMethodArrange(shelf_info_.type_arrange);
	UpdateData(FALSE);
	return TRUE;
}

void DlgSettingShelf::CreateShelf(CWnd* pParent) {
	CDialog::Create(DlgSettingShelf::IDD, pParent);
	parent_ = reinterpret_cast<FormBar*>(pParent);
}

void DlgSettingShelf::UpdateInfo() {
	CString str;
	str.Format(_T("%.2f"), shelf_info_.longs);
  edit_shelf_long_.SetWindowText(str);
	str.Format(_T("%.2f"), shelf_info_.width);
  edit_shelf_width_.SetWindowText(str);
	str.Format(_T("%.2f"), shelf_info_.height);
  edit_shelf_height_.SetWindowText(str);
	str.Format(_T("%d"), shelf_info_.numf);
  edit_number_of_floor_.SetWindowText(str);
	str.Format(_T("%.2f"), shelf_info_.dis_wall);
	edit_drag_wall_.SetWindowText(str);
	str.Format(_T("%.2f"), shelf_info_.shelf_radius);
	edit_shelf_radius_.SetWindowText(str);
	str.Format(_T("%.2f"), shelf_info_.dis_drag);
	edit_shelf_drag_.SetWindowText(str);
}


void DlgSettingShelf::OnNumberOfFloor() {
  UpdateData(TRUE);
	CString str;
  edit_number_of_floor_.GetWindowText(str);
	shelf_info_.numf = _ttof(str);
}


void DlgSettingShelf::OnEditShelfLong() {
  UpdateData(TRUE);
	CString str;
  edit_shelf_long_.GetWindowText(str);
	shelf_info_.longs = _ttof(str);
}

void DlgSettingShelf::OnEditShelfWidth() {
  UpdateData(TRUE);
	CString str;
  edit_shelf_width_.GetWindowText(str);
	shelf_info_.width = _ttof(str);
}

void DlgSettingShelf::OnEditShelfDrag() {
	UpdateData(TRUE);
	CString str;
  edit_shelf_drag_.GetWindowText(str);
	shelf_info_.dis_drag = _ttof(str);

}

void DlgSettingShelf::OnEditShelfDisWall() {
	UpdateData(TRUE);
	CString str;
	edit_drag_wall_.GetWindowText(str);
	shelf_info_.dis_wall = _ttof(str);
}

void DlgSettingShelf::OnEditShelfHeight() {
  UpdateData(TRUE);
	CString str;
  edit_shelf_height_.GetWindowText(str);
	shelf_info_.height = _ttof(str);
}

void DlgSettingShelf::OnEditShelfRadius() {
	UpdateData(TRUE);
	CString str;
	edit_shelf_radius_.GetWindowText(str);
	shelf_info_.shelf_radius = _ttof(str);

}


void DlgSettingShelf::OnComboxShelfType() {
	UINT item = combox_shelf_type_.GetCurSel();
	if(item == ShelfType::SIMPLE_SHELF) {
	  shelf_info_.shelf_type = ShelfType::SIMPLE_SHELF;
		SetBitmapShelf(ShelfType::SIMPLE_SHELF);
	}else if (item == ShelfType::DOUBLE_SHELF) {
	  shelf_info_.shelf_type = ShelfType::DOUBLE_SHELF;
		SetBitmapShelf(ShelfType::DOUBLE_SHELF);
	}else if (item == ShelfType::CIRCLE_SHELF) {
		SetBitmapShelf(ShelfType::CIRCLE_SHELF);
	  shelf_info_.shelf_type = ShelfType::CIRCLE_SHELF;
	}else if (item == ShelfType::CONTAINER) {
		shelf_info_.shelf_type = ShelfType::CONTAINER;

		//shelf_info_.longs = 420;
		//shelf_info_.width = 200;
		//shelf_info_.height = 400;
		//CString str;
		//str.Format(_T("%.2f"), shelf_info_.longs);
		//edit_shelf_long_.SetWindowText(str);
		//str.Format(_T("%.2f"), shelf_info_.width);
		//edit_shelf_width_.SetWindowText(str);
		//str.Format(_T("%.2f"), shelf_info_.height);
		//edit_shelf_height_.SetWindowText(str);
		//UpdateData(false);
	}
  SetStatusInfoShelf(item);
}


void DlgSettingShelf::SetStatusInfoShelf(UINT shelf_type) {
	switch (shelf_type) {
		case ShelfType::SIMPLE_SHELF: {
			edit_shelf_long_.EnableWindow(TRUE);
			edit_shelf_width_.EnableWindow(TRUE);
			edit_shelf_height_.EnableWindow(TRUE);
			edit_number_of_floor_.EnableWindow(TRUE);
			edit_shelf_drag_.EnableWindow(TRUE);
			edit_drag_wall_.EnableWindow(TRUE);
      edit_shelf_radius_.EnableWindow(FALSE);
			break;
		}
		case ShelfType::DOUBLE_SHELF: {
			edit_shelf_long_.EnableWindow(TRUE);
			edit_shelf_width_.EnableWindow(TRUE);
			edit_shelf_height_.EnableWindow(TRUE);
			edit_number_of_floor_.EnableWindow(TRUE);
			edit_shelf_drag_.EnableWindow(TRUE);
			edit_drag_wall_.EnableWindow(TRUE);
      edit_shelf_radius_.EnableWindow(FALSE);
			break;
		}
		case ShelfType::CIRCLE_SHELF : {
			edit_shelf_long_.EnableWindow(TRUE);
			edit_shelf_width_.EnableWindow(TRUE);
			edit_shelf_height_.EnableWindow(TRUE);
			edit_number_of_floor_.EnableWindow(TRUE);
			edit_shelf_drag_.EnableWindow(TRUE);
			edit_drag_wall_.EnableWindow(TRUE);
      edit_shelf_radius_.EnableWindow(TRUE);
			break;
		}
		case ShelfType::CONTAINER : {
			edit_shelf_long_.EnableWindow(TRUE);
			edit_shelf_width_.EnableWindow(TRUE);
			edit_shelf_height_.EnableWindow(TRUE);
			edit_number_of_floor_.EnableWindow(FALSE);
			edit_shelf_drag_.EnableWindow(FALSE);
			edit_drag_wall_.EnableWindow(TRUE);
      edit_shelf_radius_.EnableWindow(FALSE);
			break;
		}
		default:{
			break;
		}
	}
}

void DlgSettingShelf::SetBitmapShelf(UINT index_bitmap) {
  if (index_bitmap == ShelfType::SIMPLE_SHELF) {
		cbitmap_.Detach();
		cbitmap_.LoadBitmap(IDB_BITMAP_SIMPLE_SHELF);
		bitmap_shelf_.SetBitmap((HBITMAP)cbitmap_);
	} else if (index_bitmap == ShelfType::DOUBLE_SHELF) {
		cbitmap_.Detach();
		cbitmap_.LoadBitmap(IDB_BITMAP_DOUBLE_SHELF);
		bitmap_shelf_.SetBitmap((HBITMAP)cbitmap_);
	} else if (index_bitmap == ShelfType::CIRCLE_SHELF) {
		cbitmap_.Detach();
		cbitmap_.LoadBitmap(IDB_BITMAP_CIRCLE_SHELF);
		bitmap_shelf_.SetBitmap((HBITMAP)cbitmap_);
	} else {
	  return;
	}
}

void DlgSettingShelf::OnEditNameProject() {
	UpdateData(TRUE);
	edit_name_shelf_project_.GetWindowText(shelf_info_.name_project);
}

void DlgSettingShelf::OnEditDragWall() {
  UpdateData(TRUE);
	CString str;
	edit_drag_wall_.GetWindowText(str);
	shelf_info_.dis_wall = _ttof(str);
}

void DlgSettingShelf::OnOk() {
	if(shelf_info_.name_project == "") {
		AfxMessageBox(_T("Xin vui lòng nhập tên"));
	} else {
		parent_->SetShelfInfoList(shelf_info_, position_item_);
		CDialog::OnOK();
	}
}

void DlgSettingShelf::OnRadioArrange0() {
	if(radio_arrange_0_.GetCheck()) {
		shelf_info_.type_arrange = SEQUENTIAL;
	} else {
		shelf_info_.type_arrange = FLOOR;
	}
	SetMethodArrange(shelf_info_.type_arrange);
}

void DlgSettingShelf::OnRadioArrange1() {
	if(radio_arrange_1_.GetCheck()) {
		shelf_info_.type_arrange = FLOOR;
	} else {
		shelf_info_.type_arrange = SEQUENTIAL;
	}
	SetMethodArrange(shelf_info_.type_arrange);
}

void DlgSettingShelf::SetData(ShelfInfo &shelf_info) {
	shelf_info_ = shelf_info;
  UpdateInfo();
	SetBitmapShelf(shelf_info_.shelf_type);
	SetStatusInfoShelf(shelf_info_.shelf_type);
	SetStatusCombobox(shelf_info_.shelf_type);
	SetMethodArrange(shelf_info_.type_arrange);
	UpdateData(FALSE);
}

void DlgSettingShelf::SetStatusCombobox(ShelfType &shelf_type) {
	switch(shelf_type) {
	case ShelfType::SIMPLE_SHELF :
		combox_shelf_type_.SetCurSel(0);
		break;
	case ShelfType::DOUBLE_SHELF :
		combox_shelf_type_.SetCurSel(1);
		break;
	case ShelfType::CIRCLE_SHELF :
		combox_shelf_type_.SetCurSel(2);
		break;
	default :
		combox_shelf_type_.SetCurSel(3);
		break;
	}
}

void DlgSettingShelf::SetMethodArrange(TypeArrange &method) {
	switch(method) {
	case TypeArrange::SEQUENTIAL :
		radio_arrange_0_.SetCheck(1);
		radio_arrange_1_.SetCheck(0);
		break;
	case TypeArrange::FLOOR :
		radio_arrange_0_.SetCheck(0);
		radio_arrange_1_.SetCheck(1);
		break;
	}
}
