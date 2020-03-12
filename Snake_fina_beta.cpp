#include <iostream>
#include <windows.h>
#include <conio.h>
#include <time.h>

using namespace std;
bool gameOver,gamePause,isFullWidth,isUpdata;
const int width = 20;
const int height = 20;
int snakeX, snakeY, fruitX, fruitY, score;
int tailX[100];
int tailY[100];
int nTail;
int fruitFlash;
int level;
int speed;
std::string wall,head,body,fruit,back;
clock_t beginTime, stopTime;
enum eDirection
{
	STOP = 0,
	LEFT,
	RIGHT,
	UP,
	DOWN
};
eDirection dir;
HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

void Initial()
{
	SetConsoleTitleA("贪吃蛇by邱靖颖");
	COORD dSize = {60, 60};
	SetConsoleScreenBufferSize(h, dSize);
	CONSOLE_CURSOR_INFO cursor = {1, false};
	SetConsoleCursorInfo(h, &cursor);

	nTail = 1;
	gameOver = false;
	gamePause=false;
	isFullWidth=false;
	isUpdata=false;
	fruitFlash = 0;
	dir = STOP;
	snakeX = width / 2;
	snakeY = height / 2;
	srand((unsigned)time(NULL));
	fruitX = rand() % width;
	fruitY = rand() % height;
	score = 0;
	beginTime = clock();
	level = 1;
	speed = 100;
	wall="#";
	head="O";
	body="o";
	fruit="M";
	back=" ";
}

void setPos(int X, int Y)
{
	COORD pos;
	if (isFullWidth)
		pos.X = 2 * (X + 1);
	else
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
		cout << wall;

	for (int j = 0; j < height; j++)
	{
		setPos(-1, j);
		for (int i = 0; i < width + 2; i++)
		{
			if (i == 0)
				cout << wall;
			else if (i == width + 1)
				cout << wall;
			else
				cout << back;
		}
		cout << endl;
	}

	setPos(-1, height);
	for (int i = 0; i < width + 2; i++)
		cout << wall;
}

void eraseSnake()
{
	for (int i = 0; i < nTail; i++)
	{
		setPos(tailX[i], tailY[i]);
		cout << back;
	}
}

void DrawLocally()
{
	stopTime = clock();
	if (stopTime - beginTime >= 500) //0.5秒为一拍
	{
		beginTime = clock();
		(fruitFlash %= 5)++;
	}

	if (fruitFlash == 4)
	{
		setPos(fruitX, fruitY);
		SetConsoleTextAttribute(h, 0x04);
		cout << back;
	}
	else
	{
		setPos(fruitX, fruitY);
		SetConsoleTextAttribute(h, 0x04);
		cout << fruit;
	}

	for (int i = 0; i < nTail; i++)
	{
		setPos(tailX[i], tailY[i]);
		if (i == 0)
		{
			SetConsoleTextAttribute(h, 0x09);
			cout << head;
		}
		else
		{
			SetConsoleTextAttribute(h, 0x0a);
			cout << body;
		}
	}
}

void GamePause()
{
	gamePause = !gamePause;
	setPos(width / 2 - 6, height / 2 - 4);
	SetConsoleTextAttribute(h,0x0f);
	cout<<"          ";
	if(gamePause)
	{
		setPos(width / 2 - 6, height / 2 - 4);
		SetConsoleTextAttribute(h,0x5a);
		cout<<"游戏暂停中";
	}
}

void ChangeChar()
{
	isUpdata=true;
	isFullWidth=!isFullWidth;
	if(isFullWidth)
	{
	wall="□";
	head="◎";
	body="●";
	fruit="★";
	back="  ";
	}
	else
	{
	wall="#";
	head="O";
	body="o";
	fruit="M";
	back=" ";
	}
}

