#include "stdafx.h"
#include "Field.h"
MyField field;
MyField::MyField(int rowN, int colN, int RectSizeN)
{
	row = rowN;
	col = colN;
	RectSize = RectSizeN;
	for (int i = 0; i < row*col; i++)
		RealField.push_back(0);
}

MyField::MyField()
{
	row = 50;
	col = 50;
	RectSize = 20;
	for (int i = 0; i < row*col; i++)
		RealField.push_back(0);
}

MyField::~MyField()
{
}

void MyField::is_on_field(int index, int &count)
{
	if (index >= 0 && index < row*col && RealField[index] == 1)count++;
}

bool MyField::is_on_field(int index)
{
	return index >= 0 && index < row*col ? true : false;
}

void MyField::Run()
{
	Dead = PotentialLive;
	ClearEmptyFields();
	vect temp = RealField;
	std::sort(Live.rbegin(), Live.rend());
	for (int i = 0; i < Live.size(); i++)
	{
		AddPotentialNeighbors(Live[i]);
		int count = 0;
		is_on_field(Live[i] - row - 1, count);
		is_on_field(Live[i] - row, count);
		is_on_field(Live[i] - row + 1, count);
		is_on_field(Live[i] - 1, count);
		is_on_field(Live[i] + 1, count);
		is_on_field(Live[i] + row - 1, count);
		is_on_field(Live[i] + row, count);
		is_on_field(Live[i] + row + 1, count);
		int tmp = Live[i];
		temp[Live[i]] = ChkRules(1, count);
		Live[i] = temp[Live[i]] == 0 ? -1 : Live[i];
		if (Live[i] == -1)
			Dead.push_back(tmp);
	}
	ClearEmptyFields();
	for (int i = 0; i < PotentialLive.size(); i++)
	{
		int count = 0;
		is_on_field(PotentialLive[i] - row - 1, count);
		is_on_field(PotentialLive[i] - row, count);
		is_on_field(PotentialLive[i] - row + 1, count);
		is_on_field(PotentialLive[i] - 1, count);
		is_on_field(PotentialLive[i] + 1, count);
		is_on_field(PotentialLive[i] + row - 1, count);
		is_on_field(PotentialLive[i] + row, count);
		is_on_field(PotentialLive[i] + row + 1, count);
		temp[PotentialLive[i]] = ChkRules(0, count);
		if (temp[PotentialLive[i]] == 1)
			Live.push_back(PotentialLive[i]);
		else
			Dead.push_back(PotentialLive[i]);
	}
	RealField = temp;
	ClearEmptyFields();
}

void MyField::Clear()
{
	for (vect::iterator i = RealField.begin(); i < RealField.end(); i++)
		*i = 0;
	Live.clear();
	PotentialLive.clear();
	Dead.clear();
}

void MyField::DrawRect(CPoint pos, int &tempX, int &tempY, int CurHScrollPos, int CurVScrollPos,  bool isDrawRect)
{
	if (isClickOnField(pos, CurHScrollPos, CurVScrollPos))
	{
		ClearEmptyFields();
		int tempIndex = row * tempY + tempX;
		RealField[tempIndex] = isDrawRect ? 1 : 0;
		if (isDrawRect)
		{
			vect::iterator i = std::find(Dead.begin(), Dead.end(), tempIndex);
			if (i != Dead.end())
			{
				*i = -1;
			}
			
			Live.push_back(tempIndex);
		}
		else
			for (vect::iterator i = Live.begin(); i != Live.end(); i++)
			{
				if (*i == tempIndex)
				{
					*i = -1;
					Dead.push_back(tempIndex);
				}
			}
		ClearEmptyFields();
	}
}

