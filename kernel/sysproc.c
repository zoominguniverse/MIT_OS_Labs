#include "types.h"
#include "riscv.h"
#include "param.h"
#include "defs.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;


  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}


//Start by implementing sys_pgaccess() in kernel/sysproc.c.
//这个系统调用函数 之前有写过sysinfo也是一样的 我们需要得到参数 第二条提示也给我们说了
//我们需要得到地址以及int数据
//因为它pgaccess_test()里面是三个参数
//第一个是要检查的第一个地址 第二个是要检查的页的数量 第三个是结果bitmasks（也就是有没有被正确设置位
//我们仿照上面的代码实现即可 sys_wait() sys_sleep()
#ifdef LAB_PGTBL
int
sys_pgaccess(void)
{
    //分别转化前三个参数即可
    uint64 addr;
    argaddr(0, &addr);
    int num;
    argint(1, &num);
    uint64 bitmasks;
    argaddr(2, &bitmasks);
    //


  // lab pgtbl: your code here.
    return 0;
}
#endif

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
