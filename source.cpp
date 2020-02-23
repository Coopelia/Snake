#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
using namespace sf;

bool gameOver, fruitFlash, isSilky;
const int W = 20;
const int H = 20;
const int PosX = 1;
const int PosY = 1;
const int size = 30;
const int dt_x = -2;
const int dt_y = -2;
const float step = 0.1;
const float scale = 0.6;
int  snakeX, snakeY, fruitX, fruitY, score;
int tailX[100];
int tailY[100];
int nTail;
int headRotation;
float stepX, stepY;
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirection dir, dir_ing;
Texture tBack, tBackground, tHead, tTail, tFruit, tRec1;
Sprite sBack, sBackground, sHead, sTail, sFruit, sRec1;
Font font;
Text t1, t2, t3, t4, t5;
Music BGM;
SoundBuffer bEat, bDie;
Sound Eat, Die;

void Initial()
{
	nTail = 0;
	headRotation = 0;
	gameOver = false;
	fruitFlash = false;
	isSilky = false;
	dir = STOP;
	dir_ing = STOP;
	snakeX = PosX + W / 2;
	snakeY = PosY + H / 2;
	stepX = 0;
	stepY = 0;
	srand(time(0));
	fruitX = rand() % W + PosX;
	fruitY = rand() % H + PosY;
	score = 0;

	tBackground.loadFromFile("image/BK.png");
	tBack.loadFromFile("image/back.jpg");
	tHead.loadFromFile("image/sh01.png");
	tTail.loadFromFile("image/sb0201.png");
	tFruit.loadFromFile("image/sb0202.png");
	tRec1.loadFromFile("image/rec01.png");
	sBackground.setTexture(tBackground);
	sBack.setTexture(tBack);
	sHead.setTexture(tHead);
	sTail.setTexture(tTail);
	sFruit.setTexture(tFruit);
	sRec1.setTexture(tRec1);
	font.loadFromFile("font/oil.ttf");
	t1.setFont(font);
	t2.setFont(font);
	t3.setFont(font);
	t4.setFont(font);
	t5.setFont(font);

	BGM.openFromFile("audios/BGM01.ogg");
	bEat.loadFromFile("audios/Eat01.ogg");
	bDie.loadFromFile("audios/Die01.ogg");
	Eat.setBuffer(bEat);
	Die.setBuffer(bDie);
	BGM.setLoop(true);
	BGM.setVolume(100);
	BGM.play();
}

void Logic()
{
	switch (dir)
	{
	case LEFT:
		snakeX--;
		headRotation = -90;
		break;
	case RIGHT:
		snakeX++;
		headRotation = 90;
		break;
	case UP:
		snakeY--;
		headRotation = 0;
		break;
	case DOWN:
		snakeY++;
		headRotation = 180;
		break;
	default:
		break;
	}

	if (snakeX == fruitX && snakeY == fruitY)
	{
		Eat.play();
		score += 10;
		fruitX = rand() % W + PosX;
		fruitY = rand() % H + PosY;
		nTail++;
	}

	//不能穿墙的蛇
	/*if (snakeX > W || snakeX < 0 || snakeY >H || snakeY < 0)
	{
		Die.setBuffer(bDie);
		Die.play();
		gameOver = true;
	}*/

		//不能碰蛇身
	for (int i = 1; i < nTail; i++)
	{
		if (tailX[i] == snakeX && tailY[i] == snakeY)
		{
			Die.setBuffer(bDie);
			Die.play();
			gameOver = true;
		}
	}

	//穿墙的蛇
	if (snakeX > W - 1 + PosX)
		snakeX = PosX;
	else if (snakeX < PosX)
		snakeX = W - 1 + PosX;
	if (snakeY > H - 1 + PosY)
		snakeY = PosY;
	else if (snakeY < PosY)
		snakeY = H - 1 + PosY;

	for (int i = nTail; i > 0; i--)
	{
		tailX[i] = tailX[i - 1];
		tailY[i] = tailY[i - 1];
	}
	tailX[0] = snakeX;
	tailY[0] = snakeY;
}

