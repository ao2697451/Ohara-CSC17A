/*
 * Project 2
   Allison Ohara
    Monopoly (based off of Project 1) with the addition of jail, houses, colors, railroads, utilities, 
    This Project also will utilize classes, inheritance, operator overloading, polymorphism, and templates, among others
 *  This is Version 3; changes from Version 2 include greater efficiency, correct rent calculations, jail, and houses
 *  Concepts Added for Version 3: templates, exceptions, aggregation, friends, pure virtual classes, STL
 *  Line Counts: main.cpp - 1151
 *               Utility.h - 35
 *               Railroad.h - 29
 *               Jail.h - 43
 *               Info.h - 44
 *               Property.h - 87
 *               Chance.h - 13
 *               Base.h - 59
 *               ScoreInfo.h - 38
 *               Base.cpp - 13
 *               ScoreInfo.cpp - 8
 *               Jail.cpp - 59
 *               Property.cpp - 35
 *       Total: 1614 lines
 */
#include <iostream> 
#include <iomanip> //for setw
#include <fstream> //for files
#include <cstring> //for cstrngs
#include <string> //for strings
#include <cstdlib> //for rand()
#include <cctype> //for toupper()
#include <cmath> //for round()
#include <ctime> //for time()
#include <vector>
//Classes
#include "Base.h"
#include "Property.h"
#include "Railroad.h"
#include "Utility.h"
#include "Jail.h"
#include "ScoreInfo.h"
//Structures
#include "Info.h"
#include "Chance.h"
using namespace std;

//Template (with exception) that finds the largest of any two types of values
template <class T>
T largest(T num1, T num2, T num3){
    T large;
    string exceptionString = "The two largest values are equal.\n";
    if(num1>num2){
        large = num1;
        if(large==num3)
            throw exceptionString;
        else if(num3>num1)
            large = num3;
    }
    else if(num2>num1){
        large = num2;
        if(large==num3)
            throw exceptionString;
        else if(num3>large)
            large = num3;
    }
    return large;
};
//Functions:
void showBoard(char []); //displays the game board
TurnInfo pTurn(int &,PlayerInfo *,int, char[], Property [], Railroad[],Utility [], Chance *); //allows the player to take his turn
TurnInfo aiTurn(int &, PlayerInfo *,int, char[], Property[], Railroad[],Utility[],Chance *); //does the AI's turn
int roll(); //allows player to roll the dice
Chance * chanceChooser(int &, Chance  *); //chooses a chance card
int getRent(Base *, int, int);

