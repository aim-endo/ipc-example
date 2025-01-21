#!/bin/bash

DISPLAY=:0 docker run -it --rm \
    --name ipc-example \
    --hostname ipc-example \
    --cap-add=CAP_SYS_PTRACE \
    --ulimit memlock=-1 \
    --ulimit stack=-1 \
    -v /home/$(id -u -n)/Workspace:/home/$(id -u -n)/Workspace \
    -p 4022:22 \
    aim-endo/ipc-example:latest zsh

