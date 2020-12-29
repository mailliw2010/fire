#ifndef GSVR_APP_SHMUTEX_H_
#define GSVR_APP_SHMUTEX_H_

#include <pthread.h>

#define MAXLINE	10
#define MAXBUFF	2048

#define _PAGE_SIZE_	4096*10
#define NAME_OR_ANONY	0   // 0: NAME; 1:ANONY


typedef struct
{
	int num;
	pthread_mutex_t mutex;
	pthread_mutexattr_t mutexattr;
	pthread_cond_t cond;
	pthread_condattr_t condattr;

}tmutex;

extern tmutex* mm;

#ifdef __cplusplus
extern "C" {
#endif
int close_mutex_map(void);
int create_mutex_map(void);
void mutex_cond_init(void);

#ifdef __cplusplus
}
#endif

#endif
