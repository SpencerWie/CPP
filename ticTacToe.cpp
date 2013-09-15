#include <iostream>
#include <string>
#include <time.h>
#include <cstdlib>
#include <sstream>
#include <cctype>

#define sizeOfArray(a) (sizeof(a)/sizeof(a[0]))

using namespace std;

//Setup Functions
int initGrid();
int printGrid();
int userInput();
int computerInput();
int addPoints(string a, string b, string c);
int checkWinner();
string toLower(string str);

//Global Variables
int SIZE = 3;
string grid[3][3];

int main(){//Go through 9 turns unless someone wins. Then break out of the main loop.
    string choice = "";
    initGrid();
    printGrid();
    int keepGoing=1;
    while(keepGoing==1){
        keepGoing=0;
        cout<<"Who goes first: 'player' or 'computer' ?"<<endl;
        cin>>choice;
        choice = toLower(choice);
        if(choice=="player"){

        if(userInput()==1){break;}//If you win stop the loop.
        if(computerInput()==2){break;}//If computer wins stop the loop.
        if(userInput()==1){break;}
        if(computerInput()==2){break;}
        if(userInput()==1){break;}
        if(computerInput()==2){break;}
        if(userInput()==1){break;}
        if(computerInput()==2){break;}
        if(userInput()==1){break;}

        }
    else if(choice=="computer"){

        if(computerInput()==2){break;}
        if(userInput()==1){break;}
        if(computerInput()==2){break;}
        if(userInput()==1){break;}
        if(computerInput()==2){break;}
        if(userInput()==1){break;}
        if(computerInput()==2){break;}
        if(userInput()==1){break;}
        if(computerInput()==2){break;}

        }
    else{
            cout<<"I didn't understand that."<<endl;
            keepGoing=1;
        }
    }
} // end main

int initGrid(){ //Reset the grid
    for(int i=0; i<SIZE; i++)
    {
        for(int j=0; j<SIZE; j++)
        {
            grid[i][j] = "-";
        }
    }
    //Tell User how to play.
    cout<<"The computer will ask you first the X, then the Y position of your coordinates."<<endl;
    cout<<"Where (0,0) is the top left-hand corner, and (2,2) the bottom right-hand corner.";
    cout<<"Enter NUMBERS only, the input of strings or chars will mess up the program."<<endl;
    cout<<"Make sure that you input a whole number from 0 to 2. \n"<<endl;
    return 0;
}

int printGrid(){//Prints out the grid currently.
    for(int i=0; i<SIZE; i++)
    {
        for(int j=0; j<SIZE; j++)
        {
            cout << grid[i][j]  << "  ";
        }
    cout << endl;
    }
    return 0;
}

int userInput(){
    //Ask for input then show that input.
    int turn = 0;
    while(turn==0){
        cout << "Which coordinate do you with to place an X?" << endl;
        int youX = 0; int youY = 0; int areInt=0;
        while(areInt==0){
            areInt=1;
            cout << "X: ";
            cin >> youX;
            cout << "Y: ";
            cin >> youY;
            cout << endl;
        }
        cout << "Your coordinates were (" << youX << "," << youY <<")"<< endl;
        if(youX < SIZE && youY < SIZE){//Make sure condtions are correct
                if(youX >=0 && youY >= 0){//Make sure they are not negative
                    if(grid[youY][youX] == "-"){//Make sure there is a space there.
                        grid[youY][youX] = "X"; //Flip is to get the normal standard grid positioning.
                        printGrid();
                        turn=1;
                    }else{
                        cout << "That spot already has an X or an O in it, chose another."<<endl;
                    }
                }else{
                    cout << "That is invalid, X and Y must not be negative nubers. And make sure they are from 0 to 2."<<endl;
                }
        }else{
            cout << "That is invalid, both X and Y must be an interger from 0 to 2."<<endl;
        }
    }
    cin.ignore(10000,'\n');
    return checkWinner();
}

