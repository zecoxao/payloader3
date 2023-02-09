#!/bin/bash 

#Get latest PL3 and build payloads
if [ ! -d PL3 ]; then
  echo "Retrieving latest PL3..."
  git clone https://github.com/kakaroto/PL3
  echo "Patching PL3..."
  patch --forward --strip=1 --input=patch/firmware_symbols.patch PL3/firmware_symbols.h.S
  patch --forward --strip=1 --input=patch/print_debug.patch PL3/print_debug.h.S
  patch --forward --strip=1 --input=patch/payload_dev.patch PL3/payload_dev.S
  patch --forward --strip=1 --input=patch/payload_trace_all_sc_calls.patch PL3/payload_trace_all_sc_calls.S
  patch --forward --strip=1 --input=patch/payload_trace_hypercalls.patch PL3/payload_trace_hypercalls.S
  patch --forward --strip=1 --input=patch/payload_trace_syscalls.patch PL3/payload_trace_syscalls.S
fi

#Clean up from previous builds
echo "Cleaning PayLoader3 directories..."
make clean

#Build PayLoader3 binaries
echo "Building payloader3..."
make pkg

