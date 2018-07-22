// Define lean-and-mean macro to prevent Windows Sockets 1.x from being loaded
// if and when the <windows.h> header is being included into the source file.
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

// This macro fixes the problem where the target macro is not being correctly
// recognized in the <ws2tcpip.h> header as it produces "not declared" error
// at least for the getaddrinfo(...) function.
#define _WIN32_WINNT 0x501

#define PORT "6666"

#include <cstdio>
#include <winsock2.h>
#include <ws2tcpip.h>

WSADATA  gWsaData;

// Initialize the support for Winsocks by initing the use of WS2_32.dll file.
// This function will initialize the WSADATA structure to contain information
// about the Windows Socket implementation. Startup takes a Winsocket version
// as a parameter to request and define the highest supported Winsock version.
//
// @returns 0 on a success and a non-zero on an error.
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
//
// @returns 0 on a success and a non-zero on an error.
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

// Resolve the hostname, IP address and relevant information about the target
// host address. This function uses the getaddrinfo to resolve network target
// information which is needed to create a new socket to start a communication.
//
// @param host Pass the target hostname or NULL to resolve current machine.
// @param hints Arguments provided as hints to resolve the required details.
// @param info A structure to be filled with the resolved network information.
// @returns 0 on a success and a non-zero on an error.
int resolveAddress(const char* host, const addrinfo& hints, addrinfo** info) {
  auto result = getaddrinfo(host, PORT, &hints, &*info);
  switch (result) {
  case 0:
    printf("getaddrinfo succeeded.\n");
    break;
  case WSATRY_AGAIN:
    printf("getaddrinfo failed: A temporary failure in name resolution occured.\n");
    break;
  case WSAEINVAL:
    printf("getaddrinfo failed: An invalid value was provided for the ai_flags member of the hints parameter.\n");
    break;
  case WSANO_RECOVERY:
    printf("getaddrinfo failed: A nonrecoverable failure in name resolution occured.\n");
    break;
  case WSAEAFNOSUPPORT:
    printf("getaddrinfo failed: The ai_family member of the hints parameter is not supported.\n");
    break;
  case WSA_NOT_ENOUGH_MEMORY:
    printf("getaddrinfo failed: A memory allocation failure occured.\n");
    break;
  case WSAHOST_NOT_FOUND:
    printf("getaddrinfo failed: nodename and service parameter were not provided or the name does not resolve.\n");
    break;
  case WSATYPE_NOT_FOUND:
    printf("getaddrinfo failed: The service parameter is not supported for the specified ai_socktype.\n");
    break;
  case WSAESOCKTNOSUPPORT:
    printf("getaddrinfo failed: The ai_socktype member of the hints parameter is not supported.\n");
    break;
  case WSANO_DATA:
    printf("getaddrinfo failed: The requested name is valid, but no data of the requested type was found.\n");
    break;
  case WSANOTINITIALISED:
    printf("getaddrinfo failed: A successful WSAStartup call must occur before using this function.\n");
    break;
  default:
    printf("getaddrinfo failed: An unknown error code %d occured..\n", result);
    break;
  }
  return result;
}

// Create a new Windows socket for the given address information descriptor.
// This function will use the provided address information details to create
// a new socket either for a server or client socket based on the details.
//
// @param addressInfo An information container about the network address.
// @returns A new valid socket or INVALID_SOCKET on an error.
SOCKET createSocket(const addrinfo* addressInfo) {
  auto result = socket(addressInfo->ai_family, addressInfo->ai_socktype, addressInfo->ai_protocol);
  if (result != INVALID_SOCKET) {
    printf("socket succeeded.\n");
  } else {
    auto errorCode = WSAGetLastError();
    switch (errorCode) {
      case WSANOTINITIALISED:
        printf("socket failed: A successful WSAStartup call must occur before using this function.\n");
        break;
      case WSAENETDOWN:
        printf("socket failed: The network subsystem or the associated service provider has failed.\n");
        break;
      case WSAEAFNOSUPPORT:
        printf("socket failed: The specified address family is not supported.\n");
        break;
      case WSAEINPROGRESS:
        printf("socket failed: A blocking Windows Sockets 1.1 call is in progress or a callback is being handled.\n");
        break;
      case WSAEMFILE:
        printf("socket failed: No more socket descriptors are available.\n");
        break;
      case WSAEINVAL:
        printf("socket failed: An invalid argument was supplied.\n");
        break;
      case WSAEINVALIDPROVIDER:
        printf("socket failed: The service provider returned a version other than 2.2.\n");
        break;
      case WSAEINVALIDPROCTABLE:
        printf("socket failed: The service provider returned an invalid or incomplete procedure table to the WSPStartup.\n");
        break;
      case WSAENOBUFS:
        printf("socket failed: No buffer space available. The socket canno be created.\n");
        break;
      case WSAEPROTONOSUPPORT:
        printf("socket failed: The specified protocol is not supported.\n");
        break;
      case WSAEPROTOTYPE:
        printf("socket failed: The specified protocol is the wrong type for this socket.\n");
        break;
      case WSAEPROVIDERFAILEDINIT:
        printf("socket failed: The service provider failed to initializer.\n");
        break;
      case WSAESOCKTNOSUPPORT:
        printf("socket failed: The specified socket type is not supported in this address family.\n");
        break;
      default:
        printf("socket failed: An unknown error code %d occured.\n", errorCode);
        break;
    }
  }
  return result;
}

