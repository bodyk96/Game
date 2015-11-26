// MainFrm.cpp : implmentation of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "aboutdlg.h"
#include "GameOfLife(1)View.h"
#include "Set_Size.h"
#include "MainFrm.h"
//#include "SetSizeDialog.h"

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	if(CFrameWindowImpl<CMainFrame>::PreTranslateMessage(pMsg))
		return TRUE;

	return m_view.PreTranslateMessage(pMsg);
}

BOOL CMainFrame::OnIdle()
{
	UIUpdateToolBar();
	return FALSE;
}

LRESULT CMainFrame::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// create command bar window
	HWND hWndCmdBar = m_CmdBar.Create(m_hWnd, rcDefault, NULL, ATL_SIMPLE_CMDBAR_PANE_STYLE);
	// attach menu
	m_CmdBar.AttachMenu(GetMenu());
	// load command bar images
	m_CmdBar.LoadImages(IDR_MAINFRAME);
	// remove old menu
	SetMenu(NULL);

	HWND hWndToolBar = CreateSimpleToolBarCtrl(m_hWnd, IDR_MAINFRAME, FALSE, ATL_SIMPLE_TOOLBAR_PANE_STYLE);
	
	CreateSimpleReBar(ATL_SIMPLE_REBAR_NOBORDER_STYLE);
	AddSimpleReBarBand(hWndCmdBar);
	AddSimpleReBarBand(hWndToolBar, NULL, TRUE);

	CreateSimpleStatusBar();

	m_hWndClient = m_view.Create(m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, WS_EX_CLIENTEDGE);

	UIAddToolBar(hWndToolBar);
	UISetCheck(ID_VIEW_TOOLBAR, 1);
	UISetCheck(ID_VIEW_STATUS_BAR, 1);

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);
	bool run = false;
	if (!run)
		m_view.CreateVector();
	run = true;
	return 0;
}

LRESULT CMainFrame::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);

	bHandled = FALSE;
	return 1;
}

LRESULT CMainFrame::OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	PostMessage(WM_CLOSE);
	return 0;
}

LRESULT CMainFrame::OnFileNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: add code to initialize document
	
	return 0;
}

LRESULT CMainFrame::OnViewToolBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	static BOOL bVisible = TRUE;	// initially visible
	bVisible = !bVisible;
	CReBarCtrl rebar = m_hWndToolBar;
	int nBandIndex = rebar.IdToIndex(ATL_IDW_BAND_FIRST + 1);	// toolbar is 2nd added band
	rebar.ShowBand(nBandIndex, bVisible);
	UISetCheck(ID_VIEW_TOOLBAR, bVisible);
	UpdateLayout();
	return 0;
}

LRESULT CMainFrame::OnViewStatusBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	BOOL bVisible = !::IsWindowVisible(m_hWndStatusBar);
	::ShowWindow(m_hWndStatusBar, bVisible ? SW_SHOWNOACTIVATE : SW_HIDE);
	UISetCheck(ID_VIEW_STATUS_BAR, bVisible);
	UpdateLayout();
	return 0;
}

LRESULT CMainFrame::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CAboutDlg dlg;
	dlg.DoModal();
	return 0;
}


LRESULT CMainFrame::OnFileRun(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: добавьте свой код обработчика команд
	if (!SetTimer(IDT_TIMER1, m_view.seconds, NULL))
		m_view.Play(NULL, NULL, NULL, m_view.ab);
	return 0;
}

LRESULT CMainFrame::OnFileFastRun(WORD, WORD, HWND, BOOL &)
{
	m_view.isFastRun = true;
	if (!SetTimer(IDT_TIMER1, m_view.seconds, NULL))
		m_view.Play(NULL, NULL, NULL, m_view.ab);
	return 0;
}


LRESULT CMainFrame::OnGameClear(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: добавьте свой код обработчика команд
	m_view.GameClear();
	KillTimer(IDT_TIMER1);
	return 0;
}


LRESULT CMainFrame::OnGameStop(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: добавьте свой код обработчика команд
	m_view.stop = true;
	KillTimer(IDT_TIMER1);
	return 0;
}

LRESULT CMainFrame::ExitProgram(WORD, WORD, HWND, BOOL &)
{
	return 0;
}


LRESULT CMainFrame::OnGameSetsiz(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: добавьте свой код обработчика команд
	CSetSize dlg;
	dlg.DoModal();
	SetDlgItemText(IDC_SPEED_GAME, L"300");
	SetDlgItemText(IDC_COL, L"19");
	SetDlgItemText(IDC_ROW, L"40");
	SetDlgItemText(IDC_SET_RECT_SIZE, L"20");
	m_view.seconds = dlg.newSeconds;
	m_view.ChangeFieldParams(dlg.newCol, dlg.newRow, dlg.newRectSize);
	return 0;
}

LRESULT CMainFrame::OnStep(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: добавьте свой код обработчика команд
	m_view.Play(NULL, NULL, NULL, m_view.ab);
	return 0;
}

void CMainFrame::AllDefault()
{
	m_view.Glider = false;
	m_view.Small_Exploder = false;
	m_view.Exploder = false;
	m_view.Ten_Cell_Row = false;
	m_view.Lightweight_Spaceship = false;
	m_view.Tumbler = false;
	m_view.Gosper_Glider_Gun = false;
}

LRESULT CMainFrame::OnFileSave(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: добавьте свой код обработчика команд
	m_view.Save();
	return 0;
}


LRESULT CMainFrame::OnFileOpen(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: добавьте свой код обработчика команд
	m_view.Open();
	return 0;
}


LRESULT CMainFrame::OnFiguresGlider(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: добавьте свой код обработчика команд
	AllDefault();
	m_view.Glider = true;
	return 0;
}


LRESULT CMainFrame::OnFiguresExploder(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: добавьте свой код обработчика команд
	AllDefault();
	m_view.Exploder = true;
	return 0;
}


LRESULT CMainFrame::OnFiguresTen(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: добавьте свой код обработчика команд
	AllDefault();
	m_view.Ten_Cell_Row = true;
	return 0;
}


LRESULT CMainFrame::OnFiguresLighweight(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: добавьте свой код обработчика команд
	AllDefault();
	m_view.Lightweight_Spaceship = true;
	return 0;
}


LRESULT CMainFrame::OnFiguresTumbler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: добавьте свой код обработчика команд
	AllDefault();
	m_view.Tumbler = true;
	return 0;
}


LRESULT CMainFrame::OnFiguresGosper(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: добавьте свой код обработчика команд
	AllDefault();
	m_view.Gosper_Glider_Gun = true;
	return 0;
}


LRESULT CMainFrame::OnFiguresDefault32820(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: добавьте свой код обработчика команд
	AllDefault();
	return 0;
}


LRESULT CMainFrame::OnFiguresSmall(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: добавьте свой код обработчика команд
	AllDefault();
	m_view.Small_Exploder = true;
	return 0;
}
