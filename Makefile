@all:
	echo -e ""

clear:
	clear

run:
	make -C build run

bochs:
	make -C build bochs

compile:
	make -C src   compile
	make -C build compile

clean:
	make -C build clean
	make -C src   clean
	make -C log   clean

config:
	make -C build config
	make -C src   config

reset: clean config
