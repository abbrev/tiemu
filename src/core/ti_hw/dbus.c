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
    Linkport management (D-bus serial input/output)
*/

#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>
#include <time.h>
#include <string.h>

#include "libuae.h"
#include "ports.h"
#include "dbus.h"
#include "m68k.h"

#include "tilibs.h"
#include "ti68k_err.h"
#include "ti68k_def.h"
#include "ti68k_int.h"

/*
	Linkport (lp) / directfile (df) mappers
*/

void	(*hw_dbus_putbyte)		(uint8_t arg);	// TI -> outside
uint8_t (*hw_dbus_getbyte)		(void);			// outside -> TI
int		(*hw_dbus_byteavail)	(void);
int		(*hw_dbus_checkread)	(void);

static void    lp_putbyte(uint8_t arg);
static uint8_t lp_getbyte(void);
static int     lp_byteavail(void);
static int     lp_checkread(void);

static void    df_putbyte(uint8_t arg);
static uint8_t df_getbyte(void);
static int     df_byteavail(void);
static int     df_checkread(void);

static void map_dbus_to_cable(void)
{
    hw_dbus_putbyte = lp_putbyte;
    hw_dbus_getbyte = lp_getbyte;
    hw_dbus_byteavail = lp_byteavail;
    hw_dbus_checkread = lp_checkread;
}

static void map_dbus_to_file(void)
{
    hw_dbus_putbyte = df_putbyte;
    hw_dbus_getbyte = df_getbyte;
    hw_dbus_byteavail = df_byteavail;
    hw_dbus_checkread = df_checkread;
}

/*
    D-bus management (HW linkport)
*/

static int init_link_cable(void);
static int exit_link_cable(void);
static int init_link_file(void);
static int exit_link_file(void);

int hw_dbus_init(void)
{
	// init link_cable
	init_link_cable();

	// init directfile
	init_link_file();

	if(link_cable.link_type == LINK_NUL)
		map_dbus_to_file();		// set mappers to link_cable
	else
		map_dbus_to_cable();	// set mappers to link_cable

    return 0;
}

int hw_dbus_reset(void)
{
    //hw_dbus_exit();
    //hw_dbus_init();

	return 0;
}

int hw_dbus_exit(void)
{
	// exit link_cable
	exit_link_cable();

	// exit link_file
	exit_link_file();

    return 0;
}

/*
	Link cable access
*/

TicableLinkCable lc;	// used in ports.c for direct access

static int init_link_cable(void)
{
	int err;

	ticable_init();
	ticable_set_param(&link_cable);
	ticable_set_cable(link_cable.link_type, &lc);

	if((err = lc.init()))
	{
		tiemu_error(err, NULL);
		return -1;
	}

	if((err = lc.open()))
	{
		tiemu_error(err, NULL);
		return -1;
	}

	return 0;
}

static int exit_link_cable(void)
{
	int err;

	if((err = lc.close()))
	{
		tiemu_error(err, NULL);
		return -1;
	}

	if((err = lc.exit()))
	{ 
		tiemu_error(err, NULL);
		return -1;
	}

	return 0;
}

static int   lp_avail_byte;
static uint8_t lp_last_byte;

static void lp_putbyte(uint8_t arg)
{
	int err;
  
    err = lc.put(arg);
	if(err)
	{
		io_bit_set(0x0d,7);	// error
		tiemu_error(err, NULL);
		return;
	}
	io_bit_set(0x0d,6);		// tx buffer empty
	io_bit_set(0x0d,2);		// link activity
}

static uint8_t lp_getbyte(void)
{
	lp_avail_byte = 0;

	return lp_last_byte;
}

static int lp_byteavail(void)
{
	return lp_avail_byte;
}

