#include <iostream>
#include <fstream>
#include <math.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#define UP 1
#define LEFT 4
#define RIGHT 2
#define DOWN 3
bool in_menu=true;
bool in_game=false;
bool game_over=false;
bool nuke=false;
int OneShootKill=0;
int menu;
char Map[3][21][30];
char Menu[16][21][30];
char Map_Play[2][21][30];
struct Tankuri{
    int type,x,y,hp,dmg,orientation;
    bool alive;
} tanks[5];
int difficulty;
int play_map;
using namespace std;
void LoadResurces();
void LoadMenu();
void NewGame();
void PrintMenu(int NumberMenu);
void SelectTank();
void GameOn();
void GameOverWin();
void GameOverLose();
void Prepare_PlayGround();
void PrintMap();
void FromHPToString(int hp_amount,char hp_value[3]);
void OrientationPrint();
void CopyGameMap();
void ProcessInput(char input);
void MoveForward(int id);
void MoveBackward(int id);
void RotateLeft(int id);
void RotateRight(int id);
void HitIA(int id);
bool IASawYou(int id);
void IAGoToLastPlayerPosition(int id,int x,int y);
void IAShoot(int id);
bool IATargetsYou(int id);
void AlertIA(int id);
void Nuke();


void Nuke()
{
    for(int i=1;i<=3;i++)
        tanks[i].hp-=30;
}

void IAGoToLastPlayerPosition(int id,int x,int y)
{

}

void IAShoot(int id)
{
    if(tanks[id].dmg>=tanks[0].hp)
        tanks[0].alive=false;

    tanks[0].hp-=tanks[id].dmg;
}

bool IATargetsYou(int id)
{
    if(tanks[id].orientation==UP)
    {
        for(int i=tanks[id].x-1;i>0;i--)
            if(Map_Play[0][i][tanks[id].y]=='#')
                break;
            else if(Map_Play[0][i][tanks[id].y]=='@')
                return true;
    }
    else if(tanks[id].orientation==RIGHT)
    {
        for(int i=tanks[id].y+1;i<30;i++)
            if(Map_Play[0][tanks[id].x][i]=='#')
                break;
            else if(Map_Play[0][tanks[id].x][i]=='@')
                return true;
    }
    else if(tanks[id].orientation==DOWN)
    {
        for(int i=tanks[id].x+1;i<21;i++)
            if(Map_Play[0][i][tanks[id].y]=='#')
                break;
        else if(Map_Play[0][i][tanks[id].y]=='@')
                return true;
    }
    else if(tanks[id].orientation==LEFT)
    {
        for(int i=tanks[id].y-1;i>0;i--)
            if(Map_Play[0][tanks[id].x][i]=='#')
                break;
            else if(Map_Play[0][tanks[id].x][i]=='@')
                return true;
    }

    return false;
}


