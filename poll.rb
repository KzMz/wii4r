require 'wii4r'

w = Wii::WiimoteManager.new
begin
  w.connect

  while(!w.wiimotes[0].pressed? Wii::BUTTON_UP)
    w.poll do |(wm, event)|
      if event == :generic
        if wm.pressed? Wii::BUTTON_A
          puts "A!"
        else
          if wm.pressed? Wii::BUTTON_B
            puts "B!"
          end
        end
      end
    end
  end
ensure
    w.cleanup!
end
