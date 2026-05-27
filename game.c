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
//inicijalizacija igre 13
void initgame(gamestate* gs) {
	if (gs == NULL)return;
	//zastita parametra 14
	memset(gs, 0, sizeof(gamestate));
	//anuliranj 18
	gs->Snake.length = 3;
	gs->Snake.Direction = dir_right;
	gs->Snake.cells[0] = (point){
		board_w / 2, board_h / 2
	};
	gs->Snake.cells[1] = (point){
		board_w / 2 - 1,board_h / 2
	};
	gs->Snake.cells[2] = (point){
		board_w / 2 - 2,board_h / 2
	};
	gs->score = 0;
	gs->isrunning = 1;
	//unija 4
	gs->spare.asint = s_gamesplayed;
	placefood(gs);
	s_gamesplayed++;
}
//crtanje plja 13
void drawgame(const gamestate* gs) {
	if (gs == NULL) return;
	clear_screan();
	printf("+");
	for (int x = 0; x < board_w; x++)printf("-");
	printf("+\n");
	for (int y = 0; y < board_h; y++) {
		printf("|");
		for (int x = 0; x < board_w; x++) {
			point cur = { x,y };
			if (pointequal(cur, gs->food)) {
				printf("%s", "\U+1F40D");//help whit head
			}
			else if (pointequal(cur, gs->food)) {
				printf("%s", "U+1F34E");
			}
			else
			{
				int isbody = 0;
				for (int i = 1; i < gs->Snake.length; i++) {
					if (pointequal(gs->Snake.cells[i], cur)) {
						isbody = 1;
						break;
					}
				}
				printf(isbody ? "%s", "U+1F7E9" : "");

			}
		}
		printf("|\n");
	
	}
	printf("+");
	for (int x = 0; x < board_w; x++) printf("-");
	printf("+\n");
	printf("points:%-6d record: %-6d round %d\n", gs->score, g_highscore, s_gamesplayed);
	printf("controls w/a/s/d or arrow keys  | Q= quit \n");
}
//citanje s tipkovnice 12
void handleinput(gamestate* gs) {
	if (gs == NULL || !kb_hit())return;
	int ch = get_ch();
#ifndef _WIN32
	if (ch = 27) {
		int c2 = get_ch();
		if (c2 == '[') {
			int c3 = get_ch();
			switch (c3)
			{
			case 'A':if (gs->Snake.Direction != dir_down) gs->Snake.Direction = dir_up; break;
			case 'B':if (gs->Snake.Direction != dir_up) gs->Snake.Direction = dir_down; break;

			case 'C':if (gs->Snake.Direction != dir_left) gs->Snake.Direction = dir_right; break;
			case 'B':if (gs->Snake.Direction != dir_right) gs->Snake.Direction = dir_left; break;
			}
		}
		return;
			}

	
#endif // !_WIN32
	switch (ch) {
	case 'w':case 'W':if (gs->Snake.Direction != dir_down) gs->Snake.Direction = dir_up; break;
	case 's':case 'S':if (gs->Snake.Direction != dir_up) gs->Snake.Direction = dir_down; break;

	case 'd':case 'D':if (gs->Snake.Direction != dir_left) gs->Snake.Direction = dir_right; break;
	case 'a':case 'A':if (gs->Snake.Direction != dir_right) gs->Snake.Direction = dir_left; break;
	case 'q':case 'Q':gs->isrunning = 0; break;
	default:break;
	}
	
}
// ažuriranje logike 13
void updategame(gamestate* gs) {
	if (gs == NULL || !gs->isrunning)return;
	point newhead = gs->Snake.cells[0];
	switch (gs->Snake.Direction) {
	case dir_up: newhead.y--; break;

	case dir_down: newhead.y++; break;

	case dir_left: newhead.x--; break;

	case dir_right: newhead.x++; break;
	default: break;
	}
	//makro  funkcija 9
	if (!in_bounds(newhead.x, newhead.y)) {
		gs->isrunning = 0;
		return;
	}
	for (int i = 0; i < gs->Snake.length; i++) {
		if (pointequal(gs->Snake.cells[i], newhead)) {
			gs->isrunning = 0;
			return;
		}
	}
	int ate = pointequal(newhead, gs->food);
	//pomaknuti staticko polje 15
	if (!ate) {
		memmove(&gs->Snake.cells[1], &gs->Snake.cells[0], (size_t)(gs->Snake.length - 1) * sizeof(point));
	}
	else {
		if (gs->Snake.length < max_snak)
		{
			memmove(&gs->Snake.cells[1], &gs->Snake.cells[0],
				(size_t)gs->Snake.length * sizeof(point));
			gs->Snake.length++;
		}
		else {
			gs->isrunning = 0;
			return;

		}
		gs->score += score_how_hungry;
		placefood(gs);
	}
	gs->Snake.cells[0] = newhead;

}
// ciscenje resursa 17,18
void freegame(gamestate* gs) {
	if (gs == NULL)return;
	memset(gs, 0, sizeof(gamestate));

}
void rungame(gamestate* gs) {
	if (gs == NULL)return;
	char playername[name_len];
	printf("\n insert name:");
	fflush(stdout);
	if (fgets(playername, name_len, stdin) == NULL) {
		strncpy(playername, "unkknown", name_len - 1);
}
	playername[strcspn(playername, "\n")] = '\0';
	if (playername[0] == '\0')strncpy(playername, "player", name_len - 1);
	initgame(gs);
	while (gs->isrunning) {
		drawgame(gs);
		handleinput(gs);
		updategame(gs);
		sleep_ms(ticks_ms);
	}
	drawgame(gs);
	printf("\n game over  \n score %d\n\n", gs->score);
	// crud insetz 1
	if (gs->score > 0) {
		insetscore(playername, gs->score);
		printf("result saved in %s\n", score_file);

	}
	freegame(gs);
	printf("\n press enter to continue ...");
	fflush(stdout);
	getchar();


}