#!/bin/bash

# Check if the g++ compiler is installed
if ! command -v g++ &> /dev/null; then
    echo "Error: g++ compiler not found. Please install g++ before running this script."
    exit 1
fi

# Compile the server.cpp
g++ -o server server.cpp

# Check if the compilation was successful
if [ $? -eq 0 ]; then
    echo "Compilation successful. The server executable is created."
else
    echo "Compilation failed."
fi
