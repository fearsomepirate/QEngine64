#ifndef SYS_H_
#define SYS_H_
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
// sys.h -- non-portable functions

//
// file IO
//

// returns the file size
// return -1 if file is not present
// the file should be in BINARY mode for stupid OSs that care
extern int Sys_FileOpenRead (char *path, int *hndl);

extern int Sys_FileOpenWrite (char *path);
extern void Sys_FileClose (int handle);
extern void Sys_FileSeek (int handle, int position);
extern int Sys_FileRead (int handle, void *dest, int count);
extern int Sys_FileWrite (int handle, void *data, int count);
extern int	Sys_FileTime (char *path);
extern void Sys_mkdir (char *path);

//
// memory protection
//
extern void Sys_MakeCodeWriteable (unsigned long startaddr, unsigned long length);

//
// system IO
//
extern void Sys_DebugLog(char *file, char *fmt, ...);

extern void Sys_Error (char *error, ...);
// an error will cause the entire program to exit

extern void Sys_Printf (char *fmt, ...);
// send text to the console

extern void Sys_Quit (void);

extern double Sys_FloatTime (void);

extern char *Sys_ConsoleInput (void);

extern void Sys_Sleep (void);
// called to yield for a little bit so as
// not to hog cpu when paused or debugging

extern void Sys_SendKeyEvents (void);
// Perform Key_Event () callbacks until the input que is empty

extern void Sys_LowFPPrecision (void);
extern void Sys_HighFPPrecision (void);
extern void Sys_SetFPCW (void);

#endif //SYS_H_