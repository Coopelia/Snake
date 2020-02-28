#include <iostream>
#include <windows.h>
#include <conio.h>
#include<time.h>

using namespace std;
bool gameOver, fruitFlash;
const int width = 50;
const int height = 20;
int snakeX, snakeY, fruitX, fruitY, score;
int tailX[100];
int tailY[100];
int nTail;
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirection dir;
HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

void Initial()
{
	SetConsoleTitleA("贪吃蛇");
	COORD dSize = { 60,60 };
	SetConsoleScreenBufferSize(h, dSize);
	CONSOLE_CURSOR_INFO cursor = { 1,false };
	SetConsoleCursorInfo(h, &cursor);

	nTail = 1;
	gameOver = false;
	fruitFlash = false;
	dir = STOP;
	snakeX = width / 2;
	snakeY = height / 2;
	srand((unsigned)time(NULL));
	fruitX = rand() % width;
	fruitY = rand() % height;
	score = 0;
}

void setPos(int X, int Y)
{
	COORD pos;
	pos.X = X + 1;
	pos.Y = Y + 1;
	SetConsoleCursorPosition(h, pos);
}

void DrawMap()
{
	system("cls");
	int textColor = 0x06;
	SetConsoleTextAttribute(h, textColor);

	setPos(-1, -1);
	for (int i = 0; i < width + 2; i++)
		cout << "#";

	for (int j = 0; j < height; j++)
	{
		setPos(-1, j);
		for (int i = 0; i < width + 2; i++)
		{
			if (i == 0)
				cout << "#";
			else if (i == width + 1)
				cout << "#";
			else
				cout << " ";
		}
		cout << endl;
	}

	setPos(-1, height);
	for (int i = 0; i < width + 2; i++)
		cout << "#";
}

void eraseSnake()
{
	for (int i = 0; i < nTail; i++)
	{
		setPos(tailX[i], tailY[i]);
		cout << " ";
	}
}

void DrawLocally()
{
	if (!fruitFlash)
	{
		setPos(fruitX, fruitY);
		SetConsoleTextAttribute(h, 0x04);
		cout << "M";
		fruitFlash = true;
	}
	else
	{
		setPos(fruitX, fruitY);
		SetConsoleTextAttribute(h, 0x04);
		cout << " ";
		fruitFlash = false;
	}

	for (int i = 0; i < nTail; i++)
	{
		setPos(tailX[i], tailY[i]);
		if (i == 0)
		{
			SetConsoleTextAttribute(h, 0x09);
			cout << "O";
		}
		else
		{
			SetConsoleTextAttribute(h, 0x0a);
			cout << "o";
		}
	}
}

void Input()
{
	if (_kbhit())
	{
		switch (tolower(_getch()))
		{
		case 'a':
		if(dir!=RIGHT)
			dir = LEFT;
			break;
		case 'd':
		if(dir!=LEFT)
			dir = RIGHT;
			break;
		case 'w':
		if(dir!=DOWN)
			dir = UP;
			break;
		case 's':
		if(dir!=UP)
			dir = DOWN;
			break;
		case 'x':
			gameOver = true;
		default:
			break;
		}
	}
}

void Logic()
{
	switch (dir)
	{
	case LEFT:
		snakeX--;
		break;
	case RIGHT:
		snakeX++;
		break;
	case UP:
		snakeY--;
		break;
	case DOWN:
		snakeY++;
		break;
	default:
		break;
	}

	if (snakeX == fruitX && snakeY == fruitY)
	{
		score += 10;
		fruitX = rand() % width;
		fruitY = rand() % height;
		nTail++;
	}

	for (int i = nTail - 1; i > 0; i--)
	{
		tailX[i] = tailX[i - 1];
		tailY[i] = tailY[i - 1];
	}
	tailX[0] = snakeX;
	tailY[0] = snakeY;

	//不能穿墙的蛇
	if (snakeX > width - 1 || snakeX < 0 || snakeY > height - 1 || snakeY < 0)
		gameOver = true;

	//不能碰蛇身
	for (int i = 1; i < nTail; i++)
	{
		if (tailX[i] == snakeX && tailY[i] == snakeY)
			gameOver = true;
	}

	//穿墙的蛇
	/*if (snakeX > width - 1)
		snakeX = 0;
	else if (snakeX < 0)
		snakeX = width - 1;
	if (snakeY > height - 1)
		snakeY = 0;
	else if (snakeY < 0)
		snakeY = height - 1;*/
}

void PromptInfo()
{
	int k = 1;
	setPos(width + 3, ++k);
	SetConsoleTextAttribute(h, 0x0f);
	cout << "■ 游戏规则";
	k++;
	setPos(width + 3, ++k);
	cout << "  1.蛇不能撞墙";
	setPos(width + 3, ++k);
	cout << "  2.蛇不能撞自己";
	k++;
	setPos(width + 3, ++k);
	cout << "■ 操作方法";
	k++;
	setPos(width + 3, ++k);
	cout << "  ◆向左：a";
	setPos(width + 3, ++k);
	cout << "  ◆向右：d";
	setPos(width + 3, ++k);
	cout << "  ◆向上：w";
	setPos(width + 3, ++k);
	cout << "  ◆向下：s";
}

void ShowScore()
{
	setPos(width + 3, height - 3);
	SetConsoleTextAttribute(h, 0x0f);
	cout << "■ 当前游戏得分: ";
	SetConsoleTextAttribute(h, 0x0c);
	cout << score;
}

void GameOverInfo()
{
	int over_x = width / 2 - 6, over_y = height / 2 - 4;
	int over_2x = over_x + 2, over_2y = over_y + 2;
	SetConsoleTextAttribute(h, 0x0f);
	for (int j = 0; j < 7; j++)
	{
		setPos(over_x, over_y);
		if (j == 0 || j == 6)
		{
			for (int k = 0; k < 12; k++)
				cout << "#";
		}
		else
		{
			for (int i = 0; i < 12; i++)
			{
				if (i == 0 || i == 11)
					cout << "#";
				else
					cout << " ";
			}
		}
		over_y++;
	}
	setPos(over_2x, over_2y);
	cout << "游戏结束";
	setPos(over_2x, ++over_2y);
	cout << "得分:";
	setPos(over_2x, ++over_2y);
	cout << score;
	setPos(0, height + 1);
	cout << "按y继续，按n退出" << endl;

	bool isOk = false;
	while (!isOk)
	{
		if (_kbhit())
		{
			char c = _getch();
			if (tolower(c) == 'y')
			{		
				gameOver = false;
				isOk = true;
			}	
			else if(tolower(c)=='n')
			{
				gameOver = true;
				isOk = true;
			}		
			else	
			isOk = false;
		}
	}
}

int main()
{
	do
	{
		Initial();
		DrawMap();
		PromptInfo();
		while (!gameOver)
		{
			Input();
			eraseSnake();
			Logic();
			DrawLocally();
			ShowScore();
			Sleep(100);
		}
		GameOverInfo();
	} while (!gameOver);

	return 0;
}