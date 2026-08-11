#include <stdio.h>
#include "types.h"

void purchaseProperty(Player *player, int playerIndex, Property *prop){
    if(prop->owner==-1 && player->cash >= prop->purchasePrice){
        player->cash -= prop->purchasePrice;
        prop->owner = playerIndex;
        printf("%s purchased %s for LKR %d.\n", player->name, prop->name, prop->purchasePrice);
        printf("Remaining Balance : LKR %d.\n", player->cash);
    }
}
void payRent(Player *payer, Player *owner, Property *prop) {
    int rent = prop->baseRent;
    payer->cash -= rent;
    owner->cash += rent;
    printf("%s landed on %s.\n", payer->name, prop->name);
    printf("Rent Paid : LKR %d.\n", rent);
    printf("Owner : %s.\n", owner->name);
}