void MyField::DrawGlider(CPoint pos, int &tempX, int &tempY)
{
	int tempIndex = row * tempY + tempX;
	if (is_on_field(tempIndex - row - 1) && is_on_field(tempIndex + row + 1))
	{
		Live.push_back(tempIndex - row);		RealField[tempIndex - row] = 1;
		Live.push_back(tempIndex + 1);			RealField[tempIndex + 1] = 1;
		Live.push_back(tempIndex + row + 1);	RealField[tempIndex + row + 1] = 1;
		Live.push_back(tempIndex + row);		RealField[tempIndex + row] = 1;
		Live.push_back(tempIndex + row - 1);	RealField[tempIndex + row - 1] = 1;
	}
}

void MyField::DrawTen_Cell_Row(CPoint pos, int &tempX, int &tempY)
{
	int tempIndex = row * tempY + tempX;
	if (is_on_field(tempIndex - 4) && is_on_field(tempIndex + 5))
	{
		Live.push_back(tempIndex - 4);	RealField[tempIndex - 4] = 1;
		Live.push_back(tempIndex - 3);	RealField[tempIndex - 3] = 1;
		Live.push_back(tempIndex - 2);	RealField[tempIndex - 2] = 1;
		Live.push_back(tempIndex - 1);	RealField[tempIndex - 1] = 1;
		Live.push_back(tempIndex);		RealField[tempIndex] = 1;
		Live.push_back(tempIndex + 1);	RealField[tempIndex + 1] = 1;
		Live.push_back(tempIndex + 2);	RealField[tempIndex + 2] = 1;
		Live.push_back(tempIndex + 3);	RealField[tempIndex + 3] = 1;
		Live.push_back(tempIndex + 4);	RealField[tempIndex + 4] = 1;
		Live.push_back(tempIndex + 5);	RealField[tempIndex + 5] = 1;
	}
}

void MyField::DrawSmall_Exploder(CPoint pos, int &tempX, int &tempY)
{
	int tempIndex = row * tempY + tempX;
	if (is_on_field(tempIndex - 2 * row - 1) && is_on_field(tempIndex + row + 1))
	{
		Live.push_back(tempIndex - 2 * row);	RealField[tempIndex - 2 * row] = 1;
		Live.push_back(tempIndex - row - 1);	RealField[tempIndex - row - 1] = 1;
		Live.push_back(tempIndex - row);		RealField[tempIndex - row] = 1;
		Live.push_back(tempIndex - row + 1);	RealField[tempIndex - row + 1] = 1;
		Live.push_back(tempIndex - 1);			RealField[tempIndex - 1] = 1;
		Live.push_back(tempIndex + 1);			RealField[tempIndex + 1] = 1;
		Live.push_back(tempIndex + row);		RealField[tempIndex + row] = 1;
	}
}

void MyField::DrawLightweight_Spaceship(CPoint pos, int &tempX, int &tempY)
{
	int tempIndex = row * tempY + tempX;
	if (is_on_field(tempIndex - 2 * row - 2) && is_on_field(tempIndex + row + 2))
	{
		Live.push_back(tempIndex - 2 * row - 1);	RealField[tempIndex - 2 * row - 1] = 1;
		Live.push_back(tempIndex - 2 * row);		RealField[tempIndex - 2 * row] = 1;
		Live.push_back(tempIndex - 2 * row + 1);	RealField[tempIndex - 2 * row + 1] = 1;
		Live.push_back(tempIndex - 2 * row + 2);	RealField[tempIndex - 2 * row + 2] = 1;
		Live.push_back(tempIndex - row - 2);		RealField[tempIndex - row - 2] = 1;
		Live.push_back(tempIndex - row + 2);		RealField[tempIndex - row + 2] = 1;
		Live.push_back(tempIndex + 2);				RealField[tempIndex + 2] = 1;
		Live.push_back(tempIndex + row - 2);		RealField[tempIndex + row - 2] = 1;
		Live.push_back(tempIndex + row + 1);		RealField[tempIndex + row + 1] = 1;
	}
}

