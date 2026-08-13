#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "types.h"


int main(void) {
    srand((unsigned int)time(NULL)); 

    Square board[40];
    Property properties[28];
    Player players[4];

    inBoard(board);
    inProperties(properties);
    inPlayers(players);

    printf("MONOPOLY-LK Simulation\n");
    int i;
    for (i = 0; i < 4; i++) {
        printf("Player %d : %s\n", i + 1, players[i].name);
    }
    printf("Each player begins with LKR %d.\n", players[0].cash);

    printf("Determining the first player\n");
    int rolls[4];
    for (i = 0; i < 4; i++) {
        rolls[i] = rollDice() + rollDice();
        printf("%s rolls %d.\n", players[i].name, rolls[i]);
    }

    int turnOrder[4] = {0, 1, 2, 3};
    determineTurnOrder(players, rolls, turnOrder);

    
   printf("%s will begin the game.\n", players[turnOrder[0]].name);
   printf("Turn order:\n");
   for (i = 0; i < 4; i++) {
    printf("%s\n", players[turnOrder[i]].name);
   }

   runGame(board, properties, players, turnOrder);

    return 0;
}