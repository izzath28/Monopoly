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

void triggerInflation(int round, Property properties[], int *currentInflationRate) {
    if (round % 10 != 0) return;

    int rates[6] = {-3, 0, 2, 5, 8, 12};
    int rate = rates[rand() % 6];
    *currentInflationRate = rate;
    int i;

    for (i = 0; i < 28; i++) {
        properties[i].permPurchasePrice += (properties[i].permPurchasePrice * rate) / 100;
        properties[i].permBaseRent      += (properties[i].permBaseRent * rate) / 100;
        properties[i].permMortgageValue += (properties[i].permMortgageValue * rate) / 100;
        properties[i].houseCost         += (properties[i].houseCost * rate) / 100;
        properties[i].hotelCost         += (properties[i].hotelCost * rate) / 100;
    }

    printf("Inflation\n%+d%%\n", rate);
}
void triggerEconomicEvent(int round, Property properties[], int *activeEconomicEvent, int *economicEventRoundsLeft) {
    if (round % 15 == 0) {
        const char *events[8] = {"Tourism Boom", "Fuel Crisis", "Heavy Monsoon", "Economic Recession",
                                 "Stock Market Boom", "Government Housing Programme", "Foreign Investment", "Political Unrest"};
        int idx = rand() % 8;
        *activeEconomicEvent = idx;
        *economicEventRoundsLeft = 15;

        printf("Economic Event\n%s\n", events[idx]);

        int i;
        switch (idx) {
            case 0: //Torist boom
                for (i = 0; i < 28; i++)
                    if (properties[i].group == GRP_YELLOW) properties[i].permPurchasePrice = properties[i].permPurchasePrice * 115 / 100;
                printf("Hotels receive double rent. Southern properties increase 15%%.\n");
                break;
            case 1: //Fuel crisis
                for (i = 0; i < 28; i++) properties[i].houseCost = properties[i].houseCost * 120 / 100;
                printf("Railway rent doubles. Construction costs increase 20%%.\n");
                break;
            case 2: //Heavy monsoon
                for (i = 0; i < 28; i++)
                    if (properties[i].group == GRP_YELLOW) properties[i].permPurchasePrice = properties[i].permPurchasePrice * 90 / 100;
                printf("Flood risk and insurance premiums increase. Coastal properties lose 10%% value.\n");
                break;
            case 3: //Economic recession
                for (i = 0; i < 28; i++) properties[i].permPurchasePrice = properties[i].permPurchasePrice * 85 / 100;
                printf("Property values decrease 15%%. Rent decreases 10%%. Loan interest increases 15%%.\n");
                break;
            case 4: //Stock market boom
                for (i = 0; i < 28; i++) properties[i].permPurchasePrice = properties[i].permPurchasePrice * 110 / 100;
                printf("Property values increase 10%%. Loan interest decreases 10%%.\n");
                break;
            case 5: //Government housing programme
                for (i = 0; i < 28; i++) properties[i].houseCost = properties[i].houseCost * 75 / 100;
                printf("House construction costs reduce 25%%.\n");
                break;
            case 6: //Foreign investment
                for (i = 0; i < 28; i++)
                    if (properties[i].group == GRP_GREEN) properties[i].permPurchasePrice = properties[i].permPurchasePrice * 120 / 100;
                printf("Commercial properties increase 20%%.\n");
                break;
            default: //Political unrest
                printf("Riot probability doubles. Hotel rent drops 50%%. Business interruption claims increase.\n");
                break;
        }
    } else if (*economicEventRoundsLeft > 0) {
        (*economicEventRoundsLeft)--;
        if (*economicEventRoundsLeft == 0) *activeEconomicEvent = -1;
    }
}
void triggerGovernmentRegulation(int round, Property properties[], Player players[], int *activeRegulation) {
    if (round % 20 != 0) return;

    const char *regs[8] = {"Increase Property Tax", "Reduce Loan Interest", "Housing Subsidy","Luxury Property Tax", "Railway Modernization",
                           "Electricity Tariff Revision", "Insurance Regulation", "Anti-Speculation Act"};
    int idx = rand() % 8;
    *activeRegulation = idx;
    printf("Government Regulation\n%s\n", regs[idx]);

    int i;
    switch (idx) {
        case 0: printf("Income Tax increases by 50%%.\n"); break;
        case 1: printf("Loan interest decreases by 2%%.\n"); break;
        case 2:
            for (i = 0; i < 28; i++) properties[i].houseCost = properties[i].houseCost * 70 / 100;
            printf("House construction costs reduce 30%%.\n");
            break;
        case 3:
            for (i = 0; i < 28; i++) {
                if (properties[i].owner != -1 && properties[i].houses == 5) {
                    int maintenanceTax = (properties[i].purchasePrice * 25) / 100;
                    players[properties[i].owner].cash -= maintenanceTax;
                }
            }
            printf("Hotels incur a 25%% maintenance tax on property value.\n");
            break;
        case 4: printf("Railway rents increase 25%%.\n"); break;
        case 5: printf("Utility rents increase 20%%.\n"); break;
        case 6: printf("Insurance premiums decrease 15%%. Coverage unchanged.\n"); break;
        default: printf("Anti-Speculation Act: players may own at most three undeveloped properties.\n"); break;
    }
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

void reviewPropertyMarket(int round, Property properties[], int *boomGroup, int *declineGroup, int groupLastAffected[]) {
    if (round % 10 != 0) return;

    int candidates[8], n = 0, g;
    for (g = 0; g < 8; g++) {
        if (round - groupLastAffected[g] >= 30) {
            candidates[n++] = g;
        }
    }

    if (n < 1) {
        *boomGroup = -1;
        *declineGroup = -1;
        printf("Property Market Review\nNo groups eligible (all in cooldown).\n");
        return;
    }

    int chosenBoom = candidates[rand() % n];
    *boomGroup = chosenBoom;
    groupLastAffected[chosenBoom] = round;

    int declineCandidates[8], dn = 0;
    for (g = 0; g < n; g++) {
        if (candidates[g] != chosenBoom) declineCandidates[dn++] = candidates[g];
    }

    if (dn < 1) {
        *declineGroup = -1;
        printf("Property Market Review\n");
        printf("Market Boom : Group %d\n", chosenBoom);
        printf("Market Decline : None available (cooldown)\n");
        return;
    }

    int chosenDecline = declineCandidates[rand() % dn];
    *declineGroup = chosenDecline;
    groupLastAffected[chosenDecline] = round;

    printf("Property Market Review\n");
    printf("Market Boom : Group %d\n", chosenBoom);
    printf("Market Decline : Group %d\n", chosenDecline);
}

void recalculatePropertyValues(Property properties[], int boomGroup, int declineGroup, int activeRegionalCard) {
    int i;
    for (i = 0; i < 28; i++) {
        properties[i].purchasePrice = properties[i].permPurchasePrice;
        properties[i].mortgageValue = properties[i].permMortgageValue;
        properties[i].baseRent      = properties[i].permBaseRent;

        if (properties[i].group == boomGroup) {
            properties[i].purchasePrice = properties[i].purchasePrice * 115 / 100;
            properties[i].mortgageValue = properties[i].mortgageValue * 115 / 100;
            properties[i].baseRent      = properties[i].baseRent * 125 / 100;
        } else if (properties[i].group == declineGroup) {
            properties[i].purchasePrice = properties[i].purchasePrice * 85 / 100;
            properties[i].mortgageValue = properties[i].mortgageValue * 90 / 100;
            properties[i].baseRent      = properties[i].baseRent * 80 / 100;
        }
    

        if (properties[i].structuralDamage) {
             properties[i].purchasePrice = properties[i].purchasePrice * 85 / 100;
             properties[i].baseRent      = properties[i].baseRent * 75 / 100;
            }
    }
    
    if (activeRegionalCard != -1) {
        const char *name;
        int propList[4], valuePercent, rentPercent, k;
        getRegionalCardData(activeRegionalCard, &name, propList, &valuePercent, &rentPercent);

        for (k = 0; k < 4 && propList[k] != -1; k++) {
            int idx = propList[k];
            if (valuePercent != 0) properties[idx].purchasePrice = properties[idx].purchasePrice * (100 + valuePercent) / 100;
            if (rentPercent != 0)  properties[idx].baseRent      = properties[idx].baseRent * (100 + rentPercent) / 100;
        }
    }
}

void getRegionalCardData(int cardIdx, const char **name, int propList[4], int *valuePercent, int *rentPercent) {
    switch (cardIdx) {
        case 0:  
                 *name = "Southern Tourism Boom";      
                 propList[0]=14; 
                 propList[1]=15; 
                 propList[2]=16; 
                 propList[3]=-1; 
                 *valuePercent=0;  
                 *rentPercent=40;  
            break;
        case 1: 
                *name = "Port City Expansion";        
                propList[0]=0;  
                propList[1]=1;  
                propList[2]=22; 
                propList[3]=-1; 
                *valuePercent=25; 
                *rentPercent=0;   
            break;
        case 2: 
                *name = "IT Industry Growth";         
                propList[0]=5;  
                propList[1]=6;  
                propList[2]=7;  
                propList[3]=-1; 
                *valuePercent=20; 
                *rentPercent=0;   
            break;
        case 3: 
                *name = "Northern Development Programme"; 
                propList[0]=17; 
                propList[1]=18; 
                propList[2]=19; 
                propList[3]=-1; 
                *valuePercent=30; 
                *rentPercent=0; 
            break;
        case 4: 
                *name = "Tea Export Boom";            
                propList[0]=20; 
                propList[1]=-1; 
                propList[2]=-1; 
                propList[3]=-1; 
                *valuePercent=35; 
                *rentPercent=0;   
            break;
        case 5: 
                *name = "Airport Expansion";          
                propList[0]=8;  
                propList[1]=9;  
                propList[2]=10; 
                propList[3]=-1; 
                *valuePercent=0;  
                *rentPercent=30;  
            break;
        case 6: 
                *name = "University City Growth";     
                propList[0]=11; 
                propList[1]=12; 
                propList[2]=-1; 
                propList[3]=-1; 
                *valuePercent=20; 
                *rentPercent=0;   
            break;
        case 7: 
                *name = "Beach Pollution";             
                propList[0]=14; 
                propList[1]=15; 
                propList[2]=16; 
                propList[3]=-1; 
                *valuePercent=0;  
                *rentPercent=-30; 
            break;
        case 8: 
                *name = "Flood Damage";                
                propList[0]=14; 
                propList[1]=15; 
                propList[2]=16; 
                propList[3]=-1; 
                *valuePercent=-20; 
                *rentPercent=0;  
            break;
        case 9: 
                *name = "Transport Strike";            
                propList[0]=22; 
                propList[1]=23; 
                propList[2]=24; 
                propList[3]=25; 
                *valuePercent=0; 
                *rentPercent=-40; 
            break;
        case 10: 
                *name = "Electricity Tariff Increase"; 
                propList[0]=26; 
                propList[1]=27; 
                propList[2]=-1; 
                propList[3]=-1; 
                *valuePercent=0; 
                *rentPercent=25; 
            break;
        default:
                *name = "Water Shortage";             
                propList[0]=27; 
                propList[1]=6;  
                propList[2]=7;  
                propList[3]=8;  
                *valuePercent=0; 
                *rentPercent=20; 
            break;
    }
}
void drawRegionalCard(int round, int *activeCard, int *cardRoundsLeft) {
    if (round % 15 == 0) {
        *activeCard = rand() % 12;
        *cardRoundsLeft = 15;

        const char *name;
        int propList[4], valuePercent, rentPercent;
        getRegionalCardData(*activeCard, &name, propList, &valuePercent, &rentPercent);

        printf("Regional Development Card\n%s\n", name);
    } else if (*cardRoundsLeft > 0) {
        (*cardRoundsLeft)--;
        if (*cardRoundsLeft == 0) {
            *activeCard = -1;
        }
    }
}

void drawNationalEventCard(int currentPlayer, Player players[], Property properties[],
                            int *powerFailureRounds, int *labourStrikeRounds) {
    const char *cards[20] = {
        "Tourism Hype", "Fuel Shortage", "Heavy Floods", "Political Rally",
        "Stock Market Rise", "Economic Downturn", "Housing Subsidy", "Interest Rate Cut",
        "Interest Rate Increase", "Tax Amnesty", "Power Failure", "Foreign Funding",
        "Port Expansion", "Festival Season", "Labour Strike", "Insurance Discount",
        "Property Revaluation", "Currency Depreciation", "Government Grant", "National Disaster"
    };

    int card = rand() % 20;
    printf("National Event Card\n%s\n", cards[card]);

    switch (card) {
        case 0: /* Tourism Hype: hotels earn double rent, 5 rounds */
            players[currentPlayer].eventCard = 0;
            players[currentPlayer].eventCardRounds = 5;
            printf("Hotels earn double rent for %s, for 5 rounds.\n", players[currentPlayer].name);
            break;

        case 1: /* Fuel Shortage: railway rent doubles, 5 rounds */
            players[currentPlayer].eventCard = 1;
            players[currentPlayer].eventCardRounds = 5;
            printf("Railway rent doubles for %s, for 5 rounds.\n", players[currentPlayer].name);
            break;

        case 2: /* Heavy Floods: random coastal (Yellow group) property damaged */
        {
            int candidates[28], n = 0, i;
            for (i = 0; i < 28; i++) {
                if (properties[i].group == GRP_YELLOW && properties[i].owner != -1) {
                    candidates[n++] = i;
                }
            }
            if (n > 0) {
                int chosen = candidates[rand() % n];
                Property *p = &properties[chosen];
                int repairCost = p->purchasePrice / 10;
                if (p->insured) {
                    int pct = (p->insuranceType == 0) ? 80 : 100;
                    players[p->owner].cash += (repairCost * pct) / 100;
                    printf("%s damaged. Insurance claim approved.\n", p->name);
                } else {
                    players[p->owner].cash -= repairCost;
                    printf("%s damaged. Owner bears repair cost of LKR %d.\n", p->name, repairCost);
                }
            }
            break;
        }

        case 3: /* Political Rally: one random property closed for 2 rounds */
        {
            int candidates[28], n = 0, i;
            for (i = 0; i < 28; i++) {
                if (properties[i].owner != -1) candidates[n++] = i;
            }
            if (n > 0) {
                int chosen = candidates[rand() % n];
                properties[chosen].closedRoundsLeft = 2;
                printf("%s is closed for 2 rounds.\n", properties[chosen].name);
            }
            break;
        }

        case 4: /* Stock Market Rise: all property values +10% */
        {
            int i;
            for (i = 0; i < 28; i++) properties[i].permPurchasePrice = properties[i].permPurchasePrice * 110 / 100;
            printf("All property values increase by 10%%.\n");
            break;
        }

        case 5: /* Economic Downturn: property values -15% */
        {
            int i;
            for (i = 0; i < 28; i++) properties[i].permPurchasePrice = properties[i].permPurchasePrice * 85 / 100;
            printf("Property values decrease by 15%%.\n");
            break;
        }

        case 6: /* Housing Subsidy: house cost -30% */
        {
            int i;
            for (i = 0; i < 28; i++) properties[i].houseCost = properties[i].houseCost * 70 / 100;
            printf("House construction cost reduced by 30%%.\n");
            break;
        }

        case 7: /* Interest Rate Cut: this player's future loans get -2%, 15 rounds */
            players[currentPlayer].eventCard = 7;
            players[currentPlayer].eventCardRounds = 15;
            printf("%s's future loan interest reduced by 2%%.\n", players[currentPlayer].name);
            break;

        case 8: /* Interest Rate Increase: this player's future loans get +2%, 15 rounds */
            players[currentPlayer].eventCard = 8;
            players[currentPlayer].eventCardRounds = 15;
            printf("%s's future loan interest increased by 2%%.\n", players[currentPlayer].name);
            break;

        case 9: /* Tax Amnesty: everyone +2000 */
        {
            int i;
            for (i = 0; i < 4; i++) if (!players[i].bankrupt) players[i].cash += 2000;
            printf("Each player receives LKR 2,000.\n");
            break;
        }

        case 10: /* Power Failure: utility income halved, system-wide, 3 rounds */
            *powerFailureRounds = 3;
            printf("Utility income halved for 3 rounds.\n");
            break;

        case 11: /* Foreign Funding: commercial (Green) property values +15% */
        {
            int i;
            for (i = 0; i < 28; i++)
                if (properties[i].group == GRP_GREEN) properties[i].permPurchasePrice = properties[i].permPurchasePrice * 115 / 100;
            printf("Commercial property values increase by 15%%.\n");
            break;
        }

        case 12: /* Port Expansion: railway station values +20% */
        {
            int i;
            for (i = 0; i < 28; i++)
                if (properties[i].group == GRP_RAILWAY) properties[i].permPurchasePrice = properties[i].permPurchasePrice * 120 / 100;
            printf("Railway station values increase by 20%%.\n");
            break;
        }

        case 13: /* Festival Season: hotels get +50% rent, this player, 15 rounds */
            players[currentPlayer].eventCard = 13;
            players[currentPlayer].eventCardRounds = 15;
            printf("%s's hotels receive 50%% additional rent for 15 rounds.\n", players[currentPlayer].name);
            break;

        case 14: /* Labour Strike: construction suspended, system-wide, 2 rounds */
            *labourStrikeRounds = 2;
            printf("Construction suspended for 2 rounds.\n");
            break;

        case 15: /* Insurance Discount: this player's premiums -20%, 15 rounds */
            players[currentPlayer].eventCard = 15;
            players[currentPlayer].eventCardRounds = 15;
            printf("%s's insurance premiums reduced by 20%% for 15 rounds.\n", players[currentPlayer].name);
            break;

        case 16: /* Property Revaluation: random group +15% */
        {
            int g = rand() % 8, i;
            for (i = 0; i < 28; i++)
                if (properties[i].group == g) properties[i].permPurchasePrice = properties[i].permPurchasePrice * 115 / 100;
            printf("A random property group appreciates by 15%%.\n");
            break;
        }

        case 17: /* Currency Depreciation: construction costs +10% */
        {
            int i;
            for (i = 0; i < 28; i++) properties[i].houseCost = properties[i].houseCost * 110 / 100;
            printf("Construction costs increase by 10%%.\n");
            break;
        }

        case 18: /* Government Grant: random player +5000 */
        {
            int i = rand() % 4;
            if (!players[i].bankrupt) {
                players[i].cash += 5000;
                printf("%s receives LKR 5,000.\n", players[i].name);
            }
            break;
        }

        case 19: /* National Disaster: random developed property damaged, any owner */
        {
            int candidates[28], n = 0, i;
            for (i = 0; i < 28; i++) {
                if (properties[i].owner != -1 && properties[i].houses > 0) candidates[n++] = i;
            }
            if (n > 0) {
                int chosen = candidates[rand() % n];
                Property *p = &properties[chosen];
                int repairCost = (p->houses == 5) ? p->hotelCost : p->houseCost;
                if (p->insured) {
                    int pct = (p->insuranceType == 0) ? 80 : 100;
                    players[p->owner].cash += (repairCost * pct) / 100;
                    printf("%s damaged. Insurance claim approved.\n", p->name);
                } else {
                    players[p->owner].cash -= repairCost;
                    printf("%s damaged. Owner bears repair cost of LKR %d.\n", p->name, repairCost);
                }
            }
            break;
        }
    }
}

void decrementEventTimers(Player players[], Property properties[], int *powerFailureRounds, int *labourStrikeRounds) {
    int i;
    for (i = 0; i < 4; i++) {
        if (players[i].eventCardRounds > 0) {
            players[i].eventCardRounds--;
            if (players[i].eventCardRounds == 0) players[i].eventCard = -1;
        }
    }
    for (i = 0; i < 28; i++) {
        if (properties[i].closedRoundsLeft > 0) properties[i].closedRoundsLeft--;
    }
    if (*powerFailureRounds > 0) (*powerFailureRounds)--;
    if (*labourStrikeRounds > 0) (*labourStrikeRounds)--;
}