bool IASawYou(int id)
{
    char MapCopy[21][30];

    for(int i=0;i<21;i++)
        for(int j=0;j<30;j++)
            {
                if(Map_Play[0][i][j]!='#')
                    MapCopy[i][j]=0;
                else
                    MapCopy[i][j]=Map_Play[0][i][j];
            }
    MapCopy[tanks[id].x][tanks[id].y]=1;

    if(tanks[id].orientation==UP)
    {
        int ystart=tanks[id].y-1,yfinish=tanks[id].y+1,xgoes=tanks[id].x-1,printer=1;
        for(;xgoes>0;xgoes--)
        {
            printer++;
            for(int j=ystart;j<=yfinish;j++)
            {
                if(MapCopy[xgoes][j]!='#')
                {
                if(j<tanks[id].y)
                {
                    if(MapCopy[xgoes+1][j+1]>0&&MapCopy[xgoes+1][j+1]!='#')
                        MapCopy[xgoes][j]=printer;
                }
                else if(j==tanks[id].y)
                {
                    if(MapCopy[xgoes+1][j]>0&&MapCopy[xgoes+1][j]!='#')
                        MapCopy[xgoes][j]=printer;
                }
                else if(j>tanks[id].y)
                {
                    if(MapCopy[xgoes+1][j-1]>0&&MapCopy[xgoes+1][j-1]!='#')
                        MapCopy[xgoes][j]=printer;
                }
                }
            }
            if(ystart>1)
                ystart--;
            if(yfinish<29)
                yfinish++;
        }
        if(MapCopy[tanks[0].x][tanks[0].y]>0)
            return true;
    }
    else if(tanks[id].orientation==RIGHT)
    {
        int ystart=tanks[id].x-1,yfinish=tanks[id].x+1,xgoes=tanks[id].y+1,printer=1;
        for(;xgoes<30;xgoes++)
        {
            printer++;
            for(int j=ystart;j<=yfinish;j++)
            {
                if(MapCopy[j][xgoes]!='#')
                {
                if(j<tanks[id].y)
                {
                    if(MapCopy[j-1][xgoes-1]>0&&MapCopy[j-1][xgoes-1]!='#')
                        MapCopy[j][xgoes]=printer;
                }
                else if(j==tanks[id].y)
                {
                    if(MapCopy[j][xgoes-1]>0&&MapCopy[j][xgoes-1]!='#')
                        MapCopy[j][xgoes]=printer;
                }
                else if(j>tanks[id].y)
                {
                    if(MapCopy[j+1][xgoes-1]>0&&MapCopy[j+1][xgoes-1]!='#')
                        MapCopy[j][xgoes]=printer;
                }
                }
            }
            if(ystart>1)
                ystart--;
            if(yfinish<20)
                yfinish++;
        }
        if(MapCopy[tanks[0].x][tanks[0].y]>0)
            return true;
    }
    else if(tanks[id].orientation==DOWN)
    {
        int ystart=tanks[id].y-1,yfinish=tanks[id].y+1,xgoes=tanks[id].x+1,printer=1;
        for(;xgoes<20;xgoes++)
        {
            printer++;
            for(int j=ystart;j<=yfinish;j++)
            {
                if(MapCopy[xgoes][j]!='#')
                {
                if(j<tanks[id].y)
                {
                    if(MapCopy[xgoes-1][j+1]>0&&MapCopy[xgoes-1][j+1]!='#')
                        MapCopy[xgoes][j]=printer;
                }
                else if(j==tanks[id].y)
                {
                    if(MapCopy[xgoes-1][j]>0&&MapCopy[xgoes-1][j]!='#')
                        MapCopy[xgoes][j]=printer;
                }
                else if(j>tanks[id].y)
                {
                    if(MapCopy[xgoes-1][j-1]>0&&MapCopy[xgoes-1][j-1]!='#')
                        MapCopy[xgoes][j]=printer;
                }
                }
            }
            if(ystart>1)
                ystart--;
            if(yfinish<29)
                yfinish++;
        }
        if(MapCopy[tanks[0].x][tanks[0].y]>0)
            return true;
    }
    else if(tanks[id].orientation==LEFT)
    {
        int ystart=tanks[id].x-1,yfinish=tanks[id].x+1,xgoes=tanks[id].y-1,printer=1;
        for(;xgoes>1;xgoes--)
        {
            printer++;
            for(int j=ystart;j<=yfinish;j++)
            {
                if(MapCopy[j][xgoes]!='#')
                {
                if(j<tanks[id].y)
                {
                    if(MapCopy[j-1][xgoes+1]>0&&MapCopy[j-1][xgoes+1]!='#')
                        MapCopy[j][xgoes]=printer;
                }
                else if(j==tanks[id].y)
                {
                    if(MapCopy[j][xgoes+1]>0&&MapCopy[j][xgoes+1]!='#')
                        MapCopy[j][xgoes]=printer;
                }
                else if(j>tanks[id].y)
                {
                    if(MapCopy[j+1][xgoes+1]>0&&MapCopy[j+1][xgoes+1]!='#')
                        MapCopy[j][xgoes]=printer;
                }
                }
            }
            if(ystart>1)
                ystart--;
            if(yfinish<20)
                yfinish++;
        }
        if(MapCopy[tanks[0].x][tanks[0].y]>0)
            return true;
    }
    return false;


}

void AlertIA(int id)
{
    if(tanks[0].orientation==UP)
    {
        if(tanks[id].orientation==LEFT)
            RotateLeft(id);
        else if(tanks[id].orientation==UP||tanks[id].orientation==RIGHT)
            RotateRight(id);
    }
    else if(tanks[0].orientation==RIGHT)
    {
        if(tanks[id].orientation==UP)
            RotateLeft(id);
        else if(tanks[id].orientation==LEFT||tanks[id].orientation==DOWN)
            RotateRight(id);
    }
    else if(tanks[0].orientation==DOWN)
    {
        if(tanks[id].orientation==RIGHT)
            RotateLeft(id);
        else if(tanks[id].orientation==DOWN||tanks[id].orientation==LEFT)
            RotateRight(id);
    }
    else if(tanks[0].orientation==LEFT)
    {
        if(tanks[id].orientation==DOWN)
            RotateLeft(id);
        else if(tanks[id].orientation==LEFT||tanks[id].orientation==UP)
            RotateRight(id);
    }

}


