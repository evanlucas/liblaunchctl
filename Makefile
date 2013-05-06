CFLAGS=-Wall
DIR=$(dir $(realpath $(lastword $(MAKEFILE_LIST))))

library:
	mkdir -p $(DIR)/build
	clang $(CFLAGS) -I$(DIR)/include -framework CoreFoundation -dynamiclib -std=gnu99 $(DIR)/src/liblaunchctl.c -o $(DIR)/build/liblaunchctl.dylib

all: clean library

test:
	clang $(CFLAGS) -L$(DIR)/build -llaunchctl -I$(DIR)/include $(DIR)/example.c -o $(DIR)/test_launchctl

clean:
	rm -rf $(DIR)/liblaunchctl.dylib $(DIR)launcher
	rm -rf $(DIR)/*.dSYM
	rm -rf $(DIR)/*.o
	rm -rf $(DIR)/build
	rm -rf $(DIR)/test_launchctl

.PHONY: clean library
