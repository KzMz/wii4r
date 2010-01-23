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

/*
 *  call-seq:
 *	nunchuk.pressed?(button)	-> true or false
 *
 *  Returns true if <i>button</i> is being pressed on <i>self</i>.
 *
 */

static VALUE rb_nun_pressed(VALUE self, VALUE arg) {
  nunchuk_t *nun;
  Data_Get_Struct(self, nunchuk_t, nun);
  if(!nun) return Qnil;
  Check_Type(arg, T_FIXNUM);
  if(IS_PRESSED(nun, NUM2INT(arg)))
    return Qtrue;
  else
    return Qfalse;
}

/*
 * call-seq:
 *	nunchuk.just_pressed?(button)		-> true of false
 *
 * Returns true if <i>button</i> is just pressed on <i>self</i>.
 *
 */

static VALUE rb_nun_jpressed(VALUE self, VALUE arg) {
  nunchuk_t *nun;
  Data_Get_Struct(self, nunchuk_t, nun);
  if(!nun) return Qnil;
  Check_Type(arg, T_FIXNUM);
  if(IS_JUST_PRESSED(nun, NUM2INT(arg)))
    return Qtrue;
  else
    return Qfalse;
}

/*
 * call-seq:
 *	nunchuk.held?(button)	-> true of false
 *
 * Returns true if <i>button</i> is being held on <i>self</i>.
 *
 */
 
static VALUE rb_nun_held(VALUE self, VALUE arg) {
  nunchuk_t *nun;
  Data_Get_Struct(self, nunchuk_t, nun);
  if(!nun) return Qnil;
  Check_Type(arg, T_FIXNUM);
  if(IS_HELD(nun, NUM2INT(arg)))
    return Qtrue;
  else
    return Qfalse;
}

/*
 * call-seq:
 *	nunchuk.released?(button)	-> true or false
 *
 * Returns true if <i>button</i> is just released on <i>self</i>.
 *
 */	

static VALUE rb_nun_rel(VALUE self, VALUE arg) {
  nunchuk_t *nun;
  Data_Get_Struct(self, nunchuk_t, nun);
  if(!nun) return Qnil;
  Check_Type(arg, T_FIXNUM);
  if(IS_RELEASED(nun, NUM2INT(arg)))
    return Qtrue;
  else
    return Qfalse;
}

/*
 * call-seq:
 *	nunchuk.pitch	-> float
 *
 * Returns the rotation around X axis (works with motion sensing activated).
 *
 * Range: [-179.9, -0.1] [0.1, 179.9] 
 *
 */

static VALUE rb_nun_pitch(VALUE self) {
  nunchuk_t *nun;
  Data_Get_Struct(self, nunchuk_t, nun);
  if(!nun) return Qnil;
  return rb_float_new(nun->orient.pitch);
}

/*
 * call-seq:
 *	nunchuk.absolute_pitch	-> float
 *
 * Returns the absolute rotation around X axis (works with motion sensing activated).
 *
 * Range: [-179.9, -0.1] [0.1, 179.9] 
 *
 */

static VALUE rb_nun_apitch(VALUE self) {
  nunchuk_t *nun;
  Data_Get_Struct(self, nunchuk_t, nun);
  if(!nun) return Qnil;
  return rb_float_new(nun->orient.a_pitch);
}

/*
 * call-seq:
 *	nunchuk.roll	-> float
 *
 * Returns the rotation around Y axis (works with motion sensing activated).
 *
 * Range: [-179.9, -0.1] [0.1, 179.9] 
 *
 */

static VALUE rb_nun_roll(VALUE self) {
  nunchuk_t *nun;
  Data_Get_Struct(self, nunchuk_t, nun);
  if(!nun) return Qnil;
  return rb_float_new(nun->orient.roll);
}

/*
 * call-seq:
 *	nunchuk.absolute_roll	-> float
 *
 * Returns the absolute rotation around Y axis (works with motion sensing activated).
 *
 * Range: [-179.9, -0.1] [0.1, 179.9] 
 *
 */

static VALUE rb_nun_aroll(VALUE self) {
  nunchuk_t *nun;
  Data_Get_Struct(self, nunchuk_t, nun);
  if(!nun) return Qnil;
  return rb_float_new(nun->orient.a_roll);
}

/*
 * call-seq:
 *	nunchuk.acceleration	-> array
 *
 * Returns an array containing the three components of acceleration [x, y, z] of <i>self</i>.
 *
 */

