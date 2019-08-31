
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
							main.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include "type.h"
#include "stdio.h"
#include "const.h"
#include "protect.h"
#include "string.h"
#include "fs.h"
#include "proc.h"
#include "tty.h"
#include "console.h"
#include "global.h"
#include "proto.h"

#include "time.h"
#include "termio.h"
#include "bits/signum.h"
#include "signal.h"
#include "sys/time.h"

#define DELAY_TIME 6000		//延迟时间
#define NULL ((void*)0)

// my code here

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                         push box
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

int pi=0;
int pj=0;
void draw_map(int map[9][11])
{
    int i;
    int j;   
    for(i=0;i<9;i++)
    {
        for(int j=0;j<11;j++)
        {
            switch(map[i][j])
            {
                case 0:
                printf(" "); //道路
                break;
                case 1:
                printf("#"); //墙壁
                break;
                case 2:
                printf(" "); //游戏边框的空白部分
                break;
                case 3:
                printf("D"); //目的地
                break;
                case 4:
                printf("b"); //箱子
                break;
                case 7:
                printf("!"); //箱子进入目的地
                break;
                case 6:
                printf("p"); //人
                break;
                case 9:
                printf("^"); //人进入目的地
                break;
            }            
        }
        printf("\n");
    }
}
//void draw_map(int map[9][11]); 
//void boxMenu();
void boxMenu()
{
	printf("      ====================================================================\n");
	printf("      #         *******                         Welcome to               #\n");
	printf("      #            **                            cherryOS                #\n");
	printf("      #        ***  **                                                   #\n");
	printf("      #     ******   **                                                  #\n");
	printf("      #   ******      **                        Instruction              #\n");
	printf("      #   ****      **  **                set: p:People b:BOX            #\n");
	printf("      #            **    **                    #:Wall   D:Destination    #\n");
	printf("      #        ******     ****            operation:                     #\n");
	printf("      #      **********  ******                s:Down d:Right            #\n");
	printf("      #      **********   ****                 w:Up   a:Left  q:Quit     #\n");
	printf("      #        ******                                                    #\n");
	printf("      #                                   PS:you can enter'q' to quit    #\n");
	printf("      #                                      anytime you like.           #\n");
	printf("      #                                                                  #\n");
	printf("      #               Powered by doubleZ, budi, flyingfish               #\n");
	printf("      #                       ALL RIGHT REVERSED                         #\n");
	printf("      ====================================================================\n");
}
void Runpushbox(fd_stdin, fd_stdout)
{
    char rdbuf[128];
    int r;
    char control;
    
    int count=0;   //定义记分变量
 
    int map[9][11] = {
        {2,1,1,1,1,1,1,1,1,1,2},
        {2,1,0,0,0,1,0,0,0,1,2},
        {2,1,0,4,4,4,4,4,0,1,2},
        {2,1,0,4,0,4,0,4,0,0,1},
        {2,1,0,0,0,6,0,0,4,0,1},
        {1,1,0,1,1,1,1,0,4,0,1},
        {1,0,3,3,3,3,3,1,0,0,1},
        {1,0,3,3,3,3,3,0,0,0,1},
        {1,1,1,1,1,1,1,1,1,1,2},
    };
    while (1)  
    {
        clear();
        printf("\n");
		boxMenu();
        draw_map(map);
        printf("Current Score:%d\n",count);
        //找初始位置
        for (pi=0;pi<9;pi++)
        {
            for (pj=0;pj<11;pj++)
            {
                if (map[pi][pj]==6||map[pi][pj]==9)
                break;
            }
            if (map[pi][pj]==6||map[pi][pj]==9)
            break;
        }
        printf("CURRENT LOCATION (%d,%d)",pi,pj);
        
        printf("\n");
        printf("Please input direction:");
        
        r = read(fd_stdin, rdbuf, 70);
        rdbuf[r] = 0;
        control=rdbuf[0];
        
        
        if(control == 'Q' || control == 'q')
        {
            break;
        }
        switch (control)
        {
            case 'w':
            //如果人前面是空地。  
            if(map[pi-1][pj]==0)
            {
                map[pi-1][pj]=6+0; 
                if(map[pi][pj]==9) 
                	map[pi][pj]=3; 
                else
                	map[pi][pj]=0;    
            }
            //如果人前面是目的地。
            else if((map[pi-1][pj]==3)||(map[pi-1][pj]==9))
            {
                map[pi-1][pj]=6+3; 
                if(map[pi][pj]==9) 
                	map[pi][pj]=3; 
                else
                	map[pi][pj]=0; 
            }
            //如果人前面是箱子。
            else if(map[pi-1][pj]==4)
            {
                if (map[pi-2][pj]==0)
                {
                    map[pi-2][pj]=4; 
                    if(map[pi-1][pj]==7) 
                    	map[pi-1][pj]=9; 
                    else
                    	map[pi-1][pj]=6; 
                    if(map[pi][pj]==9) 
                    	map[pi][pj]=3; 
                    else
                    	map[pi][pj]=0; 
                }
                //如果人的前面是箱子，而箱子前面是目的地。
                else if (map[pi-2][pj]==3)
                {
                    map[pi-2][pj]=7;
                    count++;
                    if(map[pi-1][pj]==7)
                    	map[pi-1][pj]=9;
                    else
                    	map[pi-1][pj]=6;
                    if(map[pi][pj]==9)
                    	map[pi][pj]=3;
                    else
                    	map[pi][pj]=0;
                }
            }
            //如果人前面是已经进入某目的地的箱子
            else if(map[pi-1][pj]==7)
            {
                //如果人前面是已经进入某目的地的箱子,箱子前面是空地。
                if(map[pi-2][pj]==0)
                {
                    count--;
                    map[pi-2][pj]=4;   
                    map[pi-1][pj]=9;   
                    if(map[pi][pj]==9)
                    	map[pi][pj]=3;
                    else
                    	map[pi][pj]=0;
                }
                //如果人前面是已经进入某目的地的箱子，箱子前面是另一目的地。
                if(map[pi-2][pj]==3)
                {
                    map[pi-2][pj]=7;   
                    map[pi-1][pj]=9;   
                    if(map[pi][pj]==9)
                    	map[pi][pj]=3;
                    else
                    	map[pi][pj]=0;
                }
            }
            break;
            case 's':
            //如果人前面是空地。
            if(map[pi+1][pj]==0)
            {
                map[pi+1][pj]=6+0;
                if(map[pi][pj]==9) 
                	map[pi][pj]=3; 
                else
                	map[pi][pj]=0;    
            }
            //如果人前面是目的地。
            else if(map[pi+1][pj]==3)
            {
                map[pi+1][pj]=6+3; 
                if(map[pi][pj]==9) 
                	map[pi][pj]=3;
                else
                	map[pi][pj]=0; 
            }
            //如果人前面是箱子。
            else if(map[pi+1][pj]==4)
            {
                //如果人前面是箱子，而箱子前面是空地。
                if (map[pi+2][pj]==0)
                {
                    map[pi+2][pj]=4; 
                    if(map[pi+1][pj]==7) 
                    	map[pi+1][pj]=9; 
                    else
                    	map[pi+1][pj]=6;
                    if(map[pi][pj]==9) 
                    	map[pi][pj]=3; 
                    else
                    	map[pi][pj]=0; 
                }
                //如果人的前面是箱子，而箱子前面是目的地。
                else if (map[pi+2][pj]==3)
                {
                    map[pi+2][pj]=7;
                    count++;
                    if(map[pi+1][pj]==7)
                    	map[pi+1][pj]=9;
                    else
                    	map[pi+1][pj]=6;
                    if(map[pi][pj]==9)
                    	map[pi][pj]=3;
                    else
                    	map[pi][pj]=0;
                }
            }
            else if(map[pi+1][pj]==7)
            {
                if(map[pi+2][pj]==0)
                {
                    count--;
                    map[pi+2][pj]=4;  
                    map[pi+1][pj]=9;  
                    if(map[pi][pj]==9)
                    	map[pi][pj]=3;
                    else
                    	map[pi][pj]=0;
                }
                if(map[pi+2][pj]==3)
                {
                    map[pi+2][pj]=7; 
                    map[pi+1][pj]=9;   
                    if(map[pi][pj]==9)
                    	map[pi][pj]=3;
                    else
                    	map[pi][pj]=0;
                }
            }
            break;
            case 'a':
            if(map[pi][pj-1]==0)
            {
                map[pi][pj-1]=6+0; 
                if(map[pi][pj]==9) 
                	map[pi][pj]=3; 
                else
                	map[pi][pj]=0;    
            }
            else if(map[pi][pj-1]==3)
            {
                map[pi][pj-1]=6+3;
                if(map[pi][pj]==9) 
                	map[pi][pj]=3;
                else
                	map[pi][pj]=0; 
            }
            else if(map[pi][pj-1]==4)
            {
                if (map[pi][pj-2]==0)
                {
                    map[pi][pj-2]=4; 
                    if(map[pi][pj-1]==7) 
                    	map[pi][pj-1]=9; 
                    else
                    	map[pi][pj-1]=6; 
                    if(map[pi][pj]==9) 
                    	map[pi][pj]=3; 
                    else
                    	map[pi][pj]=0; 
                }
                else if (map[pi][pj-2]==3)
                {
                    count++;
                    map[pi][pj-2]=7;  
                    if(map[pi][pj-1]==7)
                    	map[pi][pj-1]=9;
                    else
                    	map[pi][pj-1]=6;
                    if(map[pi][pj]==9)
                    	map[pi][pj]=3;
                    else
                    	map[pi][pj]=0;
                }
            }
            else if(map[pi][pj-1]==7)
            {
                if(map[pi][pj-2]==0)
                {
                    count--;
                    map[pi][pj-2]=4;   
                    map[pi][pj-1]=9;
                    if(map[pi][pj]==9) 
                    	map[pi][pj]=3;
                    else
                    	map[pi][pj]=0;
                }
                if(map[pi][pj-2]==3)
                {
                    map[pi][pj-2]=7;   
                    map[pi][pj-1]=9;
                    if(map[pi][pj]==9) 
                    	map[pi][pj]=3;
                    else
                    	map[pi][pj]=0;
                }
            }
            break;
            case 'd':
            if(map[pi][pj+1]==0)
            {
                map[pi][pj+1]=6+0; 
                if(map[pi][pj]==9) 
                	map[pi][pj]=3; 
                else
                	map[pi][pj]=0;          
            }
            else if(map[pi][pj+1]==3)
            {
                map[pi][pj+1]=6+3; 
                if(map[pi][pj]==9) 
                	map[pi][pj]=3; 
                else
                	map[pi][pj]=0; 
            }
            else if(map[pi][pj+1]==4)
            {
                if (map[pi][pj+2]==0)
                { 
                    map[pi][pj+2]=4; 
                    if(map[pi][pj+1]==7) 
                    	map[pi][pj+1]=9; 
                    else
                    	map[pi][pj+1]=6; 
                    if(map[pi][pj]==9) 
                    	map[pi][pj]=3; 
                    else
                    	map[pi][pj]=0; 
                }
                else if (map[pi][pj+2]==3)
                { 
                    count++;
                    map[pi][pj+2]=7;   
                    if(map[pi][pj+1]==7)
                    	map[pi][pj+1]=9;
                    else
                    	map[pi][pj+1]=6;
                    if(map[pi][pj]==9)
                    	map[pi][pj]=3;
                    else
                    	map[pi][pj]=0;
                }
            }
            else if(map[pi][pj+1]==7)
            {
                if(map[pi][pj+2]==0)
                {
                    count--;
                    map[pi][pj+2]=4;   
                    map[pi][pj+1]=9;  
                    if(map[pi][pj]==9) 
                    	map[pi][pj]=3;
                    else
                    	map[pi][pj]=0;
                }
                if(map[pi][pj+2]==3)
                {
                    map[pi][pj+2]=7;  
                    map[pi][pj+1]=9;   
                    if(map[pi][pj]==9) 
                    	map[pi][pj]=3;
                    else
                    	map[pi][pj]=0;
                }
            }
            break;
        }
        if(count==8)   
        {
            draw_map(map); 
            printf("\nCongratulations!!\n"); 
            break;    //退出死循环
        }
    }
}



/*****************************************************************************
 *                               five chess
 *****************************************************************************/
#define WIDTH 15
#define HIGHT 15
#define WHITE 0
#define BLACK 1
#define WIN 2

char WHITE_FLAG='O';//大写o
char BLACK_FLAG='X';

char map[WIDTH][HIGHT];
int situationPC[WIDTH][HIGHT]={0};  //situation
int situationPlayer[WIDTH][HIGHT]={0};

int player = BLACK;

void init_map();
void draw_pc_map();
void draw_player_map();
void draw_map_chess();
int horizontal(int row, int col, char whoFlag);
int vertical(int row, int col, char whoFlag);
int leftSlope(int row, int col, char whoFlag);
int rightSlope(int row, int col, char whoFlag);
int result( int left, int right, int count, int k, char num );
void pcLoad();
void benefit();
int playerLoad(fd_stdin);
int win();