int main(int argc, char** argv){
    //declare file and variables
    fstream file; 
    SaveInfo saved;
    Property greatest;
    int numPlayers, endValue, drawn = 0, totalOwned, winner=0,highest=0, temp=0;
    int * jailTurns;
    char ch, input;
    bool done = false, aI = false, propFinish=false, bankruptcyFinish=false, valueFinish=false, tie = false;
    // arrays to keep track of which properties are in a set with other, same-colored properties
    int arr1 [] = {0,1};
    int arr2 []= {0,-1};
    int arr3 []= {0,1,2};
    int arr4 [] = {0,-1,1};
    int arr5 [] = {0,-1,-2};
    TurnInfo lastTurn;
    string most;
    PlayerInfo * players;
    Jail * *jailed = nullptr;
    vector <ScoreInfo> scores;
    //make the game board
    char board[]="GXCXTRXCXXAXUXXRXCXXFXCXXRXXUXJXXCXRCXTX";
    //store information on the 22 properties
    Property properties[22] ={
        {"Meditteranean Avenue","Brown",60,2,2,1 ,0, 50, arr1},
        {"Baltic Avenue","Brown",60,4,2,3, 1 , 50,arr2},
        {"Oriental Avenue","Light Blue",100,6,3,6,2 ,50, arr3},
        {"Vermont Avenue","Light Blue",100,6,3,8,3 , 50, arr4 },
        {"Connecticut Avenue","Light Blue",120,8,3,9,4,50, arr5},
        {"St. Charles Place","Pink",140,10,3,11,5,100, arr3},
        {"States Avenue","Pink",140,10,3,13,6,100, arr4},
        {"Virginia Avenue","Pink",160,12,3,14,7,100, arr5},
        {"St. James Place","Orange",180,14,3,16,8,100, arr3},
        {"Tennessee Avenue","Orange",180,14,3,18,9,100, arr4},
        {"New York Avenue","Orange",200,16,3,19,10,100, arr5},
        {"Kentucky Avenue","Red",220,18,3,21,11,150, arr3},
        {"Indiana Avenue","Red",220,18,3,23,12,150, arr4},
        {"Illinois Avenue","Red",240,20,3,24,13,150, arr5},
        {"Atlantic Avenue","Yellow",260,22,3,26,14,150, arr3},
        {"Ventnor Avenue","Yellow",260,22,3,27,15,150, arr4},
        {"Marvin Gardens","Yellow",280,24,3,29,16,150, arr5},
        {"Pacific Avenue","Green",300,26,3,31,17,200, arr3},
        {"North Carolina Avenue","Green",300,26,3,32,18,200, arr4},
        {"Pennsylvania Avenue","Green",320,28,3,34,19,200, arr5},
        {"Park Place","Blue",350,35,2,37,20,200, arr1},
        {"Boardwalk","Blue",400,50,2,39,21,200, arr2}
};
    //store information on the railroads
    Railroad railroads[4]=
    {
        {"Reading Railroad",5},
        {"Pennsylvania Railroad",15},
        {"B&O Railroad",25},
        {"Short Line",25}
    };
    //store information on the utilities
    
    Utility utilities[2]=
    {
        {"Electric Company",12},
        {"Water Works",28}
    };
    //build the chance deck
    Chance *originalDeck = new Chance [10];
    Chance *chanceDeck = new Chance[10];
    Chance *newDeck;
    chanceDeck[0]={"Oh no! You got robbed! You lose $50!",-50};
    chanceDeck[1]={"Yay! You won the lottery! You win $200!",200};
    chanceDeck[2]={"Huh, that's strange. You can't find your $10 bill...", -10};
    chanceDeck[3]={"Happy Birthday! You received $20!",20};
    chanceDeck[4]={"You broke your leg and have to pay $100 in hospital bills.", -100};
    chanceDeck[5]={"You got a holiday bonus check! You earned $150!", 150};
    chanceDeck[6]={"You treated your friend to a steak dinner. There goes $30.",-30};
    chanceDeck[7]={"You just won a bet! You got an extra $50!",50};
    chanceDeck[8]={"You accidently broke a teacup, and have to buy it for $20.",-20};
    chanceDeck[9]={"Hey, look at that! You found a $10 bill!",10};
    for(int i = 0; i<10; i++)
        originalDeck[i]=chanceDeck[i];
    //open the scoreboard file and read out the scores
    file.open("Scoreboard.txt", ios::in | ios::binary);
    file.read(reinterpret_cast<char*>(&saved),sizeof(saved));
    cout<<"Scores to Beat: \n";
    while(!file.eof()){
        cout<<"$"<<saved.money<<" and a total value of all properties of $"<<saved.propValue<<endl;
        file.read(reinterpret_cast<char*>(&saved),sizeof(saved));
    }
     file.close();
     //find the number of players
    cout<<"How many players are there?\n";
    cin>>numPlayers;
    while(numPlayers>4){
        cout<<"You must chose a number less than four.\n";
        cin>>numPlayers;
    }
    players = new PlayerInfo[numPlayers==1?2:numPlayers];
    jailTurns = new int [numPlayers==1?2:numPlayers];
    jailed = new Jail * [numPlayers==1?2:numPlayers];
    for(int i = 0; i<numPlayers; i++){
        cout<<"What is Player "<<i+1<<"'s name?\n";
        cin>>players[i].name;
        players[i].number=i;
        jailTurns[i] = 1;
        jailed[i] = nullptr;
    }
    //if there is only one player, then they will play against the Bot
    if(numPlayers==1)
        aI=true;
    
    if(aI){
        players[1].name = "Bot";
        players[1].number=1;
        jailTurns[1]=1;
        jailed[1] = nullptr;
    }
    //player gets to choose how the game ends
    cout<<"How would you like the game to end? Enter B for bankruptcy, P for all properties are owned, and T for a certain total value attained.\n";
    cin>>input;
    while(toupper(input)!='B'&&toupper(input)!='P'&&toupper(input)!='T'){
        cout<<"invalid input, try again\n";
        cin>>input;
    }
    //B means the game ends when someone goes bankrupt
    if(toupper(input)=='B')
        bankruptcyFinish= true;
    //P means the game ends when all the properties are owned
    else if(toupper(input) == 'P')
        propFinish = true;
    //T means the game will end when someone reaches a certain total value
    else{
        valueFinish=true;
        cout<<"At what point would you like to the game to end? Enter a whole number value. Once a player acquires this much in money and property value, the game will end.\n";
        cout<<"Suggestion: Choose a number around 2000\n";
        cin>>endValue;
        cout<<endValue<<endl;
    }
    cout<<"All players start on the Go space marked 'G'. This is space 0, and from here players move in a clockwise direction.\n";
    //allow the players to go their turns before the game-ending criteria is met
    do{
        for(int i = 0; i<numPlayers;i++){
            //if the player is jailed, the do jailTurn instead of pTurn
            if(players[i].jailed){
                cout<<"It's your turn, "<<players[i].name<<endl;
                if(jailTurns[i]==1){
                    jailed[i] = new Jail;
                    for(int j = 0; j<players[i].numberOfProperties;j++)
                        jailed[i]->rentFreeze(true, properties[players[i].ownedProps[j].getNum()]);
                }
                players[i].money-=jailed[i]->jailTurn(jailTurns[i], false);
                jailTurns[i]++;
                if(jailed[i]->getFreed()){
                    players[i].jailed = false;
                    cout<<"You are freed from jail!\n";
                    jailTurns[i] = 1;
                    for(int j = 0; j<players[i].numberOfProperties;j++)
                        jailed[i]->rentFreeze(false, properties[players[i].ownedProps[j].getNum()]);
                    delete  jailed[i];
                }
                cin.ignore();
                cout<<"Hit enter to end the turn.\n";
                cin.get(ch);
            }
            else{
            showBoard(board);
            lastTurn = pTurn(drawn,players, i,board, properties,railroads, utilities,chanceDeck);
            //if all the chance cards are drawn, reshuffle the deck
            if(drawn==10){
                drawn=0;
                delete [] chanceDeck;
                chanceDeck = new Chance [10];
                for(int i = 0; i<10; i++){
                   chanceDeck[i]=originalDeck[i];
                }
            }
            //give any paid rent to the other player
            totalOwned +=players[i].numberOfProperties;
            if (lastTurn.paidRent){
                players[lastTurn.player].money+=lastTurn.money;
                cout<<"Congratulations "<<players[lastTurn.player].name;
                cout<<" you got $"<<lastTurn.money<<" in rent.\n";
            }
            }
            //see if the game-ending criteria has been met
            if(bankruptcyFinish&&players[i].money<=0)
                done=true;
            else if(propFinish&&totalOwned>21)
                done = true;
            else if(valueFinish&&players[i].money+players[i].totalPropertyValue + players[i].totalRailroadValue + players[i].totalUtilityValue>=endValue)
                done = true;
        }
        //if the bot is playing, allowing it to play its turn
        if(aI){
            cout<<"AI's turn\n";
            if(players[1].jailed){
                if(jailTurns[1]==1){
                    jailed[1] = new Jail;
                    for(int j = 0; j<players[1].numberOfProperties;j++)
                        jailed[1]->rentFreeze(true, properties[players[1].ownedProps[j].getNum()]);
                }
                players[1].money-=jailed[1]->jailTurn(jailTurns[1],aI);
                jailTurns[1]++;
                if(jailed[1]->getFreed()){
                    players[1].jailed = false;
                    cout<<"You are freed from jail!\n";
                    jailTurns[1] = 1;
                    for(int j = 0; j<players[1].numberOfProperties;j++)
                        jailed[1]->rentFreeze(false, properties[players[1].ownedProps[j].getNum()]);
                    delete jailed[1];
                }
                
                cout<<"Hit enter to end the turn.\n";
                cin.get(ch);
            }
            else{
                showBoard(board);
                lastTurn=aiTurn(drawn,players,1,board,properties, railroads, utilities,chanceDeck);
                //if all the chance cards have been drawn, reshuffle the deck
                if(drawn==10){
                    drawn=0;
                    delete [] chanceDeck;
                    chanceDeck = new Chance [10];
                    for(int i = 0; i<10; i++){
                        chanceDeck[i]=originalDeck[i];
                    }
                }
                //allow the other player to get their rent money
                totalOwned+=players[1].numberOfProperties;
                if (lastTurn.paidRent){
                    players[lastTurn.player].money+=lastTurn.money;
                    cout<<"Congratulations "<<players[lastTurn.player].name;
                    cout<<" you got $"<<lastTurn.money<<" in rent.\n";
                }
            }
            //see if the game-ending criteria has been met
            if(bankruptcyFinish&&players[1].money<=0)
                done=true;
            else if(propFinish&&totalOwned>21)
                done = true;
            else if(valueFinish&&players[1].money+players[1].totalPropertyValue + players[1].totalRailroadValue +players[1].totalUtilityValue>=endValue)
                done = true;
        }
        totalOwned = 0;
    }while(!done);
    //once the criteria are met, the game ends
    //find the winner, and their stats
    for(int i = 0; i<(numPlayers==1?2:numPlayers); i++){
        players[i].totalRailroadValue = 200 * players[i].numberOfRailroads;
        players[i].totalUtilityValue = 150 * players[i].numberOfUtilities;
        if(players[i].money+players[i].totalPropertyValue + players[i].totalUtilityValue + players[i].totalRailroadValue>highest){
            highest = players[i].money+players[i].totalPropertyValue + players[i].totalUtilityValue + players[i].totalRailroadValue;
            winner = i;
        }
        scores.push_back(ScoreInfo(players[i].money, players[i].numberOfProperties, players[i].numberOfRailroads+players[i].numberOfUtilities,players[i].totalPropertyValue, players[i].money+players[i].totalPropertyValue + players[i].totalUtilityValue + players[i].totalRailroadValue));
    }
    cout<<"Player Info: "<<endl;
    temp = 0;
    for(auto it =scores.begin(); it!=scores.end(); it++){
        cout<<players[temp].name<<endl;
        cout<<*it;
        cout<<endl;
        temp++;
    }
    temp = 0;
    jailed[0] = new Jail(true);
    for(int i = 0; i<(numPlayers==1?2:numPlayers); i++){
        if(players[i].money+players[i].totalPropertyValue + players[i].totalUtilityValue + players[i].totalRailroadValue==highest)
            temp++;
    }
    if(temp>1)
        tie =true;
    if(!tie){
        greatest.setPrice(0);
    greatest.setHouses(0);
    for(int i = 0; i<players[winner].numberOfProperties; i++){
        if(players[winner].ownedProps[i]>greatest)
            greatest = players[winner].ownedProps[i];
    }
    ScoreInfo winning;
    winning.setProp(greatest);
    cout<<"Congratulations "<<players[winner].name;
    cout<<"! You win! You had $"<<players[winner].money<<" "<<players[winner].numberOfProperties<<" properties,";
    cout<<players[winner].numberOfRailroads<<" railroads, and "<<players[winner].numberOfUtilities<<" utilities.\n";
    cout<<"A total of "<<jailed[0]->getJailTimes()-1<<" arrests took place.\n";
    cout<<"The winner's highest valued property was "<<winning.returnProp().getName()<<endl;
    try{
        highest = largest(players[winner].totalPropertyValue,players[winner].totalRailroadValue,players[winner].totalUtilityValue);
        if(highest == players[winner].totalPropertyValue)
            most = "regular properties";
        else if(highest == players[winner].totalRailroadValue)
            most = "railroads";
        else
            most = "utilities";
        cout<<"The type of property valuing the most was "<<most<<" which had a total value of $"<<highest<<endl;
    }
    catch(string exceptionString){
        cout<<exceptionString<<endl;
    }
    cout<<"Your score will be saved to the scoreboard ";
    cout<<players[winner].name<<endl;
    //save winner's data to scoreboard
    file.open("Scoreboard.txt", ios::out | ios::binary|ios::app);
    saved.money = players[winner].money;
    saved.propValue = players[winner].totalPropertyValue + players[winner].totalUtilityValue + players[winner].totalRailroadValue;
    file.write(reinterpret_cast<char*>(&saved),sizeof(saved)); 
    file.close();
    }
    else{
        cout<<"The winners tied! No score will be saved, as no one won.\n";
    }
    delete [] players;
   // delete scores;
    return 0;
}