int computerInput(){
    //NOTE: I know this can be simplified into a single multi-dimensional array loop.
    int eleSIZE = 3;
    //points for every spot on grid.
    int pnt00 = 0;//(0,0)
    int pnt10 = 0;//(1,0)
    int pnt20 = 0;//(2,0)
    int pnt01 = 0;//(0,1)
    int pnt11 = 0;//(1,1)
    int pnt21 = 0;//(2,1)
    int pnt02 = 0;//(0,2)
    int pnt12 = 0;//(1,2)
    int pnt22 = 0;//(2,2)
    int pntArrayR[] = {pnt00,pnt10,pnt20,pnt01,pnt11,pnt21,pnt02,pnt12,pnt22};//Points through rows
    int pntArrayC[] = {pnt00,pnt01,pnt02,pnt10,pnt11,pnt12,pnt20,pnt21,pnt22};//Points through cols
    int pntArrayD[] = {pnt00,pnt11,pnt22,pnt20,pnt11,pnt02};//Points through Diags

    //Lets make the array of 'checks' first and print them
    int c=0;
    string row1[] = {grid[0][0], grid[0][1], grid[0][2]};

    int points = addPoints(row1[0],row1[1],row1[2]);

    //Add points to - elements
    for(int i=0;i<eleSIZE;i++){
        if(row1[i]=="-"){
            pntArrayR[i+c] += points;
            //cout << "+"<<points<<"  ";
        }
    }

    string row2[] = {grid[1][0], grid[1][1], grid[1][2]};
    c += 3;
    points = addPoints(row2[0],row2[1],row2[2]);

    //Add points to - elements
    for(int i=0;i<eleSIZE;i++){
        if(row2[i]=="-"){
            pntArrayR[i+c] += points;
            //cout << "+"<<points<<"  ";
        }
    }

    string row3[] = {grid[2][0], grid[2][1], grid[2][2]};
    c += 3;
    points = addPoints(row3[0],row3[1],row3[2]);

    //Add points to - elements
    for(int i=0;i<eleSIZE;i++){
        if(row3[i]=="-"){
            pntArrayR[i+c] += points;
        }
    }    if(checkWinner()==1){return 1;}
    else if(checkWinner()==2){return 2;}

    c=0;
    string col1[] = {grid[0][0], grid[1][0], grid[2][0]};

    points = addPoints(col1[0],col1[1],col1[2]);

    //Add points to - elements
    for(int i=0;i<eleSIZE;i++){
        if(col1[i]=="-"){
            pntArrayC[i+c] += points;
            //cout << "+"<<points<<"  ";
        }
    }

    string col2[] = {grid[0][1], grid[1][1], grid[2][1]};
    c += 3;
    points = addPoints(col2[0],col2[1],col2[2]);

    //Add points to - elements
    for(int i=0;i<eleSIZE;i++){
        if(col2[i]=="-"){
            pntArrayC[i+c] += points;
            //cout << "+"<<points<<"  ";
        }
    }

    string col3[] = {grid[0][2], grid[1][2], grid[2][2]};
    c += 3;
    points = addPoints(col3[0],col3[1],col3[2]);

    //Add points to - elements
    for(int i=0;i<eleSIZE;i++){
        if(col3[i]=="-"){
            pntArrayC[i+c] += points;
        }
    }
    c=0;
    string dia1[] = {grid[0][0], grid[1][1], grid[2][2]};

    points = addPoints(dia1[0],dia1[1],dia1[2]);

    //Add points to - elements
    for(int i=0;i<eleSIZE;i++){
        if(dia1[i]=="-"){
            pntArrayD[i+c] += points;
        }
    }
    cout << endl;

    string dia2[] = {grid[2][0], grid[1][1], grid[0][2]};
    c += 3;
    points = addPoints(dia2[0],dia2[1],dia2[2]);

    //Add points to - elements
    for(int i=0;i<eleSIZE;i++){
        if(dia2[i]=="-"){
            pntArrayD[i+c] += points;
        }
    }

    //Tally Up into a new array.string col3[] = {grid[0][2], grid[1][2], grid[2][2]};
    int pntArrayA[] = {pntArrayR[0]+pntArrayD[0]+pntArrayC[0], pntArrayR[1]+pntArrayC[3],pntArrayR[2]+pntArrayD[5]+pntArrayC[6], pntArrayR[3]+pntArrayC[1], pntArrayR[4]+pntArrayD[1]+pntArrayD[4]+pntArrayC[4], pntArrayR[5]+pntArrayC[7], pntArrayR[6]+pntArrayD[3]+pntArrayC[2], pntArrayR[7]+pntArrayC[5], pntArrayR[8]+pntArrayD[2]+pntArrayC[8]};
    //Lets find the biggest one
    //cout<<endl;
    cout <<"-------------------\n"<<endl;
    int large=0;
    for(int i=0;i<sizeOfArray(pntArrayA);i++){
        //cout<<pntArrayA[i]<<" "; //Final tally for ALL
        if(pntArrayA[i]>=large){
            large=pntArrayA[i];
        }
    }
    //cout<<" = "<<large<<endl;

    //Now lets place an 'O' in the spot where the lardgest number is.
   if(pntArrayA[0] == large){grid[0][0] = 'O';}
    else if(pntArrayA[1] == large){grid[0][1] = 'O';}
    else if(pntArrayA[2] == large){grid[0][2] = 'O';}
    else if(pntArrayA[3] == large){grid[1][0] = 'O';}
    else if(pntArrayA[4] == large){grid[1][1] = 'O';}
    else if(pntArrayA[5] == large){grid[1][2] = 'O';}
    else if(pntArrayA[6] == large){grid[2][0] = 'O';}
    else if(pntArrayA[7] == large){grid[2][1] = 'O';}
    else if(pntArrayA[8] == large){grid[2][2] = 'O';}

    printGrid();cout<<"\n"<<endl;
    return checkWinner();

}

