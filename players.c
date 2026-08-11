#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "types.h"

void inPlayers(Player players[]){
    strcpy(players[0].name, "Aggressive Investor"); 
    players[0].strategy = STRAT_AGGRESSIVE_INVESTOR;
    players[0].cash = 30000;
    players[0].position = 0;
    players[0].bankrupt = 0;
 
   strcpy(players[1].name, "Conservative Banker");
    players[1].strategy = STRAT_CONSERVATIVE_BANKER;
    players[1].cash = 30000;
    players[1].position = 0;
    players[1].bankrupt = 0;

   strcpy(players[2].name, "Risk Taker");
    players[2].strategy = STRAT_RISK_TAKER;
    players[2].cash = 30000;
    players[2].position = 0;
    players[2].bankrupt = 0;

   strcpy(players[3].name, "Opportunistic Trader");
    players[3].strategy = STRAT_OPPORTUNISTIC_TRADER;
    players[3].cash = 30000;
    players[3].position = 0;
    players[3].bankrupt = 0;
}

int rollDice(void) {
    return (rand() % 6) + 1; 
}

void sortRange(int rolls[], int turnOrder[], int start, int end){
        int i, j, MaxIndex, temp;

        for(i=start; i<end; i++){
            MaxIndex = i;
            for(j=i+1; j<=end; j++){
                if(rolls[turnOrder[j]] > rolls[turnOrder[MaxIndex]]){
                    MaxIndex = j;
                }
            }
            temp = turnOrder[i];
            turnOrder[i] = turnOrder[MaxIndex];
            turnOrder[MaxIndex] = temp;
        }
    }
void determineTurnOrder(Player players[], int rolls[], int turnOrder[]) {
    int i;

    sortRange(rolls, turnOrder, 0, 3);

    int start = 0;
    while (start < 4) {
        int end = start;
        while (end + 1 < 4 && rolls[turnOrder[end + 1]] == rolls[turnOrder[start]]) {
            end++;
        }

        while (end > start) {
            for (i = start; i <= end; i++) {
                int p = turnOrder[i];
                rolls[p] = rollDice() + rollDice();
                printf("%s rolls %d.\n", players[p].name, rolls[p]);
            }
            sortRange(rolls, turnOrder, start, end);

            while (end > start && rolls[turnOrder[end]] != rolls[turnOrder[start]]) end--;
        }

        start = end + 1;
    }
}