//display the game board
void showBoard(char board[]){
    cout<<"Board:\n";
    int count = 12;
    for(int i = 20; i<31; i++)
        cout<<board[i]<<" ";
    cout<<endl;
    for(int i = 19; i>10; i--){
        cout<<board[i]<<setw(20)<<board[i+count];
        cout<<endl;
        count+=2;
    }
    for(int i = 10; i>=0; i--)
        cout<<board[i]<<" ";
}
//function for retrieiving the rent, with calcRent being different the different classes (Property/Railroad/Utility)
int getRent(Base *prop,int y, int x){
    int rent;
    rent = prop->calcRent(y,x);
    return rent;
}

//the player's turn
TurnInfo pTurn(int & drawn,PlayerInfo *players, int p, char board[], Property  properties[], Railroad railroads[],Utility utilities[],Chance *chanceDeck){
    //declare variables
    
    Chance * newDeck;
    PlayerInfo player = players[p];
    char input;
    bool chanceSpace = false;
    int rolled, rent;
    string owner;
    int ownerNum, x = 0 , y =0;
    //output the player's info
    cout<<"\nIt's your turn, "<<player.name;
    cout<<"\nYou are on space "<<player.space<<endl;
    cout<<"You have $"<<player.money<<" and "<<player.numberOfProperties<<" properties\n";
    if(player.numberOfProperties!=0){
        cout<<"You own: \n";
        for(int i = 0; i<player.numberOfProperties; i++){
            cout<<player.ownedProps[i].getName()<<endl;
        }
    }
    cout<<"You have "<<player.numberOfRailroads<<" railroads\n";
    if(player.numberOfRailroads!=0){
        cout<<"You own: \n";
        for(int i = 0; i<player.numberOfRailroads; i++){
            cout<<player.ownedRailroads[i].getName()<<endl;
        }
    }
    cout<<"You have "<<player.numberOfUtilities<<" utilities\n";
    if(player.numberOfUtilities!=0){
        cout<<"You own: \n";
        for(int i = 0; i<player.numberOfUtilities; i++){
            cout<<player.ownedUtilities[i].getName()<<endl;
        }
    }
    //allow the player to buy houses if they have a complete property set
    if(player.filled>0){
        cout<<"You have a complete set for "<<player.filled<<" set(s) of properties:\n";
        cout<<"The sets you have completed are: "<<endl;
        do{
            cout<<player.filledProps[x].getColor()<<":"<<endl;
            y = x;
            while(y<(player.filledProps[x].getNumColor()+x)){
                cout<<y<<" - "<<player.filledProps[y].getName()<<"    House Cost: $"<<player.filledProps[y].getHouseCost();
                cout<<"  # of Houses: "<<player.filledProps[y].getHouses()<<endl;
                cout<<"    Current rent: $"<<getRent(&properties[player.filledProps[y].getNum()],2,player.filledProps[y].getHouses());
                cout<<"    Rent with an additional house: $"<<getRent(&properties[player.filledProps[y].getNum()],2,player.filledProps[y].getHouses()+1 )<<endl;
                y++;
            }
            x = y;
        }while(x<player.filledSize);
        cout<<"Would you like to buy a house for one of these properties? Enter 'Y' or 'N'\n";
        cout<<"Note: you can't buy more than three houses for a property\n";
        cin>>input;
        while(toupper(input)!='Y'&&toupper(input)!='N'){
            cout<<"Invalid input; try again.\n";
            cin>>input;
        }
        if(toupper(input)=='Y'){
            cout<<"Which property would you like to purchase a house for? Enter then number corresponding with the property, as shown above.\n";
            cin>>x;
            while(x<0||x>=player.filledSize){
                cout<<"Invalid input; try again.\n";
                cin>>x;
            }
            if(player.filledProps[x].getHouses()==3)
                cout<<"You already have the maximum number of houses for that property.\n";
            else{
                cout<<"You are buying a house for: "<<player.filledProps[x].getName()<<endl;
                cout<<"This will cost $"<<player.filledProps[x].getHouseCost()<<endl;
                player.money-=player.filledProps[x].getHouseCost();
                properties[player.filledProps[x].getNum()].increaseHouses();
                for(int i = 0; i<player.numberOfProperties; i++){
                    if(player.ownedProps[i].getNum()==player.filledProps[x].getNum())
                        player.ownedProps[i].increaseHouses();
                }
                player.filledProps[x].increaseHouses();
            }
        }
    }
    //roll the dice and move that many spaces
    cout<<"Enter R to roll the dice.\n";
    cin>>input;
    while(toupper(input)!='R'){
        cout<<"Invalid entry. Try again.\n";
        cin>>input;
    }
    Property * newProps = nullptr;
    Property *tempProps = nullptr;
    Railroad * newRails = nullptr;
    Utility * newUts= nullptr;
    int propNum;
    TurnInfo turn;
    char taxChoice, yesNo;
    float taxRate;
    rolled = roll();
    player.space+=rolled;
    //if the player passed go, give them $200 and restart the space numbers
    if(player.space>=32){
        player.space-=32;
        cout<<"You passed Go! You earn $200!\n";
        player.money+=200;
    }
    //find where the player landed
    //if they landed on Go, or Free Parking, nothing happens
    //if they land on income tax, they choose their rate and pay it
    //if they land on an unowned property, they can buy it if they can afford it
    // if the land on a railroad, they can buy it if they can afford it
    // if they land on a utility, they can buy it if they can afford it
    // if they land on Go to Jail, they are arrested
    // if they land on jail, nothing happens
    //if they land on an owned property, they have to pay rent unless they are the owner
    cout<<"You landed on ";
    if(board[player.space]=='G')
        cout<<"the go space. Nothing else happens here.\n";
    else if(board[player.space]=='T'){
        cout<<"income tax! Would you like to pay the flat rate of $200 or the calculated rate of 10% of your total worth?\n";
        cout<<"Enter F for the 200$ flat rate and C for the calculated 10%.\n";
        cin>>taxChoice;
        while(toupper(taxChoice)!='F'&&toupper(taxChoice)!='C'){
            cout<<"Please enter either F or C.\n";
            cin>>taxChoice;
        }
        if(toupper(taxChoice)=='F'){
            cout<<"You paid $200.\n";
            player.money-=200;
        }
        else{
            taxRate= round((player.money+player.totalPropertyValue) * 0.10);
            player.money-=static_cast<int>(taxRate);
            cout<<"You paid $"<<taxRate<<endl;
        }
    }
    else if(board[player.space]=='F')
        cout<<"free parking! Take a break and relax for a turn\n";
    else if(board[player.space]=='C'){
        cout<<"a chance space!\n";
        chanceSpace = true;
        cin.ignore();
        cout<<"Hit enter to choose your chance card.\n";
        cin.get(input);
        cout<<"Randomly selecting your Chance card...\n";
        newDeck = chanceChooser(drawn, chanceDeck);
        cout<<newDeck[0].details<<endl;
        player.money+=newDeck[0].effect;
        for(int i = 0; i<11-drawn; i++){
            chanceDeck[i]=newDeck[i];
        }
        delete [] newDeck;
    }
    else if(board[player.space]=='X'){
        cout<<"an unowned property!\n";   
        for(int i = 0; i<22; i++){
            if(properties[i].getSpace()==player.space)
                propNum = i;
        }
        cout<<"You landed on "<<properties[propNum].getName()<<" which is "<<properties[propNum].getColor()<<endl;
        if(properties[propNum].getPrice()>player.money)
            cout<<"Sorry, you can't afford this!";
        else{
            cout<<"Would you like to buy "<<properties[propNum].getName()<<" for $"<<properties[propNum].getPrice()<<"?\n";
            cout<<"Enter Y for yes and N for no.\n";
            cin>>yesNo;
            while(toupper(yesNo)!='Y'&&toupper(yesNo)!='N'){
                cout<<"Invalid entry, try again.\n";
                cin>>yesNo;
            }
            if(toupper(yesNo)=='Y'){
                
                turn.propertyPurchased = propNum;
                turn.boughtProp=true;
                player.money-=properties[propNum].getPrice();
                
                for(int i  = 0; i< player.numberOfProperties; i++){
                    if(properties[propNum].getColor()==player.ownedProps[i].getColor()){
                        ++properties[propNum];
                        ++properties[player.ownedProps[i].getNum()];
                        if(properties[propNum].getTypeOwned()==properties[propNum].getNumColor()){
                            int z = 0;
                            player.filled++;
                            player.filledSize += properties[propNum].getNumColor();
                            tempProps = new Property[player.filledSize];
                            for(int a = 0; a<player.filledSize - properties[propNum].getNumColor(); a++)
                                tempProps[a]=player.filledProps[a];
                            for(int a = player.filledSize - properties[propNum].getNumColor(); a<player.filledSize; a++){
                               tempProps[a]=properties[propNum+properties[propNum].set[z]];
                               z++;
                            }
                            delete [] player.filledProps;
                            player.filledProps = new Property[player.filledSize];
                            for(int a =0; a<player.filledSize; a++){
                                player.filledProps[a]=tempProps[a];
                            }
                            delete [] tempProps;
                        }
                    }
                }
                
                player.numberOfProperties++;
                newProps = new Property [player.numberOfProperties];
                for (int i = 0; i<player.numberOfProperties-1; i++)
                    newProps[i]=player.ownedProps[i];
                
                newProps[player.numberOfProperties-1]=properties[propNum];
                delete []player.ownedProps;
                player.ownedProps = new Property [player.numberOfProperties];
                for(int i = 0; i<player.numberOfProperties; i++)
                    player.ownedProps[i]=newProps[i];
                delete []newProps;
                
                properties[propNum].setOwner(player.number);
                board[player.space] ='O';
                properties[propNum].setOwnersName(player.name);
                player.totalPropertyValue+=properties[propNum].getPrice();
                 
                cout<<"Congratulations! You bought "<<properties[propNum].getName()<<endl;
            }
            else{
                cout<<properties[propNum].getName()<<" remains unowned.\n";
           }
        }
        
    }
    else if(board[player.space]=='R'){
        cout<<"a railroad!\n";
        for(int i = 0; i<4; i++){
            if(player.space==railroads[i].getSpace())
                propNum = i;
        }
        cout<<"You landed on "<<railroads[propNum].getName()<<endl;
        if(railroads[propNum].getOwned()==false){
            if(railroads[propNum].getPrice()<=player.money){
            cout<<"This railroad is unowned! Would you like to purchase it for $200?\n";
            cin>>yesNo;
            while(toupper(yesNo)!='Y'&&toupper(yesNo)!='N'){
                cout<<"Invalid entry, try again.\n";
                cin>>yesNo;
            }
            if(toupper(yesNo)=='Y'){
                cout<<"You are buying "<<railroads[propNum].getName()<<" for $200.\n";
                player.money-=railroads[propNum].getPrice();
                railroads[propNum].setOwned(true);
                railroads[propNum].setOwnersName(player.name);
                railroads[propNum].setOwner(player.number);
                player.numberOfRailroads++;
                newRails = new Railroad [player.numberOfRailroads];
                for (int i = 0; i<player.numberOfRailroads-1; i++)
                    newRails[i]=player.ownedRailroads[i];
                newRails[player.numberOfRailroads-1]=railroads[propNum];
                delete [] player.ownedRailroads;
                player.ownedRailroads = new Railroad [player.numberOfRailroads];
                for(int i = 0; i<player.numberOfRailroads; i++)
                    player.ownedRailroads[i]=newRails[i];
                delete []newRails;
            }
            else{
                cout<<railroads[propNum].getName()<<" remains unpurchased.\n";
            }
            }
            else
                cout<<"Sorry, you can't afford this.\n";
        }
        else{
            cout<<"This railroad is owned!\n";
            owner = railroads[propNum].getOwnersName();
            if(owner == player.name){
                cout<<"You are the owner of this railroad.\n";
                cout<<"You do not need to pay rent.\n";
            }
            else{
                cout<<owner<<" owns "<<railroads[propNum].getName()<<endl;
                ownerNum = railroads[propNum].getOwner();
                rent = getRent(&railroads[propNum], rolled, players[ownerNum].numberOfRailroads);
                cout<<"You need to pay "<<owner<<" $"<<rent<<endl;
                player.money-=rent;
                turn.money = rent;
                turn.player = ownerNum;
            }
        }
    }
    else if(board[player.space]=='U'){
        cout<<"a utility!\n";
        for(int i = 0; i<2; i++){
            if(player.space==utilities[i].getSpace())
                propNum = i;
        }
        cout<<"You landed on "<<utilities[propNum].getName()<<endl;
        if(utilities[propNum].getOwned()==false){
            if(utilities[propNum].getPrice()<player.money){
            cout<<"This utility is unowned! Would you like to purchase it for $150?\n";
            cin>>yesNo;
            while(toupper(yesNo)!='Y'&&toupper(yesNo)!='N'){
                cout<<"Invalid entry, try again.\n";
                cin>>yesNo;
            }
            if(toupper(yesNo)=='Y'){
                cout<<"You are buying "<<utilities[propNum].getName()<<" for $"<<utilities[propNum].getPrice()<<endl;
            player.money-=utilities[propNum].getPrice();
            utilities[propNum].setOwned(true);
            utilities[propNum].setOwnersName(player.name);
            utilities[propNum].setOwner(player.number);
            player.numberOfUtilities++;
            newUts = new Utility [player.numberOfUtilities];
                for (int i = 0; i<player.numberOfUtilities-1; i++)
                    newUts[i]=player.ownedUtilities[i];
                newUts[player.numberOfUtilities-1]=utilities[propNum];
                delete [] player.ownedUtilities;
                player.ownedUtilities = new Utility [player.numberOfUtilities];
                for(int i = 0; i<player.numberOfUtilities; i++)
                    player.ownedUtilities[i]=newUts[i];
                delete []newUts;
            }
            else
                cout<<utilities[propNum].getName()<<" remains unpurchased.\n";
            }
            else
                cout<<"Sorry, you can't afford this.\n";
        }
        else{
            cout<<"This utility is owned!\n";
            owner = utilities[propNum].getOwnersName();
            if(owner == player.name){
                cout<<"You are the owner of this utilty.\n";
                cout<<"You don't need to pay rent to yourself.\n";
            }
            else{
                cout<<owner<<" owns "<<utilities[propNum].getName()<<endl;
                ownerNum = utilities[propNum].getOwner();
                rent = getRent(&utilities[propNum], rolled,players[ownerNum].numberOfUtilities);
                cout<<"You need to pay "<<owner<<" $"<<rent<<endl;
                player.money-=rent;
                turn.money = rent;
                turn.player = ownerNum;
            }
        }
    }
    else if(board[player.space]=='A'){
        cout<<"go to jail! Oh no! You are arrested!\n";
        player.jailed = true;
        player.space = 30;
    }
    else if(board[player.space]=='J'){
        cout<<"jail! You visit a friend, and nothing else happens here.\n";
    }
    else{
        cout<<"an owned property!\n";
        for(int i = 0; i<22; i++){
            if(properties[i].getSpace()==player.space)
                propNum = i;
        }
        cout<<"You landed on "<<properties[propNum].getName()<<" which is owned by Player "<<properties[propNum].getOwnersName()<<endl;
        if(properties[propNum].getOwner()==player.number){
            cout<<"You own this property!\n";
        }
        else{
            if(properties[propNum].getJailed()){
                cout<<"The owner of this property is in jail, so you don't have to pay rent.\n";
            }
            else{
            ownerNum = properties[propNum].getOwner();
            x=1;
            if(properties[propNum].getTypeOwned()==properties[propNum].getNumColor())
                x = 2;
            rent = getRent(&(properties[propNum]), x,properties[propNum].getHouses());
            cout<<"You have to pay "<<properties[propNum].getOwnersName();
            cout<<" $"<<rent<<endl;
            player.money-=rent;
            turn.money = rent;
            turn.player=ownerNum;
            turn.paidRent = true;
        }
        }
    }
    cout<<"You have $"<<player.money<<" and are on space "<<player.space<<endl;
    cout<<"You have "<<player.numberOfProperties<<" properties, "<<player.numberOfRailroads<<" railroads, and "<<player.numberOfUtilities<<" utilities.\n";
    if (!chanceSpace)
        cin.ignore();
    cout<<"Hit enter to end your turn.\n";
    cin.get(input);
    players[p] = player;
    return turn;
}

