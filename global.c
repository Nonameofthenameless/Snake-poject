#define  _CRT_SECURE_NO_WARNINGS
#include "snake.h"
//Globalne varijable – extern u snake.h 8
int         g_highscore = 0;
scoreentry* g_scoreboard = NULL;//dinamički alocirano (koncept 16)
int       g_scorecout = 0;

#ifndef _WIN32
#include <termios.h>

static struct termios s_origTermios;//static lokalna varijabla  6
void enableRawMode(void) {
    struct termios raw;
    tcgetattr(STDIN_FILENO, &s_origTermios);
    raw = s_origTermios;
    raw.c_lflag &= ~(unsigned)(ICANON | ECHO);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void disableRawMode(void) {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &s_origTermios);
}

int kb_hit(void) {
    int ch = getchar();
    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}

int get_ch(void) {
    return getchar();
}
#endif


