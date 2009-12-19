all:
	cd ext && ruby extconf.rb && make && mv wii4r.so ..
clean:
	cd ext && make clean
install:
	cd ext && make install
