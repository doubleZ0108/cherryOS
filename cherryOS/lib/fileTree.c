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

#define MAX_ARRAY_NUM 1000; //文件树最大数目
#define MAX_FILE_INFO 4096; //文件树最大数目

struct FTreeArray
{
	int currDir;
	int fatherDir;
	int isDir;  //0 no,1 yes
	char fileName[12];  //dir is "dir"

}fTreeArray[MAX_ARRAY_NUM];

// struct ListNode
// {
// 	char fileName[12];
// 	struct ListNode* next;
// }ListNode[MAX_NODE];

int flRecord = 0;
void CreatFTreeArray(char fTreeInfo[])
{
	char currDir[4]={0};
	char fatherDir[4]={0};
	char isDir = 0;
	char fileName[12] = {0};

	int index = 0;

	char temp[12]={0}

	for(int count = 0,i=0,j=0;count < 4 && *fTreeInfo != '\0')
	{
		temp[i] = fTreeInfo[j];
		i++;
		j++;

		if(fTreeInfo[i] == ' ')
		{
			printf("SPACE!!!!");
			if(count == 0)
			{
				atoi(temp,fTreeArray[index].currDir)
				printf("currDir: %d",fTreeArray[index].currDir);
			}
			if(count == 1)
			{
				atoi(temp,fTreeArray[index].fatherDir);
				printf("fatherDir: %d",fTreeArray[index].fatherDir);
			}
			if(count == 2)
			{
				atoi(temp,fTreeArray[index].isDir);
				printf("isDir: %d",fTreeArray[index].isDir);
			}
			if(count == 3)
			{
				strcpy(fTreeArray[index].fileName, temp);
				printf("fileName: %s",fTreeArray[index].fileName);
			}
			
			//clear temp
			for (int k = 0; k < 12; ++k)
			{
				temp[k] = 0;
			}
			index++;
			count++;
			i = 0;
		}

		fTreeInfo++;
	}

}
