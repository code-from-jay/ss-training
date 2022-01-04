#!/bin/bash

# Change into a temporary directory
#pushd /tmp

# Extract the latest release tag for protobufs
release_tag=$(curl --silent "https://api.github.com/repos/protocolbuffers/protobuf/releases/latest" | grep -Po '"tag_name": "\K.*?(?=")' | awk '{print substr($0, 2)}')

## Download and Install ProtoC

echo "Downloading ProtoC v$release_tag..."

# Download latest ProtoC binaries
curl --silent -LO "https://github.com/protocolbuffers/protobuf/releases/download/v$release_tag/protoc-$release_tag-linux-x86_64.zip"

echo "Extracting protoc-$release_tag-linux-x86_64.zip..."

# Extract
unzip protoc-$release_tag-linux-x86_64.zip -d protoc

echo "Installing ProtoC binaries and scripts..."
# Install
cp -rv protoc/bin/* /usr/bin
cp -rv protoc/include/* /usr/include

## Download and Install Protobuf Library

echo "Downloading Protobuf C++ Library Files..."
# Download Latest Protobuf C++ Library Files
curl --silent -LO "https://github.com/protocolbuffers/protobuf/releases/download/v$release_tag/protobuf-cpp-$release_tag.tar.gz"

echo "Extracting protobuf-cpp-$release_tag.tar.gz..."
# Extract
tar -xvf protobuf-cpp-$release_tag.tar.gz
pushd protobuf-$release_tag

echo "Configuring ProtoBuf for compilation with clang..."
# Configure Protobuf
CC=clang CXX=clang++ ./configure --prefix=/usr

echo "Building ProtoBuf..."
# Build Protobuf
make -j4

echo "Installing ProtoBuf libraries..."
# Install Protobuf
make install

popd
