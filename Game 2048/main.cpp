#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <windows.h>
#include <fstream>
#include <conio.h>
#include <time.h>
#include <math.h>
using namespace std;

//Gia tri cua cac o trong bang 4x4
const string giatritrong1o[14] = {"    ","  2 ", "  4 ","  8 ", " 16 ", " 32 "," 64 ",
                         " 128", " 256"," 512", "1024", "2048","4096", "8192"};

//Ma tran 4x4 luu gia tri cac o trong bang
int Matrix[4][4];

//Ma tran toa do cac ki tu
int ToaDo[21][9];

int diem, diemcao, luachon = 1, phimnhan;

bool themSo;


bool ConOTrong();

// Hàm ẩn con trỏ nằm trên màn hình console
void ancontrodi(){
	CONSOLE_CURSOR_INFO Info;
	Info.bVisible = FALSE;
	Info.dwSize = 20;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Info);
}

void gotoxy( int column, int line )
{
  COORD coord;
  coord.X = column;
  coord.Y = line;
  SetConsoleCursorPosition(GetStdHandle( STD_OUTPUT_HANDLE ),coord);
}
// Hàm xóa màn hình.
void XoaManHinh(){
	HANDLE hOut;
	COORD Position;
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	Position.X = 0;
	Position.Y = 0;
	SetConsoleCursorPosition(hOut, Position);
}
void textcolor(int x){
	HANDLE mau;
	mau=GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(mau,x);
}

void Hinh1()
{
    gotoxy(23,20); cout <<"The Game";
    gotoxy(32,12); cout << "  ______    ______   __    __   ______   ";
    gotoxy(32,13);cout << " /      \\  /      \\ |  \\  |  \\ /      \\  ";
    gotoxy(32,14);cout << "|  $$$$$$\\|  $$$$$$\\| $$  | $$|  $$$$$$\\ ";
    gotoxy(32,15);cout << " \\$$__| $$| $$$\\| $$| $$__| $$| $$__/ $$ ";
    gotoxy(32,16);cout << " /      $$| $$$$\\ $$| $$    $$ >$$    $$ ";
    gotoxy(32,17);cout << "|  $$$$$$ | $$\\$$\\$$ \\$$$$$$$$|  $$$$$$  ";
    gotoxy(32,18);cout << "| $$_____ | $$_\\$$$$      | $$| $$__/ $$ ";
    gotoxy(32,19);cout << "| $$     \\ \\$$  \\$$$      | $$ \\$$    $$ ";
    gotoxy(32,20);cout << " \\$$$$$$$$  \\$$$$$$        \\$$  \\$$$$$$ ";
}
void Hinh2()
{
    ifstream filein;
    string kitu;
    char c;
    filein.open("nhaphinh.txt", ios_base::in);
    while(!filein.eof())
    {
		filein.get(c);
        cout << c;
    }
    filein.close();
}
void TaoKhung()
{
	int temp;
	ifstream in("Data.in");
	for (int j=0; j<9; j++)
    {
		for (int i=0; i<21; i++)
		{
			in >> temp;
			ToaDo[i][j] = temp;
		}
	}
}

void VeKhung()
{
	for (int j=0; j<9; j++)
    {
		for (int i=0; i<21; i++)
        {
			if( j % 2 != 0)
            {
                cout << (char)179;
            }
            else if(i % 5 == 0 && i != 0 && i != 20)
            {
                cout << (char) 197;
            }
            else if(i == 20 && j % 2 == 0)
            {
                cout << (char)180;
            }
            else if(i == 0 && j % 2 == 0)
            {
                cout << (char)195;
            }
            else cout <<(char)196;
		}
		cout <<endl;
	}
}
int TaoSo()
{
	int a=rand()%20;
	if (!a) return 2;	//dien gia tri 4. Xac suat la 5%
	else return 1;		//Dien gia tri 2. Xac suat la 95%
}

