// GameOfLife(1)View.h : interface of the CGameOfLife1View class
//
/////////////////////////////////////////////////////////////////////////////
#include<vector>
#pragma once
#include "Set_Size.h"
#include "Field.h"

class CGameOfLife1View : public CWindowImpl<CGameOfLife1View>
{
public:
	DECLARE_WND_CLASS(NULL)

	BOOL PreTranslateMessage(MSG* pMsg);

	BEGIN_MSG_MAP(CGameOfLife1View)
		MSG_WM_RBUTTONDOWN(OnRButtonDown)
		MSG_WM_LBUTTONDOWN(OnLButtonDown)
		MSG_WM_MOUSEWHEEL(OnMouseWheel)
		MSG_WM_MOUSEMOVE(OnMouseMove)
		MESSAGE_HANDLER(WM_ERASEBACKGROUND, OnPaint)
		MESSAGE_HANDLER(WM_VSCROLL, OnVScroll)
		MESSAGE_HANDLER(WM_HSCROLL, OnHScroll)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MSG_WM_CREATE(OnCreate)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
	LRESULT Play(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	void GameClear();
	void CreateVector();
	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnMouseWheel(UINT uKeys, short nDistance, CPoint pt);
	void OnErase(CPaintDC &dc);
	LRESULT OnLButtonDown(UINT nFlags, CPoint pos);
	LRESULT OnRButtonDown(UINT nFlags, CPoint pos);
	LRESULT OnMouseMove(UINT nFlags, CPoint pos);
	LRESULT OnCreate(LPCREATESTRUCT pCreate);
	LRESULT OnVScroll(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnHScroll(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	void DrawHScroll(int width, bool isDraw);
	void DrawVScroll(int height, bool isDraw);
	int CurHScrollPos = 0;
	int CurVScrollPos = 0;
	int ChangeXCoord = 0;
	int ChangeYCoord = 0;
	vect TempFieldLive;
	vect TempFieldLiveP;
	vect TempFieldDead;
	LRESULT Save();
	LRESULT Open();
	void ChangeFieldParams(int col, int row, int RectSize);
	void CGameOfLife1View::DrawField(CPaintDC &dc);
	void DrawRectByCoordinates(CPaintDC & dc, int i, int j, bool isDraw);
	void RedrawRect(CPaintDC &dc);
	bool isRunning = true;
	
	int RectSize = 20;
	int row = 50;
	int col = 50;
	bool isFastRun = false;
	static void ThreadProc(void * ipMyID);
	bool stop = false;
	BOOL ab = false;
	std::vector<int> arrVector = { 0 };
	int tempX = 100;
	int tempY = 100;
	bool isMouseUp = true;
	UINT seconds = 50;
	void GetPosition(CPoint pos);
	//«м≥нн≥ шаблон≥в ф≥гур
	bool Glider = false;
	bool Small_Exploder = false;
	bool Exploder = false;
	bool Ten_Cell_Row = false;
	bool Lightweight_Spaceship = false;
	bool Tumbler = false;
	bool Gosper_Glider_Gun = false;
};
