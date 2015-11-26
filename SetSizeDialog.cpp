 aboutdlg.cpp : implementation of the CSetSizeDlg class

///////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "GameOfLife(1).h"
#include "GameOfLife(1)View.h"
#include "SetSizeDialog.h"
#include "MainFrm.h"

LRESULT CSetSizeDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CenterWindow(GetParent());
	return TRUE;
}

LRESULT CSetSizeDlg::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	
	return 0;
}

