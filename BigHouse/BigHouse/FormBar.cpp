﻿// FormBar.cpp : implementation file
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
#include <libxml/encoding.h>
#include <libxml/parser.h>
#include <libxml/xmlwriter.h>
#include "DlgProduction.h"
#include <vector>
// FormBar

IMPLEMENT_DYNCREATE(FormBar, CFormView)

FormBar::FormBar()
	: CFormView(FormBar::IDD)
{
	index_shelf_ = -1;
	index_product_ = -1;
}

FormBar::~FormBar() {
	
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
	ON_BN_CLICKED(IDC_BTN_SHELF_EXPORT, &FormBar::ExportFile)
	ON_BN_CLICKED(IDC_BTN_SHELF_IMPORT_CAD, &FormBar::ImportCad)
	ON_BN_CLICKED(IDC_BTN_SHELF_EXPORT_CAD, &FormBar::ExportCad)
	ON_BN_CLICKED(IDC_BTN_REMOVE_LIST_SHELF, &FormBar::RemoveListShelf)
	ON_BN_CLICKED(IDC_BUTTON_DEL_ITEM_SHELF, &FormBar::DelItemFromListShelf)//
	ON_BN_CLICKED(IDC_BUTTON_DEL_ITEM_PRODUCTION, &FormBar::DelItemFromListProduction)//
	ON_BN_CLICKED(IDC_BTN_REMOVE_LIST_COMMODITY, &FormBar::RemoveListCommodity)
	ON_BN_CLICKED(IDC_BUTTON_STORE_COMMODITY, &FormBar::ShowDialogStore)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_DATA_COMMODITY, &FormBar::SaveCommodityToData)
	ON_NOTIFY(NM_CLICK, IDC_LIST_PRODUCT, OnClickListProduction)
	ON_NOTIFY(NM_CLICK, IDC_LIST_SHELF, OnClickListShelf)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_PRODUCT, OnDoubleClickListProduction)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_SHELF, OnDoubleClickListShelf)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_SHELF, OnRightClickListShelf)
	ON_WM_DESTROY()

	ON_MESSAGE(IDC_MESAGER_PRODUCTION, OnMyMessage)
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
	LoadDataFromFileData();
  return ;
}

void FormBar::OnDestroy( ) {
	SaveCommodityToData();
}

void FormBar::InitListViewShelf() {
  list_view_shelf_.SetExtendedStyle(list_view_shelf_.GetExtendedStyle() |
																		LVS_EX_FULLROWSELECT |
																		LVS_EX_GRIDLINES);
	list_view_shelf_.InsertColumn(0, _T("Tên"), LVCFMT_LEFT, 50);
	list_view_shelf_.InsertColumn(1, _T("Loại"), LVCFMT_LEFT, 50);
	list_view_shelf_.InsertColumn(2, _T("Chiêu Dài"), LVCFMT_LEFT, 50);
	list_view_shelf_.InsertColumn(3, _T("Chiêu Rộng"), LVCFMT_LEFT, 50);
	list_view_shelf_.InsertColumn(4, _T("Chiêu Cao"), LVCFMT_LEFT, 50);
	list_view_shelf_.InsertColumn(5, _T("Bán Kính"), LVCFMT_LEFT, 50);
	list_view_shelf_.InsertColumn(6, _T("Số tầng"), LVCFMT_LEFT, 50);
}

void FormBar::InitListViewProduct() {
	list_view_product_.SetExtendedStyle(list_view_shelf_.GetExtendedStyle() |
																			LVS_EX_FULLROWSELECT |
																			LVS_EX_GRIDLINES);


	list_view_product_.InsertColumn(0, _T("Tên Sản Phẩm"), LVCFMT_LEFT, 60);
	list_view_product_.InsertColumn(1, _T("loại"), LVCFMT_LEFT, 60);
	list_view_product_.InsertColumn(2, _T("chiều dài"), LVCFMT_LEFT, 60);
	list_view_product_.InsertColumn(3, _T("chiều rộng"), LVCFMT_LEFT, 60);
	list_view_product_.InsertColumn(4, _T("chiều cao"), LVCFMT_LEFT, 60);
	list_view_product_.InsertColumn(5, _T("trong lượng"), LVCFMT_LEFT, 60);
	list_view_product_.InsertColumn(6, _T("chon tầng"), LVCFMT_LEFT, 60);
	
}


