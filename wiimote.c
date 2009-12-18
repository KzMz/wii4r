#include "wii4r.h"

void free_wiimote(void * wm) {
  wiiuse_disconnect((wiimote *) wm);
}

static VALUE rb_wm_new(VALUE self) {
  wiimote *wm;
  VALUE obj = Data_Make_Struct(self, wiimote, NULL, free_wiimote, wm);
  rb_obj_call_init(obj, 0, 0);
  return obj;
}

static VALUE rb_wm_init(VALUE self) {
  rb_iv_set(self, "@rumble", Qfalse);
  wiimote *wm;
  Data_Get_Struct(self, wiimote, wm);
  if(!WIIUSE_USING_ACC(wm))
    rb_iv_set(self, "@motion_sensing", Qfalse);
  else 
    rb_iv_set(self, "@motion_sensing", Qtrue);
  if(!WIIUSE_USING_IR(wm)) 
    rb_iv_set(self, "@ir", Qfalse);
  else
    rb_iv_set(self, "@ir", Qtrue);
  if(!WIIUSE_USING_SPEAKER(wm))
    rb_iv_set(self, "@speaker", Qfalse);
  else
    rb_iv_set(self, "@speaker", Qtrue);
  return self;
}

static VALUE rb_wm_get_rumble(VALUE self) {
  VALUE rumble = rb_iv_get(self, "@rumble");
  return rumble;
}

static VALUE rb_wm_set_rumble(VALUE self, VALUE arg) {
  wiimote * wm;
  Data_Get_Struct(self, wiimote, wm);
  int rumble;
  switch(arg) {
    case Qfalse:
      rumble = 0;
      break;
    case Qtrue:
      rumble = 1;
      break;
    default:
      return Qnil;
  }
  wiiuse_rumble(wm, rumble);
  rb_iv_set(self, "@rumble", arg);
  return arg;
}

static VALUE rb_wm_rumble(int argc, VALUE * argv, VALUE self) {
  wiimote * wm;
  Data_Get_Struct(self, wiimote, wm);
  wiiuse_rumble(wm, 1);
  rb_iv_set(self, "@rumble", Qtrue);
  if(argc == 1) {
    switch(TYPE(argv[0])) {
      case T_FIXNUM:
        //fork o thread?
        sleep(NUM2INT(argv[0]));
        wiiuse_rumble(wm, 0);
        rb_iv_set(self, "@rumble", Qfalse);
        break;
      case T_ARRAY:
        break;
      default:
        rb_raise(rb_eTypeError, "Invalid Argument");
    }
  }
  else if(argc == 2) {
    if(TYPE(argv[0]) == T_FIXNUM && TYPE(argv[1]) == T_FIXNUM) {
      int i = NUM2INT(argv[1]);
      int stime = NUM2INT(argv[0]);
      while(i > 1) {
        sleep(stime);
        wiiuse_rumble(wm, 0);
        sleep(1);
        wiiuse_rumble(wm, 1);
        i--;
      }
      sleep(stime);
      wiiuse_rumble(wm, 0);
      rb_iv_set(self, "@rumble", Qfalse);
    }
  }
  else {
    //raise
  }
  return Qnil;
}

static VALUE rb_wm_stop(VALUE self) {
  wiimote * wm;
  Data_Get_Struct(self, wiimote, wm);
  wiiuse_rumble(wm, 0);
  rb_iv_set(self, "@rumble", Qfalse);
  return Qnil;
}

static VALUE rb_wm_leds(VALUE self, VALUE arg) {
  wiimote * wm;
  Data_Get_Struct(self, wiimote, wm);
  wiiuse_set_leds(wm, NUM2INT(arg));
  return Qnil;
}

static VALUE rb_wm_turnoff(VALUE self) {
  wiimote * wm;
  Data_Get_Struct(self, wiimote, wm);
  wiiuse_set_leds(wm, WIIMOTE_LED_NONE);
  return Qnil;
}

static VALUE rb_wm_get_ms(VALUE self) {
  VALUE ms = rb_iv_get(self, "@motion_sensing");
  return ms;
}

