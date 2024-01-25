// kết hợp 2 ngôn ngữ c/c++

#include <iostream>
#include <cstdio>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <string>

using namespace std;

void Menu();
bool ThuaGame, dangdichuyen, xephatno, dead;
int height = 20, width = 19, speed, myCarX, myCarY, tocdogame = 45, sucben = 0, mauxe = 15; // 6,11,12
int diem = 0, diemcao[5], highestScore[5], mangsong = 2, m = 30, sodu, diemdedoi, tempdiem;
int vitrikethuX, vitrikethuY, soluongkethu, kethuX[4], kethuY[4] = {-8, -18, -28, -38};
int luachon = 1, phimnhan = 1, sohuuxe1 = 0,sohuuxe2 = 0,sohuuxe3 = 0,temp_tocdogame = 0,temp_tocdogame1 = 0, temp_tocdogame2 = 0;
char a[21][31];
FILE *h;

string hatbomno1[4] = {"o   o"," ooo "," ooo ", "o   o"};
string hatbomno2[4] = {" ooo ","o   o","o   o", " ooo "};
string myCar[3] = {"O^O","|X|","O-O"};
string myCar1[3] = {"@^@","|W|","@=@"};


// Hàm dịch chuyển con trỏ đến tọa độ x, y.
void gotoxy(int x,int y)
{
	COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void hideCursor()
{
    CONSOLE_CURSOR_INFO cursor;
    cursor.dwSize = 100;
    cursor.bVisible = false;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);
}

void ToMau(int x)
{
    HANDLE mau;
    mau = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(mau, x);
}

void HoanVi(int &a, int &b)
{
    int temp = a;
    a = b;
    b = temp;
}

void TaoCacFileCanThiet()
{
    h = fopen("luutru.txt", "r");
    if (h == NULL)
    {
        h = fopen("luutru.txt", "w");
        fclose(h);
    }
    luachon = 1;
}
void NhapDiemCao(const char *x)
{
    h = fopen(x, "r");
    fscanf(h, "%d ", &diemdedoi);
    fscanf(h, "%d ", &sodu);
    for (int i = 0; i < 5; i++)
    {
        fscanf(h, "%d", &diemcao[i]);
    }
    fscanf(h, "%d ", &sohuuxe1);
    fscanf(h, "%d ", &sohuuxe2);
    fscanf(h, "%d ", &sohuuxe3);
    fclose(h);
}
void XuatDiemCao(const char *x)
{
    h = fopen(x, "w");
    fprintf(h, "%d\n", diemdedoi);
    fprintf(h, "%d\n", sodu);
    for (int i = 0; i < 5; i++)
    {
        fprintf(h, "%d\n", diemcao[i]);
    }
    fprintf(h, "%d ", sohuuxe1);
    fprintf(h, "%d ", sohuuxe2);
    fprintf(h, "%d ", sohuuxe3);
    fclose(h);
}
void KhoiTao()
{
    srand((unsigned)time(NULL));
    dangdichuyen = false;
    ThuaGame = false;
    dead = false;
    myCarX = 8;
    myCarY = 17;
    speed = 1;
    soluongkethu = 3;

    for(int i =0;i<soluongkethu;++i)
    {
        vitrikethuX = rand() % 3;
        if(vitrikethuX == 0)
        {
            kethuX[i] = 2;
        }
        else if(vitrikethuX == 1)
        {
            kethuX[i] = 8;
        }
        else if(vitrikethuX == 2)
        {
            kethuX[i] = 14;
        }
    }
    kethuY[0] = -8;
    kethuY[1] = -18;
    kethuY[2] = -18;
    kethuY[3] = -28;
}
void XoaVaTaoDuongDi()
{
    for(int i =0;i<height;++i)
    {
        gotoxy(0,i); cout << (char)186 <<"                 " << (char)186;
        if(i%2 == 0 && dangdichuyen)
        {
            gotoxy(6,i); cout << "|     |";
        }
        else if(i%2!=0 && !dangdichuyen)
        {
            gotoxy(6,i); cout << "|     |";
        }
    }
}
void XuatThongTin()
{
    gotoxy(22,19); cout << "[" << (char) 16 << "]" << "[" << (char) 17 << "]" << "[" << (char) 30 << "]" << "[" << (char) 31 << "]" << ": di chuyen xe \t [Esc]: Quay lai";
    gotoxy(25,2); cout << "Suc ben: " << sucben;
    gotoxy(25,3); cout << "Mang song: " << mangsong;
    gotoxy(25,4); cout << "Diem: " << diem;
}
void TaoXePlayer()
{
    if(!dead)
    {
        for(int i=0;i<3;++i)
        {
            ToMau(mauxe);
            gotoxy(myCarX,myCarY+i); cout << myCar[i];
        }
        ToMau(15);
    }
    else
    {
        for(int i =0;i<3;++i)
        {
            gotoxy(myCarX,myCarY+i); cout << "     ";
        }
    }
}

