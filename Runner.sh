#!/bin/sh

#  Runner.sh
#  liblaunchctl
#
#  Created by Evan Lucas on 6/21/13.
#  Copyright (c) 2013 Hattiesburg Clinic. All rights reserved.

echo "$1"ing liblaunchctl

if [[ "$1" == "clean" ]]; then
    xcodebuild clean
else
    make
fi