void HitIA(int id)
{


    int chance=rand()%100+1;
    if(tanks[0].type==0)
        {
        if(chance<=5)
           tanks[0].dmg*=2;
        }
    else if(tanks[0].type==1)
        {
        tanks[0].hp+=2;
            if(tanks[0].hp>120)
                tanks[0].hp=120;
        }

    if(tanks[0].dmg>=tanks[id].hp)
        tanks[id].alive=false;
    else
        tanks[id].hp-=tanks[0].dmg;
    if(difficulty==2)
    {
        for(int i=1;i<=3;i++)
            if(!IASawYou(i)&&!IATargetsYou(i))
                {
                    AlertIA(i);
                }
    }
    else if(difficulty==1||difficulty==0)
    {
        if(!IASawYou(id)&&!IATargetsYou(id))
                {
                    AlertIA(id);
                }
    }
    if(tanks[0].type==0)
        if(chance<=5)
           tanks[0].dmg/=2;
    if(OneShootKill)
    {
        tanks[id].alive=false;
        tanks[id].hp=0;
        OneShootKill--;
    }
}

void Shoot()
{
    if(tanks[0].orientation==UP)
    {
        for(int i=tanks[0].x-1;i>0;i--)
            if(Map_Play[0][i][tanks[0].y]=='#')
                break;
            else if(Map_Play[0][i][tanks[0].y]=='1')
                HitIA(1);
            else if(Map_Play[0][i][tanks[0].y]=='2')
                HitIA(2);
            else if(Map_Play[0][i][tanks[0].y]=='3')
                HitIA(3);
    }
    else if(tanks[0].orientation==RIGHT)
    {
        for(int i=tanks[0].y+1;i<30;i++)
            if(Map[0][tanks[0].x][i]=='#')
                break;
            else if(Map[0][tanks[0].x][i]=='1')
                HitIA(1);
            else if(Map[0][tanks[0].x][i]=='2')
                HitIA(2);
            else if(Map[0][tanks[0].x][i]=='3')
                HitIA(3);

    }
    else if(tanks[0].orientation==DOWN)
    {
        for(int i=tanks[0].x+1;i<21;i++)
            if(Map_Play[0][i][tanks[0].y]=='#')
                break;
            else if(Map_Play[0][i][tanks[0].y]=='1')
                HitIA(1);
            else if(Map_Play[0][i][tanks[0].y]=='2')
                HitIA(2);
            else if(Map_Play[0][i][tanks[0].y]=='3')
                HitIA(3);
    }
    else if(tanks[0].orientation==LEFT)
    {
        for(int i=tanks[0].y-1;i>0;i--)
            if(Map[0][tanks[0].x][i]=='#')
                break;
            else if(Map[0][tanks[0].x][i]=='1')
                HitIA(1);
            else if(Map[0][tanks[0].x][i]=='2')
                HitIA(2);
            else if(Map[0][tanks[0].x][i]=='3')
                HitIA(3);

    }
}


