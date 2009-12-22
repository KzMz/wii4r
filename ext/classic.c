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
 *	classicctrl.pressed?(button)	-> true or false
 *
 *  Returns true if <i>button</i> is being pressed on <i>self</i>.
 *
 */

static VALUE rb_cc_pressed(VALUE self, VALUE arg) {

}

/*
 * call-seq:
 *	classicctrl.just_pressed?(button)		-> true of false
 *
 * Returns true if <i>button</i> is just pressed on <i>self</i>.
 *
 */

static VALUE rb_cc_jpressed(VALUE self, VALUE arg) {

}

/*
 * call-seq:
 *	classicctrl.held?(button)	-> true of false
 *
 * Returns true if <i>button</i> is being held on <i>self</i>.
 *
 */
 
static VALUE rb_cc_held(VALUE self, VALUE arg) {

}

/*
 * call-seq:
 *	classicctrl.released?(button)	-> true or false
 *
 * Returns true if <i>button</i> is just released on <i>self</i>.
 *
 */	

static VALUE rb_cc_rel(VALUE self, VALUE arg) {

}

/*
 * call-seq:
 *	classicctrl.right_joystick_angle	-> int
 *
 * Returns the angle at which the right joystick is being held.
 * 0   => Straight up
 * 90  => right
 * 180 => down
 * 270 => left
 *
 */

static VALUE rb_cc_rjangle(VALUE self) {

}

/*
 * call-seq:
 *	classicctrl.right_joystick_magnitude	-> float
 *
 * Returns the magnitude at which the right joystick is being held.
 * Range: [0, 1]
 * 0 => center
 * 1 => edges
 *
 */

static VALUE rb_cc_rjmag(VALUE self) {

}

/*
 * call-seq:
 *	classicctrl.left_joystick_angle	-> int
 *
 * Returns the angle at which the left joystick is being held.
 * 0   => Straight up
 * 90  => right
 * 180 => down
 * 270 => left
 *
 */

static VALUE rb_cc_ljangle(VALUE self) {

}

/*
 * call-seq:
 *	classicctrl.left_joystick_magnitude	-> float
 *
 * Returns the magnitude at which the left joystick is being held.
 * Range: [0, 1]
 * 0 => center
 * 1 => edges
 *
 */
 
static VALUE rb_cc_ljmag(VALUE self) {

}

/*
 * call-seq:
 *	classicctrl.left_shoulder	-> float
 *
 * Returns the magnitude at which the left shoulder button is being held.
 * Range: [0, 1]
 * 0 => not pressed
 * 1 => fully pressed
 *
 */
 
static VALUE rb_cc_lshoulder(VALUE self) {

}

/*
 * call-seq:
 *	classicctrl.right_shoulder	-> float
 *
 * Returns the magnitude at which the right shoulder button is being held.
 * Range: [0, 1]
 * 0 => not pressed
 * 1 => fully pressed
 *
 */
 
static VALUE rb_cc_rshoulder(VALUE self) {

}

/*
 * Provides a set of methods for accessing ClassicController properties and members.
 *	- button events (pressed, released, held, just pressed)
 *	- joystick values (angles & magnitude)
 *
 *
 */

void init_cc() {
  cc_class = rb_define_class_under(wii_class, "ClassicController", rb_cObject);
  
  rb_define_method(cc_class, "pressed?", rb_cc_pressed, 1);
  rb_define_method(cc_class, "just_pressed?", rb_cc_jpressed, 1);
  rb_define_method(cc_class, "held?", rb_cc_held, 1);
  rb_define_method(cc_class, "released?", rb_cc_rel, 1);
  rb_define_method(cc_class, "left_joystick_angle", rb_cc_ljangle, 0);
  rb_define_method(cc_class, "left_joystick_magnitude", rb_cc_ljmag, 0);
  rb_define_method(cc_class, "right_joystick_angle", rb_cc_rjangle, 0);
  rb_define_method(cc_class, "right_joystick_magnitude", rb_cc_rjmag, 0);
  rb_define_method(cc_class, "right_shoulder", rb_cc_rshoulder, 0);
  rb_define_method(cc_class, "left_shoulder", rb_cc_lshoulder, 0);
}
