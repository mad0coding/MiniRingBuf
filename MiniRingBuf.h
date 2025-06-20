/*
	File:    MiniRingBuf.h
	Author:  Light&Electricity
	Date:    2025.6.20
	Version: 0.5
*/
#ifndef _MINIRINGBUF_H
#define _MINIRINGBUF_H

#include <stdint.h>

#include <pthread.h>

/* Type definition */
#define MRB_TYPE_BYTE	uint8_t		// byte type
#define MRB_TYPE_BOOL	uint8_t		// bool type
#define MRB_TYPE_BUF	uint8_t		// buf data type
#define MRB_TYPE_SIZE	uint16_t	// length type
#define MRB_TYPE_USE	uint16_t	// read/write length type, must <= MRB_TYPE_SIZE

/* Settings definition */
#define MRB_CALLBACK_EN				1 // enable err handle callback

/* Thread safety options */
#define MRB_CRITICAL_EN				0 // critical section enable
#define MRB_CRITICAL_START(mrb)		// critical section start, add code here
#define MRB_CRITICAL_END(mrb)		// critical section end, add code here
#define MRB_MUTEX_EN				1 // mutex enable
#define MRB_MUTEX_LOCK(mrb)			pthread_mutex_lock(mrb->mutex)// mutex lock, add code here
#define MRB_MUTEX_UNLOCK(mrb)		pthread_mutex_unlock(mrb->mutex)// mutex unlock, add code here

/* Copy method option */
#define MRB_COPY_METHOD				MRB_COPY_METHOD_MEMCPY
#define MRB_COPY_METHOD_LOOP		0	// use a simple loop
#define MRB_COPY_METHOD_MEMCPY		1	// use memcpy()
#define MRB_COPY_METHOD_MRBCPY		2	// use a simple copy function
#if MRB_COPY_METHOD == MRB_COPY_METHOD_MEMCPY
#define MRB_COPY_FUNC(dest, src, size)	memcpy(dest, src, size)
#else
#define MRB_COPY_FUNC(dest, src, size)	memcpy(dest, src, size)
#endif


#define MRB_SET_SKIPWRITE	0x00
#define MRB_SET_PARTWRITE	0x01
#define MRB_SET_OVERWRITE	0x02


/* Buffer struct definition */
typedef struct _MiniRingBuf{
	MRB_TYPE_BUF *buf;
	MRB_TYPE_SIZE start, end, size; // (size-1) is the max num of items
	MRB_TYPE_BYTE set; // setting
#if MRB_MUTEX_EN
	void *mutex;
#endif
#if MRB_CALLBACK_EN
	void (*callback)(struct _MiniRingBuf*, MRB_TYPE_BYTE);
#endif
}MiniRingBuf;



/* Macro functions */
#define MRB_clear(mrb)	((mrb)->end = (mrb)->start)
#define MRB_empty(mrb)	((mrb)->end == (mrb)->start)
#define MRB_full(mrb)	(((mrb)->start > 0 && (mrb)->end == (mrb)->start - 1) || \
						((mrb)->start == 0 && (mrb)->end == (mrb)->size - 1))
#define MRB_len(mrb)	(((mrb)->end < (mrb)->start) ? \
						((mrb)->size + (mrb)->end - (mrb)->start) : \
						((mrb)->end - (mrb)->start))

/* Write one item. Check buf is not full before writing. */
#define MRB_write_one(mrb, item)	do{ \
	(mrb)->buf[(mrb)->end++] = (item); \
	if((mrb)->end >= (mrb)->size){ (mrb)->end = 0; } \
}while(0)

/* Read one item. Check buf is not empty before reading. */
#define MRB_read_one(mrb, item)	do{ \
	(item) = (mrb)->buf[(mrb)->start++]; \
	if((mrb)->start >= (mrb)->size){ (mrb)->start = 0; } \
}while(0)





/* Functions */
void mrb_init(MiniRingBuf *mrb, MRB_TYPE_BUF *buf, MRB_TYPE_SIZE size, MRB_TYPE_BYTE set);
void mrb_clear(MiniRingBuf *mrb);
MRB_TYPE_BOOL mrb_empty(MiniRingBuf *mrb);
MRB_TYPE_BOOL mrb_full(MiniRingBuf *mrb);
MRB_TYPE_SIZE mrb_len(MiniRingBuf *mrb);
MRB_TYPE_USE mrb_del(MiniRingBuf *mrb, MRB_TYPE_USE len);
MRB_TYPE_USE mrb_read(MiniRingBuf *mrb, MRB_TYPE_BUF *buf, MRB_TYPE_USE len);
MRB_TYPE_USE mrb_write(MiniRingBuf *mrb, MRB_TYPE_BUF *buf, MRB_TYPE_USE len);





#endif



