#include <iostream>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>

#define DoDai 37
#define DoCao 20
#define MinX 2
#define MinY 2
#define MaxX 36
#define MaxY 19

using namespace std;

int Gach[49][2] =
{
		{2,2},{2,7},{2,12},{2,17},{2,22},{2,27},{2,32},
		{3,2},{3,7},{3,12},{3,17},{3,22},{3,27},{3,32},
		{4,2},{4,7},{4,12},{4,17},{4,22},{4,27},{4,32},
		{5,2},{5,7},{5,12},{5,17},{5,22},{5,27},{5,32},
		{6,2},{6,7},{6,12},{6,17},{6,22},{6,27},{6,32},
		{7,2},{7,7},{7,12},{7,17},{7,22},{7,27},{7,32},
		{8,2},{8,7},{8,12},{8,17},{8,22},{8,27},{8,32}
};
int SoGachConLai = 49;

// 1 - hien thi, 0- an di
int HienThiGach[49] =
{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 };

int HienThiGach2[49] =
{ 1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1 };

int HienThiGach3[49] =
{ 1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,0,1,0,1,0,1,1,0,0,1,0,0,1,1,0,1,0,1,0,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1 };

int HienThiGach4[49] =
{ 0,0,0,0,0,0,1,1,0,0,0,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,0,0,0,1,1,0,0,0,0,0,0 };

int HienThiGach5[49] =
{ 1,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,1,0,0,0,0,1,1,1,1,0,0,0,1,1,1,1,1,0,0,1,1,1,1,1,1,0,1,1,1,1,1,1,1 };

int HienThiGach6[49] =
{ 0,0,0,1,0,0,0,0,0,1,1,1,0,0,0,1,1,1,1,1,0,1,1,1,1,1,1,1,0,1,1,1,1,1,0,0,0,1,1,1,0,0,0,0,0,1,0,0,0 };

int HienThiGach7[49] =
{ 1,1,1,0,1,1,1,1,1,1,0,1,1,1,1,1,1,0,1,1,1,1,1,1,0,1,1,1,1,1,1,0,1,1,1,1,1,1,0,1,1,1,1,1,1,0,1,1,1 };

int HienThiGach8[49] =
{ 0,0,0,1,0,0,0,0,0,1,1,1,0,0,0,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,0,0,0,0,0,1 };

int HienThiGach9[49] =
{ 0,1,1,0,1,1,0,1,1,0,1,0,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,0,0,0,1,1,1,0,0,0,0,0,1,0,0,0 };

int ViTriThanhDo[2] = {18,15}, ViTriBong[2] = {17,19};
int BatDau = 0, huong = 1;
int thang = 0, thua = 0, diem = 0;

void gotoxy(int x, int y)
{
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD CursorPosition;

	CursorPosition.X = x;
	CursorPosition.Y = y;
	SetConsoleCursorPosition(console, CursorPosition);
}

void TaoVaInKhung()
{
	gotoxy(0,0);cout<<"**************************************";
	gotoxy(0,DoCao);cout<<"**************************************";

	for(int i=0; i < DoCao; i++){
		gotoxy(0,i); cout<<"*";
		gotoxy(DoDai,i); cout<<"*";
	}
}

void TaoVaInGach(int lv)
{
	for( int i=0; i<50; i++)
	{
	    if(lv == 1)
        {
            if(HienThiGach[i] == 1 )
            {
                gotoxy(Gach[i][1], Gach[i][0]);
                cout<<"####";
            }
        }
        else if(lv == 2)
        {
            if(HienThiGach2[i] == 1 )
            {
                gotoxy(Gach[i][1], Gach[i][0]);
                cout<<"####";
            }
        }
        else if(lv == 3)
        {
            if(HienThiGach3[i] == 1 )
            {
                gotoxy(Gach[i][1], Gach[i][0]);
                cout<<"####";
            }
        }
        else if(lv == 4)
        {
            if(HienThiGach4[i] == 1 )
            {
                gotoxy(Gach[i][1], Gach[i][0]);
                cout<<"####";
            }
        }
        else if(lv == 5)
        {
            if(HienThiGach5[i] == 1 )
            {
                gotoxy(Gach[i][1], Gach[i][0]);
                cout<<"####";
            }
        }
        else if(lv == 6)
        {
            if(HienThiGach6[i] == 1 )
            {
                gotoxy(Gach[i][1], Gach[i][0]);
                cout<<"####";
            }
        }
        else if(lv == 7)
        {
            if(HienThiGach7[i] == 1 )
            {
                gotoxy(Gach[i][1], Gach[i][0]);
                cout<<"####";
            }
        }
        else if(lv == 8)
        {
            if(HienThiGach8[i] == 1 )
            {
                gotoxy(Gach[i][1], Gach[i][0]);
                cout<<"####";
            }
        }
        else if(lv == 9)
        {
            if(HienThiGach9[i] == 1 )
            {
                gotoxy(Gach[i][1], Gach[i][0]);
                cout<<"####";
            }
        }
	}
}

