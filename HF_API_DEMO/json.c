#include <stdio.h>
#include <string.h>

#define	BuffSize	1024

typedef struct json{
	char	code[16];
	char	temp[16];
	char	icon[16];
	char	text[16];
	char	humidity[16];
}JSON,*pJSON;

size_t getLine(char *mem,char *reBackBuff,int minSize)
{
	size_t strSize=0;
	int i,j=0;
	memset((void *)reBackBuff,'\0',minSize);
	
	for(i = 0; i < minSize; i++)
	{
		if(mem[i] == ',')break;
		if(mem[i] == '{' || mem[i] == '}')continue;
		if(mem[i] == '[' || mem[i] == ']')continue;
		reBackBuff[j++]	=	mem[i];
		strSize++;
	}
	return	strSize+2;
}

int	getNameValue(char *mem,char *reBackName,char *reBackValue,int lineLen)
{
	int i=0,j=0,readed;
	//get name
	for(i = 0; i < lineLen; i ++)
	{
		if(mem[i] == ':')break;
		if(mem[i] == '"')continue;
		reBackName[j++]	=	mem[i];
	}
	reBackName[j]	=	'\0';

	//get value
	j=0;
	for(;i < lineLen; i++)
	{
		if(mem[i] == ':' || mem[i] == '"')continue;
		reBackValue[j++]	=	mem[i];
	}
	reBackValue[j]	=	'\0';

	return i;
}

int main(int argc,char **argv)
{
	FILE *fp			=	NULL;
	char buff[BuffSize]	=	{0};
	int readBackByte	=	0;
	char line[64]		=	{0};
	char name[32]		=	{0};
	char value[32]		=	{0};
	int p	=	0;
	struct json myJson={0};
	
	char code[10]=	{0};
	int i,j,ret;
	
	fp	=	fopen("json.txt","rb");
	if(NULL == fp)
		printf("file open error.");
	
	readBackByte	=	fread((void*)buff,1,BuffSize,fp);
	printf("reback:%d\n",readBackByte);
	
	if(readBackByte < 30)//查询失败 
	{
		//find":"
		for(i = 0; i < readBackByte; i ++)
		{
			if(buff[i] == ':')
			{
				i += 2;
				break;
			}
			if(i == (readBackByte-1))
			{
				i = -1;
				break;
			}
		}
		j = 0;
		if(i != -1)
			for(;i < readBackByte; i++)
			{
				if(buff[i] != '"' & buff[i] != '}')code[j++]	=	buff[i];
			}
		return -1;
	}
	else	//查询成功
	{
		
		for(i = 0; p < readBackByte; i++)
		{
			ret=getLine((char*)(buff+p),line,64);
		//	printf("\nline:%s,ret:%d\n",line,ret);
			p+=ret;
			ret=getNameValue(line,name,value,ret);
		//	printf("name:%s\nvalue:%s\n",name,value);
			//code 不是 200 
			if(strcmp(name,"code") == 0 && strcmp(value,"200") != 0)
			{
				strcpy(myJson.code,value);
				break;
			}
			//获取code
			if(strcmp(name,"code") == 0)strcpy(myJson.code,value);
			//获取temp
			if(strcmp(name,"temp") == 0)strcpy(myJson.temp,value);
			//获取icon
			if(strcmp(name,"icon") == 0)strcpy(myJson.icon,value);
			//获取text
			if(strcmp(name,"text") == 0)strcpy(myJson.text,value);
			//获取humidity
			if(strcmp(name,"humidity") == 0)strcpy(myJson.humidity,value);
		}
		printf("code:%s,temp:%s,icon:%s,text:%s,humidity:%s\n",myJson.code,myJson.temp,myJson.icon,myJson.text,myJson.humidity);

	}
	
	fclose(fp);
	
	return 0;
}
