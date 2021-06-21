/*                      HiLCoE
        School of Computer Science and Technology

        Group Project on DAMA Game

        Done By: Elroe Tadele
                 Fitsum Mekonen
                 Ezana Hailesellasie

        DRB1802 Section B

        Submitted To: Mr. Atnafu

        May 11,2019
*/
#include<iostream>
using namespace std;
#include <conio.h>
#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctime>
#include <ctype.h>
#include <fstream>
struct cell
{
    int row;
    int col;
    int type; //type of cell: 0 not occupied, 1 occupied by player 1 soldier ,2 occupied by player 1 king, 3 occupied by player 2 soldier,4 occupied by player 2 king
};
struct game
{
    int cells[8][8]; //contains the type of cell in the board
    char player1[80]; //Name of player 1
    char player2[80]; // name of player2
};
struct ai
{
    cell c;
    int direction;//1 for left top, 2 for right top, 3 for left bottom, 4 for right bottom
};
const int BOARD_WIDTH 	= 400;
const int BOARD_HEIGHT	= 400;
const int LEFT_CORNER	= 20;
const int TOP_CORNER	= 10;
const int NB_ROWS  	= 8;
const int NB_COLS  	= 8;
const int CELL_SIZE	= BOARD_HEIGHT/NB_ROWS;
const int STATUS_AREA_WIDTH  = 190;
const int STATUS_AREA_HEIGHT= 50;
const int STATUS_AREA_LEFT = LEFT_CORNER + BOARD_WIDTH + 10;
const int STATUS_AREA_TOP1 = TOP_CORNER;
const int STATUS_AREA_TOP2 = TOP_CORNER + 230;
const int INFO_AREA_WIDTH  = 600;
const int INFO_AREA_HEIGHT= 40;
const int INFO_AREA_LEFT = LEFT_CORNER;
const int INFO_AREA_TOP = TOP_CORNER+ + BOARD_HEIGHT + 10;
const int GOVER_WIDTH = 540;
const int GOVER_HEIGHT = 200;
const int CELL1_COLOR =WHITE;
const int CELL2_COLOR =LIGHTGRAY;
const int PLAYER1_COLOR =WHITE;//BLUE;
const int PLAYER2_COLOR =BROWN;//RED;
const int INFO_COLOR =WHITE;
const int ESC =27;
const int NEW_GAME ='n';
const int SAVED_GAME = 'l';
const int SAVE_GAME = 's';
int getCellColor(cell c);    //returns cell back color
void putQuarki(cell, int &);  //put qorki on board
void drawKing(cell qurki);      //makes a given soilder king (not implemented}
void putQuarkis(cell quarkis[], int n, int c[][8]); //put several quarkis
void drawCell(cell c); //draw any type of cell empty, solider, and king
void drawBoard();   //draw board
void getPlayer(int playerId, char name[]); //get player  name
void displayInfo(char *msg);    //display message in the information area
void displayStatus1(char *msg); // display playe1 status
void displayStatus2(char *msg); // display player2 status
void  initialize(game & g, int & turn,int newGame); //initialize new or saved game
cell getCell(int x, int y, int cells[][8]);  // get cell from pixel position
void moveQuarki(cell startCell, cell destCell, game &g, int &turn); //move quarki
int validmove(cell startcell, cell destcell, game &g, int pass=0);
int  checkmiddleCell(cell destcell, cell startcell, game g, cell &mid);
void removeMidCell(cell &m, game &g);
void changeTurn(int &turn);
int turnError(int turn, cell selectedcell);
void updateScore(int type, int n=0);
void printScore(int score, int type);
void displayName1(char *msg);
void displayName2(char *msg);
int checkGameOver(game g);
void gameOverpopup(char* player1, char* player2, int saved);
int winner(int score1, int score2);
void printFinalWinner(char*player1,char*player2);
void saveGame(game g, int turn);
void retrieveSavedGame(game &g, int &turn, int i);
void retrieveScore(int &score1, int &score2, int i);
int checkSavedGame();
int checkSavedName(char*player1,char*player2,int &n);
void deleteSavedGame(int i);
void makeKing(cell c, game &g);
void playAI(game &g);
void AIturn(game &g, int &giveUp, int &turn);
int cntQuarki(game g);
int checkEatQuarki(game g, ai n[12], int &p);
void findEatQuarki(game &g,ai n[],int p, int &turn);
int checkAbleMove(game g, ai m[],int &q);
void moveAbleQuarki(game &g,ai m[], int q, int &turn);
int validcell(cell c);


int validcell(cell c)
{
    int y=TOP_CORNER+(c.row*CELL_SIZE),x=LEFT_CORNER+(c.col*CELL_SIZE);
    if(x<=LEFT_CORNER || x>= LEFT_CORNER + BOARD_WIDTH || y<=TOP_CORNER || y>= TOP_CORNER + BOARD_HEIGHT )
        return 0;
    else
        return 1;
}

