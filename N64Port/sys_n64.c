/*
Copyright (C) 1996-1997 Id Software, Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#include <nusys.h>
#include <malloc.h>
#include <errno.h>
//#include <signal.h>
#include <stdlib.h>
#include <limits.h>
//#include <sys/types.h>
//#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
//#include <sys/stat.h>
#include <string.h>
#include <conio.h>

#include "quakedef.h"
#include "sys.h"
//#include "dosisms.h"

extern void TrapKey(void);

#define MINIMUM_WIN_MEMORY			0x800000
#define MINIMUM_WIN_MEMORY_LEVELPAK	(MINIMUM_WIN_MEMORY + 0x100000)

int			end_of_memory;
qboolean	lockmem, lockunlockmem, unlockmem;
static int	win95;

#define STDOUT	1

#define	KEYBUF_SIZE	256
static unsigned char	keybuf[KEYBUF_SIZE];
static int				keybuf_head=0;
static int				keybuf_tail=0;

static quakeparms_t	quakeparms;
int					sys_checksum;
static double		curtime = 0.0;
static double		lastcurtime = 0.0;
static double		oldtime = 0.0;

static int			minmem;

float				fptest_temp;

//extern char	start_of_memory __asm__("start");

//=============================================================================

// this is totally dependent on cwsdpmi putting the stack right after tge
// global data



//=============================================================================


void TrapKey(void)
{
#if 0 //Needs to handle an N64 controller
//	static int ctrl=0;
	keybuf[keybuf_head] = dos_inportb(0x60);
	dos_outportb(0x20, 0x20);
	/*
	if (scantokey[keybuf[keybuf_head]&0x7f] == K_CTRL)
		ctrl=keybuf[keybuf_head]&0x80;
	if (ctrl && scantokey[keybuf[keybuf_head]&0x7f] == 'c')
		Sys_Error("ctrl-c hit\n");
	*/
	keybuf_head = (keybuf_head + 1) & (KEYBUF_SIZE-1);
#endif
}


void MaskExceptions (void);
void Sys_InitFloatTime (void);
void Sys_PushFPCW_SetHigh (void);
void Sys_PopFPCW (void);

#define LEAVE_FOR_CACHE (512*1024)		//FIXME: tune
#define LOCKED_FOR_MALLOC (128*1024)	//FIXME: tune


/*
============
Sys_FileTime

returns -1 if not present
============
*/
int	Sys_FileTime (char *path)
{
#if 0
	struct	stat	buf;
	
	if (stat (path,&buf) == -1)
		return -1;
	
	return buf.st_mtime;
#else
   return -1;
#endif
}

void Sys_mkdir (char *path)
{
   //No file I/O!
//	mkdir (path, 0777);
}


void Sys_Sleep(void)
{
}


char* Sys_ConsoleInput(void)
{
   return NULL;
}

double Sys_FloatTime(void)
{
   static int sametimecount = 0; //Use this to force a frame update
   oldtime = curtime;
   curtime = 0;
   lastcurtime = curtime;

   return curtime;
}

void Host_Frame(float time) //just a placeholder
{

}

void HostFrame64(int n)
{
   double newtime = Sys_FloatTime();
   double time = newtime - oldtime;

   Host_Frame(time);

   //Sys_StackCheck ();

   oldtime = newtime;
}


void Sys_Init(void)
{
   oldtime = Sys_FloatTime();

   nuGfxInit();
   nuGfxFuncSet((NUGfxFunc)HostFrame64);
   nuGfxDisplayOn();


#if 0 //Anything relevant here?
	MaskExceptions ();

	Sys_SetFPCW ();

    dos_outportb(0x43, 0x34); // set system timer to mode 2
    dos_outportb(0x40, 0);    // for the Sys_FloatTime() function
    dos_outportb(0x40, 0);

	Sys_InitFloatTime ();

	_go32_interrupt_stack_size = 4 * 1024;;
	_go32_rmcb_stack_size = 4 * 1024;
#endif 
}

void Sys_Shutdown(void)
{
   //Nothing to do here...n64 just shuts down.
}


void Sys_SendKeyEvents (void)
{

#if 0 //more n64 controller stuff
	int k, next;
	int outkey;

// get key events

	while (keybuf_head != keybuf_tail)
	{

		k = keybuf[keybuf_tail++];
		keybuf_tail &= (KEYBUF_SIZE-1);

		if (k==0xe0)
			continue;               // special / pause keys
		next = keybuf[(keybuf_tail-2)&(KEYBUF_SIZE-1)];
		if (next == 0xe1)
			continue;                               // pause key bullshit
		if (k==0xc5 && next == 0x9d) 
		{ 
			Key_Event (K_PAUSE, true);
			continue; 
		} 

		// extended keyboard shift key bullshit 
		if ( (k&0x7f)==SC_LSHIFT || (k&0x7f)==SC_RSHIFT ) 
		{ 
			if ( keybuf[(keybuf_tail-2)&(KEYBUF_SIZE-1)]==0xe0 ) 
				continue; 
			k &= 0x80; 
			k |= SC_RSHIFT; 
		} 

		if (k==0xc5 && keybuf[(keybuf_tail-2)&(KEYBUF_SIZE-1)] == 0x9d)
			continue; // more pause bullshit

		outkey = scantokey[k & 0x7f];

		if (k & 0x80)
			Key_Event (outkey, false);
		else
			Key_Event (outkey, true);

	}
#endif 

}


