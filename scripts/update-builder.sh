#!/usr/bin/env bash

# configurable
BUILDER_IMAGE_TAG=sshambir/compiler-builder:0.0.1

# exit on any error
set -e

SCRIPT_DIR=$(dirname "$(readlink -f "$0")")
BUILDER_DIR=$(dirname "$SCRIPT_DIR")/builder

docker build --tag "$BUILDER_IMAGE_TAG" "$BUILDER_DIR"
