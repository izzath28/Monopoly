#include <stdio.h>
#include "types.h"

void purchaseProperty(Player *player, int playerIndex, Property *prop, Property properties[], int activeRegulation) {
    if (prop->owner != -1 || !shouldBuyProperty(player, prop)) return;

    if (activeRegulation == 7) {
        int undeveloped = 0, i;
        for (i = 0; i < 28; i++) {
            if (properties[i].owner == playerIndex && properties[i].houses == 0) undeveloped++;
        }
        if (undeveloped >= 3) return;  // Anti-Speculation Act: cap reached
    }

    player->cash -= prop->purchasePrice;
    prop->owner = playerIndex;
    printf("%s purchased %s for LKR %d.\n", player->name, prop->name, prop->purchasePrice);
    printf("Remaining Balance : LKR %d.\n", player->cash);
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
void payRent(Player *payer, int payerIndex, Player *owner, int ownerIndex, Property *prop, Property properties[], int diceRoll, Player players[], int powerFailureActive, int activeEconomicEvent, int activeRegulation) {
    int rent;
    if (prop->mortgaged) return;  
    if (prop->closedRoundsLeft > 0) return;  

    if (prop->group == GRP_RAILWAY) {
        int owned = countGroupOwned(properties, ownerIndex, GRP_RAILWAY);
        if (owned == 1)      rent = 250;
        else if (owned == 2) rent = 500;
        else if (owned == 3) rent = 1000;
        else                  rent = 2000;
        if (players[ownerIndex].eventCard == 1) rent *= 2;  // Fuel Shortage 
        if (activeEconomicEvent == 1) rent *= 2;  // Fuel Crisis 
        if (activeRegulation == 4) rent = rent * 125 / 100;  // Railway Modernization 

    } else if (prop->group == GRP_UTILITY) {
        int owned = countGroupOwned(properties, ownerIndex, GRP_UTILITY);
        if (owned >= 2) rent = diceRoll * 10;
        else             rent = diceRoll * 4;
        if (powerFailureActive) rent /= 2;  // Power Failure 
        if (activeRegulation == 5) rent = rent * 120 / 100;  // Electricity Tariff Revision

    } else {
    if (prop->houses == 0)      rent = prop->baseRent * 1;
    else if (prop->houses == 1) rent = prop->baseRent * 2;
    else if (prop->houses == 2) rent = prop->baseRent * 3;
    else if (prop->houses == 3) rent = prop->baseRent * 5;
    else if (prop->houses == 4) rent = prop->baseRent * 7;
    else                          rent = prop->baseRent * 10;
    if (players[ownerIndex].eventCard == 0 && prop->houses == 5) rent *= 2;        // Tourism Hype 
    else if (players[ownerIndex].eventCard == 13 && prop->houses == 5) rent = rent * 150 / 100;  // Festival Season

    // Economic Events
    if (activeEconomicEvent == 0 && prop->houses == 5) rent *= 2;              //Tourism Boom 
    else if (activeEconomicEvent == 7 && prop->houses == 5) rent = rent / 2;   // Political Unrest
    else if (activeEconomicEvent == 3) rent = rent * 90 / 100;                 // Economic Recession

    if (prop->houses > 0) {
    int conditionPct;
    if (prop->buildingCondition >= 90)      conditionPct = 100;
    else if (prop->buildingCondition >= 75) conditionPct = 90;
    else if (prop->buildingCondition >= 50) conditionPct = 75;
    else if (prop->buildingCondition >= 25) conditionPct = 50;
    else                                      conditionPct = 0;
    rent = rent * conditionPct / 100;
}
}

    payer->cash -= rent;
    owner->cash += rent;
    printf("%s landed on %s.\n", payer->name, prop->name);
    printf("Rent Paid : LKR %d.\n", rent);
    printf("Owner : %s.\n", owner->name);

    if (calculateNetWorth(payer, payerIndex, properties) < 0) {
       payer->bankrupt = 1;
       printf("%s has been declared bankrupt.\n", payer->name);
       releasePlayerAssets(payer, payerIndex, properties);
    }
}

void payIncomeTax(Player *player, int playerIndex, Property properties[], int activeRegulation) {
    int assets = calculateNetWorth(player, playerIndex, properties);
    int tax = (assets * 15) / 100;
    if (activeRegulation == 0) tax = tax * 150 / 100;  /* Increase Property Tax regulation */

    player->cash -= tax;
    printf("%s landed on Income Tax.\n", player->name);
    printf("Tax Paid : LKR %d.\n", tax);
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

void takeLoan(Player *player, int playerIndex, Property properties[], int activeEconomicEvent, int activeRegulation) {
    int amount = maxLoanAmount(player, playerIndex, properties);
    if (amount <= 0 || player->hasLoan) return;

    player->hasLoan = 1;
    player->loanAmount = amount;

    int rate = 80;
    if (player->eventCard == 7)       rate = 60;   // Interest Rate Cut card
    else if (player->eventCard == 8)  rate = 100;  // Interest Rate Increase card
    if (activeEconomicEvent == 3)     rate = rate * 115 / 100;  // Recession
    else if (activeEconomicEvent == 4) rate = rate * 90 / 100;   // Stock Boom

    if (activeRegulation == 1) rate = rate - 20; 
    if (rate < 10) rate = 10;

    player->loanInterestRate = rate;

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
    printf("Interest Rate : %d%%\n", rate / 10);
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
                int idx = players[i].collateral[j];
                properties[idx].owner = -1;
                properties[idx].houses = 0;
                properties[idx].mortgaged = 0;
                properties[idx].insured = 0;
                properties[idx].insuranceType = -1;
                properties[idx].insuranceRoundsLeft = 0;
                properties[idx].structuralDamage = 0;
                properties[idx].maintenanceSurchargePercent = 0;
                properties[idx].buildingCondition = 100;
            }

            players[i].hasLoan = 0;
            players[i].loanAmount = 0;
            players[i].numCollateral = 0;
        }
    }
}
void purchaseInsurance(Player *player, Property *prop, int type, int activeEconomicEvent, int activeRegulation) {
    int premium;
    if (type == 0)      premium = (prop->purchasePrice * 5) / 100;
    else if (type == 1) premium = (prop->purchasePrice * 10) / 100;
    else                  premium = (prop->purchasePrice * 15) / 100;

    if (player->eventCard == 15) premium = premium * 80 / 100; //Insurance discount card
    if (activeRegulation == 6) premium = premium * 85 / 100;  // Insurance Regulation 
    if (activeEconomicEvent == 2) premium = premium * 120 / 100; //Heavy monsoon

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

void applyBuildingDecay(Property properties[]) {
    int i;
    for (i = 0; i < 28; i++) {
        if (properties[i].houses > 0) {
            properties[i].buildingCondition -= 2;
            if (properties[i].buildingCondition < 0) properties[i].buildingCondition = 0;

            properties[i].roundsSinceMaintenance++;
            if (properties[i].roundsSinceMaintenance > 20 && !properties[i].structuralDamage) {
                properties[i].structuralDamage = 1;
                properties[i].maintenanceSurchargePercent = 50;
                printf("%s has suffered structural damage from lack of maintenance.\n", properties[i].name);
            }
        }
    }
}

void releasePlayerAssets(Player *player, int playerIndex, Property properties[]) {
    int i;
    for (i = 0; i < 28; i++) {
        if (properties[i].owner == playerIndex) {
            properties[i].owner = -1;
            properties[i].houses = 0;
            properties[i].mortgaged = 0;
            properties[i].insured = 0;
            properties[i].insuranceType = -1;
            properties[i].insuranceRoundsLeft = 0;
            properties[i].structuralDamage = 0;
            properties[i].maintenanceSurchargePercent = 0;
            properties[i].buildingCondition = 100;
        }
    }
    player->hasLoan = 0;
    player->loanAmount = 0;
    player->numCollateral = 0;
}

void performMaintenance(Player *player, Property *prop) {
    if (prop->houses == 0) return;
    int baseCost = (prop->houses == 5) ? (prop->hotelCost * 8) / 100 : (prop->houseCost * 5) / 100;
    int cost = baseCost * (100 + prop->maintenanceSurchargePercent) / 100;
    if (cost > player->cash) return;

    player->cash -= cost;
    prop->buildingCondition = 100;
    prop->roundsSinceMaintenance = 0;
    printf("%s performed maintenance on %s. Cost : LKR %d.\n", player->name, prop->name, cost);
}

void renovateDamagedBuilding(Player *player, Property *prop) {
    if (!prop->structuralDamage) return;
    int replacementValue = (prop->houses == 5) ? prop->hotelCost : prop->houses * prop->houseCost;
    int cost = (replacementValue * 25) / 100;
    if (cost > player->cash) return;

    player->cash -= cost;
    prop->structuralDamage = 0;
    prop->maintenanceSurchargePercent = 0;
    prop->buildingCondition = 100;
    prop->roundsSinceMaintenance = 0;
    printf("%s renovated %s, repairing structural damage. Cost : LKR %d.\n", player->name, prop->name, cost);
}