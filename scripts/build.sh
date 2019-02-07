#!/usr/bin/env bash

# configurable
IMAGE_TAG=sshambir/compiler:0.0.1

# exit on any error
set -e

SCRIPT_DIR=$(dirname "$(readlink -f "$0")")
PROJECT_DIR=$(dirname "$SCRIPT_DIR")

docker build --tag "$IMAGE_TAG" "$PROJECT_DIR"
