#include<stdio.h>
#include<stdlib.h>

#ifndef WIN32
	#include<unistd.h>
#endif

#include<ruby.h>
#include<wiiuse.h>
#define WIIMOTE_IS_SET(wm, s)			((wm->state & (s)) == (s))
#define WIIMOTE_IS_CONNECTED(wm)		(WIIMOTE_IS_SET(wm, 0x0008))

VALUE wii_mod = Qnil;

typedef struct _connman {
  wiimote **wms;
  int n;
} connman;

void free_wiimote(void * wm) {
  wiiuse_disconnect((wiimote *) wm);
}

static VALUE rb_wm_new(VALUE self) {

  wiimote *wm;
  VALUE obj = Data_Make_Struct(self, wiimote, NULL, free_wiimote, wm); 
  wiiuse_rumble(wm, 0);
  rb_obj_call_init(obj, 0, 0);
  
  return obj;
}

static VALUE rb_wm_init(VALUE self) {
  rb_iv_set(self, "@rumble", Qfalse);
  rb_iv_set(self, "@motion_sensing", Qfalse);
  return self;
}

static VALUE rb_cm_new(VALUE self) {
  connman * conn;

  VALUE m = rb_const_get(wii_mod, rb_intern("MAX_WIIMOTES"));

  int max = NUM2INT(m);

  VALUE obj = Data_Make_Struct(self, connman, NULL, free, conn);

  conn->wms = wiiuse_init(max);
  conn->n = max; 

  rb_obj_call_init(obj, 0, 0);
  
  return obj;
}

static VALUE rb_cm_init(VALUE self) {
  VALUE ary = rb_ary_new();
  rb_iv_set(self, "@wiimotes", ary);
  return self;
}

static VALUE rb_cm_connected(VALUE self) {
  VALUE wm = rb_iv_get(self, "@wiimotes");
  VALUE size = rb_funcall(wm, rb_intern("size"), 0, NULL);
  return size;
}

static VALUE rb_cm_cleanup(VALUE self) {
  connman * conn;
  Data_Get_Struct(self, connman, conn);
  wiiuse_cleanup(conn->wms, conn->n);
  VALUE ary = rb_iv_get(self, "@wiimotes");
  rb_funcall(ary, rb_intern("clear"), 0, NULL);
  return Qnil;
}

