#define _CRT_SECURE_NO_WARNINGS
#include "snake.h"

static void printmainmenue(void) {
    printf("\n");
    printf("||        S    N    A    K   E                       ||\n");
    printf("|| %d. New game\n", menue_play);
    printf("|| %d. High scores\n", menue_scores);
    printf("|| %d. Da rulez\n", menue_rules);
    printf("|| %d. Deleat save data \n", menue_del_score);
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
    printf("%d. points\n", sort_by_score);
    printf("%d. names\n", sort_by_name);
    printf("%d. date\n", sort_by_date);
    printf("0 return \n");
    printf(" chose ");
    fflush(stdout);
    int choice = 0;
    if (scanf("%d", &choice) != 1) choice = 0;
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    if (choice == 0) {
        running = 0;
    }
    if (choice >= sort_by_score && choice <= sort_by_date)
    {
        clear_screan();
        printf("\n High scores \n");
        loadscores();
        listscores((sortchoice)choice);
        printf("Press enter to continue ...");
        fflush(stdout);
        getchar();
    }
}

void showdelmenu(void) {
    clear_screan();
    printf("\n Deleating results \n\n");
    loadscores();
    if (g_scorecout == 0) {
        printf("no results \n");
        printf("Press enter ...");
        fflush(stdout);
        getchar();
        return;
    }
    listscores(sort_by_score);
    printf("chose a score to deleate  (give up = 0)");
    fflush(stdout);
    int idx = 0;
    if (scanf("%d", &idx) != 1) idx = 0;
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    if (idx <= 0 || idx > g_scorecout) {
        printf("canceling\n");
    }
    else {
        sortscoresby(sort_by_score);
        if (confirmaction("are you sure you want to deleate")) {
            if (delscoresentry(idx - 1))
                printf("deleated \n");
            else
                printf("error while deleating \n");
        }
        printf("Press enter to continue ...");
        fflush(stdout);
        getchar();
    }
}

void showrules(void) {
    clear_screan();
    printf("\n rules of the game \n\n");
    printf("Control the snake with W/A/S/D or arrow keys.\n");
    printf("Eat food (*) to make the snake grow and collect points.\n");
    printf("Izbjegavajte zidove i vlastito tijelo.\n");
    printf("Each piece of food earns %d points.\n", score_how_hungry);
    printf("Board: %d x %d fields.\n", board_w, board_h);
    printf("Speed: %d ms per step.\n", ticks_ms);
    printf("Q to exit the game.\n\n");
    printf("Symbols: @ = head o = body * = food\n\n");
    printf("Press enter to continue ...");
    fflush(stdout);
    getchar();
}

void showmainmenue(void) {
    loadscores();
    gamestate gs;
    memset(&gs, 0, sizeof(gs));
    int running = 1;
    while (running) {
        clear_screan();
        printmainmenue();
        MainMenueChoice choice = readmenuchoices();
        switch (choice) {
        case menue_play:
#ifndef _WIN32
            enableRawMode();
#endif
            rungame(&gs);
#ifndef _WIN32
            disableRawMode();
#endif
            loadscores();
            break;
        case menue_scores:
            showscoremenu();
            break;
        case menue_rules:
            showrules();
            break;
        case menue_del_score:
            showdelmenu();
            break;
        case menue_alt4f:
            running = 0;
            break;
        default:
            printf("int eror \n");
            sleep_ms(800);
            break;
        }
    }
}