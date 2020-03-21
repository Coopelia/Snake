#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<time.h>
using namespace sf;

class Snake
{
public:
	Snake() :x(0), y(0), rotation(0) {};
	int x, y;
	int rotation;
};

bool isPlay = true;
bool gameOver, isSilky, isPause,isAppearCoin;
const int W = 20;
const int H = 20;
const int PosX = 1;
const int PosY = 1;
const int size = 30;
const int dt_x = -2;
const int dt_y = -2;
const float scale = 0.6;
Snake sn[100];
int snakeX,snakeY,fruitX, fruitY, score;
int nTail;
int rank;
int fruitFlash;
int headRotation;
int vol;
float step = 0.1;
float stepX, stepY;
float coinX, coinY;
float elapsedTime;
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirection dir, dir_ing,dir_ed,dir_coin;
clock_t beginTime, stopTime;
Clock m_clock;
Texture tBack, tBackground, tHead, tTail, tFruit, tRec1,tCoin,tTail0;
Sprite sBack, sBackground, sSnake, sFruit, sRec1,sCoin;
Font font;
Text t1, t2, t3, t4, t5,t6;
Music BGM;
SoundBuffer bEat, bDie;
Sound Eat, Die;

void Initial()
{
	nTail = 2;
	rank = 1;
	elapsedTime = 0;
	vol = 100;
	gameOver = false;
	isSilky = false;
	isPause = false;
	isAppearCoin = false;
	dir = STOP;
	dir_ing = STOP;
	dir_ed = STOP;
	dir_coin = UP;
	sn[0].x=snakeX = PosX + W / 2;
	sn[0].y=snakeY = PosY + H / 2;
	sn[0].rotation=headRotation = 0;
	sn[1].x = PosX + W / 2;
	sn[1].y = PosY + H / 2 + 1;
	sn[1].rotation = 0;
	stepX = 0;
	stepY = 0;
	srand(time(0));
	fruitX = rand() % W + PosX;
	fruitY = rand() % H + PosY;
	coinX = rand() % W + PosX;
	coinY = rand() % H + PosY;
	score = 0;

	tBackground.loadFromFile("image/BK.png");
	tBack.loadFromFile("image/back.jpg");
	tHead.loadFromFile("image/sh01.png");
	tTail.loadFromFile("image/sb0201.png");
	tFruit.loadFromFile("image/sb0202.png");
	tRec1.loadFromFile("image/rec01.png");
	tCoin.loadFromFile("image/coin.png");
	tTail0.loadFromFile("image/re.png");
	sBackground.setTexture(tBackground);
	sBack.setTexture(tBack);
	sFruit.setTexture(tFruit);
	sRec1.setTexture(tRec1);
	sCoin.setTexture(tCoin);

	font.loadFromFile("font/oil.ttf");
	t1.setFont(font);
	t2.setFont(font);
	t3.setFont(font);
	t4.setFont(font);
	t5.setFont(font);
	t6.setFont(font);

	BGM.openFromFile("audios/BGM01.ogg");
	bEat.loadFromFile("audios/Eat01.ogg");
	bDie.loadFromFile("audios/Die01.ogg");
	Eat.setBuffer(bEat);
	Die.setBuffer(bDie);
	BGM.setLoop(true);
	BGM.setVolume(vol);
	BGM.play();
}

