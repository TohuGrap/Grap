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
#include "XmlUtls.h"
#include <algorithm>
// FormBar

IMPLEMENT_DYNCREATE(FormBar, CFormView)

FormBar::FormBar()
	: CFormView(FormBar::IDD)
{
	index_shelf_ = -1;
	index_product_ = -1;
}

FormBar::~FormBar()
{
}

void FormBar::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PRODUCT, list_view_product_);
	DDX_Control(pDX, IDC_LIST_SHELF, list_view_shelf_);
}

BEGIN_MESSAGE_MAP(FormBar, CFormView)
	ON_BN_CLICKED(IDC_BTN_SHELF_SELECTED, &FormBar::OnBnShelfSelected)
	ON_BN_CLICKED(IDC_BTN_PRODUCTION_SELECTED, &FormBar::OnBnProductionSelected)
	ON_BN_CLICKED(IDC_BTN_SHELF_IMPORT, &FormBar::ImportFile)
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


	list_view_shelf_.SetFormBarShelf(this);
	list_view_product_.SetFormBarProduct(this);
  CButton* btn_select_shelf = reinterpret_cast<CButton*>(GetDlgItem(IDC_BTN_SHELF_SELECTED));
	btn_select_shelf->EnableWindow(FALSE);
	// Init list view shelf
	InitListViewShelf();
	// Init list view production
	InitListViewProduct();

  return ;
}

void FormBar::InitListViewShelf() {
  list_view_shelf_.SetExtendedStyle(list_view_shelf_.GetExtendedStyle() |
																		LVS_EX_FULLROWSELECT |
																		LVS_EX_GRIDLINES);

	list_view_shelf_.InsertColumn(0, _T("Loại Kệ"), LVCFMT_LEFT, 80);
	list_view_shelf_.InsertColumn(1, _T("Chiêu Dài"), LVCFMT_LEFT, 80);
	list_view_shelf_.InsertColumn(2, _T("Chiêu Rộng"), LVCFMT_LEFT, 80);
	list_view_shelf_.InsertColumn(3, _T("Chiêu Cao"), LVCFMT_LEFT, 80);
	list_view_shelf_.InsertColumn(4, _T("Bán Kính"), LVCFMT_LEFT, 80);
	list_view_shelf_.InsertColumn(5, _T("Góc Bắt Đầu"), LVCFMT_LEFT, 80);
	list_view_shelf_.InsertColumn(6, _T("Góc Kết Thúc"), LVCFMT_LEFT, 80);
	list_view_shelf_.InsertColumn(7, _T("Độ Tròn"), LVCFMT_LEFT, 60);
	list_view_shelf_.InsertColumn(8, _T("Số tầng"), LVCFMT_LEFT, 60);
}

void FormBar::InitListViewProduct() {
	list_view_product_.SetExtendedStyle(list_view_shelf_.GetExtendedStyle() |
																			LVS_EX_FULLROWSELECT |
																			LVS_EX_GRIDLINES);

	list_view_product_.InsertColumn(0, _T("Mã Sản Phẩm"), LVCFMT_LEFT, 80);
	list_view_product_.InsertColumn(1, _T("Tên Sản Phẩm"), LVCFMT_LEFT, 80);
}


void FormBar::SetDataForListShelf(UINT number_of_shelf) {
  CString str_type;
	CString str_longs;
	CString str_width;
	CString str_height;
  CString str_radius;
  CString str_start_angle;
	CString str_end_angle;
	CString str_flat_angle;
	CString str_numf_;

	UINT i = number_of_shelf - 1;
		if (shelf_info_list_[i].shelf_type == ShelfType::SIMPLE_SHELF) {
			str_type = _T("Kệ Đơn");
		} else if (shelf_info_list_[i].shelf_type == ShelfType::DOUBLE_SHELF) {
			str_type = _T("Kệ Đôi");
		} else if (shelf_info_list_[i].shelf_type == ShelfType::CIRCLE_SHELF) {
			str_type = _T("Kệ Tròn");
		} else {
			str_type = _T("Không rõ");
		}

		if (shelf_info_list_[i].shelf_type == ShelfType::SIMPLE_SHELF ||
			shelf_info_list_[i].shelf_type == ShelfType::DOUBLE_SHELF ) {
				str_longs.Format(_T("%.2f"), shelf_info_list_[i].longs);
				str_width.Format(_T("%.2f"), shelf_info_list_[i].width);
		}

		str_height.Format(_T("%.2f"), shelf_info_list_[i].height);
		str_numf_.Format(_T("%.2f"), shelf_info_list_[i].numf);

		if (shelf_info_list_[i].shelf_type == ShelfType::CIRCLE_SHELF) {
			str_start_angle.Format(_T("%.2f"), shelf_info_list_[i].shelf_start_angle_);
			str_end_angle.Format(_T("%.2f"), shelf_info_list_[i].shelf_end_angle_);
			str_flat_angle.Format(_T("%.2f"), shelf_info_list_[i].shelf_flat_angle_);
			str_radius.Format(_T("%.2f"), shelf_info_list_[i].shelf_radius);
		}

		list_view_shelf_.InsertItem(0, str_type);
		list_view_shelf_.SetItemText(0, 1, str_longs);
		list_view_shelf_.SetItemText(0, 2, str_width);
		list_view_shelf_.SetItemText(0, 3, str_height);
		list_view_shelf_.SetItemText(0, 4, str_radius);
		list_view_shelf_.SetItemText(0, 5, str_start_angle);
		list_view_shelf_.SetItemText(0, 6, str_end_angle);
		list_view_shelf_.SetItemText(0, 7, str_flat_angle);
		list_view_shelf_.SetItemText(0, 8, str_numf_);
}


