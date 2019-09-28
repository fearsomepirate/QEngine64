#ifndef N64_GFX_H_
#define N64_GFX_H_

#include <nusys.h>

#define SCREEN_HT        240
#define SCREEN_WD        320
#define GFX_GLIST_LEN     2048

typedef struct {
   Mtx     projection;
   u16     perspNorm;
} ScreenSpace;

typedef struct
{
   Gfx          gList[2][GFX_GLIST_LEN];
   ScreenSpace scr;
   Gfx* gListp;
   u32          taskId; //0 or 1
} DisplayList;

extern DisplayList gfx;
extern Gfx setup_rdpstate[];
extern Gfx setup_rspstate[];

extern void SysGfx_RCPInit(void);
extern void SysGfx_ClearCfb(void);
extern void SysGfx_InitFrame(void);
extern void SysGfx_FinishFrame(void);

#endif //N64_GFX_H_