int fiveChess(fd_stdin) {
    int result;
    int nums;
    init_map();

    draw_map_chess();

    for (nums = 0; nums < 225; nums++) {
        if (player == BLACK) {
            //玩家下子
            printf("please white(%c) play\n",WHITE_FLAG);
            player = WHITE;
            result = playerLoad(fd_stdin);
            if( !result ) break;
        } else {
            //电脑下子
            player = BLACK;
            pcLoad();
        }

        draw_map_chess();
        if ( win() ) {
            if (player == WHITE) {
                printf("WHITE(%c) is winner.\n",WHITE_FLAG);
            } else {
                printf("BLACK(%c) is winner.\n",BLACK_FLAG);
            }
            printf("Enter anything to exit...\n");
            char rdbuf[128];
            int r = read(fd_stdin, rdbuf, 70);
            rdbuf[r]=0;
            while(r<1)
            {
                r = read(fd_stdin, rdbuf, 70);
                rdbuf[r]=0;
            }

            break;
        }

    }

    clear();
}

void init_map() {
    int i = 0;
    int j = 0;
    for (i = 0; i < HIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            map[i][j] = '+';
        }
    }
}

void draw_pc_map() {
    int i, j, k;

    for (i = 0; i < WIDTH; i++) {
        for (j = 0; j < WIDTH; j++) {
            printf("%d",situationPC[i][j]);
            if(j<(WIDTH-1)) printf("-");
        }
        printf("\n");
    }
}

void draw_player_map() {
    int i=0, j=0;

    for (i = 0; i < WIDTH; i++) {
        for (j = 0; j < WIDTH; j++) {
            printf("%d",situationPlayer[i][j]);
            if(j<(WIDTH-1)) printf("-");
        }
        printf("\n");
    }
}

void draw_map_chess() {
    int i=0, j=0, k=0;

    clear();

    printf("           Five Chess Game       \n");
    printf("You can quit anytime by entering [q]\n");
    printf("    X--------------------------->\n");
    printf("    ");
    for (k = 0; k < HIGHT; k++) {
        if( k>=0 && k<=8){
            printf("%d ",k+1);
        }else{
            printf("%d",k+1);
        }
    }
    printf("\n");
    for (i = 0; i < WIDTH; i++) {
        printf("Y");
        if( i>=0 && i<=8){
            printf("0%d ",i+1);
        }else{
            printf("%d ",i+1);
        }
        for (j = 0; j < WIDTH; j++) {
            printf("%c", map[i][j]);
            if(j<(WIDTH-1)) printf("-");
        }
        printf("\n");
    }
}

int horizontal(int row, int col, char whoFlag){
    int spaceNum=0;//空白数
    int count =1;//几连，包含当前要下的子
    int leftHad=0;//左边是否有同子
    int x=row;
    int y=col;
    int liveLeft = 0;
    int liveRight=0;

    if( map[row][col] != '+' ){
        return 0;
    }
    while( y>0 && ( map[x][y-1]=='+' || map[x][y-1]==whoFlag ) ){
        if( map[x][y-1]=='+' && spaceNum<1 ){//第一个空白
            if( map[x][y-2] != whoFlag ){
                liveLeft = 1;
                break;
            }
            spaceNum++;
            y--;
        }else if( map[x][y-1] == whoFlag ){
            leftHad=1;
            y--;
            count++;
        }else{//第2个空白
            liveLeft=1;
            break;
        }
    }

    //如果左边没有同色子，设置空白数为0
    if( !leftHad ){
        spaceNum=0;
    }

    y=col;
    while( y<14 && ( map[x][y+1]=='+' || map[x][y+1]==whoFlag ) ){
        if( map[x][y+1]=='+' && spaceNum<1){//第一个空白
            if( map[x][y+2] != whoFlag ){
                liveRight = 1;
                break;
            }
            spaceNum++;
            y++;
        }else if( map[x][y+1]=='+' && spaceNum>0 ){//第2个空白
            liveRight = 1;
            break;
        }else{
            y++;
            count++;
        }

    }
    return result( liveLeft,liveRight, count,spaceNum, whoFlag );
}

int vertical(int row, int col, char whoFlag){
    int spaceNum=0;//空白数
    int count =1;//几连，包含当前要下的子
    int topHad=0;//上边是否有同子
    int x=row;
    int y=col;
    int liveLeft = 0;
    int liveRight=0;

    if( map[row][col] != '+' ){
        return 0;
    }
    while( x>0 && ( map[x-1][y]=='+' || map[x-1][y]==whoFlag ) ){
        if( map[x-1][y]=='+' && spaceNum<1 ){//第一个空白
            if( map[x-2][y] != whoFlag ){
                liveLeft = 1;
                break;
            }
            spaceNum++;
            x--;
        }else if( map[x-1][y] == whoFlag ){
            topHad=1;
            x--;
            count++;
        }else{//第2个空白
            liveLeft=1;
            break;
        }
    }

    //如果左边没有同色子，设置空白数为0
    if( !topHad ){
        spaceNum=0;
    }
    x=row;
    while( x<14 && ( map[x+1][y]=='+' || map[x+1][y]==whoFlag ) ){
        if( map[x+1][y]=='+' && spaceNum<1){//第一个空白
            if( map[x+2][y] != whoFlag ){
                liveRight = 1;
                break;
            }
            spaceNum++;
            x++;
        }else if( map[x+1][y]=='+' && spaceNum>0 ){//第2个空白
            liveRight = 1;
            break;
        }else{
            x++;
            count++;
        }

    }
    return result( liveLeft,liveRight, count,spaceNum, whoFlag );
}

// +-+-+-@-+-+
// +-+-@-+-+
// +-@-+-+-+
int leftSlope(int row, int col, char whoFlag){
    int spaceNum=0;//空白数
    int count =1;//几连，包含当前要下的子
    int topHad=0;//上边是否有同子
    int x=row;
    int y=col;
    int liveLeft = 0;
    int liveRight=0;

    if( map[row][col] != '+' ){
        return 0;
    }
    //向下
    while( x<14 && y>0 && ( map[x+1][y-1]=='+' || map[x+1][y-1]==whoFlag ) ){
        if( map[x+1][y-1]=='+' && spaceNum<1 ){//第一个空白
            if( map[x+2][y-2] != whoFlag ){
                liveLeft = 1;
                break;
            }
            spaceNum++;
            x++;
            y--;
        }else if( map[x+1][y-1] == whoFlag ){
            topHad=1;
            x++;
            y--;
            count++;
        }else{//第2个空白
            liveLeft=1;
            break;
        }
    }

    //如果上边没有同色子，设置空白数为0
    if( !topHad ){
        spaceNum=0;
    }

    x=row;
    y=col;

    //向上
    while( x>0 && y<14 && ( map[x-1][y+1]=='+' || map[x-1][y+1]==whoFlag ) ){
        if( map[x-1][y+1]=='+' && spaceNum<1){//第一个空白
            if( map[x-2][y+2] != whoFlag ){
                liveRight = 1;
                break;
            }
            spaceNum++;
            x--;
            y++;
        }else if( map[x-1][y+1]=='+' && spaceNum>0 ){//第2个空白
            liveRight = 1;
            break;
        }else{
            x--;
            y++;
            count++;
        }

    }
    return result( liveLeft,liveRight, count,spaceNum, whoFlag );
}

int rightSlope(int row, int col, char whoFlag){
    int spaceNum=0;//空白数
    int count =1;//几连，包含当前要下的子
    int topHad=0;//上边是否有同子
    int x=row;
    int y=col;
    int liveLeft = 0;
    int liveRight=0;

    if( map[row][col] != '+' ){
        return 0;
    }
    //向上
    while( x>0 && y>0 && ( map[x-1][y-1]=='+' || map[x-1][y-1]==whoFlag ) ){
        if( map[x-1][y-1]=='+' && spaceNum<1 ){//第一个空白
            if( map[x-2][y-2] != whoFlag ){
                liveLeft = 1;
                break;
            }
            spaceNum++;
            x--;
            y--;
        }else if( map[x-1][y-1] == whoFlag ){
            topHad=1;
            x--;
            y--;
            count++;
        }else{//第2个空白
            liveLeft=1;
            break;
        }
    }

    //如果上边没有同色子，设置空白数为0
    if( !topHad ){
        spaceNum=0;
    }

    x=row;
    y=col;

    //向下
    while( x<14 && y<14 && ( map[x+1][y+1]=='+' || map[x+1][y+1]==whoFlag ) ){
        if( map[x+1][y+1]=='+' && spaceNum<1){//第一个空白
            if( map[x+2][y+2] != whoFlag ){
                liveRight = 1;
                break;
            }
            spaceNum++;
            x++;
            y++;
        }else if( map[x+1][y+1]=='+' && spaceNum>0 ){//第2个空白
            liveRight = 1;
            break;
        }else{
            x++;
            y++;
            count++;
        }

    }
    return result( liveLeft,liveRight, count,spaceNum, whoFlag );
}

int result( int left, int right, int count, int k, char num ){
    if( count ==1 ){
        return 1;
    }else if( count == 2 ){
        if( left && right ){//左右两边都是空的
            if( k==0 ){
                //电脑60
                return num == BLACK_FLAG ? 60 : 50;
            }else{
                return num == BLACK_FLAG ? 40 : 35;
            }
        }else if( !left && !right ){
            return 1;
        }else{
            return 10;
        }
    }else if( count == 3 ){

        if( left && right ){//左右两边都是空的
            if( k==0 ){
                //电脑950
                return num == BLACK_FLAG ? 950 : 700;
            }else{
                return num == BLACK_FLAG ? 900 : 650;
            }
        }else if( !left && !right ){
            return 1;
        }else{
            return 100;
        }
    }else if( count == 4 ){
        if( left && right ){//左右两边都是空的
            if( k==0 ){
                return num == BLACK_FLAG ? 6000 : 3500;
            }else{
                return num == BLACK_FLAG ? 5000 : 3000;
            }
        }else if( !left && !right ){
            return 1;
        }else{
            if( k==0 ){
                return num == BLACK_FLAG ? 4000 : 800;
            }else{
                return num == BLACK_FLAG ? 3600 : 750;
            }
        }
    }else{
        if( k==0 ){
            return num == BLACK_FLAG ? 20000 : 15000;
        }else{
            return num == BLACK_FLAG ? 10000 : 3300;
        }
    }

}

void pcLoad(){
    benefit();

    int count=0;
    int row=0;
    int col=0;
    int i=0;
    int j=0;
    for( i=0; i< 15; i++ ){
        for( j=0; j< 15; j++ ){
            if( situationPC[i][j] > count ){
                count = situationPC[i][j];
                row = i;
                col = j;
            }
            if( situationPlayer[i][j] > count ){
                count = situationPlayer[i][j];
                row = i;
                col = j;
            }
        }
    }

    printf("qinxingPC[%d][%d]:%d\n",row,col,situationPC[row][col]);
    printf("qinxingPlayer[%d][%d]:%d\n",row,col,situationPlayer[row][col]);

    if( map[row][col] == '+' ){
        map[row][col] = BLACK_FLAG;
    }

}

//记分
void benefit(){
    int n=0;
    int m=0;
    for( n=0;n<15;n++){
        for( m=0;m<15; m++ ){
            situationPC[n][m] = horizontal(n,m,BLACK_FLAG)+vertical(n,m,BLACK_FLAG)+leftSlope(n,m,BLACK_FLAG)+rightSlope(n,m,BLACK_FLAG);
            situationPlayer[n][m] = horizontal(n,m,WHITE_FLAG)+vertical(n,m,WHITE_FLAG)+leftSlope(n,m,WHITE_FLAG)+rightSlope(n,m,WHITE_FLAG);
        }
    }

}

//玩家下子
int playerLoad(fd_stdin) {
    int x, y;
    char rdbuf[128];
    int r=0;

    printf("input x:");
    r=read(fd_stdin, rdbuf, 70);
    rdbuf[r]=0;
    if(strcmp(rdbuf, "q")==0||strcmp(rdbuf, "Q")==0)
    {
        return 0;
    }
    atoi(rdbuf,&x);
    if(r>2)
    {
        x=-1;
    }

    printf("input y:");
    r=read(fd_stdin, rdbuf, 70);
    rdbuf[r]=0;
    if(strcmp(rdbuf, "q")==0||strcmp(rdbuf, "Q")==0)
    {
        return 0;
    }
    atoi(rdbuf,&y);
    
    while(x<0||y<0||x>HIGHT||y>WIDTH){
        printf("Please input a valid coordinate!Input again:\n");
        printf("input x:");
        r=read(fd_stdin, rdbuf, 70);
        rdbuf[r]=0;
        if(strcmp(rdbuf, "q")==0||strcmp(rdbuf, "Q")==0)
        {
            return 0;
        }
        atoi(rdbuf,&x);
        if(r>2)
        {
            x=-1;
        }

        printf("input y:");
        r=read(fd_stdin, rdbuf, 70);
        rdbuf[r]=0;
        if(strcmp(rdbuf, "q")==0||strcmp(rdbuf, "Q")==0)
        {
            return 0;
        }
        atoi(rdbuf,&y);
    }

    x--;
    y--;

    while(x<0||y<0||x>=HIGHT||y>=WIDTH||map[x][y] != '+')
    {
        printf("Please input a valid coordinate!Input again:\n");
        printf("input x:");
        r=read(fd_stdin, rdbuf, 70);
        rdbuf[r]=0;
        if(strcmp(rdbuf, "q")==0||strcmp(rdbuf, "Q")==0)
        {
            return 0;
        }
        atoi(rdbuf,&x);
        if(r>2)
        {
            x=-1;
        }

        printf("input y:");
        r=read(fd_stdin, rdbuf, 70);
        rdbuf[r]=0;
        if(strcmp(rdbuf, "q")==0||strcmp(rdbuf, "Q")==0)
        {
            return 0;
        }
        atoi(rdbuf,&y);
        x--;
        y--;
    }

    if (player == WHITE) {
        map[x][y] = WHITE_FLAG;
    }
    return 1;
}