// Close the given existing socket. This function will first try to close the
// socket by calling the WS2 closing function and if there's an error, it will
// be queried from the Windows Sockets API for further use and information.
//
// @param socket The socket to be closed.
// @returns 0 on a success and SOCKET_ERROR on an error.
int closeSocket(SOCKET socket) {
  auto result = closesocket(socket);
  if (result == 0) {
    printf("closesocket succeeded.\n");
  } else {
    auto errorCode = WSAGetLastError();
    switch (errorCode) {
      case WSANOTINITIALISED:
        printf("closesocket failed: A successful WSAStartup call must occur before using this function.\n");
        break;
      case WSAENETDOWN:
        printf("closesocket failed: The network subsystem has failed.\n");
        break;
      case WSAENOTSOCK:
        printf("closesocket failed: The descriptor is not a socket.\n");
        break;
      case WSAEINPROGRESS:
        printf("closesocket failed: A blocking Windows Sockets call or callback is in progress.\n");
        break;
      case WSAEINTR:
        printf("closesocket failed: The blocking Windows Socket call was cancelled.\n");
        break;
      case WSAEWOULDBLOCK:
        printf("closesocket failed: The socket is marked as nonblocking but there is linger and nonzero timeout.\n");
        break;
      default:
        printf("closesocket failed: Unknown error code %d occured.\n", errorCode);
        break;
    }
  }
  return result;
}

// Bound the target socket to a network address within the system. Bound is required
// to a server socket to start accepting incoming client connections. This function
// should be used with a successfully created socket and queried address information.
//
// @param socket The target socket.
// @param addressInfo A reference to the address information pointer.
// @returns 0 on a success and SOCKET_ERROR on an error.
int bindSocket(SOCKET socket, addrinfo** addressInfo) {
  auto result = bind(socket, (*addressInfo)->ai_addr, (int)(*addressInfo)->ai_addrlen);
  if (result == 0) {
    printf("bind succeeded.\n");
  } else {
    auto errorCode = WSAGetLastError();
    switch (errorCode) {
      case WSANOTINITIALISED:
        printf("bind failed: A successful WSAStartup call must occur before using this function.\n");
        break;
      case WSAENETDOWN:
        printf("bind failed: The network subsystem has failed.\n");
        break;
      case WSAEACCES:
        printf("bind failed: An attempt was made to access a socket in a way forbidden by its access permissions.\n");
        break;
      case WSAEADDRINUSE:
        printf("bind failed: Computer has already bound a socket to the same protocol, network address and port.\n");
        break;
      case WSAEADDRNOTAVAIL:
        printf("bind failed: The requested address is not valid in its context.\n");
        break;
      case WSAEFAULT:
        printf("bind failed: System detected an invalid pointer address to use a pointer argument in a call.\n");
        break;
      case WSAEINPROGRESS:
        printf("bind failed: A blocking socket call or callback handling is in progress.\n");
        break;
      case WSAEINVAL:
        printf("bind failed: An invalid argument was supplied (socket is already bound to an address).\n");
        break;
      case WSAENOBUFS:
        printf("bind failed: There's not enought buffers available or there are too many connections.\n");
        break;
      case WSAENOTSOCK:
        printf("bind failed: An operation was attempted on something that is not a socket.\n");
        break;
      default:
        printf("bind failed: Unknown error code %d occured.\n", errorCode);
        break;
    }
  }
  return result;
}

