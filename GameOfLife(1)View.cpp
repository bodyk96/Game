// GameOfLife(1)View.cpp : implementation of the CGameOfLife1View class
//
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "resource.h"
#include "GameOfLife(1)View.h"
#include <thread>

#include "MainFrm.h"
//#include "SetSizeDialog.h"
#include <fstream>
#include <Windows.h>
//#include "Field.h"

BOOL CGameOfLife1View::PreTranslateMessage(MSG* pMsg)
{
	pMsg;
	return FALSE;
}
bool flag = true;

void CGameOfLife1View::DrawField(CPaintDC &dc)
{
	//Draw vertical lines of field
	for (int i = 0; i <= row - CurHScrollPos; i++)
		dc.FillSolidRect(i * RectSize, 0, 1, col * RectSize - CurVScrollPos * field.RectSize, RGB(0, 0, 0));

	//Draw horizontal lines of field
	for (int i = 0; i <= col - CurVScrollPos; i++)
		dc.FillSolidRect(0, i * RectSize, row * RectSize - CurHScrollPos * field.RectSize, 1, RGB(0, 0, 0));
}

void CGameOfLife1View::DrawRectByCoordinates(CPaintDC & dc, int i, int j, bool isDraw)
{
	if (isDraw)
		dc.FillSolidRect(i * RectSize + 1, j * RectSize + 1, RectSize - 1, RectSize - 1, RGB(70, 30, 90));
	else
		dc.FillSolidRect(i * RectSize + 1, j * RectSize + 1, RectSize - 1, RectSize - 1, RGB(255, 255, 255));
}

void CGameOfLife1View::RedrawRect(CPaintDC & dc)
{
	for (int i = 0; i < TempFieldLive.size(); i++)
		DrawRectByCoordinates(dc, TempFieldLive[i] % row, TempFieldLive[i] / row, false);

	for (int i = 0; i < field.Live.size(); i++)
		DrawRectByCoordinates(dc, field.Live[i] % row - CurHScrollPos, field.Live[i] / row - CurVScrollPos, true);

	for (int i = 0; i < field.PotentialLive.size(); i++)
		DrawRectByCoordinates(dc, field.PotentialLive[i] % row - CurHScrollPos, field.PotentialLive[i] / row - CurVScrollPos, false);

	for (int i = 0; i < field.Dead.size(); i++)
		DrawRectByCoordinates(dc, field.Dead[i] % row - CurHScrollPos, field.Dead[i] / row - CurVScrollPos, false);

	TempFieldLive = field.Live;
}

void CGameOfLife1View::ThreadProc(void * ipMyID)
{
	do
	{
		WaitForSingleObject(field.hFieldMutex, INFINITE);
		field.Dead = field.PotentialLive;
		field.ClearEmptyFields();
		vect temp = field.RealField;
		std::sort(field.Live.rbegin(), field.Live.rend());
		for (int i = 0; i < field.Live.size(); i++)
		{
			field.AddPotentialNeighbors(field.Live[i]);
			int count = 0;
			field.is_on_field(field.Live[i] - field.row - 1, count);
			field.is_on_field(field.Live[i] - field.row, count);
			field.is_on_field(field.Live[i] - field.row + 1, count);
			field.is_on_field(field.Live[i] - 1, count);
			field.is_on_field(field.Live[i] + 1, count);
			field.is_on_field(field.Live[i] + field.row - 1, count);
			field.is_on_field(field.Live[i] + field.row, count);
			field.is_on_field(field.Live[i] + field.row + 1, count);
			int tmp = field.Live[i];
			temp[field.Live[i]] = field.ChkRules(1, count);
			field.Live[i] = temp[field.Live[i]] == 0 ? -1 : field.Live[i];
			if (field.Live[i] == -1)
				field.Dead.push_back(tmp);
		}
		field.ClearEmptyFields();
		for (int i = 0; i < field.PotentialLive.size(); i++)
		{
			int count = 0;
			field.is_on_field(field.PotentialLive[i] - field.row - 1, count);
			field.is_on_field(field.PotentialLive[i] - field.row, count);
			field.is_on_field(field.PotentialLive[i] - field.row + 1, count);
			field.is_on_field(field.PotentialLive[i] - 1, count);
			field.is_on_field(field.PotentialLive[i] + 1, count);
			field.is_on_field(field.PotentialLive[i] + field.row - 1, count);
			field.is_on_field(field.PotentialLive[i] + field.row, count);
			field.is_on_field(field.PotentialLive[i] + field.row + 1, count);
			temp[field.PotentialLive[i]] = field.ChkRules(0, count);
			if (temp[field.PotentialLive[i]] == 1)
				field.Live.push_back(field.PotentialLive[i]);
			else
				field.Dead.push_back(field.PotentialLive[i]);
		}
		field.RealField = temp;
		field.ClearEmptyFields();
		ReleaseMutex(field.hFieldMutex);
	} while (WaitForSingleObject(field.hRunMutex, 75L) == WAIT_TIMEOUT);
}

