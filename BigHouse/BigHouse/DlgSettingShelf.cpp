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
	str_shelf_start_angle_ = L"0";
	str_shelf_end_angle_  = L"360";
	str_shelf_flat_angle_ = L"50";



	shelf_info_.longs = 200;
	shelf_info_.width = 200;
	shelf_info_.height = 400;
	shelf_info_.numf = 5;
	shelf_info_.shelf_radius = 100;
	shelf_info_.shelf_start_angle_ = 0.0;
	shelf_info_.shelf_end_angle_ = 360.0;
	shelf_info_.shelf_flat_angle_ = 50.0;

	shelf_info_.shelf_type = ShelfType::SIMPLE_SHELF;
	shelf_info_.shelf_shape = shelf_shape_ = ShelfShape::FRONT_DRIECTION;
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
	DDX_Control(pDX, IDC_EDIT_SHELF_START_ANGLE, edit_shelf_start_angle_);
	DDX_Control(pDX, IDC_EDIT_SHELF_END_ANGLE, edit_shelf_end_angle_);
	DDX_Control(pDX, IDC_EDIT_SHELF_FLAT_ANGLE, edit_shelf_flat_angle_);


	DDX_Control(pDX, IDC_COMBO_SHELF_TYPE, combox_shelf_type_);
	DDX_Control(pDX, IDC_RADIO_FRONT, shelf_front_);
	DDX_Control(pDX, IDC_RADIO_BACK, shelf_back_);
	DDX_Control(pDX, IDC_RADIO_LEFT, shelf_left_);
	DDX_Control(pDX, IDC_RADIO_RIGHT, shelf_right_);

  DDX_Control(pDX, IDC_BITMAP_SHELF, bitmap_shelf_);
}


BEGIN_MESSAGE_MAP(DlgSettingShelf, CDialogEx)
  ON_EN_CHANGE(IDC_EDIT_SHELF_LONG, &DlgSettingShelf::OnEditShelfLong)
  ON_EN_CHANGE(IDC_EDIT_SHELF_WIDTH, &DlgSettingShelf::OnEditShelfWidth)
  ON_EN_CHANGE(IDC_EDIT_SHELF_HEIGH, &DlgSettingShelf::OnEditShelfHeight)
  ON_EN_CHANGE(IDC_EDIT_NUMBER_FLOOR, &DlgSettingShelf::OnNumberOfFloor)
	ON_EN_CHANGE(IDC_EDIT_SHELF_RADIUS, &DlgSettingShelf::OnEditShelfRadius)
	ON_EN_CHANGE(IDC_EDIT_SHELF_START_ANGLE, &DlgSettingShelf::OnEditShelfStartAngle)
	ON_EN_CHANGE(IDC_EDIT_SHELF_END_ANGLE, &DlgSettingShelf::OnEditShelfEndAngle)
	ON_EN_CHANGE(IDC_EDIT_SHELF_FLAT_ANGLE, &DlgSettingShelf::OnEditShelfFlatAngle)
	ON_CBN_SELENDOK(IDC_COMBO_SHELF_TYPE, &DlgSettingShelf::OnComboxShelfType)
	ON_BN_CLICKED(IDC_RADIO_FRONT, &DlgSettingShelf::OnBnClickedRadioFront)
	ON_BN_CLICKED(IDC_RADIO_BACK, &DlgSettingShelf::OnBnClickedRadioBack)
	ON_BN_CLICKED(IDC_RADIO_LEFT, &DlgSettingShelf::OnBnClickedRadioLeft)
	ON_BN_CLICKED(IDC_RADIO_RIGHT, &DlgSettingShelf::OnBnClickedRadioRight)
END_MESSAGE_MAP()


// DlgSettingShelf message handlers

