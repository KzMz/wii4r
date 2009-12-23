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

extern void set_expansion(VALUE self, VALUE exp_obj);

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

/* 
 *  call-seq:
 *	WiimoteManager.new
 *
 *  Returns a new empty WiimoteManager.
 */
 
static VALUE rb_cm_init(VALUE self) {
  VALUE ary = rb_ary_new();
  rb_iv_set(self, "@wiimotes", ary);
  return self;
}

/*
 *  call-seq:
 *	manager.connected	-> int
 *
 *  Returns the number of wiimotes (see <code>Wiimote</code>) connected and managed by <i>self</i>.
 *  
 * 	wm = WiimoteManager.new
 * 	wm.connected	#=> 0
 *
 */

static VALUE rb_cm_connected(VALUE self) {
  VALUE wm = rb_iv_get(self, "@wiimotes");
  VALUE size = rb_funcall(wm, rb_intern("size"), 0, NULL);
  return size;
}

/*
 *  call-seq:
 *	manager.cleanup!
 *
 *  Disconnect all the wiimotes (see <code>Wiimote</code>) managed by <i>self</i> and dealloc all structures.
 *
 */

static VALUE rb_cm_cleanup(VALUE self) {
  connman * conn;
  Data_Get_Struct(self, connman, conn);
  wiiuse_cleanup(conn->wms, conn->n);
  VALUE ary = rb_iv_get(self, "@wiimotes");
  rb_funcall(ary, rb_intern("clear"), 0, NULL);
  return Qnil;
}

/*
 *  call-seq:
 *	manager.wiimotes	-> array
 *
 *  Returns an array containing the wiimotes (see <code>Wiimote</code>) connected to <i>self</i>.
 *  May be an empty array.
 *
 *	wm = WiimoteManager.new
 *	wm.wiimotes	#=> []
 */

static VALUE rb_cm_wiimotes(VALUE self) {
  VALUE wii = rb_iv_get(self, "@wiimotes");
  return wii;
}

/*
 *  call-seq:
 *	manager.found	-> int
 *
 *  Returns the number of wiimotes (see <code>Wiimote</code>) found in bluetooth range but not already connected.
 *
 */

static VALUE rb_cm_found(VALUE self) {
  connman * conn;
  Data_Get_Struct(self, connman, conn);
  VALUE timeout = rb_const_get(wii_mod, rb_intern("TIMEOUT"));
  VALUE max = rb_const_get(wii_mod, rb_intern("MAX_WIIMOTES"));
  int found = wiiuse_find(conn->wms, NUM2INT(max), NUM2INT(timeout));
  return INT2NUM(found);
}

/*
 *  call-seq:
 *	manager.connect		-> int
 *
 *  Tries to connect to all found wiimotes (see <code>Wiimote</code>) and returns the number of successfull connections.
 *
 */

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
  VALUE wm, exp = Qnil;
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
      switch(conn->wms[i]->exp.type) {
        case EXP_NUNCHUK:
          exp = Data_Wrap_Struct(nun_class, NULL, NULL, &(conn->wms[i]->exp.nunchuk));
          break;
        case EXP_CLASSIC:
          exp = Data_Wrap_Struct(cc_class, NULL, NULL, &(conn->wms[i]->exp.classic));
          break;
        case EXP_GUITAR_HERO_3:
          exp = Data_Wrap_Struct(gh3_class, NULL, NULL, &(conn->wms[i]->exp.gh3));
          break;
      }
      set_expansion(wm, exp);
      rb_ary_push(rb_iv_get(self, "@wiimotes"), wm);
    }
  }
  return INT2NUM(connected);
}

/*
 *  call-seq:
 *	manager.poll { |(wiimote, event)| block }	-> nil
 *
 *  Invokes <i>block</i> once per event captured by the WiimoteManager class. <code>wiimote</code> is the Wiimote which caused 
 *  the event <code>event</code>, a Symbol who represents the type of the event caused.
 *
 *	wm.poll { |(wiimote, event)|
 *		if event == :generic
 *			puts "generic event occurred"
 *		end
 *	}
 */ 

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
                VALUE nun = Data_Wrap_Struct(nun_class, NULL, NULL, &(wmm->exp.nunchuk));
                set_expansion(wm, nun);
                break;
              case WIIUSE_NUNCHUK_REMOVED:
                event_name = ID2SYM(rb_intern("nunchuk_removed"));
                set_expansion(wm, Qnil);
                break;
              case WIIUSE_CLASSIC_CTRL_INSERTED:
                event_name = ID2SYM(rb_intern("classic_inserted"));
                VALUE cc = Data_Wrap_Struct(cc_class, NULL, NULL, &(wmm->exp.classic));
                set_expansion(wm, cc);
                break;
              case WIIUSE_CLASSIC_CTRL_REMOVED:
                event_name = ID2SYM(rb_intern("classic_removed"));
                set_expansion(wm, Qnil);
                break;
              case WIIUSE_GUITAR_HERO_3_CTRL_INSERTED:
                event_name = ID2SYM(rb_intern("guitarhero3_inserted"));
                VALUE gh3 = Data_Wrap_Struct(gh3_class, NULL, NULL, &(wmm->exp.gh3));
                set_expansion(wm, gh3);
                break;
              case WIIUSE_GUITAR_HERO_3_CTRL_REMOVED:
                event_name = ID2SYM(rb_intern("guitarhero3_removed"));
                set_expansion(wm, Qnil);
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

/*
 *  call-seq:
 *	manager.each_wiimote { |wiimote| block }	-> nil
 *
 *  Calls <i>block</i> once for each Wiimote connected to <i>self</i>, passing a Wiimote object as a parameter.
 *	wm.each_wiimote { |wiimote|
 *		puts wiimote
 *	}
 *
 */

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


/*
 *  call-seq:
 *	manager.positions	-> array
 *
 *  Returns an array containing the actual position (x, y) of all wiimotes (see <code>Wiimote</code>) connected with <i>self</i>
 *
 */

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
    
/*
 *
 *  Provides an interface for searching wiimotes and connecting to them via bluetooth.
 *  WiimoteManager also provides event management. 
 *  The events that can be caused by a Wiimote device are:
 *	:generic		->	pression of a button, accelerometer or ir event
 *	:status			->	status response received from wiimote
 *	:connect		->	fired when a Wiimote connects
 *	:disconnect		->	fired when a Wiimote disconnects
 *	:enexpected_disconnect	->	fired when a Wiimote disconnects enexpectedly
 *	:nunchuk_inserted	->	fired when a Nunchuk is inserted in a Wiimote
 *	:nunchuk_removed	->	fired when a Nunchuk is removed from a Wiimote
 *	:classic_inserted	->	fired when a Classic Controller is inserted in a Wiimote
 *	:classic_removed	->	fired when a Classic Controller is removed from a Wiimote
 *	:guitarhero3_inserted	->	fired when a Guitar Hero 3 Controller is inserted in a Wiimote
 *	:guitarhero3_removed	->	fired when a Guitar Hero 3 Controller is removed from a Wiimote
 *
 */
  
void init_wiimotemanager(void) {
  
  cm_class = rb_define_class_under(wii_mod, "WiimoteManager", rb_cObject);
  rb_define_singleton_method(cm_class, "new", rb_cm_new, 0);
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