LRESULT CGameOfLife1View::OnPaint(UINT uMsg, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	//TODO: Add your drawing code here
	CPaintDC dc(m_hWnd);
	field.SetSize(row, col);
	field.RectSize = RectSize;
	if (uMsg == WM_ERASEBACKGROUND)
		OnErase(dc);
	DrawField(dc);
	RedrawRect(dc);
	WINDOWINFO info;
	GetWindowInfo(m_hWnd, &info);
	RECT rect = info.rcWindow;

	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	if (rect.right - rect.left < col * field.RectSize + field.RectSize)
		DrawHScroll(width, true);
	else
		DrawHScroll(width, false);

	if (rect.bottom - rect.top < row * field.RectSize + field.RectSize)
		DrawVScroll(height, true);
	else
		DrawVScroll(height, false);
	return 0;
}

LRESULT CGameOfLife1View::OnMouseWheel(UINT uKeys, short nDistance, CPoint pt)
{
	for (int i = 0; i < field.Live.size(); i++)
		field.AddPotentialNeighbors(field.Live[i]);
	if (nDistance < 0)
	{
		SCROLLINFO si;
		si.cbSize = sizeof(si);
		si.fMask = SIF_PAGE | SIF_POS | SIF_RANGE;
		GetScrollInfo(SB_VERT, &si);
		if (si.nPos != si.nMax) 
			si.nPos += 1;
		CurVScrollPos = si.nPos;
		RedrawWindow();
		SetScrollInfo(SB_VERT, &si);
	}
	if (nDistance > 0)
	{
		SCROLLINFO si;
		si.cbSize = sizeof(si);
		si.fMask = SIF_PAGE | SIF_POS | SIF_RANGE;
		GetScrollInfo(SB_VERT, &si);
		if (si.nPos != 0)
			si.nPos -= 1;
		CurVScrollPos = si.nPos;
		RedrawWindow();
		SetScrollInfo(SB_VERT, &si);
	}
	return 0;
}

void CGameOfLife1View::OnErase(CPaintDC &dc)
{
	dc.FillSolidRect(0, 0, 100000, 100000, RGB(255, 255, 255));
	DrawField(dc);
	RedrawRect(dc);
}

LRESULT CGameOfLife1View::OnRButtonDown(UINT nFlags, CPoint pos)
{
	GetPosition(pos);
	field.DrawRect(pos, tempX, tempY,CurHScrollPos, CurVScrollPos, false);
	RedrawWindow();
	return 0;
}

LRESULT CGameOfLife1View::OnLButtonDown(UINT nFlags, CPoint pos)
{
	GetPosition(pos);
	if (Glider)
	{
		field.DrawGlider(pos, tempX, tempY);
	}
	else if (Small_Exploder)
	{
		field.DrawSmall_Exploder(pos, tempX, tempY);
	}
	else if (Exploder)
	{
		field.DrawExploder(pos, tempX, tempY);
	}
	else if (Ten_Cell_Row)
	{
		field.DrawTen_Cell_Row(pos, tempX, tempY);
	}
	else if (Lightweight_Spaceship)
	{
		field.DrawLightweight_Spaceship(pos, tempX, tempY);
	}
	else if (Tumbler)
	{
		field.DrawTumbler(pos, tempX, tempY);
	}
	else if (Gosper_Glider_Gun)
	{
		field.DrawGosper_Glider_Gun(pos, tempX, tempY);
	}
	else
	{
		field.DrawRect(pos, tempX, tempY, CurHScrollPos, CurVScrollPos, true);
	}
	RedrawWindow();

	return 0;
}

LRESULT CGameOfLife1View::OnMouseMove(UINT nFlags, CPoint pos)
{
	if (field.isClickOnField(pos, CurHScrollPos, CurVScrollPos))
	{
		if (nFlags == MK_LBUTTON)
		{
			GetPosition(pos);
			OnLButtonDown(nFlags, pos);
		}
		else if (nFlags == MK_RBUTTON)
		{
			GetPosition(pos);
			OnRButtonDown(nFlags, pos);
		}
	}
	return 0;
}

LRESULT CGameOfLife1View::OnCreate(LPCREATESTRUCT pCreate)
{
	SetTimer(1, 1000);
	SetMsgHandled(false);
	return 0;
}