void FormBar::SetDataForListProduct( int number_of_product )
{
	UINT i = number_of_product - 1;
	CString str_id;
	str_id.Format(_T("%d"), i);
	list_view_product_.InsertItem(0, str_id);
	list_view_product_.SetItemText(0, 1, Base::RemoveExtensionFile(production_list_[i]));
}


void FormBar::OnPaint() {
 // CFormView::OnPaint();

	CPaintDC dc(this);
	BigHouseApp *pApp = (BigHouseApp*)AfxGetApp();

}

void FormBar::OnSize(UINT nType, int cx, int cy) {
  EnableScrollBarCtrl(SB_VERT, FALSE);

	CRect rect;
	CRect rect_list_shelf;
	CRect rect_list_product;
  GetClientRect(rect);
	if (list_view_shelf_.m_hWnd != NULL) {
		list_view_shelf_.GetWindowRect(rect_list_shelf);

		list_view_shelf_.SetWindowPos(NULL, 2, 2, rect.Width(), rect_list_shelf.Height(), SWP_SHOWWINDOW);
	}

	if (list_view_product_.m_hWnd != NULL) {
		int height = rect_list_shelf.Height() + 60;
		list_view_product_.GetWindowRect(rect_list_product);
		list_view_product_.SetWindowPos(NULL, 2, height, rect.Width(), rect_list_product.Height(), SWP_SHOWWINDOW);
	}
  CFormView::OnSize(nType, cx, cy);
}
// set to view
BigHouseView *FormBar::GetBigHouseView() {
  BigHouseApp *pApp= (BigHouseApp*)AfxGetApp();
  MainFrame *pMainFrame = (MainFrame*)pApp->m_pMainWnd;
  BigHouseView*pView = reinterpret_cast<BigHouseView*>(pMainFrame->splitter_.GetPane(0, 1));
  return pView;
}


void FormBar::OnBnProductionSelected()
{
	int i = production_list_.size() - 1 - index_product_;
	if (i < 0) {
		AfxMessageBox(_T("Bạn chưa chọn sản phẩm trong danh sách"));
		return;
	}
	theApp.LoadFileCad(production_list_[i]);
}


void FormBar::OnBnShelfSelected()
{
	int i = shelf_info_list_.size() - 1 - index_shelf_;
	if (i < 0) {
		AfxMessageBox(_T("Bạn chưa chọn kệ trong danh sách"));
		return;
	}

	UINT shelf_type = shelf_info_list_[i].shelf_type;
	CButton* production_setting= reinterpret_cast<CButton*>(GetDlgItem(IDC_OBJECT_OPTION));

	CButton *production_selected = reinterpret_cast<CButton*>(GetDlgItem(IDC_BTN_PRODUCTION_SELECTED));

	if (shelf_type == ShelfType::SIMPLE_SHELF) {
	GetBigHouseView()->MakeSimpleShelf(shelf_info_list_[i].width, shelf_info_list_[i].longs,
																		 shelf_info_list_[i].height, shelf_info_list_[i].numf);
		CButton* btn_select_product = reinterpret_cast<CButton*>(GetDlgItem(IDC_BTN_PRODUCTION_SELECTED));
		btn_select_product->EnableWindow(TRUE);

	} else if (shelf_type == ShelfType::DOUBLE_SHELF) {
		GetBigHouseView()->MakeDoubleShelf(shelf_info_list_[i].width, shelf_info_list_[i].longs,
																			 shelf_info_list_[i].height, shelf_info_list_[i].numf);
		CButton* btn_select_product = reinterpret_cast<CButton*>(GetDlgItem(IDC_BTN_PRODUCTION_SELECTED));
		btn_select_product->EnableWindow(TRUE);

	} else if (shelf_type == ShelfType::CIRCLE_SHELF) {
	  GetBigHouseView()->MakeCircleShelf(shelf_info_list_[i].shelf_radius,
																			 shelf_info_list_[i].height,
																			 shelf_info_list_[i].shelf_start_angle_,
																			 shelf_info_list_[i].shelf_end_angle_,
																			 shelf_info_list_[i].shelf_flat_angle_,
																			 shelf_info_list_[i].numf);
		CButton* btn_select_product = reinterpret_cast<CButton*>(GetDlgItem(IDC_BTN_PRODUCTION_SELECTED));
		btn_select_product->EnableWindow(TRUE);

	} else {
		GetBigHouseView()->MakeSimpleShelf(shelf_info_list_[i].width, shelf_info_list_[i].longs,
		shelf_info_list_[i].height, shelf_info_list_[i].numf);
		CButton* btn_select_product = reinterpret_cast<CButton*>(GetDlgItem(IDC_BTN_PRODUCTION_SELECTED));
		btn_select_product->EnableWindow(TRUE);
	}
}


