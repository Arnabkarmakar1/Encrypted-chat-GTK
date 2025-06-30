# Encrypted Multi-Client Chatroom using Socket Programming and GTK

This project is a real-time, encrypted multi-client chatroom application built using C. It combines TCP socket programming for backend communication with GTK+ for graphical user interface management. The system is designed to allow multiple users to chat securely and concurrently, with each client having a dedicated GUI window. A launcher GUI handles the startup of the server and the creation of clients.

---

## Features

- Real-time multi-client messaging with individual user sessions.
- XOR-based symmetric encryption for secure communication between clients and server.
- Multi-threaded server architecture using POSIX threads.
- GTK-based GUI launcher that provides options to launch the server and add new users.
- Each client has a dedicated GTK chat window.
- Graceful exit handling: any client can disconnect without affecting others or the server.
- Real-time message display in the server terminal.

---

## Technologies Used

- Programming Language: C
- Networking: TCP socket programming (IPv4)
- GUI Framework: GTK+ 3
- Concurrency: POSIX threads (pthreads)
- Operating System: Linux (Ubuntu tested)

---

## Directory Structure

Project/
│
├── server.c -> Server source code (handles multiple clients)
├── client_ui.c -> Client-side GUI with messaging logic
├── launcher.c -> GTK launcher for server and client management
├── .gitignore -> Excludes compiled binaries from Git
├── README.md -> Project documentation (this file)


---

## How to Build and Run

### 1. Install Dependencies (Ubuntu/Debian)

```bash
sudo apt update
sudo apt install build-essential libgtk-3-dev
```
### 2. Compile the Files

```bash
gcc server.c -o server -pthread
gcc client_ui.c -o client_ui `pkg-config --cflags --libs gtk+-3.0` -pthread
gcc launcher.c -o launcher `pkg-config --cflags --libs gtk+-3.0`
```
### 3. Launch the Program

```bash
./launcher
```

From the launcher:
  Click "Launch Server" to start the server in a new terminal.
  Click "Add User" to spawn a new GTK chat window.
  Each client must enter a username to join the chatroom.

How It Works:
- The launcher GUI initializes the server and client instances.
- The server listens on a fixed port and handles each client in a separate thread.
- Each message is XOR-encrypted before being sent and decrypted after reception.
- Messages are displayed in the GTK chat window of each client and printed on the server terminal.
- Typing /exit in a client window disconnects that client only, without affecting others or the server.

Example Use Case:
- ./launcher opens a GUI window.
- Click "Launch Server": opens the terminal and starts the server.
- Click "Add User": opens a new client chat window.
- Enter a username and begin chatting.
- Add multiple users to test real-time multi-client communication.

Future Improvements:
- Replace XOR encryption with a robust algorithm such as AES or TLS-based secure sockets.
- Implement private messaging support between users.
- Add timestamps, message delivery status, and user notifications.
- Implement server-side chat logs and analytics.
- Provide a cross-platform version using libraries like Qt or FLTK.
- Add administrative features to kick or mute users.

Author:
- Arnab Karmakar
- B.Tech in Information Technology
- Government College of Engineering and Textile Technology, Serampore
- West Bengal, India
- GitHub: https://github.com/Arnabkarmakar1










