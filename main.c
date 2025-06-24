/*
	File:    main.c
	Author:  Light&Electricity
	Date:    2025.6.20
	Version: 0.5
*/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
//#include <unistd.h>

#include "MiniRingBuf.h"

//#pragma comment(lib, ""pthreadVC2.lib"")

#define LOCK		pthread_mutex_lock(&mutex_thread)
#define UNLOCK		pthread_mutex_unlock(&mutex_thread)

pthread_mutex_t mutex_mrb = PTHREAD_MUTEX_INITIALIZER, mutex_thread = PTHREAD_MUTEX_INITIALIZER;

uint32_t inCnt = 0, outCnt = 0, inSum = 0, outSum = 0;

MiniRingBuf ringBuf;
MRB_TYPE_BUF bufData[1000];

void RW_rand_test(MiniRingBuf *mrb, int n, int p){
	MRB_TYPE_BUF inBuf[1000];
	MRB_TYPE_BUF outBuf[1000];
	for(int i = 0; ; i++){
		if(i > n || rand() % 100 >= p){ // read
			if(i > n && mrb_len(&ringBuf) == 0) break;
			int len = rand() % (sizeof(outBuf)/sizeof(MRB_TYPE_BUF));
			len = mrb_read(&ringBuf, outBuf, len);

			LOCK;
			outCnt += len;
			for(int j = 0; j < len; j++) outSum += outBuf[j];
			UNLOCK;
		}
		else{ // write
			int len = rand() % (sizeof(inBuf)/sizeof(MRB_TYPE_BUF));
			for(int j = 0; j < len; j++) inBuf[j] = rand();
			len = mrb_write(&ringBuf, inBuf, len);

			LOCK;
			inCnt += len;
			for(int j = 0; j < len; j++) inSum += inBuf[j];
			UNLOCK;
		}
	}
}

void* thread_function(void* arg) {
	LOCK;
    printf("thread %d running, time:%d\n", *(int*)arg, clock());
	UNLOCK;
	
	RW_rand_test(&ringBuf, 10000000, *(int*)arg == 1 ? 0 : 100);
	//sleep(1);

	LOCK;
    printf("thread %d over, time:%d\n", *(int*)arg, clock());
	UNLOCK;
    return NULL;
}

void callback(MiniRingBuf *mrb, MRB_TYPE_BYTE errCode){
	//printf("code:%d,	start:%d,	end:%d\n", errCode, mrb->start, mrb->end);
}

int main()
{
	mrb_init(&ringBuf, bufData, sizeof(bufData)/sizeof(MRB_TYPE_BUF), MRB_SET_SKIPWRITE);
#if MRB_MUTEX_EN
	ringBuf.mutex = &mutex_mrb;
#endif
#if MRB_CALLBACK_NOSPACE
	ringBuf.callback = callback;
#endif

	srand(time(NULL)*0);

	pthread_t thread1, thread2;
    int arg1 = 1, arg2 = 2;
    // create thread
    int ret = 0;
	ret |= pthread_create(&thread1, NULL, thread_function, &arg1);
	ret |= pthread_create(&thread2, NULL, thread_function, &arg2);
    if (ret != 0) {
        perror("pthread_create failed.");
        return 1;
    }
    // wait thread to finish
    pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
    printf("All thread over.\n");

	
	// read write test
	//RW_test_01(&ringBuf, 10000);
    
	/*for(int i = 0; i < 100; i++){
		if(i % 10 == 0 && i) printf("\n");
		printf("%d	",bufData[i]);
	}*/

	printf("inCnt:	%u,	inSum:	%u.\n", inCnt, inSum);
	printf("outCnt:	%u,	outSum:	%u.\n", outCnt, outSum);

	return 0;
}