int getCellColor(cell c)//EZ
{
    if(c.row%2 == 0 && c.col%2!=0 ||  c.row%2 != 0 && c.col%2==0)
    {
        return CELL2_COLOR;
    }
    else
    {
        return CELL1_COLOR;
    }
}
void putQuarki(cell quarki, int & cell)
{

    int x, y, fcolor;
    x=LEFT_CORNER + quarki.col*CELL_SIZE;
    y=TOP_CORNER + quarki.row*CELL_SIZE;
    if(quarki.type==1)
    {
        setcolor(BLACK);
        fcolor=PLAYER1_COLOR;
        setfillstyle(SOLID_FILL, fcolor);
        fillellipse(x+0.5*CELL_SIZE , y + 0.5*CELL_SIZE, CELL_SIZE/2.5,CELL_SIZE/2.5);
        fillellipse(x+0.5 * CELL_SIZE , y + 0.5 *CELL_SIZE, CELL_SIZE/2.8,CELL_SIZE/2.8);
    }
    else if(quarki.type == 3)
    {
        setcolor(WHITE);
        fcolor=PLAYER2_COLOR;
        setfillstyle(SOLID_FILL, fcolor);
        fillellipse(x+0.5*CELL_SIZE , y + 0.5*CELL_SIZE, CELL_SIZE/2.5,CELL_SIZE/2.5);
        fillellipse(x+0.5 * CELL_SIZE , y + 0.5 *CELL_SIZE, CELL_SIZE/2.8,CELL_SIZE/2.8);
    }
    else if(quarki.type==2 || quarki.type==4)
        drawKing(quarki);

    cell=quarki.type;

}
void drawKing(cell c)
{
    int x, y, fcolor;
    x=LEFT_CORNER + c.col*CELL_SIZE;
    y=TOP_CORNER + c.row*CELL_SIZE;
    if(c.type==2)
    {
        fcolor=PLAYER1_COLOR;
        setcolor(BLACK);
        setfillstyle(SOLID_FILL, fcolor);
        fillellipse(x+0.5*CELL_SIZE , y + 0.5*CELL_SIZE, CELL_SIZE/2.5,CELL_SIZE/2.5);
        fillellipse(x+0.5 * CELL_SIZE , y + 0.5 *CELL_SIZE, CELL_SIZE/2.8,CELL_SIZE/2.8);
        fillellipse(x+0.5 * CELL_SIZE , y + 0.5 *CELL_SIZE, CELL_SIZE/4.0,CELL_SIZE/4.0);

        setfillstyle(SOLID_FILL, fcolor);
        fillellipse(x+0.5 * CELL_SIZE , y + 0.5 *CELL_SIZE, CELL_SIZE/5.0,CELL_SIZE/5.0);
        fillellipse(x+0.5 * CELL_SIZE , y + 0.5 *CELL_SIZE, CELL_SIZE/7.0,CELL_SIZE/7.0);
    }
    else if (c.type==4)
    {
        fcolor=PLAYER2_COLOR;
        setcolor(WHITE);
        setfillstyle(SOLID_FILL, fcolor);
        fillellipse(x+0.5*CELL_SIZE , y + 0.5*CELL_SIZE, CELL_SIZE/2.5,CELL_SIZE/2.5);
        fillellipse(x+0.5 * CELL_SIZE , y + 0.5 *CELL_SIZE, CELL_SIZE/2.8,CELL_SIZE/2.8);
        fillellipse(x+0.5 * CELL_SIZE , y + 0.5 *CELL_SIZE, CELL_SIZE/4.0,CELL_SIZE/4.0);

        setfillstyle(SOLID_FILL, fcolor);
        fillellipse(x+0.5 * CELL_SIZE , y + 0.5 *CELL_SIZE, CELL_SIZE/5.0,CELL_SIZE/5.0);
        fillellipse(x+0.5 * CELL_SIZE , y + 0.5 *CELL_SIZE, CELL_SIZE/7.0,CELL_SIZE/7.0);
    }


}
void putQuarkis(cell quarkis[], int n, int c[][8])
{
    for(int i=0; i< n; i++)
    {
        putQuarki(quarkis[i], c[quarkis[i].row][quarkis[i].col]);
        if(quarkis[i].type==2 || quarkis[i].type==4)
            drawKing(quarkis[i]);
        //delay(500);
    }
}
void drawCell(cell c)
{
    int fcolor;
    int x, y;
    x=LEFT_CORNER + c.col*CELL_SIZE;
    y=TOP_CORNER + c.row*CELL_SIZE;
    fcolor=getCellColor(c);
    setfillstyle(SOLID_FILL, fcolor);
    bar(x,y,x+CELL_SIZE,y+CELL_SIZE);
}
void displayName1(char *msg)
{
    /* int i=0;
     while(msg[i]!='\n')
     {
         msg[i]=toupper(char(msg[i]));
         i++;
     }*/
    setcolor(PLAYER1_COLOR);
    setfillstyle(SOLID_FILL, BLACK/*PLAYER1_COLOR*/);
    bar(STATUS_AREA_LEFT,STATUS_AREA_TOP1,STATUS_AREA_LEFT+STATUS_AREA_WIDTH, STATUS_AREA_TOP1+STATUS_AREA_HEIGHT);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
    outtextxy(STATUS_AREA_LEFT+15,STATUS_AREA_TOP1+15,msg);
    setcolor(WHITE);
}

void displayName2(char *msg)
{
    setcolor(PLAYER2_COLOR);
    setfillstyle(SOLID_FILL, BLACK/*PLAYER2_COLOR*/);
    bar(STATUS_AREA_LEFT,STATUS_AREA_TOP2,STATUS_AREA_LEFT+STATUS_AREA_WIDTH, STATUS_AREA_TOP2+STATUS_AREA_HEIGHT);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
    outtextxy(STATUS_AREA_LEFT+15,STATUS_AREA_TOP2+15,msg);
    setcolor(WHITE);
}