void Logic()
{	
	switch (dir)
	{
	case LEFT:
		if (dir_ed != RIGHT)
			dir_ed = dir;
		break;
	case RIGHT:
		if (dir_ed != LEFT)
			dir_ed = dir;
		break;
	case UP:
		if (dir_ed != DOWN)
			dir_ed = dir;
		break;
	case DOWN:
		if (dir_ed != UP)
			dir_ed = dir;
		break;
	default:
		break;
	}

	switch (dir_ed)
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

	if (sn[0].x == fruitX && sn[0].y == fruitY)
	{
		Eat.play();
		score += 10;
		fruitX = rand() % W + PosX;
		fruitY = rand() % H + PosY;
		nTail++;
	}

	//不能碰蛇身
	for (int i = 1; i < nTail; i++)
	{
		if (sn[i].x == sn[0].x && sn[i].y == sn[0].y)
		{
			Die.setBuffer(bDie);
			Die.play();
			gameOver = true;
		}
	}

	//穿墙
	if (snakeX > W - 1 + PosX)
		snakeX = PosX;
	else if (snakeX < PosX)
		snakeX = W - 1 + PosX;
	if (snakeY > H - 1 + PosY)
		snakeY = PosY;
	else if (snakeY < PosY)
		snakeY = H - 1 + PosY;

	if (dir != STOP)
		for (int i = nTail - 1; i > 0; i--)
			sn[i] = sn[i - 1];
	sn[0].x = snakeX;
	sn[0].y = snakeY;
	sn[0].rotation = headRotation;

	if(rank<10)
		rank = score / 50+1;
}

void LogicCoin()
{
	if (isAppearCoin)
	{
		switch (rand() % 100)
		{
		case 0:
			if(dir_coin!=DOWN)
				dir_coin = UP;
			break;
		case 25:
			if (dir_coin != UP)
				dir_coin = DOWN;
			break;
		case 50:
			if (dir_coin != RIGHT)
				dir_coin = LEFT;
			break;
		case 75:
			if (dir_coin != LEFT)
				dir_coin = RIGHT;
			break;
		default:
			break;
		}

		switch (dir_coin)
		{
		case LEFT:
			coinX -= 0.1;
			break;
		case RIGHT:
			coinX += 0.1;
			break;
		case UP:
			coinY -= 0.1;
			break;
		case DOWN:
			coinY + 0.1;
			break;
		default:
			break;
		}
	}
	else
	{
		if (rand() % (25 * rank) == 0)
		{
			isAppearCoin = true;
			coinX = rand() % W + PosX;
			coinY = rand() % H + PosY;
		}
	}

	if (coinX > W - 1 + PosX)
		coinX = PosX;
	else if (coinX < PosX)
		coinX = W - 1 + PosX;
	if (coinY > H - 1 + PosY)
		coinY = PosY;
	else if (coinY < PosY)
		coinY = H - 1 + PosY;

	if (fabs(sn[0].x - coinX)<0.5 && fabs(sn[0].y - coinY)<0.5)
	{
		Eat.play();
		score += 50;
		isAppearCoin = false;
	}
}

void LogicStep()
{
	switch (dir)
	{
	case LEFT:
		if (dir_ing != RIGHT)
			dir_ed = dir;
		break;
	case RIGHT:
		if (dir_ing != LEFT)
			dir_ed = dir;
		break;
	case UP:
		if (dir_ing != DOWN)
			dir_ed = dir;
		break;
	case DOWN:
		if (dir_ing != UP)
			dir_ed = dir;
		break;
	default:
		break;
	}

	step = 1.0/ (11 - rank);
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
			dir_ing = dir_ed;
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
			dir_ing = dir_ed;
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
			dir_ing = dir_ed;
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
			dir_ing = dir_ed;
			headRotation = 180;
			updateFlag = true;
		}
		break;
	default:
		dir_ing = dir_ed;
		break;
	}

	if (updateFlag)
	{
		if (sn[0].x == fruitX && sn[0].y == fruitY)
		{
			Eat.play();
			score += 10;
			fruitX = rand() % W + PosX;
			fruitY = rand() % H + PosY;
			nTail++;
		}

		//不能碰蛇身
		for (int i = 1; i < nTail; i++)
		{
			if (sn[i].x == sn[0].x && sn[i].y == sn[0].y)
			{
				Die.setBuffer(bDie);
				Die.play();
				gameOver = true;
			}
		}

		//穿墙
		if (snakeX > W - 1 + PosX)
			snakeX = PosX;
		else if (snakeX < PosX)
			snakeX = W - 1 + PosX;
		if (snakeY > H - 1 + PosY)
			snakeY = PosY;
		else if (snakeY < PosY)
			snakeY = H - 1 + PosY;

		if(dir!=STOP)
			for (int i = nTail - 1; i > 0; i--)
				sn[i] = sn[i - 1];
		sn[0].x = snakeX;
		sn[0].y = snakeY;
		sn[0].rotation = headRotation;
	}

	if (rank < 10)
		rank = score / 50 + 1;
}

