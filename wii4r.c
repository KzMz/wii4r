#include<stdio.h>
#include<stdlib.h>

#ifndef WIN32
	#include<unistd.h>
#endif

#include<ruby.h>
#include<wiiuse.h>

VALUE wii_mod = Qnil;

typedef struct _connman {
  wiimote **wms;
  int n;
} connman;

void free_wiimote(void * wm) {
  wiiuse_disconnect((wiimote *) wm);
}

static VALUE rb_wiimote_new(VALUE self) {

  wiimote *wm;
  VALUE obj = Data_Make_Struct(self, wiimote, NULL, free_wiimote, wm); 
  
  return obj;
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

static VALUE rb_cm_wiimotes(VALUE self) {
  VALUE wii = rb_iv_get(self, "@wiimotes");
  return wii;
}

void Init_wii4r() {
  wii_mod = rb_define_module("Wii");
  rb_define_const(wii_mod, "MAX_WIIMOTES", INT2NUM(4));

  VALUE cm_class = rb_define_class_under(wii_mod, "ConnectionManager", rb_cObject);
  VALUE wii_class = rb_define_class_under(wii_mod, "Wiimote", rb_cObject);

  rb_define_singleton_method(wii_class, "new", rb_wiimote_new, 0);
  rb_define_singleton_method(cm_class, "new", rb_cm_new, 0);
  rb_define_method(cm_class, "wiimotes", rb_cm_wiimotes, 0);
  rb_define_method(cm_class, "initialize", rb_cm_init, 0);
  rb_define_method(cm_class, "connected", rb_cm_connected, 0);
}
