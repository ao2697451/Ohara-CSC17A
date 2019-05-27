
#ifndef RAILRAOD_H
#define RAILRAOD_H
#include <string>
#include "Base.h"

using namespace std;

class Railroad : public Base
{
private:
    int number;
public: 
    Railroad(string n, int s) : Base(n,200,25,s)
    {number = 4;}
    Railroad()
    {Railroad("",0);}
    const Railroad operator = ( Railroad & right){
        if(this!=&right)
            transfer(right);
        return *this;
    }
    //rent is equal to 25 * the number of railroads owned
    int calcRent(int y, int x){
        setRent(25 * x);
        return returnRent();
    };
};
#endif /* RAILRAOD_H */