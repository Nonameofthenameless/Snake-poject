#define _CRT_SECURE_NO_WARNINGS
#include "snake.h"
//staticka lokalna varijabla broj odigranh partija 6
static int s_gamesplayed = 0;
//inline helper funkcije 9
static inline int pointequal(point a, point b) {
	return(a.x == b.x && a.y == b.y);
}
static inline void placefood(gamestate* gs)
{
	if (gs == NULL) return;
	int valid;
	do {
		valid = 1;
		gs->food.x = rand() % board_w;
		gs->food.y = rand() % board_h;
		for (int i = 0; i < gs->Snake.length; i++) {
			if (pointequal(gs->Snake.cells[i], gs->food))
			{
				valid = 0;
				break;
			}

		}

	} while (!valid);
}