void MyField::DrawTumbler(CPoint pos, int &tempX, int &tempY)
{
	int tempIndex = row * tempY + tempX;
	if (is_on_field(tempIndex - 3 * row - 3) && is_on_field(tempIndex + 2 * row + 2))
	{
		Live.push_back(tempIndex - 3 * row - 2);	RealField[tempIndex - 3 * row - 2] = 1;
		Live.push_back(tempIndex - 3 * row - 1);	RealField[tempIndex - 3 * row - 1] = 1;
		Live.push_back(tempIndex - 3 * row + 1);	RealField[tempIndex - 3 * row + 1] = 1;
		Live.push_back(tempIndex - 3 * row + 2);	RealField[tempIndex - 3 * row + 2] = 1;
		Live.push_back(tempIndex - 2 * row - 2);	RealField[tempIndex - 2 * row - 2] = 1;
		Live.push_back(tempIndex - 2 * row - 1);	RealField[tempIndex - 2 * row - 1] = 1;
		Live.push_back(tempIndex - 2 * row + 1);	RealField[tempIndex - 2 * row + 1] = 1;
		Live.push_back(tempIndex - 2 * row + 2);	RealField[tempIndex - 2 * row + 2] = 1;
		Live.push_back(tempIndex - row - 1);		RealField[tempIndex - row - 1] = 1;
		Live.push_back(tempIndex - row + 1);		RealField[tempIndex - row + 1] = 1;
		Live.push_back(tempIndex - 3);				RealField[tempIndex - 3] = 1;
		Live.push_back(tempIndex - 1);				RealField[tempIndex - 1] = 1;
		Live.push_back(tempIndex + 1);				RealField[tempIndex + 1] = 1;
		Live.push_back(tempIndex + 3);				RealField[tempIndex + 3] = 1;
		Live.push_back(tempIndex + row - 3);		RealField[tempIndex + row - 3] = 1;
		Live.push_back(tempIndex + row - 1);		RealField[tempIndex + row - 1] = 1;
		Live.push_back(tempIndex + row + 1);		RealField[tempIndex + row + 1] = 1;
		Live.push_back(tempIndex + row + 3);		RealField[tempIndex + row + 3] = 1;
		Live.push_back(tempIndex + 2 * row - 3);	RealField[tempIndex + 2 * row - 3] = 1;
		Live.push_back(tempIndex + 2 * row - 2);	RealField[tempIndex + 2 * row - 2] = 1;
		Live.push_back(tempIndex + 2 * row + 2);	RealField[tempIndex + 2 * row + 2] = 1;
		Live.push_back(tempIndex + 2 * row + 3);	RealField[tempIndex + 2 * row + 3] = 1;
	}
}

