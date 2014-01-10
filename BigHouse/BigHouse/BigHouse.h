
// BigHouse.h : main header file for the BigHouse application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "Triangle3D.h"
#include <vector>
#include <afxtempl.h>
#include "Struct.h"
// BigHouseApp:
// See BigHouse.cpp for the implementation of this class
//

typedef float Point[3];
typedef Point Vector;

typedef struct _Triangle {
 Point Vertex[3];
} Triangle;

	struct CadPosOnFloor {
		int x_pos;
		int y_pos;
	};

struct FloorSize{
	float x_size;
	float y_size;
	float z_size;
};

struct Floor {
	Vector3D	color;
	double height_floor;
	CadPosOnFloor	cad_pos;
	FloorSize			floor_size;
	Vector3D		cad_origin_point;
};



class BigHouseApp : public CWinAppEx
{
public:
	BigHouseApp();
	~BigHouseApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
  std::vector<std::pair<FloorSize, std::vector<Triangle3D*>>> GetCadBoy();
	void ConvertCommodityZise(CadInfo &cad_info,
														std::pair<Floor, std::vector<Triangle3D*>> &body);
private:
	void GetRectBody(std::vector<Triangle3D*>& cad_triangle_list, CadInfo & cad_info);

	// Passion88 TODO
	std::vector<Triangle3D*> production_triangle_list_;
	std::vector<CString> opened_file_list_;
	std::vector<std::vector<Triangle3D*>> production_list_;

	std::vector<std::pair<FloorSize, std::vector<Triangle3D*>>> list_cad_boydy_;


// Implementation
	BOOL  m_bHiColorIcons;
public:
	COLORREF option_color_glback;
public:
  void FreePoint();
  void OnFileOpen();
	void OnFileSave();
	void LoadFileCad(CadInfo &cad_info);
	bool IsOpenedFile(CString str, int &index);
	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();
  virtual BOOL LoadState(LPCTSTR lpszSectionName = NULL, CFrameImpl *pFrameImpl = NULL);
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern BigHouseApp theApp;