static VALUE rb_wm_set_ms(VALUE self, VALUE arg) {
  rb_iv_set(self, "@motion_sensing", arg);
  int motion_sensing;
  switch(arg) {
    case Qfalse:
      motion_sensing = 0;
      break;
    case Qtrue:
      motion_sensing = 1;
      break;
    default:
      return Qnil;
  }
  wiimote * wm;
  Data_Get_Struct(self, wiimote, wm);
  wiiuse_motion_sensing(wm, motion_sensing);
  return arg;
}

static VALUE rb_wm_status(VALUE self) {
  wiimote * wm;
  Data_Get_Struct(self, wiimote, wm);
  wiiuse_status(wm);
  VALUE status_hash = rb_hash_new();
  VALUE speaker = Qnil;
  VALUE led = Qnil;
  VALUE ir = Qnil;
  VALUE expansion = Qnil;
  
	if(WIIUSE_USING_SPEAKER(wm)) 
		speaker = Qtrue;
	else
		speaker = Qfalse;
  
        if(WIIUSE_USING_IR(wm))
              	ir = Qtrue;
        else
               	ir = Qfalse;
  	
  	switch(wm->exp.type){
		case EXP_NUNCHUK:		
			expansion = rb_str_new2("Nunchuck");
		case EXP_CLASSIC:	
			expansion = rb_str_new2("Classic Controller"); 
		case EXP_GUITAR_HERO_3:	
			expansion = rb_str_new2("Guitar Hero Controller");
		case EXP_NONE:	
			expansion = rb_str_new2("Nothing Inserted");
        }
		
  
  if(WIIUSE_IS_LED_SET(wm, 1)) led = rb_str_new2("LED_1");
  else if(WIIUSE_IS_LED_SET(wm, 2)) led = rb_str_new2("LED_2");
  else if(WIIUSE_IS_LED_SET(wm, 3)) led = rb_str_new2("LED_3");
  else if(WIIUSE_IS_LED_SET(wm, 4)) led = rb_str_new2("LED_4");
  else led = rb_str_new2("NONE");
  
  rb_hash_aset(status_hash,ID2SYM(rb_intern("id")),INT2NUM(wm->unid));
  rb_hash_aset(status_hash,ID2SYM(rb_intern("battery")),rb_float_new(wm->battery_level));
  rb_hash_aset(status_hash,ID2SYM(rb_intern("speaker")),speaker);
  rb_hash_aset(status_hash,ID2SYM(rb_intern("ir")),ir);
  rb_hash_aset(status_hash,ID2SYM(rb_intern("led")), led);
  rb_hash_aset(status_hash,ID2SYM(rb_intern("attachment")),expansion);
  return status_hash;
}

int wm_connected(wiimote *wm) {
  if(!wm) return 0;
  else return WIIMOTE_IS_CONNECTED(wm);
}

static VALUE rb_wm_connected(VALUE self) {
  wiimote * wm;
  Data_Get_Struct(self, wiimote, wm);
  int connected = wm_connected(wm);
  if(connected == 0) return Qfalse;
  else return Qtrue;
}

static VALUE rb_wm_pressed(VALUE self, VALUE arg) {
  wiimote *wm;
  Data_Get_Struct(self, wiimote, wm);
  int pressed = IS_PRESSED(wm, NUM2INT(arg));
  if(pressed == 0) return Qfalse;
  else return Qtrue;
}

static VALUE rb_wm_just_pressed(VALUE self, VALUE arg) {
  wiimote *wm;
  Data_Get_Struct(self, wiimote, wm);
  int pressed = IS_JUST_PRESSED(wm, NUM2INT(arg));
  if(pressed == 0) return Qfalse;
  else return Qtrue;
}

static VALUE rb_wm_held(VALUE self, VALUE arg) {
  wiimote *wm;
  Data_Get_Struct(self, wiimote, wm);
  int held = IS_HELD(wm, NUM2INT(arg));
  if(held == 0) return Qfalse;
  else return Qtrue;
}

