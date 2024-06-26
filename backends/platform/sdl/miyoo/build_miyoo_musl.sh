#!/bin/bash

set -e

TOOLCHAIN=/opt/miyoo-musl
SYSROOT=$TOOLCHAIN/arm-miyoo-linux-musleabi

export PATH=$TOOLCHAIN/usr/bin:$SYSROOT/usr/include:$TOOLCHAIN/bin:$PATH
export CXX=arm-linux-g++

./configure --host=miyoo --enable-release --disable-detection-full --enable-plugins --default-dynamic --enable-engine=testbed

make -j5 all sd-root sd-zip
