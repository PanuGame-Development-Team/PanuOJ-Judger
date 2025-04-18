#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include<string.h>
char buf[128];
char smapfile[128];
char command[128];
char outfilename[128];
char *statstr;
int compile(char *compilecommand)// outputfile: target     inputffile: source.ext
{
    if(system(compilecommand)!=0)
    {
        printf("CE\n");
        return 0;
    }
    printf("OK\n");
    return 1;
}
void run()
{
    execl("./target","target",NULL);
    _exit(EXIT_FAILURE);
}
int cmpstr(char *f1,char *f2)
{
    FILE *fp1,*fp2;
    struct stat st1,st2;
    char ch1[1025],ch2[1025];
    fp1=fopen(f1,"r");
    fp2=fopen(f2,"r");
    if(!fp1||!fp2)
    {
        return 1;
    }
    stat(f1,&st1);
    stat(f2,&st2);
    if(st1.st_size>=10000000||st2.st_size>=10000000)
    {
        return 2;
    }
    while(!feof(fp1)||!feof(fp2))
    {
        fscanf(fp1,"%1024s",ch1);
        fscanf(fp2,"%1024s",ch2);
        if(strcmp(ch1,ch2))
        {
            return 3;
        }
    }
    fclose(fp1);
    fclose(fp2);
    return 0;
}
int main(int argc,char *argv[])// ./judge 1000 512000 "g++ ......" 10 strcmp
{
    FILE *fp,*outfile;
    pid_t pid;
    clock_t start,now;
    int mem,cnt,mmem;
    int maxmem;//kb
    int maxtime;//ms
    int testnum,i;
    int subpstatus,ansstatus;
    srand(time(0));
    sscanf(argv[1],"%d",&maxtime);
    sscanf(argv[2],"%d",&maxmem);
    sscanf(argv[4],"%d",&testnum);
    if(compile(argv[3])==0)
    {
        return 0;
    }
    for(i=0;i<testnum;++i)
    {
        sprintf(command,"mv test%d.in test.in",i+1);
        system(command);
        sprintf(outfilename,"test%d.ans",rand());
        sprintf(command,"mv test%d.ans %s",i+1,outfilename);
        system(command);
        pid=vfork();
        if(pid==0)
        {
            run();
        }
        sprintf(smapfile,"/proc/%d/smaps_rollup",pid);
        start=clock();
        cnt=0;
        mmem=0;
        while(1)
        {
            if(waitpid(pid,&subpstatus,WNOHANG)==pid)
            {
                if(WEXITSTATUS(subpstatus)==0)
                {
                    ansstatus=cmpstr(outfilename,"test.out");
                    switch(ansstatus)
                    {
                        case 0:
                            statstr="AC";
                            break;
                        case 1:
                            statstr="FE";
                            break;
                        case 2:
                            statstr="OLE";
                            break;
                        case 3:
                            statstr="WA";
                            break;
                    }
                }
                else
                {
                    statstr="RE";
                }
                break;
            }
            fp=fopen(smapfile,"r");
            if(!fp)
            {
                break;
            }
            while(!feof(fp))
            {
                fgets(buf,sizeof(buf),fp);
                if(buf[0]=='P'&&buf[1]=='r'&&buf[2]=='i'&&buf[3]=='v'&&buf[8]=='D')//Private_Dirty
                {
                    sscanf(buf,"%*s%d%*s",&mem);
                    break;
                }
            }
            fclose(fp);
            mmem=(mem>mmem?mem:mmem);
            if(mem>maxmem)
            {
                statstr="MLE";
                sprintf(command,"kill -9 %d",pid);
                system(command);
                break;
            }
            now=clock();
            if((now-start)/(double)CLOCKS_PER_SEC*1000>=maxtime)
            {
                statstr="TLE";
                sprintf(command,"kill -9 %d",pid);
                system(command);
                break;
            }
            cnt++;
        }
        printf("%d %s %d %ld\n",cnt,statstr,(cnt?mmem:120),(cnt?(now-start)*1000/CLOCKS_PER_SEC:0));
        sprintf(command,"mv %s test%d.ans",outfilename,i+1);
        system(command);
        sprintf(command,"mv test.in test%d.in",i+1);
        system(command);
    }
    return 0;
}