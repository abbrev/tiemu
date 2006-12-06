/* Hey EMACS -*- linux-c -*- */
/* $Id$ */

/*  TiEmu - Tiemu Is an EMUlator
 *
 *  Copyright (c) 2000-2001, Thomas Corvazier, Romain Lievin
 *  Copyright (c) 2001-2003, Romain Lievin
 *  Copyright (c) 2003, Julien Blache
 *  Copyright (c) 2004, Romain Li�vin
 *  Copyright (c) 2005, Romain Li�vin
 *  Copyright (c) 2006, Kevin Kofler and Romain Li�vin
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "struct.h"
#include "version.h"
#include "ti68k_def.h"
#include "ti68k_int.h"
#include "tie_error.h"

#ifdef __WIN32__
#include "registry.h"
#include "oleaut.h"
#endif

/*
  Display the program version
*/
int tiemu_version(void)
{
	fprintf(stdout, "TiEmu III - Version %s\n", TIEMU_VERSION);
	fprintf(stdout, "  (C) Romain Lievin & Thomas Corvazier  2000-2001\n");
	fprintf(stdout, "  (C) Romain Lievin 2001-2003\n");
	fprintf(stdout, "  (C) Julien Blache 2003\n");
	fprintf(stdout, "  (C) Romain Lievin 2004-2005\n");
	fprintf(stdout, "  (C) Romain Lievin & Kevin Kofler 2005-2006\n");
	fprintf(stdout, "THIS PROGRAM COMES WITH ABSOLUTELY NO WARRANTY\n");
	fprintf(stdout, "PLEASE READ THE DOCUMENTATION FOR DETAILS\n");

	return 0;
}

/*
  Display a short help
*/
int help(void)
{
	fprintf(stdout, "\n");

	tiemu_version();

	fprintf(stdout, "usage: tiemu [-options] [image]\n");
	fprintf(stdout, "\n");
	fprintf(stdout, "-h, --help     display this information page and exit\n");
	fprintf(stdout, "-v, --version  display the version information and exit\n");
	fprintf(stdout, "--import       import ROM or TIB into repository without loading\n");
	fprintf(stdout, "-rom=          ROM dump to convert and load\n");
	fprintf(stdout, "-tib=          TIB or upgrade to convert and load\n");
	fprintf(stdout, "-sav=          state image to load\n");
	fprintf(stdout, "\n");
	fprintf(stdout, "filename       a filename which contains a TiEmu image\n");
	fprintf(stdout, "\n");

	exit(0);
	return 0;
}

#define strexact(p1,p2) (!strcmp((p1),(p2)))

/*
  Scan the command line, extract arguments and init variables
*/
int scan_cmdline(int argc, char **argv)
{
	int cnt;
	char *p;
	char *q;
	char msg[80];
	gchar *dstname;
	int import = 0;

	//for(cnt = 0; cnt < argc; cnt++)
	//	fprintf(stdout, "%i: [%s]\n", cnt, argv[cnt]);
  
	for(cnt=1; cnt<argc; cnt++) 
	{
		p = argv[cnt];

#ifdef __WIN32__
		if(!stricmp(p, "/RegServer") || !stricmp(p, "-RegServer")
		   || !stricmp(p, "--RegServer")) {
			char *p;
			ITypeLib *tlb;
			char szModule[512];
			wchar_t tlbname[512];
			HMODULE hModule = GetModuleHandle(NULL);
			DWORD dwResult = GetModuleFileName(hModule, szModule, sizeof(szModule));

			if (!dwResult) exit(1);
			p = szModule + strlen(szModule) - 4;

			if (stricmp(p,".exe")) exit(1);
			strcpy(++p,"tlb");
			mbstowcs(tlbname, szModule, strlen(szModule)+1);

			if (RegisterServer(&CLSID_TiEmuOLE,
			                   "TiEmu OLE Interface",
			                   "TiEmu.TiEmuOLE",
			                   "TiEmu.TiEmuOLE.1", NULL)
			    || LoadTypeLib(tlbname, &tlb))
				exit(1);
			else {
				if (RegisterTypeLib(tlb, tlbname, NULL)) {
					tlb->lpVtbl->Release(tlb);
					exit(1);
				} else {
					tlb->lpVtbl->Release(tlb);
					fprintf(stdout, "TiEmu OLE Interface successfully registered.");
					exit(0);
				}
			}
		}

		if(!stricmp(p, "/UnregServer") || !stricmp(p, "-UnregServer")
		   || !stricmp(p, "--UnregServer")) {
			if (UnregisterServer(&CLSID_TiEmuOLE, "TiEmu.TiEmuOLE",
			                     "TiEmu.TiEmuOLE.1")
			    || UnRegisterTypeLib(&LIBID_TiEmuOLELib, 1, 0, 0,
			                         SYS_WIN32))
				exit(1);
			else {
				fprintf(stdout, "TiEmu OLE Interface successfully unregistered.");
				exit(0);
			}
		}
		if(stricmp(p, "/Embedding") || stricmp(p, "-Embedding")
		   || stricmp(p, "--Embedding")) {
			// VB runs it with this option.
			continue;
		}
#endif

		if(*p == '-') 
		{
			// a long option (like --help)
			p++;
		} else 
		{
			// an image to load
			g_free(params.rom_file);
			params.rom_file = g_strdup(p);
		}

		strcpy(msg, p);

		if(strexact(msg, "-import")) 
			import = !0;

		if(strstr(msg, "rom=") || strstr(msg, "tib="))
		{	
			q=msg+4;

			if(ti68k_is_a_rom_file(q))
			{
				int err = ti68k_convert_rom_to_image(q, inst_paths.img_dir, &dstname);
				if(err) 
				{
					tiemu_error(err, NULL);
					exit(-1);
				}

				if(import)
					exit(0);

				g_free(params.rom_file);
				params.rom_file = dstname;
				g_free(params.sav_file);
				params.sav_file = g_strdup("");
			}
			else if(ti68k_is_a_tib_file(q))
			{
				int err = ti68k_convert_tib_to_image(q, inst_paths.img_dir, &dstname, -1);
				if(err) 
				{
					tiemu_error(err, NULL);
					exit(-1);
				}

				if(import)
					exit(0);

				g_free(params.rom_file);
				params.rom_file = dstname;
				g_free(params.sav_file);
				params.sav_file = g_strdup("");
			}
			else
				exit(-1);
		}

		if(strstr(msg, "sav=")) 
		{
			q=msg+4;
			g_free(params.sav_file);
			params.sav_file = g_strdup(p);
		}
	      
		if(strexact(msg, "-help") || strexact(msg, "h")) 
			help();

		if(strexact(msg, "-version") || strexact(msg, "v")) 
		{ 
			//version(); 
			exit(0); 
		}
	}

	return 0;
}