LRESULT CGameOfLife1View::OnVScroll(UINT, WPARAM wParam, LPARAM, BOOL &)
{
	SCROLLINFO si;
	si.cbSize = sizeof(si);
	si.fMask = SIF_PAGE | SIF_POS | SIF_RANGE;
	GetScrollInfo(SB_VERT, &si);
	for (int i = 0; i < field.Live.size(); i++)
		field.AddPotentialNeighbors(field.Live[i]);
	switch (LOWORD(wParam))
	{
	case SB_LINEUP:
	{
		if (si.nPos == 0) break;
		ChangeYCoord -= field.RectSize;
		si.nPos -= 1;
		break;
	}
	case SB_PAGEUP:
	{
		if (si.nPos == 0) break;
		ChangeYCoord += field.RectSize;
		si.nPos -= 1;
		break;
	}
	case SB_LINEDOWN:
	{
		if (si.nPos == si.nMax)
			break;
		ChangeYCoord -= field.RectSize;
		si.nPos += 1;
		break;
	}
	case SB_PAGEDOWN:
	{
		if (si.nPos == si.nMax) break;
		ChangeYCoord += field.RectSize;
		si.nPos += 1;
		break;
	}

	case SB_TOP:
	{
		si.nPos = si.nMin;
		break;
	}
	case SB_BOTTOM:
	{
		si.nPos = si.nMax;
		break;
	}
	case SB_THUMBPOSITION:
	{
		WORD y1 = HIWORD(wParam);
		si.nPos = HIWORD(wParam);
		break;
	}
	case SB_THUMBTRACK:
	{
		si.nPos = HIWORD(wParam);
		break;
	}
	}
	CurVScrollPos = si.nPos;
	RedrawWindow();
	SetScrollInfo(SB_VERT, &si);
	return 0;
}

LRESULT CGameOfLife1View::OnHScroll(UINT, WPARAM wParam, LPARAM, BOOL &)
{
	SCROLLINFO si;
	si.cbSize = sizeof(si);
	si.fMask = SIF_PAGE | SIF_POS | SIF_RANGE;
	GetScrollInfo(SB_HORZ, &si);
	for (int i = 0; i < field.Live.size(); i++)
		field.AddPotentialNeighbors(field.Live[i]);
	switch (LOWORD(wParam))
	{

	case SB_LINELEFT:
	{
		if (si.nPos == 0) break;
		ChangeXCoord -= field.RectSize;
		si.nPos -= 1;
		break;
	}
	case SB_PAGELEFT:
	{
		if (si.nPos == 0) break;
		ChangeXCoord += field.RectSize;
		si.nPos -= 1;
		break;
	}
	case SB_LINERIGHT:
	{
		if (si.nPos == si.nMax) break;
		ChangeXCoord -= field.RectSize;
		si.nPos += 1;
		break;
	}
	case SB_PAGERIGHT:
	{
		if (si.nPos == si.nMax) break;
		ChangeXCoord += field.RectSize;
		si.nPos += 1;
		break;
	}

	case SB_TOP:
	{
		si.nPos = si.nMin; break;
	}
	case SB_BOTTOM:
	{
		si.nPos = si.nMax; break;
	}
	case SB_THUMBPOSITION:
	{
		WORD y1 = HIWORD(wParam);
		si.nPos = HIWORD(wParam);
		break;
	}
	case SB_THUMBTRACK:
	{
		si.nPos = HIWORD(wParam); 
		break;
	}
	}
	CurHScrollPos = si.nPos;
	RedrawWindow();
	SetScrollInfo(SB_HORZ, &si);
	return 0;
}

void CGameOfLife1View::DrawHScroll(int width, bool isDraw)
{
	if (isDraw)
	{
		int countVisualCells = width / field.RectSize;
		int countHiddenCells = field.row - countVisualCells;
		countHiddenCells = countHiddenCells == 0 ? 1 : countHiddenCells;
		SCROLLINFO si;
		ShowScrollBar(SB_HORZ, 1);
		si.cbSize = sizeof(si);
		si.fMask = SIF_PAGE | SIF_POS | SIF_RANGE;
		si.nMin = 0;
		si.nMax = countHiddenCells;
		si.nPage = 1;
		si.nPos = CurHScrollPos;

		SetScrollInfo(SB_HORZ, &si);
	}
	else
	{
		ShowScrollBar(SB_HORZ, 0);
	}
}

void CGameOfLife1View::DrawVScroll(int height, bool isDraw)
{
	if (isDraw)
	{
		int countVisualCells = height / field.RectSize;
		int countHiddenCells = field.col - countVisualCells;
		countHiddenCells = countHiddenCells == 0 ? 1 : countHiddenCells;
		WINDOWINFO info;
		GetWindowInfo(m_hWnd, &info);
		RECT rect = info.rcWindow;
		SCROLLINFO si;
		ShowScrollBar(SB_VERT, 1);
		si.cbSize = sizeof(si);
		si.fMask = SIF_PAGE | SIF_POS | SIF_RANGE;
		si.nMin = 0;
		si.nMax = countHiddenCells;
		si.nPage = 1;
		si.nPos = CurVScrollPos;
		SetScrollInfo(SB_VERT, &si);
	}
	else
	{
		ShowScrollBar(SB_VERT, 0);
	}
}

