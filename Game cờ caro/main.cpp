#include <iostream>
#include <Windows.h>
#include <math.h>
#include <iomanip>
#include <conio.h>
#include <stack>	// thư viện hỗ trợ ngăn xếp
#include <time.h>
#include <fstream>
#include <string>
#include <string.h>
#include <vector>

#define Max 11
using namespace std;

char a[Max][Max];	// khai bao mang co max dong, max cot

int sodong = 10, socot = 10;
int soquancotoithieudewin = 5;
int doronggiua2cot = 3, doronggiua2dong = 1;	// do rong cua dong voi cot
int vitrixbandau = 2, vitriybandau = 1;	// vi tri ban dau cua ban co
int luotdibandau = 0;	  // 0 - la X, 1 - la O.
int loadmap = 0;	// mac dinh che do load map = 1, con khong co thi thoi, danh ban co moi

struct NutNhan
{
	string ChucNang; // Nội dung chức năng hiển thị ra
	int MaMau; // mã màu in ra
	int x, y; // Tọa độ xuất hiện của nút tính theo góc trên cùng bên trái
	int DoRong; // độ rộng bề ngang của nút nhấn
};
typedef struct NutNhan NUTNHAN;

HANDLE hStdin;
DWORD fdwSaveOldMode;

void textcolor(int x);
void TaoNutNhanChucNang(NUTNHAN nut);
void XuLyVaTaoNut();
void XuLyVaTaoNut2();

int SoLuongCacNutNhan = 4;
NUTNHAN *DanhSachNutNhan = new NUTNHAN[SoLuongCacNutNhan];
int indexCuaMenuDangDuocChon = 0;

VOID ErrorExit(LPSTR lpszMessage)
{
	fprintf(stderr, "%s\n", lpszMessage);

	// Restore input mode on exit.

	SetConsoleMode(hStdin, fdwSaveOldMode);

	ExitProcess(0);
}
/* vì do ngăn xếp chỉ được lưu 1 kiểu dữ liệu duy nhất, nhưng chương trình của chúng ta có nhiều kiểu dữ liệu khác nhau, vì vậy ta sẽ dùng struct
quy lại thành chỉ có 1 biến thôi ( biến struct). Và stack này mình sẽ dùng chung cho toàn bài, mỗi lần đánh quân cơ sẽ lưu lại vị trí của nó vào
*/
struct ToaDo
{
	int toado_dong, toado_cot;
};
typedef struct ToaDo toado;	// thay vì gọi 2 từ ở trên để truyền đến trong hàm thì mình chỉ cần gọi toado là được rồi

void textcolor(int x) {
	HANDLE mau;
	mau = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(mau, x);
}
// Hàm dịch chuyển con trỏ đến tọa độ x, y.
void gotoxy(int x, int y) {
	HANDLE hConsoleOutput;
	COORD Cursor_an_Pos = { x - 1,y - 1 };
	hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hConsoleOutput, Cursor_an_Pos);
}
// Hàm xóa màn hình.
void XoaManHinh() {
	HANDLE hOut;
	COORD Position;
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	Position.X = 0;
	Position.Y = 0;
	SetConsoleCursorPosition(hOut, Position);
}
void Hinh()
{
    gotoxy(50,12); cout<<"                        __  ,-.   ,---.   ";
    gotoxy(50,13); cout<<"                      ,' ,'/ /|  '   ,'\\  ";
    gotoxy(50,14); cout<<"   ,---.     ,--.--.  '  | |' | /   /   | ";
    gotoxy(50,15); cout<<"  /     \\   /       \\ |  |   ,'.   ; ,. : ";
    gotoxy(50,16); cout<<" /    / '  .--.  .-. |'  :  /  '   | |: : ";
    gotoxy(50,17); cout<<".    ' /    \\__\\/: . .|  | '   '   | .; : ";
    gotoxy(50,18); cout<<"'   ; :__   ,* .--.; |;  : |   |   :    | ";
    gotoxy(50,19); cout<<"'   | '.'| /  /  ,.  ||  , ;    \\   \\  /  ";
    gotoxy(50,20); cout<<"|   :    :;  :   .'   \\\\---'      `----'   ";
    gotoxy(50,21); cout<<" \\   \\  / |  ,     .-./                   ";
    gotoxy(50,22); cout<<"  `----'   `--`---'                  ";
}
void HINHNEN()
{   // hình nền giao diện
    gotoxy(18,3); cout<<"/\\\\\\\\\\\\\\\\\\     /\\\\\\\\\\\\\\\\\\       /\\\\\\\\\\\\\\\\\\            /\\\\\\\\\\ ";
    gotoxy(16,4); cout<<"/\\\\\\////////    /\\\\\\\\\\\\\\\\\\\\\\\\\\   /\\\\\\///////\\\         /\\\\\\///\\\\\\ ";
    gotoxy(15,5); cout<<"/\\\\\\/            /\\\\\\/////////\\\\\\ \\/\\\\\\     \\/\\\\\\     /\\\\\\/  \\///\\\\\\ ";
    gotoxy(15,6); cout<<"/\\\\\\             \\/\\\\\\       \\/\\\\\\ \\/\\\\\\\\\\\\\\\\\\\\\\/     /\\\\\\      \\//\\\\\ ";
    gotoxy(15,7); cout<<"\\/\\\\\\             \\/\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ \\/\\\\\\//////\\\\\\    \\/\\\\\\       \\/\\\\\ ";
    gotoxy(16,8); cout<<"\\//\\\\\\            \\/\\\\\\/////////\\\\\\ \\/\\\\\\    \\//\\\\\\   \\//\\\\\\      /\\\\\\ ";
    gotoxy(18,9); cout<<"\\///\\\\\\          \\/\\\\\\       \\/\\\\\\ \\/\\\\\\     \\//\\\\\\   \\///\\\\\\  /\\\\\\ ";
    gotoxy(21,10); cout<<"\\////\\\\\\\\\\\\\\\\\\ \\/\\\\\\       \\/\\\\\\ \\/\\\\\\      \\//\\\\\\    \\///\\\\\\\\\\/ ";
    gotoxy(25,11); cout<<"\\/////////  \\///        \\///  \\///        \\///       \\///// ";
}
void VeBanCoCoToaDo(int sodong, int socot, int doronggiua2cot, int doronggiua2dong, char a[][Max]) {	// ban chat mang a luu toa do roi nen khong can khai bao nua
	int sokhoangcachluisovoiletrai = (int)log10((double)sodong) + 1;
	// in ra chi so cot:
	cout << setw((doronggiua2cot + 2) / 2 + 1 + sokhoangcachluisovoiletrai);
	for (int j = 0; j < socot - 1; j++) {
		textcolor(7);
		cout << j;
		cout << setw(doronggiua2cot + 1);
	}
	cout << socot - 1 << endl;
	//textcolor(7);
	cout << setw(sokhoangcachluisovoiletrai + 1);
	// neu muon thay doi kich co ban co thi ta se co 1 bien t, neu nhan phim t thi doronggiua2cot ++, hoac g thi doronggiua2cot --
	textcolor(1);
	cout << (char)218;
	for (int j = 1; j <= socot - 1; j++) {	// ve hang ngang dau tien
		for (int k = 1; k <= doronggiua2cot; k++) {
			cout << (char)196;
		}
		cout << (char)194;
	}
	for (int k = 1; k <= doronggiua2cot; k++) {
		cout << (char)196;
	}
	cout << (char)191 << endl;

	// in ra nhung dong ben trong
	for (int i = 1; i <= sodong - 1; i++) {
		for (int l = 1; l <= doronggiua2dong; l++) {
			textcolor(7);
			//in ra chi so cua dong:
			if (l != doronggiua2dong / 2 + 1) {

				cout << setw(sokhoangcachluisovoiletrai + 1);
			}
			else {
				cout << i - 1;
				int sochuso = (i - 1) == 0 ? 1 : (int)log10(i - 1) + 1;
				cout << setw(sokhoangcachluisovoiletrai - sochuso + 1);
			}
			textcolor(1);
			// in ra gach dung ben phai ban co
			for (int j = 1; j <= socot; j++) {

				cout << (char)179;
				for (int k = 1; k <= doronggiua2cot; k++) {
					cout << " ";
				}
			}
			cout << (char)179 << endl;
		}
		//textcolor(7);
		cout << setw(sokhoangcachluisovoiletrai + 1);
		cout << (char)195;
		textcolor(1);
		for (int j = 1; j <= socot - 1; j++) {	// ve hang ngang dau tien
			for (int k = 1; k <= doronggiua2cot; k++) {
				cout << (char)196;
			}
			cout << (char)197;
		}
		for (int k = 1; k <= doronggiua2cot; k++) {	// ve goc chu thap ngan cach cac o vuong
			cout << (char)196;
		}
		cout << (char)180 << endl;

	}
	for (int l = 1; l <= doronggiua2dong; l++) {
		textcolor(7);
		// in ra chi so dong cua hang cuoi cung:
		if (l != doronggiua2dong / 2 + 1) {
			cout << setw(sokhoangcachluisovoiletrai + 1);
		}
		else {
			cout << sodong - 1;
			int sochuso = (sodong - 1) == 0 ? 1 : (int)log10(sodong - 1) + 1;
			//textcolor(7);
			cout << setw(sokhoangcachluisovoiletrai - sochuso + 1);
		}
		textcolor(1);
		// in ra gach dung ben phai ban co
		for (int j = 1; j <= socot; j++) {
			cout << (char)179;
			for (int k = 1; k <= doronggiua2cot; k++) {
				cout << " ";
			}
		}
		cout << (char)179 << endl;
	}
	// in ra hang ngang cuoi cung de tao thanh 1 cai ban co hoan chinh
	// ve hang ngang cuoi cung:
	textcolor(1);
	cout << setw(sokhoangcachluisovoiletrai + 1);
	cout << (char)192;
	for (int j = 1; j <= socot - 1; j++) {	// ve hang ngang dau tien
		for (int k = 1; k <= doronggiua2cot; k++) {
			cout << (char)196;
		}
		cout << (char)193;
	}
	for (int k = 1; k <= doronggiua2cot; k++) {
		cout << (char)196;
	}
	cout << (char)217 << endl;
}

