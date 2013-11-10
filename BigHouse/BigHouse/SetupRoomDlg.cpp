// SetupRoomDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BigHouse.h"
#include "SetupRoomDlg.h"
#include "afxdialogex.h"


// SetupRoomDlg dialog

IMPLEMENT_DYNAMIC(SetupRoomDlg, CDialogEx)

SetupRoomDlg::SetupRoomDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(SetupRoomDlg::IDD, pParent)
  , str_long_room_(_T(""))
  , str_width_room_(_T(""))
  , str_height_room_(_T(""))
{
  size_room_.long_room = 0.0f;
  size_room_.width_room = 0.0f;
  size_room_.height_room = 0.0f;
}

SetupRoomDlg::SetupRoomDlg(float long_room, float width_room, float height_room, CWnd* pParent) 
  : CDialogEx(SetupRoomDlg::IDD, pParent)
{
  size_room_.long_room = long_room;
  size_room_.width_room = width_room;
  size_room_.height_room = height_room;
}

SetupRoomDlg::~SetupRoomDlg()
{
}



void SetupRoomDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialogEx::DoDataExchange(pDX);
  DDX_Text(pDX, IDC_EDIT_LONG_ROOM, str_long_room_);
  DDX_Text(pDX, IDC_EDIT_WIDTH_ROOM, str_width_room_);
  DDX_Text(pDX, IDC_EDIT_HEIGHT_ZOOM, str_height_room_);
}


BEGIN_MESSAGE_MAP(SetupRoomDlg, CDialogEx)
END_MESSAGE_MAP()

BOOL SetupRoomDlg::OnInitDialog() {
  CDialogEx::OnInitDialog();

  CString str_long;
  CString str_width;
  CString str_height;

  str_long.Format(_T("%f"), size_room_.long_room);
  str_long_room_ = str_long;

  str_width.Format(_T("%f"), size_room_.width_room);
  str_width_room_ = str_width;

  str_height.Format(_T("%f"), size_room_.height_room);
  str_height_room_ = str_height;

  UpdateData(FALSE);
  return TRUE;
}

void SetupRoomDlg::OnOK() {
  UpdateData(TRUE);

  CString str_long = str_long_room_;
  CString str_width = str_width_room_;
  CString str_height = str_height_room_;

  size_room_.long_room = _tstof(str_long);
  size_room_.width_room = _tstof(str_width);
  size_room_.height_room = _tstof(str_height);

  CDialogEx::OnOK();
}
// SetupRoomDlg message handlers