void FormBar::SetDataForListShelf() {
	int count = list_view_shelf_.GetItemCount();
	for(int i = 0; i < count; i ++) {
		list_view_shelf_.DeleteItem(0);
	}
	CString str_name_project;
  CString str_type;
	CString str_longs;
	CString str_width;
	CString str_height;
  CString str_radius;
	CString str_numf;

	//UINT i = number_of_shelf - 1;
	for (int i = 0; i < shelf_info_list_.size(); i++) {
	str_name_project = shelf_info_list_.at(i).name_project;

	if (shelf_info_list_.at(i).shelf_type == ShelfType::SIMPLE_SHELF) {
		str_type = _T("Kệ Đơn");
	} else if (shelf_info_list_.at(i).shelf_type == ShelfType::DOUBLE_SHELF) {
		str_type = _T("Kệ Đôi");
	} else if (shelf_info_list_.at(i).shelf_type == ShelfType::CIRCLE_SHELF) {
		str_type = _T("Kệ Tròn");
	} else if (shelf_info_list_.at(i).shelf_type == ShelfType::CONTAINER) {
		str_type = _T("Thùng chứa");
	} else {
		str_type = _T("Không rõ");
	}

	if(shelf_info_list_.at(i).shelf_type == ShelfType::SIMPLE_SHELF ||
		 shelf_info_list_.at(i).shelf_type == ShelfType::DOUBLE_SHELF||
		 shelf_info_list_.at(i).shelf_type == ShelfType::CONTAINER) {
		str_longs.Format(_T("%.2f"), shelf_info_list_.at(i).longs);
		str_width.Format(_T("%.2f"), shelf_info_list_.at(i).width);
	}

	str_height.Format(_T("%.2f"), shelf_info_list_.at(i).height);

	if(shelf_info_list_.at(i).shelf_type != ShelfType::CONTAINER) {
		str_numf.Format(_T("%d"), shelf_info_list_.at(i).numf);
	}

	if (shelf_info_list_.at(i).shelf_type == ShelfType::CIRCLE_SHELF) {
		str_radius.Format(_T("%.2f"), shelf_info_list_.at(i).shelf_radius);
	}
	int pos = list_view_shelf_.GetItemCount();
	list_view_shelf_.InsertItem(pos, str_name_project);
	list_view_shelf_.SetItemText(pos, 1, str_type);
	list_view_shelf_.SetItemText(pos, 2, str_longs);
	list_view_shelf_.SetItemText(pos, 3, str_width);
	list_view_shelf_.SetItemText(pos, 4, str_height);
	list_view_shelf_.SetItemText(pos, 5, str_radius);
	list_view_shelf_.SetItemText(pos, 6, str_numf);
	}
}


