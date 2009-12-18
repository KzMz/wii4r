#ifndef WII4R_H
#define WII4R_H

#include<stdio.h>
#include<stdlib.h>
#include<wiiuse.h>
#include<ruby.h>

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