void drawBoard()
{
    cell c;
    for(int i=0; i< NB_ROWS; i++)
    {
        for(int j=0; j<NB_COLS; j++)
        {
            c.row=i;
            c.col=j;
            c.type=0;
            drawCell(c);
        }
    }
    //STATUS 1
    setcolor(WHITE);
    setfillstyle(SOLID_FILL, PLAYER1_COLOR);
    bar(STATUS_AREA_LEFT,STATUS_AREA_TOP1+60,STATUS_AREA_LEFT+STATUS_AREA_WIDTH,
        STATUS_AREA_TOP1+60+STATUS_AREA_HEIGHT);
    //STATUS 2
    setcolor(WHITE);
    setfillstyle(SOLID_FILL, PLAYER2_COLOR);
    bar(STATUS_AREA_LEFT,STATUS_AREA_TOP2+60,STATUS_AREA_LEFT+STATUS_AREA_WIDTH,
        STATUS_AREA_TOP2+60+STATUS_AREA_HEIGHT);
    //NAME 1
    setcolor(PLAYER1_COLOR);
    setfillstyle(SOLID_FILL,BLACK/*PLAYER1_COLOR*/);
    bar(STATUS_AREA_LEFT,STATUS_AREA_TOP1,STATUS_AREA_LEFT+STATUS_AREA_WIDTH,
        STATUS_AREA_TOP1+STATUS_AREA_HEIGHT);
    //NAME 2
    setcolor(PLAYER2_COLOR);
    setfillstyle(SOLID_FILL, BLACK/*PLAYER2_COLOR*/);
    bar(STATUS_AREA_LEFT,STATUS_AREA_TOP2,STATUS_AREA_LEFT+STATUS_AREA_WIDTH,
        STATUS_AREA_TOP2+STATUS_AREA_HEIGHT);
    //SCORE 1
    setcolor(WHITE);
    setfillstyle(SOLID_FILL, PLAYER1_COLOR);
    bar(STATUS_AREA_LEFT,STATUS_AREA_TOP1+2*STATUS_AREA_HEIGHT+20,STATUS_AREA_LEFT+STATUS_AREA_WIDTH,
        STATUS_AREA_TOP2-60);
    //SCORE 2
    setcolor(WHITE);
    setfillstyle(SOLID_FILL, PLAYER2_COLOR);
    bar(STATUS_AREA_LEFT,STATUS_AREA_TOP2+2*STATUS_AREA_HEIGHT+20,STATUS_AREA_LEFT+STATUS_AREA_WIDTH,
        STATUS_AREA_TOP2+3*STATUS_AREA_HEIGHT+20);



}
void getPlayer(int playerId, char name[])
{
    cout << "Enter Player"  << playerId << " name: ";
    cin.getline(name, 79);
}
void displayInfo(char *msg)
{
    setfillstyle(SOLID_FILL, INFO_COLOR);
    bar(INFO_AREA_LEFT,INFO_AREA_TOP,INFO_AREA_LEFT+INFO_AREA_WIDTH,INFO_AREA_TOP+INFO_AREA_HEIGHT);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1.5);
    outtextxy(INFO_AREA_LEFT+5,INFO_AREA_TOP+5,msg);
}
void displayStatus1(char *msg)
{
    setcolor(WHITE);
    setfillstyle(SOLID_FILL, PLAYER1_COLOR);
    bar(STATUS_AREA_LEFT,STATUS_AREA_TOP1+60,STATUS_AREA_LEFT+STATUS_AREA_WIDTH, STATUS_AREA_TOP1+60+STATUS_AREA_HEIGHT);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    outtextxy(STATUS_AREA_LEFT+15,STATUS_AREA_TOP1+75,msg);
}
void displayStatus2(char *msg)
{
    setcolor(WHITE);
    setfillstyle(SOLID_FILL, PLAYER2_COLOR);
    bar(STATUS_AREA_LEFT,STATUS_AREA_TOP2+60,STATUS_AREA_LEFT+STATUS_AREA_WIDTH,STATUS_AREA_TOP2+60+STATUS_AREA_HEIGHT);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    outtextxy(STATUS_AREA_LEFT+15,STATUS_AREA_TOP2+75,msg);
}

void printScore(int score, int type)//ANA
{
    char arr[50];
    if(type==1)
    {
        setcolor(WHITE);
        setfillstyle(SOLID_FILL, PLAYER1_COLOR);
        bar(STATUS_AREA_LEFT,STATUS_AREA_TOP1+2*STATUS_AREA_HEIGHT+20,STATUS_AREA_LEFT+STATUS_AREA_WIDTH, STATUS_AREA_TOP2-60);
        setcolor(YELLOW);
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
        sprintf(arr,"SCORE: %d",score);
        outtextxy(STATUS_AREA_LEFT+15,STATUS_AREA_TOP1+2*STATUS_AREA_HEIGHT+35,arr);
    }
    else if(type==2)
    {
        setcolor(WHITE);
        setfillstyle(SOLID_FILL, PLAYER2_COLOR);
        bar(STATUS_AREA_LEFT,STATUS_AREA_TOP2+2*STATUS_AREA_HEIGHT+20,STATUS_AREA_LEFT+STATUS_AREA_WIDTH,STATUS_AREA_TOP2+3*STATUS_AREA_HEIGHT+20);
        setcolor(YELLOW);
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
        sprintf(arr,"SCORE: %d",score);
        outtextxy(STATUS_AREA_LEFT+15,STATUS_AREA_TOP2+2*STATUS_AREA_HEIGHT+35,arr);
    }
    setcolor(WHITE);
}


cell getCell(int x, int y, int cells[][8])//FIT
{
    cell c;
    if(x<=LEFT_CORNER || x>= LEFT_CORNER + BOARD_WIDTH || y<=TOP_CORNER || y>= TOP_CORNER + BOARD_HEIGHT )
    {
        c.row=-1;
        c.col=-1;
    }
    else
    {
        c.row=(y-TOP_CORNER)/CELL_SIZE;
        c.col=(x-LEFT_CORNER)/CELL_SIZE;
    }
    c.type=cells[c.row][c.col];
    return c;
}

