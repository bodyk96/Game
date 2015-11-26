#include <vector>
#include<algorithm>
#include <string>
#pragma once
typedef std::vector<int> vect;
using std::string;
class MyField
{
public:
	MyField();
	MyField(int row, int col, int RectSize);
	~MyField();
	void Run();
	void Clear();
	void DrawRect(CPoint pos, int &tempX, int &tempY, int CurHScrollPos, int CurVScrollPos, bool isDrawRect);
	void DrawGlider(CPoint pos, int &tempX, int &tempY);
	void DrawSmall_Exploder(CPoint pos, int &tempX, int &tempY);
	void DrawExploder(CPoint pos, int &tempX, int &tempY);
	void DrawTen_Cell_Row(CPoint pos, int &tempX, int &tempY);
	void DrawLightweight_Spaceship(CPoint pos, int &tempX, int &tempY);
	void DrawTumbler(CPoint pos, int &tempX, int &tempY);
	void DrawGosper_Glider_Gun(CPoint pos, int &tempX, int &tempY);
	int row;
	int col;
	int RectSize;
	vect RealField = { 0 };
	vect Dead = { 0 };
	vect Live;
	vect PotentialLive;
	void SetSize(int rowN, int colN);
	bool isRectDraw(int x, int y);
	bool isClickOnField(CPoint pos, int CurHScrollPos, int CurVScrollPos);
	bool isChangeField = false;
	void ChangeParams(int rowN, int colN);
	void AddPotentialNeighbors(int index);
	HANDLE hFieldMutex;
	HANDLE hRunMutex;
	bool isDrawRect;
	int ChkRules(int num, int count);
	void is_on_field(int index, int &count);
	bool is_on_field(int index);
	void ClearEmptyInLive();
	void ClearEmptyInLiveP();
	void ClearEmptyinDead();
	void ClearEmptyFields();
	

};
	extern MyField field;