void MyField::DrawGosper_Glider_Gun(CPoint pos, int &tempX, int &tempY)
{
	int tempIndex = row * tempY + tempX;
	if (is_on_field(tempIndex - 7 * row - 18) && is_on_field(tempIndex + 7 * row + 19))
	{
		Live.push_back(tempIndex - 5 * row - 18);	RealField[tempIndex - 5 * row - 18] = 1;
		Live.push_back(tempIndex - 5 * row - 17);	RealField[tempIndex - 5 * row - 17] = 1;
		Live.push_back(tempIndex - 4 * row - 18);	RealField[tempIndex - 4 * row - 18] = 1;
		Live.push_back(tempIndex - 4 * row - 17);	RealField[tempIndex - 4 * row - 17] = 1;

		Live.push_back(tempIndex - 5 * row - 9);	RealField[tempIndex - 5 * row - 9] = 1;
		Live.push_back(tempIndex - 5 * row - 8);	RealField[tempIndex - 5 * row - 8] = 1;
		Live.push_back(tempIndex - 4 * row - 10);	RealField[tempIndex - 4 * row - 10] = 1;
		Live.push_back(tempIndex - 4 * row - 8);	RealField[tempIndex - 4 * row - 8] = 1;
		Live.push_back(tempIndex - 3 * row - 10);	RealField[tempIndex - 3 * row - 10] = 1;
		Live.push_back(tempIndex - 3 * row - 9);	RealField[tempIndex - 3 * row - 9] = 1;

		Live.push_back(tempIndex - 3 * row - 2);	RealField[tempIndex - 3 * row - 2] = 1;
		Live.push_back(tempIndex - 3 * row - 1);	RealField[tempIndex - 3 * row - 1] = 1;
		Live.push_back(tempIndex - 2 * row - 2);	RealField[tempIndex - 2 * row - 2] = 1;
		Live.push_back(tempIndex - 2 * row);		RealField[tempIndex - 2 * row] = 1;
		Live.push_back(tempIndex - row - 2);		RealField[tempIndex - row - 2] = 1;

		Live.push_back(tempIndex - 7 * row + 5);	RealField[tempIndex - 7 * row + 5] = 1;
		Live.push_back(tempIndex - 7 * row + 6);	RealField[tempIndex - 7 * row + 6] = 1;
		Live.push_back(tempIndex - 6 * row + 4);	RealField[tempIndex - 6 * row + 4] = 1;
		Live.push_back(tempIndex - 6 * row + 6);	RealField[tempIndex - 6 * row + 6] = 1;
		Live.push_back(tempIndex - 5 * row + 4);	RealField[tempIndex - 5 * row + 4] = 1;
		Live.push_back(tempIndex - 5 * row + 5);	RealField[tempIndex - 5 * row + 5] = 1;

		Live.push_back(tempIndex + 7 * row + 7);	RealField[tempIndex + 7 * row + 7] = 1;
		Live.push_back(tempIndex + 6 * row + 6);	RealField[tempIndex + 6 * row + 6] = 1;
		Live.push_back(tempIndex + 5 * row + 6);	RealField[tempIndex + 5 * row + 6] = 1;
		Live.push_back(tempIndex + 5 * row + 7);	RealField[tempIndex + 5 * row + 7] = 1;
		Live.push_back(tempIndex + 5 * row + 8);	RealField[tempIndex + 5 * row + 8] = 1;

		Live.push_back(tempIndex - 7 * row + 16);	RealField[tempIndex - 7 * row + 16] = 1;
		Live.push_back(tempIndex - 7 * row + 17);	RealField[tempIndex - 7 * row + 17] = 1;
		Live.push_back(tempIndex - 6 * row + 16);	RealField[tempIndex - 6 * row + 16] = 1;
		Live.push_back(tempIndex - 6 * row + 17);	RealField[tempIndex - 6 * row + 17] = 1;

		Live.push_back(tempIndex + 17);				RealField[tempIndex + 17] = 1;
		Live.push_back(tempIndex + 18);				RealField[tempIndex + 18] = 1;
		Live.push_back(tempIndex + row + 17);		RealField[tempIndex + row + 17] = 1;
		Live.push_back(tempIndex + row + 19);		RealField[tempIndex + row + 19] = 1;
		Live.push_back(tempIndex + 2 * row + 17);	RealField[tempIndex + 2 * row + 17] = 1;
	}
}

void MyField::DrawExploder(CPoint pos, int &tempX, int &tempY)
{
	int tempIndex = row * tempY + tempX;
	if (is_on_field(tempIndex - 2 * row - 2) && is_on_field(tempIndex + 2 * row + 2))
	{
		Live.push_back(tempIndex - 2 * row - 2);	RealField[tempIndex - 2 * row - 2] = 1;
		Live.push_back(tempIndex - 2 * row);		RealField[tempIndex - 2 * row] = 1;
		Live.push_back(tempIndex - 2 * row + 2);	RealField[tempIndex - 2 * row + 2] = 1;
		Live.push_back(tempIndex - row - 2);		RealField[tempIndex - row - 2] = 1;
		Live.push_back(tempIndex - row + 2);		RealField[tempIndex - row + 2] = 1;
		Live.push_back(tempIndex - 2);				RealField[tempIndex - 2] = 1;
		Live.push_back(tempIndex + 2);				RealField[tempIndex + 2] = 1;
		Live.push_back(tempIndex + row - 2);		RealField[tempIndex + row - 2] = 1;
		Live.push_back(tempIndex + row + 2);		RealField[tempIndex + row + 2] = 1;
		Live.push_back(tempIndex + 2 * row - 2);	RealField[tempIndex + 2 * row - 2] = 1;
		Live.push_back(tempIndex + 2 * row);		RealField[tempIndex + 2 * row] = 1;
		Live.push_back(tempIndex + 2 * row + 2);	RealField[tempIndex + 2 * row + 2] = 1;
	}
}

