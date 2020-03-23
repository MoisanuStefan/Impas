#include <iostream>
#include <winbgim.h>
#include <stdlib.h>
#include <graphics.h>
#include <stdio.h>
#include <windows.h>
#include <math.h>
#include <string.h>

#define NMAX 13
#define NMAX1 37

using namespace std;

struct
{
    char text[10]; // the actual string that is displayed on screen
    int lenght, value; // the length IN PIXELS, not characters of those strings and their value (color codes, dimensions, directions)
} colors[7],directions[2], gridSizes[5], themes[2];

struct pozitie
{
    int row,column;/*coordonatele unei piese pe tabla de joc, in matricea asociata*/
};

void drawBackButton();
void displayMenu();
void drawDropMenuOnClick();
void deleteDropMenuOnClick();
void drawDisableSoundSettingCircle(int, int);
void playSoundtrack(int);
void drawArrowHeadUp(int, int, int, int);
void drawArrowHeadToRight(int, int,int, int);
void drawArrowHeadToLeft(int, int, int, int);
void displayEndTextPvP();
void displayEndTextPvCPU();
void drawResetScoreButton();
void updateScoreBoardPvP(int );
void updateScoreBoardPvCPU(int );

int player1Score, player2Score, CPUScore, PLAYERScore, TablaDeJoc[NMAX][NMAX], stanga, sus, width, height, latura, playerD, playerS, piesaPreced, selectedSong = 1;
int BKGColor = 0, PionColor = 1, GridColor = 4, GridBKGColor = 5, gridSize = 2, direction = 0, setDirection = direction, theme = 0, windowToEnter = 4;
char scoreChar [4], SFXDirectories [][20] = {"error.wav", "gamewon.wav", "gamelost.wav","move.wav"};

bool stillPlaying, choice, animatieRuleaza=true, iesiEcran, settingsExit, ExitGame, dropMenuIsDrawn, musicIsDisabled, SFXIsDisabled, musicDropMenuIsDrawn, gameRunning = true;

pozitie piese[NMAX1];
int nrElem;

void curataBuffer()/*C - functia elimina mutarile 'pripite' (prin mutare 'pripita' vorbim de o mutare cauzata de un click pe tabla de joc, in timpul mutarii facute de celalalt jucator)*/
{
    while(ismouseclick(WM_LBUTTONDOWN))
        clearmouseclick(WM_LBUTTONDOWN);
}

void selectareJucator()/*C - functia determina ordinea jucatorilor in modul PvP*/
{
    if(direction==0)
    {
        playerD=1;
        playerS=2;
    }
    else
    {
        playerS=1;
        playerD=2;
    }
}

int cautarePiesa(pozitie v[NMAX1],int i,int j)/*C - functia cauta dintre piese pe aceea care se afla in casuta de coordonate i,j*/
{
    int k;
    for(k=1; k<=nrElem; k++)
        if(piese[k].row==i and piese[k].column==j)
            return k;
    return -1;
}

void posibilaMutare()/*C - functia verifica daca urmatorul jucator mai poate muta*/
{
    int k,i,j,l,u;
    for(k=1; k<=nrElem; k++)
    {
        i=piese[k].row;
        j=piese[k].column;
        if(TablaDeJoc[i][j]==1)
        {
            if(direction == 1)
            {
                u=i-1;
                if(u>=1 and TablaDeJoc[u][j]!=1)
                {
                    stillPlaying=true;
                    return;
                }
            }
            else
            {
                l=j+1;
                if(l<=gridSizes[gridSize].value and TablaDeJoc[i][l]!=1)
                {
                    stillPlaying=true;
                    return;
                }
            }
        }
    }
}

void drawPlayingDirectionArrow (int xUp, int yUp, int xRight, int yRight,int color, int direction) // S - the arrow that shows the playing direction under the board
{
    setlinestyle(SOLID_LINE, 1, 1);
    setcolor(color);
    setfillstyle(SOLID_FILL, color);
    if (direction == 0) // to the right
    {
        rectangle(xRight, yRight, xRight + 60, yRight + 30);
        floodfill(xRight + 1, yRight + 1,color);
        drawArrowHeadToRight(xRight + 60, yRight - 10,color, 50);
    }
    else // up
    {
        rectangle(xUp, yUp, xUp + 30, yUp - 60);
        floodfill(xUp + 1, yUp - 1,color);
        drawArrowHeadUp(xUp - 10, yUp - 60,color, 50);
    }
}

void playingArrowVibrationAnimation (int xUp, int yUp, int xRight, int yRight, int direction) // S - the function makes the playing direction arrow vibrate when user make a wrong move
{

    int x,y;
    setlinestyle(SOLID_LINE, 1, 1);
    if (direction == 0) // to the right
    {
        x = xRight;
        xRight += 10;
        while (x <= xRight ) // shifts to the right by 10 pixels
        {
            drawPlayingDirectionArrow(285,743,x,680,colors[BKGColor].value, direction);
            x++;
            drawPlayingDirectionArrow(285,743,x,680,colors[PionColor].value, direction);
            delay(10);
        }
        drawPlayingDirectionArrow(285,743,x,680,colors[BKGColor].value, direction);
        x--;
        xRight -= 20;
        while (x >= xRight ) // shifts back to left by 20 pixels
        {
            drawPlayingDirectionArrow(285,743,x,680,colors[BKGColor].value, direction);
            x--;
            drawPlayingDirectionArrow(285,743,x,680,colors[PionColor].value, direction);
            delay(10);
        }
        drawPlayingDirectionArrow(285,743,x,680,colors[BKGColor].value, direction);
        x++;
        xRight += 10;
        while (x <= xRight ) // shifts back to initial position, 10 pixels to right
        {
            drawPlayingDirectionArrow(285,743,x,680,colors[BKGColor].value, direction);
            x++;
            drawPlayingDirectionArrow(285,743,x,680,colors[PionColor].value, direction);
            delay(10);
        }
        drawPlayingDirectionArrow(285,743,x,680,colors[BKGColor].value, direction);
        x--;
        drawPlayingDirectionArrow(285,743,x,680,colors[PionColor].value, direction);
    }
    else // up, same shifting movement as above, on vertical direction
    {
        y= yUp;
        yUp -= 10;
        while (y >=yUp )
        {
            drawPlayingDirectionArrow(285,y,242,680,colors[BKGColor].value, direction);
            y--;
            drawPlayingDirectionArrow(285,y,242,680,colors[PionColor].value, direction);
            delay(10);
        }
        drawPlayingDirectionArrow(285,y,242,680,colors[BKGColor].value, direction);
        y++;
        yUp += 20;
        while (y <=yUp )
        {
            drawPlayingDirectionArrow(285,y,242,680,colors[BKGColor].value, direction);
            y++;
            drawPlayingDirectionArrow(285,y,242,680,colors[PionColor].value, direction);
            delay(10);
        }
        drawPlayingDirectionArrow(285,y,242,680,colors[BKGColor].value, direction);
        y--;
        yUp -= 10;
        while (y >=yUp )
        {
            drawPlayingDirectionArrow(285,y,242,680,colors[BKGColor].value, direction);
            y--;
            drawPlayingDirectionArrow(285,y,242,680,colors[PionColor].value, direction);
            delay(10);
        }
        drawPlayingDirectionArrow(285,y,242,680,colors[BKGColor].value, direction);
        y++;
        drawPlayingDirectionArrow(285,y,242,680,colors[PionColor].value, direction);
    }
}

void deplasareDreapta(int i, int j, int startPoint)/*C - functia realizeaza "glisarea" unei piese la dreapta spre casuta unde trebuie mutata, schimband totodata si starea matricei asociate tablei de joc*/
{
    playSoundtrack(7);
    TablaDeJoc[i][j]=1;
    int pornire,oprire,intermediar;
    pornire=stanga+latura*(startPoint-1)+latura/2;
    intermediar=pornire+latura/2;
    oprire=stanga+latura*(j-1)+latura/2;
    for(; pornire<=oprire-1; pornire++)
    {
        setcolor(colors[PionColor].value);
        setlinestyle(SOLID_LINE,1,2);
        setfillstyle(SOLID_FILL,colors[PionColor].value);
        fillellipse(pornire,sus+latura*(i-1)+latura/2,latura/2-5,latura/2-5);
        delay(2);
        setcolor(colors[GridBKGColor].value);
        setfillstyle(SOLID_FILL,colors[GridBKGColor].value);
        fillellipse(pornire,sus+latura*(i-1)+latura/2,latura/2-4,latura/2-4);
        if(pornire==intermediar)
        {
            setcolor(colors[GridColor].value);
            setlinestyle(SOLID_LINE,1,1);
            rectangle(pornire-latura,sus+latura*(i-1),pornire,sus+latura*i);
            intermediar=intermediar+latura;
        }
    }
    setcolor(colors[PionColor].value);
    setlinestyle(SOLID_LINE,1,2);
    setfillstyle(SOLID_FILL,colors[PionColor].value);
    fillellipse(pornire,sus+latura*(i-1)+latura/2,latura/2-5,latura/2-5);
    setcolor(colors[GridColor].value);
    setlinestyle(SOLID_LINE,1,1);
    rectangle(pornire-latura/2,sus+latura*(i-1),pornire+latura/2,sus+latura*i);
    TablaDeJoc[i][startPoint]=0;
}

void deplasareSus(int i, int j, int startPoint)/*C - functia realizeaza "glisarea" unei piese in sus spre casuta unde trebuie mutata, schimband totodata si starea matricei asociate tablei de joc*/
{
    playSoundtrack(7);
    TablaDeJoc[i][j]=1;
    int pornire,oprire,intermediar;
    pornire=sus+latura*(startPoint-1)+latura/2;
    intermediar=pornire-latura/2;
    oprire=sus+latura*(i-1)+latura/2;
    for(; pornire>=oprire+1; pornire--)
    {
        setcolor(colors[PionColor].value);
        setlinestyle(SOLID_LINE,1,2);
        setfillstyle(SOLID_FILL,colors[PionColor].value);
        fillellipse(stanga+latura*(j-1)+latura/2,pornire,latura/2-5,latura/2-5);
        delay(2);
        setcolor(colors[GridBKGColor].value);
        setfillstyle(SOLID_FILL,colors[GridBKGColor].value);
        fillellipse(stanga+latura*(j-1)+latura/2,pornire,latura/2-4,latura/2-4);
        if(pornire==intermediar)
        {
            setcolor(colors[GridColor].value);
            setlinestyle(SOLID_LINE,1,1);
            rectangle(stanga+latura*(j-1),pornire,stanga+latura*j,pornire+latura);
            intermediar=intermediar-latura;
        }
    }
    setcolor(colors[PionColor].value);
    setlinestyle(SOLID_LINE,1,2);
    setfillstyle(SOLID_FILL,colors[PionColor].value);
    fillellipse(stanga+latura*(j-1)+latura/2,pornire,latura/2-5,latura/2-5);
    setcolor(colors[GridColor].value);
    setlinestyle(SOLID_LINE,1,1);
    rectangle(stanga+latura*(j-1),pornire-latura/2,stanga+latura*j,pornire+latura/2);
    TablaDeJoc[startPoint][j]=0;
}

