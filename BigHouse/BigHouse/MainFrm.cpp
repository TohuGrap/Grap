
// MainFrm.cpp : implementation of the MainFrame class
//

#include "stdafx.h"
#include "BigHouse.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// MainFrame

IMPLEMENT_DYNCREATE(MainFrame, CFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(MainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &MainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &MainFrame::OnToolbarCreateNew)


	ON_COMMAND(ID_VIEW_TOP, &MainFrame::OnViewTop)
	ON_COMMAND(ID_VIEW_BOTTOM, &MainFrame::OnViewBottom)
	ON_COMMAND(ID_VIEW_LEFT, &MainFrame::OnViewLeft)
	ON_COMMAND(ID_VIEW_RIGHT, &MainFrame::OnViewRight)
	ON_COMMAND(ID_VIEW_FRONT, &MainFrame::OnViewFront)
	ON_COMMAND(ID_VIEW_BACK, &MainFrame::OnViewBack)
	ON_COMMAND(ID_VIEW_ISO, &MainFrame::OnViewIso)
	ON_COMMAND(ID_VIEW_COORDINATE, &MainFrame::OnShowCoordinate)
	ON_UPDATE_COMMAND_UI(ID_VIEW_COORDINATE, &MainFrame::OnUpdateShowCoordinate)
	ON_COMMAND(ID_VIEW_BAR, &MainFrame::ShowViewBar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_BAR, &MainFrame::OnUpdateShowViewBar)
	ON_COMMAND(ID_SETUP_ROOM, &MainFrame::SetupRoom)
	ON_COMMAND(ID_SETUP_SHELF, &MainFrame::SetupShelf)
	ON_COMMAND(ID_CLEAR_ALL_SHELF, &MainFrame::ClearAllShelf)
	ON_COMMAND(ID_SETUP_PRODUCT, &MainFrame::SetupProduction)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// MainFrame construction/destruction

MainFrame::MainFrame()
{
	is_full_screen_ = false;
}

MainFrame::~MainFrame()
{
}

int MainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;

	// set the visual manager used to draw all user interface elements
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
  /*
	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("Failed to create menubar\n");
		return -1;      // fail to create
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);
  m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
  DockPane(&m_wndMenuBar);
  */

	// prevent the menu bar from taking the focus on activation
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE |
																	CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS |
																	CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME_256))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	CString strToolBarName;
	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);
	m_wndToolBar.SetWindowText(strToolBarName);

	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);
	m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

	// Allow user-defined toolbars operations:
	InitUserToolbars(NULL, uiFirstUserToolBarId, uiLastUserToolBarId);

  // Create View toolbar
  if (!m_wndViewBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE |
																		CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS |
																		CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
    !m_wndViewBar.LoadToolBar(IDR_VIEW_TOOLBAR))
	{
		TRACE0("Failed to create View bar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: Delete these five lines if you don't want the toolbar and menubar to be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
  m_wndViewBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndViewBar);
  DockPaneLeftOf(&m_wndToolBar, &m_wndViewBar);

	// enable Visual Studio 2005 style docking window behavior
	CDockingManager::SetDockingMode(DT_SMART);
	// enable Visual Studio 2005 style docking window auto-hide behavior
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// Enable toolbar and docking window menu replacement
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// enable quick (Alt+drag) toolbar customization
	CMFCToolBar::EnableQuickCustomization();
  /*
	if (CMFCToolBar::GetUserImages() == NULL)
	{
		// load user-defined toolbar images
		if (m_UserImages.Load(_T(".\\UserImages.bmp")))
		{
			CMFCToolBar::SetUserImages(&m_UserImages);
		}
	} */

	// enable menu personalization (most-recently used commands)
	// TODO: define your own basic commands, ensuring that each pulldown menu has at least one basic command.
	CList<UINT, UINT> lstBasicCommands;

	lstBasicCommands.AddTail(ID_FILE_NEW);
	lstBasicCommands.AddTail(ID_FILE_OPEN);
	lstBasicCommands.AddTail(ID_FILE_SAVE);
	lstBasicCommands.AddTail(ID_FILE_PRINT);
	lstBasicCommands.AddTail(ID_APP_EXIT);
	lstBasicCommands.AddTail(ID_EDIT_CUT);
	lstBasicCommands.AddTail(ID_EDIT_PASTE);
	lstBasicCommands.AddTail(ID_EDIT_UNDO);
	lstBasicCommands.AddTail(ID_APP_ABOUT);
	lstBasicCommands.AddTail(ID_VIEW_STATUS_BAR);
	lstBasicCommands.AddTail(ID_VIEW_TOOLBAR);

	CMFCToolBar::SetBasicCommands(lstBasicCommands);

	// enable full screen mode
	EnableFullScreenMode (ID_VIEW_FULLSCREEN);
	EnableFullScreenMainMenu(FALSE);

	return 0;
}


