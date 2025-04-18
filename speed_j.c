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
int chkout(char *f1)
{
    unsigned int speed;
    FILE *fp1;
    fp1=fopen(f1,"r");
    fscanf(fp1,"%d",&speed);
    fclose(fp1);
    printf("SPEED:%u\n",speed);
    if(speed>=4e8)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
    return 0;
}
int main(int argc,char *argv[])// ./speed_j "g++ ......"
{
    FILE *fp,*outfile;
    pid_t pid;
    clock_t start,now;
    int mem,cnt,mmem;
    int testnum,i;
    int subpstatus,ansstatus;
    srand(time(0));
    if(compile("gcc speed_check.c -o target -w")==0)
    {
        return 0;
    }
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
                ansstatus=chkout("speed_check.out");
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
        if(mem>2147483647)
        {
            statstr="MLE";
            sprintf(command,"kill -9 %d",pid);
            system(command);
            break;
        }
        now=clock();
        if((now-start)/(double)CLOCKS_PER_SEC*1000>=2147483647)
        {
            statstr="TLE";
            sprintf(command,"kill -9 %d",pid);
            system(command);
            break;
        }
        cnt++;
    }
    return 0;
}