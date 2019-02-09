#!/usr/bin/env bash

# exit on any error
set -e

# avoid interactive questions when installing packages
export DEBIAN_FRONTEND=noninteractive

install_tools() {
    echo 1. install runtime build tools
    apt-get -qq update
    apt-get -qq install -y --no-install-recommends build-essential
}

install_libraries() {
    echo 2. install runtime libraries
    apt-get -qq update
    apt-get -qq install -y --no-install-recommends \
        libstdc++6 \
        libllvm3.9 \
        libboost-system1.62.0 \
        libboost-filesystem1.62.0 \
        libboost-program-options1.62.0 \
        zlib1g
}

install_llvm_tools() {
    echo 3. install LLVM build tools
    apt-get -qq update
    apt-get -qq install -y --no-install-recommends llvm-3.9-dev
    ln -s /usr/bin/llc-3.9 /usr/bin/llc
}

install_python3() {
    apt-get -qq update
    apt-get -qq install -y --no-install-recommends python3
}

install_libraries
install_tools
install_llvm_tools
install_python3
