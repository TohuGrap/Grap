#pragma once
#include "resource.h"
#include "afxwin.h"
#include "Struct.h"
#include "afxcmn.h"
#include "CustomListView.h"
#include <vector>

class BigHouseView;
class BigHouseDoc;

// FormBar form view

class FormBar : public CFormView
{
	DECLARE_DYNCREATE(FormBar)

public:
	FormBar();           // protected constructor used by dynamic creation
	virtual ~FormBar();

public:
	enum { IDD = IDD_FORMBAR };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

public:
  BigHouseView* GetBigHouseView();
	void	InitListViewShelf();
	void InitListViewProduct();
	void SetShelfInfoList(ShelfInfo shelf_info, bool &is_exist);
	void SetProductionList(CadInfo & cad_info, bool &is_exist);
	void SetDataForListShelf(UINT number_of_shelf); 
	void SetDataForListProduct();
	void HandleListViewShelf(int item);
	void HandleListViewProduct(int item);
	void DisableLoadProduct();
	void LoadSimShelfFile(CString& path_file);
	bool SaveSimShelfFile(CString& path_file);
	void ImportFile();
	void ExportFile();
	void RemoveListShelf();
	void RemoveListCommodity();

	void ImportCad();
	void LoadCadFile(CString& str);
	bool SaveCadFile(CString& str);
	void ExportCad();
  enum ObjectShape {
    OBJ_TABLE = 0,
    OBJ_DESK = 1,
  };

	enum ShelfType {
		SIMPLE_SHELF = 0,
		DOUBLE_SHELF = 1,
		CIRCLE_SHELF = 2,
		CONTAINER = 3
	};

protected:
  virtual void OnPaint();
  virtual void OnInitialUpdate();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  afx_msg void OnBnNext();
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg void OnShowCoordinate();
	afx_msg void ShowSizeRoom();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint pt);
	afx_msg void OnBnShelfSelected();
	afx_msg void OnBnProductionSelected();
	DECLARE_MESSAGE_MAP()
private:
	int index_shelf_;
	int index_product_;
	//std::vector<CString> production_list_;
	std::vector<CadInfo> cad_info_;
	std::vector<ShelfInfo> shelf_info_list_;
	CustomListView list_view_product_;
	CustomListView list_view_shelf_;
};


