require 'wii4r'

w = Wii::WiimoteManager.new

begin

  puts "Connecting.... Press 1 and 2 BUTTONS"
  
  w.connect

  while(!w.wiimotes[0].pressed? Wii::BUTTON_UP)
    w.poll do |(wm, event)|
      if event == :generic
        if wm.pressed? Wii::BUTTON_A
			puts "A!"
        elsif wm.pressed? Wii::BUTTON_B
            puts "B!"
        elsif wm.pressed? Wii::BUTTON_DOWN
            puts "DOWN!"
        elsif wm.pressed? Wii::BUTTON_LEFT
            puts "LEFT!"
        elsif wm.pressed? Wii::BUTTON_RIGHT
            puts "RIGHT!"
        elsif wm.pressed? Wii::BUTTON_PLUS
            puts "+!"
        elsif wm.pressed? Wii::BUTTON_MINUS
            puts "-!"
        elsif wm.pressed? Wii::BUTTON_HOME
            puts "HOME!"
        elsif wm.pressed? Wii::BUTTON_ONE
            puts "1!"
        elsif wm.pressed? Wii::BUTTON_TWO
            puts "2!"
        end
      end
    end
end

ensure
    w.cleanup!
end
