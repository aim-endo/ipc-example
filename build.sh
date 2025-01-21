#!/bin/bash

DOCKER_BUILDKIT=1 docker build --secret id=PASSWORD --ssh default -t aim-endo/ipc-example:latest \
    --build-arg USERNAME=$(id -u -n) \
    --build-arg GROUPNAME=$(id -g -n) \
    --build-arg UID=$(id -u) \
    --build-arg GID=$(id -g) \
    .