void MyField::ClearEmptyInLive()
{
	Live.resize(std::unique(Live.begin(), Live.end()) - Live.begin());
	Live.erase(std::remove(Live.begin(), Live.end(), -1), Live.end());
}

void MyField::ClearEmptyInLiveP()
{
	for (int i = 0; i < Live.size(); i++)
	{
		int temp = Live[i];
		if (std::find(PotentialLive.begin(), PotentialLive.end(), temp) != PotentialLive.end())
			PotentialLive.erase(std::remove(PotentialLive.begin(), PotentialLive.end(), temp), PotentialLive.end());
	}
	PotentialLive.resize(std::unique(PotentialLive.begin(), PotentialLive.end()) - PotentialLive.begin());
	std::sort(PotentialLive.begin(), PotentialLive.end());

}

void MyField::ClearEmptyinDead()
{
	Dead.resize(std::unique(Dead.begin(), Dead.end()) - Dead.begin());
	Dead.erase(std::remove(Dead.begin(), Dead.end(), -1), Dead.end());
	for (int i = 0; i < Live.size(); i++)
	{
		int temp = Live[i];
		if (std::find(Dead.begin(), Dead.end(), temp) != Dead.end())
			Dead.erase(std::remove(Dead.begin(), Dead.end(), temp), Dead.end());
	}
}

void MyField::ClearEmptyFields()
{
	ClearEmptyInLive();
	ClearEmptyInLiveP();
	ClearEmptyinDead();
}


void MyField::SetSize(int rowN, int colN)
{
	row = rowN;
	col = colN;
}

bool MyField::isRectDraw(int x, int y)
{
	return RealField[row * y + x] == 1 ? true : false;
}

int MyField::ChkRules(int num, int count)
{
	int answer = 0;
	if (num == 0 && count == 3)
		return 1;

	if (num == 1)
		if (count == 2 || count == 3)
			return 1;
		else if (count <= 1 || count >= 4)
			return 0;

	return answer;
}

void MyField::AddPotentialNeighbors(int index)
{
	if (index - row - 1 >= 0 && index % row != 0 && index >= row && RealField[index - row - 1] != 1)
		PotentialLive.push_back(index - row - 1);
	if (index - 1 >= 0 && index % row != 0 && RealField[index - 1] != 1)
		PotentialLive.push_back(index - 1);
	if (index + row - 1 < row * col && index % row != 0 && index < row*col - row &&
		RealField[index + row - 1] != 1)
		PotentialLive.push_back(index + row - 1);
	if (index - row >= 0 && index >= row && RealField[index - row] != 1)
		PotentialLive.push_back(index - row);
	if (index - row + 1 >= 0 && index >= row && (index + 1) % row != 0 && RealField[index - row + 1] != 1)
		PotentialLive.push_back(index - row + 1);
	if (index + 1 < row * col && (index + 1) % row != 0 && RealField[index + 1] != 1)
		PotentialLive.push_back(index + 1);
	if (index + row < row * col && index < row*col - row && RealField[index + row] != 1)
		PotentialLive.push_back(index + row);
	if (index + row + 1 < row * col && (index + 1) % row != 0 && index < row*col - row &&
		RealField[index + row + 1] != 1)
		PotentialLive.push_back(index + row + 1);
}

void MyField::ChangeParams(int rowN, int colN)
{
	for (int i = 0; i < rowN; i++)
		for (int j = 0; j < colN; j++)
			RealField.push_back(0);
}

bool MyField::isClickOnField(CPoint pos, int CurHScrollPos, int CurVScrollPos)
{
	return pos.x / RectSize + CurHScrollPos >= row  || pos.y / RectSize + CurVScrollPos >= col  ? false : true;
}