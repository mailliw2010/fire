#include "gsvr_app_shmutex.h"
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>

int fdmutexmapFile = 0;
const char smutexmapfile[] = "/tmp/Mapfile";

/*****share by two process*****/
tmutex* mm = NULL;

void mutex_cond_init(void)
{
/*
	printf("mm->mutexattr->__size = %x\n", mm->mutexattr.__size[0]);
	printf("mm->mutexattr->__size = %x\n", mm->mutexattr.__size[1]);
	printf("mm->mutexattr->__size = %x\n", mm->mutexattr.__size[2]);
	printf("mm->mutexattr->__size = %x\n", mm->mutexattr.__size[3]);
	printf("mm->mutexattr->__size = %x\n", mm->mutexattr.__align);
*/	
	pthread_mutexattr_init(&mm->mutexattr);
	pthread_mutexattr_setpshared(&mm->mutexattr, PTHREAD_PROCESS_SHARED);
	pthread_mutex_init(&mm->mutex, &mm->mutexattr);

	pthread_condattr_init(&mm->condattr);
	pthread_condattr_setpshared(&mm->condattr, PTHREAD_PROCESS_SHARED);
	pthread_cond_init(&mm->cond, &mm->condattr);
/*
	printf("__SIZEOF_PTHREAD_MUTEXATTR_T = %d\n", __SIZEOF_PTHREAD_MUTEXATTR_T);
	printf("mm->mutexattr->__size = %x\n", mm->mutexattr.__size[0]);
	printf("mm->mutexattr->__size = %x\n", mm->mutexattr.__size[1]);
	printf("mm->mutexattr->__size = %x\n", mm->mutexattr.__size[2]);
	printf("mm->mutexattr->__size = %x\n", mm->mutexattr.__size[3]);
	printf("mm->mutexattr->__size = %x\n", mm->mutexattr.__align);
	printf("PTHREAD_PROCESS_SHARED = %d\n", PTHREAD_PROCESS_SHARED);

*/	
}




int close_mutex_map(void)
{
	printf("# clear and munmaping mm!\n");
	memset(mm, 0, sizeof(tmutex));
	munmap(mm,sizeof(tmutex));
#if NAME_OR_ANONY

#else

	close(fdmutexmapFile);
#endif
	return 0;
}


int create_mutex_map(void)
{

#if NAME_OR_ANONY
	mm = (tmutex*)mmap(NULL, sizeof(tmutex), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);  //匿名内存映射，只能用在血缘关系的进程通信(fork)
#else
	fdmutexmapFile = open(smutexmapfile,O_CREAT|O_RDWR|O_NONBLOCK,0666);
	if(fdmutexmapFile < 0){
		printf("%s: open %s failed!\n",__func__,smutexmapfile);
		return -1;
	}
	lseek(fdmutexmapFile,sizeof(tmutex),SEEK_SET);	
	write(fdmutexmapFile,"",1);		
	mm = (tmutex*)mmap(NULL, sizeof(tmutex), PROT_READ|PROT_WRITE, MAP_SHARED,fdmutexmapFile, 0);  

#endif
	
	if(!mm){
		printf("%s: mmap failed!\n",__func__);
		return -1;
	}
	
	
	
	return 0;


}




