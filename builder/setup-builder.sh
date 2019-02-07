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
    apt-get -qq install -y libstdc++-6-dev clang-3.9 llvm-3.9-dev

    if ! [ -f /usr/bin/cc ]; then
        ln -s /usr/bin/clang-3.9 /usr/bin/cc
    fi
    ln -s /usr/bin/clang-3.9 /usr/bin/clang
    if ! [ -f /usr/bin/c++ ]; then
        ln -s /usr/bin/clang++-3.9 /usr/bin/c++
    fi
    ln -s /usr/bin/clang++-3.9 /usr/bin/clang++
}

install_cmake() {
    echo 3. install cmake
    apt-get -qq update
    apt-get -qq install -y  cmake
}

install_dev_libraries() {
    echo 4. install boost
    apt-get -qq update
    apt-get -qq install -y  libboost-all-dev zlib1g-dev
}

prepare_install
install_clang
install_cmake
install_dev_libraries
