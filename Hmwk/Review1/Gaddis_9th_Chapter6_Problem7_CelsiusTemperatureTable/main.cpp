/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: allison ohara
 *
 * Created on February 20, 2019, 1:48 PM
 */

#include <iostream>
#include <iomanip>

using namespace std;

/*
 * 
 */
float celsius(float temp);
int main(int argc, char** argv) {
    float temp;
    cout<<"Fahrenheit"<<setw(13)<<"Celsius"<<endl;
    for (int i = 0; i<=20; i++){
    temp = celsius(i);
    cout<<setw(5)<<i<<setw(17)<<setprecision(2)<<fixed<<temp<<endl;
    }
    return 0;
}

float celsius(float temp){
    float tempC;
    tempC = (temp -32) *5/9;
    return tempC;
}