int validmove(cell startcell, cell destcell, game &g, int pass)
{

    if (getCellColor(destcell)== CELL1_COLOR)
        return 0;
    else if(destcell.type!=0)
        return 0;
    else if(startcell.col==destcell.col || startcell.row==destcell.row)
        return 0;
    else if(startcell.type == 1 && destcell.row < startcell.row )
        return 0;
    else if(startcell.type == 3 && destcell.row > startcell.row )
        return 0;
    else if((destcell.col-startcell.col)==(destcell.row-startcell.row) || (destcell.col-startcell.col)==(-1*(destcell.row-startcell.row)))
    {
        int n=destcell.col-startcell.col;
        if(n>1 || n<-1)
        {
            cell m;
            if(checkmiddleCell(destcell,startcell,g,m))
            {
                if(pass==0)
                {
                    if((m.type==2 && startcell.type==4)||(m.type==4 && startcell.type==2))
                    {
                        updateScore(startcell.type);
                        updateScore(startcell.type);

                    }
                    else
                        updateScore(startcell.type);

                    removeMidCell(m,g);

                }
                return 1;
            }
            else
                return 0;
        }
        else
        {
            return 1;
        }
    }

    else
        return 0;

}
void moveQuarki(cell startCell, cell destCell, game &g, int &turn)
{
    if(!turnError(turn,startCell))
    {
        if(validmove(startCell,destCell,g))
        {
            drawCell(startCell);
            destCell.type=startCell.type;
            putQuarki(destCell, g.cells[destCell.row][destCell.col]);
            makeKing(destCell,g);
            startCell.type=0;
            g.cells[startCell.row][startCell.col]=startCell.type;
            changeTurn(turn);
        }
        else
        {
            displayInfo("Invalid move!");
            delay(1000);
            displayInfo("              ");
        }
    }
}
int  checkmiddleCell(cell destcell, cell startcell, game g, cell &mid)
{
    if(startcell.type==1)
    {
        if(startcell.col>destcell.col)
        {
            mid.row=startcell.row+1;
            mid.col=startcell.col-1;
            if(g.cells[mid.row][mid.col]==3)
                return 1;
            else
                return 0;
        }
        else if(startcell.col<destcell.col)
        {
            mid.row=startcell.row+1;
            mid.col=startcell.col+1;
            if(g.cells[mid.row][mid.col]==3)
                return 1;
            else
                return 0;
        }
    }
    else if(startcell.type==3)
    {
        if(startcell.col>destcell.col)
        {
            mid.row=startcell.row-1;
            mid.col=startcell.col-1;
            if(g.cells[mid.row][mid.col]==1)
                return 1;
            else
                return 0;

        }
        else if(startcell.col<destcell.col)
        {
            mid.row=startcell.row-1;
            mid.col=startcell.col+1;
            if(g.cells[mid.row][mid.col]==1)
                return 1;
            else
                return 0;
        }
    }
    else if(startcell.type==2)
    {
        if(startcell.col>destcell.col && startcell.row>destcell.row)
        {
            mid.row=startcell.row-1;
            mid.col=startcell.col-1;
            if(g.cells[mid.row][mid.col]==3 || (g.cells[mid.row][mid.col]==4))
                return 1;
            else
                return 0;
        }

        else if(startcell.col<destcell.col && startcell.row>destcell.row)
        {
            mid.row=startcell.row-1;
            mid.col=startcell.col+1;
            if(g.cells[mid.row][mid.col]==3 || (g.cells[mid.row][mid.col]==4))
                return 1;
            else
                return 0;
        }

        else if(startcell.col>destcell.col && startcell.row<destcell.row)
        {
            mid.row=startcell.row+1;
            mid.col=startcell.col-1;
            if(g.cells[mid.row][mid.col]==3 || (g.cells[mid.row][mid.col]==4))
                return 1;
            else
                return 0;
        }

        else if(startcell.col<destcell.col && startcell.row<destcell.row)
        {
            mid.row=startcell.row+1;
            mid.col=startcell.col+1;
            if(g.cells[mid.row][mid.col]==3 || (g.cells[mid.row][mid.col]==4))
                return 1;
            else
                return 0;
        }
    }
    else
    {
        if(startcell.col>destcell.col && startcell.row>destcell.row)
        {
            mid.row=startcell.row-1;
            mid.col=startcell.col-1;
            if(g.cells[mid.row][mid.col]==1 || (g.cells[mid.row][mid.col]==2))
                return 1;
            else
                return 0;
        }

        else if(startcell.col<destcell.col && startcell.row>destcell.row)
        {
            mid.row=startcell.row-1;
            mid.col=startcell.col+1;
            if(g.cells[mid.row][mid.col]==1 || (g.cells[mid.row][mid.col]==2))
                return 1;
            else
                return 0;
        }

        else if(startcell.col>destcell.col && startcell.row<destcell.row)
        {
            mid.row=startcell.row+1;
            mid.col=startcell.col-1;
            if(g.cells[mid.row][mid.col]==1 || (g.cells[mid.row][mid.col]==2))
                return 1;
            else
                return 0;
        }

        else if(startcell.col<destcell.col && startcell.row<destcell.row)
        {
            mid.row=startcell.row+1;
            mid.col=startcell.col+1;
            if(g.cells[mid.row][mid.col]==1 || (g.cells[mid.row][mid.col]==2))
                return 1;
            else
                return 0;
        }

    }

}

void removeMidCell(cell &m, game &g)
{
    m.type=0;
    g.cells[m.row][m.col]=m.type;
    drawCell(m);
}

void makeKing(cell c, game &g)
{
    if(c.type ==1 && c.row == 7)
    {
        c.type=2;
        drawKing(c);
        g.cells[c.row][c.col]=c.type;
    }
    else if(c.type == 3 && c.row==0)
    {
        c.type=4;
        drawKing(c);
        g.cells[c.row][c.col]=c.type;
    }
    else
        putQuarki(c, g.cells[c.row][c.col]);
}


int turnError(int turn, cell selectedcell)
{
    if(turn==1 && (selectedcell.type==3 || selectedcell.type ==4))
    {
        displayInfo("Wrong turn!");
        delay(1000);
        return 1;
    }
    else if(turn==2 &&(selectedcell.type==1 || selectedcell.type==2))
    {
        displayInfo("Wrong turn!");
        delay(1000);
        return 1;
    }
    else
        return 0;


}
void displayTurn(int turn)
{
    if(turn==1)
    {
        displayStatus2("    ");
        displayStatus1("TURN");
        delay(300);
        displayStatus1("    ");
        delay(300);
        displayStatus1("TURN");
    }
    else
    {
        displayStatus1("    ");
        displayStatus2("TURN");
        delay(300);
        displayStatus2("    ");
        delay(300);
        displayStatus2("TURN");
    }
}

void changeTurn(int &turn)//SUM
{
    if(turn==1)
        turn=2;
    else
        turn=1;
    displayTurn(turn);
}
void saveScore(int score1, int score2)
{
    ofstream ofs;
    ofs.open("scores.bin",ios::binary|ios::app);
    ofs.write((char*)&score1, sizeof(int));
    ofs.write((char*)&score2, sizeof(int));
    ofs.close();
}

void loadNewGame(cell quarkis[])
{
    //quarkis[n]= {{0,1,1}, {0,3,1}, {0,5,1}, {0,7,1}, {1,0,1},{1,2,1},{1,4,1},{1,6,1}, {2,1,1},{2,3,1}, {2,5,1}, {2,7,1}, {5,0,3}, {5,2,3}, {5,4,3}, {5,6,3}, {6,1,3},{6,3,3}, {6,5,3}, {6,7,3}, {7,0,3}, {7,2,3}, {7,4,3}, {7,6,3}};
    int k=0;
    for(int i=0; i<NB_ROWS; i++)
    {
        if(i==3 || i==4)
            continue;
        else
        {
            for(int j=0; j<NB_COLS; j++)
            {
                if((i%2==0 && j%2==0)|| (i%2!=0 && j%2!=0))
                    continue;
                else
                {
                    if(i<3)
                    {
                        quarkis[k].row=i;
                        quarkis[k].col=j;
                        quarkis[k].type=1;
                        k++;
                    }
                    else
                    {
                        quarkis[k].row=i;
                        quarkis[k].col=j;
                        quarkis[k].type=3;
                        k++;
                    }
                }
            }
        }
    }
}

