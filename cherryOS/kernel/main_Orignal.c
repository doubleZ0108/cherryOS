
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            main.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Forrest Yu, 2005
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

// my code here
#define MAX_ARRAY_NUM 1000 //文件树最大数目

//
//  main.c
//  draw_map
//
//  Created by wennie on 2017/9/18.
//  Copyright © 2017年 wennie. All rights reserved.
//

/*****************************************************************************
 *                                box man
 *****************************************************************************/
#include <stdio.h>
//#include <conio.h>
int pi=0;
int pj=0;
void draw_map(int map[9][11]); //声明画图函数
void boxman(fd_stdin, fd_stdout)
{
    char rdbuf[128];
    int r;
    char control;
    printf("    *                                                                     \n");
    printf("    *     ~~~~  ** **** **  ~~~~                                          \n");
    printf("    *    ~~  **           **   ~~     Welcome!                            \n");
    printf("    *   ~~  **   ^^    ^^  **   ~~    Game-4:BoxMan               \n");
    printf("    *  ~~~~**     @    @    **~~~~~                                       \n");
    printf("    *      **    _______    **        RULES:                              \n");
    printf("    *      **    | * * |    **        s:DOWN d:RIGHT s:UP  a:LEFT q:QUIT      \n");
    printf("    *      **    ~~~~~~~    **        ILLUSTRATE:                             \n");
    printf("    *       **      3      **         #:WALL d:DESTINATION b:BOX  &:PEOPLE  \n");
    printf("    *        **           **           GGGG    A    M  M EEEE              \n");
    printf("    *          ***********             G  __  A A   M\~M E___              \n");
    printf("    *                                  G  G  AAAAA  M  M E                 \n");
    printf("    *                                  GGGG A     A M  M EEEE              \n");
    printf("    *                                                                     \n");
    printf("    *    ********************************************************         \n");
    int count=0;   //定义记分变量
    /*下面定义一个二维数组，其中的元素是不同图像的ID号，在后面的函数中将讲解不同数字分别代表的图像元素*/
    int map[9][11] = {
        {2,1,1,1,1,1,1,1,1,1,2},
        {2,1,0,0,0,1,0,0,0,1,2},
        {2,1,0,4,4,4,4,4,0,1,2},
        {2,1,0,4,0,4,0,4,0,1,1},
        {2,1,0,0,0,6,0,0,4,0,1},
        {1,1,0,1,1,1,1,0,4,0,1},
        {1,0,3,3,3,3,3,1,0,0,1},
        {1,0,3,3,3,3,3,0,0,1,1},
        {1,1,1,1,1,1,1,1,1,1,2},
    };
    while (1)   //死循环，等待用户命令
    {
        /*for (i=1;i<10;i++)
         {
         printf("%d\n",i);
         }*/
        clear();
        printf("\n");
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
        
//        input = getch();   //用getch()函数无需回车确认地获取用户输入，用于控制行走方向。
        printf("\n");
        printf("Please input direction:");
        
//        scanf("%c",&control);
//        getchar();
        
        r = read(fd_stdin, rdbuf, 70);
        rdbuf[r] = 0;
        control=rdbuf[0];
        
        //printf("%c",control);
        
        if(control == 'Q' || control == 'q')
        {
            break;
        }
        switch (control)
        {
            case 'w':
            //如果人前面是空地。  //0代表空地  6代表人  //3代表目的地
            if(map[pi-1][pj]==0)
            {
                map[pi-1][pj]=6+0; //人往前走一步，ID为人的ID（）加上空地的ID（）。
                if(map[pi][pj]==9) //如果当前人站的位置为目的地，则ID为（即人的ID（）加上目的地ID（））。
                map[pi][pj]=3; //将人往前走一步后原地的ID修改为空地ID（）。
                else
                map[pi][pj]=0;    //否则原地ID修改为空地ID 。
            }
            //如果人前面是目的地。
            else if((map[pi-1][pj]==3)||(map[pi-1][pj]==9))
            {
                map[pi-1][pj]=6+3; //人往前走一步，ID为人ID+目的地ID=9。
                if(map[pi][pj]==9) //如果原地也是目的地（ID为）。
                map[pi][pj]=3; //人走后把原地ID修改回目的地ID。
                else
                map[pi][pj]=0; //否则原地ID修改为为空地ID
            }
            //如果人前面是箱子。//4代表箱子   //7箱子进入目的地
            else if(map[pi-1][pj]==4)
            {
                //如果人前面是箱子，而箱子前面是空地。
                if (map[pi-2][pj]==0)
                {
                    map[pi-2][pj]=4; //人推箱子往前走一步，把空地ID修改为箱子ID（）
                    //下面是对箱子原地进行判断
                    if(map[pi-1][pj]==7) //如果箱子原地为目的地。
                    map[pi-1][pj]=9; //人站在箱子的原地（目的地）时该地ID应该为人的ID+目的地ID=9。
                    else
                    map[pi-1][pj]=6; //否则，人站在了原来的空地上，ID应该为+0=6。
                    //下面是对人原地进行判断
                    if(map[pi][pj]==9) //如果之前是目的地。
                    map[pi][pj]=3; //人走了之后修改回目的地ID。
                    else
                    map[pi][pj]=0; //否则就是空地。
                }
                //如果人的前面是箱子，而箱子前面是目的地。
                else if (map[pi-2][pj]==3)
                {
                    map[pi-2][pj]=7;   //ID为（目的地ID（）+箱子ID（）=7），表示已经把箱子推入了目的地。
                    count++;
                    //下面是对箱子原先位置的判断，同上。
                    if(map[pi-1][pj]==7)
                    map[pi-1][pj]=9;
                    else
                    map[pi-1][pj]=6;
                    //下面是对人原先位置进行判断，同上。
                    if(map[pi][pj]==9)
                    map[pi][pj]=3;
                    else
                    map[pi][pj]=0;
                }
            }
            //如果人前面是已经进入某目的地的箱子（ID=7）。
            else if(map[pi-1][pj]==7)
            {
                //如果人前面是已经进入某目的地的箱子,而箱子前面是空地。
                if(map[pi-2][pj]==0)
                {
                    count--;
                    map[pi-2][pj]=4;   //把箱子重新推到空地上，ID=箱子ID+空地ID=4。
                    map[pi-1][pj]=9;   //人自然而然的就站在了原先的目的地上了。
                    //下面是对人原先地进行判断，方法同上。
                    if(map[pi][pj]==9)
                    map[pi][pj]=3;
                    else
                    map[pi][pj]=0;
                }
                //如果人前面是已经进入某目的地的箱子，而箱子前面是另一目的地。
                if(map[pi-2][pj]==3)
                {
                    map[pi-2][pj]=7;   //把箱子推入了另一目的地，自然，ID也应是。
                    map[pi-1][pj]=9;   //人站在了目的地上。
                    //下面是对人原先站立地进行判断，方法同上。
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
                map[pi+1][pj]=6+0; //人往前走一步，ID为人的ID（）加上空地的ID（）。
                if(map[pi][pj]==9) //如果当前人站的位置为目的地，则ID为（即人的ID（）加上目的地ID（））。
                map[pi][pj]=3; //将人往前走一步后原地的ID修改为空地ID（）。
                else
                map[pi][pj]=0;    //否则原地ID修改为空地ID 。
            }
            //如果人前面是目的地。
            else if(map[pi+1][pj]==3)
            {
                map[pi+1][pj]=6+3; //人往前走一步，ID为人ID+目的地ID=9。
                if(map[pi][pj]==9) //如果原地也是目的地（ID为）。
                map[pi][pj]=3; //人走后把原地ID修改回目的地ID。
                else
                map[pi][pj]=0; //否则原地ID修改为为空地ID
            }
            //如果人前面是箱子。
            else if(map[pi+1][pj]==4)
            {
                //如果人前面是箱子，而箱子前面是空地。
                if (map[pi+2][pj]==0)
                {
                    map[pi+2][pj]=4; //人推箱子往前走一步，把空地ID修改为箱子ID（）
                    //下面是对箱子原地进行判断
                    if(map[pi+1][pj]==7) //如果箱子原地为目的地。
                    map[pi+1][pj]=9; //人站在箱子的原地（目的地）时该地ID应该为人的ID+目的地ID=9。
                    else
                    map[pi+1][pj]=6; //否则，人站在了原来的空地上，ID应该为+0=6。
                    //下面是对人原地进行判断
                    if(map[pi][pj]==9) //如果之前是目的地。
                    map[pi][pj]=3; //人走了之后修改回目的地ID。
                    else
                    map[pi][pj]=0; //否则就是空地。
                }
                //如果人的前面是箱子，而箱子前面是目的地。
                else if (map[pi+2][pj]==3)
                {
                    map[pi+2][pj]=7;   //ID为（目的地ID（）+箱子ID（）=7），表示已经把箱子推入了目的地。
                    count++;
                    //下面是对箱子原先位置的判断，同上。
                    if(map[pi+1][pj]==7)
                    map[pi+1][pj]=9;
                    else
                    map[pi+1][pj]=6;
                    //下面是对人原先位置进行判断，同上。
                    if(map[pi][pj]==9)
                    map[pi][pj]=3;
                    else
                    map[pi][pj]=0;
                }
            }
            //如果人前面是已经进入某目的地的箱子（ID=7）。
            else if(map[pi+1][pj]==7)
            {
                //如果人前面是已经进入某目的地的箱子,而箱子前面是空地。
                if(map[pi+2][pj]==0)
                {
                    count--;
                    map[pi+2][pj]=4;   //把箱子重新推到空地上，ID=箱子ID+空地ID=4。
                    map[pi+1][pj]=9;   //人自然而然的就站在了原先的目的地上了。
                    //下面是对人原先地进行判断，方法同上。
                    if(map[pi][pj]==9)
                    map[pi][pj]=3;
                    else
                    map[pi][pj]=0;
                }
                //如果人前面是已经进入某目的地的箱子，而箱子前面是另一目的地。
                if(map[pi+2][pj]==3)
                {
                    map[pi+2][pj]=7;   //把箱子推入了另一目的地，自然，ID也应是。
                    map[pi+1][pj]=9;   //人站在了目的地上。
                    //下面是对人原先站立地进行判断，方法同上。
                    if(map[pi][pj]==9)
                    map[pi][pj]=3;
                    else
                    map[pi][pj]=0;
                }
            }
            break;
            case 'a':
            //如果人前面是空地。
            if(map[pi][pj-1]==0)
            {
                map[pi][pj-1]=6+0; //人往前走一步，ID为人的ID（）加上空地的ID（）。
                if(map[pi][pj]==9) //如果当前人站的位置为目的地，则ID为（即人的ID（）加上目的地ID（））。
                map[pi][pj]=3; //将人往前走一步后原地的ID修改为空地ID（）。
                else
                map[pi][pj]=0;    //否则原地ID修改为空地ID 。
            }
            //如果人前面是目的地。
            else if(map[pi][pj-1]==3)
            {
                map[pi][pj-1]=6+3; //人往前走一步，ID为人ID+目的地ID=9。
                if(map[pi][pj]==9) //如果原地也是目的地（ID为）。
                map[pi][pj]=3; //人走后把原地ID修改回目的地ID。
                else
                map[pi][pj]=0; //否则原地ID修改为为空地ID
            }
            //如果人前面是箱子。
            else if(map[pi][pj-1]==4)
            {
                //如果人前面是箱子，而箱子前面是空地。
                if (map[pi][pj-2]==0)
                {
                    map[pi][pj-2]=4; //人推箱子往前走一步，把空地ID修改为箱子ID（）
                    //下面是对箱子原地进行判断
                    if(map[pi][pj-1]==7) //如果箱子原地为目的地。
                    map[pi][pj-1]=9; //人站在箱子的原地（目的地）时该地ID应该为人的ID+目的地ID=9。
                    else
                    map[pi][pj-1]=6; //否则，人站在了原来的空地上，ID应该为+0=6。
                    //下面是对人原地进行判断
                    if(map[pi][pj]==9) //如果之前是目的地。
                    map[pi][pj]=3; //人走了之后修改回目的地ID。
                    else
                    map[pi][pj]=0; //否则就是空地。
                }
                //如果人的前面是箱子，而箱子前面是目的地。
                else if (map[pi][pj-2]==3)
                {
                    count++;
                    map[pi][pj-2]=7;   //ID为（目的地ID（）+箱子ID（）=7），表示已经把箱子推入了目的地。
                    //下面是对箱子原先位置的判断，同上。
                    if(map[pi][pj-1]==7)
                    map[pi][pj-1]=9;
                    else
                    map[pi][pj-1]=6;
                    //下面是对人原先位置进行判断，同上。
                    if(map[pi][pj]==9)
                    map[pi][pj]=3;
                    else
                    map[pi][pj]=0;
                }
            }
            //如果人前面是已经进入某目的地的箱子（ID=7）。
            else if(map[pi][pj-1]==7)
            {
                //如果人前面是已经进入某目的地的箱子,而箱子前面是空地。
                if(map[pi][pj-2]==0)
                {
                    count--;
                    map[pi][pj-2]=4;   //把箱子重新推到空地上，ID=箱子ID+空地ID=4。
                    map[pi][pj-1]=9;   //人自然而然的就站在了原先的目的地上了。
                    //下面是对人原先地进行判断，方法同上。
                    if(map[pi][pj]==9) 
                    map[pi][pj]=3;
                    else
                    map[pi][pj]=0;
                }
                //如果人前面是已经进入某目的地的箱子，而箱子前面是另一目的地。
                if(map[pi][pj-2]==3)
                {
                    map[pi][pj-2]=7;   //把箱子推入了另一目的地，自然，ID也应是。
                    map[pi][pj-1]=9;   //人站在了目的地上。
                    //下面是对人原先站立地进行判断，方法同上。
                    if(map[pi][pj]==9) 
                    map[pi][pj]=3;
                    else
                    map[pi][pj]=0;
                }
            }
            break;
            case 'd':
            //如果人前面是空地。
            if(map[pi][pj+1]==0)
            {
                map[pi][pj+1]=6+0; //人往前走一步，ID为人的ID（）加上空地的ID（）。
                if(map[pi][pj]==9) //如果当前人站的位置为目的地，则ID为（即人的ID（）加上目的地ID（））。
                map[pi][pj]=3; //将人往前走一步后原地的ID修改为空地ID（）。
                else
                map[pi][pj]=0;    //否则原地ID修改为空地ID 。      
            }
            //如果人前面是目的地。
            else if(map[pi][pj+1]==3)
            {
                map[pi][pj+1]=6+3; //人往前走一步，ID为人ID+目的地ID=9。
                if(map[pi][pj]==9) //如果原地也是目的地（ID为）。
                map[pi][pj]=3; //人走后把原地ID修改回目的地ID。
                else
                map[pi][pj]=0; //否则原地ID修改为为空地ID
            }
            //如果人前面是箱子。
            else if(map[pi][pj+1]==4)
            {
                //如果人前面是箱子，而箱子前面是空地。
                if (map[pi][pj+2]==0)
                { 
                    map[pi][pj+2]=4; //人推箱子往前走一步，把空地ID修改为箱子ID（）
                    //下面是对箱子原地进行判断
                    if(map[pi][pj+1]==7) //如果箱子原地为目的地。
                    map[pi][pj+1]=9; //人站在箱子的原地（目的地）时该地ID应该为人的ID+目的地ID=9。
                    else
                    map[pi][pj+1]=6; //否则，人站在了原来的空地上，ID应该为+0=6。
                    //下面是对人原地进行判断
                    if(map[pi][pj]==9) //如果之前是目的地。
                    map[pi][pj]=3; //人走了之后修改回目的地ID。
                    else
                    map[pi][pj]=0; //否则就是空地。
                }
                //如果人的前面是箱子，而箱子前面是目的地。
                else if (map[pi][pj+2]==3)
                { 
                    count++;
                    map[pi][pj+2]=7;   //ID为（目的地ID（）+箱子ID（）=7），表示已经把箱子推入了目的地。
                    //下面是对箱子原先位置的判断，同上。
                    if(map[pi][pj+1]==7)
                    map[pi][pj+1]=9;
                    else
                    map[pi][pj+1]=6;
                    //下面是对人原先位置进行判断，同上。
                    if(map[pi][pj]==9)
                    map[pi][pj]=3;
                    else
                    map[pi][pj]=0;
                }
            }
            //如果人前面是已经进入某目的地的箱子（ID=7）。
            else if(map[pi][pj+1]==7)
            {
                //如果人前面是已经进入某目的地的箱子,而箱子前面是空地。
                if(map[pi][pj+2]==0)
                {
                    count--;
                    map[pi][pj+2]=4;   //把箱子重新推到空地上，ID=箱子ID+空地ID=4。
                    map[pi][pj+1]=9;   //人自然而然的就站在了原先的目的地上了。
                    //下面是对人原先地进行判断，方法同上。
                    if(map[pi][pj]==9) 
                    map[pi][pj]=3;
                    else
                    map[pi][pj]=0;
                }
                //如果人前面是已经进入某目的地的箱子，而箱子前面是另一目的地。
                if(map[pi][pj+2]==3)
                {
                    map[pi][pj+2]=7;   //把箱子推入了另一目的地，自然，ID也应是。
                    map[pi][pj+1]=9;   //人站在了目的地上。
                    //下面是对人原先站立地进行判断，方法同上。
                    if(map[pi][pj]==9) 
                    map[pi][pj]=3;
                    else
                    map[pi][pj]=0;
                }
            }
            break;
        }
        if(count==8)   //如果分数达到分
        {
            draw_map(map); 
            printf("\nCongratulations!!\n"); //过关提示
            break;    //退出死循环
        }
    }
}
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
                printf(" "); //数字代表道路
                break;
                case 1:
                printf("#"); //数字代表墙壁
                break;
                case 2:
                printf(" "); //数字是游戏边框的空白部分
                break;
                case 3:
                printf("D"); //数字代表目的地
                break;
                case 4:
                printf("B"); //数字代表箱子
                break;
                case 7:
                printf("!"); //数字代表箱子进入目的地
                break;
                case 6:
                printf("&"); //数字代表人
                break;
                case 9:
                printf("&"); //数字代表人进入目的地
                break;
            }            
        }
        printf("\n");    //分行输出
    }
}