void punerePiesa()/*C+S - functia realizeaza mutarea unei piese pentru jucatorul curent, jucatorul fiind dat prin directia pe care acesta muta piesa (0 pentru dreapta si 1 pentru sus)*/
{
    int linia,coloana,x,y;
    int l,u,val;
    drawPlayingDirectionArrow(285,743,242,680,colors[PionColor].value, direction);
    switch(direction)
    {
    case 0: // right
        while(1)
        {
            if(ismouseclick(WM_LBUTTONDOWN))
            {
                clearmouseclick(WM_LBUTTONDOWN);
                x=mousex();
                y=mousey();
                if ( x <= 30 && x >= 8 && y > 30 + 0.3 * (30 - x) && y < 50 - 0.3 * (30 - x)) // back button
                {
                    animatieRuleaza  = true;
                    iesiEcran = false;
                    ExitGame = true;
                    windowToEnter = 4;
                    setlinestyle(SOLID_LINE,1,1);
                    return;
                }
                if ( x <= 550 && x >= 450 && y > 155 && y < 180) // reset button
                {
                    player1Score = 0;
                    player2Score = 0;
                    updateScoreBoardPvP(colors[BKGColor].value);
                    updateScoreBoardPvP(colors[PionColor].value);
                }
                if(x>=stanga && x<=stanga+width && y>=sus&&y<=sus+height)
                {
                    linia=(y-sus)/latura+1;
                    coloana=(x-stanga)/latura+1;
                    if (TablaDeJoc[linia][coloana]==0)
                    {
                        l=coloana-1;
                        while(l>=1 and TablaDeJoc[linia][l]!=1)
                            l--;
                        if(l>=1)
                        {
                            deplasareDreapta(linia,coloana,l);
                            val=cautarePiesa(piese,linia,l);
                            piese[val].column=coloana;
                            drawPlayingDirectionArrow(285,743,242,680,colors[BKGColor].value, direction); // delete the arrow because it will be drawn in the other direction
                            return;
                        }
                        else
                        {
                            playSoundtrack(4); // error sound
                            playingArrowVibrationAnimation(285,743,242,680, direction);
                        }

                    }
                    else
                    {
                        playSoundtrack(4); // error sound
                        playingArrowVibrationAnimation(285,743,242,680, direction);
                    }

                }
            }
        }
    case 1: // up
        while(1)
        {
            if(ismouseclick(WM_LBUTTONDOWN))
            {
                clearmouseclick(WM_LBUTTONDOWN);
                x=mousex();
                y=mousey();
                if ( x <= 30 && x >= 8 && y > 30 + 0.3 * (30 - x) && y < 50 - 0.3 * (30 - x))//back button
                {

                    animatieRuleaza  = true;
                    iesiEcran = false;
                    ExitGame = true;
                    windowToEnter = 4;
                    setlinestyle(SOLID_LINE,1,1);
                    return;
                }
                if ( x <= 550 && x >= 450 && y > 155 && y < 180) // reset button
                {
                    player1Score = 0;
                    player2Score = 0;
                    updateScoreBoardPvP(colors[BKGColor].value);
                    updateScoreBoardPvP(colors[PionColor].value);
                }
                if(x>=stanga && x<=stanga+width && y>=sus&&y<=sus+height)
                {
                    linia=(y-sus)/latura+1;
                    coloana=(x-stanga)/latura+1;
                    if (TablaDeJoc[linia][coloana]==0)
                    {
                        u=linia+1;
                        while(u<=gridSizes[gridSize].value and TablaDeJoc[u][coloana]!=1)
                            u++;
                        if(u<=gridSizes[gridSize].value)
                        {
                            deplasareSus(linia,coloana,u);
                            val=cautarePiesa(piese,u,coloana);
                            piese[val].row=linia;
                            drawPlayingDirectionArrow(285,743,242,680,colors[BKGColor].value, direction); // delete the arrow because it will be drawn in the other direction
                            return;
                        }
                        else
                        {
                            playSoundtrack(4); // error sound
                            playingArrowVibrationAnimation(285,743,242,680, direction);
                        }

                    }
                    else
                    {
                        playSoundtrack(4); // error sound
                        playingArrowVibrationAnimation(285,743,242,680, direction);
                    }
                }
            }
        }
    }
}

void convertIntToChar (int numberToConvert) // S - given an int (the score for one player) the function converts it to a string and places it in scoreChar in order to use outtextxy() to output it
{
    char auxChar[2];
    int mirror = 0;
    while (numberToConvert)
    {
        mirror = mirror * 10 + numberToConvert % 10;
        numberToConvert /= 10;
    }
    scoreChar[0]='\0';
    scoreChar[1]='\0';
    scoreChar[2]='\0';
    auxChar[1] = NULL;
    do
    {
        auxChar[0] = mirror % 10 + 48;
        strcat(scoreChar, auxChar);
        mirror /= 10;
    }
    while (mirror);
}

void updateScoreBoardPvP(int color) // S - draws the current score
{
    settextstyle(3,HORIZ_DIR, 4);
    setcolor(color);
    convertIntToChar(player1Score);
    outtextxy(187, 150, scoreChar);
    convertIntToChar(player2Score);
    outtextxy(387, 150, scoreChar);
}

void updateScoreBoardPvCPU(int color)/*C (pe baza design-ului de la updateScoreBoardPvP)*/
{
    settextstyle(3,HORIZ_DIR, 4);
    setcolor(color);
    convertIntToChar(PLAYERScore);
    outtextxy(187, 150, scoreChar);
    convertIntToChar(CPUScore);
    outtextxy(387, 150, scoreChar);
}

void drawScoreBoardPvP () // S - draws the fixed elements of the scoreboard that never change, and also the 0 - 0 beginning score
{
    setcolor(colors[PionColor].value);
    settextstyle(3,HORIZ_DIR, 4);
    setfillstyle(1, colors[PionColor].value);
    outtextxy(130, 100, "Player 1");
    outtextxy(330, 100, "Player 2");
    rectangle(290, 160, 310,170);
    floodfill(291, 161, colors[PionColor].value);
    drawArrowHeadToLeft(290,160, colors[PionColor].value, 10);
    drawArrowHeadToRight(310, 160, colors[PionColor].value, 10);
    updateScoreBoardPvP(colors[PionColor].value);
}

void drawScoreBoardPvCPU()/*C (pe baza design-ului de la drawScoreBoardPvP)*/
{
    setcolor(colors[PionColor].value);
    settextstyle(3,HORIZ_DIR, 4);
    setfillstyle(1, colors[PionColor].value);
    outtextxy(140, 100, "Player");
    outtextxy(360, 100, "CPU");
    rectangle(290, 160, 310,170);
    floodfill(291, 161, colors[PionColor].value);
    drawArrowHeadToLeft(290,160, colors[PionColor].value, 10);
    drawArrowHeadToRight(310, 160, colors[PionColor].value, 10);
    updateScoreBoardPvCPU(colors[PionColor].value);
}

void desen()/*C+S - functia creeaza tabla de joc si plaseaza piesele in zona de inceput (patratul cu latura egala cu jumatate din dimensiunea tablei de joc din coltul stanga-jos)*/
{
    int i,j;
    width=400;
    height=400;
    nrElem=0;
    latura=width/gridSizes[gridSize].value;
    sus=(getmaxy()-width)/2;
    stanga=(getmaxx()-height)/2;
    setbkcolor(colors[BKGColor].value);
    clearviewport();
    drawBackButton();
    if(choice==0)
        drawScoreBoardPvP();
    else
        drawScoreBoardPvCPU();
    drawResetScoreButton();
    setcolor(colors[GridColor].value);
    setlinestyle(SOLID_LINE, 1, 3);
    rectangle(stanga, sus, stanga + latura * gridSizes[gridSize].value, sus + latura * gridSizes[gridSize].value);/*desenarea tablei de joc*/
    setfillstyle(SOLID_FILL, colors[GridBKGColor].value);
    floodfill(stanga + 4, sus + 4, colors[GridColor].value);
    setlinestyle(SOLID_LINE, 1, 1);
    for(i=1; i<=gridSizes[gridSize].value; i++)/*desenarea casutelor pe tabla de joc*/
        for(j=1; j<=gridSizes[gridSize].value; j++)
            rectangle(stanga+latura*(i-1),sus+latura*(j-1),stanga+latura*i,sus+latura*j);
    for(i=gridSizes[gridSize].value/2+1; i<=gridSizes[gridSize].value; i++)/*"plasarea" pieselor de joc*/
        for(j=1; j<=gridSizes[gridSize].value/2; j++)
        {
            TablaDeJoc[i][j]=1;
            setcolor(colors[PionColor].value);
            setlinestyle(SOLID_LINE,1,2);
            setfillstyle(SOLID_FILL,colors[PionColor].value);
            fillellipse(stanga+latura*(j-1)+latura/2,sus+latura*(i-1)+latura/2,latura/2-5,latura/2-5);
            nrElem++;
            piese[nrElem].row=i;
            piese[nrElem].column=j;
        }
}

void resetGame()//S+C
{
    int i,j;
    for (i=1;i<=gridSizes[gridSize].value;i++)
        for (j=1;j<=gridSizes[gridSize].value;j++)
            TablaDeJoc[i][j] = 0;
    for (i=1;i<=nrElem;i++)
    {
        piese[i].row=0;
        piese[i].column=0;
    }
    ExitGame = false;
    direction = setDirection; // setDirection remembers the playing direction set in settings
}

void drawRevengeButton () // S - at the end of a game this button starts a new one
{
    setcolor(colors[PionColor].value);
    setfillstyle(SOLID_FILL, colors[PionColor].value);
    rectangle(242,680, 342, 720);
    floodfill(243, 681, colors[PionColor].value);
    setbkcolor(colors[PionColor].value);
    setcolor(colors[GridColor].value);
    settextstyle(3, HORIZ_DIR, 2);
    outtextxy(250, 690, "Revenge?");
    setbkcolor(colors[BKGColor].value);
}

void drawResetScoreButton () // S - resets the score to 0 -0 at any time during a game
{
    setcolor(colors[PionColor].value);
    setfillstyle(SOLID_FILL, colors[PionColor].value);
    rectangle(450,155, 550, 180);
    floodfill(451, 156, colors[PionColor].value);
    setbkcolor(colors[PionColor].value);
    setcolor(colors[GridColor].value);
    settextstyle(3, HORIZ_DIR, 1);
    outtextxy(455, 158, "Reset Score");
    setbkcolor(colors[BKGColor].value);
}

