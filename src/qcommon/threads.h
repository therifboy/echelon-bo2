#ifndef __THREADS_H__
#define __THREADS_H__

#include "../universal/q_shared.h"
#include "cmd.h"

#include <setjmp.h>

struct TraceCheckCount
{
	unsigned short global;
	unsigned short *partitions;
	unsigned short *brushes;
};

typedef char cbrush_t[0x60];
typedef char cmodel_t[0x4C];

typedef struct PhysGeomList;

struct TraceThreadInfo
{
	TraceCheckCount checkcount;
	cbrush_t * box_brush;
	cmodel_t * box_model;
	PhysGeomList ** geoms;
};

struct tls_t
{
	int zero;
	va_info_t* va;
	jmp_buf* env;
	TraceThreadInfo* data;
	CmdArgs* cmd;
};

#endif /* __THREADS_H__ */
