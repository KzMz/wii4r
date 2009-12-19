require 'wii4r'

w = Wii::WiimoteManager.new

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