void loadretrieveSavedGame(game g, cell quarkis[], int &k)
{
    k=0;
    for(int i=0; i<NB_ROWS; i++)
    {
        for(int j=0; j<NB_COLS; j++)
        {
            if(g.cells[i][j]!=0)
            {
                quarkis[k].row=i;
                quarkis[k].col=j;
                quarkis[k].type=g.cells[i][j];
                k++;
            }
        }
    }
}

void  initialize(game & g,int & turn,int newGame)
{
    for(int i=0; i<NB_ROWS; i++)
    {
        for(int j=0; j<NB_COLS; j++)
            g.cells[i][j]=0;
    }
    //drawBoard();
    displayInfo("Select(R Click). Move (L Click). Quit(ESC).");
    int n=24,k;
    cell quarkis[n];
    if(newGame==1)         //new game left for you
    {
        updateScore(0);
        loadNewGame(quarkis);
        putQuarkis(quarkis, n, g.cells);
        turn=rand()%2 + 1;
        displayTurn(turn);
    }
    else              //saved game left for you
    {
        int n,tmp;
        tmp=checkSavedName(g.player1,g.player2,n);
        retrieveSavedGame(g,turn,n);
        updateScore(-2,n);
        loadretrieveSavedGame(g,quarkis,k);
        putQuarkis(quarkis, k, g.cells);
        displayTurn(turn);

    }
    displayName1(g.player1);
    displayName2(g.player2);

}

void retrieveScore(int &score1, int &score2, int n)
{
    ifstream ifs;
    ifs.open("scores.bin", ios::in|ios::binary);
    int k=(n/(sizeof(game)+sizeof(int)))*(sizeof(int)+sizeof(int));
    int tmp1,tmp2;
    if(ifs==NULL)
    {
        score1=0;
        score2=0;
    }
    else
    {
        ifs.seekg(k,ios::beg);
        ifs.read((char*)&tmp1,sizeof(int));
        score1=tmp1;
        ifs.read((char*)&tmp2,sizeof(int));
        score2=tmp2;
    }
    ifs.close();
}
void updateScore(int type,int n)
{
    static int n1=0,n2=0;
    if(type==1)
    {
        n1+=10;
        printScore(n1,1);
    }
    else if(type==2)
    {
        n1+=20;
        printScore(n1,1);
    }
    else if(type==3)
    {
        n2+=10;
        printScore(n2,2);
    }
    else if(type==4)
    {
        n2+=20;
        printScore(n2,2);
    }
    else if(type==0)//newgame
    {
        n1=0;
        n2=0;
        printScore(n1,1);
        printScore(n2,2);
    }
    else if(type==-1)//save game
    {
        saveScore(n1,n2);
    }
    else //saved game
    {
        retrieveScore(n1,n2,n);
        printScore(n1,1);
        printScore(n2,2);
    }
    int currentWinner=winner(n1,n2);
}


int checkGameOver(game g)
{
    int b[24],n=0,cnt1=0,cnt2=0;
    for(int i=0; i<NB_ROWS; i++)
    {
        for(int j=0; j<NB_COLS; j++)
        {
            if(g.cells[i][j]!=0)
            {
                b[n]=g.cells[i][j];
                n++;
            }
        }
    }
    for(int i=0; i<=n; i++)
    {
        if(b[i]==1||b[i]==2)
            cnt1++;
        else
            cnt2++;
    }
    if(n<=2)
        return 1;
    else if(cnt1<=1 || cnt2<=1)
        return 1;
    else
        return 0;

}

void gameOverpopup(char* player1, char* player2, int saved)
{
    setcolor(WHITE);
    setfillstyle(SOLID_FILL, YELLOW);
    bar(LEFT_CORNER+30,TOP_CORNER+100,LEFT_CORNER+30+GOVER_WIDTH,TOP_CORNER+100+GOVER_HEIGHT);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
    if(!saved)
    {
        outtextxy(LEFT_CORNER+70,TOP_CORNER+150,"GAME OVER!!");
        printFinalWinner(player1,player2);
    }
    else
        outtextxy(LEFT_CORNER+70,TOP_CORNER+150,"GAME SAVED!!");

    delay(5000);
    setfillstyle(SOLID_FILL, BLACK);
    bar(LEFT_CORNER+30,TOP_CORNER+100,LEFT_CORNER+30+GOVER_WIDTH,TOP_CORNER+100+GOVER_HEIGHT);
    drawBoard();
    displayName1(player1);
    displayName2(player2);
    displayInfo("Press 'n' for new game, 'l' for saved game, 'ESC' to escape");
}

void newGame(game &g)
{
    updateScore(0);
    int turn,x,y,saved=0;
    char key,ans;
    cell selectedCell= {-1,-1,0};
    cell destCell=selectedCell;
    initialize(g,turn,1);
    displayInfo("Select(R Click). Move (L Click). Save Game(s). Quit(ESC).");
    do
    {
        if(kbhit())
        {
            key=getch();
            switch (key)
            {
            case ESC:
                displayInfo ("Are you sure you want to end the game?(Y/y) to end any other key to return.");
                do
                {
                    if(kbhit())
                    {
                        ans=getch();
                        if(ans=='y' || ans=='Y')
                        {
                            displayInfo ("Come Again ... ");
                            delay(1000);
                            restorecrtmode();
                            exit(1);
                        }
                        else
                            break;
                    }
                }
                while(1);
                displayInfo("Select(R Click). Move (L Click). Save Game(s). Quit(ESC).");
                break;
            /*case NEW_GAME:
                initialize(g,turn,1);
                break;*/
            case SAVE_GAME:
                saveGame(g,turn);
                updateScore(-1);
                saved=1;
                break;
            }

        }

        if(ismouseclick(WM_RBUTTONDOWN))
        {
            getmouseclick(WM_RBUTTONDOWN, x,y);
            selectedCell=getCell(x,y,g.cells);
            clearmouseclick(WM_RBUTTONDOWN);
        }
        else if(ismouseclick(WM_LBUTTONDOWN))
        {
            getmouseclick(WM_LBUTTONDOWN, x,y);
            destCell=getCell(x,y,g.cells);
            if(selectedCell.type != 0)
            {
                moveQuarki(selectedCell, destCell,g,turn);
                displayInfo("Select(R Click). Move (L Click). Save Game(s). Quit(ESC).");
                selectedCell.row =selectedCell.col=-1;
                selectedCell.type=0;
            }
            clearmouseclick(WM_LBUTTONDOWN);
        }
    }
    while(!checkGameOver(g) && !saved);
    gameOverpopup(g.player1,g.player2, saved);
}

