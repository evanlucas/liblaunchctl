CFLAGS=-Wall

all:
	make lib

lib:
	mkdir -p ./build
	clang $(CFLAGS) -I./include -framework CoreFoundation -dynamiclib -std=gnu99 ./src/liblaunchctl.c -o ./build/liblaunchctl.dylib

test:
	clang $(CFLAGS) -L./build -llaunchctl -I./include example.c -o test_launchctl

clean:
	rm -rf liblaunchctl.dylib launcher
	rm -rf *.dSYM
	rm -rf *.o
	rm -rf ./build
	rm -rf test_launchctl
