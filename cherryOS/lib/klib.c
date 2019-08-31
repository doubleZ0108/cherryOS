
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            klib.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Forrest Yu, 2005
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include "type.h"
#include "stdio.h"
#include "const.h"
#include "protect.h"
#include "tty.h"
#include "console.h"
#include "string.h"
#include "fs.h"
#include "proc.h"
#include "global.h"
#include "proto.h"


/*======================================================================*
                               itoa
 *======================================================================*/
PUBLIC char * itoa(char * str, int num)/* 数字前面的 0 不被显示出来, 比如 0000B800 被显示成 B800 */
{
	char *	p = str;
	char	ch;
	int	i;
	int	flag = 0;

	*p++ = '0';
	*p++ = 'x';

	if(num == 0){
		*p++ = '0';
	}
	else{	
		for(i=28;i>=0;i-=4){
			ch = (num >> i) & 0xF;
			if(flag || (ch > 0)){
				flag = 1;
				ch += '0';
				if(ch > '9'){
					ch += 7;
				}
				*p++ = ch;
			}
		}
	}

	*p = 0;

	return str;
}

char * itoaDec(char *str, int value)
{
	if(value == 0)
	{
		str[0] = '0';
		str[1] = 0;
		return str;
	}

    if (value < 0) //如果是负数,则str[0]='-',并把value取反(变成正整数)

    {
        str[0] = '-';
        value = 0-value;
    }
    int i,j;
    i=1;
    for(; value > 0; i++,value/=10) //从value[1]开始存放value的数字字符，不过是逆序，等下再反序过来
    {
        str[i] = value%10+'0'; //将数字加上0的ASCII值(即'0')就得到该数字的ASCII值
    }
    str[i] = 0;

    // printf("itoaDec::%s::\n", str);

    j=i-1;
    i=1;
    for(;j-i>=1; j--,i++) //将数字字符反序存放
    {
        str[i] = str[i]^str[j];
        str[j] = str[i]^str[j];
        str[i] = str[i]^str[j];
    }

    if(str[0] != '-') //如果不是负数，则需要把数字字符下标左移一位，即减1
    {
    	i=0;
        for(; str[i+1]!='\0'; i++)
            str[i] = str[i+1];
        str[i] = '\0';
    }
    return str;
}


PUBLIC int atoi(const char * str, int * pRet)
{
	int iRet = 0;
	int  fNeg = 0;
	if (*str == '-')
	{
		fNeg = 1;
		++str;
	}
	else if (*str == '+')
	{
		fNeg = 0;
		++str;
	}
	while (*str)
	{
		if (*str == ' ' || *str == '.') break;
		if (*str < '0' || *str > '9')
		{
			return 0;
		}
		iRet = iRet * 10 + (*str - '0');
		++str;
	}
	*pRet = !fNeg ? iRet : -iRet;
	return 1;
}


/*======================================================================*
                               disp_int
 *======================================================================*/
PUBLIC void disp_int(int input)
{
	char output[16];
	itoa(output, input);
	disp_str(output);
}

/*======================================================================*
                               delay
 *======================================================================*/
PUBLIC void delay(int time)
{
	int i, j, k;
	for(k=0;k<time;k++){
		/*for(i=0;i<10000;i++){	for Virtual PC	*/
		for(i=0;i<10;i++){/*	for Bochs	*/
			for(j=0;j<10000;j++){}
		}
	}
}
