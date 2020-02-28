#include <iostream>
#include<windows.h>
#include <conio.h>
using namespace std;

bool gameOver;
const int width = 20;
const int height = 20;
int snakeX, snakeY, fruitX, fruitY, score;
int tailX[100], tailY[100];
int nTail;

HANDLE hOutput, hOutBuf;
COORD coord = { 0,0 };
DWORD bytes = 0;
WORD textColor;
bool BufferSwapFlag = false;
char ScreenData[width + 5][height + 5];

enum eDirection
{
	STOP = 0, LEFT, RIGHT, UP, DOWN
};
eDirection dir;

void Initial()
{
	gameOver = false;
	dir = STOP;
	snakeX = width / 2;
	snakeY = height / 2;
	fruitX = rand() % width;
	fruitY = rand() % height;
	nTail = 1;
	score = 0;
	hOutput = CreateConsoleScreenBuffer(GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	hOutBuf = CreateConsoleScreenBuffer(GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	CONSOLE_CURSOR_INFO cur;
	cur.bVisible = 0;
	cur.dwSize = 1;
	SetConsoleCursorInfo(hOutput, &cur);
	SetConsoleCursorInfo(hOutBuf, &cur);
}

void Draw()
{
	system("cls");
	for (int i = 0; i < width + 2; i++)
		cout << "#";
	cout << endl;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (j == 0)
				cout << "#";

			if (i == snakeY && j == snakeX)
				cout << "0";
			else if (i == fruitY && j == fruitX)
				cout << "F";
			else
			{
				bool flagPrint = false;
				for (int k = 0; k < nTail; k++)
				{
					if (tailX[k] == j && tailY[k] == i) {
						cout << "o";
						flagPrint = true;
					}
				}
				if (!flagPrint)
					cout << " ";
			}

			if (j == width - 1)
				cout << "#";
		}
		cout << endl;
	}
	for (int i = 0; i < width + 2; i++)
		cout << "#";
	cout << endl;
	cout << "游戏分数" << score << endl;
}

void Draw2()
{
	int i, j;	int currentLine = 0;
	for (j = 0; j < width + 2; j++) ScreenData[currentLine][j] = '#';
	currentLine++;
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			if (j == 0)ScreenData[currentLine + i][j] = '#';
			else if (i == fruitY && j == fruitX)
			{
				ScreenData[currentLine + i][j] = 'F';
			}
			else if (i == snakeY && j == snakeX)
			{
				ScreenData[currentLine + i][j] = 'O';
			}
			else {
				bool flagPrint = false;
				for (int k = 0; k < nTail; k++)
				{
					if (tailX[k] == j && tailY[k] == i)
					{
						ScreenData[currentLine + i][j] = 'o';
						flagPrint = true;
					}
				}
				if (!flagPrint)  ScreenData[currentLine + i][j] = ' ';;
			}
			if (j == width - 1)	ScreenData[currentLine + i][j] = '#';
		}
	}
	for (j = 0; j < width + 2; j++)ScreenData[currentLine + i][j] = '#';
	currentLine++;
	sprintf(ScreenData[currentLine + i], "游戏得分: %d", score);
}

void Show_doubleBuffer()
{
	int i;
	Draw2(); //在缓冲区画当前游戏区
	if (BufferSwapFlag == false)
	{
		BufferSwapFlag = true;
		for (i = 0; i < height + 5; i++) //输出缓冲区hOutBuf到屏幕
		{
			coord.Y = i;
			for (int j = 0; j < width + 5; j++)
			{
				coord.X = j;
				if (j==snakeX&&i==snakeY+1)
				{
					textColor = 0x03;
				}
				else if (j == fruitX&& i == fruitY + 1)
				{
					textColor = 0x04;
				}
				else 
				{
					bool flagPrint = false;
					for (int k = 0; k < nTail; k++)
					{
						if (tailX[k] == j && tailY[k]+1 == i)
						{
							textColor = 0x0a;
							flagPrint = true;
						}
					}
					if (!flagPrint) textColor = 0x06;
				}
				WriteConsoleOutputAttribute(hOutBuf, &textColor, 1, coord, &bytes);
			}
			coord.X = 0;
			WriteConsoleOutputCharacterA(hOutBuf, ScreenData[i], width, coord, &bytes);
		}
		SetConsoleActiveScreenBuffer(hOutBuf);
	}
	else//输出缓冲区hOutput到屏幕
	{
		BufferSwapFlag = false;
		for (i = 0; i < height + 5; i++)
		{
			coord.Y = i;
			for (int j = 0; j < width + 5; j++)
			{
				coord.X = j;
				if (j == snakeX && i == snakeY+1)
				{
					textColor = 0x03;
				}
				else if (j == fruitX && i == fruitY + 1)
				{
					textColor = 0x04;
				}
				else
				{
					bool flagPrint = false;
					for (int k = 0; k < nTail; k++)
					{
						if (tailX[k] == j && tailY[k]+1 == i)
						{
							textColor = 0x0a;
							flagPrint = true;
						}
					}
					if (!flagPrint) textColor = 0x06;
				}
				WriteConsoleOutputAttribute(hOutput, &textColor, 1, coord, &bytes);
			}
			coord.X = 0;
			WriteConsoleOutputCharacterA(hOutput, ScreenData[i], width, coord, &bytes);
		}
		SetConsoleActiveScreenBuffer(hOutput);
	}
}


void Input()
{
	if (_kbhit())
	{
		switch (tolower(_getch()))
		{
		case 'a':
			if (dir != RIGHT)
				dir = LEFT;
			break;
		case 'd':
			if (dir != LEFT)
				dir = RIGHT;
			break;
		case 'w':
			if (dir != DOWN)
				dir = UP;
			break;
		case 's':
			if (dir != UP)
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

	////不能穿墙的蛇
	//if (snakeX > width - 1 || snakeX < 0 || snakeY > height - 1 || snakeY < 0)
	//	gameOver = true;

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
}

void gameControl()
{
	bool isOk = false;
	while (gameOver&&!isOk)
	{
		coord.X = 1;
		coord.Y = height / 2;
		WriteConsoleOutputCharacterA(hOutput, "游戏结束", 10, coord, &bytes);
		coord.Y = height / 2 + 1;
		WriteConsoleOutputCharacterA(hOutput, "按Y继续/N退出", 20, coord, &bytes);
		char c;
		if (_kbhit())
		{
			switch (tolower(_getch()))
			{
			case 'y':
				gameOver = false;
				isOk = true;
			case 'n':
				gameOver = true;
				isOk = true;
			default:
				break;
			}
		}
	}
}

int main()
{
	Initial();
	while (!gameOver)
	{
		//Draw();
		Show_doubleBuffer();
		Input();
		Logic();
		Sleep(100);
		gameControl();
	}

	return 0;
}