void TaoXeMenu(int x, int y, int soluong)
{
    if(soluong == 4)
    {
        ToMau(6);
    }
    if(soluong == 3)
    {
        ToMau(6);
    }
    while(soluong--)
    {
        for(int i=0;i<3;++i)
        {
            gotoxy( x,i + y); cout << myCar[i];
        }
        x+= 10;
        if(soluong == 2)
        {
            ToMau(11);
        }
        else if(soluong == 1)
        {
            ToMau(12);
        }
        else ToMau(15);
    }
}

void TaoXeChuongNgai()
{
    //{"O^O","|H|","O-O"};
    for(int i=0;i<soluongkethu;++i)
    {
        if(kethuY[i] + 2 > 0 )
        {
            gotoxy(kethuX[i], kethuY[i]+2);
            cout << myCar1[2];
        }

        if(kethuY[i] + 1 > 0)
        {
            gotoxy(kethuX[i], kethuY[i]+1);
            cout << myCar1[1];
        }
        if(kethuY[i] > 0)
        {
            gotoxy(kethuX[i], kethuY[i]);
            cout << myCar1[0];
        }
        if(kethuY[i] + 2 >= 20)
        {
            gotoxy(kethuX[i], kethuY[i]+2);
            cout << "   ";
        }
        if(kethuY[i] + 1 >= 20)
        {
            gotoxy(kethuX[i], kethuY[i]+1);
            cout << "   ";
        }
        if(kethuY[i] >= 20)
        {
            gotoxy(kethuX[i], kethuY[i]);
            cout << "   ";
        }

    }
}

void XuLy()
{
    srand((unsigned)time(NULL));

    if(dangdichuyen) dangdichuyen = false;
    else dangdichuyen = true;

    for(int i=0;i<soluongkethu;++i)
    {
        kethuY[i]++;
    }

    for(int i =0;i<soluongkethu;++i)
    {
        if(kethuY[i] > 21)
        {
            vitrikethuX = rand() % 3;
            if(vitrikethuX == 0)
            {
                kethuX[i] = 2;
            }
            else if(vitrikethuX == 1)
            {
                kethuX[i] = 8;
            }
            else if(vitrikethuX == 2)
            {
                kethuX[i] = 14;
            }

            kethuY[i] = 1;
            diem++;
        }
    }
    for(int i =0;i<soluongkethu;++i)
    {
        if((myCarY <= kethuY[i] + 3 && myCarY >= kethuY[i] || kethuY[i] >= myCarY && kethuY[i] <= myCarY + 3) && myCarX == kethuX[i])
        {
            dead = true;
        }
    }
}
void NoXe()
{
    if(xephatno)
    {
        for(int i=0;i<4;++i)
        {
            gotoxy(myCarX - 1, myCarY - 1 + i);
            cout << hatbomno1[i];
        }
        xephatno = false;
    }
    else
    {
        for(int i =0;i<4;++i)
        {
            gotoxy(myCarX - 1, myCarY - 1+i);
            cout << hatbomno2[i] << endl;
        }
        xephatno = true;
    }
    Sleep(150);
}

