#pragma warning(disable: 4996)
#include <iostream>
#include <ctime>
#include <windows.h>
#include <vector>
#include <conio.h>
#include <algorithm>
using namespace std;

const int LEFT = 4;
const int TOP = 4;
const int MaxI = 22;
const int MaxJ = 10;
const int A_X = 21;
const int A_Y = 31;

int Board[MaxI][MaxJ];
char a[A_X][A_Y];
int capdo = 1, score = 0;
float speed = 0.35;

typedef struct {
    vector < vector < int >> arr1;
    int Row1;
    int Col1;
    int iBoard1;
    int jBoard1;
}
KhoiGach_ver2;

vector < KhoiGach_ver2 > TaoKhoiGach_Ver2(int ID) {
    vector < KhoiGach_ver2 > pkhoigach(sizeof(KhoiGach_ver2));
    if (ID == 15) {
        pkhoigach[4].Row1 = 4;
        pkhoigach[1].Col1 = 1;
        pkhoigach[2].iBoard1 = 0;
        pkhoigach[3].jBoard1 = 5;
    } else if (ID == 31) {
        pkhoigach[4].Row1 = 2;
        pkhoigach[1].Col1 = 2;
        pkhoigach[2].iBoard1 = 2;
        pkhoigach[3].jBoard1 = 5;
    } else {
        pkhoigach[4].Row1 = 2;
        pkhoigach[1].Col1 = 3;
        pkhoigach[2].iBoard1 = 2;
        pkhoigach[3].jBoard1 = 5;
    }

    pkhoigach[0].arr1.resize(pkhoigach[4].Row1, vector < int > (pkhoigach[1].Col1 * sizeof(int)));

    for (int k = 0; k < pkhoigach[1].Col1 * pkhoigach[4].Row1; ++k) {
        pkhoigach[0].arr1[k / pkhoigach[1].Col1][k % pkhoigach[1].Col1] = (ID >> (pkhoigach[1].Col1 * pkhoigach[4].Row1 - 1 - k)) & 1;
    }
    return pkhoigach;
}

void AnConTro() {
    CONSOLE_CURSOR_INFO Info;
    Info.bVisible = FALSE;
    Info.dwSize = 20;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), & Info);
}

void gotoxy(int x, int y) {
    COORD c;
    c.X = x - 1;
    c.Y = y - 1;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void textcolor(WORD color) {
    HANDLE hConsoleOutput;
    hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;
    GetConsoleScreenBufferInfo(hConsoleOutput, & screen_buffer_info);

    WORD wAttributes = screen_buffer_info.wAttributes;
    color &= 0x000f;
    wAttributes &= 0xfff0;
    wAttributes |= color;

    SetConsoleTextAttribute(hConsoleOutput, wAttributes);
}

void XoaManHinh() {
    HANDLE hOut;
    COORD Position;
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    Position.X = 0;
    Position.Y = 0;
    SetConsoleCursorPosition(hOut, Position);
}

void VeKhoiGach_Ver2(vector < KhoiGach_ver2 > & pkhoigach) {
    for (int i = 0; i < pkhoigach[4].Row1; ++i) {
        for (int j = 0; j < pkhoigach[1].Col1; ++j) {
            if (pkhoigach[0].arr1[i][j] == 1 && (pkhoigach[2].iBoard1 + i) > 3) {
                gotoxy(LEFT + pkhoigach[3].jBoard1 + j + 1, TOP + pkhoigach[2].iBoard1 + i - 3);
                textcolor(11);
                cout << '#';
            }
        }
    }
}

void XoaKhoiGach(vector < KhoiGach_ver2 > & pkhoigach) {
    for (int i = 0; i < pkhoigach[4].Row1; ++i) {
        for (int j = 0; j < pkhoigach[1].Col1; ++j) {
            if (pkhoigach[0].arr1[i][j] == 1 && (pkhoigach[2].iBoard1 + i) > 3) {
                gotoxy(LEFT + pkhoigach[3].jBoard1 + j + 1, TOP + pkhoigach[2].iBoard1 + i - 3);
                cout << ' ';
            }
        }
    }
}

void Ve_Next(int ID) {
    vector < KhoiGach_ver2 > pNext = TaoKhoiGach_Ver2(ID);
    int iRoot = LEFT + MaxJ + 5;
    int jRoot = TOP;
    for (int i = 0; i < pNext[4].Row1; ++i) {
        for (int j = 0; j < pNext[1].Col1; ++j) {
            if (pNext[0].arr1[i][j] == 1) {
                textcolor(11);
                gotoxy(iRoot + j + 1, jRoot + i + 1);
                cout << '#';
            }
        }
    }
}

void Xoa_Next() {
    int iRoot = LEFT + MaxJ + 5;
    int jRoot = TOP;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            textcolor(0);
            gotoxy(iRoot + j + 1, jRoot + i + 1);
            cout << ' ';
        }
    }
}

