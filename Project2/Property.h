#ifndef PROPERTY_H
#define PROPERTY_H
#include "Base.h"
#include "Jail.h"
#include <string>


using namespace std;

//Property class to be the base for properties, and railroad and utility properties
class Property : public Base
{
private:
    //information about the property, and the houses on it
    string color;
    int numColor, num,houseCost = 0;
    int typeOwned = 1, houses = 0;
    bool jailed;
public:
    int * set = nullptr;
    Property(string n, string c, int p, int r, int nc, int s, int number, int ph, int arr[]) : Base(n,p,r,s)
    {
        color = c;
        numColor = nc;
        num = number;
        houseCost = ph;
        set = arr; 
        jailed = false;}
    Property()
    {Property("","",0,0,0,0,0,0,{0});}
    Property(const Property & obj){
        setName(obj.getName());
        setPrice(obj.getPrice());
        setRent(obj.returnRent());
        color = obj.color;
        numColor = obj.numColor;
        houseCost = obj.houseCost;
    }
    const Property operator=(Property &right)
    {
        if(this!=&right)
        {
            transfer(right);
            typeOwned = right.typeOwned;
            houses = right.houses;
            color = right.color;
            numColor = right.numColor;
            houseCost = right.houseCost;
            num = right.num;
            owned = right.owned;
        }
        return *this;
    }
    void setTypeOwned(int a)
    {typeOwned = a;}
    void setHouses(int a)
    {houses = a;}
    void setColor(string a)
    {color = a;}
    void setNum(int a)
    {num = a;}
    void increaseHouses()
    {houses++;}
    friend void Jail::rentFreeze(bool, Property &);
    bool getJailed() const
    {return jailed;}
    int getNum() const
    {return num;}
    int getTypeOwned() const
    {return typeOwned;}
    void setHouseCost(int a)
    {houseCost = a;}
    int getHouseCost()const
    {return houseCost;}
    void setNumColor(int a)
    {numColor = a;}
    int getNumColor()const
    {return numColor;}
    string getColor()const
    {return color;}
    int getHouses()const
    {return houses;}
    Property operator++();
    bool operator>(const Property &);
    int calcRent(int ,int );
};
#endif /* PROPERTY_H */