void FormBar::SetDataForListProduct() {
	int size_item = list_view_product_.GetItemCount();
	for(int i = 0; i< size_item; i++) {
		list_view_product_.DeleteItem(0);
	}
	for(int i = 0; i < cad_info_.size(); i ++) {
		CString str_name = cad_info_.at(i).name_production;
		CString str_type;
		if(cad_info_.at(i).type_commodity == TypeCommodity::COMMODITY) {
			str_type = _T("hàng");
		} else {
			str_type = _T("thùng hàng");
		}
		CString str_width;
		str_width.Format(_T("%0.2f"), cad_info_.at(i).width);
		CString str_l;
		str_l.Format(_T("%0.2f"), cad_info_.at(i).lenght);
		CString str_h;
		str_h.Format(_T("%0.2f"), cad_info_.at(i).height);
		CString str_weight;
		str_weight.Format(_T("%0.2f"), cad_info_.at(i).weight);
		CString str_floor;
		str_floor.Format(_T("%d"), cad_info_.at(i).floor);
		CString str_type_commodity;
		if(cad_info_.at(i).type_commodity == TypeCommodity::COMMODITY) {
			str_type_commodity = _T("Hàng");
		} else {
			str_type_commodity = _T("Thùng hàng");
		}
	
		int pos = list_view_product_.GetItemCount();
		list_view_product_.InsertItem(pos, str_name);
		list_view_product_.SetItemText(pos, 1, str_type_commodity);
		list_view_product_.SetItemText(pos, 2, str_l);
		list_view_product_.SetItemText(pos, 3, str_width);
		list_view_product_.SetItemText(pos, 4, str_h);
		list_view_product_.SetItemText(pos, 5, str_weight);
		list_view_product_.SetItemText(pos, 6, str_floor);
	}
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


void FormBar::OnBnProductionSelected() {
	int i = cad_info_.size() - 1 - index_product_;
	if (i < 0) {
		AfxMessageBox(_T("Bạn chưa chọn sản phẩm trong danh sách"));
		return;
	}
	//theApp.LoadFileCad(cad_info_.at(i));
	GetBigHouseView()->CreateArrangeCommodity(cad_info_);
}


void FormBar::OnBnShelfSelected() {
	//int i = shelf_info_list_.size() - 1 - index_shelf_;
	//if (i < 0) {
	//	AfxMessageBox(_T("Bạn chưa chọn kệ trong danh sách"));
	//	return;
	//}
	if( index_shelf_ == -1) {
		return;
	}
	UINT shelf_type = shelf_info_list_[index_shelf_].shelf_type;
	CButton* production_setting= reinterpret_cast<CButton*>(GetDlgItem(IDC_OBJECT_OPTION));

	CButton *production_selected = reinterpret_cast<CButton*>(GetDlgItem(IDC_BTN_PRODUCTION_SELECTED));

	if (shelf_type == ShelfType::SIMPLE_SHELF) {
	GetBigHouseView()->MakeSimpleShelf(shelf_info_list_[index_shelf_].width, 
		                                 shelf_info_list_[index_shelf_].longs,
																		 shelf_info_list_[index_shelf_].height, 
																		 shelf_info_list_[index_shelf_].dis_drag,
																		 shelf_info_list_[index_shelf_].dis_wall,
																		 shelf_info_list_[index_shelf_].numf);
		CButton* btn_select_product = reinterpret_cast<CButton*>(GetDlgItem(IDC_BTN_PRODUCTION_SELECTED));
		btn_select_product->EnableWindow(TRUE);

	} else if (shelf_type == ShelfType::DOUBLE_SHELF) {
		GetBigHouseView()->MakeDoubleShelf(shelf_info_list_[index_shelf_].width, 
			                                 shelf_info_list_[index_shelf_].longs,
																			 shelf_info_list_[index_shelf_].height, 
																			 shelf_info_list_[index_shelf_].dis_drag, 
																			 shelf_info_list_[index_shelf_].numf);
		CButton* btn_select_product = reinterpret_cast<CButton*>(GetDlgItem(IDC_BTN_PRODUCTION_SELECTED));
		btn_select_product->EnableWindow(TRUE);

	} else if (shelf_type == ShelfType::CIRCLE_SHELF) {
	  GetBigHouseView()->MakeCircleShelf(shelf_info_list_[index_shelf_].shelf_radius,
																			 shelf_info_list_[index_shelf_].height,
																			 shelf_info_list_[index_shelf_].dis_drag,
																			 shelf_info_list_[index_shelf_].numf);
		CButton* btn_select_product = reinterpret_cast<CButton*>(GetDlgItem(IDC_BTN_PRODUCTION_SELECTED));
		btn_select_product->EnableWindow(TRUE);

	} else if (shelf_type == ShelfType::CIRCLE_SHELF) {
		GetBigHouseView()->MakeSimpleShelf(shelf_info_list_[index_shelf_].width, 
			                                 shelf_info_list_[index_shelf_].longs,
		                                   shelf_info_list_[index_shelf_].height, 
																			 shelf_info_list_[index_shelf_].dis_drag,
																			 shelf_info_list_[index_shelf_].dis_wall,
																			 shelf_info_list_[index_shelf_].numf);
		CButton* btn_select_product = reinterpret_cast<CButton*>(GetDlgItem(IDC_BTN_PRODUCTION_SELECTED));
		btn_select_product->EnableWindow(TRUE);
	} else {
		GetBigHouseView()->MakeArrangeCommodity(shelf_info_list_[index_shelf_].longs,
																			shelf_info_list_[index_shelf_].width,
																			shelf_info_list_[index_shelf_].height,
																			shelf_info_list_[index_shelf_].dis_wall);
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
//
//void FormBar::HandleListViewShelf( int item )
//{
//	//CButton* btn_select_shelf = reinterpret_cast<CButton*>(GetDlgItem(IDC_BTN_SHELF_SELECTED));
//	//btn_select_shelf->EnableWindow(TRUE);
//	//index_shelf_ = item;
//}
//
//void FormBar::HandleListViewProduct(int item) {
// // CButton* btn_select_product = reinterpret_cast<CButton*>(GetDlgItem(IDC_BTN_PRODUCTION_SELECTED));
// // btn_select_product->EnableWindow(TRUE);
//	//index_product_ = item;
//}

void FormBar::SetupShelf() {
	if(!dlg_shelf_.GetSafeHwnd()) {
		dlg_shelf_.CreateShelf(this);
	} 
	dlg_shelf_.SetPositionItem(shelf_info_list_.size());
	dlg_shelf_.ShowWindow(SW_SHOW);
}

void FormBar::SetShelfInfoList(ShelfInfo &shelf_info, int pos_item) {
	if(pos_item < 0)
		return;
	if(pos_item == shelf_info_list_.size()) {
		shelf_info_list_.push_back(shelf_info);
	} else {
		shelf_info_list_.at(pos_item) = shelf_info;
	}
	SetDataForListShelf();

}

void FormBar::SetProductionList(CadInfo & cad_info, int pos_item) {
	if(pos_item == - 1 )
		return;
	if(pos_item == cad_info_.size()|| pos_item == - 2) {
		cad_info_.push_back(cad_info);
	} else {
		cad_info_.at(pos_item) = cad_info;
	}
	SetDataForListProduct();
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

	std::vector<ShelfType> type_list_;
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
		shelf_info_list.push_back(shelf_info);
	}

  RemoveListShelf();
	shelf_info_list_ = shelf_info_list;
	//for (int i = shelf_info_list.size(); i > 0; i--) {
  SetDataForListShelf();
	//}

	std::reverse(shelf_info_list_.begin(), shelf_info_list_.end());
}


void FormBar::ImportFile() {
	CFileDialog dlg(TRUE, L"", L"",
		OFN_HIDEREADONLY | OFN_FILEMUSTEXIST,
		_T("File kệ (*.sish)|*.sish||"));
	CString path_file(_T(""));
	if (dlg.DoModal() == IDOK) {
		path_file = dlg.GetPathName();
		LoadSimShelfFile(path_file);
	}
}

void FormBar::ExportFile() {
	CFileDialog dlg (FALSE, L"", L"",
									OFN_HIDEREADONLY | OFN_FILEMUSTEXIST,
		               _T("File kệ(*.sish)|*.sish||"));
	CString str_path(_T(""));
	if (dlg.DoModal() == IDOK) {
    str_path = dlg.GetPathName();
		SaveSimShelfFile(str_path);
	}
}

bool FormBar::SaveSimShelfFile( CString& path_file ) {
	std::vector<ShelfInfo> shelf_info_list;
	shelf_info_list = shelf_info_list_;
	reverse(shelf_info_list.begin(), shelf_info_list.end());

	std::string str = CStringA(path_file);

	xmlTextWriterPtr writer = xmlNewTextWriterFilename(str.c_str(), 0);
	if (NULL == writer) {
		return false;
	}

	// Set indentation
	int rc = xmlTextWriterSetIndent(writer, 1);
	if (rc < 0) return false;

	const char* kXmlDefaultVersion = NULL;
	const char* kUtf8Encoding = "UTF-8";
	const char* kDefaultStandalon = NULL;
	rc = xmlTextWriterStartDocument(writer,
		kXmlDefaultVersion,
		kUtf8Encoding,
		kDefaultStandalon);
	if (rc < 0) {
		return false;
	}

	rc = xmlTextWriterStartElement(writer, BAD_CAST "root");
	if (rc < 0) return false;
	for (int i = 0; i < shelf_info_list.size(); i++) {
		 char cstr[10];
		if (shelf_info_list[i].shelf_type == ShelfType::CIRCLE_SHELF) {
       strcpy(cstr, "tron");
		} else if (shelf_info_list[i].shelf_type == ShelfType::DOUBLE_SHELF) {
       strcpy(cstr, "doi");
		} else {
			strcpy(cstr, "don");
		}

			rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST"loai", "%s", (const xmlChar*)cstr);
			if (rc < 0) return false;

		if (shelf_info_list[i].shelf_type == ShelfType::SIMPLE_SHELF ||
				shelf_info_list[i].shelf_type == ShelfType::DOUBLE_SHELF) {
			rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST"dai", "%f", shelf_info_list[i].longs);
			if (rc < 0) return false;

			rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST"rong", "%f", shelf_info_list[i].width);
			if (rc < 0) return false;
		}
		// Height
		rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST"cao","%f",shelf_info_list[i].height);
		if (rc < 0) return false;
		
		// Floor
		rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST"tang", "%d", shelf_info_list[i].numf);
		if (rc < 0) return false;
		

		if (shelf_info_list[i].shelf_type == ShelfType::CIRCLE_SHELF) {
			rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST"bankinh", "%f", shelf_info_list[i].shelf_radius);
			if (rc < 0) return false;
		}
	}
	// End document.
	rc = xmlTextWriterEndDocument(writer);
	if (rc < 0) return false;

	xmlFreeTextWriter(writer);

	return true;
}