static VALUE rb_wm_released(VALUE self, VALUE arg) {
  wiimote *wm;
  Data_Get_Struct(self, wiimote, wm);
  int released = IS_RELEASED(wm, NUM2INT(arg));
  if(released == 0) return Qfalse;
  else return Qtrue;
}

static VALUE rb_wm_yaw(VALUE self) {
  wiimote *wm;
  Data_Get_Struct(self, wiimote, wm);
  if(!WIIUSE_USING_ACC(wm)) return Qnil;
  return rb_float_new(wm->orient.yaw);
}

static VALUE rb_wm_pitch(VALUE self) {
  wiimote *wm;
  Data_Get_Struct(self, wiimote, wm);
  if(!WIIUSE_USING_ACC(wm)) return Qnil;
  return rb_float_new(wm->orient.pitch);
}

static VALUE rb_wm_apitch(VALUE self) {
  wiimote *wm;
  Data_Get_Struct(self, wiimote, wm);
  if(!WIIUSE_USING_ACC(wm)) return Qnil;
  return rb_float_new(wm->orient.a_pitch);
}

static VALUE rb_wm_roll(VALUE self) {
  wiimote *wm;
  Data_Get_Struct(self, wiimote, wm);
  if(!WIIUSE_USING_ACC(wm)) return Qnil;
  return rb_float_new(wm->orient.roll);
}

static VALUE rb_wm_aroll(VALUE self) {
  wiimote *wm;
  Data_Get_Struct(self, wiimote, wm);
  if(!WIIUSE_USING_ACC(wm)) return Qnil;
  return rb_float_new(wm->orient.a_roll);
}

static VALUE rb_wm_ir(VALUE self) {
  return rb_iv_get(self, "@ir");
}

static VALUE rb_wm_set_ir(VALUE self, VALUE arg) {
  wiimote *wm;
  Data_Get_Struct(self, wiimote, wm);
  int ir;
  if(arg == Qtrue) ir = 1;
  else if(arg == Qfalse) ir = 0;
  else return Qnil;

  if(WIIUSE_USING_IR(wm) && ir == 0) wiiuse_set_ir(wm, ir);
  else if(ir == 1) wiiuse_set_ir(wm, ir);
  
  if(WIIUSE_USING_IR(wm)) rb_iv_set(self, "@ir", Qtrue);
  else rb_iv_set(self, "@ir", Qfalse);
  return arg;
}

static VALUE rb_wm_ir_sources(VALUE self) {
  wiimote *wm;
  Data_Get_Struct(self, wiimote, wm);
  if(!WIIUSE_USING_IR(wm)) return Qnil;
  VALUE ary = rb_ary_new();
  int i = 0;
  VALUE source;
  for(; i < 4; i++) {
    if(wm->ir.dot[i].visible) {
      source = rb_ary_new();
      rb_ary_push(source, INT2NUM(wm->ir.dot[i].x));
      rb_ary_push(source, INT2NUM(wm->ir.dot[i].y));
      rb_ary_push(ary, source);
    }
  }
  return ary;
}

static VALUE rb_wm_ir_cursor(VALUE self) {
  wiimote *wm;
  Data_Get_Struct(self, wiimote, wm);
  if(!WIIUSE_USING_IR(wm)) return Qnil;
  VALUE ary = rb_ary_new();
  rb_ary_push(ary, INT2NUM(wm->ir.x));
  rb_ary_push(ary, INT2NUM(wm->ir.y));
  return ary;
}

static VALUE rb_wm_ir_z(VALUE self) {
  wiimote *wm;
  Data_Get_Struct(self, wiimote, wm);
  if(!WIIUSE_USING_IR(wm)) return Qnil;
  return rb_float_new(wm->ir.z);
}

static VALUE rb_wm_sensitivity(VALUE self) {
  wiimote *wm;
  int level;
  Data_Get_Struct(self, wiimote, wm);	
  if(!WIIUSE_USING_IR(wm)) return Qnil;
  WIIUSE_GET_IR_SENSITIVITY(wm, &level);	
  return INT2NUM(level);
}

