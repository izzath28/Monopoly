#include <stdio.h>
#include <string.h>
#include "types.h"

void inBoard(Square board[]){   
    for(int i=0; i<40; i++){
        board[i].name[0] = '\0';
        board[i].type = SQR_GO;
        board[i].propertyindex = -1; // Initialize propertyindex to -1 for non-property squares
    }

    strcpy(board[0].name, "Go");                          board[0].type = SQR_GO;         
    strcpy(board[1].name, "Pettah");                      board[1].type = SQR_PROPERTY;    board[1].propertyindex = 0; // Index of the property in the properties array
    strcpy(board[2].name, "Community Development Fund");  board[2].type = SQR_EVENT;      
    strcpy(board[3].name, "Maradana");                    board[3].type = SQR_PROPERTY;    board[3].propertyindex = 1;
    strcpy(board[4].name, "Income Tax");                  board[4].type = SQR_TAX;        
    strcpy(board[5].name, "Colombo Fort Railway Station"); board[5].type = SQR_RAILWAY;    board[5].propertyindex = 22; 
    strcpy(board[6].name, "Bambalapitiya");               board[6].type = SQR_PROPERTY;    board[6].propertyindex = 2;
    strcpy(board[7].name, "National Event Card");         board[7].type = SQR_EVENT;    
    strcpy(board[8].name, "Wellawatte");                  board[8].type = SQR_PROPERTY;    board[8].propertyindex = 3;
    strcpy(board[9].name, "Mount Lavinia");               board[9].type = SQR_PROPERTY;    board[9].propertyindex = 4;
    strcpy(board[10].name, "Jail/Jail visiting");         board[10].type = SQR_SPECIAL;   
    strcpy(board[11].name, "Nugegoda");                   board[11].type = SQR_PROPERTY;   board[11].propertyindex = 5;
    strcpy(board[12].name, "Ceylon Electricity Board");   board[12].type = SQR_UTILITY;    board[12].propertyindex = 26;
    strcpy(board[13].name, "Maharagama");                 board[13].type = SQR_PROPERTY;   board[13].propertyindex = 6;
    strcpy(board[14].name, "Kottawa");                    board[14].type = SQR_PROPERTY;   board[14].propertyindex = 7;
    strcpy(board[15].name, "Kandy Railway Station");      board[15].type = SQR_RAILWAY;    board[15].propertyindex = 23;
    strcpy(board[16].name, "Negambo");                    board[16].type = SQR_PROPERTY;   board[16].propertyindex = 8;
    strcpy(board[17].name, "Sri Lanka Insurance");        board[17].type = SQR_INSURANCE;  
    strcpy(board[18].name, "Katunayake");                 board[18].type = SQR_PROPERTY;   board[18].propertyindex = 9;
    strcpy(board[19].name, "Ja-Ela");                     board[19].type = SQR_PROPERTY;   board[19].propertyindex = 10;
    strcpy(board[20].name, "Free Parking");               board[20].type = SQR_SPECIAL;   
    strcpy(board[21].name, "Kandy City");                 board[21].type = SQR_PROPERTY;   board[21].propertyindex = 11;
    strcpy(board[22].name, "National Event Card");        board[22].type = SQR_EVENT;      
    strcpy(board[23].name, "Peradeniya");                 board[23].type = SQR_PROPERTY;   board[23].propertyindex = 12;
    strcpy(board[24].name, "Katugastota");                board[24].type = SQR_PROPERTY;   board[24].propertyindex = 13;
    strcpy(board[25].name, "Galle Railway Station");      board[25].type = SQR_RAILWAY;    board[25].propertyindex = 24;
    strcpy(board[26].name, "Galle Fort");                 board[26].type = SQR_PROPERTY;   board[26].propertyindex = 14;
    strcpy(board[27].name, "Unawatuna");                  board[27].type = SQR_PROPERTY;   board[27].propertyindex = 15;
    strcpy(board[28].name, "National Water Supply and Drainage Board"); board[28].type = SQR_UTILITY;     board[28].propertyindex = 27;
    strcpy(board[29].name, "Hikkaduwa");                  board[29].type = SQR_PROPERTY;   board[29].propertyindex = 16;
    strcpy(board[30].name, "Go to Jail");                 board[30].type = SQR_SPECIAL;    
    strcpy(board[31].name, "Jaffna Town");                board[31].type = SQR_PROPERTY;   board[31].propertyindex = 17;
    strcpy(board[32].name, "Nallur");                     board[32].type = SQR_PROPERTY;   board[32].propertyindex = 18;
    strcpy(board[33].name, "Ceylinco Insurance");         board[33].type = SQR_INSURANCE; 
    strcpy(board[34].name, "Trincomalee");                board[34].type = SQR_PROPERTY;   board[34].propertyindex = 19;
    strcpy(board[35].name, "Jaffna Railway Station");     board[35].type = SQR_RAILWAY;    board[35].propertyindex = 25;
    strcpy(board[36].name, "National Event Card");        board[36].type = SQR_EVENT;    
    strcpy(board[37].name, "Nuwara Eliya");               board[37].type = SQR_PROPERTY;   board[37].propertyindex = 20;
    strcpy(board[38].name, "Bank of Ceylon");             board[38].type = SQR_BANK;       
    strcpy(board[39].name, "Galle Face");                 board[39].type = SQR_PROPERTY;   board[39].propertyindex = 21;
}   

