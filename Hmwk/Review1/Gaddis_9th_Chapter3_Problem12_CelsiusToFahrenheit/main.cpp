/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: allis
 *
 * Created on February 13, 2019, 1:13 PM
 */

#include <cstdlib>
#include <iostream>

using namespace std;

/*
 * 
 */
void convert(float &temp);
int main(int argc, char** argv) {
    float temp;
    cout<<"What is the temperature in Celsius?"<<endl;
    cin>>temp;
    convert(temp);
    cout<<"The temperature in Fahrenheit is: "<<temp<<endl;
    return 0;
}

void convert(float &temp){
    temp= temp*9/5 +32;
}