static VALUE rb_wm_set_sens(VALUE self, VALUE arg) {
  wiimote *wm;
  Data_Get_Struct(self,wiimote,wm);
  wiiuse_set_ir_sensitivity(wm,NUM2INT(arg));
  return Qnil;
}

/*
static VALUE rb_wm_set_speaker(VALUE self) {

}
*/

static VALUE rb_wm_speaker(VALUE self) {
  wiimote *wm;
  Data_Get_Struct(self, wiimote, wm);
  if (WIIUSE_USING_SPEAKER(wm)) return Qtrue;
  else return Qfalse;	
}

static VALUE rb_wm_bl(VALUE self) {
  wiimote *wm;	
  Data_Get_Struct(self,wiimote,wm);
  wiiuse_status(wm);
  return rb_float_new(wm->battery_level); 
}

static VALUE rb_wm_aratio(VALUE self) {
  wiimote *wm;
  Data_Get_Struct(self,wiimote,wm);
  wiiuse_status(wm);
  if (wm->ir.aspect == WIIUSE_ASPECT_4_3) return rb_str_new2("4:3"); 
  else if (wm->ir.aspect == WIIUSE_ASPECT_16_9) return rb_str_new2("16:9");
  return Qnil;
}

static VALUE rb_wm_set_aratio(VALUE self, VALUE arg) {
  wiimote *wm;
  Data_Get_Struct(self,wiimote,wm);
  wiiuse_set_aspect_ratio(wm,NUM2INT(arg));
  return Qnil;
}

static VALUE rb_wm_vres(VALUE self) {
  wiimote *wm;
  Data_Get_Struct(self,wiimote,wm);	
  VALUE v_res = rb_ary_new();
  rb_ary_push(v_res, INT2NUM(wm->ir.vres[0]));
  rb_ary_push(v_res, INT2NUM(wm->ir.vres[1]));
  return v_res;
}

static VALUE rb_wm_set_vres(VALUE self, VALUE arg) {
  wiimote *wm;
  Data_Get_Struct(self,wiimote,wm);
  VALUE v_ary[2],argv[1];
  int i = 0;
  for(;i < 2;i++)
  {
	argv[0] = INT2NUM(i);
	v_ary[i] = rb_ary_aref(1, argv, arg);
  }
  wiiuse_set_ir_vres(wm, NUM2INT(v_ary[0]), NUM2INT(v_ary[1]));
  return Qnil;	
}

static VALUE rb_wm_pos(VALUE self) {
  wiimote *wm;
  Data_Get_Struct(self,wiimote,wm);
  if(wm->ir.pos == WIIUSE_IR_ABOVE) return rb_str_new2("ABOVE"); 		
  else if (wm->ir.pos == WIIUSE_IR_BELOW) return rb_str_new2("BELOW");	
  return Qnil;
}

static VALUE rb_wm_set_pos(VALUE self, VALUE arg) {
  wiimote *wm;
  Data_Get_Struct(self,wiimote,wm);	
  wiiuse_set_ir_position(wm,NUM2INT(arg));
  return Qnil;
}

static VALUE rb_wm_led(VALUE self) {
  wiimote *wm;
  Data_Get_Struct(self, wiimote, wm);
  if(WIIUSE_IS_LED_SET(wm, 1)) return rb_str_new2("LED_1");
  if(WIIUSE_IS_LED_SET(wm, 2)) return rb_str_new2("LED_2");
  if(WIIUSE_IS_LED_SET(wm, 3)) return rb_str_new2("LED_3");
  if(WIIUSE_IS_LED_SET(wm, 4)) return rb_str_new2("LED_4");
  return Qnil;
}