void Input(RenderWindow& app, Event& e)
{
	while (app.pollEvent(e))
	{
		if (e.type == Event::Closed)
			app.close();
		if (e.type == Event::EventType::KeyReleased && e.key.code == Keyboard::Space)
			isSilky = !isSilky;
		if (e.type == Event::EventType::KeyReleased && e.key.code == Keyboard::P)
			isPause = !isPause;
		if (e.type == Event::EventType::KeyReleased && e.key.code == Keyboard::X)
			gameOver = true;
		if (e.type==Event::EventType::KeyReleased&&e.key.code==Keyboard::V)
		{
			isPlay = !isPlay;
			if (isPlay)
				BGM.play();
			else
				BGM.pause();
		}
	}

	if (Keyboard::isKeyPressed(Keyboard::PageUp))
	{
		vol+=10;
		if (vol >= 100)
			vol = 100;
		BGM.setVolume(vol);
	}
	else if (Keyboard::isKeyPressed(Keyboard::PageDown))
	{
		vol -= 10;
		if (vol <= 0)
			vol = 0;
		BGM.setVolume(vol);
	}

	if (!isPause)
	{
		if (Keyboard::isKeyPressed(Keyboard::W))
			dir = UP;
		if (Keyboard::isKeyPressed(Keyboard::S))
			dir = DOWN;
		if (Keyboard::isKeyPressed(Keyboard::A))
			dir = LEFT;
		if (Keyboard::isKeyPressed(Keyboard::D))
			dir = RIGHT;
	}	
}

void DrawMap(RenderWindow& app)
{
	sBack.setPosition(0, 0);
	sBack.setScale(0.5, 0.6);
	app.draw(sBack);
	sRec1.setPosition(630, 60);
	sRec1.setScale(0.85, 0.72);
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
	stopTime = clock();
	if (stopTime - beginTime >= 500) //0.5秒为一拍
	{
		beginTime = clock();
		(fruitFlash %= 5)++;
	}
	if (fruitFlash != 4)
	{
		sFruit.setPosition(fruitX * size + dt_x, fruitY * size + dt_y);
		sFruit.setOrigin(25, 25);
		sFruit.setScale(scale, scale);
		app.draw(sFruit);
	}
	if (isAppearCoin)
	{
		elapsedTime += m_clock.restart().asSeconds();
		int animFrame = static_cast<int>(elapsedTime * 6) % 6;
		sCoin.setTextureRect(IntRect(animFrame*64 , 0, 64, 64));
		sCoin.setPosition(coinX*size+dt_x, coinY*size+dt_y);
		sCoin.setOrigin(32, 32);
		sCoin.setScale(scale, scale);
		app.draw(sCoin);
	}
}

void DrawSnake(RenderWindow& app)
{
	for (int i = 0; i < nTail; i++)
	{
		if (i == 0)
			sSnake.setTexture(tHead);
		else if (i==nTail-1)
			sSnake.setTexture(tTail0);
		else
			sSnake.setTexture(tTail);
		sSnake.setRotation(sn[i].rotation);
		sSnake.setPosition(sn[i].x * size + dt_x, sn[i].y * size + dt_y);
		sSnake.setOrigin(25, 25);
		sSnake.setScale(scale, scale);
		app.draw(sSnake);
	}
}