void BongChuaChamGach()
{
	if( ViTriBong[1] >= ViTriThanhDo[1] && ViTriBong[1] <= ViTriThanhDo[1]+8)
	{
		if( ViTriBong[0] == ViTriThanhDo[0]-1 )
		{
			if( huong == 3 )
				huong = 2;
			else if( huong == 4 )
				huong = 1;
		}
	}
}

void BongChamGach(int lv)
{
	for( int i=0; i<50; i++)
	{
	    if(lv == 1)
        {

            if(HienThiGach[i]==1)
            {
                if( ViTriBong[1] >= Gach[i][1] && ViTriBong[1] < Gach[i][1]+8)
                {
                    if( ViTriBong[0] == Gach[i][0] )
                    {
                        HienThiGach[i] = 0;
                        SoGachConLai--;
                        diem+=10;
                    }
                }
            }
        }
        else if(lv == 2)
        {
            if(HienThiGach2[i]==1)
            {
                if( ViTriBong[1] >= Gach[i][1] && ViTriBong[1] < Gach[i][1]+8)
                {
                    if( ViTriBong[0] == Gach[i][0] )
                    {
                        HienThiGach2[i] = 0;
                        SoGachConLai--;
                        diem+=11;
                    }
                }
            }
        }
        else if(lv == 3)
        {
            if(HienThiGach3[i]==1)
            {
                if( ViTriBong[1] >= Gach[i][1] && ViTriBong[1] < Gach[i][1]+8)
                {
                    if( ViTriBong[0] == Gach[i][0] )
                    {
                        HienThiGach3[i] = 0;
                        SoGachConLai--;
                        diem+=12;
                    }
                }
            }
        }
        else if(lv == 4)
        {
            if(HienThiGach4[i]==1)
            {
                if( ViTriBong[1]>=Gach[i][1] && ViTriBong[1]< Gach[i][1]+8)
                {
                    if( ViTriBong[0] == Gach[i][0] )
                    {
                        HienThiGach4[i] = 0;
                        SoGachConLai--;
                        diem+=13;
                    }
                }
            }
        }
        else if(lv == 5)
        {
            if(HienThiGach5[i]==1)
            {
                if( ViTriBong[1]>=Gach[i][1] && ViTriBong[1]< Gach[i][1]+8)
                {
                    if( ViTriBong[0] == Gach[i][0] )
                    {
                        HienThiGach5[i] = 0;
                        SoGachConLai--;
                        diem+=14;
                    }
                }
            }
        }
        else if(lv == 6)
        {
            if(HienThiGach6[i]==1)
            {
                if( ViTriBong[1]>=Gach[i][1] && ViTriBong[1]< Gach[i][1]+8)
                {
                    if( ViTriBong[0] == Gach[i][0] )
                    {
                        HienThiGach6[i] = 0;
                        SoGachConLai--;
                        diem+=15;
                    }
                }
            }
        }
        else if(lv == 7)
        {
            if(HienThiGach7[i]==1)
            {
                if( ViTriBong[1]>=Gach[i][1] && ViTriBong[1]< Gach[i][1]+8)
                {
                    if( ViTriBong[0] == Gach[i][0] )
                    {
                        HienThiGach7[i] = 0;
                        SoGachConLai--;
                        diem+=16;
                    }
                }
            }
        }
        else if(lv == 8)
        {

            if(HienThiGach8[i]==1)
            {
                if( ViTriBong[1]>=Gach[i][1] && ViTriBong[1]< Gach[i][1]+8)
                {
                    if( ViTriBong[0] == Gach[i][0] )
                    {
                        HienThiGach8[i] = 0;
                        SoGachConLai--;
                        diem+=17;
                    }
                }
            }
        }
        else if(lv == 9)
        {
            if(HienThiGach9[i]==1)
            {
                if( ViTriBong[1]>=Gach[i][1] && ViTriBong[1]< Gach[i][1]+8)
                {
                    if( ViTriBong[0] == Gach[i][0] )
                    {
                        HienThiGach9[i] = 0;
                        SoGachConLai--;
                        diem+=20;
                    }
                }
            }
        }
	}
}
void HoanThanh1Level()
{
    system("cls");
    for (int i = 0; i < 6; i++)
    {
        system("color 70");
        Sleep(50);
        system("color 07");
        Sleep(50);
    }
}
void Play()
{
    int level = 1;
	while(1)
	{
		system("cls");
		TaoVaInGach(level);
		TaoVaInKhung();

		gotoxy(ViTriThanhDo[1],ViTriThanhDo[0]);
		cout<<"==========";

		gotoxy(ViTriBong[1],ViTriBong[0]);
		cout<<"O";

        gotoxy(50,5); cout << "Diem: " << diem;
        gotoxy(50,7); cout << "Cap do: " << level;
        gotoxy(50,8); cout << "So gach con lai: " << SoGachConLai;

		if(kbhit())
		{
			char ch = getch();
			if(ch==77 )
			{
				if(ViTriThanhDo[1] < 26)
					ViTriThanhDo[1] = ViTriThanhDo[1]+2;
			}
			if( ch==75 )
			{
				if(ViTriThanhDo[1] > 2)
					ViTriThanhDo[1] = ViTriThanhDo[1]- 2;
			}
			if(ch==32)
			{
				BatDau = 1;
			}
			if(ch==27)
			{
				break;
			}
		}

		if( BatDau == 1 )
		{
			if( huong == 1)
			{ // hướng trên phải
				ViTriBong[0] = ViTriBong[0] - 1;
				ViTriBong[1] = ViTriBong[1] + 2 ;
				if( ViTriBong[1] > MaxX )
				{
					huong = 2;
				}
				else if( ViTriBong[0] < MinY )
				{
					huong = 4;
				}
			}
			else if( huong == 2)
			{ // hướng trên trái
				ViTriBong[0] = ViTriBong[0] - 1;
				ViTriBong[1] = ViTriBong[1] - 2;
				if( ViTriBong[0] < MinY )
				{
					huong = 3;
				}
				else if( ViTriBong[1] < MinX  )
				{
					huong = 1;
				}
			}
			else if( huong == 3)
			{ // hướng dưới trái
				ViTriBong[0] = ViTriBong[0] + 1;
				ViTriBong[1] = ViTriBong[1] - 2;

				if( ViTriBong[0] > MaxY )
				{
					thua = 1;
					break;
				}
				else if( ViTriBong[1] < MinX  )
				{
					huong = 4;
				}
			}
			else if( huong == 4)
			{ // hướng dưới phải
				ViTriBong[0] = ViTriBong[0] + 1;
				ViTriBong[1] = ViTriBong[1] + 2;
				if( ViTriBong[1] > MaxX){
					huong = 3;
				}
				else if( ViTriBong[0] > MaxY){
					thua = 1;
					break;
				}
			}

			BongChuaChamGach();
			BongChamGach(level);
		}

		if(SoGachConLai == 0)
		{
			level++;
			HoanThanh1Level();
			if(level == 2)
            {
                SoGachConLai = 40;
            }
            else if(level == 3)
            {
                SoGachConLai = 33;
            }
            else if(level == 4)
            {
                SoGachConLai = 29;
            }
            else if(level == 5)
            {
                SoGachConLai = 28;
            }
            else if(level == 6)
            {
                SoGachConLai = 25;
            }
            else if(level == 7)
            {
                SoGachConLai = 42;
            }
            else if(level == 8)
            {
                SoGachConLai = 29;
            }
            else if(level == 9)
            {
                SoGachConLai = 31;
            }
            else if (level == 10)
            {
                thang = 1;
                break;
            }
		}

		Sleep(42);
	}

	if( thua == 1)
	{
		system("cls");
		gotoxy(10,6); cout<<"THUA ROI, AHUHU";
		gotoxy(10,7);cout << "diem cua ban: " << diem;
		Sleep(2000);
	}

	if( thang == 1)
	{
		system("cls");
		gotoxy(10,6); cout<<"THANG ROI NE, AHIHI";
		gotoxy(10,7);cout << "diem cua ban: " << diem;
		Sleep(2000);
		system("cls");
	}
}

int main()
{
	Play();
	return 0;
}
