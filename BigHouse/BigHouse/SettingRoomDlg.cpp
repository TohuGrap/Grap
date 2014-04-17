// SettingRoomDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BigHouse.h"
#include "SettingRoomDlg.h"
#include "afxdialogex.h"


// SettingRoomDlg dialog

IMPLEMENT_DYNAMIC(SettingRoomDlg, CDialogEx)

SettingRoomDlg::SettingRoomDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(SettingRoomDlg::IDD, pParent)
{

}

SettingRoomDlg::SettingRoomDlg(RoomSize room_size, bool is_show_size, CWnd*pParent) 
	: CDialogEx(SettingRoomDlg::IDD, pParent)
{
	room_size_.longs = room_size.longs;
	room_size_.width = room_size.width;
	room_size_.height = room_size.height;
	room_size_.depth = room_size.depth;

	str_long_.Format(_T("%.2f"), room_size.longs);
	str_width_.Format(_T("%.2f"), room_size.width);
	str_height_.Format(_T("%.2f"), room_size_.height);
	str_depth_.Format(_T("%.2f"), room_size_.depth);

	show_room_size_ = is_show_size ? 1: 0;
	is_show_room_size_ = is_show_size;
	is_arrange_in_room_ = true;
}

SettingRoomDlg::~SettingRoomDlg()
{
}

void SettingRoomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_LONG, edit_room_long_);
	DDX_Control(pDX, IDC_EDIT_WIDTH, edit_room_width_);
	DDX_Control(pDX, IDC_EDIT_HEIGHT, edit_room_height_);
	DDX_Control(pDX, IDC_EDIT_DEPTH, edit_room_depth_);
	DDX_Check(pDX, IDC_CHECK_SHOW_SIZE, show_room_size_);
	DDX_Control(pDX, IDC_RADIO_ARRANGE_IN_ROOM, show_room_);
	DDX_Control(pDX, IDC_RADIO_ARRANGE_IN_CONTAINER  , show_container_);

}


BEGIN_MESSAGE_MAP(SettingRoomDlg, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT_LONG, OnEditRoomLong)
	ON_EN_CHANGE(IDC_EDIT_WIDTH, OnEditRoomWidth)
	ON_EN_CHANGE(IDC_EDIT_HEIGHT, OnEditRoomHeight)
	ON_EN_CHANGE(IDC_EDIT_DEPTH, OnEditRoomDepth)
	ON_BN_CLICKED(IDC_CHECK_SHOW_SIZE, ShowRoomSize)
	ON_COMMAND(IDC_RADIO_ARRANGE_IN_ROOM, &ArrangeInRoom)
	ON_COMMAND(IDC_RADIO_ARRANGE_IN_CONTAINER, &ArrangeInContainer)
END_MESSAGE_MAP()

// SettingRoomDlg message handlers

BOOL SettingRoomDlg::OnInitDialog() {
	CDialogEx::OnInitDialog();

	edit_room_long_.SetWindowText(str_long_);
	edit_room_width_.SetWindowText(str_width_);
	edit_room_height_.SetWindowText(str_height_);
	edit_room_depth_.SetWindowText(str_depth_);
	
	UpdateData(FALSE);
	return TRUE;
}


void SettingRoomDlg::OnEditRoomLong() {
	UpdateData(TRUE);
	edit_room_long_.GetWindowText(str_long_);
	room_size_.longs = _tstof(str_long_);
}


void SettingRoomDlg::OnEditRoomWidth() {
  UpdateData(TRUE);
	edit_room_width_.GetWindowText(str_width_);
	room_size_.width = _tstof(str_width_);
}


void SettingRoomDlg::OnEditRoomHeight() {
  UpdateData(TRUE);
	edit_room_height_.GetWindowText(str_height_);
	room_size_.height = _tstof(str_height_);
}

void SettingRoomDlg::OnEditRoomDepth() {
  UpdateData(TRUE);
	edit_room_depth_.GetWindowText(str_depth_);
	room_size_.depth = _tstof(str_depth_);
}

void SettingRoomDlg::ShowRoomSize()
{
	UpdateData(TRUE);
	if (show_room_size_ == 1) {
		is_show_room_size_ = true;
	} else {
    is_show_room_size_ = false;
	}
}

void SettingRoomDlg::ArrangeInRoom() {
	edit_room_depth_.EnableWindow(true);
	edit_room_long_.EnableWindow(true);
	edit_room_width_.EnableWindow(true);
	edit_room_height_.EnableWindow(true);
	is_arrange_in_room_ =  true;
	//show_room_size_.EnableWindow(true);
}


void SettingRoomDlg::ArrangeInContainer() {
	edit_room_depth_.EnableWindow(false);
	edit_room_long_.EnableWindow(false);
	edit_room_width_.EnableWindow(false);
	edit_room_height_.EnableWindow(false);
	is_show_room_size_ = false;
	is_arrange_in_room_ = false;
	//show_room_size_.EnableWindow(true);
}