int LoaiGach() {
    int x = rand() % 7;
    switch (x) {
    case 0:
        return 15;
        break;
    case 1:
        return 31;
        break;
    case 2:
        return 51;
        break;
    case 3:
        return 30;
        break;
    case 4:
        return 58;
        break;
    case 5:
        return 57;
        break;
    case 6:
        return 60;
        break;
    }
}

void ReSetBoard() {
    for (int i = 0; i < MaxI; ++i) /// O(N^2)
    {
        for (int j = 0; j < MaxJ; ++j) {
            Board[i][j] = 0;
        }
    }
}

void DrawBoard() {
    for (int i = LEFT; i <= LEFT + 10 + 1; ++i) /// O(N^2)
    {
        for (int j = TOP; j <= TOP + 18 + 1; ++j) {
            if ((j == TOP || j == TOP + 18 + 1) && i > LEFT && i < LEFT + 10 + 1) {
                gotoxy(i, j);
                textcolor(7);
                cout << (char) 196;
            }
            if ((i == LEFT || i == LEFT + 10 + 1) && j > TOP && j < TOP + 18 + 1) {
                gotoxy(i, j);
                textcolor(7);
                cout << (char) 179;
            }
        }
    }
    gotoxy(20, 11);
    cout << "Diem so: " << score;
}

void DisplayBoard() {
    for (int i = 0; i < MaxI; ++i) {
        for (int j = 0; j < MaxJ; ++j) {
            if (Board[i][j] == 1 && i >= 4) {
                gotoxy(j + LEFT + 1, i + TOP + 1 - 4);
                textcolor(15);
                cout << '#';
            }
            if (Board[i][j] == 0 && i >= 4) {
                gotoxy(j + LEFT + 1, i + TOP + 1 - 4);
                cout << ' ';
            }
        }
    }
}

int Inside(int i, int j) {
    return (i >= 0 && i < MaxI && j >= 0 && j < MaxJ);
}

int Left(int i, int j) {
    if (j > 0 && Inside(i, j) && Board[i][j - 1] == 0) return 1;
    return 0;
}

int Right(int i, int j) {
    if (j < MaxJ - 1 && Inside(i, j) && Board[i][j + 1] == 0) return 1;
    return 0;
}

int Down(int i, int j) {
    if (i < MaxI - 1 && Inside(i, j) && Board[i + 1][j] == 0) return 1;
    return 0;
}

void SangTrai(vector < KhoiGach_ver2 > & pkhoigach) {
    for (int i = 0; i < pkhoigach[4].Row1; ++i) {
        for (int j = 0; j < pkhoigach[1].Col1; ++j) {
            if (pkhoigach[0].arr1[i][j] == 1) {
                if (Left(pkhoigach[2].iBoard1 + i, pkhoigach[3].jBoard1 + j) == 0 || pkhoigach[2].iBoard1 <= 3) return;
            }
        }
    }
    pkhoigach[3].jBoard1 -= 1; //Dich vi tri cua bang trang thai sang trai 1 so voi Board[22][10].
}

void SangPhai(vector < KhoiGach_ver2 > & pkhoigach) {
    for (int i = 0; i < pkhoigach[4].Row1; ++i) {
        for (int j = 0; j < pkhoigach[1].Col1; ++j) {
            if (pkhoigach[0].arr1[i][j] == 1) {
                if (Right(pkhoigach[2].iBoard1 + i, pkhoigach[3].jBoard1 + j) == 0 || pkhoigach[2].iBoard1 <= 3) return;
            }
        }
    }
    pkhoigach[3].jBoard1 += 1; //Dich vi tri cua bang trang thai sang trai 1 so voi Board[22][10].
}

int RoiXuong(vector < KhoiGach_ver2 > & pkhoigach) {
    for (int i = 0; i < pkhoigach[4].Row1; ++i) {
        for (int j = 0; j < pkhoigach[1].Col1; ++j) {
            if (pkhoigach[0].arr1[i][j] == 1) {
                if (Down(pkhoigach[2].iBoard1 + i, pkhoigach[3].jBoard1 + j) == 0) return 0;
            }
        }
    }
    pkhoigach[2].iBoard1 += 1; //Roi xuong 1 so voi Board[22][10]
    return 1;
}

void GanGiaTri(vector < KhoiGach_ver2 > & pkhoigach) {
    for (int i = 0; i < pkhoigach[4].Row1; ++i) {
        for (int j = 0; j < pkhoigach[1].Col1; ++j) {
            if (pkhoigach[0].arr1[i][j] == 1) {
                Board[pkhoigach[2].iBoard1 + i][pkhoigach[3].jBoard1 + j] = 1;
            }
        }
    }
}