void MoveForward(int id)
{
    if(tanks[id].orientation==UP)
    {
        if(Map_Play[0][tanks[id].x-1][tanks[id].y]==' ')
        {
            Map_Play[0][tanks[id].x-1][tanks[id].y]='@';
            Map_Play[0][tanks[id].x][tanks[id].y]=' ';
            tanks[id].x--;
        }
        else if(Map_Play[0][tanks[id].x-1][tanks[id].y]=='N')
        {
            Map_Play[0][tanks[id].x-1][tanks[id].y]='@';
            Map_Play[0][tanks[id].x][tanks[id].y]=' ';
            tanks[id].x--;
            nuke=true;
        }
        else if(Map_Play[0][tanks[id].x-1][tanks[id].y]=='O')
        {
            Map_Play[0][tanks[id].x-1][tanks[id].y]='@';
            Map_Play[0][tanks[id].x][tanks[id].y]=' ';
            tanks[id].x--;
            OneShootKill=1;
        }
        else if(Map_Play[0][tanks[id].x-1][tanks[id].y]=='H')
        {
            Map_Play[0][tanks[id].x-1][tanks[id].y]='@';
            Map_Play[0][tanks[id].x][tanks[id].y]=' ';
            tanks[id].x--;
            tanks[0].hp+=30;
        }
        else if(Map_Play[0][tanks[id].x-1][tanks[id].y]=='D')
        {
            Map_Play[0][tanks[id].x-1][tanks[id].y]='@';
            Map_Play[0][tanks[id].x][tanks[id].y]=' ';
            tanks[id].x--;
            tanks[0].dmg+=5;
        }

    }
    else if(tanks[id].orientation==RIGHT)
    {
        if(Map_Play[0][tanks[id].x][tanks[id].y+1]==' ')
        {
            Map_Play[0][tanks[id].x][tanks[id].y+1]='@';
            Map_Play[0][tanks[id].x][tanks[id].y]=' ';
            tanks[id].y++;
        }
        else if(Map_Play[0][tanks[id].x][tanks[id].y+1]=='N')
        {
            Map_Play[0][tanks[id].x][tanks[id].y+1]='@';
            Map_Play[0][tanks[id].x][tanks[id].y]=' ';
            tanks[id].y++;
            nuke=true;
        }
        else if(Map_Play[0][tanks[id].x][tanks[id].y+1]=='O')
        {
            Map_Play[0][tanks[id].x][tanks[id].y+1]='@';
            Map_Play[0][tanks[id].x][tanks[id].y]=' ';
            tanks[id].y++;
            OneShootKill=1;
        }
        else if(Map_Play[0][tanks[id].x][tanks[id].y+1]=='H')
        {
            Map_Play[0][tanks[id].x][tanks[id].y+1]='@';
            Map_Play[0][tanks[id].x][tanks[id].y]=' ';
            tanks[id].y++;
            tanks[0].hp+=30;
        }
        else if(Map_Play[0][tanks[id].x][tanks[id].y+1]=='D')
        {
            Map_Play[0][tanks[id].x][tanks[id].y+1]='@';
            Map_Play[0][tanks[id].x][tanks[id].y]=' ';
            tanks[id].y++;
            tanks[0].dmg+=5;
        }
    }
    else if(tanks[id].orientation==DOWN)
    {
        if(Map_Play[0][tanks[id].x+1][tanks[id].y]==' ')
        {
            Map_Play[0][tanks[id].x+1][tanks[id].y]='@';
            Map_Play[0][tanks[id].x][tanks[id].y]=' ';
            tanks[id].x++;
        }
        else if(Map_Play[0][tanks[id].x+1][tanks[id].y]=='N')
        {
            Map_Play[0][tanks[id].x+1][tanks[id].y]='@';
            Map_Play[0][tanks[id].x][tanks[id].y]=' ';
            tanks[id].x++;
            nuke=true;
        }
        else if(Map_Play[0][tanks[id].x+1][tanks[id].y]=='O')
        {
            Map_Play[0][tanks[id].x+1][tanks[id].y]='@';
            Map_Play[0][tanks[id].x][tanks[id].y]=' ';
            tanks[id].x++;
            OneShootKill=1;
        }
        else if(Map_Play[0][tanks[id].x+1][tanks[id].y]=='H')
        {
            Map_Play[0][tanks[id].x+1][tanks[id].y]='@';
            Map_Play[0][tanks[id].x][tanks[id].y]=' ';
            tanks[id].x++;
            tanks[0].hp+=30;
        }
        else if(Map_Play[0][tanks[id].x+1][tanks[id].y]=='D')
        {
            Map_Play[0][tanks[id].x+1][tanks[id].y]='@';
            Map_Play[0][tanks[id].x][tanks[id].y]=' ';
            tanks[id].x++;
            tanks[0].dmg+=5;
        }
    }
    else if(tanks[id].orientation==LEFT)
    {
        if(Map_Play[0][tanks[id].x][tanks[id].y-1]==' ')
        {
            Map_Play[0][tanks[id].x][tanks[id].y-1]='@';
            Map_Play[0][tanks[id].x][tanks[id].y]=' ';
            tanks[id].y--;
        }
        else if(Map_Play[0][tanks[id].x][tanks[id].y-1]=='N')
        {
            Map_Play[0][tanks[id].x][tanks[id].y-1]='@';
            Map_Play[0][tanks[id].x][tanks[id].y]=' ';
            tanks[id].y--;
            nuke=true;
        }
        else if(Map_Play[0][tanks[id].x][tanks[id].y-1]=='O')
        {
            Map_Play[0][tanks[id].x][tanks[id].y-1]='@';
            Map_Play[0][tanks[id].x][tanks[id].y]=' ';
            tanks[id].y--;
            OneShootKill=1;
        }
        else if(Map_Play[0][tanks[id].x][tanks[id].y-1]=='H')
        {
            Map_Play[0][tanks[id].x][tanks[id].y-1]='@';
            Map_Play[0][tanks[id].x][tanks[id].y]=' ';
            tanks[id].y--;
            tanks[0].hp+=30;
        }
        else if(Map_Play[0][tanks[id].x][tanks[id].y-1]=='D')
        {
            Map_Play[0][tanks[id].x][tanks[id].y-1]='@';
            Map_Play[0][tanks[id].x][tanks[id].y]=' ';
            tanks[id].y--;
           tanks[0].dmg+=5;
        }
    }
}