void displayEndTextPvP () // S - the flashing text that says who won at the end of a game
{
    int x, y;
    drawRevengeButton();
    char endGameMessage[50]="Congratulations! Player number ";
    char jucator[2];
    if(direction == setDirection)
    {
        itoa(playerS,jucator,10);
    }
    else
        itoa(playerD,jucator,10);
    strcat(endGameMessage,jucator);
    strcat(endGameMessage," has won!");
    while(1)
    {
        if (!ismouseclick(WM_LBUTTONDOWN)) // while there is no click on either of the buttons, colors of the text change, resulting in a flashing message on the screen
        {
            setcolor(colors[PionColor].value);
            outtextxy(115,30, endGameMessage);
            setcolor(colors[GridBKGColor].value);
            outtextxy(115,30, endGameMessage);
        }
        else
        {
            clearmouseclick(WM_LBUTTONDOWN);
            x = mousex();
            y = mousey();
            if ( x <= 30 && x >= 8 && y > 30 + 0.3 * (30 - x) && y < 50 - 0.3 * (30 - x)) // back button
            {
                player1Score = 0;
                player2Score = 0;
                playSoundtrack(selectedSong);
                animatieRuleaza  = true;
                iesiEcran = false;
                ExitGame = true;
                windowToEnter = 4;
                setlinestyle(SOLID_LINE,1,1);
                return;
            }
            if ( x <= 342 && x >= 242 && y > 680&& y < 720 ) // revenge button
            {
                return;
            }
            if ( x <= 550 && x >= 450 && y > 155 && y < 180) // reset button
            {
                player1Score = 0;
                player2Score = 0;
                updateScoreBoardPvP(colors[BKGColor].value);
                updateScoreBoardPvP(colors[PionColor].value);
                return;
            }
        }
    }
}

void displayEndTextPvCPU()/*C (pe baza design-ului de la displayEndTextPvP)*/
{
    int x, y;
    drawRevengeButton();
    char endGameMessage[50];
    if(direction == setDirection)
    {
        strcpy (endGameMessage,"You lost. The computer won");
    }
    else
        strcpy(endGameMessage, "Congratulations! You won");
    while(1)
    {
        if (!ismouseclick(WM_LBUTTONDOWN))
        {
            setcolor(colors[PionColor].value);
            outtextxy(180,30, endGameMessage);
            setcolor(colors[GridBKGColor].value);
            outtextxy(180,30, endGameMessage);
        }
        else
        {
            clearmouseclick(WM_LBUTTONDOWN);
            x = mousex();
            y = mousey();
            if ( x <= 30 && x >= 8 && y > 30 + 0.3 * (30 - x) && y < 50 - 0.3 * (30 - x)) // back button
            {
                PLAYERScore = 0;
                CPUScore = 0;
                playSoundtrack(selectedSong);
                animatieRuleaza  = true;
                iesiEcran = false;
                ExitGame = true;
                windowToEnter = 4;
                setlinestyle(SOLID_LINE,1,1);
                return;
            }
            if ( x <= 342 && x >= 242 && y > 680&& y < 720 ) // revenge button
            {
                return;
            }
            if ( x <= 550 && x >= 450 && y > 155 && y < 180) // reset button
            {
                PLAYERScore = 0;
                CPUScore = 0;
                updateScoreBoardPvCPU(colors[BKGColor].value);
                updateScoreBoardPvCPU(colors[PionColor].value);
                return;
            }
        }
    }
}

void endGamePvP()  //S - updates the score and says who won
{
    playSoundtrack(5); // cheerful sound
    if ( direction == setDirection )
        player2Score ++;
    else
        player1Score ++;
    updateScoreBoardPvP(colors[BKGColor].value);
    updateScoreBoardPvP(colors[PionColor].value);
    displayEndTextPvP();
    resetGame();
}

void endGamePvCPU()/*C (pe baza design-ului de la endGamePvP)*/
{

    if(direction == setDirection)
    {
        CPUScore ++;
        playSoundtrack(6); // "game lost" type of sound
    }
    else
    {
        PLAYERScore ++;
        playSoundtrack(5);
    }
    updateScoreBoardPvCPU(colors[BKGColor].value);
    updateScoreBoardPvCPU(colors[PionColor].value);
    displayEndTextPvCPU();
    resetGame();
}

void stopAnimation(int x, int y) // S - stops the menu animation and sets the next window to enter depending on where was clicked
{

    if (x > 215 && x < 370 && y < 330 && y > 315) // click on New P v P Game button
    {

        animatieRuleaza = false;
        iesiEcran = true;
        windowToEnter = 1;
        dropMenuIsDrawn = false;

    }
    if (x > 130 && x < 315 && y > 363 && y < 382 && windowToEnter == 4) // click on New P v CPU Game, opens or closes the dropdown menu
    {

        if (!dropMenuIsDrawn)
        {
            drawDropMenuOnClick();
            dropMenuIsDrawn = true;
        }
        else
        {
            deleteDropMenuOnClick();
            dropMenuIsDrawn = false;
        }

    }
    if (x > 72 && x < 117 && y > 428 && y < 448 && dropMenuIsDrawn) // click on easy PvCPU mode
    {

        animatieRuleaza = false;
        iesiEcran = true;
        windowToEnter = 5;
        dropMenuIsDrawn = false;

    }
    if (x > 72 && x < 117 && y > 480 && y < 497 & dropMenuIsDrawn) // click on hard PvCPU mode
    {

        animatieRuleaza = false;
        iesiEcran = true;
        windowToEnter = 6;
        dropMenuIsDrawn = false;

    }
    if (x > 200 && x < 271 && y > 412 && y < 432) // click on Settings
    {

        animatieRuleaza = false;
        iesiEcran = true;
        windowToEnter = 3;
        dropMenuIsDrawn = false;

    }
    if (x > 190 && x < 223 && y > 465 && y < 482) // click on Exit
    {

        animatieRuleaza = false;
        iesiEcran = true;
        windowToEnter = 7;
        dropMenuIsDrawn = false;

    }
}

void displayMenuGrid ()  // S
{
    setcolor(colors[GridColor].value);
    line (0, 700, 470, 260); // diagonal 1
    line (50, 800, 522, 260); //diagonal 2
    line (200, 800, 571, 260); //diagonal 3
    line (350, 800, 750, 0); //diagonal 4
    line (500, 800, 750, 0); //diagonal 5
    line (0, 700, 600, 700); // horizontal 1
    line (99, 610, 800, 610); // horizontal 2
    line (182, 530, 600, 530); // horizontal 3
    line (257, 460, 600, 460); // horizontal 4
    line (322, 400, 600, 400); // horizontal 5
    line (375, 350, 600, 350); // horizontal 6
    line (418, 310, 600, 310); // horizontal 7
    line (451, 280, 600, 280); // horizontal 8
    line (470, 260, 600, 260); // top border
}

void displayTitle () // S
{
    setcolor(colors[PionColor].value);
    settextstyle(3,HORIZ_DIR, 10);
    outtextxy(140, 40, "IMPAS");
}

void displayNewPvPGameText() // S
{
    setcolor(colors[PionColor].value);
    settextstyle(3,HORIZ_DIR, 3);
    outtextxy(260, 310, "P v P Game");
}

void displayNewPvCPUGameText() // S
{
    setcolor(colors[PionColor].value);
    settextstyle(3,HORIZ_DIR, 3);
    outtextxy(185, 360, "P v CPU Game");
}

void displaySettingsText() // S
{
    setcolor(colors[PionColor].value);
    settextstyle(3,HORIZ_DIR, 3);
    outtextxy(200, 410, "Settings");
}


void displayExitText () // S
{
    setcolor(colors[PionColor].value);
    settextstyle(3,HORIZ_DIR, 3);
    outtextxy(190, 460, "Exit");
}
void displayMenu () // S - gets the animation going until a click on one of the options
{
    displayMenuGrid();
    displayTitle();
    displayNewPvPGameText();
    displayNewPvCPUGameText();
    displaySettingsText();
    displayExitText();
    while (1)
    {
        if (animatieRuleaza)
        {
            int xStart1 = 237, yStart1 = 656, xStart2 = 269, yStart2 = 496;
            float j,xradius1 = 40, yradius1 = 30, xradius2 = 30, yradius2 = 20,i;

            setfillstyle(SOLID_FILL, colors[PionColor].value);//colors of pion
            setcolor(colors[PionColor].value);
            fillellipse(xStart2, yStart2, xradius2, yradius2);
            for (i = 0, j = 0; animatieRuleaza && xStart1 + i <= 491; i++,j = j + 1.27)
            {
                setfillstyle(SOLID_FILL, colors[PionColor].value);//colors of pion
                setcolor(colors[PionColor].value);
                fillellipse(xStart1 + i, yStart1 - j, xradius1, yradius1);
                delay(5);// for() would move way too fast without a delay
                setfillstyle(SOLID_FILL, colors[BKGColor].value);//reset to BKG color
                setcolor(colors[BKGColor].value);
                fillellipse(xStart1 + i, yStart1 - j, xradius1, yradius1);// now I delete the drawn ellipse to simulate movement
                if (yStart1 - j <= 585 && yStart1 - j >= 583) // these if s are needed to redraw grid lines after the pions pass them
                {
                    setcolor(colors[GridColor].value);
                    line (99, 610, 800, 610); // horizontal 2
                }
                if (yStart1 - j <= 505 && yStart1 - j >= 503)
                {
                    setcolor(colors[GridColor].value);
                    line (182, 530, 600, 530); // horizontal 3
                }
                if (yStart1 - j <= 440 && yStart1 - j >= 438)
                {
                    setcolor(colors[GridColor].value);
                    line (257, 460, 600, 460); // horizontal 4
                }
                if (yStart1 - j <= 383 && yStart1 - j >= 381)
                {
                    setcolor(colors[GridColor].value);
                    line (322, 400, 600, 400); // horizontal 5
                }
                if (yStart1 - j <= 335 && yStart1 - j >= 333)
                {
                    setcolor(colors[GridColor].value);
                    line (375, 350, 600, 350); // horizontal 6
                }
                xradius1 -= 0.06;
                yradius1 -= 0.07;
            }

            if (!animatieRuleaza) // needed to be able to access other windows in mid-animation
            {
                continue;
            }

            setfillstyle(SOLID_FILL, colors[PionColor].value);
            setcolor(colors[PionColor].value);
            fillellipse(xStart1 + i, yStart1 - j, xradius1, yradius1); // make it stay in final position
            xStart1 += i -1;
            yStart1 = yStart1 -j +1.27;


            // second pion movement //
            for (i = 0; xStart2 + i <= 548 && animatieRuleaza; i++)
            {
                setfillstyle(SOLID_FILL, colors[PionColor].value);//colors of pion
                setcolor(colors[PionColor].value);
                fillellipse(xStart2 + i, yStart2, xradius2, yradius2);
                delay(5);// for() would move way too fast without a delay
                setfillstyle(SOLID_FILL, colors[BKGColor].value);//reset to BKG color
                setcolor(colors[BKGColor].value);
                fillellipse(xStart2 + i, yStart2, xradius2, yradius2);// now I delete the drawn elipse to simulate movement
                if (xStart2 + i >= 359 && xStart2 <= 360)
                {
                    setcolor(colors[GridColor].value);
                    line (50, 800, 522, 260); //diagonal 2
                }
                if (xStart2 + i >= 460 && xStart2 + i <= 461)
                {
                    setcolor(colors[GridColor].value);
                    line (200, 800, 571, 260); //diagonal 3
                }
                if (xStart2 + i >= 540 && xStart2 + i <= 541)
                {
                    setcolor(colors[GridColor].value);
                    line (350, 800, 750, 0); //diagonal 4
                }
                if (xStart2 + i >= 650 && xStart2 + i <= 651)
                {
                    setcolor(colors[GridColor].value);
                    line (500, 800, 750, 0); //diagonal 5
                }
            }
            setfillstyle(SOLID_FILL, colors[PionColor].value );
            setcolor(colors[PionColor].value);
            fillellipse(xStart2 + i, yStart2, xradius2, yradius2);
            xStart2 += i -1;

            if (!animatieRuleaza)
            {
                continue;
            }

            //first pion to right//
            setfillstyle(SOLID_FILL, colors[BKGColor].value);
            setcolor(colors[BKGColor].value);
            fillellipse(xStart1, yStart1, xradius1+3, yradius1+ 5);// delete the pion first because when it leaves it leaves a little white line behind
            for (i = 0; xStart1 + i <= 700 && animatieRuleaza; i++)
            {
                setfillstyle(SOLID_FILL, colors[PionColor].value);//colors of pion
                setcolor(colors[PionColor].value);
                fillellipse(xStart1 + i, yStart1, xradius1, yradius1);
                delay(5);// for() would move way too fast without a delay
                setfillstyle(SOLID_FILL, colors[BKGColor].value);//reset to BKG color
                setcolor(colors[BKGColor].value);
                fillellipse(xStart1 + i, yStart1, xradius1, yradius1);// now I delete the drawn elipse to simulate movement
                if (xStart1 + i >= 550 && xStart1 <= 551)
                {
                    setcolor(colors[GridColor].value);
                    line (200, 800, 571, 260); //diagonal 3
                }
                if (xStart1 + i >= 620 && xStart1 + i <= 621)
                {
                    setcolor(colors[GridColor].value);
                    line (350, 800, 750, 0); //diagonal 4
                }
            }

            if (!animatieRuleaza)
            {
                continue;
            }

            delay(500);

            //second pion movement up
            setfillstyle(SOLID_FILL, colors[BKGColor].value);
            setcolor(colors[BKGColor].value);
            fillellipse(xStart2, yStart2, xradius2+3, yradius2+3); //same bug fix
            for (i = 0, j = 0; xStart2 + i <= 700 && animatieRuleaza; i = i + 0.7,j = j + 1.27)
            {
                setfillstyle(SOLID_FILL, colors[PionColor].value);//colors of pion
                setcolor(colors[PionColor].value);
                fillellipse(xStart2 + i, yStart2 - j, xradius2, yradius2);
                delay(5);// for() would move way too fast without a delay
                setfillstyle(SOLID_FILL, colors[BKGColor].value);//reset to BKG color
                setcolor(colors[BKGColor].value);
                fillellipse(xStart2 + i, yStart2 - j, xradius2, yradius2);// now I delete the drawn ellipse to simulate movement

                if (yStart2- j <= 440 && yStart2- j >= 438)
                {
                    setcolor(colors[GridColor].value);
                    line (257, 460, 600, 460); // horizontal 4
                }
                if (yStart2- j <= 383 && yStart2- j >= 381)
                {
                    setcolor(colors[GridColor].value);
                    line (322, 400, 600, 400); // horizontal 5
                }
                if (yStart2- j <= 335 && yStart2- j >= 333)
                {
                    setcolor(colors[GridColor].value);
                    line (375, 350, 600, 350); // horizontal 6
                }
                xradius1 -= 0.06;
                yradius1 -= 0.07;

            }
            if (!animatieRuleaza)
            {
                continue;
            }

        }
        if (iesiEcran)
        {
            break;
        }
    }
}

