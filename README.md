#Encrypted Multi-Client Chatroom using Socket programming and GTK

A secure, real-time multi-client chatroom with a GTK UI frontend

## Features:

 - XOR encrypted messaging
 - Multiple Client Handling
 - GTK Launcher with Add user and Launch Server Ui
 - Graceful exit without affecting other users

## Technologies used:

 - C, Sockets
 - GTK+ for UI
 - pthreads for concurrency

## Run instructions

```bash
gcc server.c -o server -pthread
gcc client_ui.c -o client_ui `pkg-config --cflags --libs gtk+-3.0` -pthread
gcc client.c -o client `pkg-config --cflags --libs gtk+-3.0`
gcc launcher.c -o launcher `pkg-config --cflags --libs gtk+-3.0`
./launcher

# Author
Arnab Karmakar
