
#ifndef PLAYERINFO_H
#define PLAYERINFO_H
#include <string>
#include "Utility.h"
#include "Railroad.h"
#include "Property.h"

using namespace std;
//PlayerInfo structure to store all important information about each player
struct PlayerInfo{
    int money = 1500;
    int totalPropertyValue = 0;
    int totalRailroadValue = 0;
    int totalUtilityValue = 0;
    int numberOfProperties = 0;
    int space = 0;
    int number;
    int numberOfRailroads = 0;
    int numberOfUtilities = 0;
    int filled = 0;
    int filledSize = 0;
    string name;
    bool jailed = false;
    Railroad *ownedRailroads = nullptr; 
    Utility *ownedUtilities = nullptr;
    Property * ownedProps = nullptr;
    Property * filledProps = nullptr; 
};

//TurnInfo structure to store information from each turn so players can earn rent from another's turn
struct TurnInfo{
    int player;
    int money = 0;
    bool chanceSpace = true;
    bool boughtProp = false;
    bool paidRent = false;
    int propertyPurchased = 0;
};

struct SaveInfo{
    int money, propValue;
};
#endif /* PLAYERINFO_H */