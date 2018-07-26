# ws2-sandbox
A sandbox for trying out Windows Sockets 2.

# Compilation
This sandbox is designed to be compiled and executed under a Windows OS.

This simple application only contains a single file, so one may decide to either compile manually or with the provided Makefile.

When compiling manually, remember to link against ws2_32 library.

# Usage
This application can be started in a server or client mode. It is preferred to first start the server before the client as the client will automatically try to connect to the server and exit if it's not available.

Application startup syntax
**test.exe [target-ip]**

An example to start a server
**$ test.exe**

An example to start a client
**$ test.exe 127.0.0.1**