void MoveBackward(int id)
{
    if(tanks[id].orientation==UP)
    {
        if(Map_Play[0][tanks[id].x+1][tanks[id].y]==' ')
        {
            Map_Play[0][tanks[id].x+1][tanks[id].y]='@';
            Map_Play[0][tanks[id].x][tanks[id].y]=' ';
            tanks[id].x++;
        }
    }
    else if(tanks[id].orientation==RIGHT)
    {
        if(Map_Play[0][tanks[id].x][tanks[id].y-1]==' ')
        {
            Map_Play[0][tanks[id].x][tanks[id].y-1]='@';
            Map_Play[0][tanks[id].x][tanks[id].y]=' ';
            tanks[id].y--;
        }
    }
    else if(tanks[id].orientation==DOWN)
    {
        if(Map_Play[0][tanks[id].x-1][tanks[id].y]==' ')
        {
            Map_Play[0][tanks[id].x-1][tanks[id].y]='@';
            Map_Play[0][tanks[id].x][tanks[id].y]=' ';
            tanks[id].x--;
        }
    }
    else if(tanks[id].orientation==LEFT)
    {
        if(Map_Play[0][tanks[id].x][tanks[id].y+1]==' ')
        {
            Map_Play[0][tanks[id].x][tanks[id].y+1]='@';
            Map_Play[0][tanks[id].x][tanks[id].y]=' ';
            tanks[id].y++;
        }
    }
}

void RotateLeft(int id)
{
    if(tanks[id].orientation==UP)
        tanks[id].orientation=LEFT;
    else if(tanks[id].orientation==RIGHT)
        tanks[id].orientation=UP;
    else if(tanks[id].orientation==DOWN)
        tanks[id].orientation=RIGHT;
    else if(tanks[id].orientation==LEFT)
        tanks[id].orientation=DOWN;
}

void RotateRight(int id)
{
    if(tanks[id].orientation==UP)
        tanks[id].orientation=RIGHT;
    else if(tanks[id].orientation==RIGHT)
        tanks[id].orientation=DOWN;
    else if(tanks[id].orientation==DOWN)
        tanks[id].orientation=LEFT;
    else if(tanks[id].orientation==LEFT)
        tanks[id].orientation=UP;
}


void ProcessInput(char input)
{
    int chanceNuke=rand()%100+1,chanceOne=rand()%100+1,chanceHP=rand()%100+1,chanceDMG=rand()%100+1;
    int x=rand()%28+2,y=rand()%2+18;
    if(chanceHP<3)
    {
        if(Map_Play[0][x][y]==' ')
                Map_Play[0][x][y]='H';
    }
    else if(chanceNuke<3)
    {
        if(Map_Play[0][x][y]==' ')
                Map_Play[0][x][y]='N';
    }
    else if(chanceDMG<3)
    {
        if(Map_Play[0][x][y]==' ')
                Map_Play[0][x][y]='D';
    }
    else if(chanceOne<3)
    {
        if(Map_Play[0][x][y]==' ')
                Map_Play[0][x][y]='O';
    }




    if(input=='q')
        Shoot();
    else if(input=='w')
        MoveForward(0);
    else if(input=='a')
        RotateLeft(0);
    else if(input=='d')
        RotateRight(0);
    else if(input=='s')
        MoveBackward(0);
    else if(input=='n')
        Nuke();

    if(tanks[0].type==2)
    {
        tanks[0].hp+=1;
        if(tanks[0].hp>150)
            tanks[0].hp=150;
    }

    if(!tanks[1].alive)
    {
        Map_Play[0][tanks[1].x][tanks[1].y]=' ';
    }
    if(!tanks[2].alive)
    {
        Map_Play[0][tanks[2].x][tanks[2].y]=' ';
    }
    if(!tanks[3].alive)
    {
        Map_Play[0][tanks[3].x][tanks[3].y]=' ';
    }

    for(int i=1;i<=3;i++)
        {
            if(IATargetsYou(i))
                IAShoot(i);
            if(IASawYou(i))
                IAGoToLastPlayerPosition(i,tanks[0].x,tanks[0].y);
        }



}


