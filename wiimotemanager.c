#include "wii4r.h"

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

static VALUE rb_cm_found(VALUE self) {
  connman * conn;
  Data_Get_Struct(self, connman, conn);
  VALUE timeout = rb_const_get(wii_mod, rb_intern("TIMEOUT"));
  VALUE max = rb_const_get(wii_mod, rb_intern("MAX_WIIMOTES"));
  int found = wiiuse_find(conn->wms, NUM2INT(max), NUM2INT(timeout));
  return INT2NUM(found);
}

static VALUE rb_cm_connect(VALUE self) {
  connman *conn;
  Data_Get_Struct(self, connman, conn);
  VALUE max = rb_const_get(wii_mod, rb_intern("MAX_WIIMOTES"));
  VALUE timeout = rb_const_get(wii_mod, rb_intern("TIMEOUT"));
  int found = wiiuse_find(conn->wms, NUM2INT(max), NUM2INT(timeout));
  if(!found) return INT2NUM(0); 
  int connected = wiiuse_connect(conn->wms, NUM2INT(max));
  int i = 0;
  int led = 1;
  VALUE wm;
  for(; i < NUM2INT(max); i++) {
    if(wm_connected(conn->wms[i])) {
      switch(led) {
        case 1:
          wiiuse_set_leds(conn->wms[i], WIIMOTE_LED_1);
          led++;
          break;
        case 2:
          wiiuse_set_leds(conn->wms[i], WIIMOTE_LED_2);
          led++;
          break;
        case 3:
          wiiuse_set_leds(conn->wms[i], WIIMOTE_LED_3);
          led++;
          break;
        case 4:
          wiiuse_set_leds(conn->wms[i], WIIMOTE_LED_4);
          break;  
      }
      wm = Data_Wrap_Struct(wii_class, NULL, free_wiimote, conn->wms[i]);
      rb_obj_call_init(wm, 0, 0);
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
            VALUE event_name = Qnil;
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
              VALUE wm = Data_Wrap_Struct(wii_class, NULL, free_wiimote, wms[i]);
              rb_obj_call_init(wm, 0, 0);
              VALUE ary = rb_ary_new();
              rb_ary_push(ary, wm);
              VALUE event_name = Qnil;
              switch(wms[i]->event) {
                case WIIUSE_EVENT:
                  event_name = ID2SYM(rb_intern("generic"));
                  break;
                case WIIUSE_STATUS:
                  event_name = ID2SYM(rb_intern("status"));
                  break;
                case WIIUSE_DISCONNECT:
                  event_name = ID2SYM(rb_intern("disconnected"));
                  connected--;
                  break;
                case WIIUSE_UNEXPECTED_DISCONNECT:
                  event_name = ID2SYM(rb_intern("unexpected_disconnect"));
                  connected--;
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

static VALUE rb_cm_pos(VALUE self) {
  VALUE wiimotes = rb_iv_get(self, "@wiimotes");
  VALUE ary = rb_ary_new();
  VALUE wsize = rb_funcall(wiimotes, rb_intern("size"), 0, NULL);
  int size = NUM2INT(wsize);
  short i;
  VALUE argv[1], wm, pos;
  for(i = 0; i < size; i++) {
    argv[0] = INT2NUM(i);
    wm = rb_ary_aref(1, argv, wiimotes);
    pos = rb_funcall(wm, rb_intern("position"), 0, NULL);
    rb_ary_push(ary, pos);
  }
  return ary;
}


void init_wiimotemanager(void) {
  
  cm_class = rb_define_class_under(wii_mod, "WiimoteManager", rb_cObject);
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
  rb_define_method(cm_class, "positions", rb_cm_pos, 0);
}