void VeBanCoKhongCoToaDo(int sodong, int socot, int doronggiua2cot, int doronggiua2dong, char a[][Max], int vitrixbandau, int vitriybandau) {	// ban chat mang a luu toa do roi nen khong can khai bao nua
	// neu muon thay doi kich co ban co thi ta se co 1 bien t, neu nhan phim t thi doronggiua2cot ++, hoac g thi doronggiua2cot --
	gotoxy(vitrixbandau, vitriybandau);

	cout << (char)218;
	for (int j = 1; j <= socot - 1; j++) {	// ve hang ngang dau tien
		for (int k = 1; k <= doronggiua2cot; k++) {
			cout << (char)196;
		}
		cout << (char)194;
	}
	for (int k = 1; k <= doronggiua2cot; k++) {
		cout << (char)196;
	}
	cout << (char)191;
	vitriybandau++;
	gotoxy(vitrixbandau, vitriybandau);

	// in ra nhung dong ben trong
	for (int i = 1; i <= sodong - 1; i++) {
		for (int l = 1; l <= doronggiua2dong; l++) {
			// in ra gach dung ben phai ban co
			for (int j = 1; j <= socot; j++) {
				cout << (char)179;
				for (int k = 1; k <= doronggiua2cot; k++) {
					cout << " ";
				}
			}
			cout << (char)179;
			vitriybandau++;
			gotoxy(vitrixbandau, vitriybandau);
		}
		cout << (char)195;
		for (int j = 1; j <= socot - 1; j++) {	// ve hang ngang dau tien
			for (int k = 1; k <= doronggiua2cot; k++) {
				cout << (char)196;
			}
			cout << (char)197;
		}
		for (int k = 1; k <= doronggiua2cot; k++) {	// ve goc chu thap ngan cach cac o vuong
			cout << (char)196;
		}
		cout << (char)180;
		vitriybandau++;
		gotoxy(vitrixbandau, vitriybandau);

	}
	for (int l = 1; l <= doronggiua2dong; l++) {
		// in ra gach dung ben phai ban co
		for (int j = 1; j <= socot; j++) {
			cout << (char)179;
			for (int k = 1; k <= doronggiua2cot; k++) {
				cout << " ";
			}
		}
		cout << (char)179;
		vitriybandau++;
		gotoxy(vitrixbandau, vitriybandau);
	}
	// in ra hang ngang cuoi cung de tao thanh 1 cai ban co hoan chinh
	// ve hang ngang cuoi cung:
	cout << (char)192;
	for (int j = 1; j <= socot - 1; j++) {	// ve hang ngang dau tien
		for (int k = 1; k <= doronggiua2cot; k++) {
			cout << (char)196;
		}
		cout << (char)193;
	}
	for (int k = 1; k <= doronggiua2cot; k++) {
		cout << (char)196;
	}
	cout << (char)217;
	vitriybandau++;
	/*gotoxy(vitrixbandau, vitriybandau);
	for (int i = 1; i < 39; i++) {
		textcolor(7);
		gotoxy(ChieuDaiConsoleDB - 69, i);
		cout << (char)179;
	}
	for (int i = 1; i < ChieuDaiConsoleDB; i++) {
		textcolor(7);
		gotoxy(i, 38);
		cout << (char)196;
		if (i == 82) {
			gotoxy(i, 38); cout << (char)193;
		}
	}
	gotoxy(ChieuDaiConsoleDB, vitriybandau);	// in ra thanh cuối cùng
	for (int i = 1; i < 39; i++) {
		textcolor(7);
		gotoxy(ChieuDaiConsoleDB, i);
		cout << (char)179;
		if (i == 38) {
			gotoxy(ChieuDaiConsoleDB, i); cout << (char)217;
		}
	}*/
}
char KiemTraHangNgang(char a[][Max], int sodong, int socot, int vitridong, int vitricot, int soquancotoithieudewin) {
	int dem = 1; // Tính luôn quân vừa đánh xuống

	// Xét qua bên trái trước (ngang trái)
	for (int j = vitricot - 1; j >= 0; --j) {
		if (a[vitridong][j] == a[vitridong][vitricot]) {
			dem++;
			if (dem == soquancotoithieudewin) {
				return a[vitridong][vitricot];
			}
		}
		// chỉ cần phát hiện không còn tính liên tục => dừng quá trình lặp lại
		else {
			break;
		}
	}

	// Xét qua bên phải (ngang phải)
	for (int j = vitricot + 1; j < socot; ++j) {
		if (a[vitridong][j] == a[vitridong][vitricot]) {
			dem++;

			if (dem == soquancotoithieudewin) {
				return a[vitridong][vitricot];
			}
		}
		// chỉ cần phát hiện không còn tính liên tục => dừng quá trình lặp lại
		else {
			break;
		}
	}
	return ' '; // chưa thắng
}
char KiemTraHangDoc(char a[][Max], int sodong, int socot, int vitridong, int vitricot, int soquancotoithieudewin) {
	int dem = 1; // Tính luôn quân vừa đánh xuống

	// Xét lên trên trước (dọc trên)
	for (int i = vitridong - 1; i >= 0; --i) {
		if (a[i][vitricot] == a[vitridong][vitricot]) {
			dem++;

			if (dem == soquancotoithieudewin) {

				return a[vitridong][vitricot];
			}
		}
		else {	// chỉ cần phát hiện không còn tính liên tục => dừng quá trình lặp lại
			break;
		}
	}
	// Xét xuống dưới (dọc dưới)
	for (int i = vitridong + 1; i < sodong; ++i) {
		if (a[i][vitricot] == a[vitridong][vitricot]) {
			dem++;

			if (dem == soquancotoithieudewin) {
				return a[vitridong][vitricot];
			}
		}
		// chỉ cần phát hiện không còn tính liên tục => dừng quá trình lặp lại
		else {
			break;
		}
	}
	return ' '; // chưa thắng
}

// vitridong: Là vị trí dòng của quân vừa đánh xuống
// vitricot: Là vị trí cột của quân vừa đánh xuống
char KiemTraCheoChinh(char a[][Max], int sodong, int socot, int vitridong, int vitricot, int soquancotoithieudewin) {
	int dem = 1; // Tính luôn quân vừa đánh xuống

	// Kiểm tra chéo chính trên: Các phần tử dòng - cột luôn giảm 1 đơn vị và giảm đến khi nào 1 trong 2 thằng xuất hiện 0
	int i = vitridong - 1;
	int j = vitricot - 1;
	while (true) {
		if (i < 0 || j < 0) {
			break; // vị trí xét không hợp lệ
		}

		// Nếu còn chạy xuống đây được thì có nghĩa vị trí i, j hợp lệ
		if (a[i][j] == a[vitridong][vitricot]) {
			dem++;

			if (dem == soquancotoithieudewin) {

				return a[vitridong][vitricot];
			}
		}
		// chỉ cần phát hiện không còn tính liên tục => dừng quá trình lặp lại
		else {

			break;
		}

		// Xét qua lần lặp mới
		i--;
		j--;
	}

	//	Kiểm tra chéo chính dưới: Các phần tử dòng - cột đều tăng lên 1 đơn vị và tăng đến khi nào 1 trong 2 thằng bằng n - 1 thì dừng lại
	i = vitridong + 1;
	j = vitricot + 1;
	while (true) {
		if (i == sodong || j == socot) {
			break; // vị trí xét không hợp lệ
		}

		// Nếu còn chạy xuống đây được thì có nghĩa vị trí i, j hợp lệ
		if (a[i][j] == a[vitridong][vitricot]) {
			dem++;

			if (dem == soquancotoithieudewin) {

				return a[vitridong][vitricot];
			}
		}
		// chỉ cần phát hiện không còn tính liên tục => dừng quá trình lặp lại
		else {
			break;
		}

		// Xét qua lần lặp mới
		i++;
		j++;
	}
	return ' '; // chưa thắng
}

// vitridong: Là vị trí dòng của quân vừa đánh xuống
// vitricot: Là vị trí cột của quân vừa đánh xuống
char KiemTraCheoPhu(char a[][Max], int sodong, int socot, int vitridong, int vitricot, int soquancotoithieudewin) {
	int dem = 1; // Tính luôn quân vừa đánh xuống

	// Kiểm tra chéo phụ trên: Dòng giảm - cột tăng. Dòng giảm tối đa tới 0, Cột tăng tối đa nới n - 1
	int i = vitridong - 1;
	int j = vitricot + 1;
	while (true) {
		if (i < 0 || j == socot) {
			break; // vị trí xét không hợp lệ
		}

		// Nếu còn chạy xuống đây được thì có nghĩa vị trí i, j hợp lệ
		if (a[i][j] == a[vitridong][vitricot]) {
			dem++;

			if (dem == soquancotoithieudewin) {
				return a[vitridong][vitricot];
			}
		}
		// chỉ cần phát hiện không còn tính liên tục => dừng quá trình lặp lại
		else {
			break;
		}

		// Xét qua lần lặp mới
		i--;
		j++;
	}

	//	Kiểm tra chéo phụ dưới: Dòng tăng - cột giảm. Dòng tăng tới tối đa là n - 1 và cột giảm tới tối đa là 0
	i = vitridong + 1;
	j = vitricot - 1;
	while (true) {
		if (i == sodong || j < 0) {
			break; // vị trí xét không hợp lệ
		}

		// Nếu còn chạy xuống đây được thì có nghĩa vị trí i, j hợp lệ
		if (a[i][j] == a[vitridong][vitricot]) {
			dem++;

			if (dem == soquancotoithieudewin) {

				return a[vitridong][vitricot];
			}
		}
		// chỉ cần phát hiện không còn tính liên tục => dừng quá trình lặp lại
		else {
			break;
		}

		// Xét qua lần lặp mới
		i++;
		j--;
	}
	return ' '; // chưa thắng
}