void startPvPGame() // S
{
    playSoundtrack(0);
    setbkcolor(colors[BKGColor].value);
    cleardevice();
    clearmouseclick(WM_LBUTTONDOWN);
    desen();
    direction = setDirection;
    selectareJucator();
    stillPlaying=true;
    while(stillPlaying && !ExitGame)
    {
        stillPlaying=false;
        punerePiesa();
        if (ExitGame)// back button was pressed
        {
            player1Score = 0;
            player2Score = 0;
            resetGame();
            playSoundtrack(selectedSong);
            return;
        }
        direction = 1 - direction;
        posibilaMutare();
        curataBuffer();
    }
    endGamePvP();
}

void drawDropMenuOnClick () // S - draws the options for easy or hard game
{
    setcolor(colors[PionColor].value);
    setfillstyle(SOLID_FILL, colors[PionColor].value);
    settextstyle(3,HORIZ_DIR,2);
    fillellipse(175, 370, 4, 4);
    fillellipse(130, 440, 4, 4);
    fillellipse(130, 490, 4, 4);
    line (175, 370, 150, 370);  //firs hor line
    line(150, 370, 150, 490);   //vertical line
    line (150, 440, 130, 440);  // first hor line
    line(150, 490, 130, 490);  //second hor line
    outtextxy(75, 425, "Easy");
    outtextxy(75, 475, "Hard");
}

void deleteDropMenuOnClick () // S
{
    setcolor(colors[BKGColor].value);
    setfillstyle(SOLID_FILL, colors[BKGColor].value);
    settextstyle(3,HORIZ_DIR,2);
    fillellipse(130, 440, 4, 4);
    fillellipse(175, 370, 4, 4);
    fillellipse(130, 490, 4, 4);
    line (175, 370, 150, 370);  //first hor line
    line(150, 370, 150, 490);   //vertical line
    line (150, 440, 130, 440);  // first hor line
    line(150, 490, 130, 490);  //second hor line
    outtextxy(75, 425, "Easy");
    outtextxy(75, 475, "Hard");
}

void drawArrowHeadToRight (int x, int y, int color, int dimension = 20) // S - arrow for navigating through settings options
{
    int baseLength = dimension,
        longSide = 125 * baseLength / 100,//25% bigger than the given baseLength
        halfBase = baseLength / 2,
        height = sqrt(longSide * longSide - halfBase * halfBase);

    setcolor(color);
    line (x, y, x, y + baseLength); //vertical side
    line (x, y, x + height, y + halfBase);
    line (x + height, y + halfBase, x, y + baseLength);
    setfillstyle(SOLID_FILL, color);
    floodfill(x + 1, y + halfBase, color);
}

void drawArrowHeadUp (int x, int y, int color, int dimension = 20) // S - for the playing direction arrow
{
    int baseLength = dimension,
        longSide = 125 * baseLength / 100,//25% bigger than the given baseLength
        halfBase = baseLength / 2,
        height = sqrt(longSide * longSide - halfBase * halfBase);

    setcolor(color);
    line (x, y, x + baseLength, y); //horizontal side
    line (x + baseLength, y, x + halfBase, y - height);
    line (x + halfBase, y - height, x, y);
    setfillstyle(SOLID_FILL, color);
    floodfill(x + halfBase, y - 1, color);
}

void drawArrowHeadToLeft (int x, int y, int color, int dimension = 20) // S - arrow for navigating through settings options
{
    int baseLength = dimension,
        longSide = 125 * baseLength / 100,//25% bigger than the given baseLength
        halfBase = baseLength / 2,
        height = sqrt(longSide * longSide - halfBase * halfBase);

    setcolor(color);
    line (x, y, x, y + baseLength); //veritcal side
    line (x, y, x - height, y + halfBase);
    line (x - height, y + halfBase, x, y + baseLength);
    setfillstyle(SOLID_FILL, color);
    floodfill(x - 1, y + halfBase, color);
}

void drawVariables() // S - sets the texts for each setting
{
    settextstyle(3, HORIZ_DIR, 3);
    setcolor(colors[PionColor].value);
    outtextxy(171 - gridSizes[gridSize].lenght / 2, 198, gridSizes[gridSize].text);
    outtextxy(410 - directions[setDirection].lenght / 2, 198, directions[setDirection].text);
    outtextxy(171 - colors[BKGColor].lenght / 2, 498, colors[BKGColor].text);
    setcolor(colors[GridColor].value);
    outtextxy(410 - colors[GridColor].lenght / 2, 498, colors[GridColor].text);
    setcolor(colors[GridBKGColor].value);
    outtextxy(171 - colors[GridBKGColor].lenght / 2, 698, colors[GridBKGColor].text);
    setcolor(colors[PionColor].value);
    outtextxy(410 - colors[PionColor].lenght / 2, 698, colors[PionColor].text);
    setcolor(themes[theme].value);
    outtextxy(300 - themes[theme].lenght / 2, 368, themes[theme].text);
}

void drawAllArrows() // S - navigation arrows
{
    drawArrowHeadToLeft(106, 200, colors[PionColor].value);
    drawArrowHeadToRight(236, 200, colors[PionColor].value);
    drawArrowHeadToLeft(342, 200, colors[PionColor].value);
    drawArrowHeadToRight(472, 200, colors[PionColor].value);

    drawArrowHeadToLeft(235, 370, colors[PionColor].value);
    drawArrowHeadToRight(365, 370, colors[PionColor].value);

    drawArrowHeadToLeft(106, 500, colors[PionColor].value);
    drawArrowHeadToRight(236, 500, colors[PionColor].value);
    drawArrowHeadToLeft(342, 500, colors[PionColor].value);
    drawArrowHeadToRight(472, 500, colors[PionColor].value);

    drawArrowHeadToLeft(106, 700, colors[PionColor].value);
    drawArrowHeadToRight(236, 700, colors[PionColor].value);
    drawArrowHeadToLeft(342, 700, colors[PionColor].value);
    drawArrowHeadToRight(472, 700, colors[PionColor].value);
}

void drawSettingsTitle() // S
{
    setcolor(colors[PionColor].value);
    settextstyle(3, HORIZ_DIR, 7);
    outtextxy(300 - 120,50,"Settings");
}

void drawAllSettingsSubtitles () // S
{
    settextstyle(3, HORIZ_DIR, 3);
    setcolor(colors[GridBKGColor].value);
    outtextxy(171 - 40, 165, "Grid Size");
    outtextxy(410 - 40, 165, "Direction");
    outtextxy(300 - 30, 335, "Theme");
    outtextxy(171 - 55, 465, "Background");
    outtextxy(410 - 20, 465, "Grid");
    outtextxy(171 - 75, 665, "Grid Background");
    outtextxy(410 - 20, 665, "Pion");
}

void drawBackButton()//S
{
    drawArrowHeadToLeft(30, 30, colors[PionColor].value);
}

void drawMusicIcon (int x, int y) // S - the double musical note
{
    setcolor(colors[PionColor].value);
    setfillstyle(SOLID_FILL, colors[PionColor].value);
    setlinestyle(SOLID_LINE,1,3);
    line (x, y + 2, x + 15, y - 2); // diagonal line
    setlinestyle(SOLID_LINE,1,1);
    line (x, y + 2, x, y + 15); //left vertical
    line (x + 15, y - 2, x + 15, y + 11); //right vertical
    fillellipse(x - 2, y + 15, 4, 3);
    fillellipse(x + 13, y + 11, 4, 3);
}

void drawSFXIcon (int x, int y)//S - SFX
{
    setcolor(colors[PionColor].value);
    settextstyle(3, HORIZ_DIR, 2);
    outtextxy(x, y, "SFX");
}

