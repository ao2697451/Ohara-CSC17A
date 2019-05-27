
#ifndef BASE_H
#define BASE_H

#include <string>
using namespace std;

//Property class to be the base for properties, and railroad and utility properties
class Base
{
private:
    //information about the property, and the houses on it
    string ownersName, name;
    int rent, price, owner;
protected:
    int space;
    bool owned;
public:
    Base (string n = "", int p = 0, int r = 0, int s = 0)
    {   name = n;
        price = p;
        rent = r;
        space = s;
}
    void transfer(const Base &);
    void setRent(int a)
    {rent = a;}
    void setPrice(int a)
    {price = a;}
    void setOwner(int a)
    {owner = a;}
    void setSpace(int a)
    {space = a;}
    void setOwned(bool a)
    {owned = a;}
    void setName(string a)
    {name =a;}
    void setOwnersName(string a)
    {ownersName = a;}
    int returnRent() const
    {return rent;}
    bool getOwned()
    {return owned;}
    string getOwnersName()
    {return ownersName;}
    string getName() const
    {return name;}
    //rent is doubled if a whole color set is owned, and increases with houses
    virtual int calcRent(int,int) = 0;
    int getPrice() const
    {return price;}
    int getOwner() const
    {return owner;}
    int getSpace() const
    {return space;}
};


#endif /* BASE_H */