int win() {
    char m;
    int i, j;

    if (player == WHITE) m = WHITE_FLAG;
    else m = BLACK_FLAG;

    for (i = 0; i < HIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            if (map[i][j] == m) {
                if ((i + 4) < HIGHT) {
                    if (map[i + 1][j] == m && map[i + 2][j] == m && map[i + 3][j] == m && map[i + 4][j] == m) return 1;
                }
                if ((j + 4) < WIDTH) {
                    if (map[i][j + 1] == m && map[i][j + 2] == m && map[i][j + 3] == m && map[i][j + 4] == m) return 1;
                }
                if ((i + 4) < HIGHT && (j + 4) < WIDTH) {
                    if (map[i + 1][j + 1] == m && map[i + 2][j + 2] == m && map[i + 3][j + 3] == m && map[i + 4][j + 4] == m) return 1;
                }
                if ((i + 4) < HIGHT && (j - 4) >= 0) {
                    if (map[i + 1][j - 1] == m && map[i + 2][j - 2] == m && map[i + 3][j - 3] == m && map[i + 4][j - 4] == m) return 1;
                }
            }
        }
    }
    return 0;
}





/*****************************************************************************
 *                                calendar
 *****************************************************************************/
int Isleap(int year)
{
	if ((year % 400 == 0) || ((year % 4 == 0) && (year % 100 != 0)))
		return 1;
	else
		return 0;
}