/*****************************************************************************
 *                                Christmas Tree
 *****************************************************************************/
void Christmas()
{
	int i, j;

	for (i = 0; i<3; i++) {

		for (j = 4; j>i; j--) {

			printf(" ");

		}

		for (j = 0; j<2 * i + 1; j++) {

			printf("*");

		}

		printf("\n");

	}

	for (i = 0; i<4; i++) {

		for (j = 4; j>i; j--) {

			printf(" ");

		}

		for (j = 0; j<2 * i + 1; j++) {

			printf("*");

		}

		printf("\n");

	}

	for (i = 0; i<5; i++) {

		for (j = 4; j>i; j--) {

			printf(" ");

		}

		for (j = 0; j<2 * i + 1; j++) {

			printf("*");

		}

		printf("\n");

	}

	for (i = 0; i<5; i++) {

		for (j = 0; j<3; j++) {

			printf(" ");

		}

		for (j = 0; j<3; j++) {

			printf("*");

		}

		printf("\n");

	}

	for (i = 0; i<2; i++) {

		for (j = 0; j<10; j++) {

			printf("*");

		}

		printf("\n");

	}
}

/*****************************************************************************
 *                                finger-guessing game
 *****************************************************************************/
int computer_game(int count)
{
	int result = 0;
	int num = count % 7;

	if (num == 1)
	{
		result = 1; //石头
	}
	else if (num == 2)
	{
		result = 0;//剪刀
	}
	else if (num == 3)
	{
		result = 1;//石头
	}
	else if (num == 4)
	{
		result = 2;//布
	}
	else if (num == 5)
	{
		result = 0;//剪刀
	}
	else if (num == 6)
	{
		result = 2;//布
	}
	else if (num == 0)
	{
		result = 1;//石头
	}
	return result;
}