void OrientationPrint()
{
                        if(tanks[0].orientation==UP)
                        {
                            if(Map_Play[0][tanks[0].x-1][tanks[0].y]==' ')
                                Map_Play[1][tanks[0].x-1][tanks[0].y]='+';
                        }
                        else if(tanks[0].orientation==RIGHT)
                        {
                            if(Map_Play[0][tanks[0].x][tanks[0].y+1]==' ')
                                Map_Play[1][tanks[0].x][tanks[0].y+1]='+';
                        }
                        else if(tanks[0].orientation==DOWN)
                        {
                            if(Map_Play[0][tanks[0].x+1][tanks[0].y]==' ')
                                Map_Play[1][tanks[0].x+1][tanks[0].y]='+';
                        }
                        else if(tanks[0].orientation==LEFT)
                        {
                            if(Map_Play[0][tanks[0].x][tanks[0].y-1]==' ')
                                Map_Play[1][tanks[0].x][tanks[0].y-1]='+';
                        }

                if(tanks[1].alive)
                {
                    if(tanks[1].orientation==UP)
                    {
                        if(Map_Play[0][tanks[1].x-1][tanks[1].y]==' ')
                            Map_Play[1][tanks[1].x-1][tanks[1].y]='+';
                    }
                    else if(tanks[1].orientation==RIGHT)
                    {
                        if(Map_Play[0][tanks[1].x][tanks[1].y+1]==' ')
                            Map_Play[1][tanks[1].x][tanks[1].y+1]='+';
                    }
                    else if(tanks[1].orientation==DOWN)
                    {
                        if(Map_Play[0][tanks[1].x+1][tanks[1].y]==' ')
                            Map_Play[1][tanks[1].x+1][tanks[1].y]='+';
                    }
                    else if(tanks[1].orientation==LEFT)
                    {
                        if(Map_Play[0][tanks[1].x][tanks[1].y-1]==' ')
                            Map_Play[1][tanks[1].x][tanks[1].y-1]='+';
                    }
                }

                if(tanks[2].alive)
                {
                    if(tanks[2].orientation==UP)
                    {
                        if(Map_Play[0][tanks[2].x-1][tanks[2].y]==' ')
                            Map_Play[1][tanks[2].x-1][tanks[2].y]='+';
                    }
                    else if(tanks[2].orientation==RIGHT)
                    {
                        if(Map_Play[0][tanks[2].x][tanks[2].y+1]==' ')
                            Map_Play[1][tanks[2].x][tanks[2].y+1]='+';
                    }
                    else if(tanks[2].orientation==DOWN)
                    {
                        if(Map_Play[0][tanks[2].x+1][tanks[2].y]==' ')
                            Map_Play[1][tanks[2].x+1][tanks[2].y]='+';
                    }
                    else if(tanks[2].orientation==LEFT)
                    {
                        if(Map_Play[0][tanks[2].x][tanks[2].y-1]==' ')
                            Map_Play[1][tanks[2].x][tanks[2].y-1]='+';
                    }
                }

                if(tanks[3].alive)
                {
                    if(tanks[3].orientation==UP)
                    {
                        if(Map_Play[0][tanks[3].x-1][tanks[3].y]==' ')
                            Map_Play[1][tanks[3].x-1][tanks[3].y]='+';
                    }
                    else if(tanks[3].orientation==RIGHT)
                    {
                        if(Map_Play[0][tanks[3].x][tanks[3].y+1]==' ')
                            Map_Play[1][tanks[3].x][tanks[3].y+1]='+';
                    }
                    else if(tanks[3].orientation==DOWN)
                    {
                        if(Map_Play[0][tanks[3].x+1][tanks[3].y]==' ')
                            Map_Play[1][tanks[3].x+1][tanks[3].y]='+';
                    }
                    else if(tanks[3].orientation==LEFT)
                    {
                        if(Map_Play[0][tanks[3].x][tanks[3].y-1]==' ')
                            Map_Play[1][tanks[3].x][tanks[3].y-1]='+';
                    }
                }

}


void FromHPToString(int hp_amount,char hp_value[3])
{
    hp_value[0]=hp_amount/100+int('0');
    hp_value[1]=hp_amount/10%10+int('0');
    hp_value[2]=hp_amount%10+int('0');
    hp_value[3]=NULL;
}

void PrintMap()
{

        char hp_value[3];
        FromHPToString(tanks[0].hp,hp_value);
        Map_Play[1][0][2]=hp_value[0];
        Map_Play[1][0][3]=hp_value[1];
        Map_Play[1][0][4]=hp_value[2];
        Map_Play[1][0][5]='H';
        Map_Play[1][0][6]='P';
        if(nuke)
            Map_Play[1][0][9]='N';
        if(OneShootKill)
            Map_Play[1][0][10]='O';
        OrientationPrint();
        system("CLS");
        for(int i=0;i<21;i++)
        {
            for(int j=0;j<30;j++)
                cout<<Map_Play[1][i][j];
            cout<<"\n";
        }

}



