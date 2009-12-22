all:
	cd ext && ruby extconf.rb && make && mv wii4r.so ..
clean:
	cd ext && make clean
install:
	cd ext && make install
rdoc:
	rdoc ext/wii4r.c ext/wiimotemanager.c ext/wiimote.c ext/nunchuk.c ext/classic.c ext/guitarhero3.c
