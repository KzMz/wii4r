require 'wii4r'

include Wii

w = WiimoteManager.new

begin
  w.connect
  
  wm = w.wiimotes.first
  wm.play_sound
ensure
  w.cleanup!
end
