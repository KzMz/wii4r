#include "wii4r.h"

VALUE wii_mod =   Qnil;
VALUE cm_class =  Qnil;
VALUE wii_class = Qnil;

extern void init_wiimote(void);
extern void init_wiimotemanager(void);

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
  
  //Wiimote nunchul button consts
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
  rb_define_const(wii_mod, "E_NONE", INT2NUM(EXP_NONE));
  rb_define_const(wii_mod, "E_NUNCHUK", INT2NUM(EXP_NUNCHUK));
  rb_define_const(wii_mod, "E_CLASSIC", INT2NUM(EXP_CLASSIC));
  rb_define_const(wii_mod, "E_GUITAR", INT2NUM(EXP_GUITAR_HERO_3));
  
  //Aspect ratio and sensor bar position constants
  rb_define_const(wii_mod, "ASPECT_4_3", INT2NUM(WIIUSE_ASPECT_4_3));
  rb_define_const(wii_mod, "ASPECT_16_9", INT2NUM(WIIUSE_ASPECT_16_9));
  rb_define_const(wii_mod, "ABOVE", INT2NUM(WIIUSE_IR_ABOVE));
  rb_define_const(wii_mod, "BELOW", INT2NUM(WIIUSE_IR_BELOW));

  init_wiimotemanager();
  init_wiimote();
}