void drawSettingsSoundIcons (int x, int y) // S - draws the icons for music settings with x and y as top left corner
{
    drawMusicIcon(565, 25);
    drawSFXIcon(500, 22);
    if (musicIsDisabled)
        drawDisableSoundSettingCircle(2, colors[GridBKGColor].value);
    if (SFXIsDisabled)
        drawDisableSoundSettingCircle(1, colors[GridBKGColor].value);
}

void drawDisableSoundSettingCircle (int soundSetting, int color) // S - draws a cut circle over the icon given by soundSetting
{
    setcolor(color);
    if (soundSetting == 1) // SFX
    {
        circle(520, 33, 15);
        setlinestyle(SOLID_LINE, 1, 2);
        line(511, 43, 528, 22);
        setlinestyle(SOLID_LINE, 1, 1);
    }
    else // music
    {
        circle(573, 32, 15);
        setlinestyle(SOLID_LINE, 1, 2);
        line(564, 43, 580, 20);
        setlinestyle(SOLID_LINE, 1, 1);
    }
}

void enableMusic () // S - deletes the circle on music
{
    drawDisableSoundSettingCircle(2, colors[BKGColor].value);
    drawMusicIcon(565,25);
    musicIsDisabled = false;
}

void drawDropDownMusicMenu () // S - menu for choosing the soundtrack
{
    setcolor(colors[PionColor].value);
    settextstyle(3, HORIZ_DIR, 1);
    setfillstyle(SOLID_FILL, colors[PionColor].value);
    if (selectedSong == 0)
        setcolor(colors[GridBKGColor].value);
    outtextxy(510, 50, "None");
    setcolor(colors[PionColor].value);
    if (selectedSong == 1)
        setcolor(colors[GridBKGColor].value);
    outtextxy(525, 75, "1");
    setcolor(colors[PionColor].value);
    if (selectedSong == 2)
        setcolor(colors[GridBKGColor].value);
    outtextxy(525, 100, "2");
    setcolor(colors[PionColor].value);
    if (selectedSong == 3)
        setcolor(colors[GridBKGColor].value);
    outtextxy(525, 125, "3");
    setcolor(colors[PionColor].value);
    line(570,60, 560, 60); // NONE line
    line(570,85, 545, 85); // 1 line
    line(570,111, 545, 111); // 2 line
    line(570,135, 545, 135); //3 line
    line(570, 47, 570, 135); // verical line
    fillellipse(560, 60, 3, 3);
    fillellipse(545, 111, 3, 3);
    fillellipse(545, 135, 3, 3);
    fillellipse(545, 85, 3, 3);
}

void deleteDropDownMusicMenu() // S
{
    setcolor(colors[BKGColor].value);
    settextstyle(3, HORIZ_DIR, 1);
    setfillstyle(SOLID_FILL, colors[BKGColor].value);
    outtextxy(510, 50, "None");
    outtextxy(525, 75, "1");
    outtextxy(525, 100, "2");
    outtextxy(525, 125, "3");
    line(570,60, 560, 60); // NONE line
    line(570,85, 545, 85); // 1 line
    line(570,111, 545, 111); // 2 line
    line(570,135, 545, 135); //3 line
    line(570, 47, 570, 135); // verical line
    fillellipse(560, 60, 3, 3);
    fillellipse(545, 111, 3, 3);
    fillellipse(545, 135, 3, 3);
    fillellipse(545, 85, 3, 3);
}

void closeDropDownMusicMenu () // S
{
    deleteDropDownMusicMenu();
    musicDropMenuIsDrawn = false;
}

void drawSettingsGraphics() // S - the main settings drawing function
{
    drawSettingsTitle();
    drawAllArrows();
    drawVariables();
    drawAllSettingsSubtitles();
    drawBackButton();
    drawSettingsSoundIcons(480, 25);
}

void arrowPressFeedbackAnimation (int x, int y, char direction) // S - vibration of arrows when clicked
{
    float counter;
    if (direction == 'L')
    {
        drawArrowHeadToLeft(x, y, colors[BKGColor].value);
        for (counter = 0; counter < 5; counter += 0.3)
        {

            drawArrowHeadToLeft(x - counter, y, colors[PionColor].value);
            Sleep(3);
            drawArrowHeadToLeft(x - counter, y, colors[BKGColor].value);

        }
        for (counter = 5; counter >= 0; counter -= 0.3)
        {

            drawArrowHeadToLeft(x - counter, y, colors[PionColor].value);
            Sleep(3);
            drawArrowHeadToLeft(x - counter, y, colors[BKGColor].value);

        }
        drawArrowHeadToLeft(x, y, colors[PionColor].value);
    }
    else
    {
        drawArrowHeadToRight(x, y, colors[BKGColor].value);
        for (counter = 0; counter < 5; counter += 0.3)
        {

            drawArrowHeadToRight(x + counter, y, colors[PionColor].value);
            Sleep(3);
            drawArrowHeadToRight(x + counter, y, colors[BKGColor].value);

        }
        for (counter = 5; counter >= 0; counter -= 0.3)
        {

            drawArrowHeadToRight(x + counter, y, colors[PionColor].value);
            Sleep(3);
            drawArrowHeadToRight(x + counter, y, colors[BKGColor].value);

        }
        drawArrowHeadToRight(x, y, colors[PionColor].value);
    }
}

void searchIfColorsMatchBKG () // S - changes to colors of elements if they match background
{
    while (PionColor == BKGColor)
        PionColor = rand() % 7;
    while (GridColor == BKGColor)
        GridColor = rand() % 7;
    while  (GridBKGColor == BKGColor)
        GridBKGColor = rand() % 7;
}

void setPresetThemes(int theme) // S - dark and light (it will reset the values of the colors of the elements, no matter what they were before)
{
    if (theme == 0) // DARK
    {
        BKGColor = 0;
        PionColor = 1;
        GridColor = 4;
        GridBKGColor = 5;
    }
    else  // LIGHT
    {
        BKGColor = 6;
        PionColor = 2;
        GridColor = 5;
        GridBKGColor = 4;
    }
    setbkcolor(colors[BKGColor].value);
}

void playSoundtrack(int soundTrack) // C + S - all the sounds in the game; music is disable by playing NULL, the SFX needed a more ingenious method (done and described by S below)
{
    switch(soundTrack)
    {
    case 0:
        PlaySound(NULL,NULL,SND_LOOP | SND_ASYNC);
        break;
    case 1:
        PlaySound(TEXT("delphi.wav"),NULL,SND_LOOP | SND_ASYNC);
        break;
    case 2:
        PlaySound(TEXT("gungeon.wav"),NULL,SND_LOOP | SND_ASYNC);
        break;
    case 3:
        PlaySound(TEXT("elevator.wav"),NULL,SND_LOOP | SND_ASYNC);
        break;
    case 4: // for the error
        PlaySound(SFXDirectories[0] + SFXIsDisabled * 19,NULL, SND_ASYNC);  // if SFX is disabled (true = 1), the char variable will point to NULL (the one at the and of any string) so no sound will be played,
        break;
    case 5 : // for win
        PlaySound(SFXDirectories [1] + SFXIsDisabled * 19,NULL, SND_ASYNC); //  otherwise, it will point to the beginning of the directory file name //
        break;
    case 6:  // for lose
        PlaySound(SFXDirectories [2] + SFXIsDisabled * 19,NULL, SND_ASYNC);
        break;
    case 7: // move pion
        PlaySound(SFXDirectories [3] + SFXIsDisabled * 19,NULL, SND_ASYNC);
        break;
    }
}

