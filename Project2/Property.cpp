#include "Property.h"

Property Property::operator++(){
    ++typeOwned;
    return *this;
}
 int Property::calcRent(int a,int b){ 
        int tempRent = returnRent();
        int z = 1;
        if(b==1)
            z = 5;
        else if(b==2)
            z = 15;
        else if(b==3)
            z = 40;
        tempRent *=z;
        if(b==0)
            tempRent *=a;
        return tempRent;
    }
bool Property::operator>(const Property &right){
    bool status;
    if(getPrice()+houses*houseCost > right.getPrice()+right.houses*right.houseCost){
        status = true;
    }
    else if (getPrice()+houses*houseCost < right.getPrice()+right.houses*right.houseCost)
        status = false; 
    else{
        if(returnRent()>right.returnRent())
            status = true;
        else
            status = false;
    }
    return status;
}