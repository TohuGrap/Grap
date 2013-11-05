
// BigHouse.h : main header file for the BigHouse application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// BigHouseApp:
// See BigHouse.cpp for the implementation of this class
//

typedef float Point[3];
typedef Point Vector;

typedef struct _Triangle {
 Point Vertex[3];
} Triangle;

class BigHouseApp : public CWinAppEx
{
public:
	BigHouseApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

private:
  long number_of_point_;
  BOOL allow_draw_data_;
  Triangle *gl_point_;
  Vector *normal_vector_;

// Implementation
	BOOL  m_bHiColorIcons;
public:
  void FreePoint();
  Vector* GetNormalVector () {return normal_vector_;} 
  long GetNumberOfPoint() { return number_of_point_;}
  Triangle* GetTrianglePoint() {return gl_point_;}
  void OnFileOpen();
  CString GetModulePath();
  void LoadFileCad();
	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern BigHouseApp theApp;