void Guessing(fd_stdin, fd_stdout)
{
	int gamer=1;  // 玩家出拳
	int computer=0;  // 电脑出拳
	int result;  // 比赛结果
	int count = 0;
				 // 为了避免玩一次游戏就退出程序，可以将代码放在循环中
	char rdbuf[128];
	int r;
	
    printf("    *                                                                     \n");
    printf("    *     ~~~~  ** **** **  ~~~~                                          \n");
    printf("    *    ~~  **           **   ~~     Welcome!                            \n");
    printf("    *   ~~  **   ^^    ^^  **   ~~    Game-1:FingerGuessing               \n");
    printf("    *  ~~~~**     @    @    **~~~~~                                       \n");
    printf("    *      **    _______    **        RULES:                              \n");
    printf("    *      **    | * * |    **        PVE                                 \n");
    printf("    *      **    ~~~~~~~    **        Enter q to quit this program        \n");
    printf("    *       **      3      **                                             \n");
    printf("    *        **           **           GGGG    A    M  M EEEE              \n");
    printf("    *          ***********             G  __  A A   M\~M E___              \n");
    printf("    *                                  G  G  AAAAA  M  M E                 \n");
    printf("    *                                  GGGG A     A M  M EEEE              \n");
    printf("    *                                                                     \n");
    printf("    *    ********************************************************         \n");

    
	while (1) {
		count++;
        printf("\n");
		printf("Please enter your choice:\n");
		printf("A:Scissors\nB:Stone\nC:Cloth\nD:Ending game\n");

		r = read(fd_stdin, rdbuf, 70);
		rdbuf[r] = 0;
		gamer=rdbuf[0];


        if(gamer == 'Q' || gamer == 'q')
        {
            break;
        }
		else if (gamer == 'A' || gamer == 'a')
		{
			gamer = 4;
		}
		else if(gamer == 'B' || gamer == 'b')
		{
			gamer = 7;
		}
		else if (gamer == 'C' || gamer == 'c')
		{
			gamer = 10;
		}
        else if (gamer == 'D' || gamer == 'd')
        {
            break;
        }
		else
		{
            printf("Your choice is %c wrong!exiting...\n", gamer);
			break;
        }
        
		computer = computer_game(count);  // 产生随机数，得到电脑出拳
		result = (int)gamer + computer;  // gamer 为 char 类型，数学运算时要强制转换类型
		printf("Computer ");
		switch (computer)
		{
		case 0:printf("Scissors\n"); break; //4    1
		case 1:printf("Stone\n"); break; //7  2
		case 2:printf("Cloth\n"); break;   //10 3
		}
		printf("You ");
		switch (gamer)
		{
		case 4:printf("Scissors\n"); break;
		case 7:printf("Stone\n"); break;
		case 10:printf("Cloth\n"); break;
		}
		if (result == 6 || result == 7 || result == 11) printf("You Win!");
		else if (result == 5 || result == 9 || result == 10) printf("Computer Wins!\n");
		else printf("Draw!");
	}
}



/*****************************************************************************
 *                                eight queen
 *****************************************************************************/
//
//  main.c
//  EightQueen
//
//  Created by wennie on 2017/9/15.
//  Copyright © 2017年 wennie. All rights reserved.
//

//int tot = 0; //记录结果的编号
#define MAX 100
int C[MAX] = {};

void gameShow(int n){ //打印函数
    int i;
    int j;
    for(i=0;i < n;i++){
        for(j=0;j < n;j++){
            if(j == C[i]){
                printf("X");
            }
            else{
                printf("0");
            }
        }
        printf("\n");
    }
}

int search_calcu(int cur, int n,int total){ //搜索函数。计算结果数。cur当前行数；n总行数
    int i, j;
    if (cur == n){ //当前行数超过总行数
        total++;
    }
    else {         //当前行数未超过总行数
        for (i = 0; i < n; i++){ //遍历当前行的每一列
            C[cur] = i; //当前行，皇后的列数为i
            int check=0;
            for (j = 0; j < cur; j++){ //检查之前的每一行
                if ((C[cur] == C[j]) || (C[cur] - cur == C[j] - j) || (C[cur] + cur == C[j] + j)){ //同一列，同一对角线上有相同皇后
                    check++;
                }
            }
            if(check==0){ //符合条件
                total=search_calcu(cur + 1, n,total);
            }
        }
    }
    return total;
}

int search_print(int cur, int n,int total){ //搜索函数。打印结果。cur当前行数；n总行数
    int i, j;
    if (cur == n){ //当前行数超过总行数
        total++;        
        printf("Outcome:%d\n",total);
        gameShow(n);
        printf("%d\n",n);
        
    }
    else {         //当前行数未超过总行数
        for (i = 0; i < n; i++){ //遍历当前行的每一列
            C[cur] = i; //当前行，皇后的列数为i
            int check=0;
            for (j = 0; j < cur; j++){ //检查之前的每一行
                if ((C[cur] == C[j]) || (C[cur] - cur == C[j] - j) || (C[cur] + cur == C[j] + j)){ //同一列，同一对角线上有相同皇后
                    check++;
                }
            }
            if(check==0){ //符合条件
                total=search_print(cur + 1, n,total);
            }
        }
    }
    return total;
}

void game(fd_stdin, fd_stdout){
    printf(" \n");
    printf("    *                                                                     \n");
    printf("    *     ~~~~  ** **** **  ~~~~                                          \n");
    printf("    *    ~~  **           **   ~~     Welcome!                            \n");
    printf("    *   ~~  **   ^^    ^^  **   ~~    Game-2:EightQueen                   \n");
    printf("    *  ~~~~**     @    @    **~~~~~                                       \n");
    printf("    *      **    _______    **        RULES:                              \n");
    printf("    *      **    | * * |    **        PVE                                 \n");
    printf("    *      **    ~~~~~~~    **        Enter q to quit this program        \n");
    printf("    *       **      3      **                                             \n");
    printf("    *        **           **           GGGG    A    M  M EEEE              \n");
    printf("    *          ***********             G  __  A A   M\/M E___              \n");
    printf("    *                                  G  G  AAAAA  M  M E                    \n");
    printf("    *                                  GGGG A     A M  M EEEE              n");
    printf("    *                                                                     \n");
    printf("    *    ********************************************************         \n");
    char rdbuf[128];
    printf("Please input the number of queens(1~100):");
    int n = 0;
    int r = read(fd_stdin, rdbuf, 70);
    rdbuf[r] = 0;
    atoi(rdbuf, &n); //n记录皇后的数量
    
    
    printf("Guessing the number of case!\n");
    int guess;
    r = read(fd_stdin, rdbuf, 70);
    rdbuf[r] = 0;
    atoi(rdbuf, &guess); //guess记录用户猜测的结果数


    int total=0;   
    total=search_calcu(0,n,total); //计算total
    printf("The true number is %d!\n",total);
    if(guess==total){
        printf("Congratulations! You answer is right！\n\n");
    }
    else{
        printf("Sorry! You answer is wrong...\n\n");
    }
    
    total=0;
    printf("Check out all the answers? (y/n)");
    r = read(fd_stdin, rdbuf, 70);
    rdbuf[r] = 0;
    if (strcmp(rdbuf, "y") == 0){
        search_print(0,n,total);
    }
    
}




/*****************************************************************************
 *                                Carlendar
 *****************************************************************************/

