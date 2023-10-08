#include <DxLib.h>
#include <stdlib.h>
#include <time.h>

#define MARIO_SIZE 32
#define FBALL_SIZE 16
#define PAKUN_SIZE 64
#define BLOCK_SIZE 64
#define DEATH_SIZE 240
#define PIXEL_SIZE 16
#define IGNITION_SIZE 476

#define JUST_NUM 256
#define ROTATE_NUM 4
#define BLOCK_NUM 8

#define WINX 640
#define WINY 480

int key[JUST_NUM] = { 0 };
char charKey[JUST_NUM] = { 0 };
bool pushFlag[JUST_NUM] = { 0 };

void GetKey()
{
    GetHitKeyStateAll(charKey);
    for (int i = 0; i < JUST_NUM; i++)
    {
        if (charKey[i] != 0)
        {
            key[i]++;
        }
        else
        {
            key[i] = 0;
        }
    }
}

bool isPush(int keyIndex)
{
    if (key[keyIndex] && !pushFlag[keyIndex])
    {
        pushFlag[keyIndex] = true;
        return true;
    }
    if (!key[keyIndex])
    {
        pushFlag[keyIndex] = false;
    }
    return false;
}

class System
{
public:
    int x, x1, x2;
    int y, y1, y2;
    int pixel;

    enum ColorBit { N, B, R, G, O, Y, P, W, };

    struct ColorValue
    {
        int black = GetColor(0, 0, 0);
        int red = GetColor(255, 0, 0);
        int green = GetColor(0, 255, 0);
        int orange = GetColor(255, 165, 0);
        int yellow = GetColor(255, 240, 80);
        int pink = GetColor(240, 180, 190);
        int white = GetColor(255, 255, 255);
    };

    struct ColorValue* c = new ColorValue;

    void SetPixel(int i, int j)
    {
        switch (pixel)
        {
            case B:DrawPixel(x1 + j, y1 + i, c->black);  break;
            case R:DrawPixel(x1 + j, y1 + i, c->red);    break;
            case G:DrawPixel(x1 + j, y1 + i, c->green);  break;
            case O:DrawPixel(x1 + j, y1 + i, c->orange); break;
            case Y:DrawPixel(x1 + j, y1 + i, c->yellow); break;
            case P:DrawPixel(x1 + j, y1 + i, c->pink);   break;
            case W:DrawPixel(x1 + j, y1 + i, c->white);  break;
            default:break;
        }
    }
};

class Object : public System
{
public:
    virtual void SetPos() = 0;
    virtual void Action() = 0;
    virtual void Draw() = 0;
};

