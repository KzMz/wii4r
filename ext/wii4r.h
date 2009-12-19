/*
############################################################################
#                                                                          #
#    Copyright (C) 2009 by KzMz KzMz@modusbibendi.org                      #
#    Copyright (C) 2009 by BuZz Gambino.Giorgio@gmail.com                  #
#                                                                          #
#    This program is free software; you can redistribute it and/or modify  #
#    it under the terms of the GNU General Public License as published by  #
#    the Free Software Foundation; either version 2 of the License, or     #
#    (at your option) any later version.                                   #
#                                                                          #
#    This program is distributed in the hope that it will be useful,       #
#    but WITHOUT ANY WARRANTY; without even the implied warranty of        #
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         #
#    GNU General Public License for more details.                          #
#                                                                          #
#    You should have received a copy of the GNU General Public License     #
#    along with this program; if not, write to the                         #
#    Free Software Foundation, Inc.,                                       #
#    59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             #
############################################################################
*/

#ifndef WII4R_H
#define WII4R_H

#include<stdio.h>
#include<stdlib.h>
#include<wiiuse.h>
#include<ruby.h>

#ifndef WIIMOTE_IS_CONNECTED
  #define WIIMOTE_IS_CONNECTED(wm)		(WIIMOTE_IS_SET(wm, 0x0008))
#endif

#ifndef WIIMOTE_IS_SET
  #define WIIMOTE_IS_SET(wm, s)			((wm->state & (s)) == (s))
#endif

//Wii module 
extern VALUE wii_mod;

//WiimoteManager class
extern VALUE cm_class;

//Wiimote class
extern VALUE wii_class;

//disconnect a wiimote when a Wiimote object is marked by the GC and must be deleted
extern void free_wiimote(void * wm);

//return true if the wiimote "wm" is connected, false otherwise
extern int wm_connected(wiimote *wm);

//struct to describe the WiimoteManager class
typedef struct _connman {
  wiimote **wms;		//array of ptrs to wiimote structures
  int n;			//max number of wiimotes connected
} connman;

#endif //WII4R_H