static VALUE rb_nun_accel(VALUE self) {
  nunchuk_t *nun;
  Data_Get_Struct(self, nunchuk_t, nun);
  VALUE ary = rb_ary_new();
  if(nun) {
    rb_ary_push(ary, INT2NUM(nun->accel.x));
    rb_ary_push(ary, INT2NUM(nun->accel.y));
    rb_ary_push(ary, INT2NUM(nun->accel.z));
  }
  return ary;
}

/*
 * call-seq:
 *	nunchuk.gravity_force	-> array
 *
 * Returns an array containing the three components of gravity force [x, y, z] of <i>self</i>.
 *
 */
 
static VALUE rb_nun_gforce(VALUE self) {
  nunchuk_t *nun;
  Data_Get_Struct(self, nunchuk_t, nun);
  VALUE ary = rb_ary_new();
  if(nun) {
    rb_ary_push(ary, INT2NUM(nun->gforce.x));
    rb_ary_push(ary, INT2NUM(nun->gforce.y));
    rb_ary_push(ary, INT2NUM(nun->gforce.z));
  }
  return ary;
}

/*
 * call-seq:
 *	wiimote.accel_threshold	-> int
 *
 * Returns the accelerometer event threshold for <i>self</i>.
 *
 */

static VALUE rb_nun_athreshold(VALUE self) {
  nunchuk_t *nun;
  Data_Get_Struct(self, nunchuk_t, nun);
  if(!nun) return Qnil;
  return INT2NUM(nun->accel_threshold);
}

/*
 * call-seq:
 *	nunchuk.orient_threshold	-> int
 *
 * Returns the orientation event threshold for <i>self</i>.
 *
 */

static VALUE rb_nun_othreshold(VALUE self) {
  nunchuk_t *nun;
  Data_Get_Struct(self, nunchuk_t, nun);
  if(!nun) return Qnil;
  return rb_float_new(nun->orient_threshold);
} 

/*
 * call-seq:
 *	nunchuk.joystick_angle	-> float
 *
 * Returns the angle at which the joystick is being held.
 *
 * 0   => Straight up
 *
 * 90  => right
 *
 * 180 => down
 *
 * 270 => left
 *
 */

static VALUE rb_nun_jangle(VALUE self) {
  nunchuk_t *nun;
  Data_Get_Struct(self, nunchuk_t, nun);
  if(!nun) return Qnil;
  return rb_float_new(nun->js.ang);
}

/*
 * call-seq:
 *	nunchuk.joystick_magnitude	-> float
 *
 * Returns the magnitude at which the joystick is being held.
 *
 * Range: [0, 1]
 *
 * 0 => center
 *
 * 1 => edges
 *
 */

static VALUE rb_nun_jmag(VALUE self) {
  nunchuk_t *nun;
  Data_Get_Struct(self, nunchuk_t, nun);
  if(!nun) return Qnil;
  return rb_float_new(nun->js.mag);
}

/*
 * Provides a set of methods for accessing Nunchuk properties and members.
 *	- button events (pressed, just pressed, released, held)
 *	- motion sensing capabilities (roll, pitch)
 *	- acceleration & gravity force vectors
 *	- joystick values (angle & magnitude)
 *
 */

void init_nunchuk() {
  nun_class = rb_define_class_under(wii_class, "Nunchuk", rb_cObject);
  
  rb_define_method(nun_class, "pressed?", rb_nun_pressed, 1);
  rb_define_method(nun_class, "just_pressed?", rb_nun_jpressed, 1);
  rb_define_method(nun_class, "held?", rb_nun_held, 1);
  rb_define_method(nun_class, "released?", rb_nun_rel, 1);
  rb_define_method(nun_class, "roll", rb_nun_roll, 0);
  rb_define_method(nun_class, "absolute_roll", rb_nun_aroll, 0);
  rb_define_method(nun_class, "pitch", rb_nun_pitch, 0);
  rb_define_method(nun_class, "absolute_pitch", rb_nun_apitch, 0);
  rb_define_method(nun_class, "acceleration", rb_nun_accel, 0);
  rb_define_method(nun_class, "gravity_force", rb_nun_gforce, 0);
  rb_define_method(nun_class, "orient_threshold", rb_nun_othreshold, 0);
  rb_define_method(nun_class, "accel_thresold", rb_nun_athreshold, 0);
  rb_define_method(nun_class, "joystick_angle", rb_nun_jangle, 0);
  rb_define_method(nun_class, "joystick_magnitude", rb_nun_jmag, 0);
}