void savedGame(game &g)
{
    int turn,x,y,saved=0,n,giveUp=0;
    char key,ans;
    cell selectedCell= {-1,-1,0};
    cell destCell=selectedCell;
    if(checkSavedName(g.player1,g.player2,n))
    {
        updateScore(-2,n);
        initialize(g,turn,2);
        deleteSavedGame(n);
    }
    else
    {
        updateScore(0);
        initialize(g,turn,1);
    }
    displayInfo("Select(R Click). Move (L Click). Save Game(s). Quit(ESC).");
    do
    {
        if(kbhit())
        {
            key=getch();
            switch (key)
            {
            case ESC:
                displayInfo ("Are you sure you want to end the game?(Y/y) to end any other key to return.");
                do
                {
                    if(kbhit())
                    {
                        ans=getch();
                        if(ans=='y' || ans=='Y')
                        {
                            displayInfo ("Come Again ... ");
                            delay(1000);
                            restorecrtmode();
                            exit(1);
                        }
                        else
                            break;
                    }
                }
                while(1);
                displayInfo("Select(R Click). Move (L Click). Save Game(s). Quit(ESC).");
                break;
            /*case NEW_GAME:
                initialize(g,turn,1);
                break;*/
            case SAVE_GAME:
                saveGame(g,turn);
                updateScore(-1);
                saved=1;
                break;
            }

        }

        if(ismouseclick(WM_RBUTTONDOWN))
        {
            getmouseclick(WM_RBUTTONDOWN, x,y);
            selectedCell=getCell(x,y,g.cells);
            clearmouseclick(WM_RBUTTONDOWN);
        }
        else if(ismouseclick(WM_LBUTTONDOWN))
        {
            getmouseclick(WM_LBUTTONDOWN, x,y);
            destCell=getCell(x,y,g.cells);
            if(selectedCell.type != 0)
            {
                moveQuarki(selectedCell, destCell,g,turn);
                displayInfo("Select(R Click). Move (L Click). Save Game(s). Quit(ESC).");
                selectedCell.row =selectedCell.col=-1;
                selectedCell.type=0;
            }
            clearmouseclick(WM_LBUTTONDOWN);
        }
        if(strcmp(g.player2,"COMPUTER")==0)
        {
            if(turn ==2)
            {
                delay(300);
                AIturn(g,giveUp,turn);
            }

        }
    }
    while(!checkGameOver(g) && !saved && !giveUp);
    gameOverpopup(g.player1,g.player2, saved);
}
int winner(int score1, int score2)
{
    static int win;
    if(score1<0 || score2<0);
    else if(score1>score2)
        win=1;
    else if(score2>score1)
        win=2;
    else
        win=0;
    return win;
}

void printFinalWinner(char*player1,char*player2)
{
    //setcolor(YELLOW);
    char arr[50];
    int win=winner(-1,-1);
    if(win==1)
    {
        sprintf(arr,"WINNER : %s",player1);
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
        outtextxy(LEFT_CORNER+70,TOP_CORNER+230,arr);
    }
    else if(win==2)
    {
        sprintf(arr,"WINNER : %s",player2);
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
        outtextxy(LEFT_CORNER+70,TOP_CORNER+230,arr);
    }
    else
    {
        settextstyle(DEFAULT_FONT, HORIZ_DIR,3);
        outtextxy(LEFT_CORNER+70,TOP_CORNER+230,"TIE!!");
    }
    //setcolor(WHITE);
}
void initializeDriver()
{
    int gdriver = DETECT, gmode;
    initgraph(&gdriver, &gmode, "...\\BGI");
    int errorcode=graphresult();
    if(errorcode!=grOk)
    {
        cout << grapherrormsg(errorcode);
        getch();
        exit(1);
    }
}
void saveGame(game g, int turn)
{
    ofstream ofs;
    ofs.open("saved.bin",ios::binary|ios::app);
    int tmp;
    char name[80];
    strcpy(name,g.player1);
    ofs.write((char*)&name,sizeof(name));
    strcpy(name,g.player2);
    ofs.write((char*)&name,sizeof(name));
    for(int i=0; i<NB_ROWS; i++)
    {
        for(int j=0; j<NB_COLS; j++)
        {
            tmp=g.cells[i][j];
            ofs.write((char*)&tmp, sizeof(int));
        }
    }
    tmp=turn;
    ofs.write((char*)&tmp, sizeof(int));
    ofs.close();

}

void retrieveSavedGame(game &g, int &turn, int i)
{
    ifstream ifs;
    ifs.open("saved.bin", ios::in|ios::binary);
    int tmp;
    char name1[80], name2[80];
    ifs.seekg(i,ios::beg);
    ifs.read((char*)&name1,sizeof(name1));
    strcpy(g.player1,name1);
    ifs.read((char*)&name2,sizeof(name2));
    strcpy(g.player2,name2);
    for(int i=0; i<NB_ROWS; i++)
    {
        for(int j=0; j<NB_COLS; j++)
        {
            ifs.read((char*)&tmp,sizeof(int));
            g.cells[i][j]=tmp;
        }
    }
    ifs.read((char*)&tmp,sizeof(int));
    turn=tmp;
    ifs.close();
}

int checkSavedGame()
{
    ifstream ifs;
    ifs.open("saved.bin", ios::in|ios::binary);
    if(ifs==NULL)
    {
        ifs.close();
        return 0;
    }
    else
    {
        ifs.close();
        return 1;
    }
}

int checkSavedName(char*player1,char*player2,int &n)
{
    int s=sizeof(game)+sizeof(int);
    n=0;
    ifstream ifs;
    if(checkSavedGame)
    {
        ifs.open("saved.bin",ios::in|ios::binary);
        char name1[80],name2[80];
        while(!ifs.eof())
        {
            ifs.seekg(n,ios::beg);
            ifs.read((char*)&name1,sizeof(name1));
            ifs.read((char*)&name2,sizeof(name2));
            if(strcmpi(name1,player1)==0 && strcmpi(name2,player2)==0)
            {
                ifs.close();
                return 1;
            }
            else
                n+=s;
        }
        ifs.close();
        return 0;
    }
    else
        return 0;
}

