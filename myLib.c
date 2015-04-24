//Name: Wenqi Hu

#include "myLib.h"
#include "game.h"

u16 *videoBuffer = (u16 *)0x6000000;

void setPixel(int row, int col, u16 color)
{
	videoBuffer[OFFSET(row, col, 240)] = color;
}

void drawRect(int row, int col, int width, int height, volatile u16 color)
{
	int r;
	for(r=0; r<height; r++)
	{
		DMA[3].src = &color;
		DMA[3].dst = &videoBuffer[OFFSET(row+r, col, 240)];
		DMA[3].cnt = width | DMA_ON | DMA_SOURCE_FIXED;
	}
}

void drawImage3(int r, int c, int width, int height, const u16* image)
{
	for (int x = 0; x < height; x++)
	{
		DMA[3].src = &image[OFFSET(x, 0, width)];
		DMA[3].dst = &videoBuffer[OFFSET(r + x, c, 240)];
		DMA[3].cnt = (width) | DMA_ON;
	}
}

void drawBG(const u16 arr[])
{
	DMA[3].src = arr;
	DMA[3].dst = videoBuffer;
	DMA[3].cnt = 38400 | DMA_ON; 
}

void waitForVblank()
{
	while(SCANLINECOUNTER >= 160);
	while(SCANLINECOUNTER < 160);
}

void fillScreen(volatile u16 color)
{
	DMA[3].src = &color;
	DMA[3].dst = videoBuffer;
	DMA[3].cnt = 38400 | DMA_ON | DMA_SOURCE_FIXED; 
}

void drawFighter(PLAYER* p)
{
	drawRect(p->oldr, p->oldc, p->width, p->height, BLACK);
	drawImage3(p->r, p->c, p->width, p->height, p->image);
}

void drawTarget(TARGET* p)
{
	drawRect(p->r, p->oldc, p->width, p->height, BLACK);
	drawImage3(p->r, p->c, p->width, p->height, p->image);
}

void drawBullet(BULLET* b)
{
	drawRect(b->r, b->c, 1, 1, BLACK);
	drawRect(b->r, (b->c) + 1, 3, 1, GREEN);
}

void drawStar(int p)
{
	videoBuffer[p + 1] = BLACK;
	videoBuffer[p] = WHITE;
}