void XuLyDungCham()
{
    if(dead)
    {
        cout << "\a";
        if(sucben > 0)
        {
            sucben--;
        }
        else mangsong--;
        int dem =0;
        while(dem != 10)
        {
            ++dem;
            NoXe();
        }
        highestScore[mangsong] = diem;
        KhoiTao();
    }
}
void CapNhatLai()
{
    luachon = 1;
    phimnhan = 1;
    mangsong = 2;
    diem = 0;
}
void LuuLaiDiemSo()
{
    NhapDiemCao("luutru.txt");
    diemdedoi += diem;
    if (diem <= diemcao[4])
    {
        gotoxy(25, 6);
        cout << "Diem cua ban: " << diem << endl << endl;
    }
    else if ((diem > diemcao[4]) && (diem <= diemcao[0]))
    {
        gotoxy(25, 6);
        cout << "Diem cao: "<< diem <<endl<<endl;
    }
    else
    {
        gotoxy(25, 6);
        cout << "Diem cao nhat: "<< diem<<endl<<endl;
    }
    if (diem > diemcao[4])
    {
        diemcao[4] = diem;
    }
    for (int i = 0; i < 4; i++)
    {
        for (int j = i + 1; j < 5; j++)
        {
            if (diemcao[i] < diemcao[j])
            {
                HoanVi(diemcao[i], diemcao[j]);
            }
        }
    }
    XuatDiemCao("luutru.txt");
}
void HinhNen()
{
    gotoxy(25,10);cout << "    ____  __  _____       _  __ ______";
    gotoxy(25,11);cout << "   / __ \\/ / / /   |     | |/ // ____/";
    gotoxy(25,12);cout << "  / / / / / / / /| |     |   // __/   ";
    gotoxy(25,13);cout << " / /_/ / /_/ / ___ |    /   |/ /___   ";
    gotoxy(25,14);cout << "/_____/\\____/_/  |_|   /_/|_/_____/   ";
}
void Hinh2()
{
    gotoxy(15,1);cout<<"     _	";
    gotoxy(15,2);cout<<"  __| |   _  _    __ _      o O O  __ __    ___  ";
    gotoxy(15,3);cout<<" / _` |  | +| |  / _` |    o       \\ \\ /   / -_) ";
    gotoxy(15,4);cout<<" \\__,_|   \\_,_|  \\__,_|   TS__[O]  /_\\_\\   \\___| ";
    gotoxy(15,5);cout<<"_|`````|_|`````|_|`````| {======|_|`````|_|`````|";
    gotoxy(15,6);cout<<"``-0-0-'*`-0-0-'*`-0-0-'./o--000'*`-0-0-'*`-0-0-'";
}
void TroChoiMoi()
{
    hideCursor();   // an con tro di
    int move1;
    KhoiTao();
    while(!dead)
    {
        HinhNen();
        XoaVaTaoDuongDi();
        XuatThongTin();

        if(_kbhit())
        {
            move1 = getch();
            if(move1 == 72)// di len
            {
                if(myCarY > 0 && !dead)
                {
                    myCarY = myCarY - speed;
                }
            }
            else if(move1 == 75)// sang trai
            {
                if(myCarX > 2 && !dead)
                {
                    myCarX = myCarX - 6;
                }
            }
            else if(move1 == 77)// sang phai
            {
                if(myCarX < 14 && !dead)
                {
                    myCarX = myCarX + 6;
                }
            }
            else if(move1 == 80)// di xuong
            {
                if(myCarY < 17 && !dead)
                {
                    myCarY = myCarY + speed;
                }
            }
            else if(move1 == 27)
            {
                break;
            }
        }
        XuLy();
        TaoXePlayer();
        TaoXeChuongNgai();
        XuLyDungCham();
        if(mangsong < 1)
        {
            system("cls");
            LuuLaiDiemSo();
            ThuaGame = true;
            system("pause");
            CapNhatLai();
            KhoiTao();
            break;
        }
        Sleep(tocdogame);
        if(diem > 10 && diem < 100)
        {// 15 - 20 - 30
            tocdogame = 40 - temp_tocdogame;    // 45 - 10 = 35
        }
        else if(diem > 100 && diem < 500)
        {
            tocdogame = 35 - temp_tocdogame1; // 35 - 10 = 25
        }
        else if(diem > 500)
        {
            tocdogame = 30 - temp_tocdogame2; // 30 - 10 = 20
        }
    }
}

