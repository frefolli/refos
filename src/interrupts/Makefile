nasm_objects=$(patsubst %.s,%.o,$(wildcard *.s))
gcc_objects=$(patsubst %.c,%.o,$(wildcard *.c))
gxx_objects=$(patsubst %.cpp,%.o,$(wildcard *.cpp))
result=../../build/$$(basename $$PWD).o
include=-I../../inc
gccflags=-m64 -Wall -Wextra -Werror $(include) -nostdlib -nostdinc -fno-builtin -fno-stack-protector
gxxflags=-m64 -Wall -Wextra -Werror $(include) -fno-rtti -nostdlib -nostdinc -fno-builtin -fno-stack-protector -fno-exceptions

@all: $(result)

$(result): $(gcc_objects) $(gxx_objects) $(nasm_objects)
	ld -relocatable $^ -o $(result)

$(nasm_objects): %.o: %.s
	nasm -f elf64 $^ -o $@

$(gcc_objects): %.o: %.c
	gcc -c $(gccflags) $^ -o $@

$(gxx_objects): %.o: %.cpp
	g++ -c $(gxxflags) $^ -o $@

clean:
	rm -rf $(gcc_objects) $(gxx_objects) $(nasm_objects)

config:
	cp ../../skel/Makefile Makefile
