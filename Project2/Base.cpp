#include "Base.h"
void Base::transfer(const Base &right)
    {
        if(this!=&right)
        {
            rent = right.rent;
            price = right.price;
            owner = right.owner;
            ownersName = right.ownersName;
            owned = right.owned;
            name = right.name;
        }
    }