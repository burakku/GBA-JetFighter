//Name: Wenqi Hu

#include <stdlib.h>
#include <stdio.h>
#include "myLib.h"
#include "text.h"
#include "game.h"
#include "splash.h"
#include "fighter.h"
#include "target.h"
#include "gameover.h"

int state;
int score = 0;
int rst = 0;
int timer = 0;
int timerS = 0;
int star[30];
char scoreStr[20];
PLAYER py;
TARGET tg[TG_NUM];
BULLET bl[BL_NUM];
static const BULLET empty_bl;
int trigger[TG_NUM];

void reset()
{
	py.r = 0;
	tg[0].r = 0;

	for(int i = 0; i < BL_NUM; i++)
	{
		bl[i] = empty_bl;
	}

	for(int i = 0; i < TG_NUM; i++)
	{
		trigger[i] = 0;
	}

	for(int i = 0; i < 30; i++)
	{
		star[i] = 2400 + rand() % 36000;
	}
	rst = 1;
	score = 0;
}

void start()
{
	drawBG(splash);
	drawString(130, 10, "Press Start", BLACK);

	while(!KEY_DOWN_NOW(BUTTON_START));
	state++;
	fillScreen(BLACK);
}

void game()
{
	waitForVblank();

	if(!rst)
	{
		reset();
	}

	if(timer)
	{
		timer--;
	}

	if(timerS)
	{
		timerS--;
	}

	if(py.r == 0)
	{
		py.r = 80;
		py.c = 0;
		py.oldr = 80;
		py.oldc = 0;
		py.width = PLAYER_W;
		py.height = PLAYER_H;
		py.image = (u16 *)fighter;
	}

	if(tg[0].r == 0)
	{
		for(int i = 0; i < TG_NUM; i++)
		{
			tg[i].r = 24 + rand() / (RAND_MAX / (146 - 24 + 1) + 1);;
			tg[i].c = 231 - i*30;
			tg[i].oldc = tg[i].c;
			tg[i].width = TARGET_W;
			tg[i].height = TARGET_H;
			tg[i].image = (u16 *)target;
		}
		trigger[0] = 1;
	}
	
	py.oldr = py.r;
	py.oldc = py.c;

	for(int i = 0; i < TG_NUM; i++)
	{
		tg[i].oldc = tg[i].c;
		tg[i].c--;
		
		if(tg[i].c < 10 && trigger[i])
		{
			state++;
		}
	}

	for(int i = 0; i < TG_NUM; i++)
	{
		if(tg[0].c < (201 - i*30) && trigger[i] == 0)
		{
			tg[i].c = 231;
			tg[i].oldc = tg[i].c;
			trigger[i] = 1;
		}
	}

	if(KEY_DOWN_NOW(BUTTON_DOWN))
	{
		py.r += 1;

		if(py.r > 136)
		{
			py.r = 136;
		}
	}

	if(KEY_DOWN_NOW(BUTTON_UP))
	{
		py.r -= 1;

		if(py.r < 10)
		{
			py.r = 10;
		}
	}

	if(KEY_DOWN_NOW(BUTTON_LEFT))
	{
		py.c -= 1;

		if(py.c < 0)
		{
			py.c = 0;
		}
	}

	if(KEY_DOWN_NOW(BUTTON_RIGHT))
	{
		py.c += 1;

		if(py.c > 195)
		{
			py.c = 195;
		}
	}

	if(KEY_DOWN_NOW(BUTTON_A))
	{
		if(!timer)
		{
			for(int i = 0; i < BL_NUM; i++)
			{
				if(!bl[i].on)
				{
					bl[i].r = py.r + 11;
					bl[i].c = py.c + 22;
					bl[i].on = 1;
					timer = 8;
					break;
				}
			}
		}
	}

	if(!timerS)
	{
		for(int i = 0; i < 30; i++)
		{
			drawStar(star[i]);
			star[i]--;
			if(star[i] < 2400)
			{
				star[i] = 38400;
			}
		}
		timerS = 11;
	}

	for(int i = 0; i < BL_NUM; i++)
	{
		if(bl[i].on)
		{
			bl[i].c++;
			if(bl[i].c > 236)
			{
				bl[i].on = 0;
				drawRect(bl[i].r, bl[i].c, 3, 1, BLACK);
			}
		}
	}

	for(int i = 0; i < BL_NUM; i++)
	{
		for(int j = 0; j < TG_NUM; j++)
		{
			if(bl[i].r >= tg[j].r && 
				bl[i].r < tg[j].r + TARGET_H && 
				bl[i].c + 4 > tg[j].c && 
				trigger[j])
			{
				destroy(i, j);
			}
		}
	}

	for(int i = 0; i < BL_NUM; i++)
	{
		if(bl[i].on)
		{
			drawBullet(&bl[i]);
		}
	}

	for(int i = 0; i < TG_NUM; i++)
	{
		if(py.r + PLAYER_H-1 > tg[i].r && 
			py.r < tg[i].r + TARGET_H-1 && 
			py.c + PLAYER_W > tg[i].c+1 && 
			py.c+1 < tg[i].c + TARGET_W && 
			trigger[i])
		{
			state++;
		}

		if(trigger[i])
		{
			drawTarget(&tg[i]);
		}
	}

	sprintf(scoreStr, "Score: %d", score);
	drawRect(0, 200, 30, 10, BLACK);
	drawString(0, 170, scoreStr, YELLOW);
	drawFighter(&py);

	waitForVblank();
}

void destroy(int i, int j)
{
	drawRect(bl[i].r, bl[i].c, 4, 1, BLACK);
	drawRect(tg[j].r, tg[j].oldc, tg[j].width, tg[j].height, BLACK);
	tg[j].r = 24 + rand() / (RAND_MAX / (146 - 24 + 1) + 1);
	for(int k = 0; k < TG_NUM; k++)
	{
		while(tg[j].r + TARGET_H > tg[k].r && 
				tg[j].r < tg[k].r + TARGET_H && 
				tg[k].c + TARGET_W > 231 &&
				!(j == k))
		{
			tg[j].r = 24 + rand() / (RAND_MAX / (146 - 24 + 1) + 1);
		}
	}
	tg[j].c = 231;
	tg[j].oldc = tg[j].c;
	bl[i].on = 0;
	score++;

}

void lose()
{
	drawBG(gameover);
	sprintf(scoreStr, "Your score is: %d", score);
	drawString(115, 64, scoreStr, WHITE);
	drawString(135, 50, "Press select to restart", WHITE);
	while(!KEY_DOWN_NOW(BUTTON_SELECT));
	state = 0;
	rst = 0;
}