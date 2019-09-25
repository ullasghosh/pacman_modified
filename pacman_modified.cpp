#include<iostream>
#include<conio.h>
#include<fstream>
#define playert 10
#define enemyt 80

using namespace std;

char s[101][101],playerprev,enemyprev[10000][1];
int w,h,val[101][101],xstart,ystart,xgoal,ygoal,enemycordxy[10000][3],botdir,playercord[3],nextenemypoint[3],level=-1,countdot=0,useddot,dot[1001][1001]={};

void left(int,int);
void right(int,int);
void up(int,int);
void down(int,int);
void initial(int,int);
void getmaze();
void enemycord(int);
void bringenemy(int);
void moveplayer(char);
void enemyapproachplayer(int);
void getroute(int,int,int,int);

void left(int x, int y)
{
if(y-1>=0 && val[x][y-1]>=0)
          if(val[x][y-1]==0)
          {
                            val[x][y-1]=val[x][y]+1;
                            up(x,y-1);
                            down(x,y-1);
                            left(x,y-1);
              }
          else if(val[x][y]+1>=val[x][y-1])
                                         return;
          else
          {
              val[x][y-1]=val[x][y]+1;
              up(x,y-1);
              down(x,y-1);
              left(x,y-1);
              }
else
return;
}

void right(int x, int y)
{
if(y+1<w && val[x][y+1]>=0)
          if(val[x][y+1]==0)
          {
                            val[x][y+1]=val[x][y]+1;
                            up(x,y+1);
                            down(x,y+1);
                            right(x,y+1);
              }
          else if(val[x][y]+1>=val[x][y+1])
                                         return;
          else
          {
              val[x][y+1]=val[x][y]+1;
              up(x,y+1);
              down(x,y+1);
              right(x,y+1);
              }
else
return;
}

void up(int x, int y)
{
if(x-1>=0 && val[x-1][y]>=0)
          if(val[x-1][y]==0)
          {
                            val[x-1][y]=val[x][y]+1;
                            up(x-1,y);
                            right(x-1,y);
                            left(x-1,y);
              }
          else if(val[x][y]+1>=val[x-1][y])
                                         return;
          else
          {
              val[x-1][y]=val[x][y]+1;
              up(x-1,y);
              right(x-1,y);
              left(x-1,y);
              }
else
return;
}

void down(int x, int y)
{
if(x+1<h && val[x+1][y]>=0)
          if(val[x+1][y]==0)
          {
                            val[x+1][y]=val[x][y]+1;
                            down(x+1,y);
                            right(x+1,y);
                            left(x+1,y);
              }
          else if(val[x][y]+1>=val[x+1][y])
                                         return;
          else
          {
              val[x+1][y]=val[x][y]+1;
              down(x+1,y);
              right(x+1,y);
              left(x+1,y);
              }
else
return;
}

void initial(int x, int y)
{
            up(x,y);
            val[x][y]=0;
            down(x,y);
            val[x][y]=0;
            right(x,y);
            val[x][y]=0;
            left(x,y);
            val[x][y]=0;
}

void getmaze()
{
     int i,j;
     ifstream fin;
     fin.open("pacman_modified_txt.txt");
     fin>>w;
     fin>>h;
     char empty[101];
     fin.getline(empty,w+1);
     for(i=0;i<h;i++)
     fin.getline(s[i],w+1);
     fin.close();
    for(i=0;i<h;i++)
    for(j=0;j<w;j++)
    {
                    if(s[i][j]=='X')
                                    val[i][j]=-1;
                    if(s[i][j]=='.' || s[i][j]==' ')
                    {
                                    val[i][j]=0;
                                    countdot++;
                                    dot[countdot][1]=i;
                                    dot[countdot][2]=j;
                    }

                    if(s[i][j]=='S')
                    {
                                    val[i][j]=0;
                                    xstart=i;
                                    ystart=j;
                    }
                    if(s[i][j]=='G')
                    {
                                    val[i][j]=0;
                                    xgoal=i;
                                    ygoal=j;
                    }
    }
    for(i=0;i<h+1;i++)
    cout<<s[i]<<endl;
}

