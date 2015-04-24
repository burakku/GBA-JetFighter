//Name: Wenqi Hu

#include <stdio.h>
#include <stdlib.h>
#include "myLib.h"
#include "game.h"

int main(void)
{
	REG_DISPCTL = MODE3 | BG2_ENABLE;

	state = 0;

	while(1)
	{
		switch(state)
		{
			case START:
			start();
			break;

			case GAME:
			game();
			break;

			case LOSE:
			lose();
			break;

			default: break;
		}
	}
}