class Mario : public Object
{
public:
int MarioPic[ROTATE_NUM][PIXEL_SIZE][PIXEL_SIZE] =
{{
{N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N},
{N, N, N, N, N, B, B, B, B, B, B, N, N, N, N, N},
{N, N, N, N, B, R, R, R, R, R, R, B, B, N, N, N},
{N, N, N, B, R, R, R, R, R, R, R, R, R, B, N, N},
{N, N, N, B, B, B, Y, Y, B, Y, B, B, B, N, N, N},
{N, N, B, Y, Y, B, B, Y, B, Y, Y, Y, Y, B, N, N},
{N, N, B, Y, Y, B, B, Y, Y, B, Y, Y, Y, B, N, N},
{N, N, N, B, B, Y, Y, Y, B, B, B, B, B, N, N, N},
{N, N, N, N, B, B, Y, Y, Y, Y, Y, B, N, N, N, N},
{N, N, N, B, R, R, B, B, R, R, B, N, N, N, N, N},
{N, N, B, R, R, R, R, B, B, R, R, B, N, N, N, N},
{N, N, B, R, R, R, R, B, B, B, B, B, N, N, N, N},
{N, N, N, B, Y, Y, Y, B, B, Y, B, B, N, N, N, N},
{N, N, N, B, Y, Y, R, R, R, B, B, B, N, N, N, N},
{N, N, N, N, B, R, R, R, R, R, B, N, N, N, N, N},
{N, N, N, N, B, B, B, B, B, B, B, N, N, N, N, N},
},{
{N, N, N, N, N, B, B, B, B, B, N, N, B, B, B, N},
{N, N, N, N, B, R, R, R, R, R, B, B, Y, Y, Y, B},
{N, N, N, B, R, R, R, R, R, R, R, R, B, Y, Y, B},
{N, N, N, B, B, B, Y, Y, B, Y, B, B, B, R, B, N},
{N, N, B, Y, Y, B, B, Y, B, Y, Y, Y, Y, R, B, N},
{N, N, B, Y, Y, B, B, Y, Y, B, Y, Y, Y, R, B, N},
{N, N, N, B, B, Y, Y, Y, B, B, B, B, B, B, N, N},
{N, N, N, N, B, B, Y, Y, Y, Y, Y, Y, Y, B, N, N},
{N, N, B, R, R, R, B, R, R, R, B, R, B, N, N, N},
{N, B, B, B, R, R, R, B, R, R, R, B, B, B, B, N},
{B, Y, Y, Y, B, R, R, B, Y, B, B, Y, B, R, R, B},
{B, Y, Y, Y, B, R, B, B, B, B, B, B, R, R, R, B},
{N, B, R, B, B, B, B, B, B, B, B, B, R, R, B, N},
{B, R, R, R, B, B, B, B, B, B, B, B, R, R, B, N},
{B, R, R, B, B, B, B, B, B, N, N, N, B, B, N, N},
{N, B, B, N, B, B, B, N, N, N, N, N, N, N, N, N},
},{
{N, B, B, B, N, N, B, B, B, B, B, N, N, N, N, N},
{B, Y, Y, Y, B, B, R, R, R, R, R, B, N, N, N, N},
{B, Y, Y, B, R, R, R, R, R, R, R, R, B, N, N, N},
{N, B, R, B, B, B, Y, B, Y, Y, B, B, B, N, N, N},
{N, B, R, Y, Y, Y, Y, B, Y, B, B, Y, Y, B, N, N},
{N, B, R, Y, Y, Y, B, Y, Y, B, B, Y, Y, B, N, N},
{N, N, B, B, B, B, B, B, Y, Y, Y, B, B, N, N, N},
{N, N, B, Y, Y, Y, Y, Y, Y, Y, B, B, N, N, N, N},
{N, N, N, B, R, B, R, R, R, B, R, R, R, B, N, N},
{N, B, B, B, B, R, R, R, B, R, R, R, B, B, B, N},
{B, R, R, B, Y, B, B, Y, B, R, R, B, Y, Y, Y, B},
{B, R, R, R, B, B, B, B, B, B, R, B, Y, Y, Y, B},
{N, B, R, R, B, B, B, B, B, B, B, B, B, R, B, N},
{N, B, R, R, B, B, B, B, B, B, B, B, R, R, R, B},
{N, N, B, B, N, N, N, B, B, B, B, B, B, R, R, B},
{N, N, N, N, N, N, N, N, N, B, B, B, N, B, B, N},
},{
{N, N, N, N, N, N, B, B, B, B, N, N, N, N, N, N},
{N, N, N, B, N, B, R, R, R, R, B, N, B, N, N, N},
{N, B, B, Y, B, R, R, R, R, R, R, B, Y, B, B, N},
{B, Y, Y, Y, B, Y, B, Y, Y, B, Y, B, Y, Y, Y, B},
{B, Y, Y, R, Y, Y, B, Y, Y, B, Y, Y, R, Y, Y, B},
{B, Y, R, R, B, B, Y, Y, Y, Y, B, B, R, R, Y, B},
{N, B, B, R, Y, B, Y, Y, Y, Y, B, Y, R, B, B, N},
{N, N, N, B, Y, Y, B, B, B, B, Y, Y, B, N, N, N},
{N, N, N, B, B, Y, Y, B, B, Y, Y, B, B, N, N, N},
{N, N, B, B, B, B, Y, Y, Y, Y, B, B, B, B, N, N},
{N, B, R, R, B, B, R, R, R, R, B, B, R, R, B, N},
{N, B, R, R, R, B, B, R, R, B, B, R, R, R, B, N},
{N, B, R, R, R, B, Y, B, B, Y, B, R, R, R, B, N},
{N, B, R, R, R, B, B, B, B, B, B, R, R, R, B, N},
{N, N, B, R, R, B, B, B, B, B, B, R, R, B, N, N},
{N, N, N, B, B, N, N, N, N, N, N, B, B, N, N, N},
}};

