#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc,char* argv[]){
	int time=0;
	if(argc!=2){
		printf(2,"sleep(Error):No Parameter Given\n");
	}
	else{
		time=atoi(argv[1]);
		sleep(time);
		printf(1,"I have slept for %d ms...\n",time);
	}
	exit(); 
} 
