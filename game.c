#include <stdio.h>
#include "types.h"

void runGame(Square board[], Property properties[], Player players[], int turnOrder[]) {
    int round;
    int i;

    for (round = 1; round <= 500; round++) {
        for (i = 0; i < 4; i++) {
            int currentPlayer = turnOrder[i];

            if (players[currentPlayer].bankrupt) {
                continue;  
            }

            int diceRoll = rollDice() + rollDice();
            printf("%s rolled %d.\n", players[currentPlayer].name, diceRoll);
            movePlayer(&players[currentPlayer], diceRoll);

            int landedSquare = players[currentPlayer].position;
            if (board[landedSquare].propertyindex != -1) {
                int propIdx = board[landedSquare].propertyindex;
                Property *prop = &properties[propIdx];

                if (prop->owner == -1) {
                    purchaseProperty(&players[currentPlayer], currentPlayer, prop);
                } else if (prop->owner != currentPlayer) {
                    payRent(&players[currentPlayer], &players[prop->owner], prop);
                }
            }
        }
    }
}