static VALUE rb_wm_exp(int argc, VALUE * argv, VALUE self) {
  wiimote *wm;
  Data_Get_Struct(self, wiimote, wm);
  if(argc == 0) {
    if(wm->exp.type != EXP_NONE) {
      return Qtrue;
    }
    else {
      return Qfalse;
    }
  }
  else {
    switch(wm->exp.type) {
      case EXP_NONE:
        return Qfalse;
      case EXP_NUNCHUK:
        if(NUM2INT(argv[0]) == EXP_NUNCHUK) return Qtrue;
        else return Qfalse;
      case EXP_CLASSIC:
        if(NUM2INT(argv[0]) == EXP_CLASSIC) return Qtrue;
        else return Qfalse;
      case EXP_GUITAR_HERO_3:
        if(NUM2INT(argv[0]) == EXP_GUITAR_HERO_3) return Qtrue;
        else return Qfalse;
    }
  }
return Qfalse;
}

static VALUE rb_wm_nunchuk(VALUE self) {
  wiimote *wm;
  Data_Get_Struct(self, wiimote, wm);
  if(wm->exp.type == EXP_NUNCHUK) return Qtrue;
  else return Qfalse;
}

static VALUE rb_wm_cc(VALUE self) {
  wiimote *wm;
  Data_Get_Struct(self, wiimote, wm);
  if(wm->exp.type == EXP_CLASSIC) return Qtrue;
  else return Qfalse;
}

static VALUE rb_wm_gh(VALUE self) {
  wiimote *wm;
  Data_Get_Struct(self, wiimote, wm);
  if(wm->exp.type == EXP_GUITAR_HERO_3) return Qtrue;
  else return Qfalse;
}

static VALUE rb_wm_ir_acursor(VALUE self) {
  wiimote *wm;
  Data_Get_Struct(self, wiimote, wm);
  VALUE ary = rb_ary_new();
  rb_ary_push(ary, INT2NUM(wm->ir.ax));
  rb_ary_push(ary, INT2NUM(wm->ir.ay));
  return ary;
}

static VALUE rb_wm_accel(VALUE self) {
  wiimote *wm;
  Data_Get_Struct(self, wiimote, wm);
  VALUE ary = rb_ary_new();
  rb_ary_push(ary, INT2NUM(wm->accel.x));
  rb_ary_push(ary, INT2NUM(wm->accel.y));
  rb_ary_push(ary, INT2NUM(wm->accel.z));
  return ary;  
}

static VALUE rb_wm_gforce(VALUE self) {
  wiimote *wm;
  Data_Get_Struct(self, wiimote, wm);
  VALUE ary = rb_ary_new();
  rb_ary_push(ary, rb_float_new(wm->gforce.x));
  rb_ary_push(ary, rb_float_new(wm->gforce.y));
  rb_ary_push(ary, rb_float_new(wm->gforce.z));
  return ary;
}

static VALUE rb_wm_accel_threshold(VALUE self) {
  wiimote *wm;
  Data_Get_Struct(self, wiimote, wm);
  return INT2NUM(wm->accel_threshold);
}

static VALUE rb_wm_set_accel_threshold(VALUE self, VALUE arg) {
  wiimote *wm;
  Data_Get_Struct(self, wiimote, wm);
  wiiuse_set_accel_threshold(wm, NUM2INT(arg));
  return Qnil;
}

static VALUE rb_wm_orient_threshold(VALUE self) {
  wiimote *wm;
  Data_Get_Struct(self, wiimote, wm);	
  return INT2NUM(wm->orient_threshold);	
} 

static VALUE rb_wm_set_orient_threshold(VALUE self, VALUE arg) {
  wiimote *wm;
  Data_Get_Struct(self, wiimote, wm);
  wiiuse_set_orient_threshold(wm,rb_float_new(arg));
  return Qnil;
}

