
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
  ON_COMMAND(ID_FILE_OPEN, &BigHouseApp::OnFileOpen)
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

// BigHouseApp message handlers


// handle reading file stl
void BigHouseApp::OnFileOpen() {
  UINT n_size = 0;
  char str[MAX_PATH];
  FILE *pFile = NULL;
  Point *value_stl = NULL;
  Point *asscii_value_stl = NULL;
  CFileDialog Dlg(TRUE);
  CString file_name_stl = L"";
  // Get file name 
  if (IDOK == Dlg.DoModal()) {
    file_name_stl = Dlg.GetPathName();
  }

}

void BigHouseApp::LoadFileCad (CString strs) {
  UINT n_size = 0;
  char str[MAX_PATH];
  FILE *pFile = NULL;
  Point *value_stl = NULL;
  Point *asscii_value_stl = NULL;

  CString str_file = Base::GetPathModule() + _T("\\cad\\") + strs;
  // convert CString to char*
  char file_name[MAX_PATH];
  n_size = str_file .GetLength();
  memset(file_name, 0, n_size + 1);
  wcstombs(file_name, str_file, n_size);

  pFile = fopen(file_name, "r");
  if (pFile == NULL)
    return;
  fclose(pFile);

    pFile = fopen(file_name, "r");
    if (pFile == NULL)
      return;
    while(!feof(pFile)) {
      fscanf(pFile, "%s", str);
      if (strcmp(str, "vertex") == 0 || strcmp(str, "VERTEX") == 0) {
        number_of_point_ += 1;
      }
    }
    fclose(pFile);
    // check number_of_point
    if (number_of_point_ == 0) {
      ::MessageBox(NULL, L"Can not read STL file", L"Inform", MB_OK | MB_ICONWARNING);
      return;
    }

    static long count_normal_vector = 0;
    if (gl_point_ != NULL) {
      delete [] gl_point_;
      gl_point_ = NULL;
    }
    if (normal_vector_ != NULL) {
      delete [] normal_vector_;
      count_normal_vector = 0;
      normal_vector_ = NULL;
    }
    gl_point_ = new Triangle[number_of_point_];
    value_stl = new Point[number_of_point_];
    normal_vector_ = new Vector[number_of_point_ /3];
    // read data form stl file
    pFile = fopen(file_name, "r");
    fgets(str, MAX_PATH, pFile);
    while(!feof(pFile)) {
      fscanf(pFile, "%s%s%f%f%f", str, str,
             &normal_vector_[count_normal_vector][0],
             &normal_vector_[count_normal_vector][1],
             &normal_vector_[count_normal_vector][2]);
      count_normal_vector = 1;
      fgets(str, MAX_PATH, pFile);
      fgets(str, MAX_PATH, pFile);
      for (long i = 0; i< number_of_point_; i++) {
        fscanf(pFile,"%s%f%f%f",str, &value_stl[i][0],
              &value_stl[i][1],
              &value_stl[i][2]);
        gl_point_->Vertex[i][0] = value_stl[i][0];
        gl_point_->Vertex[i][1] = value_stl[i][1];
			  gl_point_->Vertex[i][2] = value_stl[i][2];
			  if((i+1)%3 == 0) {
				  fgets(str,100,pFile);
				  fgets(str,100,pFile);
				  fgets(str,100,pFile);
				  fscanf(pFile,"%s%s%f%f%f", str, str,
                 &normal_vector_[count_normal_vector][0],
                 &normal_vector_[count_normal_vector][1],
                 &normal_vector_[count_normal_vector][2]);
          count_normal_vector ++;
				  fgets(str,100,pFile);
				  fgets(str,100,pFile);
        }
      }
    }
    allow_draw_data_ = TRUE ;
	  InvalidateRect(NULL,NULL,FALSE);
    if (value_stl != NULL) {
      delete [] value_stl;
      value_stl = NULL;
    }

	  fclose(pFile);
}

void BigHouseApp::FreePoint() {
  if (gl_point_ != NULL) {
    delete [] gl_point_;
    gl_point_ = NULL;
  }
  if (normal_vector_ != NULL) {
    delete [] normal_vector_;
    normal_vector_ = NULL;
  }
}

