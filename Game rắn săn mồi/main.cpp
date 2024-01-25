#pragma warning(disable : 4996)
#pragma warning(disable : 6386)
#include <cstdio>
#include <conio.h>
#include <string.h>
#include <time.h>
#include <Windows.h>

// khai báo điều điều hướng cho con rắn:
// dx, dy = 1 -> con rắn hướng lên trên
// dx, dy = 2 -> con rắn hướng xuống dưới
// dx, dy = 3 -> con rắn hướng sang phải
// dx, dy = 4 -> con rắn hướng sang trái
const int dx[4] = {-1, 1, 0, 0};
const int dy[4] = {0, 0, -1, 1};

struct TOADO
{
    int x, y;
} s[1000], f, db, tg; // s- số lượng đốt max của con rắn, f: fruit - quả, db - quả đặc biệt

FILE *h;

int luachon, temp_map, luachonmapmecungcoidx, idxnutdatlai, i, j, dichuyen, diem, soluongdotmacdinh, map;
int demsoluongcnv, luachonmaptrongmecung, capdo, khoitaoxuathienquadacbiet, solandichuyendenquadb, temp_capdo;
int diemcao[5], Mau = 2, tomaumap = 2;
char a[21][31], phimnhan;
int stop, phimnhandieukhien, reset_cfg[3];

// hàm dịch chuyển con trỏ đến tọa độ xy:
void gotoxy(int x1, int y1)
{
    HANDLE hConsoleOutput;
    COORD Cursor_an_Pos = {x1 - 1, y1 - 1};
    hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hConsoleOutput, Cursor_an_Pos);
}
void DeLay(int x)
{
    Sleep(x);
}
// hàm ẩn con trỏ nháy:
void AnConTroDi()
{
    CONSOLE_CURSOR_INFO Info;
    Info.bVisible = FALSE;
    Info.dwSize = 20;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Info);
}

// hàm tô màu:
void ToMau(int x)
{
    HANDLE mau;
    mau = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(mau, x);
}

// hàm xóa màn hình:
void XoaManHinh()
{
    HANDLE hOut;
    COORD Position;
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    Position.X = 0;
    Position.Y = 0;
    SetConsoleCursorPosition(hOut, Position);
}

void HoanVi(int &a, int &b)
{
    int temp = a;
    a = b;
    b = temp;
}

int FileRong(const char *x)
{
    h = fopen(x, "r");
    if (h != NULL)
    {
        fseek(h, 0, SEEK_END);
        if (ftell(h) == 0)
            return 1;
        else
            return 0;
        fclose(h);
    }
    return 0;
}
int KiemTraHoaQua()
{
    for (i = 0; i < soluongdotmacdinh; i++)
    {
        if ((f.x == s[i].x) && (f.y == s[i].y))
        {
            a[f.x][f.y] = 'o';
            return 0;
        }
    }
    return 1;
}
int KiemTraQuaDB()
{
    for (i = 0; i < soluongdotmacdinh; i++)
    {
        if ((s[i].x == db.x) && (s[i].y == db.y))
        {
            a[db.x][db.y] = 'o';
            return 0;
        }
    }
    return 1;
}
int KiemTraViTriCuaQuaDB()
{
    for (i = 0; i < 17; i++)
    {
        for (j = 0; j < 27; j++)
        {
            if ((strncmp(&a[i + 1][j + 1], "  ", 2) == 0) && (strncmp(&a[i + 2][j + 1], "  ", 2) == 0))
            {
                return 1;
            }
        }
    }
    return 0;
}
int KiemTraChumQuaDB()
{
    for (i = 0; i < soluongdotmacdinh; i++)
    {
        if ((s[i].x == db.x) && (s[i].y == db.y))
        {
            a[db.x][db.y] = 'o';
            return 0;
        }
        else if ((s[i].x == db.x + 1) && (s[i].y == db.y))
        {
            a[db.x + 1][db.y] = 'o';
            return 0;
        }
        else if ((s[i].x == db.x) && (s[i].y == db.y + 1))
        {
            a[db.x][db.y + 1] = 'o';
            return 0;
        }
        else if ((s[i].x == db.x + 1) && (s[i].y == db.y + 1))
        {
            a[db.x + 1][db.y + 1] = 'o';
            return 0;
        }
    }
    if ((a[db.x][db.y] == (char)35) || (a[db.x + 1][db.y] == (char)35) || (a[db.x][db.y + 1] == (char)35) || (a[db.x + 1][db.y + 1] == (char)35))
    {
        return 0;
    }
    return 1;
}

