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
	shelf_info_.longs = 200;
	shelf_info_.width = 200;
	shelf_info_.height = 400;
	shelf_info_.numf = 5;
	shelf_info_.nums = 1;
	shelf_info_.shelf_angle = 0;

	 str_production_ = _T("");
}

FormBar::~FormBar()
{
}

void FormBar::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_SHOW_SIZE, show_size_);
	DDX_Control(pDX, IDC_BKGN_COLOR, background_color_);
}

BEGIN_MESSAGE_MAP(FormBar, CFormView)
  ON_COMMAND(IDC_OBJECT_OPTION, FormBar::OnGetProduction)
  ON_COMMAND(IDC_OBJECT_OPTION_SHELF, FormBar::OnSettingShelf)
  ON_COMMAND(IDC_CLEAR_SHELF,  FormBar::ClearShelf)
  ON_COMMAND(IDC_BTN_ROOM_SIZE, FormBar::OnSettingRoom)
	ON_BN_CLICKED(IDC_CHECK_SHOW_SIZE, &FormBar::ShowSizeRoom)
	ON_BN_CLICKED(IDC_BTN_SHELF_SELECTED, &FormBar::OnBnShelfSelected)
	ON_BN_CLICKED(IDC_BTN_PRODUCTION_SELECTED, &FormBar::OnBnProductionSelected)
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

	CButton* production_btn = reinterpret_cast<CButton*>(GetDlgItem(IDC_BTN_PRODUCTION_SELECTED));
	production_btn->EnableWindow(FALSE);
  return ;
}

void FormBar::OnPaint() {
 // CFormView::OnPaint();

	CPaintDC dc(this);
	BigHouseApp *pApp = (BigHouseApp*)AfxGetApp();

	CRect rect;
	background_color_.GetWindowRect(&rect);
	ScreenToClient(&rect);
	CBrush BrushBack(pApp->option_color_glback);
	dc.FillRect(&rect, &BrushBack);
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


void FormBar::OnGetProduction() {
	DlgProduction dlg;
	if (dlg.DoModal() == IDOK) {
		str_production_ = dlg.GetCurrentStrProduction();
		CButton* production_btn= reinterpret_cast<CButton*>(GetDlgItem(IDC_BTN_PRODUCTION_SELECTED));
		production_btn->EnableWindow(TRUE);
	}
}


void FormBar::OnBnProductionSelected()
{
	theApp.LoadFileCad(str_production_);
}

void FormBar::OnSettingShelf() {
	DlgSettingShelf dlg;
	if (dlg.DoModal() == IDOK) {
		shelf_info_ = dlg.GetShelfInfo();
	}
}

void FormBar::OnBnShelfSelected()
{
	UINT shelf_type = shelf_info_.shelf_type;

	if (shelf_type == ShelfType::SIMPLE_SHELF) {
	GetBigHouseView()->MakeSimpleShelf((int)shelf_info_.width, (int)shelf_info_.longs,
		(int)shelf_info_.height, (int)shelf_info_.numf,
		(int)shelf_info_.nums, shelf_info_.shelf_angle);
	  
	} else if (shelf_type == ShelfType::DOUBLE_SHELF) {
	  
	} else if (shelf_type == ShelfType::CIRCLE_SHELF) {
	  
	} else {
	  AfxMessageBox(L"Không tạo được kệ sản phẩm");
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
  
	float red_color = 0.0f;
	float green_color = 0.0f;
	float blue_color = 0.0f;
	CRect rect;

	BigHouseApp *pApp = (BigHouseApp*)AfxGetApp();
	// option custom color for background
	background_color_.GetWindowRect(&rect);
	ScreenToClient(&rect);
	if (rect.PtInRect(pt)) {
		CColorDialog dlg(pApp->option_color_glback);
		if (dlg.DoModal() == IDOK) {
			pApp->option_color_glback = dlg.GetColor();
			BigHouseView *pView = (BigHouseView*)GetBigHouseView();
			red_color = GetRValue(pApp->option_color_glback)/255.0F;
			green_color = GetGValue(pApp->option_color_glback)/255.0f;
			blue_color = GetBValue(pApp->option_color_glback)/255.0f;
			pView->SetColorForBackGround(red_color, green_color, blue_color);

			this->InvalidateRect(&rect,FALSE);
			pView->InvalidateRect(NULL,FALSE); 
		}
	}


	CFormView::OnLButtonDown(nFlags, pt);
}


