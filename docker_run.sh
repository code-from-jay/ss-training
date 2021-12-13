#!/bin/bash

SRC_DIR=$(cd $(dirname "$0") && pwd)

# Spin up the 'dockerdev' image created by docker_build.bat, then enter into bash
docker run -it -v $SRC_DIR:/home/development/ss-training dockerdev bash --init-file "/home/development/.profile"