void enemycord(int k)       ///generates enemy's starting point, used by bringenemy
{
int rnum,i,j,dist,l,m,n;
srand (k*time(NULL));
rnum=rand()%useddot+1;
i=dot[rnum][1];
j=dot[rnum][2];
DOIT:
for(n=1;n<=k-1;n++)
        if(enemycordxy[n][1]==i && enemycordxy[n][2]==j)
        {
            srand (k*(i+1)*n*time(NULL));
            rnum=rand()%useddot+1;
            i=dot[rnum][1];
            j=dot[rnum][2];
            goto DOIT;
        }

    enemycordxy[k][1]=i;
    enemycordxy[k][2]=j;
    initial(enemycordxy[k][1],enemycordxy[k][2]);
    dist=val[xstart][ystart];
    for(l=0;l<h;l++)
    for(m=0;m<w;m++)
    if(val[l][m]!=-1)
    val[l][m]=0;
    if(dist>10)
    {
        dot[rnum][1]=dot[useddot][1];
        dot[rnum][2]=dot[useddot][2];
        useddot--;
        return;
    }
    else
    {
        dot[rnum][1]=dot[useddot][1];
        dot[rnum][2]=dot[useddot][2];
        useddot--;
        enemycord(k);
    }
}

void bringenemy(int k)
{
int i,j;
enemycord(k);
i=enemycordxy[k][1];
j=enemycordxy[k][2];
enemyprev[k][0]=s[i][j];
s[i][j]='@';
}

void moveplayer(char c)
{
    int flag,xprev,yprev,xpos,ypos;
    flag=0;
    xprev=playercord[1];
    yprev=playercord[2];
    xpos=playercord[1];
    ypos=playercord[2];
    if(c==75)
    {
        ypos=ypos-1;
        if(val[xpos][ypos]!=0 || ypos<0)
            ypos++;
        else
            flag=1;
    }
    if(c==77)
    {
        ypos=ypos+1;
        if(val[xpos][ypos]!=0 || ypos>=w)
            ypos--;
        else
            flag=1;
    }
    if(c==72)
    {
        xpos=xpos-1;
        if(val[xpos][ypos]!=0 || xpos<0)
            xpos++;
        else
            flag=1;
    }
    if(c==80)
    {
        xpos=xpos+1;
        if(val[xpos][ypos]!=0 || xpos>=h)
            xpos--;
        else
            flag=1;
    }
    if(flag==1)                 ///player has moved
    {
        s[xprev][yprev]=playerprev;
        playerprev=s[xpos][ypos];
    }
    s[xpos][ypos]='O';
    playercord[1]=xpos;
    playercord[2]=ypos;
}

void enemyapproachplayer(int k)
{
    int i,j,l,xprev,yprev;
    xprev=enemycordxy[k][1];
    yprev=enemycordxy[k][2];
    initial(enemycordxy[k][1],enemycordxy[k][2]);
    getroute(enemycordxy[k][1],enemycordxy[k][2],playercord[1],playercord[2]);
    if(s[nextenemypoint[1]][nextenemypoint[2]]=='@')
    {
         for(i=0;i<h;i++)
         for(j=0;j<w;j++)
         if(val[i][j]!=-1)
            val[i][j]=0;
        return;
    }
    //s[enemycordxy[k][1]][enemycordxy[k][2]]=' ';
    enemycordxy[k][1]=nextenemypoint[1];
    enemycordxy[k][2]=nextenemypoint[2];
    if(xprev!=enemycordxy[k][1] || yprev!=enemycordxy[k][2])
        s[xprev][yprev]=enemyprev[k][0];
    enemyprev[k][0]=s[enemycordxy[k][1]][enemycordxy[k][2]];
    s[enemycordxy[k][1]][enemycordxy[k][2]]='@';
    for(i=0;i<h;i++)
    for(j=0;j<w;j++)
    if(val[i][j]!=-1)
    val[i][j]=0;
}

