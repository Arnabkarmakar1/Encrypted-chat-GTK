#!/bin/bash

echo "Compiling server..."
gcc server.c -o server -pthread
if [ $? -ne 0 ]; then
  echo "Error compiling server.c"
  exit 1
fi

echo "Compiling client_ui..."
gcc client_ui.c -o client_ui `pkg-config --cflags --libs gtk+-3.0` -pthread
if [ $? -ne 0 ]; then
  echo "Error compiling client_ui.c"
  exit 1
fi

echo "Compiling launcher..."
gcc launcher.c -o launcher `pkg-config --cflags --libs gtk+-3.0`
if [ $? -ne 0 ]; then
  echo "Error compiling launcher.c"
  exit 1
fi

echo "All files compiled successfully!"