void deleteSavedScore(int i)
{
    ofstream ofs;
    ofs.open("scores.bin",ios::binary|ios::ate);
    int k=(i/(sizeof(game)+sizeof(int)))*(sizeof(int)+sizeof(int));
    ofs.seekp(k,ios::beg);
    int tmp=-1;
    ofs.write((char*)&tmp,sizeof(int));
    ofs.write((char*)&tmp,sizeof(int));
    ofs.close();
}


void deleteSavedGame(int n)
{
    ofstream ofs;
    ofs.open("saved.bin",ios::binary|ios::ate);
    ofs.seekp(n,ios::beg);
    char name[80]="";
    int tmp=-1;
    ofs.write((char*)name,sizeof(name));
    ofs.write((char*)name,sizeof(name));
    for(int i=0; i<NB_ROWS; i++)
    {
        for(int j=0; j<NB_COLS; j++)
            ofs.write((char*)&tmp, sizeof(int));
    }
    ofs.write((char*)&tmp, sizeof(int));
    ofs.close();
    deleteSavedScore(n);

}

void playAI(game &g)
{
    updateScore(0);
    int turn,x,y,saved=0,giveUp=0;
    char key,ans;
    cell selectedCell= {-1,-1,0};
    cell destCell=selectedCell;
    initialize(g,turn,1);
    displayInfo("Select(R Click). Move (L Click). Save Game(s). Quit(ESC).");
    do
    {
        if(kbhit())
        {
            key=getch();
            switch (key)
            {
            case ESC:
                displayInfo ("Are you sure you want to end the game?(Y/y) to end any other key to return.");
                do
                {
                    if(kbhit())
                    {
                        ans=getch();
                        if(ans=='y' || ans=='Y')
                        {
                            displayInfo ("Come Again ... ");
                            delay(1000);
                            restorecrtmode();
                            exit(1);
                        }
                        else
                            break;
                    }
                }
                while(1);
                displayInfo("Select(R Click). Move (L Click). Save Game(s). Quit(ESC).");
                break;
            /*case NEW_GAME:
                initialize(g,turn,1);
                break;*/
            case SAVE_GAME:
                saveGame(g,turn);
                updateScore(-1);
                saved=1;
                break;
            }

        }

        if(ismouseclick(WM_RBUTTONDOWN))
        {
            getmouseclick(WM_RBUTTONDOWN, x,y);
            selectedCell=getCell(x,y,g.cells);
            clearmouseclick(WM_RBUTTONDOWN);
        }
        else if(ismouseclick(WM_LBUTTONDOWN))
        {
            getmouseclick(WM_LBUTTONDOWN, x,y);
            destCell=getCell(x,y,g.cells);
            if(selectedCell.type != 0)
            {
                moveQuarki(selectedCell, destCell,g,turn);
                displayInfo("Select(R Click). Move (L Click). Save Game(s). Quit(ESC).");
                selectedCell.row =selectedCell.col=-1;
                selectedCell.type=0;
            }
            clearmouseclick(WM_LBUTTONDOWN);
        }
        if(turn ==2)
        {
            delay(300);
            AIturn(g,giveUp,turn);
        }
    }
    while(!checkGameOver(g) && !saved && !giveUp);
    gameOverpopup(g.player1,g.player2, saved);
}

void AIturn(game &g, int &giveUp, int &turn)
{
    ai n[12],m[12];
    int cnt=cntQuarki(g),p=0,q=0;
    if(checkEatQuarki(g,n,p))
    {
        findEatQuarki(g,n,p,turn);
    }
    else if(checkAbleMove(g,m,q))
    {
        moveAbleQuarki(g,m,q,turn);
    }
    else
    {
        giveUp=1;
    }
}

int cntQuarki(game g)
{
    int cnt=0;
    for(int i=0; i<NB_ROWS; i++)
    {
        for(int j=0; j<NB_COLS; j++)
        {
            if(g.cells[i][j]==3 || g.cells[i][j]==4)
                cnt++;
        }
    }
    return cnt;
}

int checkEatQuarki(game g, ai n[12], int &p)
{
    int found=0;
    cell scell,dcell;
    p=0;
    for(int i=0; i<NB_ROWS; i++)
    {
        for(int j=0; j<NB_COLS; j++)
        {
            scell.row=i;
            scell.col=j;
            scell.type=g.cells[i][j];
            if(scell.type==3)
            {
                dcell.row=i-2;
                dcell.col=j-2;
                dcell.type=g.cells[i-2][j-2];
                if(validmove(scell,dcell,g,1) && validcell(dcell) )
                {
                    found=1;
                    n[p].c=scell;
                    n[p].direction=1;
                    p++;
                }
                dcell.row=i-2;
                dcell.col=j+2;
                dcell.type=g.cells[i-2][j+2];
                if(validmove(scell,dcell,g,1)&& validcell(dcell) )
                {
                    found=1;
                    n[p].c=scell;
                    n[p].direction=2;
                    p++;
                }
            }
            else if(scell.type==4)
            {
                dcell.row=i-2;
                dcell.col=j-2;
                dcell.type=g.cells[i-2][j-2];
                if(validmove(scell,dcell,g,1)&& validcell(dcell) )
                {
                    found=1;
                    n[p].c=scell;
                    n[p].direction=1;
                    p++;
                }
                dcell.row=i-2;
                dcell.col=j+2;
                dcell.type=g.cells[i-2][j+2];
                if(validmove(scell,dcell,g,1)&& validcell(dcell) )
                {
                    found=1;
                    n[p].c=scell;
                    n[p].direction=2;
                    p++;
                }
                dcell.row=i+2;
                dcell.col=j-2;
                dcell.type=g.cells[i+2][j-2];
                if(validmove(scell,dcell,g,1)&& validcell(dcell) )
                {
                    found=1;
                    n[p].c=scell;
                    n[p].direction=3;
                    p++;
                }
                dcell.row=i+2;
                dcell.col=j+2;
                dcell.type=g.cells[i+2][j+2];
                if(validmove(scell,dcell,g,1)&& validcell(dcell) )
                {
                    found=1;
                    n[p].c=scell;
                    n[p].direction=4;
                    p++;
                }


            }
        }
    }
    return found;
}