void FormBar::ImportCad() {
	CFileDialog dlg (TRUE, L"", L"",
		OFN_HIDEREADONLY | OFN_FILEMUSTEXIST,
		_T(" File Sản Phẩm(*.prd)|*.prd||"));
	CString str_path(_T(""));
	if (dlg.DoModal() == IDOK) {
		str_path = dlg.GetPathName();
		cad_info_.clear();
		//LoadCadFile(str_path);
		LoadListCommodity(str_path, cad_info_);
		RemoveListCommodity();
		SetDataForListProduct();
	}
}

void FormBar::LoadCadFile(CString& str) {
	xmlDocPtr doc;
	xmlNodePtr cur;
	std::string file_name = CStringA(str);

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

	std::vector<CadInfo> cad_info;
	//std::vector<CString> cad_list;

	cur = cur->children;
	CadInfo production;
	while (cur != NULL) {

		if (!xmlStrcmp(cur->name, (const xmlChar*)"ten")) {
			std::wstring name =  XmlUtls::GetStringContent(doc, cur);
			CString str = name.c_str();
			production.name_production = str;
		}
		if (!xmlStrcmp(cur->name, (const xmlChar*)"loai")) {
			int type_co =  XmlUtls::GetIntContent(doc, cur);

			if(type_co == 0) {
				production.type_commodity = TypeCommodity::COMMODITY;
			} else {
				production.type_commodity = TypeCommodity::CO_CONTAINER;
			}
		}
		if (!xmlStrcmp(cur->name, (const xmlChar*)"dai")) {
			double lenght =  XmlUtls::GetDoubleContent(doc, cur);
			production.lenght = lenght;
		}

		if (!xmlStrcmp(cur->name, (const xmlChar*)"rong")) {
			double width =  XmlUtls::GetDoubleContent(doc, cur);
			production.width = width;
		}

		if (!xmlStrcmp(cur->name, (const xmlChar*)"cao")) {
			double height =  XmlUtls::GetDoubleContent(doc, cur);
			production.height = height;
		}
		if (!xmlStrcmp(cur->name, (const xmlChar*)"trong_luong")) {
			double weight =  XmlUtls::GetDoubleContent(doc, cur);
			production.weight = weight;
		}

		if (!xmlStrcmp(cur->name, (const xmlChar*)"so_luong")) {
			double count =  XmlUtls::GetDoubleContent(doc, cur);
			production.count = count;
		}

		if (!xmlStrcmp(cur->name, (const xmlChar*)"tang")) {
			double floor =  XmlUtls::GetDoubleContent(doc, cur);
			production.floor = floor;
			cad_info.push_back(production);
		}
		//if (!xmlStrcmp(cur->name, (const xmlChar*)"End")) {
		//	cad_info.push_back(production);
		//}
		cur = cur->next;
	}
	xmlFreeDoc(doc);

	RemoveListCommodity();
	//production_list_ = cad_list;
	cad_info_ = cad_info;
	SetDataForListProduct();

	//std::reverse(cad_info_.begin(), production_list_.end());
}