int ThemSo()
{
	int i, j;
	//Kiem tra xem con cho trong hay khong
	bool ConCho=false;
	for (int i=0; i<4; i++)
    {
		for (int j=0; j<4; j++)
		{
			if (!Matrix[i][j])
			{
				ConCho=true;
				break;
			}
		}
	}
	//Neu con cho thi chon cho trong ngau nhien de khoi tao gia tri
	if (ConCho)
	{
		do
		{
			i = rand()%4;
			j = rand()%4;
		}while (Matrix[i][j]);
		Matrix[i][j] = TaoSo();
	}
}
void InBang()
{
	int i, j;
	gotoxy(0,1);
	VeKhung(); //Ve khung truoc roi dien gia tri
	for (i=0; i<4; i++)
    {
		for (int j=0; j<4; j++)
		{
			gotoxy(1+i*5, 2+j*2);
			cout << giatritrong1o[Matrix[i][j]];
		}
	}
	//In diem nguoi choi va thong tin can biet
	gotoxy(30, 2); cout << "BEST: " << diemcao;
	gotoxy(30, 4); cout << "DIEM: " << diem;
	gotoxy(30, 6); cout << "Nhan [esc] de thoat";
	gotoxy(30, 8); cout << "Nhan cac phim [" << (char) 16 << "]" << "[" << (char) 17 << "]" << "[" << (char) 30 << "]" << "[" << (char) 31 << "]" << " de di chuyen";
}
void ResetLai()
{
    int i,j;
    //Khoi tao gia tri ban dau bang 0
	for (i=0; i<4; i++)
    {
		for (j=0; j<4; j++)
		{
			Matrix[i][j]=0;
		}
	}
}
bool ThuaCuoc()
{
	bool kq1, kq2;
	if (ConOTrong()) { kq1 = true; } //Con o trong thi true
	for (int i=0; i<3; i++)
    {
		for (int j=0; j<3; j++)
		{
			//Con 2 o canh nhau co cung gia tri thi true
			if (Matrix[i][j]==Matrix[i][j+1] || Matrix[i][j]==Matrix[i+1][j])
            {
				kq2 = true;
			}
		}
	}
	if ((!kq1) && (!kq2)) return true;
	else return false;
}

bool ConOTrong()    //Ktra con o trong khong?
{
	bool kq = false;
	for (int i=0; i<4; i++)
    {
		for (int j=0; j<4; j++)
		{
			if (!Matrix[i][j])
			{
				kq = true;
			}
		}
	}
	return kq;
}

void Down()
{
	int i, j, vt1, vt2, temp;
	for (i=0; i<4; i++)
    {
		//Gop het nhung o trong ben duoi
		for (j=3; j>0; j--)
        {
			if (!Matrix[i][j])
			{
				vt1=vt2=j;
				while (!Matrix[i][vt2])
				{
				    vt2--;
                    if (vt2==0) break;
                }
				if (vt2 || (!vt2 && Matrix[i][vt2]))
                {
					for (int k=vt2*2+3+1; k<vt1*2+3+1; k++)
					{
						gotoxy(i*5,vt2*2+2);
						cout << "    ";//Ghi chen so bi di chuyen nam trong ma tran khi InBang
						gotoxy(i*5, k);
                        cout << giatritrong1o[Matrix[i][vt2]];
						Sleep(5);
					}
					//Doi cho o trong voi o co gia tri trong ma tran
					temp = Matrix[i][vt1];
					Matrix[i][vt1] = Matrix[i][vt2];
					Matrix[i][vt2]= temp;
					themSo = true;
				}
			}
		}
		//Cong 2 o co cung gia tri
		for (j=3; j>0; j--)
        {
			if (Matrix[i][j]!=0 && Matrix[i][j]==Matrix[i][j-1])
			{
				Matrix[i][j]++;
				diem+=pow(2,Matrix[i][j]);
				Matrix[i][j-1] = 0;
				themSo = true;
			}
		}
		//Gop het nhung o trong ben duoi
		for (j=3; j>0; j--)
		{
			if (!Matrix[i][j])
			{
				vt1=vt2=j;
				while (!Matrix[i][vt2])
                {
				    vt2--;
                    if (vt2==0) break;
                }
				if (vt2 || (!vt2 && Matrix[i][vt2]))
				{
					for (int k=vt2*2+3+1; k<vt1*2+3+1; k++)
					{
						gotoxy(i*5,vt2*2+2);
						cout << "    ";//Ghi chen so bi di chuyen nam trong ma tran khi InBang
						gotoxy(i*5, k); cout << giatritrong1o[Matrix[i][vt2]];
						Sleep(5);
					}
					//Doi cho o trong voi o co gia tri trong ma tran
					temp = Matrix[i][vt1];
					Matrix[i][vt1] = Matrix[i][vt2];
					Matrix[i][vt2]= temp;
					themSo = true;
				}
			}
		}
	}
}
void Up()
{
	int i, j, vt1, vt2, temp;
	for (i=0; i<4; i++)
    {
		//Gop het nhung o trong ben tren
		for (j=0; j<3; j++)
		{
			if (!Matrix[i][j])
			{
				vt1=vt2=j;
				while (!Matrix[i][vt2])
                {
                    vt2++;
                    if (vt2==3) break;
                }
				if (vt2!=3 || (vt2==3 && Matrix[i][vt2]))
                {
					for (int k=vt2*2+3; k>vt1*2+3; k--)
					{
						gotoxy(i*5,vt2*2+4);
						cout << "    ";//Ghi chen so bi di chuyen nam trong ma tran khi InBang
						gotoxy(i*5, k);
                        cout << giatritrong1o[Matrix[i][vt2]];
						Sleep(5);
					}
					//Doi cho o trong voi o co gia tri trong ma tran
					temp = Matrix[i][vt1];
					Matrix[i][vt1] = Matrix[i][vt2];
					Matrix[i][vt2]= temp;
					themSo = true;
				}
			}
		}
		//Cong 2 o co cung gia tri
		for (j=0; j<3; j++)
		{
			if (Matrix[i][j]!=0 && Matrix[i][j]==Matrix[i][j+1])
            {
				Matrix[i][j]++;
				diem+=pow(2,Matrix[i][j]);
				Matrix[i][j+1] = 0;
				themSo = true;
			}
		}
		//Gop het nhung o trong ben tren
		for (j=0; j<3; j++)
        {
			if (!Matrix[i][j])
			{
				vt1=vt2=j;
				while (!Matrix[i][vt2])
                {
				    vt2++;
                    if (vt2==3) break;
                }
				if (vt2!=3 || (vt2==3 && Matrix[i][vt2]))
				{
					for (int k=vt2*2+3; k>vt1*2+3; k--)
					{
						gotoxy(i*5,vt2*2+4);
						XoaManHinh();
						cout << "    ";//Ghi chen so bi di chuyen nam trong ma tran khi InBang
						gotoxy(i*5, k);
						cout << giatritrong1o[Matrix[i][vt2]];
						Sleep(5);
					}
					//Doi cho o trong voi o co gia tri trong ma tran
					temp = Matrix[i][vt1];
					Matrix[i][vt1] = Matrix[i][vt2];
					Matrix[i][vt2]= temp;
					themSo = true;
				}
			}
		}
	}
}

