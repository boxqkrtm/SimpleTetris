#include <iostream> // stdioh 포함
#include <Windows.h>
#include <conio.h>
using namespace std;

void gotoxy(int x, int y)
{
	COORD Pos;
	Pos.X = x;
	Pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

class Tet
{
public:
	Tet(Tet *g)
	{
		for (int i = 0; i < 20; i++)
			for (int j = 0; j < 10; j++)
			{
				gamefield[i][j] = g->gamefield[i][j];
				inputfield[i][j] = g->inputfield[i][j];
				ghostfield[i][j] = g->ghostfield[i][j];
			}
		shafty = g->shafty;
		shaftx = g->shaftx;
	}
	Tet()
	{
		for (int i = 0; i < 20; i++)
			for (int j = 0; j < 10; j++)
			{
				gamefield[i][j] = 0;
				inputfield[i][j] = 0;
				ghostfield[i][j] = 0;
			}
	}
	int gamefield[20][10];
	int inputfield[20][10];
	int ghostfield[20][10];
	int shaftx;
	int shafty;
	int mino[7][4][4] = {
		{{0, 0, 0, 0},
		 {1, 1, 1, 1},
		 {0, 0, 0, 0},
		 {0, 0, 0, 0}},
		{{0, 0, 0, 0},
		 {0, 1, 1, 0},
		 {0, 1, 1, 0},
		 {0, 0, 0, 0}},
		{{0, 0, 1, 0},
		 {1, 1, 1, 0},
		 {0, 0, 0, 0},
		 {0, 0, 0, 0}},
		{{1, 0, 0, 0},
		 {1, 1, 1, 0},
		 {0, 0, 0, 0},
		 {0, 0, 0, 0}},
		{{0, 1, 1, 0},
		 {1, 1, 0, 0},
		 {0, 0, 0, 0},
		 {0, 0, 0, 0}},
		{{1, 1, 0, 0},
		 {0, 1, 1, 0},
		 {0, 0, 0, 0},
		 {0, 0, 0, 0}},
		{{0, 1, 0, 0},
		 {1, 1, 1, 0},
		 {0, 0, 0, 0},
		 {0, 0, 0, 0}} };

	void printfield(int next[])
	{
		cout << "┏━━━━━━━━━━━━━━━━━━━━┓" << endl;
		for (int i = 0; i < 20; i++)
		{
			cout << "┃";
			for (int j = 0; j < 10; j++)
			{

				if (gamefield[i][j] == 1 || inputfield[i][j] == 1)
				{
					cout << "▣";
				}
				else if (ghostfield[i][j] == 1)
				{
					cout << "□";
				}
				else
				{
					cout << "  ";
				}
			}
			cout << "┃";
			cout << "  ";
			if (i < 4)
			{
				for (int k = 0; k < 4; k++)
				{
					if (mino[next[0]][i][k] == 1)
					{
						cout << "▣";
					}
					else
					{
						cout << "  ";
					}
				}
			}
			cout << endl;
		}
		cout << "┗━━━━━━━━━━━━━━━━━━━━┛" << endl;
	}

	void spawn(int minoindex)
	{
		int(*nowmino)[4] = mino[minoindex];
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				inputfield[i][j + 3] = nowmino[i][j];
			}
		}
		shafty = 0;
		shaftx = 3;
	}

	bool inputLeft()
	{
		for (int i = 0; i < 20; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				if (inputfield[i][j] == 1)
				{
					if (j - 1 >= 0 && gamefield[i][j - 1] == 0)
					{
						//succeed
					}
					else
					{
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
				if (inputfield[i][j] == 1)
				{
					inputfield[i][j - 1] = 1;
					inputfield[i][j] = 0;
				}
		}
		return true;
	}

	bool inputRight()
	{
		for (int i = 0; i < 20; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				if (inputfield[i][j] == 1)
				{
					if (j + 1 <= 9 && gamefield[i][j + 1] == 0)
					{
						//succeed
					}
					else
					{
						return false;
					}
				}
			}
		}

		//move right
		shaftx += 1;
		for (int i = 0; i < 20; i++)
		{
			for (int j = 10 - 1; j >= 0; j--)
				if (inputfield[i][j] == 1)
				{
					inputfield[i][j + 1] = 1;
					inputfield[i][j] = 0;
				}
		}
		return true;
	}

	bool inputDown()
	{
		//collision check
		for (int i = 20 - 1; i > 0; i--)
		{
			for (int j = 0; j < 10; j++)
			{
				if (inputfield[i][j] == 1)
				{
					if (gamefield[i + 1][j] == 1)
						return false;
					if (i + 1 >= 20)
						return false;
				}
			}
		}

		//down
		for (int i = 20 - 1; i > 0; i--)
		{
			for (int j = 0; j < 10; j++)
			{
				if (inputfield[i][j] == 0 && inputfield[i - 1][j] == 1)
				{
					inputfield[i][j] = inputfield[i - 1][j];
					inputfield[i - 1][j] = 0;
				}
			}
		}
		shafty += 1;
		return true;
	}

	void inputSpin(int nowMinoIndex)
	{
		if (nowMinoIndex == 1)
			return;
		if (nowMinoIndex == 0)
		{
			int spin[4][4];
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					if (inputfield[shafty + i][shaftx + j] != 0)
					{
						//i=j //j=3-i
						if ((shafty + i) >= 0 && (shaftx + j) >= 0 && (shafty + i) < 20 && (shaftx + j) < 10)
						{
							spin[j][3 - i] = inputfield[shafty + i][shaftx + j];
						}
					}
					else
					{
						spin[j][3 - i] = 0;
					}
				}
			}
			//collision check
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					if (spin[i][j] == 1)
					{
						//map wall collision
						if (shafty + i >= 0 && shafty + i < 20 && shaftx + j >= 0 && shaftx + j < 10)
						{
							//field collision
							if (gamefield[i + shafty][j + shaftx] == 0)
							{
								//succeed
							}
							else
							{
								return; //fail
							}
						}
						else
						{
							return; //fail
						}
					}
				}
			}
			//spin
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					if (spin[i][j] == 1)
					{
						inputfield[shafty + i][shaftx + j] = 1;
					}
					else
					{
						inputfield[shafty + i][shaftx + j] = 0;
					}
				}
			}
		}
		else
		{
			int spin[3][3];
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					if (inputfield[shafty + i][shaftx + j] != 0)
					{
						//i=j //j=3-i
						if ((shafty + i) >= 0 && (shaftx + j) >= 0 && (shafty + i) < 20 && (shaftx + j) < 10)
						{
							spin[j][2 - i] = inputfield[shafty + i][shaftx + j];
						}
					}
					else
					{
						spin[j][2 - i] = 0;
					}
				}
			}
			//collision check
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					if (spin[i][j] == 1)
					{
						//map wall collision
						if (shafty + i >= 0 && shafty + i < 20 && shaftx + j >= 0 && shaftx + j < 10)
						{
							//field collision
							if (gamefield[i + shafty][j + shaftx] == 0)
							{
								//succeed
							}
							else
							{
								return; //fail
							}
						}
						else
						{
							return; //fail
						}
					}
				}
			}
			//spin
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					if (spin[i][j] == 1)
					{
						inputfield[shafty + i][shaftx + j] = 1;
					}
					else
					{
						inputfield[shafty + i][shaftx + j] = 0;
					}
				}
			}
		}
	}

	void lineClear()
	{
		bool isClear = false;
		int lines = 0;
		for (int i = 0; i < 20; i++)
		{
			int fline = 0;
			for (int j = 0; j < 10; j++)
			{
				if (gamefield[i][j] == 1)
				{
					fline++;
				}
			}

			if (fline == 10)
			{
				//remove line
				lines++;
				for (int j = 0; j < 10; j++)
				{
					gamefield[i][j] = 0;
				}
				//up to down
				for (int j = i; j >= 1; j--)
				{
					for (int k = 0; k < 10; k++)
					{
						gamefield[j][k] = gamefield[j - 1][k];
					}
				}
			}
		}
		if (lines > 0)
		{
			//Sleep(500);
		}
	}

	void commitInput()
	{
		//commit
		for (int i = 0; i < 20; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				if (inputfield[i][j] == 1)
				{
					gamefield[i][j] = 1;
				}
			}
		}

		//reset input
		for (int i = 0; i < 20; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				inputfield[i][j] = 0;
			}
		}
	}

	bool ghostDrop()
	{

		for (int i = 20 - 1; i > 0; i--)
		{
			for (int j = 0; j < 10; j++)
			{
				if (ghostfield[i][j] == 1)
				{
					if (gamefield[i + 1][j] == 1)
						return false;
					if (i + 1 >= 20)
						return false;
				}
			}
		}

		//down
		for (int i = 20 - 1; i > 0; i--)
		{
			for (int j = 0; j < 10; j++)
			{
				if (ghostfield[i][j] == 0 && ghostfield[i - 1][j] == 1)
				{
					ghostfield[i][j] = ghostfield[i - 1][j];
					ghostfield[i - 1][j] = 0;
				}
			}
		}
		return true;
	}

	void makeGhost()
	{
		//
		for (int i = 0; i < 20; i++)
			for (int j = 0; j < 10; j++)
			{
				ghostfield[i][j] = inputfield[i][j];
			}

		while (ghostDrop() == true)
			;
	}

	int m()
	{
		int minoIndex = 0;
		spawn(minoIndex++ % 7);
		int frame = 0;
		while (1)
		{
			makeGhost();
			gotoxy(0, 0);
			int next[] = { minoIndex % 7 };
			printfield(next);
			cout << getScore() << endl;
			//user control
			if (GetAsyncKeyState(17) && frame % 4 == 0)
			{
				inputSpin((minoIndex - 1) % 7);
				inputSpin((minoIndex - 1) % 7);
				inputSpin((minoIndex - 1) % 7);
			}
			if (_kbhit())
			{
				int key = _getch();
				if (key == 97)//a키
				{
					
					//경우의수 4*9
					Tet* sim[36];
					int index = 0;//sim index
					int maxScoreIndex = 0;
					int maxScore = -9999999999;
					int score[36];
					for (int move = -4; move <= 4; move++)
					{
						for (int spin = 0; spin < 4; spin++)
						{
							score[index] = ScoreFinder(this, spin, minoIndex, move);
							index++;
						}
					}
					for (int i = 0; i < 36; i++)
					{
						if (score[i] > maxScore)
						{
							maxScore = score[i];
							maxScoreIndex = i;
						}
					}

					//입력함
					index = 0;
					cout << maxScoreIndex << endl;
					for (int move = -4; move <= 4; move++)
					{
						bool isb = false;
						for (int spin = 0; spin < 4; spin++)
						{
							if (index == maxScoreIndex)
							{
								isb = true;
								for (int m = 0; m < spin; m++)
									inputSpin((minoIndex % 7) - 1);

								if (move < 0)
									for (int m = 0; m < abs(move); m++)
										inputLeft();
								if (move > 0)
									for (int m = 0; m < abs(move); m++)
										inputRight();
								while (inputDown() == true);
								commitInput();
								lineClear();
								spawn(minoIndex++ % 7);
								break;
							}
							index++;
						}
						if (isb == true) break;
					}
					
				}
				else if (key == 224)
				{
					key = _getch();
					if (key == 75)
					{
						inputLeft();
					}
					if (key == 77)
					{
						inputRight();
					}
					if (key == 80)
					{
						if (inputDown() == true)
						{
							//gravity
						}
						else
						{
							//commit
							commitInput();
							lineClear();
							spawn(minoIndex++ % 7);
						}
						Sleep(10);
					}
					if (key == 72)
					{
						inputSpin((minoIndex - 1) % 7);
					}
				}
				else
				{
					if (key == 'z')
					{
						inputSpin((minoIndex - 1) % 7);
						inputSpin((minoIndex - 1) % 7);
						inputSpin((minoIndex - 1) % 7);
					}
					else if (key == 'x')
					{
						inputSpin((minoIndex - 1) % 7);
					}
					else if (key == ' ')
					{
						//harddrop
						while (inputDown() == true)
							;
						commitInput();
						lineClear();
						spawn(minoIndex++ % 7);
						Sleep(10);
					}
					else if (key == 'r')
					{
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
			if (frame % 10 == 0 && inputDown() == false)
			{
				commitInput();
				lineClear();
				spawn(minoIndex++ % 7);
			}
			Sleep(16);
			frame++;
		}
	}

	int ScoreFinder(Tet* nowState, int spin, int minoIndex, int move, int depth = 0, int score = 0)
	{
		Tet* sim = new Tet(nowState);
		for (int m = 0; m < spin; m++)
			sim->inputSpin((minoIndex % 7) - 1);
		if (move < 0)
			for (int m = 0; m < abs(move); m++)
				sim->inputLeft();
		if (move > 0)
			for (int m = 0; m < abs(move); m++)
				sim->inputRight();
		while (sim->inputDown() == true);
		sim->commitInput();
		sim->lineClear();
		score += sim->getScore();

		if (depth != 1)
		{
			sim->spawn(minoIndex++ % 7);
			int max=-99999999999;
			int maxI=0;
			int index = 0;
			for (int move = -4; move <= 4; move++)
			{
				for (int spin = 0; spin < 4; spin++)
				{
					int stest = ScoreFinder(sim, spin, minoIndex, move, depth + 1, 0);
					if (max < stest)
					{
						max = stest;
						maxI = index;
					}
					index++;
				}
			}
			return max;
		}
		else
			return score;
	}

	int getScore()
	{
		int score = 0;
		//less hole more score
		for (int j = 0; j < 10; j++)
		{
			for (int i = 0; i < 20; i++)
			{
				if (gamefield[i][j] == 1)
				{
					//좌우의 미노높이가 +3이거나 -3이 넘어가면 감점
					//좌
					if (j - 1 >= 0)
					{
						int h = 0;
						for (int a = 0; a < 20; a++)
						{
							if (gamefield[a][j - 1] == 1)
							{
								h = a;
							}
						}
						if (abs(h - i) >= 3) score -= 30* abs(h - i) * abs(h - i);
					}
					//우
					if (j + 1 <= 19)
					{
						int h = 0;
						for (int a = 0; a < 20; a++)
						{
							if (gamefield[a][j + 1] == 1)
							{
								h = a;
							}
						}
						if (abs(h - i) >= 3) score -= 30* abs(h - i) * abs(h - i);
					}
					break;
				}
			}

			//블록 밑에 빈칸하나당 감점
			for (int j = 0; j < 10; j++)
			{
				bool isFindBlock = false;
				for (int i = 0; i < 20; i++) 
				{
					if (isFindBlock == true)
					{
						if (gamefield[i][j] == 0)
						{
							score -= 1000;
						}
					}
					else
					{
						if (gamefield[i][j] == 1)
						{
							isFindBlock = true;
						}
					}
					
				}
			}
		}
		//평균 높이가 평평하지 않을수록 감점
		int height[10] = { 0 };
		float sum=0;
		int maxHeight = 20;
		for (int j = 0; j < 10; j++)
		{
			for (int i = 0; i < 20; i++)
			{
				if (gamefield[i][j] == 1)
				{
					height[j] = i;
					if (height[j] < maxHeight)
					{
						maxHeight = height[j];
					}
					sum += i;
					break;
				}
			}
		}
		for (int j = 0; j < 10; j++)
		{
			float avg = sum / 10;
			score -= (int)(avg - height[j] * 100);
		}
		//최고높이 낮을수록 추가점수
		score += (maxHeight) * 10000;
		


		return score;
	}
};

int main()
{
	//좌우 4//회전4
	//8*4=32노드
	system("chcp 65001");
	system("title tetris");
	Tet* g = new Tet();
	
	int score[32];
	int minoIndex = 0;
	g->m();
}