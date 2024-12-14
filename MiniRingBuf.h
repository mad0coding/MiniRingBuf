#ifndef _MINIRINGBUF_H
#define _MINIRINGBUF_H

#include <stdint.h>

/* Type definition */
#define MRB_TYPE_BYTE	uint8_t		// byte type
#define MRB_TYPE_BOOL	uint8_t		// bool type
#define MRB_TYPE_BUF	uint8_t		// buf data type
#define MRB_TYPE_SIZE	uint16_t	// length type
#define MRB_TYPE_USE	uint16_t	// read/write length type, must <= MRB_TYPE_SIZE


/* Copy method definition */
#define MRB_COPY_METHOD			MRB_COPY_METHOD_LOOP
#define MRB_COPY_METHOD_LOOP	0 // use a simple loop
#define MRB_COPY_METHOD_MEMCPY	1 // use memcpy()
#define MRB_COPY_METHOD_MRBCPY	2 // use a simple copy function
#if MRB_COPY_METHOD == MRB_COPY_METHOD_MEMCPY
#define MRB_COPY_FUNC(dest, src, size)	memcpy(dest, src, size)
#else
#define MRB_COPY_FUNC(dest, src, size)	memcpy(dest, src, size)
#endif


#define MRB_SET_BUF 0x01
#define MRB_SET_BUF 0x02
#define MRB_SET_BUF 0x04
#define MRB_SET_BUF 0x08

/* Critical section definition */
#define MRB_CRITICAL_START
#define MRB_CRITICAL_END

/* Buffer struct definition */
typedef struct _MiniRingBuf{
	MRB_TYPE_BUF *buf;
	MRB_TYPE_SIZE start, end, size;
	MRB_TYPE_BYTE set; // setting
}MiniRingBuf;



/* Macro function definition */
#define MRB_empty(mrb)	((mrb)->end == (mrb)->start)
#define MRB_full(mrb)	(((mrb)->start > 0 && (mrb)->end == (mrb)->start - 1) || \
						((mrb)->start == 0 && (mrb)->end == (mrb)->size - 1))
#define MRB_len(mrb)	(((mrb)->end < (mrb)->start) ? \
						((mrb)->size + (mrb)->end - (mrb)->start) : \
						((mrb)->end - (mrb)->start))

/* Check buf is not full before writing. */
#define MRB_write_one(mrb, entity)	do{ \
	(mrb)->buf[(mrb)->end++] = (entity); \
	if((mrb)->end >= (mrb)->size){ (mrb)->end = 0; } \
}while(0)

/* Check buf is not empty before reading. */
#define MRB_read_one(mrb, entity)	do{ \
	(entity) = (mrb)->buf[(mrb)->start++]; \
	if((mrb)->start >= (mrb)->size){ (mrb)->start = 0; } \
}while(0)






void mrb_init(MiniRingBuf *mrb, MRB_TYPE_BUF *buf, MRB_TYPE_SIZE size, MRB_TYPE_BYTE set);
void mrb_clear(MiniRingBuf *mrb);
MRB_TYPE_BOOL mrb_empty(MiniRingBuf *mrb);
MRB_TYPE_BOOL mrb_full(MiniRingBuf *mrb);
MRB_TYPE_SIZE mrb_len(MiniRingBuf *mrb);
MRB_TYPE_USE mrb_del(MiniRingBuf *mrb, MRB_TYPE_USE len);
MRB_TYPE_USE mrb_read(MiniRingBuf *mrb, MRB_TYPE_BUF *buf, MRB_TYPE_USE len);
MRB_TYPE_USE mrb_write(MiniRingBuf *mrb, MRB_TYPE_BUF *buf, MRB_TYPE_USE len);
MRB_TYPE_BYTE mrb_errHandle(MiniRingBuf *mrb, uint8_t err);




#endif