void Prepare_PlayGround()
{
    for(int i=0;i<21;i++)
        for(int j=0;j<30;j++)
            {
                Map_Play[0][i][j]=Map_Play[1][i][j]=Map[play_map][i][j];
                if(Map_Play[0][i][j]=='@')
                {

                    tanks[0].x=i;
                    tanks[0].y=j;
                    tanks[0].orientation=rand()%4+1;
                    tanks[0].alive=true;
                    if(tanks[0].type==0)
                    {
                        tanks[0].hp=80;
                        tanks[0].dmg=25;
                    }
                    else if(tanks[0].type==1)
                    {
                        tanks[0].hp=120;
                        tanks[0].dmg=20;
                    }
                    else if(tanks[0].type==2)
                    {
                        tanks[0].hp=150;
                        tanks[0].dmg=15;
                    }
                    else if(tanks[0].type==3)
                    {
                        tanks[0].hp=60;
                        tanks[0].dmg=40;
                    }
                    else if(tanks[0].type==4)
                    {
                        tanks[0].hp=60;
                        tanks[0].dmg=20;
                    }

                }
                else if(Map_Play[0][i][j]=='1')
                {
                        tanks[1].dmg=20;
                        tanks[1].hp=80;
                        tanks[1].x=i;
                        tanks[1].y=j;
                        tanks[1].type=5;
                        tanks[1].orientation=rand()%4+1;
                        tanks[1].alive=true;

                }
                else if(Map_Play[0][i][j]=='2')
                {
                        tanks[2].dmg=20;
                        tanks[2].hp=80;
                        tanks[2].x=i;
                        tanks[2].y=j;
                        tanks[2].type=5;
                        tanks[2].orientation=rand()%4+1;
                        tanks[2].alive=true;

                }
                else if(Map_Play[0][i][j]=='3')
                {
                        tanks[3].dmg=20;
                        tanks[3].hp=80;
                        tanks[3].x=i;
                        tanks[3].y=j;
                        tanks[3].type=5;
                        tanks[3].orientation=rand()%4+1;
                        tanks[3].alive=true;

                }

            }

}

void CopyGameMap()
{
    for(int i=0;i<21;i++)
        for(int j=0;j<30;j++)
            Map_Play[1][i][j]=Map_Play[0][i][j];


}



void GameOn()
{
    Prepare_PlayGround();
    char input;
    PrintMap();
    while(in_game)
    {
        PrintMap();
        cin>>input;
        ProcessInput(input);
        CopyGameMap();
        if(tanks[0].hp<=0||((!tanks[1].alive)&&(!tanks[1].alive)&&(!tanks[1].alive)))
            {
                in_game=false;
                game_over=true;
                if(tanks[0].hp>0)
                    GameOverWin();
                else
                    GameOverLose();
            }
    }

}

void LoadResurces()
{
    ifstream MenuGetNull("res/Menu0.txt");
    ifstream MenuGetOne("res/Menu1.txt");
    ifstream MenuGetTwo("res/Menu2.txt");
    ifstream MenuGetThree("res/Menu3.txt");
    ifstream MenuGetFour("res/Menu4.txt");
    ifstream MenuGetFive("res/Menu5.txt");
    ifstream MenuGetSix("res/Menu6.txt");
    ifstream MenuGetSeven("res/Menu7.txt");
    ifstream MenuGetEight("res/Menu8.txt");
    ifstream MenuGetNine("res/Menu9.txt");
    ifstream MenuGetTen("res/Menu10.txt");
    ifstream MenuGetOneOne("res/Menu11.txt");
    ifstream MenuGetOneTwo("res/Menu12.txt");
    ifstream MenuGetOneThree("res/Menu13.txt");
    ifstream MenuGetOneFour("res/Menu14.txt");
    ifstream MenuGetOneFive("res/Menu15.txt");

    ifstream MapGetNull("res/Map0.txt");
    ifstream MapGetOne("res/Map1.txt");
    ifstream MapGetTwo("res/Map2.txt");

    for(int i=0;i<21;i++)
        for(int j=0;j<30;j++)
            {
                MenuGetNull>>Menu[0][i][j];if(Menu[0][i][j]=='.')Menu[0][i][j]=' ';
                MenuGetOne>>Menu[1][i][j];if(Menu[1][i][j]=='.')Menu[1][i][j]=' ';
                MenuGetTwo>>Menu[2][i][j];if(Menu[2][i][j]=='.')Menu[2][i][j]=' ';
                MenuGetThree>>Menu[3][i][j];if(Menu[3][i][j]=='.')Menu[3][i][j]=' ';
                MenuGetFour>>Menu[4][i][j];if(Menu[4][i][j]=='.')Menu[4][i][j]=' ';
                MenuGetFive>>Menu[5][i][j];if(Menu[5][i][j]=='.')Menu[5][i][j]=' ';
                MenuGetSix>>Menu[6][i][j];if(Menu[6][i][j]=='.')Menu[6][i][j]=' ';
                MenuGetSeven>>Menu[7][i][j];if(Menu[7][i][j]=='.')Menu[7][i][j]=' ';
                MenuGetEight>>Menu[8][i][j];if(Menu[8][i][j]=='.')Menu[8][i][j]=' ';
                MenuGetNine>>Menu[9][i][j];if(Menu[9][i][j]=='.')Menu[9][i][j]=' ';
                MenuGetTen>>Menu[10][i][j];if(Menu[10][i][j]=='.')Menu[10][i][j]=' ';
                MenuGetOneOne>>Menu[11][i][j];if(Menu[11][i][j]=='.')Menu[11][i][j]=' ';
                MenuGetOneTwo>>Menu[12][i][j];if(Menu[12][i][j]=='.')Menu[12][i][j]=' ';
                MenuGetOneThree>>Menu[13][i][j];if(Menu[13][i][j]=='.')Menu[13][i][j]=' ';
                MenuGetOneFour>>Menu[14][i][j];if(Menu[14][i][j]=='.')Menu[14][i][j]=' ';
                MenuGetOneFive>>Menu[15][i][j];if(Menu[15][i][j]=='.')Menu[15][i][j]=' ';

                MapGetNull>>Map[0][i][j];if(Map[0][i][j]=='.')Map[0][i][j]=' ';
                MapGetOne>>Map[1][i][j];if(Map[1][i][j]=='.')Map[1][i][j]=' ';
                MapGetTwo>>Map[2][i][j];if(Map[2][i][j]=='.')Map[2][i][j]=' ';
            }

}