/*判断输入年份二月份的天数
返回值:month的天数*/
int Max_day(int year, int month)
{
	int Day[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
	if (Isleap(year) == 1)
		Day[1] = 29;
	return Day[month - 1];
}

/*计算输入的日期是这一年的多少天*/
int Total_day(int year, int month, int day)
{
	int sum = 0;
	int i = 1;
	for (i = 1; i < month; i++)
		sum = sum + Max_day(year, i);
	sum = sum + day;
	return sum;
}

/*由输入的日期判断当天是星期几
**返回值:count,0～6，分别表示星期日～星期六
*/
int Weekday(int year, int month, int day)
{
	int count;
	count = (year - 1) + (year - 1) / 4 - (year - 1) / 100 + (year - 1) / 400 + Total_day(year, month, day);
	count = count % 7;
	return count;
}


/*显示输入的日期是星期几*/
void display_week(int year, int month, int day)
{
	int count;
	count = Weekday(year, month, day);
	switch (count)
	{
	case 0:printf("\n%d-%d-%d is Sunday\n", year, month, day); break;
	case 1:printf("\n%d-%d-%d is Monday\n", year, month, day); break;
	case 2:printf("\n%d-%d-%d is Tuesday\n", year, month, day); break;
	case 3:printf("\n%d-%d-%d is Wednesday\n", year, month, day); break;
	case 4:printf("\n%d-%d-%d is Thursday\n", year, month, day); break;
	case 5:printf("\n%d-%d-%d is Friday\n", year, month, day); break;
	case 6:printf("\n%d-%d-%d is Saturday\n", year, month, day); break;
	}
}

/*显示输入的日期的当月日历*/
void display_month(int year, int month)
{
	int i = 0, j = 1;
	int week, max;
	week = Weekday(year, month, 1); //由每月1号确定打印制表符的个数
	max = Max_day(year, month);
	printf("\n                   %d/%d\n", year, month);
	printf("Sun    Mon    Tue    Wed    Thu    Fri    Sat\n");
	for (i = 0; i < week; i++)
		printf("       ");
	for (j = 1; j <= max; j++)
	{
		printf("%d     ", j);
		if (j < 10)
			printf(" ");
		if (i % 7 == 6)
			printf("\n");
		i++;
	}
	printf("\n");
}

/*****************************************************************************
 *                                计算器
 *****************************************************************************/

typedef int bool;
typedef int DATA;
#define False 0
#define True 1
#define EMPTY_CH '\0'
#define EMPTY_NUM -999999
#define SIZE 50

/*============ 判断表达式括号是否匹配 ============*/
char bucket_stack[100] = "\0";
int bucket_stack_index = -1;

bool isempty_bucket_stack() {
	return bucket_stack_index == -1;
}

void bucket_stack_push(char ch) {
	bucket_stack_index++;
	bucket_stack[bucket_stack_index] = ch;
}

char bucket_stack_pop(void) {
	if (isempty_bucket_stack()) {
		return EMPTY_CH;
	}
	char ch = bucket_stack[bucket_stack_index];
	bucket_stack_index--;
	return ch;
}

void bucket_stack_clear(void) {
	memset(bucket_stack, '\0', sizeof(bucket_stack));
	bucket_stack_index = -1;
}


/*============ 操作数栈 ============*/
int num_stack[100] = { 0 };
int num_stack_index = -1;

bool isempty_num_stack() {
	return num_stack_index == -1;
}

void num_stack_push(int num) {
	num_stack_index++;
	num_stack[num_stack_index] = num;
}

int num_stack_pop(void) {
	if (isempty_num_stack()) {
		return EMPTY_NUM;
	}
	int num = num_stack[num_stack_index];
	num_stack[num_stack_index] = 0;
	num_stack_index--;
	return num;
}

void num_stack_clear(void) {
	memset(num_stack, 0, sizeof(num_stack));
	num_stack_index = -1;
}


/*============ 操作符栈 ============*/
char op_stack[100] = "\0";
int op_stack_index = -1;

bool isempty_op_stack() {
	return op_stack_index == -1;
}

void op_stack_push(char ch) {
	op_stack_index++;
	op_stack[op_stack_index] = ch;
}

char op_stack_pop(void) {
	if (isempty_op_stack()) {
		return EMPTY_CH;
	}
	char ch = op_stack[op_stack_index];
	op_stack[op_stack_index] = '\0';
	op_stack_index--;
	return ch;
}

char op_stack_top(void) {
	if (isempty_op_stack()) {
		return EMPTY_CH;
	}
	return op_stack[op_stack_index];
}

void op_stack_clear(void) {
	memset(op_stack, '\0', sizeof(op_stack));
	op_stack_index = -1;
}


/*============ 优先级 ============*/
int isp(char ch)
//栈内优先级
{
	switch (ch)
	{
	case '#':return 0;
	case '(':return 1;
	case '*':case '/':return 5;
	case '+':case '-':return 3;
	case ')':return 6;
	}
}
int icp(char ch)
//栈外优先级
{
	switch (ch)
	{
	case '#':return 0;
	case '(':return 6;
	case '*':case '/':return 4;
	case '+':case '-':return 2;
	case ')':return 1;
	}
}


/*============ 运算符判断 ============*/
bool isOperator(char c)
{
	switch (c)
	{
	case '+':
	case '-':
	case '*':
	case '/':
	case '(':
	case ')':
	case '#':
		return True;
		break;
	default:
		return False;
		break;
	}
}

/*============ 单个数字字符判断 ============*/
bool isDigit(char ch)
{
	return (ch >= '0' && ch <= '9');
}

/*============ 数字判断 ============*/
bool isNum(char* exp)
//数字包括 正数 负数 浮点数 (其中+3要进行特殊处理)
{
	char ch = exp[0];
	if (ch == '+' && strlen(exp) > 1)
		// 如 +3 就是储存 3
	{
		exp = exp + 1;	//把+删除
		ch = exp[0];		//更新一下ch
	}

	if (isDigit(ch) || (ch == '-' && strlen(exp) > 1))
		//储存各种数字, 包括正数,负数,浮点数
	{
		return True;
	}

	return False;
}

/*============ 在表达式最后添加 # 标识符 ============*/
void addTail(char* exp) {
	int i;
	for (i = 0; i < strlen(exp); ++i);
	exp[i] = ' ';
	exp[i + 1] = '#';
}

/*============ 封装表达式中的项 ============*/
struct Data {
	int data;	//数据本身
	int flag;	//0->char, 1->int
};
int _current = 0;

/*============ 获取表达式中的下一项 ============*/
struct Data NextContent(char* exp)
{
	char _next[100] = "\0";
	char ch;
	int index = 0;

	for (int i = _current; i < strlen(exp); ++i)
	{
		ch = exp[i];
		if (ch != ' ')
		{
			_next[index] = ch;
			index++;	//因为不同对象以空格隔开,所以只要不是空格就加到_next
		}
		else
		{
			while (exp[i] == ' ') {
				i++;
			}
			_current = i;	//_current指向下一个位置,结束当前对象的寻找
			break;
		}
	}

	if (isOperator(_next[0])) {
		struct Data d;
		d.data = _next[0];
		d.flag = 0;
		return d;
	}
	else {
		struct Data d;
		atoi(_next, &d.data);
		d.flag = 1;
		return d;
	}
}


/*============ 根据运算符和两个操作数计算值 ============*/
int Cal(int left, char op, int right)
{
	switch (op)
	{
	case '+':
		return left + right;
		break;
	case '-':
		return left - right;
		break;
	case '*':
		return left * right;
		break;
	case '/':
		return left / right;
		break;
	default:
		return left + right;
		break;
	}
}

/*============ 输出后缀表达式 ============*/
void showBackMode(struct Data result[], int size) {
	printf("The reverse polish notation is: ");
	for (int i = 0; i < size; ++i) {
		if (result[i].flag == 0) {
			printf("%c ", result[i].data);
		}
		else {
			printf("%d ", result[i].data);
		}
	}
	printf("\n");
}

/*============ 计算后缀表达式的结果 ============*/
int calResult(struct Data result[], int size)
{
	num_stack_clear();
	for (int i = 0; i < size; ++i) {
		if (result[i].flag == 1) {
			num_stack_push(result[i].data);
		}
		else {
			int right = num_stack_pop();
			int left = num_stack_pop();
			num_stack_push(Cal(left, result[i].data, right));
		}
	}
	return num_stack_pop();
}


/*============ 顶层计算函数 ============*/
int calculate(char* origin_exp, bool if_showrev, bool if_beauty) {
	/*============ 表达式美化 ============*/
	char exp[100] = "\0";
	int pos = 0;
	for (int i = 0; i < strlen(origin_exp); ++i) {
		if (isOperator(origin_exp[i])) {
			exp[pos] = ' ';
			++pos;
			exp[pos] = origin_exp[i];
			++pos;
			exp[pos] = ' ';
			++pos;
		}
		else if (isDigit(origin_exp[i])) {
			exp[pos] = origin_exp[i];
			++pos;
		}
	}

	if (if_beauty) {
		printf("After beautify: %s\n", exp);
	}

	/*初始两个栈*/
	num_stack_clear();
	op_stack_clear();
	_current = 0;

	struct Data result[100];
	int index = 0;

	/*在表达式尾部添加结束标识符*/
	addTail(exp);

	op_stack_push('#');
	struct Data elem = NextContent(exp);
	while (!isempty_op_stack()) {
		char ch = elem.data;

		if (elem.flag == 1) {		//如果是操作数, 直接读入下一个内容
			result[index] = elem;
			index++;
			elem = NextContent(exp);
		}
		else if (elem.flag == 0) {	//如果是操作符,判断ch的优先级icp和当前栈顶操作符的优先级isp
			char topch = op_stack_top();
			if (isp(topch) < icp(ch)) {		//当前操作符优先级大,将ch压栈,读入下一个内容
				op_stack_push(ch);
				elem = NextContent(exp);
			}
			else if (isp(topch) > icp(ch)) {	//当前优先级小,推展并输出到结果中
				struct Data buf;
				buf.data = op_stack_pop();
				buf.flag = 0;
				result[index] = buf;
				index++;
			}
			else {
				if (op_stack_top() == '(') {	//如果退出的是左括号则读入下一个内容
					elem = NextContent(exp);
				}
				op_stack_pop();
			}
		}
	}

	if (if_showrev) {
		showBackMode(result, index);
	}

	return calResult(result, index);
}

/*判断表达式括号是否匹配*/
bool check_exp_bucket(char* exp) {
	char ch = '\0';

	for (int i = 0; i < strlen(exp); ++i) {
		if (exp[i] == '(') {
			bucket_stack_push('(');
		}
		else if (exp[i] == ')') {
			ch = bucket_stack_pop();
			if (ch == EMPTY_CH || ch != '(') {
				printf("Buckets in the exprssion you input do not match.\n");
				return False;
			}
		}
	}
	return isempty_bucket_stack();
}

/*判断表达式是否有非法符号*/
bool check_exp_notion(char* exp) {
	for (int i = 0; i < strlen(exp); ++i) {
		if (isDigit(exp[i]) || isOperator(exp[i]) || exp[i] == ' ') {
			continue;
		}
		else {
			printf("The operator we support: [+-*/()], you have input %c.\n", exp[i]);
			return False;
		}
	}
	return True;
}


/*****************************************************************************
 *                                2048
 *****************************************************************************/

#define KEY_CODE_UP    0x41
#define KEY_CODE_DOWN  0x42
#define KEY_CODE_LEFT  0x44
#define KEY_CODE_RIGHT 0x43
#define KEY_CODE_QUIT  0x71

struct termios old_config; /* linux下终端属性配置备份 */


static char config_path[4096] = { 0 }; /* 配置文件路径 */

static void init_game();    /* 初始化游戏 */
static void loop_game(int fd_stdin);    /* 游戏循环 */
static void reset_game();   /* 重置游戏 */
static void release_game(int signal); /* 释放游戏 */

static char* read_keyboard(int fd_stdin);

static void move_left();  /* 左移 */
static void move_right(); /* 右移 */
static void move_up();    /* 上移 */
static void move_down();  /* 下移 */

static void add_rand_num();    /* 生成随机数，本程序中仅生成2或4，概率之比设为9:1 */
static void check_game_over(); /* 检测是否输掉游戏，设定游戏结束标志 */
static int get_null_count();   /* 获取游戏面板上空位置数量 */
static void clear_screen();    /* 清屏 */
static void refresh_show();    /* 刷新界面显示 */

static int board[4][4];     /* 游戏数字面板，抽象为二维数组 */
static int score;           /* 游戏得分 */
static int best;            /* 游戏最高分 */
static int if_need_add_num; /* 是否需要生成随机数标志，1表示需要，0表示不需要 */
static int if_game_over;    /* 是否游戏结束标志，1表示游戏结束，0表示正常 */
static int if_prepare_exit; /* 是否准备退出游戏，1表示是，0表示否 */

/* main函数 函数定义 */
void Run2048(fd_stdin, fd_stdout)
{
	clear();
	init_game();
	loop_game(fd_stdin);
	release_game(0);
}

/* 开始游戏 函数定义 */
void loop_game(int fd_stdin) {
	while (1) {
		/* 接收标准输入流字符命令 */
		char rdbuf[128];
		int r = 0;
		r = read(fd_stdin, rdbuf, 70);
		if (r > 1)
		{
			refresh_show();
			continue;
		}
		rdbuf[r] = 0;
		char cmd = rdbuf[0];
		/* 判断是否准备退出游戏 */
		if (if_prepare_exit) {
			if (cmd == 'y' || cmd == 'Y') {
				/* 退出游戏，清屏后退出 */
				clear_screen();
				return;
			}
			else if (cmd == 'n' || cmd == 'N') {
				/* 取消退出 */
				if_prepare_exit = 0;
				refresh_show();
				continue;
			}
			else {
				continue;
			}
		}

		/* 判断是否已经输掉游戏 */
		if (if_game_over) {
			if (cmd == 'y' || cmd == 'Y') {
				/* 重玩游戏 */
				reset_game();
				continue;
			}
			else if (cmd == 'n' || cmd == 'N') {
				/* 退出游戏，清屏后退出  */
				clear();
				return;
			}
			else {
				continue;
			}
		}

		if_need_add_num = 0; /* 先设定不默认需要生成随机数，需要时再设定为1 */
		/* 命令解析，上下左右箭头代表上下左右命令，q代表退出 */
		switch (cmd) {
		case 'a':
			move_left();
			break;
		case 's':
			move_down();
			break;
		case 'w':
			move_up();
			break;
		case 'd':
			move_right();
			break;
		case 'q':
			if_prepare_exit = 1;
			break;
		default:
			refresh_show();
			continue;
		}

		/* 默认为需要生成随机数时也同时需要刷新显示，反之亦然 */
		if (if_need_add_num) {
			add_rand_num();
			refresh_show();
		}
		else if (if_prepare_exit) {
			refresh_show();
		}
	}
}

/* 重置游戏 函数定义 */
void reset_game() {
	score = 0;
	if_need_add_num = 1;
	if_game_over = 0;
	if_prepare_exit = 0;

	/* 了解到游戏初始化时出现的两个数一定会有个2，所以先随机生成一个2，其他均为0 */
	int n = get_ticks() % 16;
	int i;
	for (i = 0; i < 4; ++i) {
		int j;
		for (j = 0; j < 4; ++j) {
			board[i][j] = (n-- == 0 ? 2 : 0);
		}
	}

	/* 前面已经生成了一个2，这里再生成一个随机的2或者4，概率之比9:1 */
	add_rand_num();

	/* 在这里刷新界面并显示的时候，界面上已经默认出现了两个数字，其他的都为空（值为0） */
	refresh_show();
}

/* 生成随机数 函数定义 */
void add_rand_num() {
	int n = get_ticks() % get_null_count(); /* 确定在何处空位置生成随机数 */
	int i;
	for (i = 0; i < 4; ++i) {
		int j;
		for (j = 0; j < 4; ++j) {
			/* 定位待生成的位置 */
			if (board[i][j] == 0 && n-- == 0) {
				board[i][j] = (get_ticks() % 10 ? 2 : 4); /* 生成数字2或4，生成概率为9:1 */
				return;
			}
		}
	}
}

/* 获取空位置数量 函数定义 */
int get_null_count() {
	int n = 0;
	int i;
	for (i = 0; i < 4; ++i) {
		int j;
		for (j = 0; j < 4; ++j) {
			board[i][j] == 0 ? ++n : 1;
		}
	}
	return n;
}

/* 检查游戏是否结束 函数定义 */
void check_game_over() {
	int i;
	for (i = 0; i < 4; ++i) {
		int j;
		for (j = 0; j < 3; ++j) {
			/* 横向和纵向比较挨着的两个元素是否相等，若有相等则游戏不结束 */
			if (board[i][j] == board[i][j + 1] || board[j][i] == board[j + 1][i]) {
				if_game_over = 0;
				return;
			}
		}
	}
	if_game_over = 1;
}

/*
 * 如下四个函数，实现上下左右移动时数字面板的变化算法
 * 左和右移动的本质一样，区别仅仅是列项的遍历方向相反
 * 上和下移动的本质一样，区别仅仅是行项的遍历方向相反
 * 左和上移动的本质也一样，区别仅仅是遍历时行和列互换
*/

/*  左移 函数定义 */
void move_left() {
	/* 变量i用来遍历行项的下标，并且在移动时所有行相互独立，互不影响 */
	int i;
	for (i = 0; i < 4; ++i) {
		/* 变量j为列下标，变量k为待比较（合并）项的下标，循环进入时k<j */
		int j, k;
		for (j = 1, k = 0; j < 4; ++j) {
			if (board[i][j] > 0) /* 找出k后面第一个不为空的项，下标为j，之后分三种情况 */
			{
				if (board[i][k] == board[i][j]) {
					/* 情况1：k项和j项相等，此时合并方块并计分 */
					score += board[i][k++] *= 2;
					board[i][j] = 0;
					if_need_add_num = 1; /* 需要生成随机数和刷新界面 */
				}
				else if (board[i][k] == 0) {
					/* 情况2：k项为空，则把j项赋值给k项，相当于j方块移动到k方块 */
					board[i][k] = board[i][j];
					board[i][j] = 0;
					if_need_add_num = 1;
				}
				else {
					/* 情况3：k项不为空，且和j项不相等，此时把j项赋值给k+1项，相当于移动到k+1的位置 */
					board[i][++k] = board[i][j];
					if (j != k) {
						/* 判断j项和k项是否原先就挨在一起，若不是则把j项赋值为空（值为0） */
						board[i][j] = 0;
						if_need_add_num = 1;
					}
				}
			}
		}
	}
}

/* 右移 函数定义 */
void move_right() {
	/* 仿照左移操作，区别仅仅是j和k都反向遍历 */
	int i;
	for (i = 0; i < 4; ++i) {
		int j, k;
		for (j = 2, k = 3; j >= 0; --j) {
			if (board[i][j] > 0) {
				if (board[i][k] == board[i][j]) {
					score += board[i][k--] *= 2;
					board[i][j] = 0;
					if_need_add_num = 1;
				}
				else if (board[i][k] == 0) {
					board[i][k] = board[i][j];
					board[i][j] = 0;
					if_need_add_num = 1;
				}
				else {
					board[i][--k] = board[i][j];
					if (j != k) {
						board[i][j] = 0;
						if_need_add_num = 1;
					}
				}
			}
		}
	}
}

/* 上移 函数定义 */
void move_up() {
	/* 仿照左移操作，区别仅仅是行列互换后遍历 */
	int i;
	for (i = 0; i < 4; ++i) {
		int j, k;
		for (j = 1, k = 0; j < 4; ++j) {
			if (board[j][i] > 0) {
				if (board[k][i] == board[j][i]) {
					score += board[k++][i] *= 2;
					board[j][i] = 0;
					if_need_add_num = 1;
				}
				else if (board[k][i] == 0) {
					board[k][i] = board[j][i];
					board[j][i] = 0;
					if_need_add_num = 1;
				}
				else {
					board[++k][i] = board[j][i];
					if (j != k) {
						board[j][i] = 0;
						if_need_add_num = 1;
					}
				}
			}
		}
	}
}

/* 下移 函数定义 */
void move_down() {
	/* 仿照左移操作，区别仅仅是行列互换后遍历，且j和k都反向遍历 */
	int i;
	for (i = 0; i < 4; ++i) {
		int j, k;
		for (j = 2, k = 3; j >= 0; --j) {
			if (board[j][i] > 0) {
				if (board[k][i] == board[j][i]) {
					score += board[k--][i] *= 2;
					board[j][i] = 0;
					if_need_add_num = 1;
				}
				else if (board[k][i] == 0) {
					board[k][i] = board[j][i];
					board[j][i] = 0;
					if_need_add_num = 1;
				}
				else {
					board[--k][i] = board[j][i];
					if (j != k) {
						board[j][i] = 0;
						if_need_add_num = 1;
					}
				}
			}
		}
	}
}

/* 刷新界面 函数定义 */
void refresh_show() {
	clear();

	printf("\n\n\n\n");
	printf("                  GAME: 2048     SCORE: %05d    \n", score);
	printf("               --------------------------------------------------");

	/* 绘制方格和数字 */
	printf("\n\n                             |----|----|----|----|\n");
	int i;
	for (i = 0; i < 4; ++i) {
		printf("                             |");
		int j;
		for (j = 0; j < 4; ++j) {
			if (board[i][j] != 0) {
				if (board[i][j] < 10) {
					printf("  %d |", board[i][j]);
				}
				else if (board[i][j] < 100) {
					printf(" %d |", board[i][j]);
				}
				else if (board[i][j] < 1000) {
					printf(" %d|", board[i][j]);
				}
				else if (board[i][j] < 10000) {
					printf("%4d|", board[i][j]);
				}
				else {
					int n = board[i][j];
					int k;
					for (k = 1; k < 20; ++k) {
						n = n >> 1;
						if (n == 1) {
							printf("2^%02d|", k); /* 超过四位的数字用2的幂形式表示，如2^13形式 */
							break;
						}
					}
				}
			}
			else printf("    |");
		}

		if (i < 3) {
			printf("\n                             |----|----|----|----|\n");
		}
		else {
			printf("\n                             |----|----|----|----|\n");
		}
	}
	printf("\n");
	printf("               --------------------------------------------------\n");
	printf("                  [W]:UP [S]:DOWN [A]:LEFT [D]:RIGHT [Q]:EXIT\n");
	printf("                  Enter your commond here:");

	if (get_null_count() == 0) {
		check_game_over();

		/* 判断是否输掉游戏 */
		if (if_game_over) {
			printf("\r                      \nGAME OVER! TRY THE GAME AGAIN? [Y/N]:     \b\b\b\b");
		}
	}

	/* 判断是否准备退出游戏 */
	if (if_prepare_exit) {
		printf("\r                   \nDO YOU REALLY WANT TO QUIT THE GAME? [Y/N]:   \b\b");

	}
}

/* 初始化游戏 */
void init_game() {
	reset_game();
}

/* 释放游戏 */
void release_game(int signal) {
	clear();

	if (signal == SIGINT) {
		printf("\n");
	}

}


/*======================================================================*
							mine
 *======================================================================*/

#define rows 11
#define cols 11
#define Count 10

char mine[rows][cols];
char show[rows][cols];

void sl_init()
{
    int i = 0;  
    int j = 0;  
    for (i = 0; i < rows - 1; i++)  
    {  
        for (j = 0; j < cols - 1; j++)  
        {  
            mine[i][j] = '0';  
            show[i][j] = '*';  
        }  
    }
}
  
void sl_set_mine()  
{  
    mine[1][3] = '1';
    mine[2][6] = '1';
    mine[3][6] = '1';
    mine[4][1] = '1';
    mine[5][4] = '1';
    mine[6][8] = '1';
    mine[7][3] = '1';
    mine[8][8] = '1';
    mine[9][5] = '1';
    mine[7][9] = '1';
}  
  

void sl_display(char a[rows][cols])    
{  
    clear();
    printf("**************************************\n");
    printf("*              saolei                *\n");
    printf("**************************************\n");
    printf("*      1. 10 bombs total             *\n");
    printf("*      2. Enter 1-9 row number       *\n");
    printf("*      3. Enter 1-9 col number       *\n");
    printf("*      4. Enter q to quit            *\n");
    printf("**************************************\n");
    int i = 0;  
    int j = 0;
    printf("\n");  
    printf(" ");  
    for (i = 1; i < cols - 1; i++)  
    {  
        printf(" %d ", i);  
    }  
    printf("\n");  
    for (i = 1; i < rows - 1; i++)  
    {  
        printf("%d", i);  
        for (j = 1; j < cols - 1; j++)  
        {  
            printf(" %c ", a[i][j]);  
        }  
        printf("\n");  
    }  
}  
  
//�����׵ĸ���  
int sl_get_num(int x, int y)  
{  
    int count = 0;  
    if (mine[x - 1][y - 1] == '1')//���Ϸ�  
    {  
        count++;  
    }  
    if (mine[x - 1][y] == '1')//���  
    {  
        count++;  
    }  
    if (mine[x - 1][y + 1] == '1')//���·�  
    {  
        count++;  
    }  
    if (mine[x][y - 1] == '1')//�Ϸ�  
    {  
        count++;  
    }  
    if (mine[x][y + 1] == '1')//�·�  
    {  
        count++;  
    }  
    if (mine[x + 1][y - 1] == '1')//���Ϸ�  
    {  
        count++;  
    }  
    if (mine[x + 1][y] == '1')//�ҷ�  
    {  
        count++;  
    }  
    if (mine[x + 1][y + 1] == '1')//���·�  
    {  
        count++;  
    }  
    return  count;  
}  

//ɨ��  
int sl_sweep()  
{  
    int count = 0;  
    int x = 0, y = 0;  
    char cx[2], cy[2];
    while (count!=((rows-2)*(cols-2)-Count))  
    {  
        printf("Please input row number: ");  
        int r = read(0, cx, 2);
	if (cx[0] == 'q')
	    return 0;
	x = cx[0] - '0';
	while(x <= 0 || x > 9)
	{
	    printf("Wrong Input!\n");
	    printf("Please input row number: ");  
            r = read(0, cx, 2);
	    if (cx[0] == 'q')
	        return 0;
	    x = cx[0] - '0';
	}
        
	printf("Please input col number: ");  
        r = read(0, cy, 2);
	if (cy[0] == 'q')
	    return 0;
	y = cy[0] - '0';
	while(y <= 0 || y > 9)
	{
	    printf("Wrong Input!\n");
	    printf("Please input col number: ");  
            r = read(0, cy, 2);
	    if (cy[0] == 'q')
	        return 0;
	    y = cy[0] - '0';
	}

        if (mine[x][y] == '1')  
        {  
            sl_display(mine); 
            printf("BOOM SHAKALAKA! Game Over!\n"); 
	    
            return 0;  
        }  
        else  
        {  
            int ret = sl_get_num(x, y);  
            show[x][y] = ret + '0';  
            sl_display(show);  
            count++;  
        }  
    }  
    printf("YOU WIN!\n");  
    sl_display(mine);  
    return 0;  
}  

int runMine(fd_stdin, fd_stdout)  
{  
    printf("**************************************\n");
    printf("*              saolei                *\n");
    printf("**************************************\n");
    printf("*      1. 10 bombs total             *\n");
    printf("*      2. Enter 1-9 row number       *\n");
    printf("*      3. Enter 1-9 col number       *\n");
    printf("*      4. Enter q to quit            *\n");
    printf("**************************************\n");
    
    sl_init();
    sl_set_mine();
    sl_display(show);
    sl_sweep();
    
    printf("\nEnter anything to continue...");
    char rdbuf[128];
    int r = read(fd_stdin, rdbuf, 70);
    rdbuf[r]=0;
    while(r<1)
    {
        r = read(fd_stdin, rdbuf, 70);
    }
    clear();
    printf("\n");
    return 0;  
}  




/*======================================================================*
                            TicTacToe
 *======================================================================*/
int tmpQP[3][3]; //定义棋盘，0为空，1为敌方棋子，-1为我方棋子
#define MAX_NUM 1000
const int NO_BLANK=-1001; 
const int TREE_DEPTH=3; //搜索树的最大深度
const int NIL=1001;    //空
static int s_count;    


struct State//棋盘状态
{
    int QP[3][3]; 
    int e_fun; //评估函数值
    int child[9]; 
    int parent; 
    int bestChild; 
}States[MAX_NUM];


void chessMenu()
{       
	printf("      ====================================================================\n");
	printf("      #         *******                         Welcome to               #\n");
	printf("      #            **                            cherryOS                #\n");
	printf("      #        ***  **                                                   #\n");
	printf("      #     ******   **                                                  #\n");
	printf("      #   ******      **                         Instruction             #\n");
	printf("      #   ****      **  **               set: O--uesr X--computer        #\n");
	printf("      #            **    **            You can choose to play first or   #\n");
	printf("      #        ******     ****        let the computer first. Enter 'x'  #\n");
	printf("      #      **********  ******        from1,2,3,and then 'y' the same.  #\n");
	printf("      #      **********   ****         Can you defeat the computer?      #\n");
	printf("      #        ******                                                    #\n");
	printf("      #                                 PS:you can enter'q' to quit      #\n");
	printf("      #                                    before the game start.        #\n");
	printf("      #                                                                  #\n");
	printf("      #               Powered by doubleZ, budi, flyingfish               #\n");
	printf("      #                       ALL RIGHT REVERSED                         #\n");
	printf("      ====================================================================\n");
}

void Init()//初始化
{
    int i,j;
    s_count=0;
    for(i=0;i<3;i++)
    {
        for(j=0;j<3;j++)
        {
            States[0].QP[i][j]=0; //清空棋盘
        }
    }
    States[0].parent=NIL;
}

void Printchessboard() //打印棋盘
{
    clear();
    chessMenu();
    int i,j;
    for(i=0;i<3;i++)
    {
        for(j=0;j<3;j++)
        {
            if (States[0].QP[i][j] == -1)
            {
                printf("O     ");
            }
            else if (States[0].QP[i][j] == 1)
            {
                printf("X     ");
            }
            else
            {
                printf("#     ");
            }
        }
        printf("\n");
    }
}

int IsWin(struct State s) //判断输赢
{
    int i,j;
    for(i=0;i<3;i++)
    {
        if(s.QP[i][0]==1&&s.QP[i][1]==1&&s.QP[i][2]==1)
        	return 1;
        if(s.QP[i][0]==-1&&s.QP[i][1]==-1&&s.QP[i][2]==-1)
        	return -1;
    }
    for(i=0;i<3;i++)
    {
        if(s.QP[0][i]==1&&s.QP[1][i]==1&&s.QP[2][i]==1)return 1;
        if(s.QP[0][i]==-1&&s.QP[1][i]==-1&&s.QP[2][i]==-1)return -1;
    }
    if((s.QP[0][0]==1&&s.QP[1][1]==1&&s.QP[2][2]==1)||(s.QP[2][0]==1&&s.QP[1][1]==1&&s.QP[0][2]==1))
    	return 1;//计算机赢
    if((s.QP[0][0]==-1&&s.QP[1][1]==-1&&s.QP[2][2]==-1)||(s.QP[2][0]==-1&&s.QP[1][1]==-1&&s.QP[0][2]==-1))
    	return -1;//人赢
    return 0;//平局
}

int e_fun(struct State s)//评估函数
{
    int flag=1;
    int i,j;
    for(i=0;i<3;i++)
    {
        for(j=0;j<3;j++)
        {
            if(s.QP[i][j]==0)
            	flag= FALSE;
        }
    }
    if(flag)
    	return NO_BLANK;

    if(IsWin(s)==-1)
    	return -MAX_NUM;
    if(IsWin(s)==1)
    	return MAX_NUM;
    int count=0;

    //将空格填满电脑的棋子
    for(i=0;i<3;i++)
    {
        for(j=0;j<3;j++)
        {
            if(s.QP[i][j]==0)
            	tmpQP[i][j]=1;
            else
            	tmpQP[i][j]=s.QP[i][j];
        }
    }
    for(i=0;i<3;i++)
    {
        count+=(tmpQP[i][0]+tmpQP[i][1]+tmpQP[i][2])/3;
    }
    for(i=0;i<3;i++)
    {
        count+=(tmpQP[0][i]+tmpQP[1][i]+tmpQP[2][i])/3;
    }
    count+=(tmpQP[0][0]+tmpQP[1][1]+tmpQP[2][2])/3;
    count+=(tmpQP[2][0]+tmpQP[1][1]+tmpQP[0][2])/3;

    //将空格填满人的棋子
    for(i=0;i<3;i++)
    {
        for(j=0;j<3;j++)
        {
            if(s.QP[i][j]==0)
            	tmpQP[i][j]=-1;
            else
            	tmpQP[i][j]=s.QP[i][j];
        }
    }
        
    //行连成3个
    for(i=0;i<3;i++)
    {
        count+=(tmpQP[i][0]+tmpQP[i][1]+tmpQP[i][2])/3;
    }
    //列连成3个
    for(i=0;i<3;i++)
    {
        count+=(tmpQP[0][i]+tmpQP[1][i]+tmpQP[2][i])/3;
    }
    //斜行连成3个
    count+=(tmpQP[0][0]+tmpQP[1][1]+tmpQP[2][2])/3;
    count+=(tmpQP[2][0]+tmpQP[1][1]+tmpQP[0][2])/3;

    return count; 
        
}


int AutoDone()//计算机决定落子位置，并判断当前棋局。
{

    int
        MAX_F=NO_BLANK, //最大的评估函数值
        parent=-1,
        count,  
        tag;  
    int
        max_min=TREE_DEPTH%2,//max_min=1取下一层评估函数的最大值(电脑下)，max_min=0取最小值（人下）
        IsOK=FALSE;    //有没有找到下一步落子的位置？
    s_count=0;   //扩展生成的节点数初始值为0

    if(IsWin(States[0])==-1)//如果人赢了
    {
        printf("Conguatulations! You Win! GAME OVER.\n");
        return TRUE;
    }

    int i,j,t,k,i1,j1;
    for(t=0;t<TREE_DEPTH;t++)//t等于偶数电脑下棋，t等于奇数人下棋
    {
        count=s_count;
        for(k=parent+1;k<=count;k++)
        {
            int n_child=0;
            for(i=0;i<3;i++)
            {
                for(j=0;j<3;j++)
                {
                    if(States[k].QP[i][j]==0)//如果在位置(i,j)可以放置一个棋子
                    {      
                        s_count++;    //生成一个节点
                        for(i1=0;i1<3;i1++)
                        {
                            for(j1=0;j1<3;j1++)
                            {
                                States[s_count].QP[i1][j1]=States[k].QP[i1][j1];//将当前棋局复制到新节点对应的棋局中
                            }
                        }
                        States[s_count].QP[i][j]=t%2==0?1:-1;//人下棋置-1，计算机下棋置1
                        States[s_count].parent=k;   //其父母节点的下标为k
                        States[k].child[n_child++]=s_count; //下标为k的父母节点多了个子女
                        if(t==0&&e_fun(States[s_count])==MAX_NUM)//如果电脑能赢，则停止扩展
                        {
                            States[k].e_fun=MAX_NUM;
                            States[k].bestChild=s_count;
                            goto L2;
                        }
                    }
                }
            }
        }
        parent=count;   //将双亲节点设置为当前双亲节点的下一层节点
    }

    tag=States[s_count].parent;
    int pos_x,pos_y;//保存计算机落子的位置
    for(i=0;i<=s_count;i++)
    {
        if(i>tag) //i>tag的为叶节点
        {
            States[i].e_fun=e_fun(States[i]);
        }
        else //否则为非叶节点
        {
            States[i].e_fun=NIL;
        }
    }
    while(!IsOK)//寻找最佳落子
    {
        for(i=s_count;i>tag;i--)
        {
            if(max_min)//取子女节点的最大值
            {
                if(States[States[i].parent].e_fun<States[i].e_fun||States[States[i].parent].e_fun==NIL)
                {
                    States[States[i].parent].e_fun=States[i].e_fun;
                    States[States[i].parent].bestChild=i;   //设置父母节点的最佳子女的下标
                }
            }
            else//取子女节点的最小值
            {
                if(States[States[i].parent].e_fun>States[i].e_fun||States[States[i].parent].e_fun==NIL)
                {
                    States[States[i].parent].e_fun=States[i].e_fun;
                    States[States[i].parent].bestChild=i;   //设置父母节点的最佳子女的下标
                }
            }
        }
        s_count=tag; //将遍历的节点上移一层
        max_min=!max_min; //如果该层都是MAX节点，则它的上一层都是MIN节点，反之亦然（电脑和人交替落子）。
        if(States[s_count].parent!=NIL)
            tag=States[s_count].parent;
        else
            IsOK=TRUE; //结束搜索
    }
    int x,y;
L2: //取落子的位置，将x,y坐标保存在变量pos_x和pos_y中，并将根（当前）节点中的棋局设为最佳儿子节点的棋局

    for(x=0;x<3;x++)
    {
        for(y=0;y<3;y++)
        {
            if(States[States[0].bestChild].QP[x][y]!=States[0].QP[x][y])
            {
                pos_x=x;
                pos_y=y;   
            }
            States[0].QP[x][y]=States[States[0].bestChild].QP[x][y];
        }
    }


    MAX_F=States[0].e_fun;

    printf("The computer put a Chessman at: %d,%d\nchessboard:\n",pos_x+1,pos_y+1);
    Printchessboard();
    if(IsWin(States[0])==1)//计算机赢
    {
        printf("The computer WIN!GAME OVER.\n");
        return TRUE;
    }
    if(MAX_F==NO_BLANK) //棋盘没空可放了，平局。
    {
        printf("DRAW GAME!\n");
        return TRUE;
    }
    return FALSE;
}


void UserInput(int fd_stdin,int fd_stdout)//用户输入落子位置
{

    int n;
    int pos = -1,x,y;
    char szCmd[80]={0};
    printf("Now put your Chessman:\n ");
L1: printf("Please Input The Line Position (x): "); //x坐标
    n = read(fd_stdin,szCmd,80);
    szCmd[1] = 0;
    atoi(szCmd,&x);
    printf("Please Input The Column Position (y): "); //y坐标
    n = read(fd_stdin,szCmd,80);
    szCmd[1] = 0;
    atoi(szCmd,&y);
    if(x>0&&x<4&&y>0&&y<4&&States[0].QP[x-1][y-1]==0)
        States[0].QP[x-1][y-1]=-1;
    else
    {
        printf("Input Error!\n");
        goto L1;
    }

}
void TicTacToe(int fd_stdin,int fd_stdout)
{
    char buf[80]={0};
    char IsFirst = 0;
    int IsFinish = FALSE;
    chessMenu();
    while(!IsFinish)
    {

        Init();
        printf("chessboard: \n");
        Printchessboard();       
        printf("Do you want to be the first one to play chess?(y/n):");
        read(fd_stdin,buf,2);
        IsFirst = buf[0];
        do{
            if(IsFirst=='y')
            {
                UserInput(fd_stdin, fd_stdout);
                IsFinish=AutoDone();
            }
	    	else if(IsFirst=='n')
	    	{
                IsFinish=AutoDone();
                if(!IsFinish)
                	UserInput(fd_stdin, fd_stdout);
            }
	    	else if(IsFirst =='q')
	    	{          
                IsFinish = TRUE;
	    	}   
        }while(!IsFinish);
        if(IsFinish)
        {
            printf("Do you want to quit?(y/n)");
            char c;
            read(fd_stdin,buf,2);
            c = buf[0];
            if (c == 'n')
            {
                IsFinish = FALSE;
            }
        }
    }   
}


/*======================================================================*
							文件系统
 *======================================================================*/
#define MAX_FILE_PER_LAYER 10
#define MAX_FILE_NAME_LENGTH 20
#define MAX_CONTENT_ 50
#define MAX_FILE_NUM 100

 //文件ID计数器
int fileIDCount = 0;
int currentFileID = 0;

struct fileBlock {
	int fileID;
	char fileName[MAX_FILE_NAME_LENGTH];
	int fileType; //0 for txt, 1 for folder
	char content[MAX_CONTENT_];
	int fatherID;
	int children[MAX_FILE_PER_LAYER];
	int childrenNumber;
};
struct fileBlock blocks[MAX_FILE_NUM];
int IDLog[MAX_FILE_NUM];

//文件管理主函数
void runFileManage(int fd_stdin) {
	char rdbuf[128];
	char cmd[8];
	char filename[120];
	char buf[1024];
	int m, n;
	char _name[MAX_FILE_NAME_LENGTH];
	FSInit();
	int len = ReadDisk();
	ShowMessage();

	while (1) {
		for (int i = 0; i <= 127; i++)
			rdbuf[i] = '\0';
		for (int i = 0; i < MAX_FILE_NAME_LENGTH; i++)
			_name[i] = '\0';
		printf("\n/%s:", blocks[currentFileID].fileName);

		int r = read(fd_stdin, rdbuf, 70);
		rdbuf[r] = 0;
		assert(fd_stdin == 0);


		char target[10];
		for (int i = 0; i <= 1 && i < r; i++) {
			target[i] = rdbuf[i];
		}
		if (rdbuf[0] == 't' && rdbuf[1] == 'o' && rdbuf[2] == 'u' && rdbuf[3] == 'c' && rdbuf[4] == 'h') {
			if (rdbuf[5] != ' ') {
				printf("You should add the filename, like \"create XXX\".\n");
				printf("Please input [help] to know more.\n");
				continue;
			}
			for (int i = 0; i < MAX_FILE_NAME_LENGTH && i < r - 3; i++) {
				_name[i] = rdbuf[i + 6];
			}
			CreateFIle(_name, 0);
		}
		else if (rdbuf[0] == 'm' && rdbuf[1] == 'k' && rdbuf[2] == 'd' && rdbuf[3] == 'i' && rdbuf[4] == 'r') {
			if (rdbuf[5] != ' ') {
				printf("You should add the dirname, like \"mkdir XXX\".\n");
				printf("Please input [help] to know more.\n");
				continue;
			}
			char N[MAX_FILE_NAME_LENGTH];
			for (int i = 0; i < MAX_FILE_NAME_LENGTH && i < r - 3; i++) {
				_name[i] = rdbuf[i + 6];
			}
			CreateFIle(_name, 1);
		}
		else if (strcmp(rdbuf, "ls") == 0) {
			showFileList();
		}
		else if (strcmp(target, "cd") == 0) {
			if (rdbuf[2] == ' ' && rdbuf[3] == '.' && rdbuf[4] == '.') {
				ReturnFile(currentFileID);
				continue;
			}
			else if (rdbuf[2] != ' ') {
				printf("You should add the dirname, like \"cd XXX\".\n");
				printf("Please input [help] to know more.\n");

				continue;
			}
			for (int i = 0; i < MAX_FILE_NAME_LENGTH && i < r - 3; i++) {
				_name[i] = rdbuf[i + 3];
			}
			printf("name: %s\n", _name);
			int ID = SearchFile(_name);
			if (ID >= 0) {
				if (blocks[ID].fileType == 1) {
					currentFileID = ID;
					continue;
				}
				else if (blocks[ID].fileType == 0) {
					while (1) {
						printf("input the character representing the method you want to operate:"
							"\nu --- update"
							"\nd --- detail of the content"
							"\nq --- quit\n");
						int r = read(fd_stdin, rdbuf, 70);
						rdbuf[r] = 0;
						if (strcmp(rdbuf, "u") == 0) {
							printf("input the text you want to write:\n");
							int r = read(fd_stdin, blocks[ID].content, MAX_CONTENT_);
							blocks[ID].content[r] = 0;
						}
						else if (strcmp(rdbuf, "d") == 0) {
							printf("--------------------------------------------"
								"\n%s\n-------------------------------------\n", blocks[ID].content);
						}
						else if (strcmp(rdbuf, "q") == 0) {
							printf("would you like to save the change? y/n");
							int r = read(fd_stdin, rdbuf, 70);
							rdbuf[r] = 0;
							if (strcmp(rdbuf, "y") == 0) {
								printf("save changes!");
							}
							else {
								printf("quit without changing");
							}
							break;
						}
					}
				}
			}
			else
				printf("No such file!");
		}
		else if (strcmp(target, "rm") == 0) {
			if (rdbuf[2] != ' ') {
				printf("You should add the filename or dirname, like \"rm XXX\".\n");
				printf("Please input [help] to know more.\n");
				continue;
			}
			for (int i = 0; i < MAX_FILE_NAME_LENGTH && i < r - 3; i++) {
				_name[i] = rdbuf[i + 3];
			}
			int ID = SearchFile(_name);
			if (ID >= 0) {
				printf("Delete successfully!\n");
				DeleteFile(ID);
				for (int i = 0; i < blocks[currentFileID].childrenNumber; i++) {
					if (ID == blocks[currentFileID].children[i]) {
						for (int j = i + 1; j < blocks[currentFileID].childrenNumber; j++) {
							blocks[currentFileID].children[i] = blocks[currentFileID].children[j];
						}
						blocks[currentFileID].childrenNumber--;
						break;
					}
				}
			}
			else
				printf("No such file!\n");
		}
		else if (strcmp(target, "sv") == 0) {
			WriteDisk(1000);
			printf("Save to disk successfully!\n");
		}
		else if (strcmp(rdbuf, "help") == 0) {
			printf("\n");
			ShowMessage();
		}
		else if (strcmp(rdbuf, "quit") == 0) {
			clear();
			break;
		}
		else if (!strcmp(rdbuf, "clear")) {
				clear();
		}
		else {
			printf("Sorry, there no such command in the File Manager.\n");
			printf("You can input [help] to know more.\n");
		}
	}

}

void initFileBlock(int fileID, char* fileName, int fileType) {
	blocks[fileID].fileID = fileID;
	strcpy(blocks[fileID].fileName, fileName);
	blocks[fileID].fileType = fileType;
	blocks[fileID].fatherID = currentFileID;
	blocks[fileID].childrenNumber = 0;
}

void toStr3(char* temp, int i) {
	if (i / 100 < 0)
		temp[0] = (char)48;
	else
		temp[0] = (char)(i / 100 + 48);
	if ((i % 100) / 10 < 0)
		temp[1] = '0';
	else
		temp[1] = (char)((i % 100) / 10 + 48);
	temp[2] = (char)(i % 10 + 48);
}

void WriteDisk(int len) {
	char temp[MAX_FILE_NUM * 150 + 103];
	int i = 0;
	temp[i] = '^';
	i++;
	toStr3(temp + i, fileIDCount);
	i = i + 3;
	temp[i] = '^';
	i++;
	for (int j = 0; j < MAX_FILE_NUM; j++) {
		if (IDLog[j] == 1) {
			toStr3(temp + i, blocks[j].fileID);
			i = i + 3;
			temp[i] = '^';
			i++;
			for (int h = 0; h < strlen(blocks[j].fileName); h++) {
				temp[i + h] = blocks[j].fileName[h];
				if (blocks[j].fileName[h] == '^')
					temp[i + h] = (char)1;
			}
			i = i + strlen(blocks[j].fileName);
			temp[i] = '^';
			i++;
			temp[i] = (char)(blocks[j].fileType + 48);
			i++;
			temp[i] = '^';
			i++;
			for (int h = 0; h < strlen(blocks[j].content); h++) {
				temp[i + h] = blocks[j].content[h];
				if (blocks[j].content[h] == '^')
					temp[i + h] = (char)1;
			}
			i = i + strlen(blocks[j].content);
			temp[i] = '^';
			i++;
			toStr3(temp + i, blocks[j].fatherID);
			i = i + 3;
			temp[i] = '^';
			i++;
			for (int m = 0; m < MAX_FILE_PER_LAYER; m++) {
				toStr3(temp + i, blocks[j].children[m]);
				i = i + 3;
			}
			temp[i] = '^';
			i++;
			toStr3(temp + i, blocks[j].childrenNumber);
			i = i + 3;
			temp[i] = '^';
			i++;
		}
	}
	int fd = 0;
	int n1 = 0;
	fd = open("ss", O_RDWR);
	assert(fd != -1);
	n1 = write(fd, temp, strlen(temp));
	assert(n1 == strlen(temp));
	close(fd);
}

int toInt(char* temp) {
	int result = 0;
	for (int i = 0; i < 3; i++)
		result = result * 10 + (int)temp[i] - 48;
	return result;
}

int ReadDisk() {
	char bufr[1000];
	int fd = 0;
	int n1 = 0;
	fd = open("ss", O_RDWR);
	assert(fd != -1);
	n1 = read(fd, bufr, 1000);
	assert(n1 == 1000);
	bufr[n1] = 0;
	int r = 1;
	fileIDCount = toInt(bufr + r);
	r = r + 4;
	for (int i = 0; i < fileIDCount; i++) {
		int ID = toInt(bufr + r);
		IDLog[ID] = 1;
		blocks[ID].fileID = ID;
		r = r + 4;
		for (int i = 0; i < MAX_FILE_NAME_LENGTH; i++) {
			if (bufr[r] == '^')
				break;
			else if (bufr[r] == (char)1)
				blocks[ID].fileName[i] = '^';
			else
				blocks[ID].fileName[i] = bufr[r];
			r++;
		}
		r++;
		blocks[ID].fileType = (int)bufr[r] - 48;
		r = r + 2;
		for (int j = 0; j < MAX_CONTENT_; j++) {
			if (bufr[r] == '^')
				break;
			else if (bufr[r] == (char)1)
				blocks[ID].content[j] = '^';
			else
				blocks[ID].content[j] = bufr[r];
			r++;
		}
		r++;
		blocks[ID].fatherID = toInt(bufr + r);
		r = r + 4;
		for (int j = 0; j < MAX_FILE_PER_LAYER; j++) {
			blocks[ID].children[j] = toInt(bufr + r);
			r = r + 3;
		}
		r++;
		blocks[ID].childrenNumber = toInt(bufr + r);
		r = r + 4;
	}
	return n1;
}

void FSInit() {

	for (int i = 0; i < MAX_FILE_NUM; i++) {
		blocks[i].childrenNumber = 0;
		blocks[i].fileID = -2;
		IDLog[i] = '\0';
	}
	IDLog[0] = 1;
	blocks[0].fileID = 0;
	strcpy(blocks[0].fileName, "home");
	strcpy(blocks[0].content, "welcome to use file system!");
	blocks[0].fileType = 2;
	blocks[0].fatherID = 0;
	blocks[0].childrenNumber = 0;
	currentFileID = 0;
	fileIDCount = 1;
}

int CreateFIle(char* fileName, int fileType) {
	if (blocks[currentFileID].childrenNumber == MAX_FILE_PER_LAYER) {
		printf("Sorry you cannot add more files in this layer.\n");
		return 0;
	}
	else {
		for (int i = 0; i < blocks[currentFileID].childrenNumber; i++) {
			if (strcmp(blocks[blocks[currentFileID].children[i]].fileName, fileName) == 0) {
				if (fileType) {
					printf("You have a folder of same name!\n");
				}
				else {
					printf("You have a file of same name!\n");
				}
				return 0;
			}
		}
		fileIDCount++;
		int target = 0;
		for (int i = 0; i < MAX_FILE_NUM; i++) {
			if (IDLog[i] == 0) {
				target = i;
				break;
			}
		}
		initFileBlock(target, fileName, fileType);
		blocks[currentFileID].children[blocks[currentFileID].childrenNumber] = target;
		blocks[currentFileID].childrenNumber++;
		if (fileType) {
			printf("Create directory %s successful!\n", fileName);
		}
		else {
			printf("Create file %s successful!\n", fileName);
		}
		IDLog[target] = 1;
		return 1;
	}
}

void showFileList() {
	printf("The elements in %s.\n", blocks[currentFileID].fileName);		//通过currentFileID获取当前路径s

	printf("-----------------------------------------\n");
	printf("  filename |    type   | id  \n");
	for (int i = 0; i < blocks[currentFileID].childrenNumber; i++) {	//遍历每个孩子
		printf("%10s", blocks[blocks[currentFileID].children[i]].fileName);
		if (blocks[blocks[currentFileID].children[i]].fileType == 0) {
			printf(" | .txt file |");
		}
		else {
			printf(" |   folder  |");
		}
		printf("%3d\n", blocks[blocks[currentFileID].children[i]].fileID);
	}
	printf("-----------------------------------------\n");
}

int SearchFile(char* name) {
	for (int i = 0; i < blocks[currentFileID].childrenNumber; i++) {
		if (strcmp(name, blocks[blocks[currentFileID].children[i]].fileName) == 0) {
			return blocks[currentFileID].children[i];
		}
	}
	return -2;
}

void ReturnFile(int ID) {
	currentFileID = blocks[ID].fatherID;
}

void DeleteFile(int ID) {
	if (blocks[ID].childrenNumber > 0) {
		for (int i = 0; i < blocks[ID].childrenNumber; i++) {
			DeleteFile(blocks[blocks[ID].children[i]].fileID);
		}
	}
	IDLog[ID] = 0;
	blocks[ID].fileID = -2;
	blocks[ID].childrenNumber = 0;
	for (int i = 0; i < MAX_CONTENT_; i++)
		blocks[ID].content[i] = '\0';
	for (int i = 0; i < MAX_FILE_NAME_LENGTH; i++)
		blocks[ID].fileName[i] = '\0';
	blocks[ID].fileType = -1;
	for (int i = 0; i < MAX_FILE_PER_LAYER; i++)
		blocks[ID].children[i] = -1;
	blocks[ID].fatherID = -2;
	fileIDCount--;
}

void ShowMessage() {
	printf("      ====================================================================\n");
	printf("      #                            Welcome to                  ******    #\n");
	printf("      #                     cherryOS ~ File Manager            **        #\n");
	printf("      #                                                        ******    #\n");
	printf("      #                                                        **        #\n");
	printf("      #         [COMMAND]                 [FUNCTION]           **        #\n");
	printf("      #                                                                  #\n");
	printf("      #     $ touch [filename]  |   create a new .txt file               #\n");
	printf("      #     $ mkdir [dirname]   |   create a new folder                  #\n");
	printf("      #     $ ls                |   list the elements in this level      #\n");
	printf("      #     $ cd [dirname]      |   switch work path to this directory   #\n");
	printf("      #     $ cd ..             |   return to the superior directory     #\n");
	printf("      #     $ rm [name]         |   delete a file or directory           #\n");
	printf("      #     $ help              |   show command list of this system     #\n");
	printf("      #     $ clear             |   clear the cmd                        #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #               Powered by doubleZ, budi, flyingfish               #\n");
	printf("      #                       ALL RIGHT REVERSED                         #\n");
	printf("      ====================================================================\n");

	printf("\n\n");
}


/*****************************************************************************
 *                                processManager
 *****************************************************************************/
 //进程管理主函数
void runProcessManage(int fd_stdin)
{
	clear();
	char readbuffer[128];
	showProcessWelcome();
	while (1)
	{
		printf("cherryOS ~ process-manager: $ ");

		int end = read(fd_stdin, readbuffer, 70);
		readbuffer[end] = 0;
		int i = 0, j = 0;
		//获得命令指令
		char cmd[20] = { 0 };
		while (readbuffer[i] != ' ' && readbuffer[i] != 0)
		{
			cmd[i] = readbuffer[i];
			i++;
		}
		i++;
		//获取命令目标
		char target[20] = { 0 };
		while (readbuffer[i] != ' ' && readbuffer[i] != 0)
		{
			target[j] = readbuffer[i];
			i++;
			j++;
		}
		//结束进程;
		if (strcmp(cmd, "kill") == 0)
		{
			killProcess(target);
			continue;
		}
		//重启进程
		else if (strcmp(cmd, "restart") == 0)
		{
			restartProcess(target);
			continue;
		}
		//弹出提示
		else if (strcmp(readbuffer, "help") == 0)
		{
			clear();
			showProcessWelcome();
		}
		//打印全部进程
		else if (strcmp(readbuffer, "ps") == 0)
		{
			showProcess();
		}
		//退出进程管理
		else if (strcmp(readbuffer, "quit") == 0)
		{
			clear();

			break;
		}
		else if (!strcmp(readbuffer, "clear")) {
			clear();
		}
		//错误命令提示
		else
		{
			printf("Sorry, there no such command in the Process Manager.\n");
			printf("You can input [help] to know more.\n");
			printf("\n");
		}
	}
}


//打印欢迎界面
void showProcessWelcome()
{
	printf("      ====================================================================\n");
	printf("      #                            Welcome to                  ******    #\n");
	printf("      #                     cherryOS ~ Process Manager         **   *    #\n");
	printf("      #                                                        ******    #\n");
	printf("      #                                                        **        #\n");
	printf("      #                                                        **        #\n");
	printf("      #             [COMMAND]                 [FUNCTION]                 #\n");
	printf("      #                                                                  #\n");
	printf("      #           $ ps           |     show all process                  #\n");
	printf("      #           $ kill [id]    |     kill a process                    #\n");
	printf("      #           $ restart [id] |     restart a process                 #\n");
	printf("      #           $ quit         |     quit process management system    #\n");
	printf("      #           $ help         |     show command list of this system  #\n");
	printf("      #           $ clear        |     clear the cmd                     #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #               Powered by doubleZ, budi, flyingfish               #\n");
	printf("      #                       ALL RIGHT REVERSED                         #\n");
	printf("      ====================================================================\n");

	printf("\n\n");
}

//打印所有进程
void showProcess()
{
	int i;
	printf("===============================================================================\n");
	printf("    ProcessID    *    ProcessName    *    ProcessPriority    *    Running?           \n");
	//进程号，进程名，优先级，是否在运行
	printf("-------------------------------------------------------------------------------\n");
	for (i = 0; i < NR_TASKS + NR_PROCS; i++)//逐个遍历
	{
		printf("        %d", proc_table[i].pid);
		printf("                 %5s", proc_table[i].name);
		printf("                   %2d", proc_table[i].priority);
		if (proc_table[i].priority == 0)
		{
			printf("                   no\n");
		}
		else
		{
			printf("                   yes\n");
		}
		//printf("        %d                 %s                   %d                   yes\n", proc_table[i].pid, proc_table[i].name, proc_table[i].priority);
	}
	printf("===============================================================================\n\n");
}

int getMag(int n)
{
	int mag = 1;
	for (int i = 0; i < n; i++)
	{
		mag = mag * 10;
	}
	return mag;
}

//计算进程pid
int getPid(char str[])
{
	int length = 0;
	for (; length < MAX_FILENAME_LEN; length++)
	{
		if (str[length] == '\0')
		{
			break;
		}
	}
	int pid = 0;
	for (int i = 0; i < length; i++)
	{
		if (str[i] - '0' > -1 && str[i] - '9' < 1)
		{
			pid = pid + (str[i] + 1 - '1') * getMag(length - 1 - i);
		}
		else
		{
			pid = -1;
			break;
		}
	}
	return pid;
}

//结束进程
void killProcess(char str[])
{
	int pid = getPid(str);

	//健壮性处理以及结束进程
	if (pid >= NR_TASKS + NR_PROCS || pid < 0)
	{
		printf("The pid exceeded the range\n");
	}
	else if (pid < NR_TASKS)
	{
		printf("System tasks cannot be killed.\n");
	}
	else if (proc_table[pid].priority == 0 || proc_table[pid].p_flags == -1)
	{
		printf("Process not found.\n");
	}
	else if (pid == 4 || pid == 6)
	{
		printf("This process cannot be killed.\n");
	}
	else
	{
		proc_table[pid].priority = 0;
		proc_table[pid].p_flags = -1;
		printf("Aim process is killed.\n");
	}

	showProcess();
}

//重启进程
void restartProcess(char str[])
{
	int pid = getPid(str);

	if (pid >= NR_TASKS + NR_PROCS || pid < 0)
	{
		printf("The pid exceeded the range\n");
	}
	else if (proc_table[pid].p_flags != -1)
	{
		printf("This process is already running.\n");
	}
	else
	{
		proc_table[pid].priority = 1;
		proc_table[pid].p_flags = 1;
		printf("Aim process is running.\n");
	}

	showProcess();
}



/*======================================================================*
							kernel_main
 *======================================================================*/
PUBLIC int kernel_main()
{
	disp_str("-----\"kernel_main\" begins-----\n");

	struct task* p_task;
	struct proc* p_proc = proc_table;
	char* p_task_stack = task_stack + STACK_SIZE_TOTAL;
	u16   selector_ldt = SELECTOR_LDT_FIRST;
	u8    privilege;
	u8    rpl;
	int   eflags;
	int   i, j;
	int   prio;
	for (i = 0; i < NR_TASKS + NR_PROCS; i++) {
		if (i < NR_TASKS) {     /* 任务 */
			p_task = task_table + i;
			privilege = PRIVILEGE_TASK;
			rpl = RPL_TASK;
			eflags = 0x1202; /* IF=1, IOPL=1, bit 2 is always 1 */
			prio = 15;
		}
		else {                  /* 用户进程 */
			p_task = user_proc_table + (i - NR_TASKS);
			privilege = PRIVILEGE_USER;
			rpl = RPL_USER;
			eflags = 0x202; /* IF=1, bit 2 is always 1 */
			prio = 5;
		}

		strcpy(p_proc->name, p_task->name);	/* name of the process */
		p_proc->pid = i;			/* pid */

		p_proc->ldt_sel = selector_ldt;

		memcpy(&p_proc->ldts[0], &gdt[SELECTOR_KERNEL_CS >> 3],
			sizeof(struct descriptor));
		p_proc->ldts[0].attr1 = DA_C | privilege << 5;
		memcpy(&p_proc->ldts[1], &gdt[SELECTOR_KERNEL_DS >> 3],
			sizeof(struct descriptor));
		p_proc->ldts[1].attr1 = DA_DRW | privilege << 5;
		p_proc->regs.cs = (0 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.ds = (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.es = (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.fs = (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.ss = (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.gs = (SELECTOR_KERNEL_GS & SA_RPL_MASK) | rpl;

		p_proc->regs.eip = (u32)p_task->initial_eip;
		p_proc->regs.esp = (u32)p_task_stack;
		p_proc->regs.eflags = eflags;

		/* p_proc->nr_tty		= 0; */

		p_proc->p_flags = 0;
		p_proc->p_msg = 0;
		p_proc->p_recvfrom = NO_TASK;
		p_proc->p_sendto = NO_TASK;
		p_proc->has_int_msg = 0;
		p_proc->q_sending = 0;
		p_proc->next_sending = 0;

		for (j = 0; j < NR_FILES; j++)
			p_proc->filp[j] = 0;

		p_proc->ticks = p_proc->priority = prio;

		p_task_stack -= p_task->stacksize;
		p_proc++;
		p_task++;
		selector_ldt += 1 << 3;
	}

	k_reenter = 0;
	ticks = 0;

	p_proc_ready = proc_table;

	init_clock();
	init_keyboard();

	restart();

	while (1) {}
}


/*****************************************************************************
 *                                get_ticks
 *****************************************************************************/
PUBLIC int get_ticks()
{
	MESSAGE msg;
	reset_msg(&msg);
	msg.type = GET_TICKS;
	send_recv(BOTH, TASK_SYS, &msg);
	return msg.RETVAL;
}


/*======================================================================*
							   TestA
 *======================================================================*/
void TestA()
{
	int fd;
	int i, n;

	char tty_name[] = "/dev_tty0";

	char rdbuf[128];
	char command3[100], command4[100], command5[100];

	int fd_stdin = open(tty_name, O_RDWR);
	assert(fd_stdin == 0);
	int fd_stdout = open(tty_name, O_RDWR);
	assert(fd_stdout == 1);

	//	char filename[MAX_FILENAME_LEN+1] = "zsp01";
	const char bufw[80] = { 0 };

	clear();

	/*================================= 这里是开机动画 ===============================*/
	//Booting();

	//Loading();

	clear();
	/*================================= 这里是显示在上面的系统信息 ===============================*/

	CommandList();

	while (1) {
		printf("master@cherryOS: $ ");

		memset(command3, 0, sizeof(command3));
		memset(command4, 0, sizeof(command4));
		memset(command5, 0, sizeof(command5));

		int r = read(fd_stdin, rdbuf, 70);
		rdbuf[r] = 0;
		mystrncpy(command3, rdbuf, 3);
		mystrncpy(command4, rdbuf, 4);
		mystrncpy(command5, rdbuf, 5);

		if (!strcmp(command4, "help"))
		{
			clear();
			CommandList();
		}
		else if (!strcmp(command5, "clear"))
		{
			clear();
		}
		else if (!strcmp(command3, "man"))
		{
			if (strlen(rdbuf) > 4) {
				manMain(rdbuf + 4);
			}
			else {
				char* str = "NULL";
				manMain(str);
			}

			continue;
		}
		else if (!strcmp(command4, "game"))
		{
			if (strlen(rdbuf) > 5) {
				gameMain(rdbuf + 5, fd_stdin, fd_stdout);
			}
			else {
				char* str = "NULL";
				gameMain(str, fd_stdin, fd_stdout);
			}
			continue;
		}
		else if (!strcmp(command4, "math"))
		{
			if (strlen(rdbuf) > 5) {
				mathMain(rdbuf + 5);
			}
			else {
				char* str = "NULL";
				mathMain(str);
			}

			continue;
		}
		else if (!strcmp(command3, "cal"))
		{
			if (strlen(rdbuf) > 4) {
				calMain(rdbuf + 4);
			}
			else {
				char* str = "NULL";
				calMain(str);
			}
			continue;
		}
		else if (!strcmp(rdbuf, "process")) {
			clear();
			runProcessManage(fd_stdin);
		}
		else if (!strcmp(rdbuf, "file")) {
			clear();
			runFileManage(fd_stdin);
		}
		else if (!strcmp(rdbuf, ""))
		{
			continue;
		}
		else
		{
			clear();
			/*================================= 这里是命令不存在的提示信息 ===============================*/
			NotFound();
		}

	}
}


/*======================================================================*
							   TestB
 *======================================================================*/
void TestB()
{
	for(;;);

}


/*****************************************************************************
 *                                TestC
 *****************************************************************************/
void TestC()
{
	spin("TestC");
}


/*****************************************************************************
 *                                panic
 *****************************************************************************/
PUBLIC void panic(const char* fmt, ...)
{
	int i;
	char buf[256];

	/* 4 is the size of fmt in the stack */
	va_list arg = (va_list)((char*)& fmt + 4);

	i = vsprintf(buf, fmt, arg);

	printl("%c !!panic!! %s", MAG_CH_PANIC, buf);

	/* should never arrive here */
	__asm__ __volatile__("ud2");
}


void clear()
{
	clear_screen(0, console_table[current_console].cursor);
	console_table[current_console].crtc_start = 0;
	console_table[current_console].cursor = 0;
}


void mystrncpy(char* dest, char* src, int len)
{
	assert(dest != NULL && src != NULL);

	char* temp = dest;
	int i = 0;
	while (i++ < len && (*temp++ = *src++) != '\0');

	if (*(temp) != '\0') {
		*temp = '\0';
	}
}

/*开机动画*/
void Booting()
{
	emptyWindow();

	gradualStart();
	emptyWindow();

}

void emptyWindow()
{
	
	clear();
}
void gradualStart()
{
	
}
void Loading() {
	printf("System is loading...");

	for (int i = 0; i < 25; ++i)
	{
		milli_delay(DELAY_TIME / 5);
		printf(".");
	}
}


/*所有指令 & help窗口*/
void CommandList()
{
	printf("      ====================================================================\n");
	printf("      #         *******                         Welcome to               #\n");
	printf("      #            **                            cherryOS                #\n");
	printf("      #        ***  **                                                   #\n");
	printf("      #     ******   **                       [COMMAND LIST]             #\n");
	printf("      #   ******      **            $ help --- show the command list     #\n");
	printf("      #   ****      **  **          $ clear --- clear the cmd            #\n");
	printf("      #            **    **         $ man [command]                      #\n");
	printf("      #        ******     ****          --- know more about the command  #\n");
	printf("      #      **********  ******     $ game [-option]                     #\n");
	printf("      #      **********   ****          --- play the built-in game       #\n");
	printf("      #        ******               $ math [-option] [expression]        #\n");
	printf("      #                                 --- calculate the value          #\n");
	printf("      #                             $ cal [-option]                      #\n");
	printf("      #                                 --- date information             #\n");
	printf("      #                             $ process --- process manager        #\n");
	printf("      #                             $ file --- file manager              #\n");
	printf("      #                                                                  #\n");
	printf("      #               Powered by doubleZ, budi, flyingfish               #\n");
	printf("      #                       ALL RIGHT REVERSED                         #\n");
	printf("      ====================================================================\n");


	printf("\n\n");
}

/*没找到该指令窗口*/
void NotFound()
{
	printf("      ====================================================================\n");
	printf("      #                                                                  #\n");
	printf("      #         *******                                                  #\n");
	printf("      #            **                                                    #\n");
	printf("      #        ***  **                                                   #\n");
	printf("      #     ******   **                                                  #\n");
	printf("      #   ******      **                                                 #\n");
	printf("      #   ****      **  **                      Sorry                    #\n");
	printf("      #            **    **            Your command is not found         #\n");
	printf("      #        ******     ****                                           #\n");
	printf("      #      **********  ******                                          #\n");
	printf("      #      **********   ****                                           #\n");
	printf("      #        ******                                                    #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                 Input [help] for more information.               #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      ====================================================================\n");
	printf("\n\n");
}


void manMain(char* option)
{
	if (!strcmp(option, "NULL")) {
		printf("Sorry, you should add an option.\n");
	}
	else if (!strcmp(option, "math")) {
		clear();
		
	}
	else if (!strcmp(option, "help")) {
		clear();

	}
	else if (!strcmp(option, "game")) {
		clear();
	
	}
	else if (!strcmp(option, "cal")) {
		clear();
		
	}
	else if (!strcmp(option, "clear")) {
		clear();
		
	}
	else if (!strcmp(option, "man")) {
		clear();
		
	}
	else if (!strcmp(option, "process")) {
		clear();
		

	}
	else if (!strcmp(option, "file")) {
		clear();
		

	}
	else {
		printf("Sorry, there no such option for man.\n");
		printf("You can input [help] to know more.\n");
	}

	printf("\n");
}

void mathMain(char* expression)
{
	if (!strcmp(expression, "NULL")) {
		printf("Sorry, you should add a math expressioin.\n");
	}
	else {
		char str_list[2][10] = { "-beauty","-rev" };
		int flag[2] = { 1,1 };
		for (int i = 0; i < 2; ++i) {
			int j = 0;
			while (expression[j] != ' ' && expression[j] != '\0') {
				if (expression[j] != str_list[i][j]) {
					flag[i] = 0;
					break;
				}
				++j;
			}
		}


		if (flag[0]) {
			if (strlen(expression) > 8) {
				char* value = expression + 8;

				if (!check_exp_notion(value)) {
					printf("Please check the expression and try again.\n");
					printf("\n");
					return;
				}

				bucket_stack_clear();
				if (!check_exp_bucket(value)) {
					printf("Please check the expression and try again.\n");
					printf("\n");
					return;
				}

				int result = calculate(value, False, True);
				if (result != EMPTY_NUM) {
					printf("The result is %d\n", result);
				}
				else {
					printf("You can input [help] to know more.\n");
				}
			}
			else {
				printf("You should add the expression you want to calculate.\n");
				printf("You can input [man cal] to know more.\n");
			}
		}
		else if (flag[1]) {
			if (strlen(expression) > 5) {
				char* value = expression + 5;

				if (!check_exp_notion(value)) {
					printf("Please check the expression and try again.\n");
					printf("\n");
					return;
				}

				bucket_stack_clear();
				if (!check_exp_bucket(value)) {
					printf("Please check the expression and try again.\n");
					printf("\n");
					return;
				}


				int result = calculate(value, True, False);
				if (result != EMPTY_NUM) {
					printf("The result is %d\n", result);
				}
				else {
					printf("You can input [help] to know more.\n");
				}
			}
			else {
				printf("You should add the expression you want to calculate.\n");
				printf("You can input [man cal] to know more.\n");
			}
		}
		else {
			if (strlen(expression) > 0) {
				char* value = expression;

				if (!check_exp_notion(value)) {
					printf("Please check the expression and try again.\n");
					printf("\n");
					return;
				}

				bucket_stack_clear();
				if (!check_exp_bucket(value)) {
					printf("Please check the expression and try again.\n");
					printf("\n");
					return;
				}

				int result = calculate(value, False, False);
				if (result != EMPTY_NUM) {
					printf("The result is %d\n", result);
				}
				else {
					printf("You can input [help] to know more.\n");
				}
			}
			else {
				printf("You should add at least a expression.\n");
				printf("You can input [help] to know more.\n");
			}
		}
	}

	printf("\n");
}

void calMain(char* option)
{
	/*判断附加的选项是何种命令*/
	char str_list[3][10] = { "-month","-week","-date" };
	int flag[3] = { 1,1,1 };
	for (int i = 0; i < 3; ++i) {
		int j = 0;
		while (option[j] != ' ' && option[j] != '\0') {
			if (option[j] != str_list[i][j]) {
				flag[i] = 0;
				break;
			}
			++j;
		}
	}

	//char value[20] = "\0";
	int year, month, day;
	char year_str[5] = "\0", month_str[3] = "\0", day_str[3] = "\0";

	if (!strcmp(option, "NULL")) {
		printf("Sorry, you should add an option.\n");
	}
	else if (flag[0]) {				//-month
		if (strlen(option) > 7) {
			char* value = option + 7;

			int i = 0;
			for (int j = 0; i < strlen(value) && value[i] != '/' && value[i] != ' '; ++i, ++j) {
				year_str[i] = value[i];
			}
			++i;
			for (int j = 0; i < strlen(value) && value[i] != ' '; ++i, ++j) {
				month_str[j] = value[i];
			}

			atoi(year_str, &year);
			atoi(month_str, &month);

			if (year > 0 && month > 0 && month < 13) {
				display_month(year, month);
			}
			else {
				if (year < 0) {
					printf("The [year] you input should greater than 0.\n");
				}
				if (month < 1 || month>12) {
					printf("The [month] you input should between 1 to 12.\n");
				}
				printf("Please input again.\n");
			}
		}
		else {
			printf("Sorry, you should add [Y/M].\n");
			printf("You can input [man cal] to know more.\n");
		}
	}
	else if (flag[1]) {				//-week
		if (strlen(option) > 6) {
			char* value = option + 6;

			int i = 0;
			for (int j = 0; i < strlen(value) && value[i] != '/' && value[i] != ' '; ++i, ++j) {
				year_str[i] = value[i];
			}
			++i;
			for (int j = 0; i < strlen(value) && value[i] != '/' && value[i] != ' '; ++i, ++j) {
				month_str[j] = value[i];
			}
			++i;
			for (int j = 0; i < strlen(value) && value[i] != ' '; ++i, ++j) {
				day_str[j] = value[i];
			}

			atoi(year_str, &year);
			atoi(month_str, &month);
			atoi(day_str, &day);

			if (year > 0 && month > 0 && month < 13 && day>0 && day < 32) {
				if (Isleap(year) == 1 && month == 2 && day > 29) {
					printf("%d is a leap year, the [day] you input should not greater than 29.\n", year);
				}
				else if (Isleap(year) == 0 && month == 2 && day > 28) {
					printf("%d is a normal year, the [day] you input should not greater than 28.\n", year);
				}
				else {
					display_week(year, month, day);
				}
			}
			else {
				if (year < 0) {
					printf("The [year] you input should greater than 0.\n");
				}
				if (month < 1 || month>12) {
					printf("The [month] you input should between 1 to 12.\n");
				}
				if (day < 1 || day>31) {
					printf("The day you input should between 1 to 31.\n");
				}
				printf("Please input again.\n");
			}
		}
		else {
			printf("Sorry, you should add .\n");
			printf("You can input [man cal] to know more.\n");
		}
	}
	else if (flag[2]) {				//-date
		if (strlen(option) > 6) {
			char* value = option + 6;

			int i = 0;
			for (int j = 0; i < strlen(value) && value[i] != '/' && value[i] != ' '; ++i, ++j) {
				year_str[i] = value[i];
			}
			++i;
			for (int j = 0; i < strlen(value) && value[i] != '/' && value[i] != ' '; ++i, ++j) {
				month_str[j] = value[i];
			}
			++i;
			for (int j = 0; i < strlen(value) && value[i] != ' '; ++i, ++j) {
				day_str[j] = value[i];
			}

			atoi(year_str, &year);
			atoi(month_str, &month);
			atoi(day_str, &day);

			if (year > 0 && month > 0 && month < 13 && day>0 && day < 32) {
				if (Isleap(year) == 1 && month == 2 && day > 29) {
					printf("%d is a leap year, the [day] you input should not greater than 29.\n", year);
				}
				else if (Isleap(year) == 0 && month == 2 && day > 28) {
					printf("%d is a normal year, the [day] you input should not greater than 28.\n", year);
				}
				else {
					printf("\n%d/%d/%d is day No.%d of the year.\n", year, month, day, Total_day(year, month, day));
				}
			}
			else {
				if (year < 0) {
					printf("The [year] you input should greater than 0.\n");
				}
				if (month < 1 || month>12) {
					printf("The [month] you input should between 1 to 12.\n");
				}
				if (day < 1 || day>31) {
					printf("The day you input should between 1 to 31.\n");
				}
				printf("Please input again.\n");
			}
		}
		else {
			printf("Sorry, you should add .\n");
			printf("You can input [man cal] to know more.\n");
		}
	}
	else {
		printf("Sorry, there no such option for cal.\n");
		printf("You can input [man cal] to know more.\n");
	}

	printf("\n");
}

void gameMain(char* option, int fd_stdin, int fd_stdout)
{
	if (!strcmp(option, "NULL")) {
		printf("Sorry, you should add an option.\n");
	}
	else if (!strcmp(option, "-2048")) {
		Run2048(fd_stdin, fd_stdout);
	}
	else if (!strcmp(option, "-chess")) {
                fiveChess(fd_stdin);
	}
	else if (!strcmp(option, "-mine")) {
                runMine(fd_stdin, fd_stdout);
	}
	else if (!strcmp(option, "-tictactoe")) {
                TicTacToe(fd_stdin, fd_stdout);
	}
        else if (!strcmp(option, "-pushbox")){
                Runpushbox(fd_stdin, fd_stdout);
        }
	else {
		printf("Sorry, there no such option for game.\n");
		printf("You can input [man game] to know more.\n");
	}

	printf("\n");
}