void LogicStep(RenderWindow &app)
{
	bool updateFlag = false;
	switch (dir_ing)
	{
	case LEFT:
		stepX -= step;
		if (stepX <= -0.9999 || stepX >= 0.9999)
		{
			snakeX--;
			stepX = 0;
			stepY = 0;
			dir_ing = dir;
			headRotation = -90;
			updateFlag = true;
		}
		break;
	case RIGHT:
		stepX += step;
		if (stepX <= -0.9999 || stepX >= 0.9999)
		{
			snakeX++;
			stepX = 0;
			stepY = 0;
			dir_ing = dir;
			headRotation = 90;
			updateFlag = true;
		}
		break;
	case UP:
		stepY -= step;
		if (stepY <= -0.9999 || stepY >= 0.9999)
		{
			snakeY--;
			stepX = 0;
			stepY = 0;
			dir_ing = dir;
			headRotation = 0;
			updateFlag = true;
		}
		break;
	case DOWN:
		stepY += step;
		if (stepY <= -0.9999 || stepY >= 0.9999)
		{
			snakeY++;
			stepX = 0;
			stepY = 0;
			dir_ing = dir;
			headRotation = 180;
			updateFlag = true;
		}
		break;
	default:
		dir_ing = dir;
		break;
	}

	if (updateFlag)
	{
		if (snakeX == fruitX && snakeY == fruitY)
		{
			Eat.play();
			score += 10;
			fruitX = rand() % W + PosX;
			fruitY = rand() % H + PosY;
			nTail++;
		}

		//不能穿墙的蛇
		/*if (snakeX > W || snakeX < 0 || snakeY >H || snakeY < 0)
			{
			Die.setBuffer(bDie);
			Die.play();
			gameOver = true;
			}*/

			//不能碰蛇身
		for (int i = 1; i < nTail; i++)
		{
			if (tailX[i] == snakeX && tailY[i] == snakeY)
			{
				Die.setBuffer(bDie);
				Die.play();
				gameOver = true;
			}
		}

		//穿墙的蛇
		if (snakeX > W - 1 + PosX)
			snakeX = PosX;
		else if (snakeX < PosX)
			snakeX = W - 1 + PosX;
		if (snakeY > H - 1 + PosY)
			snakeY = PosY;
		else if (snakeY < PosY)
			snakeY = H - 1 + PosY;

		for (int i = nTail; i > 0; i--)
		{
			tailX[i] = tailX[i - 1];
			tailY[i] = tailY[i - 1];
		}
		tailX[0] = snakeX;
		tailY[0] = snakeY;
	}
}

void Input(RenderWindow& app)
{
	Event e;
	while (app.pollEvent(e))
	{
		if (e.type == Event::Closed)
			app.close();
		if (e.type == Event::EventType::KeyReleased && e.key.code == Keyboard::Space)
			isSilky = !isSilky;
	}

	if (Keyboard::isKeyPressed(Keyboard::W)&&dir!=DOWN)
		dir = UP;
	if (Keyboard::isKeyPressed(Keyboard::S)&&dir!=UP)
		dir = DOWN;
	if (Keyboard::isKeyPressed(Keyboard::A)&&dir!=RIGHT)
		dir = LEFT;
	if (Keyboard::isKeyPressed(Keyboard::D)&&dir!=LEFT)
		dir = RIGHT;
}

void DrawMap(RenderWindow& app)
{
	sBack.setPosition(0, 0);
	sBack.setScale(0.5, 0.6);
	app.draw(sBack);
	sRec1.setPosition(630, 60);
	sRec1.setScale(0.55, 0.7);
	app.draw(sRec1);

	for (int i = PosX; i < W + PosX; i++)
		for (int j = PosY; j < H + PosY; j++)
		{
			sBackground.setPosition(i * size, j * size);
			sBackground.setOrigin(25, 25);
			sBackground.setScale(scale, scale);
			app.draw(sBackground);
		}
}

