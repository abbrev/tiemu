/* Hey EMACS -*- linux-c -*- */
/* $Id$ */

/*  TiEmu - Tiemu Is an EMUlator
 *
 *  Copyright (c) 2000-2001, Thomas Corvazier, Romain Lievin
 *  Copyright (c) 2001-2003, Romain Lievin
 *  Copyright (c) 2003, Julien Blache
 *  Copyright (c) 2004, Romain Li�vin
 *  Copyright (c) 2005-2006, Romain Li�vin
 *  Copyright (c) 2006, Kevin Kofler
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
 *  Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston, MA 02110-1301, USA.
 */

/*
  	State images
*/

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <string.h>

#include "libuae.h"
#include "ti68k_int.h"
#include "ti68k_err.h"
#include "flash.h"
#include "logging.h"
#include "rtc_hw3.h"

#define SAV_REVISION	21	// Current revision
#define SAV_MINI		20	// Minimum supported revision

int ti68k_state_parse(const char *filename, char **rom_file, char **tib_file)
{
	FILE *f;
  	IMG_INFO img;
  	SAV_INFO sav;
	long pos, len;
	int ret = 0;

	// No filename, exits
	if(!strcmp(filename, ""))
		return 0;
  
  	// Open file
  	f = fopen(filename, "rb");
  	if(f == NULL)
  		return ERR_CANT_OPEN_STATE;
  	
  	// Load ROM image header
	fread(&img, 1, sizeof(IMG_INFO), f);

    // Determine state image revision for backwards compatibility and load
	pos = ftell(f);
	fread(&sav.revision, sizeof(sav.revision), 1, f);
	fread(&sav.size, sizeof(sav.revision), 1, f);
	fseek(f, pos, SEEK_SET);
	fread(&sav, 1, sav.size, f);

	if(sav.revision < SAV_MINI)
	{
		ret = -2;
		goto ti68k_state_parse_exit;
	}

	ret = fseek(f, sav.str_offset, SEEK_SET);

	fread(&len, 1, sizeof(long), f);
	*rom_file = (char *)g_malloc(len);
	fread(*rom_file, 1, len, f);

	fread(&len, 1, sizeof(long), f);
	*tib_file = (char *)g_malloc(len);
	fread(*tib_file, 1, len, f);

	if(strcmp(params.rom_file, *rom_file) || strcmp(params.tib_file, *tib_file))
	{
		ret = -2;
		goto ti68k_state_parse_exit;
	}

ti68k_state_parse_exit:
	fclose(f);
	return ret;
}

static int load_bkpt(FILE *f, GList **l)
{
    int ret;
    int i;
    long n, v;

    ret = fread(&n, sizeof(n), 1, f);
    for(i = 0; i < n; i++)
    {
        ret = fread(&v, sizeof(v), 1, f);
        *l = g_list_append(*l, GINT_TO_POINTER(v));
    }

    return ret;
}

static int load_bkpt2(FILE *f, GList **l)
{
    int ret;
    int i;
    long n;

    ret = fread(&n, sizeof(n), 1, f);
    for(i = 0; i < n; i++)
    {
        ADDR_RANGE *s = g_malloc(sizeof(ADDR_RANGE));

        ret = fread(s, sizeof(ADDR_RANGE), 1, f);
        *l = g_list_append(*l, s);
    }

    return ret;
}

static int load_bkpt3(FILE *f, GList **l)
{
    int ret;
    int i;
    long n;

    ret = fread(&n, sizeof(n), 1, f);
    for(i = 0; i < n; i++)
    {
        ADDR_BIT *s = g_malloc(sizeof(ADDR_BIT));

        ret = fread(s, sizeof(ADDR_BIT), 1, f);
        *l = g_list_append(*l, s);
    }

    return ret;
}

