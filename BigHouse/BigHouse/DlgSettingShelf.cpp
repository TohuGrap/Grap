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
	str_number_of_shelf_ = L"1";
  str_shelf_long_ = L"200";
  str_shelf_width_ = L"200";
  str_shelf_height_ = L"400";
  str_number_of_floor_ = L"5";
  str_shelf_angle_ = L"0";

	shelf_info_.longs = 200;
	shelf_info_.width = 200;
	shelf_info_.height = 400;
	shelf_info_.numf = 5;
	shelf_info_.nums = 1;
	shelf_info_.shelf_angle = 0;
}

DlgSettingShelf::~DlgSettingShelf()
{

}

void DlgSettingShelf::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NUMBER_SHELF, edit_number_of_shelf_);
	DDX_Text(pDX, IDC_EDIT_SHELF_LONG,  edit_shelf_long_);
	DDX_Text(pDX, IDC_EDIT_SHELF_WIDTH, edit_shelf_width_);
	DDX_Text(pDX, IDC_EDIT_SHELF_HEIGH, edit_shelf_height_);
	DDX_Text(pDX, IDC_EDIT_NUMBER_FLOOR, edit_number_of_floor_);
	DDX_Text(pDX, IDC_EDIT_SHELF_ANGLE, edit_shelf_angle_);

}


BEGIN_MESSAGE_MAP(DlgSettingShelf, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT_NUMBER_SHELF, &DlgSettingShelf::OnNumberOfShelf)
  ON_EN_CHANGE(IDC_EDIT_SHELF_LONG, &DlgSettingShelf::OnEditShelfLong)
  ON_EN_CHANGE(IDC_EDIT_SHELF_WIDTH, &DlgSettingShelf::OnEditShelfWidth)
  ON_EN_CHANGE(IDC_EDIT_SHELF_HEIGH, &DlgSettingShelf::OnEditShelfHeight)
  ON_EN_CHANGE(IDC_EDIT_NUMBER_FLOOR, &DlgSettingShelf::OnNumberOfFloor)
  ON_EN_CHANGE(IDC_EDIT_SHELF_ANGLE, &DlgSettingShelf::OnEditShelfAngle)
END_MESSAGE_MAP()


// DlgSettingShelf message handlers

BOOL  DlgSettingShelf::OnInitDialog() {
	CDialogEx::OnInitDialog();
  
	edit_number_of_shelf_ = str_number_of_shelf_;
  edit_shelf_long_ = str_shelf_long_;
  edit_shelf_width_ = str_shelf_width_;
  edit_shelf_height_ = str_shelf_height_;
  edit_number_of_floor_ = str_number_of_floor_;
  edit_shelf_angle_ = str_shelf_angle_;


	UpdateData(FALSE);
	return TRUE;
}
void DlgSettingShelf::OnNumberOfFloor() {
  UpdateData(TRUE);
  str_number_of_floor_ = edit_number_of_floor_;
	shelf_info_.numf = _ttof(str_number_of_floor_);
}

void DlgSettingShelf::OnNumberOfShelf() {
  UpdateData(TRUE);
  str_number_of_shelf_ = edit_number_of_shelf_;
	shelf_info_.nums = _ttof(str_number_of_shelf_);
}

void DlgSettingShelf::OnEditShelfLong() {
  UpdateData(TRUE);
  str_shelf_long_ = edit_shelf_long_;
	shelf_info_.longs = _ttof(str_shelf_long_);
}

void DlgSettingShelf::OnEditShelfWidth() {
  UpdateData(TRUE);
  str_shelf_width_ = edit_shelf_width_;
	shelf_info_.width = _ttof(str_shelf_width_);
}

void DlgSettingShelf::OnEditShelfHeight() {
  UpdateData(TRUE);
  str_shelf_height_ = edit_shelf_height_;
	shelf_info_.height = _ttof(str_shelf_height_);
}

void DlgSettingShelf::OnEditShelfAngle() {
  UpdateData(TRUE);
  str_shelf_angle_ = edit_shelf_angle_;
	shelf_info_.shelf_angle = _ttof(str_shelf_angle_);
}