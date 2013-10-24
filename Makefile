NPM := $(shell which npm 2>/dev/null)
GYP := $(shell which node-gyp 2>/dev/null)
PATH := ${PATH}:/usr/local/bin
NODE_PATH=:/usr/local/lib/node_modules

all:
	/usr/local/bin/npm install -g node-gyp
	/usr/local/bin/node-gyp clean
	/usr/local/bin/node-gyp configure
	/usr/local/bin/node-gyp build

clean:
	xcodebuild clean
	/usr/local/bin/node-gyp clean

test:
	xcodebuild -scheme make test

.PHONY: all
