
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
PidInfo pidinfos[10000];
int pid_count=0;
char* readcmdops(int argc, char *argv[])//get commandline ops
{
	assert(argc>=1);
	if(argc==1)//no ops 
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
				//printf("%d (%s) %d\n",pidinfos[pid_count].pid,pidinfos[pid_count].name,pidinfos[pid_count].ppid);
				pid_count++;
			}
		}
	}
}

 //   /proc/pid/stat 文件中有name和ppid
 //  /proc/pid/task/ pid1 pid2 pid3   如pid1=pid 则/proc/pid/task/pid1/childrens文件写着该pid的子进程
												// pid2与pid3也是pid的子进程
typedef struct processtree{
	pid_t pid;
	char name[40];
	struct childprocesses *children;
}Processtree;
struct childprocesses{
	struct childprocesses *next;
	Processtree *proc;
};

void findallchildrens(int pid ,int index[])
{
	int indexcount=0;
	size_t i= 0;
	for (; i <pid_count-1; i+=2)
	{
		if(pidinfos[i].ppid==pid)
		{
			index[indexcount++]=i;
		}
		if(pidinfos[i+1].ppid==pid)
		{
			index[indexcount++]=i+1;
		}
	}
	for (; i <pid_count; i++)
	{
		if(pidinfos[i].ppid==pid)
		{
			index[indexcount++]=i;
		}
	}
}
void creat_tree(Processtree * root,int tab_length)
{
	char str[100]={0};
	int allchildrensindex[500]={0}; // Log the index of all root child processes in Pidinfos
	findallchildrens(root->pid,allchildrensindex);
	root->children=(struct childprocesses*)malloc(sizeof(struct childprocesses));
	struct childprocesses *child=root->children;
	if(allchildrensindex[0]==0)//it's leaf node,no child,return
	{
		printf("%s(%d)",root->name,root->pid);
		return;
	}
	sprintf(str,"%s(%d)-",root->name,root->pid);
	printf("%s",str);
	if(allchildrensindex[1]!=0)
	{
		printf("+-");
	}
	for(int i=0;i<500 && allchildrensindex[i]!=0 ;i++)//Traverses all children of the process
	{
		child->proc=(Processtree *)malloc(sizeof(Processtree));
		child->proc->pid=pidinfos[allchildrensindex[i]].pid;
		strcpy(child->proc->name,pidinfos[allchildrensindex[i]].name);
		creat_tree(child->proc,strlen(str)+tab_length+2);   // recursive
		if(i+1<500 && allchildrensindex[i+1]!=0)
		{
			child->next=(struct childprocesses*)malloc(sizeof(struct childprocesses));
			child=child->next;
			char tabs[100];
			printf("\n");
			for (size_t i = 0; i <strlen(str)+tab_length; i++)
			{
				printf(" ");
			}
			printf("|-");
		}
	}
	free(root);
}
void creat_tree_nopid(Processtree * root,int tab_length)
{
	char str[100]={0};
	int allchildrensindex[500]={0}; // Log the index of all root child processes in Pidinfos
	findallchildrens(root->pid,allchildrensindex);
	root->children=(struct childprocesses*)malloc(sizeof(struct childprocesses));
	struct childprocesses *child=root->children;
	if(allchildrensindex[0]==0)//it's leaf node,no child,return
	{
		printf("%s",root->name);
		return;
	}
	sprintf(str,"%s-",root->name);
	printf("%s",str);
	if(allchildrensindex[1]!=0)
	{
		printf("+-");
	}
	for(int i=0;i<500 && allchildrensindex[i]!=0 ;i++)//Traverses all children of the process
	{
		child->proc=(Processtree *)malloc(sizeof(Processtree));
		child->proc->pid=pidinfos[allchildrensindex[i]].pid;
		strcpy(child->proc->name,pidinfos[allchildrensindex[i]].name);
		creat_tree_nopid(child->proc,strlen(str)+tab_length+2);   // recursive
		if(i+1<500 && allchildrensindex[i+1]!=0)
		{
			child->next=(struct childprocesses*)malloc(sizeof(struct childprocesses));
			child=child->next;
			char tabs[100];
			printf("\n");
			for (size_t i = 0; i <strlen(str)+tab_length; i++)
			{
				printf(" ");
			}
			printf("|-");
		}
	}
	free(root);
}												
int main(int argc, char *argv[]) {
    char* ops=readcmdops(argc,argv);//ops是选项
	if(!ops)//ops为空时  mo ren
	{
		setProcessInfo();
		Processtree *root=(Processtree *)malloc(sizeof(Processtree));
		root->pid=pidinfos[0].pid;
		strcpy(root->name,pidinfos[0].name);
		creat_tree_nopid(root,0);
	}
	 else
	{
		if(!strcmp(ops,"p"))   //ops = -p
		{
			//printf("%s",ops);
			setProcessInfo();
			Processtree *root=(Processtree *)malloc(sizeof(Processtree));
			root->pid=pidinfos[0].pid;
			strcpy(root->name,pidinfos[0].name);
			creat_tree(root,0);
		}
		else if(!strcmp(ops,"n"))   //  -n
		{
			printf("%s",ops);
			//TODO
		}
		else if(!strcmp(ops,"V"))  //    -V
		{
			printf("pstree (PSmisc) UNKNOWN\nCopyright (C) 1993-2019 Werner Almesberger and Craig Small\nPSmisc comes with ABSOLUTELY NO WARRANTY.\nThis is free software, and you are welcome to redistribute it under\nthe terms of the GNU General Public License.\nFor more information about these matters, see the files named COPYING.");
		}
		else
		{
			printf("%s","wrong ops");
		}
	}
	return 0;
}
