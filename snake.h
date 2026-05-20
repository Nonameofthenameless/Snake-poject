#ifndef SNAKE_H
#define SNAKE_H
#ifndef _GNU_SORCE 
#define _GNU_SORCE
#endif // !_GNU_SORCE 
#pragma GCC diagnostic ignored "-wunused result"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#define clear_screan() system("cls");
#define seep_ms(ms) sleep(ms)
#define kb_hit() _kbhit()
#define get_ch() _getch()
#else
#include <unistd.h>
#include <termios.h>
#define clear_screan() system("cls");
#define seep_ms(ms) usleep((ms)*1000U)
int kb_hit(void);
int get_ch(void);
#define kb_hit() _kbhit()
#define get_ch() _getch()
#endif // 
//makro const 11
#define board_w 20
#define board_h 20
#define max_snak (board_w * board_h)
#define max_scores 10
#define name_len 32
#define score_file "scores.dat"
#define backupscore_files "its_just_one_of_those_days.dat"
#define ticks_ms 150
//makrofunkcije 9
#define MIN(a,b) ((a)<(b) ? (a):(b))
#define MAX(a,b) ((a)>(b) ? (a):(b))
#define in_bounds(x,y)((x)>=0 && (x)<board_w && (y)<board_h)
#define score_how_hungry 10
//enum tipovi 4 i 11
typedef enum
{
	dir_up = 0,
	dri_down,
	dri_left,
	dri_right,

}direction;
typedef enum {
	menue_play = 1,
	menue_scores,
	menue_rules,
	menue_del_score,
	menue_alt4f,
}MainMenueChoice;
typedef enum {
	cell_empty = 0,
	cell_head,
	cell_body,
	cell_food,
}celltype;
typedef enum {
	sort_by_score = 1,
	sort_by_name,
	sort_by_date,

}sortchoice;
typedef struct {
	int x; //cjelobrojni i tip 2
	int y;
}point;
typedef struct {//statičko polje 15
	point cells[max_snak];
	int length;
	direction Direction;

}snake;
typedef struct {
	char playername[name_len];
	int score;
	long timestamp; //long dovoljan za time t 2
	direction Direction;


}scoreentry;
typedef union {
	int asint;
	float asfloat; //float realni tip 2
	char asbytes[4];
}gamevalue;
typedef struct {
	snake Snake;
	point food;
	int score;
	int isrunning;
	gamevalue spare;

}gamestate;
//exterm globalna deklaracija varijabla 8
extern int g_highscore;
extern scoreentry* g_scoreboard;
//amički alocirano 16
extern int g_scorecout;
int loadscores(void);
int savescores(void);
int backupscores(void);
int delscoresentry(int index);
void listscores(sortchoice sortBy);
void insetscore(const char* name, int score);
void initgame(gamestate* gs);
void rungame(gamestate* gs);
void handleint(gamestate* gs);
void updategame(gamestate* gs);
void drawgame(const gamestate* gs);
void freegame(gamestate* gs);

void showmainmenue(void);
void showscoremenu(void);
void showrules(void);
void showdelmenu(void);

void safefree(void** ptr);
int confirmaction(const char* msg);
void printerror(const char* context);
int comparescoredesc(const void* a, const void* b);
int comparescorename(const void* a, const void* b);
int comparescoredate(const void* a, const void* b);
scoreentry* findscore(const char* name);
void sortscoresby(sortchoice choice);
//pokazivač nafunkciju 2i6
typedef int(*cmpfn)(const void*, const void*);
// rekurzivna deklaracija 25
int binaryserchscore(const scoreentry* arr, int lo, int hi, int target);


#endif