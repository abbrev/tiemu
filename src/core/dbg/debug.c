/* Hey EMACS -*- linux-c -*- */
/* $Id$ */

/*  TiEmu - an TI emulator
 *
 *  Copyright (c) 2000, Thomas Corvazier, Romain Lievin
 *  Copyright (c) 2001-2002, Romain Lievin, Julien Blache
 *  Copyright (c) 2003-2004, Romain Li�vin
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

/*
    Debug: debugging functions
*/

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdio.h>

#include "libuae.h"
#include "m68k.h"
#include "romcalls.h"
#include "debug.h"
#include "ti68k_def.h"
#include "ti68k_err.h"


int ti68k_debug_get_pc(void)
{
	return m68k_getpc();
}

int ti68k_debug_break(void)
{
    specialflags |= SPCFLAG_BRK;
    return 0;
}

int ti68k_debug_trace(void)
{
    // Set up an internal trap (DBTRACE) which will 
    // launch/refresh the debugger when encountered
    specialflags |= SPCFLAG_DBTRACE;

    return 0;
}

int ti68k_debug_step(void)
{
    specialflags |= SPCFLAG_DBSKIP;
	return ti68k_debug_do_instructions(1);
}

static const uint16_t rets[] = { 
	
	0x4e77,		// RTR
	0x4e75,		// RTS
	0x4e74,		// RTD
	0x4e73,		// RTE
	0x4e72,		// STOP
};

static inline is_ret_inst(uint16_t inst)
{
	int i;
	for(i = 0; i < sizeof(rets) / sizeof(uint16_t); i++)
		if(curriword() == rets[i])
			return !0;
	return 0;
}

static inline is_bsr_inst(uint16_t ci)
{
	return 
		((ci >> 6) == 0x13a) ||										/* jsr */
        ((ci >> 8) == 0x61)  ||										/* bsr */
        (ci >= 0xf800 && ci <= 0xffee) ||							/* fline */
		(ci == 0xfff0) || (ci == 0xfff2) ||
		((ci & 0xf000) == 0x5000) && ((ci & 0x00f8) == 0x00c8)	||	/* dbcc */
		((ci & 0x4e40) == 0x4e40)									/* trap */
		;
}

int ti68k_debug_step_over(void)
{
    uint32_t curr_pc, next_pc;
	gchar *output;

	// get current PC and next PC
	curr_pc = m68k_getpc();	
	next_pc = curr_pc + ti68k_debug_disassemble(curr_pc, &output);
	//printf("$%06x => $%06x %04x <%s>\n", curr_pc, next_pc, curriword(), output);
	g_free(output);	

	// check current instruction
	if(!is_bsr_inst(curriword()))
	{
		ti68k_debug_step();
		return 0;
	}

	// run emulation until address after instruction is reached
	do
	{
		hw_m68k_run(1);

		// force GUI refresh in order to be able to cancel operation
		while(gtk_events_pending()) gtk_main_iteration_do(FALSE);
	}
	while ((next_pc != m68k_getpc()) && !(specialflags & SPCFLAG_BRK));

	if(specialflags & SPCFLAG_BRK)
		specialflags &= ~SPCFLAG_BRK;

    return 0;
}

int ti68k_debug_step_out(void)
{
	uint32_t curr_pc, next_pc;
	gchar *output;
	//int i;

	// get current PC and next PC
	curr_pc = m68k_getpc();	
	next_pc = curr_pc + ti68k_debug_disassemble(curr_pc, &output);
	//printf("$%06x => $%06x <%s>\n", curr_pc, next_pc, output);
	g_free(output);	

	// run emulation until address is reached
	do
	{
		hw_m68k_run(1);

		//printf("$%06x: %06x\n", m68k_getpc(), curriword());
		if(is_ret_inst(curriword()))
		{
			hw_m68k_run(1);
			return 0;
		}	
	}
	while(1);

    return 0;
}

int ti68k_debug_skip(uint32_t next_pc)
{
	// run emulation until address is reached
    do 
    {
		ti68k_debug_step();
/*
		// too far: stop
		if(m68k_getpc() > next_pc) 
			break;

		// jump back: stop
		if(next_pc - m68k_getpc() > 0x80) 
			break;
*/
		// force GUI refresh in order to be able to cancel operation
		while(gtk_events_pending()) gtk_main_iteration_do(FALSE);
    } 
    while ((next_pc != m68k_getpc()) && !(specialflags & SPCFLAG_BRK));

	if(specialflags & SPCFLAG_BRK)
		specialflags &= ~SPCFLAG_BRK;

    return 0;
}

int ti68k_debug_do_instructions(int n)
{
    return hw_m68k_run(n);
}

// Used to read/modify/write memory directly from debugger
uint8_t* ti68k_get_real_address(uint32_t addr)
{
	return hw_get_real_address(addr);
}

int ti68k_debug_load_symbols(const char *filename)
{
	if(!strcmp(filename, ""))
		return 0;

	return romcalls_preload(filename);
}
