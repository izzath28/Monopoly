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
    char name[50];
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
    char name[50];
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
    int permPurchasePrice;
    int permBaseRent;
    int permMortgageValue;
    int closedRoundsLeft;  // Political Rally: 0 = open, >0 = collecting no rent
    int buildingCondition;
    int roundsSinceMaintenance;
    int structuralDamage;
    int maintenanceSurchargePercent;
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
    int loanInterestRate;   //per-mille, e.g. 80 = 8.0% 
    int loanRoundsLeft;
    int collateral[28];
    int numCollateral;
    int eventCard;         // -1 = none, else 0-19 
int eventCardRounds;
} Player;

void inPlayers(Player players[]);


int rollDice(void); //Dice
void sortRange(int rolls[], int turnOrder[], int start, int end); //sorting players

void determineTurnOrder(Player players[], int rolls[], int turnOrder[]); //playing order

void movePlayer(Player *player, int steps);

//finance.c
void purchaseProperty(Player *player, int playerIndex, Property *prop, Property properties[], int activeRegulation);
int calculateNetWorth(Player *player, int playerIndex, Property properties[]);
int countGroupOwned(Property properties[], int ownerIndex, PropertyGroup group);
void payRent(Player *payer, int payerIndex, Player *owner, int ownerIndex, Property *prop, Property properties[], int diceRoll, Player players[], int powerFailureActive, int activeEconomicEvent, int activeRegulation);
void payIncomeTax(Player *player, int playerIndex, Property properties[], int activeRegulation);

void releasePlayerAssets(Player *player, int playerIndex, Property properties[]);


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
void takeLoan(Player *player, int playerIndex, Property properties[], int activeEconomicEvent, int activeRegulation);
void repayLoan(Player *player);
void accrueInterest(Player players[]);
void checkLoanDefault(Player players[], Property properties[]);
void purchaseInsurance(Player *player, Property *prop, int type, int activeEconomicEvent, int activeRegulation);
int decideTakeLoan(Player *player);
int decideRepayLoan(Player *player);

int decideMaintenance(Player *player, Property *prop);
int decideRenovateDamage(Player *player, Property *prop);

void applyDepreciation(Property properties[]);
void triggerInflation(int round, Property properties[], int *currentInflationRate);
void triggerEconomicEvent(int round, Property properties[], int *activeEconomicEvent, int *economicEventRoundsLeft);
void triggerGovernmentRegulation(int round, Property properties[], Player players[], int *activeRegulation);
void triggerDisaster(int round, Player players[], Property properties[]);

void applyBuildingDecay(Property properties[]);
void performMaintenance(Player *player, Property *prop);
void renovateDamagedBuilding(Player *player, Property *prop);

void printRoundSummary(int round, Player players[], Property properties[]);

void mortgageProperty(Player *player, Property *prop);
int decideMortgage(Player *player); 


//events.c
void reviewPropertyMarket(int round, Property properties[], int *boomGroup, int *declineGroup, int groupLastAffected[]);
void getRegionalCardData(int cardIdx, const char **name, int propList[4], int *valuePercent, int *rentPercent);
void drawRegionalCard(int round, int *activeCard, int *cardRoundsLeft);
void recalculatePropertyValues(Property properties[], int boomGroup, int declineGroup, int activeRegionalCard);

void drawNationalEventCard(int currentPlayer, Player players[], Property properties[], int *powerFailureRounds, int *labourStrikeRounds);
void decrementEventTimers(Player players[], Property properties[], int *powerFailureRounds, int *labourStrikeRounds);

const char *groupName(PropertyGroup g);
void printMarketConditions(int boomGroup, int boomRoundsLeft, int declineGroup, int declineRoundsLeft, int regionalCardIdx, int regionalCardRoundsLeft, int currentInflationRate, int currentLoanInterestRate);

#endif // TYPES_H