void findEatQuarki(game &g,ai n[],int p, int &turn)
{
    int k=rand()%p;
    cell scell=n[k].c,dcell;
    int i=scell.row,j=scell.col;
    if(n[k].c.type==3)
    {
        if(n[k].direction==1)
        {
            dcell.row=i-2;
            dcell.col=j-2;
            dcell.type=g.cells[i-2][j-2];
        }
        else
        {
            dcell.row=i-2;
            dcell.col=j+2;
            dcell.type=g.cells[i-2][j+2];
        }
    }
    else if(n[k].c.type==4)
    {
        if(n[k].direction==1)
        {
            dcell.row=i-2;
            dcell.col=j-2;
            dcell.type=g.cells[i-2][j-2];
        }
        else if(n[k].direction==2)
        {
            dcell.row=i-2;
            dcell.col=j+2;
            dcell.type=g.cells[i-2][j+2];
        }
        else if(n[k].direction==3)
        {
            dcell.row=i+2;
            dcell.col=j-2;
            dcell.type=g.cells[i+2][j-2];
        }
        else
        {
            dcell.row=i+2;
            dcell.col=j+2;
            dcell.type=g.cells[i+2][j+2];
        }
    }
    moveQuarki(scell,dcell,g,turn);
}

int checkAbleMove(game g, ai m[],int &q)
{
    int found=0;
    cell scell,dcell;
    q=0;
    for(int i=0; i<NB_ROWS; i++)
    {
        for(int j=0; j<NB_COLS; j++)
        {
            scell.row=i;
            scell.col=j;
            scell.type=g.cells[i][j];
            if(scell.type==3)
            {
                dcell.row=i-1;
                dcell.col=j-1;
                dcell.type=g.cells[i-1][j-1];
                if(validmove(scell,dcell,g,1)&& validcell(dcell) )
                {
                    found=1;
                    m[q].c=scell;
                    m[q].direction=1;
                    q++;
                }
                dcell.row=i-1;
                dcell.col=j+1;
                dcell.type=g.cells[i-1][j+1];
                if(validmove(scell,dcell,g,1)&& validcell(dcell) )
                {
                    found=1;
                    m[q].c=scell;
                    m[q].direction=2;
                    q++;
                }
            }
            else if(scell.type==4)
            {
                dcell.row=i-1;
                dcell.col=j-1;
                dcell.type=g.cells[i-1][j-1];
                if(validmove(scell,dcell,g,1)&& validcell(dcell) )
                {
                    found=1;
                    m[q].c=scell;
                    m[q].direction=1;
                    q++;
                }
                dcell.row=i-1;
                dcell.col=j+1;
                dcell.type=g.cells[i-1][j+1];
                if(validmove(scell,dcell,g,1)&& validcell(dcell) )
                {
                    found=1;
                    m[q].c=scell;
                    m[q].direction=2;
                    q++;
                }
                dcell.row=i+1;
                dcell.col=j-1;
                dcell.type=g.cells[i+1][j-1];
                if(validmove(scell,dcell,g,1)&& validcell(dcell) )
                {
                    found=1;
                    m[q].c=scell;
                    m[q].direction=3;
                    q++;
                }
                dcell.row=i+1;
                dcell.col=j+1;
                dcell.type=g.cells[i+1][j+1];
                if(validmove(scell,dcell,g,1)&& validcell(dcell) )
                {
                    found=1;
                    m[q].c=scell;
                    m[q].direction=4;
                    q++;
                }


            }
        }
    }
    return found;
}

void moveAbleQuarki(game &g,ai m[], int q, int &turn)
{
    int k=rand()%q;
    cell scell=m[k].c,dcell;
    int i=scell.row,j=scell.col;
    if(scell.type==3)
    {
        if(m[k].direction==1)
        {
            dcell.row=i-1;
            dcell.col=j-1;
            dcell.type=g.cells[i-1][j-1];
        }
        else if(m[k].direction==2)
        {
            dcell.row=i-1;
            dcell.col=j+1;
            dcell.type=g.cells[i-1][j+1];
        }
    }
    else
    {
        if(m[k].direction==1)
        {
            dcell.row=i-1;
            dcell.col=j-1;
            dcell.type=g.cells[i-1][j-1];
        }
        else if(m[k].direction==2)
        {
            dcell.row=i-1;
            dcell.col=j+1;
            dcell.type=g.cells[i-1][j+1];
        }
        else if(m[k].direction==3)
        {
            dcell.row=i+1;
            dcell.col=j-1;
            dcell.type=g.cells[i+1][j-1];
        }
        else
        {
            dcell.row=i+1;
            dcell.col=j+1;
            dcell.type=g.cells[i+1][j+1];
        }
    }
    moveQuarki(scell,dcell,g,turn);
}
int main()
{
    srand(time(NULL));
    char key,ans;
    int pl;
    game g= {{0}, "", ""};
    char msg[200];
    cout<<"Press 1 for single player, 2 for double player > ";
    cin>>pl;
    cin.ignore();
    getPlayer(1, g.player1);
    if(pl==2)
    {
        getPlayer(2, g.player2);
    }
    else
    {
        strcpy(g.player2,"COMPUTER");
    }
    initializeDriver();
    //newGame(g);

    displayInfo("Select(R Click). Move (L Click). New Game(n). Saved Game (l). Quit(ESC).");
    do
    {
        drawBoard();
        displayName1(g.player1);
        displayName2(g.player2);
        key=getch();
        switch (key)
        {
        case ESC:
            displayInfo ("Are you sure you want to end the game?(Y/y) to end any other key to return.");
            do
            {
                if(kbhit())
                {
                    ans=getch();
                    if(ans=='y' || ans=='Y')
                    {
                        displayInfo ("Come Again ... ");
                        delay(1000);
                        restorecrtmode();
                        return 0;
                    }
                    else
                        break;
                }
            }
            while(1);
            displayInfo("Select(R Click). Move (L Click). New Game(n). Saved Game(l). Quit(ESC).");
            break;
        case NEW_GAME:
            if(strcmp(g.player2,"COMPUTER")==0)
                playAI(g);
            else
                newGame(g);
            break;
        case SAVED_GAME:
            savedGame(g);
            break;


        }


    }
    while(1);

    return 0;
}