/*
  Must be done between init_hardware and M68000_run.
  Typically called after initLib68k.
  This function (re)load the state of the calculator.
  It automagically determine the state file format.

  Return an error code if an error occured, 0 otherwise
*/
int ti68k_state_load(const char *filename)
{
	FILE *f;
  	IMG_INFO img;
  	SAV_INFO sav;
	Ti68kHardware thw;
    int ret;
	long pos;
	int i;
	gchar *rf=NULL, *tf=NULL;
  
  	// No filename, exits
	if(!strcmp(filename, ""))
		return 0;
  
  	// Open file
  	tiemu_info(_("loading state image: %s"), g_basename(filename));
  	f = fopen(filename, "rb");
  	if(f == NULL)
  		return ERR_CANT_OPEN_STATE;
  	
  	// Load ROM image header
	fread(&img, 1, sizeof(IMG_INFO), f);

    // Determine state image revision and load state image header
	pos = ftell(f);
	fread(&sav.revision, sizeof(sav.revision), 1, f);
	fread(&sav.size, sizeof(sav.revision), 1, f);
	fseek(f, pos, SEEK_SET);
	fread(&sav, 1, sav.size, f);

	if(sav.revision < SAV_MINI)
	{
		fclose(f);
		return ERR_REVISION_MATCH;
	}

	// Does not accept state image different of emulator image
	if(ti68k_state_parse(filename, &rf, &tf) < 0)
	{
		if (rf) g_free(rf);
		if (tf) g_free(tf);
		return ERR_STATE_MATCH;
	}
	if (rf) g_free(rf);
	if (tf) g_free(tf);

	// Compare image infos with current image
	if(memcmp(&img, &img_infos, sizeof(IMG_INFO) - sizeof(char *)))
		return ERR_HEADER_MATCH;
	
	// Load internal hardware (registers and special flags)
    ret = fseek(f, sav.regs_offset, SEEK_SET);
    fread(&regs, sizeof(regs), 1, f);
    
    // Load I/O ports state
    ret = fseek(f, sav.io_offset, SEEK_SET);
    fread(tihw.io , tihw.io_size, 1, f);
    fread(tihw.io2, tihw.io2_size, 1, f);
	fread(tihw.io3, tihw.io3_size, 1, f);
    
    // Load RAM content
    ret = fseek(f, sav.ram_offset, SEEK_SET);
    fread(tihw.ram, tihw.ram_size, 1, f);

	// Load extra infos
	ret = fseek(f, sav.misc_offset, SEEK_SET);
	fread(&thw, sizeof(Ti68kHardware), 1, f);
	tihw.on_off = thw.on_off;
	tihw.lcd_adr = thw.lcd_adr;
	tihw.contrast = thw.contrast;
	tihw.protect = thw.protect;
	tihw.archive_limit = thw.archive_limit;
	memcpy(tihw.ram_exec, thw.ram_exec, sizeof(tihw.ram_exec));

	tihw.rtc3_beg = thw.rtc3_beg;
	tihw.rtc3_load = thw.rtc3_load;
	rtc3_state_load();

	// Load modified FLASH segments
	ret = fseek(f, sav.rom_offset, SEEK_SET);
	for(i=0; i<wsm.nblocks; i++)
    {
		fread(&wsm.changed[i], sizeof(int), 1, f);
		if(wsm.changed[i])
			fread(&tihw.rom[i<<16], 1, 65536, f);
    }

    // Load bkpts
    ti68k_bkpt_clear_access();
	ti68k_bkpt_clear_range();
    ti68k_bkpt_clear_address();
	ti68k_bkpt_clear_exception();
	ti68k_bkpt_clear_pgmentry();

    ret = fseek(f, sav.bkpts_offset, SEEK_SET);
    load_bkpt(f, &bkpts.code);
    load_bkpt(f, &bkpts.exception);
	load_bkpt(f, &bkpts.pgmentry);

    load_bkpt(f, &bkpts.mem_rb);
	load_bkpt(f, &bkpts.mem_rw);
	load_bkpt(f, &bkpts.mem_rl);
	load_bkpt(f, &bkpts.mem_wb);
	load_bkpt(f, &bkpts.mem_ww);
	load_bkpt(f, &bkpts.mem_wl);

    load_bkpt2(f, &bkpts.mem_rng_r);
	load_bkpt2(f, &bkpts.mem_rng_w);

	if(sav.revision >= 21)
		load_bkpt3(f, &bkpts.bits);
    
	// Update UAE structures
	m68k_setpc(m68k_getpc());
    MakeFromSR();

	fclose(f);

	// Update SAV file to latest revision
	if(sav.revision < SAV_REVISION)
	{
		ti68k_state_save(filename);
	}

  	return 0;
}

static void save_bkpt(FILE *f, GList *l)
{
    int i;
    long n, v;

    n = g_list_length(l);
    fwrite(&n, sizeof(n), 1, f);
    for(i = 0; i < n; i++)
    {
        v = GPOINTER_TO_INT(g_list_nth(l, i)->data);
        fwrite(&v, sizeof(v), 1, f);
    }
}

static void save_bkpt2(FILE *f, GList *l)
{
    int i;
    long n;

    n = g_list_length(l);
    fwrite(&n, sizeof(n), 1, f);
    for(i = 0; i < n; i++)
    {
        ADDR_RANGE *s = g_list_nth(l, i)->data;

        fwrite(s, sizeof(ADDR_RANGE), 1, f);
    }
}

static void save_bkpt3(FILE *f, GList *l)
{
	int i;
    long n;

    n = g_list_length(l);
    fwrite(&n, sizeof(n), 1, f);
    for(i = 0; i < n; i++)
    {
        ADDR_BIT *s = g_list_nth(l, i)->data;

        fwrite(s, sizeof(ADDR_BIT), 1, f);
    }
}