static int lp_checkread(void)
{
	int err = 0;
	int status = 0;

	if(lp_avail_byte)
	    return 0;
	
	err = lc.check(&status);
	if(err)
	{
	    io_bit_set(0x0d,7);		// error
	    tiemu_error(err, NULL);
	    lp_last_byte = 0;
	}
  
	if(status & STATUS_RX)
	{
        err = lc.get(&lp_last_byte);
		if(err)
        {
			io_bit_set(0x0d,7);	// error
			tiemu_error(err, NULL);
        }

		io_bit_set(0x0d,5);		// rx buf full
		io_bit_set(0x0d,2);		// link activity
		lp_avail_byte = 1;
    }
	else
		lp_avail_byte = 0;
  
	return lp_avail_byte;
}


/*
	Link file access
*/

static int sip = 0;	// sending in progress
 
int t2f_data;   // ti => file data
int t2f_flag;   // data available

int f2t_data;   // file => ti data
int f2t_flag;   // data available

void df_putbyte(uint8_t arg)
{
	t2f_data = arg;
	t2f_flag = 1;

	if(sip == 0)
	{
		printf("receiving %02x !\n", arg);
		io_bit_set(0x0d,7);	// SLE=1
		t2f_flag = f2t_flag = 0;
	}
}

uint8_t df_getbyte(void)
{
	f2t_flag = 0;
    return f2t_data;
}

int df_byteavail(void)
{
    return f2t_flag;
}

int df_checkread(void)
{
    return f2t_flag;
}


/*
    Internal link port emulation for direct sending/receiving files.
    The code below is not very obvious at first glance. This is the
    reason why I am explaining the idea and mechanisms.

    The idea is to use TiLP's libticalcs since libticalcs contains all the code
    needed for communicating with a TI (software protocol, file handling). 
    The libticalcs is built on libticables, a library which handles link cables
    (hardware protocol).
    In fact, I simply reimplement the basic functions of libticables usually 
    used by libticalcs for sending/receiving data. These functions exchange
    bytes with the linkport at HW level (ports.c).
    The libticalcs provides the abstraction we need for this.

	Wonderful, isn't it ?! Take a look at the 'TiLP framework' power ;-)
*/

#ifdef __LINUX__
static TicableLinkCable 	ilc = { 0 };
static TicalcFncts			itc = { 0 };
static TicalcInfoUpdate 	iu = { 0 };
static TicableDataRate*     tdr;
#else
TicableLinkCable	ilc = { 0 };
TicalcFncts			itc = { 0 };
TicalcInfoUpdate 	iu = { 0 };
TicableDataRate*	tdr = NULL;
#endif


/* libticables functions (link API) */
static int ilp_init(void)     
{ 
	ticable_get_datarate(&tdr);

	tdr->count = 0;
  	toSTART(tdr->start);

	return 0; 
}

static int ilp_open(void)     
{
	return 0; 
}

static int ilp_put(uint8_t data)
{ 
	tiTIME clk;

	tdr->count++;

  	f2t_data = data; 
  	f2t_flag = 1;

	io_bit_set(0x0d,5);	// SRX=1 (rx reg is full)
	io_bit_set(0x0d,2);	// link activity
	hw_m68k_irq(4);		// this turbo-boost transfer !

	toSTART(clk);
  	while(f2t_flag/* && !iu.cancel*/) 
    { 
      	ti68k_debug_do_instructions(1); 
		if(toELAPSED(clk, 15))	// 2s
			return ERR_WRITE_TIMEOUT;
    };

  	return 0;
}

static int ilp_get(uint8_t *data)
{ 
	tiTIME clk;

	toSTART(clk);
  	while(!t2f_flag/* && !iu.cancel*/) 
    { 
      	ti68k_debug_do_instructions(1);
		if(toELAPSED(clk, 15))
			return ERR_WRITE_TIMEOUT;
    };
    
  	*data = t2f_data;
  	t2f_flag = 0;

	io_bit_set(0x0d,6);	// STX=1 (tx reg is empty)
	hw_m68k_irq(4);		// this turbo-boost transfer !

	tdr->count++;

	return 0;
}

static int ilp_probe(void)    	{ return 0; }
static int ilp_close(void)    	{ return 0; }
static int ilp_term(void)     	{ return 0; }
static int ilp_check(int *st)	{ return 0; }

