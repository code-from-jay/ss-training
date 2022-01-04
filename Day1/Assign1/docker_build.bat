@echo off
REM Stop the various commands from being printed to the terminal.

REM Sets the variable "SRC_DIR" to the directory that this script is in.
set SRC_DIR=%~dp0

REM Build the actual Docker image using the Dockerfile next to this script.
REM Additionally, set the name of the container to 'dockerdev'
docker build -f %SRC_DIR%\Dockerfile -t dockerdev %SRC_DIR%
