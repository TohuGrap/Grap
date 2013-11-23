
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

struct RectBody{
	Vector3D bbmin;
	Vector3D bbmax;
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

  long number_of_point_;
  BOOL allow_draw_data_;
  Triangle *gl_point_;
  Vector *normal_vector_;
	std::vector<Triangle3D*> cad_body_;
	std::vector<std::pair<RectBody, std::vector<Triangle3D*>>> list_cad_boydy_;

// Implementation
	BOOL  m_bHiColorIcons;
public:
  void FreePoint();
  Vector* GetNormalVector () {return normal_vector_;} 
  long GetNumberOfPoint() { return number_of_point_;}
  Triangle* GetTrianglePoint() {return gl_point_;}
  void OnFileOpen();
  void LoadFileCad(CString str_file);
	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern BigHouseApp theApp;
