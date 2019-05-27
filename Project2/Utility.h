
#ifndef UTILITY_H
#define UTILITY_H

#include <string>
#include "Base.h"

class Utility: public Base
{
private:
    int multiplier = 4;
public:
    Utility(string n , int s) : Base(n, 150, multiplier, s )
    {multiplier = 4;}
    Utility()
    {Utility("",0);}
    const Utility operator = (const Utility &right){
        if(this!=&right)
            transfer(right);
        return *this;
    }
    //rent is equal to 4 * the amount rolled * the number of utilities owned
    int calcRent(int x, int y){
        for(int i = 1; i< y ; i++){
            multiplier+=4;
        }
        setRent(multiplier * x);
        return returnRent();
    }
    void increaseMultiplier()
    {multiplier *=2;}
    int getMultiplier()
    {return multiplier;}
};
#endif /* UTILITY_H */