#include <stdio.h>
#include "types.h"

int countSolventPlayers(Player players[]) {
    int count = 0;
    int i;
    for (i = 0; i < 4; i++) {
        if (!players[i].bankrupt) count++;
    }
    return count;
}

void runGame(Square board[], Property properties[], Player players[], int turnOrder[]) {
    int round;
    int i;
    int currentBoomGroup = -1;
    int currentDeclineGroup = -1;
    int groupLastAffected[8] = {-100, -100, -100, -100, -100, -100, -100, -100};
    int activeRegionalCard = -1;
    int regionalCardRoundsLeft = 0;
    int powerFailureRounds = 0;
    int labourStrikeRounds = 0;
    int activeEconomicEvent = -1;
    int economicEventRoundsLeft = 0;
    int activeRegulation = -1;
    int marketBoomRoundsLeft = 0;
    int marketDeclineRoundsLeft = 0;
    int currentInflationRate = 0;

    for (round = 1; round <= 500; round++) {
        for (i = 0; i < 4; i++) {
            int currentPlayer = turnOrder[i];

            if (players[currentPlayer].bankrupt) continue;

            if (resolveJail(&players[currentPlayer])) continue;

            {
             int p;
             for (p = 0; p < 28; p++) {
                 if (properties[p].owner == currentPlayer) {
                      if (decideRenovateDamage(&players[currentPlayer], &properties[p])) {
                         renovateDamagedBuilding(&players[currentPlayer], &properties[p]);
                        } else if (decideMaintenance(&players[currentPlayer], &properties[p])) {
                               performMaintenance(&players[currentPlayer], &properties[p]);
                            }
                    }
                }
            }

            int diceRoll = rollDice() + rollDice();
            printf("%s rolled %d.\n", players[currentPlayer].name, diceRoll);
            movePlayer(&players[currentPlayer], diceRoll);

            int landedSquare = players[currentPlayer].position;
            printf("%s landed on %s.\n", players[currentPlayer].name, board[landedSquare].name);

            if (players[currentPlayer].position == 30) {
                players[currentPlayer].inJail = 1;
                players[currentPlayer].jailTurns = 0;
                players[currentPlayer].position = 10;
                printf("%s is sent to Jail.\n", players[currentPlayer].name);
                continue;
            }

            
            if (board[landedSquare].propertyindex != -1) {
                int propIdx = board[landedSquare].propertyindex;
                Property *prop = &properties[propIdx];

                if (prop->owner == -1) {
                     if (shouldBuyProperty(&players[currentPlayer], prop)) {
                           purchaseProperty(&players[currentPlayer], currentPlayer, prop, properties, activeRegulation);
                        } else {
                              auctionProperty(players, prop);
                            }
                } else if (prop->owner != currentPlayer) {
                          payRent(&players[currentPlayer], currentPlayer, &players[prop->owner], prop->owner, prop, properties, diceRoll, players, powerFailureRounds > 0, activeEconomicEvent, activeRegulation);
                        }
                }

                if (landedSquare == 2 || landedSquare == 7 || landedSquare == 22 || landedSquare == 36) {
                     drawNationalEventCard(currentPlayer, players, properties, &powerFailureRounds, &labourStrikeRounds);
                    }
            
              if (landedSquare == 38) {  /* Bank of Ceylon */
                if (decideRepayLoan(&players[currentPlayer])) {
                    repayLoan(&players[currentPlayer]);
                } else if (decideTakeLoan(&players[currentPlayer])) {
                   takeLoan(&players[currentPlayer], currentPlayer, properties, activeEconomicEvent, activeRegulation);
                }
                }

               if (landedSquare == 4) {
                 payIncomeTax(&players[currentPlayer], currentPlayer, properties, activeRegulation);
                }

                if (landedSquare == 17 || landedSquare == 33) {  /* Insurance squares */
                int p;
                for (p = 0; p < 28; p++) {
                    if (properties[p].owner == currentPlayer && !properties[p].insured) {
                        purchaseInsurance(&players[currentPlayer], &properties[p], 0, activeEconomicEvent, activeRegulation);
                        break;
                    }
                }
            }

            if (decideMortgage(&players[currentPlayer])) {
               int p;
               for (p = 0; p < 28; p++) {
                  if (properties[p].owner == currentPlayer && !properties[p].mortgaged && properties[p].houses == 0) {
                     mortgageProperty(&players[currentPlayer], &properties[p]);
                     break;
                    }
                }
            }
           int buildTarget;
              if (labourStrikeRounds == 0 && shouldBuildHouse(&players[currentPlayer], currentPlayer, properties, &buildTarget)) {
                   buildHouse(&players[currentPlayer], &properties[buildTarget]);
                }

            printf("\n");
            printf("------------------------------------------------");
            printf("\n");
            }
        
            
       //round checkups
        accrueInterest(players);
        checkLoanDefault(players, properties);

        applyDepreciation(properties);
        applyBuildingDecay(properties);
       triggerInflation(round, properties, &currentInflationRate);
        triggerEconomicEvent(round, properties, &activeEconomicEvent, &economicEventRoundsLeft);
        triggerGovernmentRegulation(round, properties, players, &activeRegulation);
        triggerDisaster(round, players, properties);

        
        reviewPropertyMarket(round, properties, &currentBoomGroup, &currentDeclineGroup, groupLastAffected);
            if (round % 10 == 0) { marketBoomRoundsLeft = 10; marketDeclineRoundsLeft = 10; }
               else { if (marketBoomRoundsLeft > 0) marketBoomRoundsLeft--; 
                      if (marketDeclineRoundsLeft > 0) marketDeclineRoundsLeft--; }

        drawRegionalCard(round, &activeRegionalCard, &regionalCardRoundsLeft);
        recalculatePropertyValues(properties, currentBoomGroup, currentDeclineGroup, activeRegionalCard);

        printRoundSummary(round, players, properties);

        int currentLoanInterest = 8;  /* base rate; +/- from active event/regulation, see note below */
        if (activeEconomicEvent == 3) currentLoanInterest = currentLoanInterest * 115 / 100;
        else if (activeEconomicEvent == 4) currentLoanInterest = currentLoanInterest * 90 / 100;
        if (activeRegulation == 1) currentLoanInterest -= 2;

        printMarketConditions(currentBoomGroup, marketBoomRoundsLeft, currentDeclineGroup, marketDeclineRoundsLeft, activeRegionalCard, regionalCardRoundsLeft, currentInflationRate, currentLoanInterest);

        decrementEventTimers(players, properties, &powerFailureRounds, &labourStrikeRounds);

        if (countSolventPlayers(players) <= 1) {
            break;
        }
    }

    printf("========================================\n");
    printf("===== GAME OVER=====\n");

    int bestNetWorth = -999999;
    int winnerIndex = -1;

    for (i = 0; i < 4; i++) {
        if (!players[i].bankrupt) {
            int worth = calculateNetWorth(&players[i], i, properties);
            if (worth > bestNetWorth) {
                bestNetWorth = worth;
                winnerIndex = i;
            }
        }
    }

    printf("Winner\n%s\n", players[winnerIndex].name);
    printf("Total Cash\nLKR %d\n", players[winnerIndex].cash);

    int propertyValue = 0, j;
    for (j = 0; j < 28; j++) {
        if (properties[j].owner == winnerIndex) propertyValue += properties[j].purchasePrice;
    }
    printf("Total Property Value\nLKR %d\n", propertyValue);
    printf("Outstanding Loans\n%s\n", players[winnerIndex].hasLoan ? "Yes" : "None");
    printf("Net Worth\nLKR %d\n", bestNetWorth);
    printf("========================================\n");
}

