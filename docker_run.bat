@echo off
REM Stop the various commands from being printed to the terminal.

REM Spin up the 'dockerdev' image created by docker_build.bat, then enter into bash
docker run -it dockerdev bash
