#include<kernel/types.h>
#include<user/user.h>
int main()
{
    char buf[]={'a'};
    int fd1[2],fd2[2];
    //fd1为父进程向子进程写
    //fd2为子进程向父进程写
    pipe(fd1);
    pipe(fd2);
    if(fork()==0)
    {
        close(fd1[1]);
        close(fd2[0]);
        if(read(fd1[0],buf,1)!=1)
            exit(1);
        printf("%d: received ping\n",getpid());
        write(fd2[1],buf,1);
        close(fd1[0]);
        close(fd2[1]);
        exit(0);
    }
    else
    {
        close(fd1[0]);
        close(fd2[1]);
        write(fd1[1],buf,1);
        if(read(fd2[0],buf,1)!=1)
         exit(1);
        printf("%d: received pong\n",getpid());
        close(fd1[1]);
        close(fd2[0]);
        exit(0);
    }
}