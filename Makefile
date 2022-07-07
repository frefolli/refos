@all:
	echo -e ""

run:
	make -C build run

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
