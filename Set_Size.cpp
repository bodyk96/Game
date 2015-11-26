// aboutdlg.cpp : implementation of the CAboutDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "Set_Size.h"
#include "MainFrm.h"

LRESULT CSetSize::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CenterWindow(GetParent());
	return TRUE;
}

LRESULT CSetSize::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}

LRESULT CSetSize::OnBtnClickSave(WORD, WORD wID, HWND, BOOL &)
{
	newCol = GetDlgItemInt(IDC_COL);
	newRow = GetDlgItemInt(IDC_ROW);
	newRectSize = GetDlgItemInt(IDC_SET_RECT_SIZE);
	newSeconds = GetDlgItemInt(IDC_SPEED_GAME);
	isChangeSize = true;
	EndDialog(wID);
	return 0;
}

LRESULT CSetSize::OnBtnClickCancel(WORD, WORD wID, HWND, BOOL &)
{

	return 0;
}


LRESULT CSetSize::OnBnClickedOk(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: добавьте свой код обработчика уведомлений

	return 0;
}
