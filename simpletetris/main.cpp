#include <iostream> // stdioh んл
#include <Windows.h>
#include <conio.h>
using namespace std;

void gotoxy(int x, int y) {
	COORD Pos;
	Pos.X = x;
	Pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

int gamefield[20][10] = { 
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0}
};
int inputfield[20][10] = {0};
int ghostfield[20][10] = {0};
int nowMinoIndex;
int shaftx;
int shafty;
int mino[7][4][4] = {
	{
		{0,0,0,0},
		{1,1,1,1},
		{0,0,0,0},
		{0,0,0,0}
	},
	{
		{0,0,0,0},
		{0,1,1,0},
		{0,1,1,0},
		{0,0,0,0}
	},
	{
		{0,0,1,0},
		{1,1,1,0},
		{0,0,0,0},
		{0,0,0,0}
	},
	{
		{1,0,0,0},
		{1,1,1,0},
		{0,0,0,0},
		{0,0,0,0}
	},
	{
		{0,1,1,0},
		{1,1,0,0},
		{0,0,0,0},
		{0,0,0,0}
	},
	{
		{1,1,0,0},
		{0,1,1,0},
		{0,0,0,0},
		{0,0,0,0}
	},
	{
		{0,1,0,0},
		{1,1,1,0},
		{0,0,0,0},
		{0,0,0,0}
	}
};

void printfield(int next[]) {
	cout << "旨收收收收收收收收收收收收收收收收收收收收旬" << endl;
	for (int i = 0; i < 20; i++)
	{
		cout << "早";
		for (int j = 0; j < 10; j++) {
			
			if (gamefield[i][j] == 1 || inputfield[i][j] == 1) {
				cout << "〡";
			}
			else if (ghostfield[i][j] == 1) {
				cout << "﹤";
			}
			else {
				cout << "  ";
			}
		}
		cout << "早";
		cout << "  ";
		if(i<4){
			for (int k = 0; k < 4; k++) {
				if (mino[next[0]][i][k] == 1) {
					cout << "〡";
				}
				else {
					cout << "  ";
				}
			}
		}
		cout << endl;
	}
	cout << "曲收收收收收收收收收收收收收收收收收收收收旭" << endl;
}

void spawn(int minoindex) {
	int (*nowmino)[4] = mino[minoindex];
	nowMinoIndex = minoindex;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++) {
			inputfield[i][j + 3] = nowmino[i][j];
		}
	}
	shafty = 0;
	shaftx = 3;
}

bool inputLeft() {
	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 10; j++) {
			if (inputfield[i][j] == 1) {
				if (j - 1 >= 0 && gamefield[i][j - 1] == 0) {
					//succeed
				}
				else {
					return false;
				}
			}
		}
	}

	//move left
	shaftx -= 1;
	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 10; j++)
			if (inputfield[i][j] == 1) {
				inputfield[i][j - 1] = 1;
				inputfield[i][j] = 0;
			}
	}
	return true;
}

bool inputRight() {
	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 10; j++) {
			if (inputfield[i][j] == 1) {
				if (j + 1 <= 9 && gamefield[i][j + 1] == 0) {
					//succeed
				}
				else {
					return false;
				}
			}
		}
	}

	//move right
	shaftx += 1;
	for (int i = 0; i < 20; i++)
	{
		for (int j = 10-1; j >= 0; j--)
			if (inputfield[i][j] == 1) {
				inputfield[i][j + 1] = 1;
				inputfield[i][j] = 0;
			}
	}
	return true;
}

bool inputDown() {
	//collision check
	for (int i = 20 - 1; i > 0; i--){
		for (int j = 0; j < 10; j++) {
			if (inputfield[i][j] == 1) {
				if (gamefield[i+1][j] == 1)return false;
				if (i + 1 >= 20) return false;
			}
		}
	}

	//down
	for (int i = 20-1; i > 0; i--)
	{
		for (int j = 0; j < 10; j++) {
			if (inputfield[i][j] == 0 && inputfield[i-1][j] == 1){
				inputfield[i][j] = inputfield[i - 1][j];
				inputfield[i - 1][j] = 0;
			}
		}
	}
	shafty += 1;
	return true;
}