    enum DIR
    {
        STAND,
        RIGHT,
        LEFT,
        DEATH,
    };

    int speed, gspeed, dir, dy;
    bool move, stand, death;

    Mario()
    {
        dir = DIR::STAND;
        speed = 5;
        gspeed = 0;
        move = death = false;
        stand = true;
        x = WINX / 2;
        y = dy = WINY / 2;
        SetPos();
    }

    void SetPos() override
    {
        x1 = x - MARIO_SIZE / 2;
        x2 = x + MARIO_SIZE / 2;
        y1 = y - MARIO_SIZE / 2;
        y2 = y + MARIO_SIZE / 2;
    }

    void Action() override
    {
        SetPos();
        if (key[KEY_INPUT_RIGHT] && gspeed && x2 < WINX)
        {
            x += speed;
            dir = DIR::RIGHT;
            move = true;
        }
        if (key[KEY_INPUT_LEFT] && gspeed && 0 < x1)
        {
            x -= speed;
            dir = DIR::LEFT;
            move = true;
        }
        if (isPush(KEY_INPUT_LSHIFT))
        {
            gspeed = -10;
            dy = y;
            stand = false;
        }
        if (dy - y > 0)
        {
            gspeed++;
        }
        if (!stand && !move)
        {
            dir = DIR::RIGHT;
        }
        if (death)
        {
            dir = DIR::DEATH;
        }
        y += gspeed;
    }

    void Draw() override
    {
		int k = MARIO_SIZE / PIXEL_SIZE;
        for (int i = 0; i < MARIO_SIZE; i++)
        {
            for (int j = 0; j < MARIO_SIZE; j++)
            {
                pixel = MarioPic[dir][i / k][j / k];
                SetPixel(i, j);
            }
        }
    }
};

