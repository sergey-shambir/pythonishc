#!/usr/bin/env bash

# configurable
BASE_IMAGE_TAG=sshambir/compiler-base:0.0.1

# exit on any error
set -e

SCRIPT_DIR=$(dirname "$(readlink -f "$0")")
SRC_DIR=$(dirname "$SCRIPT_DIR")/base

docker build --tag "$BASE_IMAGE_TAG" "$SRC_DIR"