// =======================================================================
// General routines
// =======================================================================

/*
================
Sys_Printf
================
*/

void Sys_Printf (char *fmt, ...)
{
#if 0 //haha, nope
	va_list		argptr;
	char		text[1024];
	
	va_start (argptr,fmt);
	vsprintf (text,fmt,argptr);
	va_end (argptr);

	if (cls.state == ca_dedicated)
		fprintf(stderr, "%s", text);
#endif
}

void Sys_AtExit (void)
{

// shutdown only once (so Sys_Error can call this function to shutdown, then
// print the error message, then call exit without exit calling this function
// again)
	Sys_Shutdown();
}


void Sys_Quit (void)
{
#if 0 //This might be cool, though useless...print the sell screen :-)
	byte	screen[80*25*2];
	byte	*d;
	char			ver[6];
	int			i;
	

// load the sell screen before shuting everything down
	if (registered.value)
		d = COM_LoadHunkFile ("end2.bin"); 
	else
		d = COM_LoadHunkFile ("end1.bin"); 
	if (d)
		memcpy (screen, d, sizeof(screen));

// write the version number directly to the end screen
	sprintf (ver, " v%4.2f", VERSION);
	for (i=0 ; i<6 ; i++)
		screen[0*80*2 + 72*2 + i*2] = ver[i];

	Host_Shutdown();

// do the text mode sell screen
	if (d)
	{
		memcpy ((void *)real2ptr(0xb8000), screen,80*25*2); 
	
	// set text pos
		regs.x.ax = 0x0200; 
		regs.h.bh = 0; 
		regs.h.dl = 0; 
		regs.h.dh = 22;
		dos_int86 (0x10); 
	}
	else
		printf ("couldn't load endscreen.\n");

	exit(0);
#endif 
}

void Sys_Error (char *error, ...)
{ 
#if 0 //maybe a good idea 
    va_list     argptr;
    char        string[1024];
    
    va_start (argptr,error);
    vsprintf (string,error,argptr);
    va_end (argptr);

	Host_Shutdown();
	fprintf(stderr, "Error: %s\n", string);
// Sys_AtExit is called by exit to shutdown the system
	exit(0);
#endif 
} 


int Sys_FileOpenRead (char *path, int *handle)
{
#if 0 //needs to load a wave

	int	h;
	struct stat	fileinfo;
    
	h = open (path, O_RDONLY|O_BINARY, 0666);
	*handle = h;
	if (h == -1)
		return -1;
	
	if (fstat (h,&fileinfo) == -1)
		Sys_Error ("Error fstating %s", path);

	return fileinfo.st_size;
#endif
}

int Sys_FileOpenWrite (char *path)
{
#if 0 //Use the controller pak
	int     handle;

	umask (0);
	
	handle = open(path,O_RDWR | O_BINARY | O_CREAT | O_TRUNC
	, 0666);

	if (handle == -1)
		Sys_Error ("Error opening %s: %s", path,strerror(errno));

	return handle;
#endif 

}

void Sys_FileClose (int handle)
{
	//close (handle);
}

void Sys_FileSeek (int handle, int position)
{
//	lseek (handle, position, SEEK_SET);
}

int Sys_FileRead (int handle, void *dest, int count)
{
   //return read (handle, dest, count);
   return 0;
}

int Sys_FileWrite (int handle, void *data, int count)
{
	//return write (handle, data, count);
   return 0;
}

/*
================
Sys_MakeCodeWriteable
================
*/
void Sys_MakeCodeWriteable (unsigned long startaddr, unsigned long length)
{
	// it's always writeable
}


/*
================
Sys_FloatTime
================
*/



/*
================
Sys_InitFloatTime
================
*/
void Sys_InitFloatTime (void)
{
   oldtime = 0;
   curtime = 0;
   lastcurtime = 0;

}



char heap[HEAP_SIZE];

/*
================
Sys_GetMemory
================
*/
static void Sys_GetMemory(void)
{	
   InitHeap(heap, HEAP_SIZE);

   quakeparms.memsize = 1024;
   quakeparms.membase = malloc (quakeparms.memsize);	
}


/*
================
main
================
*/
void mainproc(void)
{   
   Sys_GetMemory();


#if 0 //Setup waves
   getwd(cwd);
   if (cwd[Q_strlen(cwd) - 1] == '/') cwd[Q_strlen(cwd) - 1] = 0;
   quakeparms.basedir = cwd; //"f:/quake";
#endif

   Sys_Init();

   //Con_Printf ("Top of stack: 0x%x\n", &time);
   while (1)
   { /* Loop forever */ }   
}


