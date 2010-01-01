require 'wii4r'

include Wii

w = WiimoteManager.new

begin
  w.connect
  
  wm = w.wiimotes.first
  wm.play("test.wav")
ensure
  w.cleanup!
end
