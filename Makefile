all:
	cd ext && ruby extconf.rb && make && mv wii4r.so ..
