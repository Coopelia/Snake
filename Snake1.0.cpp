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
	score = 0;
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

	if (snakeX == fruitX && snakeY == fruitY)
	{
		score += 10;
		fruitX = rand() % width;
	    fruitY = rand() % height;
		nTail++;
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

int main()
{
	Initial();
	while (!gameOver)
	{	
		Draw();
		Input();
		Logic();
	}
	system("pause");
	return 0;
}