void NhapToaDoQuanCoCanDanhXuong(int& luotdi, char a[][Max], int sodong, int socot, int soquancotoithieudewin, int doronggiua2cot, int doronggiua2dong) {
	char quanco;
	int toadodong = -1, toadocot = -1;	// 0 - la X hoac 1 - la X hoac nguoc lai, toa do vi tri nguoi dung muon danh co xuong
	// do ban co moi dau chua co gi nen cho dai la 1 so am bat ki di :vv
	if (luotdi == 0) { quanco = 'X'; }
	else { quanco = 'O'; }
	cout << "\nLuot di hien tai la cua quan co: ";
	if (quanco == 'X') textcolor(14);
	else textcolor(12);
	cout << quanco << endl;
	textcolor(7);
	cout << "Nhap vao toa do can danh (dong,cot): ";
	do {
		cin >> toadodong >> toadocot;
		if (toadodong < 0 || toadodong > sodong - 1 || toadocot < 0 || toadocot > socot - 1) {
			cout << "Toa do de danh co khong hop le! Xin hay kiem tra lai\n";
		}
	} while (toadodong < 0 || toadodong > sodong - 1 || toadocot < 0 || toadocot > socot - 1);

	a[toadodong][toadocot] = quanco;	// nho phai luu lai quan co trong mang a de tranh bi xoa di bi mat quan co
		// kiểm tra thắng thua:

	int x = (int)log10((double)sodong) + 2, y = 2;	// vị trí ban đầu của góc trên cùng bên trái của bàn cờ, y = 2 vi phai lui xuong 1 o de ve khung tren
	gotoxy(x + doronggiua2cot / 2 + 1 + toadocot * (doronggiua2cot + 1), y + doronggiua2dong / 2 + 1 + toadodong * (doronggiua2dong + 1));
	if (quanco == 'X') textcolor(14);
	else textcolor(12);
	cout << quanco;
	textcolor(7);
	gotoxy(1, doronggiua2dong * sodong + sodong + 1 + 3);
	cout << "                                                                \n";
	cout << "\n                                                                ";
	gotoxy(1, doronggiua2dong * sodong + sodong + 1 + 3);	// phải quay lại vị trí ban đầu tránh lỗi

	if (KiemTraHangNgang(a, sodong, socot, toadodong, toadocot, soquancotoithieudewin) != ' ' || KiemTraHangDoc(a, sodong, socot, toadodong, toadocot, soquancotoithieudewin) != ' ' ||
		KiemTraCheoChinh(a, sodong, socot, toadodong, toadocot, soquancotoithieudewin) != ' ' || KiemTraCheoPhu(a, sodong, socot, toadodong, toadocot, soquancotoithieudewin) != ' ') {
		//a[toadodong][toadocot] = quanco;
		Sleep(500);
		gotoxy(1, doronggiua2dong * sodong + sodong + 1 + 2);
		cout << "                                                                \n";
		cout << "\n                                                                ";
		cout << "\nNguoi choi " << quanco << " win!";
		exit(0);
	}

	//nếu đánh xong 1 nước thì phải đổi nước khác đánh luôn, tránh trùng nhau:
	if (luotdi == 0) { luotdi = 1; }
	else { luotdi = 0; }
}
void XuLyGameTheoHuongNhapToaDoCanDanh(char a[][Max], int sodong, int socot, int doronggiua2cot, int doronggiua2dong, int soquancotoithieudewin, int luotdi) {
	int demsolandanh = 0;
	// vẽ bàn cờ caro:
	textcolor(1);
	VeBanCoCoToaDo(sodong, socot, doronggiua2cot, doronggiua2dong, a);
	cout << endl;
	textcolor(7);
	while (true) {

		NhapToaDoQuanCoCanDanhXuong(luotdi, a, sodong, socot, soquancotoithieudewin, doronggiua2cot, doronggiua2dong);
		demsolandanh++;
		//system("cls");	// xoa man hinh di tranh du thua nhung cai khong dang co :vv
		if (demsolandanh == (sodong * socot)) {
			gotoxy(1, doronggiua2dong * sodong + sodong + 1 + 2);
			cout << "                                                                \n";
			cout << "\n                                                                ";
			cout << "\nHoa nhau!";
			exit(0);
		}
	}
}
// phai co ham nay truoc:
void Undo(stack<toado>& s_undo, stack<toado>& s_redo, int& toadodong, int& toadocot, int& temp_toadodong, int& temp_toadocot, int vitrixbandau, int vitriybandau, int doronggiua2cot, int doronggiua2dong, char a[][Max], int& demsolandanh, int& luotdi, int sodong, char& quanco) {
	if (s_undo.empty() != 1) {	// neu stack khac rong
		toado td;
		td = s_undo.top();	// gan phan tu dau stack qua bien struct td
		s_undo.pop();	// loai bo no di

		s_redo.push(td);
		// cap nhat lai de ma khi danh tiep theo thi no danh tai vi tri o dau nhay do
		toadodong = td.toado_dong;
		toadocot = td.toado_cot;

		/*
		cap nhat lai bien temp = -1, -1 la chua co cai gi het de khi danh quan co moi thi no se khong chay den xoa mat vien background
		roi di xet in lai het ca ban va quan co, roi lai di in ra nhung thu lung tung
		*/
		temp_toadodong = -1;
		temp_toadocot = -1;

		// sau khi da co duoc vi tri toa do nuoc co vua danh gan nhat dc luu tru trong bien struct td thi :
		// cho dau nhay nhay toi o tuong ung toa do do ( da gan cho bien toa do dong va toadocot ).
		// va xoa phan tu di
		gotoxy(vitrixbandau + doronggiua2cot / 2 + 1 + toadocot * (doronggiua2cot + 1), vitriybandau + doronggiua2dong / 2 + 1 + toadodong * (doronggiua2dong + 1));
		textcolor(7);
		cout << " ";	// in de khoang trang de coi nhu la xoa no di
		// giam so lan dem di de tranh ao nhay qua som:
		demsolandanh--;
		// lap xong roi thi qua lai mang a, mang a dang luu thi ta xoa not di
		a[td.toado_dong][td.toado_cot] = ' ';

		// Cập nhật lại lượt đi để câu thông báo lượt đi hiện tại là của quân cờ nào và quân cờ sẽ đánh xuống tiếp tục chính là quân cờ vừa bị Undo xóa đi

		if (luotdi == 0) { luotdi = 1; }
		else { luotdi = 0; }

		gotoxy(vitrixbandau, (doronggiua2dong + 1) * sodong + vitriybandau + 1);

		//cout << "Luot di hien tai cua quan co: ";
		if (luotdi == 0) { quanco = 'X'; textcolor(9); }
		else { quanco = 'O'; textcolor(12); }
		cout << quanco;
		textcolor(7);

		/*
		neu ngan xep van con phan tu va di lay vi tri toa do cua nuo co ap cuoi vua xoa di, roi sau do nhay con tro den quan co dang sau do
		 va in lai mau len:
		(kieu la nuoc do la nuoc cuoi cung vua danh, vi nuoc cuoi cung truoc do da bi undo)
		*/
		if (s_undo.empty() != 1) {
			td = s_undo.top();
			temp_toadodong = td.toado_dong;
			temp_toadocot = td.toado_cot;

			toadodong = td.toado_dong;
			toadocot = td.toado_cot;
			gotoxy(vitrixbandau + doronggiua2cot / 2 + 1 + td.toado_cot * (doronggiua2cot + 1), vitriybandau + doronggiua2dong / 2 + 1 + td.toado_dong * (doronggiua2dong + 1));
			/* neu quan co thong bao hien tai la X thi truoc do phai la O va nguoc lai
			(co nghia la cho quan co truoc do tro thanh quan co vua danh)
			*/
			if (quanco == 'X') {
				textcolor(252);
				cout << "O";
				textcolor(7);
			}
			else {
				textcolor(251);
				cout << "X";
				textcolor(7);
			}
		}
		else {	// neu stack rong thi:
			// nhay lai con tro lai trong o tranh de con tro o dong thong bao roi in lung tung, se bi sai, loi.
			gotoxy(vitrixbandau + doronggiua2cot / 2 + 1 + toadocot * (doronggiua2cot + 1), vitriybandau + doronggiua2dong / 2 + 1 + toadodong * (doronggiua2dong + 1));
		}
	}
}