void inputSpin() {
	if (nowMinoIndex == 1) return;
	if (nowMinoIndex == 0) {
		int spin[4][4];
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4;j++)
			{
				if (inputfield[shafty + i][shaftx + j] != 0){
					//i=j //j=3-i
					if ((shafty + i)>=0&& (shaftx + j)>= 0
						&& (shafty + i) < 20 && (shaftx + j) < 10) {
						spin[j][3 - i] = inputfield[shafty + i][shaftx + j];
					}
				}
				else {
					spin[j][3 - i] = 0;
				}
			}
		}
		//collision check
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				if(spin[i][j]==1){
					//map wall collision
					if (shafty+i >= 0 && shafty+i < 20
						&& shaftx+j >=0 && shaftx+j <10) {
						//field collision
						if (gamefield[i + shafty][j + shaftx] == 0) {
							//succeed
						}
						else {
							return;//fail
						}
					}
					else {
						return;//fail
					}
				}
			}
		}
		//spin
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				if (spin[i][j] == 1) {
					inputfield[shafty+i][shaftx+j] = 1;
				}
				else {
					inputfield[shafty+i][shaftx+j] = 0;
				}
			}
		}
	}
	else {
		int spin[3][3];
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				if (inputfield[shafty + i][shaftx + j] != 0) {
					//i=j //j=3-i
					if ((shafty + i) >= 0 && (shaftx + j) >= 0
						&& (shafty + i) < 20 && (shaftx + j) < 10) {
						spin[j][2 - i] = inputfield[shafty + i][shaftx + j];
					}
				}
				else {
					spin[j][2 - i] = 0;
				}
			}
		}
		//collision check
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				if (spin[i][j] == 1) {
					//map wall collision
					if (shafty + i >= 0 && shafty + i < 20
						&& shaftx + j >= 0 && shaftx + j < 10) {
						//field collision
						if (gamefield[i + shafty][j + shaftx] == 0) {
							//succeed
						}
						else {
							return;//fail
						}
					}
					else {
						return;//fail
					}
				}
			}
		}
		//spin
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				if (spin[i][j] == 1) {
					inputfield[shafty + i][shaftx + j] = 1;
				}
				else {
					inputfield[shafty + i][shaftx + j] = 0;
				}
			}
		}
	}
}

void lineClear() {
	bool isClear = false;
	int lines = 0;
	for (int i = 0; i < 20; i++)
	{
		int fline = 0;
		for (int j = 0; j < 10; j++) {
			if (gamefield[i][j] == 1) {
				fline++;
			}
		}

		if (fline == 10) {
			//remove line
			lines++;
			for (int j = 0; j < 10; j++) {
				gamefield[i][j] = 0;
			}
			//up to down
			for (int j = i; j >= 1; j--)
			{
				for (int k = 0; k < 10; k++) {
					gamefield[j][k] = gamefield[j - 1][k];
				}
			}
		}
	}
	if (lines > 0) {
		//Sleep(500);
	}
}

void commitInput() {
	//commit
	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 10; j++) {
			if (inputfield[i][j] == 1) {
				gamefield[i][j] = 1;
			}
		}
	}

	//reset input
	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 10; j++) {
			inputfield[i][j] = 0;
		}
	}
}

bool ghostDrop() {

	for (int i = 20 - 1; i > 0; i--) {
		for (int j = 0; j < 10; j++) {
			if (ghostfield[i][j] == 1) {
				if (gamefield[i + 1][j] == 1)return false;
				if (i + 1 >= 20) return false;
			}
		}
	}

	//down
	for (int i = 20 - 1; i > 0; i--)
	{
		for (int j = 0; j < 10; j++) {
			if (ghostfield[i][j] == 0 && ghostfield[i - 1][j] == 1) {
				ghostfield[i][j] = ghostfield[i - 1][j];
				ghostfield[i - 1][j] = 0;
			}
		}
	}
	return true;
}

void makeGhost() {
	//
	for (int i = 0; i < 20; i++)
		for (int j = 0; j < 10; j++)
		{
			ghostfield[i][j] = inputfield[i][j];
		}

	while (ghostDrop() == true);

}


int main() 
{
	system("chcp 949");
	system("title tetris");
	int minoIndex = 0;

	spawn(minoIndex++ % 7);
	int frame = 0;
	while (1) {
		makeGhost();
		gotoxy(0, 0);
		int next[] = { minoIndex % 7 };
		printfield(next);
		//user control
		if (GetAsyncKeyState(17) && frame % 4 == 0) {
			inputSpin();
			inputSpin();
			inputSpin();
		}
		if (_kbhit()) {
			int key = _getch();
			if (key == 224) {
				key = _getch();
				if (key == 75) {
					inputLeft();
				}
				if (key == 77) {
					inputRight();
				}
				if (key == 80) {
					if (inputDown() == true) {
						//gravity
					}
					else {
						//commit
						commitInput();
						lineClear();
						spawn(minoIndex++ % 7);
					}
					Sleep(10);
				}
				if (key == 72) {
					inputSpin();
				}
			}
			else {
				if (key == 'z' ) {
					inputSpin();
					inputSpin();
					inputSpin();
				}
				else if (key == 'x') {
					inputSpin();
				}
				else if (key == ' ') {
					//harddrop
					while (inputDown() == true);
					commitInput();
					lineClear();
					spawn(minoIndex++ % 7);
					Sleep(10);
				}
				else if (key == 'r') {
					//retry
					for (int i = 0; i < 20; i++)
						for (int j = 0; j < 10; j++)
						{
							inputfield[i][j] = 0;
							gamefield[i][j] = 0;
							ghostfield[i][j] = 0;
						}
					minoIndex = 0;
					spawn(minoIndex++ % 7);
					frame = 0;
				}
			}
		}
		if (frame%10 == 0 && inputDown() == false) {
			commitInput();
			lineClear();
			spawn(minoIndex++%7);
		}
		Sleep(16);
		frame++;
	}
}