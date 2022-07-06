
#include <stdio.h>
#include <assert.h>
#include <string.h>
char* readcmdops(int argc, char *argv[])
{
	assert(argc>=1);
	if(argc==1)
	{
		return NULL;
	}
	char *p=argv[1];
	char ops[10];int j=0;
	for(int i=0;i<strlen(p);i++)
	{
		if(p[i]=='-')
		{
			continue;
		}
		ops[j]=p[i];
		j++;
	}
	strcpy(p,ops);
	return p;
}
int getppid(char *path)
{

}

 //   /proc/pid/stat 文件中有name和ppid
 //  /proc/pid/task/ pid1 pid2 pid3   如pid1=pid 则/proc/pid/task/pid1/childrens文件写着该pid的子进程
														// pid2与pid3也是pid的子进程
int main(int argc, char *argv[]) {
     char* ops=readcmdops(argc,argv);//ops是选项
	 printf("%s\n",argv[0]);
	 if(ops)//ops不为空时
	{
			//printf("%s\n",ops);
	}
	 else
	{
		if(!strcmp(ops,"p"))
		{

		}
		else if(!strcmp(ops,'n'))
		{

		}
		else if(!strcmp(ops,"V"))
		{

		}
		else
		{

		}
	}
}