void DrawFruit(RenderWindow& app)
{
	sFruit.setPosition(fruitX * size + dt_x, fruitY * size + dt_y);
	sFruit.setOrigin(25, 25);
	sFruit.setScale(scale, scale);
	app.draw(sFruit);
}

void DrawSnake(RenderWindow& app)
{
	sHead.setPosition(snakeX * size + dt_x, snakeY * size + dt_y);
	sHead.setOrigin(25, 25);
	sHead.setRotation(headRotation);
	sHead.setScale(scale, scale);
	app.draw(sHead);
	for (int i = 1; i <= nTail; i++)
	{
		sTail.setPosition(tailX[i] * size + dt_x, tailY[i] * size + dt_y);
		sTail.setOrigin(25, 25);
		sTail.setScale(scale, scale);
		app.draw(sTail);
	}
}

void DrawSnakeStep(RenderWindow& app)
{
	float stepLength = 0;
	stepLength = fabs(stepX + stepY);

	sHead.setPosition((snakeX + stepX) * size + dt_x, (snakeY + stepY) * size + dt_y);
	sHead.setOrigin(25, 25);
	sHead.setRotation(headRotation);
	sHead.setScale(scale, scale);
	app.draw(sHead);
	for (int i = 1; i <= nTail; i++)
	{
		if (tailX[i] == tailX[i - 1] && tailY[i] != tailY[i - 1])
			sTail.setPosition(tailX[i] * size + dt_x, (tailY[i] + (tailY[i - 1] - tailY[i]) * stepLength) * size + dt_y);
		if (tailX[i] != tailX[i - 1] && tailY[i] == tailY[i - 1])
			sTail.setPosition((tailX[i] + (tailX[i - 1] - tailX[i]) * stepLength) * size + dt_x, tailY[i] * size + dt_y);
		sTail.setOrigin(25, 25);
		sTail.setScale(scale, scale);
		app.draw(sTail);
	}
}

void GameOver(RenderWindow& app)
{
		t5.setString("GAME OVER!\n(Y-exit||N-replay)");
		t5.setCharacterSize(80);
		t5.setFillColor(Color::Red);
		t5.setPosition(60, 20);
		app.draw(t5);
		BGM.pause();
		if (Keyboard::isKeyPressed(Keyboard::Y))
			gameOver = true;
		else if (Keyboard::isKeyPressed(Keyboard::N))
			Initial();
}

void ShowMessage(RenderWindow& app)
{
	t1.setString("  Score:  @_@");
	t1.setCharacterSize(36);
	t1.setFillColor(Color::Blue);
	t1.setPosition(620, 100);
	t2.setString(std::to_string(score));
	t2.setCharacterSize(40);
	t2.setFillColor(Color::Red);
	t2.setPosition(670, 140);
	t3.setString("Tip------");
	t3.setCharacterSize(50);
	t3.setFillColor(Color::Yellow);
	t3.setPosition(640, 220);
	t4.setString("W:up\nS:down\nA:left\nD:right\nSpace:Change-\nanimation mode");
	t4.setCharacterSize(30);
	t4.setFillColor(Color::White);
	t4.setPosition(650, 280);
	app.draw(t1);
	app.draw(t2);
	app.draw(t3);
	app.draw(t4);
}

int main()
{
	RenderWindow app(VideoMode(840, 640), "Sanke by QJY");
	app.setFramerateLimit(60);
	int timer = 0;

	Initial();
	while (app.isOpen())
	{
		Input(app);
		DrawMap(app);
		DrawFruit(app);

		if (!gameOver)
		{
			if (!isSilky)
			{
				timer++;
				if (timer % 10 == 0)
				{
					Logic();
					timer = 0;
				}
				DrawSnake(app);
			}
			else
			{
				LogicStep(app);
				DrawSnakeStep(app);
			}
		}
		else
			GameOver(app);
		ShowMessage(app);

		app.display();
	}

	return 0;
}