/* libticalcs functions (GUI callbacks API) */
static void ilp_start(void)   { }
static void ilp_stop(void)    { }
static void ilp_refresh(void) { }
static void ilp_pbar(void)    { }
static void ilp_label(void)   { }

/* Initialize a pseudo link cable to be connected with HW */
static int init_link_file(void)
{
  	ilc.init  = ilp_init;
  	ilc.open  = ilp_open;
  	ilc.put   = ilp_put;
  	ilc.get   = ilp_get;
  	ilc.close = ilp_close;
  	ilc.exit  = ilp_term;
  	ilc.probe = ilp_probe;
  	ilc.check = ilp_check;

  	ticalc_set_cable(&ilc);

  	switch(tihw.calc_type)
    {
    	case TI92: ticalc_set_calc(CALC_TI92, &itc);
      	break;
    	case TI89: ticalc_set_calc(CALC_TI89, &itc);
      	break;
		case TI89t: ticalc_set_calc(CALC_TI89T, &itc);
		break;
    	case TI92p: ticalc_set_calc(CALC_TI92P, &itc);
      	break;
		case V200: ticalc_set_calc(CALC_V200, &itc);
      	break;
    	default: return ERR_NONE;
      	break;
    }

  	//ticalc_set_update(&iu, ilp_start, ilp_stop, ilp_refresh, ilp_pbar, ilp_label);

    t2f_flag = f2t_flag = 0;

	ilc.init();

  	return 0;
}

static int exit_link_file(void)
{
	ilc.exit();
	t2f_flag = f2t_flag = 0;

    return 0;
}

static int test_sendfile(void)
{
    map_dbus_to_file();
    itc.send_var("C:\\str.9xs", 0, NULL);
    map_dbus_to_cable();

    return 0;
}

int send_ti_file(const char *filename)
{
    gint ok = 0;
	int ret;
	clock_t start, finish;
	double duration;

    // Check for TI file
    if(!tifiles_is_a_ti_file(filename))
        return ERR_NOT_TI_FILE;

	if(((tifiles_which_calc_type(filename) == CALC_TI92) && (tihw.calc_type == TI92)) ||
		(tifiles_which_calc_type(filename) == CALC_TI89)  ||
		(tifiles_which_calc_type(filename) == CALC_TI89T) ||
		(tifiles_which_calc_type(filename) == CALC_TI92P) ||
		(tifiles_which_calc_type(filename) == CALC_V200)
	  )
    {
        ok = 1;
    } 
	else
        return ERR_NOT_TI_FILE;

	// Use direct file loading
	start = clock();
	sip = 1;

    // FLASH APP file ?
    if(tifiles_is_a_flash_file(filename) && !strcasecmp(tifiles_flash_app_file_ext(), tifiles_get_extension(filename)))
    {
        
        ret = itc.send_flash(filename, MODE_APPS);
    }

    // FLASH OS file ?
    if(tifiles_is_a_flash_file(filename) && !strcasecmp(tifiles_flash_os_file_ext(), tifiles_get_extension(filename)))
    {
        ret = itc.send_flash(filename, MODE_AMS);
    }
  
    // Backup file ?
    else if(tifiles_is_a_backup_file(filename))
    {
        ret = itc.send_backup(filename, MODE_NORMAL);
    }

    // Group file ?
    else if(tifiles_is_a_group_file(filename))
    {
        ret = itc.send_var(filename, MODE_NORMAL, NULL);
    }

    // Single file
    else if(tifiles_is_a_single_file(filename))
    {
        ret = itc.send_var(filename, MODE_NORMAL, NULL);
    }

	// Restore link cable use
	sip = 0;
	finish = clock();
	duration = (double)(finish - start) / CLOCKS_PER_SEC;
	//printf("Duration: %2.1f seconds.\n", duration);

	// Transfer aborted ? Set hw link error
	if(ret != 0)
	{
		tiemu_error(ret, NULL);
		io_bit_set(0x0d,7);	// SLE=1
		t2f_flag = f2t_flag = 0;
	}

  return 0;
}