void Redo(stack<toado>& s_redo, stack<toado>& s_undo, int& toadodong, int& toadocot, char& quanco, int& luotdi, int vitrixbandau, int vitriybandau, int doronggiua2dong, int doronggiua2cot, char a[][Max], int& demsolandanh, int& temp_toadodong, int& temp_toadocot, int sodong) {
	if (s_redo.empty() != 1) {
		toado td = s_redo.top();
		s_redo.pop();
		// gan luon toa do lay ra tu ngan xep stack redo cho 2 bien toa do thi neu ta di chuyen dau nhay thi no dung ngay tai do
		toadodong = td.toado_dong;
		toadocot = td.toado_cot;
		gotoxy(vitrixbandau + doronggiua2cot / 2 + 1 + toadocot * (doronggiua2cot + 1), vitriybandau + doronggiua2dong / 2 + 1 + toadodong * (doronggiua2dong + 1));
		if (luotdi == 0) { quanco = 'X'; textcolor(228); }
		else { quanco = 'O'; textcolor(228); }
		cout << quanco;
		textcolor(7);

		a[toadodong][toadocot] = quanco;	// cap nhat lai quan co trong mang va dua vi tri quan co vua gia xuong ban co do vao so tach:

		s_undo.push(td);	// them vao struct
		demsolandanh++;

		// cho no nhay den o tuong ung voi toa do vua danh truoc do de xoa di vien background noi bat cho quan co tro lai nhu thuong
		if (temp_toadodong != -1 && temp_toadocot != -1) {
			gotoxy(vitrixbandau + doronggiua2cot / 2 + 1 + temp_toadocot * (doronggiua2cot + 1), vitriybandau + doronggiua2dong / 2 + 1 + temp_toadodong * (doronggiua2dong + 1));
			if (quanco == 'X') {	// neu nuoc di do la x thi truoc do phai la o
				textcolor(12);
				cout << "O";
			}
			else {
				textcolor(9);
				cout << "X";
			}
			textcolor(7);
		}

		// cap nhat lai bien phu luu lai toa do nuoc di cua quan co:
		temp_toadodong = toadodong;
		temp_toadocot = toadocot;

		if (luotdi == 0) { luotdi = 1; }
		else { luotdi = 0; }// phủ định của chính nó rồi gán lại cho nó để cứ luân phiên: true - false - true - false

		gotoxy(vitrixbandau, (doronggiua2dong + 1) * sodong + vitriybandau + 1);
		cout << "Luot di hien tai cua quan co: ";
		if (luotdi == 0) { quanco = 'X'; textcolor(9); }
		else { quanco = 'O'; textcolor(12); }
		cout << quanco;
		textcolor(7);
		gotoxy(vitrixbandau + doronggiua2cot / 2 + 1 + toadocot * (doronggiua2cot + 1), vitriybandau + doronggiua2dong / 2 + 1 + toadodong * (doronggiua2dong + 1));
	}
}
void XuLyGameTheoHuongDungPhim(char a[][Max], stack<toado>& s_undo, stack<toado>& s_redo, int sodong, int socot, int doronggiua2cot, int doronggiua2dong, int vitrixbandau, int vitriybandau, int soquancotoithieudewin, int luotdi, int loadmap) {
	int temp_luotdi = luotdi;	// luu cai luot di ban dau sang bien tam
	char quanco;
	// vẽ bàn cờ caro:
	textcolor(2);
	VeBanCoKhongCoToaDo(sodong, socot, doronggiua2cot, doronggiua2dong, a, vitrixbandau, vitriybandau);
	textcolor(7);

	// Trước khi đi vào vòng lặp chơi game thì mình sẽ in ra luật chơi đã chọn từ đầu ở bên hông của bàn cờ để người chơi nhìn vào mà nhớ:
	gotoxy(48, 2);
	cout << "Quan co lien tiep de chien thang: ";
	textcolor(12);
	cout << soquancotoithieudewin;
	textcolor(7);
	gotoxy(48, 3);
	cout << "Nhan cac phim "; textcolor(12); cout << "mui ten "; textcolor(7); cout << "de di chuyen";
	gotoxy(48, 4);
	cout << "Phim "; textcolor(12); cout << "Space"; textcolor(7); cout << " hoac "; textcolor(12);  cout << "nhan chuot"; textcolor(7); cout << " de danh quan co";
	gotoxy(48, 5);
	cout << "Phim"; textcolor(12); cout << " R"; textcolor(7); cout << " de", textcolor(12); cout << " Redo"; textcolor(7);
	gotoxy(48, 6);
	cout << "Phim"; textcolor(12); cout << " U"; textcolor(7); cout << " de"; textcolor(12); cout << " Undo"; textcolor(7);
	gotoxy(48, 7);
	cout << "Phim"; textcolor(12); cout << " S"; textcolor(7); cout << " de "; textcolor(12); cout << "Save"; textcolor(7);
	gotoxy(48, 8);
	cout << "Phim"; textcolor(12); cout << " Esc"; textcolor(7); cout << " de thoat";
    Hinh();

	int demsolandanh = 0;	// khoi tao so lan danh mac dinh cua quan co la 0.(neu la load map thi se la 1 so khac)
	int toadodong = 0, toadocot = 0;	// toa do mac dinh dau tien cua con tro
	int temp_toadodong = -1, temp_toadocot = -1;	// bien phu luu tru toa do cua o danh truoc do

	// phai in ra cau nay truoc:
	gotoxy(vitrixbandau, (doronggiua2dong + 1) * sodong + vitriybandau + 1);
	cout << "Luot di hien tai cua quan co: ";
	if (luotdi == 0) { quanco = 'X'; textcolor(9); }
	else { quanco = 'O'; textcolor(12); }
	cout << quanco;
	textcolor(7);
	//int temp_luotdi = luotdi;	// luu cai luot di ban dau sang bien tam
	// co nen ve lai con khong nhir?
	// di chuyen con tro den vi tri can danh quan co va kiem tra luon tinh thang thua:
	// Nếu là chế độ Load Map thì chúng ta sẽ đi in ra hết tất cả các quân cờ mà người dùng đã đánh và đã được lưu lại => Nhờ vào tính năng Redo để làm điều này (giống như chế độ Play/Pause mà chúng ta đã tận dụng trước đó)

	if (loadmap == 1) {
		while (s_redo.empty() != 1) {
			Redo(s_redo, s_undo, toadodong, toadocot, quanco, luotdi, vitrixbandau, vitriybandau, doronggiua2dong, doronggiua2cot, a, demsolandanh, temp_toadodong, temp_toadocot, sodong);
		}
	}
	//exit(0);
	gotoxy(vitrixbandau + doronggiua2cot / 2 + 1 + toadocot * (doronggiua2cot + 1), vitriybandau + doronggiua2dong / 2 + 1 + toadodong * (doronggiua2dong + 1));

	DWORD cNumRead, fdwMode, i, j;
	INPUT_RECORD irInBuf[128];
	int counter = 0;

	char cmdline[] = "GetStdHandle";
	char cmdline1[] = "Unknown ";

	hStdin = GetStdHandle(STD_INPUT_HANDLE);
	if (hStdin == INVALID_HANDLE_VALUE)
		ErrorExit(cmdline);


	if (!GetConsoleMode(hStdin, &fdwSaveOldMode))
		ErrorExit(cmdline);


	fdwMode = ENABLE_EXTENDED_FLAGS;
	if (!SetConsoleMode(hStdin, fdwMode))
		ErrorExit(cmdline);


	fdwMode = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;
	if (!SetConsoleMode(hStdin, fdwMode))
		ErrorExit(cmdline);

#ifndef MOUSE_HWHEELED
#define MOUSE_HWHEELED 0x0008
#endif

	while (true) {
		if (!ReadConsoleInput(hStdin, irInBuf, 128, &cNumRead))
			ErrorExit(cmdline);

		// nếu có sự kiện chuột tương tác vào:
		if (GetAsyncKeyState(VK_LBUTTON))
		{		// giống hàm kbhit, nếu không có cái gì tương tác vào thì tất các các thứ khác vẫn chạy bth

			for (i = 0; i < cNumRead; i++) 		// duyet so luong da duoc doc.
			{
				if (irInBuf[i].EventType == MOUSE_EVENT)
				{
					if (irInBuf[i].Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
					{
						int x = irInBuf[i].Event.MouseEvent.dwMousePosition.X + 1;
						int y = irInBuf[i].Event.MouseEvent.dwMousePosition.Y + 1;
						// đi xét tính hợp lệ của click vào quân cờ?
						if ((x >= vitrixbandau && x <= (socot * (doronggiua2cot + 1) + vitrixbandau))
							&& ((y >= vitriybandau) && y <= (sodong * (doronggiua2dong + 1) + vitriybandau))) {
							if (((x - vitrixbandau) % (doronggiua2cot + 1) != 0) && ((y - vitriybandau) % (doronggiua2dong + 1) != 0))
							{
								// vị trí x - 1 nếu không chia hết cho (doronggiua2cot + 1) thì vị trí đó thỏa mãn là nằm bên trong của ô
								// tương tự như vậy với y: nếu vị trí y - 1 không chia hết cho (doronggiua2dong + 1) thì vị trí đó thỏa mãn là nằm bên trong của ô

								// Công thức tự nhận ra và kiểm nghiệm qua các trường hợp ví dụ đều đúng và đem áp dụng
								toadocot = (x - vitrixbandau) / (doronggiua2cot + 1);
								toadodong = (y - vitriybandau) / (doronggiua2dong + 1);
								gotoxy(vitrixbandau + doronggiua2cot / 2 + 1 + toadocot * (doronggiua2cot + 1), vitriybandau + doronggiua2dong / 2 + 1 + toadodong * (doronggiua2dong + 1));
								keybd_event(VK_RETURN, 0, 0, 0);
								keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);
							}
						}
					}
				}
			}

		}
		// nếu có sự kiện bàn phím nhấn vào:
		for (j = 0; j < cNumRead; j++) 		// duyet so luong da duoc doc.
		{
			if (irInBuf[j].EventType == KEY_EVENT)
			{
				if (irInBuf[j].Event.KeyEvent.wVirtualKeyCode)
				{
					if (GetAsyncKeyState(VK_UP)) {
						if (toadodong > 0) toadodong--;
						//gotoxy(vitrixbandau + doronggiua2cot / 2 + 1 + toadocot * (doronggiua2cot + 1), vitriybandau + doronggiua2dong / 2 + 1 + toadodong * (doronggiua2dong + 1));
					}
					else if (GetAsyncKeyState(VK_DOWN)) {
						if (toadodong < sodong - 1) toadodong++;
						//gotoxy(vitrixbandau + doronggiua2cot / 2 + 1 + toadocot * (doronggiua2cot + 1), vitriybandau + doronggiua2dong / 2 + 1 + toadodong * (doronggiua2dong + 1));
					}
					else if (GetAsyncKeyState(VK_RIGHT)) {
						if (toadocot < socot - 1) toadocot++;
						//gotoxy(vitrixbandau + doronggiua2cot / 2 + 1 + toadocot * (doronggiua2cot + 1), vitriybandau + doronggiua2dong / 2 + 1 + toadodong * (doronggiua2dong + 1));
					}
					else if (GetAsyncKeyState(VK_LEFT)) {
						if (toadocot > 0) toadocot--;
					}
					gotoxy(vitrixbandau + doronggiua2cot / 2 + 1 + toadocot * (doronggiua2cot + 1), vitriybandau + doronggiua2dong / 2 + 1 + toadodong * (doronggiua2dong + 1));
				}
			}
		}
		if (GetAsyncKeyState(VK_SPACE) ||GetAsyncKeyState(VK_RETURN)) {
			if (a[toadodong][toadocot] == ' ') {
				// kiem tra vi tri chua co quan co danh vao nen co the danh duoc quan co tiep theo
				// bug: neu nguoi dung nhap quan co, roi chon redo thi chay on, nhung truoc do dung undo roi thi cac nuoc redo cu in ra,
				//nguoi dung lai di danh them nua thi loi ban co luon, roi loan het ca ban co khong biet ai X ai O
				// vay nen da danh roi thi xoa di de reset lai, cho nay k lam ki nhin vao la ngu nguoi luon.
				while (s_redo.empty() != 1) {
					s_redo.pop();
				}

				if (luotdi == 0) { quanco = 'X'; textcolor(228); }
				else { quanco = 'O'; textcolor(228); }
				cout << quanco;
				textcolor(7);

				a[toadodong][toadocot] = quanco;	// cap nhat lai quan co trong mang va dua vi tri quan co vua gia xuong ban co do vao so tach:
				toado td;
				td.toado_dong = toadodong;
				td.toado_cot = toadocot;
				s_undo.push(td);	// them vao struct
				demsolandanh++;

				// kiem tra neu so luong quan co giong nhau hang loat (nghia la cac quan co giong het nhau ay :vv)
				// Lam sao de khi ma nguoi choi nao thang thi quan co nhay mau len de nguoi ta biet la win day?
				// cho no nhay den o tuong ung voi toa do vua danh truoc do de xoa di vien background noi bat cho quan co tro lai nhu thuong
				if (temp_toadodong != -1 && temp_toadocot != -1) {
					gotoxy(vitrixbandau + doronggiua2cot / 2 + 1 + temp_toadocot * (doronggiua2cot + 1), vitriybandau + doronggiua2dong / 2 + 1 + temp_toadodong * (doronggiua2dong + 1));
					if (quanco == 'X') {	// neu nuoc di do la x thi truoc do phai la o
						textcolor(12);
						cout << "O";
					}
					else {
						textcolor(9);
						cout << "X";
					}
					textcolor(7);
				}

				// di kiem tra cac quan co giong nhau, roi tu do kiem tra tinh thang thua.
				if (KiemTraHangNgang(a, sodong, socot, toadodong, toadocot, soquancotoithieudewin) != ' ') {

					gotoxy(vitrixbandau + doronggiua2cot / 2 + 1 + toadocot * (doronggiua2cot + 1), vitriybandau + doronggiua2dong / 2 + 1 + toadodong * (doronggiua2dong + 1));
					for (int l = 1; l < 16; l++) {
						// Xét qua bên trái trước (ngang trái)
						for (int j = toadocot - 1; j >= 0; --j) {
							if (a[toadodong][j] == a[toadodong][toadocot]) {

								gotoxy(vitrixbandau + doronggiua2cot / 2 + 1 + j * (doronggiua2cot + 1), vitriybandau + doronggiua2dong / 2 + 1 + toadodong * (doronggiua2dong + 1));
								textcolor(l);
								cout << quanco;

							}
							// chỉ cần phát hiện không còn tính liên tục => dừng quá trình lặp lại
							else {
								break;
							}
						}

						// Xét qua bên phải (ngang phải)
						for (int j = toadocot + 1; j < socot; ++j) {
							if (a[toadodong][j] == a[toadodong][toadocot]) {

								gotoxy(vitrixbandau + doronggiua2cot / 2 + 1 + j * (doronggiua2cot + 1), vitriybandau + doronggiua2dong / 2 + 1 + toadodong * (doronggiua2dong + 1));
								textcolor(l);
								cout << quanco;

							}
							// chỉ cần phát hiện không còn tính liên tục => dừng quá trình lặp lại
							else {
								break;
							}
						}

						gotoxy(vitrixbandau, (doronggiua2dong + 1) * sodong + vitriybandau + 3);
						textcolor(l);	// cho to mau tao cam giac nhay nhay cac thu
						cout << "Nguoi choi " << quanco << " da thang!";
						Sleep(70);
					}

					textcolor(7);
					gotoxy(vitrixbandau, (doronggiua2dong + 1)* sodong + vitriybandau + 3);
					cout << "                                     ";
					gotoxy(vitrixbandau, (doronggiua2dong + 1)* sodong + vitriybandau + 3);
					cout << "Nguoi choi " << quanco << " da thang!";
                    Sleep(1000);
					break;
				}

				if (KiemTraHangDoc(a, sodong, socot, toadodong, toadocot, soquancotoithieudewin) != ' ') {
					gotoxy(vitrixbandau + doronggiua2cot / 2 + 1 + toadocot * (doronggiua2cot + 1), vitriybandau + doronggiua2dong / 2 + 1 + toadodong * (doronggiua2dong + 1));
					for (int l = 1; l < 14; l++) {
						// Xét lên trên trước (dọc trên)
						for (int i = toadodong - 1; i >= 0; --i) {
							if (a[i][toadocot] == a[toadodong][toadocot]) {
								gotoxy(vitrixbandau + doronggiua2cot / 2 + 1 + toadocot * (doronggiua2cot + 1), vitriybandau + doronggiua2dong / 2 + 1 + i * (doronggiua2dong + 1));
								textcolor(l);
								cout << quanco;
							}
							else {	// chỉ cần phát hiện không còn tính liên tục => dừng quá trình lặp lại
								break;
							}
						}
						// Xét xuống dưới (dọc dưới)
						for (int i = toadodong + 1; i < sodong; ++i) {
							if (a[i][toadocot] == a[toadodong][toadocot]) {
								gotoxy(vitrixbandau + doronggiua2cot / 2 + 1 + toadocot * (doronggiua2cot + 1), vitriybandau + doronggiua2dong / 2 + 1 + i * (doronggiua2dong + 1));
								textcolor(l);
								cout << quanco;
							}
							// chỉ cần phát hiện không còn tính liên tục => dừng quá trình lặp lại
							else {
								break;
							}
						}
						gotoxy(vitrixbandau, (doronggiua2dong + 1) * sodong + vitriybandau + 3);
						textcolor(l);	// cho to mau tao cam giac nhay nhay cac thu
						cout << "Nguoi choi " << quanco << " da thang!";
						Sleep(70);
					}
					textcolor(7);
					gotoxy(vitrixbandau, (doronggiua2dong + 1) * sodong + vitriybandau + 3);
					cout << "                                     ";
					gotoxy(vitrixbandau, (doronggiua2dong + 1) * sodong + vitriybandau + 3);
					cout << "Nguoi choi " << quanco << " da thang!";
					Sleep(1000);
					break;
				}

				if (KiemTraCheoChinh(a, sodong, socot, toadodong, toadocot, soquancotoithieudewin) != ' ') {
					for (int k = 1; k < 16; k++) {
						// Kiểm tra chéo chính trên: Các phần tử dòng - cột luôn giảm 1 đơn vị và giảm đến khi nào 1 trong 2 thằng xuất hiện 0
						int i = toadodong - 1;
						int j = toadocot - 1;
						while (true) {
							if (i < 0 || j < 0) {
								break; // vị trí xét không hợp lệ
							}

							// Nếu còn chạy xuống đây được thì có nghĩa vị trí i, j hợp lệ
							if (a[i][j] == a[toadodong][toadocot]) {
								gotoxy(vitrixbandau + doronggiua2cot / 2 + 1 + j * (doronggiua2cot + 1), vitriybandau + doronggiua2dong / 2 + 1 + i * (doronggiua2dong + 1));
								textcolor(k);
								cout << quanco;

							}
							// chỉ cần phát hiện không còn tính liên tục => dừng quá trình lặp lại
							else {

								break;
							}

							// Xét qua lần lặp mới
							i--;
							j--;
						}

						//	Kiểm tra chéo chính dưới: Các phần tử dòng - cột đều tăng lên 1 đơn vị và tăng đến khi nào 1 trong 2 thằng bằng n - 1 thì dừng lại
						i = toadodong + 1;
						j = toadocot + 1;
						while (true) {
							if (i == sodong || j == socot) {
								break; // vị trí xét không hợp lệ
							}

							// Nếu còn chạy xuống đây được thì có nghĩa vị trí i, j hợp lệ
							if (a[i][j] == a[toadodong][toadocot]) {
								gotoxy(vitrixbandau + doronggiua2cot / 2 + 1 + j * (doronggiua2cot + 1), vitriybandau + doronggiua2dong / 2 + 1 + i * (doronggiua2dong + 1));
								textcolor(k);
								cout << quanco;
							}
							// chỉ cần phát hiện không còn tính liên tục => dừng quá trình lặp lại
							else {
								break;
							}

							// Xét qua lần lặp mới
							i++;
							j++;
						}
						gotoxy(vitrixbandau, (doronggiua2dong + 1) * sodong + vitriybandau + 3);
						textcolor(k);	// cho to mau tao cam giac nhay nhay cac thu
						cout << "Nguoi choi " << quanco << " da thang!";
						Sleep(70);
					}
					textcolor(7);
					gotoxy(vitrixbandau, (doronggiua2dong + 1)* sodong + vitriybandau + 3);
					cout << "                                     ";
					gotoxy(vitrixbandau, (doronggiua2dong + 1)* sodong + vitriybandau + 3);
					cout << "Nguoi choi " << quanco << " da thang!";
					Sleep(1000);
					break;
				}
				if (KiemTraCheoPhu(a, sodong, socot, toadodong, toadocot, soquancotoithieudewin) != ' ') {
					for (int l = 1; l < 16; l++) {
						// Kiểm tra chéo phụ trên: Dòng giảm - cột tăng. Dòng giảm tối đa tới 0, Cột tăng tối đa nới n - 1
						int i = toadodong - 1;
						int j = toadocot + 1;
						while (true) {
							if (i < 0 || j == socot) {
								break; // vị trí xét không hợp lệ
							}

							// Nếu còn chạy xuống đây được thì có nghĩa vị trí i, j hợp lệ
							if (a[i][j] == a[toadodong][toadocot]) {
								gotoxy(vitrixbandau + doronggiua2cot / 2 + 1 + j * (doronggiua2cot + 1), vitriybandau + doronggiua2dong / 2 + 1 + i * (doronggiua2dong + 1));
								textcolor(l);
								cout << quanco;
							}
							// chỉ cần phát hiện không còn tính liên tục => dừng quá trình lặp lại
							else {
								break;
							}

							// Xét qua lần lặp mới
							i--;
							j++;
						}

						//	Kiểm tra chéo phụ dưới: Dòng tăng - cột giảm. Dòng tăng tới tối đa là n - 1 và cột giảm tới tối đa là 0
						i = toadodong + 1;
						j = toadocot - 1;
						while (true) {
							if (i == sodong || j < 0) {
								break; // vị trí xét không hợp lệ
							}

							// Nếu còn chạy xuống đây được thì có nghĩa vị trí i, j hợp lệ
							if (a[i][j] == a[toadodong][toadocot]) {
								gotoxy(vitrixbandau + doronggiua2cot / 2 + 1 + j * (doronggiua2cot + 1), vitriybandau + doronggiua2dong / 2 + 1 + i * (doronggiua2dong + 1));
								textcolor(l);
								cout << quanco;
							}
							// chỉ cần phát hiện không còn tính liên tục => dừng quá trình lặp lại
							else {
								break;
							}

							// Xét qua lần lặp mới
							i++;
							j--;
						}

						gotoxy(vitrixbandau, (doronggiua2dong + 1) * sodong + vitriybandau + 3);
						textcolor(l);	// cho to mau tao cam giac nhay nhay cac thu
						cout << "Nguoi choi " << quanco << " da thang!";
						Sleep(70);
					}
					textcolor(7);
					gotoxy(vitrixbandau, (doronggiua2dong + 1)* sodong + vitriybandau + 3);
					cout << "                                     ";
					gotoxy(vitrixbandau, (doronggiua2dong + 1)* sodong + vitriybandau + 3);
					cout << "Nguoi choi " << quanco << " da thang!";
					Sleep(1000);
					break;
				}

				// neu ma khong co thang ham nao thang:
				if (demsolandanh == (sodong * socot)) {
					for (int i = 1; i < 16; i++) {
						gotoxy(vitrixbandau, (doronggiua2dong + 1) * sodong + vitriybandau + 3);
						textcolor(i);	// cho to mau tao cam giac nhay nhay cac thu
						Sleep(70);
						cout << "Hoa nhau!";
					}
					Sleep(1000);
					textcolor(7);	// phai tra lai mau goc cho no.
					break;
				}
				// cap nhat lai bien phu luu lai toa do nuoc di cua quan co:
				temp_toadodong = toadodong;
				temp_toadocot = toadocot;

				if (luotdi == 0) { luotdi = 1; }
				else { luotdi = 0; }

				gotoxy(vitrixbandau, (doronggiua2dong + 1) * sodong + vitriybandau + 1);

				//gotoxy(1, doronggiua2dong * sodong + sodong + 1 + 2);	//cong thuc nay van dung trong 1 so truong hop, nhung sai hau het vs so luong kich co khac nhau
				cout << "Luot di hien tai cua quan co: ";
				if (luotdi == 0) { quanco = 'X'; textcolor(9); }
				else { quanco = 'O'; textcolor(12); }
				cout << quanco;
				textcolor(7);

				gotoxy(vitrixbandau + doronggiua2cot / 2 + 1 + toadocot * (doronggiua2cot + 1), vitriybandau + doronggiua2dong / 2 + 1 + toadodong * (doronggiua2dong + 1));

			}
			else {
				// nguoc lai neu da co quan co truoc do danh roi thi:
				for (int i = 1; i < 16; i++) {
					gotoxy(vitrixbandau, (doronggiua2dong + 1) * sodong + vitriybandau + 2);
					textcolor(i);	// cho to mau tao cam giac nhay nhay cac thu
					cout << "Da co quan co tai vi tri nay!";
					Sleep(10);
				}
				gotoxy(vitrixbandau, (doronggiua2dong + 1) * sodong + vitriybandau + 2);
				cout << "                                  ";
				gotoxy(vitrixbandau + doronggiua2cot / 2 + 1 + toadocot * (doronggiua2cot + 1), vitriybandau + doronggiua2dong / 2 + 1 + toadodong * (doronggiua2dong + 1));
				textcolor(7);	// phai tra lai mau goc cho no.
			}
		}

		//lam chuc nang undo - cho phep nguoi choi lui lai 1 nuoc de danh lai quan co: bam phim u
		else if (GetAsyncKeyState(0x55)) {
			Undo(s_undo, s_redo, toadodong, toadocot, temp_toadodong, temp_toadocot, vitrixbandau, vitriybandau, doronggiua2cot, doronggiua2dong, a, demsolandanh, luotdi, sodong, quanco);
		}

		// lam chuc nang Redo - cho nguoi choi tien len 1 nuoc đe danh tiep nuoc khac (neu can): bam phim r
		else if (GetAsyncKeyState(0x52)) {
			Redo(s_redo, s_undo, toadodong, toadocot, quanco, luotdi, vitrixbandau, vitriybandau, doronggiua2dong, doronggiua2cot, a, demsolandanh, temp_toadodong, temp_toadocot, sodong);
		}
		else if (GetAsyncKeyState(0x53)) {	// luu lai ban co: bam phim s
			// mac dinh ta se luu o dau? va lam sao de in ra duoc khi ma da save roi?
			// ta se luu cai ten file khi save lai theo la: <ngay_thang_nam><gio_phut_giay>.txt

			ofstream fileout1;
			fileout1.open("cacmapcu.txt", ios_base::out);

			//ifstream filename1;
			//filename1.open("cacmapcu.txt", ios_base::in);

			time_t t = time(0);
			struct tm* Now = localtime(&t);
			string FileName;	// tao ten file de luu lai
			FileName = to_string(Now->tm_mday) + "_" + to_string(Now->tm_mon + 1) + "_" + to_string(Now->tm_year + 1900) + "_" + to_string(Now->tm_hour) + "_" + to_string(Now->tm_min) + "_" + to_string(Now->tm_sec);
			ofstream FileOut;

			fileout1 << FileName;
			FileOut.open(FileName + ".txt", ios_base::out);
			FileOut << "sodong " << sodong << endl;
			FileOut << "socot " << socot << endl;
			FileOut << "doronggiua2cot " << doronggiua2cot << endl;
			FileOut << "doronggiua2dong " << doronggiua2dong << endl;
			FileOut << "vitrixbandau " << vitrixbandau << endl;
			FileOut << "vitriybandau " << vitriybandau << endl;
			FileOut << "s_undo " << endl;
			FileOut << s_undo.size() << endl;
			// vi stack undo no co anh huong den toan bai nen ta khong the dem no di truc tiep ma di xet duoc, ta se phai dung 1 bien phu luu lai roi di xet bien phu do thay cho bien stack chinh, bao dam cho bien stack chinh luon duoc bao toan
			stack<toado> temp_s_undo = s_undo;
			while (temp_s_undo.empty() != 1) {
				toado td = temp_s_undo.top();
				temp_s_undo.pop();

				FileOut << td.toado_dong << " " << td.toado_cot << endl;
			}

			FileOut << "soquancotoithieudewin " << soquancotoithieudewin << endl;
			FileOut << "temp_luotdi " << temp_luotdi;

			FileOut.close();
			fileout1.close();
			gotoxy(50, 9);
			cout << "Da luu!";
		}
		else if (GetAsyncKeyState(0x1B)) { // Esc
            system("cls");
            HINHNEN();
            XuLyVaTaoNut();
		}
	}
	SetConsoleMode(hStdin, fdwSaveOldMode);
}
void XuLyLanCuoi (stack<toado>& s_undo, stack<toado>& s_redo,int loadmap)
{

    if(sodong > 10 || socot > 10)
    {
        sodong = 10;
        socot = 10;
    }
    else if(sodong < 3 || socot < 3)
    {
        sodong = 3;
        socot = 3;
    }

    // khoi tao tat ca cac phan tu trong mang phai la ki tu khang trang de di xet o ben tren, neu khong se ro vao truong hop NULL, loi se sai:
	fill((char*)a, (char*)a + sizeof(a) / sizeof(char), ' ');

	while (!s_undo.empty()) {
		s_undo.pop();
	}

	while (!s_redo.empty()) {
		s_redo.pop();
	}

	ifstream FileIn, openfilecu;
    string docvb;
	if (loadmap == 1)
    {
        openfilecu.open("cacmapcu.txt", ios_base::in);

        openfilecu >> docvb;

        FileIn.open(docvb + ".txt", ios_base::in);
		if (!FileIn)
		{
			cout << "\n\n\n\t\tMap cu khong ton tai!\n\t\tTu dong chuyen den map moi khac trong ";
			for(int i = 3;i>= 1;i--)
            {
                gotoxy(35,7); cout << i << "s";
                Sleep(1000);
            }
			loadmap = 0;
		}
		system("cls");
		Sleep(100);
		// neu doc duoc thi ta tien hanh doc xuat ra ban co cu luon:
		string docvao;
		FileIn >> docvao;
		FileIn >> sodong;

		FileIn >> docvao;
		FileIn >> socot;

		FileIn >> docvao;
		FileIn >> doronggiua2cot;

		FileIn >> docvao;
		FileIn >> doronggiua2dong;

		FileIn >> docvao;
		FileIn >> vitrixbandau;

		FileIn >> docvao;
		FileIn >> vitriybandau;

		FileIn >> docvao;
		int size;
		FileIn >> size;

		for (int i = 1; i <= size; i++)
        {
			toado td;
			FileIn >> td.toado_dong >> td.toado_cot;
			s_redo.push(td);
		}
		FileIn >> docvao;
		FileIn >> soquancotoithieudewin;

		FileIn >> docvao;
		FileIn >> luotdibandau;

		FileIn.close();
		openfilecu.close();
	}
	XuLyGameTheoHuongDungPhim(a, s_undo, s_redo, sodong, socot, doronggiua2cot, doronggiua2dong, vitrixbandau, vitriybandau, soquancotoithieudewin, luotdibandau, loadmap);
    system("cls");
    HINHNEN();
    XuLyVaTaoNut();
}
void TaoNutNhanChucNang(NUTNHAN nut)
{
	int length = nut.ChucNang.length();
	gotoxy(nut.x, nut.y);
	textcolor(nut.MaMau);
	cout << nut.ChucNang;
	textcolor(7); // Trở lại màu bình thường
}

