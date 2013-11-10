#pragma once
#include "resource.h"
#include "struct.h"
// SetupRoomDlg dialog

class SetupRoomDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SetupRoomDlg)

public:
	SetupRoomDlg(CWnd* pParent = NULL);   // standard constructor
  SetupRoomDlg(float long_room, float width_room, float height_room, CWnd* pParent = NULL);
	virtual ~SetupRoomDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_SETUP_ROOM };

  SizeRoom GetSizeRoom() {return size_room_;}
protected:
  CString str_long_room_;
  CString str_width_room_;
  CString str_height_room_;

  SizeRoom size_room_;
protected:
  virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  virtual void OnOK();
	DECLARE_MESSAGE_MAP()
};
