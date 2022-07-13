
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <dirent.h>  
#include <sys/stat.h>
#include <stdlib.h>
typedef struct pidinfo{
	char name[40];
	__pid_t pid;
	__pid_t ppid;
}PidInfo;
PidInfo pidinfos[10000];
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
	char *pidpath=strcat("/proc/",str);
	pidpath=strcat(pidpath,"/stat");
	FILE *fp=fopen(pidpath,"r");
	if (fp){
		char i;
		__pid_t _pid,ppid;
		char pname[40];
		fscanf(fp,"%d (%s %c %d",&_pid,pname,&i,&ppid);
		strncpy(name,pname,strlen(pname)-1);
		name[strlen(name)]='\0';
		assert(pid==_pid);
		fclose(fp);
		free(str);
		return ppid;
	}else{
		printf("open failed");
		free(str);
		return NULL;
	}
}
void setProcessInfo()
{
	// read /proc/
	DIR *dp;
	//struct stat *statbuf;
	if(dp=opendir("/proc/")==NULL)
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
				pid_count++;
				// TODO getname and ppid
			}
		}
	}
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