void TaoKhungVienMenu()
{
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 30; j++)
        {
            if ((i == 0) || (i == 10 - 1))
            {
                a[i][j] = (char)196;
            }
            else if ((j == 0) || (j == 29))
            {
                a[i][j] = (char)179;
            }
            else
            {
                a[i][j] = ' ';
            }
        }
    }
    a[0][0] = (char)218;
    a[0][29] = (char)191;
    a[10 - 1][0] = (char)192;
    a[10 - 1][29] = (char)217;
}
void InKhungVienMenu()
{
    for (int i = 0; i < 10; i++)
    {
        gotoxy(20, i + 8);
        for (int j = 0; j < 30; j++)
        {
            printf("%c", a[i][j]);
            printf("\u001b[0m");
        }
        printf("\n");
    }
}
void TaoKhungVienMenu2()
{
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            if ((i == 0) || (i == 5 - 1))
            {
                a[i][j] = (char)196;
            }
            else if ((j == 0) || (j == 6))
            {
                a[i][j] = (char)179;
            }
            else
            {
                a[i][j] = ' ';
            }
        }
    }
    a[0][0] = (char)218;
    a[0][6] = (char)191;
    a[5 - 1][0] = (char)192;
    a[5 - 1][6] = (char)217;
}
void InKhungVienMenu2(int x, int y)
{
    for (int i = 0; i < 5; i++)
    {
        gotoxy(x, i + y);
        for (int j = 0; j < 7; j++)
        {
            printf("%c", a[i][j]);
            printf("\u001b[0m");
        }
        printf("\n");
    }
}
void thongtinxe()
{
    if(m == 20)
    {
        gotoxy(31,9); cout << "XE CAM";
        gotoxy(21,10); cout << "- Mang song: 3";
        gotoxy(21,11); cout << "- Toc do: 0.15s";
        gotoxy(21,12); cout << "- Suc ben: 0";
        gotoxy(21,13); cout << "- Exp: +5";
        gotoxy(21,14); cout << "- Tuy yeu ve the xac, nhung";
        gotoxy(21,15); cout << "lai so huu toc do kha nhanh.";
        gotoxy(40,16); cout << "=> 500$";
    }
    else if(m == 30)
    {
        gotoxy(31,9); cout << "XE XANH";
        gotoxy(21,10); cout << "- Mang song: 3";
        gotoxy(21,11); cout << "- Toc do: 0.20s";
        gotoxy(21,12); cout << "- Suc ben: 1";
        gotoxy(21,13); cout << "- Exp: +15";
        gotoxy(21,14); cout << "- Noi cong, than xac va toc";
        gotoxy(21,15); cout << "do kha dong deu.";
        gotoxy(40,16); cout << "=> 900$";
    }

    else if(m == 40)
    {
        gotoxy(31,9); cout << "XE DO";
        gotoxy(21,10); cout << "- Mang song: 4";
        gotoxy(21,11); cout << "- Toc do: 0.30s";
        gotoxy(21,12); cout << "- Suc ben: 1";
        gotoxy(21,13); cout << "- Exp: +10";
        gotoxy(21,14); cout << "- Than xac trau bo, nhung";
        gotoxy(21,15); cout << "toc do kha rua bo.";
        gotoxy(40,16); cout << "=> 1200$";
    }
}

void ThongTinQuydoi()
{
    system("cls");
    cout << "\nTai day, voi so diem da tich luy duoc, ban co the doi sang so du de nang cap chiec xe cua minh.";
    cout << "\n\nVoi moi so diem, ban se quy doi voi so du cua minh voi le phi la: 10% / 1 lan quy doi.";
    cout << "\n\n - 10 diem -> 9$.\n\n - 300 diem -> 270$. \n\n - 900 diem -> 810$. \n\n - ...";
    cout << endl << endl << endl;
    for(int i = 3;i>=1;--i)
    {
        gotoxy(20,15); cout << "Xin vui long doi " << i;
        Sleep(1000);
    }
    luachon = 1;
}

