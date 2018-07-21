// Define lean-and-mean macro to prevent Windows Sockets 1.x from being loaded
// if and when the <windows.h> header is being included into the source file.
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstdio>

WSADATA gWsaData;

// Initialize the support for Winsocks by initing the use of WS2_32.dll file.
// This function will initialize the WSADATA structure to contain information
// about the Windows Socket implementation. Startup takes a Winsocket version
// as a parameter to request and define the highest supported Winsock version.
void initWSA() {
  auto result = WSAStartup(MAKEWORD(2, 2), &gWsaData);
  switch (result) {
    case 0:
      printf("WSAStartup succeeded.");
      break;
    case WSASYSNOTREADY:
      printf("WSAStartup failed: The network subsystem is not ready for network communication.");
      break;
    case WSAVERNOTSUPPORTED:
      printf("WSAStartup failed: The requested Windows Sockets version is not supported.");
      break;
    case WSAEINPROGRESS:
      printf("WSAStartup failed: A blocking Windows Sockets 1.1 operation is in progress.");
      break;
    case WSAEPROCLIM:
      printf("WSAStartup failed: A task limit of Windows Sockets implementation has been reached.");
      break;
    case WSAEFAULT:
      printf("WSAStartup failed: The provided WSADATA parameter is not a valid pointer.");
      break;
    default:
      printf("WSAStartup failed: An unknown error code %d occured during initialization.", result);
      break;
  }
}

int main() {
  initWSA();
  return 0;
}
