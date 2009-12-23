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

#include "wii4r.h"

//Wii module
VALUE wii_mod =   Qnil;

//WiimoteManager class
VALUE cm_class =  Qnil;

//Wiimote class
VALUE wii_class = Qnil;

//ClassicController class
VALUE cc_class = Qnil;

//GH3Controller class
VALUE gh3_class = Qnil;

//Nunchuk class
VALUE nun_class = Qnil;

//define Wiimote class
extern void init_wiimote(void);

//define WiimoteManager class
extern void init_wiimotemanager(void);

//define ClassicController class
extern void init_cc(void);

//define GH3Controller class
extern void init_gh3(void);

//define Nunchuk class
extern void init_nunchuk(void);

/*
 *  Document-class: Wii
 *
 *  Module that encapsulates Wiimote and WiimoteManager classes and defines some constants
 *
 *  Led Constants:
 *  	- LED_1
 *  	- LED_2
 *  	- LED_3
 *  	- LED_4
 *	- LED_NONE   
 *
 *  Wiimote Button Constants:
 *	- BUTTON_A
 *	- BUTTON_B
 *	- BUTTON_ONE
 *	- BUTTON_TWO
 *	- BUTTON_PLUS
 *	- BUTTON_MINUS
 *	- BUTTON_HOME
 *      - BUTTON_LEFT
 *	- BUTTON_RIGHT
 *	- BUTTON_UP
 *	- BUTTON_DOWN
 *
 *  Expansion Constants:
 *	- EXP_NONE
 *	- EXP_NUNCHUK
 *	- EXP_CLASSIC
 *	- EXP_GUITAR
 *
 *  Aspect Ratio Constants:
 *	- ASPECT_4_3
 *	- ASPECT_16_9
 *
 *  Sensor Bar Position Constants:
 *	- ABOVE
 *	- BELOW
 */