// Place the given socket in a state in which it is listening for incoming
// connections. This function should be called after the server socket has
// been successfully bound to an network address. The preferred size for the
// backlog is SOMAXCONN, where the service automatically selects a value.
//
// Sidenote: This will trigger a firewall alarm when launched for a first time.
//
// @param socket The target socket.
// @param maxBackLogSize The maximum length of the queue of pending connections.
// @returns 0 on a success and SOCKET_ERROR on an error.
int listenSocket(SOCKET socket, int maxBacklogSize) {
  auto result = listen(socket, maxBacklogSize);
  if (result == 0) {
    printf("listen succeeded.\n");
  } else {
    auto errorCode = WSAGetLastError();
    switch (errorCode) {
      case WSANOTINITIALISED:
        printf("listen failed: A successful WSAStartup call must occur before using this function.\n");
        break;
      case WSAENETDOWN:
        printf("listen failed: The network subsystem has failed.\n");
        break;
      case WSAEADDRINUSE:
        printf("listen failed: The socket's local address is already in use and the socket was not marked to allow address reuse.\n");
        break;
      case WSAEINPROGRESS:
        printf("listen failed: A blocking socket call or callback is in progress.\n");
        break;
      case WSAEINVAL:
        printf("listen failed: The socket has not been bound with bind.\n");
        break;
      case WSAEISCONN:
        printf("listen failed: The socket is already connected.\n");
        break;
      case WSAEMFILE:
        printf("listen failed: No more socket descriptors are available.\n");
        break;
      case WSAENOBUFS:
        printf("listen failed: No buffer space is available.\n");
        break;
      case WSAENOTSOCK:
        printf("listen failed: The descriptor is not a socket.\n");
        break;
      case WSAEOPNOTSUPP:
        printf("listen failed: The referenced socket is not of a type that supports the listen operation.\n");
        break;
      default:
        printf("listen failed: An unknown error code %d occured.\n", errorCode);
        break;
    }
  }
  return result;
}

// Accept a pending client connection. Note that this function would be normally
// used by allowing connections from multiple clients. For real high-performance
// servers, multiple threads should be used to handle multiple client connections.
//
// One example from the MS documentation for handling multiple clients:
// Create a loop that checks for connection requests using the listen function.
// If a connection request occurs, the application calls accept and passes it to
// a another thread to handle the actual request.
//
// @param socket The server socket used to accept the client.
// @returns A descriptor for the new socket.
SOCKET acceptClient(SOCKET socket) {
  SOCKET clientSocket = accept(socket, NULL, NULL);
  if (clientSocket != INVALID_SOCKET) {
    printf("accept succeeded.\n");
  } else {
    auto errorCode = WSAGetLastError();
    switch (errorCode) {
      case WSANOTINITIALISED:
        printf("accept failed: A successful WSAStartup call must occur before using this function.\n");
        break;
      case WSAECONNRESET:
        printf("accept failed: An incoming connection was indicated but suddenhly terminated by the remote peer.\n");
        break;
      case WSAEFAULT:
        printf("accept failed: The addrlen parameter is too small or addr is not a valid part of the user address space.\n");
        break;
      case WSAEINTR:
        printf("accept failed: A blocking socket call was cancelled.\n");
        break;
      case WSAEINVAL:
        printf("accept failed: The listen function was not invoked prior to accept.\n");
        break;
      case WSAEINPROGRESS:
        printf("accept failed: A blocking socket call or callbackk is in progress.\n");
        break;
      case WSAEMFILE:
        printf("accept failed: The queue is nonempty upon entry to accept and there are not descriptors available.\n");
        break;
      case WSAENETDOWN:
        printf("accept failed: The network subsystem has failed.\n");
        break;
      case WSAENOBUFS:
        printf("accept failed: No buffer space is available.\n");
        break;
      case WSAENOTSOCK:
        printf("accept failed: The descriptor is not a socket.\n");
        break;
      case WSAEOPNOTSUPP:
        printf("accept failed: The referenced socket is not a type that supports connection-oriented service.\n");
        break;
      case WSAEWOULDBLOCK:
        printf("accept failed: The socket is marked as nonblocking and no connections are present to be accepted.\n");
        break;
      default:
        printf("accept failed: An unknown error code %d occured.\n", errorCode);
        break;
    }
  }
  return clientSocket;
}

void startTcpServer() {
  // create an address descriptor for a TCP server socket.
  addrinfo hints;
  ZeroMemory(&hints, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;
  hints.ai_flags = AI_PASSIVE;

  // resolve address details and open a new socket.
  addrinfo* information = NULL;
  if (resolveAddress(NULL, hints, &information) == 0) {
    auto socket = createSocket(information);
    if (socket != INVALID_SOCKET) {
      if (bindSocket(socket, &information) == 0) {
        if (listen(socket, SOMAXCONN) == 0) {
          printf("Waiting for client to connect...\n");
          auto clientSocket = acceptClient(socket);
          if (clientSocket != INVALID_SOCKET) {
            // ...
          }
        }
      }
      closeSocket(socket);
    }
  }
  freeaddrinfo(information);
}

void startTcpClient(const char* host) {
  // create an address descriptor for a TCP client socket.
  addrinfo hints;
  ZeroMemory(&hints, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;

  // resolve address details and open a new socket.
  addrinfo* information = NULL;
  if (resolveAddress(host, hints, &information) == 0) {
    auto socket = createSocket(information);
    if (socket != INVALID_SOCKET) {
      // ...
      closeSocket(socket);
    }
  }
  freeaddrinfo(information);
}

int main() {
  auto executionStatus = initWSA();
  if (executionStatus == 0) {
    // TODO switch between server and client.
    startTcpServer();

    executionStatus = cleanupWSA();
  }
  return executionStatus;
}