//player rolls two dice, and the sum is returned
int roll(){
    cout<<"Rolling...\n";
    unsigned seed = time(0);
    srand(seed);
    int x, y, sum;
    const int MAX_VALUE = 6;
    const int MIN_VALUE = 1;
    x = (rand()% (MAX_VALUE - MIN_VALUE+1)) +MIN_VALUE;
    y = (rand()% (MAX_VALUE - MIN_VALUE+1)) +MIN_VALUE;
    cout<<x<<"   "<<y<<endl<<endl;
    sum = x+y;
    return sum;
}

//a chance card is randomly chosen from the deck, and then removed from the deck
Chance * chanceChooser(int &drawn, Chance *chanceDeck){
    int chosen, minValue = drawn>0? 1: 0, maxValue = 9-drawn;
    if(drawn == 9)
        maxValue =1;
    int x = drawn>0? 0 : 1;
    unsigned seed = time(0);
    srand(seed);
    Chance * newDeck;
    newDeck = new Chance [10-drawn];
    chosen = (rand()%(maxValue-minValue+1))+minValue;
    newDeck[0]=chanceDeck[chosen];
    for(int i =1; i<10-drawn; i++){
        if(i-x!=chosen&&(i-x)<chosen)
            newDeck[i]=chanceDeck[i-x];
        else{
            newDeck[i]=chanceDeck[i-x+1];
        }
    }
    drawn++;
    return newDeck;
}

