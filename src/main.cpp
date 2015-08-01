#include "handle.h"
#include "utils.h"
#include "echelon.h"
#include "echelon_mp.h"
#include "precache.h"
#include "stdio.h"

#include "client_mp/client_mp.h"
#include "game/g_local.h"
#include "qcommon/qcommon.h"
#include "qcommon/threads.h"

#include <sys/ppu_thread.h>
#include <sys/timer.h>
#include <sys/prx.h>
#include <sysutil/sysutil_msgdialog.h>

SYS_MODULE_INFO( Echelon, 0, 1, 5);
SYS_MODULE_START( main );

//#define ZOMBIE_MODE

#define YES_NO			(CELL_MSGDIALOG_TYPE_BUTTON_TYPE_YESNO|CELL_MSGDIALOG_TYPE_DISABLE_CANCEL_ON)

#define DIALOG_RES_WAITING		1
#define DIALOG_RES_YES			2
#define DIALOG_RES_NO			3

int dialog_result = DIALOG_RES_WAITING;

void DialogCallback(int buttonType, void *userData )
{
	switch(buttonType)
	{
	case CELL_MSGDIALOG_BUTTON_YES:
		dialog_result = DIALOG_RES_YES;
		break;
	case CELL_MSGDIALOG_BUTTON_NO:
		dialog_result = DIALOG_RES_NO;
		break;
	}
}

/// should only be called from one thread to avoid undefined behavior
/// must be inlined to avoid stack from being overriden
inline int SetupThreadData()
{
	static va_info_t va_buffer;
	static jmp_buf env;
	static cbrush_t box_brush;
	static cmodel_t box_model;
	static PhysGeomList* geoms;
	static unsigned short partitions[0x1ED0];
	static unsigned short brushes[0x1ED0];
	static TraceThreadInfo threadInfo = { { 0, partitions, brushes }, &box_brush, &box_model, &geoms };
	static CmdArgs cmd_args;
	static tls_t tls = { 0, &va_buffer, &env, &threadInfo, &cmd_args };

	register sys_addr_t tls_ptr asm("13");
	*(sys_addr_t*)(tls_ptr + TLS_BGS) = LEVEL_BGS;
	*(sys_addr_t*)(tls_ptr + TLS_VALUE) = (sys_addr_t)&tls;

	int ret = setjmp(env);

	// reset cmd_args everytime an exception occurs
	cmd_args.nesting = -1;
	cmd_args.totalUsedArgvPool = 0;
	cmd_args.totalUsedTextPool = 0;
	return ret;
}

void thread_entry(uint64_t)
{
	sys_timer_sleep(10);
	cellMsgDialogOpen2(YES_NO, "Load Echelon?", DialogCallback, NULL, NULL);
	dialog_result = DIALOG_RES_WAITING;

	while (dialog_result == DIALOG_RES_WAITING)
	{
		cellSysutilCheckCallback();
		Sleep(20);
	}

	if (dialog_result == DIALOG_RES_YES)
	{
		bool ingame = false, precached = false;
		
		if (SetupThreadData())
		{
			// wait 5 seconds whenever longjmp
			// is called with a value != 0
			sys_timer_sleep(5);
		}
		for (size_t i = 0; i < ARRAY_SIZE(Echelon); i++)
		{
			Echelon[i].destroy();
		}

		TTY_Write("Echelon[0]\n");
		Echelon[0].setClient(0);
		Echelon[0].setBackColor(back_colors[0]);
		Echelon[0].setEdgeColor(edge_colors[0], edge_dark_colors[0]);
		Echelon[0].verify(VERIF_HOST);

		level->matchState.unarchivedState.matchUIVisibilityFlags |= UNARCHIVED_GAME_ENDED;
		ingame = false;
		precached = false;

		for (;;)
		{
			if (!ingame)
			{
				if (!precached)
				{
					if (!(level->matchState.unarchivedState.matchUIVisibilityFlags & UNARCHIVED_GAME_ENDED))
					{
						sys_timer_sleep(1);
						TTY_Write("precaching Shaders\n");
						precacheShaders();
						TTY_Write("precaching Models\n");
						precacheModels();
						TTY_Write("precaching Effects\n");
						precacheEffects();
						TTY_Write("precaching WeaponModels\n");
						precacheWeaponModels();
						TTY_Write("initializing...\n");
						modmenu_s::initialize(&menu_title_mp, &main_menu_mp, "MAIN MENU");
						precached = 1;
					}
				}
				else
				{
					if (cl_ingame[0]->current.boolean)
					{
						TTY_Write("Enabling Echelon\n");
						Echelon[0].enable();
						ingame = 1;
					}
				}
			}
			else
			{
				if (level->matchState.unarchivedState.matchUIVisibilityFlags & UNARCHIVED_GAME_ENDED)
				{
					TTY_Write("Disabling Echelon\n");
					Echelon[0].disable();
					ingame = 0;
					precached = 0;
					com_timescale[0]->current.value = 1;
					handleStopAll();
				}
				handleExecute();
			}
			Sleep(5);
		}
	}
	system_call_1(41, 0);
}

// ingame_loader has been updated to add support for argc and argv
extern "C" int main(int argc, char* argv[])
{
	sys_ppu_thread_t id;
	sys_ppu_thread_create(&id, thread_entry, 0, 1010, 0x10000, 0, "Echelon");

#ifdef ZOMBIE_MODE
	// Launches zombie mode instead of multiplayer
		sys_ppu_thread_stack_t sp;
		sys_ppu_thread_get_stack_information(&sp);
		sys_addr_t addr = sp.pst_addr + sp.pst_size - 0x1000;
		*(int32_t*)addr = 0xA;
#endif
    return SYS_PRX_RESIDENT;
}