void Right()
{
	int i, j, vt1, vt2, temp;
	for (j=0; j<4; j++)
    {
		//Gop het nhung o trong ben phai
		for (i=3; i>0; i--)
        {
			if (!Matrix[i][j])
			{
				vt1=vt2=i;
				while (!Matrix[vt2][j])
                {
                    vt2--;
                    if (vt2==0) break;
                }
				if (vt2 || (!vt2 && Matrix[vt2][j]))
				{
					for (int k=vt2*5+1; k<vt1*5+1; k++)
                    {
						gotoxy(vt2*5+1,j*2+2);
						cout << "    ";//Ghi chen so bi di chuyen nam trong ma tran khi InBang
						gotoxy(k, j*2+2);
						cout << giatritrong1o[Matrix[vt2][j]];
						Sleep(5);
					}
					//Doi cho o trong voi o co gia tri trong ma tran
					temp = Matrix[vt1][j];
					Matrix[vt1][j] = Matrix[vt2][j];
					Matrix[vt2][j]= temp;
					themSo = true;
				}
			}
		}
		//Cong 2 o co cung gia tri
		for (i=3; i>0; i--)
        {
			if (Matrix[i][j]!=0 && Matrix[i][j]==Matrix[i-1][j])
			{
				Matrix[i][j]++;
				diem+=pow(2,Matrix[i][j]);
				Matrix[i-1][j] = 0;
				themSo = true;
			}
		}
		//Gop het nhung o trong ben phai
		for (i=3; i>0; i--)
		{
			if (!Matrix[i][j])
			{
				vt1=vt2=i;
				while (!Matrix[vt2][j])
				{
				    vt2--;
                    if (vt2==0) break;
                }
				if (vt2 || (!vt2 && Matrix[vt2][j]))
                {
					for (int k=vt2*5+1; k<vt1*5+1; k++)
					{
						gotoxy(vt2*5+1,j*2+2);
						cout << "    ";//Ghi chen so bi di chuyen nam trong ma tran khi InBang
						gotoxy(k, j*2+2);
						cout << giatritrong1o[Matrix[vt2][j]];
						Sleep(5);
					}
					//Doi cho o trong voi o co gia tri trong ma tran
					temp = Matrix[vt1][j];
					Matrix[vt1][j] = Matrix[vt2][j];
					Matrix[vt2][j]= temp;
					themSo = true;
				}
			}
		}
	}
}