void auctionProperty(Player players[], Property *prop) {
    int currentBid = (prop->purchasePrice * 50) / 100;
    int highestBidder = -1;
    int active[4];
    int i, rounds;

    for (i = 0; i < 4; i++) active[i] = !players[i].bankrupt;

    printf("Auction Started.\n");
    printf("Property :\n%s\n", prop->name);
    printf("Opening Bid :\nLKR %d.\n", currentBid);

    for (rounds = 0; rounds < 20; rounds++) {
        int anyBid = 0;
        for (i = 0; i < 4; i++) {
            if (!active[i]) continue;
            int bid = decideAuctionBid(&players[i], prop, currentBid);
            if (bid > currentBid) {
                currentBid = bid;
                highestBidder = i;
                anyBid = 1;
                printf("%s bids LKR %d.\n", players[i].name, currentBid);
            } else {
                active[i] = 0;
                printf("%s withdraws.\n", players[i].name);
            }
        }
        if (!anyBid) break;
    }

    if (highestBidder == -1) {
        printf("No bids received. Ownership remains with the Bank.\n");
        return;
    }

    prop->owner = highestBidder;
    players[highestBidder].cash -= currentBid;
    printf("%s wins the auction.\n", players[highestBidder].name);
}

int resolveJail(Player *player) {
    if (!player->inJail) return 0;

    if (player->cash >= 300) {
        player->cash -= 300;
        player->inJail = 0;
        player->jailTurns = 0;
        printf("%s pays bail of LKR 300 and is released from jail.\n", player->name);
        return 0;
    }

    int d1 = rollDice(), d2 = rollDice();
    if (d1 == d2) {
        player->inJail = 0;
        player->jailTurns = 0;
        printf("%s rolls doubles (%d,%d) and is released from jail.\n", player->name, d1, d2);
        return 0;
    }

    player->jailTurns++;
    if (player->jailTurns >= 3) {
        player->inJail = 0;
        player->jailTurns = 0;
        printf("%s has served 3 turns and is released from jail.\n", player->name);
        return 0;
    }

    printf("%s remains in jail (turn %d of 3).\n", player->name, player->jailTurns);
    return 1;
}
    