// Hàm này nhận 3 tham số x, y là tọa độ của chuột và khoảng cách giữa 2 menu rồi kiểm tra nếu đang ở trong phạm vi của 1 Menu nào đó thì trả về index của Menu đó, nếu không ở trong phạm vi của Menu nào cả thì trả về -1
int KiemTraToaDoChuotHopLe(int x, int y, int khoangcachgiua2menu)
{
	if(x < DanhSachNutNhan[0].x || x > DanhSachNutNhan[0].x + DanhSachNutNhan[0].DoRong - 1)
		return -1;
	else // x hợp lệ
	{
		if(y < DanhSachNutNhan[0].y || y > DanhSachNutNhan[0].y + (SoLuongCacNutNhan - 1) * (khoangcachgiua2menu + 1))
			return -1;
		else // y hợp lệ nhưng coi chừng nó nằm vào những vị trí khoảng trắng ngăn cách giữa 2 nút thì cũng không phải hợp lệ
		{
			if((y - DanhSachNutNhan[0].y) % (khoangcachgiua2menu + 1) != 0)
				return -1;
		}
	}
	return (y - DanhSachNutNhan[0].y) / (khoangcachgiua2menu + 1); // trả về index của nút đang đứng
}

// Hàm này nhận 3 tham số x, y là tọa độ của chuột và khoảng cách giữa 2 menu rồi kiểm tra nếu đang ở trong phạm vi của 1 Menu nào đó thì trả về index của Menu đó, nếu không ở trong phạm vi của Menu nào cả thì trả về -1
// Version 2 này khác trước đó: Lúc này nó sẽ không quy chuẩn tất cả các Menu có cùng bề ngang giống nhau, mà tùy Menu sẽ có bề ngang khác nhau (nó phụ thuộc vào độ dài chức năng của Menu tương ứng) => buộc người điều khiển phải đưa hẳn con chuột vào bên trong phạm vi nội dung của nút
// Hoặc là nếu áp dụng độ rộng của tất cả các Menu quy chuẩn theo 1 độ rộng lớn nhất thì hàm này vẫn đáp ứng chạy ngon lành cành đào
int KiemTraToaDoChuotHopLe_Ver2(int x, int y, int khoangcachgiua2menu)
{

	// Xét tính hợp lệ của y
	if(y < DanhSachNutNhan[0].y || y > DanhSachNutNhan[0].y + (SoLuongCacNutNhan - 1) * (khoangcachgiua2menu + 1))
		return -1;

	// Chạy được xuống dòng này tức là điều kiện if ở trên không thỏa tức là y hợp lệ nhưng coi chừng nó nằm vào những vị trí khoảng trắng ngăn cách giữa 2 nút thì cũng không phải hợp lệ
	if((y - DanhSachNutNhan[0].y) % (khoangcachgiua2menu + 1) != 0)
		return -1;

	// Lúc này chạy xuống đây được tức là y đã là y hợp lệ hoàn toàn và thuộc về y của 1 Menu cụ thể. Lúc này ta sẽ xét xem vị trí x có nằm trong đoạn của nút nhấn đó hay không?
	int index_Menu = (y - DanhSachNutNhan[0].y) / (khoangcachgiua2menu + 1);

	if(x < DanhSachNutNhan[index_Menu].x || x > DanhSachNutNhan[index_Menu].x + DanhSachNutNhan[index_Menu].DoRong - 1)
		return -1;

	return index_Menu; // trả về index của nút đang đứng và nó đang nằm trong phạm vi nội dung của nút đó và hợp lệ
}