class Pakun : public Object
{
public:
int PakunPic[ROTATE_NUM][PIXEL_SIZE][PIXEL_SIZE] =
{{
{N, N, N, N, N, N, B, G, G, B, N, N, N, N, N, N},
{N, N, N, N, B, B, B, B, B, B, B, B, N, N, N, N},
{N, N, B, B, R, R, R, W, W, R, R, R, B, B, N, N},
{N, B, R, W, R, R, R, R, R, R, R, R, W, R, B, N},
{N, B, R, R, R, R, R, R, R, R, R, R, R, R, B, N},
{B, W, R, R, R, R, W, R, R, W, R, R, R, R, W, B},
{B, R, R, R, R, R, B, B, B, B, R, R, R, R, R, B},
{B, R, W, R, R, B, W, W, W, W, B, R, R, W, R, B},
{N, B, R, R, B, W, W, W, W, W, W, B, R, R, B, N},
{N, B, R, B, W, W, W, B, B, W, W, W, B, R, B, N},
{N, N, B, W, W, W, B, W, W, B, W, W, W, B, N, N},
{N, B, W, W, W, B, B, B, B, B, B, W, W, W, B, N},
{B, W, W, W, W, B, W, B, B, W, B, W, W, W, W, B},
{B, W, W, W, B, B, B, N, N, B, B, B, W, W, W, B},
{B, W, W, B, W, B, N, N, N, N, B, W, B, W, W, B},
{N, B, B, B, B, N, N, N, N, N, N, B, B, B, B, N},
},{
{N, B, B, B, B, N, N, N ,N, N, N, B, B, B, B, N},
{B, W, W, B, W, B, N, N, N, N, B, W, B, W, W, B},
{B, W, W, W, B, B, B, N, N, B, B, B, W, W, W, B},
{B, W, W, W, W, B, W, B, B, W, B, W, W, W, W, B},
{N, B, W, W, W, B, B, B, B, B, B, W, W, W, B, N},
{N, N, B, W, W, W, B, W, W, B, W, W, W, B, N, N},
{N, B, R, B, W, W, W, B, B, W, W, W, B, R, B, N},
{N, B, R, R, B, W, W, W, W, W, W, B, R, R, B, N},
{B, R, W, R, R, B, W, W, W, W, B, R, R, W, R, B},
{B, R, R, R, R, R, B, B, B, B, R, R, R, R, R, B},
{B, W, R, R, R, R, W, R, R, W, R, R, R, R, W, B},
{N, B, R, R, R, R, R, R, R, R, R, R, R, R, B, N},
{N, B, R, W, R, R, R, R, R, R, R, R, W, R, B, N},
{N, N, B, B, R, R, R, W, W, R, R, R, B, B, N, N},
{N, N, N, N, B, B, B, B, B, B, B, B, N, N, N, N},
{N, N, N, N, N, N, B, G, G, B, N, N, N, N, N, N},
},{
{N, N, N, N, N, B, B, B, B, B, B, N, N, N, N, N},
{N, N, N, B, B, R, R, W, W, R, R, B, B, N, N, N},
{N, N, B, R, W, R, R, R, R, R, R, W, R, B, N, N},
{N, N, B, R, R, R, W, R, R, W, R, R, R, B, N, N},
{N, B, R, R, R, R, B, B, B, B, R, R, R, R, B, N},
{N, B, W, R, R, B, W, W, W, W, B, R, R, W, B, N},
{N, B, R, R, R, B, W, W, W, W, B, R, R, R, B, N},
{N, B, R, W, B, W, W, W, W, W, W, B, W, R, B, N},
{N, N, B, R, B, W, W, B, B, W, W, B, R, B, N, N},
{N, N, B, R, B, W, W, B, B, W, W, B, R, B, N, N},
{N, N, N, B, W, W, W, B, B, W, W, W, B, N, N, N},
{N, N, N, B, W, W, W, B, B, W, W, W, B, N, N, N},
{N, N, B, W, W, W, W, B, B, W, W, W, W, B, N, N},
{N, N, B, W, W, W, W, B, B, W, W, W, W, B, N, N},
{N, N, B, W, W, W, B, B, B, B, W, W, W, B, N, N},
{N, N, N, B, B, B, B, N, N, B, B, B, B, N, N, N},
},{
{N, N, N, B, B, B, B, N, N, B, B, B, B, N, N, N},
{N, N, B, W, W, W, B, B, B, B, W, W, W, B, N, N},
{N, N, B, W, W, W, W, B, B, W, W, W, W, B, N, N},
{N, N, B, W, W, W, W, B, B, W, W, W, W, B, N, N},
{N, N, N, B, W, W, W, B, B, W, W, W, B, N, N, N},
{N, N, N, B, W, W, W, B, B, W, W, W, B, N, N, N},
{N, N, B, R, B, W, W, B, B, W, W, B, R, B, N, N},
{N, N, B, R, B, W, W, B, B, W, W, B, R, B, N, N},
{N, B, R, W, B, W, W, W, W, W, W, B, W, R, B, N},
{N, B, R, R, R, B, W, W, W, W, B, R, R, R, B, N},
{N, B, W, R, R, B, W, W, W, W, B, R, R, W, B, N},
{N, B, R, R, R, R, B, B, B, B, R, R, R, R, B, N},
{N, N, B, R, R, R, W, R, R, W, R, R, R, B, N, N},
{N, N, B, R, W, R, R, R, R, R, R, W, R, B, N, N},
{N, N, N, B, B, R, R, W, W, R, R, B, B, N, N, N},
{N, N, N, N, N, B, B, B, B, B, B, N, N, N, N, N},
}};

    Mario* mario;
    int clock, tick, type, eat;

    Pakun(int i, Mario* mario)
    {
		this->mario = mario;
        type = i;
        eat = clock = 0;
        tick = 5;
		x = WINX / 2;
		y = PAKUN_SIZE / 2 + type * (WINY - PAKUN_SIZE);
    }

	void SetPos() override
    {
		x = mario->x;
        x1 = x - PAKUN_SIZE / 2;
        x2 = x + PAKUN_SIZE / 2;
        y1 = y - PAKUN_SIZE / 2;
        y2 = y + PAKUN_SIZE / 2;
    }