int addPoints(string num1, string num2, string num3){
    string row[] = {num1,num2,num3};
    int eleSIZE = 3;
    int xAmount=0;
    int oAmount=0;
    int pntsToAdd=0;
    //Get X and Y amount in row
    for(int i=0;i<eleSIZE;i++){
        if(row[i] == "X"){
            xAmount++;
        }
        else if(row[i] == "O"){
            oAmount++;
        }
    }
    //Find how many points to add to - spots
    if(xAmount==0 && oAmount==0){
        pntsToAdd=10;
    }
    else if(xAmount==1 && oAmount==0){
        pntsToAdd=20;
    }
    else if(xAmount==2 && oAmount==0){
        pntsToAdd=100;
    }
    else if(xAmount==0 && oAmount==1){
        pntsToAdd=30;
    }
    else if(xAmount==0 && oAmount==2){
        pntsToAdd=300;
    }
    else if(xAmount==1 && oAmount==1){
        pntsToAdd=5;
    }
    else if(xAmount==3 || oAmount==3 || (xAmount==2 && oAmount==1) || (xAmount==1 && oAmount==2)){
        pntsToAdd=0;
    }

    return pntsToAdd;
}

string toLower(string str){
    for(int i=0;i<str.length();i++){
    str[i] = tolower(str[i]);
    }
    return str;
}

int checkWinner(){
    //Check through and array of rows,cols, and diags. Check if it finds X,X,X or O,O,O.
    string items[8][3] = {{grid[0][0], grid[0][1], grid[0][2]},{grid[1][0], grid[1][1], grid[1][2]},{grid[2][0], grid[2][1], grid[2][2]},{grid[0][0], grid[1][0], grid[2][0]},{grid[0][1], grid[1][1], grid[2][1]},{grid[0][2], grid[1][2], grid[2][2]},{grid[0][0], grid[1][1], grid[2][2]},{grid[2][0], grid[1][1], grid[0][2]}};
    for(int i=0;i<8;i++){
        if(items[i][0] == "X" && items[i][1] == "X" && items[i][2] == "X"){
            cout << "You Win!"<<endl;
            return 1;
        }
        else if(items[i][0] == "O" && items[i][1] == "O" && items[i][2] == "O"){
            cout << "You Lose!"<<endl;
            return 2;
        }
    }

}

