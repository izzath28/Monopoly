#include <stdio.h>
#include "types.h"

void purchaseProperty(Player *player, int playerIndex, Property *prop) {
    if (prop->owner == -1 && shouldBuyProperty(player, prop)) {
        player->cash -= prop->purchasePrice;
        prop->owner = playerIndex;
        printf("%s purchased %s for LKR %d.\n", player->name, prop->name, prop->purchasePrice);
        printf("Remaining Balance : LKR %d.\n", player->cash);
    }
}
int calculateNetWorth(Player *player, int playerIndex, Property properties[]) {
    int worth = player->cash;
    int i;
    for (i = 0; i < 28; i++) {
        if (properties[i].owner == playerIndex) {
            worth += properties[i].purchasePrice;
        }
    }
    return worth;
}
void payRent(Player *payer, int payerIndex, Player *owner, int ownerIndex, Property *prop, Property properties[], int diceRoll) {
    int rent;
    if (prop->mortgaged) return;  

    if (prop->group == GRP_RAILWAY) {
        int owned = countGroupOwned(properties, ownerIndex, GRP_RAILWAY);
        if (owned == 1)      rent = 250;
        else if (owned == 2) rent = 500;
        else if (owned == 3) rent = 1000;
        else                  rent = 2000;
    } else if (prop->group == GRP_UTILITY) {
        int owned = countGroupOwned(properties, ownerIndex, GRP_UTILITY);
        if (owned >= 2) rent = diceRoll * 10;
        else             rent = diceRoll * 4;
    } else {
    if (prop->houses == 0)      rent = prop->baseRent * 1;
    else if (prop->houses == 1) rent = prop->baseRent * 2;
    else if (prop->houses == 2) rent = prop->baseRent * 3;
    else if (prop->houses == 3) rent = prop->baseRent * 5;
    else if (prop->houses == 4) rent = prop->baseRent * 7;
    else                          rent = prop->baseRent * 10;
}

    payer->cash -= rent;
    owner->cash += rent;
    printf("%s landed on %s.\n", payer->name, prop->name);
    printf("Rent Paid : LKR %d.\n", rent);
    printf("Owner : %s.\n", owner->name);

    if (calculateNetWorth(payer, payerIndex, properties) < 0) {
        payer->bankrupt = 1;
        printf("%s has been declared bankrupt.\n", payer->name);
    }
}

int countGroupOwned(Property properties[], int ownerIndex, PropertyGroup group) {
    int count = 0;
    int i;
    for (i = 0; i < 28; i++) {
        if (properties[i].owner == ownerIndex && properties[i].group == group) {
            count++;
        }
    }
    return count;
}

int hasMonopoly(Property properties[], int ownerIndex, PropertyGroup group) {
    int i, total = 0, owned = 0;
    if (group == GRP_RAILWAY || group == GRP_UTILITY) return 0;

    for (i = 0; i < 28; i++) {
        if (properties[i].group == group) {
            total++;
            if (properties[i].owner == ownerIndex) owned++;
        }
    }
    return (total > 0 && total == owned);
}

void buildHouse(Player *player, Property *prop) {
    if (prop->houses < 4) {
        player->cash -= prop->houseCost;
        prop->houses++;
        printf("%s constructed one house on %s.\n", player->name, prop->name);
        printf("Construction Cost : LKR %d.\n", prop->houseCost);
    } else if (prop->houses == 4) {
        player->cash -= prop->hotelCost;
        prop->houses = 5;
        printf("%s upgraded %s to a Hotel.\n", player->name, prop->name);
    }
}

int maxLoanAmount(Player *player, int playerIndex, Property properties[]) {
    int total = 0, i;
    for (i = 0; i < 28; i++) {
        if (properties[i].owner == playerIndex && !properties[i].mortgaged) {
            total += properties[i].mortgageValue;
        }
    }
    return (total * 75) / 100;
}

void takeLoan(Player *player, int playerIndex, Property properties[]) {
    int amount = maxLoanAmount(player, playerIndex, properties);
    if (amount <= 0 || player->hasLoan) return;

    player->hasLoan = 1;
    player->loanAmount = amount;
    player->loanInterestRate = 80;   /* 8.0% */
    player->loanRoundsLeft = 20;
    player->numCollateral = 0;

    int i;
    for (i = 0; i < 28; i++) {
        if (properties[i].owner == playerIndex && !properties[i].mortgaged) {
            player->collateral[player->numCollateral] = i;
            player->numCollateral++;
        }
    }
    player->cash += amount;

    printf("%s obtained a secured loan.\n", player->name);
    printf("Loan Amount : LKR %d.\n", amount);
    printf("Interest Rate : 8%%\n");
    printf("Duration : 20 Rounds\n");
}

