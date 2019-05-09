/*
 * Project 2
   Allison Ohara
    Monopoly (based off of Project 1) with the addition of jail, houses, colors, railroads, utilities, 
    This Project also utilizes classes, inheritance, operator overloading, polymorphism, and templates
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

using namespace std;

//Classes
//Jail class to hold information about jail
class Jail
{
private:
    int cost, turns;
    bool freed = false, doubles = false;
public:
    Jail();
    int pay()
    {return -50;}
};
//Special class to be the base for railroad and utility properties
class Property
{
protected:
    int rent, price, owner, space;
    bool owned;
private:
    string ownersName, name, color;
    int numColor;
public:
    Property(string, string, int, int, int);
    Property();
    int calcRent();
    const Property operator=(const Property &right)
    {
        if(this!=&right)
        {
            rent = right.rent;
            price = right.price;
            owner = right.owner;
            ownersName = right.ownersName;
            color = right.color;
            numColor = right.numColor;
            owned = right.owned;
            name = right.name;
        }
        return *this;
    }
    void setRent(int a)
    {rent = a;}
    void setPrice(int a)
    {price = a;}
    void setOwner(int a)
    {owner = a;}
    void setSpace(int a)
    {space = a;}
    void setColor(string a)
    {color = a;}
    void setName(string a)
    {name =a;}
    void setOwnersName(string a)
    {ownersName = a;}
    string getOwnersName()
    {return ownersName;}
    string getName()
    {return name;}
    int getRent()
    {return rent;}
    int getPrice()
    {return price;}
    int getOwner()
    {return owner;}
    int getSpace()
    {return space;}
    void setNumColor(int a)
    {numColor = a;}
    int getNumColor()
    {return numColor;}
    string getColor()
    {return color;}
};
//Railroad class for the railroad property, inherits Special class
class Railroad : public Property
{
private:
    string name;
public: 
    Railroad(int);
    int calcRent(int);
};
//Utility class for the utilities: water and electric, inherits Special class
class Utility: public Property
{
private:
    string name;
public:
    Utility(int, int);
    int calcRent(int,int);
};
//Structures

//PlayerInfo structure to store all important information about each player
struct PlayerInfo{
    int money = 1500;
    int totalPropertyValue = 0;
    int numberOfProperties = 0;
    int space = 0;
    int number = 0;
    string name;
    Property * ownedProps = nullptr;
};

//Chance structure to store information of each chance card
struct Chance{
    string details;
    int effect;
};

//TurnInfo structure to store information from each turn so players can earn rent from another's turn
struct TurnInfo{
    int player;
    int money = 0;
    bool chanceSpace = true;
    bool boughtProp = false;
    bool paidRent = false;
    int propertyPurchased = 0;
};

//ScoreInfo structure to store information to be put on the scoreboard
struct ScoreInfo{
    int money;
    int numProps;
    int totalValueOfProps;
    int totalValue;
};

//Functions:
Property::Property()
{
    Property("","",0,0,0);
}

Property::Property(string n, string c, int p, int r, int nc){
    setName(n);
    setColor(c);
    setPrice(p);
    setRent(r);
    setNumColor(nc);
}

void showBoard(char []); //displays the game board
TurnInfo pTurn(int &,PlayerInfo &, char[], Property [], Chance *); //allows the player to take his turn
TurnInfo aiTurn(int &, PlayerInfo &, char[], Property[], Chance *); //does the AI's turn
int roll(); //allows player to roll the dice
Chance * chanceChooser(int &, Chance  *); //chooses a chance card
int spaceToProperty(int); //translates the number of the board space to the number of the property

int main(int argc, char** argv){
    //declare file and variables
    fstream file; 
    int numPlayers, endValue, drawn = 0, totalOwned, winner=0,highest=0, temp=0;
    char ch, input;
    bool done = false, aI = false, propFinish=false, bankruptcyFinish=false, valueFinish=false;
    TurnInfo lastTurn;
    PlayerInfo * players;
    ScoreInfo winners;
    //make the game board
    char board[]="GXCXTRXCXXAXUXXRXCXXFXCXXRXXUXJXXCXRCXTX";
    //store information on the 22 properties
    Property properties[22] ={
        {"Meditteranean Avenue","Brown",60,2,2},
        {"Baltic Avenue","Brown",60,4,2},
        {"Oriental Avenue","Light Blue",100,6,3},
        {"Vermont Avenue","Light Blue",100,6,3},
        {"Connecticut Avenue","Light Blue",120,8,3},
        {"St. Charles Place","Pink",140,10,3},
        {"States Avenue","Pink",140,10,3},
        {"Virginia Avenue","Pink",160,12,3},
        {"St. James Place","Orange",180,14,3},
        {"Tennessee Avenue","Orange",180,14,3},
        {"New York Avenue","Orange",200,16,3},
        {"Kentucky Avenue","Red",220,18,3},
        {"Indiana Avenue","Red",220,18,3},
        {"Illinois Avenue","Red",240,20,3},
        {"Atlantic Avenue","Yellow",260,22,3},
        {"Ventnor Avenue","Yellow",260,22,3},
        {"Marvin Gardens","Yellow",280,24,3},
        {"Pacific Avenue","Green",300,26,3},
        {"North Carolina Avenue","Green",300,26,3},
        {"Pennsylvania Avenue","Green",320,28,3},
        {"Park Place","Blue",350,35,2},
        {"Boardwalk","Blue",400,50,2}
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
    file.read(reinterpret_cast<char*>(&winners),sizeof(winners));
    cout<<"Scores to Beat: \n";
    while(!file.eof()){
        cout<<"$"<<winners.money<<endl;
        cout<<"Number of Owned Properties: "<<winners.numProps<<" valuing $"<<winners.totalValueOfProps<<endl;
        cout<<"Total value of properties and money: "<<winners.totalValue<<endl<<endl;
        file.read(reinterpret_cast<char*>(&winners),sizeof(winners));
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
    for(int i = 0; i<numPlayers; i++){
        cout<<"What is Player "<<i+1<<"'s name?\n";
        cin>>players[i].name;
        players[i].number=i;
    }
    //if there is only one player, then they will play against the Bot
    if(numPlayers==1)
        aI=true;
    
    if(aI){
        players[1].name = "Bot";
        players[1].number=1;
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
    
    //allow the players to go their turns before the game-ending criteria is met
    do{
        for(int i = 0; i<numPlayers;i++){
            showBoard(board);
            lastTurn = pTurn(drawn,players[i], board, properties,chanceDeck);
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
            //see if the game-ending criteria has been met
            if(bankruptcyFinish&&players[i].money<=0)
                done=true;
            else if(propFinish&&totalOwned>21)
                done = true;
            else if(valueFinish&&players[i].money+players[i].totalPropertyValue>=endValue)
                done = true;
        }
        //if the bot is playing, allowing it to play its turn
        if(aI){
            cout<<"AI's turn\n";
            cout<<"testing:\n";
            showBoard(board);
            lastTurn=aiTurn(drawn,players[1],board,properties, chanceDeck);
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
            //see if the game-ending criteria has been met
            if(bankruptcyFinish&&players[1].money<=0)
                done=true;
            else if(propFinish&&totalOwned>21)
                done = true;
            else if(valueFinish&&players[1].money+players[1].totalPropertyValue>=endValue)
                done = true;
        }
        totalOwned = 0;
    }while(!done);
    //once the criteria are met, the game ends
    //find the winner, and their stats
    cout<<"Game Over!\n";
    for(int i = 0; i<(numPlayers==1?2:numPlayers); i++){
        if(players[i].money+players[i].totalPropertyValue>highest){
            highest = players[i].money+players[i].totalPropertyValue;
            winner = i;
        }
    }
    cout<<"Congratulations "<<players[winner].name;
    cout<<"! You win! You had $"<<players[winner].money<<" "<<players[winner].numberOfProperties<<" properties and a total value of "<<highest<<endl;
    cout<<"Your score will be saved to the scoreboard.\n";
    cout<<players[winner].name<<endl;
    //save winner's data to scoreboard
    file.open("Scoreboard.txt", ios::out | ios::binary|ios::app);
    winners.money=players[winner].money;
    winners.numProps=players[winner].numberOfProperties;
    winners.totalValueOfProps=players[winner].totalPropertyValue;
    winners.totalValue=players[winner].money+players[winner].totalPropertyValue;
    file.write(reinterpret_cast<char*>(&winners),sizeof(winners));
                  
    return 0;
}

//display the game board
void showBoard(char board[]){
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

//the player's turn
TurnInfo pTurn(int & drawn,PlayerInfo &player, char board[], Property properties[], Chance *chanceDeck){
    //declare variables
    Chance * newDeck;
    char input;
    bool chanceSpace = false;
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
    //roll the dice and move that many spaces
    cout<<"Enter R to roll the dice.\n";
    cin>>input;
    while(toupper(input)!='R'){
        cout<<"Invalid entry. Try again.\n";
        cin>>input;
    }
    Property * newProps = nullptr;
    int propNum;
    TurnInfo turn;
    char taxChoice, yesNo;
    float taxRate;
    player.space+=roll();
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
        propNum=spaceToProperty(player.space);
        cout<<"You landed on "<<properties[propNum].getName()<<endl;
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
    }
    else if(board[player.space]=='U'){
        cout<<"a utility!\n";
    }
    else if(board[player.space]=='A'){
        cout<<"go to jail! Oh no! You are arrested!\n";
    }
    else if(board[player.space]=='J'){
        cout<<"jail! You visit a friend, and nothing else happens here.\n";
    }
    else{
        cout<<"an owned property!\n";
        propNum=spaceToProperty(player.space);
        cout<<"You landed on "<<properties[propNum].getName()<<" which is owned by Player "<<properties[propNum].getOwnersName()<<endl;
        if(properties[propNum].getOwner()==player.number){
            cout<<"You own this property!\n";
        }
        else{
            cout<<"You have to pay "<<properties[propNum].getOwnersName();
            cout<<" $"<<properties[propNum].getRent()<<endl;
            player.money-=properties[propNum].getRent();
            turn.money = properties[propNum].getRent();
            turn.player=properties[propNum].getOwner();
            turn.paidRent = true;
        }
    }
    cout<<"You have $"<<player.money<<" and "<<player.numberOfProperties<<" properties and are on space "<<player.space<<endl;
    if (!chanceSpace)
        cin.ignore();
    cout<<"Hit enter to end your turn.\n";
    cin.get(input);
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

//a chance card is randomly chosne from the deck, and then removed from the deck
Chance * chanceChooser(int &drawn, Chance *chanceDeck){
    int chosen, minValue = drawn>0? 1: 0, maxValue = 9-drawn;
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
TurnInfo aiTurn(int & drawn,PlayerInfo &player, char board[], Property properties[], Chance *chanceDeck){
    //declare variables and output the bot's stats
    char input;
    Chance * newDeck;
    bool chanceSpace = false;
    cout<<"\nIt's your turn, "<<player.name;
    cout<<"\nYou are on space "<<player.space<<endl;
    cout<<"You have $"<<player.money<<" and "<<player.numberOfProperties<<" properties\n";
    if(player.numberOfProperties!=0){
        cout<<"You own: \n";
        for(int i = 0; i<player.numberOfProperties; i++){
            cout<<player.ownedProps[i].getName()<<endl;
        }
    }
    Property * newProps = nullptr;
    int propNum;
    TurnInfo turn;
    player.space+=roll();
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
        propNum=spaceToProperty(player.space);
        cout<<"The AI bot landed on "<<properties[propNum].getName()<<endl;
        if(properties[propNum].getPrice()>player.money)
            cout<<"It cannot afford it!";
        else{
            cout<<"It bought it for $"<<properties[propNum].getPrice()<<endl;
                turn.propertyPurchased = propNum;
                turn.boughtProp=true;
                player.money-=properties[propNum].getPrice();
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
        
    }
    else if(board[player.space]=='R'){
        cout<<"a railroad!\n";
    }
    else if(board[player.space]=='U'){
        cout<<"a utility!\n";
    }
    else if(board[player.space]=='A'){
        cout<<"go to jail! Oh no! You are arrested!\n";
    }
    else if(board[player.space]=='J'){
        cout<<"jail! You visit a friend, and nothing else happens here.\n";
    }
    else{
        cout<<"an owned property!\n";
        propNum=spaceToProperty(player.space);
        cout<<"The AI landed on "<<properties[propNum].getName()<<" which is owned by Player "<<properties[propNum].getOwner()+1<<endl;
        if(properties[propNum].getOwner()==player.number){
            cout<<"The AI owns this property, so nothing happens\n";
        }
        else{
            cout<<"The AI has to pay "<<properties[propNum].getOwnersName();
            cout<<" $"<<properties[propNum].getRent()<<endl;
            player.money-=properties[propNum].getRent();
            turn.money = properties[propNum].getRent();
            turn.player=properties[propNum].getOwner();
            turn.paidRent = true;
        }
    }
    cout<<"The AI has $"<<player.money<<" and "<<player.numberOfProperties<<" properties and is on space "<<player.space<<endl;
    cout<<"Hit enter to end the AI's turn\n";
    cin.get(input);
    return turn;
}

//find the property number corresponding with the space
int spaceToProperty(int space){
    int property;
    if(space==1)
        property=0;
    else if (space==3)
        property=1;
    else if (space==6)
        property=2;
    else if(space<10)
        property=space-5;
    else if(space==11)
        property=5;
    else if (space<15)
        property=space-7;
    else if(space==16)
        property = 8;
    else if(space<20)
        property = space-9;
    else if(space==21)
        property = 11;
    else if(space<25)
        property = space -11;
    else if(space<28)
        property = space - 12;
    else if (space==29)
        property = 16;
    else if (space<33)
        property = space - 14;
    else if(space==34)
        property = 19;
    else if (space==37)
        property = 20;
    else
        property = 21;
    return property;
}