#!/bin/bash

# Sets the variable "SRC_DIR" to the directory that this script is in
SRC_DIR=$(cd $(dirname "$0") && pwd)

# Build the actual Docker image using the Dockerfile next to this script.
# Additionally, set the name of the container to 'dockerdev'
docker build -f $SRC_DIR/Dockerfile -t dockerdev $SRC_DIR
