/*
	File:    main.c
	Author:  Light&Electricity
	Date:    2025.6.19
	Version: 0.5
*/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include "MiniRingBuf.h"

uint32_t inCnt = 0, outCnt = 0, inSum = 0, outSum = 0;

MiniRingBuf ringBuf;
uint8_t bufData[1000];
uint8_t inBuf[100];
uint8_t outBuf[100];

void RW_test_01(MiniRingBuf *mrb){
	srand(time(NULL));
	for(int i = 0; ; i++){
		if(i > 10000 || rand() % 2){ // read
			if(i > 10000 && mrb_len(&ringBuf) == 0) break;
			int len = rand() % sizeof(outBuf);
			len = mrb_read(&ringBuf, outBuf, len);
			outCnt += len;
			for(int j = 0; j < len; j++) outSum += outBuf[j];
		}
		else{ // write
			int len = rand() % sizeof(inBuf);
			for(int j = 0; j < len; j++) inBuf[j] = rand();
			len = mrb_write(&ringBuf, inBuf, len);
			inCnt += len;
			for(int j = 0; j < len; j++) inSum += inBuf[j];
		}
	}
}

void callback(MiniRingBuf *mrb, MRB_TYPE_BYTE errCode){
	printf("code:%d,	start:%d,	end:%d\n", errCode, mrb->start, mrb->end);
}

int main()
{
	mrb_init(&ringBuf, bufData, sizeof(bufData), MRB_SET_NOWRITE);
	ringBuf.callback = callback;
	
	// read write test
	RW_test_01(&ringBuf);
    
	for(int i = 0; i < 100; i++){
		if(i % 10 == 0 && i) printf("\n");
		printf("%d	",bufData[i]);
	}

	printf("\ninCnt:%d,	outCnt:%d,	inSum:%d,	outSum:%d\n", inCnt, outCnt, inSum, outSum);

	return 0;
}