void Init_wii4r() {
  wii_mod = rb_define_module("Wii");
  rb_define_const(wii_mod, "MAX_WIIMOTES", INT2NUM(4));
  rb_define_const(wii_mod, "TIMEOUT", INT2NUM(5));
  
  //Wiimote led consts
  rb_define_const(wii_mod, "LED_NONE", INT2NUM(WIIMOTE_LED_NONE));
  rb_define_const(wii_mod, "LED_1", INT2NUM(WIIMOTE_LED_1));
  rb_define_const(wii_mod, "LED_2", INT2NUM(WIIMOTE_LED_2));
  rb_define_const(wii_mod, "LED_3", INT2NUM(WIIMOTE_LED_3));
  rb_define_const(wii_mod, "LED_4", INT2NUM(WIIMOTE_LED_4));
  
  //Wiimote button consts
  rb_define_const(wii_mod, "BUTTON_A", INT2NUM(WIIMOTE_BUTTON_A));
  rb_define_const(wii_mod, "BUTTON_B", INT2NUM(WIIMOTE_BUTTON_B));
  rb_define_const(wii_mod, "BUTTON_ONE", INT2NUM(WIIMOTE_BUTTON_ONE));
  rb_define_const(wii_mod, "BUTTON_TWO", INT2NUM(WIIMOTE_BUTTON_TWO));
  rb_define_const(wii_mod, "BUTTON_PLUS", INT2NUM(WIIMOTE_BUTTON_PLUS));
  rb_define_const(wii_mod, "BUTTON_MINUS", INT2NUM(WIIMOTE_BUTTON_MINUS));
  rb_define_const(wii_mod, "BUTTON_HOME", INT2NUM(WIIMOTE_BUTTON_HOME));
  rb_define_const(wii_mod, "BUTTON_LEFT", INT2NUM(WIIMOTE_BUTTON_LEFT));
  rb_define_const(wii_mod, "BUTTON_RIGHT", INT2NUM(WIIMOTE_BUTTON_RIGHT));
  rb_define_const(wii_mod, "BUTTON_DOWN", INT2NUM(WIIMOTE_BUTTON_DOWN));
  rb_define_const(wii_mod, "BUTTON_UP", INT2NUM(WIIMOTE_BUTTON_UP));
  rb_define_const(wii_mod, "BUTTON_ALL", INT2NUM(WIIMOTE_BUTTON_ALL));
  
  //Wiimote nunchuk button consts
  rb_define_const(wii_mod, "N_BUTTON_Z", INT2NUM(NUNCHUK_BUTTON_Z));
  rb_define_const(wii_mod, "N_BUTTON_C", INT2NUM(NUNCHUK_BUTTON_C));
  rb_define_const(wii_mod, "N_BUTTON_ALL", INT2NUM(NUNCHUK_BUTTON_ALL));
  
  //Wiimote classic controller button codes
  rb_define_const(wii_mod, "C_BUTTON_UP", INT2NUM(CLASSIC_CTRL_BUTTON_UP));
  rb_define_const(wii_mod, "C_BUTTON_LEFT", INT2NUM(CLASSIC_CTRL_BUTTON_LEFT));
  rb_define_const(wii_mod, "C_BUTTON_ZR", INT2NUM(CLASSIC_CTRL_BUTTON_ZR));
  rb_define_const(wii_mod, "C_BUTTON_X", INT2NUM(CLASSIC_CTRL_BUTTON_X));
  rb_define_const(wii_mod, "C_BUTTON_A", INT2NUM(CLASSIC_CTRL_BUTTON_A));
  rb_define_const(wii_mod, "C_BUTTON_Y", INT2NUM(CLASSIC_CTRL_BUTTON_Y));
  rb_define_const(wii_mod, "C_BUTTON_B", INT2NUM(CLASSIC_CTRL_BUTTON_B));
  rb_define_const(wii_mod, "C_BUTTON_ZL", INT2NUM(CLASSIC_CTRL_BUTTON_ZL));
  rb_define_const(wii_mod, "C_BUTTON_FULL_R", INT2NUM(CLASSIC_CTRL_BUTTON_FULL_R));
  rb_define_const(wii_mod, "C_BUTTON_PLUS", INT2NUM(CLASSIC_CTRL_BUTTON_PLUS));
  rb_define_const(wii_mod, "C_BUTTON_HOME", INT2NUM(CLASSIC_CTRL_BUTTON_HOME));
  rb_define_const(wii_mod, "C_BUTTON_MINUS", INT2NUM(CLASSIC_CTRL_BUTTON_MINUS));
  rb_define_const(wii_mod, "C_BUTTON_FULL_L", INT2NUM(CLASSIC_CTRL_BUTTON_FULL_L));
  rb_define_const(wii_mod, "C_BUTTON_DOWN", INT2NUM(CLASSIC_CTRL_BUTTON_DOWN));
  rb_define_const(wii_mod, "C_BUTTON_RIGHT", INT2NUM(CLASSIC_CTRL_BUTTON_RIGHT));
  rb_define_const(wii_mod, "C_BUTTON_ALL", INT2NUM(CLASSIC_CTRL_BUTTON_ALL));
  
  //Wiimote hero button codes
  rb_define_const(wii_mod, "GUITAR_BUTTON_UP", INT2NUM(GUITAR_HERO_3_BUTTON_STRUM_UP));
  rb_define_const(wii_mod, "GUITAR_BUTTON_YELLOW", INT2NUM(GUITAR_HERO_3_BUTTON_YELLOW));
  rb_define_const(wii_mod, "GUITAR_BUTTON_GREEN", INT2NUM(GUITAR_HERO_3_BUTTON_GREEN));
  rb_define_const(wii_mod, "GUITAR_BUTTON_BLUE", INT2NUM(GUITAR_HERO_3_BUTTON_BLUE));
  rb_define_const(wii_mod, "GUITAR_BUTTON_RED", INT2NUM(GUITAR_HERO_3_BUTTON_RED));
  rb_define_const(wii_mod, "GUITAR_BUTTON_ORANGE", INT2NUM(GUITAR_HERO_3_BUTTON_ORANGE));
  rb_define_const(wii_mod, "GUITAR_BUTTON_PLUS", INT2NUM(GUITAR_HERO_3_BUTTON_PLUS));
  rb_define_const(wii_mod, "GUITAR_BUTTON_MINUS", INT2NUM(GUITAR_HERO_3_BUTTON_MINUS));
  rb_define_const(wii_mod, "GUITAR_BUTTON_DOWN", INT2NUM(GUITAR_HERO_3_BUTTON_STRUM_DOWN));
  rb_define_const(wii_mod, "GUITAR_BUTTON_ALL", INT2NUM(GUITAR_HERO_3_BUTTON_ALL));
  
  //Wiimote expansion codes 
  rb_define_const(wii_mod, "EXP_NONE", INT2NUM(EXP_NONE));
  rb_define_const(wii_mod, "EXP_NUNCHUK", INT2NUM(EXP_NUNCHUK));
  rb_define_const(wii_mod, "EXP_CLASSIC", INT2NUM(EXP_CLASSIC));
  rb_define_const(wii_mod, "EXP_GUITAR", INT2NUM(EXP_GUITAR_HERO_3));
  
  //Aspect ratio and sensor bar position constants
  rb_define_const(wii_mod, "ASPECT_4_3", INT2NUM(WIIUSE_ASPECT_4_3));
  rb_define_const(wii_mod, "ASPECT_16_9", INT2NUM(WIIUSE_ASPECT_16_9));
  rb_define_const(wii_mod, "ABOVE", INT2NUM(WIIUSE_IR_ABOVE));
  rb_define_const(wii_mod, "BELOW", INT2NUM(WIIUSE_IR_BELOW));

  init_wiimotemanager();
  init_wiimote();
  init_nunchuk();
  init_gh3();
  init_cc();
}

