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
    int mortgageValue;
    int baseRent;
    int houseCost;
    int hotelCost;
    int owner;         //if bank owes it, then -1
    int mortgaged;      // 1-morgaged  0-not morgaged
    int insured;      // 1-insured  0-not insured
    int insuranceType; // 0=Basic, 1=Comprehensive, 2=Business, -1=none 
    int insuranceRoundsLeft;
    int houses;      //5th house will be the hotel
    int age;
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
    int inJail;
    int jailTurns;
    int hasLoan;
    int loanAmount;
    int loanInterestRate;   /* per-mille, e.g. 80 = 8.0% */
    int loanRoundsLeft;
    int collateral[28];
    int numCollateral;
} Player;

void inPlayers(Player players[]);


int rollDice(void); //Dice
void sortRange(int rolls[], int turnOrder[], int start, int end); //sorting players

void determineTurnOrder(Player players[], int rolls[], int turnOrder[]); //playing order

void movePlayer(Player *player, int steps);

//finance.c
void purchaseProperty(Player *player, int playerIndex, Property *prop);
int calculateNetWorth(Player *player, int playerIndex, Property properties[]);
int countGroupOwned(Property properties[], int ownerIndex, PropertyGroup group);
void payRent(Player *payer, int payerIndex, Player *owner, int ownerIndex, Property *prop, Property properties[], int diceRoll);

int hasMonopoly(Property properties[], int ownerIndex, PropertyGroup group);
void buildHouse(Player *player, Property *prop);

//game.c
void runGame(Square board[], Property properties[], Player players[], int turnOrder[]);
int countSolventPlayers(Player players[]);

//players.c
int shouldBuyProperty(Player *player, Property *prop);
int shouldBuildHouse(Player *player, int playerIndex, Property properties[], int *outPropIndex);

int decideAuctionBid(Player *player, Property *prop, int currentBid);
void auctionProperty(Player players[], Property *prop);
int resolveJail(Player *player);

int maxLoanAmount(Player *player, int playerIndex, Property properties[]);
void takeLoan(Player *player, int playerIndex, Property properties[]);
void repayLoan(Player *player);
void accrueInterest(Player players[]);
void checkLoanDefault(Player players[], Property properties[]);
void purchaseInsurance(Player *player, Property *prop, int type);
int decideTakeLoan(Player *player);
int decideRepayLoan(Player *player);

void applyDepreciation(Property properties[]);
void triggerInflation(int round, Property properties[]);
void triggerEconomicEvent(int round);
void triggerGovernmentRegulation(int round);
void triggerDisaster(int round, Player players[], Property properties[]);

void printRoundSummary(int round, Player players[], Property properties[]);

#endif // TYPES_H