int year, month, day;
int day_of_month[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
int current_year, current_month, current_day;

int judgement(int y)  // runnian
{
	if (y % 400 == 0 || (y % 100 != 0 && y % 4 == 0))
		return 1;
	else return 0;
}

int show_week(int year, int month, int day)
{
	/*
	公式:w=(y+[y/4]+[c/4]-2c+[26(m+1)/10]+d-1)%7
	*/
	int w, k;   //zhou ji
	int year_last = year % 100, c = year / 100, m = month;
	if (month == 1)
	{
		year_last -= 1;
		m = 13;
	}
	else if (month == 2)
	{
		year_last -= 1;
		m = 14;
	}
	w = (year_last + year_last / 4 + c / 4 - 2 * c + 26 * (m + 1) / 10 + day - 1); // abs  
	if (w<0)
	{
		k = (w % 7 + 7) % 7;
	}
	else k = w % 7;
	return k;
}

void print_year(int year, int fd_stdin)
{
	char rdbuf[128];
	int r = 0;
	int i, k, x, first_week;
	printf("Please input the year you want to search(like 2016):");
	//scanf("%d", &year);
	r = read(fd_stdin, rdbuf, 70);
	rdbuf[r] = 0;
	atoi(rdbuf, &year);
	printf("=======================Year %d===========================\n", year);
	printf("\n");
	if (judgement(year))
	{
		day_of_month[1] = 29;
	}
	else day_of_month[1] = 28;
	for (i = 1; i <13; i++)
	{
		first_week = show_week(year, i, 1);
		printf("=================Carlendar of year %d===========================\n", i);
		printf("Sun    Mon    Tue    Wed    Thu    Fri    Sat\n");
		for (x = 1; x <= first_week; x++)
		{
			printf("       ");
			if (x % 7 == 0) printf("\n");
		}
		for (k = 1; k <= day_of_month[i - 1]; k++)
		{
			printf("%d     ", k);
			if(k < 10)
			    printf(" ");
			if (x % 7 == 0) printf("\n");
			x++;
		}
		printf("\n");
		printf("\n");
		printf("\n");
	}
}

void print_year_month( int fd_stdin)
{
	char rdbuf[128];
	int k, x, first_week;
	int r = 0;
	printf("Please input the year:");
	do
	{
		r = read(fd_stdin, rdbuf, 70);
		rdbuf[r] = 0;
		atoi(rdbuf, &year);
		printf("Please input the month: ");
		r = read(fd_stdin, rdbuf, 70);
		rdbuf[r] = 0;
		atoi(rdbuf, &month);
		if (month<1 || month>12)
		{
			printf("The month you input is wrong, please input again!\n");
			printf("Please input the year:");
		}
	} while (1>month || month>12);
	printf("=====================Year%dMonth%d======================\n", year, month);
	if (judgement(year))
	{
		day_of_month[1] = 29;
	}
	else day_of_month[1] = 28;
	first_week = show_week(year, month, 1);
	printf("Sun    Mon    Tue    Wed    Thu    Fri    Sat\n");
	for (x = 1; x <= first_week; x++)
	{
		printf("       ");
		if (x % 7 == 0) printf("\n");
	}
	for (k = 1; k <= day_of_month[month - 1]; k++)
	{
		printf("%d     ", k);
		if (k < 10) printf(" ");
		if (x % 7 == 0) printf("\n");
		x++;
	}
	printf("\n");
}

void print(int year, int month, int day, int fd_stdin)
{
	char rdbuf[128];
	int r = 0;
	int week;
	printf("Please input the year:");
	do
	{
		r = read(fd_stdin, rdbuf, 70);
		rdbuf[r] = 0;
		atoi(rdbuf, &year);

		printf("Please input the month:");
		r = read(fd_stdin, rdbuf, 70);
		rdbuf[r] = 0;
		atoi(rdbuf, &month);

		printf("Please input the day:");
		r = read(fd_stdin, rdbuf, 70);
		rdbuf[r] = 0;
		atoi(rdbuf, &day);
		//scanf("%d %d %d", &year, &month, &day);
		if (judgement(year))
		{
			day_of_month[1] = 29;
		}
		printf("\n");
		if (day <= 0 || day >day_of_month[month - 1])
			printf("Month %d don't have day %d,please input again:", month, day);
	} while (day <= 0 || day >day_of_month[month - 1]);
	week = show_week(year, month, day);
	printf("\n");
	switch (month)  
	{
	case 1:switch (day)
	{
		default:printf("not festival");
	}break;
	case 2:switch (day)
	{
	case 14:printf("Valentines Day"); break;
	default:printf("not festival");
	}break;
	case 3:switch (day)
	{
	case 8:printf("Women's Day"); break;
	case 12:printf("Arbor Day"); break;
	default:printf("not festival");
	}break;
	case 4:switch (day)
	{
	case 1:printf("April Fools Day"); break;
	case 5:printf("Tomb-sweeping Day"); break;
	default:printf("not festival");
	}break;
	case 5:switch (day)
	{
	case 1:printf("Labor Day"); break;
	case 4:printf("Chinese Youth Day");
	default:printf("not festival");
	}break;
	case 6:switch (day)
	{
	case 1:printf("Children's Day"); break;
	default:printf("not festival");
	}break;
	case 8:switch (day)
	{
	case 1:printf("the Army's Day"); break;
	default:printf("not festival");
	}break;
	case 9:switch (day)
	{
	case 10:printf("Teacher's Day"); break;
	default:printf("not festival");
	}break;
	case 10:switch (day)
	{
	case 1:printf("National Day"); break;
	case 31:printf("Helloween Day"); break;
	default:printf("not festival");
	}break;
	case 12:switch (day)
	{
	case 25:printf("Christmas Day"); break;
	default:printf("not festival");
	}break;
	}
	printf("\n");
	printf("%d %d %d is:", year, month, day);
	switch (week)  //zhouji
	{
	case 0:printf("Sunday"); break;
	case 1:printf("Monday"); break;
	case 2:printf("Tuesday"); break;
	case 3:printf("Wednesday");; break;
	case 4:printf("Thursday"); break;
	case 5:printf("Friday"); break;
	case 6:printf("Saturday"); break;
	}
	printf("\n");
}

void Carlendar( int fd_stdin,int fd_stdout)
{
	char rdbuf[128];
	int r = 0;
	int choice, year=1000, month=0, day=0, flag = 1;
	char c, k;
	for (; 1;)  
	{
        printf("    *                                                                     \n");
        printf("    *     ~~~~  ** **** **  ~~~~                                          \n");
        printf("    *    ~~  **           **   ~~     Welcome!                            \n");
        printf("    *   ~~  **   ^^    ^^  **   ~~    Carlendar!                          \n");
        printf("    *  ~~~~**     @    @    **~~~~~                                       \n");
        printf("    *      **    _______    **        Please Choose:                      \n");
        printf("    *      **    | * * |    **        1.Carlendar of a year               \n");
        printf("    *      **    ~~~~~~~    **        2.Carlender of a month              \n");
        printf("    *       **      3      **         3.Find a day                        \n");
        printf("    *        **           **          0.exit                              \n");
        printf("    *          ***********                                                \n");
        printf("    *                                                                     \n");
        printf("    *                                                                     \n");
        printf("    *    ********************************************************         \n");
        printf("\n");
		printf("Please input your choose:   ");
		do
		{
			if (flag)
			{
				r = read(fd_stdin, rdbuf, 70);
				rdbuf[r] = 0;
				if (strcmp(rdbuf, "clear") == 0||strcmp(rdbuf, "Clear") == 0)
				{
					clear();
                    printf("    *                                                                     \n");
                    printf("    *     ~~~~  ** **** **  ~~~~                                          \n");
                    printf("    *    ~~  **           **   ~~     Welcome!                            \n");
                    printf("    *   ~~  **   ^^    ^^  **   ~~    Carlendar!                          \n");
                    printf("    *  ~~~~**     @    @    **~~~~~                                       \n");
                    printf("    *      **    _______    **        Please Choose:                      \n");
                    printf("    *      **    | * * |    **        1.Carlendar of a year               \n");
                    printf("    *      **    ~~~~~~~    **        2.Carlender of a month              \n");
                    printf("    *       **      3      **         3.Find a day                        \n");
                    printf("    *        **           **          0.exit                              \n");
                    printf("    *          ***********                                                \n");
                    printf("    *                                                                     \n");
                    printf("    *                                                                     \n");
                    printf("    *    ********************************************************         \n");
                    printf("\n");
					printf("Please input your choose:   ");
					continue;
				}
				c = rdbuf[0];
				printf("\n");
				printf("Are you sure choose %c? (y/Y)or(n/N):   ", c);
			}
			if (flag)
			{
				r = read(fd_stdin, rdbuf, 70);
				rdbuf[r] = 0;
				k = rdbuf[0];
			}
			if (k == 'y' || k == 'Y')
			{
				printf("\n");
				if (c == '\n')
				{
					printf("\n");
					printf("input is wrong,please input the number\n");
					printf("\n");
					printf("Please input your choose:   ");
				}
				else
				{
					choice = c - '0';
					if (choice < 0 || choice > 3)
					{
						printf("input is wrong,please input number between 1-3\n");
					}
					else break;
				}
			}
			else if (k == 'n' || k == 'N')
			{
				printf("\n");
				printf("You choose NO,please input again:   ");
				flag = 1;
			}
			if ((k != 'y'&& k != 'Y'&& k != 'n'&& k != 'N'))
			{
				printf("\n");
				printf("please input y/n:   ");
				flag = 0;
				if (flag == 0)
				{
					r = read(fd_stdin, rdbuf, 70);
					rdbuf[r] = 0;
					k = rdbuf[0];
					printf("\n");
					//getchar();
				}
			}
		} while (1);
		if (choice == 1)
		{
			print_year(year,fd_stdin);
			
		}
		else if (choice == 2)
		{
			print_year_month(fd_stdin);
			
		}
		else if (choice == 3)
		{
			print(year, month, day,fd_stdin);
			
		}
		
		else if (choice == 0)
			break;
		else
		{
			printf("input is wrong,please input the number\n");
		}
	}
}

/*===========================================================================
                                 maze
============================================================================*/

#define MAZE_LENGTH 5
#define MAZE_HEIGHT 5
/*' ' for accessble, '*'for blocked*/
char maze[MAZE_HEIGHT][MAZE_LENGTH];
int posHeight = 0;
int posLength = 0;

struct point{
    int posLength;
    int posHeight;
    int able;
}Point;

struct point array[MAZE_LENGTH * MAZE_HEIGHT];
int l = 0;

void Push(int height, int length){
    array[l].posHeight = height;
    array[l].posLength = length;
    array[l].able = 1;
    maze[height][length] = '*';
    l++;
}

int Pop(){
    if(l == 0)
        return 0;
    else{
        l--;
        array[l].posLength = -1;
        array[l].posHeight = -1;
        array[l].able = 0;
        return 1;
    }
}

void InitArray(){
    for(int i = 0; i < MAZE_HEIGHT * MAZE_LENGTH; i++){
        array[i].able = 0;
    }
}

int MazeStart(){
   for(int i = 0; i < MAZE_HEIGHT; i++){
       if(maze[i][0] == ' '){
           Push(i, 0);
           while(1){
                if(l == 0)
                    break;
                else{
                    if(array[l - 1].posHeight != 0 && maze[array[l - 1].posHeight - 1][array[l - 1].posLength] == ' '){
                        Push(array[l - 1].posHeight - 1, array[l - 1].posLength);
                    }
                    else if(maze[array[l - 1].posHeight][array[l - 1].posLength + 1] == ' '){
                        Push(array[l - 1].posHeight, array[l - 1].posLength + 1);
                        if(array[l - 1].posLength == MAZE_LENGTH - 1)
                            break;
                    }
                    else if(maze[array[l - 1].posHeight + 1][array[l - 1].posLength] == ' '){
                        Push(array[l - 1].posHeight + 1, array[l - 1].posLength);
                    }
                    else if(array[l - 1].posLength != 0 && maze[array[l - 1].posHeight][array[l - 1].posLength - 1] == ' '){
                        Push(array[l - 1].posHeight, array[l - 1].posLength - 1);
                    }else{
                        Pop();
                    }
                }
           }
           if(l == 0)
                printf("no way");
            else{
                for(int j = 0; j < l; j++){
                    printf("(%d,%d)", array[j].posHeight, array[j].posLength);
                }
                printf("\n");
            }
           return 1;
       }
   } 
   return 0;
}

#define MAX_FILE_PER_LAYER 10
#define MAX_FILE_NAME_LENGTH 20
#define MAX_CONTENT_ 50
#define MAX_FILE_NUM 100

//文件ID计数器
int fileIDCount = 0;
int currentFileID = 0;

struct fileBlock{
    int fileID;
    char fileName[MAX_FILE_NAME_LENGTH];
    int fileType; //0 for txt, 1 for folder
    char content[MAX_CONTENT_];
    int fatherID;
    int children[MAX_FILE_PER_LAYER];
    int childrenNumber;
} ;
struct fileBlock blocks[MAX_FILE_NUM];
int IDLog[MAX_FILE_NUM];

void initFileBlock(int fileID, char * fileName, int fileType){
    blocks[fileID].fileID = fileID;
    strcpy(blocks[fileID].fileName, fileName);
    blocks[fileID].fileType = fileType;
    blocks[fileID].fatherID = currentFileID;
    blocks[fileID].childrenNumber = 0;
}
void toStr3(char * temp, int i){
    if(i / 100 < 0)
        temp[0] = (char)48;
    else
        temp[0] = (char)(i / 100 + 48);
    if((i % 100 )/10 < 0)
        temp[1] = '0';
    else
        temp[1] = (char)((i % 100 )/10 + 48);
    temp[2] = (char)(i % 10 + 48);
}

void WriteDisk(int len){
    char temp[MAX_FILE_NUM * 150 + 103];
    int i = 0;
    temp[i] = '^';
    i++;
    toStr3(temp + i, fileIDCount);
    i = i + 3;
    temp[i] = '^';
    i++;
    for(int j = 0; j < MAX_FILE_NUM; j++){
        if(IDLog[j] == 1){
            toStr3(temp + i, blocks[j].fileID);
            i = i + 3;
            temp[i] = '^';
            i++;
            for(int h = 0; h < strlen(blocks[j].fileName); h++){
                temp[i + h] = blocks[j].fileName[h];
                if(blocks[j].fileName[h] == '^')
                temp[i + h] = (char)1;
            }
            i = i + strlen(blocks[j].fileName);
            temp[i] = '^';
            i++;
            temp[i] = (char)(blocks[j].fileType + 48);
            i++;
            temp[i] = '^';
            i++;
            for(int h = 0; h < strlen(blocks[j].content); h++){
                temp[i + h] = blocks[j].content[h];
                if(blocks[j].content[h] == '^')
                    temp[i + h] = (char)1;
            }
            i = i + strlen(blocks[j].content);
            temp[i] = '^';
            i++;
            toStr3(temp + i, blocks[j].fatherID);
            i = i + 3;
            temp[i] = '^';
            i++;
            for(int m = 0; m < MAX_FILE_PER_LAYER; m++){
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

int toInt(char * temp){
    int result = 0;
    for(int i = 0; i < 3; i++)
        result = result * 10 + (int)temp[i] - 48;
    return result;
}

int ReadDisk(){
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
    for(int i = 0; i < fileIDCount ; i++){
        int ID = toInt(bufr + r);
        IDLog[ID] = 1;
        blocks[ID].fileID = ID;
        r = r + 4;
        for(int i = 0; i < MAX_FILE_NAME_LENGTH; i++){
            if(bufr[r] == '^')
                break;
            else if(bufr[r] == (char)1)
                blocks[ID].fileName[i] = '^';
            else
                blocks[ID].fileName[i] = bufr[r];
            r++;
        }
        r++;
        blocks[ID].fileType = (int)bufr[r] - 48;
        r = r + 2;
        for(int j = 0; j < MAX_CONTENT_; j++){
            if(bufr[r] == '^')
                break;
            else if(bufr[r] == (char)1)
                blocks[ID].content[j] = '^';
            else
                blocks[ID].content[j] = bufr[r];
            r++;
        }
        r++;
        blocks[ID].fatherID = toInt(bufr + r);
        r = r + 4;
        for(int j = 0; j < MAX_FILE_PER_LAYER; j++){
            blocks[ID].children[j] = toInt(bufr + r);
            r = r + 3;
        }
        r++;
        blocks[ID].childrenNumber = toInt(bufr + r);
        r = r + 4;
    }
    return n1;
}

void FSInit(){

    for(int i  = 0; i < MAX_FILE_NUM; i++){
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

int CreateFIle(char * fileName, int fileType) {
    if (blocks[currentFileID].childrenNumber == MAX_FILE_PER_LAYER) {
        printf("Sorry you cannot add more files in this layer");
        return 0;
    }
    else {
        for(int i = 0; i < blocks[currentFileID].childrenNumber; i++){
            if(strcmp(blocks[blocks[currentFileID].children[i]].fileName, fileName) == 0){
                printf("You have a file of same name!");
                return 0;
            }
        }
        fileIDCount++;
        int target = 0;
        for(int i = 0; i < MAX_FILE_NUM; i++){
            if(IDLog[i] == 0){
                target = i;
                break;
            }
        }
        initFileBlock(target, fileName, fileType);
        blocks[currentFileID].children[blocks[currentFileID].childrenNumber] = target;
        blocks[currentFileID].childrenNumber++;
        printf("Create file s% successful!", fileName);
        IDLog[target] = 1;
        return 1;
    }
}

void showFileList(){
    printf("-----------------------------------------\n");
    for(int i = 0; i < blocks[currentFileID].childrenNumber; i++){
        printf("%s", blocks[blocks[currentFileID].children[i]].fileName);
        if(blocks[blocks[currentFileID].children[i]].fileType == 0){
            printf(".txt");
        }
        else{
            printf(".fd ");
        }
        printf(" id:%d\n", blocks[blocks[currentFileID].children[i]].fileID);
    }
    printf("-----------------------------------------");
}

int SearchFile(char * name){
    for (int i = 0; i < blocks[currentFileID].childrenNumber; i++){
        if(strcmp(name, blocks[blocks[currentFileID].children[i]].fileName) == 0){
            return blocks[currentFileID].children[i];
        }
    }
    return -2;
}

void ReturnFile(int ID){
    currentFileID = blocks[ID].fatherID;
}

void DeleteFile(int ID){
    if(blocks[ID].childrenNumber > 0){
        for (int i = 0; i < blocks[ID].childrenNumber; i++){
            DeleteFile(blocks[blocks[ID].children[i]].fileID);
        }
    }
    IDLog[ID] = 0;
    blocks[ID].fileID = -2;
    blocks[ID].childrenNumber = 0;
    for (int i = 0; i < MAX_CONTENT_; i++)
        blocks[ID].content[i] = '\0';
    for(int i = 0; i < MAX_FILE_NAME_LENGTH; i++)
        blocks[ID].fileName[i] = '\0';
    blocks[ID].fileType = -1;
    for(int i = 0; i < MAX_FILE_PER_LAYER; i++)
        blocks[ID].children[i] = -1;
    blocks[ID].fatherID = -2;
    fileIDCount--;
}

void ShowMessage(){
    printf("    *    ********************************************************         \n");
    printf("    *                    PIGGY OS's File Manager                          \n");
    printf("    *              touch ------ create a new text file                    \n");
    printf("    *              mkdir ------ create a new folder                       \n");
    printf("    *              ls --------- show the files of this level              \n");
    printf("    *              cd --------- open a folder                             \n");
    printf("    *              cd .. ------ return to the superior level              \n");
    printf("    *              rm --------- delete a file/folder                      \n");
    printf("    *              sv --------- save changes                              \n");
    printf("    *              help ------- Instruction prompt                        \n");
    printf("    *    ********************************************************         \n");
}

/*======================================================================*
                            Calculator
 *======================================================================*/
 #define FORMULA_LENGTH_ 500
 
 char Input[FORMULA_LENGTH_];
 char formula[FORMULA_LENGTH_];
 int token = 0;
 int InitFormula();
 double level1();
 double level2();
 double level3();
 int IsAllowed(char temp);
 int IsBlank(char c);
 double Transfer();

 int InitFormula(){
    int t = 0, i = 0;
    int type = 0;
    for(int i = 0; i < strlen(Input); i++){
        if(!IsAllowed(Input[i]))
            return 0;
        else if(IsBlank(Input[i]))
            continue;
        else if(Input[i] == '\0')
            break;
        else {
            formula[t] = Input[i];
            t++;
        }
        if(Input[i] == '.')
            type = 1;
    }
    if(type == 0)
        return 1;
    else
        return 2;
}

//+-
double level1(){
    double temp = level2();
    while(formula[token] == '+' || formula[token] == '-'){
        switch(formula[token]){
            case '+': token++;
                temp  = temp + level2();
                break;
            case '-': token++;
                temp = temp - level2();
                break;
        }
    }
    return temp;
}

//*/
double level2(){
    double temp = level3();
    while(formula[token] == '*' || formula[token] == '/'){
        switch(formula[token]){
            case '*': token++;
                temp = temp * level3();
                break;
            case '/': token++;
                temp = temp / level3();
                break;
        }
    }
    return temp;
}

double level3(){
    double temp;
    if(formula[token] == '('){
        token++;
        temp = level1();
        token++;
    }
    else if((int)formula[token] >= 48 && (int)formula[token] <= 57){
        temp = Transfer();
    }
    return temp;
}

int IsAllowed(char temp){
    if(temp == '+' || temp == '-'||temp == '*' || temp == '/'
        ||(int)temp >= 48 && (int)temp <= 57
        ||temp == '(' || temp == ')' || temp == '.'
        || IsBlank(temp))
        return 1;
    else
        return 0;
}

int IsBlank(char c){
    if(c == ' ' || c == '\t' || c == '\n')
        return 1;
    else 
        return 0;
}

double Transfer(){
    double temp = 0;
    while((int)formula[token] >= 48 && (int)formula[token] <= 57){
        temp = temp * 10 + (int)formula[token] - 48;
        token++;
    }
    if(formula[token] == '.'){
        token++;
        while((int)formula[token] >= 48 && (int)formula[token] <= 57 && token < strlen(Input)){
            double add = (int)formula[token] - 48;
                add = add * 0.1;
            temp = temp + add;
            token++;
        }
    }
    return temp;
}



// end of my code

/*======================================================================*
                            kernel_main
 *======================================================================*/
PUBLIC int kernel_main()
{
	disp_str("-----\"kernel_main\" begins-----\n");

	struct task* p_task;
	struct proc* p_proc= proc_table;
	char* p_task_stack = task_stack + STACK_SIZE_TOTAL;
	u16   selector_ldt = SELECTOR_LDT_FIRST;
        u8    privilege;
        u8    rpl;
	int   eflags;
	int   i, j;
	int   prio;
	for (i = 0; i < NR_TASKS+NR_PROCS; i++) {
	        if (i < NR_TASKS) {     /* 任务 */
                        p_task    = task_table + i;
                        privilege = PRIVILEGE_TASK;
                        rpl       = RPL_TASK;
                        eflags    = 0x1202; /* IF=1, IOPL=1, bit 2 is always 1 */
			prio      = 15;
                }
                else {                  /* 用户进程 */
                        p_task    = user_proc_table + (i - NR_TASKS);
                        privilege = PRIVILEGE_USER;
                        rpl       = RPL_USER;
                        eflags    = 0x202; /* IF=1, bit 2 is always 1 */
			prio      = 5;
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
		p_proc->regs.cs	= (0 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.ds	= (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.es	= (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.fs	= (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.ss	= (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.gs	= (SELECTOR_KERNEL_GS & SA_RPL_MASK) | rpl;

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

	p_proc_ready	= proc_table;

	init_clock();
        init_keyboard();

	restart();

	while(1){}
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


	int fd_stdin  = open(tty_name, O_RDWR);
	assert(fd_stdin  == 0);
	int fd_stdout = open(tty_name, O_RDWR);
	assert(fd_stdout == 1);

//	char filename[MAX_FILENAME_LEN+1] = "zsp01";
	const char bufw[80] = {0};

	clear();
	long int ii = 9999;
	Christmas();
printf("                                                                           \n");while(ii-->0);ii=9999;

printf("    ##      ###\n");
printf("    ##      ###\n");
printf("    ##      ###\n");
printf("    ##      ###\n");
printf("    ##      ###\n");
printf("    ##      ###\n");
printf("    ##      ###\n");
printf("    ##      ###\n");
printf("    ###########\n");
printf("    ###########\n");
printf("    ##      ###\n");
printf("    ##      ###\n");
printf("    ##      ###\n");
printf("    ##      ###\n");
printf("    ##      ###\n");
printf("    ##      ###\n");
printf("    ##      ###\n");
printf("    ##      ###\n");
printf("               \n\n");
		milli_delay(1000);
		clear();
printf("    ##      ###              \n");
printf("    ##      ###              \n");
printf("    ##      ###              \n");
printf("    ##      ###              \n");
printf("    ##      ###              \n");
printf("    ##      ###              \n");
printf("    ##      ###      #####   \n");
printf("    ##      ###     ######## \n");
printf("    ###########    ###   ####\n");
printf("    ###########   ###     ###\n");
printf("    ##      ###  ###      ###\n");
printf("    ##      ###  ############\n");
printf("    ##      ###  ###         \n");
printf("    ##      ###  ###         \n");
printf("    ##      ###   ###     ###\n");
printf("    ##      ###   ####    ###\n");
printf("    ##      ###    ####  ####\n");
printf("    ##      ###     ######## \n");
printf("                      ####   \n\n");
		milli_delay(1000);
		clear();
printf("    ##      ###                     ###\n");
printf("    ##      ###                     ###\n");
printf("    ##      ###                     ###\n");
printf("    ##      ###                     ###\n");
printf("    ##      ###                     ###\n");
printf("    ##      ###                     ###\n");
printf("    ##      ###      #####          ###\n");
printf("    ##      ###     ########        ###\n");
printf("    ###########    ###  #####       ###\n");
printf("    ###########   ###     ###       ###\n");
printf("    ##      ###  ###      ###       ###\n");
printf("    ##      ###  ############       ###\n");
printf("    ##      ###  ###                ###\n");
printf("    ##      ###  ###                ###\n");
printf("    ##      ###   ###     ###       ###\n");
printf("    ##      ###   ####    ###       ###\n");
printf("    ##      ###    ####  ####       ###\n");
printf("    ##      ###     ########        ###\n");
printf("                      ####          ###\n\n");
		milli_delay(1000);
		clear();

printf("    ##      ###                     ###           ###\n");
printf("    ##      ###                     ###           ###\n");
printf("    ##      ###                     ###           ###\n");
printf("    ##      ###                     ###           ###\n");
printf("    ##      ###                     ###           ###\n");
printf("    ##      ###                     ###           ###\n");
printf("    ##      ###      #####          ###           ###\n");
printf("    ##      ###     ########        ###           ###\n");
printf("    ###########    ###  #####       ###           ###\n");
printf("    ###########   ###     ###       ###           ###\n");
printf("    ##      ###  ###      ###       ###           ###\n");
printf("    ##      ###  ############       ###           ###\n");
printf("    ##      ###  ###                ###           ###\n");
printf("    ##      ###  ###                ###           ###\n");
printf("    ##      ###   ###     ###       ###           ###\n");
printf("    ##      ###   ####    ###       ###           ###\n");
printf("    ##      ###    ####  ####       ###           ###\n");
printf("    ##      ###     ########        ###           ###\n");
printf("                      ###           ###           ###\n\n");
		milli_delay(1000);
		clear();
printf("    ##      ###                     ###           ###                      \n");
printf("    ##      ###                     ###           ###                      \n");
printf("    ##      ###                     ###           ###                      \n");
printf("    ##      ###                     ###           ###                      \n");
printf("    ##      ###                     ###           ###                      \n");
printf("    ##      ###                     ###           ###                      \n");
printf("    ##      ###      #####          ###           ###           ####       \n");
printf("    ##      ###     ########        ###           ###         ########     \n");
printf("    ###########    ###  #####       ###           ###        ###    ###    \n");
printf("    ###########   ###     ###       ###           ###       ###      ###   \n");
printf("    ##      ###  ###      ###       ###           ###       ###      ###   \n");
printf("    ##      ###  ############       ###           ###       ###      ###   \n");
printf("    ##      ###  ###                ###           ###       ###      ###   \n");
printf("    ##      ###  ###                ###           ###       ###      ###   \n");
printf("    ##      ###   ###     ###       ###           ###       ###      ###   \n");
printf("    ##      ###   ####    ###       ###           ###       ####    ####   \n");
printf("    ##      ###    ####  ####       ###           ###        ####  ####    \n");
printf("    ##      ###     ########        ###           ###         ########     \n");
printf("                      ###           ###           ###           ####       \n\n");


	startAnimate();
	
	clear();
    printf("    *                                                              \n");
    printf("    *     ~~~~  ** **** **  ~~~~                                   \n");
    printf("    *    ~~  **           **   ~~      Welcome!                    \n");
    printf("    *   ~~  **   ^^    ^^  **   ~~     PIGGY OS                    \n");
    printf("    *  ~~~~**     @    @    **~~~~~                                \n");
    printf("    *      **    _______    **      by 1552535 Hu Jiaxin           \n");
    printf("    *      **    | o o |    **      by 1552622 Wang Ning           \n");
    printf("    *      **    ~~~~~~~    **                                     \n");
    printf("    *       **      3      **                                      \n");
    printf("    *        **           **                                       \n");
    printf("    *          ***********                                         \n");
    printf("    *                         Input [help] for more information    \n");
    printf("    *                                                              \n");
    printf("    *    ********************************************************  \n");

	while (1) {           
		printf("\n\n\n command:");

		int r = read(fd_stdin, rdbuf, 70);
		rdbuf[r] = 0;
		//show();
        if (strcmp(rdbuf, "ProcessManage") == 0||strcmp(rdbuf, "processmanage") == 0)
        {
            ProcessManage();
			continue;
        }
		else if (strcmp(rdbuf, "FileManage") == 0||strcmp(rdbuf, "filemanage") == 0)
		{
			printf("File Manager is already running on CONSOLE-1 ! \n");
			continue;

		}else if (strcmp(rdbuf, "Help") == 0||strcmp(rdbuf, "help") == 0)
		{
			help();
		}
		else if (strcmp(rdbuf, "timer") == 0)
		{
			timer(fd_stdin, fd_stdout);
		}
		else if(strcmp(rdbuf, "G2") == 0||strcmp(rdbuf, "g2") == 0){
			clear();
			game(fd_stdin, fd_stdout);
		}
		
		else if(strcmp(rdbuf,"Carlendar") == 0||strcmp(rdbuf,"carlendar") == 0)
		{
			clear();
			Carlendar(fd_stdin, fd_stdout);
		}
		else if(strcmp(rdbuf,"Calculator") == 0||strcmp(rdbuf,"calculator") == 0)
		{
            clear();
            printf("    *                                                                     \n");
            printf("    *     ~~~~  ** **** **  ~~~~                                          \n");
            printf("    *    ~~  **           **   ~~     Welcome!                            \n");
            printf("    *   ~~  **   ^^    ^^  **   ~~    Calculator!                         \n");
            printf("    *  ~~~~**     @    @    **~~~~~                                       \n");
            printf("    *      **    _______    **        RULES:                              \n");
            printf("    *      **    | * * |    **        Use + - * / ( and )in the formula   \n");
            printf("    *      **    ~~~~~~~    **        Enter q to quit this program        \n");
            printf("    *       **      3      **                                             \n");
            printf("    *        **           **                                              \n");
            printf("    *          ***********                                                \n");
            printf("    *                                                                     \n");
            printf("    *                                                                     \n");
            printf("    *    ********************************************************         \n");

            while(1){
                for(int i = 0; i < FORMULA_LENGTH_; i++){
                    Input[i] = '\0';
                    formula[i] = '\0';
                }
                token = 0;
                int r = read(fd_stdin, Input, FORMULA_LENGTH_);
                Input[r] = 0;
                if(strcmp(Input, "q") == 0){
                    break;
                }
                int test = InitFormula();
                if(!test){
                    printf("error!");
                    continue;
                }
                double result = level1();
                if(test == 1)
                    printf("result is : %d\n",(int)result);
                else if(test == 2){
                    char re[5];
                    for(int i = 0; i < 5; i++){
                        int t = 1;
                        for(int j = 0; j <= i; j++)
                            t = t * 10;
                        re[i] = (char)(result * t) % 10 + 48;
                    }
                    printf("result is : %d.%s\n",(int)result, re);
                }
            }
		}
		else if(strcmp(rdbuf,"G1") == 0)
		{
				Guessing(fd_stdin, fd_stdout);
		}
		else if(strcmp(rdbuf,"G4") == 0)
		{
				boxman(fd_stdin, fd_stdout);
        }
        else if(strcmp(rdbuf, "G3") == 0|| strcmp(rdbuf, "g3") == 0)
        {
            clear();
            printf("\n---------------------Welcome to the maze world-----------------------\n");
			printf("------ You can use * on behalf of wall and ' ' on behalf of way  ------\n");
			printf("- Input 5 * 5 array to create a map, and teh way will be shown behind -\n");
            while(1){
                for(int i = 0; i < MAZE_HEIGHT; i++){
                    char ttemp[MAZE_LENGTH];
                    int r = read(fd_stdin, ttemp, MAZE_LENGTH);
                    ttemp[r] = '\0';
                    for(int j = 0; j < MAZE_LENGTH; j++)
                        maze[i][j] = ttemp[j];
                }
                InitArray();
                if(!MazeStart())
                    printf("no entry!\n");
                printf("try again? y/n\n");
                char judge[10];
                int r = read(fd_stdin, judge,10);
                judge[r] = '\0';
                if(strcmp(judge, "y") != 0){
                    clear();
                    break;
                }
            }
        }
		else if (strcmp(rdbuf, "Clear") == 0)
		{
			clear();
            printf("    *                                                              \n");
            printf("    *     ~~~~  ** **** **  ~~~~                                    \n");
            printf("    *    ~~  **           **   ~~     Welcome!                    \n");
            printf("    *   ~~  **   ^^    ^^  **   ~~    PIGGY OS                    \n");
            printf("    *  ~~~~**     @    @    **~~~~~                              \n");
            printf("    *      **    _______    **     by 1552535 Hu Jiaxin           \n");
            printf("    *      **    | * * |    **     by 1552622 Wang Ning           \n");
            printf("    *      **    ~~~~~~~    **                                    \n");
            printf("    *       **      3      **                                     \n");
            printf("    *        **           **                                       \n");
            printf("    *          ***********                                         \n");
            printf("    *                        Input [help] for more information    \n");
            printf("    *                                                             \n");
            printf("    *    ******************************************************** \n");
            
		}
		else
        {
            printf("    *                                                              \n");
            printf("    *     ~~~~  ** **** **  ~~~~                                   \n");
            printf("    *    ~~  **           **   ~~                                  \n");
            printf("    *   ~~  **   ^^    ^^  **   ~~     Sorry!                       \n");
            printf("    *  ~~~~**     @    @    **~~~~~    Command Not Found!           \n");
            printf("    *      **    _______    **                                     \n");
            printf("    *      **    | ! ! |    **         Please Check!               \n");
            printf("    *      **    ~~~~~~~    **                                    \n");
            printf("    *       **      3      **                                     \n");
            printf("    *        **           **                                       \n");
            printf("    *          ***********                                         \n");
            printf("    *                        Input [help] for more information    \n");
            printf("    *                                                             \n");
            printf("    *    ******************************************************** \n");
        }
			
	}
}

/*======================================================================*
                               TestB
 *======================================================================*/
void TestB()
{
    char tty_name[] = "/dev_tty1";
    int fd_stdin  = open(tty_name, O_RDWR);
    assert(fd_stdin  == 0);
    int fd_stdout = open(tty_name, O_RDWR);
    assert(fd_stdout == 1);

    char rdbuf[128];
    char cmd[8];
    char filename[120];
    char buf[1024];
    int m,n;
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
        assert(fd_stdin  == 0);
        char target[10];
        for(int i = 0; i <= 1 && i < r; i++) {
            target[i] = rdbuf[i];
        }
        if(rdbuf[0] == 't' && rdbuf[1] == 'o' && rdbuf[2] == 'u' && rdbuf[3] == 'c' && rdbuf[4] == 'h') {
            if(rdbuf[5] != ' '){
                printf("enter the name like this : create XXX");
                continue;
            }
            for(int i = 0; i < MAX_FILE_NAME_LENGTH && i < r - 3; i++){
                _name[i] = rdbuf[i + 6];
            }
            CreateFIle(_name, 0);
        }else if(rdbuf[0] == 'm' && rdbuf[1] == 'k' && rdbuf[2] == 'd' && rdbuf[3] == 'i' && rdbuf[4] == 'r'){
            if(rdbuf[5]!= ' '){
                printf("enter the name like this : mkdir XXX");
                continue;
            }
            char N[MAX_FILE_NAME_LENGTH];
            for(int i = 0; i < MAX_FILE_NAME_LENGTH && i < r - 3; i++){
                _name[i] = rdbuf[i + 6];
            }
            CreateFIle(_name, 1);
        }else if(strcmp(target, "ls") == 0){
            showFileList();
        }else if(strcmp(target, "cd") == 0){
            if(rdbuf[2] == ' ' && rdbuf[3] == '.' && rdbuf[4] == '.'){
                ReturnFile(currentFileID);
                continue;
            }
            else if(rdbuf[2]!= ' '){
                printf("enter the name like this : mt XXX");
                continue;
            }
            for(int i = 0; i < MAX_FILE_NAME_LENGTH && i < r - 3; i++){
                _name[i] = rdbuf[i + 3];
            }
            printf("name: %s\n", _name);
            int ID = SearchFile(_name);
            if(ID >= 0){
                if(blocks[ID].fileType == 1){
                    currentFileID = ID;
                    continue;
                }
                else if(blocks[ID].fileType == 0){
                    while(1){
                        printf("input the character representing the method you want to operate:"
                                       "\nu --- update"
                                       "\nd --- detail of the content"
                                       "\nq --- quit\n");
                        int r = read(fd_stdin, rdbuf, 70);
                        rdbuf[r] = 0;
                        if(strcmp(rdbuf, "u") == 0){
                            printf("input the text you want to write:\n");
                            int r = read(fd_stdin, blocks[ID].content, MAX_CONTENT_);
                            blocks[ID].content[r] = 0;
                        }else if(strcmp(rdbuf, "d") == 0){
                            printf("--------------------------------------------"
                                           "\n%s\n-------------------------------------\n", blocks[ID].content);
                        }
                        else if(strcmp(rdbuf, "q") == 0){
                            printf("would you like to save the change? y/n");
                            int r = read(fd_stdin, rdbuf, 70);
                            rdbuf[r] = 0;
                            if(strcmp(rdbuf, "y") == 0){
                                printf("save changes!");
                            }
                            else{
                                printf("quit without changing");
                            }
                            break;
                        }
                    }
                }
            } else
                printf("No such file!");
        }else if(strcmp(target, "rm") == 0){
            if(rdbuf[2]!= ' '){
                printf("enter the name like this : rm XXX");
                continue;
            }
            for(int i = 0; i < MAX_FILE_NAME_LENGTH && i < r - 3; i++){
                _name[i] = rdbuf[i + 3];
            }
            int ID = SearchFile(_name);
            if(ID >= 0){
                printf("delete successfully!\n");
                DeleteFile(ID);
                for (int i = 0; i < blocks[currentFileID].childrenNumber; i++){
                    if(ID == blocks[currentFileID].children[i]){
                        for(int j = i + 1; j < blocks[currentFileID].childrenNumber; j++){
                            blocks[currentFileID].children[i] = blocks[currentFileID].children[j];
                        }
                        blocks[currentFileID].childrenNumber--;
                        break;
                    }
                }
            } else
                printf("No such file!\n");
        }else if(strcmp(target, "sv") == 0){
            WriteDisk(1000);
        }else if(strcmp(rdbuf, "help") == 0){
            printf("\n\n");
            ShowMessage();
            printf("\n\n");
        }else{
            printf("Wrong instruction!");
        }
    }

	assert(0); /* never arrive here */
}

/*****************************************************************************
 *                                TestC
 *****************************************************************************/
void TestC()
{
	//ProcessManage();
	spin("Testc");
}

void timer(int fd_stdin,int fd_stdout)
{

	
		printf("How many seconds do you want to set?\n");
		char rdbuf[128];
		int tmp = 0;
		int r = read(fd_stdin, rdbuf, 70);
		rdbuf[r] = 0;
		atoi(rdbuf,&tmp);

		int i = tmp;
		printf("the time you set is %d\nNow begin....\n",tmp);
		for(;i >=0;i--){
			printf("%d left\n",i);
			milli_delay(10000);
		}
		printf("Time up!");
}

void startAnimate(){
	long int ii = 9999;
		printf("System is loading...");
	

	while(ii-->0);ii=2999999;
	printf("................");
	while(ii-->0);ii=2999999;
	printf("......");
	while(ii-->0);ii=999999;
	printf("..");
	while(ii-->0);ii=6999999;
	printf("..............................");
	while(ii-->0);ii=9999;
	printf("......");
	while(ii-->0);ii=999999;
	printf("..");
	while(ii-->0);ii=9999;
	printf("..");
	while(ii-->0);ii=999;
	printf(".......");
}

/*****************************************************************************
 *                                panic
 *****************************************************************************/
PUBLIC void panic(const char *fmt, ...)
{
	int i;
	char buf[256];

	/* 4 is the size of fmt in the stack */
	va_list arg = (va_list)((char*)&fmt + 4);

	i = vsprintf(buf, fmt, arg);

	printl("%c !!panic!! %s", MAG_CH_PANIC, buf);

	/* should never arrive here */
	__asm__ __volatile__("ud2");
}

void clear()
{
	clear_screen(0,console_table[current_console].cursor);
	console_table[current_console].crtc_start = 0;
	console_table[current_console].cursor = 0;
	
}

//void show()
//{
//	printf("%d  %d  %d  %d",console_table[current_console].con_size, console_table[current_console].crtc_start, console_table[current_console].cursor, console_table[current_console].orig);
//}

void help()
{
    printf(" *                                                                          \n");
    printf(" *     ~~~~  ** **** **  ~~~~                  Welcome!                     \n");
    printf(" *    ~~  **           **   ~~                 PIGGY OS                     \n");
    printf(" *   ~~  **   ^^    ^^  **   ~~                                             \n");
    printf(" *  ~~~~**     @    @    **~~~~~           ~~Command List~~                 \n");
    printf(" *      **    _______    **       1. Help zzzzz             6. G1:Guessing   \n");
    printf(" *      **    | * * |    **       2. FileManage        7. G2:EightQueen \n");
    printf(" *      **    ~~~~~~~    **       3. ProcessManage     8. G3:Maze       \n");
    printf(" *       **      3      **        4. Carlendar         9. G4:BoxMan     \n");
    printf(" *        **           **         5. Calculator        10.Clear             \n");
    printf(" *          ***********                                                     \n");
    printf(" *                                                                          \n");
    printf(" *                                                                          \n");
    printf(" *    **************************************************************        \n");

//    printf("=============================================================================\n");
//	printf("Command List     :\n");
//	printf("1. process       : A process manage,show you all process-info here\n");
//	printf("2. filemng       : Run the file manager\n");
//	printf("3. clear         : Clear the screen\n");
//	printf("4. help          : Show this help message\n");
//	printf("5. game   : Play an interesting game which help you improve math\n");
//	printf("5. timer        : Run a easy timer Accurate to seconds\n");
//	printf("6. carlendar   :Run an easy Calendar\n");
//	printf("7. calculator   :Run an easy Calculator\n");
//	printf("8. guessing   :Run an interesting game\n");
//	printf("==============================================================================\n");		
}

void ProcessManage()
{
	int i;
    printf("    *                                                                     \n");
    printf("    *     ~~~~  ** **** **  ~~~~                                          \n");
    printf("    *    ~~  **           **   ~~                                         \n");
    printf("    *   ~~  **   ^^    ^^  **   ~~                                        \n");
    printf("    *  ~~~~**     @    @    **~~~~~                                       \n");
    printf("    *      **    _______    **              Welcome!                      \n");
    printf("    *      **    | * * |    **              PIGGY OS                      \n");
    printf("    *      **    ~~~~~~~    **                                            \n");
    printf("    *       **      3      **            ProcessManage!                   \n");
    printf("    *        **           **                                              \n");
    printf("    *          ***********                                                \n");
    printf("    *                                                                     \n");
    printf("    *                                                                     \n");
    printf("    *    ********************************************************         \n");
    
	printf("=============================================================================\n");
	printf("   ProcessID   |   ProcessName   |   ProcessPriority   |   Running?\n");
	//进程号，进程名，优先级，是否是系统进程，是否在运行
	printf("-----------------------------------------------------------------------------\n");
	for ( i = 0 ; i < NR_TASKS + NR_PROCS ; ++i )//逐个遍历
	{
//		if ( proc_table[i].priority == 0) continue;//系统资源跳过
	printf("    %d             %s                %d                    yes\n", proc_table[i].pid, proc_table[i].name, proc_table[i].priority);
	}
	printf("=============================================================================\n");
}