/*
  	This function saves the state of the calculator.
  	Can be called at any time.

  	Return an error code if an error occured, 0 otherwise
*/
int ti68k_state_save(const char *filename)
{
  	FILE *f;
  	IMG_INFO *img = &img_infos;
    SAV_INFO sav;
	int i;
	long len;
	long bkpts_size;
  
  	if(!strlen(filename))
  		return ERR_CANT_OPEN_STATE;
  
  	// Open file
  	tiemu_info(_("saving state image (TiEmu v2.00 format): %s"), filename);
  	f = fopen(filename, "wb");
  	if(f == NULL)
  		return ERR_CANT_OPEN_STATE;
  	
  	// Save current image infos
	fwrite(img, 1, sizeof(IMG_INFO), f);

    // Fill state image infos
	sav.revision = SAV_REVISION;
	sav.size = sizeof(SAV_INFO);

    sav.regs_offset = sizeof(IMG_INFO) + sizeof(SAV_INFO);
    sav.io_offset = sav.regs_offset + sizeof(regs);
    sav.ram_offset = sav.io_offset + tihw.io_size + tihw.io2_size + tihw.io3_size;
	sav.misc_offset = sav.ram_offset + tihw.ram_size;
	sav.rom_offset = sav.misc_offset + sizeof(Ti68kHardware);
    sav.bkpts_offset = sav.rom_offset + wsm.nblocks*sizeof(int) + hw_flash_nblocks()*65536;

	bkpts_size = 
		g_list_length(bkpts.code) * sizeof(long) + sizeof(long) +
		g_list_length(bkpts.exception) * sizeof(long) + sizeof(long) +
		g_list_length(bkpts.pgmentry) * sizeof(long) + sizeof(long) +
		g_list_length(bkpts.mem_rb) * sizeof(long) + sizeof(long) +
		g_list_length(bkpts.mem_rw) * sizeof(long) + sizeof(long) +
		g_list_length(bkpts.mem_rl) * sizeof(long) + sizeof(long) +
		g_list_length(bkpts.mem_wb) * sizeof(long) + sizeof(long) +
		g_list_length(bkpts.mem_ww) * sizeof(long) + sizeof(long) +
		g_list_length(bkpts.mem_wl) * sizeof(long) + sizeof(long) +
		g_list_length(bkpts.mem_rng_r) * sizeof(ADDR_RANGE) + sizeof(long) +
		g_list_length(bkpts.mem_rng_w) * sizeof(ADDR_RANGE) + sizeof(long) +
		g_list_length(bkpts.bits) * sizeof(ADDR_BIT) + sizeof(long)
		;

	sav.str_offset = sav.bkpts_offset + bkpts_size;

    fwrite(&sav, 1, sizeof(SAV_INFO), f);
	
	// Update UAE structures
    MakeSR();
    m68k_setpc(m68k_getpc());
    
    // Save registers and special flags
    fwrite(&regs, sizeof(regs), 1, f);
    
    // Save I/O ports state
    fwrite(tihw.io , tihw.io_size, 1, f);
    fwrite(tihw.io2, tihw.io2_size, 1, f);
	fwrite(tihw.io3, tihw.io3_size, 1, f);	
    
    // Save RAM content
    fwrite(tihw.ram, tihw.ram_size, 1, f);

	// Save misc informations
	rtc3_state_save();
	fwrite(&tihw, sizeof(Ti68kHardware), 1, f);

	// Save modified FLASH segments
	for(i=0; i<wsm.nblocks; i++)
    {
		fwrite(&wsm.changed[i], sizeof(int), 1, f);
		if(wsm.changed[i])
			fwrite(&tihw.rom[i<<16], 1, 65536, f);
    }

    // Save breakpoints
    save_bkpt(f, bkpts.code);
    save_bkpt(f, bkpts.exception);
	save_bkpt(f, bkpts.pgmentry);

    save_bkpt(f, bkpts.mem_rb);
	save_bkpt(f, bkpts.mem_rw);
	save_bkpt(f, bkpts.mem_rl);
	save_bkpt(f, bkpts.mem_wb);
	save_bkpt(f, bkpts.mem_ww);
	save_bkpt(f, bkpts.mem_wl);

    save_bkpt2(f, bkpts.mem_rng_r);
	save_bkpt2(f, bkpts.mem_rng_w);

	save_bkpt3(f, bkpts.bits);

	// Save image location associated with this state image
	len = strlen(params.rom_file) + 1;
	fwrite(&len, 1, sizeof(len), f);
	fwrite(params.rom_file, len, 1, f);
	
	len = strlen(params.tib_file) + 1;
	fwrite(&len, 1, sizeof(len), f);
	fwrite(params.tib_file, len, 1, f);
    
    fclose(f);

  	return 0;
}
