#pragma once
#include "resource.h"
#include "afxwin.h"
#include "Struct.h"
#include "afxcmn.h"
#include "CustomListView.h"
#include <vector>
#include "EditProduction.h"
#include "DlgProduction.h"
#include "DlgSettingShelf.h"
#include "DlgLiveStock.h"

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
	void SetShelfInfoList(ShelfInfo &shelf_info, int pos_item);
	void SetProductionList(CadInfo & cad_info, int pos_item);
	void SetDataForListShelf(); 
	void SetDataForListProduct();
	//void HandleListViewShelf(int item);
	//void HandleListViewProduct(int item);
	void DisableLoadProduct();
	void LoadSimShelfFile(CString& path_file);
	bool SaveSimShelfFile(CString& path_file);
	void ImportFile();
	void ExportFile();
	void RemoveListShelf();
	void RemoveListCommodity();
	void OnClickListProduction(NMHDR* pNMHDR, LRESULT* pResult);// huu
	void OnDoubleClickListProduction(NMHDR* pNMHDR, LRESULT* pResult);// huu
	void OnClickListShelf(NMHDR* pNMHDR, LRESULT* pResult);// huu
	void OnDoubleClickListShelf(NMHDR* pNMHDR, LRESULT* pResult);// huu
	void OnRightClickListShelf(NMHDR* pNMHDR, LRESULT* pResult);// huu
	void ImportCad();
	void LoadCadFile(CString& str);
	bool SaveCadFile(CString& str);
	void ExportCad();
	void SetupProduction();
	void SetCommodityToData(CadInfo & cad_info, int pos_item);
	void SetupShelf();
  enum ObjectShape {
    OBJ_TABLE = 0,
    OBJ_DESK = 1,
  };

	void SaveListShelf();
	void LoadListShelf();

	bool SaveListCommodity(CString &name_path, std::vector<CadInfo> &data_commodity);
	void LoadListCommodity(CString &name_path, std::vector<CadInfo> &data_commodity);
	void ShowDialogStore();
	void SaveCommodityToData();
	void LoadDataFromFileData();
	bool IsSameACommodity(std::vector<CadInfo> &data_commodity, CadInfo &commodity);
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
	afx_msg LRESULT OnMyMessage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDestroy();
	void DelItemFromListShelf();
	void DelItemFromListProduction();

	DECLARE_MESSAGE_MAP()
private:
	DlgProduction dlg_production_;
	EditProduction edit_production_;
	DlgSettingShelf dlg_shelf_;
	DlgLiveStock dlg_store_;
	///
	int index_shelf_;
	int index_product_;

	std::vector<CadInfo> data_commodity_;
	std::vector<ShelfInfo*> data_shelf_;

	std::vector<CadInfo> cad_info_;
	std::vector<ShelfInfo> shelf_info_list_;
	CustomListView list_view_product_;
	CustomListView list_view_shelf_;
};


