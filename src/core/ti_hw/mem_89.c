/* Hey EMACS -*- linux-c -*- */
/* $Id: main.c 245 2004-05-23 20:45:43Z roms $ */

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
    Memory management: TI89/92+/V200 FLASH without any HW protection
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "libuae.h"
#include "ports.h"
#include "hw.h"
#include "mem.h"
#include "images.h"
#include "bkpts.h"
#include "m68k.h"
#include "ti68k_def.h"
#include "ti68k_int.h"
#include "flash.h"

/* Put/Get byte/word/longword */
#define bput(adr, arg) { mem_tab[(adr)>>20][(adr) & mem_mask[(adr)>>20]] = (arg); }
#define wput(adr, arg) { bput((adr), (arg)>> 8); bput((adr)+1, (arg)&0x00ff); }
#define lput(adr, arg) { wput((adr), (uint16_t)((arg)>>16)); wput((adr)+2, (uint16_t)((arg)&0xffff)); }

#define bget(adr) (mem_tab[(adr)>>20][(adr)&mem_mask[(adr)>>20]])
#define wget(adr) ((uint16_t)(((uint16_t)bget(adr))<< 8 | bget((adr)+1)))
#define lget(adr) ((uint32_t)(((uint32_t)wget(adr))<<16 | wget((adr)+2)))

#define IN_RANGE(a,v,b)	(((v) >= (a)) && ((v) <= (b)))

uint32_t ti89_get_long(uint32_t adr) 
{
    if(IN_RANGE(0x000000, adr, 0x1f0000))				// RAM access
	{
        return lget(adr);
	}
    else if(IN_RANGE(0x200000, adr, 0x5fffff))			// FLASH access
	{
        return (lget(adr) | wsm.ret_or);
	}
    else if(IN_RANGE(0x600000, adr, 0x6fffff))			// memory-mapped I/O
	{
       return io_get_long(adr & 0x1f);
	}
	else if(IN_RANGE(0x700000, adr, 0x7fffff))			// memory-mapped I/O (hw2)
	{
		return io2_get_long(adr & 0x1f);
	}

    return 0;
}

uint16_t ti89_get_word(uint32_t adr) 
{
    if(IN_RANGE(0x000000, adr, 0x1f0000))				// RAM access
	{
        return wget(adr);
	}
    else if(IN_RANGE(0x200000, adr, 0x5fffff))			// FLASH access
	{
        return (wget(adr) | wsm.ret_or);
	}
    else if(IN_RANGE(0x600000, adr, 0x6fffff))			// memory-mapped I/O
	{
       return io_get_word(adr & 0x1f);
	}
	else if(IN_RANGE(0x700000, adr, 0x7fffff))			// memory-mapped I/O (hw2)
	{
		return io2_get_word(adr & 0x1f);
	}

    return 0;
}

uint8_t ti89_get_byte(uint32_t adr) 
{
    if(IN_RANGE(0x000000, adr, 0x1f0000))				// RAM access
	{
        return bget(adr);
	}
    else if(IN_RANGE(0x200000, adr, 0x5fffff))			// FLASH access
	{
        return (bget(adr) | wsm.ret_or);
	}
    else if(IN_RANGE(0x600000, adr, 0x6fffff))			// memory-mapped I/O
	{
       return io_get_byte(adr & 0x1f);
	}
	else if(IN_RANGE(0x700000, adr, 0x7fffff))			// memory-mapped I/O (hw2)
	{
		return io2_get_byte(adr & 0x1f);
	}

    return 0;
}

void ti89_put_long(uint32_t adr, uint32_t arg) 
{
    if(IN_RANGE(0x000000, adr, 0x1f0000))				// RAM access
	{
        lput(adr, arg);
	}
    else if(IN_RANGE(0x200000, adr, 0x5fffff))			// FLASH access
	{
        FlashWriteByte(adr,(arg>>24)&0xff);
        FlashWriteByte(adr+1,(arg>>16)&0xff);
        FlashWriteByte(adr+2,(arg>>8)&0xff);
        FlashWriteByte(adr+3,arg&0xff);
	}
    else if(IN_RANGE(0x600000, adr, 0x6fffff))			// memory-mapped I/O
	{
		io_put_long(adr & 0x1f, arg);
	}
	else if(IN_RANGE(0x700000, adr, 0x7fffff))			// memory-mapped I/O (hw2)
	{
		io2_put_long(adr & 0x1f, arg);
	}

    return;
}

void ti89_put_word(uint32_t adr, uint16_t arg) 
{
	if(IN_RANGE(0x000000, adr, 0x1f0000))				// RAM access
	{
        wput(adr, arg);
	}
    else if(IN_RANGE(0x200000, adr, 0x5fffff))			// FLASH access
	{
        FlashWriteByte(adr,(arg>>8)&0xff);
	    FlashWriteByte(adr+1,arg&0xff);
	}
    else if(IN_RANGE(0x600000, adr, 0x6fffff))			// memory-mapped I/O
	{
		io_put_word(adr & 0x1f, arg);
	}
	else if(IN_RANGE(0x700000, adr, 0x7fffff))			// memory-mapped I/O (hw2)
	{
		io2_put_word(adr & 0x1f, arg);
	}

    return;
}

void ti89_put_byte(uint32_t adr, uint8_t arg) 
{
    if(IN_RANGE(0x000000, adr, 0x1f0000))				// RAM access
	{
        bput(adr, arg);
	}
    else if(IN_RANGE(0x200000, adr, 0x5fffff))			// FLASH access
	{
        FlashWriteByte(adr,arg&0xff);
	}
    else if(IN_RANGE(0x600000, adr, 0x6fffff))			// memory-mapped I/O
	{
		io_put_byte(adr & 0x1f, arg);
	}
	else if(IN_RANGE(0x700000, adr, 0x7fffff))			// memory-mapped I/O (hw2)
	{
		io2_put_byte(adr & 0x1f, arg);
	}

    return;
}
