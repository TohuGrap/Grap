#pragma once
#include "stdafx.h"
#include "Struct.h"
// DlgProduction dialog

class DlgProduction : public CDialogEx
{
	DECLARE_DYNAMIC(DlgProduction)

public:
	DlgProduction(CWnd* pParent = NULL);   // standard constructor
	virtual ~DlgProduction();

// Dialog Data
	enum { IDD = IDD_DLG_PRODUCTION };
	CString GetCurrentStrProduction() {return str_current_production_;}
	void NewProduction();
	void NewImage();
	void ClearProduction();
	void UpdateProduct();
	void HandleBitmap(CString str_name);
	void LoadProductionBkgn();
	CString GetProductName() {return str_current_production_;}
	void GetCadInfor(CadInfo & cad_info) const;
	bool CheckProductionPic(std::string str_name);
	std::string GetPicName(std::string &str_name);
protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnLBSelChange();
	afx_msg void OnEditCadLenght();
	afx_msg void OnEditCadWidth();
	afx_msg void OnEditCadHeight();
	//afx_msg void OnCheckProportion();
	afx_msg void OnEditCountProduction();
	afx_msg void OnEditWeighProduction();
	afx_msg void OnEditSelectionFloor();
	afx_msg void OnOk();
	DECLARE_MESSAGE_MAP()

protected:
	CListBox list_box_ctrl_;
	CString str_current_production_;
	CStatic bitmap_image_ctrl;
  CBitmap cbitmap_;
	//CString product_cad_path_;
	//CString product_bitmap_path_;
	CString product_pic_path_;
	CString module_path_;
private:
	//CButton edit_cad_proportion_;
	//CString str_cad_proportion_;
	//CButton check_proportion_;
  CString str_cad_lenght_;
  CString str_cad_width_;
  CString str_cad_height_;
	CString str_commodity_weight_;
	CString str_selection_floor_;
	CString str_count_commodity_;

	CEdit edit_count_commodity_;
	CEdit edit_cad_lenght_;
  CEdit edit_cad_width_;
  CEdit edit_cad_height_;

	CEdit edit_commodity_weight_;
	CEdit edit_selection_floor_;
	//float lenght_;
	//float width_;
	//float height_;
	CadInfo cad_info_;
};
