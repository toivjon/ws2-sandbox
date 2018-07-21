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
int initWSA() {
  auto result = WSAStartup(MAKEWORD(2, 2), &gWsaData);
  switch (result) {
    case 0:
      printf("WSAStartup succeeded.\n");
      break;
    case WSASYSNOTREADY:
      printf("WSAStartup failed: The network subsystem is not ready for network communication.\n");
      break;
    case WSAVERNOTSUPPORTED:
      printf("WSAStartup failed: The requested Windows Sockets version is not supported.\n");
      break;
    case WSAEINPROGRESS:
      printf("WSAStartup failed: A blocking Windows Sockets 1.1 operation is in progress.\n");
      break;
    case WSAEPROCLIM:
      printf("WSAStartup failed: A task limit of Windows Sockets implementation has been reached.\n");
      break;
    case WSAEFAULT:
      printf("WSAStartup failed: The provided WSADATA parameter is not a valid pointer.\n");
      break;
    default:
      printf("WSAStartup failed: An unknown error code %d occured during initialization.\n", result);
      break;
  }
  return result;
}

// Shutdown and release the handle holding a usage reference to WS2_32.dll file.
// This function will release the OS handle to Windows Sockets so the library is
// aware that it is no longer needed or used by this application.
int cleanupWSA() {
  auto result = WSACleanup();
  if (result == 0) {
    printf("WSACleanup succeeded.\n");
  } else {
    auto errorCode = WSAGetLastError();
    switch (errorCode) {
      case WSANOTINITIALISED:
        printf("WSACleanup failed: A successful WSAStartup call must occur before using this function.\n");
        break;
      case WSAENETDOWN:
        printf("WSACleanup failed: The network subsystem has failed.\n");
        break;
      case WSAEINPROGRESS:
        printf("WSACleanup failed: A blocking callback or Windows Sockets 1.1 call is still in progress.\n");
        break;
      default:
        printf("WSACleanup failed: An unknown error code %d occured during cleanup.\n", errorCode);
        break;
    }
  }
  return result;
}

int main() {
  auto executionStatus = initWSA();
  if (executionStatus == 0) {
    // ...
    executionStatus = cleanupWSA();
  }
  return executionStatus;
}
