#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
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
  backtrace();
  argint(0, &n);
  if(n < 0)
    n = 0;
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


void load(struct trapframe *old , struct trapframe *new){
    /*   0 */ new->kernel_satp=old->kernel_satp ;   // kernel page table
    /*   8 */ new->kernel_sp=old->kernel_sp;     // top of process's kernel stack
    /*  16 */ new->kernel_trap=old->kernel_trap;   // usertrap()
    /*  24 */ new->epc=old->epc;           // saved user program counter
    /*  32 */ new->kernel_hartid=old->kernel_hartid; // saved kernel tp
    /*  40 */ new->ra=old->ra;
    /*  48 */ new->sp=old->sp;
    /*  56 */ new->gp=old->gp;
    /*  64 */ new->tp=old->tp;
    /*  72 */ new->t0=old->t0;
    /*  80 */ new->t1=old->t1;
    /*  88 */ new->t2=old->t2;
    /*  96 */ new->s0=old->s0;
    /* 104 */ new->s1=old->s1;
    /* 112 */ new->a0=old->a0;
    /* 120 */ new->a1=old->a1;
    /* 128 */ new->a2=old->a2;
    /* 136 */ new->a3=old->a3;
    /* 144 */ new->a4=old->a4;
    /* 152 */ new->a5=old->a5;
    /* 160 */ new->a6=old->a6;
    /* 168 */ new->a7=old->a7;
    /* 176 */ new->s2=old->s2;
    /* 184 */ new->s3=old->s3;
    /* 192 */ new->s4=old->s4;
    /* 200 */ new->s5=old->s5;
    /* 208 */ new->s6=old->s6;
    /* 216 */ new->s7=old->s7;
    /* 224 */ new->s8=old->s8;
    /* 232 */ new->s9=old->s9;
    /* 240 */ new->s10=old->s10;
    /* 248 */ new->s11=old->s11;
    /* 256 */ new->t3=old->t3;
    /* 264 */ new->t4=old->t4;
    /* 272 */ new->t5=old->t5;
    /* 280 */ new->t6=old->t6;
}

uint64
sys_sigalarm(void){
    struct proc *p = myproc();
    int ticks;
    argint(0,&ticks);
    p->ticks =ticks;
    uint64 handler;
    argaddr(1,&handler);
    p->handler = handler;
    p->ticks_cnt = 0 ;
    //load(p->trapframe,p->saved_trapframe);
    return 0;
}


uint64
sys_sigreturn(void){
    struct proc *p = myproc();
    load(p->saved_trapframe,p->trapframe);
    p->handler_working = 0;
    p->trapframe->a0 = 0;
    return 0;
}


