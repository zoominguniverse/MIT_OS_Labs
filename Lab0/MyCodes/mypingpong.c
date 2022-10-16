#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[])
{

    //parent to child / child to parent  模仿P2P的命名规则 
    int pc[2];
    int cp[2]; 
    //只需键入 mypingpong 即可运行程序  
    if(argc > 1){
       printf(2,"Only 1 argument is needed!\n");
       exit();
    }
    // 创建两个管道 用于父子进程间双向通信 
    pipe(pc);
    pipe(cp);
    //子进程的处理 
    if(fork() == 0) {
        char buffer[32];
        //读
        close(pc[1]);
        close(cp[0]);
        read(pc[0],buffer,4);
        close(pc[0]);
        //写 
        printf(1,"%d: received ping\n",getpid());
        write(cp[1],"pong", 4);
        close(cp[1]);
    } else { 
    	char buffer[32];
        //读
        close(pc[0]);
        close(cp[1]);
        write(pc[1],"ping", 4);
        close(pc[1]);
        wait();
        //写 
        printf(1,"%d: received pong\n",getpid());
        read(cp[0],buffer,4);
        close(cp[0]);
    }
    exit(); 
}
