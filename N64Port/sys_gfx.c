

#include "sys_gfx.h"

#include <assert.h>

/* Basic graphics initialization & global-type stuff goes here */

DisplayList gfx;

static Vp vp = {
    SCREEN_WD * 2, SCREEN_HT * 2, G_MAXZ / 2, 0,
    SCREEN_WD * 2, SCREEN_HT * 2, G_MAXZ / 2, 0,
};


static char errBufs[8][32] = { "0", "1", "2", "3", "4", "5", "6", "7" };

Gfx setup_rdpstate[] = {
   //gsDPSetRenderMode(G_RM_AA_ZB_XLU_DECAL, G_RM_AA_ZB_XLU_DECAL2),
   gsDPSetRenderMode(G_RM_OPA_SURF, G_RM_OPA_SURF2),
   gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
   gsDPSetScissor(G_SC_NON_INTERLACE, 0,0, SCREEN_WD,SCREEN_HT),
   //gsDPSetColorDither(G_CD_BAYER),
   gsSPEndDisplayList(),
};


Gfx setup_rspstate[] = {
  gsSPViewport(&vp),
  gsSPClearGeometryMode(0xFFFFFFFF),
  gsSPSetGeometryMode(G_ZBUFFER | G_SHADE | G_SHADING_SMOOTH | G_CULL_BACK),
  gsSPTexture(0, 0, 0, 0, G_OFF),
  gsSPEndDisplayList(),
};


void SysGfx_RCPInit(void)
{
   gSPSegment(gfx.gListp++, 0, 0x0);
   gSPDisplayList(gfx.gListp++, OS_K0_TO_PHYSICAL(setup_rspstate));
   gSPDisplayList(gfx.gListp++, OS_K0_TO_PHYSICAL(setup_rdpstate));
}

void SysGfx_ClearCfb(void)
{
   gDPSetDepthImage(gfx.gListp++, OS_K0_TO_PHYSICAL(nuGfxZBuffer));
   gDPSetCycleType(gfx.gListp++, G_CYC_FILL);
   gDPSetColorImage(gfx.gListp++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WD,
      OS_K0_TO_PHYSICAL(nuGfxZBuffer));
   gDPSetFillColor(gfx.gListp++, (GPACK_ZDZ(G_MAXFBZ, 0) << 16 |
      GPACK_ZDZ(G_MAXFBZ, 0)));
   gDPFillRectangle(gfx.gListp++, 0, 0, SCREEN_WD - 1, SCREEN_HT - 1);
   gDPPipeSync(gfx.gListp++);

   gDPSetColorImage(gfx.gListp++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WD,
      osVirtualToPhysical(nuGfxCfb_ptr));
   gDPSetFillColor(gfx.gListp++, (GPACK_RGBA5551(0, 0, 0, 1) << 16 |
      GPACK_RGBA5551(0, 0, 0, 1)));
   gDPFillRectangle(gfx.gListp++, 0, 0, SCREEN_WD - 1, SCREEN_HT - 1);
   gDPPipeSync(gfx.gListp++);
}

void SysGfx_InitFrame(void)
{
   OSTime t;

   static double rot = -90.0F;
   //Transforms* tPtr;

   //rot+=10.0;
   if (rot > 360.0)
      rot -= 360.0;


   //tPtr = &gfx.transforms[gfx.taskId];
   gfx.gListp = &gfx.gList[gfx.taskId][0];

   SysGfx_RCPInit();
   SysGfx_ClearCfb();

   //We're gonna render in 4:3 for now...probably should do 16:9, though.
   //guPerspective(&tPtr->projection, &tPtr->perspNorm, 75, (4.0 / 3.0), 100.0, 5000.0, 1.0);

   //Move & scale the box
   //guTranslate(&(tPtr->m3), 0.0, 0.0, -35.0);
   //guRotate(&(tPtr->m2), rot, 1.0F, 0.0F, 0.0F);
   //guScale(&(tPtr->m1), 10.0, 10.0, 10.0);

   
   
}


void SysGfx_FinishFrame(void)
{

   //Draw the box
   //shadetri(tPtr);
   {
      //u64 us;
      //OSTime s;
      //t = osGetTime();
      //shadeGrunt(tPtr);
      //s = osGetTime();

      /*Some kinda ending code...always end like this, I guess. */
      gDPFullSync(gfx.gListp++);
      gSPEndDisplayList(gfx.gListp++);

      //Check display list bounds.
      assert((gfx.gListp - gfx.gList[gfx.taskId]) < GFX_GLIST_LEN);


      //We've got an array of display lists now. 
      nuGfxTaskStart(&gfx.gList[gfx.taskId][0],
         (s32)(gfx.gListp - gfx.gList[gfx.taskId]) * sizeof(Gfx),
         NU_GFX_UCODE_F3DEX, NU_SC_NOSWAPBUFFER);

      //us = s - t;
      //(void*)& us;

      {
         int i;

         for (i = 0; i < 4; ++i)
         {
            nuDebConTextPos(i, 1, 1 + i);
            nuDebConCPuts(i, errBufs[i]);
         }
      }

      nuDebConDisp(NU_SC_SWAPBUFFER);
   }
   gfx.taskId ^= 1;


   
}