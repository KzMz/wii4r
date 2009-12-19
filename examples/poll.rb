############################################################################
# 																		   #
# Copyright (C) 2009 by KzMz KzMz@modusbibendi.org                         #
# Copyright (C) 2009 by BuZz Gambino.Giorgio@gmail.com                     #
#                                                                          #
# This program is free software; you can redistribute it and/or modify     #
# it under the terms of the GNU General Public License as published by     #
# the Free Software Foundation; either version 2 of the License, or        #
# (at your option) any later version.                                      #
#                                                                          #
# This program is distributed in the hope that it will be useful,          #
# but WITHOUT ANY WARRANTY; without even the implied warranty of           #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the             #
# GNU General Public License for more details.                             #
#                                                                          #
# You should have received a copy of the GNU General Public License        #
# along with this program; if not, write to the                            #
# Free Software Foundation, Inc.,                                          #
# 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.                 #
############################################################################

require 'wii4r'
include Wii

def set_rumble(w)
	w.rumble= w.rumble? ?  false : true 
end

def status(w)
	puts w.status
end

def accel_on(w)
    w.motion_sensing = true
	puts "Pitch: #{w.pitch}"
	puts "Roll:  #{w.roll}"
	w.motion_sensing = false
end

def vres(w)
	puts "Virtual Resolution => #{w.virtual_resolution}"
end

def sens_pos(w)
	puts "Sensor Bar Position => #{w.sensor_bar_position}"
end

def a_ratio(w)
	puts "Aspect Ratio => #{w.aspect_ratio}"
end

def sens(w)
	w.ir = true
	puts "IR Sensitivity Level => #{w.sensitivity}"
end

def speaker(w)

	if w.speaker? 
	puts "Wiimote is using speaker"
	else puts "Wiimote is not using speaker"
	end
end

begin
   
  w = WiimoteManager.new
  
  puts "Connecting.... Press 1 and 2 BUTTONS"
  
  w.connect

  while(!w.wiimotes[0].pressed? Wii::BUTTON_UP)
    w.poll do |(wm, event)|
      if event == :generic
        if wm.pressed? Wii::BUTTON_A
			set_rumble(wm)
		elsif wm.pressed? Wii::BUTTON_B
            status(wm)
        elsif wm.pressed? Wii::BUTTON_DOWN
            speaker(wm)
        elsif wm.pressed? Wii::BUTTON_LEFT
            puts "LEFT!"
        elsif wm.pressed? Wii::BUTTON_RIGHT
            puts "RIGHT!"
        elsif wm.pressed? Wii::BUTTON_PLUS
            accel_on(wm)
        elsif wm.pressed? Wii::BUTTON_MINUS
            vres(wm)
        elsif wm.pressed? Wii::BUTTON_HOME
            sens_pos(wm)
        elsif wm.pressed? Wii::BUTTON_ONE
            a_ratio(wm)
        elsif wm.pressed? Wii::BUTTON_TWO
            sens(wm)
        end
      end
    end
end

ensure
    w.cleanup!
end