void startSettings() // S - opens settings screen
{
    int x, y;
    cleardevice();
    drawSettingsGraphics();
    while (1)
    {
        if (ismouseclick(WM_LBUTTONDOWN))
        {
            clearmouseclick(WM_LBUTTONDOWN);
            x = mousex();
            y = mousey();

            if ( x <= 30 && x >= 8 && y > 30 + 0.3 * (30 - x) && y < 50 - 0.3 * (30 - x)) //back button
            {
                animatieRuleaza = true;
                iesiEcran = false;
                windowToEnter = 4;
                settingsExit = true;
                musicDropMenuIsDrawn = false;
                return;
            }

            if ( x > 489 && x < 539 && y > 27 && y < 41) // click on SFX
            {
                if (!SFXIsDisabled)
                {
                    drawDisableSoundSettingCircle(1, colors[GridBKGColor].value);
                    SFXIsDisabled = true;
                }
                else
                {
                    drawDisableSoundSettingCircle(1, colors[BKGColor].value);
                    drawSFXIcon(500,22);
                    SFXIsDisabled = false;

                }
            }

            if ( x > 560 &&  x < 580 && y > 24 && y < 40) // click on music
            {
                if (!musicDropMenuIsDrawn)
                {
                    drawDropDownMusicMenu();
                    musicDropMenuIsDrawn = true;
                }
                else
                {
                    closeDropDownMusicMenu();
                }
            }

            if  ( x > 507 &&  x < 549 && y > 52 && y < 66) // click on none
            {
                playSoundtrack(0);
                closeDropDownMusicMenu();
                drawDisableSoundSettingCircle(2, colors[GridBKGColor].value);
                musicIsDisabled = true;
                selectedSong = 0;

            }

            if ( x > 523 &&  x < 531 && y > 78 && y < 92) // click on song 1
            {
                 playSoundtrack (1);
                enableMusic(); //deletes the circle if the music was disabled
                closeDropDownMusicMenu();
                selectedSong = 1;

            }

            if ( x > 523 &&  x < 534 && y > 103 && y < 116) // click on song 2
            {
                playSoundtrack(2);
                enableMusic();
                closeDropDownMusicMenu();
                selectedSong = 2;

            }

            if ( x > 523 &&  x < 534 && y > 129 && y < 143) // click on song 3
            {
                playSoundtrack(3);
                enableMusic();
                closeDropDownMusicMenu();
                selectedSong = 3;

            }
            // the next ifs are for each individual arrowhead
            if ( x <= 106 && x >= 84 && y > 200 + 0.3 * (106 - x) && y < 220 - 0.3 * (106 - x)) // as x goes further away from the base of the arrow head, the boundries for y decrease
            {
                arrowPressFeedbackAnimation(106, 200, 'L');
                if (gridSize > 0)
                {
                    setcolor(colors[BKGColor].value);
                    outtextxy(171 - gridSizes[gridSize].lenght / 2, 198, gridSizes[gridSize].text);
                    gridSize --;
                    startSettings();
                    if (settingsExit)
                    {
                        return;
                    }
                    setcolor(colors[PionColor].value);
                    outtextxy(171 - gridSizes[gridSize].lenght / 2, 198, gridSizes[gridSize].text);
                }
            }

            if ( x >= 236 && x < 258 && y > 200 - 0.3 * (236 - x) && y < 220 + 0.3 * (236 - x))
            {
                arrowPressFeedbackAnimation(236, 200, 'R');
                if (gridSize < 3)
                {
                    setcolor(colors[BKGColor].value);
                    outtextxy(171 - gridSizes[gridSize].lenght / 2, 198, gridSizes[gridSize].text);
                    gridSize ++;
                    startSettings();
                    if (settingsExit)
                    {
                        return;
                    }
                    setcolor(colors[PionColor].value);
                    outtextxy(171 - gridSizes[gridSize].lenght / 2, 198, gridSizes[gridSize].text);
                }
            }

            if ( x <= 342 && x >= 320 && y > 200 + 0.3 * (342 - x) && y < 220 - 0.3 * (342 - x))
            {
                arrowPressFeedbackAnimation(342, 200, 'L');
                if ( setDirection == 1)
                {
                    setcolor(colors[BKGColor].value);
                    outtextxy(410 - directions[setDirection].lenght / 2, 198, directions[setDirection].text);
                    setDirection --;
                    startSettings();
                    if (settingsExit)
                    {
                        return;
                    }
                    setcolor(colors[PionColor].value);
                    outtextxy(410 - directions[setDirection].lenght / 2, 198, directions[setDirection].text);
                }
            }

            if ( x >= 472 && x < 494 && y > 200 - 0.3 * (472 - x) && y < 220 + 0.3 * (472 - x))
            {
                arrowPressFeedbackAnimation(472, 200, 'R');
                if ( setDirection == 0 )
                {
                    setcolor(colors[BKGColor].value);
                    outtextxy(410 - directions[setDirection].lenght / 2, 198, directions[setDirection].text);
                    setDirection++;
                    startSettings();
                    if (settingsExit)
                    {
                        return;
                    }
                    setcolor(colors[PionColor].value);
                    outtextxy(410 - directions[setDirection].lenght / 2, 198, directions[setDirection].text);
                }
            }

            if ( x <= 235 && x >= 213 && y > 370 + 0.3 * (235 - x) && y < 390 - 0.3 * (235 - x))
            {
                arrowPressFeedbackAnimation(235, 370, 'L');
                if (theme == 1 )
                {
                    setcolor(colors[BKGColor].value);
                    outtextxy(300 - themes[theme].lenght / 2, 368, themes[theme].text);
                    theme = 0;
                    setPresetThemes(0);
                    startSettings();
                    if (settingsExit)
                    {
                        return;
                    }
                    setcolor(colors[6].value);
                    outtextxy(300 - themes[theme].lenght / 2, 368, themes[theme].text);
                }
            }

            if ( x >= 365 && x < 387 && y > 370 - 0.3 * (365 - x) && y < 390 + 0.3 * (365 - x))
            {
                arrowPressFeedbackAnimation(365, 370, 'R');
                if (theme == 0 )
                {
                    setcolor(colors[BKGColor].value);
                    outtextxy(300 - themes[theme].lenght / 2, 368, themes[theme].text);
                    theme = 1;
                    setPresetThemes(1);
                    startSettings();
                    if (settingsExit)
                    {
                        return;
                    }
                    setcolor(colors[0].value);
                    outtextxy(300 - themes[theme].lenght / 2, 368, themes[theme].text);
                }
            }

            if ( x <= 106 && x >= 84 && y > 500 + 0.3 * (106 - x) && y < 520 - 0.3 * (106 - x)) // as x goes further away from the base of the arrow head the boundries for y decrease
            {
                arrowPressFeedbackAnimation(106, 500, 'L');
                if (BKGColor > 0 )
                {
                    setcolor(colors[BKGColor].value);
                    outtextxy(171 - colors[BKGColor + 1].lenght / 2, 498, colors[BKGColor ].text);
                    BKGColor --;
                    setbkcolor(colors[BKGColor].value);
                    searchIfColorsMatchBKG();
                    startSettings();
                    if (settingsExit)
                    {
                        return;
                    }
                    setcolor(colors[BKGColor + 1].value);
                    outtextxy(171 - colors[BKGColor].lenght / 2, 498, colors[BKGColor].text);
                }
            }

            if ( x >= 236 && x < 258 && y > 500 - 0.3 * (236 - x) && y < 520 + 0.3 * (236 - x))
            {
                arrowPressFeedbackAnimation(236, 500, 'R');
                if (BKGColor < 6 )
                {
                    setcolor(colors[BKGColor].value);
                    outtextxy(171 - colors[BKGColor].lenght / 2, 498, colors[BKGColor].text);
                    BKGColor ++;
                    setbkcolor(colors[BKGColor].value);
                    searchIfColorsMatchBKG();
                    startSettings();
                    if (settingsExit)
                    {
                        return;
                    }
                    setcolor(colors[BKGColor - 1].value);
                    outtextxy(171 - colors[BKGColor].lenght / 2, 498, colors[BKGColor].text);
                }
            }

            if ( x <= 342 && x >= 320 && y > 500 + 0.3 * (342 - x) && y < 520 - 0.3 * (342 - x))
            {
                arrowPressFeedbackAnimation(342, 500, 'L');
                if (GridColor > 0)
                {
                    setcolor(colors[BKGColor].value);
                    outtextxy(410 - colors[GridColor].lenght / 2, 498, colors[GridColor].text);
                    GridColor --;
                    searchIfColorsMatchBKG();
                    startSettings();
                    if (settingsExit)
                    {
                        return;
                    }
                    setcolor(colors[GridColor].value);
                    outtextxy(410 - colors[GridColor].lenght / 2, 498, colors[GridColor].text);
                }
            }

            if ( x >= 472 && x < 494 && y > 500 - 0.3 * (472 - x) && y < 520 + 0.3 * (472 - x))
            {
                arrowPressFeedbackAnimation(472, 500, 'R');
                if (GridColor < 6 )
                {
                    setcolor(colors[BKGColor].value);
                    outtextxy(410 - colors[GridColor].lenght / 2, 498, colors[GridColor].text);
                    GridColor ++;
                    searchIfColorsMatchBKG();
                    startSettings();
                    if (settingsExit)
                    {
                        return;
                    }
                    setcolor(colors[GridColor].value);
                    outtextxy(410 - colors[GridColor].lenght / 2, 498, colors[GridColor].text);
                }
            }

            if ( x <= 106 && x >= 84 && y > 700 + 0.3 * (106 - x) && y < 720 - 0.3 * (106 - x)) // as x goes further away from the base of the arrow head the boundries for y decrease
            {
                arrowPressFeedbackAnimation(106, 700, 'L');
                if (GridBKGColor > 0 )
                {
                    setcolor(colors[BKGColor].value);
                    outtextxy(171 - colors[GridBKGColor].lenght / 2, 698, colors[GridBKGColor].text);
                    GridBKGColor --;
                    searchIfColorsMatchBKG();
                    startSettings();
                    if (settingsExit)
                    {
                        return;
                    }
                    setcolor(colors[GridBKGColor].value);
                    outtextxy(171 - colors[GridBKGColor].lenght / 2, 698, colors[GridBKGColor].text);
                }
            }

            if ( x >= 236 && x < 258 && y > 700 - 0.3 * (236 - x) && y < 720 + 0.3 * (236 - x))
            {
                arrowPressFeedbackAnimation(236, 700, 'R');
                if (GridBKGColor < 6)
                {
                    setcolor(colors[BKGColor].value);
                    outtextxy(171 - colors[GridBKGColor].lenght / 2, 698, colors[GridBKGColor].text);
                    GridBKGColor ++;
                    searchIfColorsMatchBKG();
                    startSettings();
                    if (settingsExit)
                    {
                        return;
                    }
                    setcolor(colors[GridBKGColor].value);
                    outtextxy(171 - colors[GridBKGColor].lenght / 2, 698, colors[GridBKGColor].text);
                }
            }

            if ( x <= 342 && x >= 320 && y > 700 + 0.3 * (342 - x) && y < 720 - 0.3 * (342 - x))
            {
                arrowPressFeedbackAnimation(342, 700, 'L');
                if (PionColor > 0 )
                {
                    setcolor(colors[BKGColor].value);
                    outtextxy(410 - colors[PionColor].lenght / 2, 698, colors[PionColor].text);
                    PionColor --;
                    searchIfColorsMatchBKG();
                    startSettings();
                    if (settingsExit)
                    {
                        return;
                    }
                    setcolor(colors[PionColor].value);
                    outtextxy(410 - colors[PionColor].lenght / 2, 698, colors[PionColor].text);
                }
            }

            if ( x >= 472 && x < 494 && y > 700 - 0.3 * (472 - x) && y < 720 + 0.3 * (472 - x))
            {
                arrowPressFeedbackAnimation(472, 700, 'R');
                if (PionColor < 6)
                {
                    setcolor(colors[BKGColor].value);
                    outtextxy(410 - colors[PionColor].lenght / 2, 698, colors[PionColor].text);
                    PionColor ++;
                    searchIfColorsMatchBKG();
                    startSettings();
                    if (settingsExit)
                    {
                        return;
                    }
                    setcolor(colors[PionColor].value);
                    outtextxy(410 - colors[PionColor].lenght / 2, 698, colors[PionColor].text);
                }
            }
        }
    }
}

int CPU,PLAYER;
void selectareJucatorSiCPU()/*C - functia determina directia pe care va juca omul, respectiv calculatorul in modul PvCPU*/
{
    if(direction==0)
    {
        PLAYER=0;
        CPU=1;
    }
    else
    {
        PLAYER=1;
        CPU=0;
    }
}

int piesaRandUrm(struct pozitie k)/*C - functia determina coloana cu o anume proprietate pe care calculatorul va muta o piesa pe dificultatea Hard (CPU=0 - dreapta)*/
{
    int j=k.column+1;
    if(k.row-1>=1)
    {
        while(j<=gridSizes[gridSize].value and (j-1==k.column or TablaDeJoc[k.row][j-1]!=1))
        {
            if(TablaDeJoc[k.row-1][j]==1 and TablaDeJoc[k.row][j]==0)
                return j;
            j++;
        }
    }
    return -1;
}

int piesaColoanaUrm(struct pozitie k)/*C - functia determina randul cu o anume proprietate pe care calculatorul va muta o piesa pe dificultatea Hard (CPU=1 - sus)*/
{
    int j=k.row-1;
    if(k.column+1<=gridSizes[gridSize].value)
    {
        while(j>=1 and (j+1==k.row or TablaDeJoc[j+1][k.column]!=1))
        {
            if(TablaDeJoc[j][k.column+1]==1 and TablaDeJoc[j][k.column]==0)
                return j;
            j--;
        }
    }
    return -1;
}

int piesaRandPreced(struct pozitie k)/*C - functia determina coloana cu o anume proprietate pe care calculatorul va muta o piesa pe dificultatea Hard (CPU=0 - dreapta)*/
{
    int j=k.column+1;
    if(k.row+1<=gridSizes[gridSize].value)
    {
        while(j<=gridSizes[gridSize].value and (j-1==k.column or TablaDeJoc[k.row][j-1]!=1))
        {
            if(TablaDeJoc[k.row+1][j]==1 and TablaDeJoc[k.row][j]==0)
                return j;
            j++;
        }
    }
    return -1;
}

