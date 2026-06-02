#define _CRT_SECURE_NO_WARNINGS
#include "snake.h"
//statička varijabla – verzija formata datoteke  6 
static const int s_fileversion = 1;
static inline int getfileversion(void) { return s_fileversion; }
int loadscores(void) {
	FILE* fp = fopen(score_file, "rb"); //binarna datoteka 19
	if (fp == NULL) {
		g_scorecout = 0;
		return 1;
	}//doznaj veličinu datoteke – fseek / ftell / rewind(koncept 20
	if (fseek(fp, 0L, SEEK_END) != 0) {
		printerror("loadscores:fseek");
		fclose(fp);
		return 0;
	}
	long filesize = ftell(fp);
	if (filesize < 0) {
		printerror("loadscores:ftell");
		fclose(fp);
		return 0;
	}
	rewind(fp);//doznaj veličinu datoteke – fseek / ftell / rewind 20
	int count = (int)(filesize / (long)sizeof(scoreentry));
	if (count <= 0) {
		fclose(fp);
		g_scorecout = 0;
		return 1;
	}//dinamička alokacija 16, 17
	void* tmp = realloc(g_scoreboard, (size_t)count * sizeof(scoreentry));
	if (tmp == NULL) {
		printerror("loadscores: relloc");
		fclose(fp);
		return 0;

	}
	g_scoreboard = (scoreentry*)tmp;
	memset(g_scoreboard, 0, (size_t)count * sizeof(scoreentry));//anuliranje  18
	size_t read = fread(g_scoreboard, sizeof(scoreentry), (size_t)count, fp);
	if (ferror(fp)) {//provjera greške 22
		printerror("loadscores: fread");
		fclose(fp);
		return 0;
	}
	fclose(fp);// zatvaranje datoteke  19
	g_scorecout = (int)read;
	(void)getfileversion();
	g_highscore = 0;
	for (int i = 0;i < g_scorecout;i++) {
		if (g_scoreboard[i].score > g_highscore)
			g_highscore = g_scoreboard[i].score;
	}
	return 1;
}
int savescores(void) {
	if (g_scoreboard == NULL || g_scorecout <= 0) return 2;
	backupscores();//backup prije prepisivanja 21
	FILE* fp = fopen(score_file, "wb");
	if (fp == NULL) {
		printerror("savescores: fopen");
		return 0;
	}
	size_t written = fwrite(g_scoreboard, sizeof(scoreentry),
		(size_t)g_scorecout, fp);
	if (ferror(fp) || (int)written != g_scorecout) {
		printerror("savscores: fwrite");
		fclose(fp);
		return 0;
	}
	fclose(fp);
	return 1;
}
int backupscores(void) {
	FILE* src = fopen(score_file, "rb");
	if (src == NULL)return 1;
	FILE* dst = fopen(backupscore_files, "wb");
	if (dst == NULL) {
		printerror("backupsaves: fopen dst");
		fclose(src);
		return 0;

	}
	unsigned char buf[512];
	size_t n;
	while ((n = fread(buf, 1, sizeof(buf), src)) > 0) {
		fwrite(buf, 1, n, dst);
		if (ferror(dst)) {
			printerror("backupScores: fwrite");
			fclose(src);
			fclose(dst);
			return 0;
		}
	}
	fclose(src);
	fclose(dst);
	return 1;
}
int delscoresentry(int index){
	if (g_scoreboard == NULL || index < 0 || index >= g_scorecout) {
		fprintf(stderr, "deleatscore entry:neispravni indeks%d\n", index);
		return 0;
	}
	if (index < g_scorecout - 1) {
		memmove(&g_scoreboard[index],
			&g_scoreboard[index + 1],
			(size_t)(g_scorecout - index - 1) * sizeof(scoreentry));
		
	}
	memset(&g_scoreboard[g_scorecout - 1], 0, sizeof(scoreentry));
	g_scorecout--;
	if (g_scorecout > 0) {
		void* tmp = realloc(g_scoreboard, (size_t)g_scorecout * sizeof(scoreentry));
		if (tmp != NULL)g_scoreboard = (scoreentry*)tmp;
	}
	else {
		safefree((void**)&g_scoreboard);//nema više unosa – oslobodi memoriju i fizički obriši datoteku  21
		if (remove(score_file) != 0)
			printerror("delscoresentry: remove");
		return 1;
	}
	return savescores();
}
void insertscore(const char* name, int score) {
	if (name == NULL || score <= 0) return;   //zaštita parametra  14 

	scoreentry entry;
	memset(&entry, 0, sizeof(entry));
	strncpy(entry.playername, name, name_len - 1);
	entry.score = score;
	entry.timestamp = (long)time(NULL);

	// Proširi dinamički niz 16, 17 
	void* tmp = realloc(g_scoreboard, (size_t)(g_scorecout + 1) * sizeof(scoreentry));
	if (tmp == NULL) {
		printerror("insertScore: realloc");
		return;
	}
	g_scoreboard = (scoreentry*)tmp;
	g_scoreboard[g_scorecout] = entry;
	g_scorecout++;

	if (score > g_highscore) g_highscore = score;

	savescores();
}
void listscores(sortchoice sortby) {
	if (g_scoreboard == NULL || g_scorecout == 0) {
		printf("  no results\n");
		return;

	}
	sortscoresby(sortby);//23
	printf("\n  %-4s %-20s %-8s %s\n", "Rbr", "Igrač", "Bodovi", "Datum");
	printf("  %s\n", "--------------------------------------------");
	for (int i = 0;i < g_scorecout;i++) {
		char datebuf[32];
		time_t ts = (time_t)g_scoreboard[i].timestamp;
		struct tm* tminfo = localtime(&ts);
		if (tminfo != NULL)
			strftime(datebuf, sizeof(datebuf), "%d.%m.%Y %H:%M", tminfo);
		else
			strncpy(datebuf, "N/A", sizeof(datebuf));
		printf("  %-4d %-20s %-8d %s\n",
			i + 1,
			g_scoreboard[i].playername,
			g_scoreboard[i].score,
			datebuf);


	}
	printf("\n");
	// primjer rekurzivnog binaryserchscore  25
	scoreentry* stmp = (scoreentry*)malloc((size_t)g_scorecout * sizeof(scoreentry));
	if (stmp != NULL) {
		memcpy(stmp, g_scoreboard, (size_t)g_scorecout * sizeof(scoreentry));
		qsort(stmp, (size_t)g_scorecout, sizeof(scoreentry), comparescoredesc);
		int pos = binaryserchscore(stmp, 0, g_scorecout - 1, g_highscore);
		if (pos >= 0)
			printf("  [Record found via recursive binary search: %d points]\n\n", g_highscore);
		free(stmp);
	}
}
