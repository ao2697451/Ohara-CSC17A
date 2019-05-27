
#ifndef SCORE_INFO_H
#define SCORE_INFO_H
#include <iostream>
#include "Property.h"
using namespace std;
class ScoreInfo;
ostream &operator << (ostream &, const ScoreInfo &);
//ScoreInfo class to store information to be put on the scoreboard
class ScoreInfo{
private:
    int money;
    int numProps,numSpecial;
    int totalValueOfProps;
    int totalValue;
    Property highestProp;
public:
    ScoreInfo()
    {ScoreInfo(0,0,0,0,0);}
    ScoreInfo(int m,int n,int s,int t,int v)
    {money = m; numProps = n; numSpecial = s; totalValueOfProps = t; totalValue = v;}
    ~ScoreInfo()
    {cout<<"GAME OVER\n";}
    int getMoney()
    {return money;}
    int getNumProps()
    {return numProps;}
    int getTotalValueOfProps()
    {return totalValueOfProps;}
    int getTotalValue()
    {return totalValue;}
    void setProp(Property prop)
    {highestProp = prop;}
    Property returnProp()
    {return highestProp;}
    friend ostream &operator <<(ostream &, const ScoreInfo&);
};
#endif /* SCORE_INFO_H */