int piesaColoanaPreced(struct pozitie k)/*C - functia determina randul cu o anume proprietate pe care calculatorul va muta o piesa pe dificultatea Hard (CPU=1 - sus)*/
{
    int j=k.row-1;
    if(k.column-1>=1)
    {
        while(j>=1 and (j+1==k.row or TablaDeJoc[j+1][k.column]!=1))
        {
            if(TablaDeJoc[j][k.column-1]==1 and TablaDeJoc[j][k.column]==0)
                return j;
            j--;
        }
    }
    return -1;
}

int determinareRand(struct pozitie k)/*C - functia determina randul pe care calculatorul va muta o piesa pe dificultatea Easy (pentru CPU=1 - sus)*/
{
    int j=1;
    int aux;
    while (j<k.row and TablaDeJoc[j][k.column]==1)
        j++;
    if(j==k.row)
        return -1;
    else
    {
        aux=j;
        while(aux<k.row and TablaDeJoc[aux][k.column]==0)
            aux++;
        if(aux!=k.row)
            return -1;
        else
            return j;
    }
}

int determinareColoana(struct pozitie k)/*C - functia determina coloana pe care calculatorul va muta o piesa pe dificultatea Easy (pentru CPU=0 - dreapta)*/
{
    int j=gridSizes[gridSize].value;
    int aux;
    while(j>k.column and TablaDeJoc[k.row][j]==1)
        j--;
    if(j==k.column)
        return -1;
    else
    {
        aux=j;
        while(aux>k.column and TablaDeJoc[k.row][aux]==0)
            aux--;
        if(aux!=k.column)
            return -1;
        else
            return j;
    }
}

void mutareEasy()/*C*/
{
    int linia,coloana,x,y;
    int l,u,val;
    int i,aux,x1,y1;
    switch(PLAYER)
    {
        case 0:
        switch(direction)
        {
        case 0:/*Player*/
            while(1)
            {
                drawPlayingDirectionArrow(285,743,242,680,colors[PionColor].value, direction);
                if(ismouseclick(WM_LBUTTONDOWN))
                {
                    clearmouseclick(WM_LBUTTONDOWN);
                    x=mousex();
                    y=mousey();
                    if ( x <= 30 && x >= 8 && y > 30 + 0.3 * (30 - x) && y < 50 - 0.3 * (30 - x))
                    {
                        animatieRuleaza  = true;
                        iesiEcran = false;
                        ExitGame = true;
                        windowToEnter = 4;
                        setlinestyle(SOLID_LINE,1,1);
                        return;
                    }
                    if ( x <= 550 && x >= 450 && y > 155 && y < 180)
                    {
                        PLAYERScore = 0;
                        CPUScore = 0;
                        updateScoreBoardPvCPU(colors[BKGColor].value);
                        updateScoreBoardPvCPU(colors[PionColor].value);
                    }
                    if(x>=stanga && x<=stanga+width && y>=sus && y<=sus+height)
                    {
                        linia=(y-sus)/latura+1;
                        coloana=(x-stanga)/latura+1;
                        if (TablaDeJoc[linia][coloana]==0)
                        {
                            l=coloana-1;
                            while(l>=1 and TablaDeJoc[linia][l]!=1)
                                l--;
                            if(l>=1)
                            {
                                deplasareDreapta(linia,coloana,l);
                                val=cautarePiesa(piese,linia,l);
                                piese[val].column=coloana;
                                drawPlayingDirectionArrow(285,743,242,680,colors[BKGColor].value, direction);
                                return;
                            }
                            else
                            {
                                playSoundtrack(4); // error sound
                                playingArrowVibrationAnimation(285,743,242,680, direction);
                            }
                        }
                        else
                        {
                            playSoundtrack(4); // error sound
                            playingArrowVibrationAnimation(285,743,242,680, direction);
                        }
                    }
                }
            }
        case 1:/*CPU*/
            if(ismouseclick(WM_LBUTTONDOWN))
            {
                clearmouseclick(WM_LBUTTONDOWN);
                x=mousex();
                y=mousey();
                if ( x <= 30 && x >= 8 && y > 30 + 0.3 * (30 - x) && y < 50 - 0.3 * (30 - x))
                {
                    animatieRuleaza  = true;
                    iesiEcran = false;
                    ExitGame = true;
                    windowToEnter = 4;
                    setlinestyle(SOLID_LINE,1,1);
                    return;
                }
                if ( x <= 550 && x >= 450 && y > 155 && y < 180)
                {
                    PLAYERScore = 0;
                    CPUScore = 0;
                    updateScoreBoardPvCPU(colors[BKGColor].value);
                    updateScoreBoardPvCPU(colors[PionColor].value);
                }
            }
            i=1;
            while(i<=nrElem)
            {
                x1=piese[i].row;
                y1=piese[i].column;
                aux=determinareRand(piese[i]);
                if(aux!=-1)
                {
                    deplasareSus(aux,y1,x1);
                    piese[i].row=aux;
                    return;
                }
                i++;
            }
        }
        case 1:
        switch(direction)
        {
        case 1:/*Player*/
            while(1)
            {
                drawPlayingDirectionArrow(285,743,242,680,colors[PionColor].value, direction);
                if(ismouseclick(WM_LBUTTONDOWN))
                {
                    clearmouseclick(WM_LBUTTONDOWN);
                    x=mousex();
                    y=mousey();
                    if ( x <= 30 && x >= 8 && y > 30 + 0.3 * (30 - x) && y < 50 - 0.3 * (30 - x))
                    {

                        animatieRuleaza  = true;
                        iesiEcran = false;
                        ExitGame = true;
                        windowToEnter = 4;
                        setlinestyle(SOLID_LINE,1,1);
                        return;
                    }
                    if ( x <= 550 && x >= 450 && y > 155 && y < 180)
                    {
                        PLAYERScore = 0;
                        CPUScore = 0;
                        updateScoreBoardPvCPU(colors[BKGColor].value);
                        updateScoreBoardPvCPU(colors[PionColor].value);
                    }
                    if(x>=stanga && x<=stanga+width && y>=sus&&y<=sus+height)
                    {
                        linia=(y-sus)/latura+1;
                        coloana=(x-stanga)/latura+1;
                        if (TablaDeJoc[linia][coloana]==0)
                        {
                            u=linia+1;
                            while(u<=gridSizes[gridSize].value and TablaDeJoc[u][coloana]!=1)
                                u++;
                            if(u<=gridSizes[gridSize].value)
                            {
                                deplasareSus(linia,coloana,u);
                                val=cautarePiesa(piese,u,coloana);
                                piese[val].row=linia;
                                drawPlayingDirectionArrow(285,743,242,680,colors[BKGColor].value, direction);
                                return;
                            }
                            else
                            {
                                playSoundtrack(4); // error sound
                                playingArrowVibrationAnimation(285,743,242,680, direction);
                            }
                        }
                        else
                        {
                            playSoundtrack(4); // error sound
                            playingArrowVibrationAnimation(285,743,242,680, direction);
                        }
                    }
                }
            }
        case 0:/*CPU*/
            if(ismouseclick(WM_LBUTTONDOWN))
            {
                clearmouseclick(WM_LBUTTONDOWN);
                x=mousex();
                y=mousey();
                if ( x <= 30 && x >= 8 && y > 30 + 0.3 * (30 - x) && y < 50 - 0.3 * (30 - x))
                {
                    animatieRuleaza  = true;
                    iesiEcran = false;
                    ExitGame = true;
                    windowToEnter = 4;
                    setlinestyle(SOLID_LINE,1,1);
                    return;
                }
                if ( x <= 550 && x >= 450 && y > 155 && y < 180)
                {
                    PLAYERScore = 0;
                    CPUScore = 0;
                    updateScoreBoardPvCPU(colors[BKGColor].value);
                    updateScoreBoardPvCPU(colors[PionColor].value);
                }
            }
            i=1;
            while(i<=nrElem)
            {
                x1=piese[i].row;
                y1=piese[i].column;
                aux=determinareColoana(piese[i]);
                if(aux!=-1)
                {
                    deplasareDreapta(x1,aux,y1);
                    piese[i].column=aux;
                    return;
                }
                i++;
            }
        }
    }
}

