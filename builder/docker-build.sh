#!/usr/bin/env bash

# exit on any error
set -e

docker build --tag sshambir/compiler-builder:0.0.1 .
