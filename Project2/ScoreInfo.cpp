#include "ScoreInfo.h"
#include <iostream>
using namespace std;

ostream &operator << (ostream &strm, const ScoreInfo &obj){
        strm<<"This player had $"<<obj.money<<" "<<obj.numProps<<" regular properties valuing $"<<obj.totalValueOfProps<<" and a total value of $"<<obj.totalValue<<endl;
        return strm;
}