void FormBar::ExportCad() {
	CFileDialog dlg (FALSE, L"", L"",
		OFN_HIDEREADONLY | OFN_FILEMUSTEXIST,
		_T("File Sản Phẩm (*.prd)|*.prd||"));
	CString str_path(_T(""));
	if (dlg.DoModal() == IDOK) {
		str_path = dlg.GetPathName();
		SaveListCommodity(str_path, cad_info_);
		//SaveCadFile(str_path);
	}
}



bool FormBar::SaveCadFile(CString& str) {

	std::vector<CadInfo> cad_info;
	//cad_list = production_list_;
	cad_info = cad_info_;
	//reverse(cad_list.begin(), cad_list.end());

	std::string str_file = CStringA(str);

	xmlTextWriterPtr writer = xmlNewTextWriterFilename(str_file.c_str(), 0);
	if (NULL == writer) {
		return false;
	}

	// Set indentation
	int rc = xmlTextWriterSetIndent(writer, 1);
	if (rc < 0) return false;

	const char* kXmlDefaultVersion = NULL;
	const char* kUtf8Encoding = "UTF-8";
	const char* kDefaultStandalon = NULL;
	rc = xmlTextWriterStartDocument(writer,
		kXmlDefaultVersion,
		kUtf8Encoding,
		kDefaultStandalon);
	if (rc < 0) {
		return false;
	}

	rc = xmlTextWriterStartElement(writer, BAD_CAST "root");
	if (rc < 0) 
		return false;
	for (int i = 0; i < cad_info.size(); i++) {
		char* chstr = Base::CStringToChar(cad_info_.at(i).name_production/*str*/);
		rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST"ten","%s", (const xmlChar*)chstr);
		if (rc < 0) {
			return false;
		}
		rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST"loai","%d", cad_info_.at(i).type_commodity);
		if (rc < 0) {
			return false;
		}
		rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST"dai", "%f", cad_info.at(i).lenght);
		if (rc < 0) return false;
		rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST"rong", "%f", cad_info.at(i).width);
		if (rc < 0) return false;
		rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST"cao", "%f", cad_info.at(i).height);
		if (rc < 0) return false;
		rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST"trong_luong", "%f", cad_info.at(i).weight);
		if (rc < 0) return false;
		rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST"so_luong", "%d", cad_info.at(i).count);
		if (rc < 0) return false;
		rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST"tang", "%d", cad_info.at(i).floor);
		if (rc < 0) return false;
	}
	// End document.
	rc = xmlTextWriterEndDocument(writer);
	if (rc < 0) 
		return false;

	xmlFreeTextWriter(writer);

	return true;
}

