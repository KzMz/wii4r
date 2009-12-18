#ifndef WII4R_H
#define WII4R_H

#ifndef WIN32
	#include<unistd.h>
#endif

#ifndef WIIMOTE_IS_SET
#define WIIMOTE_IS_SET(wm, s)			((wm->state & (s)) == (s))
#endif

#ifndef WIIMOTE_IS_CONNECTED
#define WIIMOTE_IS_CONNECTED(wm)		(WIIMOTE_IS_SET(wm, 0x0008))
#endif


#include<stdio.h>
#include<stdlib.h>
#include<wiiuse.h>
#include<ruby.h>

extern VALUE wii_mod;
extern VALUE cm_class;
extern VALUE wii_class;

extern void free_wiimote(void * wm);
extern int wm_connected(wiimote *wm);


typedef struct _connman {
  wiimote **wms;
  int n;
} connman;

#endif