LRESULT CGameOfLife1View::Play(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	if (isFastRun)
	{
		field.hFieldMutex = CreateMutex(NULL, TRUE, NULL);
		field.hRunMutex = CreateMutex(NULL, TRUE, NULL);
		RedrawWindow();
		_beginthread(this->ThreadProc, 4096, NULL);
		if (stop)KillTimer(IDT_TIMER1);

		CloseHandle(field.hFieldMutex);
		CloseHandle(field.hRunMutex);
	}
	else
	{
		field.Run();
		RedrawWindow();
		if (stop)KillTimer(IDT_TIMER1);
	}
	return 0;
}

void CGameOfLife1View::GameClear()
{
	field.Clear();
	for (int i = 0; i < row*col; i++)
	{
		field.Dead.push_back(i);
	}
	KillTimer(IDT_TIMER1);
	RedrawWindow();
}

void CGameOfLife1View::CreateVector()
{
	for (int i = 0; i < field.row*field.col * 10; i++)
		arrVector.push_back(0);
}


LRESULT CGameOfLife1View::Save()
{
	using namespace std;
	LPTSTR url;
	LPCTSTR szFilter(L"Text files (*.txt)|*.txt|");
	LPTSTR s1 = { L" " };
	// TODO: Add your command handler code here
	CFileDialog lg1(FALSE, L"txt", L"life_game_files", OFN_HIDEREADONLY | OFN_FILEMUSTEXIST,
		L"Camera Data Files (*.txt)|*.txt|*.dat||", m_hWnd);
	if (lg1.DoModal() == IDOK) {

		url = lg1.m_ofn.lpstrFile;
		char a[1024];
		string d;


		int i = 0;
		for (i = 0; i <= 1024; i++) {
			if (!url[i]) break;
		}
		for (int j = 0; j < i; j++) {
			d += url[j];
		}
		ofstream fout(d);
		fout << row << "/" << col << "/" << RectSize << "/" << seconds;
		for (int i = 0; i < row; i++)
		{
			for (int j = 0; j < col; j++)
			{
				if (field.RealField[row*j + i] == 1)
				{
					fout << "1";
				}
				else
				{
					fout << "0";
				}
			}
		}

	}
	return 0;
}

LRESULT CGameOfLife1View::Open()
{
	using namespace std;
	LPTSTR url;
	LPCTSTR szFilter(L"Text files (*.txt)|*.txt|");
	LPTSTR s1 = { L" " };
	// TODO: Add your command handler code here
	CFileDialog lg1(TRUE, L"txt", L"vicon_cams_data", OFN_HIDEREADONLY | OFN_FILEMUSTEXIST,
		L"Camera Data Files (*.txt)|*.txt|*.dat||", m_hWnd);
	if (lg1.DoModal() == IDOK) {

		url = lg1.m_ofn.lpstrFile;
		char a[1024];
		string d;
		int i = 0;
		for (i = 0; i <= 1024; i++) {
			if (!url[i]) break;
		}
		for (int j = 0; j < i; j++) {
			d += url[j];
		}
		char bv;
		ifstream fin(d);

		fin >> row >> a >> col >> a >> a >> RectSize >> a >> seconds >> a;
		RedrawWindow();
		/*int j = 6;
		for (int z = 0; z < x_main / size_of_rect; z++) {
			for (j; j < y_main / size_of_rect; j++) {
				fin >> that.cont[that.ret_index(z, j, 1)] >> a;
			}
			j = 0;
		}*/
		char b;
		for (int i = 0; i < row; i++)
		{
			for (int j = 0; j < col; j++)
			{
				fin >> b;
				if (b == '0')
					field.RealField[row*j + i] = 0;
				else if (b == '1')
					field.RealField[row*j + i] = 1;
			}
			fin >> a;
		}
	}
	RedrawWindow();
	return 0;
}

void CGameOfLife1View::ChangeFieldParams(int col, int row, int RectSize)
{
	this->col = (col == 0) ? this->col : col;
	this->row = (row == 0) ? this->row : row;
	this->RectSize = RectSize == 0 ? this->RectSize : RectSize;
	field.ChangeParams(row, col);
	RedrawWindow();
}


void CGameOfLife1View::GetPosition(CPoint pos)
{
	if (!field.isClickOnField(pos, CurHScrollPos, CurVScrollPos))
		return;
	tempX = (pos.x) / RectSize + CurHScrollPos;
	tempY = (pos.y) / RectSize + CurVScrollPos;
}
