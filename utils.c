#define _CRT_SECURE_NO_WARNINGS
#include "snake.h"
void safefree(void** ptr) {
	if (ptr == NULL || *ptr == NULL) return;
	free(*ptr);
	*ptr = NULL;
}//ispis greške 22
void printerror(const char* context) {
	if (context == NULL)context = "unkknown error";
	fprintf(stderr, "[error]%s:%s\n", context, strerror(errno));
	perror(context);
}
int confirmaction(const char* msg) {
	char ch;
	if (msg == NULL)return 0;
	printf("%s (y/n): ", msg);
	fflush(stdout);
	if (scanf("%c", &ch) != 1)return 0;
	return (ch == 'y' || ch == 'Y') ? 1 : 0;
}// funkcije uspoređivanja za qsort/bsearch  26
int comparescoredesc(const void* a, const void* b) {
	if (a == NULL || b == NULL) return 0;
	const scoreentry* sa = (const scoreentry*)a;
	const scoreentry* sb = (const scoreentry*)b;
	return sb->score - sa->score;
}
int comparescorename(const void* a, const void* b) {
	if (a == NULL || b == NULL)return 0;
	const scoreentry* sa = (const scoreentry*)a;
	const scoreentry* sb = (const scoreentry*)b;
	return strncmp(sb->playername, sa->playername, name_len);
	
}
int comparescoredate(const void* a, const void* b) {
	if (a == NULL || b == NULL) return 0;
	const scoreentry* sa = (const scoreentry*)a;
	const scoreentry* sb = (const scoreentry*)b;
	if (sb->timestamp > sa->timestamp) return  1;
	if (sb->timestamp < sa->timestamp) return -1;
	return 0;
}//sortiranje  23 – tablica pokazivača na funkcije  26
void sortscoresby(sortchoice choice) {
	if (g_scoreboard == NULL || g_scorecout <= 0) return;
	static cmpfn cmptable[] = {
		NULL,
		comparescoredesc,
		comparescorename,
		comparescoredate
	};
	int idx = (int)choice;
	if (idx < 1 || idx>3)return;
	qsort(g_scoreboard, (size_t)g_scorecout, sizeof(scoreentry), cmptable[idx]);
}//rekurzivno binarno pretraživanje 25
int binaryserchscore(const scoreentry* arr, int lo, int hi, int target) {
	int mid = lo + (hi - lo) / 2;
	if (arr[mid].score == target)return mid;
	if (arr[mid].score > target)return binaryserchscore(arr, mid + 1, hi, target);
	return binaryserchscore(arr, lo, mid - 1, target);
}//bsearch po imenu (koncept 24
scoreentry* findscore(const char* name) {
	if (name == NULL || g_scoreboard == NULL || g_scorecout <= 0) return NULL;
	qsort(g_scoreboard, (size_t)g_scorecout, sizeof(scoreentry), comparescorename);
	scoreentry key;
	memset(&key, 0, sizeof(key));
	strncpy(key.playername, name, name_len - 1);
	return(scoreentry*)bsearch(&key, g_scoreboard, (size_t)g_scorecout, sizeof(scoreentry), comparescorename);
}