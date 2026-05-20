#define _CRT_SECURE_NO_WARNINGS
#include "snake.h"
static void printmainmenue(void) {
	printf("\n");
	printf("||        S    N    A    K   E                       ||");
	printf("|| %d. New game\n", menue_play);
	printf("|| %d. High scores\n", menue_scores);
	printf("|| %d. Da rulez\n", menue_rules);
	printf("|| %d. Deleat french langige packet\n", menue_del_score);
	printf("|| %d. alt f4\n", menue_alt4f);
	printf("   izbor:  ");
	fflush(stdout);
}
static MainMenueChoice readmenuchoices(void) {
	int choice = 0;
	if (scanf("%d", &choice) != 1) {
		int c;
		while ((c = getchar()) != '\n' && c != EOF);
		return (MainMenueChoice)0;
	}
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
	return (MainMenueChoice)choice;

 }
void showscoremenu(void) {
	int running = 1;
	clear_screan();
	printf("High scores\n");
	printf("sortby:\n");
	printf("d%, points\n",sort_by_score);
	printf("d%, names\n", sort_by_name);
	printf("d%, date\n", sort_by_date);
	printf("0 return \n");
	printf(" chose ");
	fflush(stdout);
	int choice = 0;
	if (scanf("%d", &choice) != 1)choice = 0;
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
	if (choice == 0) {
		running = 0;
	}

}
