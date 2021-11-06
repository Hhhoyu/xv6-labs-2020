#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"
int main(int argc,char *argv[])
{
    if(argc<2)
    {
        fprintf(2,"Xargs: xargs <> <>\n");
        exit(1);
    }
    char *Input[MAXARG]; //记录一行的完整参数，即xargs 后面的一堆参数
    char buf[MAXARG];   //每次从标准输入读取的缓冲区
    char bufBlock[MAXARG];//每次从标准输入读取的一个参数字符串，从buf中获取，再加入到Input中去
    char *p=bufBlock;//p用来记录一行中的一个参数字符串
    int index=0;//Input的索引
    int indexread=0;//在缓冲区的索引
    int indexBlock=0;//读取的每一行的索引
    int len=0;
    for(index=0;index<argc-1;index++)
        Input[index]=argv[index+1];
    while((len=read(0,buf,sizeof(buf)))>0)
    {
        for(indexread=0;indexread<len;indexread++)
        {
            if(buf[indexread]==' ') //一个参数读取完毕
            {
                bufBlock[indexBlock++]=0; //字符串结尾加0
                Input[index++]=p;//将参数放入参数列表
                p=&bufBlock[indexBlock];//将p重置到数组末尾，开始记录下一个参数
            }
            else if(buf[indexread]=='\n') //一行参数读完，需要执行程序
            {
                bufBlock[indexBlock]=0;
                indexBlock=0;//重置索引
                Input[index++]=p;
                p=bufBlock;
                Input[index]=0;
                index=argc-1;
                if(fork()==0)
                    exec(argv[1],Input);
                wait(0);
            }
            else 
            {
                bufBlock[indexBlock++]=buf[indexread];
            }
        }
    }
    exit(0);
}