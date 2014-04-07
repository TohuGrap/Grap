// DlgSettingShelf.cpp : implementation file
//

#include "stdafx.h"
#include "BigHouse.h"
#include "DlgSettingShelf.h"
#include "afxdialogex.h"


// DlgSettingShelf dialog

IMPLEMENT_DYNAMIC(DlgSettingShelf, CDialogEx)

DlgSettingShelf::DlgSettingShelf(CWnd* pParent /*=NULL*/)
	: CDialogEx(DlgSettingShelf::IDD, pParent)
{
  str_shelf_long_ = L"200";
  str_shelf_width_ = L"200";
  str_shelf_height_ = L"400";
  str_number_of_floor_ = L"5";

	str_shelf_radius_ = L"100";
	str_shelf_drag_ = L"4";
  str_drag_wall_ = L"5";



	shelf_info_.longs = 200;
	shelf_info_.width = 200;
	shelf_info_.height = 400;
	shelf_info_.dis_drag = 4;
	shelf_info_.numf = 5;
	shelf_info_.dis_wall = 5;
	shelf_info_.shelf_radius = 100;
	shelf_info_.shelf_type = ShelfType::SIMPLE_SHELF;

}

DlgSettingShelf::~DlgSettingShelf()
{

}

void DlgSettingShelf::DoDataExchange(CDataExchange* pDX)
{
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


// DlgSettingShelf message handlers

BOOL  DlgSettingShelf::OnInitDialog() {
	CDialogEx::OnInitDialog();

  edit_shelf_long_.SetWindowText(str_shelf_long_);
  edit_shelf_width_.SetWindowText(str_shelf_width_);
  edit_shelf_height_.SetWindowText(str_shelf_height_);
  edit_number_of_floor_.SetWindowText(str_number_of_floor_);
	edit_drag_wall_.SetWindowText(str_drag_wall_);
	edit_shelf_radius_.SetWindowText(str_shelf_radius_);
	edit_shelf_drag_.SetWindowText(str_shelf_drag_);
	radio_arrange_0_.SetCheck(1);
	radio_arrange_1_.SetCheck(0);
	shelf_info_.type_arrange = SEQUENTIAL;
	combox_shelf_type_.AddString(_T("Kệ Đơn"));
	combox_shelf_type_.AddString(_T("Kệ Đôi"));
	combox_shelf_type_.AddString(_T("Kệ Tròn"));
	combox_shelf_type_.AddString(_T("Thùng chứa"));

	combox_shelf_type_.SetCurSel(0);
	
	//Update Bitmap 
	SetBitmapShelf(ShelfType::SIMPLE_SHELF);

	SetStatusInfoShelf(ShelfType::SIMPLE_SHELF);

	UpdateData(FALSE);
	return TRUE;
}
void DlgSettingShelf::OnNumberOfFloor() {
  UpdateData(TRUE);
  edit_number_of_floor_.GetWindowText(str_number_of_floor_);
	shelf_info_.numf = _ttof(str_number_of_floor_);
}


void DlgSettingShelf::OnEditShelfLong() {
  UpdateData(TRUE);
  edit_shelf_long_.GetWindowText(str_shelf_long_);
	shelf_info_.longs = _ttof(str_shelf_long_);
}

void DlgSettingShelf::OnEditShelfWidth() {
  UpdateData(TRUE);
  edit_shelf_width_.GetWindowText(str_shelf_width_);
	shelf_info_.width = _ttof(str_shelf_width_);
}

void DlgSettingShelf::OnEditShelfDrag() {
	UpdateData(TRUE);
  edit_shelf_drag_.GetWindowText(str_shelf_drag_);
	shelf_info_.dis_drag = _ttof(str_shelf_drag_);

}

void DlgSettingShelf::OnEditShelfDisWall() {
	UpdateData(TRUE);
	edit_drag_wall_.GetWindowText(str_drag_wall_);
	shelf_info_.dis_wall = _ttof(str_drag_wall_);
}

void DlgSettingShelf::OnEditShelfHeight() {
  UpdateData(TRUE);
  edit_shelf_height_.GetWindowText(str_shelf_height_);
	shelf_info_.height = _ttof(str_shelf_height_);
}

void DlgSettingShelf::OnEditShelfRadius() {
	UpdateData(TRUE);
	edit_shelf_radius_.GetWindowText(str_shelf_radius_);
	shelf_info_.shelf_radius = _ttof(str_shelf_radius_);

}


void DlgSettingShelf::OnComboxShelfType() {
	UINT item = combox_shelf_type_.GetCurSel();
	if (item == ShelfType::SIMPLE_SHELF) {
	  shelf_info_.shelf_type = ShelfType::SIMPLE_SHELF;
		SetBitmapShelf(ShelfType::SIMPLE_SHELF);
	} else if (item == ShelfType::DOUBLE_SHELF) {
	  shelf_info_.shelf_type = ShelfType::DOUBLE_SHELF;
		SetBitmapShelf(ShelfType::DOUBLE_SHELF);
	} else if (item == ShelfType::CIRCLE_SHELF) {
		SetBitmapShelf(ShelfType::CIRCLE_SHELF);
	  shelf_info_.shelf_type = ShelfType::CIRCLE_SHELF;
	} else if (item == ShelfType::CONTAINER) {
		shelf_info_.shelf_type = ShelfType::CONTAINER;
		str_shelf_long_ = L"420";
		str_shelf_width_ = L"200";
		str_shelf_height_ = L"200";
		shelf_info_.longs = 420;
		shelf_info_.width = 200;
		shelf_info_.height = 400;
		edit_shelf_long_.SetWindowText(str_shelf_long_);
		edit_shelf_width_.SetWindowText(str_shelf_width_);
		edit_shelf_height_.SetWindowText(str_shelf_height_);
		UpdateData(false);
	}
  SetStatusInfoShelf(item);
}


void DlgSettingShelf::SetStatusInfoShelf(UINT shelf_type)
{
	switch (shelf_type) {
	case ShelfType::SIMPLE_SHELF: {
		edit_shelf_radius_.EnableWindow(FALSE);
		edit_shelf_long_.EnableWindow(TRUE);
		edit_shelf_width_.EnableWindow(TRUE);
		break;
	}
	case ShelfType::DOUBLE_SHELF: {
		edit_shelf_radius_.EnableWindow(FALSE);
		edit_shelf_long_.EnableWindow(TRUE);
		edit_shelf_width_.EnableWindow(TRUE);
		break;
	}
		case ShelfType::CIRCLE_SHELF : {
			edit_shelf_radius_.EnableWindow(TRUE);
			edit_shelf_long_.EnableWindow(FALSE);
			edit_shelf_width_.EnableWindow(FALSE);
			break;
		}
		case ShelfType::CONTAINER : {
			edit_shelf_radius_.EnableWindow(FALSE);
			edit_shelf_drag_.EnableWindow(FALSE);
			//edit_number_of_shelf_.EnableWindow(FALSE);
			edit_number_of_floor_.EnableWindow(FALSE);
			edit_shelf_long_.EnableWindow(TRUE);
			edit_shelf_width_.EnableWindow(TRUE);
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
	edit_drag_wall_.GetWindowText(str_drag_wall_);
	shelf_info_.dis_wall = _ttof(str_drag_wall_);
}

void DlgSettingShelf::OnOk() {
	if(shelf_info_.name_project == "") {
		AfxMessageBox(_T("Xin vui lòng nhập tên"));
	} else {
		CDialog::OnOK();
	}
}

void DlgSettingShelf::OnRadioArrange0() {
	if(radio_arrange_0_.GetCheck()) {
		shelf_info_.type_arrange = SEQUENTIAL;
	} else {
		shelf_info_.type_arrange = FLOOR;
	}
}

void DlgSettingShelf::OnRadioArrange1() {
	if(radio_arrange_1_.GetCheck()) {
		shelf_info_.type_arrange = SEQUENTIAL;
	} else {
		shelf_info_.type_arrange = FLOOR;
	}
}

