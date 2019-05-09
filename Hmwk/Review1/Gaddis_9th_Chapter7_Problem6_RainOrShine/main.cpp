/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: allison ohara
 *
 * Created on February 20, 2019, 2:33 PM
 */

#include <iostream>
#include <fstream>
using namespace std;

 main(int argc, char** argv) {
     ifstream inFile;
     char weather[3][30];
     int row, column, rainiest;
     int Rs[3] = {0,0,0};
     int Ss[3] = {0,0,0};
     int Cs[3] = {0,0,0};
     int total[3]={0,0,0};
     inFile.open("RainOrShine.txt");
     while(row<3){
         column=0;
         while(column<30){
             inFile>>weather[row][column];
             column++;
         }
         row++;
     }
     inFile.close();
     for(int i = 0; i<3; i++){
         for(int x = 0; x<30; x++){
            if(weather[i][x]=='R'){
                Rs[i]++;
            }
            else if (weather[i][x]=='S'){
                Ss[i]++;
            }
            else{
                Cs[i]++;
            }
         }
     }
     total[0] = Rs[0]+ Rs[1] +Rs[2];
     total[1] = Ss[0]+ Ss[1] +Ss[2];
     total[2] = Cs[0]+ Cs[1] +Cs[2];
     cout<<"June had "<<Rs[0]<<" rainy days, "<<Ss[0]<<" sunny days and "<<Cs[0]<<" cloudy days.\n";
     cout<<"July had "<<Rs[1]<<" rainy days, "<<Ss[1]<<" sunny days and "<<Cs[1]<<" cloudy days.\n";
     cout<<"August had "<<Rs[2]<<" rainy days, "<<Ss[2]<<" sunny days and "<<Cs[2]<<" cloudy days.\n";
     cout<<"In total, there were "<<total[0]<<" rainy days, "<<total[1]<<" sunny days, and "<<total[2]<<" cloudy days.\n";
     if (Rs[0]>Rs[1]){
         if(Rs[0]>Rs[2]){
             cout<<"June had the most rainy days.";
         }
         else{
             cout<<"August had the most rainy days.";
         }
     }
     else{
         if(Rs[1]>Rs[2]){
             cout<<"July had the most rainy days.";
         }
         else{
             cout<<"August had the most rainy days.";
         }
     }
    return 0;
}

