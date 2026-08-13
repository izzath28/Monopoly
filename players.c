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
    players[0].inJail = 0; 
    players[0].jailTurns = 0;
    players[0].hasLoan = 0;  
    players[0].loanAmount = 0;  
    players[0].numCollateral = 0;

   strcpy(players[1].name, "Conservative Banker");
    players[1].strategy = STRAT_CONSERVATIVE_BANKER;
    players[1].cash = 30000;
    players[1].position = 0;
    players[1].bankrupt = 0;
    players[1].inJail = 0;  
    players[1].jailTurns = 0;
    players[1].hasLoan = 0;  
    players[1].loanAmount = 0;  
    players[1].numCollateral = 0;

   strcpy(players[2].name, "Risk Taker");
    players[2].strategy = STRAT_RISK_TAKER;
    players[2].cash = 30000;
    players[2].position = 0;
    players[2].bankrupt = 0;
    players[2].inJail = 0; 
    players[2].jailTurns = 0;
    players[2].hasLoan = 0;  
    players[2].loanAmount = 0;  
    players[2].numCollateral = 0;

   strcpy(players[3].name, "Opportunistic Trader");
    players[3].strategy = STRAT_OPPORTUNISTIC_TRADER;
    players[3].cash = 30000;
    players[3].position = 0;
    players[3].bankrupt = 0;
    players[3].inJail = 0;  
    players[3].jailTurns = 0;
    players[3].hasLoan = 0;  
    players[3].loanAmount = 0;  
    players[3].numCollateral = 0;
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

int shouldBuyProperty(Player *player, Property *prop) {
    if (player->cash < prop->purchasePrice) {
        return 0;   /* can't afford it, no matter the strategy */
    }

    switch (player->strategy) {
        case STRAT_AGGRESSIVE_INVESTOR:
            /* buys as long as at least one future rent payment remains affordable */
            return (player->cash - prop->purchasePrice) >= prop->baseRent;

        case STRAT_CONSERVATIVE_BANKER:
            /* only if at least 50% of current cash remains afterward */
            return (player->cash - prop->purchasePrice) >= (player->cash / 2);

        case STRAT_RISK_TAKER:
            /* buys everything it can possibly afford */
            return 1;

        case STRAT_OPPORTUNISTIC_TRADER:
            /* only if it leaves a reasonable cash cushion */
            return (player->cash - prop->purchasePrice) >= (player->cash / 3);
    }

    return 0;
}

int shouldBuildHouse(Player *player, int playerIndex, Property properties[], int *outPropIndex) {
    int g, i;

    for (g = GRP_BROWN; g <= GRP_DARK_BLUE; g++) {
        if (!hasMonopoly(properties, playerIndex, (PropertyGroup)g)) continue;

        int minHouses = 99, targetIdx = -1;
        for (i = 0; i < 28; i++) {
            if (properties[i].group == (PropertyGroup)g && properties[i].owner == playerIndex) {
                if (properties[i].houses < minHouses) {
                    minHouses = properties[i].houses;
                    targetIdx = i;
                }
            }
        }

        if (targetIdx == -1 || minHouses >= 5) continue;

        int cost = (minHouses == 4) ? properties[targetIdx].hotelCost : properties[targetIdx].houseCost;
        if (cost > player->cash) continue;

        *outPropIndex = targetIdx;
        return 1;
    }
    return 0;
}

int decideAuctionBid(Player *player, Property *prop, int currentBid) {
    int nextBid = currentBid + 250;
    if (nextBid > player->cash) return 0;

    switch (player->strategy) {
        case STRAT_AGGRESSIVE_INVESTOR:
            if (nextBid <= (prop->purchasePrice * 120) / 100) return nextBid;
            return 0;
        case STRAT_CONSERVATIVE_BANKER:
            if (nextBid < prop->purchasePrice) return nextBid;
            return 0;
        case STRAT_RISK_TAKER:
            return nextBid;
        case STRAT_OPPORTUNISTIC_TRADER:
            if (nextBid <= (prop->purchasePrice * 80) / 100) return nextBid;
            return 0;
    }
    return 0;
}

int decideTakeLoan(Player *player) {
    switch (player->strategy) {
        case STRAT_AGGRESSIVE_INVESTOR:  return !player->hasLoan;
        case STRAT_CONSERVATIVE_BANKER:  return (!player->hasLoan && player->cash < 1000);
        case STRAT_RISK_TAKER:           return !player->hasLoan;
        case STRAT_OPPORTUNISTIC_TRADER: return (!player->hasLoan && player->cash < 3000);
    }
    return 0;
}

int decideRepayLoan(Player *player) {
    if (!player->hasLoan) return 0;
    switch (player->strategy) {
        case STRAT_AGGRESSIVE_INVESTOR:  return (player->cash > 2 * player->loanAmount);
        case STRAT_CONSERVATIVE_BANKER:  return (player->cash > player->loanAmount);
        case STRAT_RISK_TAKER:           return 0;
        case STRAT_OPPORTUNISTIC_TRADER: return (player->cash > (3 * player->loanAmount) / 2);
    }
    return 0;
}

int decideMortgage(Player *player) {
    return (player->cash < 500);
}