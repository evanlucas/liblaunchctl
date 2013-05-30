DIR=$(dir $(realpath $(lastword $(MAKEFILE_LIST))))

library:
	xcodebuild -configuration 'Release' CONFIGURATION_BUILD_DIR=$(DIR)

clean:
	xcodebuild clean

all: clean library

test:
	xcodebuild -configuration 'Release' CONFIGURATION_BUILD_DIR=$(DIR) -alltargets

install:
	xcodebuild -configuration 'Release' CONFIGURATION_BUILD_DIR=$(DIR) -alltargets
	cp build/liblaunchctl.build/Releasebuild/liblaunchctl.dylib /usr/local/lib/

.PHONY: clean library
