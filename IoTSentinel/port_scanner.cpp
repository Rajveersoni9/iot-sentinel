#include "port_scanner.h"
#include "settings_manager.h"
#include <fcntl.h>

std::vector<int> PortScanner::scanDevicePorts(IPAddress ip, const std::vector<int>& portsToScan) {
  std::vector<int> openPorts;
  std::vector<SocketScanState> sockets;
  
  // Convert IPAddress to String for POSIX socket usage
  String ipStr = ip.toString();
  
  // 1. Initialize all socket connections in non-blocking mode
  for (int port : portsToScan) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
      continue;
    }
    
    // Set socket to non-blocking
    int flags = fcntl(sockfd, F_GETFL, 0);
    fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
    
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    inet_pton(AF_INET, ipStr.c_str(), &serv_addr.sin_addr);
    
    // Trigger non-blocking connection
    connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    
    SocketScanState state;
    state.fd = sockfd;
    state.port = port;
    state.isChecking = true;
    state.startTime = millis();
    sockets.push_back(state);
  }
  
  // 2. Select multiplexing / polling loop
  // Wait up to configured timeout for connections to resolve
  unsigned long timeout = settingsManager.scanTimeout; 
  unsigned long startSelect = millis();

  
  while (millis() - startSelect < timeout) {
    bool allResolved = true;
    fd_set writeSet;
    FD_ZERO(&writeSet);
    int maxFd = -1;
    
    for (const auto& s : sockets) {
      if (s.isChecking) {
        FD_SET(s.fd, &writeSet);
        if (s.fd > maxFd) {
          maxFd = s.fd;
        }
        allResolved = false;
      }
    }
    
    if (allResolved) {
      break;
    }
    
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 15000; // 15ms resolution check
    
    int selectResult = select(maxFd + 1, NULL, &writeSet, NULL, &tv);
    
    if (selectResult > 0) {
      for (auto& s : sockets) {
        if (s.isChecking && FD_ISSET(s.fd, &writeSet)) {
          int error = 0;
          socklen_t len = sizeof(error);
          
          // Verify if socket connection completed successfully
          if (getsockopt(s.fd, SOL_SOCKET, SO_ERROR, &error, &len) == 0) {
            if (error == 0) {
              openPorts.push_back(s.port);
            }
          }
          
          close(s.fd);
          s.isChecking = false;
        }
      }
    }
    
    delay(5);
  }
  
  // 3. Clean up remaining active sockets (timed out connections)
  for (auto& s : sockets) {
    if (s.isChecking) {
      close(s.fd);
    }
  }
  
  return openPorts;
}