void FormBar::OnLButtonUp(UINT nFlags, CPoint pt) {
	GetBigHouseView()->SetIsLbuttonDown(false);
  
	/*float red_color = 0.0f;
	float green_color = 0.0f;
	float blue_color = 0.0f;
	CRect rect;

	BigHouseApp *pApp = (BigHouseApp*)AfxGetApp();*/
	// option custom color for background
	/*
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
	*/

	CFormView::OnLButtonDown(nFlags, pt);
}

void FormBar::HandleListViewShelf( int item )
{
	CButton* btn_select_shelf = reinterpret_cast<CButton*>(GetDlgItem(IDC_BTN_SHELF_SELECTED));
	btn_select_shelf->EnableWindow(TRUE);
	index_shelf_ = item;
}

void FormBar::HandleListViewProduct(int item) {
  CButton* btn_select_product = reinterpret_cast<CButton*>(GetDlgItem(IDC_BTN_PRODUCTION_SELECTED));
  btn_select_product->EnableWindow(TRUE);
	index_product_ = item;
}

void FormBar::SetShelfInfoList(ShelfInfo shelf_info, bool &is_exist) {
	bool is_exist_shelf_ = false;
	for (int i = 0; i < shelf_info_list_.size(); i++) {
    if (shelf_info_list_[i].shelf_type == shelf_info.shelf_type &&
				shelf_info_list_[i].longs == shelf_info.longs  &&
				shelf_info_list_[i].width == shelf_info.width &&
				shelf_info_list_[i].height == shelf_info.height &&
				shelf_info_list_[i].numf == shelf_info.numf &&
				shelf_info_list_[i].shelf_start_angle_ == shelf_info.shelf_start_angle_ &&
				shelf_info_list_[i].shelf_end_angle_ == shelf_info.shelf_end_angle_ &&
				shelf_info_list_[i].shelf_radius == shelf_info.shelf_radius &&
				shelf_info_list_[i].shelf_flat_angle_ == shelf_info.shelf_flat_angle_) {
      is_exist_shelf_ = true;
			break;
		} else {
			is_exist_shelf_ = false;
		}
	}
	is_exist = is_exist_shelf_;
	if (is_exist_shelf_ == false) {
		shelf_info_list_.push_back(shelf_info);
	}
}

void FormBar::SetProductionList(CString name_product, bool &is_exist) {
	bool is_exist_product_ = false;
  for (int i = 0; i < production_list_.size(); i++) {
	  if (production_list_[i] == name_product) {
			is_exist_product_ = true;
			break;
		} else {
			is_exist_product_ = false;
		}
	}
	is_exist = is_exist_product_;
	if (is_exist_product_ == false) {
		production_list_.push_back(name_product);
	}
}

void FormBar::DisableLoadProduct() {
	CButton* btn = reinterpret_cast<CButton*>(GetDlgItem(IDC_BTN_PRODUCTION_SELECTED));
  btn->EnableWindow(FALSE);
}