void FormBar::RemoveListShelf() {
	int count = list_view_shelf_.GetItemCount();
	for (int i = 0; i <count; i++) {
    list_view_shelf_.DeleteItem(0);
	}
	shelf_info_list_.clear();
}
void FormBar::RemoveListCommodity() {
	int count = list_view_product_.GetItemCount();
	for (int i = 0; i < count; i++) {
		list_view_product_.DeleteItem(0);
	}
	cad_info_.clear();
}

void FormBar::OnDoubleClickListProduction(NMHDR* pNMHDR, LRESULT* pResult) {
	HWND hWnd1 =  ::GetDlgItem (m_hWnd,IDC_LIST_EDIT_PRODUCTION);
	LPNMITEMACTIVATE temp = (LPNMITEMACTIVATE) pNMHDR;
	RECT rect;
	//get the row number
	int n_item = temp->iItem;
	//get the column number
	//nSubItem_ = temp->iSubItem;
	if(n_item == -1) {
		return;
	}
	if(!dlg_production_.GetSafeHwnd()) {
		dlg_production_.Create(this);
	}
	if(dlg_production_.IsWindowVisible()) {
		return;
	} else {
		dlg_production_.ShowWindow(SW_SHOW);
	}
	if(n_item >= cad_info_.size()) {
		dlg_production_.ShowWindow(SW_HIDE);
		return;
	}
	dlg_production_.SetData(cad_info_.at(n_item));
	dlg_production_.SetPositionListViewCtrl(n_item);

}

void FormBar::OnClickListProduction(NMHDR* pNMHDR, LRESULT* pResult) {
	HWND hWnd1 =  ::GetDlgItem (m_hWnd,IDC_LIST_EDIT_PRODUCTION);
	LPNMITEMACTIVATE temp = (LPNMITEMACTIVATE) pNMHDR;
	//get the row number
   index_product_ = temp->iItem;
}

void FormBar::OnClickListShelf(NMHDR* pNMHDR, LRESULT* pResult) {
	HWND hWnd1 =  ::GetDlgItem (m_hWnd,IDC_LIST_SHELF);
	LPNMITEMACTIVATE temp = (LPNMITEMACTIVATE) pNMHDR;

	//get the row number
	index_shelf_ = temp->iItem;
	CButton* btn_select_shelf = reinterpret_cast<CButton*>(GetDlgItem(IDC_BTN_SHELF_SELECTED));
	if(index_shelf_ != -1) {
		btn_select_shelf->EnableWindow(TRUE);
	} else {
		btn_select_shelf->EnableWindow(FALSE);
	}
}

void FormBar::OnDoubleClickListShelf(NMHDR* pNMHDR, LRESULT* pResult) {
	HWND hWnd1 =  ::GetDlgItem (m_hWnd,IDC_LIST_SHELF);
	LPNMITEMACTIVATE temp = (LPNMITEMACTIVATE) pNMHDR;

	//get the row number
	int n_item = temp->iItem;
	//get the column number
	//nSubItem_ = temp->iSubItem;
	if(n_item == -1) {
		return;
	}
	if(!dlg_shelf_.GetSafeHwnd()) {
		dlg_shelf_.CreateShelf(this);
	}
	if(dlg_shelf_.IsWindowVisible()) {
		return;
	} else {
		dlg_shelf_.ShowWindow(SW_SHOW);
	}
	if(n_item >= shelf_info_list_.size()) {
		dlg_shelf_.ShowWindow(SW_HIDE);
		return;
	}
	dlg_shelf_.SetPositionItem(n_item);
	dlg_shelf_.SetData(shelf_info_list_.at(n_item));


}

