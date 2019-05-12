/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: allis
 *
 * Created on February 13, 2019, 4:19 PM
 */

#include <cstdlib>
#include <iostream>
#include <iomanip>
using namespace std;

bool determine(char choice);
float convertYen(float);
float convertEuro(float);
/*
 * 
 */
int main(int argc, char** argv) {
    float dollars, yen, euros;
    char choice;
    bool type;
    cout<<"How many US dollars?"<<endl;
    cin>>dollars;
    cout<<"What would you like to convert to? Enter 'y' for yen and 'e' for Euros"<<endl;
    cin>>choice;
    type=determine(choice);
    if (type){
        yen = convertYen(dollars);
    }
    else{
        euros = convertEuro(dollars);
    }
    return 0;
}

bool determine (char x){
    char choice;
    choice = x;
    while(choice!='y'&&choice!='Y'&&choice!='E'&&choice!='e'){
        cout<<"Invalid selection. Enter either 'y' or 'e'."<<endl;
    }
    if (choice=='y'||choice=='Y'){
        return 1;
    }
    else{
        return 0;
    }
}

float convertYen(float dollars){
    const float YEN_PER_DOLLAR =101.43;
    float yen;
    yen = dollars * YEN_PER_DOLLAR;
    cout<<setprecision(2)<<fixed;
    cout<<"$"<<dollars<<" = "<<yen<<" yen.\n";
}

float convertEuro(float dollars){
    const float EUROS_PER_DOLLAR = 0.89;
    float euros;
    euros = dollars * EUROS_PER_DOLLAR;
    cout<<setprecision(2)<<fixed;
    cout<<"$"<<dollars<<" = "<<euros<<" eruos.\n";
}