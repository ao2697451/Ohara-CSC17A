#include <ctime>
#include <cstdlib>
#include <iostream>
#include "Jail.h"
#include "Property.h"
using namespace std;
int Jail::jailTimes = 0;

//if a player is in jail, this takes the place of their turn
int Jail::jailTurn(int turns,bool ai){
    char a;
    int roll1, roll2;
    const int MAX_VALUE = 6;
    const int MIN_VALUE = 1;
    if(turns!=4){
        if(ai)
            a='R';
        else{
    cout<<"Would you like to roll (and try to get doubles) or pay $50?\n";
    cout<<"Enter 'R' for roll and 'P' for pay.\n";
    cin>>a;
        while(toupper(a)!='R'&&toupper(a)!='P'){
        cout<<"Invalid entry; try again.\n";
        cin>>a;
            }
        }
    if(toupper(a)=='R'){
        cout<<"Rolling die 1...\n";
        roll1 =  (rand()% (MAX_VALUE - MIN_VALUE+1)) +MIN_VALUE;
        roll2= (rand()% (MAX_VALUE - MIN_VALUE+1)) +MIN_VALUE;
        cout<<roll1<<endl;
        cout<<"Rolling die 2...\n";
        cout<<roll2<<endl;
        if(roll1==roll2){
            cout<<"Yay! You rolled doubles!\n";
            freed = true;
        }
        else{
            cout<<"Sorry, no doubles. You're still in jail.\n";
        }
    }
    else{
        cout<<"You are paying $50.\n";
        freed = true;
        return 50;
    }
    }
    else
    {
        cout<<"You have already tried for doubles 3 times, and now you must pay $50.\n";
        freed = true;
        return 50;
    }
    return 0;
}

void Jail::rentFreeze(bool frozen, Property & prop){
    prop.jailed = frozen;
}