void init_wiimote(void) {
	
  wii_class = rb_define_class_under(wii_mod, "Wiimote", rb_cObject);
  rb_define_singleton_method(wii_class, "new", rb_wm_new, 0);
  rb_define_method(wii_class, "initialize", rb_wm_init, 0);
  rb_define_method(wii_class, "rumble?", rb_wm_get_rumble, 0);
  rb_define_method(wii_class, "rumble=", rb_wm_set_rumble, 1);
  rb_define_method(wii_class, "rumble!", rb_wm_rumble, -1);
  rb_define_method(wii_class, "stop!", rb_wm_stop, 0);
  rb_define_method(wii_class, "leds=", rb_wm_leds, 1);
  rb_define_method(wii_class, "led", rb_wm_led, 0);
  rb_define_method(wii_class, "turn_off_leds!", rb_wm_turnoff, 0);
  rb_define_method(wii_class, "motion_sensing?", rb_wm_get_ms, 0);
  rb_define_method(wii_class, "motion_sensing=", rb_wm_set_ms, 1);
  rb_define_method(wii_class, "status", rb_wm_status, 0);
  rb_define_method(wii_class, "connected?", rb_wm_connected, 0);
  rb_define_method(wii_class, "expansion?", rb_wm_exp, -1);
  rb_define_method(wii_class, "has_nunchuk?", rb_wm_nunchuk, 0);
  rb_define_method(wii_class, "has_classic_controller?", rb_wm_cc, 0);
  rb_define_method(wii_class, "has_guitar_hero_controller?", rb_wm_gh, 0);
  rb_define_method(wii_class, "pressed?", rb_wm_pressed, 1);
  rb_define_method(wii_class, "just_pressed?", rb_wm_just_pressed, 1);
  rb_define_method(wii_class, "held?", rb_wm_held, 1);
  rb_define_method(wii_class, "released?", rb_wm_released, 1);
  rb_define_alias(wii_class, "using_accelerometer?", "motion_sensing?");
  rb_define_method(wii_class, "roll", rb_wm_roll, 0);
  rb_define_method(wii_class, "absolute_roll", rb_wm_aroll, 0);
  rb_define_method(wii_class, "pitch", rb_wm_pitch, 0);
  rb_define_method(wii_class, "absolute_pitch", rb_wm_apitch, 0);
  rb_define_method(wii_class, "yaw", rb_wm_yaw, 0);
  rb_define_method(wii_class, "using_ir?", rb_wm_ir, 0);
  rb_define_method(wii_class, "ir_sources", rb_wm_ir_sources, 0);
  rb_define_method(wii_class, "position", rb_wm_ir_cursor, 0);
  rb_define_method(wii_class, "absolute_position", rb_wm_ir_acursor, 0);
  rb_define_method(wii_class, "distance", rb_wm_ir_z, 0);
  rb_define_method(wii_class, "ir=", rb_wm_set_ir, 1);
 // rb_define_method(wii_class, "speaker=", rb_wm_set_speaker, 1);
  rb_define_method(wii_class, "speaker?", rb_wm_speaker, 0);
  rb_define_method(wii_class, "sensitivity", rb_wm_sensitivity, 0);
  rb_define_method(wii_class, "sensitivity=", rb_wm_set_sens, 1);
  rb_define_method(wii_class, "sensor_bar_position", rb_wm_pos, 0);
  rb_define_method(wii_class, "sensor_bar_position=", rb_wm_set_pos, 1);
  rb_define_method(wii_class, "virtual_resolution=", rb_wm_set_vres, 1);
  rb_define_method(wii_class, "virtual_resolution", rb_wm_vres, 0);
  rb_define_method(wii_class, "aspect_ratio=", rb_wm_set_aratio, 1);
  rb_define_method(wii_class, "aspect_ratio", rb_wm_aratio, 0);
  rb_define_method(wii_class, "battery_level", rb_wm_bl, 0);
  rb_define_method(wii_class, "acceleration", rb_wm_accel, 0);
  rb_define_method(wii_class, "gravity_force", rb_wm_gforce, 0);
  rb_define_method(wii_class, "orient_threshold", rb_wm_orient_threshold, 0);
  rb_define_method(wii_class, "orient_threshold=", rb_wm_set_orient_threshold, 1);
  rb_define_method(wii_class, "accel_threshold", rb_wm_accel_threshold, 0);
  rb_define_method(wii_class, "accel_threshold=", rb_wm_set_accel_threshold, 1);
	
}
