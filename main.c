#define _CRT_SECURE_NO_WARNINGS
#include "snake.h"
int main(void) {
	srand((unsigned int)time(NULL));
	showmainmenue();
	//oslobođavanje globalne memorije prije izlaza17 i 18
	safefree((void**)&g_scoreboard);
	printf("\nthank you so much for to playing my game bye bye\n\n ");
	return 0;
}