	void Action() override
	{
        SetPos();
        if      (clock < 1 * tick) eat = 2;
		else if (clock < 2 * tick) eat = 0;
		else clock = 0;
		clock++;
	}

	void Draw() override
	{
		int k = PAKUN_SIZE / PIXEL_SIZE;
		for (int i = 0; i < PAKUN_SIZE; i++)
		{
			for (int j = 0; j < PAKUN_SIZE; j++)
			{
				pixel = PakunPic[type + eat][i / k][j / k];
				SetPixel(i, j);
			}
		}
	}
};

class PakunManager
{
public:
	Mario* mario;

    Pakun* pakun[2];

    PakunManager(Mario* mario)
    {
		this->mario = mario;
        for (int i = 0 ; i < 2; i++)
        {
            pakun[i] = new Pakun(i, mario);
        }
    }

    ~PakunManager()
    {
        for (int i = 0; i < 2; i++)
        {
            delete pakun[i];
        }
    }

	void Collision()
	{
		if (mario->y2 < pakun[0]->y2 || pakun[1]->y1 < mario->y1)
		{
			mario->death = true;
		}
	}

    void Action()
    {
        for (int i = 0; i < 2; i++)
        {
            pakun[i]->Action();
        }
		Collision();
    }

    void Draw()
    {
        for (int i = 0; i < 2; i++)
        {
            pakun[i]->Draw();
        }
    }
};

class FBall : public Object
{
public:
int FBallPic[ROTATE_NUM][PIXEL_SIZE / 2][PIXEL_SIZE / 2] =
{{
{N, N, R, R, R, R, N, N},
{N, R, W, W, W, R, R, N},
{R, W, W, W, W, W, R, N},
{R, W, W, W, W, R, R, R},
{R, W, W, W, R, N, N, R},
{R, R, W, W, R, N, R, N},
{N, R, R, R, N, N, N, R},
{N, N, N, N, N, R, N, N},
},{
{N, N, N, R, R, N, R, N},
{N, R, R, R, N, R, N, N},
{R, R, W, R, N, N, N, R},
{R, W, W, W, R, R, N, N},
{R, W, W, W, W, W, R, N},
{R, W, W, W, W, W, R, N},
{N, R, W, W, W, R, R, N},
{N, N, R, R, R, R, N, N},
},{
{N, N, R, N, N, N, N, N},
{R, N, N, N, R, R, R, N},
{N, R, N, R, W, W, R, R},
{R, N, N, R, W, W, W, R},
{R, R, R, W, W, W, W, R},
{N, R, W, W, W, W, W, R},
{N, R, R, W, W, W, R, N},
{N, N, R, R, R, R, N, N},
},{
{N, N, R, N, N, N, N, N},
{R, N, N, N, R, R, R, N},
{N, R, N, R, W, W, R, R},
{R, N, N, R, W, W, W, R},
{R, R, R, W, W, W, W, R},
{N, R, W, W, W, W, W, R},
{N, R, R, W, W, W, R, N},
{N, N, R, R, R, R, N, N},
}};

	Mario* mario;

	int clock, tick, speed, rotate;

	FBall(Mario* mario)
	{
		this->mario = mario;
		speed = rand() % 3 + 1;
        y = rand() % 312 + 84;
		x = WINX - FBALL_SIZE;
		clock = rotate = 0;
		tick = 4;
	}

	void SetPos() override
	{
        x1 = x - FBALL_SIZE / 2;
        x2 = x + FBALL_SIZE / 2;
        y1 = y - FBALL_SIZE / 2;
        y2 = y + FBALL_SIZE / 2;
	}

	void Action() override
	{
        SetPos();
		x -= speed;
		if      (clock < 1 * tick) rotate = 0;
		else if (clock < 2 * tick) rotate = 1;
		else if (clock < 3 * tick) rotate = 2;
		else if (clock < 4 * tick) rotate = 3;
		else clock = 0;
		clock++;
	}

