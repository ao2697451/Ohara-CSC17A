/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: allison Ohara
 *
 * Created on February 19, 2019, 2:07 PM
 */

#include <iostream>
#include <iomanip>

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    int days;
    float increase, population;
    cout<<"How many organisms are there?\n";
    cin>>population;
    while(population<2){
        cout<<"Enter a number greater than 1.\n";
        cin>>population;
    }
    cout<<"By what percetange does the population increase each day?\n"<<"Enter a positive number from 1-100\n";
    cin>>increase;
    while(increase<0){
        cout<<"Enter a positive number.\n";
        cin>>increase;
    }
    cout<<"How many days does the population multiply for?\n";
    cin>>days;
    while(days<1){
        cout<<"Enter a number greater than 0\n";
        cin>>days;
    }
    cout<<setprecision(0)<<fixed<<endl;
    cout<<"Day 0 Population: "<<population<<endl;
    for(int i=0; i<days; i++){
        population=population + population*increase*0.01;
        cout<<"Day "<<i+1<<" Population: "<<population<<endl;
    }
    return 0;
}

