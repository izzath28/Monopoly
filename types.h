#ifndef TYPES_H
#define TYPES_H

typedef enum {
    SQR_GO,
    SQR_PROPERTY,
    SQR_RAILWAY,
    SQR_UTILITY,
    SQR_EVENT,
    SQR_TAX,
    SQR_SPECIAL,
    SQR_INSURANCE,
    SQR_BANK
} SquareType;

typedef struct {
    char name[40];
    SquareType type;
    int propertyindex; //so that can treat non-properties differently. If -1 then it's a non-property square.
} Square;

typedef enum{
    GRP_BROWN,
    GRP_LIGHT_BLUE,
    GRP_PINK,
    GRP_ORANGE,
    GRP_RED,
    GRP_YELLOW,
    GRP_GREEN,
    GRP_DARK_BLUE,
    GRP_RAILWAY,                
    GRP_UTILITY,             
} PropertyGroup;

typedef struct{
    char name[40];
    PropertyGroup group;
    int purchasePrice;
    int morgageValue;
    int baseRent;
    int houseCost;
    int hotelCost;
    int owner;         //if bank owes it, then -1
    int morgaged;      // 1-morgaged  0-not morgaged
    int insured;      // 1-insured  0-not insured
    int houses;      //5th house will be the hotel
} Property;

void inBoard(Square board[]);
void inProperties(Property properties[]);
const char *square_type_name(SquareType t);

typedef enum{
    STRAT_AGGRESSIVE_INVESTOR,
    STRAT_CONSERVATIVE_BANKER,
    STRAT_RISK_TAKER,
    STRAT_OPPORTUNISTIC_TRADER, 
} PlayerStrategy;

typedef struct{
    char name[40];
    PlayerStrategy strategy;
    int cash;
    int position; //where the player is on the board
    int bankrupt; //1-bankrupt 0-not bankrupt
} Player;

void inPlayers(Player players[]);


int rollDice(void); //Dice
void sortRange(int rolls[], int turnOrder[], int start, int end); //sorting players

void determineTurnOrder(Player players[], int rolls[], int turnOrder[]); //playing order

void movePlayer(Player *player, int steps);

//finance.c
void purchaseProperty(Player *player, int playerIndex, Property *prop); 
void payRent(Player *payer, Player *owner, Property *prop);

//game.c
void runGame(Square board[], Property properties[], Player players[], int turnOrder[]);



#endif // TYPES_H