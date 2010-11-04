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

#ifndef _INTERUPT_H
#define _INTERUPT_H

#include "global.h"

#define HBLANK_PER 0
#define VBLANK_PER 1
#define OAM_PER 2
#define VRAM_PER 3
#define END_VBLANK_PER 4
#define LINE_90_BEGIN 5
#define LINE_90_END 6
#define BEGIN_OAM_PER 7

#define NO_INT 0
#define VBLANK_INT 0x01
#define LCDC_INT 0x02
#define TIMEOWFL_INT 0x04
#define SERIAL_INT 0x08

extern UINT32 nb_cycle;

typedef struct {
  INT16 cycle;
  UINT8 mode;
  UINT8 nb_spr;

  UINT32 vblank_cycle;

  UINT16 mode1cycle;
  UINT16 mode2cycle;
  UINT8 inc_line;
  UINT8 timing;
}GBLCDC;

GBLCDC *gblcdc;

typedef struct {

  UINT16 clk_inc;
  UINT32 cycle;

}GBTIMER;

GBTIMER *gbtimer;

UINT8 skip_next_frame;

void gblcdc_init(void);
void gblcdc_reset(void);

void gbtimer_init(void);
void gbtimer_reset(void);

void go2double_speed(void);
void go2simple_speed(void);
UINT32 get_nb_cycle(void);

void set_interrupt(UINT8 n);
void unset_interrupt(UINT8 n);
UINT8 make_interrupt(UINT8 n);
UINT8 request_interrupt(UINT8 n);

void gblcdc_set_on(void);
void gblcdc_addcycle(INT32 c);
UINT16 gblcdc_update(void);
void gbtimer_update(void);
void halt_update(void); 

#endif




