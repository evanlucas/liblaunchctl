CFLAGS=-Wall

all:
	make lib

lib:
	mkdir -p ./build
	clang $(CFLAGS) -I./include -framework CoreFoundation -dynamiclib -std=gnu99 ./src/liblaunchctl.c -o ./build/liblaunchctl.dylib

clean:
	rm -rf liblaunchctl.dylib launcher
	rm -rf *.dSYM
	rm -rf *.o
	rm -rf ./build
