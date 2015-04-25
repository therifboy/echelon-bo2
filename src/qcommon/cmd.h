#ifndef __CMD_H__
#define __CMD_H__

// cmd.h -- Command buffer and command execution

#define CMD_MAX_NESTING		8

struct CmdArgs
{
	int nesting; //0x0
	int localClientNum[CMD_MAX_NESTING]; //0x4
	int controllerIndex[CMD_MAX_NESTING]; //0x24
	void *itemDef[CMD_MAX_NESTING]; //0x44
	int argshift[CMD_MAX_NESTING]; //0x64
	int argc[CMD_MAX_NESTING]; //0x84
	const char **argv[CMD_MAX_NESTING]; //0xA4
	char textPool[0x2000]; //0xC4
	const char* argvPool[0x200]; //0x20C4
	int usedTextPool[CMD_MAX_NESTING]; //0x28C4
	int totalUsedArgvPool; //0x28E4
	int totalUsedTextPool;//0x28E8
};

#endif /* __CMD_H__ */
