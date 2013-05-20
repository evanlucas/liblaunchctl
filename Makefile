DIR=$(dir $(realpath $(lastword $(MAKEFILE_LIST))))

library:
	xcodebuild -configuration 'Release' CONFIGURATION_BUILD_DIR=$(DIR)

clean:
	xcodebuild clean

all: clean library

test:
	xcodebuild -configuration 'Release' CONFIGURATION_BUILD_DIR=$(DIR) -alltargets

.PHONY: clean library