	void Draw() override
	{
		int k = FBALL_SIZE / (PIXEL_SIZE / 2);
		for (int i = 0; i < FBALL_SIZE; i++)
		{
			for (int j = 0; j < FBALL_SIZE; j++)
			{
				pixel = FBallPic[rotate][i / k][j / k];
				SetPixel(i, j);
			}
		}
	}
	
	bool Collision()
	{
        if (mario->x2 < x1)return false;
		if (mario->y2 < y1)return false;
        if (x2 < mario->x1)return false;
        if (y2 < mario->y1)return false;
		return true;
	}

    bool Ignition()
    {
        if (x1 < BLOCK_SIZE)return true;
        return false;
    }
};

class FBallManager
{
public:
	Mario* mario;
	FBall* fball[JUST_NUM];

	int clock, tick, score;
	bool ShotSpeedUp;

	FBallManager(Mario* mario)
	{
		this->mario = mario;
		for (int i = 0; i < JUST_NUM; i++)
		{
			fball[i] = NULL;
		}
		fball[0] = new FBall(mario);
        fball[0]->speed = 2;
        fball[0]->y = 200;
		clock = score = 0;
		tick = 50;
		ShotSpeedUp = false;
	}

	~FBallManager()
	{
		for (int i = 0; i < JUST_NUM; i++)
		{
			delete fball[i];
		}
	}

    int Score()
    {
        return score;
    }

	void Shot()
	{
		for (int i = 0; i < JUST_NUM; i++)
		{
			if (fball[i] == NULL)
			{
				fball[i] = new FBall(mario);
				break;
			}
		}
	}

	void Action()
	{
		if (tick < clock)
        {
            Shot();
            clock = 0;
        }
        if (0 < score && score % 3 == 0 && !ShotSpeedUp)
        {
            tick--;
            ShotSpeedUp = true;
        }
        if (score % 3 == 2)
        {
            ShotSpeedUp = false;
        }
		for (int i = 0; i < JUST_NUM; i++)
		{
			if (fball[i] != NULL)
			{
				fball[i]->Action();
				if (fball[i]->Collision())
				{
					fball[i] = NULL;
					score++;
                    break;
				}
                if (fball[i]->Ignition())
                {
                    mario->death = true;
                    break;
                }
			}
		}
        clock++;
	}

    void Draw()
    {
        for (int i = 0; i < JUST_NUM; i++)
        {
            if (fball[i] != NULL)
            {
                fball[i]->Draw();
            }
        }
    }
};

class Block : public Object
{
public:
int BlockPic[PIXEL_SIZE][PIXEL_SIZE] =
{
{R, R, R, R, R, R, R, R, R, R, R, R, R, R, R, R},
{R, R, R, R, R, R, R, R, R, R, R, R, R, R, R, R},
{R, R, R, R, R, R, R, R, R, R, R, R, R, R, R, R},
{R, R, R, R, R, R, R, R, R, R, R, R, R, R, R, R},
{W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W},
{W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W},
{W, W, B, B, B, W, B, W, W, B, W, B, B, B, W, W},
{W, W, W, B, W, W, B, B, W, B, W, W, B, W, W, W},
{W, W, W, B, W, W, B, W, B, B, W, W, B, W, W, W},
{W, W, W, B, W, W, B, W, W, B, W, W, B, W, W, W},
{W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W},
{W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W},
{R, R, R, R, R, R, R, R, R, R, R, R, R, R, R, R},
{R, R, R, R, R, R, R, R, R, R, R, R, R, R, R, R},
{R, R, R, R, R, R, R, R, R, R, R, R, R, R, R, R},
{R, R, R, R, R, R, R, R, R, R, R, R, R, R, R, R},
};

    Mario* mario;

    Block(int i, Mario* mario)
    {
        this->mario = mario;
        x = BLOCK_SIZE / 2;
        y = BLOCK_SIZE / 2 + i * BLOCK_SIZE;
    }

    void SetPos() override
    {
        x1 = x - BLOCK_SIZE / 2;
        x2 = x + BLOCK_SIZE / 2;
        y1 = y - BLOCK_SIZE / 2;
        y2 = y + BLOCK_SIZE / 2;
    }

    void Action() override
    {
        SetPos();
        if (mario->x1 < x2)
        {
            mario->death = true;
        }
    }

