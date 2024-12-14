#include "MiniRingBuf.h"


/* init mrb struct */
void mrb_init(MiniRingBuf *mrb, MRB_TYPE_BUF *buf, MRB_TYPE_SIZE size, MRB_TYPE_BYTE set)
{
	mrb->start = mrb->end = 0;
	mrb->buf = buf;
	mrb->size = size;
	mrb->set = set;
}

/* clear entire buf */
void mrb_clear(MiniRingBuf *mrb)
{
	mrb->start = mrb->end = 0;
}

/* check if it's empty */
MRB_TYPE_BOOL mrb_empty(MiniRingBuf *mrb)
{
	return MRB_empty(mrb);
}

/* check if it's full */
MRB_TYPE_BOOL mrb_full(MiniRingBuf *mrb)
{
	return MRB_full(mrb);
}

/* get used length */
MRB_TYPE_SIZE mrb_len(MiniRingBuf *mrb)
{
	return MRB_len(mrb);
}

/*  */
MRB_TYPE_USE mrb_del(MiniRingBuf *mrb, MRB_TYPE_USE len)
{
	MRB_TYPE_SIZE ringBufLenNow = mrb_len(mrb); // get current len
	if(ringBufLenNow == 0) return 0;//no data
	if(ringBufLenNow < len) len = ringBufLenNow;//*If the data is insufficient, only delete the existing ones
	mrb->start += len;//*Move the data header to delete
	if(mrb->start >= mrb->size) mrb->start -= mrb->size;
	return len; // *Return the length of successfully delete data
}

/* read from mrb */
MRB_TYPE_USE mrb_read(MiniRingBuf *mrb, MRB_TYPE_BUF *buf, MRB_TYPE_USE len)
{
	MRB_TYPE_SIZE ringBufLenNow = MRB_len(mrb); // get current len
	//if(ringBufLenNow == 0) return 0;//no data
	if(ringBufLenNow < len) len = ringBufLenNow;//*If the data is insufficient, only read the existing ones
	else ringBufLenNow = len; // *Temporarily store the value of len as the return value
	while(len--){
		// *buf++ = mrb->buf[mrb->start++];
		// if(mrb->start >= mrb->size) mrb->start = 0;
		MRB_read_one(mrb, *buf++);
	}
	return ringBufLenNow; // *Return the length of successfully read data
}

/* write to mrb */
MRB_TYPE_USE mrb_write(MiniRingBuf *mrb, MRB_TYPE_BUF *buf, MRB_TYPE_USE len)
{
	MRB_TYPE_SIZE tmpLen = MRB_len(mrb); // get current len
	if(tmpLen + len >= mrb->size){ // no enough space
		if(0) mrb_errHandle(mrb, 0x00); // err handle
		return 2;
	}
#if MRB_COPY_METHOD == MRB_COPY_METHOD_LOOP
	while(len--){
		MRB_write_one(mrb, *buf++);
		// mrb->buf[mrb->end++] = *buf++;
		// if(mrb->end >= mrb->size) mrb->end = 0;
	}
#elif MRB_COPY_METHOD == MRB_COPY_METHOD_MRBCPY

#elif MRB_COPY_METHOD == MRB_COPY_METHOD_MEMCPY
	tmpLen = mrb->size - mrb->end;
	if(len <= tmpLen){
		MRB_COPY_MEMCPY(&mrb->buf[mrb->end], buf, sizeof(MRB_TYPE_BUF) * len);
		mrb->end += len;
	}
	else{
		MRB_COPY_MEMCPY(&mrb->buf[mrb->end], buf, sizeof(MRB_TYPE_BUF) * tmpLen);
		MRB_COPY_MEMCPY(&mrb->buf[0], &buf[tmpLen], sizeof(MRB_TYPE_BUF) * (len - tmpLen));
		mrb->end = len - tmpLen;
	}
#else
	#error "MRB_COPY_METHOD ERROR!"
#endif
	return 0;
}

/* err handle */
MRB_TYPE_BYTE mrb_errHandle(MiniRingBuf *mrb, uint8_t err)
{
	
	return 0;
}




