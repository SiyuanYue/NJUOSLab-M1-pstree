
#include <stdio.h>
#include <assert.h>
#include <string.h>

struct pidinfo{
	char *name;
	__pid_t pid;
	__pid_t ppid;
};


char* readcmdops(int argc, char *argv[])//get commandline ops
{
	assert(argc>=1);
	if(argc==1)//no ops 
	{
		char *p=argv[0];
		return p;
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
	 if(!ops)//ops为空时  moren
	{
			printf("%s\n",ops);
	}
	 else
	{
		if(!strcmp(ops,"p"))   //ops = -p
		{
			printf("%s",ops);
		}
		else if(!strcmp(ops,"n"))   //  -n
		{
			printf("%s",ops);
		}
		else if(!strcmp(ops,"V"))  //    -V
		{
			printf("%s",ops);
		}
		else
		{
			printf("%s","wrong ops");
		}
	}
}
