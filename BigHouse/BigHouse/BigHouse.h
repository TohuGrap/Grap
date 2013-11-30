
// BigHouse.h : main header file for the BigHouse application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "Triangle3D.h"
#include <vector>

// BigHouseApp:
// See BigHouse.cpp for the implementation of this class
//

typedef float Point[3];
typedef Point Vector;

typedef struct _Triangle {
 Point Vertex[3];
} Triangle;

	struct Size_rec {
		int i;
		int j;
	};
struct RectBody{
	float x;
	float y;
};
	struct Floor {
		Vector3D color;
		int height_floor;
		Size_rec s_r;
		RectBody s_b;
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
std::vector<std::pair<RectBody, std::vector<Triangle3D*>>> GetCadBoy();
private:
	void GetRectBody(std::vector<Triangle3D*> &cad_body);

	std::vector<Triangle3D*> cad_body_;
	std::vector<std::pair<RectBody, std::vector<Triangle3D*>>> list_cad_boydy_;
	Triangle3D *tr_;
// Implementation
	BOOL  m_bHiColorIcons;
public:
  void FreePoint();
  void OnFileOpen();
  void LoadFileCad(CString str_file);
	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();
  virtual BOOL LoadState(LPCTSTR lpszSectionName = NULL, CFrameImpl *pFrameImpl = NULL);
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern BigHouseApp theApp;
