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
    //第五个提示说我们可以对len保护一下 设置上限 我们就此保护一下
    if(num>32||num<0){
        return -1;
    }
    uint64 bitmasks;
    argaddr(2, &bitmasks);
    //第四个提示说到我们需要用到copyout进行传参 如果在内核中被调用完毕 我们需要把它返回用户态
    //我们用到的copyout函数是那个vm.c里面的copyout函数 详见其中注释
    //我们需要获取当前进程的页表 以便对他操作 以及对copyout操作
    //我们可以测试一下 先不对其进行置位 直接写一个值 之后再调用的时候打印一下bitmasks 之后运行一下能否是我们设置的1010
    struct proc *p = myproc();

    int res = 0x1010; //此处0b的话会打印A 不是想要的 我们应当和32位保持一致

    if(copyout(p->pagetable, bitmasks, (char *)&res, sizeof(res)) < 0){
      return -1;
    }
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
