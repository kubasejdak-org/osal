#!/bin/bash

conan install .. --build -pr arm-linux-gnueabihf-gcc-9 -s build_type=Debug
cmake .. -DPLATFORM=linux-arm "${@}"