void TaoCacFileCanThiet()
{
    h = fopen("cfg.ini", "r");
    if (h == NULL)
    {
        h = fopen("cfg.ini", "w");
        fprintf(h, "%d %d %d", 1, 1, 0);
        fclose(h);
    }
    h = fopen("save.ini", "r");
    if (h == NULL)
    {
        h = fopen("save.ini", "w");
        fclose(h);
    }
    system("attrib -r cfg.ini");
    system("attrib -r save.ini");
    h = fopen("dckd.txt", "r");
    if (h == NULL)
    {
        h = fopen("dckd.txt", "w");
        fclose(h);
    }
    h = fopen("dchd.txt", "r");
    if (h == NULL)
    {
        h = fopen("dchd.txt", "w");
        fclose(h);
    }
    h = fopen("dccd.txt", "r");
    if (h == NULL)
    {
        h = fopen("dccd.txt", "w");
        fclose(h);
    }
    luachon = 1;
    for (i = 0; i < 3; i++)
    {
        reset_cfg[i] = 0;
    }
}
void MoLaiDaLuuTruocDo()
{
    h = fopen("cfg.ini", "r");
    fscanf(h, "%d %d %d", &capdo, &map, &luachonmaptrongmecung);
    fclose(h);
}
void DocThongTinTrangThai(const char *x)
{
    h = fopen(x, "r");
    fscanf(h, "%d %d %d", &capdo, &map, &luachonmaptrongmecung);
    fscanf(h, "%d %d", &diem, &soluongdotmacdinh);
    fscanf(h, "%d", &dichuyen);
    fscanf(h, "%d %d %d %d %d %d", &f.x, &f.y, &db.x, &db.y, &khoitaoxuathienquadacbiet, &solandichuyendenquadb);
    for (i = 0; i < soluongdotmacdinh; i++)
    {
        fscanf(h, "%d %d", &s[i].x, &s[i].y);
    }
    fclose(h);
}
void XuatThongTinTrangThai(const char *x)
{
    h = fopen(x, "w");
    fprintf(h, "%d %d %d\n", capdo, map, luachonmaptrongmecung);
    fprintf(h, "%d %d\n", diem, soluongdotmacdinh);
    fprintf(h, "%d\n", dichuyen);
    fprintf(h, "%d %d %d %d %d %d\n", f.x, f.y, db.x, db.y, khoitaoxuathienquadacbiet, solandichuyendenquadb);
    for (i = 0; i < soluongdotmacdinh; i++)
    {
        fprintf(h, "%d %d\n", s[i].x, s[i].y);
    }
    fclose(h);
}
void NhapDiemCao(const char *x)
{
    h = fopen(x, "r");
    for (i = 0; i < 5; i++)
    {
        fscanf(h, "%d", &diemcao[i]);
    }
    fclose(h);
}
void XuatDiemCao(const char *x)
{
    h = fopen(x, "w");
    for (i = 0; i < 5; i++)
    {
        fprintf(h, "%d\n", diemcao[i]);
    }
    fclose(h);
}
void LuuLaiTruocKhiThoat()
{
    h = fopen("cfg.ini", "w");
    fprintf(h, "%d %d %d", capdo, map, luachonmaptrongmecung);
    fclose(h);
}
void hinh()
{
    gotoxy(40,6); printf(".d8888.  d8b   db  .d8b.   db   dD d88888b ");
    gotoxy(40,7); printf("88'  YP  888o  88 d8' `8b  88 ,8P' 88'     ");
    gotoxy(40,8); printf("`8bo.    88V8o 88 88ooo88  88,8P   88ooooo ");
    gotoxy(40,9); printf("  `Y8b.  88 V8o88 88~~~88  88`8b   88~~~~~ ");
    gotoxy(40,10); printf("db   8D  88  V888 88   88  88 `88. 88.     ");
    gotoxy(40,11); printf("`8888Y'  VP   V8P YP   YP  YP   YD Y88888P ");
}
void hinh2()
{
    gotoxy(20,2);printf(" ______     __   __     ______     __  __     ______    ");
    gotoxy(20,3);printf("/\\  ___\\   /\\ *-.\\ \\   /\\  __ \\   /\\ \\/ /    /\\  ___\\   ");
    gotoxy(20,4);printf("\\ \\___  \\  \\ \\ \\-.  \\  \\ \\  __ \\  \\ \\  _*-.  \\ \\  __\\   ");
    gotoxy(20,5);printf(" \\/\\_____\\  \\ \\_\\\\*\\_\\  \\ \\_\\ \\_\\  \\ \\_\\ \\_\\  \\ \\_____\\ ");
    gotoxy(20,6);printf("  \\/_____/   \\/_/ \\/_/   \\/_/\\/_/   \\/_/\\/_/   \\/_____/ ");
}
void KhungVienGame()
{
    for (i = 0; i < 20; i++)
    {
        for (j = 0; j < 30; j++)
        {
            if ((i == 0) || (i == 19))
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
    a[19][0] = (char)192;
    a[19][29] = (char)217;
}
void VeCacMapMeCung()
{
    if (luachonmaptrongmecung == 1)
    { // map hộp:
        for (i = 0; i < 18; i++)
        {
            for (j = 0; j < 28; j++)
            {
                if ((i == 0) || (i == 17) || (j == 0) || (j == 27))
                {
                    a[i + 1][j + 1] = (char)35;
                }
            }
        }
    }
    else if (luachonmaptrongmecung == 2)
    { // map hộp 2:
        for (i = 0; i < 30; i++)
        {
            if (((i >= 0) && (i <= 5)) || ((i > 21) && (i <= 27)))
            {
                a[1][i + 1] = (char)35;
            }
        }
        for (i = 0; i < 30; i++)
        {
            if (((i >= 0) && (i <= 5)) || ((i > 21) && (i <= 27)))
            {
                a[18][i + 1] = (char)35;
            }
        }
        for (i = 1; i < 6; i++)
        {
            a[i + 1][1] = (char)35;
        }
        for (i = 1; i < 6; i++)
        {
            a[i + 1][28] = (char)35;
        }
        for (i = 12; i < 18; i++)
        {
            a[i + 1][1] = (char)35;
        }
        for (i = 12; i < 18; i++)
        {
            a[i + 1][28] = (char)35;
        }
    }
    else if (luachonmaptrongmecung == 3)
    { // map đường hầm 1:
        for (i = 0; i < 30; i++)
        {
            if (((i >= 0) && (i <= 5)) || ((i > 21) && (i <= 27)))
            {
                a[1][i + 1] = (char)35;
            }
        }
        for (i = 0; i < 30; i++)
        {
            if (((i >= 0) && (i <= 5)) || ((i > 21) && (i <= 27)))
            {
                a[18][i + 1] = (char)35;
            }
        }
        for (i = 1; i < 6; i++)
        {
            a[i + 1][1] = (char)35;
        }
        for (i = 1; i < 6; i++)
        {
            a[i + 1][28] = (char)35;
        }
        for (i = 12; i < 18; i++)
        {
            a[i + 1][1] = (char)35;
        }
        for (i = 12; i < 18; i++)
        {
            a[i + 1][28] = (char)35;
        }
        for (i = 8; i < 20; i++)
        {
            a[6][i + 1] = (char)35;
        }
        for (i = 8; i < 20; i++)
        {
            a[13][i + 1] = (char)35;
        }
    }
    else if (luachonmaptrongmecung == 4)
    { // map đường hầm 2:
        for (i = 0; i < 30; i++)
        {
            if (((i >= 0) && (i <= 5)) || ((i > 21) && (i <= 27)))
            {
                a[1][i + 1] = (char)35;
            }
        }
        for (i = 0; i < 30; i++)
        {
            if (((i >= 0) && (i <= 5)) || ((i > 21) && (i <= 27)))
            {
                a[18][i + 1] = (char)35;
            }
        }
        for (i = 1; i < 6; i++)
        {
            a[i + 1][1] = (char)35;
        }
        for (i = 1; i < 6; i++)
        {
            a[i + 1][28] = (char)35;
        }
        for (i = 12; i < 18; i++)
        {
            a[i + 1][1] = (char)35;
        }
        for (i = 12; i < 18; i++)
        {
            a[i + 1][28] = (char)35;
        }
        for (i = 6; i < 12; i++)
        {
            a[i + 1][10] = (char)35;
        }
        for (i = 6; i < 12; i++)
        {
            a[i + 1][18] = (char)35;
        }
    }
    else if (luachonmaptrongmecung == 5)
    { // map cối xoay gió:
        for (i = 0; i < 6; i++)
        {
            a[i + 1][5] = (char)35;
        }
        for (i = 0; i < 19; i++)
        {
            a[13][i + 1] = (char)35;
        }
        for (i = 9; i < 28; i++)
        {
            a[6][i + 1] = (char)35;
        }
        for (i = 12; i < 18; i++)
        {
            a[i + 1][24] = (char)35;
        }
    }
    else if (luachonmaptrongmecung == 6)
    { // map đường ray:
        for (i = 0; i < 28; i++)
        {
            a[1][i + 1] = (char)35;
        }
        for (i = 0; i < 28; i++)
        {
            a[18][i + 1] = (char)35;
        }
        for (i = 1; i < 6; i++)
        {
            a[i + 1][1] = (char)35;
        }
        for (i = 1; i < 6; i++)
        {
            a[i + 1][28] = (char)35;
        }
        for (i = 12; i < 18; i++)
        {
            a[i + 1][1] = (char)35;
        }
        for (i = 12; i < 18; i++)
        {
            a[i + 1][28] = (char)35;
        }
        for (i = 8; i < 20; i++)
        {
            a[6][i + 1] = (char)35;
        }
        for (i = 8; i < 20; i++)
        {
            a[13][i + 1] = (char)35;
        }
    }
    else if (luachonmaptrongmecung == 7)
    { // map đường ray:
        for (i = 0; i < 28; i++)
        {
            a[1][i + 1] = (char)35;
        }
        for (i = 0; i < 28; i++)
        {
            a[18][i + 1] = (char)35;
        }
        for (i = 1; i < 6; i++)
        {
            a[i + 1][1] = (char)35;
        }
        for (i = 1; i < 6; i++)
        {
            a[i + 1][28] = (char)35;
        }
        for (i = 12; i < 18; i++)
        {
            a[i + 1][1] = (char)35;
        }
        for (i = 12; i < 18; i++)
        {
            a[i + 1][28] = (char)35;
        }

        for (i = 6; i < 12; i++)
        {
            a[i + 1][9] = (char)35;
        }
        for (i = 6; i < 12; i++)
        {
            a[i + 1][20] = (char)35;
        }
    }
    else if (luachonmaptrongmecung == 8)
    { // map căn hộ 1
        for (i = 0; i < 5; i++)
        {
            a[1][i + 1] = (char)35;
        }
        for (i = 1; i < 4; i++)
        {
            a[i + 1][1] = (char)35;
        }
        for (i = 9; i < 22; i++)
        {
            a[1][i + 1] = (char)35;
        }
        for (i = 1; i < 6; i++)
        {
            a[i + 1][13] = (char)35;
        }
        for (i = 0; i < 13; i++)
        {
            a[7][i + 1] = (char)35;
        }
        for (i = 17; i < 28; i++)
        {
            a[7][i + 1] = (char)35;
        }
        for (i = 0; i < 28; i++)
        {
            a[13][i + 1] = (char)35;
        }
        for (i = 13; i < 18; i++)
        {
            a[i + 1][18] = (char)35;
        }
    }
    else if (luachonmaptrongmecung == 9)
    { // map căn hộ 2
        for (i = 0; i < 18; i++)
        {
            a[i + 1][9] = (char)35;
        }
        for (i = 0; i < 9; i++)
        {
            a[13][i + 1] = (char)35;
        }
        for (i = 0; i < 9; i++)
        {
            a[i + 1][18] = (char)35;
        }
        for (i = 12; i < 18; i++)
        {
            a[i + 1][18] = (char)35;
        }
        for (i = 5; i < 14; i++)
        {
            a[i + 1][28] = (char)35;
        }
        for (i = 18; i < 28; i++)
        {
            a[9][i + 1] = (char)35;
        }
        for (i = 22; i < 28; i++)
        {
            a[1][i + 1] = (char)35;
        }
        a[2][28] = (char)35;
        a[3][28] = (char)35;
    }
    demsoluongcnv = 0;
    for (i = 0; i < 18; i++)
    {
        for (j = 0; j < 26; j++)
        {
            if (a[i + 1][j + 1] == '+')
            {
                demsoluongcnv++;
            }
        }
    }
}

void InMoiThuTrongMap()
{

    for (i = 0; i < 20; i++)
    {
        for (j = 0; j < 30; j++)
        {
            if (s[soluongdotmacdinh - 1].x == i && s[soluongdotmacdinh - 1].y == j)
            {
                ToMau(14);
            }
            else if (f.x == i && f.y == j)
            {
                ToMau(Mau);
            }
            else if (map != 3)
            {
                ToMau(7);
            }
            else
                ToMau(tomaumap);
            printf("%c", a[i][j]);
            printf("\u001b[0m");
        }
        printf("\n");
    }
    hinh();
    gotoxy(33, 2);
    printf("DIEM: %06d", diem);
    gotoxy(1, 22);
    printf("[%c] [%c] [%c] [%c]: Dieu khien \t [esc]: Tam dung", 30, 31, 17, 16);

}
void TaoKhungVienMenu(int &h)
{
    for (i = 0; i < h; i++)
    {
        for (j = 0; j < 30; j++)
        {
            if ((i == 0) || (i == h - 1))
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
    a[h - 1][0] = (char)192;
    a[h - 1][29] = (char)217;
}
void InKhungVienMenu(int &h)
{
    for (i = 0; i < h; i++)
    {
        gotoxy(20, i + 8);
        for (j = 0; j < 30; j++)
        {
            printf("%c", a[i][j]);
            printf("\u001b[0m");
        }
        printf("\n");
    }
}
void TaoQua()
{
    do
    {
        f.x = rand() % 16 + 1;
        f.y = rand() % 26 + 1;
    } while ((KiemTraHoaQua() == 0) || (a[f.x][f.y] == (char)35) || (a[f.x][f.y] == (char)233));
    a[f.x][f.y] = (char)254;
}
void TaoThanhPhanQuaDacBiet()
{
    do
    {
        db.x = rand() % 14 + 1;
        db.y = rand() % 24 + 1;
    } while ((KiemTraQuaDB() == 0) || (a[db.x][db.y] == (char)35) || (a[db.x][db.y] == (char)254));
    a[db.x][db.y] = (char)233;
}
void TaoChumQuaDacBiet()
{
    do
    {
        db.x = rand() % 14 + 1;
        db.y = rand() % 24 + 1;
    } while ((KiemTraChumQuaDB() == 0) || (KiemTraViTriCuaQuaDB() == 0));
    if ((f.x == db.x) && (f.y == db.y))
    {
        a[db.x][db.y] = (char)233;
    }
    else
    {
        a[db.x][db.y] = (char)233;
    }
    if ((f.x == db.x + 1) && (f.y == db.y))
    {
        a[db.x + 1][db.y] = (char)233;
    }
    else
    {
        a[db.x + 1][db.y] = (char)233;
    }
    if ((f.x == db.x) && (f.y == db.y + 1))
    {
        a[db.x][db.y + 1] = (char)233;
    }
    else
    {
        a[db.x][db.y + 1] = (char)233;
    }
    if ((f.x == db.x + 1) && (f.y == db.y + 1))
    {
        a[db.x + 1][db.y + 1] = (char)233;
    }
    else
    {
        a[db.x + 1][db.y + 1] = (char)233;
    }
    khoitaoxuathienquadacbiet = 1;
}
void XoaChumQuaDacBiet()
{
    if ((khoitaoxuathienquadacbiet == 1) && ((a[db.x][db.y] == (char)233) || (a[db.x + 1][db.y] == (char)233) || (a[db.x][db.y + 1] == (char)233) || (a[db.x + 1][db.y + 1] == (char)233)))
    {
        a[db.x][db.y] = ' ';
        a[db.x + 1][db.y] = ' ';
        a[db.x][db.y + 1] = ' ';
        a[db.x + 1][db.y + 1] = ' ';
        if ((s[soluongdotmacdinh - 1].x == db.x) && (s[soluongdotmacdinh - 1].y == db.y))
        {
            a[db.x][db.y] = 'o';
        }
        else if ((s[soluongdotmacdinh - 1].x == db.x + 1) && (s[soluongdotmacdinh - 1].y == db.y))
        {
            a[db.x + 1][db.y] = 'o';
        }
        else if ((s[soluongdotmacdinh - 1].x == db.x) && (s[soluongdotmacdinh - 1].y == db.y + 1))
        {
            a[db.x][db.y + 1] = 'o';
        }
        else if ((s[soluongdotmacdinh - 1].x == db.x + 1) && (s[soluongdotmacdinh - 1].y == db.y + 1))
        {
            a[db.x + 1][db.y + 1] = 'o';
        }
    }
    else if ((khoitaoxuathienquadacbiet == 1) && ((a[db.x][db.y] == (char)233) || (a[db.x + 1][db.y] == (char)233) || (a[db.x][db.y + 1] == (char)233) || (a[db.x + 1][db.y + 1] == (char)233)))
    {
        a[db.x][db.y] = ' ';
        a[db.x + 1][db.y] = ' ';
        a[db.x][db.y + 1] = ' ';
        a[db.x + 1][db.y + 1] = ' ';
        if ((s[soluongdotmacdinh - 1].x == db.x) && (s[soluongdotmacdinh - 1].y == db.y))
        {
            a[db.x][db.y] = 'o';
        }
        else if ((s[soluongdotmacdinh - 1].x == db.x + 1) && (s[soluongdotmacdinh - 1].y == db.y))
        {
            a[db.x + 1][db.y] = 'o';
        }
        else if ((s[soluongdotmacdinh - 1].x == db.x) && (s[soluongdotmacdinh - 1].y == db.y + 1))
        {
            a[db.x][db.y + 1] = 'o';
        }
        else if ((s[soluongdotmacdinh - 1].x == db.x + 1) && (s[soluongdotmacdinh - 1].y == db.y + 1))
        {
            a[db.x + 1][db.y + 1] = 'o';
        }
    }
    else if (a[db.x][db.y] == (char)233)
    {
        a[db.x][db.y] = ' ';
    }
    db.x = 0;
    db.y = 0;
    solandichuyendenquadb = 0;
    khoitaoxuathienquadacbiet = 0;
}
void CapNhatLai()
{
    XoaManHinh();
    InMoiThuTrongMap();
}
void LuuLaiDiemSo()
{
    if (map == 1)
    {
        NhapDiemCao("dckd.txt");
    }
    else if (map == 2)
    {
        NhapDiemCao("dchd.txt");
    }
    else if (map == 3)
    {
        NhapDiemCao("dccd.txt");
    }
    if (diem <= diemcao[4])
    {
        gotoxy(25, 6);
        printf("Diem cua ban: %d", diem);
    }
    else if ((diem > diemcao[4]) && (diem <= diemcao[0]))
    {
        gotoxy(25, 6);
        printf("Diem cao: %d", diem);
    }
    else
    {
        gotoxy(25, 6);
        printf("Diem cao nhat: %d", diem);
    }
    if (diem > diemcao[4])
    {
        diemcao[4] = diem;
    }
    for (i = 0; i < 4; i++)
    {
        for (j = i + 1; j < 5; j++)
        {
            if (diemcao[i] < diemcao[j])
            {
                HoanVi(diemcao[i], diemcao[j]);
            }
        }
    }
    if (map == 1)
    {
        XuatDiemCao("dckd.txt");
    }
    else if (map == 2)
    {
        XuatDiemCao("dchd.txt");
    }
    else if (map == 3)
    {
        XuatDiemCao("dccd.txt");
    }
}
void KhoiTaoBanDau()
{
    soluongdotmacdinh = 2;
    if (map == 1)
    {
        luachonmaptrongmecung = 0;
    }
    else if ((map == 3) && (luachonmaptrongmecung != 1))
    {
        luachonmaptrongmecung = 1;
    }
    if (luachonmaptrongmecung == 3)
    {
        s[0].x = 10;
    }
    else
    {
        s[0].x = 10;
    }
    s[0].y = 11;
    if (luachonmaptrongmecung == 3)
    {
        s[1].x = 10;
    }
    else
    {
        s[1].x = 10;
    }
    s[1].y = 12;
    dichuyen = 3;
    a[s[0].x][s[0].y] = 'o';
    a[s[1].x][s[1].y] = 'O';
    stop = 0;
    phimnhandieukhien = 0;
}
void HoanThanh1ChienDich()
{
    system("cls");
    DeLay(1000);
    for (i = 0; i < 6; i++)
    {
        system("color 70");
        DeLay(100);
        system("color 07");
        DeLay(100);
    }
}
void TiepTheo()
{
    HoanThanh1ChienDich();
    system("cls");
    KhungVienGame();
    soluongdotmacdinh = 2;
    luachonmaptrongmecung++;
    if (luachonmaptrongmecung == 3)
    {
        s[0].x = 10;
    }
    else
    {
        s[0].x = 10;
    }
    s[0].y = 11;
    if (luachonmaptrongmecung == 3)
    {
        s[1].x = 10;
    }
    else
    {
        s[1].x = 10;
    }
    s[1].y = 12;
    dichuyen = 3;
    a[s[0].x][s[0].y] = 'o';
    a[s[soluongdotmacdinh - 1].x][s[soluongdotmacdinh - 1].y] = 'O';
    stop = 0;
    phimnhandieukhien = 1;
    VeCacMapMeCung();
    TaoQua();
    InMoiThuTrongMap();
    XuatThongTinTrangThai("save.ini");
    while (phimnhandieukhien == 1)
    {
        phimnhan = getch();
        if (phimnhan == 27)
        {
            break;
        }
        else
        {
            phimnhandieukhien = 1;
        }

        if ((phimnhan == 72) && (dichuyen != 1))
        {
            dichuyen = 0;
        }
        else if ((phimnhan == 80) && (dichuyen != 0))
        {
            dichuyen = 1;
        }
        else if ((phimnhan == 75) && (dichuyen != 3))
        {
            dichuyen = 2;
        }
        else if ((phimnhan == 77) && (dichuyen != 2))
        {
            dichuyen = 3;
        }
        phimnhandieukhien = 0;
    }
}
void XuLyVaDiChuyenSnake(int dichuyen)
{
    if (solandichuyendenquadb > 0)
    {
        solandichuyendenquadb--;
    }
    if (solandichuyendenquadb == 0)
    {
        XoaChumQuaDacBiet();
    }
    tg = s[0];

    a[s[0].x][s[0].y] = ' ';    // an phan tu dau di
    // them 1 phan tu vao cuoi
    for (i = 0; i < soluongdotmacdinh - 1; i++)
    {
        s[i] = s[i + 1];
        a[s[i].x][s[i].y] = 'o';
    }
    s[soluongdotmacdinh - 1].x = s[soluongdotmacdinh - 2].x + dx[dichuyen];
    s[soluongdotmacdinh - 1].y = s[soluongdotmacdinh - 2].y + dy[dichuyen];

    if (s[soluongdotmacdinh - 1].x == 0)
    {
        s[soluongdotmacdinh - 1].x = 18;
    }
    else if (s[soluongdotmacdinh - 1].x == 19)
    {
        s[soluongdotmacdinh - 1].x = 1;
    }
    if (s[soluongdotmacdinh - 1].y == 0)
    {
        s[soluongdotmacdinh - 1].y = 28;
    }
    else if (s[soluongdotmacdinh - 1].y == 29)
    {
        s[soluongdotmacdinh - 1].y = 1;
    }
    for (i = 0; i < soluongdotmacdinh - 1; i++)
    {
        for (j = i + 1; j < soluongdotmacdinh; j++)
        {
            if ((s[i].x == s[j].x) && (s[i].y == s[j].y))
            {
                stop = 1;
                break;
            }
        }
    }
    if (a[s[soluongdotmacdinh - 1].x][s[soluongdotmacdinh - 1].y] == (char)35)
    {
        stop = 1;
    }
    a[s[soluongdotmacdinh - 1].x][s[soluongdotmacdinh - 1].y] = 'O';

    if ((s[soluongdotmacdinh - 1].x == f.x) && (s[soluongdotmacdinh - 1].y) == f.y)
    {
        printf("\a");
        Mau++;
        if (Mau > 15)
        {
            Mau = 3;
        }
        if ((map != 3) || ((map == 3) && (soluongdotmacdinh < 25)))
        {
            diem += capdo;
        }
        if (diem > 99999)
        {
            diem = 99999;
        }
        if ((((luachonmaptrongmecung == 0) || (luachonmaptrongmecung == 3)) && (soluongdotmacdinh < 1000)) || ((luachonmaptrongmecung != 0) && (luachonmaptrongmecung != 3) && (soluongdotmacdinh < 1000 - demsoluongcnv)))
        {
            soluongdotmacdinh++;
            for (i = soluongdotmacdinh - 1; i >= 0; i--)
            {
                s[i + 1] = s[i];
            }
            s[0] = tg;
            a[s[0].x][s[0].y] = '*';
            if ((map != 3) || ((map == 3) && (soluongdotmacdinh < 25)))
            {
                TaoQua();
            }
            if (soluongdotmacdinh % 5 == 2)
            {
                if (solandichuyendenquadb > 0)
                {
                    XoaChumQuaDacBiet();
                }
                if (KiemTraViTriCuaQuaDB() == 1)
                {
                    TaoChumQuaDacBiet();
                }
                else
                {
                    TaoThanhPhanQuaDacBiet();
                }
                solandichuyendenquadb = 35;
            }
            if ((map == 3) && (soluongdotmacdinh == 25))
            {
                XoaChumQuaDacBiet();
            }
        }
        else
        {
            TaoQua();
        }
    }
    if (khoitaoxuathienquadacbiet == 1)
    {
        if (((s[soluongdotmacdinh - 1].x == db.x) && (s[soluongdotmacdinh - 1].y == db.y)) || ((s[soluongdotmacdinh - 1].x == db.x + 1) && (s[soluongdotmacdinh - 1].y == db.y)) || ((s[soluongdotmacdinh - 1].x == db.x) && (s[soluongdotmacdinh - 1].y == db.y + 1)) || ((s[soluongdotmacdinh - 1].x == db.x + 1) && (s[soluongdotmacdinh - 1].y == db.y + 1)))
        {
            printf("\a");
            diem += (solandichuyendenquadb * 8 * capdo);
            XoaChumQuaDacBiet();
        }
        else if ((s[soluongdotmacdinh - 1].x == db.x) && (s[soluongdotmacdinh - 1].y == db.y))
        {
            printf("\a");
            diem += (solandichuyendenquadb * 8 * capdo);
            XoaChumQuaDacBiet();
        }
    }
    if ((f.x != 0) && (f.y != 0) && (a[f.x][f.y] == ' '))
    {
        a[f.x][f.y] = (char)254;
    }
    CapNhatLai();
    if (stop == 1)
    {
        DeLay(1000);
    }
}
void VanMoi()
{
    system("cls 2>save.ini");
    diem = 0;
    KhungVienGame();
    KhoiTaoBanDau();
    VeCacMapMeCung();
    TaoQua();
    InMoiThuTrongMap();
    while (phimnhandieukhien == 0)
    {
        if (kbhit())
        {
            phimnhan = getch();

            if ((phimnhan == 72) && (dichuyen != 1)) // di len
            {
                dichuyen = 0;
            }
            else if ((phimnhan == 80) && (dichuyen != 0)) // di xuong
            {
                dichuyen = 1;
            }
            else if ((phimnhan == 75) && (dichuyen != 3)) // di sang trai
            {
                dichuyen = 2;
            }
            else if ((phimnhan == 77) && (dichuyen != 2)) // di sang phai
            {
                dichuyen = 3;
            }

            if (phimnhan == 27)
            {
                 system("cls");
                if (stop == 0)
                {
                    XuatThongTinTrangThai("save.ini");
                    phimnhandieukhien = 1;
                    luachon--;
                }
                else
                {
                    system("cls 2>save.ini");
                    LuuLaiDiemSo();
                }
                luachon = 1;
                break;
            }
        }
        if (stop == 0)
        {
            if ((kbhit() == 0) && ((phimnhan == 72) || (phimnhan == 80) || (phimnhan == 75) || (phimnhan == 77)))
            {
                XuLyVaDiChuyenSnake(dichuyen);
                DeLay(200 - (25 * capdo));
            }
            if ((map == 3) && (soluongdotmacdinh == 25))
            {
                if (luachonmaptrongmecung < 9)
                {
                    tomaumap++;
                    TiepTheo();
                }
                else
                {
                    stop = 1;
                    DeLay(1000);
                }
            }
        }
        else
        {
            system("cls 2>save.ini");
            if ((map == 3) && (luachonmaptrongmecung == 9) && (soluongdotmacdinh == 25))
            {
                gotoxy(28, 4);
                printf("----CHIEN DICH DA HOAN THANH----");
            }
            else
            {
                gotoxy(28, 4);
                printf("----GAME OVER----");
            }
            LuuLaiDiemSo();
            gotoxy(25, 7);
            printf("Xin nhan phim bat ki de quay lai menu");
            getch();
            luachon = 1;
            system("cls");
            break;
        }
    }
}
void TiepTuc()
{
    system("cls");
    DocThongTinTrangThai("save.ini");
    KhungVienGame();
    VeCacMapMeCung();
    for (i = 0; i < soluongdotmacdinh; i++)
    {
        a[s[i].x][s[i].y] = '*';
    }
    if ((f.x != 0) && (f.y != 0))
    {
        a[f.x][f.y] = (char)254;
    }
    if ((db.x != 0) && (db.y != 0))
    {
        if (khoitaoxuathienquadacbiet == 1)
        {
            if ((f.x == db.x) && (f.y == db.y))
            {
                a[db.x][db.y] = (char)233;
            }
            else
            {
                a[db.x][db.y] = (char)233;
            }
            if ((f.x == db.x + 1) && (f.y == db.y))
            {
                a[db.x + 1][db.y] = (char)233;
            }
            else
            {
                a[db.x + 1][db.y] = (char)233;
            }
            if ((f.x == db.x) && (f.y == db.y + 1))
            {
                a[db.x][db.y + 1] = (char)233;
            }
            else
            {
                a[db.x][db.y + 1] = (char)233;
            }
            if ((f.x == db.x + 1) && (f.y == db.y + 1))
            {
                a[db.x + 1][db.y + 1] = (char)233;
            }
            else
            {
                a[db.x + 1][db.y + 1] = (char)233;
            }
        }
        else
        {
            a[db.x][db.y] = (char)233;
        }
    }
    stop = 0;
    phimnhandieukhien = 1;
    CapNhatLai();

    while (phimnhandieukhien == 1)
    {
        phimnhan = getch();
        if (phimnhan == 27)
        {
            break;
        }
        else
        {
            phimnhandieukhien = 1;
        }

        if ((phimnhan == 72) && (dichuyen != 1))
        {
            dichuyen = 0;
        }
        else if ((phimnhan == 80) && (dichuyen != 0))
        {
            dichuyen = 1;
        }
        else if ((phimnhan == 75) && (dichuyen != 3))
        {
            dichuyen = 2;
        }
        else if ((phimnhan == 77) && (dichuyen != 2))
        {
            dichuyen = 3;
        }
        phimnhandieukhien = 0;
    }
    while (phimnhandieukhien == 0)
    {
        if (kbhit())
        {
            phimnhan = getch();

            if ((phimnhan == 72) && (dichuyen != 1))
            {
                dichuyen = 0;
            }
            else if ((phimnhan == 80) && (dichuyen != 0))
            {
                dichuyen = 1;
            }
            else if ((phimnhan == 75) && (dichuyen != 3))
            {
                dichuyen = 2;
            }
            else if ((phimnhan == 77) && (dichuyen != 2))
            {
                dichuyen = 3;
            }

            if (phimnhan == 27)
            {
                system("cls");
                if (stop == 0)
                {
                    XuatThongTinTrangThai("save.ini");
                    phimnhandieukhien = 1;
                }

                else
                {
                    system("cls 2>save.ini");
                    LuuLaiDiemSo();
                }
                luachon = 1;
                break;
            }
        }
        if (stop == 0)
        {
            if ((kbhit() == 0) && ((phimnhan == 72) || (phimnhan == 80) || (phimnhan == 75) || (phimnhan == 77)))
            {
                XuLyVaDiChuyenSnake(dichuyen);
                DeLay(200 - (25 * capdo));
            }
            if ((map == 3) && (soluongdotmacdinh == 25))
                if (luachonmaptrongmecung < 9)
                {
                    TiepTheo();
                }
                else
                {
                    stop = 1;
                    DeLay(1000);
                }
        }
        else
        {
            system("cls 2>save.ini");
            if ((map == 3) && (luachonmaptrongmecung == 9) && (soluongdotmacdinh == 25))
            {
                gotoxy(28, 4);
                printf("----CHIEN DICH DA HOAN THANH----");
            }
            else
            {
                gotoxy(28, 4);
                printf("----GAME OVER----");
            }
            LuuLaiDiemSo();
            gotoxy(25, 7);
            printf("Xin nhan phim bat ki de quay lai menu");
            getch();
            luachon = 1;
            system("cls");
            break;
        }
    }
}
void LuaChonCapDo()
{
    system("cls");
    gotoxy(33, 5); printf("MUC DO:");
    temp_capdo = capdo;
    while (true)
    {
        XoaManHinh();
        hinh2();
        gotoxy(27, 9);
        printf("%c", 218);
        gotoxy(44, 9);
        printf("%c", 191);
        for (i = 1; i < 17; i++)
        {
            gotoxy(i + 27, 9);
            printf("%c", 196);
        }
        gotoxy(27, 10);
        printf("%c", 179);
        for (i = 0; i < temp_capdo; i++)
        {
            ToMau(160);
            printf("  \u001b[0m");
        }
        for (i = 0; i < 8 - temp_capdo; i++)
        {
            ToMau(239);
            printf("  \u001b[0m");
        }
        printf("%c %d", 179, temp_capdo);
        gotoxy(27, 11);
        printf("%c", 192);
        gotoxy(44, 11);
        printf("%c", 217);
        for (i = 1; i < 17; i++)
        {
            gotoxy(i + 27, 11);
            printf("%c", 196);
        }
        gotoxy(10, 20);
        printf("[%c] [%c]: Thay doi tuy chon \t [enter]: Xac nhan \t [esc]: Quay lai menu", 17, 16);
        phimnhan = getch();

        if ((phimnhan == 75) && (temp_capdo > 1))
        {
            temp_capdo--;
        }
        else if ((phimnhan == 77) && (temp_capdo < 8))
        {
            temp_capdo++;
        }
        else if ((phimnhan == 13) || (phimnhan == 27))
        {
            system("cls");
            break;
        }
    }
    if (phimnhan == 13)
    {
        if ((capdo != temp_capdo) && (FileRong("save.ini") == 0))
        {
            system("cls 2>save.ini");
            luachon--;
        }
        capdo = temp_capdo;
        LuuLaiTruocKhiThoat();
    }
}
void LuaChonMeCung()
{
    system("cls");
    int h = 12;
    if (map == 3)
    {
        luachonmapmecungcoidx = 0;
    }
    else
    {
        luachonmapmecungcoidx = luachonmaptrongmecung;
    }
    while (true)
    {
        XoaManHinh();
        hinh2();
        TaoKhungVienMenu(h);
        InKhungVienMenu(h);
        if (luachonmapmecungcoidx == 0)
        {
            gotoxy(22, 9);
            printf("%c", 16);
            ToMau(180);
            gotoxy(26, 9);
            printf("[0]: KHONG CO ME CUNG%1s\u001b[0m\n", "");
        }
        else
        {
            gotoxy(26, 9);
            printf("[0]: Khong co me cung%1s\u001b[0m\n", "");
        }
        if (luachonmapmecungcoidx == 1)
        {
            gotoxy(22, 10);
            printf("%c", 16);
            ToMau(180);
            gotoxy(26, 10);
            printf("[1]: HOP(1)% 11s\u001b[0m\n", "");
        }
        else
        {
            gotoxy(26, 10);
            printf("[1]: Hop(1)%11s\u001b[0m\n", "");
        }
        if (luachonmapmecungcoidx == 2)
        {
            gotoxy(22, 11);
            printf("%c", 16);
            ToMau(180);
            gotoxy(26, 11);
            printf("[2]: HOP(2)%11s\u001b[0m\n", "");
        }
        else
        {
            gotoxy(26, 11);
            printf("[2]: Hop(2)%11s\u001b[0m\n", "");
        }
        if (luachonmapmecungcoidx == 3)
        {
            gotoxy(22, 12);
            printf("%c", 16);
            ToMau(180);
            gotoxy(26, 12);
            printf("[3]: DUONG HAM(1)%5s\u001b[0m\n", "");
        }
        else
        {
            gotoxy(26, 12);
            printf("[3]: Duong ham(1)%5s\u001b[0m\n", "");
        }
        if (luachonmapmecungcoidx == 4)
        {
            gotoxy(22, 13);
            printf("%c", 16);
            ToMau(180);

            gotoxy(26, 13);
            printf("[4]: DUONG HAM(2)%5s\u001b[0m\n", "");
        }
        else
        {
            gotoxy(26, 13);
            printf("[4]: Duong ham(2)%5s\u001b[0m\n", "");
        }
        if (luachonmapmecungcoidx == 5)
        {
            gotoxy(22, 14);
            printf("%c", 16);
            ToMau(180);
            gotoxy(26, 14);
            printf("[5]: COI XOAY GIO%5s\u001b[0m\n", "");
        }
        else
        {
            gotoxy(26, 14);
            printf("[5]: Coi xoay gio%5s\u001b[0m\n", "");
        }
        if (luachonmapmecungcoidx == 6)
        {
            gotoxy(22, 15);
            printf("%c", 16);
            ToMau(180);
            gotoxy(26, 15);
            printf("[6]: DUONG RAY(1)%5s\u001b[0m\n", "");
        }
        else
        {
            gotoxy(26, 15);
            printf("[6]: Duong ray(1)%5s\u001b[0m\n", "");
        }
        if (luachonmapmecungcoidx == 7)
        {
            gotoxy(22, 16);
            printf("%c", 16);
            ToMau(180);
            gotoxy(26, 16);
            printf("[7]: DUONG RAY(2)%5s\u001b[0m\n", "");
        }
        else
        {
            gotoxy(26, 16);
            printf("[7]: Duong ray(2)%5s\u001b[0m\n", "");
        }
        if (luachonmapmecungcoidx == 8)
        {
            gotoxy(22, 14);
            printf("%c", 17);
            ToMau(180);
            gotoxy(26, 17);
            printf("[8]: CAN HO(1)%8s\u001b[0m\n", "");
        }
        else
        {
            gotoxy(26, 17);
            printf("[8]: Can ho(1)%8s\u001b[0m\n", "");
        }
        if (luachonmapmecungcoidx == 9)
        {
            gotoxy(22, 18);
            printf("%c", 16);
            ToMau(180);
            gotoxy(26, 18);
            printf("[9]: CAN HO(2)%8s\u001b[0m\n", "");
        }
        else
        {
            gotoxy(26, 18);
            printf("[9]: Can ho(2)%8s\u001b[0m\n", "");
        }
        gotoxy(20, 20);
        printf("[%c] [%c]: Chon muc \t [enter]: Xac nhan \t [esc]: Quay lai che do", 30, 31);

        phimnhan = getch();
        if ((phimnhan == 13) || (phimnhan == 27))
        {
            system("cls");
            break;
        }
        else if (phimnhan == 72)
        {
            luachonmapmecungcoidx--;
        }
        else if (phimnhan == 80)
        {
            luachonmapmecungcoidx++;
        }

        if (luachonmapmecungcoidx < 0)
        {
            luachonmapmecungcoidx = 9;
        }
        else if (luachonmapmecungcoidx > 9)
        {
            luachonmapmecungcoidx = 0;
        }
    }
    if (phimnhan == 13)
    {
        if (((map == 1) || (map == 3) || ((map == 2) && (luachonmaptrongmecung != luachonmapmecungcoidx))) && (FileRong("save.ini") == 0))
        {
            system("cls 2>save.ini");
            luachon--;
        }
        map = 2;
        luachonmaptrongmecung = luachonmapmecungcoidx;
        LuuLaiTruocKhiThoat();
    }
}
void LuaChonCheDo()
{
    system("cls");
    int h = 5;
    temp_map = map;
    while (true)
    {
        XoaManHinh();
        hinh2();
        TaoKhungVienMenu(h);
        InKhungVienMenu(h);
        if (temp_map == 1)
        {
            gotoxy(22, 9);
            printf("%c", 16);
            ToMau(180);
            gotoxy(26, 9);
            printf("[1]: KINH DIEN%5s\u001b[0m\n", "");
        }
        else
        {
            gotoxy(26, 9);
            printf("[1]: Kinh dien%8s\u001b[0m\n", "");
        }
        if (temp_map == 2)
        {
            gotoxy(22, 10);
            printf("%c", 16);
            ToMau(180);
            gotoxy(26, 10);
            printf("[2]: HIEN DAI%5s\u001b[0m\n", "");
        }
        else
        {
            gotoxy(26, 10);
            printf("[2]: Hien dai%8s\u001b[0m\n", "");
        }
        if (temp_map == 3)
        {
            gotoxy(22, 11);
            printf("%c", 16);
            ToMau(180);
            gotoxy(26, 11);
            printf("[3]: CHIEN DICH%5s\u001b[0m\n", "");
        }
        else
        {
            gotoxy(26, 11);
            printf("[3]: Chien dich%8s\u001b[0m", "");
        }
        gotoxy(20, 20);
        printf("[%c] [%c]: Chon muc \t [enter]: Xac nhan \t [esc]: Quay lai menu", 30, 31);
        phimnhan = getch();
        if (phimnhan == 13)
        {
            if (temp_map == 2)
            {
                LuaChonMeCung();
            }
            else
                break;
        }
        else if (phimnhan == 27)
        {
            system("cls");
            break;
        }
        // else if (phimnhan == -32)
        {

            if (phimnhan == 72)
            {
                temp_map--;
            }
            else if (phimnhan == 80)
            {
                temp_map++;
            }
        }
        if (temp_map < 1)
        {
            temp_map = 3;
        }
        else if (temp_map > 3)
        {
            temp_map = 1;
        }
    }
    if (temp_map == 1)
    {
        luachonmaptrongmecung = 0;
    }
    else if (temp_map == 3)
    {
        luachonmaptrongmecung = 1;
    }
    if (phimnhan == 13)
    {
        if ((map != temp_map) && (FileRong("save.ini") == 0))
        {
            system("cls 2>save.ini");
            luachon--;
        }
        map = temp_map;
        LuuLaiTruocKhiThoat();
    }
}
void HuongDan()
{
        system("cls");
        gotoxy(21, 9);
        printf("Su dung cac phim mui ten de dieu khien ran. Tranh cham cac chuong");
        gotoxy(21, 10);
        printf("ngai vat va an thuc an dac biet nhanh chong de ghi them diem.");
        gotoxy(20, 15);
        printf("Nhan bat ki de quay lai menu");
        if (getch())
        {
            system("cls");
        }
}
void XemDiemCao()
{
    int h = 7;
    if (map == 1)
    {
        NhapDiemCao("dckd.txt");
    }
    else if (map == 2)
    {
        NhapDiemCao("dchd.txt");
    }
    else if (map == 3)
    {
        NhapDiemCao("dccd.txt");
    }
    while (true)
    {
        system("cls");
        hinh2();
        TaoKhungVienMenu(h);
        InKhungVienMenu(h);

        for (i = 0; i < 5; i++)
        {
            gotoxy(21, i + 9);
            printf("\t[%d]: %d\n", i + 1, diemcao[i]);
        }
        gotoxy(20, 15);
        printf("Nhan bat ki de quay lai menu");
        if (getch())
        {
            system("cls");
            break;
        }
    }
}
void LuaChonDatLai()
{
    system("cls");
    int h = 5;
    idxnutdatlai = 1;
    while (true)
    {
        XoaManHinh();
        hinh2();
        TaoKhungVienMenu(h);
        InKhungVienMenu(h);
        if (idxnutdatlai == 1)
        {
            gotoxy(22, 9);
            printf("%c", 16);
            ToMau(180);
            gotoxy(26, 9);
            printf("[1]: THIET LAP DU LIEU%s\u001b[0m\n", "");
        }
        else
        {
            gotoxy(26, 9);
            printf("[1]: Thiet lap du lieu%s\u001b[0m\n", "");
        }
        if (idxnutdatlai == 2)
        {
            gotoxy(22, 10);
            printf("%c", 16);
            ToMau(180);
            gotoxy(26, 10);
            printf("[2]: DIEM CAO%9s\u001b[0m\n", "");
        }
        else
        {
            gotoxy(26, 10);
            printf("[2]: Diem cao%9s\u001b[0m\n", "");
        }
        if (idxnutdatlai == 3)
        {
            gotoxy(22, 11);
            printf("%c", 16);
            ToMau(180);
            gotoxy(26, 11);
            printf("[3]: KHOI TAO TAT CA%2s\u001b[0m\n", "");
        }
        else
        {
            gotoxy(26, 11);
            printf("[3]: Khoi tao tat ca%2s\u001b[0m\n", "");
        }
        gotoxy(20, 20);
        printf("[%c] [%c]: Chon muc \t [enter]: Xac nhan \t [esc]: Quay lai menu", 30, 31);
        phimnhan = getch();
        if (phimnhan == 13)
        {
            if (idxnutdatlai == 1)
            {
                while (reset_cfg[0] == 0)
                {
                    system("cls");
                    gotoxy(26, 6);
                    printf("Dong y cai dat lai?");
                    gotoxy(26, 7);
                    printf("Thiet lap du lieu");
                    gotoxy(26, 8);
                    printf("[enter]\t: Xac nhan");
                    gotoxy(26, 9);
                    printf("[esc]\t\t: Huy");
                    phimnhan = getch();
                    if (phimnhan == 27)
                    {
                        system("cls");
                        break;
                    }
                    if (phimnhan == 13)
                    {
                        system("cls 2>cfg.ini");
                        if (FileRong("save.ini") == 0)
                        {
                            system("cls 2>save.ini");
                            luachon--;
                        }
                        capdo = 1;
                        map = 1;
                        luachonmaptrongmecung = 0;
                        LuuLaiTruocKhiThoat();
                        reset_cfg[0] = 1;
                        system("cls");
                         gotoxy(26, 9); printf("Thanh cong!");Sleep(1000);
                         system("cls");
                    }
                    if ((phimnhan != 13) && (phimnhan != 27))
                    {
                        reset_cfg[0] = 0;
                    }
                }
            }
            else if (idxnutdatlai == 2)
            {
                while (reset_cfg[1] == 0)
                {
                    system("cls");
                    gotoxy(26, 6);
                    printf("Dong y cai dat lai?");
                    gotoxy(26, 7);
                    printf("Diem cao");
                    gotoxy(26, 8);
                    printf("[enter]\t: Xac nhan");
                    gotoxy(26, 9);
                    printf("[esc]\t\t: Huy");
                    phimnhan = getch();
                    if (phimnhan == 27)
                    {
                        system("cls");
                        break;
                    }
                    if (phimnhan == 13)
                    {
                        for (i = 0; i < 5; i++)
                        {
                            diemcao[i] = 0;
                        }
                        XuatDiemCao("dckd.txt");
                        XuatDiemCao("dchd.txt");
                        XuatDiemCao("dccd.txt");
                        reset_cfg[1] = 1;

                        system("cls");
                         gotoxy(26, 9); printf("Thanh cong!");Sleep(1000);
                         system("cls");
                    }
                    if ((phimnhan != 13) && (phimnhan != 27))
                    {
                        reset_cfg[1] = 0;
                    }
                }
            }
            else if (idxnutdatlai == 3)
            {
                while (reset_cfg[2] == 0)
                {
                    system("cls");
                    gotoxy(26, 6);
                    printf("Dong y cai dat lai?");
                    gotoxy(26, 7);
                    printf("Khoi tao tat ca");
                    gotoxy(26, 8);
                    printf("[enter]\t: Xac nhan");
                    gotoxy(26, 9);
                    printf("[esc]\t\t: Huy");
                    phimnhan = getch();
                    if (phimnhan == 27)
                    {
                        system("cls");
                        break;
                    }
                    if (phimnhan == 13)
                    {
                        system("cls 2>cfg.ini");
                        if (FileRong("save.ini") == 0)
                        {
                            system("cls 2>save.ini");
                            luachon--;
                        }
                        capdo = 1;
                        map = 1;
                        luachonmaptrongmecung = 0;
                        for (i = 0; i < 5; i++)
                        {
                            diemcao[i] = 0;
                        }
                        XuatDiemCao("dckd.txt");
                        XuatDiemCao("dchd.txt");
                        XuatDiemCao("dccd.txt");
                        LuuLaiTruocKhiThoat();
                        reset_cfg[2] = 1;

                        system("cls");
                         gotoxy(26, 9); printf("Thanh cong!"); Sleep(1000);
                         system("cls");
                    }
                    if ((phimnhan != 13) && (phimnhan != 27))
                    {
                        reset_cfg[2] = 0;
                    }
                }
            }
            for (i = 0; i < 3; i++)
            {
                reset_cfg[i] = 0;
            }
        }

        else if (phimnhan == 27)
        {
            system("cls");
            break;
        }

        if (phimnhan == 72)
        {
            idxnutdatlai--;
        }
        else if (phimnhan == 80)
        {
            idxnutdatlai++;
        }

        if (idxnutdatlai < 1)
        {
            idxnutdatlai = 3;
        }
        else if (idxnutdatlai > 3)
        {
            idxnutdatlai = 1;
        }
    }
}

void Menu()
{
    XoaManHinh();
    int h = 9;
    hinh2();
    TaoKhungVienMenu(h);
    InKhungVienMenu(h);
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
        printf("[2]: CAP DO%7s\u001b[0m\n", "");
    }
    else
    {
        gotoxy(27, 10);
        printf("[2]: Cap do%7s\u001b[0m\n", "");
    }
    if (luachon == 3)
    {
        gotoxy(22, 11);
        printf("%c", 16);
        ToMau(180);
        gotoxy(26, 11);
        printf("[3]: CHE DO%7s\u001b[0m\n", "");
    }
    else
    {
        gotoxy(27, 11);
        printf("[3]: Che do%7s\u001b[0m\n", "");
    }
    if (luachon == 4)
    {
        gotoxy(22, 12);
        printf("%c", 16);
        ToMau(180);
        gotoxy(26, 12);
        printf("[4]: HUONG DAN%4s\u001b[0m\n", "");
    }
    else
    {
        gotoxy(27, 12);
        printf("[4]: Huong dan%4s\u001b[0m\n", "");
    }
    if (luachon == 5)
    {
        gotoxy(22, 13);
        printf("%c", 16);
        ToMau(180);
        gotoxy(26, 13);
        printf("[5]: DIEM CAO%5s\u001b[0m\n", "");
    }
    else
    {
        gotoxy(27, 13);
        printf("[5]: Diem cao%5s\u001b[0m\n", "");
    }
    if (luachon == 6)
    {
        gotoxy(22, 14);
        printf("%c", 16);
        ToMau(180);
        gotoxy(26, 14);
        printf("[6]: DAT LAI%6s\u001b[0m\n", "");
    }
    else
    {
        gotoxy(27, 14);
        printf("[6]: Dat lai%6s\u001b[0m\n", "");
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
            VanMoi();
        }
        else if (luachon == 2)
        {
            LuaChonCapDo();
        }
        else if (luachon == 3)
        {
            LuaChonCheDo();
        }
        else if (luachon == 4)
        {
            HuongDan();
        }
        else if (luachon == 5)
        {
            XemDiemCao();
        }
        else if (luachon == 6)
        {
            LuaChonDatLai();
        }
        else if (luachon == 7)
        {
            LuuLaiTruocKhiThoat();
            system("cls");
            system("attrib +r cfg.ini");
            system("attrib +r save.ini");
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
void MenuTiepTuc()
{
    XoaManHinh();
    int h = 10;
    hinh2();
    TaoKhungVienMenu(h);
    InKhungVienMenu(h);
    if (luachon == 1)
    {
        gotoxy(22, 9);
        printf("%c", 16);
        ToMau(180);
        gotoxy(26, 9);
        printf("[1]: TIEP TUC%5s\u001b[0m\n", "");
    }
    else
    {
        gotoxy(27, 9);
        printf("[1]: Tiep tuc%5s\u001b[0m\n", "");
    }
    if (luachon == 2)
    {
        gotoxy(22, 10);
        printf("%c", 16);
        ToMau(180);
        gotoxy(26, 10);
        printf("[2]: TRO CHOI MOI%1s\u001b[0m\n", "");
    }
    else
    {
        gotoxy(27, 10);
        printf("[2]: Tro choi moi%1s\u001b[0m\n", "");
    }
    if (luachon == 3)
    {
        gotoxy(22, 11);
        printf("%c", 16);
        ToMau(180);
        gotoxy(26, 11);
        printf("[3]: CAP DO%7s\u001b[0m\n", "");
    }
    else
    {
        gotoxy(27, 11);
        printf("[3]: Cap do%7s\u001b[0m\n", "");
    }
    if (luachon == 4)
    {
        gotoxy(22, 12);
        printf("%c", 16);
        ToMau(180);
        gotoxy(26, 12);
        printf("[4]: CHE DO%7s\u001b[0m\n", "");
    }
    else
    {
        gotoxy(27, 12);
        printf("[4]: Che do%7s\u001b[0m\n", "");
    }
    if (luachon == 5)
    {
        gotoxy(22, 13);
        printf("%c", 16);
        ToMau(180);
        gotoxy(26, 13);
        printf("[5]: HUONG DAN%4s\u001b[0m\n", "");
    }
    else
    {
        gotoxy(27, 13);
        printf("[5]: Huong dan%4s\u001b[0m\n", "");
    }
    if (luachon == 6)
    {
        gotoxy(22, 14);
        printf("%c", 16);
        ToMau(180);
        gotoxy(26, 14);
        printf("[6]: DIEM CAO%5s\u001b[0m\n", "");
    }
    else
    {
        gotoxy(27, 14);
        printf("[6]: Diem cao%5s\u001b[0m\n", "");
    }
    if (luachon == 7)
    {
        gotoxy(22, 15);
        printf("%c", 16);
        ToMau(180);
        gotoxy(26, 15);
        printf("[7]: DAT LAI%6s\u001b[0m\n", "");
    }
    else
    {
        gotoxy(27, 15);
        printf("[7]: Dat lai%6s\u001b[0m\n", "");
    }
    if (luachon == 8)
    {
        gotoxy(22, 16);
        printf("%c", 16);
        ToMau(180);
        gotoxy(26, 16);
        printf("[8]: THOAT%8s\u001b[0m", "");
    }
    else
    {
        gotoxy(27, 16);
        printf("[8]: Thoat%8s\u001b[0m\n", "");
    }
    gotoxy(20, 20);
    printf("[%c] [%c]: Chon muc \t [enter]: Xac nhan", 30, 31);
    phimnhan = getch();

    if (phimnhan == 13)
    {
        if (luachon == 1)
        {
            TiepTuc();
        }
        else if (luachon == 2)
        {
            VanMoi();
        }
        else if (luachon == 3)
        {
            LuaChonCapDo();
        }
        else if (luachon == 4)
        {
            LuaChonCheDo();
        }
        else if (luachon == 5)
        {
            HuongDan();
        }
        else if (luachon == 6)
        {
            XemDiemCao();
        }
        else if (luachon == 7)
        {
            LuaChonDatLai();
        }
        else if (luachon == 8)
        {
            LuuLaiTruocKhiThoat();
            system("cls");
            system("attrib +r cfg.ini");
            system("attrib +r save.ini");
            exit(0);
        }
    }
    if (phimnhan == 72)
    {
        luachon--;
    }
    else if (phimnhan == 80)
    {
        luachon++;
    }
    if (luachon < 1)
    {
        luachon = 8;
    }
    else if (luachon > 8)
    {
        luachon = 1;
    }
}
void HinhNen()
{
        printf("\n\n");
        printf("\n\t             /^\\/^\\                                            ");
        printf("\n\t           _|_o|  O|                                             ");
        printf("\n\t \\/     /~     \\_/ \\                                          ");
        printf("\n\t  \\____|__________/  \\                                         ");
        printf("\n\t         \\_______      \\                                       ");
        printf("\n\t                 `\\     \\                  \\                  ");
        printf("\n\t                   |     |                   \\                  ");
        printf("\n\t                  /      /                    \\                 ");
        printf("\n\t                 /     /                       \\\\              ");
        printf("\n\t               /      /                         \\ \\            ");
        printf("\n\t              /     /                            \\  \\          ");
        printf("\n\t            /     /             _----_            \\   \\        ");
        printf("\n\t           /     /           _-~      ~-_         |   |          ");
        printf("\n\t          (      (        _-~    _--_    ~-_     _/   |          ");
        printf("\n\t           \\      ~-____-~    _-~    ~-_    ~-_-~    /          ");
        printf("\n\t             ~-_           _-~          ~-_       _-~            ");
        printf("\n\t                ~--______-~                ~-___-~             \n\n");
        printf("\n                                Nhan phim bat ki ...        ");
        if (getch())
        {
            system("cls");
        }
}
int main()
{
    system("cls");
    AnConTroDi();
    HinhNen();
    srand((unsigned) time(NULL));
    TaoCacFileCanThiet();
    MoLaiDaLuuTruocDo();
    while (true)
    {
        if (FileRong("save.ini") == 0)
        {
            MenuTiepTuc();
        }
        else
        {
            Menu();
        }
    }
    LuuLaiTruocKhiThoat();

    return 0;
}
