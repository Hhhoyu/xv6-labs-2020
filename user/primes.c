#include<kernel/types.h>
#include<user/user.h>
void primes()
{
    int fd[2];
    int index,index2;
    pipe(fd);
    if(read(0,&index,4)==0)
    {
        //fprintf(2,"read error!\n");
        exit(1);
    }
    printf("prime %d\n",index);
    if(fork()==0)
    {
        close(0);
        dup(fd[0]);
        close(fd[0]);
        close(fd[1]);
        primes();
    }
    else
    {
        close(fd[0]);
        while(read(0,&index2,4)!=0)
        {
            if(index2%index!=0)
                write(fd[1],&index2,4);
        }
        close(fd[1]);
        wait(0);

    }
}
int main()
{
    int fd[2];
    int index;
    pipe(fd);
    if(fork()==0)
    {
        close(0);
        dup(fd[0]);
        close(fd[1]);
        close(fd[0]);
        primes();
    }
    else 
    {
        close(fd[0]);
        for(index=2;index<=35;index++)
        {
            if(write(fd[1],&index,4)!=4)
            {
                fprintf(2,"write error!\n");
                exit(1);
            }
        }
        close(fd[1]);
        wait(0);
    }
    exit(0);
}