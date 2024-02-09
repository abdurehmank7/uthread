#!/bin/bash

ARCH=$(uname -m)

if [ "$ARCH" == "x86_64" ]; then
    BUILD="docker build -t os-fall-2023:latest ."
elif [ "$ARCH" == "arm64" ]; then
    BUILD="docker buildx build --platform linux/amd64 -t os-fall-2023:latest ."
fi

echo "Building container"
echo BUILD: $BUILD
$BUILD