void CapNhatLaiToaDo(int hang) {
    //textcolor(11);
    for (int i = hang; i > 0; --i) {
        for (int j = 0; j < MaxJ; ++j) {
            Board[i][j] = Board[i - 1][j];
        }
    }
}

void ToMauToaDo(int hang) {
    for (int i = hang; i > 0; --i) {
        for (int j = 0; j < MaxJ; ++j) {
            gotoxy(j + LEFT + 1, i + 1);
            textcolor(12);
            cout << '#';
        }
        break;
    }
}

void CapNhat(int x) {
    score += x;
    textcolor(7);
    gotoxy(29, 11);
    cout << score;
    if (score >= 200 && score < 500) {
        speed = 0.30;
    } else if (score >= 500) {
        speed = 0.25;
    }
}

void XoayKhoiGach(vector < KhoiGach_ver2 > & pkhoigach) {
    int i, j;

    int tmpCol = pkhoigach[4].Row1;
    int tmpRow = pkhoigach[1].Col1;

    vector < vector < int >> tmpArr((tmpRow * sizeof(int)), vector < int > (tmpCol * sizeof(int)));

    for (i = pkhoigach[4].Row1 - 1; i >= 0; --i) {
        for (j = pkhoigach[1].Col1 - 1; j >= 0; --j) {
            tmpArr[j][pkhoigach[4].Row1 - i - 1] = pkhoigach[0].arr1[i][j];
        }
    }
    //check hop le
    for (i = 0; i < tmpRow; ++i) {
        for (j = 0; j < tmpCol; ++j) {
            if (!Inside(pkhoigach[2].iBoard1 + i, pkhoigach[3].jBoard1 + j) || Board[pkhoigach[2].iBoard1 + i][pkhoigach[3].jBoard1 + j] == 1) {
                return;
            }
        }
    }

    //cap nhat thay doi sau khi xoay
    pkhoigach[1].Col1 = tmpCol;
    pkhoigach[4].Row1 = tmpRow;
    pkhoigach[0].arr1 = tmpArr;
}

int KiemTra(vector < KhoiGach_ver2 > & pkhoigach) //-1 : gameover 0: win
{
    int i, j, count;
    i = pkhoigach[4].Row1 - 1;
    if (pkhoigach[2].iBoard1 <= 3) return -1; //Gameover

    if (score >= 1500) return 0; //Win

    while (i >= 0) {
        count = 0;
        for (j = 0; j < MaxJ; ++j) {
            if (Board[pkhoigach[2].iBoard1 + i][j] == 1) {
                count++;
            }
        }
        if (count == MaxJ) // neu ma gach tao nen 1 day deu nhau, khong bi dut doan thi cap nhat lai diem:
        {
            CapNhat(20);
            ToMauToaDo(pkhoigach[2].iBoard1 + i);
            CapNhatLaiToaDo(pkhoigach[2].iBoard1 + i);
            Sleep(50);
            DisplayBoard();
        } else {
            i = i - 1;
        }
    }
    return 1;
}

int xuly() {
    system("cls");
    int IDKhoiTiepTheo;
    clock_t Start, End;
    char c;
    vector < KhoiGach_ver2 > currKhoi = TaoKhoiGach_Ver2(LoaiGach());
    IDKhoiTiepTheo = LoaiGach();
    DrawBoard();
    while (true) {
        VeKhoiGach_Ver2(currKhoi);
        Xoa_Next();
        Ve_Next(IDKhoiTiepTheo);
        Start = clock();
        while (float(End - Start) / CLK_TCK < speed) {
            if (kbhit()) {
                c = int(getch());
                XoaKhoiGach(currKhoi);
                if (c == 72) {
                    XoayKhoiGach(currKhoi);
                } else if (c == 75) {
                    SangTrai(currKhoi);
                } else if (c == 77) {
                    SangPhai(currKhoi);
                } else if (c == 80) {
                    RoiXuong(currKhoi);
                } else if (c == 27) {
                    ReSetBoard();
                    return 0;
                }
                VeKhoiGach_Ver2(currKhoi);
            }
            End = clock();
        }
        Sleep(10);
        XoaKhoiGach(currKhoi);
        if (RoiXuong(currKhoi) == 0) {
            GanGiaTri(currKhoi);
            int ketqua = KiemTra(currKhoi);
            if ((ketqua == -1 || ketqua == 0)) {
                ReSetBoard();
                return 0;
            }
            currKhoi = TaoKhoiGach_Ver2(IDKhoiTiepTheo);
            IDKhoiTiepTheo = LoaiGach();
        }
        DisplayBoard();
    }
    return 1;
}

signed main() {
    srand(time(0));
    AnConTro();
    XoaManHinh();
    xuly();
    Sleep(1000);
    system("cls");
    textcolor(7);
    gotoxy(10, 11);
    cout << "Diem so: " << score;
    getch();
    return 0;
}
