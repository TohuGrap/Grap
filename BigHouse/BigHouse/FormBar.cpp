// FormBar.cpp : implementation file
//

#include "stdafx.h"
#include "BigHouse.h"
#include "FormBar.h"
#include "resource.h"
#include "BigHouseView.h"
#include "MainFrm.h"
#include "BigHouse.h"
#include "base.h"
#include "SettingRoomDlg.h"
#include "Struct.h"
#include "DlgSettingShelf.h"
#include "DlgProduction.h"
// FormBar

IMPLEMENT_DYNCREATE(FormBar, CFormView)

FormBar::FormBar()
	: CFormView(FormBar::IDD)
	, show_size_(0)
{
  object_index_ = -1;
}

FormBar::~FormBar()
{
}

void FormBar::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_SHOW_SIZE, show_size_);
}

BEGIN_MESSAGE_MAP(FormBar, CFormView)
  ON_COMMAND(IDC_OBJECT_OPTION, FormBar::OnOption)
  ON_COMMAND(IDC_OBJECT_OPTION_SHELF, FormBar::OnSelectShelf)
  ON_COMMAND(IDC_CLEAR_SHELF,  FormBar::ClearShelf)
  ON_COMMAND(IDC_BTN_ROOM_SIZE, FormBar::OnSettingRoom)
	ON_BN_CLICKED(IDC_CHECK_SHOW_SIZE, &FormBar::ShowSizeRoom)
  ON_WM_PAINT()
  ON_WM_SIZE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// FormBar diagnostics

#ifdef _DEBUG
void FormBar::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void FormBar::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// FormBar message handlers


void FormBar::OnInitialUpdate() {
  CFormView::OnInitialUpdate();
  return ;
}

void FormBar::OnPaint() {
  CFormView::OnPaint();
}

void FormBar::OnSize(UINT nType, int cx, int cy) {
  EnableScrollBarCtrl(SB_VERT, FALSE);
  CFormView::OnSize(nType, cx, cy);
}
// set to view
BigHouseView *FormBar::GetBigHouseView() {
  BigHouseApp *pApp= (BigHouseApp*)AfxGetApp();
  MainFrame *pMainFrame = (MainFrame*)pApp->m_pMainWnd;
  BigHouseView*pView = reinterpret_cast<BigHouseView*>(pMainFrame->splitter_.GetPane(0, 1));
  return pView;
}


void FormBar::OnOption() {
	DlgProduction dlg;
	if (dlg.DoModal() == IDOK) {
		CString str_production = dlg.GetCurrentStrProduction();
    theApp.LoadFileCad(str_production);
	}
}

void FormBar::OnSelectShelf() {

	DlgSettingShelf dlg;
	ShelfInfo shelf_info;
	if (dlg.DoModal() == IDOK) {
		shelf_info = dlg.GetShelfInfo();
		GetBigHouseView()->MakeShelf((int)shelf_info.width, (int)shelf_info.longs, (int)shelf_info.height,
			                           (int)shelf_info.numf, (int)shelf_info.nums, shelf_info.shelf_angle);
	}
}

void FormBar::ClearShelf() {
  GetBigHouseView()->ClearShelf();
}


void FormBar::OnSettingRoom() {
  
	RoomSize room_size_;
	room_size_ = GetBigHouseView()->GetRoomSize();
	SettingRoomDlg dlg(room_size_);
	if (IDOK == dlg.DoModal()) {
		room_size_ = dlg.GetRoomSize();
	  GetBigHouseView()->SetRoomSize(room_size_);
	}
}

void FormBar::ShowSizeRoom() {
  UpdateData(TRUE);
	bool is_show_size = false;
	if (show_size_ == 1) {
	 is_show_size = true;
	} else {
	  is_show_size = false;
	}
	
	GetBigHouseView()->SetShowSize(is_show_size);
}

void FormBar::OnLButtonUp(UINT nFlags, CPoint pt) {
	GetBigHouseView()->SetIsLbuttonDown(false);
	CFormView::OnLButtonDown(nFlags, pt);
}