void getroute(int x1,int y1,int x2, int y2)
{
    int dist,k;
     dist=val[x2][y2];
     for(k=1;k<dist;k++)
    {
                    if(val[x2-1][y2]==val[x2][y2]-1)
                    x2=x2-1;
                    else if(val[x2+1][y2]==val[x2][y2]-1)
                    x2=x2+1;
                    else if(val[x2][y2-1]==val[x2][y2]-1)
                    y2=y2-1;
                    else
                    y2=y2+1;
    }
    nextenemypoint[2]=y2;
    nextenemypoint[1]=x2;
}
int main()
{
RESTART:
playerprev=' ';
level=-1;
countdot=0;
int i,j,score=0,xpos,ypos,xprev,yprev,flag,count=1,tough;
long double t_player,t_enemy;
char c='z',a='z';
getmaze();
useddot=countdot;
cout<<"\nObjective: Reach from 'S' to 'G' avoiding enemies '@'\n\nDirection control by arrow keys\n\n'X' are barriers\n\n";
while(level<0)
{
cout<<"Enter number of enemies: ";
cin>>level;
if(level>countdot/4)
    {
        cout<<"Number enemies too large...\n";
        level=-1;
    }
else if(level<0)
        cout<<"Enter non-negative number...\n";
}
while(a!='s' && a!='m' && a!='f')
{
cout<<"Press speed of enemies ('s'-slow,'m'-moderate,'f'-fast): ";
while(a!='s' && a!='m' && a!='f')
a=_getch();
cout<<a;
}

switch(a)
{
case 's':
    tough=5;
    break;
case 'm':
    tough=3;
    break;
case 'f':
    tough=2;
    break;
}

cout<<"\nTo play press any direction key";
while(c!=72 && c!=75 && c!=77 && c!=80)
c=_getch();
system("cls");
s[xstart][ystart]='O';
playercord[1]=xstart;
playercord[2]=ystart;

for(i=1;i<=level;i++)
{
    bringenemy(i);
    system("cls");
    cout<<"Loading..."<<i*100/level<<"%";
}

system("cls");
for(i=0;i<h+1;i++)
        puts(s[i]);

t_player=clock();
t_enemy=t_player;
while(score==0)
{
    if(clock()-t_player>=playert)
    {
        t_player=clock();
        if(kbhit())
        {
            c=_getch();
            moveplayer(c);///move one step if c not equal to NULL
            system("cls");
            for(i=0;i<h+1;i++)
                puts(s[i]);
        }
    }

    if(playercord[1]==xgoal && playercord[2]==ygoal)
    {
            score=1;
            break;
    }
    for(i=1;i<=level;i++)
    if(playercord[1]==enemycordxy[i][1] && playercord[2]==enemycordxy[i][2])
    {
            score=-1;
            break;
    }

if(clock()-t_enemy>=enemyt*tough)
{
    t_enemy=clock();
    for(i=1;i<=level;i++)
    {
        enemyapproachplayer(i);
        if(playercord[1]==enemycordxy[i][1] && playercord[2]==enemycordxy[i][2])
            score=-1;
    }
    system("cls");
            for(i=0;i<h+1;i++)
                puts(s[i]);
}
}
if(score==1)
    cout<<"\n\nYou won!\n";
else
    cout<<"\n\nYou lose!\n";
cout<<"\nTo play again press 'r', to quit press 'q', press 'c' for credit";
AGAIN:
while(c!='r' && c!='q' && c!='c')
c=_getch();
if(c=='r')
{
    system("cls");
    goto RESTART;
}
if(c=='c')
{
    system("cls");
    cout<<"\n\nDeveloped by Ullas\n\n\n";
    cout<<"\nTo play again press 'r', to quit press 'q'";
    c='z';
    goto AGAIN;
}
}
