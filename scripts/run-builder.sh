#!/usr/bin/env bash

# configurable
BUILDER_IMAGE_TAG=sshambir/compiler-builder:0.0.1

# exit on any error
set -e

DOCKER_OPTIONS="--interactive"
if [ -t 0 ]; then
  DOCKER_OPTIONS="$DOCKER_OPTIONS --tty"
fi

docker run $DOCKER_OPTIONS --rm "$BUILDER_IMAGE_TAG" bash