VOID MouseEventProc(MOUSE_EVENT_RECORD mer, int khoangcachgiua2menu)
{

#ifndef MOUSE_HWHEELED
#define MOUSE_HWHEELED 0x0008
#endif
	stack<toado> s_undo;	// khai báo, khởi tạo stack cho undo (sẽ có thay đổi nên mình phải có tham chiếu).
	stack<toado> s_redo;	// khai báo, khởi tạo stack cho undo (sẽ có thay đổi nên mình phải có tham chiếu).

	switch(mer.dwEventFlags)
	{
	case 0:
		if(mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
		{
			int x = mer.dwMousePosition.X + 1;
			int y = mer.dwMousePosition.Y + 1;

			int index_Menu = KiemTraToaDoChuotHopLe_Ver2(x, y, khoangcachgiua2menu);

			if(index_Menu != -1)
			{
				// Tô màu đỏ cho nó
				DanhSachNutNhan[indexCuaMenuDangDuocChon].MaMau = 12; // màu đỏ
				TaoNutNhanChucNang(DanhSachNutNhan[indexCuaMenuDangDuocChon]);
				if(indexCuaMenuDangDuocChon == 0)
                {
                    system("cls");
                    XuLyLanCuoi(s_undo,s_redo,0);

                }
                else if(indexCuaMenuDangDuocChon == 1)
                {
                    system("cls");
                    XuLyLanCuoi(s_undo,s_redo,1);
                }
                else if(indexCuaMenuDangDuocChon == 2)
                {
                    system("cls");
                    XuLyVaTaoNut2();
                    //exit(0);
                }
                else if(indexCuaMenuDangDuocChon == 3)
                {
                    system("cls");
                    exit(0);
                }
			}
			else
			{

			}

		}
		else // Xảy ra sự kiện này khi thả chuột ra (Trước đó nhấn vào)
		{
			DanhSachNutNhan[indexCuaMenuDangDuocChon].MaMau = 14; // màu vàng
			TaoNutNhanChucNang(DanhSachNutNhan[indexCuaMenuDangDuocChon]);
		}
		break;
	case MOUSE_MOVED:
		int x = mer.dwMousePosition.X + 1;
		int y = mer.dwMousePosition.Y + 1;

		int index_Menu = KiemTraToaDoChuotHopLe_Ver2(x, y, khoangcachgiua2menu);

		if(index_Menu != -1) // chuột di chuyển vào trong phạm vi của 1 nút nào đó
		{

			//int idx = (y - DanhSachNutNhan[0].y) / 2;

			if(index_Menu != indexCuaMenuDangDuocChon) // Tức là chuột di chuyển vào 1 Menu mới khác Menu đang được chọn ban đầu
			{
				// Xóa màu vàng ở Menu cũ
				DanhSachNutNhan[indexCuaMenuDangDuocChon].MaMau = 15; // trở lại thành màu trắng
				TaoNutNhanChucNang(DanhSachNutNhan[indexCuaMenuDangDuocChon]); // Vẽ lại nó với màu trắng

				// Tô màu vàng ở Menu mới
				indexCuaMenuDangDuocChon = index_Menu; // Cập nhật index của Menu đang được di chuyển chuột vào sang biến index đang xử lý
				DanhSachNutNhan[indexCuaMenuDangDuocChon].MaMau = 14; // Cập nhật mã màu vàng cho nó
				TaoNutNhanChucNang(DanhSachNutNhan[indexCuaMenuDangDuocChon]); // Vẽ lại nó với màu vàng

			}
		}
		break;
	}
}

VOID MouseEventProc2(MOUSE_EVENT_RECORD mer, int khoangcachgiua2menu)
{

#ifndef MOUSE_HWHEELED
#define MOUSE_HWHEELED 0x0008
#endif
	stack<toado> s_undo;	// khai báo, khởi tạo stack cho undo (sẽ có thay đổi nên mình phải có tham chiếu).
	stack<toado> s_redo;	// khai báo, khởi tạo stack cho undo (sẽ có thay đổi nên mình phải có tham chiếu).

	switch(mer.dwEventFlags)
	{
	case 0:
		if(mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
		{
			int x = mer.dwMousePosition.X + 1;
			int y = mer.dwMousePosition.Y + 1;

			int index_Menu = KiemTraToaDoChuotHopLe_Ver2(x, y, khoangcachgiua2menu);

			if(index_Menu != -1)
			{
				// Tô màu đỏ cho nó
				DanhSachNutNhan[indexCuaMenuDangDuocChon].MaMau = 12; // màu đỏ
				TaoNutNhanChucNang(DanhSachNutNhan[indexCuaMenuDangDuocChon]);
				if(indexCuaMenuDangDuocChon == 0)
                {
                    sodong = 3;
                    socot = 3;
                    soquancotoithieudewin = 3;
                    system("cls");
                    XuLyLanCuoi(s_undo,s_redo,0);

                }
                else if(indexCuaMenuDangDuocChon == 1)
                {
                    sodong = 5;
                    socot = 5;
                    soquancotoithieudewin = 5;
                    system("cls");
                    XuLyLanCuoi(s_undo,s_redo,0);
                }
                else if(indexCuaMenuDangDuocChon == 2)
                {
                    sodong = 10;
                    socot = 10;
                    soquancotoithieudewin = 5;
                    system("cls");
                    XuLyLanCuoi(s_undo,s_redo,0);
                }
                else if(indexCuaMenuDangDuocChon == 3)
                {
                    system("cls");
                    XuLyVaTaoNut();
                }
			}
			else
			{

			}

		}
		else // Xảy ra sự kiện này khi thả chuột ra (Trước đó nhấn vào)
		{
			DanhSachNutNhan[indexCuaMenuDangDuocChon].MaMau = 14; // màu vàng
			TaoNutNhanChucNang(DanhSachNutNhan[indexCuaMenuDangDuocChon]);
		}
		break;
	case MOUSE_MOVED:
		int x = mer.dwMousePosition.X + 1;
		int y = mer.dwMousePosition.Y + 1;

		int index_Menu = KiemTraToaDoChuotHopLe_Ver2(x, y, khoangcachgiua2menu);

		if(index_Menu != -1) // chuột di chuyển vào trong phạm vi của 1 nút nào đó
		{

			//int idx = (y - DanhSachNutNhan[0].y) / 2;

			if(index_Menu != indexCuaMenuDangDuocChon) // Tức là chuột di chuyển vào 1 Menu mới khác Menu đang được chọn ban đầu
			{
				// Xóa màu vàng ở Menu cũ
				DanhSachNutNhan[indexCuaMenuDangDuocChon].MaMau = 15; // trở lại thành màu trắng
				TaoNutNhanChucNang(DanhSachNutNhan[indexCuaMenuDangDuocChon]); // Vẽ lại nó với màu trắng

				// Tô màu vàng ở Menu mới
				indexCuaMenuDangDuocChon = index_Menu; // Cập nhật index của Menu đang được di chuyển chuột vào sang biến index đang xử lý
				DanhSachNutNhan[indexCuaMenuDangDuocChon].MaMau = 14; // Cập nhật mã màu vàng cho nó
				TaoNutNhanChucNang(DanhSachNutNhan[indexCuaMenuDangDuocChon]); // Vẽ lại nó với màu vàng

			}
		}
		break;
	}
}

void XuLyVaTaoNut()
{
	vector<string> DanhSachTenCacNutNhan;
	DanhSachTenCacNutNhan.resize(SoLuongCacNutNhan);
	DanhSachTenCacNutNhan[0] = "Tro Choi Moi";
	DanhSachTenCacNutNhan[1] = "Tiep Tuc";
	DanhSachTenCacNutNhan[2] = "Lua Chon";
	DanhSachTenCacNutNhan[3] = "Thoat";
     HINHNEN();
    // Tìm tên nút chức năng có độ dài lớn nhất
	int DoDaiLonNhat = DanhSachTenCacNutNhan[0].length();

	for(int i = 1; i < SoLuongCacNutNhan; ++i)
	{
		int length = DanhSachTenCacNutNhan[i].length();

		if(length > DoDaiLonNhat)
			DoDaiLonNhat = length;
	}
	DoDaiLonNhat += 2; // để trước và sau nó có 1 khoảng trắng cách lề biên trái phải ra

	// Vị trí xuất hiện ban đầu của nút nhấn đầu tiên
	int vitriXbandau = 40;
	int vitriYbandau = 15;
	int KhoangCachGiua2Menu = 1;

	DanhSachNutNhan[0].ChucNang = DanhSachTenCacNutNhan[0];
	DanhSachNutNhan[0].MaMau = 14; // chữ màu vàng, nền màu đen

	// Tùy ý người dùng muốn quy chuẩn tất cả các nút về 1 độ rộng giống nhau hay mỗi nút sẽ có độ rộng tùy biến theo độ dài nội dung chức năng của nút đó
	//DanhSachNutNhan[0].DoRong = DoDaiLonNhat;
	DanhSachNutNhan[0].DoRong = DanhSachNutNhan[0].ChucNang.length();

	DanhSachNutNhan[0].x = vitriXbandau;
	DanhSachNutNhan[0].y = vitriYbandau;
	TaoNutNhanChucNang(DanhSachNutNhan[0]);

	for(int i = 1; i < SoLuongCacNutNhan; ++i)
	{
		DanhSachNutNhan[i].ChucNang = DanhSachTenCacNutNhan[i];
		DanhSachNutNhan[i].MaMau = 15; // chữ màu vàng, nền màu đen

		// Tùy ý người dùng muốn quy chuẩn tất cả các nút về 1 độ rộng giống nhau hay mỗi nút sẽ có độ rộng tùy biến theo độ dài nội dung chức năng của nút đó
		//DanhSachNutNhan[i].DoRong = DoDaiLonNhat;
		DanhSachNutNhan[i].DoRong = DanhSachNutNhan[i].ChucNang.length();

		DanhSachNutNhan[i].x = vitriXbandau;
		DanhSachNutNhan[i].y = DanhSachNutNhan[i - 1].y + KhoangCachGiua2Menu + 1; // cộng 1 chính là vị trí của nút tiếp theo

		TaoNutNhanChucNang(DanhSachNutNhan[i]);
	}

	DWORD cNumRead, fdwMode, i;
	INPUT_RECORD irInBuf[128];

	hStdin = GetStdHandle(STD_INPUT_HANDLE);
	if (hStdin == INVALID_HANDLE_VALUE)
		ErrorExit("GetStdHandle");

	if (! GetConsoleMode(hStdin, &fdwSaveOldMode) )
		ErrorExit("GetConsoleMode");

	// Tự động tắt chế độ Quick Edit Mode nếu đang được bật để từ đó có thể bắt sự kiện chuột được
	fdwMode = ENABLE_EXTENDED_FLAGS;
	if (! SetConsoleMode(hStdin, fdwMode) )
		ErrorExit("SetConsoleMode");

	fdwMode = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;
	if (! SetConsoleMode(hStdin, fdwMode) )
		ErrorExit("SetConsoleMode");

	while(true)
	{
		if (! ReadConsoleInput(hStdin, irInBuf, 128, &cNumRead) )
			ErrorExit("ReadConsoleInput");

		for (i = 0; i < cNumRead; i++)
		{
			switch(irInBuf[i].EventType)
			{
			case MOUSE_EVENT: // mouse input
				MouseEventProc(irInBuf[i].Event.MouseEvent, KhoangCachGiua2Menu);
				break;
			}
		}
	}
	SetConsoleMode(hStdin, fdwSaveOldMode);
}
void XuLyVaTaoNut2()
{
	vector<string> DanhSachTenCacNutNhan;
	DanhSachTenCacNutNhan.resize(SoLuongCacNutNhan);
	DanhSachTenCacNutNhan[0] = "3x3";
	DanhSachTenCacNutNhan[1] = "5x5";
	DanhSachTenCacNutNhan[2] = "10x10";
	DanhSachTenCacNutNhan[3] = "Quay lai";
     HINHNEN();
    // Tìm tên nút chức năng có độ dài lớn nhất
	int DoDaiLonNhat = DanhSachTenCacNutNhan[0].length();

	for(int i = 1; i < SoLuongCacNutNhan; ++i)
	{
		int length = DanhSachTenCacNutNhan[i].length();

		if(length > DoDaiLonNhat)
			DoDaiLonNhat = length;
	}
	DoDaiLonNhat += 2; // để trước và sau nó có 1 khoảng trắng cách lề biên trái phải ra

	// Vị trí xuất hiện ban đầu của nút nhấn đầu tiên
	int vitriXbandau = 40;
	int vitriYbandau = 15;
	int KhoangCachGiua2Menu = 1;

	DanhSachNutNhan[0].ChucNang = DanhSachTenCacNutNhan[0];
	DanhSachNutNhan[0].MaMau = 14; // chữ màu vàng, nền màu đen

	// Tùy ý người dùng muốn quy chuẩn tất cả các nút về 1 độ rộng giống nhau hay mỗi nút sẽ có độ rộng tùy biến theo độ dài nội dung chức năng của nút đó
	//DanhSachNutNhan[0].DoRong = DoDaiLonNhat;
	DanhSachNutNhan[0].DoRong = DanhSachNutNhan[0].ChucNang.length();

	DanhSachNutNhan[0].x = vitriXbandau;
	DanhSachNutNhan[0].y = vitriYbandau;
	TaoNutNhanChucNang(DanhSachNutNhan[0]);

	for(int i = 1; i < SoLuongCacNutNhan; ++i)
	{
		DanhSachNutNhan[i].ChucNang = DanhSachTenCacNutNhan[i];
		DanhSachNutNhan[i].MaMau = 15; // chữ màu vàng, nền màu đen

		// Tùy ý người dùng muốn quy chuẩn tất cả các nút về 1 độ rộng giống nhau hay mỗi nút sẽ có độ rộng tùy biến theo độ dài nội dung chức năng của nút đó
		//DanhSachNutNhan[i].DoRong = DoDaiLonNhat;
		DanhSachNutNhan[i].DoRong = DanhSachNutNhan[i].ChucNang.length();

		DanhSachNutNhan[i].x = vitriXbandau;
		DanhSachNutNhan[i].y = DanhSachNutNhan[i - 1].y + KhoangCachGiua2Menu + 1; // cộng 1 chính là vị trí của nút tiếp theo

		TaoNutNhanChucNang(DanhSachNutNhan[i]);
	}

	DWORD cNumRead, fdwMode, i;
	INPUT_RECORD irInBuf[128];

	hStdin = GetStdHandle(STD_INPUT_HANDLE);
	if (hStdin == INVALID_HANDLE_VALUE)
		ErrorExit("GetStdHandle");

	if (! GetConsoleMode(hStdin, &fdwSaveOldMode) )
		ErrorExit("GetConsoleMode");

	// Tự động tắt chế độ Quick Edit Mode nếu đang được bật để từ đó có thể bắt sự kiện chuột được
	fdwMode = ENABLE_EXTENDED_FLAGS;
	if (! SetConsoleMode(hStdin, fdwMode) )
		ErrorExit("SetConsoleMode");

	fdwMode = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;
	if (! SetConsoleMode(hStdin, fdwMode) )
		ErrorExit("SetConsoleMode");

	while(true)
	{
		if (! ReadConsoleInput(hStdin, irInBuf, 128, &cNumRead) )
			ErrorExit("ReadConsoleInput");

		for (i = 0; i < cNumRead; i++)
		{
			switch(irInBuf[i].EventType)
			{
			case MOUSE_EVENT: // mouse input
				MouseEventProc2(irInBuf[i].Event.MouseEvent, KhoangCachGiua2Menu);
				break;
			}
		}
	}
	SetConsoleMode(hStdin, fdwSaveOldMode);
}
int main()
{
    while(1)
    {

        XuLyVaTaoNut();
    }
    return 0;
}
