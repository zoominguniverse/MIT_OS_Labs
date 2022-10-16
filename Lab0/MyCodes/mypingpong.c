#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[])
{

    //parent to child / child to parent  ģ��P2P���������� 
    int pc[2];
    int cp[2]; 
    //ֻ����� mypingpong �������г���  
    if(argc > 1){
       printf(2,"Only 1 argument is needed!\n");
       exit();
    }
    // ���������ܵ� ���ڸ��ӽ��̼�˫��ͨ�� 
    pipe(pc);
    pipe(cp);
    //�ӽ��̵Ĵ��� 
    if(fork() == 0) {
        char buffer[32];
        //��
        close(pc[1]);
        close(cp[0]);
        read(pc[0],buffer,4);
        close(pc[0]);
        //д 
        printf(1,"%d: received ping\n",getpid());
        write(cp[1],"pong", 4);
        close(cp[1]);
    } else { 
    	char buffer[32];
        //��
        close(pc[0]);
        close(cp[1]);
        write(pc[1],"ping", 4);
        close(pc[1]);
        wait();
        //д 
        printf(1,"%d: received pong\n",getpid());
        read(cp[0],buffer,4);
        close(cp[0]);
    }
    exit(); 
}