const char *square_type_name(SquareType t){
    switch(t){
        case SQR_GO:
              return "GO";
        case SQR_PROPERTY:
              return "Property";
        case SQR_RAILWAY:
              return "Railway";
        case SQR_UTILITY:
              return "Utility";
        case SQR_EVENT:
              return "Event";
        case SQR_TAX:
              return "Tax";
        case SQR_SPECIAL:
              return "Special";
        case SQR_INSURANCE:
              return "Insurance";
        case SQR_BANK:
              return "Bank";
                }
        return "Unknown";
            }

void inProperties(Property properties[]){
    int i;
    for(i=0; i<28; i++){
        properties[i].name[0] = '\0';
        properties[i].owner = -1; //Banks owes it initially
        properties[i].mortgaged = 0; //Not morgaged initially
        properties[i].insured = 0; //Not insured initially
        properties[i].insuranceType = -1;
        properties[i].insuranceRoundsLeft = 0;
        properties[i].houses = 0; //No houses initially
        properties[i].age = 0;
    }
    
    strcpy(properties[0].name, "Pettah");             properties[0].group = GRP_BROWN; 
     properties[0].purchasePrice = 1500;             properties[0].baseRent = 100;     properties[0].mortgageValue = 750; properties[0].houseCost = 50; properties[0].hotelCost = 50;
    strcpy(properties[1].name, "Maradana");           properties[1].group = GRP_BROWN; 
     properties[1].purchasePrice = 1800;             properties[1].baseRent = 120;     properties[1].mortgageValue = 750; properties[1].houseCost = 50; properties[1].hotelCost = 50;
    strcpy(properties[2].name, "Bambalapitiya");      properties[2].group = GRP_LIGHT_BLUE;
     properties[2].purchasePrice = 2500;             properties[2].baseRent = 180;     properties[2].mortgageValue = 1000; properties[2].houseCost = 100; properties[2].hotelCost = 100;
    strcpy(properties[3].name, "Wellawatte");         properties[3].group = GRP_LIGHT_BLUE;
     properties[3].purchasePrice = 2700;             properties[3].baseRent = 200;     properties[3].mortgageValue = 1000; properties[3].houseCost = 100; properties[3].hotelCost = 100;
    strcpy(properties[4].name, "Mount Lavinia");      properties[4].group = GRP_LIGHT_BLUE; 
     properties[4].purchasePrice = 3000;             properties[4].baseRent = 220;     properties[4].mortgageValue = 1000; properties[4].houseCost = 100; properties[4].hotelCost = 100;
    strcpy(properties[5].name, "Nugegoda");           properties[5].group = GRP_PINK; 
     properties[5].purchasePrice = 3500;             properties[5].baseRent = 260;     properties[5].mortgageValue = 1500; properties[5].houseCost = 150; properties[5].hotelCost = 150; 
    strcpy(properties[6].name, "Maharagama");         properties[6].group = GRP_PINK; 
     properties[6].purchasePrice = 3800;             properties[6].baseRent = 280;     properties[6].mortgageValue = 1500; properties[6].houseCost = 150; properties[6].hotelCost = 150;
    strcpy(properties[7].name, "Kottawa");            properties[7].group = GRP_PINK; 
     properties[7].purchasePrice = 4000;             properties[7].baseRent = 300;     properties[7].mortgageValue = 1500; properties[7].houseCost = 150; properties[7].hotelCost = 150;
    strcpy(properties[8].name, "Negambo");            properties[8].group = GRP_ORANGE;
     properties[8].purchasePrice = 4500;             properties[8].baseRent = 350;     properties[8].mortgageValue = 2000; properties[8].houseCost = 200; properties[8].hotelCost = 200; 
    strcpy(properties[9].name, "Katunayake");         properties[9].group = GRP_ORANGE;
     properties[9].purchasePrice = 4700;             properties[9].baseRent = 370;     properties[9].mortgageValue = 2000; properties[9].houseCost = 200; properties[9].hotelCost = 200;
    strcpy(properties[10].name, "Ja-Ela");            properties[10].group = GRP_ORANGE; 
     properties[10].purchasePrice = 5000;            properties[10].baseRent = 400;    properties[10].mortgageValue = 2000; properties[10].houseCost = 200; properties[10].hotelCost = 200; 
    strcpy(properties[11].name, "Kandy City");        properties[11].group = GRP_RED; 
     properties[11].purchasePrice = 5500;            properties[11].baseRent = 450;    properties[11].mortgageValue = 2500; properties[11].houseCost = 250; properties[11].hotelCost = 250;
    strcpy(properties[12].name, "Peradeniya");        properties[12].group = GRP_RED; 
     properties[12].purchasePrice = 5800;            properties[12].baseRent = 480;    properties[12].mortgageValue = 2500; properties[12].houseCost = 250; properties[12].hotelCost = 250;
    strcpy(properties[13].name, "Katugastota");       properties[13].group = GRP_RED; 
     properties[13].purchasePrice = 6000;            properties[13].baseRent = 500;    properties[13].mortgageValue = 2500; properties[13].houseCost = 250; properties[13].hotelCost = 250;
    strcpy(properties[14].name, "Galle Fort");        properties[14].group = GRP_YELLOW;
     properties[14].purchasePrice = 6500;            properties[14].baseRent = 600;    properties[14].mortgageValue = 3000; properties[14].houseCost = 300; properties[14].hotelCost = 300;
    strcpy(properties[15].name, "Unawatuna");         properties[15].group = GRP_YELLOW;
     properties[15].purchasePrice = 6800;            properties[15].baseRent = 620;    properties[15].mortgageValue = 3000; properties[15].houseCost = 300; properties[15].hotelCost = 300;
    strcpy(properties[16].name, "Hikkaduwa");         properties[16].group = GRP_YELLOW; 
     properties[16].purchasePrice = 7000;            properties[16].baseRent = 650;    properties[16].mortgageValue = 3000; properties[16].houseCost = 300; properties[16].hotelCost = 300;
    strcpy(properties[17].name, "Jaffna Town");       properties[17].group = GRP_GREEN;  
     properties[17].purchasePrice = 8000;            properties[17].baseRent = 750;    properties[17].mortgageValue = 3500; properties[17].houseCost = 350; properties[17].hotelCost = 350;
    strcpy(properties[18].name, "Nallur");            properties[18].group = GRP_GREEN;
     properties[18].purchasePrice = 8300;            properties[18].baseRent = 780;    properties[18].mortgageValue = 3500; properties[18].houseCost = 350; properties[18].hotelCost = 350;
    strcpy(properties[19].name, "Trincomalee");       properties[19].group = GRP_GREEN; 
     properties[19].purchasePrice = 8500;            properties[19].baseRent = 800;    properties[19].mortgageValue = 3500; properties[19].houseCost = 350; properties[19].hotelCost = 350; 
    strcpy(properties[20].name, "Nuwara Eliya");      properties[20].group = GRP_DARK_BLUE;
      properties[20].purchasePrice = 10000;           properties[20].baseRent = 1000;   properties[20].mortgageValue = 4000; properties[20].houseCost = 400; properties[20].hotelCost = 400;
    strcpy(properties[21].name, "Galle Face");        properties[21].group = GRP_DARK_BLUE; 
     properties[21].purchasePrice = 12000;           properties[21].baseRent = 1200;   properties[21].mortgageValue = 5000; properties[21].houseCost = 3000; properties[21].hotelCost = 12000;
    strcpy(properties[22].name, "Colombo Fort Railway Station"); properties[22].group = GRP_RAILWAY; 
     properties[22].purchasePrice = 2000;            properties[22].baseRent = 250;    properties[22].mortgageValue = 2000; properties[22].houseCost = 0; properties[22].hotelCost = 0;
    strcpy(properties[23].name, "Kandy Railway Station"); properties[23].group = GRP_RAILWAY;
     properties[23].purchasePrice = 2000;            properties[23].baseRent = 250;    properties[23].mortgageValue = 2000; properties[23].houseCost = 0; properties[23].hotelCost = 0;
    strcpy(properties[24].name, "Galle Railway Station"); properties[24].group = GRP_RAILWAY; 
     properties[24].purchasePrice = 2000;            properties[24].baseRent = 250;    properties[24].mortgageValue = 2000; properties[24].houseCost = 0; properties[24].hotelCost = 0;
    strcpy(properties[25].name, "Jaffna Railway Station"); properties[25].group = GRP_RAILWAY; 
     properties[25].purchasePrice = 2000;            properties[25].baseRent = 250;    properties[25].mortgageValue = 2000; properties[25].houseCost = 0; properties[25].hotelCost = 0;
    strcpy(properties[26].name, "Ceylon Electricity Board"); properties[26].group = GRP_UTILITY; 
     properties[26].purchasePrice = 1500;            properties[26].baseRent = 0;      properties[26].mortgageValue = 1500; properties[26].houseCost = 0; properties[26].hotelCost = 0;
    strcpy(properties[27].name, "National Water Supply and Drainage Board"); properties[27].group = GRP_UTILITY;
     properties[27].purchasePrice = 1500;            properties[27].baseRent = 0;      properties[27].mortgageValue = 750; properties[27].houseCost = 0; properties[27].hotelCost = 0;

}

void movePlayer(Player *player, int steps){
    int oldPosition = player->position;
    int newPosition = (oldPosition + steps)%40;

    player->position = newPosition;
     printf("%s moves from Square %d to Square %d.\n", player->name, oldPosition, newPosition);
    
    if(newPosition < oldPosition){
        player->cash += 2000;
    printf("%s passed GO\n", player->name);
    printf("Collected LKR 2000.\n");
    printf("Current Balance : LKR %d.\n", player->cash);
    }
}   