    void Draw() override
    {
        int k = BLOCK_SIZE / PIXEL_SIZE;
        for (int i = 0; i < BLOCK_SIZE; i++)
        {
            for (int j = 0; j < BLOCK_SIZE; j++)
            {
                pixel = BlockPic[i / k][j / k];
                SetPixel(i, j);
            }
        }
    }
};

class BlockManager
{
public:
    Block* block[BLOCK_NUM];

    BlockManager(Mario* mario)
    {
        for (int i = 0; i < BLOCK_NUM; i++)
        {
            block[i] = new Block(i, mario);
        }
    }

    ~BlockManager()
    {
        for (int i = 0; i < BLOCK_NUM; i++)
        {
            delete block[i];
        }
    }

    void Action()
    {
        for (int i = 0; i < BLOCK_NUM; i++)
        {
            block[i]->Action();
        }
    }

    void Draw()
    {
        for (int i = 0; i < BLOCK_NUM; i++)
        {
            block[i]->Draw();
        }
    }
};

class GameControl : public System
{
public:
int IgnitionPic[PIXEL_SIZE + 1][PIXEL_SIZE + 1] =
{
{N, N, N, N, N, N, N, N, B, N, N, N, N, N, N, N, N},
{N, B, N, N, N, N, N, N, B, N, N, N, N, N, N, B, N},
{N, N, B, B, N, N, N, B, R, B, N, N, N, B, B, N, N},
{N, N, B, R, B, B, B, R, R, R, B, B, B, R, B, N, N},
{N, N, N, B, R, R, R, R, O, R, R, R, R, B, N, N, N},
{N, N, N, B, R, R, O, O, O, O, O, R, R, B, N, N, N},
{N, N, N, B, R, O, O, O, Y, O, O, O, R, B, N, N, N},
{N, N, B, R, R, O, O, Y, Y, Y, O, O, R, R, B, N, N},
{B, B, R, R, O, O, Y, Y, Y, Y, Y, O, O, R, R, B, B},
{N, N, B, R, R, O, O, Y, Y, Y, O, O, R, R, B, N, N},
{N, N, N, B, R, O, O, O, Y, O, O, O, R, B, N, N, N},
{N, N, N, B, R, R, O, O, O, O, O, R, R, B, N, N, N},
{N, N, N, B, R, R, R, R, O, R, R, R, R, B, N, N, N},
{N, N, B, R, B, B, B, R, R, R, B, B, B, R, B, N, N},
{N, N, B, B, N, N, N, B, R, B, N, N, N, B, B, N, N},
{N, B, N, N, N, N, N, N, B, N, N, N, N, N, N, B, N},
{N, N, N, N, N, N, N, N, B, N, N, N, N, N, N, N, N},
};

    Mario* mario;
    PakunManager* pakun;
    FBallManager* fball;
    BlockManager* block;

    int GameState;
    int score, high;
    int clock, tick;
    int font[5];

    enum STATE
    {
        TITLE,
        PREV,
        GAME,
        RESULT,
    };

    GameControl()
    {
        DxLib_Init();
        Window_Init();

        mario = new Mario();
        pakun = new PakunManager(mario);
        fball = new FBallManager(mario);
        block = new BlockManager(mario);

		GameState = STATE::TITLE;
        clock = score = high = 0;
        tick = 70;

        font[0] = CreateFontToHandle("ＭＳ ゴシック", 85, 10, DX_FONTTYPE_NORMAL);
        font[1] = CreateFontToHandle("ＭＳ ゴシック", 20, 10, DX_FONTTYPE_NORMAL);
        font[2] = CreateFontToHandle("ＭＳ ゴシック", 15, 10, DX_FONTTYPE_NORMAL);
        font[3] = CreateFontToHandle("ＭＳ ゴシック", 50, 10, DX_FONTTYPE_NORMAL);
        font[4] = CreateFontToHandle("ＭＳ ゴシック", 30, 10, DX_FONTTYPE_NORMAL);

        while (!ProcessMessage() && !key[KEY_INPUT_ESCAPE])
        {
            ClearDrawScreen();

            Update();

            ScreenFlip();
        }
    }