void PrintMenu(int NumberMenu)
{
        system("CLS");
        for(int i=0;i<21;i++)
        {
            for(int j=0;j<30;j++)
                cout<<Menu[NumberMenu][i][j];
            cout<<"\n";
        }

}

void GameOverWin()
{
    char input;
    int position=0;
    PrintMenu(position+12);
    while(game_over)
    {
        PrintMenu(position+12);
        cin>>input;
        if(input=='q')
        {
            game_over=false;
            if(position==0)
            {
                in_menu=true;
                LoadMenu();
            }
        }
        else if(input=='w')
        {
            if(position==0)
                position++;
            else
                position--;
        }
        else if(input=='s')
        {
            if(position==1)
                position--;
            else
                position++;
        }
    }
}


void GameOverLose()
{
    char input;
    int position=0;
    PrintMenu(position+14);
    while(game_over)
    {
        PrintMenu(position+14);
        cin>>input;
        if(input=='q')
        {
            game_over=false;
            if(position==0)
            {
                in_menu=true;
                LoadMenu();
            }
        }
        else if(input=='w')
        {
            if(position==0)
                position++;
            else
                position--;
        }
        else if(input=='s')
        {
            if(position==1)
                position--;
            else
                position++;
        }
    }
}


void SelectTank()
{
    char input;
    int position=0;
    PrintMenu(position+6);
    while(in_menu)
    {
        PrintMenu(position+6);
        cin>>input;
        if(input=='q')
        {
            if(position==5)
            {
                break;
                NewGame();
            }
            else
            {
                tanks[0].type=position;
                srand(time(NULL));
                play_map=rand() % 3 + 1;
                in_game=true;
                in_menu=false;
                GameOn();
            }
        }
        else if(input=='a')
        {
            if(position==0)
            {
                position=5;
            }
            else
            {
                position--;
            }
        }
        else if(input=='d')
        {
            if(position==5)
            {
                position=0;
            }
            else
            {
                position++;
            }
        }



    }
}

void NewGame()
{
    char input;
    int position=0;
    PrintMenu(position+2);
    while(in_menu)
    {

        PrintMenu(position+2);
        cin>>input;
        if(input=='q')
        {

            if(position==3)
            {
                LoadMenu();
            }
            else
            {

                difficulty=position;
                SelectTank();
            }

        }
        else if(input=='s')
        {
            if(position==3)
                position=0;
            else position++;
        }
        else if(input=='w')
        {
            if(position==0)
                position=3;
            else position--;
        }

    }

}


void LoadMenu()
{

        char input;
        int position=0;
        PrintMenu(0);
        while(in_menu)
        {
            cin>>input;
            if(input=='q')
            {
                if(position==0)
                    NewGame();
                else
                    in_menu=false;
            }
            else if(input=='s')
            {
                if(position==1)
                    position--;
                else
                    position++;
            }
            else if(input=='w')
            {
                if(position==0)
                    position++;
                else
                    position--;
            }
            PrintMenu(position);
        }
}

int main()
{
    LoadResurces();
    while(in_menu)
        LoadMenu();
    return 0;



}
