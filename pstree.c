
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <dirent.h>  
#include <sys/stat.h>
#include <stdlib.h>
typedef struct pidinfo{
	char name[50];
	__pid_t pid;
	__pid_t ppid;
}PidInfo;
PidInfo pidinfos[4000];
int pid_count=0;
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
pid_t readprocessname_ppid(__pid_t pid,char name[])
{
	char *str=(char *)malloc(sizeof(char)*20);
	sprintf(str,"%d",pid);
	char processpath[20]="/proc/";
	strcat(processpath,str);
	strcat(processpath,"/stat");
	FILE *fp=fopen(processpath,"r");
	if (fp){
		char i;
		__pid_t _pid,ppid;
		char pname[40];
		fscanf(fp,"%d (%s %c %d",&_pid,pname,&i,&ppid);
		pname[strlen(pname)-1]='\0';
		strcpy(name,pname);
		assert(pid==_pid);
		fclose(fp);
		free(str);
		return ppid;
	}else{
		printf("open failed");
		free(str);
		return 0;
	}
}
void setProcessInfo()
{
	// read /proc/
	DIR *dp=opendir("/proc");
	if(!dp)
	{
		fprintf(stderr,"%s","Can 't open /proc/");
		return;
	}
	else
	{
		int pid=0;
		struct dirent *entry;
		while ((entry=readdir(dp))!=NULL)
		{
			if((pid=atoi(entry->d_name))==0)
			{
				continue;
			}
			else{// store in pidinfo (name and pid and ppid)
				pidinfos[pid_count].pid=pid;
				pidinfos[pid_count].ppid=readprocessname_ppid(pid,pidinfos[pid_count].name);
				assert(pidinfos[pid_count].ppid>-1);
				printf("%d (%s) %d\n",pidinfos[pid_count].pid,pidinfos[pid_count].name,pidinfos[pid_count].ppid);
				pid_count++;
			}
		}
	}
}

 //   /proc/pid/stat 文件中有name和ppid
 //  /proc/pid/task/ pid1 pid2 pid3   如pid1=pid 则/proc/pid/task/pid1/childrens文件写着该pid的子进程
														// pid2与pid3也是pid的子进程
int main(int argc, char *argv[]) {
    //  char* ops=readcmdops(argc,argv);//ops是选项
	//  if(!ops)//ops为空时  moren
	// {
	// 		printf("%s\n",ops);
	// }
	//  else
	// {
	// 	if(!strcmp(ops,"p"))   //ops = -p
	// 	{
	// 		printf("%s",ops);
	// 	}
	// 	else if(!strcmp(ops,"n"))   //  -n
	// 	{
	// 		printf("%s",ops); char* ops=readcmdops(argc,argv);//ops是选项
	//  if(!ops)//ops为空时  moren
	// {
	// 		printf("%s\n",ops);
	// }
	//  else
	// {
	// 	if(!strcmp(ops,"p"))   //ops = -p
	// 	{
	// 		printf("%s",ops);
	// 	}
	// 	else if(!strcmp(ops,"n"))   //  -n
	// 	{
	// 		printf("%s",ops);
	// 	}
	// 	else if(!strcmp(ops,"V"))  //    -V
	// 	{
	// 		printf("%s",ops);
	// 	}
	// 	else
	// 	{
	// 		printf("%s","wrong ops");
	// 	}
	// }
	// 	{
	// 		printf("%s","wrong ops");
	// 	}
	// }
	//printf("OD\n");
	setProcessInfo();
}