static VALUE rb_cm_wiimotes(VALUE self) {
  VALUE wii = rb_iv_get(self, "@wiimotes");
  return wii;
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

static VALUE rb_wm_rumble(VALUE self) {
  wiimote * wm;
  Data_Get_Struct(self, wiimote, wm);
  wiiuse_rumble(wm, 1);
  rb_iv_set(self, "@rumble", Qtrue);
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
  return Qnil;
}

static VALUE rb_cm_found(VALUE self) {
  connman * conn;
  Data_Get_Struct(self, connman, conn);
  VALUE timeout = rb_const_get(wii_mod, rb_intern("TIMEOUT"));
  VALUE max = rb_const_get(wii_mod, rb_intern("MAX_WIIMOTES"));
  int found = wiiuse_find(conn->wms, NUM2INT(max), NUM2INT(timeout));
  return INT2NUM(found);
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
  wiimote *wm;
  Data_Get_Struct(self, wiimote, wm);
  int ir = WIIUSE_USING_IR(wm);
  if(ir == 0) return Qfalse;
  else return Qtrue;
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

static VALUE rb_wm_accelerometer(VALUE self) {
  wiimote *wm;
  Data_Get_Struct(self, wiimote, wm);
  if(!WIIUSE_USING_ACC(wm)) return Qfalse;
  else return Qtrue;
}

static VALUE rb_cm_connect(VALUE self) {
  connman *conn;
  Data_Get_Struct(self, connman, conn);
  VALUE max = rb_const_get(wii_mod, rb_intern("MAX_WIIMOTES"));
  int connected = wiiuse_connect(conn->wms, NUM2INT(max));
  int i = 0;
  VALUE wm;
  for(; i < NUM2INT(max); i++) {
    if(wm_connected(conn->wms[i])) {
      wm = rb_wm_new(rb_const_get(wii_mod, rb_intern("Wiimote")));
      Data_Wrap_Struct(wm, NULL, free_wiimote, conn->wms[i]);
      rb_ary_push(rb_iv_get(self, "@wiimotes"), wm);
    }
  }
  return INT2NUM(connected);
}

static VALUE rb_cm_poll(VALUE self) {
  if(rb_block_given_p()) {
    VALUE connected = rb_funcall(self, rb_intern("connected"), 0, NULL);
    if(NUM2INT(connected) > 0) {
      VALUE wiimotes = rb_iv_get(self, "@wiimotes");
      connman *conn;
      Data_Get_Struct(self, connman, conn);
      VALUE max = rb_const_get(wii_mod, rb_intern("MAX_WIIMOTES"));
      if(wiiuse_poll(conn->wms, NUM2INT(max))) {
        int i = 0;
        VALUE ary;
        for(; i < NUM2INT(connected); i++) {
          VALUE argv[1];
          argv[0] = INT2NUM(i);
          VALUE wm = rb_ary_aref(1, argv, wiimotes);
          wiimote * wmm;
          Data_Get_Struct(wm, wiimote, wmm);
          if(wmm->event != WIIUSE_NONE) {
            ary = rb_ary_new();
            rb_ary_push(ary, wm);
            VALUE event_name;
            switch(wmm->event) {
              case WIIUSE_EVENT:
                event_name = ID2SYM(rb_intern("generic"));
                break;
              case WIIUSE_STATUS:
                event_name = ID2SYM(rb_intern("status"));
                break;
              case WIIUSE_DISCONNECT:
                event_name = ID2SYM(rb_intern("disconnected"));
                break;
              case WIIUSE_UNEXPECTED_DISCONNECT:
                event_name = ID2SYM(rb_intern("unexpected_disconnect"));
                break;
              case WIIUSE_READ_DATA:
                event_name = ID2SYM(rb_intern("read"));
                break;
              case WIIUSE_NUNCHUK_INSERTED:
                event_name = ID2SYM(rb_intern("nunchuk_inserted"));
                break;
              case WIIUSE_NUNCHUK_REMOVED:
                event_name = ID2SYM(rb_intern("nunchuk_removed"));
                break;
              case WIIUSE_CLASSIC_CTRL_INSERTED:
                event_name = ID2SYM(rb_intern("classic_inserted"));
                break;
              case WIIUSE_CLASSIC_CTRL_REMOVED:
                event_name = ID2SYM(rb_intern("classic_removed"));
                break;
              case WIIUSE_GUITAR_HERO_3_CTRL_INSERTED:
                event_name = ID2SYM(rb_intern("guitarhero3_inserted"));
                break;
              case WIIUSE_GUITAR_HERO_3_CTRL_REMOVED:
                event_name = ID2SYM(rb_intern("guitarhero3_removed"));
                break;
              case WIIUSE_CONNECT:
                event_name = ID2SYM(rb_intern("connected"));
                break;
            }
            rb_ary_push(ary, event_name);
            rb_yield(ary);
          }
        }
      }
    }
  }
  return Qnil;
}

static VALUE rb_cm_each(VALUE self) {
  if(rb_block_given_p()) {
    VALUE connected = rb_funcall(self, rb_intern("connected"), 0, NULL);
    VALUE wiimotes = rb_iv_get(self, "@wiimotes");
    int i = 0;
    for(; i < NUM2INT(connected); i++) {
      VALUE argv[1];
      argv[0] = INT2NUM(i);
      VALUE wm = rb_ary_aref(1, argv, wiimotes);
      rb_yield(wm);
    }
  } 
  return Qnil;
}

static VALUE rb_cm_cp(VALUE self) {
  if(rb_block_given_p()) {
    VALUE m = rb_const_get(wii_mod, rb_intern("MAX_WIIMOTES"));
    int max = NUM2INT(m);
    wiimote **wms = wiiuse_init(max);
    int connected = wiiuse_connect(wms, max);
    while(connected > 0) {
      if(wiiuse_poll(wms, max)) {
        int i = 0;
        for(; i < max; i++) {
          if(wm_connected(wms[i])) {
            if(wms[i]->event != WIIUSE_NONE) {
              VALUE wm = rb_wm_new(rb_const_get(wii_mod, rb_intern("Wiimote")));
              Data_Wrap_Struct(wm, NULL, free_wiimote, wms[i]);
              VALUE ary = rb_ary_new();
              rb_ary_push(ary, wm);
              VALUE event_name;
              switch(wms[i]->event) {
                case WIIUSE_EVENT:
                  event_name = ID2SYM(rb_intern("generic"));
                  break;
                case WIIUSE_STATUS:
                  event_name = ID2SYM(rb_intern("status"));
                  break;
                case WIIUSE_DISCONNECT:
                  event_name = ID2SYM(rb_intern("disconnected"));
                  break;
                case WIIUSE_UNEXPECTED_DISCONNECT:
                  event_name = ID2SYM(rb_intern("unexpected_disconnect"));
                  break;
                case WIIUSE_READ_DATA:
                  event_name = ID2SYM(rb_intern("read"));
                  break;
                case WIIUSE_NUNCHUK_INSERTED:
                  event_name = ID2SYM(rb_intern("nunchuk_inserted"));
                  break;
                case WIIUSE_NUNCHUK_REMOVED:
                  event_name = ID2SYM(rb_intern("nunchuk_removed"));
                  break;
                case WIIUSE_CLASSIC_CTRL_INSERTED:
                  event_name = ID2SYM(rb_intern("classic_inserted"));
                  break;
                case WIIUSE_CLASSIC_CTRL_REMOVED:
                  event_name = ID2SYM(rb_intern("classic_removed"));
                  break;
                case WIIUSE_GUITAR_HERO_3_CTRL_INSERTED:
                  event_name = ID2SYM(rb_intern("guitarhero3_inserted"));
                  break;
                case WIIUSE_GUITAR_HERO_3_CTRL_REMOVED:
                  event_name = ID2SYM(rb_intern("guitarhero3_removed"));
                  break;
                case WIIUSE_CONNECT:
                  event_name = ID2SYM(rb_intern("connected"));
                  break;
              }
              rb_ary_push(ary, event_name);
              rb_yield(ary);
            } 
          }
        }
      }
    }
    wiiuse_cleanup(wms, max);
  }
  return Qnil;
}

void Init_wii4r() {
  wii_mod = rb_define_module("Wii");
  rb_define_const(wii_mod, "MAX_WIIMOTES", INT2NUM(4));
  rb_define_const(wii_mod, "TIMEOUT", INT2NUM(10));
  rb_define_const(wii_mod, "LED_1", INT2NUM(WIIMOTE_LED_1));
  rb_define_const(wii_mod, "LED_2", INT2NUM(WIIMOTE_LED_2));
  rb_define_const(wii_mod, "LED_3", INT2NUM(WIIMOTE_LED_3));
  rb_define_const(wii_mod, "LED_4", INT2NUM(WIIMOTE_LED_4));
  rb_define_const(wii_mod, "LED_ALL", INT2NUM(WIIMOTE_LED_1 | WIIMOTE_LED_2 | WIIMOTE_LED_3 | WIIMOTE_LED_4));
  rb_define_const(wii_mod, "LED_1_AND_2", INT2NUM(WIIMOTE_LED_1 | WIIMOTE_LED_2));
  //TODO: define led consts
  rb_define_const(wii_mod, "A", INT2NUM(WIIMOTE_BUTTON_A));
  rb_define_const(wii_mod, "B", INT2NUM(WIIMOTE_BUTTON_B));
  //TODO: define button consts

  VALUE cm_class = rb_define_class_under(wii_mod, "WiimoteManager", rb_cObject);
  VALUE wii_class = rb_define_class_under(wii_mod, "Wiimote", rb_cObject);

  rb_define_singleton_method(wii_class, "new", rb_wm_new, 0);
  rb_define_method(wii_class, "initialize", rb_wm_init, 0);
  rb_define_method(wii_class, "rumble?", rb_wm_get_rumble, 0);
  rb_define_method(wii_class, "rumble=", rb_wm_set_rumble, 1);
  rb_define_method(wii_class, "rumble!", rb_wm_rumble, 0);
  rb_define_method(wii_class, "stop!", rb_wm_stop, 0);
  rb_define_method(wii_class, "leds=", rb_wm_leds, 1);
  rb_define_method(wii_class, "turn_off_leds!", rb_wm_turnoff, 0);
  rb_define_method(wii_class, "motion_sensing?", rb_wm_get_ms, 0);
  rb_define_method(wii_class, "motion_sensing=", rb_wm_set_ms, 1);
  rb_define_method(wii_class, "status", rb_wm_status, 0);
  rb_define_method(wii_class, "connected?", rb_wm_connected, 0);
  rb_define_method(wii_class, "pressed?", rb_wm_pressed, 1);
  rb_define_method(wii_class, "just_pressed?", rb_wm_just_pressed, 1);
  rb_define_method(wii_class, "using_accelerometer?", rb_wm_accelerometer, 0);
  rb_define_method(wii_class, "roll", rb_wm_roll, 0);
  rb_define_method(wii_class, "absolute_roll", rb_wm_aroll, 0);
  rb_define_method(wii_class, "pitch", rb_wm_pitch, 0);
  rb_define_method(wii_class, "absolute_pitch", rb_wm_pitch, 0);
  rb_define_method(wii_class, "yaw", rb_wm_yaw, 0);
  rb_define_method(wii_class, "using_ir?", rb_wm_ir, 0);
  rb_define_method(wii_class, "ir_sources", rb_wm_ir_sources, 0);
  rb_define_method(wii_class, "ir_cursor", rb_wm_ir_cursor, 0);
  rb_define_method(wii_class, "ir_z", rb_wm_ir_z, 0);
  
  rb_define_singleton_method(cm_class, "new", rb_cm_new, 0);
  rb_define_singleton_method(cm_class, "connect_and_poll", rb_cm_cp, 0);
  rb_define_method(cm_class, "wiimotes", rb_cm_wiimotes, 0);
  rb_define_method(cm_class, "initialize", rb_cm_init, 0);
  rb_define_method(cm_class, "connected", rb_cm_connected, 0);
  rb_define_method(cm_class, "cleanup!", rb_cm_cleanup, 0);
  rb_define_method(cm_class, "found", rb_cm_found, 0);
  rb_define_method(cm_class, "connect", rb_cm_connect, 0);
  rb_define_method(cm_class, "poll", rb_cm_poll, 0);
  rb_define_method(cm_class, "each_wiimote", rb_cm_each, 0);
}
