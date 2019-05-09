/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: allis
 *
 * Created on February 15, 2019, 12:29 PM
 */

#include <cstdlib>
#include <iostream>

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    int month, year;
    bool leap;
    leap = false;
    cout<<"Enter a month (1-12): "<<endl;
    cin>>month;
    cout<<"Enter a year: "<<endl;
    cin>>year;
    if(year%100==0){
        if(year%400==0){
            leap = true;
        }
    }
    else if(year%4==0){
        leap = true;
    }
    switch (month){
        case 1: cout<<"31\n";
        break;
        case 2: if (leap){
            cout<<"29\n";
            }
                else{
            cout<<"28\n";
            }
        break;
        case 3: cout<<"31 days\n";
        break;
        case 4: cout<<"30 days\n";
        break;
        case 5: cout<<"31 days\n";
        break;
        case 6: cout<<"30 days\n";
        break;
        case 7: cout<<"31 days\n";
        break;
        case 8: cout<<"31 days\n";
        break;
        case 9: cout<<"30 days\n";
        break;
        case 10: cout<<"31 days\n";
        break;
        case 11: cout<<"30 days\n";
        break;
        case 12: cout<<"31 days\n";
        break;
        default: {
            cout<<"invalid entry"<<endl;
        }
    }
    return 0;
}

