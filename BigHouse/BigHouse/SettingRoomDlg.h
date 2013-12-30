#pragma once
#include "resource.h"
#include "afxwin.h"
#include "Struct.h"


// SettingRoomDlg dialog

class SettingRoomDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SettingRoomDlg)

public:
	SettingRoomDlg(CWnd* pParent = NULL);   // standard constructor
	SettingRoomDlg(RoomSize room_size, bool is_show_size, CWnd*pPatent = NULL);
	virtual ~SettingRoomDlg();

// Dialog Data
	enum { IDD = IDD_DLG_SETTING_ROOM };

public:
	RoomSize GetRoomSize() {return room_size_;}
	virtual BOOL OnInitDialog();
	bool IsShowRoomSize() {return is_show_room_size_;}
protected:
	afx_msg void OnEditRoomLong();
	afx_msg void OnEditRoomWidth();
	afx_msg void OnEditRoomHeight();
	afx_msg void OnEditRoomDepth();
	afx_msg void ShowRoomSize();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
protected:
	RoomSize room_size_;

	CString str_long_;
	CString str_width_;
	CString str_height_;
	CString str_depth_;

  CEdit edit_room_long_;
	CEdit edit_room_width_;
	CEdit edit_room_height_;
	CEdit edit_room_depth_;
	int show_room_size_;
	bool is_show_room_size_;
};
