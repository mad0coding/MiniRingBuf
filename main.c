#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "RingBuf.h"


MiniRingBuf ringTest;
uint8_t ringBufArray[100];
uint8_t inBuf[] = {255,1,2,3,4,5,6,7,8,9,};
uint8_t outBuf[100];

int main()
{
	mrb_init(&ringTest, ringBufArray, sizeof(ringBufArray), 0x00);
	printf("len:%d\n", mrb_len(&ringTest));
	mrb_write(&ringTest, inBuf, sizeof(inBuf));
	mrb_del(&ringTest, 5);
	printf("len:%d\n", mrb_len(&ringTest));
	mrb_write(&ringTest, inBuf, sizeof(inBuf));
	mrb_read(&ringTest, outBuf, sizeof(inBuf)*2);
    
	for(int i = 0; i < 100; i++){
		if(i % 10 == 0 && i) printf("\n");
		printf("%d	",outBuf[i]);
	}
	return 0;
}