void mutareHard()/*C*/
{
    int linia,coloana,x,y;
    int l,u,val;
    int i,val1,val2,k,x1,y1;
    switch(PLAYER)
    {
    case 0:
        switch(direction)
        {
        case 0:/*Player*/
            while(1)
            {
                drawPlayingDirectionArrow(285,743,242,680,colors[PionColor].value, direction);
                if(ismouseclick(WM_LBUTTONDOWN))
                {
                    clearmouseclick(WM_LBUTTONDOWN);
                    x=mousex();
                    y=mousey();
                    if ( x <= 30 && x >= 8 && y > 30 + 0.3 * (30 - x) && y < 50 - 0.3 * (30 - x))
                    {
                        animatieRuleaza  = true;
                        iesiEcran = false;
                        ExitGame = true;
                        windowToEnter = 4;
                        setlinestyle(SOLID_LINE,1,1);
                        return;
                    }
                    if ( x <= 550 && x >= 450 && y > 155 && y < 180)
                    {
                        PLAYERScore = 0;
                        CPUScore = 0;
                        updateScoreBoardPvCPU(colors[BKGColor].value);
                        updateScoreBoardPvCPU(colors[PionColor].value);
                    }
                    if(x>=stanga && x<=stanga+width && y>=sus && y<=sus+height)
                    {
                        linia=(y-sus)/latura+1;
                        coloana=(x-stanga)/latura+1;
                        if (TablaDeJoc[linia][coloana]==0)
                        {
                            l=coloana-1;
                            while(l>=1 and TablaDeJoc[linia][l]!=1)
                                l--;
                            if(l>=1)
                            {
                                deplasareDreapta(linia,coloana,l);
                                val=cautarePiesa(piese,linia,l);
                                piese[val].column=coloana;
                                piesaPreced=val;
                                drawPlayingDirectionArrow(285,743,242,680,colors[BKGColor].value, direction);
                                return;
                            }
                            else
                            {
                                playSoundtrack(4); // error sound
                                playingArrowVibrationAnimation(285,743,242,680, direction);
                            }
                        }
                        else
                        {
                            playSoundtrack(4); // error sound
                            playingArrowVibrationAnimation(285,743,242,680, direction);
                        }
                    }
                }
            }
        case 1:/*CPU*/
            if(ismouseclick(WM_LBUTTONDOWN))
            {
                clearmouseclick(WM_LBUTTONDOWN);
                x=mousex();
                y=mousey();
                if ( x <= 30 && x >= 8 && y > 30 + 0.3 * (30 - x) && y < 50 - 0.3 * (30 - x))
                {
                    animatieRuleaza  = true;
                    iesiEcran = false;
                    ExitGame = true;
                    windowToEnter = 4;
                    setlinestyle(SOLID_LINE,1,1);
                    return;
                }
                if ( x <= 550 && x >= 450 && y > 155 && y < 180)
                {
                    PLAYERScore = 0;
                    CPUScore = 0;
                    updateScoreBoardPvCPU(colors[BKGColor].value);
                    updateScoreBoardPvCPU(colors[PionColor].value);
                }
            }
            i=piesaPreced;
            while(i<=nrElem)/*de la piesa mutata de jucator pana la ultima piesa*/
            {
                y1=piese[i].column;
                x1=piese[i].row;
                val1=piesaColoanaUrm(piese[i]);
                val2=piesaColoanaPreced(piese[i]);
                if(val1!=-1 or val2!=-1)
                {
                    if(val1!=-1 and val2!=-1 and val1==val2)
                    {
                        deplasareSus(val1,y1,x1);
                        piese[i].row=val1;
                        return;
                    }
                    else
                    {
                        if(val1!=-1)
                        {
                            deplasareSus(val1,y1,x1);
                            piese[i].row=val1;
                            return;
                        }
                        else
                        {
                            deplasareSus(val2,y1,x1);
                            piese[i].row=val2;
                            return;
                        }
                    }
                }
                i++;
            }
            i=1;
            while(i<piesaPreced)/*de la prima piesa pana la cea imediat precedenta celei mutate de jucator*/
            {
                y1=piese[i].column;
                x1=piese[i].row;
                val1=piesaColoanaUrm(piese[i]);
                val2=piesaColoanaPreced(piese[i]);
                if(val1!=-1 or val2!=-1)
                {
                    if(val1!=-1 and val2!=-1 and val1==val2)
                    {
                        deplasareSus(val1,y1,x1);
                        piese[i].row=val1;
                        return;
                    }
                    else
                    {
                        if(val1!=-1)
                        {
                            deplasareSus(val1,y1,x1);
                            piese[i].row=val1;
                            return;
                        }
                        else
                        {
                            deplasareSus(val2,y1,x1);
                            piese[i].row=val2;
                            return;
                        }
                    }
                }
                i++;
            }
            i=1;
            while(i<=nrElem)
            {
                y1=piese[i].column;
                x1=piese[i].row;
                k=x1-1;
                if(TablaDeJoc[k][y1]==0 and k>=1)
                {
                    deplasareSus(k,y1,x1);
                    piese[i].row=k;
                    return;
                }
            i++;
            }
        }
    case 1:
        switch(direction)
        {
        case 1:/*Player*/
            while(1)
            {
                drawPlayingDirectionArrow(285,743,242,680,colors[PionColor].value, direction);
                if(ismouseclick(WM_LBUTTONDOWN))
                {
                    clearmouseclick(WM_LBUTTONDOWN);
                    x=mousex();
                    y=mousey();
                    if ( x <= 30 && x >= 8 && y > 30 + 0.3 * (30 - x) && y < 50 - 0.3 * (30 - x))
                    {

                        animatieRuleaza  = true;
                        iesiEcran = false;
                        ExitGame = true;
                        windowToEnter = 4;
                        setlinestyle(SOLID_LINE,1,1);
                        return;
                    }
                    if ( x <= 550 && x >= 450 && y > 155 && y < 180)
                    {
                        PLAYERScore = 0;
                        CPUScore = 0;
                        updateScoreBoardPvCPU(colors[BKGColor].value);
                        updateScoreBoardPvCPU(colors[PionColor].value);
                    }
                    if(x>=stanga && x<=stanga+width && y>=sus&&y<=sus+height)
                    {
                        linia=(y-sus)/latura+1;
                        coloana=(x-stanga)/latura+1;
                        if (TablaDeJoc[linia][coloana]==0)
                        {
                            u=linia+1;
                            while(u<=gridSizes[gridSize].value and TablaDeJoc[u][coloana]!=1)
                                u++;
                            if(u<=gridSizes[gridSize].value)
                            {
                                deplasareSus(linia,coloana,u);
                                val=cautarePiesa(piese,u,coloana);
                                piese[val].row=linia;
                                piesaPreced=val;
                                drawPlayingDirectionArrow(285,743,242,680,colors[BKGColor].value, direction);
                                return;
                            }
                            else
                            {
                                playSoundtrack(4); // error sound
                                playingArrowVibrationAnimation(285,743,242,680, direction);
                            }
                        }
                        else
                        {
                            playSoundtrack(4); // error sound
                            playingArrowVibrationAnimation(285,743,242,680, direction);
                        }
                    }
                }
            }
        case 0:/*CPU*/
            if(ismouseclick(WM_LBUTTONDOWN))
            {
                clearmouseclick(WM_LBUTTONDOWN);
                x=mousex();
                y=mousey();
                if ( x <= 30 && x >= 8 && y > 30 + 0.3 * (30 - x) && y < 50 - 0.3 * (30 - x))
                {
                    animatieRuleaza  = true;
                    iesiEcran = false;
                    ExitGame = true;
                    windowToEnter = 4;
                    setlinestyle(SOLID_LINE,1,1);
                    return;
                }
                if ( x <= 550 && x >= 450 && y > 155 && y < 180)
                {
                    PLAYERScore = 0;
                    CPUScore = 0;
                    updateScoreBoardPvCPU(colors[BKGColor].value);
                    updateScoreBoardPvCPU(colors[PionColor].value);
                }
            }
            i=piesaPreced;
            while(i<=nrElem)/*de la piesa mutata de jucator pana la ultima piesa*/
            {
                y1=piese[i].column;
                x1=piese[i].row;
                val1=piesaRandUrm(piese[i]);
                val2=piesaRandPreced(piese[i]);
                if(val1!=-1 or val2!=-1)
                {
                    if(val1!=-1 and val2!=-1 and val1==val2)
                    {
                        deplasareDreapta(x1,val1,y1);
                        piese[i].column=val1;
                        return;
                    }
                    else
                    {
                        if(val1!=-1)
                        {
                            deplasareDreapta(x1,val1,y1);
                            piese[i].column=val1;
                            return;
                        }
                        else
                        {
                            deplasareDreapta(x1,val2,y1);
                            piese[i].column=val2;
                            return;
                        }
                    }
                }
                i++;
            }
            i=1;
            while(i<piesaPreced)/*de la prima piesa pana la cea imediat precedenta celei mutate de jucator*/
            {
                y1=piese[i].column;
                x1=piese[i].row;
                val1=piesaRandUrm(piese[i]);
                val2=piesaRandPreced(piese[i]);
                if(val1!=-1 or val2!=-1)
                {
                    if(val1!=-1 and val2!=-1 and val1==val2)
                    {
                        deplasareDreapta(x1,val1,y1);
                        piese[i].column=val1;
                        return;
                    }
                    else
                    {
                        if(val1!=-1)
                        {
                            deplasareDreapta(x1,val1,y1);
                            piese[i].column=val1;
                            return;
                        }
                        else
                        {
                            deplasareDreapta(x1,val2,y1);
                            piese[i].column=val2;
                            return;
                        }
                    }
                }
                i++;
            }
            i=1;
            while(i<=nrElem)
            {
                y1=piese[i].column;
                x1=piese[i].row;
                k=y1+1;
                if(TablaDeJoc[x1][k]==0 and k<=gridSizes[gridSize].value)
                {
                    deplasareDreapta(x1,k,y1);
                    piese[i].column=k;
                    return;
                }
            i++;
            }
        }
    }
}

void startEasyPvCPU()/*C (pe baza design-ului de la startPvPGame)*/
{
    playSoundtrack(0);
    setbkcolor(colors[BKGColor].value);
    cleardevice();
    clearmouseclick(WM_LBUTTONDOWN);
    desen();
    direction = setDirection;
    selectareJucatorSiCPU();
    stillPlaying=true;
    while(stillPlaying && !ExitGame)
    {
        stillPlaying=false;
        mutareEasy();
        if (ExitGame)// back button was pressed
        {
            PLAYERScore = 0;
            CPUScore = 0;
            resetGame();
            playSoundtrack(selectedSong);
            return;
        }
        direction = 1 - direction;
        posibilaMutare();
        curataBuffer();
    }
    endGamePvCPU();
}

void startHardPvCPU()/*C (pe baza design-ului de la startPvPGame)*/
{
    playSoundtrack(0);
    setbkcolor(colors[BKGColor].value);
    cleardevice();
    clearmouseclick(WM_LBUTTONDOWN);
    desen();
    direction = setDirection;
    selectareJucatorSiCPU();
    stillPlaying=true;
    while(stillPlaying && !ExitGame)
    {
        stillPlaying=false;
        mutareHard();
        if (ExitGame)// back button was pressed
        {
            PLAYERScore = 0;
            CPUScore = 0;
            resetGame();
            playSoundtrack(selectedSong);
            return;
        }
        direction = 1 - direction;
        posibilaMutare();
        curataBuffer();
    }
    endGamePvCPU();
}

void declarations() // S - for all the global settings that need to be stored to be kept while navigating through screens
{
    strcpy(colors[0].text, "BLACK");
    strcpy(colors[1].text, "GREEN");
    strcpy(colors[2].text, "RED");
    strcpy(colors[3].text, "BROWN");
    strcpy(colors[4].text, "LIGHTGRAY");
    strcpy(colors[5].text, "DARKGRAY");
    strcpy(colors[6].text, "WHITE");

    colors[0].lenght = 66;
    colors[1].lenght = 70;
    colors[2].lenght = 40;
    colors[3].lenght = 76;
    colors[4].lenght = 110;
    colors[5].lenght = 110;
    colors[6].lenght = 64;

    colors[0].value = 0;
    colors[1].value = 2;
    colors[2].value = 4;
    colors[3].value = 6;
    colors[4].value = 7;
    colors[5].value = 8;
    colors[6].value = 15;

    strcpy(directions[0].text, "RIGHT");
    strcpy(directions[1].text, "UP");

    directions[0].lenght = 62;
    directions[1].lenght = 25;

    strcpy(themes[0].text, "DARK");
    strcpy(themes[1].text, "LIGHT");

    themes[0].lenght = 55;
    themes[1].lenght = 60;

    themes[0].value = 15;
    themes[1].value = 0;

    strcpy(gridSizes[0].text, "4x4");
    strcpy(gridSizes[1].text, "6x6");
    strcpy(gridSizes[2].text, "8x8");
    strcpy(gridSizes[3].text, "10x10");

    gridSizes[0].lenght = 34;
    gridSizes[1].lenght = 34;
    gridSizes[2].lenght = 34;
    gridSizes[3].lenght = 55;

    gridSizes[0].value = 4;
    gridSizes[1].value = 6;
    gridSizes[2].value = 8;
    gridSizes[3].value = 10;
}

int main()
{
    declarations();
    initwindow(600, 800);
    playSoundtrack(1);
    registermousehandler(WM_LBUTTONDOWN,stopAnimation);
    displayMenu();

    while (gameRunning)
    {
        switch(windowToEnter)
        {
        case 1:
            cleardevice();
            choice=0;
            startPvPGame();
            break;
        case 3:
            cleardevice();
            startSettings();
            break;
        case 4: //going back to menu from other screens
            cleardevice();
            displayMenu();
            break;
        case 5:
            cleardevice();
            choice=1;
            startEasyPvCPU();
            break;
        case 6:
            cleardevice();
            choice=1;
            startHardPvCPU();
            break;
        case 7:
            cleardevice();
            gameRunning = false;
            break;
        }
    }

    closegraph();
    return 0;
}