LRESULT FormBar::OnMyMessage(WPARAM wParam, LPARAM lParam){
	cad_info_ = std::vector<CadInfo>(wParam);
	return 0;
}

void FormBar::SetupProduction() {
	if(!dlg_production_.GetSafeHwnd()) {
		dlg_production_.Create(this);
	}
	dlg_production_.SetPositionListViewCtrl(-2);
	dlg_production_.InitCommodity();
	dlg_production_.ShowWindow(SW_SHOW);
}

void FormBar::OnRightClickListShelf(NMHDR* pNMHDR, LRESULT* pResult) {
  Invalidate();
	HWND hWnd1 =  ::GetDlgItem (m_hWnd,IDC_LIST_SHELF);
	LPNMITEMACTIVATE temp = (LPNMITEMACTIVATE) pNMHDR;
	int n_item = temp->iItem;
	if(n_item == - 1) {
		return;
	}
	if(shelf_info_list_.at(n_item).shelf_type != ShelfType::CONTAINER) {
		AfxMessageBox(_T("Chương trình không hỗ trợ chuyển sang mặt hàng đối với danh mục này"));
		return;
	}
	if(!dlg_production_.GetSafeHwnd()) {
		dlg_production_.Create(this);
	}
	if(dlg_shelf_.IsWindowVisible())
		dlg_shelf_.ShowWindow(SW_HIDE);
	if(dlg_production_.IsWindowVisible()) {
		return;
	} else {
		dlg_production_.ShowWindow(SW_SHOW);
	}
	// convert shefl to commodity
	CadInfo container;
	container.count = 1;
	container.floor = 0;
	container.weight = 0;
	container.height = shelf_info_list_.at(n_item).height;
	container.lenght = shelf_info_list_.at(n_item).longs;
	container.width = shelf_info_list_.at(n_item).width;
	container.name_production = shelf_info_list_.at(n_item).name_project;
	container.type_commodity = TypeCommodity::CO_CONTAINER;
	dlg_production_.SetData(container);
	dlg_production_.SetPositionListViewCtrl(- 2);

}

void FormBar::DelItemFromListShelf() {
	if(index_shelf_ < 0 || index_shelf_ >= shelf_info_list_.size()) {
		return;
	}
	shelf_info_list_.erase(shelf_info_list_.begin() + index_shelf_);
	SetDataForListShelf();
	index_shelf_ = - 1;
}

void FormBar::DelItemFromListProduction() {
	if(index_product_ < 0 || index_product_ >= cad_info_.size()) {
		return;
	}
	cad_info_.erase(cad_info_.begin() + index_product_);
	SetDataForListProduct();
	index_product_ = - 1;
}

void FormBar::SaveListShelf() {

}

void FormBar::LoadListShelf() {

}

bool FormBar::SaveListCommodity(CString &name_path, std::vector<CadInfo> &data_commodity) {


	std::string str_file = CStringA(name_path);

	xmlTextWriterPtr writer = xmlNewTextWriterFilename(str_file.c_str(), 0);
	if (NULL == writer) {
		return false;
	}

	// Set indentation
	int rc = xmlTextWriterSetIndent(writer, 1);
	if (rc < 0) return false;

	const char* kXmlDefaultVersion = NULL;
	const char* kUtf8Encoding = "UTF-8";
	const char* kDefaultStandalon = NULL;
	rc = xmlTextWriterStartDocument(writer,
		kXmlDefaultVersion,
		kUtf8Encoding,
		kDefaultStandalon);
	if (rc < 0) {
		return false;
	}

	rc = xmlTextWriterStartElement(writer, BAD_CAST "root");
	if (rc < 0) 
		return false;

	for (int i = 0; i < data_commodity.size(); i++) {
		char* chstr = Base::CStringToChar(data_commodity.at(i).name_production/*str*/);
		rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST"ten","%s", (const xmlChar*)chstr);
		if (rc < 0) {
			return false;
		}
		rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST"loai","%d", data_commodity.at(i).type_commodity);
		if (rc < 0) {
			return false;
		}
		rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST"dai", "%f", data_commodity.at(i).lenght);
		if (rc < 0) return false;
		rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST"rong", "%f", data_commodity.at(i).width);
		if (rc < 0) return false;
		rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST"cao", "%f", data_commodity.at(i).height);
		if (rc < 0) return false;
		rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST"trong_luong", "%f", data_commodity.at(i).weight);
		if (rc < 0) return false;
		rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST"so_luong", "%d", data_commodity.at(i).count);
		if (rc < 0) return false;
		rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST"tang", "%d", data_commodity.at(i).floor);
		if (rc < 0) return false;

	}
	rc = xmlTextWriterEndDocument(writer);
	if (rc < 0) 
		return false;
	// End document.
	xmlFreeTextWriter(writer);
	return true;
}

