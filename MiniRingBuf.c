/*
	File:    MiniRingBuf.c
	Author:  Light&Electricity
	Date:    2025.6.19
	Version: 0.5
*/
#include "MiniRingBuf.h"

/* init mrb struct */
void mrb_init(MiniRingBuf *mrb, MRB_TYPE_BUF *buf, MRB_TYPE_SIZE size, MRB_TYPE_BYTE set)
{
	mrb->start = mrb->end = 0;
	mrb->buf = buf;
	mrb->size = size;
	mrb->set = set;
#if MRB_CALLBACK_EN
	mrb->callback = (void*)0; // NULL
#endif
}

/* clear entire buf */
void mrb_clear(MiniRingBuf *mrb)
{
	MRB_clear(mrb);
}

/* check if it's empty */
MRB_TYPE_BOOL mrb_empty(MiniRingBuf *mrb)
{
	return MRB_empty(mrb);
}

/* check if it's full */
MRB_TYPE_BOOL mrb_full(MiniRingBuf *mrb)
{
#if (MRB_MUTEX_EN)
	MRB_MUTEX_LOCK(mrb);
#endif
#if (MRB_CRITICAL_EN)
	MRB_CRITICAL_START(mrb);
#endif

	MRB_TYPE_BOOL ret = MRB_full(mrb);

#if (MRB_CRITICAL_EN)
	MRB_CRITICAL_END(mrb);
#endif
#if (MRB_MUTEX_EN)
	MRB_MUTEX_UNLOCK(mrb);
#endif

	return ret;
}

/* get used length */
MRB_TYPE_SIZE mrb_len(MiniRingBuf *mrb)
{
#if (MRB_MUTEX_EN)
	MRB_MUTEX_LOCK(mrb);
#endif
#if (MRB_CRITICAL_EN)
	MRB_CRITICAL_START(mrb);
#endif

	MRB_TYPE_SIZE ret = MRB_len(mrb);

#if (MRB_CRITICAL_EN)
	MRB_CRITICAL_END(mrb);
#endif
#if (MRB_MUTEX_EN)
	MRB_MUTEX_UNLOCK(mrb);
#endif

	return ret;
}

/* delete items */
MRB_TYPE_USE mrb_del(MiniRingBuf *mrb, MRB_TYPE_USE len)
{
	MRB_TYPE_SIZE tmpLen;

#if (MRB_MUTEX_EN)
	MRB_MUTEX_LOCK(mrb);
#endif
#if (MRB_CRITICAL_EN)
	MRB_CRITICAL_START(mrb);
#endif

	tmpLen = MRB_len(mrb); // get current length
	if(tmpLen < len) len = tmpLen; // no enough data, only delete the existing data
	mrb->start += len; // move the data header to delete
	if(mrb->start >= mrb->size) mrb->start -= mrb->size;

#if (MRB_CRITICAL_EN)
	MRB_CRITICAL_END(mrb);
#endif
#if (MRB_MUTEX_EN)
	MRB_MUTEX_UNLOCK(mrb);
#endif

	return len; // return the length of successfully delete data
}

/* read from mrb */
MRB_TYPE_USE mrb_read(MiniRingBuf *mrb, MRB_TYPE_BUF *buf, MRB_TYPE_USE len)
{
	MRB_TYPE_SIZE tmpLen;

#if (MRB_MUTEX_EN)
	MRB_MUTEX_LOCK(mrb);
#endif
#if (MRB_CRITICAL_EN)
	MRB_CRITICAL_START(mrb);
#endif

	tmpLen = MRB_len(mrb); // get current length
	if(tmpLen < len) len = tmpLen; // no enough data, only read the existing data
	else tmpLen = len; // temporarily store the value of len as the return value

#if MRB_COPY_METHOD == MRB_COPY_METHOD_LOOP
	while(len--){ MRB_read_one(mrb, *buf++); }
#elif MRB_COPY_METHOD == MRB_COPY_METHOD_MRBCPY

#elif MRB_COPY_METHOD == MRB_COPY_METHOD_MEMCPY

#else
	#error "MRB_COPY_METHOD ERROR!"
#endif

#if (MRB_CRITICAL_EN)
	MRB_CRITICAL_END(mrb);
#endif
#if (MRB_MUTEX_EN)
	MRB_MUTEX_UNLOCK(mrb);
#endif

	return tmpLen; // return the length of successfully read data
}

/* write to mrb */
MRB_TYPE_USE mrb_write(MiniRingBuf *mrb, MRB_TYPE_BUF *buf, MRB_TYPE_USE len)
{
	MRB_TYPE_SIZE tmpLen;

#if (MRB_MUTEX_EN)
	MRB_MUTEX_LOCK(mrb);
#endif
#if (MRB_CRITICAL_EN)
	MRB_CRITICAL_START(mrb);
#endif

	tmpLen = MRB_len(mrb); // get current length
	if(tmpLen + len >= mrb->size){ // no enough space
#if MRB_CALLBACK_EN
		if(mrb->callback) mrb->callback(mrb, 123);
#endif
		if(mrb->set == MRB_SET_NOWRITE) len = 0;
		else if(mrb->set == MRB_SET_PARTWRITE) len = mrb->size - tmpLen - 1;
		else if(mrb->set == MRB_SET_OVERWRITE){
			len = 0;
		}
	}
	tmpLen = len; // temporarily store the value of len as the return value

#if MRB_COPY_METHOD == MRB_COPY_METHOD_LOOP
	while(len--){ MRB_write_one(mrb, *buf++); }
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

#if (MRB_CRITICAL_EN)
	MRB_CRITICAL_END(mrb);
#endif
#if (MRB_MUTEX_EN)
	MRB_MUTEX_UNLOCK(mrb);
#endif

	return tmpLen;
}






