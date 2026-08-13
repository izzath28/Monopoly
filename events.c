#include <stdio.h>
#include <stdlib.h>
#include "types.h"

void applyDepreciation(Property properties[]) {
    int i;
    for (i = 0; i < 28; i++) {
        if (properties[i].owner == -1) continue;
        properties[i].age++;
        if (properties[i].age > 50 && properties[i].age % 5 == 0) {
            properties[i].purchasePrice = properties[i].purchasePrice * 99 / 100;
        }
    }
}

void triggerInflation(int round, Property properties[]) {
    if (round % 10 != 0) return;

    int rates[6] = {-3, 0, 2, 5, 8, 12};
    int rate = rates[rand() % 6];
    int i;

    for (i = 0; i < 28; i++) {
        properties[i].purchasePrice += (properties[i].purchasePrice * rate) / 100;
        properties[i].baseRent      += (properties[i].baseRent * rate) / 100;
        properties[i].houseCost     += (properties[i].houseCost * rate) / 100;
        properties[i].hotelCost     += (properties[i].hotelCost * rate) / 100;
    }

    printf("Inflation\n%+d%%\n", rate);
}

void triggerEconomicEvent(int round) {
    if (round % 15 != 0) return;

    const char *events[8] = {
        "Tourism Boom", "Fuel Crisis", "Heavy Monsoon", "Economic Recession",
        "Stock Market Boom", "Government Housing Programme", "Foreign Investment", "Political Unrest"
    };
    int idx = rand() % 8;
    printf("Economic Event\n%s\n", events[idx]);
}

void triggerGovernmentRegulation(int round) {
    if (round % 20 != 0) return;

    const char *regs[8] = {
        "Increase Property Tax", "Reduce Loan Interest", "Housing Subsidy",
        "Luxury Property Tax", "Railway Modernization", "Electricity Tariff Revision",
        "Insurance Regulation", "Anti-Speculation Act"
    };
    int idx = rand() % 8;
    printf("Government Regulation\n%s\n", regs[idx]);
}

void triggerDisaster(int round, Player players[], Property properties[]) {
    if (round % 10 != 0) return;

    const char *disasters[5] = {"Fire", "Flood", "Riot", "Building Collapse", "Electrical Failure"};
    int candidates[28], n = 0, i;

    for (i = 0; i < 28; i++) {
        if (properties[i].owner != -1 && properties[i].houses > 0) {
            candidates[n++] = i;
        }
    }
    if (n == 0) return;

    int chosen = candidates[rand() % n];
    int disasterIdx = rand() % 5;
    Property *p = &properties[chosen];
    int repairCost = (p->houses == 5) ? p->hotelCost : p->houseCost;

    printf("Disaster\n%s occurred.\n", disasters[disasterIdx]);
    printf("Affected Property :\n%s.\n", p->name);

    if (p->insured) {
        int pct = (p->insuranceType == 0) ? 80 : 100;
        int compensation = (repairCost * pct) / 100;
        players[p->owner].cash += compensation;
        printf("Insurance Claim Approved.\n");
        printf("Compensation Paid :\nLKR %d.\n", compensation);
    } else {
        players[p->owner].cash -= repairCost;
        printf("No insurance. %s bears the repair cost of LKR %d.\n", players[p->owner].name, repairCost);
    }
}

void triggerMarketReview(int round, Property properties[]) {
    if (round % 10 != 0) return;

    int boomGroup = rand() % 8;      /* 0-7 = the 8 color groups */
    int declineGroup;
    do {
        declineGroup = rand() % 8;
    } while (declineGroup == boomGroup);

    int i;
    for (i = 0; i < 28; i++) {
        if (properties[i].group == boomGroup) {
            properties[i].purchasePrice = properties[i].purchasePrice * 115 / 100;
            properties[i].mortgageValue = properties[i].mortgageValue * 115 / 100;
            properties[i].baseRent      = properties[i].baseRent * 125 / 100;
        } else if (properties[i].group == declineGroup) {
            properties[i].purchasePrice = properties[i].purchasePrice * 85 / 100;
            properties[i].mortgageValue = properties[i].mortgageValue * 90 / 100;
            properties[i].baseRent      = properties[i].baseRent * 80 / 100;
        }
    }

    printf("Property Market Review\n");
    printf("Market Boom : Group %d\n", boomGroup);
    printf("Market Decline : Group %d\n", declineGroup);
}