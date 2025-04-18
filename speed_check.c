#include<stdio.h>
#include<stdlib.h>
#include<time.h>
int main()
{
    freopen("speed_check.out","w",stdout);
    time_t t0=time(0);
    unsigned short i;
    __volatile__ unsigned int cnt=0;
    while(time(0)==t0);
    while(time(0)-t0==1)
    {
        for(i=1;i!=0;++i)
        {
            cnt++;
        }
    }
    printf("%u\n",cnt);
    fclose(stdout);
    return 0;
}