void DrawSnakeStep(RenderWindow& app)
{
	float stepLength = 0;
	stepLength = fabs(stepX + stepY);

	sSnake.setTexture(tHead);
	sSnake.setPosition((snakeX + stepX) * size + dt_x, (snakeY + stepY) * size + dt_y);
	sSnake.setOrigin(25, 25);
	sSnake.setRotation(headRotation);
	sSnake.setScale(scale, scale);
	app.draw(sSnake);

	for (int i = 1; i < nTail; i++)
	{
		if (i == nTail - 1)
			sSnake.setTexture(tTail0);
		else
			sSnake.setTexture(tTail);
		if (sn[i].x == sn[i - 1].x && sn[i].y != sn[i - 1].y)
			sSnake.setPosition(sn[i].x * size + dt_x, (sn[i].y + (sn[i - 1].y - sn[i].y) * stepLength) * size + dt_y);
		if (sn[i].x != sn[i - 1].x && sn[i].y == sn[i - 1].y)
			sSnake.setPosition((sn[i].x + (sn[i - 1].x - sn[i].x) * stepLength) * size + dt_x, sn[i].y * size + dt_y);
		sSnake.setOrigin(25, 25);
		sSnake.setScale(scale, scale);
		app.draw(sSnake);
	}
}

void GameOver(RenderWindow& app)
{
	t5.setString(L"GAME OVER!\n(Y-退出||N-重来)");
	t5.setCharacterSize(80);
	t5.setFillColor(Color::Red);
	t5.setPosition(60, 20);
	app.draw(t5);
	BGM.pause();
	if (Keyboard::isKeyPressed(Keyboard::Y))
		app.close();
	else if (Keyboard::isKeyPressed(Keyboard::N))
		Initial();
}

void ShowMessage(RenderWindow& app)
{
	t1.setString(L"得分: \n\n等级: "+std::to_string(rank));
	t1.setCharacterSize(36);
	t1.setFillColor(Color::Blue);
	t1.setPosition(640, 100);
	t2.setString(std::to_string(score));
	t2.setCharacterSize(40);
	t2.setFillColor(Color::Red);
	t2.setPosition(720, 100);
	t3.setString(L" ------Tip------\n\n\n\n\n\n\n吃到金币额外加分");
	t3.setCharacterSize(40);
	t3.setFillColor(Color::Yellow);
	t3.setPosition(640, 220);
	t4.setString(L"W:  向上  S:  向下\nA:  向左  D:  向右\nPageUp::音量+\nPageDown::音量-\nV:BGM开关\nSpace: 改变动画模式\nP: 暂停/继续\nX: 退出");
	t4.setCharacterSize(30);
	t4.setFillColor(Color::White);
	t4.setPosition(650, 275);
	app.draw(t1);
	app.draw(t2);
	app.draw(t3);
	app.draw(t4);
	if (isPause)
	{
		sRec1.setPosition(190, 240);
		sRec1.setScale(0.6, 0.1);
		app.draw(sRec1);
		t6.setString("-PAUSE-");
		t6.setCharacterSize(56);
		t6.setFillColor(Color::Blue);
		t6.setPosition(200, 240);
		app.draw(t6);
	}
}

int main()
{
	RenderWindow app(VideoMode(960, 640), "Sanke by QJY");
	app.setFramerateLimit(60);
	int timer = 0, timer0 = 0;;

	Initial();
	while (app.isOpen())
	{
		Event e;
		Input(app,e);
		DrawMap(app);
		DrawFruit(app);
		if (!gameOver)
		{
			timer0++;
			if (timer0 % 5 == 0)
			{
				if (!isPause)
					LogicCoin();
				timer0 = 0;
			}
			if (!isSilky)
			{
				timer++;
				if (timer % (11-rank) == 0)
				{
					if(!isPause)
						Logic();
					timer = 0;
				}
				DrawSnake(app);
			}
			else
			{
				if(!isPause)
					LogicStep();
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