void repayLoan(Player *player) {
    if (!player->hasLoan) return;
    int amount = player->loanAmount < player->cash ? player->loanAmount : player->cash;
    player->cash -= amount;
    player->loanAmount -= amount;
    printf("%s repaid LKR %d.\n", player->name, amount);
    printf("Outstanding Balance : LKR %d.\n", player->loanAmount);

    if (player->loanAmount <= 0) {
        player->hasLoan = 0;
        player->loanAmount = 0;
        player->numCollateral = 0;
    }
}

void accrueInterest(Player players[]) {
    int i;
    for (i = 0; i < 4; i++) {
        if (players[i].hasLoan) {
            players[i].loanAmount += (players[i].loanAmount * players[i].loanInterestRate) / 1000;
            players[i].loanRoundsLeft--;
        }
    }
}

void checkLoanDefault(Player players[], Property properties[]) {
    int i, j;
    for (i = 0; i < 4; i++) {
        if (players[i].hasLoan && players[i].loanRoundsLeft <= 0) {
            printf("%s has defaulted.\n", players[i].name);
            printf("Collateral has been foreclosed.\n");
            printf("Outstanding debt cleared.\n");
            for (j = 0; j < players[i].numCollateral; j++) {
                properties[players[i].collateral[j]].owner = -1;
                properties[players[i].collateral[j]].houses = 0;
            }
            players[i].hasLoan = 0;
            players[i].loanAmount = 0;
            players[i].numCollateral = 0;
        }
    }
}

void purchaseInsurance(Player *player, Property *prop, int type) {
    int premium;
    if (type == 0)      premium = (prop->purchasePrice * 5) / 100;
    else if (type == 1) premium = (prop->purchasePrice * 10) / 100;
    else                  premium = (prop->purchasePrice * 15) / 100;

    if (premium > player->cash) return;

    player->cash -= premium;
    prop->insured = 1;
    prop->insuranceType = type;
    prop->insuranceRoundsLeft = 20;

    printf("Insurance purchased.\n");
    printf("Property : %s\n", prop->name);
    printf("Premium : LKR %d.\n", premium);
}

void printRoundSummary(int round, Player players[], Property properties[]) {
    printf("========================================\n");
    printf("===== Round %d Summary\n", round);
    printf("========================================\n");

    int i, j;
    for (i = 0; i < 4; i++) {
        int propCount = 0, hotels = 0;
        for (j = 0; j < 28; j++) {
            if (properties[j].owner == i) {
                propCount++;
                if (properties[j].houses == 5) hotels++;
            }
        }

        printf("%s\n", players[i].name);
        if (players[i].bankrupt) {
            printf("BANKRUPT\n");
        } else {
            printf("Cash : LKR %d\n", players[i].cash);
            printf("Net Worth : LKR %d\n", calculateNetWorth(&players[i], i, properties));
            printf("Properties : %d\n", propCount);
            printf("Hotels : %d\n", hotels);
            if (players[i].hasLoan) {
                printf("Outstanding Loan : LKR %d\n", players[i].loanAmount);
            } else {
                printf("Outstanding Loan : None\n");
            }
        }
        printf("---------------------------------------------\n");
    }
    printf("========================================\n");
}

void mortgageProperty(Player *player, Property *prop) {
    if (prop->owner == -1 || prop->mortgaged || prop->houses > 0) return;

    prop->mortgaged = 1;
    player->cash += prop->mortgageValue;
    printf("%s mortgaged %s for LKR %d.\n", player->name, prop->name, prop->mortgageValue);
}

void unmortgageProperty(Player *player, Property *prop) {
    int cost = (prop->mortgageValue * 110) / 100;  /* 10% interest to lift a mortgage, common Monopoly convention */
    if (!prop->mortgaged || cost > player->cash) return;

    prop->mortgaged = 0;
    player->cash -= cost;
    printf("%s paid LKR %d to lift the mortgage on %s.\n", player->name, cost, prop->name);
}