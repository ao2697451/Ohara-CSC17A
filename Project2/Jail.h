
#ifndef JAIL_H
#define JAIL_H

#include <iostream>

using namespace std;

//forward declaration of base class
class Property;
//Jail class to hold information about jail
class Jail
{
private:
    int cost, turns;
    static int jailTimes;
    bool freed = false, doubles = false;
public:
    Jail(bool info){
    if(!info){
    cout<<"You are in jail!\n";
    cout<<"While you're in jail, you cannot collect rent on normal properties (though you can collect it on special properties like railroads and utilities.\n";
    cout<<"In order to get out of jail, you must either roll doubles or pay $50.\n";
    cout<<"You have three turns/chances to roll doubles; after this, you must pay the $50 and be free your next turn\n";
    cost = 50;
    turns = 1;}
    jailTimes++;}
    Jail()
    {Jail(false);}
    void setTurns(int a)
    {turns = a;}
    int getTurns()
    {return turns;}
    void setFreed(bool a)
    {freed =a;}
    bool getFreed()
    {return freed;}
    int jailTurn(int,bool);
    int getJailTimes()
    {return jailTimes;}
    void rentFreeze(bool,Property &);
};
#endif /* JAIL_H */