BOOL  DlgSettingShelf::OnInitDialog() {
	CDialogEx::OnInitDialog();

  edit_shelf_long_.SetWindowText(str_shelf_long_);
  edit_shelf_width_.SetWindowText(str_shelf_width_);
  edit_shelf_height_.SetWindowText(str_shelf_height_);
  edit_number_of_floor_.SetWindowText(str_number_of_floor_);

	edit_shelf_radius_.SetWindowText(str_shelf_radius_);
	edit_shelf_start_angle_.SetWindowText(str_shelf_start_angle_);
	edit_shelf_end_angle_.SetWindowText(str_shelf_end_angle_);
	edit_shelf_flat_angle_.SetWindowText(str_shelf_flat_angle_);

	combox_shelf_type_.AddString(_T("Kệ Đơn"));
	combox_shelf_type_.AddString(_T("Kệ Đôi"));
	combox_shelf_type_.AddString(_T("Kệ Tròn"));
	combox_shelf_type_.SetCurSel(0);
	
	//Update Bitmap 
	SetBitmapShelf(ShelfType::SIMPLE_SHELF);

	SetStatusInfoShelf(ShelfType::SIMPLE_SHELF);
	SetStatusShelfShape(ShelfType::SIMPLE_SHELF);

	shelf_front_.SetCheck(1);
  
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

void DlgSettingShelf::OnEditShelfHeight() {
  UpdateData(TRUE);
  edit_shelf_height_.GetWindowText(str_shelf_height_);
	shelf_info_.height = _ttof(str_shelf_height_);
}

void DlgSettingShelf::OnEditShelfRadius()
{
	UpdateData(TRUE);
	edit_shelf_radius_.GetWindowText(str_shelf_radius_);
	shelf_info_.shelf_radius = _ttof(str_shelf_radius_);

}

void DlgSettingShelf::OnEditShelfStartAngle()
{
	UpdateData(TRUE);
	edit_shelf_start_angle_.GetWindowText(str_shelf_start_angle_);
	shelf_info_.shelf_start_angle_= _ttof(str_shelf_start_angle_);

}

void DlgSettingShelf::OnEditShelfEndAngle()
{
	UpdateData(TRUE);
	edit_shelf_end_angle_.GetWindowText(str_shelf_end_angle_);
	shelf_info_.shelf_end_angle_ = _ttof(str_shelf_end_angle_);

}

void DlgSettingShelf::OnEditShelfFlatAngle()
{
	UpdateData(TRUE);
	edit_shelf_flat_angle_.GetWindowText(str_shelf_flat_angle_);
	shelf_info_.shelf_flat_angle_ = _ttof(str_shelf_flat_angle_);

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
	} else {
	  ;
	}
  SetStatusInfoShelf(item);
	SetStatusShelfShape(item);
}


void DlgSettingShelf::SetStatusInfoShelf(UINT shelf_type)
{
	switch (shelf_type) {
	case ShelfType::SIMPLE_SHELF: {
		edit_shelf_radius_.EnableWindow(FALSE);
	  edit_shelf_start_angle_.EnableWindow(FALSE);
		edit_shelf_end_angle_.EnableWindow(FALSE);
		edit_shelf_flat_angle_.EnableWindow(FALSE);
		edit_shelf_long_.EnableWindow(TRUE);
		edit_shelf_width_.EnableWindow(TRUE);
		break;
	}
	case ShelfType::DOUBLE_SHELF: {
		edit_shelf_radius_.EnableWindow(FALSE);
		edit_shelf_start_angle_.EnableWindow(FALSE);
		edit_shelf_end_angle_.EnableWindow(FALSE);
		edit_shelf_flat_angle_.EnableWindow(FALSE);
		edit_shelf_long_.EnableWindow(TRUE);
		edit_shelf_width_.EnableWindow(TRUE);
		break;
	}
		case ShelfType::CIRCLE_SHELF : {
			edit_shelf_radius_.EnableWindow(TRUE);
			edit_shelf_start_angle_.EnableWindow(TRUE);
			edit_shelf_end_angle_.EnableWindow(TRUE);
			edit_shelf_flat_angle_.EnableWindow(TRUE);
			edit_shelf_long_.EnableWindow(FALSE);
			edit_shelf_width_.EnableWindow(FALSE);
			break;
		}
		default:{
			break;
		}
	}
}


void DlgSettingShelf::OnBnClickedRadioFront()
{
	shelf_shape_ = ShelfShape::FRONT_DRIECTION;
	shelf_info_.shelf_shape = shelf_shape_;
	shelf_back_.SetCheck(0);
	shelf_left_.SetCheck(0);
	shelf_right_.SetCheck(0);
}


void DlgSettingShelf::OnBnClickedRadioBack()
{
	shelf_shape_ = ShelfShape::BACK_DRIECTION;
	shelf_info_.shelf_shape = shelf_shape_;
	shelf_front_.SetCheck(0);
	shelf_left_.SetCheck(0);
	shelf_right_.SetCheck(0);
}


void DlgSettingShelf::OnBnClickedRadioLeft()
{
	shelf_shape_ = ShelfShape::LEFT_DIRECTION;
	shelf_info_.shelf_shape = shelf_shape_;
	shelf_back_.SetCheck(0);
	shelf_front_.SetCheck(0);
	shelf_right_.SetCheck(0);
}


void DlgSettingShelf::OnBnClickedRadioRight() {
  shelf_shape_ = ShelfShape::RIGHT_DIRECTION;
	shelf_info_.shelf_shape = shelf_shape_;
	shelf_back_.SetCheck(0);
	shelf_left_.SetCheck(0);
	shelf_front_.SetCheck(0);
}

void DlgSettingShelf::SetStatusShelfShape( UINT shelf_type )
{
	switch (shelf_type) {
	case ShelfType::SIMPLE_SHELF: {
		shelf_back_.EnableWindow(TRUE);
		shelf_right_.EnableWindow(TRUE);
		break;
	}
	case ShelfType::DOUBLE_SHELF: {
		shelf_back_.EnableWindow(FALSE);
		shelf_right_.EnableWindow(FALSE);
		break;
																}
	case ShelfType::CIRCLE_SHELF : {
		shelf_front_.EnableWindow(FALSE);
		shelf_back_.EnableWindow(FALSE);
		shelf_left_.EnableWindow(FALSE);
		shelf_right_.EnableWindow(FALSE);
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
