
// BigHouse.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "BigHouse.h"
#include "MainFrm.h"

#include "BigHouseDoc.h"
#include "BigHouseView.h"
#include "base.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// BigHouseApp

BEGIN_MESSAGE_MAP(BigHouseApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &BigHouseApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
 // ON_COMMAND(ID_FILE_OPEN, &BigHouseApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// BigHouseApp construction

BigHouseApp::BigHouseApp()
{
	m_bHiColorIcons = TRUE;
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// If the application is built using Common Language Runtime support (/clr):
	//     1) This additional setting is needed for Restart Manager support to work properly.
	//     2) In your project, you must add a reference to System.Windows.Forms in order to build.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("BigHouse.AppID.NoVersion"));

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

BigHouseApp::~BigHouseApp() {
	//for(int i = 0; i <cad_body_.size(); i ++) {
	//	delete cad_body_.at(i);
	//}
}

// The one and only BigHouseApp object

BigHouseApp theApp;


// BigHouseApp initialization

BOOL BigHouseApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// AfxInitRichEdit2() is required to use RichEdit control	
	// AfxInitRichEdit2();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(BigHouseDoc),
		RUNTIME_CLASS(MainFrame),       // main SDI frame window
		RUNTIME_CLASS(BigHouseView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);



	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand

  //LoadFileCad(); 
	return TRUE;
}

int BigHouseApp::ExitInstance()
{
	//TODO: handle additional resources you may have added
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

// BigHouseApp message handlers


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// App command to run the dialog
void BigHouseApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// BigHouseApp customization load/save methods

void BigHouseApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void BigHouseApp::LoadCustomState()
{
}

void BigHouseApp::SaveCustomState()
{
}



BOOL BigHouseApp::LoadState(LPCTSTR lpszSectionName, CFrameImpl *pFrameImpl) {
  return TRUE;
}
// BigHouseApp message handlers


// handle reading file stl


void BigHouseApp::LoadFileCad (CString strs) {
  unsigned int n_size = 0;
  char str[MAX_PATH];
  FILE *pFile = NULL;
  CString str_file = Base::GetPathModule() + _T("\\cad\\") + strs;
  
	// convert CString to char*
  char file_name[MAX_PATH];
  n_size = str_file .GetLength();
  memset(file_name, 0, n_size + 1);
  wcstombs(file_name, str_file, n_size);


	pFile = fopen(file_name, "r");
   if (pFile == NULL) {
     AfxMessageBox(L"Sản phẩm không tồn tại");
		 return;
	 }

	int index = -1;
  bool ret = IsOpenedFile(strs, index);
	if (ret == true) {
   	GetRectBody(production_list_[index]);	
	} else {
		// if production is not opened
		// Save name production into list
		production_triangle_list_.clear(); // clear data old production 
		opened_file_list_.push_back(strs);

		// read data form stl file
		pFile = fopen(file_name, "r");

		fgets(str, MAX_PATH, pFile);

		//std::vector<Triangle3D*> cad;

		while (!feof(pFile)) {
			Triangle3D * tr_ = new Triangle3D();
			fscanf(pFile, "%s%s%f%f%f", str, str,
						 &tr_->normal.v[0],
							&tr_->normal.v[1],
							&tr_->normal.v[2]);
			fgets(str, MAX_PATH, pFile);
			fgets(str, MAX_PATH, pFile);
			fscanf(pFile, "%s%f%f%f", str, &tr_->m_v0.v[0], &tr_ ->m_v0.v[1], &tr_->m_v0.v[2]);
			fscanf(pFile, "%s%f%f%f", str, &tr_->m_v1.v[0], &tr_ ->m_v1.v[1], &tr_->m_v1.v[2]);
			fscanf(pFile, "%s%f%f%f", str, &tr_->m_v2.v[0], &tr_ ->m_v2.v[1], &tr_ ->m_v2.v[2]);
			production_triangle_list_.push_back(tr_);
			fgets(str, MAX_PATH, pFile);
			fgets(str, MAX_PATH, pFile);
			fgets(str, MAX_PATH, pFile);
		}
		int size = production_triangle_list_.size();
		production_triangle_list_.erase(production_triangle_list_.begin() + size - 1);
		GetRectBody(production_triangle_list_);
		
		// save number of project
		production_list_.push_back(production_triangle_list_);
	}

	fclose(pFile);
	InvalidateRect(NULL, NULL, FALSE);
}

std::vector<std::pair<RectBody, std::vector<Triangle3D*>>> BigHouseApp::GetCadBoy() {
	return list_cad_boydy_;
}

void BigHouseApp::GetRectBody(std::vector<Triangle3D*> &cad_body) {
	if(cad_body.empty()) {
		return ;
	}
	Vector3D bbmin(cad_body.at(0)->m_v0.v);
	Vector3D bbmax(cad_body.at(0)->m_v1.v);

	for(int i = 0; i < cad_body.size(); i ++) {
		for(int j = 0; j < 2; j ++) {
			if(cad_body.at(i)->m_v0.v[j] < bbmin.v[j]) {
			  bbmin.v[j] = cad_body.at(i)->m_v0.v[j];
			}
			if(cad_body.at(i)->m_v1.v[j] < bbmin.v[j]) {
			  bbmin.v[j] = cad_body.at(i)->m_v1.v[j];
			}
			if(cad_body.at(i)->m_v2.v[j] < bbmin.v[j]) {
				bbmin.v[j] = cad_body.at(i)->m_v2.v[j];
			}


			if(cad_body.at(i)->m_v0.v[j] > bbmax.v[j]) {
				bbmax.v[j] = cad_body.at(i)->m_v0.v[j];
			}
			if(cad_body.at(i)->m_v1.v[j] > bbmax.v[j]) {
				bbmax.v[j] = cad_body.at(i)->m_v1.v[j];
			}
			if(cad_body.at(i)->m_v2.v[j] > bbmax.v[j]) {
				bbmax.v[j] = cad_body.at(i)->m_v2.v[j];
			}
		}
	}
	RectBody rect;
	//bbmax.v[2] = 0;
	//bbmin.v[2] = 0;
	rect.x = bbmax.v[0] - bbmin.v[0];
	rect.y = bbmax.v[1] - bbmin.v[1];
	std::pair<Floor, std::vector<Triangle3D*>> temp;
	temp.first.s_b = rect;
	temp.second = cad_body;
	//list_cad_boydy_.push_back(temp);
	MainFrame *pMainFrame = (MainFrame*)this->m_pMainWnd;
  BigHouseView*pView = reinterpret_cast<BigHouseView*>(pMainFrame->splitter_.GetPane(0, 1));
	pView->SetCadToView(temp);

}

void BigHouseApp::FreePoint() {
	if (!production_triangle_list_.empty()) {
		for (int i = 0; i < production_triangle_list_.size(); i++) {
	   delete production_triangle_list_.at(i);
		}
		production_triangle_list_.clear();
	}

	if (!opened_file_list_.empty()) {
	  opened_file_list_.clear();
	}

	if (!production_list_.empty()) {
	  production_list_.clear();
	}
}

bool BigHouseApp::IsOpenedFile( CString str, int& index ) {
	bool ret = false;
	if ( opened_file_list_.empty() == false) {
		for (size_t i = 0; i < opened_file_list_.size(); ++i) {
			if (opened_file_list_[i] == str) {
				ret = true;
				index = i;
				break;
			}
		}
	}
	return ret;
}