void Input()
{
	if (_kbhit())
	{
		switch (tolower(_getch()))
		{
		case 'a':
			if (dir != RIGHT&&!gamePause)
				dir = LEFT;
			break;
		case 'd':
			if (dir != LEFT&&!gamePause)
				dir = RIGHT;
			break;
		case 'w':
			if (dir != DOWN&&!gamePause)
				dir = UP;
			break;
		case 's':
			if (dir != UP&&!gamePause)
				dir = DOWN;
			break;
		case 'x':
			gameOver = true;
			break;
		case 'c':
			ChangeChar();
			break;
		case 'f':
			GamePause();
			break;
		case 224:
		switch (_getch())
		{
		case 75:
			if (dir != RIGHT && !gamePause)
				dir = LEFT;
			break;
		case 77:
			if (dir != LEFT && !gamePause)
				dir = RIGHT;
			break;
		case 72:
			if (dir != DOWN && !gamePause)
				dir = UP;
			break;
		case 80:
			if (dir != UP && !gamePause)
				dir = DOWN;
			break;
		}
		break;
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

	//不能穿墙的蛇
	// if (snakeX > width - 1 || snakeX < 0 || snakeY > height - 1 || snakeY < 0)
	// 	gameOver = true;

	//不能碰蛇身
	for (int i = 1; i < nTail; i++)
	{
		if (tailX[i] == snakeX && tailY[i] == snakeY)
			gameOver = true;
	}

	//穿墙的蛇
	if (snakeX > width - 1)
		snakeX = 0;
	else if (snakeX < 0)
		snakeX = width - 1;
	if (snakeY > height - 1)
		snakeY = 0;
	else if (snakeY < 0)
		snakeY = height - 1;

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

	level = (score / 100) % 6 + 1;
	switch (level)
	{
	case 1:
		speed = 100;
		break;
	case 2:
		speed = 80;
		break;
	case 3:
		speed = 60;
		break;
	case 4:
		speed = 40;
		break;
	case 5:
		speed = 20;
		break;
	case 6:
		speed = 10;
		break;
	default:
		break;
	}
}

void PromptInfo()
{
	int k = 0,j=width + 4/wall.length();
	setPos(j, ++k);
	SetConsoleTextAttribute(h, 0x0f);
	cout << "■ 游戏规则";
	k++;
	setPos(j, ++k);
	cout << "  1.蛇能穿墙";
	setPos(j, ++k);
	cout << "  2.蛇不能撞自己";
	k++;
	setPos(j, ++k);
	cout << "■ 操作方法";
	k++;
	setPos(j, ++k);
	cout << "  ◆向左：←a";
	setPos(j, ++k);
	cout << "  ◆向右：→d";
	setPos(j, ++k);
	cout << "  ◆向上：↑w";
	setPos(j, ++k);
	cout << "  ◆向下：↓s";
	setPos(j, ++k);
	cout << "  ◆结束：x";
	setPos(j, ++k);
	cout << "  ◆全角/半角切换：c";
	setPos(j, ++k);
	cout << "  ◆暂停/继续：f";
	setPos(j, height - 4);
	SetConsoleTextAttribute(h, 0x0f);
	cout << "★ 游戏等级: ";
	setPos(j, height - 2);
	SetConsoleTextAttribute(h, 0x0f);
	cout << "■ 当前游戏得分: ";
}

void ShowDynamically()
{
	setPos(width + 16/wall.length(), height - 4);
	SetConsoleTextAttribute(h, 0x0a);
	cout << level;
	setPos(width + 20/wall.length(), height - 2);
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
				cout << wall;
		}
		else
		{
			for (int i = 0; i < 12; i++)
			{
				if (i == 0 || i == 11)
					cout << wall;
				else
					cout << back;
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
	setPos(over_2x-4, over_2y+3);
	SetConsoleTextAttribute(h,0x5a);
	cout << "按y继续，按n退出" << endl;
	SetConsoleTextAttribute(h,0x0f);

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
			else if (tolower(c) == 'n')
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
		do
		{
		isUpdata=false;
		DrawMap();
		PromptInfo();
		if(gamePause)
		{
			gamePause=false;
			GamePause();
		}
		while (!gameOver&&!isUpdata)
		{
			Input();
			if (!gamePause)
			{
				eraseSnake();
				Logic();
			}
			DrawLocally();
			ShowDynamically();
			Sleep(speed);
		}
		if(gameOver)
			GameOverInfo();
		} while (isUpdata);	
	} while (!gameOver);

	return 0;
}