void QuyDoiSoDu()
{
    NhapDiemCao("luutru.txt");
    ThongTinQuydoi();
    while(1)
    {
        system("cls");
        gotoxy(5,2); cout << "So diem doi: " << diemdedoi;
        gotoxy(5,3); cout << "So du: " << sodu << "$";
        gotoxy(20, 7); cout << "Ban muon quy doi (Le phi 10%): ";

        if (luachon == 1)
        {
            gotoxy(22, 9);
            printf("%c", 16);
            ToMau(180);
            gotoxy(26, 9);
            printf("10 diem -> 9$%4s\u001b[0m\n", "");
        }
        else
        {
            gotoxy(26, 9);
            printf("10 diem -> 9$%4s\u001b[0m\n", "");
        }
        if (luachon == 2)
        {
            gotoxy(22, 10);
            printf("%c", 16);
            ToMau(180);
            gotoxy(26, 10);
            printf("40 diem -> 36$%3s\u001b[0m\n", "");
        }
        else
        {
            gotoxy(26, 10);
            printf("40 diem -> 36$%3s\u001b[0m\n", "");
        }
        if (luachon == 3)
        {
            gotoxy(22, 11);
            printf("%c", 16);
            ToMau(180);
            gotoxy(26, 11);
            printf("70 diem -> 63$%3s\u001b[0m\n", "");
        }
        else
        {
            gotoxy(26, 11);
            printf("70 diem -> 63$%3s\u001b[0m\n", "");
        }
        if (luachon == 4)
        {
            gotoxy(22, 12);
            printf("%c", 16);
            ToMau(180);
            gotoxy(26, 12);
            printf("300 diem -> 270$%1s\u001b[0m\n", "");
        }
        else
        {
            gotoxy(26, 12);
            printf("300 diem -> 270$%1s\u001b[0m\n", "");
        }
        if (luachon == 5)
        {
            gotoxy(22, 13);
            printf("%c", 16);
            ToMau(180);
            gotoxy(26, 13);
            printf("600 diem -> 540$%1s\u001b[0m\n", "");
        }
        else
        {
            gotoxy(26, 13);
            printf("600 diem -> 540$%1s\u001b[0m\n", "");
        }
        if (luachon == 6)
        {
            gotoxy(22, 14);
            printf("%c", 16);
            ToMau(180);
            gotoxy(26, 14);
            printf("900 diem -> 810$%1s\u001b[0m\n", "");
        }
        else
        {
            gotoxy(26, 14);
            printf("900 diem -> 810$%1s\u001b[0m\n", "");
        }
        gotoxy(15, 20);
        printf("[%c] [%c]: Chon muc \t [enter]: Xac nhan\t [esc]: Quay lai", 30, 31);
        phimnhan = getch();
        if (phimnhan == 72)
        {
            luachon--;
        }
        else if (phimnhan == 80)
        {
            luachon++;
        }

        else if(phimnhan == 27)
        {
            luachon = 1;
            break;
        }
        else if(phimnhan == 13)
        {
            if (luachon == 1)
            {
                if(diemdedoi < 1 || diemdedoi < 10)
                {
                    system("cls");
                    gotoxy(15,10); cout << "So diem khong du!";
                    Sleep(1000);
                }
                else
                {
                    diemdedoi -= 10;
                    sodu += 9;
                    system("cls");
                    gotoxy(15,10); cout << "Quy doi thanh cong!";
                    Sleep(1000);
                }

            }
            else if (luachon == 2)
            {
               if(diemdedoi < 40)
                {
                    system("cls");
                    gotoxy(15,10); cout << "So diem khong du!";
                    Sleep(1000);
                }
                else
                {
                    diemdedoi -= 40;
                    sodu += 36;
                    system("cls");
                    gotoxy(15,10); cout << "Quy doi thanh cong!";
                    Sleep(1000);
                }
            }
            else if (luachon == 3)
            {
                if(diemdedoi < 70)
                {
                    system("cls");
                    gotoxy(15,10); cout << "So diem khong du!";
                    Sleep(1000);
                }
                else
                {
                    diemdedoi -= 70;
                    sodu += 63;
                    system("cls");
                    gotoxy(15,10); cout << "Quy doi thanh cong!";
                    Sleep(1000);
                }
            }
            else if (luachon == 4)
            {
                if(diemdedoi < 300)
                {
                    system("cls");
                    gotoxy(15,10); cout << "So diem khong du!";
                    Sleep(1000);
                }
                else
                {
                    diemdedoi -= 300;
                    sodu += 270;
                    system("cls");
                    gotoxy(15,10); cout << "Quy doi thanh cong!";
                    Sleep(1000);
                }
            }
            else if (luachon == 5)
            {
                if(diemdedoi < 600)
                {
                    system("cls");
                    gotoxy(15,10); cout << "So diem khong du!";
                    Sleep(1000);
                }
                else
                {
                    diemdedoi -= 600;
                    sodu += 540;
                    system("cls");
                    gotoxy(15,10); cout << "Quy doi thanh cong!";
                    Sleep(1000);
                }
            }
            else if (luachon == 6)
            {
                if(diemdedoi < 900)
                {
                    system("cls");
                    gotoxy(15,10); cout << "So diem khong du!";
                    Sleep(1000);
                }
                else
                {
                    diemdedoi -= 900;
                    sodu += 810;
                    system("cls");
                    gotoxy(15,10); cout << "Quy doi thanh cong!";
                    Sleep(1000);
                }
            }
        }
        if (luachon < 1)
        {
            luachon = 6;
        }
        else if (luachon > 6)
        {
            luachon = 1;
        }
    }
    XuatDiemCao("luutru.txt");
}
void HuongDan()
{
    system("cls");
    Hinh2();
    cout << "\n\nHuong dan: ";
    cout << "\n\n\tDung cac phim: ";
    cout << "[" << (char) 16 << "]" << "[" << (char) 17 << "]" << "[" << (char) 30 << "]" << "[" << (char) 31 << "]" << " de di chuyen xe";
    cout << "\n\n\tCo gang di chuyen tranh dung cham cac xe khac.";
    cout << "\n\n\tTich luy that nhieu diem roi doi thanh so du \n\n\t\t-> Nang cap xe xin hon nhe!\n\n\n";
    system("pause");
}
void LuaChonXe(int thongtin)
{
     NhapDiemCao("luutru.txt");
    sohuuxe1 = 0;
    sohuuxe2 = 0;
    sohuuxe3 = 0;

    luachon = 2;
    int i,j;
    system("cls");
    for(int i = 3;i>=1;--i)
    {
        Hinh2();
        gotoxy(20,13); cout << "Xin cho ... " << i;
        Sleep(500);
    }
    while (true)
    {
        system("cls");
        TaoKhungVienMenu2();
        InKhungVienMenu2(m,3);
        TaoXeMenu(22,4,3);

        if(thongtin == 1)
        {
            gotoxy(2,2); cout << "So du: " << sodu <<"$";
            TaoKhungVienMenu();
            InKhungVienMenu();
            thongtinxe();
        }
        gotoxy(10, 20);
        printf("[%c] [%c]: Thay doi tuy chon \t [enter]: Xac nhan \t [esc]: Quay lai menu", 17, 16);
        phimnhan = getch();

        if ((phimnhan == 75) && (m > 20))
        {
             m -=10;
        }
        else if ((phimnhan == 77) && (m < 40))
        {
            m += 10;
        }
        else if (phimnhan == 27)
        {
            system("cls");
            break;
        }
        else if (phimnhan == 13)
        {
            if(m == 20 )
            {
                if(sohuuxe1 == 1)
                {
                    system("cls");
                    gotoxy(10,10); cout << "Da co xe nay!";
                    Sleep(1000);
                    continue;
                }
                if(sodu < 500)
                {
                    system("cls");
                    gotoxy(10,10); cout << "So du khong du!";
                    Sleep(1000);
                }
                else
                {
                    sohuuxe1 = 1;
                    sodu -= 500;
                    system("cls");
                    gotoxy(10,10); cout << "Thanh cong!";
                    Sleep(1000);
                }
            }
            else if(m == 30)
            {
                if(sohuuxe2 == 1)
                {
                    system("cls");
                    gotoxy(10,10); cout << "Da co xe nay!";
                    Sleep(1000);
                    continue;
                }
                 if(sodu < 900)
                {
                    system("cls");
                    gotoxy(10,10); cout << "So du khong du!";
                    Sleep(1000);
                }
                else
                {
                    sohuuxe2 = 1;
                    sodu -= 900;
                    system("cls");
                    gotoxy(10,10); cout << "Thanh cong!";
                    Sleep(1000);
                }
            }
            else if(m == 40 )
            {
                if(sohuuxe3 == 1)
                {
                    system("cls");
                    gotoxy(10,10); cout << "Da co xe nay!";
                    Sleep(1000);
                    continue;
                }
                 if(sodu < 1200)
                {
                    system("cls");
                    gotoxy(10,10); cout << "So du khong du!";
                    Sleep(1000);
                }
                else
                {
                    sohuuxe3 = 1;
                    sodu -= 1200;
                    system("cls");
                    gotoxy(10,10); cout << "Thanh cong!";
                    Sleep(1000);
                }
            }

        }
    }
    XuatDiemCao("luutru.txt");
}
void XemDiemCao()
{
    NhapDiemCao("luutru.txt");
    system("cls");
    Hinh2();
    for (int i = 0; i < 5; i++)
    {
        gotoxy(21, i + 9);
        printf("\t[%d]: %d\n", i + 1, diemcao[i]);
    }
    gotoxy(20, 15);
    printf("Nhan bat ki de quay lai menu");
}