void Left()
{
	int i, j, vt1, vt2, temp;
	for (j=0; j<4; j++)
    {
		//Gop het nhung o trong ben trai
		for (i=0; i<3; i++)
		{
			if (!Matrix[i][j])
			{
				vt1=vt2=i;
				while (!Matrix[vt2][j])
                {
				    vt2++;
                    if (vt2==3) break;
                }
				if (vt2!=3 || (vt2==3 && Matrix[vt2][j]))
                {
					for (int k=vt2*5+1; k>vt1*5+1; k--)
					{
						gotoxy(vt2*5+1,j*2+2);
						cout << "    ";//Ghi chen so bi di chuyen nam trong ma tran khi InBang
						gotoxy(k, j*2+2);
						cout << giatritrong1o[Matrix[vt2][j]];
						Sleep(5);
					}
					//Doi cho o trong voi o co gia tri trong ma tran
					temp = Matrix[vt1][j];
					Matrix[vt1][j] = Matrix[vt2][j];
					Matrix[vt2][j]= temp;
					themSo = true;
				}
			}
		}
		//Cong 2 o co cung gia tri
		for (i=0; i<3; i++)
        {
			if (Matrix[i][j]!=0 && Matrix[i][j]==Matrix[i+1][j])
			{
				Matrix[i][j]++;
				diem+=pow(2,Matrix[i][j]);
				Matrix[i+1][j] = 0;
				themSo = true;
			}
		}
		//Gop het nhung o trong ben trai
		for (i=0; i<3; i++)
        {
			if (!Matrix[i][j])
			{
				vt1=vt2=i;
				while (!Matrix[vt2][j])
                {
				    vt2++;
                    if (vt2==3) break;
                }
				if (vt2!=3 || (vt2==3 && Matrix[vt2][j]))
				{
					for (int k=vt2*5+1; k<vt1*5+1; k++)
					{
						gotoxy(vt2*5+1,j*2+2);
						cout << "    ";//Ghi chen so bi di chuyen nam trong ma tran khi InBang
						gotoxy(k, j*2+2);
						cout << giatritrong1o[Matrix[vt2][j]];
						Sleep(5);
					}
					//Doi cho o trong voi o co gia tri trong ma tran
					temp = Matrix[vt1][j];
					Matrix[vt1][j] = Matrix[vt2][j];
					Matrix[vt2][j]= temp;
					themSo = true;
				}
			}
		}
	}
}

void XuLy()
{
	int i, j;
	ifstream inScore("Score.in");
	inScore >> diemcao;
	inScore.close();
	bool KiTuKep=false;//Do phim mui ten la ki tu kep, KiTuKep de loai bo ki tu kep phat sinh khi nhap
	char move;
	ResetLai();
	ThemSo();
	do
	{
		InBang();
		move=getch();//Doc 1 ki tu tu ban phim
		int x=(int)move;
		themSo = false;
		switch(x)
		{
			case 80:Down();	break;
			case 72:Up(); break;
			case 77:Right(); break;
			case 75:Left();	break;
			default: KiTuKep = true; break;
		}
		if (themSo) ThemSo();
		if (x==27)
        {
            break;
		}
		if (KiTuKep)
        {
			KiTuKep=false;
			continue;
		}//Neu co KiTuKep thi continue de bo qua viec ThemSo
	} while (!ThuaCuoc());
	system("cls");
	InBang();
	if (diem > diemcao)
    {
		ofstream outScore("Score.in");
		outScore << diem;
		outScore.close();
		cout << "   --->> Ki luc moi!!!";
	}
}
void XuLyChoiGame()
{
    srand((unsigned)time(NULL));
	char chon ='c';
    ancontrodi();
	while (chon=='c')
    {
		diem = 0;
		system("cls");
		Hinh1();
		TaoKhung();
		XuLy();
		do
		{
		    system("cls");
			gotoxy(30,8); cout << "BAN CO MUON TIEP TUC?";
			gotoxy(30,9); cout << "Co/Khong (c/k): ";
			chon = getch();
		} while(!(chon=='c' || chon=='k'));
	}
	system("cls");
}
void HuongDan()
{
	cout << "Huong dan:\n\n\t-Su dung cac nut dieu huong de di chuyen:\n" << endl
		 << "\t\t+ Len: "<< (char)24 << endl << endl
		 << "\t\t+ Xuong: "<< (char)25 << endl << endl
		 << "\t\t+ Phai: " << (char)26 << endl << endl
		 << "\t\t+ Trai: "<< (char)27 << endl << endl
		 << "\t--> Nhan ESC de thoat tro choi!\n\n";
	system("pause");
}

int main()
{
     XuLyChoiGame();
	return 0;
}