BOOL MainFrame::OnCreateClient(LPCREATESTRUCT lpCreateStruct, CCreateContext *pContext) {
  if (!splitter_.CreateStatic(this, 1, 2)) {
    TRACE0("Failed to create Static windows");
    return FALSE;
  }

  if (!splitter_.CreateView(0, 0, RUNTIME_CLASS(FormBar),
                                CSize(250, 400),
                                pContext)) {
    TRACE0("Failed to view 0");
    return FALSE;
  }

  if (!splitter_.CreateView(0, 1, RUNTIME_CLASS(BigHouseView), CSize(800, 400), pContext)) {
    TRACE0("Failed to view 1");
    return FALSE;
  }

  big_house_view_ = reinterpret_cast<BigHouseView*>(splitter_.GetPane(0, 1));
  form_view_ = reinterpret_cast<FormBar*>(splitter_.GetPane(0, 0));
  big_house_view_->SetFormView(form_view_);

	// Choose BighouseView is activated when start
	splitter_.SetActivePane(0, 1);


  return TRUE;
}



BOOL MainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

// MainFrame diagnostics

#ifdef _DEBUG
void MainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void MainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// MainFrame message handlers

void MainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* scan menus */);
	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();
}

LRESULT MainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = CFrameWndEx::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}

BOOL MainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	// base class does the real work

	if (!CFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}


	// enable customization button for all user toolbars
	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	for (int i = 0; i < iMaxUserToolbars; i++)
	{
		CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
		if (pUserToolbar != NULL)
		{
			pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
		}
	}

	return TRUE;
}


void MainFrame::OnViewFullscreen() {
	if (is_full_screen_ == false) {
    is_full_screen_ = true;
		//form_view_->ShowWindow(SW_HIDE);
	} else {
		is_full_screen_ = false;
		//form_view_->ShowWindow(SW_SHOW);
		ShowAndDockToolbar();
	}
	ShowFullScreen();
}

void MainFrame::OnViewTop()
{
	 big_house_view_->OnViewTop();
}

void MainFrame::OnViewBottom()
{
	big_house_view_->OnViewBottom();
}

void MainFrame::OnViewLeft()
{
	big_house_view_->OnViewLeft();
}

void MainFrame::OnViewRight()
{
	big_house_view_->OnViewRight();
}

void MainFrame::OnViewFront()
{
	big_house_view_->OnViewFront();
}

void MainFrame::OnViewBack()
{
	big_house_view_->OnViewBack();
}

void MainFrame::OnViewIso()
{
	big_house_view_->OnViewIso();
}

void MainFrame::OnShowCoordinate()
{
	big_house_view_->OnShowCoordinate();
}

void MainFrame::OnUpdateShowCoordinate( CCmdUI * cmd )
{
	big_house_view_->OnUpdateShowCoordinate(cmd);
}

void MainFrame::SetupRoom() {
  big_house_view_->SetupRoom();
}

void MainFrame::SetupShelf() {
  big_house_view_->SetupShelf();
}

void MainFrame::ClearAllShelf() {
  big_house_view_->ClearAllShelf();
}

void MainFrame::SetupProduction() {
  big_house_view_->SetupProduction();
}



void MainFrame::ShowViewBar()
{
	ShowPane(&m_wndViewBar, !m_wndViewBar.IsVisible(), FALSE, TRUE);
}

void MainFrame::OnUpdateShowViewBar( CCmdUI* cmd )
{
	 cmd->SetCheck(m_wndViewBar.IsVisible() ? 1 : 0);
}

void MainFrame::HandleEscape() {
	is_full_screen_ = false;
  ShowAndDockToolbar();
}

void MainFrame::ShowAndDockToolbar() {
	ShowPane(&m_wndViewBar, TRUE, FALSE, TRUE);
	ShowPane(&m_wndToolBar, TRUE, FALSE, TRUE);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndViewBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndViewBar);
	DockPaneLeftOf(&m_wndToolBar, &m_wndViewBar);
}