void FormBar::LoadSimShelfFile(CString& path_file )
{
  xmlDocPtr doc;
	xmlNodePtr cur;
	std::string file_name = CStringA(path_file);

	// get doc
	const char* chstr = file_name.c_str();
	doc = xmlParseFile(chstr);
	if (doc == NULL) {
		::MessageBox(NULL, _T("Load file thất bại \n Cấu trúc xml của file không chính xác"), _T("Thông báo"), MB_OK|MB_ICONERROR);
		return;
	}
	// get cur
	cur = xmlDocGetRootElement(doc);
	if (cur == NULL) {
		xmlFree(doc);
		return;
	}
	// get "root" element
	if (xmlStrcmp(cur->name, (const xmlChar*)"root")) {
		xmlFree(doc);
		return;
	}

	std::vector<UINT> type_list_;
	std::vector<double> longs_list;
	std::vector<double> width_list;
	std::vector<double> height_list;
	std::vector<int> floor_list;
	std::vector<double> radius_list;
	std::vector<double> angle_start_list;
	std::vector<double> angle_end_list;
	std::vector<double> angle_flat_list;

	std::vector<ShelfInfo> shelf_info_list;

		cur = cur->children;
		while (cur != NULL) {
			if (!xmlStrcmp(cur->name, (const xmlChar*)"loai")) {
				std::wstring str_type = XmlUtls::GetStringContent(doc, cur);
				if (str_type == L"doi") {
					type_list_.push_back(ShelfType::DOUBLE_SHELF);
				} else if (str_type == L"tron") {
					type_list_.push_back(ShelfType::CIRCLE_SHELF);
				} else {
					type_list_.push_back(ShelfType::SIMPLE_SHELF);
				}
			}
			if (!xmlStrcmp(cur->name, (const xmlChar*)"dai")) {
				double longs =  XmlUtls::GetDoubleContent(doc, cur);
			  longs_list.push_back(longs);
				radius_list.push_back(0);
				angle_start_list.push_back(0);
				angle_end_list.push_back(0);
				angle_flat_list.push_back(0);
			}
			if (!xmlStrcmp(cur->name, (const xmlChar*)"rong")) {
				double width = XmlUtls::GetDoubleContent(doc, cur);
			  width_list.push_back(width);
			}
			if (!xmlStrcmp(cur->name, (const xmlChar*)"cao")) {
				double height = XmlUtls::GetDoubleContent(doc, cur);
			  height_list.push_back(height);
			}
			if (!xmlStrcmp(cur->name, (const xmlChar*)"tang")) {
				unsigned int floors = XmlUtls::GetIntContent(doc, cur);
			  floor_list.push_back(floors);
			}
			if (!xmlStrcmp(cur->name, (const xmlChar*)"bankinh")) {
				double radius = XmlUtls::GetDoubleContent(doc, cur);
				radius_list.push_back(radius);
				longs_list.push_back(0);
				width_list.push_back(0);
			}

			if (!xmlStrcmp(cur->name, (const xmlChar*)"gocbd")) {
        double angle_start = XmlUtls::GetDoubleContent(doc, cur);
				angle_start_list.push_back(angle_start);
			}

			if (!xmlStrcmp(cur->name, (const xmlChar*)"gockt")) {
				double end_start = XmlUtls::GetDoubleContent(doc, cur);
				angle_end_list.push_back(end_start);
			}

			if (!xmlStrcmp(cur->name, (const xmlChar*)"min")) {
				double angle_flat = XmlUtls::GetDoubleContent(doc, cur);
				angle_flat_list.push_back(angle_flat);
			}

			cur = cur->next;
		}
	xmlFreeDoc(doc);

	 ShelfInfo shelf_info;
	int number = type_list_.size();
	for (int i = 0; i< number; i++) {
		shelf_info.shelf_type = type_list_[i];
		shelf_info.longs = longs_list[i];
		shelf_info.width = width_list[i];
		shelf_info.height = height_list[i];
		shelf_info.numf = floor_list[i];
		shelf_info.shelf_radius = radius_list[i];
		shelf_info.shelf_start_angle_ = angle_start_list[i];
		shelf_info.shelf_end_angle_ = angle_end_list[i];
		shelf_info.shelf_flat_angle_ = angle_flat_list[i];

		shelf_info_list.push_back(shelf_info);
	}

	for (int i = 0; i <shelf_info_list_.size(); i++) {
    list_view_shelf_.DeleteItem(0);
	}

	shelf_info_list_.clear();
	shelf_info_list_ = shelf_info_list;
	for (int i = shelf_info_list.size(); i > 0; i--) {
    SetDataForListShelf(i);
	}

	std::reverse(shelf_info_list_.begin(), shelf_info_list_.end());
}


void FormBar::ImportFile() {
	CFileDialog dlg(TRUE, L"", L"",
		OFN_HIDEREADONLY | OFN_FILEMUSTEXIST,
		_T("Sim Shelf File (*.sish)|*.sish||"));
	CString path_file(_T(""));
	if (dlg.DoModal() == IDOK) {
		path_file = dlg.GetPathName();
		LoadSimShelfFile(path_file);
	}
}