    ~GameControl()
    {
        delete mario;
        delete pakun;
        delete fball;
        delete block;
        DxLib_End();
    }

    void Window_Init()
    {
        ChangeWindowMode(true);
        SetBackgroundColor(240, 180, 190);
        SetDrawScreen(DX_SCREEN_BACK);
        SetWindowText("やけくそ配管工");
    }

    void DrawIgnition()
    {
        x1 = WINX / 2 - IGNITION_SIZE / 2;
        y1 = WINY / 2 - IGNITION_SIZE / 2;
        int k = IGNITION_SIZE / (PIXEL_SIZE + 1);
        for (int i = 0; i < IGNITION_SIZE; i++)
        {
            for (int j = 0; j < IGNITION_SIZE; j++)
            {
                pixel = IgnitionPic[i / k][j / k];
                SetPixel(i, j);
            }
        }
    }

    void DrawDeath()
    {
        x1 = 40;
        y1 = 80;
        int k = DEATH_SIZE / PIXEL_SIZE;
        for (int i = 0; i < DEATH_SIZE; i++)
        {
            for (int j = 0; j < DEATH_SIZE; j++)
            {
                pixel = mario->MarioPic[mario->dir][i / k][j / k];
                SetPixel(i, j);
            }
        }
    }

    void AllAction()
    {
		pakun->Action();
        fball->Action();
        block->Action();
        mario->Action();
    }

    void AllDraw()
    {
        fball->Draw();
        mario->Draw();
        pakun->Draw();
        block->Draw();
    }

    void Reset()
    {
        delete mario;
        delete pakun;
        delete fball;
        delete block;
        mario = new Mario();
        pakun = new PakunManager(mario);
        fball = new FBallManager(mario);
        block = new BlockManager(mario);
        if (high < score)high = score;
        clock = 0;
    }

    void Title()
    {
        mario->Draw();
        DrawFormatStringToHandle(15, 75, c->black, font[0], "やけくそ配管工");
        DrawFormatStringToHandle(230, 300, c->black, font[1], "Press Shift Key");
        DrawFormatStringToHandle(560, 460, c->black, font[2], "Ver 2.0");
        if (isPush(KEY_INPUT_LSHIFT))
        {
            GameState = STATE::PREV;
        }
    }

    void Prev()
    {
        mario->Action();
        mario->Draw();
        DrawFormatStringToHandle(30, 75, c->black, font[3], "<- Key    : Move Left");
        DrawFormatStringToHandle(30, 120, c->black, font[3], "-> Key    : Move Right");
        DrawFormatStringToHandle(30, 165, c->black, font[3], "Shift Key : Jump");
        DrawFormatStringToHandle(180, 320, c->black, font[3], "SAVE TNT !!");
        if (!mario->stand)
        {
			srand((unsigned)time(NULL));
            GameState = STATE::GAME;
        }
    }

    void Game()
    {
        AllAction();
        AllDraw();

        score = fball->Score();

		DrawFormatStringToHandle(460, 20, c->black, font[4], "SCORE : %d", score);

        if (mario->death)
        {
            GameState = STATE::RESULT;
        }
    }

    void Result()
    {
        if (clock < tick)
        {
            DrawIgnition();
            clock++;
            return;
        }
        DrawDeath();
        if (high < score)
        {
            DrawFormatStringToHandle(320, 40, c->red, font[3], "HIGT");
        }
        DrawFormatStringToHandle(320, 90, c->black, font[3], "Score : %d", score);
        DrawFormatStringToHandle(320, 230, c->black, font[4], "Again  -> Shift");
        DrawFormatStringToHandle(320, 290, c->black, font[4], "Finish -> Esc");
        if (isPush(KEY_INPUT_LSHIFT))
        {
            Reset();
            GameState = STATE::PREV;
        }
    }

    void Update()
    {
        GetKey();
        switch (GameState)
        {
			case STATE::TITLE:  Title();  break;
			case STATE::PREV:   Prev();   break;
			case STATE::GAME:   Game();   break;
			case STATE::RESULT: Result(); break;
        }
    }
};

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    GameControl* GC = new GameControl;
    delete GC;
    return 0;
}