void SuuTap()
{
    NhapDiemCao("luutru.txt");

    luachon = 1;
    int i,j;
    while (true)
    {
        system("cls");
        Hinh2();
        TaoKhungVienMenu2();
        InKhungVienMenu2(m,9);
        TaoXeMenu(22,10,4);

        gotoxy(10, 20);
        printf("[%c] [%c]: Thay doi tuy chon \t [enter]: Xac nhan \t [esc]: Quay lai menu", 17, 16);
        phimnhan = getch();

        if ((phimnhan == 75) && (m > 20))
        {
             m -=10;
        }
        else if ((phimnhan == 77) && (m < 50))
        {
            m += 10;
        }
        else if (phimnhan == 27)
        {
            system("cls");
            break;
        }
        else if (phimnhan == 13)
        {
            if(m == 20 )
            {
                if(sohuuxe1 == 0)
                {
                    system("cls");
                    gotoxy(10,10); cout << "Chua co xe nay nen chua the dung!";
                    Sleep(1000);
                    m = 30;
                    break;
                }
                temp_tocdogame = 30;
                temp_tocdogame1 = 2;
                temp_tocdogame2 = 2;
                mauxe = 6;
                mangsong = 3;
                m = 30;
                break;
            }
            else if(m == 30)
            {

                mauxe = 15;
                m = 30;
                break;
            }
            else if(m == 40 )
            {
                if(sohuuxe2 == 0)
                {
                    system("cls");
                    gotoxy(10,10); cout << "Chua co xe nay nen chua the dung!";
                    Sleep(1000);
                    m = 30;
                    break;
                }
                temp_tocdogame = 15;
                temp_tocdogame1 = 5;
                temp_tocdogame2 = 5;
                sucben = 1;
                mauxe = 11;
                mangsong = 3;
                m = 30;
                break;
            }
            else if(m == 50)
            {
                if(sohuuxe3 == 0)
                {
                    system("cls");
                    gotoxy(10,10); cout << "Chua co xe nay nen chua the dung!";
                    Sleep(1000);
                    m = 30;
                    break;

                }
                temp_tocdogame = 5;
                temp_tocdogame1 = 5;
                temp_tocdogame2 = 5;
                sucben = 1;
                mauxe = 12;
                mangsong = 4;
                m = 30;
                break;
            }

        }
    }
    XuatDiemCao("luutru.txt");
}
void Menu()
{
    system("cls");
    Hinh2();
    if (luachon == 1)
    {
        gotoxy(22, 9);
        printf("%c", 16);
        ToMau(180);
        gotoxy(26, 9);
        printf("[1]: TRO CHOI MOI%1s\u001b[0m\n", "");
    }
    else
    {
        gotoxy(27, 9);
        printf("[1]: Tro choi moi%1s\u001b[0m\n", "");
    }
    if (luachon == 2)
    {
        gotoxy(22, 10);
        printf("%c", 16);
        ToMau(180);
        gotoxy(26, 10);
        printf("[2]: QUY DOI%5s\u001b[0m\n", "");
    }
    else
    {
        gotoxy(27, 10);
        printf("[2]: Quy doi%5s\u001b[0m\n", "");
    }
    if (luachon == 3)
    {
        gotoxy(22, 11);
        printf("%c", 16);
        ToMau(180);
        gotoxy(26, 11);
        printf("[3]: CUA HANG%4s\u001b[0m\n", "");
    }
    else
    {
        gotoxy(27, 11);
        printf("[3]: Cua hang%4s\u001b[0m\n", "");
    }
    if (luachon == 4)
    {
        gotoxy(22, 12);
        printf("%c", 16);
        ToMau(180);
        gotoxy(26, 12);
        printf("[4]: HUONG DAN%5s\u001b[0m\n", "");
    }
    else
    {
        gotoxy(27, 12);
        printf("[4]: Huong dan%5s\u001b[0m\n", "");
    }
    if (luachon == 5)
    {
        gotoxy(22, 13);
        printf("%c", 16);
        ToMau(180);
        gotoxy(26, 13);
        printf("[5]: DIEM CAO%6s\u001b[0m\n", "");
    }
    else
    {
        gotoxy(27, 13);
        printf("[5]: Diem cao%6s\u001b[0m\n", "");
    }
    if (luachon == 6)
    {
        gotoxy(22, 14);
        printf("%c", 16);
        ToMau(180);
        gotoxy(26, 14);
        printf("[6]: SUU TAP%8s\u001b[0m\n", "");
    }
    else
    {
        gotoxy(27, 14);
        printf("[6]: Suu tap%8s\u001b[0m\n", "");
    }
    if (luachon == 7)
    {
        gotoxy(22, 15);
        printf("%c", 16);
        ToMau(180);
        gotoxy(26, 15);
        printf("[7]: THOAT%8s\u001b[0m\n", "");
    }
    else
    {
        gotoxy(27, 15);
        printf("[7]: Thoat%8s\u001b[0m\n", "");
    }
    gotoxy(20, 20);
    printf("[%c] [%c]: Chon muc \t [enter]: Xac nhan", 30, 31);
    phimnhan = getch();
    if (phimnhan == 72)
    {
        luachon--;
    }
    else if (phimnhan == 80)
    {
        luachon++;
    }
    if (phimnhan == 13)
    {
        if (luachon == 1)
        {
            system("cls");
            TroChoiMoi();
        }
        else if (luachon == 2)
        {
            QuyDoiSoDu();
        }
        else if (luachon == 3)
        {
            system("cls");
            LuaChonXe(1);
        }
        else if (luachon == 4)
        {
            HuongDan();
        }
        else if (luachon == 5)
        {
            system("cls");
            XemDiemCao();
            getch();
        }
        else if (luachon == 6)
        {
            SuuTap();
        }
        else if (luachon == 7)
        {
           /* NhapDiemCao("luutru.txt");
            sohuuxe1 = 0;
            sohuuxe2 = 0;
            sohuuxe3 = 0;
            XuatDiemCao("luutru.txt");*/
            system("cls");
            exit(0);
        }
    }

    if (luachon < 1)
    {
        luachon = 7;
    }
    else if (luachon > 7)
    {
        luachon = 1;
    }
}
int main()
{
    TaoCacFileCanThiet();
    while(1)
    {
        Menu();
    }
    return 0;
}
