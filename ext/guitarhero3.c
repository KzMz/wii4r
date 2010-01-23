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
 *	gh3ctrl.pressed?(button)	-> true or false
 *
 *  Returns true if <i>button</i> is being pressed on <i>self</i>.
 *
 */

static VALUE rb_gh3_pressed(VALUE self, VALUE arg) {
  guitar_hero_3_t *gh3;
  Data_Get_Struct(self, guitar_hero_3_t, gh3);
  if(!gh3) return Qnil;
  Check_Type(arg, T_FIXNUM);
  if(IS_PRESSED(gh3, NUM2INT(arg)))
    return Qtrue;
  else
    return Qfalse;
}

/*
 * call-seq:
 *	gh3ctrl.just_pressed?(button)		-> true of false
 *
 * Returns true if <i>button</i> is just pressed on <i>self</i>.
 *
 */

static VALUE rb_gh3_jpressed(VALUE self, VALUE arg) {
  guitar_hero_3_t *gh3;
  Data_Get_Struct(self, guitar_hero_3_t, gh3);
  if(!gh3) return Qnil;
  Check_Type(arg, T_FIXNUM);
  if(IS_JUST_PRESSED(gh3, NUM2INT(arg)))
    return Qtrue;
  else
    return Qfalse;
}

/*
 * call-seq:
 *	gh3ctrl.held?(button)	-> true of false
 *
 * Returns true if <i>button</i> is being held on <i>self</i>.
 *
 */
 
static VALUE rb_gh3_held(VALUE self, VALUE arg) {
  guitar_hero_3_t *gh3;
  Data_Get_Struct(self, guitar_hero_3_t, gh3);
  if(!gh3) return Qnil;
  Check_Type(arg, T_FIXNUM);
  if(IS_HELD(gh3, NUM2INT(arg)))
    return Qtrue;
  else
    return Qfalse;
}

/*
 * call-seq:
 *	gh3ctrl.released?(button)	-> true or false
 *
 * Returns true if <i>button</i> is just released on <i>self</i>.
 *
 */	

static VALUE rb_gh3_rel(VALUE self, VALUE arg) {
  guitar_hero_3_t *gh3;
  Data_Get_Struct(self, guitar_hero_3_t, gh3);
  if(!gh3) return Qnil;
  Check_Type(arg, T_FIXNUM);
  if(IS_RELEASED(gh3, NUM2INT(arg)))
    return Qtrue;
  else
    return Qfalse;
}

/*
 * call-seq:
 *	gh3ctrl.joystick_angle	-> float
 *
 * Returns the angle at which the joystick is being held.
 * 0   => Straight up
 * 90  => right
 * 180 => down
 * 270 => left
 *
 */

static VALUE rb_gh3_jangle(VALUE self) {
  guitar_hero_3_t *gh3;
  Data_Get_Struct(self, guitar_hero_3_t, gh3);
  if(!gh3) return Qnil;
  return rb_float_new(gh3->js.ang);
}

/*
 * call-seq:
 *	gh3ctrl.joystick_magnitude	-> float
 *
 * Returns the magnitude at which the joystick is being held.
 * Range: [0, 1]
 * 0 => center
 * 1 => edges
 *
 */

static VALUE rb_gh3_jmag(VALUE self) {
  guitar_hero_3_t *gh3;
  Data_Get_Struct(self, guitar_hero_3_t, gh3);
  if(!gh3) return Qnil;
  return rb_float_new(gh3->js.mag);
}

/*
 * call-seq:
 *	gh3ctrl.whammy_bar	-> float
 *
 * Returns the magnitude at which the whammy bar is being held.
 * Range: [0, 1]
 * 0 => not pressed
 * 1 => fully pressed
 *
 */

static VALUE rb_gh3_wbar(VALUE self) {
  guitar_hero_3_t *gh3;
  Data_Get_Struct(self, guitar_hero_3_t, gh3);
  if(!gh3) return Qnil;
  return rb_float_new(gh3->whammy_bar);
}

/*
 * Provides a set of methods for accessing Guitar Hero 3 Controller properties and members.
 *	- button events (pressed, just pressed, held, released)
 *	- whammy bar value
 *	- joystick values (angle, magnitude)
 *
 */

void init_gh3() {
  gh3_class = rb_define_class_under(wii_class, "GH3Controller", rb_cObject);
  
  rb_define_method(gh3_class, "pressed?", rb_gh3_pressed, 1);
  rb_define_method(gh3_class, "just_pressed?", rb_gh3_jpressed, 1);
  rb_define_method(gh3_class, "held?", rb_gh3_held, 1);
  rb_define_method(gh3_class, "released?", rb_gh3_rel, 1);
  rb_define_method(gh3_class, "whammy_bar", rb_gh3_wbar, 0);
  rb_define_method(gh3_class, "joystick_angle", rb_gh3_jangle, 0);
  rb_define_method(gh3_class, "joystick_magnitude", rb_gh3_jmag, 0);
}
