#!/usr/bin/env bash

# exit on any error
set -e

# avoid interactive questions when installing packages
export DEBIAN_FRONTEND=noninteractive

add_apt_repository() {
    local FILENAME=/etc/apt/sources.list.d/$1.list
    local URL=$2
    local DISTRO_NAME=$3
    local SECTION=$4
    echo "deb $URL $DISTRO_NAME $SECTION" >$FILENAME
}

prepare_install() {
    echo 1. prepare install
    apt-get -qq update
    apt-get -qq install -y wget gnupg
}

install_clang() {
    echo 2. install clang
    wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add -
    add_apt_repository llvm-toolchain-stretch http://apt.llvm.org/stretch/ llvm-toolchain-stretch main
    apt-get -qq update
    apt-get -qq install -y libstdc++-6-dev clang-3.9 llvm-3.9-dev cmake

    ln -s /usr/bin/clang-3.9 /usr/bin/cc
    ln -s /usr/bin/clang-3.9 /usr/bin/clang
    ln -s /usr/bin/clang++-3.9 /usr/bin/clang++
    ln -s /usr/bin/clang++-3.9 /usr/bin/c++
    ln -s /usr/bin/clang++-3.9 /usr/bin/c++
}

prepare_install
install_clang
