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

#ifndef SGB_H
#define SGB_H

#include <SDL.h>
#include <string.h>
#include "global.h"
#include "vram.h"

#define SGB_WIDTH 256
#define SGB_HEIGHT 224

#define SGB_PACKSIZE 16		/* 128/8=16 */

typedef struct {
  Uint8 on;			/*  on!=0 during a transfert */
  Uint8 cmd;
  Sint8 nb_pack;		        /* nb packet for the cmd */
  Uint8 b;
  Uint8 pack[SGB_PACKSIZE];
  Sint16 b_i;			/* ieme bit du package */
  Uint8 player;
}SGB;

SGB sgb;

Uint16 sgb_pal[4][4];		/* 4 pallete of 4 colour */
Uint8 sgb_pal_map[20][18];      /* Map of Pallete Tiles */

Uint8 sgb_mask;

extern SDL_Surface *sgb_buf;


#define sgb_init_transfer() { \
          sgb.on=1;           \
          sgb.b_i=-1;         \
          memset(sgb.pack,0,SGB_PACKSIZE);}
void sgb_exec_cmd(void);

void sgb_init(void);

#endif
