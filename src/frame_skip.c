/*  gngb, a game boy color emulator
 *  Copyright (C) 2001 Peponas Thomas & Peponas Mathieu
 * 
 *  This program is free software; you can redistribute it and/or modify  
 *  it under the terms of the GNU General Public License as published by   
 *  the Free Software Foundation; either version 2 of the License, or    
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA. 
 */


#include <unistd.h>
#include <sys/time.h>
#include <math.h>
#include <SDL/SDL.h>
#include "frame_skip.h"
#include "message.h"
#include "emu.h"


#ifndef uclock_t
#define uclock_t UINT32
#endif

#define TICKS_PER_SEC 1000000UL
#define CPU_FPS 59.7
#define MAX_FRAME_SKIP 25


static char init_frame_skip=1;
char skip_next_frame=0;
static struct timeval init_tv = {0,0};

void reset_frame_skip(void) {
  init_tv.tv_usec = 0;
  init_tv.tv_sec = 0;
  skip_next_frame=0;
  init_frame_skip=1;
}

uclock_t get_ticks(void) {
  struct timeval tv;

  gettimeofday(&tv, 0);
  if (init_tv.tv_sec == 0) init_tv = tv;
  return (tv.tv_sec - init_tv.tv_sec) * TICKS_PER_SEC + tv.tv_usec - init_tv.tv_usec;
}

int frame_skip(int init) {
  static float frame2skip;
  static uclock_t F=(uclock_t)((double)TICKS_PER_SEC/CPU_FPS);
  static uclock_t t=0,lt=0,sec=0;
  static uclock_t f=(uclock_t)((double)TICKS_PER_SEC/CPU_FPS);
  static int nbFrame=0;
  static int skpFrm=0;
  uclock_t dt;
  
  if (init_frame_skip) {
    init_frame_skip=0;
    //printf("init frame skip\n");
    t=get_ticks();
    frame2skip=0;
    sec=get_ticks();
    nbFrame=0;
    return 0;
  } else {
    lt=t;
    t=get_ticks();
  }

  if (frame2skip>1.0) {
    skpFrm++;
    frame2skip-=1.0;
    return 1;
  } else 
    skpFrm=0;


  dt=t-lt;
 
  /*if (dt>F*12)
    dt=F*12;*/
  
  nbFrame++;
  if ((t-sec)>=TICKS_PER_SEC) {
    //printf("%d\n",nbFrame);
    if (conf.show_fps) set_message("fps:%d",nbFrame);
    nbFrame=0;
    sec=get_ticks();
  }
  

  //printf("%d %d\n",dt,f);
  /*
  if (skpFrm>0) {
    skpFrm--;
    return 1;
    }*/
  
  
  if (dt<f) {
    while(dt<f) {
      // printf("wait\n");
      //SDL_Delay(1);
      //usleep(1);
    //SDL_Delay((f-dt)/1000);

      dt=(t=get_ticks())-lt;
      
      /*dt=elapsed_clock(0)-lt;
	t=elapsed_clock(0);*/
      //printf("%d %d\n",dt,i++);
    }
    
    //f=F-(dt-f);
    return 0;
  } else {
    //printf("skip\n");
    // printf("skip\n");
    //f=F-((dt-f)%F);
    // printf("%d %d\n",F,f);
    //printf("%d %d %f %f\n",dt,f,dt/(float)f,frame2skip);
    frame2skip+=(dt/(float)f)-1;
    if ((int)frame2skip>MAX_FRAME_SKIP)
      frame2skip=MAX_FRAME_SKIP;
    //printf("frame2skip %d:\n",frame2skip);
    return 1;
  }
}





