#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#define PATH "/proc"
#define BUFFSIZE_INFO 50
//pid ppid
struct pinfo
{
    int pid;
    int ppid;
};

struct pinfo processInfos[500];
int number_process=0;//进程个数

//得到PPID,str为文件名
int getPPid(char *filename);
//得到pid和ppid，存入processInfos数组
void setPid_Ppid();

int main()
{
    setPid_Ppid();
    for(int i=0;i<number_process;i++)
    {
        printf("%d %d\n",processInfos[i].pid,processInfos[i].ppid);
    }
    return 0;
}

//得到PPID
int getPPid(char *filename)
{
   int ppid=-100;
   char *right=NULL;
   FILE *fp=fopen(filename,"r");
   char info[BUFFSIZE_INFO+1];
   info[BUFFSIZE_INFO]='\0';


   if(fp==NULL)
   {
       fprintf(stderr,"open file %s error!\n",filename);
       return -1;
   }

    //读取信息到info中
   if(fgets(info,BUFFSIZE_INFO,fp)==NULL)
   {
       puts("fgets error!");
       exit(0);
   }
   //printf("%s\n",info);

   right=strrchr(info,')');
   if(right==NULL)
   {
       printf("not find )\n");
   }
   //printf("%s\n",right);
   right+=3;//移动到PPID的位置
   //right++;
   sscanf(right,"%d",&ppid);

   return ppid; 
}

//得到pid和ppid，存入processInfos数组
void setPid_Ppid()
{
    DIR * dir_ptr;
    struct dirent*direntp;
    int pid;
    int ppid;
    char process_path[51]="/proc/";
    char stat[6]="/stat";
    char pidStr[20];

    dir_ptr=opendir(PATH);

    if(dir_ptr==NULL)
    {
        fprintf(stderr,"can not open /proc\n");
        exit(0);
    }
    
    while(direntp=readdir(dir_ptr))
    {
        //memcpy(name,direntp->d_name,strlen(direntp->d_name)+1);
        pid=atoi(direntp->d_name);
        if(pid!=0)
        {
            //拼接字符串process_path
            //examples: /proc/pid/stat
            //itoa(pid,pidStr,10);
            processInfos[number_process].pid=pid;
            //printf(" %d process: %d\n",number_process,pid);
            sprintf(pidStr,"%d",pid);
            strcat(process_path,pidStr);
            strcat(process_path,stat);
            //process_path中为指定process的对应stat文件路径

            int ppid=getPPid(process_path);//返回-1表示解析出错
            if(ppid!=-1)
                processInfos[number_process++].ppid=ppid;
            else
                number_process++;

            //重置process_path
            process_path[6]=0;
            //printf("%s\n",process_path);
        }
    }
    //printf("the number of process is %d\n",number_process);
}




