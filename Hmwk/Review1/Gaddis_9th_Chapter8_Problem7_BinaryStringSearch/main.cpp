/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: allison ohara
 *
 * Created on February 21, 2019, 3:30 PM
 */

#include <iostream>
#include <string>

using namespace std;

void bubbleSort(string names[], int size);
void swap(string &a, string &b);
int binarySearch(const string names[], int, string);
int main(int argc, char** argv) {
    const int NUM_NAMES = 20;
    string name = " ";
    int position;
    string names[NUM_NAMES]= {"Collins, Bill", "Smith, Bart", "Allen, Jim"
            ,"Griffin, Jim", "Stamey, Marty", "Rose, Geri", "Taylor, Terri",
            "Johnson, Jill", "Allison, Jeff","Looney, Joe","Wolfe, Bill","James, Jean",
            "Weaver, Jim","Pore, Bob","Rutherford, Greg","Javens, Renee","Harrison, Rose",
            "Setzer, Cathy","Pike, Gordon","Holland, Beth" };
     
    bubbleSort(names, NUM_NAMES);
    
    
    cout<<"What name are you searching for? Enter in the form Last, First. \n";
    cout<<"Example: Smith, John\n";
    getline(cin, name);
    position = binarySearch(names, NUM_NAMES, name);
    if (position != -1){
        cout<<"That name was found at position "<<position<<endl;
    }
    else
        cout<<"Sorry, that name was not found in the list.\n";
    return 0;
}

void swap(string &a, string &b){
    string temp = a;
    a = b;
    b = temp;
}
void bubbleSort(string names[], int size){
    int max, index;
    for (max = size -1; max> 0; max--){ 
        for(index = 0; index < max; index++){
            if (names[index]>names[index+1])
                swap(names[index],names[index+1]);
        }
    }
}

int binarySearch(const string names[], int size, string value){
    int first = 0, last = size -1, middle, position = -1;
    bool found = false;
    while (!found &&first<=last){
        middle = (first +last )/2;
        if (names[middle]==value){
            found = true;
            position = middle;
        }
        else if (names[middle]>value){
            last = middle - 1;
        }
        else
            first = middle + 1;
    }
    return position;
}