void FormBar::LoadListCommodity(CString &name_path, std::vector<CadInfo> &data_commodity) {
	xmlDocPtr doc;
	xmlNodePtr cur;
	std::string file_name = CStringA(name_path);

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

	std::vector<CadInfo> cad_info;


	cur = cur->children;
	CadInfo production;
	while (cur != NULL) {
		if (!xmlStrcmp(cur->name, (const xmlChar*)"ten")) {
			std::wstring name =  XmlUtls::GetStringContent(doc, cur);
			CString str = name.c_str();
			production.name_production = str;
		}
		if (!xmlStrcmp(cur->name, (const xmlChar*)"loai")) {
			int type_co =  XmlUtls::GetIntContent(doc, cur);

			if(type_co == 0) {
				production.type_commodity = TypeCommodity::COMMODITY;
			} else {
				production.type_commodity = TypeCommodity::CO_CONTAINER;
			}
		}
		if (!xmlStrcmp(cur->name, (const xmlChar*)"dai")) {
			double lenght =  XmlUtls::GetDoubleContent(doc, cur);
			production.lenght = lenght;
		}

		if (!xmlStrcmp(cur->name, (const xmlChar*)"rong")) {
			double width =  XmlUtls::GetDoubleContent(doc, cur);
			production.width = width;
		}

		if (!xmlStrcmp(cur->name, (const xmlChar*)"cao")) {
			double height =  XmlUtls::GetDoubleContent(doc, cur);
			production.height = height;
		}
		if (!xmlStrcmp(cur->name, (const xmlChar*)"trong_luong")) {
			double weight =  XmlUtls::GetDoubleContent(doc, cur);
			production.weight = weight;
		}

		if (!xmlStrcmp(cur->name, (const xmlChar*)"so_luong")) {
			double count =  XmlUtls::GetDoubleContent(doc, cur);
			production.count = count;
		}

		if (!xmlStrcmp(cur->name, (const xmlChar*)"tang")) {
			double floor =  XmlUtls::GetDoubleContent(doc, cur);
			production.floor = floor;
			data_commodity.push_back(production);
		}
		cur = cur->next;
	}
	xmlFreeDoc(doc);
}

void FormBar::ShowDialogStore() {
	if(!dlg_store_.GetSafeHwnd()) {
		dlg_store_.Create(this);
	}
	dlg_store_.SetData(data_commodity_);
	dlg_store_.ShowWindow(SW_SHOW);
}

void FormBar::SetCommodityToData(CadInfo & cad_info, int pos_item) {
	if(!IsSameACommodity(data_commodity_, cad_info)) {
		data_commodity_.push_back(cad_info);
	} else {
	//	AfxMessageBox(_T("Mã sản phẩm vừa tạo trùng với 1 sản phẩm đã có trong dữ liệu,\n vì vậy nó sẽ không được lưu lại trong dữ liệu lưu chữ"));
	}
	SetProductionList(cad_info, pos_item);
}

void FormBar::SaveCommodityToData() {
	CString str_path = Base::GetPathModule() + _T("\\data\\Commodity.prd");
	SaveListCommodity(str_path, data_commodity_);
}

void FormBar::LoadDataFromFileData() {
	CString str_path = Base::GetPathModule() + _T("\\data\\Commodity.prd");
	data_commodity_.clear();
	LoadListCommodity(str_path, data_commodity_);
}

bool FormBar::IsSameACommodity(std::vector<CadInfo> &data_commodity, CadInfo &commodity) {
	for(int i = 0; i < data_commodity.size(); i++) {
		if(data_commodity.at(i).name_production == commodity.name_production) {
			if(data_commodity.at(i).type_commodity == commodity.type_commodity) {
				return true;
			}
		}
	}
	return false;
}