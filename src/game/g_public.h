#ifndef __G_PUBLIC_H__
#define __G_PUBLIC_H__

struct entityShared_t
{
	char linked; //0x0
	char bmodel; //0x1
	char svFlags; //0x2
	char inuse; //0x3
	int broadcastTime; //0x4
	float mins[3]; //0x8
	float maxs[3]; //0x14
	int contents; //0x20
	float absmin[3]; //0x24
 	float absmax[3]; //0x30
	float currentOrigin[3]; //0x3C
	float currentAngles[3]; //0x48
	int ownerNum; //0x54
	int eventTime; //0x58
}; //0x5C

#endif /* __G_PUBLIC_H__ */