//the bot does its turn
TurnInfo aiTurn(int & drawn,PlayerInfo *players,int p, char board[], Property properties[], Railroad railroads[], Utility utilities[], Chance *chanceDeck){
    //declare variables and output the bot's stats
    char input;
    string owner;
    int ownerNum;
    PlayerInfo player = players[p];
    Chance * newDeck;
    int rolled, rent, x= 0, y = 0;
    bool chanceSpace = false;
    bool purchase = false;
    cout<<"\nIt's your turn, "<<player.name;
    cout<<"\nThe bot is on space "<<player.space<<endl;
    cout<<"It has $"<<player.money<<" and "<<player.numberOfProperties<<" properties\n";
    if(player.numberOfProperties!=0){
        cout<<"AI owns: \n";
        for(int i = 0; i<player.numberOfProperties; i++){
            cout<<player.ownedProps[i].getName()<<endl;
        }
    }
    cout<<"The AI has "<<player.numberOfRailroads<<" railroads\n";
    if(player.numberOfRailroads!=0){
        cout<<"AI owns: \n";
        for(int i = 0; i<player.numberOfRailroads; i++){
            cout<<player.ownedRailroads[i].getName()<<endl;
        }
    }
    cout<<"The AI has "<<player.numberOfUtilities<<" utilties\n";
    if(player.numberOfUtilities!=0){
        cout<<"AI owns: \n";
        for(int i = 0; i<player.numberOfUtilities; i++){
            cout<<player.ownedUtilities[i].getName()<<endl;
        }
    }
    //The bot can buy a house if it has a complete set. There is a 1 in 4 chance that it will purchase a house
    if(player.filled>0){
        cout<<"The AI has a complete set for "<<player.filled<<" set(s) of properties:\n";
        cout<<"The sets AI has completed are: "<<endl;
        do{
            cout<<player.filledProps[x].getColor()<<":"<<endl;
            y = x;
            while(y<(player.filledProps[x].getNumColor()+x)){
                cout<<y<<" - "<<player.filledProps[y].getName()<<"    House Cost: $"<<player.filledProps[y].getHouseCost();
                cout<<"  # of Houses: "<<player.filledProps[y].getHouses()<<endl;
                cout<<"    Current rent: $"<<getRent(&properties[player.filledProps[y].getNum()],2,player.filledProps[y].getHouses());
                cout<<"    Rent with an additional house: $"<<getRent(&properties[player.filledProps[y].getNum()],2,player.filledProps[y].getHouses()+1 )<<endl;
                y++;
            }
            x = y+1;
        }while(x<player.filledSize);
        if((rand()%4)+1==1)
            purchase = true;
        if(purchase){
            x = (rand()% ((player.filledSize-1)- 0) + 1);
            if(player.filledProps[x].getHouses()!=3){
                cout<<"AI is buying a house for: "<<player.filledProps[x].getName()<<endl;
                cout<<"This will cost $"<<player.filledProps[x].getHouseCost()<<endl;
                player.money-=player.filledProps[x].getHouseCost();
                properties[player.filledProps[x].getNum()].increaseHouses();
                for(int i = 0; i<player.numberOfProperties; i++){
                    if(player.ownedProps[i].getNum()==player.filledProps[x].getNum())
                        player.ownedProps[i].increaseHouses();
                }
                player.filledProps[x].increaseHouses();
            }
            else
                cout<<"The AI will not buy a house.\n";
        }
        else
            cout<<"The AI will not buy a house.\n";
    }
    //there is a 1 in 2 chance that the bot will buy a property/railroad/utility (if it lands on it)
    if((rand()%2)+1==1)
            purchase = true;
    else
        purchase = false;
    Property * newProps = nullptr;
    Property * tempProps = nullptr;
    Railroad * newRails = nullptr;
    Utility * newUts = nullptr;
    int propNum;
    TurnInfo turn;
    rolled = roll();
    player.space+=rolled;
    //if it passed go, reset the space numbers and it gets $200
    if(player.space>=32){
        player.space-=32;
        cout<<"The AI bot passed Go! You earn $200!\n";
        player.money+=200;
    }
    //find where the player landed
    //if they landed on Go, or Free Parking, nothing happens
    //if they land on income tax, they choose their rate and pay it
    //if they land on an unowned property, they can buy it if they can afford it
    //if they land on an owned property, they have to pay rent unless they are the owner
    //if they land on a railroad, they can buy it if they can afford it
    //if they land on a utility, they can buy it if they can afford it
    //if they land on Go to Jail, they are arrested
    //if they land on jail, nothing happens
    cout<<"The AI bot landed on ";
    if(board[player.space]=='G')
        cout<<"the go space. Nothing else happens here.\n";
    else if(board[player.space]=='T'){
        cout<<"income tax! The AI bot loses $200!\n";
        player.money-=200;
    }
    else if(board[player.space]=='F')
        cout<<"free parking! Take a break and relax for a turn\n";
    else if(board[player.space]=='C'){
        cout<<"a chance space!\n";
        chanceSpace = true;
        newDeck = chanceChooser(drawn, chanceDeck);
        cout<<newDeck[0].details<<endl;
        player.money+=newDeck[0].effect;
        for(int i = 0; i<11-drawn; i++){
            chanceDeck[i]=newDeck[i];
        }
        delete [] newDeck;
    }
    else if(board[player.space]=='X'){
        cout<<"an unowned property!\n";   
        for(int i = 0; i<22; i++){
            if(properties[i].getSpace()==player.space)
                propNum = i;
        }
        cout<<"The AI bot landed on "<<properties[propNum].getName()<<" which is "<<properties[propNum].getColor()<<endl;
        if(properties[propNum].getPrice()>player.money)
            cout<<"It cannot afford it!";
        else{
            if(purchase){
                cout<<"It bought it for $"<<properties[propNum].getPrice()<<endl;
                turn.propertyPurchased = propNum;
                turn.boughtProp=true;
                player.money-=properties[propNum].getPrice();
                player.totalPropertyValue+=properties[propNum].getPrice();
                for(int i  = 0; i< player.numberOfProperties; i++){
                    if(properties[propNum].getColor()==player.ownedProps[i].getColor()){
                        ++properties[propNum];
                        ++properties[player.ownedProps[i].getNum()];
                        if(properties[propNum].getTypeOwned()==properties[propNum].getNumColor()){
                            int z = 0;
                            player.filled++;
                            player.filledSize += properties[propNum].getNumColor();
                            tempProps = new Property[player.filledSize];
                            for(int a = 0; a<player.filledSize - properties[propNum].getNumColor(); a++)
                                tempProps[a]=player.filledProps[a];
                            for(int a = player.filledSize - properties[propNum].getNumColor(); a<player.filledSize; a++){
                               tempProps[a]=properties[propNum+properties[propNum].set[z]];
                               z++;
                            }
                            delete [] player.filledProps;
                            player.filledProps = new Property[player.filledSize];
                            for(int a =0; a<player.filledSize; a++){
                                player.filledProps[a]=tempProps[a];
                            }
                            delete [] tempProps;
                        }
                    }
                }
                player.numberOfProperties++;
                newProps = new Property [player.numberOfProperties];
                for (int i = 0; i<player.numberOfProperties-1; i++)
                    newProps[i]=player.ownedProps[i];
                newProps[player.numberOfProperties-1]=properties[propNum];
                delete [] player.ownedProps;
                player.ownedProps = new Property [player.numberOfProperties];
                for(int i = 0; i<player.numberOfProperties; i++)
                    player.ownedProps[i]=newProps[i];
                delete []newProps;
                properties[propNum].setOwner(player.number);
                board[player.space] ='O';
                properties[propNum].setOwnersName(player.name);
            }
            else
                cout<<"It did not buy it.\n";
        }
        
    }
    else if(board[player.space]=='R'){
        cout<<"a railroad!\n";
        for(int i = 0; i<4; i++){
            if(player.space==railroads[i].getSpace())
                propNum = i;
        }
        cout<<"The AI landed on "<<railroads[propNum].getName()<<endl;
        if(railroads[propNum].getOwned()==false){
            cout<<"This railroad is unowned!\n";
            if(railroads[propNum].getPrice()>player.money)
            cout<<"The AI can't afford it, so it remains unowned.\n";
            else{
            if(purchase){
            cout<<"The AI purchases it for $200.\n";
            cout<<"The AI is buying "<<railroads[propNum].getName()<<" for $200.\n";
            player.money-=railroads[propNum].getPrice();
            railroads[propNum].setOwned(true);
            railroads[propNum].setOwnersName(player.name);
            railroads[propNum].setOwner(player.number);
            player.numberOfRailroads++;
            newRails = new Railroad [player.numberOfRailroads];
                for (int i = 0; i<player.numberOfRailroads-1; i++)
                    newRails[i]=player.ownedRailroads[i];
                newRails[player.numberOfRailroads-1]=railroads[propNum];
                delete [] player.ownedRailroads;
                player.ownedRailroads = new Railroad [player.numberOfRailroads];
                for(int i = 0; i<player.numberOfRailroads; i++)
                    player.ownedRailroads[i]=newRails[i];
                delete []newRails;
            }
            else
                cout<<"The AI did not buy it.\n";
            }
        }
        else{
            cout<<"This railroad is owned!\n";
            owner = railroads[propNum].getOwnersName();
            if(owner == player.name){
                cout<<"The AI is the owner of this railroad.\n";
                cout<<"The AI doesn't need to pay rent to itself.\n";
            }
            else{
                cout<<owner<<" owns "<<railroads[propNum].getName()<<endl;
                ownerNum = railroads[propNum].getOwner();
                rent = getRent(&railroads[propNum],rolled, players[ownerNum].numberOfRailroads);
                cout<<"The AI needs to pay "<<owner<<" $"<<rent<<endl;
                player.money-=rent;
                turn.money = rent;
                turn.player = ownerNum;
            }
        }
    }
    else if(board[player.space]=='U'){
        cout<<"a utility!\n";
        for(int i = 0; i<2; i++){
            if(player.space==utilities[i].getSpace())
                propNum = i;
        }
        cout<<"You landed on "<<utilities[propNum].getName()<<endl;
        if(utilities[propNum].getOwned()==false){
            cout<<"This utility is unowned!\n";
            if(utilities[propNum].getPrice()>player.money)
                cout<<"The AI can't afford this, so it remains unowned.\n";
            else{
            if(purchase){
            cout<<"The AI purchases it for $150.\n";
            cout<<"The AI is buying "<<utilities[propNum].getName()<<" for $150.\n";
            player.money-=utilities[propNum].getPrice();
            utilities[propNum].setOwned(true);
            utilities[propNum].setOwnersName(player.name);
            utilities[propNum].setOwner(player.number);
            player.numberOfUtilities++;
            newUts = new Utility [player.numberOfUtilities];
                for (int i = 0; i<player.numberOfUtilities-1; i++)
                    newUts[i]=player.ownedUtilities[i];
                newUts[player.numberOfUtilities-1]=utilities[propNum];
                delete [] player.ownedUtilities;
                player.ownedUtilities = new Utility [player.numberOfUtilities];
                for(int i = 0; i<player.numberOfUtilities; i++)
                    player.ownedUtilities[i]=newUts[i];
                delete []newUts;
            }
            else
                cout<<"The AI did not buy it.\n";
        }
        }
        else{
            cout<<"This utilty is owned!\n";
            owner = utilities[propNum].getOwnersName();
            if(owner == player.name){
                cout<<"The AI is the owner of this utilty.\n";
                cout<<"The AI doesn't need to pay rent to itself.\n";
            }
            else{
                cout<<owner<<" owns "<<utilities[propNum].getName()<<endl;
                ownerNum = utilities[propNum].getOwner();
                rent = getRent(&utilities[propNum], rolled,players[ownerNum].numberOfUtilities);
                cout<<"The AI needs to pay "<<owner<<" $"<<rent<<endl;
                player.money-=rent;
                turn.money = rent;
                turn.player = ownerNum;
            }
        }
    }
    else if(board[player.space]=='A'){
        cout<<"go to jail! Oh no! You are arrested!\n";
        player.jailed = true;
        player.space = 30;
    }
    else if(board[player.space]=='J'){
        cout<<"jail! You visit a friend, and nothing else happens here.\n";
    }
    else{
        cout<<"an owned property!\n";
        for(int i = 0; i<22; i++){
            if(properties[i].getSpace()==player.space)
                propNum = i;
        }
        cout<<"The AI landed on "<<properties[propNum].getName()<<" which is owned by Player "<<properties[propNum].getOwner()+1<<endl;
        if(properties[propNum].getOwner()==player.number){
            cout<<"The AI owns this property, so nothing happens\n";
        }
        else{
            if(properties[propNum].getJailed())
                cout<<"The owner of this property is in jail, so the AI doesn't need to pay rent.\n";
            else{
            ownerNum = properties[propNum].getOwner();
            x=1;
            if(properties[propNum].getTypeOwned()==properties[propNum].getNumColor())
                x = 2;
            rent = getRent(&(properties[propNum]), x,properties[propNum].getHouses());
            cout<<"The AI has to pay "<<properties[propNum].getOwnersName();
            cout<<" $"<<rent<<endl;
            player.money-=rent;
            turn.money = rent;
            turn.player=ownerNum;
            turn.paidRent = true;
            }
        }
    }
    cout<<"The AI has $"<<player.money<<" and is on space "<<player.space<<endl;
    cout<<"The AI has "<<player.numberOfProperties<<" properties, "<<player.numberOfRailroads<<" railroads, and "<<player.numberOfUtilities<<" utilities.\n";
    cout<<"Hit enter to end the AI's turn\n";
    cin.get(input);
    players[p]=player;
    return turn;
}
